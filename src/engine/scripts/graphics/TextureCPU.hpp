#ifndef FINALYEARPROJECT_TEXTURECPU_HPP
#define FINALYEARPROJECT_TEXTURECPU_HPP

namespace LouiEriksson::Engine::Graphics {
	
	template<typename T, glm::length_t L = 4>
	class TextureCPU final {
		
	private:
		
		std::vector<glm::vec<L, T>> m_Pixels;
		
		size_t m_Width,
		       m_Height;
		
		static constexpr void assert_color() {
			static_assert(is_valid_color(), "T must be an arithmetic type");
		}
		
		static constexpr bool is_valid_color() noexcept {
		    return std::is_arithmetic_v<T>;
		};
		
		[[nodiscard]] size_t GetIndex(glm::vec<2, size_t> _pixel) const {
			
			const auto index = Utils::To1D(
				{
					 std::clamp(               static_cast<size_t>(_pixel.x), static_cast<size_t>(0),  Width()),
					 std::clamp(Height() - 1 - static_cast<size_t>(_pixel.y), static_cast<size_t>(0), Height()),
				},
				Width()
			);
			
			return std::clamp(index, static_cast<size_t>(0), m_Pixels.size() - 1);
		}
		
	public:
		
		TextureCPU(const std::vector<glm::vec<L, T>>& _pixels, const size_t& _width, const size_t& _height) :
			m_Pixels (_pixels),
			m_Width  (std::max(_width,  static_cast<size_t>(1U))),
			m_Height (std::max(_height, static_cast<size_t>(1U)))
		{
			assert_color();
			
			if (m_Pixels.empty()) {
				
				Debug::Log("Texture initialised with no pixels!", LogType::Warning);
				
				m_Pixels.emplace_back(1.0);
				m_Width  = 1U;
				m_Height = 1U;
			}
			else if (m_Width * m_Height != m_Pixels.size()) {
				
				Debug::Log("The width and height of the texture does not match the length of the provided buffer!", LogType::Warning);
				
				m_Pixels.resize(m_Width * m_Height);
			}
		}
		
		[[nodiscard]] size_t Width()  const noexcept { return m_Width;  }
		[[nodiscard]] size_t Height() const noexcept { return m_Height; }
		
		[[nodiscard]] const std::vector<glm::vec<L, T>>& Pixels() const noexcept {
			return m_Pixels;
		}
		
		[[nodiscard]] glm::vec<L, T> GetPixel(const glm::vec<2, size_t>& _pixel) const {
			return m_Pixels[GetIndex(_pixel)];
		}
		
		template<typename Y = scalar_t, glm::qualifier V = glm::defaultp>
		[[nodiscard]] glm::vec<L, T> GetPixelBilinear(const glm::vec<2, Y, V>& _uv) const {
			
			static_assert(std::is_floating_point<Y>::value, "Y must be a floating point type");
			
			const glm::vec<2, size_t> p(
				(_uv.x * static_cast<Y>( Width() - 1)),
				(_uv.y * static_cast<Y>(Height() - 1))
			);
			
			const glm::vec<2, Y, V> d(glm::fract(_uv.x), glm::fract(_uv.y));
			
			glm::vec<L, T> result{};
			
			for (size_t i = 0U; i < L; ++i) {
				
				const auto A = GetPixel({ p.x    , p.y     });
				const auto B = GetPixel({ p.x + 1, p.y     });
				const auto C = GetPixel({ p.x    , p.y + 1 });
				const auto D = GetPixel({ p.x + 1, p.y + 1 });
				
				result += glm::mix(glm::mix(A, B, d.x), glm::mix(C, D, d.x), d.y);
			}
			
			result /= static_cast<T>(L);
			
			return result;
		}
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_TEXTURECPU_HPP