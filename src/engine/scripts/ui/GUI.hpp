#ifndef FINALYEARPROJECT_GUI_HPP
#define FINALYEARPROJECT_GUI_HPP

#include "../core/Window.hpp"

#include <glm/common.hpp>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <imgui.h>

namespace LouiEriksson::Engine {
	
	class Application;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Input {
	
	class Input;
	
} // LouiEriksson::Engine::Input

namespace LouiEriksson::Engine::UI {
	
	class GUI final {
		
		friend LouiEriksson::Engine::Application;
		friend LouiEriksson::Engine::Input::Input;
		
	private:
	
		struct Parameters final {
			
			enum Style : unsigned char {
				Dark  = 0U, /**< @brief  Dark theme. */
				Light = 1U, /**< @brief Light theme. */
			};
			
		};
		
		class GUIWindows final {
			
			friend GUI;
			
		private:
			
			/** @brief Constant for the margins between the window and screen edge. */
			inline static const ImVec2 s_WindowMargin { 16.0, 16.0 };
			
			static void DiagnosticsWindow(const Window& _window, const bool& _draw) {
				
				// Initialise FPS sampling window.
				static std::vector<tick_t> s_Timestamps;
				static std::vector<scalar_t> s_Samples;
				
				static constexpr tick_t s_Plot_SamplingWindowSize = 10.0;
				static constexpr tick_t s_FPS_SamplingWindowSize =  0.5;
				
				// Append the current frame.
				s_Timestamps.emplace_back(Time::Elapsed());
				   s_Samples.emplace_back(1.0 / Time::UnscaledDeltaTime());
				   
				// Extract various values from the sampling window:
				tick_t oldest_avg_timestamp  = std::numeric_limits<tick_t>::infinity();
				tick_t oldest_plot_timestamp = std::numeric_limits<tick_t>::infinity();
				scalar_t min_fps             = std::numeric_limits<scalar_t>::infinity();
				scalar_t max_fps             = static_cast<tick_t>(0.0);
				scalar_t avg_fps             = static_cast<tick_t>(0.0);
				scalar_t avg_fps_count       = static_cast<tick_t>(0.0);
				
				for (size_t i = 0U; i < s_Samples.size(); ++i) {
					
					const auto timestamp = s_Timestamps[i];
					auto fps = s_Samples[i];
					
					if (timestamp > 0.0) {
						
						if (timestamp < Time::Elapsed() - s_Plot_SamplingWindowSize) {
							
							s_Timestamps.erase(s_Timestamps.begin() + static_cast<std::vector<  tick_t>::difference_type>(i));
							   s_Samples.erase(   s_Samples.begin() + static_cast<std::vector<scalar_t>::difference_type>(i));
						}
						else {
							
							oldest_plot_timestamp = std::min(timestamp, oldest_plot_timestamp);
							
							min_fps = std::min(fps, min_fps);
							max_fps = std::max(fps, max_fps);
							
							if (timestamp > Time::Elapsed() - s_FPS_SamplingWindowSize) {
							
								oldest_avg_timestamp = std::min(timestamp, oldest_avg_timestamp);
								
								avg_fps += fps;
								avg_fps_count++;
							}
						}
					}
				}
				
				avg_fps /= std::max(avg_fps_count, static_cast<scalar_t>(1.0));
				
				// Diagnostics window:
				if (_draw) {
					
					// Set default values (on first run):
		            const auto screenSize = glm::vec2(_window.Dimensions());
					const auto windowSize = ImVec2(300, 200);
					
					ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
					ImGui::SetNextWindowPos(ImVec2(screenSize.x - windowSize.x - s_WindowMargin.x, s_WindowMargin.y), ImGuiCond_Once);
					ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
					
					// Create dynamically changing window title using the ### operator:
					{
						std::ostringstream title;
						title << std::fixed << std::setprecision(2)
						      << "Diagnostics ("
							  <<           avg_fps << " fps, "
							  << 1000.0 / avg_fps << " ms)###Diagnostics";
						
						// Draw elements:
						ImGui::Begin(title.str().c_str(), nullptr);
					}
					
					// Perform set up for rendering the plot:
					const scalar_t plot_vMargin = 15.0;
				
					auto cursor = ImGui::GetCursorPos();
				
					ImGui::SetCursorPosY(cursor.y + plot_vMargin);
				
					auto plot_cursor = ImGui::GetCursorPos();
					auto plot_size   = ImGui::GetContentRegionAvail();
					
					plot_size.y -= plot_vMargin;
					
					const scalar_t bottom = plot_cursor.y + plot_size.y;
					const scalar_t right  = plot_cursor.x + plot_size.x;
					const scalar_t hMargin = 5.0;
					
					const scalar_t range = max_fps - min_fps;
					
					// Draw the main plot:
					{
						// Make the main plot's lines slightly more transparent.
						auto plotLinesCol = ImGui::GetStyleColorVec4(ImGuiCol_PlotLines);
						plotLinesCol.w *= 0.7;
						
						ImGui::PushStyleColor(ImGuiCol_PlotLines, plotLinesCol);
						
						ImGui::PlotLines(
							"",
							s_Samples.data(),
							static_cast<int>(s_Samples.size() - 1),
							1,
							nullptr,
							min_fps,
							max_fps,
							plot_size
						);
						
						ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLines
					}
					
					/*
					 * Some relevant refresh rates:
					 * 1-10 fps:
					 *     - Indicates extremely low performance.
					 *     - May be seen briefly when transitioning between scenes or performing intensive operations.
					 *     - Some networking APIs may sync at a low polling-rate that falls within this range.
					 * 10-12 fps:
					 *     - Point at which the human eye starts to perceive separate images as continuous.
					 * 24 fps:
					 *     - Standard cinematic frame rate.
					 * 30 fps:
					 *     - Half of NTSC refresh rate (approximately).
					 *     - Became standard for some time.
					 *     - Is now considered the acceptable minimum fps for some platforms.
					 * 50 fps:
					 *     - PAL standard.
					 * 60 fps:
					 *     - NTSC standard (approximately).
					 *     - Modern standard for acceptable refresh rate.
					 *     - Highest supported framerate on most non-overclocked consumer displays.
					 * 72 fps:
					 *     - 3 x 24 fps.
					 *     - Half of 144 fps.
					 *     - Not used much outside of high-speed cinematography.
					 * 90 fps:
					 *     - 3:4 ratio of 120 fps
					 *     - Considered by some to be the entry-level for VR rendering.
					 *     - This is the point where I start finding it hard to distinguish smoother values by eye.
					 * 100 fps:
					 *     - 2 x 50 fps.
					 *     - A "low" polling-rate used by some peripherals.
					 * 120 fps:
					 *     - 2 x 60 fps.
					 *     - Some modern mobile devices and higher-end consumer displays use this.
					 *     - Commonly supported by VR headsets.
					 * 144 fps:
					 *     - 2 x 72 fps.
					 *     - Some gaming monitors use this.
					 * 240, 360, 480 fps:
					 *     - Multiples of 120 fps.
					 *     - Only seen on high-end displays
					 *     - Rarely achieved on low-end systems.
					 *     - At this point, there may be no need to post-process motion blur, or an
					 *           accumulation approach may be more viable.
					 * 500-1000 fps:
					 *     - Only seen on extremely high-end displays typically inaccessible to consumers.
					 * 1000+ fps:
					 *     - <1ms rendering time.
					 *     - Polling-rate for some consumer peripherals.
					 *     - Likely pointless in most consumer use-cases.
					 */
					const std::vector<scalar_t> fps_ticks {
						   1.0,   3.0,   6.0,  12.0,  24.0,   30.0,
						  50.0,  60.0,  72.0,  90.0, 100.0,  120.0,
						 144.0, 240.0, 360.0, 480.0, 500.0, 1000.0,
						2000.0
					};
					
					// Make plot backgrounds transparent and match their 'hovered' color to their normal color.
				    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0, 0.0, 0.0, 0.0));
					ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
					
