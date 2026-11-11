#include "TileEntity.h"

// ==== CodeBreaker-generated reflection factory (replaces newInstance/getConstructor) ====
// Built from the class registration table; returns a real `new` instead of the NULL stub.
TileEntity* TileEntity_createByReflection(Class* c)
{
	if (!c) return NULL;
	if (*c == Class(typeid(TileEntityFurnace))) return new TileEntityFurnace();
	if (*c == Class(typeid(TileEntityChest))) return new TileEntityChest();
	return NULL;
}


void TileEntity::init() {
	addMapping(j2xClassOf(typeid(TileEntityFurnace)), "Furnace");
	addMapping(j2xClassOf(typeid(TileEntityChest)), "Chest");
}

TileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0) {
	TileEntity* var1 = NULL;
	var1 = TileEntity_createByReflection(this->entityClass[var2]);
	return var1;
}
