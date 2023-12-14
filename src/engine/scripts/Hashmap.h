#pragma once

#ifndef FINALYEARPROJECT_HASHMAP_H
#define FINALYEARPROJECT_HASHMAP_H

namespace LouiEriksson {
	
	/// <summary>
	/// Custom hashmap implementation accepting a customisable key and value type. Created using a combination of prior knowledge and brief online tutorial.
	/// <para><remarks>This implementation requires that your "key" type is compatible with std::hash and that the stored data types are copyable.</remarks></para>
	/// <para>Reference: ("Implementing Your Own HashMap (Explanation + Code)", Random Coder, 14 Sept 2020 [https://www.youtube.com/watch?v=_Q-eNqTOxlE]).</para>
	/// </summary>
	/// <typeparam name="Tk">Key type of the hashmap.</typeparam>
	/// <typeparam name="Tv">Value type of the hashmap.</typeparam>
	template<typename Tk, typename Tv>
	class Hashmap {
	
	public:
		
		struct KeyValuePair {
			
			Tk first;
			Tv second;
			
			KeyValuePair(Tk _key, Tv _value) :
				 first(_key),
				second(_value) {}
			
            KeyValuePair(const KeyValuePair& other) :
				 first(other.first),
				second(other.second) {}
		};
		
	private:
		
		/// <summary>
		/// Buckets of the hashmap.
		/// </summary>
		std::vector<std::vector<KeyValuePair>> m_Buckets;
		
		/// <summary>
		/// Current number of elements within the hashmap.
		/// </summary>
		size_t m_Size;
		
		/// <summary>
		/// Calculate the hashcode of a given object using std::hash.
		/// <remarks>
		/// This function will throw if the type given is not supported by std::hash.
		/// </remarks>
		/// <param name="_item">Item to calculate hash of.</param>
		/// </summary>
		static size_t GetHashcode(const Tk& _item) {
			return std::hash<Tk>()(_item);
		}
		
		/// <summary>
		/// Reinitialise the hashmap. An expensive operation that increases the hashmap's capacity.
		/// </summary>
		void Resize() {
			
			const size_t resize_amount = size();
			
			std::vector<std::vector<KeyValuePair>> shallowCopy(m_Buckets);
			
			m_Size = 0;
			m_Buckets.clear();
			m_Buckets.resize(size() + resize_amount);
			
			for (auto& bucket: shallowCopy) {
				for (auto& kvp: bucket) {
					Add(kvp.first, kvp.second);
				}
			}
		}
	
	public:
		
		/// <summary>
		/// Initialise hashmap.
		/// </summary>
		/// <param name="_capacity">Initial capacity of the hashmap. Must be larger than 0.</param>
		Hashmap(size_t _capacity = 1) {
			
			m_Size = 0;
			
			m_Buckets.resize(_capacity);
		}
		
		/// <summary>
		/// Returns the number of items stored within the hashmap.
		/// </summary>
		[[nodiscard]] size_t size() const {
			return m_Size;
		}
		
		/// <summary>
		/// Returns true if the Hashmap contains no entries.
		/// </summary>
		[[nodiscard]] bool empty() const {
			return m_Size == 0;
		}
		
		/// <summary>
		/// Inserts a new entry into the hashmap with given key and value, if one does not already exist.
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
			
			bool result = true;
			
			if (size() >= m_Buckets.size()) {
				Resize();
			}
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			// In the case of a hash collision, determine if the key is unique.
			// We will treat duplicate insertions as a mistake on the developer's part and return failure.
			for (auto& kvp: bucket) {
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
		/// Inserts or replaces an entry within the hashmap with the given key.
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
			
			bool exists = false;
			for (auto& kvp: bucket) {
				
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
		/// Removes entry with given key from the hashmap.
		/// </summary>
		/// <param name="_key">Key of the entry to be removed.</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool Remove(const Tk& _key) {
			
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
		/// Retrieves a reference to the entry within the hashmap with the given key, if one exists.
		/// </summary>
		/// <param name="_key">Key of the entry to retrieve.</param>
		/// <param name="_out">Out value result.</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool Get(const Tk& _key, Tv& _out) const {
			
			bool result = false;
			
			// Create an index by taking the key's hash value and "wrapping" it with the number of buckets.
			size_t hash = GetHashcode(_key);
			size_t i = hash % m_Buckets.size();
			
			auto& bucket = m_Buckets[i];
			
			for (auto& kvp: bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					result = true;
					
					_out = kvp.second;
					
					break;
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Retrieves a reference to the entry within the hashmap with the given key, if one exists.
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
			
			for (auto& kvp: bucket) {
				
				if (GetHashcode(kvp.first) == hash) {
					result = &kvp.second;
					
					break;
				}
			}
			
			if (result == nullptr) {
				throw std::runtime_error("Attempted to access a nonexistent entry from the hashmap.");
			}
			
			return *result;
		}
		
		/// <summary>
		/// Trims unused entries from the end of the hashmap.
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
		/// Returns the keys of all entries stored within the hashmap.
		/// </summary>
		std::vector<Tk> Keys() const {
			
			std::vector<Tk> result;
			
			for (auto& bucket: m_Buckets) {
				for (auto& kvp: bucket) {
					result.push_back(kvp.first);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Returns the values of all entries stored within the hashmap.
		/// </summary>
		std::vector<Tv> Values() const {
			
			std::vector<Tv> result;
			
			for (auto& bucket: m_Buckets) {
				for (auto& kvp: bucket) {
					result.push_back(kvp.second);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Returns all entries stored within the hashmap.
		/// </summary>
		std::vector<KeyValuePair> GetAll() const {
			
			std::vector<KeyValuePair> result;
			
			for (auto& bucket: m_Buckets) {
				for (auto& kvp: bucket) {
					result.push_back(kvp);
				}
			}
			
			return result;
		}
		
		/// <summary>
		/// Clears all entries from the hashmap.
		/// <para>
		/// <remarks>
		/// This function is not responsible for memory management of items contained within the hashmap.
		/// </remarks>
		/// </para>
		/// </summary>
		void Clear() {
			
			m_Buckets.clear();
			m_Buckets.resize(1);
		}
	};
}

#endif //FINALYEARPROJECT_HASHMAP_H