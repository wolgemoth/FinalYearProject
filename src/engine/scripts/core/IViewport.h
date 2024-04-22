#ifndef FINALYEARPROJECT_IVIEWPORT_H
#define FINALYEARPROJECT_IVIEWPORT_H

namespace LouiEriksson::Engine {
	
	template <typename T, typename U>
	struct IViewport {
		
		virtual inline glm::vec<2, U> Dimensions() const = 0;
	
		[[nodiscard]] inline T Aspect() const {
			const auto dimensions = Dimensions();
		
			return static_cast<T>(dimensions[0]) / static_cast<T>(dimensions[1]);
		}
		
		virtual bool IsDirty() const = 0;
	};
	
}

#endif //FINALYEARPROJECT_IVIEWPORT_H