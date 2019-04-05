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
	/*	for (int i = numParticles - 1; i >= 0;--i,++m_pParticles) {
			m_pParticles[i] = *new Particle;
		}*/
		
		//memset(m_pParticles, 0, sizeof(Particle) * numParticles);
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

constexpr float forceDistance = 5.0f;
void ParticleEmitter::update2(unsigned int start, unsigned int end, float dt){
	Particle* particle = m_pParticles;
	particle += start;
	for (unsigned int i = start; i < end; ++i, ++particle)
	{
		if (particle->life > 0) // if particle has no life remaining
		{
			// Update physics

			// Update acceleration
			float *diffValue2 = 0;
			float *diffValue3 = &dt;
			diffValue2 = &position.x;

			//acceleration
			diffValue2 += 6;
			*diffValue2 = (*(diffValue2 + 3) / (*(diffValue2+6)));
			diffValue2++;
			*diffValue2 = (*(diffValue2 + 3) / (*(diffValue2+5)));
			diffValue2++;
			*diffValue2 = (*(diffValue2 + 3) / (*(diffValue2+4)));
			diffValue2++;

			//velocity
			diffValue2 -= 6; 
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;

			//position
			diffValue2 -= 6;
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;
			*diffValue2 = *diffValue2 + (*(diffValue2+3) * *diffValue3);
			diffValue2++;

			diffValue2 += 12;
			*diffValue2 -= *diffValue3;

			//particle->acceleration = particle->force / particle->mass;
			//particle->velocity = particle->velocity + (particle->acceleration * dt);
			//particle->position = particle->position + (particle->velocity * dt);
			// We've applied the force, let's remove it so it does not get applied next frame
			//particle->force = vec3(0.0f);
			// Decrease particle life
			//particle->life -= dt;

			// Update visual properties?
		}
		else {
			// Respawn particle
			// Note: we are not freeing memory, we are "Recycling" the particles
			
			float *diffValue2 = 0;
			diffValue2 = &position.x;

			//Force
			vec3 force = vec3();
			force += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
			force += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
			force += force;


			particle->acceleration = vec3(0.0f);
			particle->velocity = random(velocityMin, velocityMax);

			particle->color = colorStart;

			particle->life = random(lifeRange.x, lifeRange.y);
			particle->lifetime = particle->life; // Keep track of total lifetime of particle

			particle->mass = random(massRange.x, massRange.y);

			particle->size = random(sizeRange.x, sizeRange.y);

			particle->position = getLocalPos() + random(emitPosMin, emitPosMax);
		}
	}
}
void ParticleEmitter::update(float dt) {
	float *diffValue2 = 0;
	// make sure memory is initialized and system is playing
	if (m_pParticles && playing) {
		// loop through each particle
		Particle* particle = m_pParticles;
		for (unsigned int i = m_pNumParticles; i > 0; --i, ++particle){
			if (particle->life > 0) {
				// Update physics // Update acceleration
				//particle->acceleration = particle->force / particle->mass;
				//particle->velocity = particle->velocity + (particle->acceleration * dt);
				//vec3 oldpos = particle->position;
				//particle->position = oldpos + (particle->velocity * dt);
				//// Decrease particle life
				//particle->life -= dt;

				
				vec3 oldpos = particle->position;
				diffValue2 = &particle->position.x;
				//acceleration
				diffValue2 += 6;
				*diffValue2 = ((*(diffValue2 + 3)) / (*(diffValue2 + 6)));
				diffValue2++;
				*diffValue2 = ((*(diffValue2 + 3)) / (*(diffValue2 + 5)));
				diffValue2++;
				*diffValue2 = ((*(diffValue2 + 3)) / (*(diffValue2 + 4)));
				diffValue2++;
				//velocity
				diffValue2 -= 6;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				//position
				diffValue2 -= 6;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				*diffValue2 = *diffValue2 + (*(diffValue2 + 3) * dt);
				diffValue2++;
				// Decrease particle life
				diffValue2 += 11;
				*diffValue2 -= dt;
				

				diffValue2 -= 13;

				if ((*diffValue2 <= CollisionPlane.y && oldpos.y >  CollisionPlane.y)
				 || (*diffValue2 >  CollisionPlane.y && oldpos.y <= CollisionPlane.y)) 
				//{ 
				//	*(diffValue2+3) = -*(diffValue2+3);
				//	//particle->position = oldpos + (particle->velocity * dt);
				//	*(diffValue2-1) = oldpos.x + (*(diffValue2 + 2)* dt);
				//	*(diffValue2)   = oldpos.y + (*(diffValue2 + 3)* dt);
				//	*(diffValue2+1) = oldpos.z + (*(diffValue2 + 4)* dt);
				//}
				{
					//magnitude
					float mag = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
					//normalize
					vec3 normalN;
					normalN.x = normal.x / mag;
					normalN.y = normal.y / mag;
					normalN.z = normal.z / mag;
					////dot product
					//vec3 dot;
					//dot.x = normalN.x*particle->velocity.x;
					//dot.y = normalN.y*particle->velocity.y;
					//dot.z = normalN.z*particle->velocity.z;
					////final calculation
					//vec3 final;
					//final.x = particle->velocity.x - 2 * dot.x * normalN.x;
					//final.y = particle->velocity.y - 2 * dot.y * normalN.y;
					//final.z = particle->velocity.z - 2 * dot.z * normalN.z;
					////Set velocity
					//particle->velocity = final;
					//particle->position = oldpos + (particle->velocity * dt);

					//velocity
					diffValue2 += 2;
					*(diffValue2) = (*diffValue2) - 2 * ((*diffValue2) * normalN.x) * normalN.x;
					diffValue2++;
					*(diffValue2) = (*diffValue2) - 2 * ((*diffValue2) * normalN.y) * normalN.y;
					diffValue2++;
					*(diffValue2) = (*diffValue2) - 2 * ((*diffValue2) * normalN.z) * normalN.z;
					
					//Position
					diffValue2 -= 5;
					*diffValue2 = oldpos.x + (*(diffValue2 + 3)* dt);
					diffValue2++;
					*diffValue2 = oldpos.y + (*(diffValue2 + 3)* dt);
					diffValue2++;
					*diffValue2 = oldpos.z + (*(diffValue2 + 3)* dt);
					diffValue2++;
				}
				
	
	

			// Update visual properties?
			}
			// if particle has no life remaining
			else {
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles

				diffValue2 = &particle->position.x;

				//position
				*diffValue2 = m_pLocalPosition.x + random(emitPosMin.x, emitPosMax.x);
				diffValue2++;
				*diffValue2 = m_pLocalPosition.y + random(emitPosMin.y, emitPosMax.y);
				if (*diffValue2 >= CollisionPlane.y) { particle->color = colorStart2; }
				else { particle->color = colorStart3; }
				diffValue2++;
				*diffValue2 = m_pLocalPosition.z + random(emitPosMin.z, emitPosMax.z);
				diffValue2++;
				//velocity
				*diffValue2 = random(velocityMin.x, velocityMax.x);
				diffValue2++;
				*diffValue2 = random(velocityMin.y, velocityMax.y);
				diffValue2++;
				*diffValue2 = random(velocityMin.z, velocityMax.z);
				diffValue2++;
				//acceleration
				*diffValue2 = 0.0f;
				diffValue2++;
				*diffValue2 = 0.0f;
				diffValue2++;
				*diffValue2 = 0.0f;
				diffValue2++;
				//Force
				vec3 force = vec3();
				vec3 *diffValue = &force;
				*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
				*diffValue += vec3(1.0f / (forceDistance - *diffValue2), 1.0f / (forceDistance - (*diffValue2 + 1)), 1.0f / (forceDistance - (*diffValue2 + 2)));
				*diffValue += force;
				*diffValue2 = force.x;
				diffValue2++;
				*diffValue2 = force.y;
				diffValue2++;
				*diffValue2 = force.z;
				diffValue2++;
				//mass
				*diffValue2 = random(massRange.x, massRange.y);
				diffValue2++;
				//size
				*diffValue2 = random(sizeRange.x, sizeRange.y);
				diffValue2++;
				//life
				*diffValue2 = random(lifeRange.x, lifeRange.y);
				diffValue2++;
				*diffValue2 = *(diffValue2-1);
				diffValue2++;

				//particle->acceleration = vec3(0.0f);
				//particle->velocity = random(velocityMin, velocityMax);
				//particle->color = colorStart;
				//particle->life = random(lifeRange.x, lifeRange.y);
				//particle->lifetime = particle->life; // Keep track of total lifetime of particle
				//particle->mass = random(massRange.x, massRange.y);
				//particle->size = random(sizeRange.x, sizeRange.y);
				//particle->position = m_pLocalPosition + random(emitPosMin, emitPosMax);
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
