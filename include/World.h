#ifndef WORLD_H
#define WORLD_H

// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

struct AxisAlignedBB;
class Class;
class Entity;
class EntityLiving;
class EntityMap;
class IWorldAccess;
class Light;
class Map;
class Material;
struct MovingObjectPosition;
class Pathfinder;
class Random;
class TileEntity;
struct Vec3D;

class World {
public:
	World();
	virtual ~World();

	int width;
	int m_length;
	int height;
	unsigned char* m_d;
	unsigned char* m_e;
	std::string m_name;
	std::string m_authorName;
	long long m_createTime;
	int m_xSpawn;
	int m_ySpawn;
	int m_zSpawn;
	float m_rotSpawn;
	int m_defaultFluid;
	Map* m_map;
	Random* m_random;
	EntityMap* m_entityMap;
	int m_waterLevel;
	int m_groundLevel;
	int m_cloudHeight;
	int m_skyColor;
	int m_fogColor;
	int m_cloudColor;
	EntityLiving* m_playerEntity;
	bool m_survivalWorld;
	int m_skyBrightness;
	int m_skylightSubtracted;
	Pathfinder* m_pathFinder;
	int m_worldTime;
	int m_difficultySetting;

	virtual void load();
	virtual void generate(int var1, int var2, int var3, unsigned char* var4);
	virtual void a(int var1, int var2, int var3, unsigned char* var4, unsigned char* var5);
	virtual void findSpawn();
	virtual void addWorldAccess(IWorldAccess* var1);
	virtual void finalize();
	virtual void removeWorldAccess(IWorldAccess* var1);
	virtual std::vector<void*> /* PORT: determine element type */ getCollidingBoundingBoxes(const AxisAlignedBB& var1);
	virtual void swap(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual bool setBlock(int var1, int var2, int var3, int var4);
	virtual bool setBlockWithNotify(int var1, int var2, int var3, int var4);
	virtual void notifyBlocksOfNeighborChange(int var1, int var2, int var3, int var4);
	virtual bool setTileNoUpdate(int var1, int var2, int var3, int var4);
	virtual int getBlockId(int var1, int var2, int var3);
	virtual bool isBlockNormalCube(int var1, int var2, int var3);
	virtual void updateEntities();
	virtual void updateLighting();
	virtual float getStarBrightness(float var1);
	virtual Vec3D getSkyColor(float var1);
	virtual float getCelestialAngle(float var1);
	virtual Vec3D getFogColor(float var1);
	virtual Vec3D getCloudColor(float var1);
	virtual int getSkyBrightness();
	virtual void tick();
	virtual int entitiesInLevelList(Class* var1);
	virtual int getGroundLevel();
	virtual int getWaterLevel();
	virtual bool getIsAnyLiquid(const AxisAlignedBB& var1);
	virtual bool isBoundingBoxBurning(const AxisAlignedBB& var1);
	virtual bool handleMaterialAcceleration(const AxisAlignedBB& var1, Material* var2);
	virtual void scheduleBlockUpdate(int var1, int var2, int var3, int var4);
	virtual bool checkIfAABBIsClear1(const AxisAlignedBB& var1);
	virtual bool checkIfAABBIsClear(const AxisAlignedBB& var1);
	virtual std::vector<void*> /* PORT: determine element type */ a(Entity* var1, const AxisAlignedBB& var2);
	virtual bool isSolid(float var1, float var2, float var3, float var4);
	virtual void setSpawnLocation(int var1, int var2, int var3, float var4);
	virtual float getLightBrightness(int var1, int var2, int var3);
	virtual unsigned char getBlockLightValue(int var1, int var2, int var3);
	virtual unsigned char getBlockMetadata(int var1, int var2, int var3);
	virtual void setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual Material* getBlockMaterial(int var1, int var2, int var3);
	virtual bool isWater(int var1, int var2, int var3);
	virtual MovingObjectPosition rayTraceBlocks(const Vec3D& var1, const Vec3D& var2);
	virtual bool growTrees(int var1, int var2, int var3);
	virtual Entity* getPlayerEntity();
	virtual void spawnEntityInWorld(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void createExplosion(Entity* var1, float var2, float var3, float var4, float var5);
	virtual Entity* findSubclassOf(Class* var1);
	virtual int a(int var1, int var2);
	virtual int fluidFlowCheck(int var1, int var2, int var3, int var4, int var5);
	virtual int floodFill(int var1, int var2, int var3, int var4, int var5);
	virtual void a(Entity* var1, const std::string& var2, float var3, float var4);
	virtual void playSoundEffect(float var1, float var2, float var3, const std::string& var4, float var5);
	virtual void playSoundAtPlayer(float var1, float var2, float var3, const std::string& var4, float var5, float var6);
	virtual void extinguishFire(int var1, int var2, int var3, int var4);
	virtual void setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual void removeBlockTileEntity(int var1, int var2, int var3);
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3);
	virtual void spawnParticle(const std::string& var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void randomDisplayUpdates(int var1, int var2, int var3);
	virtual const std::string& debugSkylightUpdates();
	virtual void setLevel();
	virtual bool canBlockSeeTheSky(int var1, int var2, int var3);

public: // was private (Java package-private)
	static float* s_lightBrightnessTable;
	std::vector<void*> /* PORT: determine element type */ m_worldAccesses;
	std::vector<void*> /* PORT: determine element type */ m_tickList;
	int* m_heightMap;
	Random* rand;
	int m_randId;
	int m_updateLCG;
	int m_playTime;
	Light* m_L;
	static short s_floodFillCounter;
	short* m_floodFillCounters;
	int* m_coords;
	int* m_floodedBlocks;

	void l();
	void notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4);
	bool isSolid(float var1, float var2, float var3);
	int getFirstUncoveredBlock(int var1, int var2);
	float getBlockDensity(const Vec3D& var1, const AxisAlignedBB& var2);
	void updateChunkLight(int var1);

};

#endif // WORLD_H