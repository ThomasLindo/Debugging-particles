#pragma once
#include <gmtk/gmtk.h>

template <typename T>
inline T invlerp(T value, T v0, T v1)
{
	return (value -v0)/(v1-v0);
}

int log10Int(int num);
int log2Int(int num);

float random(float min, float max);
vec2 random2(float min, float max);
vec3 random3(float min, float max);
vec4 random4(float min, float max);
vec2 random(vec2 min, vec2 max);
vec3 random(vec3 min, vec3 max);
vec4 random(vec4 min, vec4 max);

template<class T>
// Clamps data to 0 - 1
T saturate(const T &data)
{
	return clamp(data, static_cast<T>(0), static_cast<T>(1));
}


namespace interp
{
	inline float smoothstep(float edge0, float edge1, float x)
	{
		x = invlerp(x, edge0, edge1);
		return x * x * (3.0f - 2.0f * x);
	}

	inline float flip(float t)
	{
		return 1.0f - t;
	}

	inline float smoothStart2(float t)
	{
		return t * t;
	}

	inline float smoothStart3(float t)
	{
		return t * t * t;
	}

	inline float smoothStart4(float t)
	{
		return t * t * t * t;
	}

	inline float smoothStart5(float t)
	{
		return t * t * t * t * t;
	}

	inline float smoothStart6(float t)
	{
		return t * t * t * t * t * t;
	}

	inline float smoothStop2(float t)
	{
		t = flip(t);
		return flip(t * t);
	}

	inline float smoothStop3(float t)
	{
		t = flip(t);
		return flip(t * t * t);
	}

	inline float smoothStop4(float t)
	{
		t = flip(t);
		return flip(t * t * t * t);
	}

	inline float smoothStop5(float t)
	{
		t = flip(t);
		return flip(t * t * t * t * t);
	}

	inline float smoothStop6(float t)
	{
		t = flip(t);
		return flip(t * t * t * t * t * t);
	}
}