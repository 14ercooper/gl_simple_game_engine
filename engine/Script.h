
#ifndef SCRIPT_CLASS
#define SCRIPT_CLASS

// These can be attached to objects or the game engine to run every frame
// Control ticks run first, then physics, then the scene is drawn, then post ticks, then engine ticks.
class Script {
public:
	// Create a new script
	Script();
	virtual ~Script();

	// Run the script. Default is no action.
	// The void** allows for passing of an array of arbitrary pointers
	virtual void** run(void** args);

	// Should we destroy this script (used by engine scripts to delete and detatch)
	void setDestroy(bool val);
	bool getDestroy();

protected:
	// Track if we should destroy
	bool shouldDestroy;
};

#endif