#include "flockingSystem.h"
#include "propertyHeader.h"

FlockingSystem::FlockingSystem()
{
	buckNum = BUCK_ROW * BUCK_COLUMN;
	buckVec.resize(buckNum);
	buckHeight = WIN_HEIGHT / BUCK_ROW;
	buckWidth = WIN_WIDTH / BUCK_COLUMN;

	alignmentFactor = 0;
	cohesionFactor = 0;
	seperationFactor = 0;
}

FlockingSystem::~FlockingSystem()
{
	for (int i = 0; i < flockingVec.size(); i++)
	{
		delete flockingVec[i];
	}
}

int FlockingSystem::getBlockId(Vector2f flockPos)
{
	int rowNum = flockPos.y / buckHeight;
	rowNum = (rowNum < 0) ? 0 : rowNum;
	rowNum = (rowNum >= BUCK_ROW) ? (BUCK_ROW - 1) : rowNum;
	int columnNum = flockPos.x / buckWidth;
	columnNum = (columnNum < 0) ? 0 : columnNum;
	columnNum = (columnNum >= BUCK_COLUMN) ? (BUCK_COLUMN - 1) : columnNum;
	return columnNum + rowNum * BUCK_COLUMN;
}

void FlockingSystem::addNewFlocking()
{
	FlockingElement *newFlocking = new FlockingElement();
	int blockIndex = getBlockId(newFlocking->getPosition());
	buckVec[blockIndex].push_back(newFlocking);
	newFlocking->setBlockIndex(blockIndex);

	flockingVec.push_back(newFlocking);
}

void FlockingSystem::decreaseFlocking()
{
	if (flockingVec.size() <= 0)
		return;

	FlockingElement *deleteOne = flockingVec.back();
	flockingVec.pop_back();
	int blockIndex = deleteOne->getBlockIndex();
	buckVec[blockIndex].erase(buckVec[blockIndex].begin() + getVecIndex(blockIndex,deleteOne));

	delete deleteOne;
}

void FlockingSystem::updateBuck()
{
	for (int i = 0; i < flockingVec.size(); i++)
	{
		int curIndex = getBlockId(flockingVec[i]->getPosition());
		int originIndex = flockingVec[i]->getBlockIndex();
		if (curIndex != originIndex)
		{
			buckVec[originIndex].erase(buckVec[originIndex].begin() + getVecIndex(originIndex, flockingVec[i]));
			buckVec[curIndex].push_back(flockingVec[i]);
			flockingVec[i]->setBlockIndex(curIndex);
		}
	}
}

void FlockingSystem::computeDesireById(int blockId,FlockingElement* element,int xOffset,int yOffset,ModeStruct &tempStruct)
{
	Vector2f seperateVec(0,0);
	int seperateSum = 0;
	for (int i = 0; i < buckVec[blockId].size(); i++)
	{
		FlockingElement *curElement = buckVec[blockId][i];
		if (curElement == element)
			continue;
		Vector2f curPos = curElement->getPosition();
		Vector2f newPos = Vector2f(curPos.x + xOffset, curPos.y + yOffset);
		float disVec = COMPUTE_DISTANCE(element->getPosition(), newPos);
		if (disVec <= CHECK_RANGE)
		{
			if (seperationFactor != 0)
			{
				Vector2f diffVec = COMPUTE_SUBSTRATION(element->getPosition(), newPos);
				tempStruct.seperationVec += diffVec;
				tempStruct.seperationSum++;
			}
			if (alignmentFactor != 0)
			{
				tempStruct.alignmentVec += curElement->getVelocity();
				tempStruct.alignmentSum++;
			}
			if (cohesionFactor != 0)
			{
				tempStruct.cohesionVec += curElement->getPosition();
				tempStruct.cohesionSum++;
			}
		}
	}
}

