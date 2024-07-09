#pragma once
#include <math.h>

namespace Math
{
	// constexpr = constant expression, runs functions at compiletime instead of runtime
	constexpr float PI		= 3.14159265359f; // 180
	constexpr float TWO_PI	= 6.28318530718f; // 360
	constexpr float HALF_PI = 1.57079632679f; // 90

	constexpr float RadToDeg(float radians) { return radians * (180.0f / PI); } // radians to degrees
	constexpr float DegToRad(float degrees) { return degrees * (PI / 180.0f); } // degrees to radians

	inline float Sin(float radians)
	{
		return sinf(radians);
	}

	inline float Cos(float radians)
	{
		return cosf(radians);
	}

	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	inline float Atan2(float y, float x) // Arctangent
	{
		return atan2f(y, x);
	}

	/// <summary>
	/// Returns the whichever input is greater
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="a">value</param>
	/// <param name="b">value</param>
	/// <returns>Whichever input is greater</returns>
	template<typename T>
	T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<typename T>
	T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	T Abs(T value)
	{
		return (value < 0) ? -value : value;
	}

	template<typename T>
	T Clamp(T value, T min, T max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}
}