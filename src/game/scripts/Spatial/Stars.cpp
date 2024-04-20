#include "Stars.h"

namespace LouiEriksson::Game::Scripts::Spatial {
	
	Stars::Stars(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
	
	void Stars::Begin() {
		
		// Change the settings to allow us to see!
		Settings::Graphics::Perspective::s_FarClip = 40000.0;
		Settings::Graphics::Skybox::s_Exposure     = 0.0;
		
		// Parse files and build star mesh:
		const auto stars = LoadStars({
			"resources/ATHYG-Database-main/data/athyg_v31-1.csv",
			"resources/ATHYG-Database-main/data/athyg_v31-2.csv"
			},
			6.5
		);
		
		Debug::Log("Spawning " + std::to_string(stars->VertexCount()) + " stars... ", LogType::Info, true);
		
		/* SET STAR SIZE */
		glPointSize(2.0);
		
		// Spawn stars:
		try {
			
			if (const auto p =      Parent().lock()) {
			if (const auto s = p->GetScene().lock()) {
			
				const auto go = ECS::GameObject::Create(s, "Stars");
				
				if (const auto transform = go->AddComponent<Transform>().lock()         ) {
				if (const auto renderer  = go->AddComponent<Graphics::Renderer>().lock()) {
				
					auto material = Resources::Get<Graphics::Material>("Stars");
					
					if (material.lock()) {
						renderer->SetMesh(stars);
						renderer->SetMaterial(material);
						renderer->SetTransform(transform);
					}
				}}
			}}
			
			Debug::Log("Done.", LogType::Info);
		}
		catch (const std::exception& e) {
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(e);
		}
	}
	
	void Stars::Tick() {
	}
	
	std::shared_ptr<Mesh> Stars::LoadStars(const std::vector<std::filesystem::path>& _athyg_paths, const double& _threshold_magnitude) {
	
		using ATHYG_VERSION = Engine::Spatial::ATHYG::V3;
		
		std::vector<glm::vec3> star_positions;
		
		for (const auto& path : _athyg_paths) {
			
			Debug::Log("Parsing \"" + path.string() + "\"... ", LogType::Info, true);
			
			try {
			
				if (exists(path)) {
					
					auto csv = File::ReadAllText(path);
					
					std::string line;
					
					// Skip the header (first line) of the CSV.
					std::getline(csv, line);
					
					// Process CSV elements:
				    while (std::getline(csv, line)) {
						
						auto elements = Utils::Split(line, ',', ATHYG_VERSION::s_ElementCount);
						
						if (elements.size() > ATHYG_VERSION::s_ElementCount) {
							elements.resize(ATHYG_VERSION::s_ElementCount);
						}
						
						if (elements.size() == ATHYG_VERSION::s_ElementCount) {
							
							const auto star = ATHYG_VERSION((Utils::MoveToArray<std::string_view, ATHYG_VERSION::s_ElementCount>(elements)));
							
							if (*star.mag <= _threshold_magnitude) {
								star_positions.emplace_back(*star.x0, *star.y0, *star.z0);
							}
						}
					}
					
					Debug::Log("Done.", LogType::Info);
				}
				else {
					throw std::runtime_error("Path is not valid.");
				}
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
		}
		
		std::shared_ptr<Mesh> result;
		
		using vertex_t = GLfloat;
		
		const auto vertex_count = star_positions.size();
	
		if (vertex_count > std::numeric_limits<GLushort>::max()) {        // (32-bit)
			result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLuint>(star_positions);
		}
		else if (vertex_count > std::numeric_limits<GLubyte>::max()) {    // (16-bit)
			result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLushort>(star_positions);
		}
		else {                                                            // (8-bit)
			result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLubyte>(star_positions);
		}
		
		return result;
	}
	
} // LouiEriksson::Game::Scripts::Spatial