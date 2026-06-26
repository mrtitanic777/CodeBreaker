#include "stdafx.h"
// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityItem.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Block.h"
#include "Entity.h"
#include "Item.h"
#include "Material.h"
#include "InventoryPlayer.h"

EntityItem::EntityItem()
	: m_item(NULL)
	, m_field_803_e(0)
	, m_age(0)
	, m_field_805_c(0)
	, health(5)
	, m_field_804_d((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
}

EntityItem::EntityItem(World* var1, double var2, double var4, double var6, ItemStack* var8)
	: Entity(var1)
	, m_item(NULL)
	, m_field_803_e(0)
	, m_age(0)
	, m_field_805_c(0)
	, health(5)
	, m_field_804_d((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
		setSize(0.25F, 0.25F);
		yOffset = height / 2.0F;
		setPosition(var2, var4, var6);
		m_item = var8;
		rotationYaw = (float)(((float)::rand() / (float)RAND_MAX) * 360.0f);
		motionX = (double)((float)(((float)::rand() / (float)RAND_MAX) * (double)0.2F - (double)0.1F));
		motionY = (double)0.2F;
		motionZ = (double)((float)(((float)::rand() / (float)RAND_MAX) * (double)0.2F - (double)0.1F));
		m_entityWalks = false;
}

EntityItem::EntityItem(World* var1)
	: Entity(var1)
	, m_item(NULL)
	, m_field_803_e(0)
	, m_age(0)
	, m_field_805_c(0)
	, health(5)
	, m_field_804_d((float)(((float)::rand() / (float)RAND_MAX) * 3.14159265358979323846f * 2.0f))
{
		setSize(0.25F, 0.25F);
		yOffset = height / 2.0F;
}

EntityItem::~EntityItem() {
	m_item = NULL;
}

void EntityItem::onUpdate() {
		Entity::onUpdate();
		if(m_field_805_c > 0) {
			--m_field_805_c;
		}

		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= (double)0.04F;
		if(worldObj->getBlockMaterial((int)floor(posX), (int)floor(posY), (int)floor(posZ)) == Material::s_lava) {
			motionY = (double)0.2F;
			motionX = (double)((rand->nextFloat() - rand->nextFloat()) * 0.2F);
			motionZ = (double)((rand->nextFloat() - rand->nextFloat()) * 0.2F);
			worldObj->playSoundAtEntity(this, "random.fizz", 0.4F, 2.0F + rand->nextFloat() * 0.4F);
		}

		func_466_g(posX, posY, posZ);
		handleWaterMovement();
		moveEntity(motionX, motionY, motionZ);
		float var1 = 0.98F;
		if(onGround) {
			var1 = 0.1F * 0.1F * 58.8F;
			int var2 = worldObj->getBlockId((int)floor(posX), (int)floor(boundingBox.minY) - 1, (int)floor(posZ));
			if(var2 > 0) {
				var1 = Block::blocksList[var2]->slipperiness * 0.98F;
			}
		}

		motionX *= (double)var1;
		motionY *= (double)0.98F;
		motionZ *= (double)var1;
		if(onGround) {
			motionY *= -0.5f;
		}

		++m_field_803_e;
		++m_age;
		if(m_age >= 6000) {
			setEntityDead();
		}

}

bool EntityItem::handleWaterMovement() {
		return worldObj->func_682_a(boundingBox, Material::s_water, this);
}

bool EntityItem::func_466_g(double var1, double var3, double var5) {
		int var7 = (int)floor(var1);
		int var8 = (int)floor(var3);
		int var9 = (int)floor(var5);
		double var10 = var1 - (double)var7;
		double var12 = var3 - (double)var8;
		double var14 = var5 - (double)var9;
		if(Block::s_opaqueCubeLookup[worldObj->getBlockId(var7, var8, var9)]) {
			bool var16 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7 - 1, var8, var9)];
			bool var17 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7 + 1, var8, var9)];
			bool var18 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7, var8 - 1, var9)];
			bool var19 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7, var8 + 1, var9)];
			bool var20 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7, var8, var9 - 1)];
			bool var21 = !Block::s_opaqueCubeLookup[worldObj->getBlockId(var7, var8, var9 + 1)];
			unsigned char var22 = -1;
			double var23 = 9999.0f;
			if(var16 && var10 < var23) {
				var23 = var10;
				var22 = 0;
			}

			if(var17 && 1.0f - var10 < var23) {
				var23 = 1.0f - var10;
				var22 = 1;
			}

			if(var18 && var12 < var23) {
				var23 = var12;
				var22 = 2;
			}

			if(var19 && 1.0f - var12 < var23) {
				var23 = 1.0f - var12;
				var22 = 3;
			}

			if(var20 && var14 < var23) {
				var23 = var14;
				var22 = 4;
			}

			if(var21 && 1.0f - var14 < var23) {
				var23 = 1.0f - var14;
				var22 = 5;
			}

			float var25 = rand->nextFloat() * 0.2F + 0.1F;
			if(var22 == 0) {
				motionX = (double)(-var25);
			}

			if(var22 == 1) {
				motionX = (double)var25;
			}

			if(var22 == 2) {
				motionY = (double)(-var25);
			}

			if(var22 == 3) {
				motionY = (double)var25;
			}

			if(var22 == 4) {
				motionZ = (double)(-var25);
			}

			if(var22 == 5) {
				motionZ = (double)var25;
			}
		}

		return false;
}

void EntityItem::func_355_a(int var1) {
		canAttackEntity(NULL, var1);
}

bool EntityItem::canAttackEntity(Entity* var1, int var2) {
		func_9281_M();
		health -= var2;
		if(health <= 0) {
			setEntityDead();
		}

		return false;
}

void EntityItem::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("Health", (short)((unsigned char)health));
		var1->setShort("Age", (short)m_age);
		var1->setCompoundTag("Item", m_item->writeToNBT(new NBTTagCompound()));
}

void EntityItem::readEntityFromNBT(NBTTagCompound* var1) {
		health = var1->getShort("Health") & 255;
		m_age = var1->getShort("Age");
		NBTTagCompound* var2 = var1->getCompoundTag("Item");
		m_item = new ItemStack(var2);
}

void EntityItem::onCollideWithPlayer(EntityPlayer* var1) {
		if(!worldObj->m_multiplayerWorld) {
			int var2 = m_item->stackSize;
			if(m_field_805_c == 0 && var1->m_inventory->addItemStackToInventory(m_item)) {
				worldObj->playSoundAtEntity(this, "random.pop", 0.2F, ((rand->nextFloat() - rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
				var1->func_443_a_(this, var2);
				setEntityDead();
			}

		}
}
