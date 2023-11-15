#pragma once

#ifndef FINALYEARPROJECT_STDAFX_H
#define FINALYEARPROJECT_STDAFX_H

/* SDL & OPENGL */
#include <SDL2/SDL.h>
#include <GL/glew.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>

/* EXTERNAL */
#include "stb_image.h"
#include "cereal/archives/xml.hpp"

/* INTERNAL */
#include "Hashmap.h"

/* STL */
#include <any>
#include <regex>
#include <queue>
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