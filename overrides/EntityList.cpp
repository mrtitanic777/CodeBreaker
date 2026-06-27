#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <typeinfo>

#include "EntityList.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
// ---- Every entity class registered by the original static{} block ----
#include "EntityArrow.h"
#include "EntitySnowball.h"
#include "EntityPainting.h"
#include "EntityLiving.h"
#include "EntityMobs.h"
#include "EntityCreeper.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EntityZombieSimple.h"
#include "EntityZombie.h"
#include "EntitySlime.h"
#include "EntityGhast.h"
#include "EntityPigZombie.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntityCow.h"
#include "EntityChicken.h"
#include "EntityTNTPrimed.h"
#include "EntityFallingSand.h"
#include "EntityMinecart.h"
#include "EntityBoat.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
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

std::map<std::string, void*> EntityList::s_stringToClassMapping;
std::map<void*, void*> EntityList::s_classToStringMapping;
std::map<int, void*> EntityList::s_IDtoClassMapping;
std::map<void*, void*> EntityList::s_classToIDMapping;

// ---- Functional registry storage (RTTI identity + (World*) factories) ----
std::map<std::string, EntityList::EntityFactory> EntityList::s_nameToFactory;
std::map<int, EntityList::EntityFactory>         EntityList::s_idToFactory;
std::map<std::string, std::string>              EntityList::s_typeToName;
std::map<std::string, int>                      EntityList::s_typeToID;

// One template factory per ctor signature. The Minecraft entities are all
// constructed with a (World*) ctor (verified against every registered header).
template<class T> static void* j2x_newWorld(void* w) { return new T((World*)w); }

