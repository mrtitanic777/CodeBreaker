#include "stdafx.h"
// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "Entity.h"
#include "Item.h"
#include "StepSound.h"

EntityLiving::EntityLiving()
	: heartsHalvesLife(20)
	, renderYawOffset(0.0f)
	, prevRenderYawOffset(0.0f)
	, m_rotationYawHead(0)
	, m_prevRotationYawHead(0)
	, texture("//* PORT: char. */ png")
	, scoreValue(0)
	, health(0)
	, prevHealth(0)
	, m_livingSoundTime(0)
	, hurtTime(0)
	, m_maxHurtTime(0)
	, attackedAtYaw(0.0f)
	, deathTime(0)
	, attackTime(0)
	, m_prevCameraPitch(0)
	, m_cameraPitch(0)
	, m_ae(0)
	, m_limbYaw(0)
	, m_limbSwing(0)
	, entityAge(0)
	, moveStrafing(0)
	, moveForward(0)
	, randomYawVelocity(0)
	, isJumping(false)
	, defaultPitch(0)
	, moveSpeed(0)
{
}

EntityLiving::EntityLiving(World* var1)
	: Entity(var1)
	, heartsHalvesLife(20)
	, renderYawOffset(0.0f)
	, prevRenderYawOffset(0.0f)
	, m_rotationYawHead(0)
	, m_prevRotationYawHead(0)
	, texture("//* PORT: char. */ png")
	, scoreValue(0)
	, health(0)
	, prevHealth(0)
	, m_livingSoundTime(0)
	, hurtTime(0)
	, m_maxHurtTime(0)
	, attackedAtYaw(0.0f)
	, deathTime(0)
	, attackTime(0)
	, m_prevCameraPitch(0)
	, m_cameraPitch(0)
	, m_ae(0)
	, m_limbYaw(0)
	, m_limbSwing(0)
	, entityAge(0)
	, moveStrafing(0)
	, moveForward(0)
	, randomYawVelocity(0)
	, isJumping(false)
	, defaultPitch(0)
	, moveSpeed(0)
{
		((float)::rand() / (float)RAND_MAX);
		entityAge = 0;
		isJumping = false;
		defaultPitch = 0.0F;
		moveSpeed = 0.7F;
		health = 10;
		preventEntitySpawning = true;
		((float)::rand() / (float)RAND_MAX);
		setPosition(posX, posY, posZ);
		((float)::rand() / (float)RAND_MAX);
		rotationYaw = (float)(((float)::rand() / (float)RAND_MAX) * (double)(3.14159265358979323846f) * 2.0f);
		stepHeight = 0.5F;
}

EntityLiving::~EntityLiving() {
	// No dynamic resources
}

const std::string& EntityLiving::getTexture() {
		return texture;
}

bool EntityLiving::canBeCollidedWith() {
		return !isDead;
}

bool EntityLiving::canBePushed() {
		return !isDead;
}

float EntityLiving::getEyeHeight() {
		return height * 0.85F;
}

