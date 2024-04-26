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
	
	private:
		
		Threading::Utils::Dispatcher        m_Dispatcher;
		Threading::Utils::CancellationToken m_CancellationToken;
		
		std::future<void> m_Task;
		
	public:
	
		explicit Stars(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
		
		~Stars() {
			m_CancellationToken.Cancel();
		}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Stars); };
		
	protected:
	
		/** @inheritdoc */
		void Begin() override  {
			
			using vertex_t = GLfloat;
			
			/* SET STAR SIZE */
			glPointSize(2.0);
			
			// Change the settings to allow us to see!
			Settings::Graphics::Perspective::s_FarClip = 40000.0;
			Settings::Graphics::Skybox::s_Exposure     = 0.0;
			
			// Parse files and build star mesh:
			m_Task = std::async([this]() {
				
				const auto stars = LoadStars<GLfloat>({
					"resources/ATHYG-Database-main/data/athyg_v31-1.csv",
					"resources/ATHYG-Database-main/data/athyg_v31-2.csv"
					},
					6.5,
					m_CancellationToken
				);
				
				if (!stars.empty()) {
					
					m_Dispatcher.Schedule([this, stars]() {
						
						// Spawn stars:
						try {
							
							Debug::Log("Spawning " + std::to_string(stars.size()) + " stars... ", LogType::Info, true);
							
							std::shared_ptr<Mesh> mesh;
							
							if (stars.size() > std::numeric_limits<GLushort>::max()) {        // (32-bit)
								mesh = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLuint>(stars);
							}
							else if (stars.size() > std::numeric_limits<GLubyte>::max()) {    // (16-bit)
								mesh = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLushort>(stars);
							}
							else {                                                            // (8-bit)
								mesh = Engine::Graphics::Mesh::Primitives::PointCloud::Create<vertex_t, GLubyte>(stars);
							}
					
							if (const auto p = Parent()) {
							if (const auto s = p->GetScene().lock()) {
							
								const auto transform = p->AddComponent<Transform>();
								const auto renderer  = p->AddComponent<Graphics::Renderer>();
								
								auto material = Resources::Get<Graphics::Material>("Stars");
								
								if (material.lock()) {
									renderer->SetMesh(mesh);
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
					});
				}
			});
		}
	
		/** @inheritdoc */
		void Tick() override {
			m_Dispatcher.Dispatch(1);
		}
		
		template <typename T, glm::qualifier Q = glm::defaultp>
		static std::vector<glm::vec<3, T, Q>> LoadStars(const std::vector<std::filesystem::path>& _athyg_paths, const double& _threshold_magnitude, Threading::Utils::CancellationToken& _cancellationToken) {
		
			using ATHYG_VERSION = Engine::Spatial::ATHYG::V3;
			
			std::vector<glm::vec<3, T, Q>> result;
			
			std::vector<std::future<std::vector<glm::vec<3, T, Q>>>> m_Tasks;
			
			// Load and parse each file in parallel:
			for (const auto& path : _athyg_paths) {
				
				m_Tasks.emplace_back(std::async([&path, &_threshold_magnitude, &_cancellationToken]() {
				
					std::vector<glm::vec<3, T, Q>> parsed;
					
					try {
						
						Debug::Log("Parsing \"" + path.string() + "\"... ", LogType::Info);
					
						if (exists(path)) {
							
							auto csv = File::ReadAllText(path);
							
							std::string line;
							
							// Skip the header (first line) of the CSV.
							std::getline(csv, line);
							
							// Process CSV elements:
						    while (std::getline(csv, line)) {
								
								if (_cancellationToken.IsCancellationRequested()) { break; }
								
								auto elements = Utils::Split(line, ',', ATHYG_VERSION::s_ElementCount);
								
								if (elements.size() > ATHYG_VERSION::s_ElementCount) {
									elements.resize(ATHYG_VERSION::s_ElementCount);
								}
								
								if (elements.size() == ATHYG_VERSION::s_ElementCount) {
									
									const auto star = ATHYG_VERSION((Utils::ToArray<std::string_view, ATHYG_VERSION::s_ElementCount>(std::move(elements))));
									
									if (*star.mag <= _threshold_magnitude) {
										parsed.emplace_back(-*star.y0, -*star.x0, *star.z0);
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
					
					return parsed;
					
				}));
			}
			
			if (_cancellationToken.IsCancellationRequested()) { return result; }
			
			/**
			 * Wait for all parallel tasks to complete and accumulate them sequentially.
			 *
			 * @note Results will arrive in the same in the order.
			 */
			for (auto& task : m_Tasks) {
				Utils::MoveInto(task.get(), result);
			}
			
			Debug::Log("Parsing of data yielded (" + std::to_string(result.size()) + ") entries(s).", LogType::Info);
			
			return result;
		}
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_STARS_HPP