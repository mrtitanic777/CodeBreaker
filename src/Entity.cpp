#include "stdafx.h"
// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include "Common.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Block.h"
#include "ItemStack.h"
#include "Material.h"
#include "NBTTagFloat.h"
#include "StepSound.h"

Entity::Entity()
	: preventEntitySpawning(false)
	, worldObj(NULL)
	, prevPosX(0)
	, prevPosY(0)
	, prevPosZ(0)
	, posX(0)
	, posY(0)
	, posZ(0)
	, motionX(0)
	, motionY(0)
	, motionZ(0)
	, rotationYaw(0)
	, rotationPitch(0)
	, prevRotationYaw(0)
	, prevRotationPitch(0)
	, onGround(false)
	, isCollidedHorizontally(false)
	, m_surfaceCollision(true)
	, isDead(false)
	, yOffset(0.0f)
	, width(0.6f)
	, height(1.8f)
	, prevDistanceWalkedModified(0.0f)
	, distanceWalkedModified(0.0f)
	, canTriggerWalking(true)
	, m_fallDistance(0.0f)
	, m_nextStepDistance(1)
	, lastTickPosX(0)
	, lastTickPosY(0)
	, lastTickPosZ(0)
	, m_ySize(0.0f)
	, stepHeight(0.0f)
	, noClip(false)
	, m_entityCollisionReduction(0.0f)
	, rand(new Random())
	, ticksExisted(0)
	, fireResistance(1)
	, fire(0)
	, maxAir(300)
	, m_inWater(false)
	, heartsLife(0)
	, air(300)
	, m_isFirstUpdate(true)
{
}

Entity::Entity(World* var1)
	: preventEntitySpawning(false)
	, worldObj(NULL)
	, prevPosX(0)
	, prevPosY(0)
	, prevPosZ(0)
	, posX(0)
	, posY(0)
	, posZ(0)
	, motionX(0)
	, motionY(0)
	, motionZ(0)
	, rotationYaw(0)
	, rotationPitch(0)
	, prevRotationYaw(0)
	, prevRotationPitch(0)
	, onGround(false)
	, isCollidedHorizontally(false)
	, m_surfaceCollision(true)
	, isDead(false)
	, yOffset(0.0f)
	, width(0.6f)
	, height(1.8f)
	, prevDistanceWalkedModified(0.0f)
	, distanceWalkedModified(0.0f)
	, canTriggerWalking(true)
	, m_fallDistance(0.0f)
	, m_nextStepDistance(1)
	, lastTickPosX(0)
	, lastTickPosY(0)
	, lastTickPosZ(0)
	, m_ySize(0.0f)
	, stepHeight(0.0f)
	, noClip(false)
	, m_entityCollisionReduction(0.0f)
	, rand(new Random())
	, ticksExisted(0)
	, fireResistance(1)
	, fire(0)
	, maxAir(300)
	, m_inWater(false)
	, heartsLife(0)
	, air(300)
	, m_isFirstUpdate(true)
{
		worldObj = var1;
		setPosition(0.0F, 0.0F, 0.0F);
}

Entity::~Entity() {
	worldObj = NULL;
	rand = NULL;
}

void Entity::preparePlayerToSpawn() {
		if(worldObj != NULL) {
			float var1 = (float)worldObj->m_xSpawn + 0.5F;
			float var2 = (float)worldObj->m_ySpawn;

			for(float var3 = (float)worldObj->m_zSpawn + 0.5F; var2 > 0.0F; ++var2) {
				setPosition(var1, var2, var3);
				if(worldObj->getCollidingBoundingBoxes(boundingBox).size() == 0) {
					break;
				}
			}

			motionX = motionY = motionZ = 0.0F;
			rotationYaw = worldObj->m_rotSpawn;
			rotationPitch = 0.0F;
		}
}

void Entity::setEntityDead() {
		isDead = true;
}

void Entity::setSize(float var1, float var2) {
		width = var1;
		height = var2;
}

