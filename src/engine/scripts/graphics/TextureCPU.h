//
// Created by ubuntu on 13/04/24.
//

#ifndef FINALYEARPROJECT_TEXTURECPU_H
#define FINALYEARPROJECT_TEXTURECPU_H

namespace LouiEriksson::Engine::Graphics {
	
	class TextureCPU final {
		
	private:
		
		std::vector<float> m_Pixels;
		
		size_t m_Width,
		       m_Height;
		
		[[nodiscard]] size_t GetIndex(glm::ivec2 _pixel) const;
		
	public:
		
		[[nodiscard]] size_t  Width() const noexcept;
		[[nodiscard]] size_t Height() const noexcept;
		
		[[nodiscard]] float GetPixel        (const glm::ivec2& _pixel) const;
		[[nodiscard]] float GetPixelBilinear(const glm:: vec2& _uv   ) const;
		
		[[nodiscard]] const std::vector<float>& Pixels() const noexcept;
		
		TextureCPU(const std::vector<float>& _pixels, const size_t& _width, const size_t& _height);
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_TEXTURECPU_H