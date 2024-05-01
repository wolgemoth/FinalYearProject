#ifndef FINALYEARPROJECT_FILE_HPP
#define FINALYEARPROJECT_FILE_HPP

#include <cmath>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace LouiEriksson::Engine {
	
	class File final {
	
	public:
		
		struct Directory final {
			
			enum EntryType : unsigned char {
				FILE      = 1U << 0U, /**< @brief File entries. */
				DIRECTORY = 1U << 1U  /**< @brief Directory entries. */
			};
			
			static std::vector<std::filesystem::path> GetEntries(const std::filesystem::path& _path, const Directory::EntryType& _type) {
			
				std::vector<std::filesystem::path> result;
				
				const std::filesystem::directory_iterator itr(_path);
				
				for (const auto& item : itr) {
					
					const bool append = item.is_directory() ?
							(_type & Directory::EntryType::DIRECTORY) != 0U :
							(_type & Directory::EntryType::FILE     ) != 0U;
					
					if (append) {
					    result.emplace_back(item);
					}
				}
				
				return result;
			}
			
			static std::vector<std::filesystem::path> GetEntriesRecursive(const std::filesystem::path& _path, const Directory::EntryType& _type) {
			
				std::vector<std::filesystem::path> result;
				std::vector<std::filesystem::path> subDirectories;
				
				// Get all entries.
				for (const auto& item : GetEntries(_path, static_cast<Directory::EntryType>(Directory::EntryType::FILE | Directory::EntryType::DIRECTORY))) {
					
					bool append;
					
					if (is_directory(item)) {
					
						// Append all subdirectories to a vector.
						subDirectories.emplace_back(item);
						
						append = (_type & Directory::EntryType::DIRECTORY) != 0U;
					}
					else {
						append = (_type & Directory::EntryType::FILE) != 0U;
					}
					
					// Append entries of requested type to result.
					if (append) { result.emplace_back(item); }
				}
				
				// Iterate through all subdirectories recursively and append output to result.
				for (const auto& subDirectory : subDirectories) {
				
					auto items = GetEntriesRecursive(subDirectory, _type);
					
					result.insert(result.end(), items.begin(), items.end());
				}
				
				return result;
			}
		};
		
		 File()                   = delete;
		 File(const File& _other) = delete;
		~File()                   = delete;
		
		File& operator = (const File& _other) = delete;
		
		/**
		 * @fn static std::stringstream ReadAllText(const std::filesystem::path& _path)
		 * @brief Reads the contents of a text file.
		 * @param _path The path to the file.
		 * @return A std::stringstream containing the contents of the file.
		 * @throws std::runtime_error If the file path is invalid.
		 *
		 * This function reads the contents of a text file specified by the file path.
		 * It returns the contents of the file as a std::stringstream object.
		 *
		 * If the file does not exist or if the file cannot be opened, an exception is thrown.
		 */
		static std::stringstream ReadAllText(const std::filesystem::path& _path) {
		
			std::stringstream result;
			
			if (exists(_path)) {
			
				std::fstream fs;
				fs.open(_path, std::ios::in);
			
				if (fs.is_open()) {
					result << fs.rdbuf();
					fs.close();
				}
			}
			else {
				throw std::runtime_error("Invalid file path");
			}
		
			return result;
		}
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_FILE_HPP