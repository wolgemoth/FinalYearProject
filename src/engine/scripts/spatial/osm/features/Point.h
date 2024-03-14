#ifndef FINALYEARPROJECT_POINT_H
#define FINALYEARPROJECT_POINT_H

#include <glm/ext/vector_float3.hpp>

namespace LouiEriksson::Spatial::OSM {

	struct Point {

		// Node m_Node
		
        glm::vec3 m_Coord;
		
		explicit Point(const glm::vec3& _coord) : m_Coord(_coord) {}
		
		Point            (const Point& other) = default;
		Point& operator= (const Point& other) = default;
	};
	
} // LouiEriksson::Spatial::OSM

#endif //FINALYEARPROJECT_POINT_H