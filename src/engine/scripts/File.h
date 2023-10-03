#pragma once

#ifndef FINALYEARPROJECT_FILE_H
#define FINALYEARPROJECT_FILE_H

namespace LouiEriksson {
	
	class File {
	
	public:
		
		 File()                   = delete;
		 File(const File& _other) = delete;
		~File()                   = delete;
		
		File& operator = (const File& _other) = delete;
		
		static std::string ReadAllText(const std::filesystem::path& _path);
	
		static std::filesystem::path AsPath(const char* _path);
	};
}

#endif //FINALYEARPROJECT_FILE_H