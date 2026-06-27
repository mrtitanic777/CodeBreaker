#ifndef WORLD_H
#define WORLD_H

// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <set>
#include "EnumSkyBlock.h"
#include "IBlockAccess.h"

class AxisAlignedBB;
class Chunk;
class Class;
class Entity;
class EntityPlayer;
class Explosion;
class File;
class IChunkProvider;
class IProgressUpdate;
class IWorldAccess;
class Material;
class MovingObjectPosition;
class NBTTagCompound;
class PathEntity;
class Random;
class TileEntity;
class TreeSet;
class Vec3D;
class WorldChunkManager;
class WorldProvider;

class World : public IBlockAccess {
public:
	World(File* var1, const JavaString& var2);
	World(const JavaString& var1, WorldProvider* var2, long long var3);
	World(World* var1, WorldProvider* var2);
	World(File* var1, const JavaString& var2, long long var3);
	World(File* var1, const JavaString& var2, long long var3, WorldProvider* var5);
	virtual ~World();

	bool scheduledUpdatesAreImmediate;
	JList<void*> lightingToUpdate;
	JList<void*> loadedEntityList;
	JList<void*> unloadedEntityList;
	TreeSet* scheduledTickTreeSet;
	std::set<void*> scheduledTickSet;
	JList<void*> loadedTileEntityList;
	JList<void*> playerEntities;
	long long worldTime;
	long long fogColor;
	int skylightSubtracted;
	int updateLCG;
	int updateLCGConstant;
	bool editingBlocks;
	long long sessionLockStamp;
	int autosavePeriod;
	int difficultySetting;
	Random* rand;
	int spawnX;
	int spawnY;
	int spawnZ;
	bool isNewWorld;
	WorldProvider* worldProvider;
	JList<void*> worldAccesses;
	IChunkProvider* chunkProvider;
	File* savesDirectory;
	File* saveDirectory;
	long long randomSeed;
	NBTTagCompound* nbtCompoundPlayer;
	long long sizeOnDisk;
	JavaString worldName;
	bool findingSpawnPoint;
	JList<void*> collidingBoundingBoxes;
	int lightingUpdateDepth;
	std::set<void*> activeChunkSet;
	int ambientSoundTimer;
	JList<void*> entitiesWithinAABB;
	bool multiplayerWorld;
	static int s_lightingUpdatesScheduled;

