#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SpawnerAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SpawnerAnimals.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkCoordIntPair.h"
#include "ChunkPosition.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EnumCreatureType.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MathHelper.h"
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
#include "EntityCreature.h"
// Spawn factory entity types: Class::newInstance() is reflection (returns 0 on console), so we
// map the biome list's runtime Class identity back to a real ctor below (_createSpawnEntity).
#include "EntityZombie.h"
#include "EntityCreeper.h"
#include "EntitySheep.h"
#include "EntityPig.h"
#include "EntityChicken.h"
#include "EntityCow.h"

std::set<void*> SpawnerAnimals::s_eligibleChunksForSpawning;

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // from EnumCreatureType.java -- the per-constant fields lost to ordinal reduction:
 *  public enum EnumCreatureType {
 *      monster(IMobs.class, 100),
 *      creature(EntityAnimals.class, 20);
 *
 *      public final Class creatureClass;
 *      public final int maxNumberOfEntityType;
 *
 *      private EnumCreatureType(Class var3, int var4) {
 *          this.creatureClass = var3;
 *          this.maxNumberOfEntityType = var4;
 *      }
 *  }
 *  // and the call sites these shims stand in for, from performSpawning():
 *  for(int var28 = 0; var28 < EnumCreatureType.values().length; ++var28) {
 *      EnumCreatureType var29 = EnumCreatureType.values()[var28];
 *      if(var0.countEntities(var29.creatureClass) <= var29.maxNumberOfEntityType * eligibleChunksForSpawning.size() / 256) {
 *  ---- end original Java ---- */
// EnumCreatureType was reduced to ordinals (monster=0, creature=1) by the transpiler, losing the
// per-constant fields. Restore the AUTHENTIC Alpha data locally so the cap math is preserved:
//   monster : maxNumberOfEntityType = 100 (IMobs)
//   creature: maxNumberOfEntityType = 20  (EntityAnimals)
static const int _ECT_COUNT = 2;
static int _ectMaxNumberOfEntityType(int ord) { return ord == EnumCreatureType::monster ? 100 : 20; }
// creatureClass: the runtime Class stub can't carry distinct java.lang.Class identities,
// so we hand World.countEntities() a per-type placeholder Class*. NOTE: countEntities
// keys off this Class, so this is an approximation (see "semantics-need-review").
static Class* _ectCreatureClass(int /*ord*/) { static Class _c; return &_c; }

// --- Mob spawn factory -----------------------------------------------------------------------
// Java: `(EntityLiving)var33[var8].getConstructor(World.class).newInstance(var0)`. C++ has NO
// reflection -- Class::newInstance() returns 0, so the transpiled call yields a NULL EntityLiving,
// and the next line (setLocationAndAngles) is a virtual call through NULL = IP 0x0. That fault
// fires a few seconds into the world, the first tick a valid spawn site + RNG align (the World::
// tick crash). Map the biome list's runtime Class identity (typeid name, see MobSpawnerBase ctor)
// back to a real ctor: monsters {Spider,Zombie,Skeleton,Creeper}, creatures {Sheep,Pig,Chicken,Cow}.
static EntityLiving* _createSpawnEntity(Class* var0, World* var1)
{
	if(var0 == NULL) return NULL;
	std::string n = var0->getName();
	if(n == "EntitySpider")   return new EntitySpider(var1);
	if(n == "EntityZombie")   return new EntityZombie(var1);
	if(n == "EntitySkeleton") return new EntitySkeleton(var1);
	if(n == "EntityCreeper")  return new EntityCreeper(var1);
	if(n == "EntitySheep")    return new EntitySheep(var1);
	if(n == "EntityPig")      return new EntityPig(var1);
	if(n == "EntityChicken")  return new EntityChicken(var1);
	if(n == "EntityCow")      return new EntityCow(var1);
	return NULL;
}

SpawnerAnimals::~SpawnerAnimals()
{
}

ChunkPosition* SpawnerAnimals::getRandomSpawningPointInChunk(World* var0, int var1, int var2)
{
	int var3 = var1 + var0->rand->nextInt(16);
	int var4 = var0->rand->nextInt(128);
	int var5 = var2 + var0->rand->nextInt(16);
	return new ChunkPosition(var3, var4, var5);
}

int SpawnerAnimals::performSpawning(World* var0)
{
	s_eligibleChunksForSpawning.clear();
	int var1;
	for(var1 = 0; var1 < var0->playerEntities.size(); ++var1) {
	EntityPlayer* var2 = (EntityPlayer*)var0->playerEntities[var1];
	int var3 = MathHelper::floor_double(var2->posX / 16.0);
	int var4 = MathHelper::floor_double(var2->posZ / 16.0);
	unsigned char var5 = 8;
	for(int var6 = -var5; var6 <= var5; ++var6) {
	for(int var7 = -var5; var7 <= var5; ++var7) {
	s_eligibleChunksForSpawning.insert(new ChunkCoordIntPair(var6 + var3, var7 + var4));
	}
	}
	}
	var1 = 0;
	for(int var28 = 0; var28 < _ECT_COUNT; ++var28) {
	int var29 = var28;
	if(var0->countEntities(_ectCreatureClass(var29)) <= (int)(_ectMaxNumberOfEntityType(var29) * s_eligibleChunksForSpawning.size() / 256)) {
	std::set<void*>::iterator var30 = s_eligibleChunksForSpawning.begin();
	while(true) {
	int var8;
	int var10;
	int var11;
	int var12;
	JArray<Class* > var33;
	do {
	do {
	ChunkCoordIntPair* var31;
	do {
	do {
	do {
	if(var30 == s_eligibleChunksForSpawning.end()) {
	goto label113_cont;
	}
	var31 = (ChunkCoordIntPair*)*var30++;
	} while(var0->rand->nextInt(50) != 0);
	MobSpawnerBase* var32 = var0->getWorldChunkManager()->getBiomeForChunk(var31);
	var33 = var32->getEntitiesForType(var29);
	} while(var33 == NULL);
	} while(var33.length == 0);
	var8 = var0->rand->nextInt(var33.length);
	ChunkPosition* var9 = getRandomSpawningPointInChunk(var0, var31->chunkXPos * 16, var31->chunkZPos * 16);
	var10 = var9->x;
	var11 = var9->y;
	var12 = var9->z;
	} while(var0->isBlockOpaqueCube(var10, var11, var12));
	} while(var0->getBlockMaterial(var10, var11, var12) != Material::s_air);
	int var13 = 0;
	for(int var14 = 0; var14 < 3; ++var14) {
	int var15 = var10;
	int var16 = var11;
	int var17 = var12;
	unsigned char var18 = 6;
	for(int var19 = 0; var19 < 4; ++var19) {
	var15 += var0->rand->nextInt(var18) - var0->rand->nextInt(var18);
	var16 += var0->rand->nextInt(1) - var0->rand->nextInt(1);
	var17 += var0->rand->nextInt(var18) - var0->rand->nextInt(var18);
	if(var0->isBlockOpaqueCube(var15, var16 - 1, var17) && !var0->isBlockOpaqueCube(var15, var16, var17) && !var0->getBlockMaterial(var15, var16, var17)->getIsLiquid() && !var0->isBlockOpaqueCube(var15, var16 + 1, var17)) {
	float var20 = (float)var15 + 0.5F;
	float var21 = (float)var16;
	float var22 = (float)var17 + 0.5F;
	if(var0->getClosestPlayer((double)var20, (double)var21, (double)var22, 24.0) == NULL) {
	float var23 = var20 - (float)var0->spawnX;
	float var24 = var21 - (float)var0->spawnY;
	float var25 = var22 - (float)var0->spawnZ;
	float var26 = var23 * var23 + var24 * var24 + var25 * var25;
	if(var26 >= 576.0F) {
	EntityLiving* var34;
	try {
	/* ---- ORIGINAL JAVA (reflection -> factory) ----
	 *  var34 = (EntityLiving)var33[var8].getConstructor(new Class[]{World.class}).newInstance(new Object[]{var0});
	 *  ---- end original Java ---- */
	var34 = _createSpawnEntity(var33[var8], var0);
	} catch (Exception var27) {
	var27.printStackTrace();
	return var1;
	}
	if(var34 == NULL) {
	continue;   // unmapped spawn class -> skip this attempt rather than deref NULL (IP 0x0)
	}
	var34->setLocationAndAngles((double)var20, (double)var21, (double)var22, var0->rand->nextFloat() * 360.0F, 0.0F);
	if(var34->getCanSpawnHere()) {
	++var13;
	var0->entityJoinedWorld(var34);
	if((dynamic_cast<EntitySpider*>(var34) != NULL) && var0->rand->nextInt(100) == 0) {
	EntitySkeleton* var35 = new EntitySkeleton(var0);
	var35->setLocationAndAngles((double)var20, (double)var21, (double)var22, var34->rotationYaw, 0.0F);
	var0->entityJoinedWorld(var35);
	var35->mountEntity(var34);
	}
	if(var13 >= var34->getMaxSpawnedInChunk()) {
	goto label110_cont;
	}
	}
	var1 += var13;
	}
	}
	}
	}
	}
	label110_cont:;
	}
	}
	label113_cont:;
	}
	return var1;
}
