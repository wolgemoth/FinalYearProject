cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(FinalYearProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto -DNDEBUG")

add_executable(FinalYearProject

        src/engine/scripts/stdafx.h

        src/engine/scripts/main.cpp

        src/engine/scripts/Application.cpp
        src/engine/scripts/Application.h

        src/engine/scripts/Ball.cpp
        src/engine/scripts/Ball.h

        src/engine/scripts/Camera.cpp
        src/engine/scripts/Camera.h

        src/engine/scripts/Collider.cpp
        src/engine/scripts/Collider.h

        src/engine/scripts/Colliders.h

        src/engine/scripts/Collision.cpp
        src/engine/scripts/Collision.h

        src/engine/scripts/Component.cpp
        src/engine/scripts/Component.h

        src/engine/scripts/Components.h

        src/engine/scripts/Entity.cpp
        src/engine/scripts/Entity.h

        src/engine/scripts/File.cpp
        src/engine/scripts/File.h

        src/engine/scripts/GameObject.cpp
        src/engine/scripts/GameObject.h

        src/engine/scripts/Hashmap.h

        src/engine/scripts/Input.cpp
        src/engine/scripts/Input.h

        src/engine/scripts/Light.cpp
        src/engine/scripts/Light.h

        src/engine/scripts/Material.cpp
        src/engine/scripts/Material.h

        src/engine/scripts/Mesh.cpp
        src/engine/scripts/Mesh.h

        src/engine/scripts/OrbitCam.cpp
        src/engine/scripts/OrbitCam.h

        src/engine/scripts/Plane.cpp
        src/engine/scripts/Plane.h

        src/engine/scripts/PlaneCollider.cpp
        src/engine/scripts/PlaneCollider.h

        src/engine/scripts/Player.cpp
        src/engine/scripts/Player.h

        src/engine/scripts/RenderTexture.cpp
        src/engine/scripts/RenderTexture.h

        src/engine/scripts/Renderer.cpp
        src/engine/scripts/Renderer.h

        src/engine/scripts/Rigidbody.cpp
        src/engine/scripts/Rigidbody.h

        src/engine/scripts/Scene.cpp
        src/engine/scripts/Scene.h

        src/engine/scripts/Script.cpp
        src/engine/scripts/Script.h

        src/engine/scripts/Scripts.h

        src/engine/scripts/Serialisation.cpp
        src/engine/scripts/Serialisation.h

        src/engine/scripts/Shader.cpp
        src/engine/scripts/Shader.h

        src/engine/scripts/SphereCollider.cpp
        src/engine/scripts/SphereCollider.h

        src/engine/scripts/Time.cpp
        src/engine/scripts/Time.h

        src/engine/scripts/Transform.cpp
        src/engine/scripts/Transform.h

        src/engine/scripts/Window.cpp
        src/engine/scripts/Window.h

        src/engine/scripts/Texture.cpp
        src/engine/scripts/Texture.h

        src/engine/scripts/Cubemap.cpp
        src/engine/scripts/Cubemap.h

        src/engine/scripts/Resources.cpp
        src/engine/scripts/Resources.h

        src/engine/scripts/Utils.cpp
        src/engine/scripts/Utils.h

        src/engine/scripts/GUI.cpp
        src/engine/scripts/GUI.h

        src/engine/scripts/Settings.cpp
        src/engine/scripts/Settings.h
)

set(SDL2_PATH        ${PROJECT_SOURCE_DIR}/external/SDL2)
set(SDL2_INCLUDE_DIR ${SDL2_PATH}/include)
set(SDL2_LIB_DIR     ${SDL2_PATH}/lib/x86/)

set(GLEW_PATH        ${PROJECT_SOURCE_DIR}/external/glew)
set(GLEW_INCLUDE_DIR ${GLEW_PATH}/include)
set(GLEW_LIB_DIR     ${GLEW_PATH}/lib)

set(IMGUI_PATH          ${PROJECT_SOURCE_DIR}/external/imgui)
set(IMGUI_BACKENDS_PATH ${PROJECT_SOURCE_DIR}/external/imgui/backends)

include_directories(
        ${PROJECT_SOURCE_DIR}/external/cereal/include
        ${GLEW_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/external/glm
        ${SDL2_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/external/wavefront/include/
        ${PROJECT_SOURCE_DIR}/external/stb/include/
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS_PATH}
)

add_library(IMGUI STATIC
        ${IMGUI_PATH}/imgui.cpp                         # MAIN ...
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp

        ${IMGUI_BACKENDS_PATH}/imgui_impl_sdl2.cpp      # BACKENDS ...
        ${IMGUI_BACKENDS_PATH}/imgui_impl_opengl3.cpp
)

link_directories(FinalYearProject
        ${GLEW_LIB_DIR}
        ${SDL2_LIB_DIR}
)

target_link_libraries(FinalYearProject
        GLEW
        GL
        GLU
        SDL2main
        SDL2
        IMGUI
)

set(   LEVELS_PATH ${PROJECT_SOURCE_DIR}/src/engine/levels   )
set(   MODELS_PATH ${PROJECT_SOURCE_DIR}/src/engine/models   )
set(MATERIALS_PATH ${PROJECT_SOURCE_DIR}/src/engine/materials)
set(  SHADERS_PATH ${PROJECT_SOURCE_DIR}/src/engine/shaders  )
set( TEXTURES_PATH ${PROJECT_SOURCE_DIR}/src/engine/textures )

file(COPY    ${LEVELS_PATH} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${MODELS_PATH} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${MATERIALS_PATH} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY   ${SHADERS_PATH} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY  ${TEXTURES_PATH} DESTINATION ${PROJECT_BINARY_DIR})

target_precompile_headers(FinalYearProject PRIVATE src/engine/scripts/stdafx.h)