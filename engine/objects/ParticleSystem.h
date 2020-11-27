
#ifndef PARTICLE_SYSTEM_CLASS
#define PARTICLE_SYSTEM_CLASS

#include <string>
#include <vector>

#include "../GameEngine.h"
#include "../Object.h"
#include "../Material.h"
#include "../Model.h"
#include "../shaders/ParticleShader.h"

// Stores all the data for a given particle
struct Particle {
	float x, y, z, dx, dy, dz, size, age, lifespan;
};

// Simple particle system. Applies gravity to particles, which have a lifetime, position, and velocity
// Particles shrink as they age. This can be changed by extending this class.
class ParticleSystem : public Object {
public:
	// Create a new particle system using the texture given and applying gravity of the given amount
	ParticleSystem(std::string texture, float gravity);
	~ParticleSystem();

	// Draw the particle system
	void draw();

	// Add a new particle at a position with a velocity
	// Lifespan is measured in frames.
	void addParticle(float x, float y, float z, float dx, float dy, float dz, float lifespan);
	
	// How many particles are in this system?
	int size();
private:
	// Stores the shader, particles, and strength of gravity
	ParticleShader* shader;
	std::vector<Particle> particles;
	float gravity;
};

#endif
