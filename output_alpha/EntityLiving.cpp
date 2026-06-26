#include "stdafx.h"
// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
#include "Common.h"
#include "ItemStack.h"
#include "MovingObjectPosition.h"
#include "NBTTagCompound.h"
#include "Vec3D.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "Material.h"
#include "StepSound.h"

EntityLiving::EntityLiving()
	: m_field_9366_o(20)
	, m_field_9365_p(0)
	, m_field_9364_q(0)
	, m_field_9363_r(0)
	, m_field_735_n(0.0f)
	, m_field_734_o(0.0f)
	, m_field_9362_u(0)
	, m_field_9361_v(0)
	, m_field_9360_w(0)
	, m_field_9359_x(0)
	, m_field_9358_y(true)
	, texture("/mob/char.png")
	, m_field_9355_A(true)
	, m_field_9353_B(0.0f)
	, m_field_9351_C("")
	, m_field_9349_D(1.0f)
	, m_field_9347_E(0)
	, m_field_9345_F(0.0f)
	, m_field_9343_G(false)
	, prevSwingProgress(0)
	, swingProgress(0)
	, health(10)
	, m_field_9335_K(0)
	, m_a(0)
	, hurtTime(0)
	, m_field_9332_M(0)
	, m_field_9331_N(0.0f)
	, deathTime(0)
	, attackTime(0)
	, m_field_9329_Q(0)
	, m_field_9328_R(0)
	, m_field_9327_S(false)
	, m_field_9326_T(-1)
	, m_field_9325_U((float)(((float)::rand() / (float)RAND_MAX) * (double)0.9f + (double)0.1f))
	, m_field_705_Q(0)
	, m_field_704_R(0)
	, m_field_703_S(0)
	, m_field_9324_Y(0)
	, m_field_9323_Z(0)
	, m_field_9356_aa(0)
	, m_field_9354_ab(0)
	, m_field_9352_ac(0)
	, m_field_9350_ad(0)
	, m_field_9348_ae(0.0f)
	, m_field_9346_af(0)
	, m_field_9344_ag(0)
	, m_field_9342_ah(0)
	, m_field_9340_ai(0)
	, m_field_9338_aj(0)
	, isJumping(false)
	, m_field_9334_al(0.0f)
	, m_field_9333_am(0.7f)
	, m_b(NULL)
	, m_c(0)
{
}

EntityLiving::EntityLiving(World* var1)
	: Entity(var1)
	, m_field_9366_o(20)
	, m_field_9365_p(0)
	, m_field_9364_q(0)
	, m_field_9363_r(0)
	, m_field_735_n(0.0f)
	, m_field_734_o(0.0f)
	, m_field_9362_u(0)
	, m_field_9361_v(0)
	, m_field_9360_w(0)
	, m_field_9359_x(0)
	, m_field_9358_y(true)
	, texture("/mob/char.png")
	, m_field_9355_A(true)
	, m_field_9353_B(0.0f)
	, m_field_9351_C("")
	, m_field_9349_D(1.0f)
	, m_field_9347_E(0)
	, m_field_9345_F(0.0f)
	, m_field_9343_G(false)
	, prevSwingProgress(0)
	, swingProgress(0)
	, health(10)
	, m_field_9335_K(0)
	, m_a(0)
	, hurtTime(0)
	, m_field_9332_M(0)
	, m_field_9331_N(0.0f)
	, deathTime(0)
	, attackTime(0)
	, m_field_9329_Q(0)
	, m_field_9328_R(0)
	, m_field_9327_S(false)
	, m_field_9326_T(-1)
	, m_field_9325_U((float)(((float)::rand() / (float)RAND_MAX) * (double)0.9f + (double)0.1f))
	, m_field_705_Q(0)
	, m_field_704_R(0)
	, m_field_703_S(0)
	, m_field_9324_Y(0)
	, m_field_9323_Z(0)
	, m_field_9356_aa(0)
	, m_field_9354_ab(0)
	, m_field_9352_ac(0)
	, m_field_9350_ad(0)
	, m_field_9348_ae(0.0f)
	, m_field_9346_af(0)
	, m_field_9344_ag(0)
	, m_field_9342_ah(0)
	, m_field_9340_ai(0)
	, m_field_9338_aj(0)
	, isJumping(false)
	, m_field_9334_al(0.0f)
	, m_field_9333_am(0.7f)
	, m_b(NULL)
	, m_c(0)
{
		m_field_618_ad = true;
		m_field_9363_r = (float)(((float)::rand() / (float)RAND_MAX) + 1.0f) * 0.01F;
		setPosition(posX, posY, posZ);
		m_field_9365_p = (float)((float)::rand() / (float)RAND_MAX) * 12398.0F;
		rotationYaw = (float)(((float)::rand() / (float)RAND_MAX) * (double)(3.14159265358979323846f) * 2.0f);
		m_field_9364_q = 1.0F;
		m_field_9286_aZ = 0.5F;
}

