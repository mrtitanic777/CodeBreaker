#include "stdafx.h"
// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPig.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityAnimal.h"

EntityPig::EntityPig()
{
}

EntityPig::EntityPig(World* var1)
	: EntityAnimal(var1)
{
		texture = "/mob/pig.png";
		setSize(0.9F, 0.9F);
}

EntityPig::~EntityPig() {
	// No dynamic resources
}

void EntityPig::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimal::writeEntityToNBT(var1);
}

void EntityPig::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimal::readEntityFromNBT(var1);
}

const std::string& EntityPig::getEntityString() {
		return "Pig";
}

const std::string& EntityPig::getLivingSound() {
		return "mob.pig";
}

const std::string& EntityPig::getHurtSound() {
		return "mob.pig";
}

const std::string& EntityPig::getDeathSound() {
		return "mob.pigdeath";
}
