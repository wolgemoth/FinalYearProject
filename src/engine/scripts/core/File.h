#ifndef FINALYEARPROJECT_FILE_H
#define FINALYEARPROJECT_FILE_H

#include "../graphics/Texture.h"

#include <GL/glew.h>

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace LouiEriksson::Engine::Audio {

	class AudioClip;
	
} // LouiEriksson::Engine::Audio

namespace LouiEriksson::Engine::Graphics {
	
	class Cubemap;
	class Material;
	class Mesh;
	class Shader;
	struct SubShader;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine {
	
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
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Audio::AudioClip>& _output);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Texture>& _output,
				const Graphics::Texture::Parameters::Format&     _format     = { GL_RGBA,   true      },
				const Graphics::Texture::Parameters::FilterMode& _filterMode = { GL_LINEAR, GL_LINEAR },
				const Graphics::Texture::Parameters::WrapMode&   _wrapMode   = { GL_REPEAT, GL_REPEAT });
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Mesh>& _output);
		
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Material>& _output);
		
		static bool TryLoad(const std::array<std::filesystem::path, 6>& _paths, std::shared_ptr<Graphics::Cubemap>& _output, GLenum _format = GL_RGBA, bool _generateMipmaps = true);
	
		static bool TryLoad(const std::filesystem::path& _path, std::shared_ptr<Graphics::Shader>& _output);

		static bool TryLoad(std::vector<Graphics::SubShader> _subshaders, std::shared_ptr<Graphics::Shader>& _output);
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_FILE_H