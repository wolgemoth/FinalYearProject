#include "TextureCPU.h"

#include "../core/Debug.h"
#include "../core/utils/Utils.h"

#include <glm/common.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	TextureCPU::TextureCPU(const std::vector<float>& _pixels, const size_t& _width, const size_t& _height) :
		m_Pixels (_pixels),
		m_Width  (std::max(_width,  static_cast<size_t>(1))),
		m_Height (std::max(_height, static_cast<size_t>(1)))
	{
		if (m_Pixels.empty()) {
			
			Debug::Log("Texture initialised with no pixels!", LogType::Warning);
			
			m_Pixels.emplace_back(1.0);
			m_Width  = 1;
			m_Height = 1;
		}
		else if (m_Width * m_Height != m_Pixels.size()) {
			
			Debug::Log("The width and height of the texture does not match the length of the provided buffer!", LogType::Warning);
			
			m_Pixels.resize(m_Width * m_Height);
		}
	}
	
	size_t TextureCPU::Width()  const noexcept { return m_Width;  }
	size_t TextureCPU::Height() const noexcept { return m_Height; }
	
	size_t TextureCPU::GetIndex(glm::ivec2 _pixel) const {
		
		const auto index = Utils::To1D(
			{
				 std::clamp(static_cast<size_t>(               _pixel.x), static_cast<size_t>(0),  Width()),
				 std::clamp(static_cast<size_t>(Height() - 1 - _pixel.y), static_cast<size_t>(0), Height()),
			},
			Width()
		);
		
		return std::clamp(index, static_cast<size_t>(0), m_Pixels.size() - 1);
	}
	
	float TextureCPU::GetPixel(const glm::ivec2& _pixel) const {
		return m_Pixels[GetIndex(_pixel)];
	}
	
	float TextureCPU:: GetPixelBilinear(const glm::vec2& _uv) const {
		
		const glm::ivec2 p {
			(_uv.x * static_cast<float>( Width() - 1)),
			(_uv.y * static_cast<float>(Height() - 1))
		};
		
		const glm::vec2 d {
			glm::fract(_uv.x),
			glm::fract(_uv.y)
		};
		
		const auto A = GetPixel({ p.x    , p.y     });
		const auto B = GetPixel({ p.x + 1, p.y     });
		const auto C = GetPixel({ p.x    , p.y + 1 });
		const auto D = GetPixel({ p.x + 1, p.y + 1 });
		
		return glm::mix(glm::mix(A, B, d.x), glm::mix(C, D, d.x), d.y);
	}
	
	const std::vector<float>& TextureCPU::Pixels() const noexcept {
		return m_Pixels;
	}
	
} // LouiEriksson::Engine::Graphics