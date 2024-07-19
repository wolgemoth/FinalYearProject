#ifndef FINALYEARPROJECT_GRAPHICS_HPP
#define FINALYEARPROJECT_GRAPHICS_HPP

#include "IGraphics.hpp"

namespace LouiEriksson::Engine {

    class Application;

} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {

    class Graphics {

        friend Application;

    private:

        static IGraphics m_API;

    public:

        static const void SetGraphicsAPI(const IGraphics& _api) {
            m_API = _api;
        }

        static const IGraphics& GetGraphicsAPI() {
            return m_API;
        }
    };

} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_GRAPHICS_HPP