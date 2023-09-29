#pragma once

#ifndef _FILE_H
#define _FILE_H

class File {

private:

	 File() = delete;
	 File(const File& _other) = delete;
	~File() = delete;

	File& operator = (const File& _other) = delete;

public:

	static std::string ReadAllText(std::filesystem::path _path);

	static std::filesystem::path AsPath(const char* _path);
};

#endif // !_FILE_H
