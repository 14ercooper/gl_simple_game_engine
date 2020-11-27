
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem (std::string texture, float gravity) {
	shader = new ParticleShader(texture);
	material = new Material();
	model = new Model();
	this->gravity = gravity;
}

ParticleSystem::~ParticleSystem() {
	if (delShader)
		delete shader;
}

void ParticleSystem::draw() {
	// Set the uniforms of the shader
	shader->uniformMat4("modelMatrix", glm::mat4(1.0f));
	shader->uniformMat4("viewMatrix", GameEngine::engineCamera->getViewMatrix());
	shader->uniformMat4("projectionMatrix", GameEngine::engineCamera->getProjectionMatrix(GameEngine::engineWindow));

	// Update particles, deleting as needed
	for (long unsigned int i = 0; i < particles.size(); i++) {
		Particle p = particles.at(i);
		if (p.age > p.lifespan) {
			particles.erase(particles.begin() + i);
			continue;
		}
		p.x += p.dx;
		p.dy -= gravity;
		if (p.dy < -0.1f)
			p.dy = -0.1f;
		p.y += p.dy;
		p.z += p.dz;
		p.age++;
		p.size = 1 - (p.age / p.lifespan); // Shrink with age
		particles.at(i) = p;
	}

	// Draw all particles
	shader->useProgram();
	for (Particle p : particles) {
		shader->drawParticle(p.x, p.y, p.z, p.size);
	}
}

void ParticleSystem::addParticle(float x, float y, float z, float dx, float dy, float dz, float lifespan) {
	// Add particle to the system
	if (lifespan < 1)
		return;

	particles.push_back({x, y, z, dx, dy, dz, 1.0f, 0.0f, lifespan});
}

int ParticleSystem::size() {
	return particles.size();
}
