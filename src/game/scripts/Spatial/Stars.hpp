#ifndef FINALYEARPROJECT_STARS_HPP
#define FINALYEARPROJECT_STARS_HPP

#include "../../include/engine_core.hpp"
#include "../../include/engine_graphics.hpp"
#include "../../include/engine_spatial.hpp"

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Engine::Graphics;
using namespace LouiEriksson::Engine::Spatial;

namespace LouiEriksson::Game::Scripts::Spatial {
	
	/**
	 * @class Stars
	 * @brief 3D stellar field centered around Sol.
	 */
	class Stars final : public Script {
	
	public:
	
		explicit Stars(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
		
		/** @inheritdoc */
		[[nodiscard]] virtual std::type_index TypeID() const noexcept override { return typeid(Stars); };
		
	protected:
	
		/** @inheritdoc */
		virtual void Begin() override  {
			
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
				
					const auto transform = p->AddComponent<Transform>();
					const auto renderer  = p->AddComponent<Graphics::Renderer>();
					
					auto material = Resources::Get<Graphics::Material>("Stars");
					
					if (material.lock()) {
						renderer->SetMesh(stars);
						renderer->SetMaterial(material);
						renderer->SetTransform(transform);
					}
				}}
				
				Debug::Log("Done.", LogType::Info);
			}
			catch (const std::exception& e) {
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(e);
			}
		}
	
		static std::shared_ptr<Mesh> LoadStars(const std::vector<std::filesystem::path>& _athyg_paths, const double& _threshold_magnitude) {
		
			using ATHYG_VERSION = Engine::Spatial::ATHYG::V3;
			using vertex_t      = GLfloat;
			
			std::shared_ptr<Mesh> result;
			
			// Pool of tasks.
			std::vector<std::future<std::vector<glm::vec<3, vertex_t>>>> s_Tasks;
			
			// Load and parse each file in parallel:
			for (const auto& path : _athyg_paths) {
				
				s_Tasks.emplace_back(std::async([&path, &_threshold_magnitude]() {
				
					std::vector<glm::vec<3, vertex_t>> result;
					
					try {
						
						Debug::Log("Parsing \"" + path.string() + "\"... ", LogType::Info);
					
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
									
									const auto star = ATHYG_VERSION((Utils::ToArray<std::string_view, ATHYG_VERSION::s_ElementCount>(std::move(elements))));
									
									if (*star.mag <= _threshold_magnitude) {
										result.emplace_back(*star.x0, *star.y0, *star.z0);
									}
								}
							}
						}
						else {
							throw std::runtime_error("Path is not valid.");
						}
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
					
					return result;
					
				}));
			}
			
			std::vector<glm::vec<3, vertex_t>> stars;
			
			/**
			 * Wait for all parallel tasks to complete and accumulate them sequentially.
			 *
			 * @note Results will arrive in the same in the order.
			 */
			for (auto& task : s_Tasks) {
				Utils::MoveInto(task.get(), stars);
			}
			
			const auto vertex_count = stars.size();
			Debug::Log("Parsing of data yielded (" + std::to_string(vertex_count) + ") entries(s).", LogType::Info);
			
			if (!stars.empty()) {
				
				Debug::Log("Creating Point Cloud...", LogType::Info, true);
			
				try {
					if (vertex_count > std::numeric_limits<GLushort>::max()) {        // (32-bit)
						result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLuint>(stars);
					}
					else if (vertex_count > std::numeric_limits<GLubyte>::max()) {    // (16-bit)
						result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLushort>(stars);
					}
					else {                                                            // (8-bit)
						result = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLubyte>(stars);
					}
					
					Debug::Log("Done.", LogType::Info);
				}
				catch (const std::exception& e) {
					Debug::Log("Failed.", LogType::Info);
					Debug::Log(e);
					Debug::Break();
				}
			}
			
			return result;
		}
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_STARS_HPP