EntityLiving::~EntityLiving() {
	m_b = NULL;
}

bool EntityLiving::canEntityBeSeen(Entity* var1) {
		return worldObj->rayTraceBlocks(Vec3D::createVector(posX, posY + (double)func_373_s(), posZ), Vec3D::createVector(var1->posX, var1->posY + (double)var1->func_373_s(), var1->posZ)) == NULL;
}

const std::string& EntityLiving::getEntityTexture() {
		return texture;
}

bool EntityLiving::canBeCollidedWith() {
		return !isDead;
}

bool EntityLiving::canBePushed() {
		return !isDead;
}

float EntityLiving::func_373_s() {
		return height * 0.85F;
}

int EntityLiving::func_421_b() {
		return 80;
}

void EntityLiving::func_391_y() {
		prevSwingProgress = swingProgress;
		Entity::func_391_y();
		if(rand->nextInt(1000) < m_a++) {
			m_a = -func_421_b();
			std::string var1 = getLivingSound();
			if(!var1.empty()) {
				worldObj->playSoundAtEntity(this, var1, getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
			}
		}

		if(isEntityAlive() && func_345_I()) {
			canAttackEntity(NULL, 1);
		}

		if(m_isImmuneToFire || worldObj->m_multiplayerWorld) {
			fire = 0;
		}

		int var8;
		if(isEntityAlive() && isInsideOfMaterial(Material::s_water)) {
			--air;
			if(air == -20) {
				air = 0;

				for(var8 = 0; var8 < 8; ++var8) {
					float var2 = rand->nextFloat() - rand->nextFloat();
					float var3 = rand->nextFloat() - rand->nextFloat();
					float var4 = rand->nextFloat() - rand->nextFloat();
					worldObj->spawnParticle("bubble", posX + (double)var2, posY + (double)var3, posZ + (double)var4, motionX, motionY, motionZ);
				}

				canAttackEntity(NULL, 2);
			}

			fire = 0;
		} else {
			air = m_field_9308_bh;
		}

		m_field_9329_Q = m_field_9328_R;
		if(attackTime > 0) {
			--attackTime;
		}

		if(hurtTime > 0) {
			--hurtTime;
		}

		if(m_field_9306_bj > 0) {
			--m_field_9306_bj;
		}

		if(health <= 0) {
			++deathTime;
			if(deathTime > 20) {
				func_6392_F();
				setEntityDead();

				for(var8 = 0; var8 < 20; ++var8) {
					double var9 = rand->nextGaussian() * 0.02f;
					double var10 = rand->nextGaussian() * 0.02f;
					double var6 = rand->nextGaussian() * 0.02f;
					worldObj->spawnParticle("explode", posX + (double)(rand->nextFloat() * width * 2.0F) - (double)width, posY + (double)(rand->nextFloat() * height), posZ + (double)(rand->nextFloat() * width * 2.0F) - (double)width, var9, var10, var6);
				}
			}
		}

		m_field_9359_x = m_field_9360_w;
		m_field_734_o = m_field_735_n;
		prevRotationYaw = rotationYaw;
		prevRotationPitch = rotationPitch;
}

void EntityLiving::spawnExplosionParticle() {
		for(int var1 = 0; var1 < 20; ++var1) {
			double var2 = rand->nextGaussian() * 0.02f;
			double var4 = rand->nextGaussian() * 0.02f;
			double var6 = rand->nextGaussian() * 0.02f;
			double var8 = 10.0f;
			worldObj->spawnParticle("explode", posX + (double)(rand->nextFloat() * width * 2.0F) - (double)width - var2 * var8, posY + (double)(rand->nextFloat() * height) - var4 * var8, posZ + (double)(rand->nextFloat() * width * 2.0F) - (double)width - var6 * var8, var2, var4, var6);
		}

}

void EntityLiving::func_350_p() {
		Entity::func_350_p();
		m_field_9362_u = m_field_9361_v;
		m_field_9361_v = 0.0F;
}

void EntityLiving::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
		yOffset = 0.0F;
		m_field_9323_Z = var1;
		m_field_9356_aa = var3;
		m_field_9354_ab = var5;
		m_field_9352_ac = (double)var7;
		m_field_9350_ad = (double)var8;
		m_field_9324_Y = var9;
}

