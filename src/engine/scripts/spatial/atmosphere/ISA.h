#ifndef FINALYEARPROJECT_ISA_H
#define FINALYEARPROJECT_ISA_H

#include <array>
#include <cstddef>

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

    /// <summary>
    /// https://github.com/maxbernard3/International_Standard_Atmosphere
    /// </summary>
	class ISA {
 
	public:
	
		struct State {
			
			float m_Pressure,
			      m_Density,
			      m_Temperature;
		};
		
		/// <summary>
		/// A wrapper function to hide calculate's extra input and output.
		/// </summary>
	    static State Solve(const float& _height, const State& _state = { 101325.0f, 1.225f, 288.15f });
		
	private:
		
        // Constants
        inline static constexpr float s_G = 9.80665f; // Acceleration due to gravity
        inline static constexpr float s_R = 287.0f;   // Molar gas constant for air
        inline static constexpr float s_E = 2.71828f; // Euler's constant

        // Temperature gradient in Kelvin per metre
        inline static constexpr const std::array<float, 8> s_A_Val = {
			-0.0065f,
			 0,
			 0.0010f,
			 0.0028f,
			 0,
			-0.0028f,
			-0.0020f,
			 0
		};

        // Altitudes Step
        inline static const std::array<float, 9> s_Alt = {
			11000,
			20000,
			32000,
			47000,
			51000,
			71000,
			84000,
			90000,
			    0
		};

	    static void Calculate(const float& _height, State& _state, size_t& _counter);
		
	    static void Pause(const float& _height, State& _state, size_t& _counter);
		
	};
	
} // LouiEriksson::Engine::Spatial::Atmosphere

#endif //FINALYEARPROJECT_ISA_H