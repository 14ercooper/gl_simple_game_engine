
#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT

// Vertex with normals for built in shaders
struct Vertex {
	float x, y, z;
	float nX, nY, nZ;
};

// Vertex with textures for built in shaders
struct VertexTextured {
	float x, y, z;
	float nX, nY, nZ;
	float u, v;	
};

#endif
