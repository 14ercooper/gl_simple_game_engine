
#ifndef SCRIPT_CLASS
#define SCRIPT_CLASS

class Script {
public:
	Script();

	virtual void** run(void** args);

	void setDestroy(bool val);
	bool getDestroy();

protected:
	bool shouldDestroy;
};

#endif