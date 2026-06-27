#ifndef PATHFINDER_H
#define PATHFINDER_H

// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class IBlockAccess;
class MCHashTable;
class Path;
class PathEntity;
class PathPoint;

class Pathfinder {
public:
	Pathfinder(IBlockAccess* var1);
	virtual ~Pathfinder();

	IBlockAccess* worldMap;
	Path* path;
	MCHashTable* pointMap;
	JArray<PathPoint* > pathOptions;

	virtual PathEntity* createEntityPathTo(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5);
	virtual PathEntity* createEntityPathTo(Entity* var1, double var2, double var4, double var6, float var8);
	virtual PathEntity* addToPath(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5);
	virtual int findPathOptions(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5);
	virtual PathPoint* getSafePoint(Entity* var1, int var2, int var3, int var4, PathPoint* var5, int var6);
	virtual PathPoint* openPoint(int var1, int var2, int var3);
	virtual int getVerticalOffset(Entity* var1, int var2, int var3, int var4, PathPoint* var5);
	virtual PathEntity* createEntityPath(PathPoint* var1, PathPoint* var2);

	static void initStatics();
};

#endif // PATHFINDER_H