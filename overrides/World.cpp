#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "World.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFluids.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkCache.h"
#include "ChunkCoordIntPair.h"
#include "ChunkPosition.h"
#include "ChunkProviderLoadOrGenerate.h"
#include "Class.h"
#include "CompressedStreamTools.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Dimension.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumSkyBlock.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FileInputStream.h"
#include "FileOutputStream.h"
#include "HashEntry.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "IProgressUpdate.h"
#include "IWorldAccess.h"
#include "IllegalStateException.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "MCHashTable.h"
#include "Material.h"
#include "MathHelper.h"
#include "MetadataChunkBlock.h"
#include "MinecraftException.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NextTickListEntry.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "Path.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Pathfinder.h"
#include "Random.h"
#include "RuntimeException.h"
#include "SpawnerAnimals.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldProviderHell.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java called java.lang.Double.isInfinite(double) (e.g. in updateEntityWithOptionalForce):
 *      if(Double.isNaN(var1.posX) || Double.isInfinite(var1.posX)) {
 *          var1.posX = var1.lastTickPosX;
 *      }
 *      ... (same for posY, posZ, rotationPitch, rotationYaw)
 *  The runtime Double namespace lacks isInfinite, so this local shim stands in for it.
 *  ---- end original Java ---- */
// --- local shim: Double.isInfinite (runtime Double namespace lacks it) -------
namespace Double {
	inline bool isInfinite(double v) {
		// +/-inf is the only finite-looking value that equals twice itself.
		return v == v && v != 0.0 && v == v * 2.0;
	}
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java used java.util.TreeSet (a sorted set ordered by NextTickListEntry.compareTo):
 *      private TreeSet scheduledTickTreeSet;
 *      ...
 *      this.scheduledTickTreeSet = new TreeSet();
 *      ...
 *      this.scheduledTickTreeSet.add(var5);          // scheduleBlockUpdate
 *      ...
 *      int var2 = this.scheduledTickTreeSet.size();   // TickUpdates
 *      NextTickListEntry var4 = (NextTickListEntry)this.scheduledTickTreeSet.first();
 *      this.scheduledTickTreeSet.remove(var4);
 *      return this.scheduledTickTreeSet.size() != 0;
 *  No runtime TreeSet exists, so this minimal local class stands in for it (std::set with a
 *  comparator that calls NextTickListEntry::compareTo). first() == smallest by that ordering.
 *  ---- end original Java ---- */
// --- local shim: java.util.TreeSet of NextTickListEntry* ---------------------
// World uses a TreeSet ordered by NextTickListEntry::compareTo (scheduledTime,
// then tickEntryID). No runtime TreeSet stub exists, so we provide a minimal
// faithful one backed by std::set with a comparator that calls compareTo().
struct NextTickListEntryComparator {
	bool operator()(NextTickListEntry* a, NextTickListEntry* b) const {
		return a->compareTo((void*)b) < 0;
	}
};

class TreeSet {
public:
	std::set<NextTickListEntry*, NextTickListEntryComparator> s;
	void add(void* var1) { s.insert((NextTickListEntry*)var1); }
	void remove(void* var1) { s.erase((NextTickListEntry*)var1); }
	int size() { return (int)s.size(); }
	void* first() { return s.empty() ? (void*)NULL : (void*)*s.begin(); }
};

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java called java.io.File.delete() and File.listFiles() directly. The spots these helpers
 *  stand in for:
 *
 *  public static void deleteWorld(File var0, String var1) {
 *      File var2 = new File(var0, "saves");
 *      File var3 = new File(var2, var1);
 *      if(var3.exists()) {
 *          deleteFiles(var3.listFiles());
 *          var3.delete();
 *      }
 *  }
 *  private static void deleteFiles(File[] var0) {
 *      for(int var1 = 0; var1 < var0.length; ++var1) {
 *          if(var0[var1].isDirectory()) {
 *              deleteFiles(var0[var1].listFiles());
 *          }
 *          var0[var1].delete();
 *      }
 *  }
 *  // and in saveLevel(): var5.delete(); var6.delete(); var4.delete();  (level.dat rotation)
 *
 *  NOTE - BEHAVIOUR NOT REPRODUCED: fileDelete() does NOT delete anything (it just returns
 *  f != NULL), and fileListFiles() always yields an empty array because the File runtime stub's
 *  listFiles() returns NULL. So world deletion and level.dat_old/_new rotation are no-ops.
 *  ---- end original Java ---- */
// --- local shims for java.io.File interop ------------------------------------
// The File runtime stub exposes neither delete() (a C++ keyword) nor a typed
// listFiles(). These helpers bridge that gap locally; see report's runtime-stub
// gaps for the central additions that would remove them.
static bool fileDelete(File* f) { return (f != NULL); }
static JArray<File*> fileListFiles(File* f) {
	void* p = (f != NULL) ? f->listFiles() : (void*)NULL;
	// stub returns NULL (no children); wrap as a zero-length File* array.
	if (p == NULL) return JArray<File*>();
	return JArray<File*>((File**)p, 0);
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java used java.util.List.removeAll (in updateEntities / the load+unload cycle):
 *      this.loadedEntityList.removeAll(this.unloadedEntityList);
 *  JList carries no removeAll, so this local helper stands in for it (remove every element of
 *  dst that is present in src). Behaviour-equivalent shim.
 *  ---- end original Java ---- */
// java.util.List.removeAll(c): remove every element of `dst` that is in `src`.
static void listRemoveAll(JList<void*>& dst, const JList<void*>& src) {
	for (size_t i = 0; i < src.size(); ++i) {
		while (dst.remove(src[i])) { /* remove all occurrences */ }
	}
}

int World::s_lightingUpdatesScheduled = 0;

World::World(File* var1, const JavaString& var2)
	: scheduledUpdatesAreImmediate(0)
	, scheduledTickTreeSet(NULL)
	, worldTime(0)
	, fogColor(0)
	, skylightSubtracted(0)
	, updateLCG(0)
	, updateLCGConstant(0)
	, editingBlocks(0)
	, sessionLockStamp(0)
	, autosavePeriod(0)
	, difficultySetting(0)
	, rand(NULL)
	, spawnX(0)
	, spawnY(0)
	, spawnZ(0)
	, isNewWorld(0)
	, worldProvider(NULL)
	, chunkProvider(NULL)
	, savesDirectory(NULL)
	, saveDirectory(NULL)
	, randomSeed(0)
	, nbtCompoundPlayer(NULL)
	, sizeOnDisk(0)
	, findingSpawnPoint(0)
	, lightingUpdateDepth(0)
	, ambientSoundTimer(0)
	, multiplayerWorld(0)
{
	// inlined this(var1, var2, (new Random())->nextLong()) -- C++03 lacks ctor delegation
	this->scheduledUpdatesAreImmediate = false;
	this->lightingToUpdate = JList<void*>();
	this->loadedEntityList = JList<void*>();
	this->unloadedEntityList = JList<void*>();
	this->scheduledTickTreeSet = new TreeSet();
	this->scheduledTickSet = std::set<void*>();
	this->loadedTileEntityList = JList<void*>();
	this->playerEntities = JList<void*>();
	this->worldTime = 0LL;
	this->fogColor = 16777215LL;
	this->skylightSubtracted = 0;
	this->updateLCG = (new Random())->nextInt();
	this->updateLCGConstant = 1013904223;
	this->editingBlocks = false;
	this->sessionLockStamp = System::currentTimeMillis();
	this->autosavePeriod = 40;
	this->rand = new Random();
	this->isNewWorld = false;
	this->worldAccesses = JList<void*>();
	this->randomSeed = 0LL;
	this->sizeOnDisk = 0LL;
	this->collidingBoundingBoxes = JList<void*>();
	this->lightingUpdateDepth = 0;
	this->activeChunkSet = std::set<void*>();
	this->ambientSoundTimer = this->rand->nextInt(12000);
	this->entitiesWithinAABB = JList<void*>();
	this->multiplayerWorld = false;
	// inlined this(var1, var2, (new Random())->nextLong()) -- delegates to the
	// World(File, String, long) constructor below; reproduced faithfully here.
	{
	long long var3 = (new Random())->nextLong();
	this->savesDirectory = var1;
	this->worldName = var2;
	var1->mkdirs();
	this->saveDirectory = new File(var1, var2);
	this->saveDirectory->mkdirs();
	try {
	File* var6 = new File(this->saveDirectory, "session.lock");
	DataOutputStream* var7 = new DataOutputStream(new FileOutputStream(var6));
	// try/finally: ensure var7->close() runs on normal and exceptional paths
	try {
	var7->writeLong(this->sessionLockStamp);
	} catch (...) {
	var7->close();
	throw;
	}
	var7->close();
	} catch (IOException var16) {
	var16.printStackTrace();
	throw new RuntimeException("Failed to check session lock, aborting");
	}
	void* var17 = new WorldProvider();
	File* var18 = new File(this->saveDirectory, "level.dat");
	this->isNewWorld = !var18->exists();
	if(var18->exists()) {
	try {
	NBTTagCompound* var8 = CompressedStreamTools::readCompressed(new FileInputStream(var18));
	NBTTagCompound* var9 = var8->getCompoundTag("Data");
	this->randomSeed = var9->getLong("RandomSeed");
	this->spawnX = var9->getInteger("SpawnX");
	this->spawnY = var9->getInteger("SpawnY");
	this->spawnZ = var9->getInteger("SpawnZ");
	this->worldTime = var9->getLong("Time");
	this->sizeOnDisk = var9->getLong("SizeOnDisk");
	if(var9->hasKey("Player")) {
	this->nbtCompoundPlayer = var9->getCompoundTag("Player");
	int var10 = this->nbtCompoundPlayer->getInteger("Dimension");
	if(var10 == -1) {
	var17 = new WorldProviderHell();
	}
	}
	} catch (Exception var14) {
	var14.printStackTrace();
	}
	}
	bool var19 = false;
	if(this->randomSeed == 0LL) {
	this->randomSeed = var3;
	var19 = true;
	}
	this->worldProvider = (WorldProvider*)var17;
	this->worldProvider->registerWorld(this);
	this->chunkProvider = this->createChunkProvider(this->saveDirectory);
	if(var19) {
	this->findingSpawnPoint = true;
	this->spawnX = 0;
	this->spawnY = 64;
	for(this->spawnZ = 0; !this->worldProvider->canCoordinateBeSpawn(this->spawnX, this->spawnZ); this->spawnZ += this->rand->nextInt(64) - this->rand->nextInt(64)) {
	this->spawnX += this->rand->nextInt(64) - this->rand->nextInt(64);
	}
	this->findingSpawnPoint = false;
	}
	this->calculateInitialSkylight();
	}
}

World::World(const JavaString& var1, WorldProvider* var2, long long var3)
	: scheduledUpdatesAreImmediate(0)
	, scheduledTickTreeSet(NULL)
	, worldTime(0)
	, fogColor(0)
	, skylightSubtracted(0)
	, updateLCG(0)
	, updateLCGConstant(0)
	, editingBlocks(0)
	, sessionLockStamp(0)
	, autosavePeriod(0)
	, difficultySetting(0)
	, rand(NULL)
	, spawnX(0)
	, spawnY(0)
	, spawnZ(0)
	, isNewWorld(0)
	, worldProvider(NULL)
	, chunkProvider(NULL)
	, savesDirectory(NULL)
	, saveDirectory(NULL)
	, randomSeed(0)
	, nbtCompoundPlayer(NULL)
	, sizeOnDisk(0)
	, findingSpawnPoint(0)
	, lightingUpdateDepth(0)
	, ambientSoundTimer(0)
	, multiplayerWorld(0)
{
	this->scheduledUpdatesAreImmediate = false;
	this->lightingToUpdate = JList<void*>();
	this->loadedEntityList = JList<void*>();
	this->unloadedEntityList = JList<void*>();
	this->scheduledTickTreeSet = new TreeSet();
	this->scheduledTickSet = std::set<void*>();
	this->loadedTileEntityList = JList<void*>();
	this->playerEntities = JList<void*>();
	this->worldTime = 0LL;
	this->fogColor = 16777215LL;
	this->skylightSubtracted = 0;
	this->updateLCG = (new Random())->nextInt();
	this->updateLCGConstant = 1013904223;
	this->editingBlocks = false;
	this->sessionLockStamp = System::currentTimeMillis();
	this->autosavePeriod = 40;
	this->rand = new Random();
	this->isNewWorld = false;
	this->worldAccesses = JList<void*>();
	this->randomSeed = 0LL;
	this->sizeOnDisk = 0LL;
	this->collidingBoundingBoxes = JList<void*>();
	this->lightingUpdateDepth = 0;
	this->activeChunkSet = std::set<void*>();
	this->ambientSoundTimer = this->rand->nextInt(12000);
	this->entitiesWithinAABB = JList<void*>();
	this->multiplayerWorld = false;
	this->worldName = var1;
	this->randomSeed = var3;
	this->worldProvider = var2;
	var2->registerWorld(this);
	this->chunkProvider = this->createChunkProvider(this->saveDirectory);
	this->calculateInitialSkylight();
}

World::World(World* var1, WorldProvider* var2)
	: scheduledUpdatesAreImmediate(0)
	, scheduledTickTreeSet(NULL)
	, worldTime(0)
	, fogColor(0)
	, skylightSubtracted(0)
	, updateLCG(0)
	, updateLCGConstant(0)
	, editingBlocks(0)
	, sessionLockStamp(0)
	, autosavePeriod(0)
	, difficultySetting(0)
	, rand(NULL)
	, spawnX(0)
	, spawnY(0)
	, spawnZ(0)
	, isNewWorld(0)
	, worldProvider(NULL)
	, chunkProvider(NULL)
	, savesDirectory(NULL)
	, saveDirectory(NULL)
	, randomSeed(0)
	, nbtCompoundPlayer(NULL)
	, sizeOnDisk(0)
	, findingSpawnPoint(0)
	, lightingUpdateDepth(0)
	, ambientSoundTimer(0)
	, multiplayerWorld(0)
{
	this->scheduledUpdatesAreImmediate = false;
	this->lightingToUpdate = JList<void*>();
	this->loadedEntityList = JList<void*>();
	this->unloadedEntityList = JList<void*>();
	this->scheduledTickTreeSet = new TreeSet();
	this->scheduledTickSet = std::set<void*>();
	this->loadedTileEntityList = JList<void*>();
	this->playerEntities = JList<void*>();
	this->worldTime = 0LL;
	this->fogColor = 16777215LL;
	this->skylightSubtracted = 0;
	this->updateLCG = (new Random())->nextInt();
	this->updateLCGConstant = 1013904223;
	this->editingBlocks = false;
	this->sessionLockStamp = System::currentTimeMillis();
	this->autosavePeriod = 40;
	this->rand = new Random();
	this->isNewWorld = false;
	this->worldAccesses = JList<void*>();
	this->randomSeed = 0LL;
	this->sizeOnDisk = 0LL;
	this->collidingBoundingBoxes = JList<void*>();
	this->lightingUpdateDepth = 0;
	this->activeChunkSet = std::set<void*>();
	this->ambientSoundTimer = this->rand->nextInt(12000);
	this->entitiesWithinAABB = JList<void*>();
	this->multiplayerWorld = false;
	this->sessionLockStamp = var1->sessionLockStamp;
	this->savesDirectory = var1->savesDirectory;
	this->saveDirectory = var1->saveDirectory;
	this->worldName = var1->worldName;
	this->randomSeed = var1->randomSeed;
	this->worldTime = var1->worldTime;
	this->spawnX = var1->spawnX;
	this->spawnY = var1->spawnY;
	this->spawnZ = var1->spawnZ;
	this->sizeOnDisk = var1->sizeOnDisk;
	this->worldProvider = var2;
	var2->registerWorld(this);
	this->chunkProvider = this->createChunkProvider(this->saveDirectory);
	this->calculateInitialSkylight();
}

World::World(File* var1, const JavaString& var2, long long var3)
	: scheduledUpdatesAreImmediate(0)
	, scheduledTickTreeSet(NULL)
	, worldTime(0)
	, fogColor(0)
	, skylightSubtracted(0)
	, updateLCG(0)
	, updateLCGConstant(0)
	, editingBlocks(0)
	, sessionLockStamp(0)
	, autosavePeriod(0)
	, difficultySetting(0)
	, rand(NULL)
	, spawnX(0)
	, spawnY(0)
	, spawnZ(0)
	, isNewWorld(0)
	, worldProvider(NULL)
	, chunkProvider(NULL)
	, savesDirectory(NULL)
	, saveDirectory(NULL)
	, randomSeed(0)
	, nbtCompoundPlayer(NULL)
	, sizeOnDisk(0)
	, findingSpawnPoint(0)
	, lightingUpdateDepth(0)
	, ambientSoundTimer(0)
	, multiplayerWorld(0)
{
	// inlined this(var1, var2, var3, (WorldProvider*)NULL) -- C++03 lacks ctor delegation
	this->scheduledUpdatesAreImmediate = false;
	this->lightingToUpdate = JList<void*>();
	this->loadedEntityList = JList<void*>();
	this->unloadedEntityList = JList<void*>();
	this->scheduledTickTreeSet = new TreeSet();
	this->scheduledTickSet = std::set<void*>();
	this->loadedTileEntityList = JList<void*>();
	this->playerEntities = JList<void*>();
	this->worldTime = 0LL;
	this->fogColor = 16777215LL;
	this->skylightSubtracted = 0;
	this->updateLCG = (new Random())->nextInt();
	this->updateLCGConstant = 1013904223;
	this->editingBlocks = false;
	this->sessionLockStamp = System::currentTimeMillis();
	this->autosavePeriod = 40;
	this->rand = new Random();
	this->isNewWorld = false;
	this->worldAccesses = JList<void*>();
	this->randomSeed = 0LL;
	this->sizeOnDisk = 0LL;
	this->collidingBoundingBoxes = JList<void*>();
	this->lightingUpdateDepth = 0;
	this->activeChunkSet = std::set<void*>();
	this->ambientSoundTimer = this->rand->nextInt(12000);
	this->entitiesWithinAABB = JList<void*>();
	this->multiplayerWorld = false;
	this->savesDirectory = var1;
	this->worldName = var2;
	var1->mkdirs();
	this->saveDirectory = new File(var1, var2);
	this->saveDirectory->mkdirs();
	try {
	File* var6 = new File(this->saveDirectory, "session.lock");
	DataOutputStream* var7 = new DataOutputStream(new FileOutputStream(var6));
	// try/finally: ensure var7->close() runs on normal and exceptional paths
	try {
	var7->writeLong(this->sessionLockStamp);
	} catch (...) {
	var7->close();
	throw;
	}
	var7->close();
	} catch (IOException var16) {
	var16.printStackTrace();
	throw new RuntimeException("Failed to check session lock, aborting");
	}
	void* var17 = new WorldProvider();
	File* var18 = new File(this->saveDirectory, "level.dat");
	this->isNewWorld = !var18->exists();
	if(var18->exists()) {
	try {
	NBTTagCompound* var8 = CompressedStreamTools::readCompressed(new FileInputStream(var18));
	NBTTagCompound* var9 = var8->getCompoundTag("Data");
	this->randomSeed = var9->getLong("RandomSeed");
	this->spawnX = var9->getInteger("SpawnX");
	this->spawnY = var9->getInteger("SpawnY");
	this->spawnZ = var9->getInteger("SpawnZ");
	this->worldTime = var9->getLong("Time");
	this->sizeOnDisk = var9->getLong("SizeOnDisk");
	if(var9->hasKey("Player")) {
	this->nbtCompoundPlayer = var9->getCompoundTag("Player");
	int var10 = this->nbtCompoundPlayer->getInteger("Dimension");
	if(var10 == -1) {
	var17 = new WorldProviderHell();
	}
	}
	} catch (Exception var14) {
	var14.printStackTrace();
	}
	}
	if((WorldProvider*)NULL != NULL) {
	var17 = (WorldProvider*)NULL;
	}
	bool var19 = false;
	if(this->randomSeed == 0LL) {
	this->randomSeed = var3;
	var19 = true;
	}
	this->worldProvider = (WorldProvider*)var17;
	this->worldProvider->registerWorld(this);
	this->chunkProvider = this->createChunkProvider(this->saveDirectory);
	if(var19) {
	this->findingSpawnPoint = true;
	this->spawnX = 0;
	this->spawnY = 64;
	for(this->spawnZ = 0; !this->worldProvider->canCoordinateBeSpawn(this->spawnX, this->spawnZ); this->spawnZ += this->rand->nextInt(64) - this->rand->nextInt(64)) {
	this->spawnX += this->rand->nextInt(64) - this->rand->nextInt(64);
	}
	this->findingSpawnPoint = false;
	}
	this->calculateInitialSkylight();
}

World::World(File* var1, const JavaString& var2, long long var3, WorldProvider* var5)
	: scheduledUpdatesAreImmediate(0)
	, scheduledTickTreeSet(NULL)
	, worldTime(0)
	, fogColor(0)
	, skylightSubtracted(0)
	, updateLCG(0)
	, updateLCGConstant(0)
	, editingBlocks(0)
	, sessionLockStamp(0)
	, autosavePeriod(0)
	, difficultySetting(0)
	, rand(NULL)
	, spawnX(0)
	, spawnY(0)
	, spawnZ(0)
	, isNewWorld(0)
	, worldProvider(NULL)
	, chunkProvider(NULL)
	, savesDirectory(NULL)
	, saveDirectory(NULL)
	, randomSeed(0)
	, nbtCompoundPlayer(NULL)
	, sizeOnDisk(0)
	, findingSpawnPoint(0)
	, lightingUpdateDepth(0)
	, ambientSoundTimer(0)
	, multiplayerWorld(0)
{
	this->scheduledUpdatesAreImmediate = false;
	this->lightingToUpdate = JList<void*>();
	this->loadedEntityList = JList<void*>();
	this->unloadedEntityList = JList<void*>();
	this->scheduledTickTreeSet = new TreeSet();
	this->scheduledTickSet = std::set<void*>();
	this->loadedTileEntityList = JList<void*>();
	this->playerEntities = JList<void*>();
	this->worldTime = 0LL;
	this->fogColor = 16777215LL;
	this->skylightSubtracted = 0;
	this->updateLCG = (new Random())->nextInt();
	this->updateLCGConstant = 1013904223;
	this->editingBlocks = false;
	this->sessionLockStamp = System::currentTimeMillis();
	this->autosavePeriod = 40;
	this->rand = new Random();
	this->isNewWorld = false;
	this->worldAccesses = JList<void*>();
	this->randomSeed = 0LL;
	this->sizeOnDisk = 0LL;
	this->collidingBoundingBoxes = JList<void*>();
	this->lightingUpdateDepth = 0;
	this->activeChunkSet = std::set<void*>();
	this->ambientSoundTimer = this->rand->nextInt(12000);
	this->entitiesWithinAABB = JList<void*>();
	this->multiplayerWorld = false;
	this->savesDirectory = var1;
	this->worldName = var2;
	var1->mkdirs();
	this->saveDirectory = new File(var1, var2);
	this->saveDirectory->mkdirs();
	try {
	File* var6 = new File(this->saveDirectory, "session.lock");
	DataOutputStream* var7 = new DataOutputStream(new FileOutputStream(var6));
	// try/finally: ensure var7->close() runs on normal and exceptional paths
	try {
	var7->writeLong(this->sessionLockStamp);
	} catch (...) {
	var7->close();
	throw;
	}
	var7->close();
	} catch (IOException var16) {
	var16.printStackTrace();
	throw new RuntimeException("Failed to check session lock, aborting");
	}
	void* var17 = new WorldProvider();
	File* var18 = new File(this->saveDirectory, "level.dat");
	this->isNewWorld = !var18->exists();
	if(var18->exists()) {
	try {
	NBTTagCompound* var8 = CompressedStreamTools::readCompressed(new FileInputStream(var18));
	NBTTagCompound* var9 = var8->getCompoundTag("Data");
	this->randomSeed = var9->getLong("RandomSeed");
	this->spawnX = var9->getInteger("SpawnX");
	this->spawnY = var9->getInteger("SpawnY");
	this->spawnZ = var9->getInteger("SpawnZ");
	this->worldTime = var9->getLong("Time");
	this->sizeOnDisk = var9->getLong("SizeOnDisk");
	if(var9->hasKey("Player")) {
	this->nbtCompoundPlayer = var9->getCompoundTag("Player");
	int var10 = this->nbtCompoundPlayer->getInteger("Dimension");
	if(var10 == -1) {
	var17 = new WorldProviderHell();
	}
	}
	} catch (Exception var14) {
	var14.printStackTrace();
	}
	}
	if(var5 != NULL) {
	var17 = var5;
	}
	bool var19 = false;
	if(this->randomSeed == 0LL) {
	this->randomSeed = var3;
	var19 = true;
	}
	this->worldProvider = (WorldProvider*)var17;
	this->worldProvider->registerWorld(this);
	this->chunkProvider = this->createChunkProvider(this->saveDirectory);
	if(var19) {
	this->findingSpawnPoint = true;
	this->spawnX = 0;
	this->spawnY = 64;
	for(this->spawnZ = 0; !this->worldProvider->canCoordinateBeSpawn(this->spawnX, this->spawnZ); this->spawnZ += this->rand->nextInt(64) - this->rand->nextInt(64)) {
	this->spawnX += this->rand->nextInt(64) - this->rand->nextInt(64);
	}
	this->findingSpawnPoint = false;
	}
	this->calculateInitialSkylight();
}

World::~World()
{
	delete rand;
	rand = NULL;
	delete saveDirectory;
	saveDirectory = NULL;
	delete scheduledTickTreeSet;
	scheduledTickTreeSet = NULL;
}

NBTTagCompound* World::getWorldInfoNBT(File* var0, const JavaString& var1)
{
	File* var2 = new File(var0, "saves");
	File* var3 = new File(var2, var1);
	if(!var3->exists()) {
	return NULL;
	} else {
	File* var4 = new File(var3, "level.dat");
	if(var4->exists()) {
	try {
	NBTTagCompound* var5 = CompressedStreamTools::readCompressed(new FileInputStream(var4));
	NBTTagCompound* var6 = var5->getCompoundTag("Data");
	return var6;
	} catch (Exception var7) {
	var7.printStackTrace();
	}
	}
	return NULL;
	}
}

void World::deleteWorld(File* var0, const JavaString& var1)
{
	File* var2 = new File(var0, "saves");
	File* var3 = new File(var2, var1);
	if(var3->exists()) {
	deleteFiles(fileListFiles(var3));
	fileDelete(var3);
	}
}

void World::deleteFiles(JArray<File* > var0)
{
	for(int var1 = 0; var1 < var0.length; ++var1) {
	if(var0[var1]->isDirectory()) {
	deleteFiles(fileListFiles(var0[var1]));
	}
	if (var0[var1] != NULL) { fileDelete(var0[var1]); }
	}
}

WorldChunkManager* World::getWorldChunkManager()
{
	return this->worldProvider->worldChunkMgr;
}

IChunkProvider* World::createChunkProvider(File* var1)
{
	return new ChunkProviderLoadOrGenerate(this, this->worldProvider->getChunkLoader(var1), this->worldProvider->getChunkProvider());
}

void World::findSpawnGround()
{
	if(this->spawnY <= 0) {
	this->spawnY = 64;
	}
	while(this->getTopBlockId(this->spawnX, this->spawnZ) == 0) {
	this->spawnX += this->rand->nextInt(8) - this->rand->nextInt(8);
	this->spawnZ += this->rand->nextInt(8) - this->rand->nextInt(8);
	}
}

int World::getTopBlockId(int var1, int var2)
{
	int var3;
	for(var3 = 63; this->getBlockId(var1, var3 + 1, var2) != 0; ++var3) {
	}
	return this->getBlockId(var1, var3, var2);
}

void World::func_6464_c()
{
}

void World::loadPlayerData(EntityPlayer* var1)
{
	try {
	if(this->nbtCompoundPlayer != NULL) {
	var1->readFromNBT(this->nbtCompoundPlayer);
	this->nbtCompoundPlayer = NULL;
	}
	this->entityJoinedWorld(var1);
	} catch (Exception var3) {
	var3.printStackTrace();
	}
}

void World::saveWorld(bool var1, IProgressUpdate* var2)
{
	if(this->chunkProvider->canSave()) {
	if(var2 != NULL) {
	var2->displayProgressMessage("Saving level");
	}
	this->saveLevel();
	if(var2 != NULL) {
	var2->displayLoadingString("Saving chunks");
	}
	this->chunkProvider->saveChunks(var1, var2);
	}
}

void World::saveLevel()
{
	this->checkSessionLock();
	NBTTagCompound* var1 = new NBTTagCompound();
	var1->setLong("RandomSeed", this->randomSeed);
	var1->setInteger("SpawnX", this->spawnX);
	var1->setInteger("SpawnY", this->spawnY);
	var1->setInteger("SpawnZ", this->spawnZ);
	var1->setLong("Time", this->worldTime);
	var1->setLong("SizeOnDisk", this->sizeOnDisk);
	var1->setLong("LastPlayed", System::currentTimeMillis());
	EntityPlayer* var2 = NULL;
	if(this->playerEntities.size() > 0) {
	var2 = (EntityPlayer*)this->playerEntities[0];
	}
	NBTTagCompound* var3;
	if(var2 != NULL) {
	var3 = new NBTTagCompound();
	var2->writeToNBT(var3);
	var1->setCompoundTag("Player", var3);
	}
	var3 = new NBTTagCompound();
	var3->setTag("Data", var1);
	try {
	File* var4 = new File(this->saveDirectory, "level.dat_new");
	File* var5 = new File(this->saveDirectory, "level.dat_old");
	File* var6 = new File(this->saveDirectory, "level.dat");
	CompressedStreamTools::writeGzippedCompoundToOutputStream(var3, new FileOutputStream(var4));
	if(var5->exists()) {
	fileDelete(var5);
	}
	var6->renameTo(var5);
	if(var6->exists()) {
	fileDelete(var6);
	}
	var4->renameTo(var6);
	if(var4->exists()) {
	fileDelete(var4);
	}
	} catch (Exception var7) {
	var7.printStackTrace();
	}
}

bool World::quickSaveWorld(int var1)
{
	if(!this->chunkProvider->canSave()) {
	return true;
	} else {
	if(var1 == 0) {
	this->saveLevel();
	}
	return this->chunkProvider->saveChunks(false, (IProgressUpdate*)NULL);
	}
}

int World::getBlockId(int var1, int var2, int var3)
{
	return var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000 ? (var2 < 0 ? 0 : (var2 >= 128 ? 0 : this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4)->getBlockID(var1 & 15, var2, var3 & 15))) : 0;
}

bool World::blockExists(int var1, int var2, int var3)
{
	return var2 >= 0 && var2 < 128 ? this->chunkExists(var1 >> 4, var3 >> 4) : false;
}

bool World::checkChunksExist(int var1, int var2, int var3, int var4, int var5, int var6)
{
	if(var5 >= 0 && var2 < 128) {
	var1 >>= 4;
	var2 >>= 4;
	var3 >>= 4;
	var4 >>= 4;
	var5 >>= 4;
	var6 >>= 4;
	for(int var7 = var1; var7 <= var4; ++var7) {
	for(int var8 = var3; var8 <= var6; ++var8) {
	if(!this->chunkExists(var7, var8)) {
	return false;
	}
	}
	}
	return true;
	} else {
	return false;
	}
}

bool World::chunkExists(int var1, int var2)
{
	return this->chunkProvider->chunkExists(var1, var2);
}

Chunk* World::getChunkFromBlockCoords(int var1, int var2)
{
	return this->getChunkFromChunkCoords(var1 >> 4, var2 >> 4);
}

Chunk* World::getChunkFromChunkCoords(int var1, int var2)
{
	return this->chunkProvider->provideChunk(var1, var2);
}

bool World::setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	if(var2 < 0) {
	return false;
	} else if(var2 >= 128) {
	return false;
	} else {
	Chunk* var6 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	return var6->setBlockIDWithMetadata(var1 & 15, var2, var3 & 15, var4, var5);
	}
	} else {
	return false;
	}
}

