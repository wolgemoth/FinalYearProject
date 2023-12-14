#ifndef FINALYEARPROJECT_STDAFX_H
#define FINALYEARPROJECT_STDAFX_H

/* EXTERNAL */

#include <SDL.h>                    // SDL2
#include <GL/glew.h>                // OPENGL

#include <imgui.h>                  // IMGUI ...
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>              // GLM ...
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>

#include "cereal/archives/xml.hpp"  // CEREAL

/* INTERNAL */
#include "Hashmap.h"

/* STL */
#include <any>
#include <regex>
#include <queue>
#include <array>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeindex>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <filesystem>
#include <functional>

#endif //FINALYEARPROJECT_STDAFX_H