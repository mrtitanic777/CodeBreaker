#include "stdafx.h"
// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityItem.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "Entity.h"
#include "Item.h"
#include "Material.h"

EntityItem::EntityItem()
	: m_item(NULL)
	, m_unknownEntityItemInt(0)
	, m_age(0)
	, m_delayBeforeCanPickup(0)
	, health(5)
	, m_hoverStart((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
}

EntityItem::EntityItem(World* var1, float var2, float var3, float var4, ItemStack* var5)
	: Entity(var1)
	, m_item(NULL)
	, m_unknownEntityItemInt(0)
	, m_age(0)
	, m_delayBeforeCanPickup(0)
	, health(5)
	, m_hoverStart((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
		setSize(0.25F, 0.25F);
		yOffset = height / 2.0F;
		setPosition(var2, var3, var4);
		m_item = var5;
		rotationYaw = (float)(((float)::rand() / (float)RAND_MAX) * 360.0f);
		motionX = (float)(((float)::rand() / (float)RAND_MAX) * (double)0.2F - (double)0.1F);
		motionY = 0.2F;
		motionZ = (float)(((float)::rand() / (float)RAND_MAX) * (double)0.2F - (double)0.1F);
		canTriggerWalking = false;
}

EntityItem::EntityItem(World* var1)
	: Entity(var1)
	, m_item(NULL)
	, m_unknownEntityItemInt(0)
	, m_age(0)
	, m_delayBeforeCanPickup(0)
	, health(5)
	, m_hoverStart((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
}

EntityItem::~EntityItem() {
	m_item = NULL;
}

void EntityItem::onEntityUpdate() {
		Entity::onEntityUpdate();
		if(m_delayBeforeCanPickup > 0) {
			--m_delayBeforeCanPickup;
		}

		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= 0.04F;
		if(worldObj->getBlockMaterial((int)posX, (int)posY, (int)posZ) == Material::s_lava) {
			motionY = 0.2F;
			motionX = (rand->nextFloat() - rand->nextFloat()) * 0.2F;
			motionZ = (rand->nextFloat() - rand->nextFloat()) * 0.2F;
			worldObj->a(*this, "random->fizz", 0.4F, 2.0F + rand->nextFloat() * 0.4F);
		}

		float var4 = posZ;
		float var3 = posY;
		float var2 = posX;
		int var5 = (int)var2;
		int var6 = (int)var3;
		int var7 = (int)var4;
		var2 -= (float)var5;
		var3 -= (float)var6;
		var4 -= (float)var7;
		if(Block::s_opaqueCubeLookup[worldObj->getBlockId(var5, var6, var7)]) {
			bool var8 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5 - 1, var6, var7)];
			bool var9 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5 + 1, var6, var7)];
			bool var10 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5, var6 - 1, var7)];
			bool var11 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5, var6 + 1, var7)];
			bool var12 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5, var6, var7 - 1)];
			bool var13 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var5, var6, var7 + 1)];
			unsigned char var14 = -1;
			float var15 = 9999.0F;
			if(var8 && var2 < 9999.0F) {
				var15 = var2;
				var14 = 0;
			}

			if(var9 && 1.0F - var2 < var15) {
				var15 = 1.0F - var2;
				var14 = 1;
			}

			if(var10 && var3 < var15) {
				var15 = var3;
				var14 = 2;
			}

			if(var11 && 1.0F - var3 < var15) {
				var15 = 1.0F - var3;
				var14 = 3;
			}

			if(var12 && var4 < var15) {
				var15 = var4;
				var14 = 4;
			}

			if(var13 && 1.0F - var4 < var15) {
				var14 = 5;
			}

			var2 = rand->nextFloat() * 0.2F + 0.1F;
			if(var14 == 0) {
				motionX = -var2;
			}

			if(var14 == 1) {
				motionX = var2;
			}

			if(var14 == 2) {
				motionY = -var2;
			}

			if(var14 == 3) {
				motionY = var2;
			}

			if(var14 == 4) {
				motionZ = -var2;
			}

			if(var14 == 5) {
				motionZ = var2;
			}
		}

		bool var10000 = false;
		moveEntity(motionX, motionY, motionZ);
		motionX *= 0.98F;
		motionY *= 0.98F;
		motionZ *= 0.98F;
		if(onGround) {
			motionX *= 0.7F;
			motionZ *= 0.7F;
			motionY *= -0.5F;
		}

		++m_unknownEntityItemInt;
		++m_age;
		if(m_age >= 6000) {
			setEntityDead();
		}

}

void EntityItem::dealFireDamage(int var1) {
		float var5 = posZ;
		float var4 = posY;
		float var3 = posX;
		World var2 = worldObj;
		ItemStack var6 = m_item;
		if(var6.getItem().onPlaced(var2, var3, var4, var5)) {
			--var6.stackSize;
		}

		if(m_item->stackSize == 0) {
			setEntityDead();
		}

}

bool EntityItem::attackEntityFrom(Entity* var1, int var2) {
		return false;
}

void EntityItem::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("Health", (unsigned char)health);
		var1->setShort("Age", (short)m_age);
		var1->setCompoundTag("Item", m_item->writeToNBT(new NBTTagCompound()));
}

void EntityItem::readEntityFromNBT(NBTTagCompound* var1) {
		health = var1->getShort("Health") & 255;
		m_age = var1->getShort("Age");
		var1 = var1->getCompoundTag("Item");
		m_item = new ItemStack(var1);
}

const std::string& EntityItem::getEntityString() {
		return "Item";
}

void EntityItem::onCollideWithPlayer(EntityPlayer* var1) {
		if(m_delayBeforeCanPickup == 0 && var1->m_inventory.storePartialItemStack(m_item)) {
			worldObj->a(*this, "random->pop", 0.2F, ((rand->nextFloat() - rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
			var1->onItemPickup(this);
			setEntityDead();
		}

}
