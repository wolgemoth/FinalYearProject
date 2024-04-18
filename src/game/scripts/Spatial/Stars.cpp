#include "Stars.h"

namespace LouiEriksson::Game::Scripts::Spatial {
	
	Stars::Stars(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
	
	Stars::~Stars() = default;
	
	void Stars::Begin() {
		
		// Change the settings to allow us to see!
		Settings::Graphics::Perspective::s_FarClip = 40000.0f;
		Settings::Graphics::Skybox::s_Exposure     = 0.0f;
		
		// Parse files and build star mesh:
		const auto stars = LoadStars({
			"resources/ATHYG-Database-main/data/athyg_v31-1.csv",
			"resources/ATHYG-Database-main/data/athyg_v31-2.csv"
			},
			6.5
		);
		
		Debug::Log("Spawning " + std::to_string(stars->VertexCount()) + " stars... ", LogType::Info, true);
		
		/* SET STAR SIZE */
		glPointSize(2.0f);
		
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
	
		std::shared_ptr<Mesh> result;
		
		std::vector<std::string> lines;
		
		for (const auto& path : _athyg_paths) {
			
			Debug::Log("Loading \"" + path.string() + "\"... ", LogType::Info, true);
			
			try {
			
				if (exists(path)) {
					
					auto data = Utils::Split(File::ReadAllText(path), '\n');
					
					std::move(data.begin(), data.end(), std::back_inserter(lines));
					
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
		
		Debug::Log("Parsing " + std::to_string(!lines.empty() ? lines.size() - 1 : lines.size()) + " lines... ", LogType::Info, true);
		
		std::vector<glm::vec3> star_positions;
		
		try {
			
			for (auto line = lines.begin() + 1; line != lines.end(); ++line) {
				
				auto elements = Utils::Split(*line, ',');
				
				if (elements.size() > 34) {
					elements.resize(34);
				}
				
				if (elements.size() == 34) {
					
					const auto star = Engine::Spatial::ATHYG::V3(Utils::MoveToArray<std::string, 34>(elements));
					
					if (*star.mag <= _threshold_magnitude) {
						star_positions.emplace_back(*star.x0, *star.y0, *star.z0);
					}
				}
			}
			
			Debug::Log("Done.", LogType::Info);
		}
		catch (const std::exception& e) {
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(e);
		}
		
		return Engine::Graphics::Mesh::Primitives::PointCloud::Create(star_positions);
	}
	
} // LouiEriksson::Game::Scripts::Spatial