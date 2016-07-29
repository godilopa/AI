#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "../include/Path.h"
#include <list>

struct Obstacle
{
	USVec2D center;
	float radius;
};

struct Params
{
    USVec2D targetPosition;
    float max_velocity;
    float max_acceleration;
    float dest_radius;
	float arrive_radius;

	float max_angular_velocity;
	float max_angular_acceleration;
	float angular_dest_radius;
	float angular_arrive_radius;
	float targetRotation;

	Path path;
	float lookAhead;
	std::list<Obstacle> obstacles;
};

bool ReadParams(const char* filename, Params& params);
bool ReadObstacles(const char* filename, Params& params);

#endif