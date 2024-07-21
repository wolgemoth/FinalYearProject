# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-build"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/tmp"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-stamp"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src"
  "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ubuntu/Projects/FinalYearProject/contrib/openal-soft/src/openal-soft-stamp${cfgdir}") # cfgdir has leading slash
endif()
