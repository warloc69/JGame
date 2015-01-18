
#include "Vectors.h"

float GHVectorLength(GHVECTOR v)
{
	return v.length();
}

GHVECTOR GHVectorNormalize(GHVECTOR v)
{
	float length = v.length();
	if(length == 0.0f)
		return v;
	
	GHVECTOR r = v / length;
	return r;
}

GHVECTOR GHVectorCross(GHVECTOR u, GHVECTOR v)
{
	GHVECTOR r;
	r.x = u.y * v.z - u.z * v.y;
	r.y = u.z * v.x - u.x * v.z;
	r.z = u.x * v.y - u.y * v.x;
	return r;
}

float GHVectorDot(GHVECTOR u, GHVECTOR v)
{
	float dot = u.x * v.x + u.y * v.y + u.z * v.z;
	return dot;
}
