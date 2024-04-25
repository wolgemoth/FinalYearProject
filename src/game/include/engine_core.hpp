#ifndef GAME_INCLUDE_ENGINE_CORE_HPP
#define GAME_INCLUDE_ENGINE_CORE_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-include-cleaner"

#include <glm/common.hpp>   // GLM ...
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>

#include "../../engine/scripts/core/Application.hpp"
#include "../../engine/scripts/core/Debug.hpp"
#include "../../engine/scripts/core/File.hpp"
#include "../../engine/scripts/core/IViewport.hpp"
#include "../../engine/scripts/core/Resources.hpp"
#include "../../engine/scripts/core/Script.hpp"
#include "../../engine/scripts/core/Serialisation.hpp"
#include "../../engine/scripts/core/Settings.hpp"
#include "../../engine/scripts/core/Time.hpp"
#include "../../engine/scripts/core/Transform.hpp"
#include "../../engine/scripts/core/Types.hpp"
#include "../../engine/scripts/core/utils/Hashmap.hpp"
#include "../../engine/scripts/core/utils/ThreadUtils.hpp"
#include "../../engine/scripts/core/utils/Utils.hpp"
#include "../../engine/scripts/core/Window.hpp"

#pragma clang diagnostic pop
#pragma clang diagnostic pop

#endif //GAME_INCLUDE_ENGINE_CORE_HPP