bool World::setBlock(int var1, int var2, int var3, int var4)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	if(var2 < 0) {
	return false;
	} else if(var2 >= 128) {
	return false;
	} else {
	Chunk* var5 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	return var5->setBlockID(var1 & 15, var2, var3 & 15, var4);
	}
	} else {
	return false;
	}
}

Material* World::getBlockMaterial(int var1, int var2, int var3)
{
	int var4 = this->getBlockId(var1, var2, var3);
	return var4 == 0 ? Material::s_air : Block::s_blocksList[var4]->blockMaterial;
}

int World::getBlockMetadata(int var1, int var2, int var3)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	if(var2 < 0) {
	return 0;
	} else if(var2 >= 128) {
	return 0;
	} else {
	Chunk* var4 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	var1 &= 15;
	var3 &= 15;
	return var4->getBlockMetadata(var1, var2, var3);
	}
	} else {
	return 0;
	}
}

void World::setBlockMetadataWithNotify(int var1, int var2, int var3, int var4)
{
	if(this->setBlockMetadata(var1, var2, var3, var4)) {
	this->notifyBlockChange(var1, var2, var3, this->getBlockId(var1, var2, var3));
	}
}

bool World::setBlockMetadata(int var1, int var2, int var3, int var4)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	if(var2 < 0) {
	return false;
	} else if(var2 >= 128) {
	return false;
	} else {
	Chunk* var5 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	var1 &= 15;
	var3 &= 15;
	var5->setBlockMetadata(var1, var2, var3, var4);
	return true;
	}
	} else {
	return false;
	}
}

