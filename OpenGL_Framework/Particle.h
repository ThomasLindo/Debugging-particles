#pragma once
#include "gmtk/gmtk.h"

struct Particle
{
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	vec3 force;

	float mass; // Determines amount of force to be applied

	float size;	// How big the particle appears
	vec4 color;

	float life; // remaining life of the particle
	float lifetime; // total lifetime of the particle
};
