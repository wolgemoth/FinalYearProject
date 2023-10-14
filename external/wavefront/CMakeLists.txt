cmake_minimum_required(VERSION 3.0)
project(WAVEFRONT)

include_directories(
  include
  src/stent
)

if(MSVC)
  include_directories(contrib/windows/include)
endif()

add_library(wavefront
  src/wavefront/Model.c
)

if(NOT MSVC)
  add_executable(example
    src/example/main.cpp
  )
  target_link_libraries(example SDL2 GLEW GL wavefront)
endif()