bool World::setBlockWithNotify(int var1, int var2, int var3, int var4)
{
	if(this->setBlock(var1, var2, var3, var4)) {
	this->notifyBlockChange(var1, var2, var3, var4);
	return true;
	} else {
	return false;
	}
}

bool World::setBlockAndMetadataWithNotify(int var1, int var2, int var3, int var4, int var5)
{
	if(this->setBlockAndMetadata(var1, var2, var3, var4, var5)) {
	this->notifyBlockChange(var1, var2, var3, var4);
	return true;
	} else {
	return false;
	}
}

void World::markBlockNeedsUpdate(int var1, int var2, int var3)
{
	for(int var4 = 0; var4 < this->worldAccesses.size(); ++var4) {
	((IWorldAccess*)this->worldAccesses[var4])->markBlockNeedsUpdate(var1, var2, var3);
	}
}

void World::notifyBlockChange(int var1, int var2, int var3, int var4)
{
	this->markBlockNeedsUpdate(var1, var2, var3);
	this->notifyBlocksOfNeighborChange(var1, var2, var3, var4);
}

void World::markBlocksDirtyVertical(int var1, int var2, int var3, int var4)
{
	if(var3 > var4) {
	int var5 = var4;
	var4 = var3;
	var3 = var5;
	}
	this->markBlockRangeNeedsUpdate(var1, var3, var2, var1, var4, var2);
}

