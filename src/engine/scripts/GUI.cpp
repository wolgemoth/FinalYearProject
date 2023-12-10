#include "stdafx.h"

#include "GUI.h"

namespace LouiEriksson {
	
	void GUI::Init(const std::weak_ptr<Window> _window, const char* _glsl_version) {
		
		// Initialise IMGUI:
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGui_ImplSDL2_InitForOpenGL(_window.lock()->operator SDL_Window *(), _window.lock()->Context());
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
	
	void GUI::OnGUI(const std::weak_ptr<Window> _window) {
		
		/* INIT GUI FRAME */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window.lock()->operator SDL_Window *());
		ImGui::NewFrame();
		
		s_Timestamps.push_back(Time::Elapsed());
		   s_Samples.push_back(1.0f / Time::DeltaTime());
		
		float oldest_avg_timestamp  = std::numeric_limits<float>::infinity(),
			  oldest_plot_timestamp = std::numeric_limits<float>::infinity(),
			  min_fps               = std::numeric_limits<float>::infinity(),
		      max_fps               = 0.0f,
		      avg_fps               = 0.0f,
		      avg_fps_count         = 0.0f;
		
		for (int i = 0; i < s_Samples.size(); ++i) {
			
			float timestamp = s_Timestamps[i],
			            fps =    s_Samples[i];
			
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
		{
			// Set default values (on first run):
			ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2(16, 16), ImGuiCond_Once);
			ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
			
			std::stringstream title;
			title << "Diagnostics ("
				  << std::fixed << std::setprecision(2) << avg_fps << " fps, "
				  << std::fixed << std::setprecision(2) << 1000.0f / avg_fps << " ms)###Diagnostics";
			
			// Draw elements:
			ImGui::Begin(title.str().c_str(), nullptr);
			
				auto cursor   = ImGui::GetCursorPos();
				auto plotSize = ImGui::GetContentRegionAvail();
				
				float max = max_fps,
				      min = min_fps,
					range = max - min;
				
				ImGui::PlotLines(
					"",
					s_Samples.data(),
					s_Samples.size(),
					1,
					nullptr,
					min,
					max,
					plotSize
				);
				
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
				std::vector<float> fps_ticks = {
					   1.0f,   3.0f,   6.0f,  12.0f,  24.0f,   30.0f,
					  50.0f,  60.0f,  72.0f,  90.0f, 100.0f,  120.0f,
					 144.0f, 240.0f, 360.0f, 480.0f, 500.0f, 1000.0f,
					2000.0f
				};
				
				// Make plots transparent.
			    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
				
				for (int i = 0; i < fps_ticks.size(); i++) {
					
					auto val = fps_ticks[i];
					
					if (val < max_fps && val > min_fps) {
							
					    ImGui::SetCursorPos(cursor);
						
						float tick[] = { val, val };
				
						ImGui::PlotLines("", tick, 2, 0, nullptr, min, max, plotSize);
						
						// Help to prevent congestion with too many labels,
						// by omitting the rendering of labels fewer than
						// 20 pixels distance from the max and min fps values.
						if (
							glm::min(
								glm::abs(val - max),
								glm::abs(val - min)
							) / range * plotSize.y > 20.0f
						) {
							
						    ImGui::SetCursorPosX(cursor.x);
						    ImGui::SetCursorPosY(cursor.y + Utils::Remap(val, min, max, plotSize.y, 0.0f));
							
					        ImGui::Text("%.1f fps", val);
						}
					}
				}
				
				ImGui::PopStyleColor();
				ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImGui::GetStyleColorVec4(ImGuiCol_PlotLines));
				
			    ImGui::SetCursorPos(cursor);
				
				float tick[] = { avg_fps, avg_fps };
		
			    ImGui::PlotLines("", tick, 2, 0, nullptr, min, max, plotSize);
				
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				
				// Label the average fps.
				ImGui::SetCursorPosX(cursor.x + plotSize.x - 70.0f);
			    ImGui::SetCursorPosY(cursor.y + Utils::Remap(avg_fps, min, max, plotSize.y, 0.0f));
		        ImGui::Text("%.1f fps", avg_fps);
				
				// Label the min fps.
				ImGui::SetCursorPosX(cursor.x);
			    ImGui::SetCursorPosY(cursor.y + plotSize.y - 15.0f);
		        ImGui::Text("%.1f fps", min);
				
				// Label the max fps.
			    ImGui::SetCursorPos(cursor);
		        ImGui::Text("%.1f fps", max);
				
				//ImGui::Text("%.1f s", Time::Elapsed() - oldest_plot_timestamp);
				
			ImGui::End();
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
	
}