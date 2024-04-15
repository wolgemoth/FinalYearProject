#ifndef FINALYEARPROJECT_STDAFX_H
#define FINALYEARPROJECT_STDAFX_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-include-cleaner"

/* EXTERNAL */
#include <al.h>                     // AL ...
#include <alc.h>
#include <alext.h>

#include <btBulletDynamicsCommon.h> // BULLET

#include <cereal/archives/xml.hpp>  // CEREAL

#include <GL/glew.h>                // OPENGL

#include <glm/glm.hpp>              // GLM ...
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>

#include <imgui.h>                  // IMGUI ...
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>

#include <vsop87a_full.h>           // VSOP87

#include <SDL.h>                    // SDL2

#include <curl/curl.h>              // CURL

/* INTERNAL */

#define HASHMAP_SUPPRESS_EXCEPTION_WARNING
#include "core/utils/Hashmap.h"

#include "core/Debug.h"

/* STL */
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <typeindex>
#include <unordered_set>
#include <utility>
#include <vector>

#pragma clang diagnostic pop

#endif //FINALYEARPROJECT_STDAFX_H