					// Draw vertical lines for time plots.
					{
						const scalar_t t_interval = s_Plot_SamplingWindowSize / 4.0;
						
						tick_t t = Time::Elapsed();
						
						while (t > oldest_plot_timestamp) {
							
							// Compute the x offset of the line.
							const auto x_offset = Utils::Remap(t, oldest_plot_timestamp, Time::Elapsed(), static_cast<tick_t>(0.0), static_cast<tick_t>(plot_size.x));
							
							// Draw the line:
							{
								ImGui::SetCursorPos(plot_cursor);
								auto cursor_screenpos = ImGui::GetCursorScreenPos();
								
								auto startPos = ImVec2(cursor_screenpos.x + static_cast<float>(x_offset), cursor_screenpos.y);
								auto   endPos = ImVec2(startPos.x, startPos.y + plot_size.y);
								
								ImGui::GetWindowDrawList()->AddLine(startPos, endPos, ImGui::GetColorU32(ImGuiCol_PlotLines));
							}
							
							// Add a label:
							{
								// Format the label string in advance and use this to calculate its size.
								std::ostringstream label;
								label << std::fixed << std::setprecision(1)
								      << "-" << Time::Elapsed() - t << " s";
								
								auto textSize = ImGui::CalcTextSize(label.str().c_str());
								
							    ImGui::SetCursorPosX(plot_cursor.x + static_cast<float>(x_offset) - textSize.x - hMargin);
							    ImGui::SetCursorPosY(bottom);
						        ImGui::Text("%s", label.str().c_str());
							}
							
							t -= t_interval;
						}
					}
					
