#pragma once

#ifndef _STDAFX_H
#define _STDAFX_H

/* SDL & OPENGL */
#include "SDL2/SDL.h"
#include "GL/glew.h"

/* GLM */
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/projection.hpp"

/* EXTERNAL */
#include "wavefront/wavefront.h"
#include "cereal/archives/xml.hpp"

/* STL */
#include <any>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeindex>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

#endif // !_STDAFX_H
