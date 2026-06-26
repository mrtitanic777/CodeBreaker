#include "stdafx.h"
// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayer.h"
#include "Common.h"
#include "Block.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "IInventory.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "TileEntityFurnace.h"
#include "TileEntitySign.h"
#include "World.h"
#include "EntityArrow.h"
#include "EntityLiving.h"
#include "Item.h"
#include "Material.h"
#include "NBTTagList.h"
#include "Player.h"
#include "EntityMobs.h"

EntityPlayer::EntityPlayer()
	: m_inventory(new InventoryPlayer(this))
	, m_field_9371_f(0)
	, m_score(0)
	, m_field_775_e(0)
	, m_field_774_f(0)
	, m_field_9369_j(false)
	, m_field_9368_k(0)
	, m_field_771_i("")
	, m_dimension(0)
	, m_field_781_a(0)
	, m_fishEntity(NULL)
{
}

EntityPlayer::EntityPlayer(World* var1)
	: EntityLiving(var1)
	, m_inventory(new InventoryPlayer(this))
	, m_field_9371_f(0)
	, m_score(0)
	, m_field_775_e(0)
	, m_field_774_f(0)
	, m_field_9369_j(false)
	, m_field_9368_k(0)
	, m_field_771_i("")
	, m_dimension(0)
	, m_field_781_a(0)
	, m_fishEntity(NULL)
{
		yOffset = 1.62F;
		setLocationAndAngles((double)var1->m_spawnX + 0.5f, (double)(var1->m_spawnY + 1), (double)var1->m_spawnZ + 0.5f, 0.0F, 0.0F);
		health = 20;
		m_field_9351_C = "humanoid";
		m_field_9353_B = 180.0F;
		m_field_9310_bf = 20;
		texture = "/mob/char.png";
}

EntityPlayer::~EntityPlayer() {
	m_inventory = NULL;
	m_fishEntity = NULL;
}

void EntityPlayer::func_350_p() {
		EntityLiving::func_350_p();
		m_field_775_e = m_field_774_f;
		m_field_774_f = 0.0F;
}

void EntityPlayer::preparePlayerToSpawn() {
		yOffset = 1.62F;
		setSize(0.6F, 1.8F);
		EntityLiving::preparePlayerToSpawn();
		health = 20;
		deathTime = 0;
}

void EntityPlayer::func_418_b_() {
		if(m_field_9369_j) {
			++m_field_9368_k;
			if(m_field_9368_k == 8) {
				m_field_9368_k = 0;
				m_field_9369_j = false;
			}
		} else {
			m_field_9368_k = 0;
		}

		swingProgress = (float)m_field_9368_k / 8.0F;
}

void EntityPlayer::onLivingUpdate() {
		if(worldObj->m_difficultySetting == 0 && health < 20 && m_field_9311_be % 20 * 4 == 0) {
			heal(1);
		}

		m_inventory->decrementAnimations();
		m_field_775_e = m_field_774_f;
		EntityLiving::onLivingUpdate();
		float var1 = sqrt(motionX * motionX + motionZ * motionZ);
		float var2 = (float)atanf(-motionY * (double)0.2F) * 15.0F;
		if(var1 > 0.1F) {
			var1 = 0.1F;
		}

		if(!onGround || health <= 0) {
			var1 = 0.0F;
		}

		if(onGround || health <= 0) {
			var2 = 0.0F;
		}

		m_field_774_f += (var1 - m_field_774_f) * 0.4F;
		m_field_9328_R += (var2 - m_field_9328_R) * 0.8F;
		if(health > 0) {
			std::vector<void*> var3 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.expands(1.0f, 0.0f, 1.0f));
			if(true) {
				for(size_t var4 = 0; var4 < var3.size(); ++var4) {
					func_451_h((Entity*)var3[var4]);
				}
			}
		}

}

void EntityPlayer::func_451_h(Entity* var1) {
		var1->onCollideWithPlayer(this);
}

int EntityPlayer::func_6417_t() {
		return m_score;
}

void EntityPlayer::onDeath(Entity* var1) {
		EntityLiving::onDeath(var1);
		setSize(0.2F, 0.2F);
		setPosition(posX, posY, posZ);
		motionY = (double)0.1F;
		if(m_field_771_i == "Notch") {
			dropPlayerItemWithRandomChoice(new ItemStack(Item::s_appleRed, 1), true);
		}

		m_inventory->dropAllItems();
		if(var1 != NULL) {
			motionX = (double)(-cosf((m_field_9331_N + rotationYaw) * 3.14159265358979323846f / 180.0F) * 0.1F);
			motionZ = (double)(-sinf((m_field_9331_N + rotationYaw) * 3.14159265358979323846f / 180.0F) * 0.1F);
		} else {
			motionX = motionZ = 0.0f;
		}

		yOffset = 0.1F;
}

void EntityPlayer::addToPlayerScore(Entity* var1, int var2) {
		m_score += var2;
}

void EntityPlayer::dropPlayerItem(ItemStack* var1) {
		dropPlayerItemWithRandomChoice(var1, false);
}