void Entity::setPosition(float var1, float var2, float var3) {
		posX = var1;
		posY = var2;
		posZ = var3;
		float var4 = width / 2.0F;
		float var5 = height / 2.0F;
		boundingBox = AxisAlignedBB(var1 - var4, var2 - var5, var3 - var4, var1 + var4, var2 + var5, var3 + var4);
}

void Entity::onEntityUpdate() {
		++ticksExisted;
		prevDistanceWalkedModified = distanceWalkedModified;
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		prevRotationPitch = rotationPitch;
		prevRotationYaw = rotationYaw;
		if(handleWaterMovement()) {
			if(!m_inWater && !m_isFirstUpdate) {
				float var1 = sqrtf(motionX * motionX * 0.2F + motionY * motionY + motionZ * motionZ * 0.2F) * 0.2F;
				if(var1 > 1.0F) {
					var1 = 1.0F;
				}

				worldObj->a(this, "random.splash", var1, 1.0F + (rand->nextFloat() - rand->nextFloat()) * 0.4F);
				var1 = (float)((int)boundingBox.minY);

				int var2;
				float var3;
				float var4;
				for(var2 = 0; (float)var2 < 1.0F + width * 20.0F; ++var2) {
					var3 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					var4 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					worldObj->spawnParticle("bubble", posX + var3, var1 + 1.0F, posZ + var4, motionX, motionY - rand->nextFloat() * 0.2F, motionZ);
				}

				for(var2 = 0; (float)var2 < 1.0F + width * 20.0F; ++var2) {
					var3 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					var4 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					worldObj->spawnParticle("splash", posX + var3, var1 + 1.0F, posZ + var4, motionX, motionY, motionZ);
				}
			}

			m_fallDistance = 0.0F;
			m_inWater = true;
			fire = 0;
		} else {
			m_inWater = false;
		}

		if(fire > 0) {
			if(fire % 20 == 0) {
				attackEntityFrom(NULL, 1);
			}

			--fire;
		}

		if(handleLavaMovement()) {
			attackEntityFrom(NULL, 10);
			fire = 600;
		}

		float var5;
		if(posX < -8.0F) {
			var5 = -(posX + 8.0F);
			motionX += var5 * 0.001F;
		}

		if(posZ < -8.0F) {
			var5 = -(posZ + 8.0F);
			motionZ += var5 * 0.001F;
		}

		if(posX > (float)worldObj->width + 8.0F) {
			var5 = posX - (float)worldObj->width + 8.0F;
			motionX -= var5 * 0.001F;
		}

		if(posZ > (float)worldObj /* PORT: array length - use tracked size */ + 8.0F) {
			var5 = posZ - (float)worldObj /* PORT: array length - use tracked size */ + 8.0F;
			motionZ -= var5 * 0.001F;
		}

		m_isFirstUpdate = false;
}

bool Entity::isOffsetPositionInLiquid(float var1, float var2, float var3) {
		float var4 = var3;
		var3 = var2;
		var2 = var1;
		AxisAlignedBB var5 = boundingBox;
		var5 = AxisAlignedBB(var5.minX + var4, var5.minY + var3, var5.minZ + var4, var5.maxX + var2, var5.maxY + var3, var5.maxZ + var4);
		std::vector<void*> /* PORT: type */ var6 = worldObj->getCollidingBoundingBoxes(var5);
		return var6.size() > 0 ? false : !worldObj->getIsAnyLiquid(var5);
}

