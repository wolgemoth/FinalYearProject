cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(FinalYearProject)

set(CMAKE_CXX_STANDARD 17)

# DETECT ARCHITECTURE FOR SIMD OPTIMISATION
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86|AMD64)$")
    set(ARCH_SSE2_FLAGS "-msse2")
    set(ARCH_AVX_FLAGS "-mavx")
    set(ARCH_AVX2_FLAGS "-mavx2")
    set(ARCH_AVX512_FLAGS "-mavx512f -mavx512dq -mavx512bw -mavx512vl")
endif()

# APPEND ARCHITECTURE-SPECIFIC FLAGS FOR SIMD OPTIMISATION
if(ARCH_SSE2_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${ARCH_SSE2_FLAGS}")
endif()
if(ARCH_AVX_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${ARCH_AVX_FLAGS}")
endif()
if(ARCH_AVX2_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${ARCH_AVX2_FLAGS}")
endif()
if(ARCH_AVX512_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} ${ARCH_AVX512_FLAGS}")
endif()

# DIFFERENT BUILD MODES:
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address,leak,undefined -Wall -Wextra -fno-omit-frame-pointer -g -O0")

set(CMAKE_CXX_FLAGS_NOSANITIZERS "-Wall -Wextra -fno-omit-frame-pointer -g -O0")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -funroll-loops -DNDEBUG")

# ADD IMGUI:
set(IMGUI_DIR          ${PROJECT_SOURCE_DIR}/contrib/imgui)
set(IMGUI_BACKENDS_DIR ${PROJECT_SOURCE_DIR}/contrib/imgui/backends)

add_library(IMGUI STATIC
        ${IMGUI_DIR}/imgui.cpp                         # MAIN ...
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp      # BACKENDS ...
)

# FIND PACKAGES:
find_package(GLEW   REQUIRED)
find_package(SDL2   REQUIRED)
find_package(Bullet REQUIRED)
find_package(OpenAL REQUIRED)
find_package(CURL   REQUIRED)
find_package(assimp REQUIRED)

# ADD JSON:
set(JSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/nlohmann/)

# ADD MAPBOX:
set(MAPBOX_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/mapbox/)

# ADD VSOP87:
set(VSOP87_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/greg-miller/vsop87/)

add_library(VSOP87 STATIC
        ${VSOP87_INCLUDE_DIR}/vsop87a_full.cpp
        ${VSOP87_INCLUDE_DIR}/vsop87a_full.h
)

# LINK:
include_directories(
        ${CURL_INCLUDE_DIRS}
        ${IMGUI_DIR}
        ${JSON_INCLUDE_DIR}
        ${OPENAL_INCLUDE_DIR}
        ${BULLET_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/contrib/cereal/include
        ${PROJECT_SOURCE_DIR}/contrib/glm
        ${PROJECT_SOURCE_DIR}/contrib/stb/include
        ${SDL2_INCLUDE_DIRS}
        ${VSOP87_INCLUDE_DIR}
        ${MAPBOX_INCLUDE_DIR}
)

# ADD FINALYEARPROJECT:
add_library(FinalYearProject STATIC
        src/engine/scripts/audio/AudioClip.hpp
        src/engine/scripts/audio/AudioSource.hpp
        src/engine/scripts/audio/Sound.hpp
        src/engine/scripts/core/Application.hpp
        src/engine/scripts/core/Debug.hpp
        src/engine/scripts/core/File.hpp
        src/engine/scripts/core/Resources.hpp
        src/engine/scripts/core/Script.hpp
        src/engine/scripts/core/Serialisation.hpp
        src/engine/scripts/core/Settings.hpp
        src/engine/scripts/core/Time.hpp
        src/engine/scripts/core/Transform.hpp
        src/engine/scripts/core/utils/Hashmap.hpp
        src/engine/scripts/core/utils/Utils.hpp
        src/engine/scripts/core/Window.hpp
        src/engine/scripts/ecs/Component.hpp
        src/engine/scripts/ecs/GameObject.hpp
        src/engine/scripts/ecs/Scene.hpp
        src/engine/scripts/graphics/Camera.hpp
        src/engine/scripts/graphics/Light.hpp
        src/engine/scripts/graphics/Material.hpp
        src/engine/scripts/graphics/Mesh.hpp
        src/engine/scripts/graphics/Renderer.hpp
        src/engine/scripts/graphics/Shader.hpp
        src/engine/scripts/graphics/Texture.hpp
        src/engine/scripts/graphics/textures/Cubemap.hpp
        src/engine/scripts/graphics/textures/RenderTexture.hpp
        src/engine/scripts/input/Cursor.hpp
        src/engine/scripts/input/Input.hpp
        src/engine/scripts/networking/Requests.hpp
        src/engine/scripts/physics/Collider.hpp
        src/engine/scripts/physics/colliders/PlaneCollider.hpp
        src/engine/scripts/physics/colliders/SphereCollider.hpp
        src/engine/scripts/physics/Collision.hpp
        src/engine/scripts/physics/Physics.hpp
        src/engine/scripts/physics/Rigidbody.hpp
        src/engine/scripts/spatial/atmosphere/ISA.hpp
        src/engine/scripts/spatial/elevation/Elevation.hpp
        src/engine/scripts/spatial/elevation/serialisation/ElevationDeserialiser.hpp
        src/engine/scripts/spatial/maths/Conversions.hpp
        src/engine/scripts/spatial/maths/Coords.hpp
        src/engine/scripts/spatial/meshing/Builder.hpp
        src/engine/scripts/spatial/osm/OSM.hpp
        src/engine/scripts/spatial/osm/serialisation/OSMDeserialiser.hpp
        src/engine/scripts/spatial/planets/VSOP.hpp
        src/engine/scripts/spatial/planets/WGCCRE.hpp
        src/engine/scripts/spatial/stars/ATHYG.hpp
        src/engine/scripts/ui/GUI.hpp
        src/engine/scripts/graphics/TextureCPU.hpp
        src/engine/scripts/core/Types.hpp
        src/engine/scripts/core/IViewport.hpp
        src/game/include/engine_networking.hpp
)

#target_precompile_headers(FinalYearProject PRIVATE src/engine/scripts/stdafx.hpp)

# Set libs to compile with release flags and no sanitizers or rtti.
set(LIB_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

string(STRIP "${LIB_FLAGS}" LIB_FLAGS)
string(REPLACE " " ";" LIB_FLAGS "${LIB_FLAGS}")

target_compile_options(IMGUI  PRIVATE ${LIB_FLAGS})
target_compile_options(VSOP87 PRIVATE ${LIB_FLAGS})

# LINK LIBS:
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(FinalYearProject PRIVATE -flto=auto -Wl,--strip-all -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
else()
    target_link_options(FinalYearProject PRIVATE -flto=auto -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
endif()

target_link_libraries(FinalYearProject

        # CORE:
        assimp::assimp
        CURL::libcurl
        GL
        GLEW
        IMGUI
        OpenAL::OpenAL
        ${BULLET_LIBRARIES}
        ${SDL2_LIBRARIES}

        # SPATIAL:
        VSOP87
)

# ADD GAME:
add_executable(Game
        src/main.cpp

        src/game/Injector.hpp
        src/game/scripts/Ball.hpp
        src/game/scripts/FlyCam.hpp
        src/game/scripts/Plane.hpp
        src/game/scripts/Spatial/Planetarium.hpp
        src/game/scripts/Spatial/Map.hpp
        src/game/scripts/Spatial/Stars.hpp
)

#target_precompile_headers(Game PRIVATE
#        src/game/include/engine_audio.hpp
#        src/game/include/engine_core.hpp
#        src/game/include/engine_ecs.hpp
#        src/game/include/engine_graphics.hpp
#        src/game/include/engine_input.hpp
#        src/game/include/engine_physics.hpp
#        src/game/include/engine_spatial.hpp
#        src/game/include/engine_ui.hpp
#)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(Game PRIVATE  -flto=auto -Wl,--strip-all -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
else()
    target_link_options(Game PRIVATE -flto=auto -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
endif()

target_link_libraries(Game PRIVATE FinalYearProject)

# COPY FILES:
set(    AUDIO_DIR ${PROJECT_SOURCE_DIR}/src/engine/audio    )
set(   LEVELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/levels   )
set(   MODELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/models   )
set(  SHADERS_DIR ${PROJECT_SOURCE_DIR}/src/engine/shaders  )
set( TEXTURES_DIR ${PROJECT_SOURCE_DIR}/src/engine/textures )
set(MATERIALS_DIR ${PROJECT_SOURCE_DIR}/src/engine/materials)
set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/src/game/resources  )

file(COPY     ${AUDIO_DIR} DESTINATION ${PROJECT_BINARY_DIR}/assets/)
file(COPY    ${LEVELS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${MODELS_DIR} DESTINATION ${PROJECT_BINARY_DIR}/assets/)
file(COPY   ${SHADERS_DIR} DESTINATION ${PROJECT_BINARY_DIR}/assets/)
file(COPY  ${TEXTURES_DIR} DESTINATION ${PROJECT_BINARY_DIR}/assets/)
file(COPY ${MATERIALS_DIR} DESTINATION ${PROJECT_BINARY_DIR}/assets/)
file(COPY ${RESOURCES_DIR} DESTINATION ${PROJECT_BINARY_DIR})