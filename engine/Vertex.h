
#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT

struct Vertex {
	float x, y, z;
	float nX, nY, nZ;
};

struct VertexTextured {
	float x, y, z;
	float nX, nY, nZ;
	float u, v;	
};

#endif
