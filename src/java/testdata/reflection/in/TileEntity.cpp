#include "TileEntity.h"

void TileEntity::init() {
	addMapping(j2xClassOf(typeid(TileEntityFurnace)), "Furnace");
	addMapping(j2xClassOf(typeid(TileEntityChest)), "Chest");
}

TileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0) {
	TileEntity* var1 = NULL;
	// J2X-REVIEW[reflection-unsupported]
	var1 = (TileEntity*)this->entityClass[var2]->newInstance();
	return var1;
}
