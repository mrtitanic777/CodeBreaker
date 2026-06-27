#ifndef RENDERSORTER_H
#define RENDERSORTER_H

// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"
#include "Comparator.h"

class EntityPlayer;
class WorldRenderer;

class RenderSorter : public Comparator {
public:
	RenderSorter(EntityPlayer* var1);
	virtual ~RenderSorter();

	EntityPlayer* entityPlayer;

	virtual int a(WorldRenderer* var1, WorldRenderer* var2);
	virtual int compare(JObject var1, JObject var2);

	static void initStatics();
};

#endif // RENDERSORTER_H