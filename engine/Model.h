
#ifndef MODEL_CLASS
#define MODEL_CLASS

// The base class for any engine models
// Default has nothing
class Model {
public:
	Model();
	virtual ~Model();

	// Draw this model. This can be overloaded to define a custom model.
	virtual void draw();
};

#endif
