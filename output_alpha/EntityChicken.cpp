#include "stdafx.h"
// Auto-generated from EntityChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityChicken.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Item.h"
#include "EntityAnimals.h"

EntityChicken::EntityChicken()
	: m_field_753_a(false)
	, m_field_752_b(0.0f)
	, m_field_758_c(0.0f)
	, m_field_757_d(0)
	, m_field_756_e(0)
	, m_field_755_h(1.0f)
	, m_timeUntilNextEgg(0)
{
}

EntityChicken::EntityChicken(World* var1)
	: EntityAnimals(var1)
	, m_field_753_a(false)
	, m_field_752_b(0.0f)
	, m_field_758_c(0.0f)
	, m_field_757_d(0)
	, m_field_756_e(0)
	, m_field_755_h(1.0f)
	, m_timeUntilNextEgg(0)
{
		texture = "/mob/chicken.png";
		setSize(0.3F, 0.4F);
		health = 4;
		m_timeUntilNextEgg = rand->nextInt(6000) + 6000;
}

EntityChicken::~EntityChicken() {
	// No dynamic resources
}

void EntityChicken::onLivingUpdate() {
		EntityAnimals::onLivingUpdate();
		m_field_756_e = m_field_752_b;
		m_field_757_d = m_field_758_c;
		m_field_758_c = (float)((double)m_field_758_c + (double)(onGround ? -1 : 4) * 0.3f);
		if(m_field_758_c < 0.0F) {
			m_field_758_c = 0.0F;
		}

		if(m_field_758_c > 1.0F) {
			m_field_758_c = 1.0F;
		}

		if(!onGround && m_field_755_h < 1.0F) {
			m_field_755_h = 1.0F;
		}

		m_field_755_h = (float)((double)m_field_755_h * 0.9f);
		if(!onGround && motionY < 0.0f) {
			motionY *= 0.6f;
		}

		m_field_752_b += m_field_755_h * 2.0F;
		if(!worldObj->m_multiplayerWorld && --m_timeUntilNextEgg <= 0) {
			worldObj->playSoundAtEntity(this, "mob.chickenplop", 1.0F, (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
			dropItem(Item::s_egg->shiftedIndex, 1);
			m_timeUntilNextEgg = rand->nextInt(6000) + 6000;
		}

}

void EntityChicken::fall(float var1) {
	// TODO: implement
}

void EntityChicken::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimals::writeEntityToNBT(var1);
}

void EntityChicken::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimals::readEntityFromNBT(var1);
}

const std::string& EntityChicken::getLivingSound() {
		static const std::string _ret = "mob.chicken"; return _ret;
}

const std::string& EntityChicken::getHurtSound() {
		static const std::string _ret = "mob.chickenhurt"; return _ret;
}

const std::string& EntityChicken::getDeathSound() {
		static const std::string _ret = "mob.chickenhurt"; return _ret;
}

int EntityChicken::getDropItemId() {
		return Item::s_feather->shiftedIndex;
}
