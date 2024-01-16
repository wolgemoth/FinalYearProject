#include "GUI.h"

#include "../core/Settings.h"
#include "../core/Time.h"
#include "../core/Window.h"
#include "../input/Cursor.h"
#include "../input/Input.h"
#include "../core/utils/Utils.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <glm/common.hpp>
#include <glm/ext/vector_float2.hpp>
#include <SDL_scancode.h>
#include <SDL_video.h>

#include <iomanip>
#include <ios>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

namespace LouiEriksson::Engine::UI {
	
	void GUI::Init(const std::weak_ptr<Window>& _window, const char* _glsl_version) {
		
		// Initialise IMGUI:
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		if (const auto w = _window.lock()) {
			ImGui_ImplSDL2_InitForOpenGL(w->operator SDL_Window *(), w->Context());
		}
		
		ImGui_ImplOpenGL3_Init(_glsl_version);
	}
	
	void GUI::Style(const GUI::Parameters::Style& _style) {
		
		// Select a visual style:
		switch (_style) {
			
			case GUI::Parameters::Style::Light: { ImGui::StyleColorsLight(); break; }
			case GUI::Parameters::Style::Dark:  { ImGui::StyleColorsDark();  break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
	}
	
	void GUI::ProcessEvent(const SDL_Event& _event) {
		ImGui_ImplSDL2_ProcessEvent(&_event);
	}
	
	void GUI::OnGUI(const std::weak_ptr<Window>& _window) {
		
		/* INIT GUI FRAME */
		ImGui_ImplOpenGL3_NewFrame();
		
		if (const auto w = _window.lock()) {
			ImGui_ImplSDL2_NewFrame(w->operator SDL_Window *());
		}
		
		ImGui::NewFrame();
		
		/* DEBUG MODE */
		{
			// Use the '~' key (on ANSI keyboard layouts or '`' key on UK layout) to enter debug mode.
			if (Input::Input::Key::GetDown(SDL_SCANCODE_GRAVE)) {
				s_DrawDebugWindows = !s_DrawDebugWindows;
			}
			
			/*
			 * If s_DrawDebugWindows is true, unlock the cursor and make it visible.
			 * This will override any previous changes to the cursor's state.
			 */
			if (s_DrawDebugWindows) {
				
				// Make cursor visible.
				Input::Cursor::SetState({ Input::Cursor::State::LockMode::Absolute, true });
			}
			
			// Update (and optionally, draw) debugging windows:
			GUIWindows::DiagnosticsWindow(_window, s_DrawDebugWindows);
			GUIWindows::PostProcessingWindow(_window, s_DrawDebugWindows);
			GUIWindows::RenderSettingsWindow(_window, s_DrawDebugWindows);
		}
		/* FINALIZE GUI FRAME */
		ImGui::Render();
		
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void GUI::Dispose() {
		
		// Safely dispose of the IMGUI context.
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		
		ImGui::DestroyContext();
	}
	
	void GUI::GUIWindows::DiagnosticsWindow(const std::weak_ptr<Window>& _window, const bool& _draw) {
		
		// Initialise FPS sampling window.
		static std::vector<float> s_Timestamps;
		static std::vector<float> s_Samples;
		
		static const auto s_Plot_SamplingWindowSize = 10.0f;
		static const auto  s_FPS_SamplingWindowSize =  0.5f;
		
		// Append the current frame.
		s_Timestamps.emplace_back(Time::Elapsed());
		   s_Samples.emplace_back(1.0f / Time::UnscaledDeltaTime());
		   
		// Extract various values from the sampling window:
		float oldest_avg_timestamp  = std::numeric_limits<float>::infinity(),
			  oldest_plot_timestamp = std::numeric_limits<float>::infinity(),
			  min_fps               = std::numeric_limits<float>::infinity(),
		      max_fps               = 0.0f,
		      avg_fps               = 0.0f,
		      avg_fps_count         = 0.0f;
		
		for (int i = 0; i < s_Samples.size(); ++i) {
			
			const auto timestamp = s_Timestamps[i];
			auto fps = s_Samples[i];
			
			if (timestamp > 0.0f) {
				
				if (timestamp < Time::Elapsed() - s_Plot_SamplingWindowSize) {
					
					s_Timestamps.erase(s_Timestamps.begin() + i);
					   s_Samples.erase(   s_Samples.begin() + i);
				}
				else {
					
					oldest_plot_timestamp = glm::min(timestamp, oldest_plot_timestamp);
					
					min_fps = glm::min(fps, min_fps);
					max_fps = glm::max(fps, max_fps);
					
					if (timestamp > Time::Elapsed() - s_FPS_SamplingWindowSize) {
					
						oldest_avg_timestamp = glm::min(timestamp, oldest_avg_timestamp);
						
						avg_fps += fps;
						avg_fps_count++;
					}
				}
			}
		}
		
		avg_fps /= glm::max(avg_fps_count, 1.0f);
		
		// Diagnostics window:
		if (_draw) {
			
			// Set default values (on first run):
			if (const auto w = _window.lock()) {
				
	            const auto screenSize   = glm::vec2(w->Dimensions());
				const auto windowSize   = ImVec2(300, 200);
				
				ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
				ImGui::SetNextWindowPos(ImVec2(screenSize.x - windowSize.x - s_WindowMargin.x, s_WindowMargin.y), ImGuiCond_Once);
				ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
			}
			
			// Create dynamically changing window title using the ### operator:
			{
				std::stringstream title;
				title << std::fixed << std::setprecision(2)
				      << "Diagnostics ("
					  <<           avg_fps << " fps, "
					  << 1000.0f / avg_fps << " ms)###Diagnostics";
				
				// Draw elements:
				ImGui::Begin(title.str().c_str(), nullptr);
			}
			
			// Perform set up for rendering the plot:
			const float plot_vMargin = 15.0f;
		
			auto cursor = ImGui::GetCursorPos();
		
			ImGui::SetCursorPosY(cursor.y + plot_vMargin);
		
			auto plot_cursor = ImGui::GetCursorPos();
			auto plot_size   = ImGui::GetContentRegionAvail();
			
			plot_size.y -= plot_vMargin;
			
			const auto bottom = plot_cursor.y + plot_size.y;
			const auto right  = plot_cursor.x + plot_size.x;
			const auto hMargin = 5.0f;
			
			const auto range = max_fps - min_fps;
			
			// Draw the main plot:
			{
				// Make the main plot's lines slightly more transparent.
				auto plotLinesCol = ImGui::GetStyleColorVec4(ImGuiCol_PlotLines);
				plotLinesCol.w *= 0.7f;
				
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
			const std::vector<float> fps_ticks = {
				   1.0f,   3.0f,   6.0f,  12.0f,  24.0f,   30.0f,
				  50.0f,  60.0f,  72.0f,  90.0f, 100.0f,  120.0f,
				 144.0f, 240.0f, 360.0f, 480.0f, 500.0f, 1000.0f,
				2000.0f
			};
			
			// Make plot backgrounds transparent and match their 'hovered' color to their normal color.
		    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
			
			// Draw vertical lines for time plots.
			{
				const float t_interval = s_Plot_SamplingWindowSize / 4.0f;
				
				float t = Time::Elapsed();
				
				while (t > oldest_plot_timestamp) {
					
					// Compute the x offset of the line.
					const auto x_offset = Utils::Remap(t, oldest_plot_timestamp, Time::Elapsed(), 0.0f, plot_size.x);
					
					// Draw the line:
					{
						ImGui::SetCursorPos(plot_cursor);
						auto cursor_screenpos = ImGui::GetCursorScreenPos();
						
						auto startPos = ImVec2(cursor_screenpos.x + x_offset, cursor_screenpos.y);
						auto   endPos = ImVec2(startPos.x, startPos.y + plot_size.y);
						
						ImGui::GetWindowDrawList()->AddLine(startPos, endPos, ImGui::GetColorU32(ImGuiCol_PlotLines));
					}
					
					// Add a label:
					{
						// Format the label string in advance and use this to calculate its size.
						std::stringstream label;
						label << std::fixed << std::setprecision(1)
						      << "-" << Time::Elapsed() - t << " s";
						
						auto textSize = ImGui::CalcTextSize(label.str().c_str());
						
					    ImGui::SetCursorPosX(plot_cursor.x + x_offset - textSize.x - hMargin);
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
						glm::min(
							glm::abs(val - max_fps),
							glm::abs(val - min_fps)
						) / range * plot_size.y > 20.0f
					) {
						
						// Draw the line:
						{
					        ImGui::SetCursorPos(plot_cursor);
						
							float tick[] = { val, val };
							ImGui::PlotLines("", tick, 2, 0, nullptr, min_fps, max_fps, plot_size);
						}
						
						// Add a label:
						{
						    ImGui::SetCursorPosX(plot_cursor.x);
						    ImGui::SetCursorPosY(plot_cursor.y + Utils::Remap(val, min_fps, max_fps, plot_size.y, 0.0f));
							
					        ImGui::Text("%.1f fps", val);
						}
					}
				}
			}
		
			// Do average fps:
			{
				ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLinesHovered
				
				// Green styling for average fps line.
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
				
				// Draw a line for the average fps:
				{
					ImGui::SetCursorPos(plot_cursor);
					
					float tick[] = { avg_fps, avg_fps };
				    ImGui::PlotLines("", tick, 2, 0, nullptr, min_fps, max_fps, plot_size);
				}
				
				ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLinesHovered
				ImGui::PopStyleColor(); // Reset ImGuiCol_PlotLines
				ImGui::PopStyleColor(); // Reset ImGuiCol_FrameBg
				
				// Add a label:
				{
					// Format the label string in advance and use this to calculate its size.
					std::stringstream label;
					label << std::fixed << std::setprecision(1) << avg_fps << " fps";
					
					auto textSize = ImGui::CalcTextSize(label.str().c_str());
					
					// Draw the label, using its size to correctly align it on the screen.
					ImGui::SetCursorPosX(right - textSize.x - hMargin);
				    ImGui::SetCursorPosY(plot_cursor.y + Utils::Remap(avg_fps, min_fps, max_fps, plot_size.y, 0.0f));
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
	
	void GUI::GUIWindows::PostProcessingWindow(const std::weak_ptr<Window>& _window, const bool& _draw) {
		
		if (_draw) {
			
			/*
			 * Set minimum size constraints as this window will
			 * auto-size and we don't want it to be too small.
			 */
			ImGui::SetNextWindowSizeConstraints(
				ImVec2(200.0f, 0.0f),
				ImVec2(
					std::numeric_limits<float>::infinity(),
					std::numeric_limits<float>::infinity()
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
						
						ImGui::SliderFloat("Intensity", &target::s_Intensity, 0.0f, 6.0f);
						ImGui::SliderFloat("Radius",    &target::s_Radius,    0.0f, 2.0f);
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
						
						ImGui::DragFloat("Intensity", &target::s_Intensity, 0.001f, 0.0f, 65535.0f);
						ImGui::DragFloat("Threshold", &target::s_Threshold, 0.001f, 0.0f, 65535.0f);
						ImGui::DragFloat("Clamp",     &target::s_Clamp,     0.001f, 0.0f, 65535.0f);
						ImGui::DragFloat("Lens Dirt", &target::s_LensDirt,  0.001f, 0.0f, 65535.0f);
						
						ImGui::SliderFloat("Anamorphism", &target::s_Anamorphism, -1.0f,  1.0f);
						ImGui::SliderFloat("Diffusion",   &target::s_Diffusion,    0.0f, 10.0f);
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
						
						ImGui::DragFloat("Gain",     &target::s_Gain,     0.001f);
						ImGui::DragFloat("Exposure", &target::s_Exposure, 0.001f);
						
						/* AUTO-EXPOSURE */
						ImGui::Checkbox("Auto Exposure", &target_autoExposure::s_Enabled);
						
					    if (target_autoExposure::s_Enabled) {
							
					        ImGui::TextWrapped("Automatically adjusts the overall exposure of the image towards a designated exposure target");
							
							ImGui::SliderFloat("Min EV", &target_autoExposure::s_MinEV, 0.0f, 9.0f);
							ImGui::SliderFloat("Max EV", &target_autoExposure::s_MaxEV, 0.0f, 9.0f);
							
							ImGui::DragFloat("Compensation", &target_autoExposure::s_Compensation, 0.001f);
							
							ImGui::SliderFloat("Speed Down", &target_autoExposure::s_SpeedDown, 0.0f, 10.0f);
							ImGui::SliderFloat("Speed Up",   &target_autoExposure::s_SpeedUp,   0.0f, 10.0f);
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
						
						ImGui::SliderFloat("Contrast Threshold",    &target::s_ContrastThreshold, 0.0312f, 0.0833f);
						ImGui::SliderFloat("Relative Threshold",    &target::s_RelativeThreshold, 0.063f,  0.333f);
						ImGui::SliderFloat("Subpixel Blending",     &target::s_SubpixelBlending,  0.0f,    1.0f);
						ImGui::SliderFloat("Edge Blending",         &target::s_EdgeBlending,      0.0f,    1.0f);
						
						ImGui::DragFloat("Local Contrast Modifier", &target::s_LocalContrastModifier, 0.001f);
						
						target::s_LocalContrastModifier = glm::max(target::s_LocalContrastModifier, 0.0f);
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
						ImGui::SliderFloat("Intensity", &target::s_Intensity, 0.0f, 1.0f);
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
						ImGui::SliderFloat("Intensity",  &target::s_Intensity,  0.0f, 1.0f);
						ImGui::SliderFloat("Smoothness", &target::s_Smoothness, 0.0f, 1.0f);
				    }
					
			        ImGui::TreePop(); // END VIGNETTE SECTION.
			    }
		    }
			
			ImGui::End();
		}
	}
	
	void GUI::GUIWindows::RenderSettingsWindow(const std::weak_ptr<Window>& _window, const bool& _draw) {
	
		if (_draw) {
			
			/*
			 * Set minimum size constraints as this window will
			 * auto-size and we don't want it to be too small.
			 */
			ImGui::SetNextWindowSizeConstraints(
				ImVec2(200.0f, 0.0f),
				ImVec2(
					std::numeric_limits<float>::infinity(),
					std::numeric_limits<float>::infinity()
				)
			);
			
			ImGui::SetNextWindowPos(ImVec2((s_WindowMargin.x * 2.0f) + 200.0f, s_WindowMargin.y), ImGuiCond_Once);
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
				
				ImGui::SliderFloat("FOV", &target_perspective::s_FOV, 0.005f, 180.0f);
				ImGui::DragFloat("NearClip", &target_perspective::s_NearClip, 0.1f, 0.005f, 65535.0f);
				ImGui::DragFloat("FarClip", &target_perspective::s_FarClip, 0.1f, target_perspective::s_NearClip, 65535.0f);
				
				// Toggle for orbit.
			    ImGui::Checkbox("Orbit", &target_perspective::Orbit::s_Enabled);
				
			    if (target_perspective::Orbit::s_Enabled) {
					
					ImGui::DragFloat3("Speed",  &target_perspective::Orbit::s_Speed [0], 0.001f, -65535.0f, 65535.0f);
					ImGui::DragFloat3("Amount", &target_perspective::Orbit::s_Amount[0], 0.001f, -65535.0f, 65535.0f);
					ImGui::DragFloat3("Offset", &target_perspective::Orbit::s_Offset[0], 0.001f, -65535.0f, 65535.0f);
			    }
				
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
				
				ImGui::SliderFloat("Blur", &target::s_Blur, 0.0f, 1.0f);
				ImGui::DragFloat("Exposure", &target::s_Exposure, 0.001f, 0.0f, 65535.0f);
				
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
				
					ImGui::DragFloat(   "Roughness", &target::s_RoughnessAmount,     0.001f, 0.0f, 65535.0f);
					ImGui::DragFloat("Displacement", &target::s_DisplacementAmount, 0.0001f, 0.0f, 65535.0f);
					ImGui::DragFloat(     "Normals", &target::s_NormalAmount,        0.001f, 0.0f, 65535.0f);
					ImGui::DragFloat(    "Emission", &target::s_EmissionAmount,      0.001f, 0.0f, 65535.0f);
					ImGui::DragFloat(          "AO", &target::s_AOAmount,            0.001f, 0.0f, 65535.0f);
					
					ImGui::DragFloat4("Texture Scale and Translate", &target::s_TextureScaleTranslate[0], 0.001f);
				}
				
				if (ImGui::CollapsingHeader("Shadows")) {
					
					ImGui::Combo("Shadow Technique",  &target::s_CurrentShadowTechnique,           target::s_ShadowTechniques.data(),  static_cast<int>(target::s_ShadowTechniques.size()));
					ImGui::Combo("Shadow Resolution", &target::s_CurrentShadowResolutionSelection, target::s_ShadowResolutions.data(), static_cast<int>(target::s_ShadowResolutions.size()));
					
					ImGui::DragFloat(       "Bias", &target::s_ShadowBias,       0.01f, 0.0f, 65535.0f);
					ImGui::DragFloat("Normal Bias", &target::s_ShadowNormalBias, 0.01f, 0.0f, 65535.0f);
					
					// PCSS and Poisson-Disk:
					if (target::s_CurrentShadowTechnique == 2 ||
					    target::s_CurrentShadowTechnique == 3) {
						
						ImGui::DragInt("Shadow Samples", &target::s_ShadowSamples, 0.1f, 0, 100);
					}
					
					// PCSS-only:
					if (target::s_CurrentShadowTechnique == 3) {
						ImGui::DragFloat("Light Size", &target::s_LightSize, 0.001f, 0.0f, 65535.0f);
					}
					
					ImGui::Checkbox("Parallax Shadows", &target::s_ParallaxShadows);
				}
				
				if (ImGui::CollapsingHeader("Light")) {
					
					ImGui::Combo("Light TypeID", &target::s_CurrentLightType, target::s_AvailableLightTypes.data(), static_cast<int>(target::s_AvailableLightTypes.size()));
					
					ImGui::DragFloat3("Light Position",  &target::s_LightPosition[0], 0.1f                 );
					ImGui::DragFloat3("Light Rotation",  &target::s_LightRotation[0], 0.1f                 );
					ImGui::ColorEdit3("Light Color",     &target::s_LightColor[0]                          );
					ImGui::DragFloat ("Light Intensity", &target::s_LightIntensity,   0.01f, 0.0f, 65535.0f);
					ImGui::DragFloat ("Light Range",     &target::s_LightRange,       0.1f,  0.0f, 65535.0f);
					
					if (target::s_CurrentLightType == 2) {
						ImGui::DragFloat("Light Angle", &target::s_LightAngle, 0.1f, 0.0f, 180.0f);
					}
				}
				
		        ImGui::TreePop(); // END MATERIAL SECTION.
		    }
			
			ImGui::End();
		}
	}

} // LouiEriksson::Engine::UI