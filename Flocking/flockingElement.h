#pragma once

#include "SFMLHeader.h"

class FlockingElement : public ConvexShape
{
public:
	FlockingElement();
	void setBlockIndex(int p_index)
	{
		blockIndex = p_index;
	}

	int getBlockIndex()
	{
		return blockIndex;
	}

	void setDesiredVec(Vector2f pVec)
	{
		desiredVec = pVec;
	}

	void updatePos(float);

	Vector2f getVelocity()
	{
		return speed * direction;
	}

	Vector2f computeUnitVec(Vector2f a)
	{
		return a / computeVecLength(a);
	}
private:
	float speed;
	float maxSpeed;
	Vector2f acceleration;
	Vector2f direction;
	Vector2f desiredVec;

	int blockIndex;

	float computeVecLength(Vector2f a)
	{
		return sqrt(a.x*a.x + a.y*a.y);
	}

	float computeDotProduct(Vector2f a, Vector2f b)
	{
		return a.x*b.x + a.y*b.y;
	}
};