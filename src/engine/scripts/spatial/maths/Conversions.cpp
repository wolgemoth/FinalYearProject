#include "Conversions.h"

#include <glm/common.hpp>

#include <cmath>
#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine::Spatial::Maths {
	
	bool Conversions::Speed::TryGuessUnit(const std::string& _symbol, Conversions::Speed::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Speed::Convert(const double& _val, const Conversions::Speed::Unit& _from, const Conversions::Speed::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Speed::Symbol(const Conversions::Speed::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Distance::TryGuessUnit(const std::string& _symbol, Conversions::Distance::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Distance::Convert(const double& _val, const Conversions::Distance::Unit& _from, const Conversions::Distance::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Distance::Symbol(const Conversions::Distance::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	double Conversions::Distance::ArcSecondsToMetres(const double& _arcSeconds, const double& _lat) {
		return _arcSeconds * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) * (1852.0f / 60.0f));
	}
	
	double Conversions::Distance::MetresToArcSeconds(const double& _metres, const double& _lat) {
		return _metres * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) / (1852.0f / 60.0f));;
	}
	
	bool Conversions::Rotation::TryGuessUnit(const std::string& _symbol, Conversions::Rotation::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Rotation::Convert(const double& _val, const Conversions::Rotation::Unit& _from, const Conversions::Rotation::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Rotation::Symbol(const Conversions::Rotation::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Time::TryGuessUnit(const std::string& _symbol, Conversions::Time::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Time::Convert(const double& _val, const Conversions::Time::Unit& _from, const Conversions::Time::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Time::Symbol(const Conversions::Time::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Temperature::TryGuessUnit(const std::string& _symbol, Conversions::Temperature::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
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
		result = glm::max(result, s_AbsoluteZero);
		
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
			glm::min(
				Convert(_val, _unit, Kelvin),
				s_PlanckTemperature
			),
			Kelvin,
			_unit
		);
	}
	
	bool Conversions::Pressure::TryGuessUnit(const std::string& _symbol, Conversions::Pressure::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Pressure::Convert(const double& _val, const Conversions::Pressure::Unit& _from, const Conversions::Pressure::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Pressure::Symbol(const Conversions::Pressure::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Mass::TryGuessUnit(const std::string& _symbol, Conversions::Mass::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Mass::Convert(const double& _val, const Conversions::Mass::Unit& _from, const Conversions::Mass::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Mass::Symbol(const Conversions::Mass::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Area::TryGuessUnit(const std::string& _symbol, Conversions::Area::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Area::Convert(const double& _val, const Conversions::Area::Unit& _from, const Conversions::Area::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Area::Symbol(const Conversions::Area::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
	bool Conversions::Volume::TryGuessUnit(const std::string& _symbol, Conversions::Volume::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	double Conversions::Volume::Convert(const double& _val, const Conversions::Volume::Unit& _from, const Conversions::Volume::Unit& _to) {
		return _val * (s_Conversion[_from] / s_Conversion[_to]);
	}
	
	std::string Conversions::Volume::Symbol(const Conversions::Volume::Unit& _unit) {
		return s_Symbol[_unit];
	}
	
} // LouiEriksson::Engine::Spatial::Maths