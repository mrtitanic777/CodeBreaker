#ifndef PATHFINDER_H
#define PATHFINDER_H

// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathPoint.h"

class Entity;
class Map;
class Path;
class PathEntity;
class World;

class Pathfinder {
public:
	Pathfinder();
	Pathfinder(World* var1);
	virtual ~Pathfinder();

	virtual PathEntity* createEntityPathTo(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5);

private:
	World* m_worldMap;
	Path* m_path;
	Map* m_pointMap;
	PathPoint* m_pathOptions;

	PathEntity* addToPath(Entity* var1, float var2, float var3, float var4, float var5);
	PathPoint a(Entity* var1, int var2, int var3, int var4, const PathPoint& var5, int var6);
	PathPoint openPoint(int var1, int var2, int var3);
	int getVerticalOffset(int var1, int var2, int var3, const PathPoint& var4);
	PathEntity* createEntityPath(const PathPoint& var0);

};

#endif // PATHFINDER_H