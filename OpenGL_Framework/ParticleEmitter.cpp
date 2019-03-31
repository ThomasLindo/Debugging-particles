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
void ParticleEmitter::update2(unsigned int start, unsigned int end, float dt){
	Particle* particle = m_pParticles;
	particle += start;
	for (unsigned int i = start; i < end; ++i, ++particle)
	{
		if (particle->life > 0) // if particle has no life remaining
		{
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
		else {
			// Respawn particle
			// Note: we are not freeing memory, we are "Recycling" the particles


			particle->acceleration = vec3(0.0f);
			particle->velocity = random(velocityMin, velocityMax);

			particle->color = colorStart;

			particle->life = random(lifeRange.x, lifeRange.y);
			particle->lifetime = particle->life; // Keep track of total lifetime of particle

			particle->mass = random(massRange.x, massRange.y);

			particle->size = random(sizeRange.x, sizeRange.y);

			particle->position = getLocalPos() + random(emitPosMin, emitPosMax);

			particle->force = vec3(0.0f);
		}
	}
}
void ParticleEmitter::update(float dt) {
	// make sure memory is initialized and system is playing
	if (m_pParticles && playing) {
		// loop through each particle
		Particle* particle = m_pParticles;
		for (unsigned int i = m_pNumParticles; i > 0; --i, ++particle){
			if (particle->life > 0) {
			// Update physics
			// Update acceleration
			particle->acceleration = particle->force / particle->mass;
			particle->velocity = particle->velocity + (particle->acceleration * dt);
			particle->position = particle->position + (particle->velocity * dt);

			if (particle->position.y < 0 && particle->velocity.y < 0) { particle->velocity.y = -particle->velocity.y; }
	
			// We've applied the force, let's remove it so it does not get applied next frame
			particle->force = vec3(0.0f);
	
			// Decrease particle life
			particle->life -= dt;

			// Update visual properties?
			}
			// if particle has no life remaining
			else {
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles
				particle->acceleration = vec3(0.0f);
				particle->velocity = random(velocityMin, velocityMax);

				particle->color = colorStart;

				particle->life = random(lifeRange.x, lifeRange.y);
				particle->lifetime = particle->life; // Keep track of total lifetime of particle

				particle->mass = random(massRange.x, massRange.y);

				particle->size = random(sizeRange.x, sizeRange.y);

				particle->position = getLocalPos() + random(emitPosMin, emitPosMax);

				particle->force = vec3(0.0f);
			}
		}
		/*std::thread t1(&ParticleEmitter::update2, this, 0, (m_pNumParticles / 5), dt);
		std::thread t2(&ParticleEmitter::update2, this, (m_pNumParticles / 5), (m_pNumParticles / 5) * 2, dt);
		std::thread t3(&ParticleEmitter::update2, this, (m_pNumParticles / 5) * 2, (m_pNumParticles / 5) * 3, dt);
		std::thread t4(&ParticleEmitter::update2, this, (m_pNumParticles / 5) * 3, (m_pNumParticles / 5) * 4, dt);
		std::thread t5(&ParticleEmitter::update2, this, (m_pNumParticles / 5) * 4, m_pNumParticles, dt);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();*/
		mesh->updateVAO();
	}
}


void ParticleEmitter::draw2(unsigned int start, unsigned int end) {
	Particle* p = m_pParticles;
	p += start;
	for (unsigned int i = start; i < end; ++i, ++p)
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
		//takes a lot of cpu (to be expected since it is a draw function?)
		mesh->draw();
		delete currentShaderProgram;
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
	for (unsigned int i = m_pNumParticles; i > 0; --i, ++p)
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
		
		//takes a lot of cpu (to be expected since it is a draw function?)
		mesh->draw();
		
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
