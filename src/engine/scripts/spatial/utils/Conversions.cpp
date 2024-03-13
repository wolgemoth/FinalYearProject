#include "Conversions.h"

#include <glm/common.hpp>

#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine::Spatial::Utils {
	
	bool Conversions::Speed::TryGuessUnit(const std::string& _symbol, Conversions::Speed::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Speed::Convert(const float& _val, const Conversions::Speed::Unit& _from, const Conversions::Speed::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Speed::Symbol(const Conversions::Speed::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Distance::Lateral::TryGuessUnit(const std::string& _symbol, Conversions::Distance::Lateral::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Distance::Lateral::Convert(const float& _val, const Conversions::Distance::Lateral::Unit& _from, const Conversions::Distance::Lateral::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Distance::Lateral::Symbol(const Conversions::Distance::Lateral::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Distance::Rotational::TryGuessUnit(const std::string& _symbol, Conversions::Distance::Rotational::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Distance::Rotational::Convert(
			const float& _val, const Conversions::Distance::Rotational::Unit& _from, const Conversions::Distance::Rotational::Unit& _to
	) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Distance::Rotational::Symbol(const Conversions::Distance::Rotational::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Time::TryGuessUnit(const std::string& _symbol, Conversions::Time::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Time::Convert(const float& _val, const Conversions::Time::Unit& _from, const Conversions::Time::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Time::Symbol(const Conversions::Time::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Temperature::TryGuessUnit(const std::string& _symbol, Conversions::Temperature::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Temperature::Convert(const float& _val, const Conversions::Temperature::Unit& _from, const Conversions::Temperature::Unit& _to) {

		float result;
		
		// Convert to Kelvin:
		switch (_from) {
			case Celsius:    { result =  _val - 272.15f;         break; }
			case Fahrenheit: { result = (_val + 459.67f) / 1.8f; break; }
			case Kelvin:     { result = _val;                    break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
		
		// Clamp result above absolute zero.
		result = glm::max(result, s_AbsoluteZero);
		
		// Convert Kelvin to target:
		switch (_to) {
			case Celsius:    { result += 273.15f;                  break; }
			case Fahrenheit: { result = (result * 1.8f) - 459.67f; break; }
			case Kelvin:     {                                     break; }
			default: {
				throw std::runtime_error("Not implemented!");
			}
		}
		
		return result;
	}
	
	std::string Conversions::Temperature::Symbol(const Conversions::Temperature::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	float Conversions::Temperature::ClampTemperature(const float& _val, Conversions::Temperature::Unit& _unit) {
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
	
	float Conversions::Pressure::Convert(const float& _val, const Conversions::Pressure::Unit& _from, const Conversions::Pressure::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Pressure::Symbol(const Conversions::Pressure::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Mass::TryGuessUnit(const std::string& _symbol, Conversions::Mass::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Mass::Convert(const float& _val, const Conversions::Mass::Unit& _from, const Conversions::Mass::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Mass::Symbol(const Conversions::Mass::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Area::TryGuessUnit(const std::string& _symbol, Conversions::Area::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Area::Convert(const float& _val, const Conversions::Area::Unit& _from, const Conversions::Area::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Area::Symbol(const Conversions::Area::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
	bool Conversions::Volume::TryGuessUnit(const std::string& _symbol, Conversions::Volume::Unit& _result) {
		return s_Lookup.Get(_symbol, _result);
	}
	
	float Conversions::Volume::Convert(const float& _val, const Conversions::Volume::Unit& _from, const Conversions::Volume::Unit& _to) {
		return _val * (s_Conversion.Return(_from) / s_Conversion.Return(_to));
	}
	
	std::string Conversions::Volume::Symbol(const Conversions::Volume::Unit& _unit) {
		return s_Symbol.Return(_unit);
	}
	
} // LouiEriksson::Engine::Spatial::Utils