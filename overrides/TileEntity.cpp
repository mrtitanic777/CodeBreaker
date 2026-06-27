#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntity.h"
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
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IllegalArgumentException.h"
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
#include "RuntimeException.h"
#include "StepSound.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include <typeinfo>

// Headers of every TileEntity subclass registered in the Java static block (see ORIGINAL JAVA).
#include "TileEntityFurnace.h"
#include "TileEntityChest.h"
#include "TileEntitySign.h"
#include "TileEntityMobSpawner.h"

// PORT: Java created tile entities by reflection (Class.newInstance()). The runtime has no
// reflection, so each registered subclass gets a no-arg template factory. The cast to TileEntity*
// happens INSIDE the template (where T is complete) so multiple-inheritance subclasses such as
// TileEntityFurnace/TileEntityChest (TileEntity + IInventory) yield a correctly base-adjusted pointer.
template<class T> static void* j2x_new0() { return (void*)(TileEntity*)(new T()); }

std::map<std::string, void*> TileEntity::s_nameToClassMap;
std::map<std::string, JavaString> TileEntity::s_classToNameMap;
std::map<std::string, TileEntity::TileEntityFactory> TileEntity::s_factoryMap;

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java keyed nameToClassMap/classToNameMap by the real `Class` object. There is no Class
 *  reflection in the runtime model, so identity is faked by a class-name string. The Java map
 *  ops this stands in for:
 *      nameToClassMap.put(var1, var0);
 *      classToNameMap.put(var0, var1);
 *  ---- end original Java ---- */
// Class-identity key string (see header note).
static std::string _tileClassKey(Class* c) { return c ? c->getName() : std::string(); }

// PORT: typeid-based stand-in for Java's reflective addMapping(Class, String). Registers all three
// maps at once: name->factory (for create), typeid(class)->name (for writeToNBT/getClass lookup),
// and name->class identity string (so s_nameToClassMap is non-empty, mirroring the Java map).
void TileEntity::registerMapping(const std::string& classKey,
                                 const JavaString& name,
                                 TileEntity::TileEntityFactory factory)
{
	if(s_classToNameMap.count(classKey)) {
		throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate id: " + name)));
	}
	s_factoryMap[name] = factory;
	// Non-null marker so s_nameToClassMap mirrors Java's nameToClassMap (a name is "registered").
	// Real construction uses s_factoryMap; this value is never dereferenced.
	s_nameToClassMap[name] = (void*)factory;
	s_classToNameMap[classKey] = name;
}

TileEntity::~TileEntity()
{
}

/* ---- ORIGINAL JAVA (reproduced below) ----
 *  Java registered all TileEntity subclasses in a static initializer that runs at class load:
 *      static {
 *          addMapping(TileEntityFurnace.class, "Furnace");
 *          addMapping(TileEntityChest.class, "Chest");
 *          addMapping(TileEntitySign.class, "Sign");
 *          addMapping(TileEntityMobSpawner.class, "MobSpawner");
 *      }
 *  PORT: .class literals / Class.newInstance() do not exist in the runtime model, so each mapping
 *  is reproduced as typeid(Subclass).name() (class identity) + a template factory &j2x_new0<Sub>
 *  (construction). The static block is reproduced by the file-local _Init struct below, which runs
 *  before main() and populates s_factoryMap / s_classToNameMap / s_nameToClassMap.
 *  ---- end original Java ---- */
void TileEntity::addMapping(Class* var0, const JavaString& var1)
{
	if(s_classToNameMap.count(var1)) {
	throw new IllegalArgumentException(JavaString(std::string(JCat() + "Duplicate id: " + var1)));
	} else {
	s_nameToClassMap[var1] = var0;
	s_classToNameMap[_tileClassKey(var0)] = var1;
	}
}

