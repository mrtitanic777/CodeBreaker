#ifndef PATHFINDER_H
#define PATHFINDER_H

// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class Map;
class Path;
class PathEntity;
class PathPoint;
class World;

class Pathfinder {
public:
	Pathfinder();
	Pathfinder(World* var1);
	virtual ~Pathfinder();

	virtual PathEntity* createEntityPathTo(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5);

public: // was private
	World* m_worldMap;
	Path* m_path;
	std::map<int, void*>* m_pointMap;
	PathPoint** m_pathOptions;

	PathEntity* addToPath(Entity* var1, float var2, float var3, float var4, float var5);
	PathPoint* a(Entity* var1, int var2, int var3, int var4, PathPoint* var5, int var6);
	PathPoint* openPoint(int var1, int var2, int var3);
	int getVerticalOffset(int var1, int var2, int var3, PathPoint* var4);
	PathEntity* createEntityPath(PathPoint* var0);

public:  // array-length companions
	int m_pathOptions_count;

};

#endif // PATHFINDER_H