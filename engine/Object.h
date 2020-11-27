
#ifndef OBJECT_CLASS
#define OBJECT_CLASS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <set>
#include <string>

#include "Material.h"
#include "Script.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "math/Quaternion.h"

class Collider;

// This is a basic object that can be added to the game. All other objects should extend from this class.
// It provides a lot of useful object functions, and can draw an arbitary thing by overloading draw in the child (default draw nothing)
class Object {
public:
	// Create a new object
	Object();
	~Object();

	// Store the current transform of this object in 3D space
	glm::vec3 position;
	Quaternion* rotation;
	glm::vec3 currentScale;
	glm::vec3 velocity; // useful for physics

	// Update the transform of this object
	void translate(glm::vec3 amount);
	int quarterstepTranslate(glm::vec3 amount); // Checks for collisions before moving, stopping if one is found. Returns the number of quartersteps moved.
	void rotate(Quaternion* rotation);
	void rotate(float theta, float x, float y, float z);
	void scale(glm::vec3 amount);
	void addVelocity(glm::vec3 amount);

	// Set and get the shader that is associated with this object
	// Delete old will delete the existing object if set to true
	void setShader(ShaderProgram* program, bool deleteOld);
	ShaderProgram* getShader();

	// Set materials, colliders, or models
	void setMaterial(Material* m, bool deleteOld);
	void setCollider(Collider* c, bool deleteOld);
	void setModel(Model* m, bool deleteOld);

	// Get materials, colliders, or models
	Material* getMaterial();
	Collider* getCollider();
	Model* getModel();

	// Provide a new script for this object for one of the three scriptable stages
	void setControlTick(Script* s, bool deleteOld);
	void setPhysicsTick(Script* s, bool deleteOld);
	void setPostTick(Script* s, bool deleteOld);

	// Run the given script
	void controlTick();
	void physicsTick();
	void postTick();

	// Draw this object to the screen. Defaults to doing nothing unless overloaded in a child class
	virtual void draw();

	// Should we destroy this object?
	void setDestroy(bool val);
	bool getDestroy();

	// Get a model matrix transformation corresponding to this object's transform
	glm::mat4 getModelMatrix();

	// Set or get the parent of the object. The object is draw heirarcichally with respect to their parent
	// The default is nullptr (no parent)
	void setParent(Object* newParent, bool destroyOld);
	Object* getParent();

	// Add, remove, or test for a tag on this object
	void addTag(std::string tag);
	void removeTag(std::string tag);
	bool hasTag(std::string tag);

protected:
	// The associated shader
	ShaderProgram* shaderProgram;

	// Object's parent
	Object* parent;

	// Object's material, collider, and model
	Material* material;
	Collider* collider;
	Model* model;

	// Object's attached scripts
	Script* physicsTickScript;
	Script* controlTickScript;
	Script* postTickScript;

	// Returns from the scripts. Currently unused.
	void** physicsTickData;
	void** controlTickData;
	void** postTickData;

	// Should we destroy this object?
	bool shouldDestroy;

	// What tags do we have?
	std::set<std::string> tags;
};

// This just feels hacky
#include "Collider.h"
#include "GameEngine.h"

#endif