// Templated registration: class identity = typeid(T).name(); store the (World*)
// factory keyed by both name and id, and the reverse type->name / type->id maps.
template<class T>
void EntityList::registerEntity(const std::string& name, int id)
{
	const std::string key = typeid(T).name();
	s_nameToFactory[name] = &j2x_newWorld<T>;
	s_idToFactory[id]     = &j2x_newWorld<T>;
	s_typeToName[key]     = name;
	s_typeToID[key]       = id;
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  static {
 *      addMapping(EntityArrow.class, "Arrow", 10);
 *      addMapping(EntitySnowball.class, "Snowball", 11);
 *      addMapping(EntityItem.class, "Item", 1);
 *      addMapping(EntityPainting.class, "Painting", 9);
 *      addMapping(EntityLiving.class, "Mob", 48);
 *      addMapping(EntityMobs.class, "Monster", 49);
 *      addMapping(EntityCreeper.class, "Creeper", 50);
 *      addMapping(EntitySkeleton.class, "Skeleton", 51);
 *      addMapping(EntitySpider.class, "Spider", 52);
 *      addMapping(EntityZombieSimple.class, "Giant", 53);
 *      addMapping(EntityZombie.class, "Zombie", 54);
 *      addMapping(EntitySlime.class, "Slime", 55);
 *      addMapping(EntityGhast.class, "Ghast", 56);
 *      addMapping(EntityPigZombie.class, "PigZombie", 57);
 *      addMapping(EntityPig.class, "Pig", 90);
 *      addMapping(EntitySheep.class, "Sheep", 91);
 *      addMapping(EntityCow.class, "Cow", 92);
 *      addMapping(EntityChicken.class, "Chicken", 93);
 *      addMapping(EntityTNTPrimed.class, "PrimedTnt", 20);
 *      addMapping(EntityFallingSand.class, "FallingSand", 21);
 *      addMapping(EntityMinecart.class, "Minecart", 40);
 *      addMapping(EntityBoat.class, "Boat", 41);
 *  }
 *  ---- end original Java ----
 *  NOTE: the Java static{} block that registered every entity Class -> id/name
 *  mapping is dropped (no runtime Class registry exists). The four maps above are
 *  therefore never populated, so every lookup below resolves to nothing. */

// Port of the original Java static{} block: register every entity class with its name + id, keyed
// by RTTI identity (typeid) and backed by (World*) factories. Run in dependency order by
// j2x_initAll() (was a file-local auto-trigger -- undefined cross-TU order).
void EntityList::initStatics()
{
	EntityList::registerEntity<EntityArrow>("Arrow", 10);
	EntityList::registerEntity<EntitySnowball>("Snowball", 11);
	EntityList::registerEntity<EntityItem>("Item", 1);
	EntityList::registerEntity<EntityPainting>("Painting", 9);
	EntityList::registerEntity<EntityLiving>("Mob", 48);
	EntityList::registerEntity<EntityMobs>("Monster", 49);
	EntityList::registerEntity<EntityCreeper>("Creeper", 50);
	EntityList::registerEntity<EntitySkeleton>("Skeleton", 51);
	EntityList::registerEntity<EntitySpider>("Spider", 52);
	EntityList::registerEntity<EntityZombieSimple>("Giant", 53);
	EntityList::registerEntity<EntityZombie>("Zombie", 54);
	EntityList::registerEntity<EntitySlime>("Slime", 55);
	EntityList::registerEntity<EntityGhast>("Ghast", 56);
	EntityList::registerEntity<EntityPigZombie>("PigZombie", 57);
	EntityList::registerEntity<EntityPig>("Pig", 90);
	EntityList::registerEntity<EntitySheep>("Sheep", 91);
	EntityList::registerEntity<EntityCow>("Cow", 92);
	EntityList::registerEntity<EntityChicken>("Chicken", 93);
	EntityList::registerEntity<EntityTNTPrimed>("PrimedTnt", 20);
	EntityList::registerEntity<EntityFallingSand>("FallingSand", 21);
	EntityList::registerEntity<EntityMinecart>("Minecart", 40);
	EntityList::registerEntity<EntityBoat>("Boat", 41);
}

EntityList::~EntityList()
{
}

void EntityList::addMapping(Class* var0, const JavaString& var1, int var2)
{
	// String value boxed as a heap JavaString*; Integer value boxed into the
	// void* itself. Class keys/values use the Class* pointer.
	s_stringToClassMapping[var1] = var0;
	s_classToStringMapping[(void*)var0] = (void*)new JavaString(var1);
	s_IDtoClassMapping[Integer::valueOf(var2)] = var0;
	s_classToIDMapping[(void*)var0] = (void*)(long long)Integer::valueOf(var2);
}

Entity* EntityList::createEntityInWorld(const JavaString& var0, World* var1)
{
	Entity* var2 = NULL;
	try {
	/* ---- ORIGINAL JAVA ----
	 *  Class var3 = (Class)stringToClassMapping.get(var0);
	 *  var2 = (Entity)var3.getConstructor(new Class[]{World.class}).newInstance(new Object[]{var1});
	 *  ---- end original Java ----
	 *  Reflection is reduced to a name->(World*) factory lookup. */
	std::map<std::string, EntityFactory>::iterator var3 = s_nameToFactory.find(var0);
	if(var3 != s_nameToFactory.end() && var3->second != NULL) {
	var2 = (Entity*)var3->second((void*)var1);
	}
	} catch (Exception var4) {
	var4.printStackTrace();
	}
	return var2;
}

Entity* EntityList::createEntityFromNBT(NBTTagCompound* var0, World* var1)
{
	Entity* var2 = NULL;
	try {
	/* ---- ORIGINAL JAVA ----
	 *  Class var3 = (Class)stringToClassMapping.get(var0.getString("id"));
	 *  var2 = (Entity)var3.getConstructor(new Class[]{World.class}).newInstance(new Object[]{var1});
	 *  ---- end original Java ----
	 *  Reflection is reduced to a name->(World*) factory lookup. */
	JavaString var3 = var0->getString("id");
	std::map<std::string, EntityFactory>::iterator var5 = s_nameToFactory.find(var3);
	if(var5 != s_nameToFactory.end() && var5->second != NULL) {
	var2 = (Entity*)var5->second((void*)var1);
	}
	} catch (Exception var4) {
	var4.printStackTrace();
	}
	if(var2 != NULL) {
	var2->readFromNBT(var0);
	} else {
	System::out.println((JCat() + "Skipping Entity with id " + var0->getString("id")));
	}
	return var2;
}

Entity* EntityList::createEntity(int var0, World* var1)
{
	Entity* var2 = NULL;
	try {
	/* ---- ORIGINAL JAVA ----
	 *  Class var3 = (Class)IDtoClassMapping.get(Integer.valueOf(var0));
	 *  var2 = (Entity)var3.getConstructor(new Class[]{World.class}).newInstance(new Object[]{var1});
	 *  ---- end original Java ----
	 *  Reflection is reduced to an id->(World*) factory lookup. */
	std::map<int, EntityFactory>::iterator var3 = s_idToFactory.find(var0);
	if(var3 != s_idToFactory.end() && var3->second != NULL) {
	var2 = (Entity*)var3->second((void*)var1);
	}
	} catch (Exception var4) {
	var4.printStackTrace();
	}
	if(var2 == NULL) {
	System::out.println((JCat() + "Skipping Entity with id " + var0));
	}
	return var2;
}

int EntityList::getEntityID(Entity* var0)
{
	/* ---- ORIGINAL JAVA ----
	 *  return ((Integer)classToIDMapping.get(var0.getClass())).intValue();
	 *  ---- end original Java ----
	 *  var0.getClass() -> typeid(*var0).name() (the entities are polymorphic, so
	 *  this resolves to the DERIVED runtime type, exactly like Java getClass()). */
	if(var0 == NULL) return 0;
	std::map<std::string, int>::iterator var1 = s_typeToID.find(typeid(*var0).name());
	return var1 != s_typeToID.end() ? var1->second : 0;
}

JavaString EntityList::getEntityString(Entity* var0)
{
	/* ---- ORIGINAL JAVA ----
	 *  return (String)classToStringMapping.get(var0.getClass());
	 *  ---- end original Java ----
	 *  var0.getClass() -> typeid(*var0).name() (derived runtime type). */
	if(var0 == NULL) return JavaString("");
	std::map<std::string, std::string>::iterator var1 = s_typeToName.find(typeid(*var0).name());
	return var1 != s_typeToName.end() ? JavaString(var1->second) : JavaString("");
}
