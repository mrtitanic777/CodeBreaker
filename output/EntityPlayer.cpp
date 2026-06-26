#include "stdafx.h"
// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayer.h"
#include "Common.h"
#include "Entity.h"
#include "IInventory.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityItem.h"
#include "EntityLiving.h"
#include "Player.h"
#include "EntityArrow.h"
#include "EntityMob.h"

EntityPlayer::EntityPlayer()
	: m_inventory(new InventoryPlayer())
	, m_unusedByte(0)
	, m_getScore(0)
	, m_prevCameraYaw(0)
	, m_cameraYaw(0)
	, cameraPitch(0)
{
}

EntityPlayer::EntityPlayer(World* var1)
	: EntityLiving(var1)
	, m_inventory(new InventoryPlayer())
	, m_unusedByte(0)
	, m_getScore(0)
	, m_prevCameraYaw(0)
	, m_cameraYaw(0)
	, cameraPitch(0)
{
		if(var1 != NULL) {
			var1->m_playerEntity = this;
			var1->releaseEntitySkin(this);
		}

		setPositionAndRotation((float)var1->m_xSpawn, (float)var1->m_ySpawn, (float)var1->m_zSpawn, 0.0F, 0.0F);
		yOffset = 1.62F;
		health = 20;
		fireResistance = 20;
		texture = "//* PORT: /* PORT: char. */  */ png";
}

EntityPlayer::~EntityPlayer() {
	m_inventory = NULL;
}

void EntityPlayer::preparePlayerToSpawn() {
		yOffset = 1.62F;
		setSize(0.6F, 1.8F);
		EntityLiving::preparePlayerToSpawn();
		if(worldObj != NULL) {
			worldObj->m_playerEntity = this;
		}

		health = 20;
		deathTime = 0;
}

void EntityPlayer::onLivingUpdate() {
		worldObj->playSoundEffect(posX, posY, posZ, "calm", 0.0F);
		if(worldObj->m_difficultySetting == 0 && health < 20 && ticksExisted % 20 << 2 == 0) {
			heal(1);
		}

		InventoryPlayer* var3 = m_inventory;

		for(int var4 = 0; var4 < var3->m_mainInventory_count; ++var4) {
			if(var3->m_mainInventory[var4] != NULL && var3->m_mainInventory[var4]->m_animationsToGo > 0) {
				--var3->m_mainInventory[var4]->m_animationsToGo;
			}
		}

		m_prevCameraYaw = m_cameraYaw;
		EntityLiving::onLivingUpdate();
		float var1 = sqrtf(motionX * motionX + motionZ * motionZ);
		float var2 = (float)atanf((double)(-motionY * 0.2F)) * 15.0F;
		if(var1 > 0.1F) {
			var1 = 0.1F;
		}

		if(!onGround || health <= 0) {
			var1 = 0.0F;
		}

		if(onGround || health <= 0) {
			var2 = 0.0F;
		}

		m_cameraYaw += (var1 - m_cameraYaw) * 0.4F;
		cameraPitch += (var2 - cameraPitch) * 0.8F;
		if(health > 0) {
			std::vector<Entity*> var5 = worldObj->a(this, boundingBox.expand(1.0F, 0.0F, 1.0F));
			if(true) {
				for(size_t var6 = 0; var6 < var5.size(); ++var6) {
					Entity* var7 = (Entity*)var5[var6];
					var7->onCollideWithPlayer(this);
				}
			}
		}

}

void EntityPlayer::onDeath(Entity* var1) {
		setSize(0.2F, 0.2F);
		setPosition(posX, posY, posZ);
		motionY = 0.1F;
		if(var1 != NULL) {
			motionX = -cosf((attackedAtYaw + rotationYaw) * 3.14159265358979323846f / 180.0F) * 0.1F;
			motionZ = -sinf((attackedAtYaw + rotationYaw) * 3.14159265358979323846f / 180.0F) * 0.1F;
		} else {
			motionX = motionZ = 0.0F;
		}

		yOffset = 0.1F;
}

void EntityPlayer::setEntityDead() {
	// TODO: implement
}

void EntityPlayer::dropPlayerItem(ItemStack* var1) {
		dropPlayerItemWithRandomChoice(var1, false);
}

void EntityPlayer::dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2) {
		if(var1 != NULL) {
			EntityItem* var4 = new EntityItem(worldObj, posX, posY - 0.3F, posZ, var1);
			var4->m_delayBeforeCanPickup = 40;
			var4->motionX = sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * 0.3F;
			var4->motionZ = -cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * 0.3F;
			var4->motionY = -sinf(rotationPitch / 180.0F * 3.14159265358979323846f) * 0.3F + 0.1F;
			float var3 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
			float var5 = 0.02F * rand->nextFloat();
			var4->motionX = (float)((double)var4->motionX + cosf((double)var3) * (double)var5);
			var4->motionY += (rand->nextFloat() - rand->nextFloat()) * 0.1F;
			var4->motionZ = (float)((double)var4->motionZ + sinf((double)var3) * (double)var5);
			worldObj->spawnEntityInWorld(var4);
		}
}

void EntityPlayer::writeEntityToNBT(NBTTagCompound* var1) {
	// TODO: implement
}

const std::string& EntityPlayer::getEntityString() {
		return NULL;
}

void EntityPlayer::displayGUIChest(IInventory* var1) {
	// TODO: implement
}

void EntityPlayer::displayWorkbenchGUI() {
	// TODO: implement
}

void EntityPlayer::onItemPickup(Entity* var1) {
	// TODO: implement
}

float EntityPlayer::getEyeHeight() {
		return 0.12F;
}

bool EntityPlayer::attackEntityFrom(Entity* var1, int var2) {
		if(dynamic_cast<EntityMob*>(var1) != NULL || dynamic_cast<EntityArrow*>(var1) != NULL) {
			if(worldObj->m_difficultySetting == 0) {
				var2 = 0;
			}

			if(worldObj->m_difficultySetting == 1) {
				var2 = var2 / 3 + 1;
			}

			if(worldObj->m_difficultySetting == 3) {
				var2 = var2 * 3 / 2;
			}
		}

		return EntityLiving::attackEntityFrom(var1, var2);
}