					// Draw horizontal lines for the common FPS values.
					for (const auto val : fps_ticks) {
						
						if (val < max_fps && val > min_fps) {
							
							// Help to prevent congestion with too many labels,
							// by omitting the rendering of labels fewer than
							// 20 pixels distance from the max and min fps values.
							if (
								std::min(
									glm::abs(val - max_fps),
									glm::abs(val - min_fps)
								) / range * plot_size.y > 20.0
							) {
								
								// Draw the line:
								{
							        ImGui::SetCursorPos(plot_cursor);
								
									std::array<scalar_t, 2U> tick { val, val };
									ImGui::PlotLines("", tick.data(), tick.size(), 0, nullptr, min_fps, max_fps, plot_size);
								}
								
								// Add a label:
								{
								    ImGui::SetCursorPosX(plot_cursor.x);
								    ImGui::SetCursorPosY(plot_cursor.y + Utils::Remap(val, min_fps, max_fps, static_cast<scalar_t>(plot_size.y), static_cast<scalar_t>(0.0)));
									
							        ImGui::Text("%.1f fps", val);
								}
							}
						}
					}
				
					// Do average fps:
					{
						ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLinesHovered
						
						// Green styling for average fps line.
						ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0, 1.0, 0.0, 1.0));
						ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
						
						// Draw a line for the average fps:
						{
							ImGui::SetCursorPos(plot_cursor);
							
							std::array<scalar_t, 2U> tick { avg_fps, avg_fps };
						    ImGui::PlotLines("", tick.data(), tick.size(), 0, nullptr, min_fps, max_fps, plot_size);
						}
						
						ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLinesHovered
						ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLines
						ImGui::PopStyleColor(); // Reset ImGuiCol_FrameBg
						
