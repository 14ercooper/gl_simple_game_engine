
#include "Material.h"

Material::Material() {
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	diffuse = 1.0f;
	specular = 0.5f;
}

void Material::use() {
	// This does nothing for now
}
