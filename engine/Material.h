
#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS

class Material {
public:
	Material();

	void use();

	float r, g, b, diffuse, specular;
};

#endif