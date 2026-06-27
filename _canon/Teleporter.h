#ifndef TELEPORTER_H
#define TELEPORTER_H

// Auto-generated from Teleporter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class Random;
class World;

class Teleporter {
public:
	Teleporter();
	virtual ~Teleporter();

	Random* random;

	virtual void placeInPortal(World* var1, Entity* var2);
	virtual bool placeInExistingPortal(World* var1, Entity* var2);
	virtual bool makePortal(World* var1, Entity* var2);

	static void initStatics();
};

#endif // TELEPORTER_H