void FlockingSystem::computeDesireByIndex(int row,int col,FlockingElement* element,ModeStruct &tempStruct)
{
	int xOffset = 0;
	int yOffset = 0;
	if (row < 0)
	{
		row = BUCK_ROW - 1;
		yOffset = -WIN_HEIGHT;
	}
	else if (row >= BUCK_ROW)
	{
		row = 0;
		yOffset = WIN_HEIGHT;
	}

	if (col < 0)
	{
		col = BUCK_COLUMN - 1;
		xOffset = -WIN_WIDTH;
	}
	else if (col >= BUCK_COLUMN)
	{
		col = 0;
		xOffset = WIN_WIDTH;
	}

	computeDesireById(row*BUCK_COLUMN + col, element, xOffset, yOffset,tempStruct);
}

void FlockingSystem::computeForce(FlockingElement* element)
{
	int blockId = element->getBlockIndex();
	int rowId = blockId / BUCK_COLUMN;
	int columnId = blockId - rowId * BUCK_COLUMN;
	Vector2f elementPos = element->getPosition();

	ModeStruct tempStruct;
	tempStruct.alignmentVec = Vector2f(0,0);
	tempStruct.cohesionVec = elementPos;
	tempStruct.seperationVec = Vector2f(0,0);
	tempStruct.seperationSum = 0;
	tempStruct.cohesionSum = 1;
	tempStruct.alignmentSum = 0;

	computeDesireById(blockId,element,0,0,tempStruct);
	float leftBound = columnId * buckWidth;
	float rightBound = (columnId + 1)*buckWidth;
	float upBound = rowId * buckHeight;
	float downBound = (rowId + 1)*buckHeight;
	bool checkLeft = false;
	bool checkRight = false;
	bool checkUp = false;
	bool checkDown = false;
	if (elementPos.x - CHECK_RANGE < leftBound)
	{
		checkLeft = true;
		computeDesireByIndex(rowId, columnId - 1, element, tempStruct);
	}
	if (elementPos.x + CHECK_RANGE > rightBound)
	{
		checkRight = true;
		computeDesireByIndex(rowId, columnId + 1, element, tempStruct);
	}
	if (elementPos.y - CHECK_RANGE < upBound)
	{
		checkUp = true;
		computeDesireByIndex(rowId - 1, columnId, element, tempStruct);
	}
	if (elementPos.y + CHECK_RANGE > downBound)
	{
		checkDown = true;
		computeDesireByIndex(rowId + 1, columnId, element, tempStruct);
	}
	if (checkLeft && checkUp)
		computeDesireByIndex(rowId - 1, columnId - 1, element, tempStruct);
	if (checkLeft && checkDown)
		computeDesireByIndex(rowId - 1, columnId + 1, element, tempStruct);
	if (checkRight && checkUp)
		computeDesireByIndex(rowId + 1, columnId - 1, element, tempStruct);
	if (checkRight && checkDown)
		computeDesireByIndex(rowId + 1, columnId + 1, element, tempStruct);

	Vector2f desireVec(0, 0);
	if (tempStruct.seperationSum > 0)
	{
		tempStruct.seperationVec /= (float)tempStruct.seperationSum;
		desireVec += (float)seperationFactor * COMPUTE_UNIT(tempStruct.seperationVec);
	}

	if (tempStruct.alignmentSum > 0)
	{
		tempStruct.alignmentVec /= (float)tempStruct.alignmentSum;
		desireVec += (float)alignmentFactor * COMPUTE_UNIT(tempStruct.alignmentVec);
	}
	
	if (tempStruct.cohesionSum > 1)
	{
		Vector2f desPoint = tempStruct.cohesionVec / (float)tempStruct.cohesionSum;
		desPoint -= element->getPosition();
		desireVec +=  (float)cohesionFactor * COMPUTE_UNIT(desPoint);
	}

	element->setDesiredVec(desireVec);
}

void FlockingSystem::updateForce()
{
	for (int i = 0; i < flockingVec.size(); i++)
	{
		computeForce(flockingVec[i]);
	}
}

void FlockingSystem::updateFlockings(float dt)
{
	for (int i = 0; i < flockingVec.size(); i++)
	{
		flockingVec[i]->updatePos(dt);
	}
}

void FlockingSystem::updateSystem(float dt)
{
	updateForce();
	updateFlockings(dt);
	updateBuck();
}