						// Add a label:
						{
							// Format the label string in advance and use this to calculate its size.
							std::ostringstream label;
							label << std::fixed << std::setprecision(1) << avg_fps << " fps";
							
							auto textSize = ImGui::CalcTextSize(label.str().c_str());
							
							// Draw the label, using its size to correctly align it on the screen.
							ImGui::SetCursorPosX(right - textSize.x - hMargin);
						    ImGui::SetCursorPosY(plot_cursor.y + Utils::Remap(avg_fps, min_fps, max_fps, static_cast<scalar_t>(plot_size.y), static_cast<scalar_t>(0.0)));
					        ImGui::Text("%s", label.str().c_str());
						}
					}
					
					// Label the min fps.
					ImGui::SetCursorPosX(cursor.x);
				    ImGui::SetCursorPosY(bottom);
			        ImGui::Text("%.1f fps", min_fps);
					
					// Label the max fps.
				    ImGui::SetCursorPos(cursor);
			        ImGui::Text("%.1f fps", max_fps);
					
					ImGui::End();
				}
			}
			
			static void PostProcessingWindow(const Window& _window, const bool& _draw) {
				
				if (_draw) {
					
					/*
					 * Set minimum size constraints as this window will
					 * auto-size and we don't want it to be too small.
					 */
					ImGui::SetNextWindowSizeConstraints(
						ImVec2(200.0, 0.0),
						ImVec2(
							std::numeric_limits<scalar_t>::infinity(),
							std::numeric_limits<scalar_t>::infinity()
						)
					);
					
					ImGui::SetNextWindowPos(s_WindowMargin, ImGuiCond_Once);
					ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
					
					/* POST PROCESSING SETTINGS */
					ImGui::Begin("Post Processing", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
					
					// Toggle for post processing.
				    ImGui::Checkbox("Enabled", &Settings::PostProcessing::s_Enabled);
					
				    if (Settings::PostProcessing::s_Enabled) {
						
						/* AMBIENT OCCLUSION */
					    if (ImGui::TreeNode("Ambient Occlusion")) {
							
							using target = Settings::PostProcessing::AmbientOcclusion;
							
					        ImGui::TextWrapped("Adds self-shadowing effects to areas of the image where light doesn't easily reach.");
					
							// Toggle for ambient occlusion.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							
						    if (target::s_Enabled) {
								
								ImGui::SliderInt("Samples",   &target::s_Samples,   1, 32);
								ImGui::SliderInt("Downscale", &target::s_Downscale, 0,  4);
								
								ImGui::SliderFloat("Intensity", &target::s_Intensity, 0.0, 6.0);
								ImGui::SliderFloat("Radius",    &target::s_Radius,    0.0, 2.0);
						    }
							
					        ImGui::TreePop(); // END AMBIENT OCCLUSION SECTION.
					    }
						
						/* BLOOM SETTINGS */
					    if (ImGui::TreeNode("Bloom")) {
							
							using target = Settings::PostProcessing::Bloom;
							
					        ImGui::TextWrapped("Simulates the diffusion of light around bright parts of an image.");
					
							// Toggle for bloom.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							
						    if (target::s_Enabled) {
								
								ImGui::DragFloat("Intensity", &target::s_Intensity, 0.001, 0.0, 65535.0);
								ImGui::DragFloat("Threshold", &target::s_Threshold, 0.001, 0.0, 65535.0);
								ImGui::DragFloat("Clamp",     &target::s_Clamp,     0.001, 0.0, 65535.0);
								ImGui::DragFloat("Lens Dirt", &target::s_LensDirt,  0.001, 0.0, 65535.0);
								
								ImGui::SliderFloat("Anamorphism", &target::s_Anamorphism, -1.0,  1.0);
								ImGui::SliderFloat("Diffusion",   &target::s_Diffusion,    0.0, 10.0);
						    }
							
					        ImGui::TreePop(); // END BLOOM SECTION.
					    }
						
						/* TONEMAPPING SETTINGS */
					    if (ImGui::TreeNode("Tone Mapping")) {
							
							using target              = Settings::PostProcessing::ToneMapping;
							using target_autoExposure = Settings::PostProcessing::ToneMapping::AutoExposure;
									
					        ImGui::TextWrapped("Remaps the colors of the image from HDR to LDR.");
							
							// Toggle for tonemapping.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							
						    if (target::s_Enabled) {
								
								ImGui::DragFloat("Gain",     &target::s_Gain,     0.001);
								ImGui::DragFloat("Exposure", &target::s_Exposure, 0.001);
								
								/* AUTO-EXPOSURE */
								ImGui::Checkbox("Auto Exposure", &target_autoExposure::s_Enabled);
								
							    if (target_autoExposure::s_Enabled) {
									
							        ImGui::TextWrapped("Automatically adjusts the overall exposure of the image towards a designated exposure target");
									
									ImGui::SliderFloat("Min EV", &target_autoExposure::s_MinEV, 0.0, 9.0);
									ImGui::SliderFloat("Max EV", &target_autoExposure::s_MaxEV, 0.0, 9.0);
									
									ImGui::DragFloat("Compensation", &target_autoExposure::s_Compensation, 0.001);
									
									ImGui::SliderFloat("Speed Down", &target_autoExposure::s_SpeedDown, 0.0, 10.0);
									ImGui::SliderFloat("Speed Up",   &target_autoExposure::s_SpeedUp,   0.0, 10.0);
							    }
						    }
							
					        ImGui::TreePop(); // END TONEMAPPING SECTION.
					    }
						
						/* ANTI-ALIASING SETTINGS */
					    if (ImGui::TreeNode("Anti-Aliasing")) {
							
							using target = Settings::PostProcessing::AntiAliasing;
							
					        ImGui::TextWrapped("Reduces jagged aliasing artifacts in the image.");
							
							// Toggle for anti-aliasing.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							 
						    if (target::s_Enabled) {
								
								ImGui::SliderFloat("Contrast Threshold",    &target::s_ContrastThreshold, 0.0312, 0.0833);
								ImGui::SliderFloat("Relative Threshold",    &target::s_RelativeThreshold, 0.063,  0.333);
								ImGui::SliderFloat("Subpixel Blending",     &target::s_SubpixelBlending,  0.0,    1.0);
								ImGui::SliderFloat("Edge Blending",         &target::s_EdgeBlending,      0.0,    1.0);
								
								ImGui::DragFloat("Local Contrast Modifier", &target::s_LocalContrastModifier, 0.001);
								
								target::s_LocalContrastModifier = std::max(target::s_LocalContrastModifier, static_cast<scalar_t>(0.0));
						    }
							
					        ImGui::TreePop(); // END ANTI-ALIASING SECTION.
					    }
						
						/* GRAIN SETTINGS */
					    if (ImGui::TreeNode("Grain")) {
							
							using target = Settings::PostProcessing::Grain;
							
					        ImGui::TextWrapped("Adds visual noise to an image, which can be applied aesthetically or as a technique to reduce the prominence of color banding.");
							
							// Toggle for grain.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							
						    if (target::s_Enabled) {
								ImGui::SliderFloat("Intensity", &target::s_Intensity, 0.0, 1.0);
						    }
							
					        ImGui::TreePop(); // END GRAIN SECTION.
					    }
						
						/* GRAIN SETTINGS */
					    if (ImGui::TreeNode("Vignette")) {
							
							using target = Settings::PostProcessing::Vignette;
							
					        ImGui::TextWrapped("Darkens areas of the image further from the center.");
							
							// Toggle for vignette.
						    ImGui::Checkbox("Enabled", &target::s_Enabled);
							
						    if (target::s_Enabled) {
								ImGui::SliderFloat("Intensity",  &target::s_Intensity,  0.0, 1.0);
								ImGui::SliderFloat("Smoothness", &target::s_Smoothness, 0.0, 1.0);
						    }
							
					        ImGui::TreePop(); // END VIGNETTE SECTION.
					    }
				    }
					
					ImGui::End();
				}
			}
			
			static void RenderSettingsWindow(const Window& _window, const bool& _draw) {
			
				if (_draw) {
					
					/*
					 * Set minimum size constraints as this window will
					 * auto-size and we don't want it to be too small.
					 */
					ImGui::SetNextWindowSizeConstraints(
						ImVec2(200.0, 0.0),
						ImVec2(
							std::numeric_limits<scalar_t>::infinity(),
							std::numeric_limits<scalar_t>::infinity()
						)
					);
					
					ImGui::SetNextWindowPos(ImVec2(static_cast<float>((s_WindowMargin.x * 2.0) + 200.0), s_WindowMargin.y), ImGuiCond_Once);
					ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
					
					/* RENDERING SETTINGS */
					ImGui::Begin("Rendering", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			
					// Display checkbox for gamma correction.
					ImGui::Checkbox("Gamma Correction", &Settings::Graphics::s_GammaCorrection);
					
					/* CAMERA */
				    if (ImGui::TreeNode("Camera")) {
						
						using target_perspective = Settings::Graphics::Perspective;
					 
						/* VSYNC OPTIONS */
					    {
							using target_vsync = Settings::Graphics::VSync;
							
							ImGui::Combo("V-Sync", &target_vsync::s_CurrentSelection, target_vsync::s_AvailableOptions.data(), static_cast<int>(target_vsync::s_AvailableOptions.size()));
						}
						
						ImGui::SliderFloat("FOV", &target_perspective::s_FOV, 0.005, 180.0);
						ImGui::DragFloat("NearClip", &target_perspective::s_NearClip, 0.1, 0.005, 65535.0);
						ImGui::DragFloat("FarClip", &target_perspective::s_FarClip, 0.1, target_perspective::s_NearClip, 65535.0);
						
				        ImGui::TreePop(); // END CAMERA SECTION.
				    }
					
					/* SKYBOX */
				    if (ImGui::TreeNode("Skybox")) {
						
						using target = Settings::Graphics::Skybox;
						
						// Dropdown selection for skybox:
						static int selected = 0;
						
						ImGui::Combo(" ", &selected, target::s_AvailableSkyboxes.data(), static_cast<int>(target::s_AvailableSkyboxes.size()));
						
						// If the selected values mismatch, it means the selection has changed...
						if (selected != target::s_CurrentSkyboxSelection) {
							target::UpdateSkybox(selected);
						}
						
						ImGui::SliderFloat("Blur", &target::s_Blur, 0.0, 1.0);
						ImGui::DragFloat("Exposure", &target::s_Exposure, 0.001, 0.0, 65535.0);
						
				        ImGui::TreePop(); // END SKYBOX SECTION.
				    }
					
					/* MATERIAL */
				    if (ImGui::TreeNode("Material")) {
						
						using target = Settings::Graphics::Material;
						
						// Dropdown selection for shader:
						static int selected_shader = 0;
						
						ImGui::Combo("Shader", &selected_shader, target::s_AvailableShaders.data(), static_cast<int>(target::s_AvailableShaders.size()));
						
						// If the selected values mismatch, it means the selection has changed...
						if (selected_shader != target::s_CurrentShaderSelection) {
							target::UpdateShader(selected_shader);
						}
						
						if (ImGui::CollapsingHeader("Parameters")) {
							ImGui::DragFloat4("Texture Scale and Translate", &target::s_TextureScaleTranslate[0], 0.001);
						}
						
						if (ImGui::CollapsingHeader("Shadows")) {
							
							ImGui::Combo("Shadow Technique",  &target::s_CurrentShadowTechnique,           target::s_ShadowTechniques.data(),  static_cast<int>(target::s_ShadowTechniques.size()));
							ImGui::Combo("Shadow Resolution", &target::s_CurrentShadowResolutionSelection, target::s_ShadowResolutions.data(), static_cast<int>(target::s_ShadowResolutions.size()));
							
							ImGui::DragFloat(       "Bias", &target::s_ShadowBias,       0.01, 0.0, 65535.0);
							ImGui::DragFloat("Normal Bias", &target::s_ShadowNormalBias, 0.01, 0.0, 65535.0);
							
							// PCSS and Poisson-Disk:
							if (target::s_CurrentShadowTechnique == 2 ||
							    target::s_CurrentShadowTechnique == 3) {
								
								ImGui::DragInt("Shadow Samples", &target::s_ShadowSamples, 0.1, 0, 100);
							}
							
							// PCSS-only:
							if (target::s_CurrentShadowTechnique == 3) {
								ImGui::DragFloat("Light Size", &target::s_LightSize, 0.001, 0.0, 65535.0);
							}
							
							ImGui::Checkbox("Parallax Shadows", &target::s_ParallaxShadows);
						}
						
						if (ImGui::CollapsingHeader("Light")) {
							
							ImGui::Combo("Light TypeID", &target::s_CurrentLightType, target::s_AvailableLightTypes.data(), static_cast<int>(target::s_AvailableLightTypes.size()));
							
							ImGui::DragFloat3("Light Position",  &target::s_LightPosition[0], 0.1                 );
							ImGui::DragFloat3("Light Rotation",  &target::s_LightRotation[0], 0.1                 );
							ImGui::ColorEdit3("Light Color",     &target::s_LightColor[0]                         );
							ImGui::DragFloat ("Light Intensity", &target::s_LightIntensity,   0.01, 0.0, 65535.0);
							ImGui::DragFloat ("Light Range",     &target::s_LightRange,       0.1,  0.0, 65535.0);
							
							if (target::s_CurrentLightType == 2) {
								ImGui::DragFloat("Light Angle", &target::s_LightAngle, 0.1, 0.0, 180.0);
							}
						}
						
				        ImGui::TreePop(); // END MATERIAL SECTION.
				    }
					
					ImGui::End();
				}
			}

			static void SpatialSettingsWindow(const Window& _window, const bool& _draw) {
			
				if (_draw) {
					
					/*
					 * Set minimum size constraints as this window will
					 * auto-size and we don't want it to be too small.
					 */
					ImGui::SetNextWindowSizeConstraints(
						ImVec2(200.0, 0.0),
						ImVec2(
							std::numeric_limits<scalar_t>::infinity(),
							std::numeric_limits<scalar_t>::infinity()
						)
					);
					
					ImGui::SetNextWindowPos(ImVec2(static_cast<float>((s_WindowMargin.x * 2.0) + 450.0), s_WindowMargin.y), ImGuiCond_Once);
					ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
					
					using target = Settings::Spatial;
					
					/* SPATIAL SETTINGS */
					ImGui::Begin("Spatial", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			
					// Box for coordinates.
					ImGui::DragFloat3("Coords", &target::s_Coord[0]);
					
					// 2D array of coordinates
				    const std::array<std::pair<std::string, glm::vec2>, 11> places = {{
						{ "Hong Kong",   { 22.28180, 114.17347 }},
						{ "Bournemouth", { 50.74794, -1.87812  }},
						{ "Null Island", {  0.0,      0.0      }},
						{ "New York",    { 40.76956, -73.97380 }},
						{ "London",      { 51.5028,   -0.1105  }},
						{ "Tokyo",       { 35.69067, 139.81876 }},
						{ "Mombasa",     { -4.0513,   39.6658  }},
						{ "Cape Town",   {-33.9894,  18.5555   }},
						{ "Exmouth",     { 50.6304,   -3.4084  }},
						{ "Mt. Everest", { 27.98806, 86.92521  }},
					    { "Heathrow",    { 51.4678,  -0.4473   }}
				    }};
					
				    for (const auto& item : places) {
						
						const auto label = item.first;
						const auto coord = item.second;

						ImGui::BeginGroup();
				        ImGui::Text("%s:", label.c_str());
						ImGui::SameLine();
				        ImGui::Text("(%.5f, %.5f)", coord.x, coord.y);
						ImGui::EndGroup();
				    }
					
			        ImGui::Text("Press [R] to load new coordinates!");
					
					ImGui::End();
				}
			}
		};
		
		inline static bool s_DrawDebugWindows { false };
		
		/**
		 * @brief Initialises the GUI.
		 *
		 * This function is responsible for initialising the GUI using the ImGui library.
		 * It creates the ImGui context, initialises the ImGui backends for the specified window, and sets up the OpenGL bindings.
		 *
		 * @param[in] _window The weak pointer to the window object.
		 * @param[in] _glsl_version The GLSL version to use.
		 */
		static void Init(const Window& _window, const char* _glsl_version) {
			
			// Initialise IMGUI:
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			
			ImGui_ImplSDL2_InitForOpenGL(_window, _window.Context());
			ImGui_ImplOpenGL3_Init(_glsl_version);
		}
		
		/**
		 * @brief Set the GUI to a specific color palette.
		 *
		 * This function sets the GUI to a specific color palette based on the given style parameter.
		 *
		 * @param[in] _style The style parameter to set the GUI color palette.
		 */
		static void Style(const GUI::Parameters::Style& _style) {
		
			try {
				
				// Select a visual style:
				switch (_style) {
					
					case GUI::Parameters::Style::Light: { ImGui::StyleColorsLight(); break; }
					case GUI::Parameters::Style::Dark:  { ImGui::StyleColorsDark();  break; }
					default: {
						throw std::runtime_error("Unknown GUI style selection");
					}
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
		}
		
		/**
		 * @brief Processes a given SDL_Event.
		 *
		 * @param[in] _event The SDL_Event to process.
		 */
		static void ProcessEvent(const SDL_Event& _event) {
			ImGui_ImplSDL2_ProcessEvent(&_event);
		}
		
		/**
		* @brief Main GUI loop.
		* @param[in] _window The weak pointer to the window object.
		*/
		static void OnGUI(const std::shared_ptr<Window>& _window) {
		
			/* INIT GUI FRAME */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(*_window);
			
			ImGui::NewFrame();
			
			/* DEBUG MODE */
			{
				/*
				 * If s_DrawDebugWindows is true, unlock the cursor and make it visible.
				 * This will override any previous changes to the cursor's state.
				 */
				if (s_DrawDebugWindows) {
					
					// Make cursor visible.
					Input::Cursor::SetState({ Input::Cursor::State::LockMode::Absolute, true });
				}
				
				// Update (and optionally, draw) debugging windows:
				GUIWindows::    DiagnosticsWindow(*_window, s_DrawDebugWindows);
				GUIWindows:: PostProcessingWindow(*_window, s_DrawDebugWindows);
				GUIWindows:: RenderSettingsWindow(*_window, s_DrawDebugWindows);
				GUIWindows::SpatialSettingsWindow(*_window, s_DrawDebugWindows);
			}
			
			/* FINALIZE GUI FRAME */
			ImGui::Render();
			
	        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		/**
		 * @brief Dispose of the GUI.
		 *
		 * This function is responsible for disposing the GUI, freeing up any resources that were allocated during initialisation.
		 * It safely disposes of the IMGUI context, shuts down the ImGui backends, and destroys the context.
		 */
		static void Dispose() noexcept {
			
			// Dispose of the IMGUI context.
			try { ImGui_ImplOpenGL3_Shutdown(); } catch (const std::exception& e) {Debug::Log(e, LogType::Critical); }
			try { ImGui_ImplSDL2_Shutdown();    } catch (const std::exception& e) {Debug::Log(e, LogType::Critical); }
			try { ImGui::DestroyContext();      } catch (const std::exception& e) {Debug::Log(e, LogType::Critical); }
		}
		
	};
	
} // LouiEriksson::Engine::UI

#endif //FINALYEARPROJECT_GUI_HPP