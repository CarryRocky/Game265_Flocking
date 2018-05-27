#include "flockingElement.h"
#include "propertyHeader.h"

FlockingElement::FlockingElement()
{
	setPointCount(3);

	setPoint(0, Vector2f(30,0));
	setPoint(1, Vector2f(20, 40));
	setPoint(2, Vector2f(40, 40));

	acceleration = Vector2f(0,0);
	speed = INIT_SPEED;
	maxSpeed = MAX_SPEED;

	int randomAngle = rand() % 360;
	float radian = PI * randomAngle / 180;
	direction = Vector2f(sin(radian), -cos(radian));

	int randomX = rand() % (WIN_WIDTH + 1);
	int randomY = rand() % (WIN_HEIGHT + 1);

	setPosition(randomX, randomY);

	int randomR = rand() % COLOR_RANGE;
	int randomG = rand() % COLOR_RANGE;
	int randomB = rand() % COLOR_RANGE;

	setFillColor(Color(randomR, randomG, randomB));

	setRotation(randomAngle);
}

void FlockingElement::updatePos(float dt)
{
	if (COMPUTE_LENGTH(desiredVec) >= 0.1)
	{
		acceleration = COMPUTE_UNIT(desiredVec)*maxSpeed - direction * speed;
	}
	else
		acceleration = Vector2f(0,0);

	Vector2f newDir = direction * speed + acceleration * dt;
	direction = COMPUTE_UNIT(newDir);

	Vector2f newPos = getPosition() + direction * speed*dt;
	if (newPos.x < 0)
		newPos.x = newPos.x + WIN_WIDTH;
	else if (newPos.x > WIN_WIDTH)
		newPos.x = newPos.x - WIN_WIDTH;

	if (newPos.y < 0)
		newPos.y = newPos.y + WIN_HEIGHT;
	else if (newPos.y > WIN_HEIGHT)
		newPos.y = newPos.y - WIN_HEIGHT;

	setPosition(newPos);

	float radian = acos(computeDotProduct(Vector2f(0,-1),direction));
	float angle = radian * 180 / PI;
	if (direction.x < 0)
		angle = -angle;
	setRotation(angle);
}
