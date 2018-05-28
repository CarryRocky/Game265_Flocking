#pragma once

#include "SFMLHeader.h"
#include "flockingElement.h"

struct ModeStruct
{
	Vector2f seperationVec;
	Vector2f alignmentVec;
	Vector2f cohesionVec;
	int seperationSum;
	int alignmentSum;
	int cohesionSum;
};

class FlockingSystem
{
public:
	FlockingSystem();
	~FlockingSystem();
	void addNewFlocking();
	void decreaseFlocking();
	void updateSystem(float);

	vector<FlockingElement*> getAllFlocking()
	{
		return flockingVec;
	}
private:
	vector<FlockingElement*> flockingVec;
	vector<vector<FlockingElement*>> buckVec;
	int buckNum;
	int buckHeight;
	int buckWidth;

	bool needSeperation;
	bool needAlignment;
	bool needCohesion;

	void updateBuck();
	int getBlockId(Vector2f);
	void updateForce();
	void computeForce(FlockingElement*);
	void computeDesireById(int,FlockingElement*,int,int,ModeStruct&);
	void computeDesireByIndex(int,int,FlockingElement*,ModeStruct&);

	void updateFlockings(float);

	int getVecIndex(int bid, FlockingElement* e)
	{
		for (int i = 0; i < buckVec[bid].size();i++)
		{
			if (buckVec[bid][i] == e)
			{
				return i;
			}
		}
	}
};