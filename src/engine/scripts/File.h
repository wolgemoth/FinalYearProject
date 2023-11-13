#pragma once

#ifndef FINALYEARPROJECT_FILE_H
#define FINALYEARPROJECT_FILE_H

#include "Mesh.h"
#include "Cubemap.h"

namespace LouiEriksson {
	
	class File {
	
	public:
		
		struct Directory {
			
			enum EntryType {
				FILE      = 1u << 0u,
				DIRECTORY = 1u << 1u
			};
			
			static std::vector<std::filesystem::path> GetEntries(const std::filesystem::path& _path, const File::Directory::EntryType& _type = (File::Directory::EntryType)(File::Directory::EntryType::FILE | File::Directory::EntryType::DIRECTORY));
		
			static std::vector<std::filesystem::path> GetEntriesRecursive(const std::filesystem::path& _path, const File::Directory::EntryType& _type = (File::Directory::EntryType)(File::Directory::EntryType::FILE | File::Directory::EntryType::DIRECTORY));
		
		};
		
		
		 File()                   = delete;
		 File(const File& _other) = delete;
		~File()                   = delete;
		
		File& operator = (const File& _other) = delete;
		
		static std::string ReadAllText(const std::filesystem::path& _path);
		
		static std::filesystem::path AsPath(const char* _path);
		
		static std::filesystem::path AsPath(const std::string& _path);
		
		static bool TryLoad(const std::filesystem::path& _path, Texture& _output, GLenum _format, bool _generateMipmaps);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Mesh>& _output);
		
		static Cubemap Load(const std::array<std::filesystem::path, 6>& _paths, GLenum _format, bool _generateMipmaps);
	};
}

#endif //FINALYEARPROJECT_FILE_H