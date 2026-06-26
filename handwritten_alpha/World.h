#ifndef WORLD_H
#define WORLD_H

// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: java.util.Set / java.util.TreeSet have no header on 360.
//  - m_scheduledTickTreeSet (TreeSet<NextTickListEntry>) -> std::vector<NextTickListEntry*>*
//    (kept ordered by NextTickListEntry::a()/compareTo when popping the soonest tick).
//  - m_scheduledTickSet (Set<NextTickListEntry>) -> std::vector<NextTickListEntry*>* (contains-check).
//  - m_field_9427_K (Set<ChunkCoordIntPair>) -> std::vector<void*>*.

#include "IBlockAccess.h"
#include "AxisAlignedBB.h"  // PORT: m_field_9428_I is now std::vector<AxisAlignedBB> (value member needs complete type)
#include <string>
#include <vector>
class Chunk;
class ChunkCoordIntPair;
class Class;
class Entity;
class EntityPlayer;
struct EnumSkyBlock;
class Explosion;
class IChunkProvider;
class IProgressUpdate;
class IWorldAccess;
class Material;
class MovingObjectPosition;
class NBTTagCompound;
class NextTickListEntry;
class PathEntity;
class Random;
class TileEntity;
class Vec3D;
class WorldChunkManager;
class WorldProvider;

