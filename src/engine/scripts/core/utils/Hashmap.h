/*
 * MIT License
 *
 * Copyright (c) 2024 Louis Eriksson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LOUIERIKSSON_HASHMAP_H
#define LOUIERIKSSON_HASHMAP_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace LouiEriksson::Engine {
	
	/**
	 * @mainpage Version 1.0.5
	 * @details Custom Hashmap implementation accepting a customisable key and value type. Created using a combination of prior knowledge and brief online tutorial,
	 *          This implementation requires that your "key" type is compatible with std::hash and that the stored data types are copyable.
	 * @see Wang, Q. (Harry) (2020). Implementing Your Own HashMap (Explanation + Code). YouTube.
	 *      Available at: https://www.youtube.com/watch?v=_Q-eNqTOxlE [Accessed 2021].
	 * @tparam Tk Key type of the Hashmap.
	 * @tparam Tv Value type of the Hashmap.
	 */
	template<typename Tk, typename Tv>
	class Hashmap {
	
	public:
		
		/**
		 * @brief Represents a key-value pair.
		 *
		 * This struct is used to store a key-value pair, where 'Tk' represents the type of the key and 'Tv' represents the type of the value.
		 */
		struct KeyValuePair final {
			
			Tk first;
			Tv second;
			
			KeyValuePair(const Tk& _key, const Tv& _value) noexcept  :
				 first(_key),
				second(_value) {}
			
			KeyValuePair(const KeyValuePair& _other) noexcept  :
				 first(_other.first),
				second(_other.second) {}
				
			KeyValuePair(KeyValuePair&& _rhs) noexcept :
					 first(std::move(_rhs.first)),
					second(std::move(_rhs.second)) {}

			KeyValuePair& operator =(const KeyValuePair& _other) noexcept {
				if (this != &_other) {
					first = _other.first;
					second = _other.second;
				}
				return *this;
			}
			
			KeyValuePair& operator =(KeyValuePair&& _rhs) noexcept {
				if (this != &_rhs) {
					 first = std::move(_rhs.first);
					second = std::move(_rhs.second);
				}
				return *this;
			}
		};
	
	private:
		
		/** @brief Buckets of the Hashmap. */
		std::vector<std::vector<KeyValuePair>> m_Buckets;
		
		/** @brief Current number of elements within the Hashmap. */
		size_t m_Size;
		
		/**
		 * @brief Calculate the hashcode of a given object using std::hash.
		 * @param[in] _item Item to calculate hash of.
		 * @return Hashcode of _item.
		 * @throw std::exception If the type of _item is not supported by std::hash.
		 */
		static size_t GetHashcode(const Tk& _item) noexcept {
			return std::hash<Tk>()(_item);
		}
		
		/**
		 * @brief Reinitialise the Hashmap. An expensive operation that increases the Hashmap's capacity.
		 */
		void Resize() {
			
			const size_t resize_amount = size();
			
			std::vector<std::vector<KeyValuePair>> shallowCopy(m_Buckets);
			
			m_Size = 0;
			m_Buckets.clear();
			m_Buckets.resize(size() + resize_amount);
			
			for (auto& bucket : shallowCopy) {
				for (auto& kvp : bucket) {
					Add(kvp.first, kvp.second);
				}
			}
		}
		
		/**
		 * @brief Retrieves a reference to the entry within the Hashmap with the given key, if one exists.
		 * This method will throw an exception if no entry is found. Consider using Get() for safe access instead.
		 *
		 * @param[in] _key Key of the entry to retrieve.
		 * @return Out value result.
		 * @throw std::runtime_error If no entry is found.
		 * @see Get(const Tk& _key, Tv& _out)
		 */
		Tv& Return(const Tk& _key) {
			
			Tv* result = nullptr;
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			for (auto& kvp : bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					result = &kvp.second;
					
					break;
				}
			}
			
			if (result == nullptr) {
				throw std::runtime_error("Attempted to access a nonexistent entry from the Hashmap.");
			}
			
			return *result;
		}
		
	public:
		
		/**
		 * @brief Initialise Hashmap.
		 * @param[in] _capacity Initial capacity of the Hashmap. Must be larger than 0.
		 */
		Hashmap(const size_t& _capacity = 1) : m_Size(0) {
			m_Buckets.resize(_capacity);
		}
		
		/**
		 * @brief Initialise Hashmap using a collection of key-value pairs.
		 * @details Please note: The provided collection should be distinct. Otherwise, some data loss may occur as duplicate entries will be ignored.
		 *
		 * @param[in] _items A collection of key-value pairs.
		 * @param[in] _capacity Initial capacity of the Hashmap. If a value less than 1 is assigned, it will use the size of the provided collection.
		 */
		Hashmap(const std::initializer_list<KeyValuePair>& _items, const size_t& _capacity = 0) : m_Size(0) {
			
			size_t auto_capacity = _capacity;
			
			if (auto_capacity < 1) {
				auto_capacity = std::max<size_t>(_items.size(), 1);
			}
			
			m_Buckets.resize(auto_capacity);
			
			for (const auto& item : _items) {
				Assign(std::move(item.first), std::move(item.second));
			}
		}
		
		/**
		 * @brief Returns the number of items stored within the Hashmap.
		 * @return The number of items stored within the Hashmap.
		 */
		[[nodiscard]] size_t size() const noexcept  {
			return m_Size;
		}
		
		/**
		 * @brief Is the Hashmap empty?
		 * @return Returns true if the Hashmap contains no entries.
		 */
		[[nodiscard]] bool empty() const noexcept  {
			return m_Size == 0;
		}
	
		/**
		 * @brief Queries for the existence of an item in the Hashmap.
		 *
		 * @param[in] _key Key of the entry.
		 * @return True if successful, false otherwise.
		 */
		bool ContainsKey(const Tk& _key) const noexcept {
			
			auto result = false;
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			for (auto& kvp : bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					result = true;
					
					break;
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Inserts a new entry into the Hashmap with given key and value, if one does not already exist.
		 * If you are trying to modify an existing key, see Hashmap::Assign.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[out] _value Value of the entry.
		 * @return True if successful, false otherwise.
		 */
		bool Add(const Tk& _key, const Tv& _value) {
			
			auto result = true;
			
			if (size() >= m_Buckets.size()) {
				Resize();
			}
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			// In the case of a hash collision, determine if the key is unique.
			// We will treat duplicate insertions as a mistake on the developer's part and return failure.
			for (auto& kvp : bucket) {
				if (GetHashcode(kvp.first) == hash) {
					result = false;
					
					break;
				}
			}
			
			// Insert the item into the bucket.
			if (result) {
				m_Size++;
				
				bucket.emplace_back(_key, _value);
			}
			
			return result;
		}
		
		/**
		 * @brief Inserts or replaces an entry within the Hashmap with the given key.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[out] _value Value of the entry.
		 */
		void Assign(const Tk& _key, const Tv& _value) {
			
			if (size() >= m_Buckets.size()) {
				Resize();
			}
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			auto exists = false;
			for (auto& kvp : bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					exists = true;
					
					kvp.second = _value;
					
					break;
				}
			}
			
			if (!exists) {
				bucket.emplace_back(_key, _value);
			}
			
			m_Size++;
		}
		
		/**
		 * @brief Inserts or replaces an entry within the Hashmap with the given key using move semantics.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[out] _value Value of the entry.
		 *
		 * @see Assign(const Tk& _key, const Tv& _value)
		 */
		void Emplace(const Tk&& _key, const Tv&& _value) {
			Assign(std::move(_key), std::move(_value));
		}
		
		/**
		 * @brief Removes entry with given key from the Hashmap.
		 *
		 * @param[in] _key Key of the entry to be removed.
		 * @return True if successful, false otherwise.
		 */
		bool Remove(const Tk& _key) noexcept {
			
			bool result = false;
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			// In the case of accessing a "collided" hash, find the value in the bucket using equality checks.
			for (auto itr = bucket.begin(); itr < bucket.end(); itr++) {
				
				if (GetHashcode(itr->first) == hash) {
					result = true;
					
					bucket.erase(itr);
					
					break;
				}
			}
			
			m_Size -= static_cast<size_t>(result);
			
			return result;
		}
		
		/**
		 * @brief Retrieves a reference to the entry within the Hashmap with the given key, if one exists.
		 *
		 * @param[in] _key Key of the entry to retrieve.
		 * @param[out] _out Out value result.
		 * @return True if successful, false otherwise.
		 */
		bool Get(const Tk& _key, Tv& _out) const noexcept {
			
			auto result = false;
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			for (auto& kvp : bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					result = true;
					
					_out = kvp.second;
					
					break;
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Trims unused entries from the end of the Hashmap.
		 */
		void Trim() {
			
			size_t trimStart = 1;
			
			for (size_t i = trimStart; i < m_Buckets.size(); ++i) {
				if (m_Buckets[i].size() != 0) {
					trimStart = i + 1;
				}
			}
			
			if (trimStart < m_Buckets.size()) {
				m_Buckets.erase(m_Buckets.begin() + trimStart);
			}
		}
		
		/**
		 * @brief Returns a shallow copy of all entries stored within the Hashmap.
		 * @return A shallow copy of all entries stored within the Hashmap.
		 */
		[[nodiscard]] std::vector<Tk> Keys() const {
			
			std::vector<Tk> result;
			
			for (const auto& bucket : m_Buckets) {
				for (const auto& kvp : bucket) {
					result.emplace_back(kvp.first);
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Returns a shallow copy of all entries stored within the Hashmap.
		 * @return A shallow copy of all entries stored within the Hashmap.
		 */
		[[nodiscard]] std::vector<Tv> Values() const {
			
			std::vector<Tv> result;
			
			for (const auto& bucket : m_Buckets) {
				for (const auto& kvp : bucket) {
					result.emplace_back(kvp.second);
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Returns a shallow copy of all entries stored within the Hashmap.
		 * @return A shallow copy of all entries stored within the Hashmap.
		 */
		[[nodiscard]] std::vector<KeyValuePair> GetAll() const {
			
			std::vector<KeyValuePair> result;
			
			for (const auto& bucket : m_Buckets) {
				for (const auto& kvp : bucket) {
					result.emplace_back(kvp);
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Clears all entries from the Hashmap.
		 *
		 * @remarks This function is not responsible for memory management of items contained within the Hashmap.
		 */
		void Clear() noexcept  {
			
			m_Buckets.clear();
			m_Buckets.resize(1);
		}
		
		/**
		 * @brief Retrieves a reference to the entry within the Hashmap with the given key, if one exists.
		 * This method will throw an exception if no entry is found. Consider using Get() for safe access instead.
		 *
		 * @param[in] _key Key of the entry to retrieve.
		 * @return Out value result.
		 * @throw std::runtime_error If no entry is found.
		 *
		 * @see Get(const Tk& _key, Tv& _out)
		 */
#ifndef HASHMAP_SUPPRESS_EXCEPTION_WARNING
		[[deprecated("This function throws if no entry exists. Consider using Get() if exception-safe access is required. Supress this warning by defining \"HASHMAP_SUPPRESS_UNSAFE_WARNING\".")]]
#endif
		Tv& operator[](const Tk& _key) {
		    return Return(_key);
		}
		
		/* ITERATORS */
		
		/**
		 * @class const_iterator
		 * @brief Represents an iterator to traverse through the elements in a Hashmap.
		 */
		class const_iterator {
		
			friend Hashmap;
			
			typedef typename std::vector<std::vector<KeyValuePair>>::const_iterator outer_itr;
			typedef typename             std::vector<KeyValuePair> ::const_iterator inner_itr;
			
		private:
			
			outer_itr m_Outer;
			outer_itr m_Outer_End;
			inner_itr m_Inner;
			
			const_iterator(outer_itr _outer,
			               outer_itr _outer_end,
			               inner_itr _inner) :
				    m_Outer(_outer),
				m_Outer_End(_outer_end),
				    m_Inner(_inner)
			{
				if (m_Outer != m_Outer_End && m_Inner == m_Outer->end()) {
					++(*this);
				}
			}
			
		public:
			
			const const_iterator& operator ++() {
				
				if (++m_Inner == m_Outer->end()) {
					
					while (++m_Outer != m_Outer_End) {
						
						if (!m_Outer->empty()) {
							m_Inner = m_Outer->begin();
							break;
						}
					}
					if (m_Outer == m_Outer_End) {
						m_Inner = inner_itr();
					}
				}
				return *this;
			}
			
			const KeyValuePair& operator *() const { return *m_Inner; }
			
			bool operator ==(const const_iterator& other) const { return ((m_Outer == other.m_Outer) && (m_Outer == m_Outer_End || m_Inner == other.m_Inner)); }
			bool operator !=(const const_iterator& other) const { return !operator ==(other); }
		};
		
		/**
		 * @class iterator
		 * @brief Represents an iterator to traverse through the elements in a Hashmap.
		 */
		class iterator {
		
			friend Hashmap;
			
			typedef typename std::vector<std::vector<KeyValuePair>>::iterator outer_itr;
			typedef typename             std::vector<KeyValuePair> ::iterator inner_itr;
			
		private:
			
			outer_itr m_Outer;
			outer_itr m_Outer_End;
			inner_itr m_Inner;
			
			iterator(outer_itr _outer,
			         outer_itr _outer_end,
			         inner_itr _inner) :
			    m_Outer(_outer),
			m_Outer_End(_outer_end),
			    m_Inner(_inner)
			{
				if (m_Outer != m_Outer_End && m_Inner == m_Outer->end()) {
					++(*this);
				}
			}
			
		public:
			
			iterator& operator ++() {
				
				if (++m_Inner == m_Outer->end()) {
					
					while (++m_Outer != m_Outer_End) {
						
						if (!m_Outer->empty()) {
							m_Inner = m_Outer->begin();
							break;
						}
					}
					if (m_Outer == m_Outer_End) {
						m_Inner = inner_itr();
					}
				}
				return *this;
			}
			
			KeyValuePair& operator *() { return *m_Inner; }
			
			bool operator ==(const iterator& other) const { return ((m_Outer == other.m_Outer) && (m_Outer == m_Outer_End || m_Inner == other.m_Inner)); }
			bool operator !=(const iterator& other) const { return !operator ==(other); }
			
			operator const_iterator() const { return const_iterator(m_Outer, m_Outer_End, m_Inner); }
		};
		
		iterator begin() { return iterator(m_Buckets.begin(), m_Buckets.end(), m_Buckets.empty() ? typename std::vector<KeyValuePair>::iterator() : m_Buckets.begin()->begin()); }
		iterator   end() { return iterator(m_Buckets.end(),   m_Buckets.end(), typename std::vector<KeyValuePair>::iterator()); }
		
		const_iterator begin() const { return const_iterator(m_Buckets.begin(), m_Buckets.end(), m_Buckets.empty() ? typename std::vector<KeyValuePair>::const_iterator() : m_Buckets.begin()->begin()); }
		const_iterator   end() const { return const_iterator(m_Buckets.end(),   m_Buckets.end(), typename std::vector<KeyValuePair>::const_iterator()); }
	};
	
} // LouiEriksson::Engine

#endif //LOUIERIKSSON_HASHMAP_H