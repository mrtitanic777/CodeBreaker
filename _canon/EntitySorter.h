#ifndef ENTITYSORTER_H
#define ENTITYSORTER_H

// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"
#include "Comparator.h"

class Entity;
class WorldRenderer;

class EntitySorter : public Comparator {
public:
	EntitySorter(Entity* var1);
	virtual ~EntitySorter();

	Entity* baseEntity;

	virtual int a(WorldRenderer* var1, WorldRenderer* var2);
	virtual int compare(JObject var1, JObject var2);

	static void initStatics();
};

#endif // ENTITYSORTER_H