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

	void renderAllFlocking(RenderWindow *window)
	{
		for (int i = 0; i < flockingVec.size(); i++)
		{
			window->draw(*flockingVec[i]);
		}
	}

	int getSysSize()
	{
		return flockingVec.size();
	}

	int getSeperationFactor()
	{
		return seperationFactor;
	}

	int getCohesionFactor()
	{
		return cohesionFactor;
	}

	int getAlignmentFactor()
	{
		return alignmentFactor;
	}

	void addSeperationFactor(int addNum)
	{
		seperationFactor += addNum;
		seperationFactor = (seperationFactor < 0) ? 0 : seperationFactor;
	}

	void addCohesionFactor(int addNum)
	{
		cohesionFactor += addNum;
		cohesionFactor = (cohesionFactor < 0) ? 0 : cohesionFactor;
	}

	void addAlignmentFactor(int addNum)
	{
		alignmentFactor += addNum;
		alignmentFactor = (alignmentFactor < 0) ? 0 : alignmentFactor;
	}
private:
	vector<FlockingElement*> flockingVec;
	vector<vector<FlockingElement*>> buckVec;
	int buckNum;
	int buckHeight;
	int buckWidth;

	int seperationFactor;
	int cohesionFactor;
	int alignmentFactor;

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