void Entity::moveEntity(float var1, float var2, float var3) {
		if(noClip) {
			boundingBox.offset(var1, var2, var3);
			posX = (boundingBox.minX + boundingBox.maxX) / 2.0F;
			posY = boundingBox.minY + yOffset - m_ySize;
			posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0F;
		} else {
			float var4 = posX;
			float var5 = posZ;
			float var6 = var1;
			float var7 = var2;
			float var8 = var3;
			AxisAlignedBB var9 = boundingBox.copy();
			std::vector<void*> /* PORT: type */ var10 = worldObj->getCollidingBoundingBoxes(boundingBox.addCoord(var1, var2, var3));

			for(size_t var11 = 0; var11 < var10.size(); ++var11) {
				var2 = var10[var11]->calculateYOffset(boundingBox, var2);
			}

			boundingBox.offset(0.0F, var2, 0.0F);
			if(!m_surfaceCollision && var7 != var2) {
				var3 = 0.0F;
				var2 = var3;
				var1 = var3;
			}

			bool var18 = onGround || var7 != var2 && var7 < 0.0F;

			size_t var12;
			for(var12 = 0; var12 < var10.size(); ++var12) {
				var1 = var10[var12]->calculateXOffset(boundingBox, var1);
			}

			boundingBox.offset(var1, 0.0F, 0.0F);
			if(!m_surfaceCollision && var6 != var1) {
				var3 = 0.0F;
				var2 = var3;
				var1 = var3;
			}

			for(var12 = 0; var12 < var10.size(); ++var12) {
				var3 = var10[var12]->calculateZOffset(boundingBox, var3);
			}

			boundingBox.offset(0.0F, 0.0F, var3);
			if(!m_surfaceCollision && var8 != var3) {
				var3 = 0.0F;
				var2 = var3;
				var1 = var3;
			}

			size_t var17;
			float var19;
			float var20;
			if(stepHeight > 0.0F && var18 && m_ySize < 0.05F && (var6 != var1 || var8 != var3)) {
				var20 = var1;
				var19 = var2;
				float var13 = var3;
				var1 = var6;
				var2 = stepHeight;
				var3 = var8;
				AxisAlignedBB var14 = boundingBox.copy();
				boundingBox = var9.copy();
				var10 = worldObj->getCollidingBoundingBoxes(boundingBox.addCoord(var6, var2, var8));

				for(var17 = 0; var17 < var10.size(); ++var17) {
					var2 = var10[var17]->calculateYOffset(boundingBox, var2);
				}

				boundingBox.offset(0.0F, var2, 0.0F);
				if(!m_surfaceCollision && var7 != var2) {
					var3 = 0.0F;
					var2 = var3;
					var1 = var3;
				}

				for(var17 = 0; var17 < var10.size(); ++var17) {
					var1 = var10[var17]->calculateXOffset(boundingBox, var1);
				}

				boundingBox.offset(var1, 0.0F, 0.0F);
				if(!m_surfaceCollision && var6 != var1) {
					var3 = 0.0F;
					var2 = var3;
					var1 = var3;
				}

				for(var17 = 0; var17 < var10.size(); ++var17) {
					var3 = var10[var17]->calculateZOffset(boundingBox, var3);
				}

				boundingBox.offset(0.0F, 0.0F, var3);
				if(!m_surfaceCollision && var8 != var3) {
					var3 = 0.0F;
					var2 = var3;
					var1 = var3;
				}

				if(var20 * var20 + var13 * var13 >= var1 * var1 + var3 * var3) {
					var1 = var20;
					var2 = var19;
					var3 = var13;
					boundingBox = var14.copy();
				} else {
					m_ySize = (float)((double)m_ySize + 0.5f);
				}
			}

			posX = (boundingBox.minX + boundingBox.maxX) / 2.0F;
			posY = boundingBox.minY + yOffset - m_ySize;
			posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0F;
			isCollidedHorizontally = var6 != var1 || var8 != var3;
			onGround = var7 != var2 && var7 < 0.0F;
			if(onGround) {
				if(m_fallDistance > 0.0F) {
					fall(m_fallDistance);
					m_fallDistance = 0.0F;
				}
			} else if(var2 < 0.0F) {
				m_fallDistance -= var2;
			}

			if(var6 != var1) {
				motionX = 0.0F;
			}

			if(var7 != var2) {
				motionY = 0.0F;
			}

			if(var8 != var3) {
				motionZ = 0.0F;
			}

			var20 = posX - var4;
			var19 = posZ - var5;
			distanceWalkedModified = (float)((double)distanceWalkedModified + (double)sqrtf(var20 * var20 + var19 * var19) * 0.6f);
			if(canTriggerWalking) {
				int var21 = (int)posX;
				int var23 = (int)(posY - 0.2F - yOffset);
				var17 = (int)posZ;
				int var16 = worldObj->getBlockId(var21, var23, var17);
				if(distanceWalkedModified > (float)m_nextStepDistance && var16 > 0) {
					++m_nextStepDistance;
					StepSound* var15 = Block::blocksList[var16].stepSound;
					worldObj->a(this, var15->stepSoundDir2(), var15->m_soundVolume * 0.15F, var15->m_soundPitch);
					Block::blocksList[var16].onEntityWalking(worldObj, var21, var23, var17);
				}
			}

			m_ySize *= 0.4F;
			bool var22 = handleWaterMovement();
			if(worldObj->isBoundingBoxBurning(boundingBox)) {
				dealFireDamage(1);
				if(!var22) {
					++fire;
					if(fire == 0) {
						fire = 300;
					}
				}
			} else if(fire <= 0) {
				fire = -fireResistance;
			}

			if(var22 && fire > 0) {
				worldObj->a(this, "random.fizz", 0.7F, 1.6F + (rand->nextFloat() - rand->nextFloat()) * 0.4F);
				fire = -fireResistance;
			}

		}
}