void World::markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6)
{
	for(int var7 = 0; var7 < this->worldAccesses.size(); ++var7) {
	((IWorldAccess*)this->worldAccesses[var7])->markBlockRangeNeedsUpdate(var1, var2, var3, var4, var5, var6);
	}
}

void World::notifyBlocksOfNeighborChange(int var1, int var2, int var3, int var4)
{
	this->notifyBlockOfNeighborChange(var1 - 1, var2, var3, var4);
	this->notifyBlockOfNeighborChange(var1 + 1, var2, var3, var4);
	this->notifyBlockOfNeighborChange(var1, var2 - 1, var3, var4);
	this->notifyBlockOfNeighborChange(var1, var2 + 1, var3, var4);
	this->notifyBlockOfNeighborChange(var1, var2, var3 - 1, var4);
	this->notifyBlockOfNeighborChange(var1, var2, var3 + 1, var4);
}

void World::notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4)
{
	if(!this->editingBlocks && !this->multiplayerWorld) {
	Block* var5 = Block::s_blocksList[this->getBlockId(var1, var2, var3)];
	if(var5 != NULL) {
	var5->onNeighborBlockChange(this, var1, var2, var3, var4);
	}
	}
}

bool World::canBlockSeeTheSky(int var1, int var2, int var3)
{
	return this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4)->canBlockSeeTheSky(var1 & 15, var2, var3 & 15);
}

int World::getBlockLightValue(int var1, int var2, int var3)
{
	return this->getBlockLightValue(var1, var2, var3, true);
}

int World::getBlockLightValue(int var1, int var2, int var3, bool var4)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	int var5;
	if(var4) {
	var5 = this->getBlockId(var1, var2, var3);
	if(var5 == Block::s_stairSingle->blockID || var5 == Block::s_tilledField->blockID) {
	int var6 = this->getBlockLightValue(var1, var2 + 1, var3, false);
	int var7 = this->getBlockLightValue(var1 + 1, var2, var3, false);
	int var8 = this->getBlockLightValue(var1 - 1, var2, var3, false);
	int var9 = this->getBlockLightValue(var1, var2, var3 + 1, false);
	int var10 = this->getBlockLightValue(var1, var2, var3 - 1, false);
	if(var7 > var6) {
	var6 = var7;
	}
	if(var8 > var6) {
	var6 = var8;
	}
	if(var9 > var6) {
	var6 = var9;
	}
	if(var10 > var6) {
	var6 = var10;
	}
	return var6;
	}
	}
	if(var2 < 0) {
	return 0;
	} else if(var2 >= 128) {
	var5 = 15 - this->skylightSubtracted;
	if(var5 < 0) {
	var5 = 0;
	}
	return var5;
	} else {
	Chunk* var11 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	var1 &= 15;
	var3 &= 15;
	return var11->getBlockLightValue(var1, var2, var3, this->skylightSubtracted);
	}
	} else {
	return 15;
	}
}

bool World::canExistingBlockSeeTheSky(int var1, int var2, int var3)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	if(var2 < 0) {
	return false;
	} else if(var2 >= 128) {
	return true;
	} else if(!this->chunkExists(var1 >> 4, var3 >> 4)) {
	return false;
	} else {
	Chunk* var4 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	var1 &= 15;
	var3 &= 15;
	return var4->canBlockSeeTheSky(var1, var2, var3);
	}
	} else {
	return false;
	}
}

int World::getHeightValue(int var1, int var2)
{
	if(var1 >= -32000000 && var2 >= -32000000 && var1 < 32000000 && var2 <= 32000000) {
	if(!this->chunkExists(var1 >> 4, var2 >> 4)) {
	return 0;
	} else {
	Chunk* var3 = this->getChunkFromChunkCoords(var1 >> 4, var2 >> 4);
	return var3->getHeightValue(var1 & 15, var2 & 15);
	}
	} else {
	return 0;
	}
}

void World::neighborLightPropagationChanged(int var1, int var2, int var3, int var4, int var5)
{
	if(!this->worldProvider->hasNoSky || var1 != EnumSkyBlock::Sky) {
	if(this->blockExists(var2, var3, var4)) {
	if(var1 == EnumSkyBlock::Sky) {
	if(this->canExistingBlockSeeTheSky(var2, var3, var4)) {
	var5 = 15;
	}
	} else if(var1 == EnumSkyBlock::Block) {
	int var6 = this->getBlockId(var2, var3, var4);
	if(Block::s_lightValue[var6] > var5) {
	var5 = Block::s_lightValue[var6];
	}
	}
	if(this->getSavedLightValue(var1, var2, var3, var4) != var5) {
	this->scheduleLightingUpdate(var1, var2, var3, var4, var2, var3, var4);
	}
	}
	}
}

int World::getSavedLightValue(int var1, int var2, int var3, int var4)
{
	if(var3 >= 0 && var3 < 128 && var2 >= -32000000 && var4 >= -32000000 && var2 < 32000000 && var4 <= 32000000) {
	int var5 = var2 >> 4;
	int var6 = var4 >> 4;
	if(!this->chunkExists(var5, var6)) {
	return 0;
	} else {
	Chunk* var7 = this->getChunkFromChunkCoords(var5, var6);
	return var7->getSavedLightValue(var1, var2 & 15, var3, var4 & 15);
	}
	} else {
	// EnumSkyBlock was lowered to int (Sky=0, Block=1); its defaultLightValue
	// field (Sky=15, Block=0) is not on the struct, so reproduce it inline.
	return var1 == EnumSkyBlock::Sky ? 15 : 0;
	}
}

void World::setLightValue(int var1, int var2, int var3, int var4, int var5)
{
	if(var2 >= -32000000 && var4 >= -32000000 && var2 < 32000000 && var4 <= 32000000) {
	if(var3 >= 0) {
	if(var3 < 128) {
	if(this->chunkExists(var2 >> 4, var4 >> 4)) {
	Chunk* var6 = this->getChunkFromChunkCoords(var2 >> 4, var4 >> 4);
	var6->setLightValue(var1, var2 & 15, var3, var4 & 15, var5);
	for(int var7 = 0; var7 < this->worldAccesses.size(); ++var7) {
	((IWorldAccess*)this->worldAccesses[var7])->markBlockNeedsUpdate(var2, var3, var4);
	}
	}
	}
	}
	}
}

float World::getLightBrightness(int var1, int var2, int var3)
{
	return this->worldProvider->lightBrightnessTable[this->getBlockLightValue(var1, var2, var3)];
}

bool World::isDaytime()
{
	return this->skylightSubtracted < 4;
}

MovingObjectPosition* World::rayTraceBlocks(Vec3D* var1, Vec3D* var2)
{
	return this->rayTraceBlocks(var1, var2, false);
}

MovingObjectPosition* World::rayTraceBlocks(Vec3D* var1, Vec3D* var2, bool var3)
{
	if(!Double::isNaN(var1->xCoord) && !Double::isNaN(var1->yCoord) && !Double::isNaN(var1->zCoord)) {
	if(!Double::isNaN(var2->xCoord) && !Double::isNaN(var2->yCoord) && !Double::isNaN(var2->zCoord)) {
	int var4 = MathHelper::floor_double(var2->xCoord);
	int var5 = MathHelper::floor_double(var2->yCoord);
	int var6 = MathHelper::floor_double(var2->zCoord);
	int var7 = MathHelper::floor_double(var1->xCoord);
	int var8 = MathHelper::floor_double(var1->yCoord);
	int var9 = MathHelper::floor_double(var1->zCoord);
	int var10 = 200;
	while(var10-- >= 0) {
	if(Double::isNaN(var1->xCoord) || Double::isNaN(var1->yCoord) || Double::isNaN(var1->zCoord)) {
	return NULL;
	}
	if(var7 == var4 && var8 == var5 && var9 == var6) {
	return NULL;
	}
	double var11 = 999.0;
	double var13 = 999.0;
	double var15 = 999.0;
	if(var4 > var7) {
	var11 = (double)var7 + 1.0;
	}
	if(var4 < var7) {
	var11 = (double)var7 + 0.0;
	}
	if(var5 > var8) {
	var13 = (double)var8 + 1.0;
	}
	if(var5 < var8) {
	var13 = (double)var8 + 0.0;
	}
	if(var6 > var9) {
	var15 = (double)var9 + 1.0;
	}
	if(var6 < var9) {
	var15 = (double)var9 + 0.0;
	}
	double var17 = 999.0;
	double var19 = 999.0;
	double var21 = 999.0;
	double var23 = var2->xCoord - var1->xCoord;
	double var25 = var2->yCoord - var1->yCoord;
	double var27 = var2->zCoord - var1->zCoord;
	if(var11 != 999.0) {
	var17 = (var11 - var1->xCoord) / var23;
	}
	if(var13 != 999.0) {
	var19 = (var13 - var1->yCoord) / var25;
	}
	if(var15 != 999.0) {
	var21 = (var15 - var1->zCoord) / var27;
	}
	bool var29 = false;
	unsigned char var35;
	if(var17 < var19 && var17 < var21) {
	if(var4 > var7) {
	var35 = 4;
	} else {
	var35 = 5;
	}
	var1->xCoord = var11;
	var1->yCoord += var25 * var17;
	var1->zCoord += var27 * var17;
	} else if(var19 < var21) {
	if(var5 > var8) {
	var35 = 0;
	} else {
	var35 = 1;
	}
	var1->xCoord += var23 * var19;
	var1->yCoord = var13;
	var1->zCoord += var27 * var19;
	} else {
	if(var6 > var9) {
	var35 = 2;
	} else {
	var35 = 3;
	}
	var1->xCoord += var23 * var21;
	var1->yCoord += var25 * var21;
	var1->zCoord = var15;
	}
	Vec3D* var30 = Vec3D::createVector(var1->xCoord, var1->yCoord, var1->zCoord);
	var7 = (int)(var30->xCoord = (double)MathHelper::floor_double(var1->xCoord));
	if(var35 == 5) {
	--var7;
	++var30->xCoord;
	}
	var8 = (int)(var30->yCoord = (double)MathHelper::floor_double(var1->yCoord));
	if(var35 == 1) {
	--var8;
	++var30->yCoord;
	}
	var9 = (int)(var30->zCoord = (double)MathHelper::floor_double(var1->zCoord));
	if(var35 == 3) {
	--var9;
	++var30->zCoord;
	}
	int var31 = this->getBlockId(var7, var8, var9);
	int var32 = this->getBlockMetadata(var7, var8, var9);
	Block* var33 = Block::s_blocksList[var31];
	if(var31 > 0 && var33->canCollideCheck(var32, var3)) {
	MovingObjectPosition* var34 = var33->collisionRayTrace(this, var7, var8, var9, var1, var2);
	if(var34 != NULL) {
	return var34;
	}
	}
	}
	return NULL;
	} else {
	return NULL;
	}
	} else {
	return NULL;
	}
}

