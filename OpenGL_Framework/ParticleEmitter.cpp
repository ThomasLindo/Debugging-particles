#include "ParticleEmitter.h"
#include "AniMath.h"
#include "IO.h"

ParticleEmitter::ParticleEmitter()
	: m_pParticles(nullptr),
	m_pNumParticles(0),
	playing(true)
{
}


ParticleEmitter::~ParticleEmitter()
{
	freeMemory();
}

void ParticleEmitter::initialize(unsigned int numParticles)
{
	freeMemory(); // destroy any existing particles

	if (numParticles > 0)
	{
		m_pParticles = new Particle[numParticles];
		memset(m_pParticles, 0, sizeof(Particle) * numParticles);
		m_pNumParticles = numParticles;
		mesh = new Mesh();
		mesh->initParticles(m_pParticles, &position);
	}
}

void ParticleEmitter::freeMemory()
{
	if (m_pParticles) // if not a null pointer
	{
		delete[] m_pParticles;
		m_pParticles = nullptr;
		m_pNumParticles = 0;
		delete mesh;
	}
}

void ParticleEmitter::update(float dt)
{
	if (m_pParticles && playing) // make sure memory is initialized and system is playing
	{
		// loop through each particle
		Particle* particle = m_pParticles;
		for (unsigned int i = 0; i < m_pNumParticles; ++i, ++particle)
		{
			if (particle->life <= 0) // if particle has no life remaining
			{
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles
				float randomTval;

				particle->acceleration = vec3(0.0f);
				randomTval = random(0.0f, 1.0f);
				particle->velocity.x = lerp(velocityMin.x, velocityMax.x, randomTval);
				randomTval = random(0.0f, 1.0f);
				particle->velocity.y = lerp(velocityMin.y, velocityMax.y, randomTval);
				randomTval = random(0.0f, 1.0f);
				particle->velocity.z = lerp(velocityMin.z, velocityMax.z, randomTval);

				particle->color = colorStart;

				randomTval = random(0.0f, 1.0f);
				particle->life = lerp(lifeRange.x, lifeRange.y, randomTval);
				particle->lifetime = particle->life; // Keep track of total lifetime of particle

				randomTval = random(0.0f, 1.0f);
				particle->mass = lerp(massRange.x, massRange.y, randomTval);

				randomTval = random(0.0f, 1.0f);
				particle->size = lerp(sizeRange.x, sizeRange.y, randomTval);

				randomTval = random(0.0f, 1.0f); 
				vec3 emitPos;
				emitPos.x = lerp(emitPosMin.x, emitPosMax.x, randomTval);
				randomTval = random(0.0f, 1.0f);
				emitPos.y = lerp(emitPosMin.y, emitPosMax.y, randomTval);
				randomTval = random(0.0f, 1.0f);
				emitPos.z = lerp(emitPosMin.z, emitPosMax.z, randomTval);

				particle->position = getLocalPos() + emitPos;

				particle->force = vec3(0.0f);
			}
	
			// Update physics
	
			// Update acceleration
			particle->acceleration = particle->force / particle->mass;
			particle->velocity = particle->velocity + (particle->acceleration * dt);
			particle->position = particle->position + (particle->velocity * dt);
	
			// We've applied the force, let's remove it so it does not get applied next frame
			particle->force = vec3(0.0f);
	
			// Decrease particle life
			particle->life -= dt;
	
			// Update visual properties?
		}
		mesh->updateVAO();
	}
}

void ParticleEmitter::draw()
{
	GameObject::draw();
	for (TextureBind tex : textures)
	{
		tex.texture->bind(tex.bindSlot);
	}
	//mesh->draw();
	Particle* p = m_pParticles;
	for (unsigned int i = 0; i < m_pNumParticles; ++i, ++p)
	{
		int* currentShaderProgram = new int;
		glGetIntegerv(GL_CURRENT_PROGRAM, currentShaderProgram);
		GLint uniformLoc = glGetUniformLocation(*currentShaderProgram, "uSize");
		glUniform1f(uniformLoc, p->size);
		uniformLoc = glGetUniformLocation(*currentShaderProgram, "uColor");
		vec4 newColor = p->color * lerp(colorEnd, colorStart, p->life / p->lifetime);
		glUniform4fv(uniformLoc, 1, &newColor.x);
		position = vec4(p->position, 1.0f);

		//takes a lot of cpu
		mesh->updateVAO();
		
		if (p->life >= 0.0f) // if particle is alive, draw it
		{
			//takes a lot of cpu (to be expected since it is a draw function?)
			mesh->draw();
		}
		
		delete currentShaderProgram;
	}
	for (TextureBind tex : textures)
	{
		tex.texture->unbind(tex.bindSlot);
	}
}

void ParticleEmitter::applyForceToParticle(unsigned int idx, vec3 force)
{
	SAT_ASSERT(idx < m_pNumParticles, "index %u out of range!", idx);

	m_pParticles[idx].force += force;
}

vec3 ParticleEmitter::getParticlePosition(unsigned int idx)
{
	SAT_ASSERT(idx < m_pNumParticles, "index %u out of range!", idx);

	return m_pParticles[idx].position;
}

float ParticleEmitter::getParticleMass(unsigned int idx)
{
	if (idx < m_pNumParticles)
	{
		//std::cout << "ParticleEmitter::getParticleMass ERROR: idx " << idx << "out of range!" << std::endl;
		return 0.0f;
	}

	return m_pParticles[idx].mass;
}
