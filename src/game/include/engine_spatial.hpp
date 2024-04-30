#ifndef FINALYEARPROJECT_ENGINE_SPATIAL_HPP
#define FINALYEARPROJECT_ENGINE_SPATIAL_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-include-cleaner"

#include "../../engine/scripts/spatial/atmosphere/ISA.hpp"
#include "../../engine/scripts/spatial/elevation/Elevation.hpp"
#include "../../engine/scripts/spatial/elevation/serialisation/ElevationDeserialiser.hpp"
#include "../../engine/scripts/spatial/maths/Conversions.hpp"
#include "../../engine/scripts/spatial/maths/Coords.hpp"
#include "../../engine/scripts/spatial/meshing/Builder.hpp"
#include "../../engine/scripts/spatial/opensky/OpenSky.hpp"
#include "../../engine/scripts/spatial/osm/OSM.hpp"
#include "../../engine/scripts/spatial/osm/serialisation/OSMDeserialiser.hpp"
#include "../../engine/scripts/spatial/planets/VSOP.hpp"
#include "../../engine/scripts/spatial/planets/WGCCRE.hpp"
#include "../../engine/scripts/spatial/stars/ATHYG.hpp"

#pragma clang diagnostic pop
#pragma clang diagnostic pop

#endif //FINALYEARPROJECT_ENGINE_SPATIAL_HPP