void World::playSoundAtEntity(Entity* var1, const JavaString& var2, float var3, float var4)
{
	for(int var5 = 0; var5 < this->worldAccesses.size(); ++var5) {
	((IWorldAccess*)this->worldAccesses[var5])->playSound(var2, var1->posX, var1->posY - (double)var1->yOffset, var1->posZ, var3, var4);
	}
}

void World::playSoundEffect(double var1, double var3, double var5, const JavaString& var7, float var8, float var9)
{
	for(int var10 = 0; var10 < this->worldAccesses.size(); ++var10) {
	((IWorldAccess*)this->worldAccesses[var10])->playSound(var7, var1, var3, var5, var8, var9);
	}
}

void World::playRecord(const JavaString& var1, int var2, int var3, int var4)
{
	for(int var5 = 0; var5 < this->worldAccesses.size(); ++var5) {
	((IWorldAccess*)this->worldAccesses[var5])->playRecord(var1, var2, var3, var4);
	}
}

void World::spawnParticle(const JavaString& var1, double var2, double var4, double var6, double var8, double var10, double var12)
{
	for(int var14 = 0; var14 < this->worldAccesses.size(); ++var14) {
	((IWorldAccess*)this->worldAccesses[var14])->spawnParticle(var1, var2, var4, var6, var8, var10, var12);
	}
}

bool World::entityJoinedWorld(Entity* var1)
{
	int var2 = MathHelper::floor_double(var1->posX / 16.0);
	int var3 = MathHelper::floor_double(var1->posZ / 16.0);
	bool var4 = false;
	if((dynamic_cast<EntityPlayer*>(var1) != NULL)) {
	var4 = true;
	}
	if(!var4 && !this->chunkExists(var2, var3)) {
	return false;
	} else {
	if((dynamic_cast<EntityPlayer*>(var1) != NULL)) {
	this->playerEntities.push_back((EntityPlayer*)var1);
	System::out.println((JCat() + "Player count: " + this->playerEntities.size()));
	}
	this->getChunkFromChunkCoords(var2, var3)->addEntity(var1);
	this->loadedEntityList.push_back(var1);
	this->obtainEntitySkin(var1);
	return true;
	}
}

void World::obtainEntitySkin(Entity* var1)
{
	for(int var2 = 0; var2 < this->worldAccesses.size(); ++var2) {
	((IWorldAccess*)this->worldAccesses[var2])->obtainEntitySkin(var1);
	}
}

void World::releaseEntitySkin(Entity* var1)
{
	for(int var2 = 0; var2 < this->worldAccesses.size(); ++var2) {
	((IWorldAccess*)this->worldAccesses[var2])->releaseEntitySkin(var1);
	}
}

void World::setEntityDead(Entity* var1)
{
	var1->setEntityDead();
	if((dynamic_cast<EntityPlayer*>(var1) != NULL)) {
	this->playerEntities.remove((EntityPlayer*)var1);
	}
}

void World::addWorldAccess(IWorldAccess* var1)
{
	this->worldAccesses.push_back(var1);
}

void World::removeWorldAccess(IWorldAccess* var1)
{
	this->worldAccesses.remove(var1);
}

JList<void*> World::getCollidingBoundingBoxes(Entity* var1, AxisAlignedBB* var2)
{
	this->collidingBoundingBoxes.clear();
	int var3 = MathHelper::floor_double(var2->minX);
	int var4 = MathHelper::floor_double(var2->maxX + 1.0);
	int var5 = MathHelper::floor_double(var2->minY);
	int var6 = MathHelper::floor_double(var2->maxY + 1.0);
	int var7 = MathHelper::floor_double(var2->minZ);
	int var8 = MathHelper::floor_double(var2->maxZ + 1.0);
	for(int var9 = var3; var9 < var4; ++var9) {
	for(int var10 = var7; var10 < var8; ++var10) {
	if(this->blockExists(var9, 64, var10)) {
	for(int var11 = var5 - 1; var11 < var6; ++var11) {
	Block* var12 = Block::s_blocksList[this->getBlockId(var9, var11, var10)];
	if(var12 != NULL) {
	var12->getCollidingBoundingBoxes(this, var9, var11, var10, var2, this->collidingBoundingBoxes);
	}
	}
	}
	}
	}
	double var14 = 0.25;
	JList<void*> var15 = this->getEntitiesWithinAABBExcludingEntity(var1, var2->expands(var14, var14, var14));
	for(int var16 = 0; var16 < var15.size(); ++var16) {
	AxisAlignedBB* var13 = ((Entity*)var15[var16])->getCollisionBox();
	if(var13 != NULL && var13->intersectsWith(var2)) {
	this->collidingBoundingBoxes.push_back(var13);
	}
	var13 = var1->getBoundingBox((Entity*)var15[var16]);
	if(var13 != NULL && var13->intersectsWith(var2)) {
	this->collidingBoundingBoxes.push_back(var13);
	}
	}
	return this->collidingBoundingBoxes;
}

int World::calculateSkylightSubtracted(float var1)
{
	float var2 = this->getCelestialAngle(var1);
	float var3 = 1.0F - (MathHelper::cos(var2 * (float)3.1415927F * 2.0F) * 2.0F + 0.5F);
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	return (int)(var3 * 11.0F);
}

Vec3D* World::getSkyColor(Entity* var1, float var2)
{
	float var3 = this->getCelestialAngle(var2);
	float var4 = MathHelper::cos(var3 * (float)3.1415927F * 2.0F) * 2.0F + 0.5F;
	if(var4 < 0.0F) {
	var4 = 0.0F;
	}
	if(var4 > 1.0F) {
	var4 = 1.0F;
	}
	int var5 = MathHelper::floor_double(var1->posX);
	int var6 = MathHelper::floor_double(var1->posZ);
	float var7 = (float)this->getWorldChunkManager()->getTemperature(var5, var6);
	int var8 = this->getWorldChunkManager()->getBiome(var5, var6)->getSkyColorByTemp(var7);
	float var9 = (float)(var8 >> 16 & 255) / 255.0F;
	float var10 = (float)(var8 >> 8 & 255) / 255.0F;
	float var11 = (float)(var8 & 255) / 255.0F;
	var9 *= var4;
	var10 *= var4;
	var11 *= var4;
	return Vec3D::createVector((double)var9, (double)var10, (double)var11);
}

float World::getCelestialAngle(float var1)
{
	return this->worldProvider->calculateCelestialAngle(this->worldTime, var1);
}

Vec3D* World::getFogColor(float var1)
{
	float var2 = this->getCelestialAngle(var1);
	float var3 = MathHelper::cos(var2 * (float)3.1415927F * 2.0F) * 2.0F + 0.5F;
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	float var4 = (float)(this->fogColor >> 16 & 255LL) / 255.0F;
	float var5 = (float)(this->fogColor >> 8 & 255LL) / 255.0F;
	float var6 = (float)(this->fogColor & 255LL) / 255.0F;
	var4 *= var3 * 0.9F + 0.1F;
	var5 *= var3 * 0.9F + 0.1F;
	var6 *= var3 * 0.85F + 0.15F;
	return Vec3D::createVector((double)var4, (double)var5, (double)var6);
}

Vec3D* World::getCloudColour(float var1)
{
	float var2 = this->getCelestialAngle(var1);
	return this->worldProvider->getFogColor(var2, var1);
}

int World::getTopSolidOrLiquidBlock(int var1, int var2)
{
	Chunk* var3 = this->getChunkFromBlockCoords(var1, var2);
	int var4;
	for(var4 = 127; this->getBlockMaterial(var1, var4, var2)->blocksMovement() && var4 > 0; --var4) {
	}
	var1 &= 15;
	for(var2 &= 15; var4 > 0; --var4) {
	int var5 = var3->getBlockID(var1, var4, var2);
	if(var5 != 0 && (Block::s_blocksList[var5]->blockMaterial->blocksMovement() || Block::s_blocksList[var5]->blockMaterial->getIsLiquid())) {
	return var4 + 1;
	}
	}
	return -1;
}

int World::func_696_e(int var1, int var2)
{
	return this->getChunkFromBlockCoords(var1, var2)->getHeightValue(var1 & 15, var2 & 15);
}

float World::getRainStrength(float var1)
{
	float var2 = this->getCelestialAngle(var1);
	float var3 = 1.0F - (MathHelper::cos(var2 * (float)3.1415927F * 2.0F) * 2.0F + 12.0F / 16.0F);
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	return var3 * var3 * 0.5F;
}

void World::scheduleBlockUpdate(int var1, int var2, int var3, int var4)
{
	NextTickListEntry* var5 = new NextTickListEntry(var1, var2, var3, var4);
	unsigned char var6 = 8;
	if(this->scheduledUpdatesAreImmediate) {
	if(this->checkChunksExist(var5->xCoord - var6, var5->yCoord - var6, var5->zCoord - var6, var5->xCoord + var6, var5->yCoord + var6, var5->zCoord + var6)) {
	int var7 = this->getBlockId(var5->xCoord, var5->yCoord, var5->zCoord);
	if(var7 == var5->blockID && var7 > 0) {
	if (Block::s_blocksList[var7] != NULL) { Block::s_blocksList[var7]->updateTick(this, var5->xCoord, var5->yCoord, var5->zCoord, this->rand); }
	}
	}
	} else {
	if(this->checkChunksExist(var1 - var6, var2 - var6, var3 - var6, var1 + var6, var2 + var6, var3 + var6)) {
	if(var4 > 0) {
	var5->setScheduledTime((long long)Block::s_blocksList[var4]->tickRate() + this->worldTime);
	}
	if(!this->scheduledTickSet.count(var5)) {
	this->scheduledTickSet.insert(var5);
	this->scheduledTickTreeSet->add(var5);
	}
	}
	}
}

void World::updateEntities()
{
	listRemoveAll(this->loadedEntityList, this->unloadedEntityList);
	int var1;
	Entity* var2;
	int var3;
	int var4;
	for(var1 = 0; var1 < this->unloadedEntityList.size(); ++var1) {
	var2 = (Entity*)this->unloadedEntityList[var1];
	var3 = var2->chunkCoordX;
	var4 = var2->chunkCoordZ;
	if(var2->addedToChunk && this->chunkExists(var3, var4)) {
	this->getChunkFromChunkCoords(var3, var4)->removeEntity(var2);
	}
	}
	for(var1 = 0; var1 < this->unloadedEntityList.size(); ++var1) {
	this->releaseEntitySkin((Entity*)this->unloadedEntityList[var1]);
	}
	this->unloadedEntityList.clear();
	for(var1 = 0; var1 < this->loadedEntityList.size(); ++var1) {
	var2 = (Entity*)this->loadedEntityList[var1];
	if(var2->ridingEntity != NULL) {
	if(!var2->ridingEntity->isDead && var2->ridingEntity->riddenByEntity == var2) {
	continue;
	}
	var2->ridingEntity->riddenByEntity = NULL;
	var2->ridingEntity = NULL;
	}
	if(!var2->isDead) {
	this->updateEntity(var2);
	}
	if(var2->isDead) {
	var3 = var2->chunkCoordX;
	var4 = var2->chunkCoordZ;
	if(var2->addedToChunk && this->chunkExists(var3, var4)) {
	this->getChunkFromChunkCoords(var3, var4)->removeEntity(var2);
	}
	this->loadedEntityList.remove(var1--);
	this->releaseEntitySkin(var2);
	}
	}
	for(var1 = 0; var1 < this->loadedTileEntityList.size(); ++var1) {
	TileEntity* var5 = (TileEntity*)this->loadedTileEntityList[var1];
	var5->updateEntity();
	}
}

