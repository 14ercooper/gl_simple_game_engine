
#ifndef LIGHT_CLASS
#define LIGHT_CLASS

// Supported light types
enum LIGHT_TYPE {
	AMBIENT,
	POINT,
	DIRECTIONAL
};

// A light that can be placed into the world
class Light {
public:
	// Create lights
	// Intensity gives how bright it is
	// XYZ is either position in space for point or direction of light for directional
	Light(LIGHT_TYPE type, float r, float g, float b, float intensity);
	Light(LIGHT_TYPE type, float r, float g, float b, float intensity, float x, float y, float z);

	// Stores the information about this light
	LIGHT_TYPE type;
	float r, g, b, intensity, x, y, z;

	// Should this light be despawned?
	void setDestroy(bool value);
	bool getDestroy();

private:
	// Should this light be removed?
	bool doDestroy;
};

#endif