void Entity::dealFireDamage(int var1) {
		attackEntityFrom(NULL, 1);
}

void Entity::fall(float var1) {
	// TODO: implement
}

bool Entity::handleWaterMovement() {
		return worldObj->handleMaterialAcceleration(boundingBox.expand(0.0F, -0.4F, 0.0F), Material::s_water);
}

bool Entity::isInsideOfMaterial() {
		int var1 = worldObj->getBlockId((int)posX, (int)(posY + getEyeHeight()), (int)posZ);
		return var1 != 0 ? Block::blocksList[var1].material == Material::s_water : false;
}

float Entity::getEyeHeight() {
		return 0.0F;
}

bool Entity::handleLavaMovement() {
		return worldObj->handleMaterialAcceleration(boundingBox.expand(0.0F, -0.4F, 0.0F), Material::s_lava);
}

void Entity::moveFlying(float var1, float var2, float var3) {
		float var4 = sqrtf(var1 * var1 + var2 * var2);
		if(var4 >= 0.01F) {
			if(var4 < 1.0F) {
				var4 = 1.0F;
			}

			var4 = var3 / var4;
			var1 *= var4;
			var2 *= var4;
			var3 = sinf(rotationYaw * 3.14159265358979323846f / 180.0F);
			var4 = cosf(rotationYaw * 3.14159265358979323846f / 180.0F);
			motionX += var1 * var4 - var2 * var3;
			motionZ += var2 * var4 + var1 * var3;
		}
}

float Entity::getEntityBrightness(float var1) {
		int var4 = (int)posX;
		int var2 = (int)(posY + yOffset / 2.0F);
		int var3 = (int)posZ;
		return worldObj->getLightBrightness(var4, var2, var3);
}

void Entity::setWorld(World* var1) {
		worldObj = var1;
}

void Entity::setPositionAndRotation(float var1, float var2, float var3, float var4, float var5) {
		prevPosX = posX = var1;
		prevPosY = posY = var2 + yOffset;
		prevPosZ = posZ = var3;
		rotationYaw = var4;
		rotationPitch = var5;
		setPosition(posX, posY, posZ);
}

float Entity::getDistanceSqToEntity(Entity* var1) {
		float var2 = posX - var1->posX;
		float var3 = posY - var1->posY;
		float var4 = posZ - var1->posZ;
		return var2 * var2 + var3 * var3 + var4 * var4;
}

void Entity::onCollideWithPlayer(EntityPlayer* var1) {
	// TODO: implement
}