	static NBTTagCompound* getWorldInfoNBT(File* var0, const JavaString& var1);
	static void deleteWorld(File* var0, const JavaString& var1);
	static void deleteFiles(JArray<File* > var0);
	virtual WorldChunkManager* getWorldChunkManager();
	virtual IChunkProvider* createChunkProvider(File* var1);
	virtual void findSpawnGround();
	virtual int getTopBlockId(int var1, int var2);
	virtual void func_6464_c();
	virtual void loadPlayerData(EntityPlayer* var1);
	virtual void saveWorld(bool var1, IProgressUpdate* var2);
	virtual void saveLevel();
	virtual bool quickSaveWorld(int var1);
	virtual int getBlockId(int var1, int var2, int var3);
	virtual bool blockExists(int var1, int var2, int var3);
	virtual bool checkChunksExist(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual bool chunkExists(int var1, int var2);
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
	virtual void markBlockNeedsUpdate(int var1, int var2, int var3);
	virtual void notifyBlockChange(int var1, int var2, int var3, int var4);
	virtual void markBlocksDirtyVertical(int var1, int var2, int var3, int var4);
	virtual void markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void notifyBlocksOfNeighborChange(int var1, int var2, int var3, int var4);
	virtual void notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4);
	virtual bool canBlockSeeTheSky(int var1, int var2, int var3);
	virtual int getBlockLightValue(int var1, int var2, int var3);
	virtual int getBlockLightValue(int var1, int var2, int var3, bool var4);
	virtual bool canExistingBlockSeeTheSky(int var1, int var2, int var3);
	virtual int getHeightValue(int var1, int var2);
	virtual void neighborLightPropagationChanged(int var1, int var2, int var3, int var4, int var5);
	virtual int getSavedLightValue(int var1, int var2, int var3, int var4);
	virtual void setLightValue(int var1, int var2, int var3, int var4, int var5);
	virtual float getLightBrightness(int var1, int var2, int var3);
	virtual bool isDaytime();
	virtual MovingObjectPosition* rayTraceBlocks(Vec3D* var1, Vec3D* var2);
	virtual MovingObjectPosition* rayTraceBlocks(Vec3D* var1, Vec3D* var2, bool var3);
	virtual void playSoundAtEntity(Entity* var1, const JavaString& var2, float var3, float var4);
	virtual void playSoundEffect(double var1, double var3, double var5, const JavaString& var7, float var8, float var9);
	virtual void playRecord(const JavaString& var1, int var2, int var3, int var4);
	virtual void spawnParticle(const JavaString& var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual bool entityJoinedWorld(Entity* var1);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void setEntityDead(Entity* var1);
	virtual void addWorldAccess(IWorldAccess* var1);
	virtual void removeWorldAccess(IWorldAccess* var1);
	virtual JList<void*> getCollidingBoundingBoxes(Entity* var1, AxisAlignedBB* var2);
	virtual int calculateSkylightSubtracted(float var1);
	virtual Vec3D* getSkyColor(Entity* var1, float var2);
	virtual float getCelestialAngle(float var1);
	virtual Vec3D* getFogColor(float var1);
	virtual Vec3D* getCloudColour(float var1);
	virtual int getTopSolidOrLiquidBlock(int var1, int var2);
	virtual int func_696_e(int var1, int var2);
	virtual float getRainStrength(float var1);
	virtual void scheduleBlockUpdate(int var1, int var2, int var3, int var4);
	virtual void updateEntities();
	virtual void updateEntity(Entity* var1);
	virtual void updateEntityWithOptionalForce(Entity* var1, bool var2);
	virtual bool checkIfAABBIsClear(AxisAlignedBB* var1);
	virtual bool getIsAnyLiquid(AxisAlignedBB* var1);
	virtual bool isBoundingBoxBurning(AxisAlignedBB* var1);
	virtual bool handleMaterialAcceleration(AxisAlignedBB* var1, Material* var2, Entity* var3);
	virtual bool func_689_a(AxisAlignedBB* var1, Material* var2);
	virtual bool isMaterialInBBExtended(AxisAlignedBB* var1, Material* var2);
	virtual Explosion* createExplosion(Entity* var1, double var2, double var4, double var6, float var8);
	virtual Explosion* newExplosion(Entity* var1, double var2, double var4, double var6, float var8, bool var9);
	virtual float addKey(Vec3D* var1, AxisAlignedBB* var2);
	virtual void onBlockHit(int var1, int var2, int var3, int var4);
	virtual Entity* findEntityByClass(Class* var1);
	virtual JavaString getDebugLoadedEntities();
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3);
	virtual void setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual void removeBlockTileEntity(int var1, int var2, int var3);
	virtual bool isBlockOpaqueCube(int var1, int var2, int var3);
	virtual void saveWorldIndicator(IProgressUpdate* var1);
	virtual bool updatingLighting();
	virtual void scheduleLightingUpdate(int var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual void scheduleLightingUpdateInternal(int var1, int var2, int var3, int var4, int var5, int var6, int var7, bool var8);
	virtual void calculateInitialSkylight();
	virtual void tick();
	virtual void updateBlocksAroundPlayers();
	virtual bool TickUpdates(bool var1);
	virtual void randomDisplayUpdates(int var1, int var2, int var3);
	virtual JList<void*> getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB* var2);
	virtual JList<void*> getEntitiesWithinAABB(Class* var1, AxisAlignedBB* var2);
	virtual JList<void*> getLoadedEntityList();
	virtual void markTileEntityForUpdate(int var1, int var2, int var3, TileEntity* var4);
	virtual int countEntities(Class* var1);
	virtual void addLoadedEntities(JList<void*> var1);
	virtual void unloadEntities(JList<void*> var1);
	virtual void loadAllChunks();
	virtual bool canBlockBePlacedAt(int var1, int var2, int var3, int var4, bool var5);
	virtual PathEntity* getPathToEntity(Entity* var1, Entity* var2, float var3);
	virtual PathEntity* getEntityPathToXYZ(Entity* var1, int var2, int var3, int var4, float var5);
	virtual bool isBlockProvidingPowerTo(int var1, int var2, int var3, int var4);
	virtual bool isBlockGettingPowered(int var1, int var2, int var3);
	virtual bool isBlockIndirectlyProvidingPowerTo(int var1, int var2, int var3, int var4);
	virtual bool isBlockIndirectlyGettingPowered(int var1, int var2, int var3);
	virtual EntityPlayer* getClosestPlayerToEntity(Entity* var1, double var2);
	virtual EntityPlayer* getClosestPlayer(double var1, double var3, double var5, double var7);
	virtual void setChunkData(int var1, int var2, int var3, int var4, int var5, int var6, JArray<unsigned char > var7);
	virtual void sendQuittingDisconnectingPacket();
	virtual void checkSessionLock();
	virtual void setWorldTime(long long var1);
	virtual void joinEntityInSurroundings(Entity* var1);
	virtual bool canMineBlock(EntityPlayer* var1, int var2, int var3, int var4);
	virtual void setEntityState(Entity* var1, unsigned char var2);
	virtual void releaseUnloadedEntities();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	World(const World&);
	World& operator=(const World&);
};

#endif // WORLD_H