void EntityLiving::onEntityUpdate() {
		Entity::onEntityUpdate();
		if(rand->nextInt(1000) < m_livingSoundTime++) {
			m_livingSoundTime = -80;
			std::string var1 = getLivingSound();
			if(!var1.empty()) {
				worldObj->a(this, var1, 1.0F, (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
			}
		}

		float var2;
		float var3;
		float var4;
		if(isInsideOfMaterial()) {
			--air;
			if(air == -20) {
				air = 0;

				for(int var7 = 0; var7 < 8; ++var7) {
					var2 = rand->nextFloat() - rand->nextFloat();
					var3 = rand->nextFloat() - rand->nextFloat();
					var4 = rand->nextFloat() - rand->nextFloat();
					worldObj->spawnParticle("bubble", posX + var2, posY + var3, posZ + var4, motionX, motionY, motionZ);
				}

				attackEntityFrom(NULL, 2);
			}

			fire = 0;
		} else {
			air = maxAir;
		}

		m_prevCameraPitch = m_cameraPitch;
		if(attackTime > 0) {
			--attackTime;
		}

		if(hurtTime > 0) {
			--hurtTime;
		}

		if(heartsLife > 0) {
			--heartsLife;
		}

		if(health <= 0) {
			++deathTime;
			if(deathTime > 20) {
				setEntityDead();
			}
		}

		prevRenderYawOffset = renderYawOffset;
		prevRotationYaw = rotationYaw;
		prevRotationPitch = rotationPitch;
		onLivingUpdate();
		float var8 = posX - prevPosX;
		var2 = posZ - prevPosZ;
		var3 = sqrtf(var8 * var8 + var2 * var2);
		var4 = renderYawOffset;
		float var5 = 0.0F;
		float var6 = 0.0F;
		if(var3 > 0.05F) {
			var6 = 1.0F;
			var5 = var3 * 3.0F;
			var4 = (float)atan2f((double)var2, (double)var8) * 180.0F / 3.14159265358979323846f - 90.0F;
		}

		if(!onGround) {
			var6 = 0.0F;
		}

		m_rotationYawHead += (var6 - m_rotationYawHead) * 0.3F;

		for(var8 = var4 - renderYawOffset; var8 < -180.0F; var8 += 360.0F) {
		}

		while(var8 >= 180.0F) {
			var8 -= 360.0F;
		}

		renderYawOffset += var8 * 0.1F;

		for(var8 = rotationYaw - renderYawOffset; var8 < -180.0F; var8 += 360.0F) {
		}

		while(var8 >= 180.0F) {
			var8 -= 360.0F;
		}

		bool var9 = var8 < -90.0F || var8 >= 90.0F;
		if(var8 < -75.0F) {
			var8 = -75.0F;
		}

		if(var8 >= 75.0F) {
			var8 = 75.0F;
		}

		renderYawOffset = rotationYaw - var8;
		renderYawOffset += var8 * 0.1F;
		if(var9) {
			var5 = -var5;
		}

		while(rotationYaw - prevRotationYaw < -180.0F) {
			prevRotationYaw -= 360.0F;
		}

		while(rotationYaw - prevRotationYaw >= 180.0F) {
			prevRotationYaw += 360.0F;
		}

		while(renderYawOffset - prevRenderYawOffset < -180.0F) {
			prevRenderYawOffset -= 360.0F;
		}

		while(renderYawOffset - prevRenderYawOffset >= 180.0F) {
			prevRenderYawOffset += 360.0F;
		}

		while(rotationPitch - prevRotationPitch < -180.0F) {
			prevRotationPitch -= 360.0F;
		}

		while(rotationPitch - prevRotationPitch >= 180.0F) {
			prevRotationPitch += 360.0F;
		}

		m_prevRotationYawHead += var5;
}

void EntityLiving::setSize(float var1, float var2) {
		Entity::setSize(var1, var2);
}

void EntityLiving::heal(int var1) {
		if(health > 0) {
			health += var1;
			if(health > 20) {
				health = 20;
			}

			heartsLife = heartsHalvesLife / 2;
		}
}

bool EntityLiving::attackEntityFrom(Entity* var1, int var2) {
		if(!worldObj->m_survivalWorld) {
			return false;
		} else {
			entityAge = 0;
			if(health <= 0) {
				return false;
			} else {
				m_limbYaw = 1.5F;
				if((float)heartsLife > (float)heartsHalvesLife / 2.0F) {
					if(prevHealth - var2 >= health) {
						return false;
					}

					health = prevHealth - var2;
				} else {
					prevHealth = health;
					heartsLife = heartsHalvesLife;
					health -= var2;
					hurtTime = m_maxHurtTime = 10;
				}

				attackedAtYaw = 0.0F;
				if(var1 != NULL) {
					float var6 = var1->posX - posX;
					float var3 = var1->posZ - posZ;
					attackedAtYaw = (float)(atan2f((double)var3, (double)var6) * 180.0f / (double)(3.14159265358979323846f)) - rotationYaw;
					float var5 = sqrtf(var6 * var6 + var3 * var3);
					motionX /= 2.0F;
					motionY /= 2.0F;
					motionZ /= 2.0F;
					motionX -= var6 / var5 * 0.4F;
					motionY += 0.4F;
					motionZ -= var3 / var5 * 0.4F;
					if(motionY > 0.4F) {
						motionY = 0.4F;
					}
				} else {
					attackedAtYaw = (float)((int)(((float)::rand() / (float)RAND_MAX) * 2.0f) * 180);
				}

				if(health <= 0) {
					worldObj->a(this, getDeathSound(), 1.0F, (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
					onDeath(var1);
				} else {
					worldObj->a(this, getHurtSound(), 1.0F, (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
				}

				return true;
			}
		}
}

const std::string& EntityLiving::getLivingSound() {
		return NULL;
}

const std::string& EntityLiving::getHurtSound() {
		return "random.hurt";
}

const std::string& EntityLiving::getDeathSound() {
		return "random.hurt";
}

void EntityLiving::onDeath(Entity* var1) {
		int var3 = rand->nextInt(4);
		int var2 = rand->nextInt(3);
		if(var3 == 0) {
			for(var3 = 0; var3 < var2; ++var3) {
				dropItemWithOffset(Item::s_feather->shiftedIndex, 1);
			}

		} else if(var3 == 1) {
			for(var3 = 0; var3 < var2; ++var3) {
				dropItemWithOffset(Item::s_hoeWood->shiftedIndex, 1);
			}

		} else if(var3 != 2) {
			if(var3 == 3) {
				dropItemWithOffset(Item::s_striker->shiftedIndex, 1);
			}

		} else {
			for(var3 = 0; var3 < var2; ++var3) {
				dropItemWithOffset(Item::s_gunpowder->shiftedIndex, 1);
			}

		}
}

void EntityLiving::fall(float var1) {
		int var3 = (int)ceilf((double)(var1 - 3.0F));
		if(var3 > 0) {
			attackEntityFrom(NULL, var3);
			var3 = worldObj->getBlockId((int)posX, (int)(posY - 0.2F - yOffset), (int)posZ);
			if(var3 > 0) {
				StepSound* var4 = Block::blocksList[var3]->stepSound;
				worldObj->a(this, var4->stepSoundDir2(), var4->m_soundVolume * 0.5F, var4->m_soundPitch * (12.0F / 16.0F));
			}
		}

}

void EntityLiving::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("Health", (short)health);
		var1->setShort("HurtTime", (short)hurtTime);
		var1->setShort("DeathTime", (short)deathTime);
		var1->setShort("AttackTime", (short)attackTime);
}

void EntityLiving::readEntityFromNBT(NBTTagCompound* var1) {
		health = var1->getShort("Health");
		hurtTime = var1->getShort("HurtTime");
		deathTime = var1->getShort("DeathTime");
		attackTime = var1->getShort("AttackTime");
}

const std::string& EntityLiving::getEntityString() {
		return "Mob";
}

bool EntityLiving::isEntityAlive() {
		return !isDead && health > 0;
}

void EntityLiving::onLivingUpdate() {
		++entityAge;
		float var2;
		float var3;
		if(entityAge > 600 && rand->nextInt(800) == 0) {
			Entity* var1 = worldObj->getPlayerEntity();
			if(var1 != NULL) {
				var2 = var1->posX - posX;
				var3 = var1->posY - posY;
				float var5 = var1->posZ - posZ;
				var2 = var2 * var2 + var3 * var3 + var5 * var5;
				if(var2 < 1024.0F) {
					entityAge = 0;
				} else {
					setEntityDead();
				}
			}
		}

		if(health <= 0) {
			isJumping = false;
			moveStrafing = 0.0F;
			moveForward = 0.0F;
			randomYawVelocity = 0.0F;
		} else {
			updatePlayerActionState();
		}

		bool var6 = handleWaterMovement();
		bool var8 = handleLavaMovement();
		if(isJumping) {
			if(var6) {
				motionY += 0.04F;
			} else if(var8) {
				motionY += 0.04F;
			} else if(onGround) {
				motionY = 0.42F;
			}
		}

		moveStrafing *= 0.98F;
		moveForward *= 0.98F;
		randomYawVelocity *= 0.9F;
		var3 = moveForward;
		var2 = moveStrafing;
		float var4;
		if(handleWaterMovement()) {
			var4 = posY;
			moveFlying(var2, var3, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= 0.8F;
			motionY *= 0.8F;
			motionZ *= 0.8F;
			motionY = (float)((double)motionY - 0.02f);
			if(isCollidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6F - posY + var4, motionZ)) {
				motionY = 0.3F;
			}
		} else if(handleLavaMovement()) {
			var4 = posY;
			moveFlying(var2, var3, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= 0.5F;
			motionY *= 0.5F;
			motionZ *= 0.5F;
			motionY = (float)((double)motionY - 0.02f);
			if(isCollidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6F - posY + var4, motionZ)) {
				motionY = 0.3F;
			}
		} else {
			moveFlying(var2, var3, onGround ? 0.1F : 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= 0.91F;
			motionY *= 0.98F;
			motionZ *= 0.91F;
			motionY = (float)((double)motionY - 0.08f);
			if(onGround) {
				motionX *= 0.6F;
				motionZ *= 0.6F;
			}
		}

		m_ae = m_limbYaw;
		var4 = posX - prevPosX;
		var2 = posZ - prevPosZ;
		var2 = sqrtf(var4 * var4 + var2 * var2) * 4.0F;
		if(var2 > 1.0F) {
			var2 = 1.0F;
		}

		m_limbYaw += (var2 - m_limbYaw) * 0.4F;
		m_limbSwing += m_limbYaw;
		std::vector<Entity*> var9 = worldObj->a(this, boundingBox.expand(0.2F, 0.0F, 0.2F));
		if(true && var9.size() > 0) {
			for(size_t var7 = 0; var7 < var9.size(); ++var7) {
				Entity* var10 = (Entity*)var9[var7];
				if(var10->canBePushed()) {
					var10->applyEntityCollision(this);
				}
			}
		}

}

void EntityLiving::updatePlayerActionState() {
		if(rand->nextFloat() < 0.07F) {
			moveStrafing = (rand->nextFloat() - 0.5F) * moveSpeed;
			moveForward = rand->nextFloat() * moveSpeed;
		}

		isJumping = rand->nextFloat() < 0.01F;
		if(rand->nextFloat() < 0.04F) {
			randomYawVelocity = (rand->nextFloat() - 0.5F) * 60.0F;
		}

		rotationYaw += randomYawVelocity;
		rotationPitch = 0.0F;
		bool var1 = handleWaterMovement();
		bool var2 = handleLavaMovement();
		if(var1 || var2) {
			isJumping = rand->nextFloat() < 0.8F;
		}

}

bool EntityLiving::getCanSpawnHere(float var1, float var2, float var3) {
		setPosition(var1, var2 + height / 2.0F, var3);
		return worldObj->checkIfAABBIsClear1(boundingBox) && worldObj->getCollidingBoundingBoxes(boundingBox).size() == 0 && !worldObj->getIsAnyLiquid(boundingBox);
}