void Entity::applyEntityCollision(Entity* var1) {
		float var2 = var1->posX - posX;
		float var3 = var1->posZ - posZ;
		float var4 = var2 * var2 + var3 * var3;
		if(var4 >= 0.01F) {
			var4 = sqrtf(var4);
			var2 /= var4;
			var3 /= var4;
			var2 /= var4;
			var3 /= var4;
			var2 *= 0.05F;
			var3 *= 0.05F;
			addVelocity(-var2, 0.0F, -var3);
			var1->addVelocity(var2, 0.0F, var3);
		}

}

void Entity::addVelocity(float var1, float var2, float var3) {
		motionX += var1;
		motionY = motionY;
		motionZ += var3;
}

bool Entity::attackEntityFrom(Entity* var1, int var2) {
		return false;
}

bool Entity::canBeCollidedWith() {
		return false;
}

bool Entity::canBePushed() {
		return false;
}

const std::string& Entity::getTexture() {
		return NULL;
}

void Entity::writeToNBT(NBTTagCompound* var1) {
		std::string var2 = getEntityString();
		if(!isDead && var2 != NULL) {
			var1->setString("id", var2);
   float _tmpVerts1[3] = {posX, posY, posZ};
			var1->setTag("Pos", newDoubleNBTList(_tmpVerts1));
   float _tmpVerts2[3] = {motionX, motionY, motionZ};
			var1->setTag("Motion", newDoubleNBTList(_tmpVerts2));
   float _tmpVerts3[2] = {rotationYaw, rotationPitch};
			var1->setTag("Rotation", newDoubleNBTList(_tmpVerts3));
			var1->setFloat("FallDistance", m_fallDistance);
			var1->setShort("Fire", (short)fire);
			var1->setShort("Air", (short)air);
			writeEntityToNBT(var1);
		}
}

void Entity::readFromNBT(NBTTagCompound* var1) {
		NBTTagList* var2 = var1->getTagList("Pos");
		NBTTagList* var3 = var1->getTagList("Motion");
		NBTTagList* var4 = var1->getTagList("Rotation");
		posX = static_cast<NBTTagFloat*>(var2->tagAt(0))->m_floatValue;
		posY = static_cast<NBTTagFloat*>(var2->tagAt(1))->m_floatValue;
		posZ = static_cast<NBTTagFloat*>(var2->tagAt(2))->m_floatValue;
		motionX = static_cast<NBTTagFloat*>(var3->tagAt(0))->m_floatValue;
		motionY = static_cast<NBTTagFloat*>(var3->tagAt(1))->m_floatValue;
		motionZ = static_cast<NBTTagFloat*>(var3->tagAt(2))->m_floatValue;
		rotationYaw = static_cast<NBTTagFloat*>(var4->tagAt(0))->m_floatValue;
		rotationPitch = static_cast<NBTTagFloat*>(var4->tagAt(1))->m_floatValue;
		m_fallDistance = var1->getFloat("FallDistance");
		fire = var1->getShort("Fire");
		air = var1->getShort("Air");
		setPositionAndRotation(posX, posY, posZ, rotationYaw, rotationPitch);
		readEntityFromNBT(var1);
}

NBTTagList* Entity::newDoubleNBTList(float* var0, int var0_count) {
		NBTTagList* var1 = new NBTTagList();
		var0 = var0;
		int var2 = 4 /* PORT: quad vertex count */;

		for(int var3 = 0; var3 < var2; ++var3) {
			float var4 = var0[var3];
			var1->setTag(new NBTTagFloat(var4));
		}

		return var1;
}

float Entity::getShadowSize() {
		return height / 2.0F;
}

EntityItem* Entity::dropItemWithOffset(int var1, int var2) {
		return entityDropItem(var1, 1, 0.0F);
}

EntityItem* Entity::entityDropItem(int var1, int var2, float var3) {
		EntityItem* var4 = new EntityItem(worldObj, posX, posY + var3, posZ, ItemStack(var1, var2));
		var4->m_delayBeforeCanPickup = 10;
		worldObj->spawnEntityInWorld(var4);
		return var4;
}

bool Entity::isEntityAlive() {
		return !isDead;
}