void EntityPlayer::dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2) {
		if(var1 != NULL) {
			EntityItem* var3 = new EntityItem(worldObj, posX, posY - (double)0.3F + (double)func_373_s(), posZ, var1);
			var3->m_field_805_c = 40;
			float var4 = 0.1F;
			float var5;
			if(var2) {
				var5 = rand->nextFloat() * 0.5F;
				float var6 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
				var3->motionX = (double)(-sinf(var6) * var5);
				var3->motionZ = (double)(cosf(var6) * var5);
				var3->motionY = (double)0.2F;
			} else {
				var4 = 0.3F;
				var3->motionX = (double)(-sinf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var4);
				var3->motionZ = (double)(cosf(rotationYaw / 180.0F * 3.14159265358979323846f) * cosf(rotationPitch / 180.0F * 3.14159265358979323846f) * var4);
				var3->motionY = (double)(-sinf(rotationPitch / 180.0F * 3.14159265358979323846f) * var4 + 0.1F);
				var4 = 0.02F;
				var5 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
				var4 *= rand->nextFloat();
				var3->motionX += cosf((double)var5) * (double)var4;
				var3->motionY += (double)((rand->nextFloat() - rand->nextFloat()) * 0.1F);
				var3->motionZ += sinf((double)var5) * (double)var4;
			}

			joinEntityItemWithWorld(var3);
		}
}

void EntityPlayer::joinEntityItemWithWorld(EntityItem* var1) {
		worldObj->entityJoinedWorld(var1);
}

float EntityPlayer::getCurrentPlayerStrVsBlock(Block* var1) {
		float var2 = m_inventory->getStrVsBlock(var1);
		if(isInsideOfMaterial(Material::s_water)) {
			var2 /= 5.0F;
		}

		if(!onGround) {
			var2 /= 5.0F;
		}

		return var2;
}

bool EntityPlayer::canHarvestBlock(Block* var1) {
		return m_inventory->canHarvestBlock(var1);
}

void EntityPlayer::readEntityFromNBT(NBTTagCompound* var1) {
		EntityLiving::readEntityFromNBT(var1);
		NBTTagList* var2 = var1->getTagList("Inventory");
		m_inventory->readFromNBT(var2);
		m_dimension = var1->getInteger("Dimension");
}

void EntityPlayer::writeEntityToNBT(NBTTagCompound* var1) {
		EntityLiving::writeEntityToNBT(var1);
		var1->setTag("Inventory", m_inventory->writeToNBT(new NBTTagList()));
		var1->setInteger("Dimension", m_dimension);
}

void EntityPlayer::displayGUIChest(IInventory* var1) {
	// TODO: implement
}

void EntityPlayer::displayWorkbenchGUI() {
	// TODO: implement
}

void EntityPlayer::func_443_a_(Entity* var1, int var2) {
	// TODO: implement
}

float EntityPlayer::func_373_s() {
		return 0.12F;
}

bool EntityPlayer::canAttackEntity(Entity* var1, int var2) {
		m_field_9344_ag = 0;
		if(health <= 0) {
			return false;
		} else {
			if(dynamic_cast<EntityMobs*>(var1) != NULL || dynamic_cast<EntityArrow*>(var1) != NULL) {
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

			return var2 == 0 ? false : EntityLiving::canAttackEntity(var1, var2);
		}
}

void EntityPlayer::damageEntity(int var1) {
		int var2 = 25 - m_inventory->getTotalArmorValue();
		int var3 = var1 * var2 + m_field_781_a;
		m_inventory->damageArmor(var1);
		var1 = var3 / 25;
		m_field_781_a = var3 % 25;
		EntityLiving::damageEntity(var1);
}

void EntityPlayer::displayGUIFurnace(TileEntityFurnace* var1) {
	// TODO: implement
}

void EntityPlayer::displayGUIEditSign(TileEntitySign* var1) {
	// TODO: implement
}

void EntityPlayer::func_6415_a_(Entity* var1) {
		var1->interact(this);
}

ItemStack* EntityPlayer::getCurrentEquippedItem() {
		return m_inventory->getCurrentItem();
}

void EntityPlayer::destroyCurrentEquippedItem() {
		m_inventory->setInventorySlotContents(m_inventory->m_currentItem, NULL);
}

double EntityPlayer::func_388_v() {
		return (double)(yOffset - 0.5F);
}

void EntityPlayer::func_457_w() {
		m_field_9368_k = -1;
		m_field_9369_j = true;
}

void EntityPlayer::attackTargetEntityWithCurrentItem(Entity* var1) {
		int var2 = m_inventory->getDamageVsEntity(var1);
		if(var2 > 0) {
			var1->canAttackEntity(this, var2);
			ItemStack* var3 = getCurrentEquippedItem();
			if(var3 != NULL && dynamic_cast<EntityLiving*>(var1) != NULL) {
				var3->hitEntity((EntityLiving*)var1);
				if(var3->stackSize <= 0) {
					var3->func_1097_a(this);
					destroyCurrentEquippedItem();
				}
			}
		}

}

void EntityPlayer::func_9367_r() {
	// TODO: implement
}
