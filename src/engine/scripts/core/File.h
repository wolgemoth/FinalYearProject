#ifndef FINALYEARPROJECT_FILE_H
#define FINALYEARPROJECT_FILE_H

#include "../audio/AudioClip.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Texture.h"
#include "../graphics/textures/Cubemap.h"

#include <GL/glew.h>

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// @Assessor: This class has changed significantly since it was submitted for 3DGP. Please mark it for GEP.

namespace LouiEriksson {
	
	class File {
	
	public:
		
		struct Directory {
			
			enum EntryType : char {
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
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<AudioClip>& _output);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Texture>& _output, GLenum _format, bool _generateMipmaps);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Mesh>& _output);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Material>& _output);
		
		static bool TryLoad(const std::array<std::filesystem::path, 6>& _paths, std::shared_ptr<Cubemap>& _output, GLenum _format, bool _generateMipmaps);
	};
}

#endif //FINALYEARPROJECT_FILE_H