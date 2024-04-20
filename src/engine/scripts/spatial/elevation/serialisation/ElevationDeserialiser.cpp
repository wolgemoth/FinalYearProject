#include "ElevationDeserialiser.h"

#include "../../../core/utils/Utils.h"

#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Spatial::Serialisation {

	ElevationDeserialiser::OTDJSON::OTDJSON(json&& _otd_json) :
		m_Root(std::move(_otd_json)) {}
	
	ElevationDeserialiser::OTDJSON::Root::Root(json&& _otd_json) :
		results()
	{
		auto jResults = _otd_json["results"];
		
		results.reserve(jResults.size());
		for (auto& item : jResults) {
			results.emplace_back(std::move(item));
		}
	}
	
	ElevationDeserialiser::OTDJSON::Root::Root(std::vector<Result>&& _results) :
		results(_results) {}
	
	ElevationDeserialiser::OTDJSON::Root::Result::Result(json&& _otd_json) :
		  dataset(Utils::As<std::string>(_otd_json["dataset"])),
		elevation(_otd_json["elevation"]),
		 location(std::move(_otd_json["location"])) {}
	
	ElevationDeserialiser::OTDJSON::Root::Result::Location::Location(json&& _otd_json) :
		lat(_otd_json["lat"]),
		lng(_otd_json["lng"]) {}
	
	ElevationDeserialiser::OEJSON::OEJSON(json&& _oe_json) :
		m_Root(std::move(_oe_json)) {}
	
	ElevationDeserialiser::OEJSON::Root::Root(json&& _oe_json) :
		results()
	{
		auto jResults = _oe_json["results"];
		
		results.reserve(jResults.size());
		for (auto& item : jResults) {
			results.emplace_back(std::move(item));
		}
	}
	
	ElevationDeserialiser::OEJSON::Root::Result::Result(json&& _oe_json) :
		 latitude(_oe_json["latitude" ]),
		longitude(_oe_json["longitude"]),
		elevation(_oe_json["elevation"]) {}
	
} // LouiEriksson::Engine::Spatial::Serialisation