void EntityLiving::onUpdate() {
		Entity::onUpdate();
		onLivingUpdate();
		double var1 = posX - prevPosX;
		double var3 = posZ - prevPosZ;
		float var5 = sqrt(var1 * var1 + var3 * var3);
		float var6 = m_field_735_n;
		float var7 = 0.0F;
		m_field_9362_u = m_field_9361_v;
		float var8 = 0.0F;
		if(var5 > 0.05F) {
			var8 = 1.0F;
			var7 = var5 * 3.0F;
			var6 = (float)atan2f(var3, var1) * 180.0F / 3.14159265358979323846f - 90.0F;
		}

		if(swingProgress > 0.0F) {
			var6 = rotationYaw;
		}

		if(!onGround) {
			var8 = 0.0F;
		}

		m_field_9361_v += (var8 - m_field_9361_v) * 0.3F;

		float var9;
		for(var9 = var6 - m_field_735_n; var9 < -180.0F; var9 += 360.0F) {
		}

		while(var9 >= 180.0F) {
			var9 -= 360.0F;
		}

		m_field_735_n += var9 * 0.3F;

		float var10;
		for(var10 = rotationYaw - m_field_735_n; var10 < -180.0F; var10 += 360.0F) {
		}

		while(var10 >= 180.0F) {
			var10 -= 360.0F;
		}

		bool var11 = var10 < -90.0F || var10 >= 90.0F;
		if(var10 < -75.0F) {
			var10 = -75.0F;
		}

		if(var10 >= 75.0F) {
			var10 = 75.0F;
		}

		m_field_735_n = rotationYaw - var10;
		if(var10 * var10 > 2500.0F) {
			m_field_735_n += var10 * 0.2F;
		}

		if(var11) {
			var7 *= -1.0F;
		}

		while(rotationYaw - prevRotationYaw < -180.0F) {
			prevRotationYaw -= 360.0F;
		}

		while(rotationYaw - prevRotationYaw >= 180.0F) {
			prevRotationYaw += 360.0F;
		}

		while(m_field_735_n - m_field_734_o < -180.0F) {
			m_field_734_o -= 360.0F;
		}

		while(m_field_735_n - m_field_734_o >= 180.0F) {
			m_field_734_o += 360.0F;
		}

		while(rotationPitch - prevRotationPitch < -180.0F) {
			prevRotationPitch -= 360.0F;
		}

		while(rotationPitch - prevRotationPitch >= 180.0F) {
			prevRotationPitch += 360.0F;
		}

		m_field_9360_w += var7;
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

			m_field_9306_bj = m_field_9366_o / 2;
		}
}

