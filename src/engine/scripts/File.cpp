#include "stdafx.h"

#include "File.h"

std::string File::ReadAllText(std::filesystem::path _path) {

	std::stringstream result;

	std::fstream newFile;
	newFile.open(_path, std::ios::in);

	if (newFile.is_open()) {

		std::string line;

		while (std::getline(newFile, line)) {
			result << line << "\n";
		}
		newFile.close();
	}

	return result.str();
}

std::filesystem::path File::AsPath(const char* _path) {
	return std::filesystem::path(_path);
}
