#pragma once

#ifndef FINALYEARPROJECT_FILE_H
#define FINALYEARPROJECT_FILE_H

#include "Cubemap.h"

namespace LouiEriksson {
	
	class File {
	
	private:

	public:
		
		 File()                   = delete;
		 File(const File& _other) = delete;
		~File()                   = delete;
		
		File& operator = (const File& _other) = delete;
		
		static std::string ReadAllText(const std::filesystem::path& _path);
		
		static std::filesystem::path AsPath(const char* _path);
		
		static std::filesystem::path AsPath(const std::string& _path);
		
		static bool TryLoad(const std::filesystem::path& _path, Texture& _output, GLenum _format, bool _generateMipmaps);
		
		static Cubemap Load(const std::array<std::filesystem::path, 6>& _paths, GLenum _format, bool _generateMipmaps);
	};
}

#endif //FINALYEARPROJECT_FILE_H