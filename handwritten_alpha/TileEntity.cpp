#include "stdafx.h"
// Auto-generated from TileEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntity.h"
#include "Common.h"
#include "Block.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Entity.h"

// PORT: subclass headers for the createAndLoadEntity dispatch (replaces Java reflection)
#include "TileEntityFurnace.h"
#include "TileEntityChest.h"
#include "TileEntitySign.h"
#include "TileEntityMobSpawner.h"

TileEntity::TileEntity()
	: worldObj(NULL)
	, xCoord(0)
	, yCoord(0)
	, zCoord(0)
{
}

TileEntity::~TileEntity() {
	worldObj = NULL;
}

const std::string& TileEntity::getEntityName() {
	// PORT: default = not persisted. Subclasses override to return their saved id.
	static const std::string empty = "";
	return empty;
}

void TileEntity::addMapping(const std::string& var1) {
	// PORT: Java kept Class<->name maps for reflection. Construction is now an
	// explicit dispatch in createAndLoadEntity, so this is a no-op registry hook.
}

void TileEntity::readFromNBT(NBTTagCompound* var1) {
		xCoord = var1->getInteger("x");
		yCoord = var1->getInteger("y");
		zCoord = var1->getInteger("z");
}

void TileEntity::writeToNBT(NBTTagCompound* var1) {
		const std::string& var2 = getEntityName();
		if(var2.empty()) {
			DbgPrintF("ERROR [RuntimeException]: %s\n", "TileEntity is missing an id mapping! This is a bug!");
		} else {
			var1->setString("id", var2);
			var1->setInteger("x", xCoord);
			var1->setInteger("y", yCoord);
			var1->setInteger("z", zCoord);
		}
}

void TileEntity::updateEntity() {
	// TODO: implement
}

TileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0) {
		TileEntity* var1 = NULL;

		// PORT: Java did nameToClassMap.get(id).newInstance() via reflection.
		// C++03/Xbox360 has no reflection, so dispatch the id string explicitly.
		std::string var2 = var0->getString("id");
		if(var2 == "Furnace") {
			var1 = new TileEntityFurnace();
		} else if(var2 == "Chest") {
			var1 = new TileEntityChest();
		} else if(var2 == "Sign") {
			var1 = new TileEntitySign();
		} else if(var2 == "MobSpawner") {
			var1 = new TileEntityMobSpawner();
		}

		if(var1 != NULL) {
			var1->readFromNBT(var0);
		} else {
			DbgPrintF("%s\n", ("Skipping TileEntity with id " + var0->getString("id")).c_str());
		}

		return var1;
}

int TileEntity::getBlockMetadata() {
		return worldObj->getBlockMetadata(xCoord, yCoord, zCoord);
}

void TileEntity::onInventoryChanged() {
		worldObj->func_698_b(xCoord, yCoord, zCoord, this);
}

double TileEntity::getDistanceFrom(double var1, double var3, double var5) {
		double var7 = (double)xCoord + 0.5f - var1;
		double var9 = (double)yCoord + 0.5f - var3;
		double var11 = (double)zCoord + 0.5f - var5;
		return var7 * var7 + var9 * var9 + var11 * var11;
}

Block* TileEntity::getBlockType() {
		return Block::blocksList[worldObj->getBlockId(xCoord, yCoord, zCoord)];
}

void TileEntity::initStatics() {
		// PORT: Java registered Class<->name mappings here for reflection.
		// Construction is now an explicit dispatch in createAndLoadEntity, so
		// there is nothing to initialise.
}