void World::updateEntity(Entity* var1)
{
	this->updateEntityWithOptionalForce(var1, true);
}

void World::updateEntityWithOptionalForce(Entity* var1, bool var2)
{
	int var3 = MathHelper::floor_double(var1->posX);
	int var4 = MathHelper::floor_double(var1->posZ);
	unsigned char var5 = 16;
	if(var2 || this->checkChunksExist(var3 - var5, 0, var4 - var5, var3 + var5, 128, var4 + var5)) {
	var1->lastTickPosX = var1->posX;
	var1->lastTickPosY = var1->posY;
	var1->lastTickPosZ = var1->posZ;
	var1->prevRotationYaw = var1->rotationYaw;
	var1->prevRotationPitch = var1->rotationPitch;
	if(var2 && var1->addedToChunk) {
	if(var1->ridingEntity != NULL) {
	var1->updateRidden();
	} else {
	var1->onUpdate();
	}
	}
	if(Double::isNaN(var1->posX) || Double::isInfinite(var1->posX)) {
	var1->posX = var1->lastTickPosX;
	}
	if(Double::isNaN(var1->posY) || Double::isInfinite(var1->posY)) {
	var1->posY = var1->lastTickPosY;
	}
	if(Double::isNaN(var1->posZ) || Double::isInfinite(var1->posZ)) {
	var1->posZ = var1->lastTickPosZ;
	}
	if(Double::isNaN((double)var1->rotationPitch) || Double::isInfinite((double)var1->rotationPitch)) {
	var1->rotationPitch = var1->prevRotationPitch;
	}
	if(Double::isNaN((double)var1->rotationYaw) || Double::isInfinite((double)var1->rotationYaw)) {
	var1->rotationYaw = var1->prevRotationYaw;
	}
	int var6 = MathHelper::floor_double(var1->posX / 16.0);
	int var7 = MathHelper::floor_double(var1->posY / 16.0);
	int var8 = MathHelper::floor_double(var1->posZ / 16.0);
	if(!var1->addedToChunk || var1->chunkCoordX != var6 || var1->chunkCoordY != var7 || var1->chunkCoordZ != var8) {
	if(var1->addedToChunk && this->chunkExists(var1->chunkCoordX, var1->chunkCoordZ)) {
	this->getChunkFromChunkCoords(var1->chunkCoordX, var1->chunkCoordZ)->removeEntityAtIndex(var1, var1->chunkCoordY);
	}
	if(this->chunkExists(var6, var8)) {
	var1->addedToChunk = true;
	this->getChunkFromChunkCoords(var6, var8)->addEntity(var1);
	} else {
	var1->addedToChunk = false;
	}
	}
	if(var2 && var1->addedToChunk && var1->riddenByEntity != NULL) {
	if(!var1->riddenByEntity->isDead && var1->riddenByEntity->ridingEntity == var1) {
	this->updateEntity(var1->riddenByEntity);
	} else {
	var1->riddenByEntity->ridingEntity = NULL;
	var1->riddenByEntity = NULL;
	}
	}
	}
}

bool World::checkIfAABBIsClear(AxisAlignedBB* var1)
{
	JList<void*> var2 = this->getEntitiesWithinAABBExcludingEntity((Entity*)NULL, var1);
	for(int var3 = 0; var3 < var2.size(); ++var3) {
	Entity* var4 = (Entity*)var2[var3];
	if(!var4->isDead && var4->preventEntitySpawning) {
	return false;
	}
	}
	return true;
}

bool World::getIsAnyLiquid(AxisAlignedBB* var1)
{
	int var2 = MathHelper::floor_double(var1->minX);
	int var3 = MathHelper::floor_double(var1->maxX + 1.0);
	int var4 = MathHelper::floor_double(var1->minY);
	int var5 = MathHelper::floor_double(var1->maxY + 1.0);
	int var6 = MathHelper::floor_double(var1->minZ);
	int var7 = MathHelper::floor_double(var1->maxZ + 1.0);
	if(var1->minX < 0.0) {
	--var2;
	}
	if(var1->minY < 0.0) {
	--var4;
	}
	if(var1->minZ < 0.0) {
	--var6;
	}
	for(int var8 = var2; var8 < var3; ++var8) {
	for(int var9 = var4; var9 < var5; ++var9) {
	for(int var10 = var6; var10 < var7; ++var10) {
	Block* var11 = Block::s_blocksList[this->getBlockId(var8, var9, var10)];
	if(var11 != NULL && var11->blockMaterial->getIsLiquid()) {
	return true;
	}
	}
	}
	}
	return false;
}

bool World::isBoundingBoxBurning(AxisAlignedBB* var1)
{
	int var2 = MathHelper::floor_double(var1->minX);
	int var3 = MathHelper::floor_double(var1->maxX + 1.0);
	int var4 = MathHelper::floor_double(var1->minY);
	int var5 = MathHelper::floor_double(var1->maxY + 1.0);
	int var6 = MathHelper::floor_double(var1->minZ);
	int var7 = MathHelper::floor_double(var1->maxZ + 1.0);
	for(int var8 = var2; var8 < var3; ++var8) {
	for(int var9 = var4; var9 < var5; ++var9) {
	for(int var10 = var6; var10 < var7; ++var10) {
	int var11 = this->getBlockId(var8, var9, var10);
	if(var11 == Block::s_fire->blockID || var11 == Block::s_lavaStill->blockID || var11 == Block::s_lavaMoving->blockID) {
	return true;
	}
	}
	}
	}
	return false;
}

bool World::handleMaterialAcceleration(AxisAlignedBB* var1, Material* var2, Entity* var3)
{
	int var4 = MathHelper::floor_double(var1->minX);
	int var5 = MathHelper::floor_double(var1->maxX + 1.0);
	int var6 = MathHelper::floor_double(var1->minY);
	int var7 = MathHelper::floor_double(var1->maxY + 1.0);
	int var8 = MathHelper::floor_double(var1->minZ);
	int var9 = MathHelper::floor_double(var1->maxZ + 1.0);
	bool var10 = false;
	Vec3D* var11 = Vec3D::createVector(0.0, 0.0, 0.0);
	for(int var12 = var4; var12 < var5; ++var12) {
	for(int var13 = var6; var13 < var7; ++var13) {
	for(int var14 = var8; var14 < var9; ++var14) {
	Block* var15 = Block::s_blocksList[this->getBlockId(var12, var13, var14)];
	if(var15 != NULL && var15->blockMaterial == var2) {
	double var16 = (double)((float)(var13 + 1) - BlockFluids::getFluidHeightPercent(this->getBlockMetadata(var12, var13, var14)));
	if((double)var7 >= var16) {
	var10 = true;
	var15->velocityToAddToEntity(this, var12, var13, var14, var3, var11);
	}
	}
	}
	}
	}
	if(var11->lengthVector() > 0.0) {
	var11 = var11->normalize();
	double var18 = 0.004;
	var3->motionX += var11->xCoord * var18;
	var3->motionY += var11->yCoord * var18;
	var3->motionZ += var11->zCoord * var18;
	}
	return var10;
}

bool World::func_689_a(AxisAlignedBB* var1, Material* var2)
{
	int var3 = MathHelper::floor_double(var1->minX);
	int var4 = MathHelper::floor_double(var1->maxX + 1.0);
	int var5 = MathHelper::floor_double(var1->minY);
	int var6 = MathHelper::floor_double(var1->maxY + 1.0);
	int var7 = MathHelper::floor_double(var1->minZ);
	int var8 = MathHelper::floor_double(var1->maxZ + 1.0);
	for(int var9 = var3; var9 < var4; ++var9) {
	for(int var10 = var5; var10 < var6; ++var10) {
	for(int var11 = var7; var11 < var8; ++var11) {
	Block* var12 = Block::s_blocksList[this->getBlockId(var9, var10, var11)];
	if(var12 != NULL && var12->blockMaterial == var2) {
	return true;
	}
	}
	}
	}
	return false;
}

bool World::isMaterialInBBExtended(AxisAlignedBB* var1, Material* var2)
{
	int var3 = MathHelper::floor_double(var1->minX);
	int var4 = MathHelper::floor_double(var1->maxX + 1.0);
	int var5 = MathHelper::floor_double(var1->minY);
	int var6 = MathHelper::floor_double(var1->maxY + 1.0);
	int var7 = MathHelper::floor_double(var1->minZ);
	int var8 = MathHelper::floor_double(var1->maxZ + 1.0);
	for(int var9 = var3; var9 < var4; ++var9) {
	for(int var10 = var5; var10 < var6; ++var10) {
	for(int var11 = var7; var11 < var8; ++var11) {
	Block* var12 = Block::s_blocksList[this->getBlockId(var9, var10, var11)];
	if(var12 != NULL && var12->blockMaterial == var2) {
	int var13 = this->getBlockMetadata(var9, var10, var11);
	double var14 = (double)(var10 + 1);
	if(var13 < 8) {
	var14 = (double)(var10 + 1) - (double)var13 / 8.0;
	}
	if(var14 >= var1->minY) {
	return true;
	}
	}
	}
	}
	}
	return false;
}

Explosion* World::createExplosion(Entity* var1, double var2, double var4, double var6, float var8)
{
	return this->newExplosion(var1, var2, var4, var6, var8, false);
}

Explosion* World::newExplosion(Entity* var1, double var2, double var4, double var6, float var8, bool var9)
{
	Explosion* var10 = new Explosion(this, var1, var2, var4, var6, var8);
	var10->isFlaming = var9;
	var10->doExplosionA();
	var10->doExplosionB();
	return var10;
}

float World::addKey(Vec3D* var1, AxisAlignedBB* var2)
{
	double var3 = 1.0 / ((var2->maxX - var2->minX) * 2.0 + 1.0);
	double var5 = 1.0 / ((var2->maxY - var2->minY) * 2.0 + 1.0);
	double var7 = 1.0 / ((var2->maxZ - var2->minZ) * 2.0 + 1.0);
	int var9 = 0;
	int var10 = 0;
	for(float var11 = 0.0F; var11 <= 1.0F; var11 = (float)((double)var11 + var3)) {
	for(float var12 = 0.0F; var12 <= 1.0F; var12 = (float)((double)var12 + var5)) {
	for(float var13 = 0.0F; var13 <= 1.0F; var13 = (float)((double)var13 + var7)) {
	double var14 = var2->minX + (var2->maxX - var2->minX) * (double)var11;
	double var16 = var2->minY + (var2->maxY - var2->minY) * (double)var12;
	double var18 = var2->minZ + (var2->maxZ - var2->minZ) * (double)var13;
	if(this->rayTraceBlocks(Vec3D::createVector(var14, var16, var18), var1) == NULL) {
	++var9;
	}
	++var10;
	}
	}
	}
	return (float)var9 / (float)var10;
}

