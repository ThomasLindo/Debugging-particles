#pragma once
#include "gmtk/gmtk.h"
#include "GameObject.h"
#include "Particle.h"

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

class ParticleEmitter : public GameObject
{
public:
	unsigned int m_pNumParticles;
	Particle* m_pParticles;
	ParticleEmitter();
	~ParticleEmitter();

	void initialize(unsigned int numParticles);
	void freeMemory();

	void update2(unsigned int start, unsigned int end, float dt);
	void update(float dt);
	void draw2(unsigned int start, unsigned int end);
	void draw();

	void applyForceToParticle(unsigned int idx, vec3 force);

	unsigned int getNumParticles() { return m_pNumParticles; }

	vec3 getParticlePosition(unsigned int idx);
	float getParticleMass(unsigned int idx);

	///// Playback properties
	bool playing;

	// Initial properties for spawned particles
	// Storing ranges for each property so each particle has some randomness
	vec3 emitPosMin;
	vec3 emitPosMax;
	vec3 velocityMin;
	vec3 velocityMax;

	// these properties are single floats, so we can pack the min and max into a vec2, just data!
	vec2 lifeRange;
	vec2 sizeRange;
	vec2 massRange;

	vec4 colorStart = vec4(vec3(1.0f), 1.0f);
	vec4 colorEnd = vec4(vec3(1.0f), 0.0f);

	vec3 position;
};