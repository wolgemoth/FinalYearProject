#ifndef FINALYEARPROJECT_ISA_HPP
#define FINALYEARPROJECT_ISA_HPP

#include <array>
#include <cstddef>
#include <exception>
#include <optional>

namespace LouiEriksson::Engine::Spatial::Atmosphere {
	
	/*
	This is free and unencumbered software released into the public domain.
	
	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.
	
	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
	
	For more information, please refer to <https://unlicense.org>
	 */

	/**
	 * @class ISA
	 * @brief Class that represents the International Standard Atmosphere (ISA)
	 *
	 * The ISA class provides a model of the atmosphere at different altitudes.
	 * It includes calculations for pressure, density, and temperature at a given altitude.
	 *
	 * @remarks Derived from a C# implementaion provided by Max Bernard.
	 * Bernard, M., 2023. International_Standard_Atmosphere/International_Standard_Atmosphere/Algorithm.cs at main · maxbernard3/International_Standard_Atmosphere [online].
	 * GitHub. Available from: https://github.com/maxbernard3/International_Standard_Atmosphere/blob/main/International_Standard_Atmosphere/Algorithm.cs [Accessed 14 Mar 2024].
	 */
	template <typename T = double>
	class ISA final {
 
	public:
		
		/**
		 * @struct State
		 * @brief Represents the state of the atmosphere
		 * Includes pressure, density and temperature
		 */
		struct State final {
			
			T m_Pressure,    /**< @brief Represents Pressure in Pascals */
			  m_Density,     /**< @brief Represents Density in kilograms per cubic metre */
			  m_Temperature; /**< @brief Represents Temperature in Kelvin */
		};
		
		static constexpr State s_DefaultState { 101325.0, 1.225, 288.15 };
		
		/**
		 * @brief Wrapper function for calculate() and pause()
		 * @param[in] _height The altitude in meters
		 * @param[in,out] _state The atmospheric state at the given altitude
		 * @return true if able to calculate, else false
		 */
	    static std::optional<State> TrySolve(const T& _height) noexcept {
	
			std::optional<State> result;
			
			try {
				
				if (_height <= s_Alt[s_Alt.size() - 2U]) {
				
					size_t ctr(0U);
					
					State state;
					Calculate(_height, state, ctr);
					
					for (size_t i = 0U; i < s_A_Val.size(); ++i) {
						
						if (_height > s_Alt[ctr]) {
		            
				            if (ctr == 0U || ctr == 3U || ctr == 6U) {
				                Pause(_height, state, ++ctr);
				            }
				            else {
				                Calculate(_height, state, ++ctr);
				            }
				        }
					}
					
					result = state;
				}
				else {
					throw std::runtime_error("Altitude out of range!");
				}
			}
			catch (const std::exception& e) {
				result = std::nullopt;
				
				Debug::Log(e);
			}
			
			return result;
		}
		
	private:
		
        /* CONSTANTS */
        static constexpr T s_G = 9.80665; // Acceleration due to gravity
        static constexpr T s_R = 287.0;   // Molar gas constant for air
        static constexpr T s_E = 2.71828; // Euler's constant

		/** @brief Temperature gradient in Kelvin per metre. */
        static constexpr const std::array<T, 8U> s_A_Val {
			-0.0065,
			 0.0,
			 0.0010,
			 0.0028,
			 0.0,
			-0.0028,
			-0.0020,
			 0.0
		};

		/** @brief Altitude steps. */
        inline static const std::array<T, 9U> s_Alt {
			11000.0,
			20000.0,
			32000.0,
			47000.0,
			51000.0,
			71000.0,
			84000.0,
			90000.0,
			    0.0
		};
		
		/**
		 * @brief Calculate the atmospheric properties at a given altitude.
		 *
		 * This function calculates the pressure, density, and temperature of the atmosphere at a specific altitude.
		 *
		 * @param[in] _height The altitude in meters.
		 * @param[in] _state The atmospheric state at the given altitude.
		 * @param[in] _counter The index of the altitude step.
		 */
		static void Calculate(const T& _height, State& _state, size_t& _counter) {
	
			const float x = std::min(_height, s_Alt[_counter]);
	        
	        const float a = s_A_Val[_counter];
	        
	        const float t = _counter == 0U ? _state.m_Temperature + a * (x - s_Alt[s_Alt.size() - 1U]) :
					                         _state.m_Temperature + a * (x - s_Alt[    _counter - 1U]);
			
			_state.m_Pressure   *= std::pow(t / _state.m_Temperature, -s_G / (s_R * (a)));
			_state.m_Density     = _state.m_Pressure / (s_R * t);
			_state.m_Temperature = t;
		}
		
		/**
		 * @brief Pause function
		 *
		 * This function updates the atmospheric state by pausing at a given altitude. It calculates the pressure and density based on the altitude.
		 *
		 * @param[in] _height The altitude in meters.
		 * @param[in] _state The atmospheric state at the given altitude.
		 * @param[in] _counter The index of the altitude step.
		 */
		static void Pause(const T& _height, State& _state, size_t& _counter) {
	
			const float x = std::min(_height, s_Alt[_counter]);
	        
	        _state.m_Pressure *= std::pow(
				s_E,
				_counter == 0U ? -(s_G * (x - s_Alt[s_Alt.size() - 1U])) / (s_R * _state.m_Temperature) :
								 -(s_G * (x - s_Alt[    _counter - 1U])) / (s_R * _state.m_Temperature)
			),
			
			_state.m_Density = _state.m_Pressure / (s_R * _state.m_Temperature);
		}
		
	};
	
} // LouiEriksson::Engine::Spatial::Atmosphere

#endif //FINALYEARPROJECT_ISA_HPP