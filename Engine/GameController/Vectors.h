
#pragma once

#include "baseTypes.h"
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

struct GHVECTOR4 : GHVECTOR
{
	float w;

	GHVECTOR4() 
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	GHVECTOR4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	const GHVECTOR4 operator+(const GHVECTOR4& v)
	{
		return GHVECTOR4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	const GHVECTOR4& operator+=(const GHVECTOR4& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;
		return *this;
	}

	const GHVECTOR4 operator-(const GHVECTOR4& v)
	{
		return GHVECTOR4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	const GHVECTOR4& operator-=(const GHVECTOR4& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;
		return *this;
	}

	const GHVECTOR4 operator*(const float k)
	{
		return GHVECTOR4(x * k, y * k, z * k, w * k);
	}

	const GHVECTOR4& operator*=(const float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;
		this->w *= k;
		return *this;
	}

	const GHVECTOR4 operator/(const float k)
	{
		return GHVECTOR4(x / k, y / k, z / k, w / k);
	}

	const GHVECTOR4& operator/=(const float k)
	{
		this->x /= k;
		this->y /= k;
		this->z /= k;
		this->w /= k;
		return *this;
	}

	bool equalf(float a, float b, float e = 0.0001f)
	{
		return fabs(b - a) <= e;
	}

	const bool operator==(const GHVECTOR4& v)
	{
		return equalf(this->x, v.x) && equalf(this->y, v.y) && equalf(this->z, v.z) && equalf(this->w, v.w);
	}

	const bool operator!=(const GHVECTOR4& v)
	{
		return !equalf(this->x, v.x) || !equalf(this->y, v.y) || !equalf(this->z, v.z) || !equalf(this->w, v.w);
	}

	float length()
	{
		float l = sqrtf(x*x+y*y+z*z+w*w);
		return l;
	}
};

float    GHVectorLength         (GHVECTOR v);
float    GHVectorDot            (GHVECTOR u, GHVECTOR v);
GHVECTOR GHVectorCross          (GHVECTOR u, GHVECTOR v);
GHVECTOR GHVectorNormalize      (GHVECTOR v);

GHVECTOR4 GHVector4NormalizeW(GHVECTOR4 v);