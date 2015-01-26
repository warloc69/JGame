
#pragma once

#include "..\Integration\baseTypes.h"
#include <math.h>

struct GHVECTOR
{
	float x, y, z;

	GHVECTOR() 
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	GHVECTOR(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	const GHVECTOR operator+(const GHVECTOR& v)
	{
		return GHVECTOR(x + v.x, y + v.y, z + v.z);
	}

	const GHVECTOR& operator+=(const GHVECTOR& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	const GHVECTOR operator-(const GHVECTOR& v)
	{
		return GHVECTOR(x - v.x, y - v.y, z - v.z);
	}

	const GHVECTOR& operator-=(const GHVECTOR& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	const GHVECTOR operator*(const float k)
	{
		return GHVECTOR(x * k, y * k, z * k);
	}

	const GHVECTOR& operator*=(const float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;
		return *this;
	}

	const GHVECTOR operator/(const float k)
	{
		return GHVECTOR(x / k, y / k, z / k);
	}

	const GHVECTOR& operator/=(const float k)
	{
		this->x /= k;
		this->y /= k;
		this->z /= k;
		return *this;
	}

	bool equalf(float a, float b, float e = 0.0001f)
	{
		return fabs(b - a) <= e;
	}

	const bool operator==(const GHVECTOR& v)
	{
		return equalf(this->x, v.x) && equalf(this->y, v.y) && equalf(this->z, v.z);
	}

	const bool operator!=(const GHVECTOR& v)
	{
		return !equalf(this->x, v.x) || !equalf(this->y, v.y) || !equalf(this->z, v.z);
	}

	float length()
	{
		float l = sqrtf(x*x+y*y+z*z);
		return l;
	}
};

float    GHVectorLength         (GHVECTOR v);
float    GHVectorDot            (GHVECTOR u, GHVECTOR v);
GHVECTOR GHVectorCross          (GHVECTOR u, GHVECTOR v);
GHVECTOR GHVectorNormalize      (GHVECTOR v);

