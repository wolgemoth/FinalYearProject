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
#include <optional>
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
			
			KeyValuePair(const Tk& _key, const Tv& _value) :
				 first(_key),
				second(_value) {}
			
			inline constexpr KeyValuePair(const KeyValuePair& _other) :
				 first(_other.first),
				second(_other.second) {}
				
			inline KeyValuePair(KeyValuePair&& _rhs) :
					 first(std::move(_rhs.first)),
					second(std::move(_rhs.second)) {}

			inline KeyValuePair& operator = (const KeyValuePair& _other) {
				if (this != &_other) {
					 first = _other.first;
					second = _other.second;
				}
				return *this;
			}
			
			inline KeyValuePair& operator = (KeyValuePair&& _other) {
				if (this != &_other) {
					 first = std::move(_other.first);
					second = std::move(_other.second);
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
		inline static constexpr size_t GetHashcode(const Tk& _item) {
			return std::hash<Tk>()(_item);
		}
		
		/**
		 * @brief Reinitialise the Hashmap. An expensive operation that increases the Hashmap's capacity.
		 */
		inline constexpr void Resize(const size_t& _newSize) {
			
			std::vector<std::vector<KeyValuePair>> shallow_cpy(m_Buckets);
			
			m_Size = 0;
			m_Buckets.clear();
			m_Buckets.resize(_newSize);
			
			for (auto& bucket : shallow_cpy) {
				for (auto& kvp : bucket) {
					Assign(std::move(kvp.first), std::move(kvp.second));
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
		constexpr const Tv& Return(const Tk& _key) const {
			
			const Tv* result = nullptr;
			
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
		constexpr Hashmap(const size_t& _capacity = 1) : m_Size(0) {
			m_Buckets.resize(_capacity);
		}
		
		/**
		 * @brief Initialise Hashmap using a collection of key-value pairs.
		 * @details Please note: The provided collection should be distinct. Otherwise, some data loss may occur as duplicate entries will be ignored.
		 *
		 * @param[in] _items A collection of key-value pairs.
		 * @param[in] _capacity Initial capacity of the Hashmap. If a value less than 1 is assigned, it will use the size of the provided collection.
		 */
		constexpr Hashmap(const std::initializer_list<KeyValuePair>& _items, const size_t& _capacity = 0) : m_Size(0) {
			
			size_t auto_capacity = _capacity;
			
			if (auto_capacity < 1) {
				auto_capacity = std::max<size_t>(_items.size(), 1);
			}
			
			m_Buckets.resize(auto_capacity);
			
			for (const auto& item : _items) {
				Assign(item.first, item.second);
			}
		}
		
		struct optional_ref final {
		
			friend Hashmap;
			
		private:
			
			using optional_t = std::optional<std::reference_wrapper<const Tv>>;
		
			const optional_t m_Optional;
			
			explicit optional_ref(optional_t&& _optional) : m_Optional(_optional) {};
			
		public:
			
			[[nodiscard]] inline const Tv& value()                 const { return m_Optional.value(); }
			[[nodiscard]] inline const Tv& value_or(const Tv&& _t) const { return m_Optional.value_or(_t); }
			
			[[nodiscard]] inline bool has_value() const { return m_Optional.has_value(); }
			
			[[nodiscard]] inline const Tv& operator  *() const { return  value(); }
			[[nodiscard]] inline const Tv* operator ->() const { return &value(); }
			
			[[nodiscard]] inline operator bool() const { return has_value(); }
		};
		
		/**
		 * @brief Returns the number of items stored within the Hashmap.
		 * @return The number of items stored within the Hashmap.
		 */
		[[nodiscard]] constexpr const size_t& size() const noexcept { return m_Size; }
		
		/**
		 * @brief Is the Hashmap empty?
		 * @return Returns true if the Hashmap contains no entries.
		 */
		[[nodiscard]] constexpr bool empty() const noexcept { return m_Size == 0; }
	
		/**
		 * @brief Queries for the existence of an item in the Hashmap.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 * @return True if successful, false otherwise.
		 */
		bool ContainsKey(const Tk& _key, std::exception_ptr _exception = nullptr) const noexcept {
			
			auto result = false;
			
			try {
				
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
			}
			catch (...) {
				_exception = std::current_exception();
			}
			
			return result;
		}
		
		/**
		 * @brief Inserts a new entry into the Hashmap with given key and value, if one does not already exist.
		 * If you are trying to modify an existing key, see Hashmap::Assign.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[in] _value Value of the entry.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 * @return True if successful, false otherwise.
		 */
		bool Add(const Tk& _key, const Tv& _value, std::exception_ptr _exception = nullptr) noexcept {
			
			auto result = true;
			
			try {
				
				if (size() >= m_Buckets.size()) {
					Resize(m_Buckets.size() * 2);
				}
				
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				const auto hash = GetHashcode(_key);
				const auto i = hash % m_Buckets.size();
				
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
			}
			catch (...) {
				_exception = std::current_exception();
			}
			
			return result;
		}
		
		/**
		 * @brief Inserts a new entry into the Hashmap with given key and value using move semantics, if one does not already exist.
		 * If you are trying to modify an existing key, see Hashmap::Assign.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[in] _value Value of the entry.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 * @return True if successful, false otherwise.
		 */
		bool Add(const Tk&& _key, const Tv&& _value, std::exception_ptr _exception = nullptr) noexcept {
			
			auto result = true;
			
			try {
				
				if (size() >= m_Buckets.size()) {
					Resize(m_Buckets.size() * 2);
				}
				
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				const auto hash = GetHashcode(_key);
				const auto i = hash % m_Buckets.size();
				
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
			}
			catch (...) {
				_exception = std::current_exception();
			}
			
			return result;
		}
		
		/**
		 * @brief Inserts or replaces an entry within the Hashmap with the given key.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[in] _value Value of the entry.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 */
		void Assign(const Tk& _key, const Tv& _value, std::exception_ptr _exception = nullptr) noexcept {
			
			try {
				
				if (size() >= m_Buckets.size()) {
					Resize(m_Buckets.size() * 2);
				}
				
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				const auto hash = GetHashcode(_key);
				const auto i = hash % m_Buckets.size();
				
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
					m_Size++;
					
					bucket.emplace_back(_key, _value);
				}
			}
			catch (...) {
				_exception = std::current_exception();
			}
		}
		
		/**
		 * @brief Inserts or replaces an entry within the Hashmap with the given key using move semantics.
		 *
		 * @param[in] _key Key of the entry.
		 * @param[in] _value Value of the entry.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 */
		void Assign(Tk&& _key, Tv&& _value, std::exception_ptr _exception = nullptr) noexcept {
			
			try {
				
				if (size() >= m_Buckets.size()) {
					Resize(m_Buckets.size() * 2);
				}
				
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				const auto hash = GetHashcode(_key);
				const auto i = hash % m_Buckets.size();
				
				auto& bucket = m_Buckets[i];
				
				auto exists = false;
				for (auto& kvp : bucket) {
					
					if (GetHashcode(kvp.first) == hash) {
						exists = true;
						
						kvp.second = std::move(_value);
						
						break;
					}
				}
				
				if (!exists) {
					m_Size++;
					
					bucket.emplace_back(std::move(_key), std::move(_value));
				}
			}
			catch (...) {
				_exception = std::current_exception();
			}
		}
		
		/**
		 * @brief Removes entry with given key from the Hashmap.
		 *
		 * @param[in] _key Key of the entry to be removed.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 * @return True if successful, false otherwise.
		 */
		bool Remove(const Tk& _key, std::exception_ptr _exception = nullptr) noexcept {
			
			bool result = false;
			
			try {
				
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				const size_t hash = GetHashcode(_key);
				const size_t i = hash % m_Buckets.size();
				
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
				
			}
			catch (...) {
				_exception = std::current_exception();
			}
			
			return result;
		}
		
		/**
		 * @brief Retrieves the value associated with the given key from the hash table.
		 *
		 * @tparam Tk The type of the key.
		 * @param[in] _key The key to retrieve the value for.
		 * @param[out] _exception A pointer to any exception caught during the operation.
		 * @return An optional reference to the value associated with the key, or std::nullopt if the key is not present.
		 * @note This function is noexcept.
		 */
		optional_ref Get(const Tk& _key, std::exception_ptr _exception = nullptr) const noexcept {
			
			typename optional_ref::optional_t result = std::nullopt;
			
			try {
			
				// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
				size_t hash = GetHashcode(_key);
				size_t i = hash % m_Buckets.size();
				
				auto& bucket = m_Buckets[i];
				
				for (auto& kvp : bucket) {
					
					if (GetHashcode(kvp.first) == hash) {
						result = std::cref(kvp.second);
						break;
					}
				}
			}
			catch (...) {
				_exception = std::current_exception();
			}
			
			return optional_ref(std::move(result));
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
		[[nodiscard]] inline std::vector<Tk> Keys() const {
			
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
		[[nodiscard]] inline std::vector<Tv> Values() const {
			
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
		[[nodiscard]] inline std::vector<KeyValuePair> GetAll() const {
			
			std::vector<KeyValuePair> result;
			
			for (const auto& bucket : m_Buckets) {
				for (const auto& kvp : bucket) {
					result.emplace_back(kvp);
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Reserves memory for the container to have a minimum capacity of _newSize elements.
		 *
		 * @param[in] _newSize The minimum capacity to reserve for the container.
		 */
		constexpr void Reserve(const std::size_t& _newSize) {
			
			if (m_Size < _newSize) {
				Resize(_newSize);
			}
		}
		
		/**
		 * @brief Clears all entries from the Hashmap.
		 */
		constexpr void Clear() {
			
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
		[[deprecated("This function does not guarantee exception-safety and will explicitly throws if no entry exists. Consider using Get() if exception-safe access is required.\nSuppress this warning by defining \"HASHMAP_SUPPRESS_UNSAFE_WARNING\".")]]
#endif
		inline constexpr const Tv& operator[](const Tk& _key) const {
		    return Return(_key);
		}
		
		/* ITERATORS */
		
		/**
		 * @class const_iterator
		 * @brief Represents an iterator to traverse through the elements in a Hashmap.
		 */
		class const_iterator final {
		
			friend Hashmap;
			
			using outer_itr = typename std::vector<std::vector<KeyValuePair>>::const_iterator;
			using inner_itr = typename std::vector<KeyValuePair>::const_iterator;
			
		private:
			
			outer_itr m_Outer;
			outer_itr m_Outer_End;
			inner_itr m_Inner;
			
			constexpr const_iterator(const outer_itr& _outer,
			                         const outer_itr& _outer_end,
			                         const inner_itr& _inner) :
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
		
		constexpr const_iterator begin() const { return const_iterator(m_Buckets.begin(), m_Buckets.end(), m_Buckets.empty() ? typename std::vector<KeyValuePair>::const_iterator() : m_Buckets.begin()->begin()); }
		constexpr const_iterator   end() const { return const_iterator(m_Buckets.end(),   m_Buckets.end(), typename std::vector<KeyValuePair>::const_iterator()); }
	};
	
} // LouiEriksson::Engine

#endif //LOUIERIKSSON_HASHMAP_H