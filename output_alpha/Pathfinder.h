#ifndef PATHFINDER_H
#define PATHFINDER_H

// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class IBlockAccess;
class MCHashTable;
class Path;
class PathEntity;
class PathPoint;

class Pathfinder {
public:
	Pathfinder();
	Pathfinder(IBlockAccess* var1);
	virtual ~Pathfinder();

	virtual PathEntity* createEntityPathTo(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5);

public: // was private
	IBlockAccess* m_worldMap;
	Path* m_path;
	MCHashTable* m_pointMap;
	PathPoint** m_pathOptions;

	PathEntity* createEntityPathTo(Entity* var1, double var2, double var4, double var6, float var8);
	PathEntity* addToPath(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5);
	int findPathOptions(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5);
	PathPoint* getSafePoint(Entity* var1, int var2, int var3, int var4, PathPoint* var5, int var6);
	PathPoint* openPoint(int var1, int var2, int var3);
	int getVerticalOffset(Entity* var1, int var2, int var3, int var4, PathPoint* var5);
	PathEntity* createEntityPath(PathPoint* var1, PathPoint* var2);

public:  // array-length companions
	int m_pathOptions_count;

};

#endif // PATHFINDER_H