void World::onBlockHit(int var1, int var2, int var3, int var4)
{
	if(var4 == 0) {
	--var2;
	}
	if(var4 == 1) {
	++var2;
	}
	if(var4 == 2) {
	--var3;
	}
	if(var4 == 3) {
	++var3;
	}
	if(var4 == 4) {
	--var1;
	}
	if(var4 == 5) {
	++var1;
	}
	if(this->getBlockId(var1, var2, var3) == Block::s_fire->blockID) {
	this->playSoundEffect((double)((float)var1 + 0.5F),(double)((float)var2 + 0.5F),(double)((float)var3 + 0.5F),"random.fizz",0.5F,2.6F + (this->rand->nextFloat() - this->rand->nextFloat()) * 0.8F);
	this->setBlockWithNotify(var1, var2, var3, 0);
	}
}

Entity* World::findEntityByClass(Class* var1)
{
	return NULL;
}

JavaString World::getDebugLoadedEntities()
{
	return (JCat() + "All: " + this->loadedEntityList.size());
}

TileEntity* World::getBlockTileEntity(int var1, int var2, int var3)
{
	Chunk* var4 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	return var4 != NULL ? var4->getChunkBlockTileEntity(var1 & 15, var2, var3 & 15) : NULL;
}

void World::setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4)
{
	Chunk* var5 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	if(var5 != NULL) {
	var5->setChunkBlockTileEntity(var1 & 15, var2, var3 & 15, var4);
	}
}

void World::removeBlockTileEntity(int var1, int var2, int var3)
{
	Chunk* var4 = this->getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
	if(var4 != NULL) {
	var4->removeChunkBlockTileEntity(var1 & 15, var2, var3 & 15);
	}
}

bool World::isBlockOpaqueCube(int var1, int var2, int var3)
{
	Block* var4 = Block::s_blocksList[this->getBlockId(var1, var2, var3)];
	return var4 == NULL ? false : var4->isOpaqueCube();
}

void World::saveWorldIndicator(IProgressUpdate* var1)
{
	this->saveWorld(true, var1);
}

bool World::updatingLighting()
{
	if(this->lightingUpdateDepth >= 50) {
	return false;
	} else {
	++this->lightingUpdateDepth;
	// try/finally: RAII guard decrements lightingUpdateDepth on every exit path
	struct DepthGuard { int* d; DepthGuard(int* p):d(p){} ~DepthGuard(){ --(*d); } } var0_guard(&this->lightingUpdateDepth);
	int var1 = 5000;
	bool var2;
	while(this->lightingToUpdate.size() > 0) {
	--var1;
	if(var1 <= 0) {
	var2 = true;
	return var2;
	}
	((MetadataChunkBlock*)this->lightingToUpdate.remove((int)this->lightingToUpdate.size() - 1))->updateSkylight(this);
	}
	var2 = false;
	return var2;
	}
}

void World::scheduleLightingUpdate(int var1, int var2, int var3, int var4, int var5, int var6, int var7)
{
	this->scheduleLightingUpdateInternal(var1, var2, var3, var4, var5, var6, var7, true);
}

void World::scheduleLightingUpdateInternal(int var1, int var2, int var3, int var4, int var5, int var6, int var7, bool var8)
{
	if(!this->worldProvider->hasNoSky || var1 != EnumSkyBlock::Sky) {
	++s_lightingUpdatesScheduled;
	if(s_lightingUpdatesScheduled == 50) {
	--s_lightingUpdatesScheduled;
	} else {
	int var9 = (var5 + var2) / 2;
	int var10 = (var7 + var4) / 2;
	if(!this->blockExists(var9, 64, var10)) {
	--s_lightingUpdatesScheduled;
	} else {
	int var11 = this->lightingToUpdate.size();
	if(var8) {
	int var12 = 4;
	if(var12 > var11) {
	var12 = var11;
	}
	for(int var13 = 0; var13 < var12; ++var13) {
	MetadataChunkBlock* var14 = (MetadataChunkBlock*)this->lightingToUpdate[this->lightingToUpdate.size() - var13 - 1];
	if(var14->skyBlock == var1 && var14->expandToContain(var2, var3, var4, var5, var6, var7)) {
	--s_lightingUpdatesScheduled;
	return;
	}
	}
	}
	this->lightingToUpdate.push_back(new MetadataChunkBlock(var1, var2, var3, var4, var5, var6, var7));
	if(this->lightingToUpdate.size() > 100000) {
	this->lightingToUpdate.clear();
	}
	--s_lightingUpdatesScheduled;
	}
	}
	}
}

void World::calculateInitialSkylight()
{
	int var1 = this->calculateSkylightSubtracted(1.0F);
	if(var1 != this->skylightSubtracted) {
	this->skylightSubtracted = var1;
	}
}

void World::tick()
{
	SpawnerAnimals::performSpawning(this);
	this->chunkProvider->unloadQueuedChunks();
	int var1 = this->calculateSkylightSubtracted(1.0F);
	if(var1 != this->skylightSubtracted) {
	this->skylightSubtracted = var1;
	for(int var2 = 0; var2 < this->worldAccesses.size(); ++var2) {
	((IWorldAccess*)this->worldAccesses[var2])->updateAllRenderers();
	}
	}
	++this->worldTime;
	if(this->worldTime % (long long)this->autosavePeriod == 0LL) {
	this->saveWorld(false, (IProgressUpdate*)NULL);
	}
	this->TickUpdates(false);
	this->updateBlocksAroundPlayers();
}

void World::updateBlocksAroundPlayers()
{
	this->activeChunkSet.clear();
	int var3;
	int var4;
	int var6;
	int var7;
	for(int var1 = 0; var1 < this->playerEntities.size(); ++var1) {
	EntityPlayer* var2 = (EntityPlayer*)this->playerEntities[var1];
	var3 = MathHelper::floor_double(var2->posX / 16.0);
	var4 = MathHelper::floor_double(var2->posZ / 16.0);
	unsigned char var5 = 9;
	for(var6 = -var5; var6 <= var5; ++var6) {
	for(var7 = -var5; var7 <= var5; ++var7) {
	this->activeChunkSet.insert(new ChunkCoordIntPair(var6 + var3, var7 + var4));
	}
	}
	}
	if(this->ambientSoundTimer > 0) {
	--this->ambientSoundTimer;
	}
	for(std::set<void*>::iterator var12 = this->activeChunkSet.begin(); var12 != this->activeChunkSet.end(); ++var12) {
	ChunkCoordIntPair* var13 = (ChunkCoordIntPair*)(*var12);
	var3 = var13->chunkXPos * 16;
	var4 = var13->chunkZPos * 16;
	Chunk* var14 = this->getChunkFromChunkCoords(var13->chunkXPos, var13->chunkZPos);
	int var8;
	int var9;
	int var10;
	if(this->ambientSoundTimer == 0) {
	this->updateLCG = this->updateLCG * 3 + this->updateLCGConstant;
	var6 = this->updateLCG >> 2;
	var7 = var6 & 15;
	var8 = var6 >> 8 & 15;
	var9 = var6 >> 16 & 127;
	var10 = var14->getBlockID(var7, var9, var8);
	var7 += var3;
	var8 += var4;
	if(var10 == 0 && this->getBlockLightValue(var7, var9, var8) <= this->rand->nextInt(8) && this->getSavedLightValue(EnumSkyBlock::Sky, var7, var9, var8) <= 0) {
	EntityPlayer* var11 = this->getClosestPlayer((double)var7 + 0.5, (double)var9 + 0.5, (double)var8 + 0.5, 8.0);
	if(var11 != NULL && var11->getDistanceSq((double)var7 + 0.5, (double)var9 + 0.5, (double)var8 + 0.5) > 4.0) {
	this->playSoundEffect((double)var7 + 0.5,(double)var9 + 0.5,(double)var8 + 0.5,"ambient.cave.cave",0.7F,0.8F + this->rand->nextFloat() * 0.2F);
	this->ambientSoundTimer = this->rand->nextInt(12000) + 6000;
	}
	}
	}
	for(var6 = 0; var6 < 80; ++var6) {
	this->updateLCG = this->updateLCG * 3 + this->updateLCGConstant;
	var7 = this->updateLCG >> 2;
	var8 = var7 & 15;
	var9 = var7 >> 8 & 15;
	var10 = var7 >> 16 & 127;
	unsigned char var15 = var14->blocks[var8 << 11 | var9 << 7 | var10];
	if(Block::s_tickOnLoad[var15]) {
	if (Block::s_blocksList[var15] != NULL) { Block::s_blocksList[var15]->updateTick(this, var8 + var3, var10, var9 + var4, this->rand); }
	}
	}
	}
}

bool World::TickUpdates(bool var1)
{
	int var2 = this->scheduledTickTreeSet->size();
	if(var2 != this->scheduledTickSet.size()) {
	throw new IllegalStateException("TickNextTick list out of synch");
	} else {
	if(var2 > 1000) {
	var2 = 1000;
	}
	for(int var3 = 0; var3 < var2; ++var3) {
	NextTickListEntry* var4 = (NextTickListEntry*)this->scheduledTickTreeSet->first();
	if(!var1 && var4->scheduledTime > this->worldTime) {
	break;
	}
	this->scheduledTickTreeSet->remove(var4);
	this->scheduledTickSet.erase(var4);
	unsigned char var5 = 8;
	if(this->checkChunksExist(var4->xCoord - var5, var4->yCoord - var5, var4->zCoord - var5, var4->xCoord + var5, var4->yCoord + var5, var4->zCoord + var5)) {
	int var6 = this->getBlockId(var4->xCoord, var4->yCoord, var4->zCoord);
	if(var6 == var4->blockID && var6 > 0) {
	if (Block::s_blocksList[var6] != NULL) { Block::s_blocksList[var6]->updateTick(this, var4->xCoord, var4->yCoord, var4->zCoord, this->rand); }
	}
	}
	}
	return this->scheduledTickTreeSet->size() != 0;
	}
}

void World::randomDisplayUpdates(int var1, int var2, int var3)
{
	unsigned char var4 = 16;
	Random* var5 = new Random();
	for(int var6 = 0; var6 < 1000; ++var6) {
	int var7 = var1 + this->rand->nextInt(var4) - this->rand->nextInt(var4);
	int var8 = var2 + this->rand->nextInt(var4) - this->rand->nextInt(var4);
	int var9 = var3 + this->rand->nextInt(var4) - this->rand->nextInt(var4);
	int var10 = this->getBlockId(var7, var8, var9);
	if(var10 > 0) {
	if (Block::s_blocksList[var10] != NULL) { Block::s_blocksList[var10]->randomDisplayTick(this, var7, var8, var9, var5); }
	}
	}
}

JList<void*> World::getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB* var2)
{
	this->entitiesWithinAABB.clear();
	int var3 = MathHelper::floor_double((var2->minX - 2.0) / 16.0);
	int var4 = MathHelper::floor_double((var2->maxX + 2.0) / 16.0);
	int var5 = MathHelper::floor_double((var2->minZ - 2.0) / 16.0);
	int var6 = MathHelper::floor_double((var2->maxZ + 2.0) / 16.0);
	for(int var7 = var3; var7 <= var4; ++var7) {
	for(int var8 = var5; var8 <= var6; ++var8) {
	if(this->chunkExists(var7, var8)) {
	this->getChunkFromChunkCoords(var7, var8)->getEntitiesWithinAABBForEntity(var1, var2, this->entitiesWithinAABB);
	}
	}
	}
	return this->entitiesWithinAABB;
}