bool EntityLiving::canAttackEntity(Entity* var1, int var2) {
		if(worldObj->m_multiplayerWorld) {
			return false;
		} else {
			m_field_9344_ag = 0;
			if(health <= 0) {
				return false;
			} else {
				m_field_704_R = 1.5F;
				bool var3 = true;
				if((float)m_field_9306_bj > (float)m_field_9366_o / 2.0F) {
					if(var2 <= m_field_9346_af) {
						return false;
					}

					damageEntity(var2 - m_field_9346_af);
					m_field_9346_af = var2;
					var3 = false;
				} else {
					m_field_9346_af = var2;
					m_field_9335_K = health;
					m_field_9306_bj = m_field_9366_o;
					damageEntity(var2);
					hurtTime = m_field_9332_M = 10;
				}

				m_field_9331_N = 0.0F;
				if(var3) {
					worldObj->func_9425_a(this, (unsigned char)2);
					func_9281_M();
					if(var1 != NULL) {
						double var4 = var1->posX - posX;

						double var6;
						for(var6 = var1->posZ - posZ; var4 * var4 + var6 * var6 < 1.0E-4f; var6 = (((float)::rand() / (float)RAND_MAX) - ((float)::rand() / (float)RAND_MAX)) * 0.01f) {
							var4 = (((float)::rand() / (float)RAND_MAX) - ((float)::rand() / (float)RAND_MAX)) * 0.01f;
						}

						m_field_9331_N = (float)(atan2f(var6, var4) * 180.0f / (double)(3.14159265358979323846f)) - rotationYaw;
						func_434_a(var1, var2, var4, var6);
					} else {
						m_field_9331_N = (float)((int)(((float)::rand() / (float)RAND_MAX) * 2.0f) * 180);
					}
				}

				if(health <= 0) {
					if(var3) {
						worldObj->playSoundAtEntity(this, getDeathSound(), getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
					}

					onDeath(var1);
				} else if(var3) {
					worldObj->playSoundAtEntity(this, getHurtSound(), getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
				}

				return true;
			}
		}
}

void EntityLiving::func_9280_g() {
		hurtTime = m_field_9332_M = 10;
		m_field_9331_N = 0.0F;
}

void EntityLiving::damageEntity(int var1) {
		health -= var1;
}

float EntityLiving::getSoundVolume() {
		return 1.0F;
}

const std::string& EntityLiving::getLivingSound() {
		// PORT: header returns const std::string&; Java returned null here. Use a
		// static empty string so callers get a valid (empty) reference.
		static const std::string s_empty("");
		return s_empty;
}

const std::string& EntityLiving::getHurtSound() {
		// PORT: return a reference to a static so we don't dangle a temporary.
		static const std::string s_hurt("random.hurt");
		return s_hurt;
}

const std::string& EntityLiving::getDeathSound() {
		static const std::string s_death("random.hurt");
		return s_death;
}

void EntityLiving::func_434_a(Entity* var1, int var2, double var3, double var5) {
		float var7 = sqrt(var3 * var3 + var5 * var5);
		float var8 = 0.4F;
		motionX /= 2.0f;
		motionY /= 2.0f;
		motionZ /= 2.0f;
		motionX -= var3 / (double)var7 * (double)var8;
		motionY += (double)0.4F;
		motionZ -= var5 / (double)var7 * (double)var8;
		if(motionY > (double)0.4F) {
			motionY = (double)0.4F;
		}

}

void EntityLiving::onDeath(Entity* var1) {
		if(m_field_9347_E > 0 && var1 != NULL) {
			var1->addToPlayerScore(this, m_field_9347_E);
		}

		m_field_9327_S = true;
		if(!worldObj->m_multiplayerWorld) {
			int var2 = getDropItemId();
			if(var2 > 0) {
				int var3 = rand->nextInt(3);

				for(int var4 = 0; var4 < var3; ++var4) {
					dropItem(var2, 1);
				}
			}
		}

		worldObj->func_9425_a(this, (unsigned char)3);
}

int EntityLiving::getDropItemId() {
		return 0;
}

void EntityLiving::fall(float var1) {
		int var2 = (int)ceilf((double)(var1 - 3.0F));
		if(var2 > 0) {
			canAttackEntity(NULL, var2);
			int var3 = worldObj->getBlockId((int)floor(posX), (int)floor(posY - (double)0.2F - (double)yOffset), (int)floor(posZ));
			if(var3 > 0) {
				StepSound* var4 = Block::blocksList[var3]->stepSound;
				worldObj->playSoundAtEntity(this, var4->func_1145_d(), var4->func_1147_b() * 0.5F, var4->func_1144_c() * (12.0F / 16.0F));
			}
		}

}

void EntityLiving::func_435_b(float var1, float var2) {
		double var3;
		if(handleWaterMovement()) {
			var3 = posY;
			func_351_a(var1, var2, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= (double)0.8F;
			motionY *= (double)0.8F;
			motionZ *= (double)0.8F;
			motionY -= 0.02f;
			if(m_field_9297_aI && func_403_b(motionX, motionY + (double)0.6F - posY + var3, motionZ)) {
				motionY = (double)0.3F;
			}
		} else if(handleLavaMovement()) {
			var3 = posY;
			func_351_a(var1, var2, 0.02F);
			moveEntity(motionX, motionY, motionZ);
			motionX *= 0.5f;
			motionY *= 0.5f;
			motionZ *= 0.5f;
			motionY -= 0.02f;
			if(m_field_9297_aI && func_403_b(motionX, motionY + (double)0.6F - posY + var3, motionZ)) {
				motionY = (double)0.3F;
			}
		} else {
			float var8 = 0.91F;
			if(onGround) {
				var8 = 546.0F * 0.1F * 0.1F * 0.1F;
				int var4 = worldObj->getBlockId((int)floor(posX), (int)floor(boundingBox.minY) - 1, (int)floor(posZ));
				if(var4 > 0) {
					var8 = Block::blocksList[var4]->slipperiness * 0.91F;
				}
			}

			float var9 = 0.16277136F / (var8 * var8 * var8);
			func_351_a(var1, var2, onGround ? 0.1F * var9 : 0.02F);
			var8 = 0.91F;
			if(onGround) {
				var8 = 546.0F * 0.1F * 0.1F * 0.1F;
				int var5 = worldObj->getBlockId((int)floor(posX), (int)floor(boundingBox.minY) - 1, (int)floor(posZ));
				if(var5 > 0) {
					var8 = Block::blocksList[var5]->slipperiness * 0.91F;
				}
			}

			if(isOnLadder()) {
				m_fallDistance = 0.0F;
				if(motionY < -0.15f) {
					motionY = -0.15f;
				}
			}

			moveEntity(motionX, motionY, motionZ);
			if(m_field_9297_aI && isOnLadder()) {
				motionY = 0.2f;
			}

			motionY -= 0.08f;
			motionY *= (double)0.98F;
			motionX *= (double)var8;
			motionZ *= (double)var8;
		}

		m_field_705_Q = m_field_704_R;
		var3 = posX - prevPosX;
		double var10 = posZ - prevPosZ;
		float var7 = sqrt(var3 * var3 + var10 * var10) * 4.0F;
		if(var7 > 1.0F) {
			var7 = 1.0F;
		}

		m_field_704_R += (var7 - m_field_704_R) * 0.4F;
		m_field_703_S += m_field_704_R;
}

bool EntityLiving::isOnLadder() {
		int var1 = (int)floor(posX);
		int var2 = (int)floor(boundingBox.minY);
		int var3 = (int)floor(posZ);
		return worldObj->getBlockId(var1, var2, var3) == Block::s_ladder->blockID || worldObj->getBlockId(var1, var2 + 1, var3) == Block::s_ladder->blockID;
}

void EntityLiving::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setShort("Health", (short)health);
		var1->setShort("HurtTime", (short)hurtTime);
		var1->setShort("DeathTime", (short)deathTime);
		var1->setShort("AttackTime", (short)attackTime);
}

void EntityLiving::readEntityFromNBT(NBTTagCompound* var1) {
		health = var1->getShort("Health");
		if(!var1->hasKey("Health")) {
			health = 10;
		}

		hurtTime = var1->getShort("HurtTime");
		deathTime = var1->getShort("DeathTime");
		attackTime = var1->getShort("AttackTime");
}

bool EntityLiving::isEntityAlive() {
		return !isDead && health > 0;
}

void EntityLiving::onLivingUpdate() {
		if(m_field_9324_Y > 0) {
			double var1 = posX + (m_field_9323_Z - posX) / (double)m_field_9324_Y;
			double var3 = posY + (m_field_9356_aa - posY) / (double)m_field_9324_Y;
			double var5 = posZ + (m_field_9354_ab - posZ) / (double)m_field_9324_Y;

			double var7;
			for(var7 = m_field_9352_ac - (double)rotationYaw; var7 < -180.0f; var7 += 360.0f) {
			}

			while(var7 >= 180.0f) {
				var7 -= 360.0f;
			}

			rotationYaw = (float)((double)rotationYaw + var7 / (double)m_field_9324_Y);
			rotationPitch = (float)((double)rotationPitch + (m_field_9350_ad - (double)rotationPitch) / (double)m_field_9324_Y);
			--m_field_9324_Y;
			setPosition(var1, var3, var5);
			setRotation(rotationYaw, rotationPitch);
		}

		if(health <= 0) {
			isJumping = false;
			m_field_9342_ah = 0.0F;
			m_field_9340_ai = 0.0F;
			m_field_9338_aj = 0.0F;
		} else if(!m_field_9343_G) {
			func_418_b_();
		}

		bool var9 = handleWaterMovement();
		bool var2 = handleLavaMovement();
		if(isJumping) {
			if(var9) {
				motionY += (double)0.04F;
			} else if(var2) {
				motionY += (double)0.04F;
			} else if(onGround) {
				func_424_C();
			}
		}

		m_field_9342_ah *= 0.98F;
		m_field_9340_ai *= 0.98F;
		m_field_9338_aj *= 0.9F;
		func_435_b(m_field_9342_ah, m_field_9340_ai);
		std::vector<void*> var10 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.expands((double)0.2F, 0.0f, (double)0.2F));
		if(true && var10.size() > 0) {
			for(size_t var4 = 0; var4 < var10.size(); ++var4) {
				Entity* var11 = (Entity*)var10[var4];
				if(var11->canBePushed()) {
					var11->applyEntityCollision(this);
				}
			}
		}

}

void EntityLiving::func_424_C() {
		motionY = (double)0.42F;
}

void EntityLiving::func_418_b_() {
		++m_field_9344_ag;
		EntityPlayer* var1 = worldObj->getClosestPlayerToEntity(this, -1.0f);
		if(var1 != NULL) {
			double var2 = var1->posX - posX;
			double var4 = var1->posY - posY;
			double var6 = var1->posZ - posZ;
			double var8 = var2 * var2 + var4 * var4 + var6 * var6;
			if(var8 > 16384.0f) {
				setEntityDead();
			}

			if(m_field_9344_ag > 600 && rand->nextInt(800) == 0) {
				if(var8 < 1024.0f) {
					m_field_9344_ag = 0;
				} else {
					setEntityDead();
				}
			}
		}

		m_field_9342_ah = 0.0F;
		m_field_9340_ai = 0.0F;
		float var10 = 8.0F;
		if(rand->nextFloat() < 0.02F) {
			var1 = worldObj->getClosestPlayerToEntity(this, (double)var10);
			if(var1 != NULL) {
				m_b = var1;
				m_c = 10 + rand->nextInt(20);
			} else {
				m_field_9338_aj = (rand->nextFloat() - 0.5F) * 20.0F;
			}
		}

		if(m_b != NULL) {
			faceEntity(m_b, 10.0F);
			if(m_c-- <= 0 || m_b->isDead || m_b->getDistanceSqToEntity(this) > (double)(var10 * var10)) {
				m_b = NULL;
			}
		} else {
			if(rand->nextFloat() < 0.05F) {
				m_field_9338_aj = (rand->nextFloat() - 0.5F) * 20.0F;
			}

			rotationYaw += m_field_9338_aj;
			rotationPitch = m_field_9334_al;
		}

		bool var3 = handleWaterMovement();
		bool var11 = handleLavaMovement();
		if(var3 || var11) {
			isJumping = rand->nextFloat() < 0.8F;
		}

}

void EntityLiving::faceEntity(Entity* var1, float var2) {
		double var3 = var1->posX - posX;
		double var7 = var1->posZ - posZ;
		double var5;
		if(dynamic_cast<EntityLiving*>(var1) != NULL) {
			EntityLiving* var9 = (EntityLiving*)var1;
			var5 = var9->posY + (double)var9->func_373_s() - (posY + (double)func_373_s());
		} else {
			var5 = (var1->boundingBox.minY + var1->boundingBox.maxY) / 2.0f - (posY + (double)func_373_s());
		}

		double var13 = (double)sqrt(var3 * var3 + var7 * var7);
		float var11 = (float)(atan2f(var7, var3) * 180.0f / (double)(3.14159265358979323846f)) - 90.0F;
		float var12 = (float)(atan2f(var5, var13) * 180.0f / (double)(3.14159265358979323846f));
		rotationPitch = -updateRotation(rotationPitch, var12, var2);
		rotationYaw = updateRotation(rotationYaw, var11, var2);
}

float EntityLiving::updateRotation(float var1, float var2, float var3) {
		float var4;
		for(var4 = var2 - var1; var4 < -180.0F; var4 += 360.0F) {
		}

		while(var4 >= 180.0F) {
			var4 -= 360.0F;
		}

		if(var4 > var3) {
			var4 = var3;
		}

		if(var4 < -var3) {
			var4 = -var3;
		}

		return var1 + var4;
}

void EntityLiving::func_6392_F() {
	// TODO: implement
}

bool EntityLiving::getCanSpawnHere() {
		return worldObj->checkIfAABBIsClear(boundingBox) && worldObj->getCollidingBoundingBoxes(this, boundingBox).size() == 0 && !worldObj->getIsAnyLiquid(boundingBox);
}

void EntityLiving::func_4034_G() {
		canAttackEntity(NULL, 4);
}

float EntityLiving::getSwingProgress(float var1) {
		float var2 = swingProgress - prevSwingProgress;
		if(var2 < 0.0F) {
			++var2;
		}

		return prevSwingProgress + var2 * var1;
}

Vec3D* EntityLiving::getPosition(float var1) {
		if(var1 == 1.0F) {
			return Vec3D::createVector(posX, posY, posZ);
		} else {
			double var2 = prevPosX + (posX - prevPosX) * (double)var1;
			double var4 = prevPosY + (posY - prevPosY) * (double)var1;
			double var6 = prevPosZ + (posZ - prevPosZ) * (double)var1;
			return Vec3D::createVector(var2, var4, var6);
		}
}

Vec3D* EntityLiving::func_4037_H() {
		return getLook(1.0F);
}

Vec3D* EntityLiving::getLook(float var1) {
		float var2;
		float var3;
		float var4;
		float var5;
		if(var1 == 1.0F) {
			var2 = cosf(-rotationYaw * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
			var3 = sinf(-rotationYaw * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
			var4 = -cosf(-rotationPitch * (3.14159265358979323846f / 180.0F));
			var5 = sinf(-rotationPitch * (3.14159265358979323846f / 180.0F));
			return Vec3D::createVector((double)(var3 * var4), (double)var5, (double)(var2 * var4));
		} else {
			var2 = prevRotationPitch + (rotationPitch - prevRotationPitch) * var1;
			var3 = prevRotationYaw + (rotationYaw - prevRotationYaw) * var1;
			var4 = cosf(-var3 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
			var5 = sinf(-var3 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
			float var6 = -cosf(-var2 * (3.14159265358979323846f / 180.0F));
			float var7 = sinf(-var2 * (3.14159265358979323846f / 180.0F));
			return Vec3D::createVector((double)(var5 * var6), (double)var7, (double)(var4 * var6));
		}
}

MovingObjectPosition* EntityLiving::rayTrace(double var1, float var3) {
		Vec3D* var4 = getPosition(var3);
		Vec3D* var5 = getLook(var3);
		Vec3D* var6 = var4->addVector(var5->xCoord * var1, var5->yCoord * var1, var5->zCoord * var1);
		return worldObj->rayTraceBlocks(var4, var6);
}

int EntityLiving::func_6391_i() {
		return 4;
}

ItemStack* EntityLiving::getHeldItem() {
		return NULL;
}

void EntityLiving::func_9282_a(unsigned char var1) {
		if(var1 == 2) {
			m_field_704_R = 1.5F;
			m_field_9306_bj = m_field_9366_o;
			hurtTime = m_field_9332_M = 10;
			m_field_9331_N = 0.0F;
			worldObj->playSoundAtEntity(this, getHurtSound(), getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
			canAttackEntity(NULL, 0);
		} else if(var1 == 3) {
			worldObj->playSoundAtEntity(this, getDeathSound(), getSoundVolume(), (rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F);
			health = 0;
			onDeath(NULL);
		} else {
			Entity::func_9282_a(var1);
		}

}
