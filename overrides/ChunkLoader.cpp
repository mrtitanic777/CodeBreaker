#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkLoader.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "CompressedStreamTools.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityList.h"
#include "EntityPlayer.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FileInputStream.h"
#include "FileOutputStream.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

// --- Local helpers ---
namespace {
// java.lang.Integer.toString(int, radix): the central runtime stub only provides the
// single-arg (base-10) overload, so reproduce the radix form locally (used for the
// base-36 chunk file/dir names). Faithful to Java's lowercase-digit output.
static JavaString intToStringRadix(int value, int radix) {
	if (radix < 2 || radix > 36) radix = 10;
	if (value == 0) return JavaString("0");
	bool neg = value < 0;
	// Use unsigned-safe accumulation to handle Integer.MIN_VALUE.
	unsigned long long v = neg ? (unsigned long long)(-(long long)value)
	                           : (unsigned long long)value;
	const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	std::string out;
	while (v > 0) { out.insert(out.begin(), digits[v % (unsigned)radix]); v /= (unsigned)radix; }
	if (neg) out.insert(out.begin(), '-');
	return JavaString(out);
}
// java.io.File.delete(): 'delete' is a C++ keyword and the File runtime stub exposes no
// equivalent, so route it through a helper. Returns success like the Java method.
// NOTE: real on-disk deletion depends on a real File impl (see runtime-stub gaps).
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // In saveChunk(...), the previous chunk file is deleted before the temp file is
 *  // renamed into place:
 *  if(var3.exists()) {
 *      var3.delete();
 *  }
 *  // File.delete() deletes the file/directory on disk and returns true on success.
 *  // The helper below no-ops (returns true without deleting), so the old chunk file
 *  // is NOT removed prior to renameTo().
 *  ---- end original Java ---- */
static bool fileDelete(File*) { return true; }

// Write every TileEntity value of a chunk's tile-entity map into an NBTTagList. Templated on
// the map type so it is agnostic to the map's key type (Chunk's chunkTileEntityMap key has
// been keyed by either string or packed-long across converter revisions).
template <typename MapT>
static void _writeTileEntities(MapT& m, NBTTagList* dst) {
	for (typename MapT::iterator it = m.begin(); it != m.end(); ++it) {
		TileEntity* te = (TileEntity*)it->second;
		NBTTagCompound* c = new NBTTagCompound();
		te->writeToNBT(c);
		dst->setTag(c);
	}
}
}

ChunkLoader::ChunkLoader(File* var1, bool var2)
	: saveDir(NULL)
	, createIfNecessary(0)
{
	this->saveDir = var1;
	this->createIfNecessary = var2;
}

ChunkLoader::~ChunkLoader()
{
}

File* ChunkLoader::chunkFileForXZ(int var1, int var2)
{
	JavaString var3 = (JCat() + "c." + intToStringRadix(var1,36) + "." + intToStringRadix(var2,36) + ".dat");
	JavaString var4 = intToStringRadix(var1 & 63, 36);
	JavaString var5 = intToStringRadix(var2 & 63, 36);
	File* var6 = new File(this->saveDir, var4);
	if(!var6->exists()) {
	if(!this->createIfNecessary) {
	return NULL;
	}
	var6->mkdir();
	}
	var6 = new File(var6, var5);
	if(!var6->exists()) {
	if(!this->createIfNecessary) {
	return NULL;
	}
	var6->mkdir();
	}
	var6 = new File(var6, var3);
	return !var6->exists() && !this->createIfNecessary ? NULL : var6;
}

Chunk* ChunkLoader::loadChunk(World* var1, int var2, int var3)
{
	// CONSOLE (gen perf): chunk persistence is DISABLED (read-only save dir -> saveChunk is a no-op), so no
	// chunk file can ever exist. The original chunkFileForXZ()+exists() probed the HOST filesystem ~3x PER
	// CHUNK -> a slow ResolvePath / C000000F (file-not-found) STORM during world-gen (the "exhaustively slow"
	// generation: hundreds of host-FS round-trips). Skip the probe entirely and return NULL so the chunk
	// provider just GENERATES the chunk -- zero filesystem access -> fast gen. Restore the body when the
	// writable Save: mount is wired (docs/platform/notes/05-xdk-subsystem-integration.md).
	(void)var1; (void)var2; (void)var3;
	return NULL;
#if 0
	File* var4 = this->chunkFileForXZ(var2, var3);
	if(var4 != NULL && var4->exists()) {
	try {
	FileInputStream* var5 = new FileInputStream(var4);
	NBTTagCompound* var6 = CompressedStreamTools::readCompressed(var5);
	if(!var6->hasKey("Level")) {
	System::out.println((JCat() + "Chunk file at " + var2 + "," + var3 + " is missing level data, skipping"));
	return NULL;
	}
	if(!var6->getCompoundTag("Level")->hasKey("Blocks")) {
	System::out.println((JCat() + "Chunk file at " + var2 + "," + var3 + " is missing block data, skipping"));
	return NULL;
	}
	Chunk* var7 = loadChunkIntoWorldFromCompound(var1, var6->getCompoundTag("Level"));
	if(!var7->isAtLocation(var2, var3)) {
	System::out.println((JCat() + "Chunk file at " + var2 + "," + var3 + " is in the wrong location; relocating. (Expected " + var2 + ", " + var3 + ", got " + var7->xPosition + ", " + var7->zPosition + ")"));
	var6->setInteger("xPos", var2);
	var6->setInteger("zPos", var3);
	var7 = loadChunkIntoWorldFromCompound(var1, var6->getCompoundTag("Level"));
	}
	return var7;
	} catch (Exception var8) {
	var8.printStackTrace();
	}
	}
	return NULL;
#endif
}

