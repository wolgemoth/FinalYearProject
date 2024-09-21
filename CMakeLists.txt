cmake_minimum_required(VERSION ${CMAKE_VERSION})

project(FinalYearProject VERSION 0.1.0 LANGUAGES CXX)

# Add an optional tag to the end of the version.
set(FYP_VERSION_METADATA "-alpha")
if (FYP_VERSION_METADATA)
    set(FYP_VERSION "${FYP_VERSION_MAJOR}.${FYP_VERSION_MINOR}.${FYP_VERSION_PATCH}${FYP_VERSION_METADATA}")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(FYP_Version.hpp.in FYP_Version.hpp)

# DETECT ARCHITECTURE FOR SIMD OPTIMISATION:

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[0-9]86|x86_64|AMD64)$") # x86 Processors
    set(SIMD_FLAGS
        -mmmx       # <- MMX Flags
        -m3dnow     # <- 3DNOW
        -msse       # <- SSE Flags
        -msse2      # <- SSE2 Flags
        -msse3      # <- SSE3 Flags
        -mssse3     # <- Supplemental SSE3 Flags
        -msse4.1    # <- SSE4.1 Flags
        -msse4.2    # <- SSE4.2 Flags
        -mavx       # <- AVX Flags
        -mavx2      # <- AVX2 Flags
        #-mavx512f -mavx512dq -mavx512bw -mavx512vl  # <- AVX-512 Flags
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^ppc64le") #PowerPC Processors
    set(SIMD_FLAGS
        -maltivec   # <- AltiVec/VMX Flags
        -mvsx       # <- VSX Flags
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm") #ARM Processors
    set(SIMD_FLAGS
        -mfpu=neon  # <- NEON Flags
        -march=armv8-a+crc+crypto
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^aarch64") #ARM64 Processors
    set(SIMD_FLAGS
        -march=armv8-a  # <- needed to have NEON support
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^sparc") #SPARC Processors
    set(SIMD_FLAGS
        -mv8
        -mvis   # <- Visual Instruction Set Flags
        -mvis2
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^mips") #MIPS Processors
    set(SIMD_FLAGS
        -mdspr2 # <- MIPS DSPr2 Flag
        -mmmx   # <- MMX Flags
        -msse   # <- SSE Flags
        -msse2  # <- SSE2 Flags
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^ia64") #Itanium Processors
    set(SIMD_FLAGS
        -march=itanium-2
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(s390|s390x)$") # IBM S/390 and z Series Processors
    set(SIMD_FLAGS
        -march=z13
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^riscv") #RISC-V Processors
    set(SIMD_FLAGS
        -march=rv64imafdc -mabi=lp64d
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^alpha") #DEC Alpha Processors
    set(SIMD_FLAGS
        -mieee
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^hppa") #HP PA-RISC Processors
    set(SIMD_FLAGS
        -mschedule=8000
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(68000|68040|68060)$") #Motorola 68k Processors
    set(SIMD_FLAGS
        -m68060
    )

elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(sh4|sh4a)$") #SuperH Processors
    set(SIMD_FLAGS
        -m4a
    )

else()
    set(SIMD_FLAGS "")
endif()

add_compile_options(${SIMD_FLAGS})

# DIFFERENT BUILD MODES:
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address,leak,undefined -Wall -Wextra -fno-omit-frame-pointer -g -O0")

set(CMAKE_CXX_FLAGS_NOSANITIZERS "-Wall -Wextra -fno-omit-frame-pointer -g -O0")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native -funroll-loops -DNDEBUG")

# IMGUI:
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

# GLEW:
find_package(GLEW)
if (NOT GLEW_FOUND)

    # Define the path to the GLEW directory
    set(GLEW_DIR "${PROJECT_SOURCE_DIR}/contrib/glew")

    # Looking for GLEW header files
    find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        PATHS ${GLEW_DIR}/include
        NO_DEFAULT_PATH           # Do not search other directories
    )

    if(WIN32)

        # Support for both 32 and 64 bit builds:
        if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(GLEW_LIBRARY_DIR ${GLEW_ROOT_DIR}/lib/Release/x64)
        else()
            set(GLEW_LIBRARY_DIR ${GLEW_ROOT_DIR}/lib/Release/Win32)
        endif()

    else()

        # This might be the case for Unix-based systems
        set(GLEW_LIBRARY_DIR ${GLEW_ROOT_DIR}/lib)

    endif()

    # Construct the full file path to the GLEW library:
    find_library(GLEW_LIBRARY
        NAMES GLEW glew32 glew glew32.lib libglew32.lib libglew32.a
        HINTS ${GLEW_LIBRARY_DIR}
    )

    if (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
        message(STATUS "Found GLEW")
        include_directories(${GLEW_INCLUDE_DIR})
    else()
        if (NOT GLEW_INCLUDE_DIR)
            message(STATUS "GLEW_INCLUDE_DIR not found")
        endif()
        if (NOT GLEW_LIBRARY)
            message(STATUS "GLEW_LIBRARY not found")
        endif()
    endif()
endif()

# SDL2:
find_package(SDL2)
if (NOT SDL2_FOUND)

    set(SDL2_PATH ${PROJECT_SOURCE_DIR}/contrib/SDL2)
    set(SDL2_INCLUDE_DIRS ${SDL2_PATH}/include)

    if(WIN32)

        # Support for both 32 and 64 bit builds:
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(SDL2_LIBRARIES ${SDL2_PATH}/win32-x64/SDL2.dll)
        else()
            set(SDL2_LIBRARIES ${SDL2_PATH}/win32-x86/SDL2.dll)
        endif()

    elseif(APPLE)

        set(SDL2_DIR "${SDL2_PATH}/macos/SDL2.framework")
        find_package(SDL2 REQUIRED COMPONENTS SDL2)

    else() # Assuming Linux / Unix environment:

        # Support for both 32 and 64 bit builds:
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_LIBRARIES
                    ${SDL2_PATH}/linux/lib64/libSDL2.a
                    ${SDL2_PATH}/linux/lib64/libSDL2-2.0.so
            )
        else ()
            set(SDL2_LIBRARIES
                ${SDL2_PATH}/linux/lib/libSDL2.a
                ${SDL2_PATH}/linux/lib/libSDL2-2.0.so
            )
        endif()
    endif()
endif()

# BULLET:
find_package(Bullet REQUIRED)

# OPEN-AL:
find_package(OpenAL)
if (NOT OpenAL_FOUND)

    # See if OpenAL has been downloaded into the contrib folder:
    set(OpenAL_DIR "${CMAKE_SOURCE_DIR}/contrib/openal-soft/lib/cmake/OpenAL")
    find_package(OpenAL NO_DEFAULT_PATH)

    if (NOT OpenAL_FOUND)

        include(ExternalProject)

        set(OpenAL_DOWNLOAD_VERSION 1.23.1)

        # Download OpenAL-Soft.
        ExternalProject_Add(openal-soft
                GIT_REPOSITORY https://github.com/kcat/openal-soft.git
                GIT_TAG ${OpenAL_DOWNLOAD_VERSION}
                PREFIX ${CMAKE_SOURCE_DIR}/contrib/openal-soft
                CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_SOURCE_DIR}/contrib/openal-soft
        )
    endif()
endif()

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

# Find source files:
file(GLOB_RECURSE FINALYEARPROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/engine/scripts/*.hpp")

# Find precompiled headers:
file(GLOB_RECURSE FINALYEARPROJECT_PCH
        "${CMAKE_SOURCE_DIR}/src/engine/**/stdafx.h"
        "${CMAKE_SOURCE_DIR}/src/engine/**/pch.h"
)

# Exclude precompiled headers from the target source, as referring to them directly is bad design.
foreach(PCH_FILE ${FINALYEARPROJECT_PCH})
  list(REMOVE_ITEM FINALYEARPROJECT_SOURCES ${PCH_FILE})
endforeach()

# Define FinalYearProject as a lib.
add_library(FinalYearProject STATIC ${FINALYEARPROJECT_SOURCES})

# If OpenAL is not found, build the downloaded version.
if (NOT OpenAL_FOUND)
    add_dependencies(FinalYearProject openal-soft)
    find_package(OpenAL REQUIRED)
endif()

# Specify precompiled headers:
target_precompile_headers(FinalYearProject PRIVATE ${FINALYEARPROJECT_PCH})

# Set libs to compile with release flags and no sanitizers or rtti.
set(LIB_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

string(STRIP "${LIB_FLAGS}" LIB_FLAGS)
string(REPLACE " " ";" LIB_FLAGS "${LIB_FLAGS}")

target_compile_options(IMGUI  PRIVATE ${LIB_FLAGS})
target_compile_options(VSOP87 PRIVATE ${LIB_FLAGS})

# LINK LIBS:

# LINK OPTIONS:
if(UNIX)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_options(FinalYearProject PRIVATE -flto=auto -Wl,--strip-all -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
    else()
        target_link_options(FinalYearProject PRIVATE -flto=auto -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
    endif()
endif(UNIX)

target_link_libraries(FinalYearProject
        ${BULLET_LIBRARIES}
        ${SDL2_LIBRARIES}
        assimp::assimp
        CURL::libcurl
        GL
        GLEW
        IMGUI
        OpenAL::OpenAL
        VSOP87
)

# ADD GAME:
file(GLOB_RECURSE GAME_SOURCES
        "${CMAKE_SOURCE_DIR}/src/game/scripts/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/game/scripts/*.h"
        "${CMAKE_SOURCE_DIR}/src/game/scripts/*.hpp"
)
add_executable(Game
        ${GAME_SOURCES}

        "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

# PRECOMPILE INCLUDED HEADERS FOR GAME
file(GLOB_RECURSE GAME_PCH
        "${CMAKE_SOURCE_DIR}/src/game/**/stdafx.h"
        "${CMAKE_SOURCE_DIR}/src/game/**/pch.h"

        "${CMAKE_SOURCE_DIR}/src/game/include/*.h"
        "${CMAKE_SOURCE_DIR}/src/game/include/*.hpp"
)
target_precompile_headers(Game PRIVATE ${GAME_PCH})

# LINK OPTIONS:
if(UNIX)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_options(Game PRIVATE  -flto=auto -Wl,--strip-all -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
    else()
        target_link_options(Game PRIVATE -flto=auto -Wl,--gc-sections -Wl,--as-needed -Wl,--no-undefined)
    endif()
endif(UNIX)

# Link FinalYearProject to Game
target_link_libraries(Game PRIVATE FinalYearProject)

# Copy Assets to the Binary location:
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