namespace {
	// Reproduces the Java `static { addMapping(...) }` block: runs at load, registers each
	// TileEntity subclass by RTTI identity + no-arg template factory.
	struct _Init {
		_Init() {
			TileEntity::registerMapping(typeid(TileEntityFurnace).name(),    JavaString("Furnace"),    &j2x_new0<TileEntityFurnace>);
			TileEntity::registerMapping(typeid(TileEntityChest).name(),      JavaString("Chest"),      &j2x_new0<TileEntityChest>);
			TileEntity::registerMapping(typeid(TileEntitySign).name(),       JavaString("Sign"),       &j2x_new0<TileEntitySign>);
			TileEntity::registerMapping(typeid(TileEntityMobSpawner).name(), JavaString("MobSpawner"), &j2x_new0<TileEntityMobSpawner>);
		}
	} _init_inst;
}

void TileEntity::readFromNBT(NBTTagCompound* var1)
{
	this->xCoord = var1->getInteger("x");
	this->yCoord = var1->getInteger("y");
	this->zCoord = var1->getInteger("z");
}

void TileEntity::writeToNBT(NBTTagCompound* var1)
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  String var2 = (String)classToNameMap.get(this.getClass());
	 *  if(var2 == null) {
	 *      throw new RuntimeException(this.getClass() + " is missing a mapping! This is a bug!");
	 *  } else {
	 *      var1.setString("id", var2);
	 *      var1.setInteger("x", this.xCoord);
	 *      var1.setInteger("y", this.yCoord);
	 *      var1.setInteger("z", this.zCoord);
	 *  }
	 *  NOTE: Java looks up by this.getClass(); the port substitutes typeid(*this).name(),
	 *  an approximation of class identity (no reflective Class map in the runtime).
	 *  ---- end original Java ---- */
	JavaString var2 = s_classToNameMap[std::string(typeid(*this).name())];
	if(var2 == NULL) {
	throw new RuntimeException(JavaString(std::string(JCat() + typeid(*this).name() + " is missing a mapping! This is a bug!")));
	} else {
	var1->setString("id", var2);
	var1->setInteger("x", this->xCoord);
	var1->setInteger("y", this->yCoord);
	var1->setInteger("z", this->zCoord);
	}
}

void TileEntity::updateEntity()
{
}

TileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0)
{
	TileEntity* var1 = NULL;
	try {
	/* ---- ORIGINAL JAVA (reproduced below) ----
	 *  Class var2 = (Class)nameToClassMap.get(var0.getString("id"));
	 *  if(var2 != null) {
	 *      var1 = (TileEntity)var2.newInstance();
	 *  }
	 *  PORT: Java instantiates the mapped subclass via reflection (Class.newInstance()). There is
	 *  no reflection in the runtime model, so the NBT "id" string is looked up in s_factoryMap and
	 *  the matching no-arg template factory is invoked. The factory already returns a base-adjusted
	 *  TileEntity* (cast done inside the template), so this is a plain pointer cast.
	 *  ---- end original Java ---- */
	std::map<std::string, TileEntityFactory>::iterator var2 = s_factoryMap.find(var0->getString("id"));
	if(var2 != s_factoryMap.end() && var2->second != NULL) {
	var1 = (TileEntity*)(var2->second());
	}
	} catch (Exception var3) {
	var3.printStackTrace();
	}
	if(var1 != NULL) {
	var1->readFromNBT(var0);
	} else {
	System::out.println((JCat() + "Skipping TileEntity with id " + var0->getString("id")));
	}
	return var1;
}

int TileEntity::getBlockMetadata()
{
	return this->worldObj->getBlockMetadata(this->xCoord, this->yCoord, this->zCoord);
}

void TileEntity::onInventoryChanged()
{
	this->worldObj->markTileEntityForUpdate(this->xCoord, this->yCoord, this->zCoord, this);
}

double TileEntity::getDistanceFrom(double var1, double var3, double var5)
{
	double var7 = (double)this->xCoord + 0.5 - var1;
	double var9 = (double)this->yCoord + 0.5 - var3;
	double var11 = (double)this->zCoord + 0.5 - var5;
	return var7 * var7 + var9 * var9 + var11 * var11;
}

Block* TileEntity::getBlockType()
{
	return Block::s_blocksList[this->worldObj->getBlockId(this->xCoord, this->yCoord, this->zCoord)];
}
