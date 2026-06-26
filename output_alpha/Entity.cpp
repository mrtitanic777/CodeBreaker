#include "stdafx.h"
// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include "Common.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Material.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Block.h"
#include "ItemStack.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "StepSound.h"
#include "BlockFluids.h"
#include "EntityList.h"
#include "MathHelper.h"

int Entity::s_field_864_a = 0;

Entity::Entity()
	: m_field_620_ab(s_field_864_a++)
	, m_field_619_ac(1.0f)
	, m_field_618_ad(false)
	, m_riddenByEntity(NULL)
	, m_ridingEntity(NULL)
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
	, boundingBox(AxisAlignedBB::getBoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
	, onGround(false)
	, m_field_9297_aI(false)
	, m_field_9296_aJ(false)
	, m_field_9295_aK(false)
	, m_field_9294_aL(false)
	, m_field_9293_aM(true)
	, isDead(false)
	, yOffset(0.0f)
	, width(0.6f)
	, height(1.8f)
	, m_field_9291_aR(0.0f)
	, m_field_9290_aS(0.0f)
	, m_entityWalks(true)
	, m_fallDistance(0.0f)
	, m_field_863_b(1)
	, lastTickPosX(0)
	, lastTickPosY(0)
	, lastTickPosZ(0)
	, m_field_9287_aY(0.0f)
	, m_field_9286_aZ(0.0f)
	, m_field_9314_ba(false)
	, m_field_632_aO(0.0f)
	, m_field_9313_bc(false)
	, rand(new Random())
	, m_field_9311_be(0)
	, m_field_9310_bf(1)
	, fire(0)
	, m_field_9308_bh(300)
	, m_field_9307_bi(false)
	, m_field_9306_bj(0)
	, air(300)
	, m_field_862_c(true)
	, skinUrl("")
	, m_isImmuneToFire(false)
	, m_minecartType(0)
	, m_field_667_e(0)
	, m_field_621_aZ(false)
	, m_field_657_ba(0)
	, m_field_656_bb(0)
	, m_field_654_bc(0)
	, m_field_9303_br(0)
	, m_field_9302_bs(0)
	, m_field_9301_bt(0)
	, m_field_9300_bu(false)
	, m_field_9299_bv(false)
	, m_field_12240_bw(false)
{
}

Entity::Entity(World* var1)
	: m_field_620_ab(s_field_864_a++)
	, m_field_619_ac(1.0f)
	, m_field_618_ad(false)
	, m_riddenByEntity(NULL)
	, m_ridingEntity(NULL)
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
	, boundingBox(AxisAlignedBB::getBoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
	, onGround(false)
	, m_field_9297_aI(false)
	, m_field_9296_aJ(false)
	, m_field_9295_aK(false)
	, m_field_9294_aL(false)
	, m_field_9293_aM(true)
	, isDead(false)
	, yOffset(0.0f)
	, width(0.6f)
	, height(1.8f)
	, m_field_9291_aR(0.0f)
	, m_field_9290_aS(0.0f)
	, m_entityWalks(true)
	, m_fallDistance(0.0f)
	, m_field_863_b(1)
	, lastTickPosX(0)
	, lastTickPosY(0)
	, lastTickPosZ(0)
	, m_field_9287_aY(0.0f)
	, m_field_9286_aZ(0.0f)
	, m_field_9314_ba(false)
	, m_field_632_aO(0.0f)
	, m_field_9313_bc(false)
	, rand(new Random())
	, m_field_9311_be(0)
	, m_field_9310_bf(1)
	, fire(0)
	, m_field_9308_bh(300)
	, m_field_9307_bi(false)
	, m_field_9306_bj(0)
	, air(300)
	, m_field_862_c(true)
	, skinUrl("")
	, m_isImmuneToFire(false)
	, m_minecartType(0)
	, m_field_667_e(0)
	, m_field_621_aZ(false)
	, m_field_657_ba(0)
	, m_field_656_bb(0)
	, m_field_654_bc(0)
	, m_field_9303_br(0)
	, m_field_9302_bs(0)
	, m_field_9301_bt(0)
	, m_field_9300_bu(false)
	, m_field_9299_bv(false)
	, m_field_12240_bw(false)
{
		worldObj = var1;
		setPosition(0.0f, 0.0f, 0.0f);
}

Entity::~Entity() {
	m_riddenByEntity = NULL;
	m_ridingEntity = NULL;
	worldObj = NULL;
	rand = NULL;
}

bool Entity::equals(void* /* PORT: was Object */ var1) {
		return var1 != NULL ? ((Entity*)var1)->m_field_620_ab == m_field_620_ab : false;
}

int Entity::hashCode() {
		return m_field_620_ab;
}

void Entity::preparePlayerToSpawn() {
		if(worldObj != NULL) {
			while(posY > 0.0f) {
				setPosition(posX, posY, posZ);
				if(worldObj->getCollidingBoundingBoxes(this, boundingBox).size() == 0) {
					break;
				}

				++posY;
			}

			motionX = motionY = motionZ = 0.0f;
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

void Entity::setRotation(float var1, float var2) {
		rotationYaw = var1;
		rotationPitch = var2;
}

void Entity::setPosition(double var1, double var3, double var5) {
		posX = var1;
		posY = var3;
		posZ = var5;
		float var7 = width / 2.0F;
		float var8 = height;
		boundingBox.setBounds(var1 - (double)var7, var3 - (double)yOffset + (double)m_field_9287_aY, var5 - (double)var7, var1 + (double)var7, var3 - (double)yOffset + (double)m_field_9287_aY + (double)var8, var5 + (double)var7);
}

void Entity::func_346_d(float var1, float var2) {
		float var3 = rotationPitch;
		float var4 = rotationYaw;
		rotationYaw = (float)((double)rotationYaw + (double)var1 * 0.15f);
		rotationPitch = (float)((double)rotationPitch - (double)var2 * 0.15f);
		if(rotationPitch < -90.0F) {
			rotationPitch = -90.0F;
		}

		if(rotationPitch > 90.0F) {
			rotationPitch = 90.0F;
		}

		prevRotationPitch += rotationPitch - var3;
		prevRotationYaw += rotationYaw - var4;
}

void Entity::onUpdate() {
		func_391_y();
}

void Entity::func_391_y() {
		if(m_ridingEntity != NULL && m_ridingEntity->isDead) {
			m_ridingEntity = NULL;
		}

		++m_field_9311_be;
		m_field_9291_aR = m_field_9290_aS;
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		prevRotationPitch = rotationPitch;
		prevRotationYaw = rotationYaw;
		if(handleWaterMovement()) {
			if(!m_field_9307_bi && !m_field_862_c) {
				float var1 = sqrt(motionX * motionX * (double)0.2F + motionY * motionY + motionZ * motionZ * (double)0.2F) * 0.2F;
				if(var1 > 1.0F) {
					var1 = 1.0F;
				}

				worldObj->playSoundAtEntity(this, "random.splash", var1, 1.0F + (rand->nextFloat() - rand->nextFloat()) * 0.4F);
				float var2 = (float)(int)floor(boundingBox.minY);

				int var3;
				float var4;
				float var5;
				for(var3 = 0; (float)var3 < 1.0F + width * 20.0F; ++var3) {
					var4 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					var5 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					worldObj->spawnParticle("bubble", posX + (double)var4, (double)(var2 + 1.0F), posZ + (double)var5, motionX, motionY - (double)(rand->nextFloat() * 0.2F), motionZ);
				}

				for(var3 = 0; (float)var3 < 1.0F + width * 20.0F; ++var3) {
					var4 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					var5 = (rand->nextFloat() * 2.0F - 1.0F) * width;
					worldObj->spawnParticle("splash", posX + (double)var4, (double)(var2 + 1.0F), posZ + (double)var5, motionX, motionY, motionZ);
				}
			}

			m_fallDistance = 0.0F;
			m_field_9307_bi = true;
			fire = 0;
		} else {
			m_field_9307_bi = false;
		}

		if(worldObj->m_multiplayerWorld) {
			fire = 0;
		} else if(fire > 0) {
			if(m_isImmuneToFire) {
				fire -= 4;
				if(fire < 0) {
					fire = 0;
				}
			} else {
				if(fire % 20 == 0) {
					canAttackEntity(NULL, 1);
				}

				--fire;
			}
		}

		if(handleLavaMovement()) {
			func_4038_J();
		}

		if(posY < -64.0f) {
			func_4034_G();
		}

		m_field_862_c = false;
}

void Entity::func_4038_J() {
		if(!m_isImmuneToFire) {
			canAttackEntity(NULL, 4);
			fire = 600;
		}

}

void Entity::func_4034_G() {
		setEntityDead();
}

bool Entity::func_403_b(double var1, double var3, double var5) {
		AxisAlignedBB var7 = boundingBox.getOffsetBoundingBox(var1, var3, var5);
		std::vector<AxisAlignedBB> var8 = worldObj->getCollidingBoundingBoxes(this, var7);
		return var8.size() > 0 ? false : !worldObj->getIsAnyLiquid(var7);
}

void Entity::moveEntity(double var1, double var3, double var5) {
		if(m_field_9314_ba) {
			boundingBox.offset(var1, var3, var5);
			posX = (boundingBox.minX + boundingBox.maxX) / 2.0f;
			posY = boundingBox.minY + (double)yOffset - (double)m_field_9287_aY;
			posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0f;
		} else {
			double var7 = posX;
			double var9 = posZ;
			double var11 = var1;
			double var13 = var3;
			double var15 = var5;
			AxisAlignedBB var17 = boundingBox.copy();
			bool var18 = onGround && func_381_o();
			if(var18) {
				double var19;
				for(var19 = 0.05f; var1 != 0.0f && worldObj->getCollidingBoundingBoxes(this, boundingBox.getOffsetBoundingBox(var1, -1.0f, 0.0f)).size() == 0; var11 = var1) {
					if(var1 < var19 && var1 >= -var19) {
						var1 = 0.0f;
					} else if(var1 > 0.0f) {
						var1 -= var19;
					} else {
						var1 += var19;
					}
				}

				for(; var5 != 0.0f && worldObj->getCollidingBoundingBoxes(this, boundingBox.getOffsetBoundingBox(0.0f, -1.0f, var5)).size() == 0; var15 = var5) {
					if(var5 < var19 && var5 >= -var19) {
						var5 = 0.0f;
					} else if(var5 > 0.0f) {
						var5 -= var19;
					} else {
						var5 += var19;
					}
				}
			}

			std::vector<AxisAlignedBB> var35 = worldObj->getCollidingBoundingBoxes(this, boundingBox.addCoord(var1, var3, var5));

			for(size_t var20 = 0; var20 < var35.size(); ++var20) {
				var3 = var35[var20].func_1172_b(boundingBox, var3);
			}

			boundingBox.offset(0.0f, var3, 0.0f);
			if(!m_field_9293_aM && var13 != var3) {
				var5 = 0.0f;
				var3 = var5;
				var1 = var5;
			}

			bool var36 = onGround || var13 != var3 && var13 < 0.0f;

			size_t var21;
			for(var21 = 0; var21 < var35.size(); ++var21) {
				var1 = var35[var21].func_1163_a(boundingBox, var1);
			}

			boundingBox.offset(var1, 0.0f, 0.0f);
			if(!m_field_9293_aM && var11 != var1) {
				var5 = 0.0f;
				var3 = var5;
				var1 = var5;
			}

			for(var21 = 0; var21 < var35.size(); ++var21) {
				var5 = var35[var21].func_1162_c(boundingBox, var5);
			}

			boundingBox.offset(0.0f, 0.0f, var5);
			if(!m_field_9293_aM && var15 != var5) {
				var5 = 0.0f;
				var3 = var5;
				var1 = var5;
			}

			double var23;
			size_t var28;
			double var37;
			if(m_field_9286_aZ > 0.0F && var36 && m_field_9287_aY < 0.05F && (var11 != var1 || var15 != var5)) {
				var37 = var1;
				var23 = var3;
				double var25 = var5;
				var1 = var11;
				var3 = (double)m_field_9286_aZ;
				var5 = var15;
				AxisAlignedBB var27 = boundingBox.copy();
				boundingBox.setBB(var17);
				var35 = worldObj->getCollidingBoundingBoxes(this, boundingBox.addCoord(var11, var3, var15));

				for(var28 = 0; var28 < var35.size(); ++var28) {
					var3 = var35[var28].func_1172_b(boundingBox, var3);
				}

				boundingBox.offset(0.0f, var3, 0.0f);
				if(!m_field_9293_aM && var13 != var3) {
					var5 = 0.0f;
					var3 = var5;
					var1 = var5;
				}

				for(var28 = 0; var28 < var35.size(); ++var28) {
					var1 = var35[var28].func_1163_a(boundingBox, var1);
				}

				boundingBox.offset(var1, 0.0f, 0.0f);
				if(!m_field_9293_aM && var11 != var1) {
					var5 = 0.0f;
					var3 = var5;
					var1 = var5;
				}

				for(var28 = 0; var28 < var35.size(); ++var28) {
					var5 = var35[var28].func_1162_c(boundingBox, var5);
				}

				boundingBox.offset(0.0f, 0.0f, var5);
				if(!m_field_9293_aM && var15 != var5) {
					var5 = 0.0f;
					var3 = var5;
					var1 = var5;
				}

				if(var37 * var37 + var25 * var25 >= var1 * var1 + var5 * var5) {
					var1 = var37;
					var3 = var23;
					var5 = var25;
					boundingBox.setBB(var27);
				} else {
					m_field_9287_aY = (float)((double)m_field_9287_aY + 0.5f);
				}
			}

			posX = (boundingBox.minX + boundingBox.maxX) / 2.0f;
			posY = boundingBox.minY + (double)yOffset - (double)m_field_9287_aY;
			posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0f;
			m_field_9297_aI = var11 != var1 || var15 != var5;
			m_field_9296_aJ = var13 != var3;
			onGround = var13 != var3 && var13 < 0.0f;
			m_field_9295_aK = m_field_9297_aI || m_field_9296_aJ;
			func_9279_a(var3, onGround);
			if(var11 != var1) {
				motionX = 0.0f;
			}

			if(var13 != var3) {
				motionY = 0.0f;
			}

			if(var15 != var5) {
				motionZ = 0.0f;
			}

			var37 = posX - var7;
			var23 = posZ - var9;
			int var26;
			int var38;
			int var40;
			if(m_entityWalks && !var18) {
				m_field_9290_aS = (float)((double)m_field_9290_aS + (double)sqrt(var37 * var37 + var23 * var23) * 0.6f);
				var38 = (int)floor(posX);
				var26 = (int)floor(posY - (double)0.2F - (double)yOffset);
				var40 = (int)floor(posZ);
				var28 = worldObj->getBlockId(var38, var26, var40);
				if(m_field_9290_aS > (float)m_field_863_b && var28 > 0) {
					++m_field_863_b;
					StepSound* var29 = Block::blocksList[var28]->stepSound;
					if(worldObj->getBlockId(var38, var26 + 1, var40) == Block::s_snow->blockID) {
						var29 = Block::s_snow->stepSound;
						worldObj->playSoundAtEntity(this, var29->func_1145_d(), var29->func_1147_b() * 0.15F, var29->func_1144_c());
					} else if(!Block::blocksList[var28]->m_blockMaterial->getIsLiquid()) {
						worldObj->playSoundAtEntity(this, var29->func_1145_d(), var29->func_1147_b() * 0.15F, var29->func_1144_c());
					}

					Block::blocksList[var28]->onEntityWalking(worldObj, var38, var26, var40, this);
				}
			}

			var38 = (int)floor(boundingBox.minX);
			var26 = (int)floor(boundingBox.minY);
			var40 = (int)floor(boundingBox.minZ);
			var28 = (int)floor(boundingBox.maxX);
			int var41 = (int)floor(boundingBox.maxY);
			int var30 = (int)floor(boundingBox.maxZ);

			for(int var31 = var38; var31 <= var28; ++var31) {
				for(int var32 = var26; var32 <= var41; ++var32) {
					for(int var33 = var40; var33 <= var30; ++var33) {
						int var34 = worldObj->getBlockId(var31, var32, var33);
						if(var34 > 0) {
							Block::blocksList[var34]->onEntityCollidedWithBlock(worldObj, var31, var32, var33, this);
						}
					}
				}
			}

			m_field_9287_aY *= 0.4F;
			bool var39 = handleWaterMovement();
			if(worldObj->isBoundingBoxBurning(boundingBox)) {
				func_355_a(1);
				if(!var39) {
					++fire;
					if(fire == 0) {
						fire = 300;
					}
				}
			} else if(fire <= 0) {
				fire = -m_field_9310_bf;
			}

			if(var39 && fire > 0) {
				worldObj->playSoundAtEntity(this, "random.fizz", 0.7F, 1.6F + (rand->nextFloat() - rand->nextFloat()) * 0.4F);
				fire = -m_field_9310_bf;
			}

		}
}

void Entity::func_9279_a(double var1, bool var3) {
		if(var3) {
			if(m_fallDistance > 0.0F) {
				fall(m_fallDistance);
				m_fallDistance = 0.0F;
			}
		} else if(var1 < 0.0f) {
			m_fallDistance = (float)((double)m_fallDistance - var1);
		}

}

bool Entity::func_381_o() {
		return false;
}

AxisAlignedBB Entity::func_372_f_() {
		return AxisAlignedBB();
}

void Entity::func_355_a(int var1) {
		if(!m_isImmuneToFire) {
			canAttackEntity(NULL, var1);
		}

}

void Entity::fall(float var1) {
	// TODO: implement
}

bool Entity::handleWaterMovement() {
		return worldObj->func_682_a(boundingBox.expands(0.0f, (double)-0.4F, 0.0f), Material::s_water, this);
}

bool Entity::isInsideOfMaterial(Material* var1) {
		double var2 = posY + (double)func_373_s();
		int var4 = (int)floor(posX);
		int var5 = (int)floorf((float)(int)floor(var2));
		int var6 = (int)floor(posZ);
		int var7 = worldObj->getBlockId(var4, var5, var6);
		if(var7 != 0 && Block::blocksList[var7]->m_blockMaterial == var1) {
			float var8 = BlockFluids::func_288_b(worldObj->getBlockMetadata(var4, var5, var6)) - 1.0F / 9.0F;
			float var9 = (float)(var5 + 1) - var8;
			return var2 < (double)var9;
		} else {
			return false;
		}
}

float Entity::func_373_s() {
		return 0.0F;
}

bool Entity::handleLavaMovement() {
		return worldObj->func_689_a(boundingBox.expands(0.0f, (double)-0.4F, 0.0f), Material::s_lava);
}

void Entity::func_351_a(float var1, float var2, float var3) {
		float var4 = sqrtf(var1 * var1 + var2 * var2);
		if(var4 >= 0.01F) {
			if(var4 < 1.0F) {
				var4 = 1.0F;
			}

			var4 = var3 / var4;
			var1 *= var4;
			var2 *= var4;
			float var5 = sinf(rotationYaw * 3.14159265358979323846f / 180.0F);
			float var6 = cosf(rotationYaw * 3.14159265358979323846f / 180.0F);
			motionX += (double)(var1 * var6 - var2 * var5);
			motionZ += (double)(var2 * var6 + var1 * var5);
		}
}

float Entity::getEntityBrightness(float var1) {
		int var2 = (int)floor(posX);
		double var3 = (boundingBox.maxY - boundingBox.minY) * 0.66f;
		int var5 = (int)floor(posY - (double)yOffset + var3);
		int var6 = (int)floor(posZ);
		return worldObj->getLightBrightness(var2, var5, var6);
}

void Entity::setWorld(World* var1) {
		worldObj = var1;
}

void Entity::setPositionAndRotation(double var1, double var3, double var5, float var7, float var8) {
		prevPosX = posX = var1;
		prevPosY = posY = var3;
		prevPosZ = posZ = var5;
		prevRotationYaw = rotationYaw = var7;
		prevRotationPitch = rotationPitch = var8;
		m_field_9287_aY = 0.0F;
		double var9 = (double)(prevRotationYaw - var7);
		if(var9 < -180.0f) {
			prevRotationYaw += 360.0F;
		}

		if(var9 >= 180.0f) {
			prevRotationYaw -= 360.0F;
		}

		setPosition(posX, posY, posZ);
		setRotation(var7, var8);
}

void Entity::setLocationAndAngles(double var1, double var3, double var5, float var7, float var8) {
		prevPosX = posX = var1;
		prevPosY = posY = var3 + (double)yOffset;
		prevPosZ = posZ = var5;
		rotationYaw = var7;
		rotationPitch = var8;
		setPosition(posX, posY, posZ);
}

float Entity::getDistanceToEntity(Entity* var1) {
		float var2 = (float)(posX - var1->posX);
		float var3 = (float)(posY - var1->posY);
		float var4 = (float)(posZ - var1->posZ);
		return sqrtf(var2 * var2 + var3 * var3 + var4 * var4);
}

double Entity::getDistanceSq(double var1, double var3, double var5) {
		double var7 = posX - var1;
		double var9 = posY - var3;
		double var11 = posZ - var5;
		return var7 * var7 + var9 * var9 + var11 * var11;
}

double Entity::getDistance(double var1, double var3, double var5) {
		double var7 = posX - var1;
		double var9 = posY - var3;
		double var11 = posZ - var5;
		return (double)sqrt(var7 * var7 + var9 * var9 + var11 * var11);
}

double Entity::getDistanceSqToEntity(Entity* var1) {
		double var2 = posX - var1->posX;
		double var4 = posY - var1->posY;
		double var6 = posZ - var1->posZ;
		return var2 * var2 + var4 * var4 + var6 * var6;
}

void Entity::onCollideWithPlayer(EntityPlayer* var1) {
	// TODO: implement
}

void Entity::applyEntityCollision(Entity* var1) {
		if(var1->m_riddenByEntity != this && var1->m_ridingEntity != this) {
			double var2 = var1->posX - posX;
			double var4 = var1->posZ - posZ;
			// PORT: converter emitted abs_max(); Java is var2*var2 + var4*var4.
			double var6 = var2 * var2 + var4 * var4;
			if(var6 >= (double)0.01F) {
				var6 = (double)sqrt(var6);
				var2 /= var6;
				var4 /= var6;
				double var8 = 1.0f / var6;
				if(var8 > 1.0f) {
					var8 = 1.0f;
				}

				var2 *= var8;
				var4 *= var8;
				var2 *= (double)0.05F;
				var4 *= (double)0.05F;
				var2 *= (double)(1.0F - m_field_632_aO);
				var4 *= (double)(1.0F - m_field_632_aO);
				addVelocity(-var2, 0.0f, -var4);
				var1->addVelocity(var2, 0.0f, var4);
			}

		}
}

void Entity::addVelocity(double var1, double var3, double var5) {
		motionX += var1;
		motionY += var3;
		motionZ += var5;
}

void Entity::func_9281_M() {
		m_field_9294_aL = true;
}

bool Entity::canAttackEntity(Entity* var1, int var2) {
		func_9281_M();
		return false;
}

bool Entity::canBeCollidedWith() {
		return false;
}

bool Entity::canBePushed() {
		return false;
}

void Entity::addToPlayerScore(Entity* var1, int var2) {
	// TODO: implement
}

bool Entity::func_390_a(Vec3D* var1) {
		double var2 = posX - var1->xCoord;
		double var4 = posY - var1->yCoord;
		double var6 = posZ - var1->zCoord;
		double var8 = var2 * var2 + var4 * var4 + var6 * var6;
		return func_384_a(var8);
}

bool Entity::func_384_a(double var1) {
		double var3 = boundingBox.getAverageEdgeLength();
		var3 *= 64.0f * m_field_619_ac;
		return var1 < var3 * var3;
}

const std::string& Entity::getEntityTexture() {
		// PORT: header returns const std::string&; Java returned null. Use a static empty string.
		static const std::string s_empty("");
		return s_empty;
}

bool Entity::func_358_c(NBTTagCompound* var1) {
		std::string var2 = getEntityString();
		if(!isDead && !var2.empty()) {
			var1->setString("id", var2);
			writeToNBT(var1);
			return true;
		} else {
			return false;
		}
}

void Entity::writeToNBT(NBTTagCompound* var1) {
  double _tmpVerts1[3] = {posX, posY, posZ};
		var1->setTag("Pos", func_375_a(_tmpVerts1, 3));
  double _tmpVerts2[3] = {motionX, motionY, motionZ};
		var1->setTag("Motion", func_375_a(_tmpVerts2, 3));
  float _tmpVerts3[2] = {rotationYaw, rotationPitch};
		var1->setTag("Rotation", func_377_a(_tmpVerts3, 2));
		var1->setFloat("FallDistance", m_fallDistance);
		var1->setShort("Fire", (short)fire);
		var1->setShort("Air", (short)air);
		var1->setBoolean("OnGround", onGround);
		writeEntityToNBT(var1);
}

void Entity::readFromNBT(NBTTagCompound* var1) {
		NBTTagList* var2 = var1->getTagList("Pos");
		NBTTagList* var3 = var1->getTagList("Motion");
		NBTTagList* var4 = var1->getTagList("Rotation");
		setPosition(0.0f, 0.0f, 0.0f);
		motionX = ((NBTTagDouble*)var3->tagAt(0))->m_doubleValue;
		motionY = ((NBTTagDouble*)var3->tagAt(1))->m_doubleValue;
		motionZ = ((NBTTagDouble*)var3->tagAt(2))->m_doubleValue;
		prevPosX = lastTickPosX = posX = ((NBTTagDouble*)var2->tagAt(0))->m_doubleValue;
		prevPosY = lastTickPosY = posY = ((NBTTagDouble*)var2->tagAt(1))->m_doubleValue;
		prevPosZ = lastTickPosZ = posZ = ((NBTTagDouble*)var2->tagAt(2))->m_doubleValue;
		prevRotationYaw = rotationYaw = static_cast<NBTTagFloat*>(var4->tagAt(0))->m_floatValue;
		prevRotationPitch = rotationPitch = static_cast<NBTTagFloat*>(var4->tagAt(1))->m_floatValue;
		m_fallDistance = var1->getFloat("FallDistance");
		fire = var1->getShort("Fire");
		air = var1->getShort("Air");
		onGround = var1->getBoolean("OnGround");
		setPosition(posX, posY, posZ);
		readEntityFromNBT(var1);
}

const std::string& Entity::getEntityString() {
		return EntityList::getEntityString(this);
}

NBTTagList* Entity::func_375_a(double* var1, int var1_count) {
		NBTTagList* var2 = new NBTTagList();
		double* var3 = var1;
		int var4 = var1_count;

		for(int var5 = 0; var5 < var4; ++var5) {
			double var6 = var3[var5];
			var2->setTag(new NBTTagDouble(var6));
		}

		return var2;
}

NBTTagList* Entity::func_377_a(float* var1, int var1_count) {
		NBTTagList* var2 = new NBTTagList();
		float* var3 = var1;
		int var4 = var1_count;

		for(int var5 = 0; var5 < var4; ++var5) {
			float var6 = var3[var5];
			var2->setTag(new NBTTagFloat(var6));
		}

		return var2;
}

float Entity::func_392_h_() {
		return height / 2.0F;
}

EntityItem* Entity::dropItem(int var1, int var2) {
		return dropItemWithOffset(var1, var2, 0.0F);
}

EntityItem* Entity::dropItemWithOffset(int var1, int var2, float var3) {
		EntityItem* var4 = new EntityItem(worldObj, posX, posY + (double)var3, posZ, new ItemStack(var1, var2));
		var4->m_field_805_c = 10;
		worldObj->entityJoinedWorld(var4);
		return var4;
}

bool Entity::isEntityAlive() {
		return !isDead;
}

bool Entity::func_345_I() {
		int var1 = (int)floor(posX);
		int var2 = (int)floor(posY + (double)func_373_s());
		int var3 = (int)floor(posZ);
		return worldObj->isBlockOpaqueCube(var1, var2, var3);
}

bool Entity::interact(EntityPlayer* var1) {
		return false;
}

AxisAlignedBB Entity::func_383_b_(Entity* var1) {
		return AxisAlignedBB();
}

void Entity::func_350_p() {
		if(m_ridingEntity->isDead) {
			m_ridingEntity = NULL;
		} else {
			motionX = 0.0f;
			motionY = 0.0f;
			motionZ = 0.0f;
			onUpdate();
			m_ridingEntity->func_366_i_();
			m_field_667_e += (double)(m_ridingEntity->rotationYaw - m_ridingEntity->prevRotationYaw);

			for(m_minecartType += (double)(m_ridingEntity->rotationPitch - m_ridingEntity->prevRotationPitch); m_field_667_e >= 180.0f; m_field_667_e -= 360.0f) {
			}

			while(m_field_667_e < -180.0f) {
				m_field_667_e += 360.0f;
			}

			while(m_minecartType >= 180.0f) {
				m_minecartType -= 360.0f;
			}

			while(m_minecartType < -180.0f) {
				m_minecartType += 360.0f;
			}

			double var1 = m_field_667_e * 0.5f;
			double var3 = m_minecartType * 0.5f;
			float var5 = 10.0F;
			if(var1 > (double)var5) {
				var1 = (double)var5;
			}

			if(var1 < (double)(-var5)) {
				var1 = (double)(-var5);
			}

			if(var3 > (double)var5) {
				var3 = (double)var5;
			}

			if(var3 < (double)(-var5)) {
				var3 = (double)(-var5);
			}

			m_field_667_e -= var1;
			m_minecartType -= var3;
			rotationYaw = (float)((double)rotationYaw + var1);
			rotationPitch = (float)((double)rotationPitch + var3);
		}
}

void Entity::func_366_i_() {
		m_riddenByEntity->setPosition(posX, posY + func_402_h() + m_riddenByEntity->func_388_v(), posZ);
}

double Entity::func_388_v() {
		return (double)yOffset;
}

double Entity::func_402_h() {
		return (double)height * 0.75f;
}

void Entity::mountEntity(Entity* var1) {
		m_minecartType = 0.0f;
		m_field_667_e = 0.0f;
		if(var1 == NULL) {
			if(m_ridingEntity != NULL) {
				setLocationAndAngles(m_ridingEntity->posX, m_ridingEntity->boundingBox.minY + (double)m_ridingEntity->height, m_ridingEntity->posZ, rotationYaw, rotationPitch);
				m_ridingEntity->m_riddenByEntity = NULL;
			}

			m_ridingEntity = NULL;
		} else if(m_ridingEntity == var1) {
			m_ridingEntity->m_riddenByEntity = NULL;
			m_ridingEntity = NULL;
			setLocationAndAngles(var1->posX, var1->boundingBox.minY + (double)var1->height, var1->posZ, rotationYaw, rotationPitch);
		} else {
			if(m_ridingEntity != NULL) {
				m_ridingEntity->m_riddenByEntity = NULL;
			}

			if(var1->m_riddenByEntity != NULL) {
				var1->m_riddenByEntity->m_ridingEntity = NULL;
			}

			m_ridingEntity = var1;
			var1->m_riddenByEntity = this;
		}
}

void Entity::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
		setPosition(var1, var3, var5);
		setRotation(var7, var8);
}

float Entity::func_4035_j_() {
		return 0.1F;
}

Vec3D* Entity::func_4037_H() {
		return NULL;
}

void Entity::func_4039_q() {
	// TODO: implement
}

void Entity::setVelocity(double var1, double var3, double var5) {
		motionX = var1;
		motionY = var3;
		motionZ = var5;
}

void Entity::func_9282_a(unsigned char var1) {
	// TODO: implement
}

void Entity::func_9280_g() {
	// TODO: implement
}
