#ifndef FINALYEARPROJECT_STDAFX_H
#define FINALYEARPROJECT_STDAFX_H

/* EXTERNAL */
#include <AL/al.h>                  // AL ...
#include <AL/alc.h>
#include <AL/alext.h>

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

#include <SDL.h>                    // SDL2

/* INTERNAL */
#include "core/utils/Hashmap.h"

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
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_set>
#include <utility>
#include <vector>

#endif //FINALYEARPROJECT_STDAFX_H