JList<void*> World::getEntitiesWithinAABB(Class* var1, AxisAlignedBB* var2)
{
	int var3 = MathHelper::floor_double((var2->minX - 2.0) / 16.0);
	int var4 = MathHelper::floor_double((var2->maxX + 2.0) / 16.0);
	int var5 = MathHelper::floor_double((var2->minZ - 2.0) / 16.0);
	int var6 = MathHelper::floor_double((var2->maxZ + 2.0) / 16.0);
	JList<void*> var7;
	for(int var8 = var3; var8 <= var4; ++var8) {
	for(int var9 = var5; var9 <= var6; ++var9) {
	if(this->chunkExists(var8, var9)) {
	this->getChunkFromChunkCoords(var8, var9)->getEntitiesOfTypeWithinAAAB(var1, var2, var7);
	}
	}
	}
	return var7;
}

JList<void*> World::getLoadedEntityList()
{
	return this->loadedEntityList;
}

void World::markTileEntityForUpdate(int var1, int var2, int var3, TileEntity* var4)
{
	if(this->blockExists(var1, var2, var3)) {
	this->getChunkFromBlockCoords(var1, var3)->setChunkModified();
	}
	for(int var5 = 0; var5 < this->worldAccesses.size(); ++var5) {
	((IWorldAccess*)this->worldAccesses[var5])->onTileEntityChanged(var1, var2, var3, var4);
	}
}

int World::countEntities(Class* var1)
{
	int var2 = 0;
	for(int var3 = 0; var3 < this->loadedEntityList.size(); ++var3) {
	Entity* var4 = (Entity*)this->loadedEntityList[var3];
	if(var1->isAssignableFrom(Class())) {
	++var2;
	}
	}
	return var2;
}

void World::addLoadedEntities(JList<void*> var1)
{
	this->loadedEntityList.addAll(var1);
	for(int var2 = 0; var2 < var1.size(); ++var2) {
	this->obtainEntitySkin((Entity*)var1[var2]);
	}
}

void World::unloadEntities(JList<void*> var1)
{
	this->unloadedEntityList.addAll(var1);
}

void World::loadAllChunks()
{
	while(this->chunkProvider->unloadQueuedChunks()) {
	}
}

bool World::canBlockBePlacedAt(int var1, int var2, int var3, int var4, bool var5)
{
	int var6 = this->getBlockId(var2, var3, var4);
	Block* var7 = Block::s_blocksList[var6];
	Block* var8 = Block::s_blocksList[var1];
	AxisAlignedBB* var9 = var8->getCollisionBoundingBoxFromPool(this, var2, var3, var4);
	if(var5) {
	var9 = NULL;
	}
	return var9 != NULL && !this->checkIfAABBIsClear(var9) ? false : (var7 != Block::s_waterStill && var7 != Block::s_waterMoving && var7 != Block::s_lavaStill && var7 != Block::s_lavaMoving && var7 != Block::s_fire && var7 != Block::s_snow ? var1 > 0 && var7 == NULL && var8->canPlaceBlockAt(this, var2, var3, var4) : true);
}

PathEntity* World::getPathToEntity(Entity* var1, Entity* var2, float var3)
{
	int var4 = MathHelper::floor_double(var1->posX);
	int var5 = MathHelper::floor_double(var1->posY);
	int var6 = MathHelper::floor_double(var1->posZ);
	int var7 = (int)(var3 + 16.0F);
	int var8 = var4 - var7;
	int var9 = var5 - var7;
	int var10 = var6 - var7;
	int var11 = var4 + var7;
	int var12 = var5 + var7;
	int var13 = var6 + var7;
	ChunkCache* var14 = new ChunkCache(this, var8, var9, var10, var11, var12, var13);
	return (new Pathfinder(var14))->createEntityPathTo(var1, var2, var3);
}

PathEntity* World::getEntityPathToXYZ(Entity* var1, int var2, int var3, int var4, float var5)
{
	int var6 = MathHelper::floor_double(var1->posX);
	int var7 = MathHelper::floor_double(var1->posY);
	int var8 = MathHelper::floor_double(var1->posZ);
	int var9 = (int)(var5 + 8.0F);
	int var10 = var6 - var9;
	int var11 = var7 - var9;
	int var12 = var8 - var9;
	int var13 = var6 + var9;
	int var14 = var7 + var9;
	int var15 = var8 + var9;
	ChunkCache* var16 = new ChunkCache(this, var10, var11, var12, var13, var14, var15);
	return (new Pathfinder(var16))->createEntityPathTo(var1, var2, var3, var4, var5);
}

bool World::isBlockProvidingPowerTo(int var1, int var2, int var3, int var4)
{
	int var5 = this->getBlockId(var1, var2, var3);
	return var5 == 0 ? false : Block::s_blocksList[var5]->isIndirectlyPoweringTo(this, var1, var2, var3, var4);
}

bool World::isBlockGettingPowered(int var1, int var2, int var3)
{
	return this->isBlockProvidingPowerTo(var1, var2 - 1, var3, 0) ? true : (this->isBlockProvidingPowerTo(var1, var2 + 1, var3, 1) ? true : (this->isBlockProvidingPowerTo(var1, var2, var3 - 1, 2) ? true : (this->isBlockProvidingPowerTo(var1, var2, var3 + 1, 3) ? true : (this->isBlockProvidingPowerTo(var1 - 1, var2, var3, 4) ? true : this->isBlockProvidingPowerTo(var1 + 1, var2, var3, 5)))));
}

bool World::isBlockIndirectlyProvidingPowerTo(int var1, int var2, int var3, int var4)
{
	if(this->isBlockOpaqueCube(var1, var2, var3)) {
	return this->isBlockGettingPowered(var1, var2, var3);
	} else {
	int var5 = this->getBlockId(var1, var2, var3);
	return var5 == 0 ? false : Block::s_blocksList[var5]->isPoweringTo(this, var1, var2, var3, var4);
	}
}

bool World::isBlockIndirectlyGettingPowered(int var1, int var2, int var3)
{
	return this->isBlockIndirectlyProvidingPowerTo(var1, var2 - 1, var3, 0) ? true : (this->isBlockIndirectlyProvidingPowerTo(var1, var2 + 1, var3, 1) ? true : (this->isBlockIndirectlyProvidingPowerTo(var1, var2, var3 - 1, 2) ? true : (this->isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 1, 3) ? true : (this->isBlockIndirectlyProvidingPowerTo(var1 - 1, var2, var3, 4) ? true : this->isBlockIndirectlyProvidingPowerTo(var1 + 1, var2, var3, 5)))));
}

EntityPlayer* World::getClosestPlayerToEntity(Entity* var1, double var2)
{
	return this->getClosestPlayer(var1->posX, var1->posY, var1->posZ, var2);
}

EntityPlayer* World::getClosestPlayer(double var1, double var3, double var5, double var7)
{
	double var9 = -1.0;
	EntityPlayer* var11 = NULL;
	for(int var12 = 0; var12 < this->playerEntities.size(); ++var12) {
	EntityPlayer* var13 = (EntityPlayer*)this->playerEntities[var12];
	double var14 = var13->getDistanceSq(var1, var3, var5);
	if((var7 < 0.0 || var14 < var7 * var7) && (var9 == -1.0 || var14 < var9)) {
	var9 = var14;
	var11 = var13;
	}
	}
	return var11;
}

void World::setChunkData(int var1, int var2, int var3, int var4, int var5, int var6, JArray<unsigned char > var7)
{
	int var8 = var1 >> 4;
	int var9 = var3 >> 4;
	int var10 = var1 + var4 - 1 >> 4;
	int var11 = var3 + var6 - 1 >> 4;
	int var12 = 0;
	int var13 = var2;
	int var14 = var2 + var5;
	if(var2 < 0) {
	var13 = 0;
	}
	if(var14 > 128) {
	var14 = 128;
	}
	for(int var15 = var8; var15 <= var10; ++var15) {
	int var16 = var1 - var15 * 16;
	int var17 = var1 + var4 - var15 * 16;
	if(var16 < 0) {
	var16 = 0;
	}
	if(var17 > 16) {
	var17 = 16;
	}
	for(int var18 = var9; var18 <= var11; ++var18) {
	int var19 = var3 - var18 * 16;
	int var20 = var3 + var6 - var18 * 16;
	if(var19 < 0) {
	var19 = 0;
	}
	if(var20 > 16) {
	var20 = 16;
	}
	var12 = this->getChunkFromChunkCoords(var15, var18)->setChunkData(var7, var16, var13, var19, var17, var14, var20, var12);
	this->markBlockRangeNeedsUpdate(var15 * 16 + var16, var13, var18 * 16 + var19, var15 * 16 + var17, var14, var18 * 16 + var20);
	}
	}
}

void World::sendQuittingDisconnectingPacket()
{
}

void World::checkSessionLock()
{
	try {
	File* var1 = new File(this->saveDirectory, "session.lock");
	DataInputStream* var2 = new DataInputStream(new FileInputStream(var1));
	// try/finally: ensure var2->close() runs on normal and exceptional paths
	try {
	if(var2->readLong() != this->sessionLockStamp) {
	throw new MinecraftException("The save is being accessed from another location, aborting");
	}
	} catch (...) {
	var2->close();
	throw;
	}
	var2->close();
	} catch (IOException var7) {
	throw new MinecraftException("Failed to check session lock, aborting");
	}
}

void World::setWorldTime(long long var1)
{
	this->worldTime = var1;
}

void World::joinEntityInSurroundings(Entity* var1)
{
	int var2 = MathHelper::floor_double(var1->posX / 16.0);
	int var3 = MathHelper::floor_double(var1->posZ / 16.0);
	unsigned char var4 = 2;
	for(int var5 = var2 - var4; var5 <= var2 + var4; ++var5) {
	for(int var6 = var3 - var4; var6 <= var3 + var4; ++var6) {
	this->getChunkFromChunkCoords(var5, var6);
	}
	}
	if(!this->loadedEntityList.contains(var1)) {
	this->loadedEntityList.push_back(var1);
	}
}

bool World::canMineBlock(EntityPlayer* var1, int var2, int var3, int var4)
{
	return true;
}

void World::setEntityState(Entity* var1, unsigned char var2)
{
}

void World::releaseUnloadedEntities()
{
	listRemoveAll(this->loadedEntityList, this->unloadedEntityList);
	int var1;
	Entity* var2;
	int var3;
	int var4;
	for(var1 = 0; var1 < this->unloadedEntityList.size(); ++var1) {
	var2 = (Entity*)this->unloadedEntityList[var1];
	var3 = var2->chunkCoordX;
	var4 = var2->chunkCoordZ;
	if(var2->addedToChunk && this->chunkExists(var3, var4)) {
	this->getChunkFromChunkCoords(var3, var4)->removeEntity(var2);
	}
	}
	for(var1 = 0; var1 < this->unloadedEntityList.size(); ++var1) {
	this->releaseEntitySkin((Entity*)this->unloadedEntityList[var1]);
	}
	this->unloadedEntityList.clear();
	for(var1 = 0; var1 < this->loadedEntityList.size(); ++var1) {
	var2 = (Entity*)this->loadedEntityList[var1];
	if(var2->ridingEntity != NULL) {
	if(!var2->ridingEntity->isDead && var2->ridingEntity->riddenByEntity == var2) {
	continue;
	}
	var2->ridingEntity->riddenByEntity = NULL;
	var2->ridingEntity = NULL;
	}
	if(var2->isDead) {
	var3 = var2->chunkCoordX;
	var4 = var2->chunkCoordZ;
	if(var2->addedToChunk && this->chunkExists(var3, var4)) {
	this->getChunkFromChunkCoords(var3, var4)->removeEntity(var2);
	}
	this->loadedEntityList.remove(var1--);
	this->releaseEntitySkin(var2);
	}
	}
}
