
#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS

// Define a simple material for shaders
class Material {
public:
	// Create a blank material. Default matte black.
	Material();

	// For future use (does nothing)
	void use();

	// The 5 values that the material stores
	float r, g, b, diffuse, specular;
};

#endif