#ifndef FINALYEARPROJECT_IVIEWPORT_HPP
#define FINALYEARPROJECT_IVIEWPORT_HPP

namespace LouiEriksson::Engine {
	
	template <typename T, typename U>
	struct IViewport {
		
		[[nodiscard]] virtual inline glm::vec<2, U> Dimensions() const = 0;
	
		[[nodiscard]] T Aspect() const {
			const auto dimensions = Dimensions();
		
			return static_cast<T>(dimensions[0]) / static_cast<T>(dimensions[1]);
		}
		
		[[nodiscard]] virtual bool IsDirty() const = 0;
	};
	
}

#endif //FINALYEARPROJECT_IVIEWPORT_HPP