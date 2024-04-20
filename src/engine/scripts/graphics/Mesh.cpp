#include "Mesh.h"

#include "../core/utils/Utils.h"
#include "../core/Debug.h"
#include "../core/Resources.h"

#include "TextureCPU.h"

#include <GL/glew.h>
#include <glm/geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int2.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	std::weak_ptr<Mesh> Mesh::Primitives::Sphere::Instance() {
		return Resources::Get<Mesh>("sphere");
	}
	
	std::weak_ptr<Mesh> Mesh::Primitives::Cube::Instance() {
		return Resources::Get<Mesh>("cube");
	}
	
} // LouiEriksson::Engine::Graphics