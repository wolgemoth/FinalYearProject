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

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>

namespace LouiEriksson {
	
	/// <summary>
	/// <para>
	/// Version 1.0.1
	/// </para>
	/// Custom Hashmap implementation accepting a customisable key and value type. Created using a combination of prior knowledge and brief online tutorial.
	/// <para><remarks>This implementation requires that your "key" type is compatible with std::hash and that the stored data types are copyable.</remarks></para>
	/// <para>Reference: Wang, Q. (Harry) (2020). Implementing Your Own HashMap (Explanation + Code). YouTube. Available at: https://www.youtube.com/watch?v=_Q-eNqTOxlE [Accessed 2021].</para>
	/// </summary>
	/// <typeparam name="Tk">Key type of the Hashmap.</typeparam>
	/// <typeparam name="Tv">Value type of the Hashmap.</typeparam>
	template<typename Tk, typename Tv>
	class Hashmap {
	
	public:
		
		struct KeyValuePair {
			
			Tk first;
			Tv second;
			
			KeyValuePair(Tk _key, Tv _value) noexcept  :
				 first(_key),
				second(_value) {}
			
            KeyValuePair(const KeyValuePair& other) noexcept  :
				 first(other.first),
				second(other.second) {}
		};
		
	private:
		
		/// <summary>
		/// Buckets of the Hashmap.
		/// </summary>
		std::vector<std::vector<KeyValuePair>> m_Buckets;
		
		/// <summary>
		/// Current number of elements within the Hashmap.
		/// </summary>
		size_t m_Size;
		
		/// <summary>
		/// Calculate the hashcode of a given object using std::hash.
		/// <remarks>
		/// This function will throw if the type given is not supported by std::hash.
		/// </remarks>
		/// <param name="_item">Item to calculate hash of.</param>
		/// </summary>
		static size_t GetHashcode(const Tk& _item) noexcept {
			return std::hash<Tk>()(_item);
		}
		
		/// <summary>
		/// Reinitialise the Hashmap. An expensive operation that increases the Hashmap's capacity.
		/// </summary>
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
	
	public:
		
		/// <summary>
		/// Initialise Hashmap.
		/// </summary>
		/// <param name="_capacity">Initial capacity of the Hashmap. Must be larger than 0.</param>
		Hashmap(const size_t& _capacity = 1) : m_Size(0) {
			m_Buckets.resize(_capacity);
		}
		
		/// <summary>
		/// Returns the number of items stored within the Hashmap.
		/// </summary>
		[[nodiscard]] size_t size() const noexcept  {
			return m_Size;
		}
		
		/// <summary>
		/// Returns true if the Hashmap contains no entries.
		/// </summary>
		[[nodiscard]] bool empty() const noexcept  {
			return m_Size == 0;
		}
		
		/// <summary>
		/// Queries for the existence of an item in the Hashmap.
		/// </summary>
		/// <param name="_key">Key of the entry.</param>
		/// <returns>True if successful, false otherwise.</returns>
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
		
		/// <summary>
		/// Inserts a new entry into the Hashmap with given key and value, if one does not already exist.
		/// <para>
		/// <remarks>
		/// If you are trying to modify an existing key, see Hashmap::Assign.
		/// </remarks>
		/// </para>
		/// </summary>
		/// <param name="_key">Key of the entry.</param>
		/// <param name="_value">Value of the entry.</param>
		/// <returns>True if successful, false otherwise.</returns>
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
				
				bucket.push_back({ _key, _value });
			}
			
			return result;
		}
		
		/// <summary>
		/// Inserts or replaces an entry within the Hashmap with the given key.
		/// </summary>
		/// <param name="_key">Key of the entry.</param>
		/// <param name="_value">Value of the entry.</param>
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
				bucket.push_back({ _key, _value });
			}
			
			m_Size++;
		}
		
		/// <summary>
		/// Removes entry with given key from the Hashmap.
		/// </summary>
		/// <param name="_key">Key of the entry to be removed.</param>
		/// <returns>True if successful, false otherwise.</returns>
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
		
		/// <summary>
		/// Retrieves a reference to the entry within the Hashmap with the given key, if one exists.
		/// </summary>
		/// <param name="_key">Key of the entry to retrieve.</param>
		/// <param name="_out">Out value result.</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool Get(const Tk& _key, Tv& _out) const noexcept  {
			
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
		
		/// <summary>
		/// Retrieves a reference to the entry within the Hashmap with the given key, if one exists.
		///	This method will throw an exception if no entry is found. Consider using Get() instead.
		/// </summary>
		/// <param name="_key">Key of the entry to retrieve.</param>
		/// <returns>Out value result.</returns>
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
		
		/// <summary>
		/// Trims unused entries from the end of the Hashmap.
		/// </summary>
		void Trim() {
			
			size_t trimStart = 1;
			
			for (size_t i = trimStart; i < m_Buckets.size(); i++) {
				if (m_Buckets[i].size() != 0) {
					trimStart = i + 1;
				}
			}
			
			if (trimStart < m_Buckets.size()) {
				m_Buckets.erase(m_Buckets.begin() + trimStart);
			}
		}
		
		/// <summary>
		/// Returns the keys of all entries stored within the Hashmap.
		/// </summary>
		[[nodiscard]] std::vector<Tk> Keys() const {
			
			std::vector<Tk> result;
			
			for (auto& bucket : m_Buckets) {
				for (auto& kvp : bucket) {
					result.emplace_back(kvp.first);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Returns the values of all entries stored within the Hashmap.
		/// </summary>
		[[nodiscard]] std::vector<Tv> Values() const {
			
			std::vector<Tv> result;
			
			for (auto& bucket : m_Buckets) {
				for (auto& kvp : bucket) {
					result.emplace_back(kvp.second);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Returns all entries stored within the Hashmap.
		/// </summary>
		[[nodiscard]] std::vector<KeyValuePair> GetAll() const {
			
			std::vector<KeyValuePair> result;
			
			for (auto& bucket : m_Buckets) {
				for (auto& kvp : bucket) {
					result.emplace_back(kvp);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Clears all entries from the Hashmap.
		/// <para>
		/// <remarks>
		/// This function is not responsible for memory management of items contained within the Hashmap.
		/// </remarks>
		/// </para>
		/// </summary>
		void Clear() noexcept  {
			
			m_Buckets.clear();
			m_Buckets.resize(1);
		}
	};
	
} // LouiEriksson

#endif //LOUIERIKSSON_HASHMAP_H