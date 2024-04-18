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
        src/engine/scripts/audio/AudioClip.cpp
        src/engine/scripts/audio/AudioClip.h
        src/engine/scripts/audio/AudioListener.cpp
        src/engine/scripts/audio/AudioListener.h
        src/engine/scripts/audio/AudioSource.cpp
        src/engine/scripts/audio/AudioSource.h
        src/engine/scripts/audio/Sound.cpp
        src/engine/scripts/audio/Sound.h
        src/engine/scripts/core/Application.cpp
        src/engine/scripts/core/Application.h
        src/engine/scripts/core/Debug.cpp
        src/engine/scripts/core/Debug.h
        src/engine/scripts/core/File.cpp
        src/engine/scripts/core/File.h
        src/engine/scripts/core/Resources.cpp
        src/engine/scripts/core/Resources.h
        src/engine/scripts/core/Screen.cpp
        src/engine/scripts/core/Screen.h
        src/engine/scripts/core/Script.cpp
        src/engine/scripts/core/Script.h
        src/engine/scripts/core/Serialisation.cpp
        src/engine/scripts/core/Serialisation.h
        src/engine/scripts/core/Settings.cpp
        src/engine/scripts/core/Settings.h
        src/engine/scripts/core/Time.cpp
        src/engine/scripts/core/Time.h
        src/engine/scripts/core/Transform.cpp
        src/engine/scripts/core/Transform.h
        src/engine/scripts/core/utils/Hashmap.h
        src/engine/scripts/core/utils/Utils.cpp
        src/engine/scripts/core/utils/Utils.h
        src/engine/scripts/core/Window.cpp
        src/engine/scripts/core/Window.h
        src/engine/scripts/ecs/Component.cpp
        src/engine/scripts/ecs/Component.h
        src/engine/scripts/ecs/GameObject.cpp
        src/engine/scripts/ecs/GameObject.h
        src/engine/scripts/ecs/Scene.cpp
        src/engine/scripts/ecs/Scene.h
        src/engine/scripts/graphics/Camera.cpp
        src/engine/scripts/graphics/Camera.h
        src/engine/scripts/graphics/Light.cpp
        src/engine/scripts/graphics/Light.h
        src/engine/scripts/graphics/Material.cpp
        src/engine/scripts/graphics/Material.h
        src/engine/scripts/graphics/Mesh.cpp
        src/engine/scripts/graphics/Mesh.h
        src/engine/scripts/graphics/Renderer.cpp
        src/engine/scripts/graphics/Renderer.h
        src/engine/scripts/graphics/Shader.cpp
        src/engine/scripts/graphics/Shader.h
        src/engine/scripts/graphics/Texture.cpp
        src/engine/scripts/graphics/Texture.h
        src/engine/scripts/graphics/textures/Cubemap.cpp
        src/engine/scripts/graphics/textures/Cubemap.h
        src/engine/scripts/graphics/textures/RenderTexture.cpp
        src/engine/scripts/graphics/textures/RenderTexture.h
        src/engine/scripts/input/Cursor.cpp
        src/engine/scripts/input/Cursor.h
        src/engine/scripts/input/Input.cpp
        src/engine/scripts/input/Input.h
        src/engine/scripts/networking/Requests.cpp
        src/engine/scripts/networking/Requests.h
        src/engine/scripts/physics/Collider.cpp
        src/engine/scripts/physics/Collider.h
        src/engine/scripts/physics/colliders/PlaneCollider.cpp
        src/engine/scripts/physics/colliders/PlaneCollider.h
        src/engine/scripts/physics/colliders/SphereCollider.cpp
        src/engine/scripts/physics/colliders/SphereCollider.h
        src/engine/scripts/physics/Collision.cpp
        src/engine/scripts/physics/Collision.h
        src/engine/scripts/physics/Physics.cpp
        src/engine/scripts/physics/Physics.h
        src/engine/scripts/physics/Rigidbody.cpp
        src/engine/scripts/physics/Rigidbody.h
        src/engine/scripts/spatial/atmosphere/ISA.cpp
        src/engine/scripts/spatial/atmosphere/ISA.h
        src/engine/scripts/spatial/elevation/Elevation.cpp
        src/engine/scripts/spatial/elevation/Elevation.h
        src/engine/scripts/spatial/elevation/serialisation/ElevationDeserialiser.cpp
        src/engine/scripts/spatial/elevation/serialisation/ElevationDeserialiser.h
        src/engine/scripts/spatial/maths/Conversions.cpp
        src/engine/scripts/spatial/maths/Conversions.h
        src/engine/scripts/spatial/maths/Coords.cpp
        src/engine/scripts/spatial/maths/Coords.h
        src/engine/scripts/spatial/meshing/Builder.cpp
        src/engine/scripts/spatial/meshing/Builder.h
        src/engine/scripts/spatial/osm/OSM.cpp
        src/engine/scripts/spatial/osm/OSM.h
        src/engine/scripts/spatial/osm/serialisation/OSMDeserialiser.cpp
        src/engine/scripts/spatial/osm/serialisation/OSMDeserialiser.h
        src/engine/scripts/spatial/planets/VSOP.h
        src/engine/scripts/spatial/planets/WGCCRE.cpp
        src/engine/scripts/spatial/planets/WGCCRE.h
        src/engine/scripts/spatial/stars/ATHYG.cpp
        src/engine/scripts/spatial/stars/ATHYG.h
        src/engine/scripts/ui/GUI.cpp
        src/engine/scripts/ui/GUI.h
        src/engine/scripts/graphics/TextureCPU.cpp
        src/engine/scripts/graphics/TextureCPU.h
)

target_precompile_headers(FinalYearProject PRIVATE src/engine/scripts/stdafx.h)

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

        src/game/Injector.h
        src/game/scripts/Ball.cpp
        src/game/scripts/Ball.h
        src/game/scripts/FlyCam.cpp
        src/game/scripts/FlyCam.h
        src/game/scripts/Plane.cpp
        src/game/scripts/Plane.h
        src/game/scripts/Spatial/Planetarium.cpp
        src/game/scripts/Spatial/Planetarium.h
        src/game/scripts/Spatial/Map.cpp
        src/game/scripts/Spatial/Map.h
        src/game/scripts/Spatial/Stars.cpp
        src/game/scripts/Spatial/Stars.h
)

target_precompile_headers(Game PRIVATE
        src/game/include/engine_audio.h
        src/game/include/engine_core.h
        src/game/include/engine_ecs.h
        src/game/include/engine_graphics.h
        src/game/include/engine_input.h
        src/game/include/engine_physics.h
        src/game/include/engine_spatial.h
        src/game/include/engine_ui.h
)

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