#pragma once

#define WIN_WIDTH 2400
#define WIN_HEIGHT 1500

#define INIT_SPEED 90
#define MAX_SPEED 150

#define PI 3.1415926

#define COLOR_RANGE 256;

#define BUCK_ROW 3
#define BUCK_COLUMN 4

#define CHECK_RANGE 100

#define COMPUTE_LENGTH(a) sqrt(a.x*a.x + a.y*a.y)
#define COMPUTE_UNIT(a) (a / COMPUTE_LENGTH(a))
#define COMPUTE_DOT(a,b) (a.x*b.x + a.y*b.y)
#define COMPUTE_DISTANCE(a,b) sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y))
#define COMPUTE_SUBSTRATION(a,b) Vector2f(a.x - b.x,a.y - b.y)