class World : public IBlockAccess {
public:
	World();
	World(std::string /* PORT: was java.io.File */* var1, const std::string& var2);
	World(const std::string& var1, WorldProvider* var2, long long var3);
	World(World* var1, WorldProvider* var2);
	World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3);
	World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5);
	virtual ~World();

	bool m_field_4214_a;
	std::vector<void*> /* PORT: determine element type */ m_loadedEntityList;
	std::vector<void*> /* PORT: determine element type */ m_loadedTileEntityList;
	std::vector<void*> /* PORT: determine element type */ m_playerEntities;
	long long m_worldTime;
	int m_skylightSubtracted;
	bool m_field_1043_h;
	int m_difficultySetting;
	Random* rand;
	int m_spawnX;
	int m_spawnY;
	int m_spawnZ;
	bool m_field_1033_r;
	WorldProvider* m_worldProvider;
	std::string /* PORT: was java.io.File */* m_field_9433_s;
	std::string /* PORT: was java.io.File */* m_field_9432_t;
	long long m_randomSeed;
	long long m_sizeOnDisk;
	std::string m_field_9431_w;
	bool m_field_9430_x;
	bool m_multiplayerWorld;

	static NBTTagCompound* func_629_a(std::string /* PORT: was java.io.File */* var0, const std::string& var1);
	static void deleteWorld(std::string /* PORT: was java.io.File */* var0, const std::string& var1);
	virtual WorldChunkManager* func_4075_a();
	virtual void func_4076_b();
	virtual int func_614_g(int var1, int var2);
	virtual void func_6464_c();
	virtual void func_608_a(EntityPlayer* var1);
	virtual void saveWorld(bool var1, IProgressUpdate* var2);
	virtual bool func_650_a(int var1);
	virtual int getBlockId(int var1, int var2, int var3);
	virtual bool blockExists(int var1, int var2, int var3);
	virtual bool checkChunksExist(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual Chunk* getChunkFromBlockCoords(int var1, int var2);
	virtual Chunk* getChunkFromChunkCoords(int var1, int var2);
	virtual bool setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5);
	virtual bool setBlock(int var1, int var2, int var3, int var4);
	virtual Material* getBlockMaterial(int var1, int var2, int var3);
	virtual int getBlockMetadata(int var1, int var2, int var3);
	virtual void setBlockMetadataWithNotify(int var1, int var2, int var3, int var4);
	virtual bool setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual bool setBlockWithNotify(int var1, int var2, int var3, int var4);
	virtual bool setBlockAndMetadataWithNotify(int var1, int var2, int var3, int var4, int var5);
	virtual void func_665_h(int var1, int var2, int var3);
	virtual void func_680_f(int var1, int var2, int var3, int var4);
	virtual void func_701_b(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void notifyBlocksOfNeighborChange(int var1, int var2, int var3, int var4);
	virtual bool canBlockSeeTheSky(int var1, int var2, int var3);
	virtual int getBlockLightValue(int var1, int var2, int var3);
	virtual int getBlockLightValue(int var1, int var2, int var3, bool var4);
	virtual bool canExistingBlockSeeTheSky(int var1, int var2, int var3);
	virtual int getHeightValue(int var1, int var2);
	virtual void neighborLightPropagationChanged(EnumSkyBlock* var1, int var2, int var3, int var4, int var5);
	virtual int getSavedLightValue(EnumSkyBlock* var1, int var2, int var3, int var4);
	virtual void setLightValue(EnumSkyBlock* var1, int var2, int var3, int var4, int var5);
	virtual float getLightBrightness(int var1, int var2, int var3);
	virtual bool isDaytime();
	virtual MovingObjectPosition* rayTraceBlocks(Vec3D* var1, Vec3D* var2);
	virtual MovingObjectPosition* rayTraceBlocks(Vec3D* var1, Vec3D* var2, bool var3);
	virtual void playSoundAtEntity(Entity* var1, const std::string& var2, float var3, float var4);
	virtual void playSoundEffect(double var1, double var3, double var5, const std::string& var7, float var8, float var9);
	virtual void playRecord(const std::string& var1, int var2, int var3, int var4);
	virtual void spawnParticle(const std::string& var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual bool entityJoinedWorld(Entity* var1);
	virtual void setEntityDead(Entity* var1);
	virtual void addWorldAccess(IWorldAccess* var1);
	virtual void removeWorldAccess(IWorldAccess* var1);
	virtual std::vector<AxisAlignedBB> getCollidingBoundingBoxes(Entity* var1, AxisAlignedBB var2);
	virtual int calculateSkylightSubtracted(float var1);
	virtual Vec3D* func_4079_a(Entity* var1, float var2);
	virtual float getCelestialAngle(float var1);
	virtual Vec3D* func_628_d(float var1);
	virtual Vec3D* func_4082_d(float var1);
	virtual int func_4083_e(int var1, int var2);
	virtual int func_696_e(int var1, int var2);
	virtual float func_679_f(float var1);
	virtual void scheduleBlockUpdate(int var1, int var2, int var3, int var4);
	virtual void func_633_c();
	virtual void func_667_e(Entity* var1);
	virtual void func_4084_a(Entity* var1, bool var2);
	virtual bool checkIfAABBIsClear(AxisAlignedBB var1);
	virtual bool getIsAnyLiquid(AxisAlignedBB var1);
	virtual bool isBoundingBoxBurning(AxisAlignedBB var1);
	virtual bool func_682_a(AxisAlignedBB var1, Material* var2, Entity* var3);
	virtual bool func_689_a(AxisAlignedBB var1, Material* var2);
	virtual bool func_707_b(AxisAlignedBB var1, Material* var2);
	virtual Explosion* func_12243_a(Entity* var1, double var2, double var4, double var6, float var8);
	virtual Explosion* func_12244_a(Entity* var1, double var2, double var4, double var6, float var8, bool var9);
	virtual float func_675_a(Vec3D* var1, AxisAlignedBB var2);
	virtual void onBlockHit(int var1, int var2, int var3, int var4);
	virtual Entity* func_4085_a(void* var1);
	virtual std::string func_687_d();
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3);
	virtual void setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual void removeBlockTileEntity(int var1, int var2, int var3);
	virtual bool isBlockOpaqueCube(int var1, int var2, int var3);
	virtual void func_651_a(IProgressUpdate* var1);
	virtual bool func_6465_g();
	virtual void func_616_a(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual void func_627_a(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7, bool var8);
	virtual void calculateInitialSkylight();
	virtual void tick();
	virtual bool TickUpdates(bool var1);
	virtual void randomDisplayUpdates(int var1, int var2, int var3);
	virtual std::vector<void*> /* PORT: determine element type */ getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2);
	virtual std::vector<void*> /* PORT: determine element type */ getEntitiesWithinAABB(void* var1, AxisAlignedBB var2);
	virtual std::vector<void*> /* PORT: determine element type */ func_658_i();
	virtual void func_698_b(int var1, int var2, int var3, TileEntity* var4);
	virtual int countEntities(void* var1);
	virtual void func_636_a(std::vector<void*> /* PORT: determine element type */ var1);
	virtual void func_632_b(std::vector<void*> /* PORT: determine element type */ var1);
	virtual void func_656_j();
	virtual bool canBlockBePlacedAt(int var1, int var2, int var3, int var4, bool var5);
	virtual PathEntity* getPathToEntity(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* getEntityPathToXYZ(Entity* var1, int var2, int var3, int var4, float var5);
	virtual bool isBlockProvidingPowerTo(int var1, int var2, int var3, int var4);
	virtual bool isBlockGettingPowered(int var1, int var2, int var3);
	virtual bool isBlockIndirectlyProvidingPowerTo(int var1, int var2, int var3, int var4);
	virtual bool isBlockIndirectlyGettingPowered(int var1, int var2, int var3);
	virtual EntityPlayer* getClosestPlayerToEntity(Entity* var1, double var2);
	virtual EntityPlayer* getClosestPlayer(double var1, double var3, double var5, double var7);
	virtual void func_693_a(int var1, int var2, int var3, int var4, int var5, int var6, unsigned char* var7);
	virtual void sendQuittingDisconnectingPacket();
	virtual void func_663_l();
	virtual void setWorldTime(long long var1);
	virtual void func_705_f(Entity* var1);
	virtual bool func_6466_a(EntityPlayer* var1, int var2, int var3, int var4);
	virtual void func_9425_a(Entity* var1, unsigned char var2);
	virtual void func_9424_o();

public: // was protected
	int m_field_9437_g;
	int m_field_9436_h;
	int m_autosavePeriod;
	std::vector<IWorldAccess*> m_worldAccesses;

	virtual IChunkProvider* func_4081_a(std::string /* PORT: was java.io.File */* var1);
	virtual void notifyBlockChange(int var1, int var2, int var3, int var4);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void func_4080_j();

public: // was private
	std::vector<void*> /* PORT: determine element type */ m_field_1051_z;
	std::vector<void*> /* PORT: determine element type */ m_field_1024_A;
	std::vector<NextTickListEntry*>* m_scheduledTickTreeSet;  // was TreeSet
	std::vector<NextTickListEntry*>* m_scheduledTickSet;      // was Set
	long long m_field_1019_F;
	long long m_field_1054_E;
	IChunkProvider* m_chunkProvider;
	NBTTagCompound* m_nbtCompoundPlayer;
	std::vector<AxisAlignedBB> m_field_9428_I;  // PORT: scratch list for Block::getCollidingBoundingBoxes (value AABBs)
	int m_field_4204_J;
	static int s_field_9429_y;
	std::vector<void*>* m_field_9427_K;                       // was Set
	int m_field_9426_L;
	std::vector<void*> /* PORT: determine element type */ m_field_1012_M;

	void deleteFiles(std::string /* PORT: was java.io.File */* var0);
	void saveLevel();
	bool chunkExists(int var1, int var2);
	void notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4);

};

#endif // WORLD_H
