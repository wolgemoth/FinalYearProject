#include "ISA.h"

#include <cmath>
#include <cstddef>
#include <iostream>

namespace LouiEriksson::Engine::Spatial::Atmosphere {

	ISA::State ISA::Solve(const float& _height, const State& _state) {

		State result{};
		
		if (_height <= s_Alt[s_Alt.size() - 2]) {
			
			size_t ctr = 0;
		
			result = _state;
			
			Calculate(_height, result, ctr);
			
			for (int i = 0; i < s_A_Val.size(); ++i) {
				
				if (_height > s_Alt[ctr]) {
            
		            if (ctr == 0 || ctr == 3 || ctr == 6) {
		                Pause(_height, result, ++ctr);
		            }
		            else {
		                Calculate(_height, result, ++ctr);
		            }
		        }
			}
		}
		else {
			std::cout << "Altitude out of range\n";
		}
		
		return result;
	}
	
	void ISA::Calculate(const float& _height, State& _state, size_t& _counter) {

		const float x = std::min(_height, s_Alt[_counter]);
        
        const float a = s_A_Val[_counter];
        
        const float t = _counter == 0 ? _state.m_Temperature + a * (x - s_Alt[s_Alt.size() - 1]) :
				                        _state.m_Temperature + a * (x - s_Alt[    _counter - 1]);
		
		_state.m_Pressure   *= std::pow(t / _state.m_Temperature, -s_G / (s_R * (a)));
		_state.m_Density     = _state.m_Pressure / (s_R * t);
		_state.m_Temperature = t;
	}
	
	void ISA::Pause(const float& _height, State& _state, size_t& _counter) {

		const float x = std::min(_height, s_Alt[_counter]);
        
        _state.m_Pressure *= std::pow(
			s_E,
			_counter == 0 ? -(s_G * (x - s_Alt[s_Alt.size() - 1])) / (s_R * _state.m_Temperature) :
							-(s_G * (x - s_Alt[    _counter - 1])) / (s_R * _state.m_Temperature)
		),
		
		_state.m_Density = _state.m_Pressure / (s_R * _state.m_Temperature);
	}
	
} // LouiEriksson::Engine::Spatial::Atmosphere