void ChunkLoader::saveChunk(World* var1, Chunk* var2)
{
	// CONSOLE: chunk persistence is DISABLED for now. The save dir is read-only on the emulator
	// (ACCESS_DENIED), and storeChunkInCompound dereferenced a NULL chunk array/entity mid-save -> a
	// hardware fault (IP 0x0) the try/catch can't catch -> crash during "Saving chunks". Skip the save
	// so world-gen completes into memory and the world is enterable.
	// TODO(persistence): real chunk save needs the writable Save: STFS mount (Save360_Init) + LZX
	// compression (CompressedStreamTools is an inert stub) + NULL-guards in storeChunkInCompound.
	// Original body is in git history; rewrite it properly when wiring persistence (docs/platform/notes/05-xdk-subsystem-integration.md).
	(void)var1; (void)var2;
}

void ChunkLoader::storeChunkInCompound(Chunk* var1, World* var2, NBTTagCompound* var3)
{
	var2->checkSessionLock();
	var3->setInteger("xPos", var1->xPosition);
	var3->setInteger("zPos", var1->zPosition);
	var3->setLong("LastUpdate", var2->worldTime);
	var3->setByteArray("Blocks", var1->blocks);
	var3->setByteArray("Data", var1->data->data);
	var3->setByteArray("SkyLight", var1->skylightMap->data);
	var3->setByteArray("BlockLight", var1->blocklightMap->data);
	var3->setByteArray("HeightMap", var1->heightMap);
	var3->setBoolean("TerrainPopulated", var1->isTerrainPopulated);
	var1->hasEntities = false;
	NBTTagList* var4 = new NBTTagList();
	NBTTagCompound* var8;
	for(int var5 = 0; var5 < var1->entities.length; ++var5) {
	JList<void*>& _ents = var1->entities[var5];
	for(JList<void*>::iterator var6 = _ents.begin(); var6 != _ents.end(); ++var6) {
	Entity* var7 = (Entity*)*var6;
	var1->hasEntities = true;
	var8 = new NBTTagCompound();
	if(var7->addEntityID(var8)) {
	var4->setTag(var8);
	}
	}
	}
	var3->setTag("Entities", var4);
	NBTTagList* var9 = new NBTTagList();
	_writeTileEntities(var1->chunkTileEntityMap, var9);
	var3->setTag("TileEntities", var9);
}

Chunk* ChunkLoader::loadChunkIntoWorldFromCompound(World* var0, NBTTagCompound* var1)
{
	int var2 = var1->getInteger("xPos");
	int var3 = var1->getInteger("zPos");
	Chunk* var4 = new Chunk(var0, var2, var3);
	var4->blocks = var1->getByteArray("Blocks");
	var4->data = new NibbleArray(var1->getByteArray("Data"));
	var4->skylightMap = new NibbleArray(var1->getByteArray("SkyLight"));
	var4->blocklightMap = new NibbleArray(var1->getByteArray("BlockLight"));
	var4->heightMap = var1->getByteArray("HeightMap");
	var4->isTerrainPopulated = var1->getBoolean("TerrainPopulated");
	if(!var4->data->isValid()) {
	var4->data = new NibbleArray(var4->blocks.length);
	}
	if(var4->heightMap == NULL || !var4->skylightMap->isValid()) {
	var4->heightMap = JArray<unsigned char >(256);
	var4->skylightMap = new NibbleArray(var4->blocks.length);
	var4->generateSkylightMap();
	}
	if(!var4->blocklightMap->isValid()) {
	var4->blocklightMap = new NibbleArray(var4->blocks.length);
	var4->removeUnknownBlocks();
	}
	NBTTagList* var5 = var1->getTagList("Entities");
	if(var5 != NULL) {
	for(int var6 = 0; var6 < var5->tagCount(); ++var6) {
	NBTTagCompound* var7 = (NBTTagCompound*)var5->tagAt(var6);
	Entity* var8 = EntityList::createEntityFromNBT(var7, var0);
	var4->hasEntities = true;
	if(var8 != NULL) {
	var4->addEntity(var8);
	}
	}
	}
	NBTTagList* var10 = var1->getTagList("TileEntities");
	if(var10 != NULL) {
	for(int var11 = 0; var11 < var10->tagCount(); ++var11) {
	NBTTagCompound* var12 = (NBTTagCompound*)var10->tagAt(var11);
	TileEntity* var9 = TileEntity::createAndLoadEntity(var12);
	if(var9 != NULL) {
	var4->addTileEntity(var9);
	}
	}
	}
	return var4;
}

void ChunkLoader::func_814_a()
{
}

void ChunkLoader::saveExtraData()
{
}

void ChunkLoader::saveExtraChunkData(World* var1, Chunk* var2)
{
}
