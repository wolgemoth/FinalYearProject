#include "Conversions.h"

#include <glm/common.hpp>

#include <cmath>
#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine::Spatial::Maths {
	
	Hashmap<std::string, Conversions::Speed::Unit>::optional_ref Conversions::Speed::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Speed::Convert(const double& _val, const Conversions::Speed::Unit& _from, const Conversions::Speed::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Speed::Symbol(const Conversions::Speed::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Distance::Unit>::optional_ref Conversions::Distance::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Distance::Convert(const double& _val, const Conversions::Distance::Unit& _from, const Conversions::Distance::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Distance::Symbol(const Conversions::Distance::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	double Conversions::Distance::ArcSecondsToMetres(const double& _arcSeconds, const double& _lat) {
		return _arcSeconds * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) * (1852.0 / 60.0));
	}
	
	double Conversions::Distance::MetresToArcSeconds(const double& _metres, const double& _lat) {
		return _metres * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) / (1852.0 / 60.0));;
	}
	
	Hashmap<std::string, Conversions::Rotation::Unit>::optional_ref Conversions::Rotation::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Rotation::Convert(const double& _val, const Conversions::Rotation::Unit& _from, const Conversions::Rotation::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Rotation::Symbol(const Conversions::Rotation::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Time::Unit>::optional_ref Conversions::Time::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Time::Convert(const double& _val, const Conversions::Time::Unit& _from, const Conversions::Time::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Time::Symbol(const Conversions::Time::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Temperature::Unit>::optional_ref Conversions::Temperature::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Temperature::Convert(const double& _val, const Conversions::Temperature::Unit& _from, const Conversions::Temperature::Unit& _to) {

		double result;
		
		// Convert to Kelvin:
		switch (_from) {
			case Celsius:    { result =  _val - 272.15;        break; }
			case Fahrenheit: { result = (_val + 459.67) / 1.8; break; }
			case Kelvin:     { result = _val;                  break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
		
		// Clamp result above absolute zero.
		result = std::max(result, s_AbsoluteZero);
		
		// Convert Kelvin to target:
		switch (_to) {
			case Celsius:    { result += 273.15;                 break; }
			case Fahrenheit: { result = (result * 1.8) - 459.67; break; }
			case Kelvin:     {                                   break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
		
		return result;
	}
	
	std::string Conversions::Temperature::Symbol(const Conversions::Temperature::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	double Conversions::Temperature::ClampTemperature(const double& _val, Conversions::Temperature::Unit& _unit) {
		return Convert(
			std::min(
				Convert(_val, _unit, Kelvin),
				s_PlanckTemperature
			),
			Kelvin,
			_unit
		);
	}
	
	Hashmap<std::string, Conversions::Pressure::Unit>::optional_ref Conversions::Pressure::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Pressure::Convert(const double& _val, const Conversions::Pressure::Unit& _from, const Conversions::Pressure::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Pressure::Symbol(const Conversions::Pressure::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Mass::Unit>::optional_ref Conversions::Mass::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Mass::Convert(const double& _val, const Conversions::Mass::Unit& _from, const Conversions::Mass::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Mass::Symbol(const Conversions::Mass::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Area::Unit>::optional_ref Conversions::Area::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Area::Convert(const double& _val, const Conversions::Area::Unit& _from, const Conversions::Area::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Area::Symbol(const Conversions::Area::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	Hashmap<std::string, Conversions::Volume::Unit>::optional_ref Conversions::Volume::TryGuessUnit(const std::string& _symbol) {
		return s_Lookup.Get(_symbol);
	}
	
	double Conversions::Volume::Convert(const double& _val, const Conversions::Volume::Unit& _from, const Conversions::Volume::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Volume::Symbol(const Conversions::Volume::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
} // LouiEriksson::Engine::Spatial::Maths