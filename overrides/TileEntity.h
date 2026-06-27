#ifndef TILEENTITY_H
#define TILEENTITY_H

// Auto-generated from TileEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class Block;
class Class;
class NBTTagCompound;
class World;

class TileEntity {
public:
	TileEntity() : worldObj(NULL), xCoord(0), yCoord(0), zCoord(0) {}
	virtual ~TileEntity();

	World* worldObj;
	int xCoord;
	int yCoord;
	int zCoord;
	// Java: Map<String,Class> name->class and Map<Class,String> class->name.
	// Class identity is keyed by its runtime type name (typeid) string.
	static std::map<std::string, void*> s_nameToClassMap;
	static std::map<std::string, JavaString> s_classToNameMap;

	static void initStatics();   // run the TileEntity registrations in order (called by j2x_initAll)
	static void addMapping(Class* var0, const JavaString& var1);

	// PORT: Reflection (Class.newInstance) does not exist in the runtime model. Construction is
	// done via per-class template factories keyed by the registered name (the NBT "id" string).
	// Each factory returns a base-adjusted TileEntity* (as void*) from `new <Subclass>()`.
	typedef void* (*TileEntityFactory)();
	// PORT: typeid + factory stand-in for Java's reflective addMapping(Class, String).
	static void registerMapping(const std::string& classKey, const JavaString& name, TileEntityFactory factory);
private:
	static std::map<std::string, TileEntityFactory> s_factoryMap;
public:
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void updateEntity();
	static TileEntity* createAndLoadEntity(NBTTagCompound* var0);
	virtual int getBlockMetadata();
	virtual void onInventoryChanged();
	virtual double getDistanceFrom(double var1, double var3, double var5);
	virtual Block* getBlockType();
};

#endif // TILEENTITY_H