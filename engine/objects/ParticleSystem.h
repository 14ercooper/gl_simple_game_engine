
#ifndef PARTICLE_SYSTEM_CLASS
#define PARTICLE_SYSTEM_CLASS

#include <string>
#include <vector>

#include "../GameEngine.h"
#include "../Object.h"
#include "../Material.h"
#include "../Model.h"
#include "../shaders/ParticleShader.h"

struct Particle {
	float x, y, z, dx, dy, dz, size, age, lifespan;
};

class ParticleSystem : public Object {
public:
	ParticleSystem(std::string texture, float gravity);

	void draw();

	void addParticle(float x, float y, float z, float dx, float dy, float dz, float lifespan);
	int size();
private:
	ParticleShader* shader;
	std::vector<Particle> particles;
	float gravity;
};

#endif
