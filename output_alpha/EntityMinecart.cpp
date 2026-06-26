#include "stdafx.h"
// Hand-ported from EntityMinecart.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9  (LOGIC ONLY, no GL)
//
// Started from ConversionTool/output_alpha/EntityMinecart.cpp and fixed the
// converter breakages: the 3D rail-direction static table, Java int[][] locals,
// pointer '.' vs '->', the (EntityMinecart)var1.d cast, the std::string& return
// of getInvName, the bogus constructor-delegation glue, and the InventoryPlayer
// incomplete type.

#include "EntityMinecart.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"
#include "Entity.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "Item.h"
#include "Random.h"
#include "MathHelper.h"
#include <string>
#include <vector>
#include <math.h>

// ---------------------------------------------------------------------------
// Rail direction table.  Java:
//   private static final int[][][] field_855_j =
//       {{{0,0,-1},{0,0,1}}, {{-1,0,0},{1,0,0}}, ...10 entries... };
// Each entry is two {dx,dy,dz} triples describing the two ends of a rail
// shape (indexed by track metadata).  We back it with a flat data array and a
// table of int* / int** pointers so the legacy access pattern
//   int** var11 = s_field_855_j[meta];   var11[0][0] ...
// works unchanged.
// ---------------------------------------------------------------------------
static int s_railData[10][2][3] = {
	{ {0, 0, -1}, {0, 0, 1} },
	{ {-1, 0, 0}, {1, 0, 0} },
	{ {-1, -1, 0}, {1, 0, 0} },
	{ {-1, 0, 0}, {1, -1, 0} },
	{ {0, 0, -1}, {0, -1, 1} },
	{ {0, -1, -1}, {0, 0, 1} },
	{ {0, 0, 1}, {1, 0, 0} },
	{ {0, 0, 1}, {-1, 0, 0} },
	{ {0, 0, -1}, {-1, 0, 0} },
	{ {0, 0, -1}, {1, 0, 0} }
};
static int* s_railRows[10][2];   // [meta][end] -> int[3]
static int** s_railTable[10];    // [meta]      -> int*[2]

static int** s_buildRailTable() {
	for(int i = 0; i < 10; ++i) {
		s_railRows[i][0] = s_railData[i][0];
		s_railRows[i][1] = s_railData[i][1];
		s_railTable[i] = s_railRows[i];
	}
	return s_railTable[0]; // dummy; real init via the function side effects
}
// Force the table to be populated before first use.
static int** s_railTableInit = s_buildRailTable();

// EntityMinecart::s_field_855_j is declared int*** in the header; point it at
// our populated lookup table.
int*** EntityMinecart::s_field_855_j = s_railTable;

// Shared init used by the 1-arg and (via delegation) the 4-arg constructor.
static void minecart_initFields(EntityMinecart* self) {
	self->m_cargoItems = new ItemStack*[36];
	for(int i = 0; i < 36; ++i) {
		self->m_cargoItems[i] = NULL;
	}
	self->m_cargoItems_count = 36;
	self->m_a = 0;
	self->m_b = 0;
	self->m_c = 1;
	self->m_field_856_i = false;
	self->m_field_618_ad = true;
	self->setSize(0.98F, 0.7F);
	self->yOffset = self->height / 2.0F;
	self->m_entityWalks = false;
}

EntityMinecart::EntityMinecart()
	: m_cargoItems(NULL)
	, m_a(0)
	, m_b(0)
	, m_c(0)
	, m_d(0)
	, m_fuel(0)
	, m_pushX(0)
	, m_pushZ(0)
	, m_field_856_i(false)
	, m_field_9415_k(0)
	, m_field_9414_l(0)
	, m_field_9413_m(0)
	, m_field_9412_n(0)
	, m_field_9411_o(0)
	, m_field_9410_p(0)
	, m_field_9409_q(0)
	, m_field_9408_r(0)
	, m_field_9407_s(0)
	, m_cargoItems_count(0)
{
}

EntityMinecart::EntityMinecart(World* var1)
	: Entity(var1)
	, m_cargoItems(NULL)
	, m_a(0)
	, m_b(0)
	, m_c(0)
	, m_d(0)
	, m_fuel(0)
	, m_pushX(0)
	, m_pushZ(0)
	, m_field_856_i(false)
	, m_field_9415_k(0)
	, m_field_9414_l(0)
	, m_field_9413_m(0)
	, m_field_9412_n(0)
	, m_field_9411_o(0)
	, m_field_9410_p(0)
	, m_field_9409_q(0)
	, m_field_9408_r(0)
	, m_field_9407_s(0)
	, m_cargoItems_count(0)
{
	minecart_initFields(this);
}

EntityMinecart::EntityMinecart(World* var1, double var2, double var4, double var6, int var8)
	: Entity(var1)
	, m_cargoItems(NULL)
	, m_a(0)
	, m_b(0)
	, m_c(0)
	, m_d(0)
	, m_fuel(0)
	, m_pushX(0)
	, m_pushZ(0)
	, m_field_856_i(false)
	, m_field_9415_k(0)
	, m_field_9414_l(0)
	, m_field_9413_m(0)
	, m_field_9412_n(0)
	, m_field_9411_o(0)
	, m_field_9410_p(0)
	, m_field_9409_q(0)
	, m_field_9408_r(0)
	, m_field_9407_s(0)
	, m_cargoItems_count(0)
{
	// Java: this(var1); -> run the shared World* init first.
	minecart_initFields(this);
	setPosition(var2, var4 + (double)yOffset, var6);
	motionX = 0.0;
	motionY = 0.0;
	motionZ = 0.0;
	prevPosX = var2;
	prevPosY = var4;
	prevPosZ = var6;
	m_d = var8;
}

EntityMinecart::~EntityMinecart() {
	delete[] m_cargoItems;
	m_cargoItems = NULL;
}

AxisAlignedBB EntityMinecart::func_383_b_(Entity* var1) {
	return var1->boundingBox;
}

AxisAlignedBB EntityMinecart::func_372_f_() {
	// Java returns null; the AABB-by-value interface has no null, so return an
	// empty box (callers of func_372_f_ treat a zero box as "no collision box").
	return AxisAlignedBB();
}

bool EntityMinecart::canBePushed() {
	return true;
}

double EntityMinecart::func_402_h() {
	return (double)height * 0.0 - (double)0.3F;
}

bool EntityMinecart::canAttackEntity(Entity* var1, int var2) {
	if(!worldObj->m_multiplayerWorld && !isDead) {
		m_c = -m_c;
		m_b = 10;
		func_9281_M();
		m_a += var2 * 10;
		if(m_a > 40) {
			dropItemWithOffset(Item::s_minecartEmpty->shiftedIndex, 1, 0.0F);
			if(m_d == 1) {
				dropItemWithOffset(Block::s_crate->blockID, 1, 0.0F);
			} else if(m_d == 2) {
				dropItemWithOffset(Block::s_stoneOvenIdle->blockID, 1, 0.0F);
			}

			setEntityDead();
		}

		return true;
	} else {
		return true;
	}
}

void EntityMinecart::func_9280_g() {
	DbgPrintF("%s\n", "Animating hurt");
	m_c = -m_c;
	m_b = 10;
	m_a += m_a * 10;
}

bool EntityMinecart::canBeCollidedWith() {
	return !isDead;
}

void EntityMinecart::setEntityDead() {
	for(int var1 = 0; var1 < getSizeInventory(); ++var1) {
		ItemStack* var2 = getStackInSlot(var1);
		if(var2 != NULL) {
			float var3 = rand->nextFloat() * 0.8F + 0.1F;
			float var4 = rand->nextFloat() * 0.8F + 0.1F;
			float var5 = rand->nextFloat() * 0.8F + 0.1F;

			while(var2->stackSize > 0) {
				int var6 = rand->nextInt(21) + 10;
				if(var6 > var2->stackSize) {
					var6 = var2->stackSize;
				}

				var2->stackSize -= var6;
				EntityItem* var7 = new EntityItem(worldObj, posX + (double)var3, posY + (double)var4, posZ + (double)var5, new ItemStack(var2->itemID, var6, var2->itemDamage));
				float var8 = 0.05F;
				var7->motionX = (double)((float)rand->nextGaussian() * var8);
				var7->motionY = (double)((float)rand->nextGaussian() * var8 + 0.2F);
				var7->motionZ = (double)((float)rand->nextGaussian() * var8);
				worldObj->entityJoinedWorld(var7);
			}
		}
	}

	Entity::setEntityDead();
}

void EntityMinecart::onUpdate() {
	if(m_b > 0) {
		--m_b;
	}

	if(m_a > 0) {
		--m_a;
	}

	double var7;
	if(worldObj->m_multiplayerWorld && m_field_9415_k > 0) {
		if(m_field_9415_k > 0) {
			double var41 = posX + (m_field_9414_l - posX) / (double)m_field_9415_k;
			double var42 = posY + (m_field_9413_m - posY) / (double)m_field_9415_k;
			double var5 = posZ + (m_field_9412_n - posZ) / (double)m_field_9415_k;

			for(var7 = m_field_9411_o - (double)rotationYaw; var7 < -180.0; var7 += 360.0) {
			}

			while(var7 >= 180.0) {
				var7 -= 360.0;
			}

			rotationYaw = (float)((double)rotationYaw + var7 / (double)m_field_9415_k);
			rotationPitch = (float)((double)rotationPitch + (m_field_9410_p - (double)rotationPitch) / (double)m_field_9415_k);
			--m_field_9415_k;
			setPosition(var41, var42, var5);
			setRotation(rotationYaw, rotationPitch);
		} else {
			setPosition(posX, posY, posZ);
			setRotation(rotationYaw, rotationPitch);
		}

	} else {
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		motionY -= (double)0.04F;
		int var1 = MathHelper::floor_double(posX);
		int var2 = MathHelper::floor_double(posY);
		int var3 = MathHelper::floor_double(posZ);
		if(worldObj->getBlockId(var1, var2 - 1, var3) == Block::s_minecartTrack->blockID) {
			--var2;
		}

		double var4 = 0.4;
		bool var6 = false;
		var7 = 1.0 / 128.0;
		if(worldObj->getBlockId(var1, var2, var3) == Block::s_minecartTrack->blockID) {
			Vec3D* var9 = func_514_g(posX, posY, posZ);
			int var10 = worldObj->getBlockMetadata(var1, var2, var3);
			posY = (double)var2;
			if(var10 >= 2 && var10 <= 5) {
				posY = (double)(var2 + 1);
			}

			if(var10 == 2) {
				motionX -= var7;
			}

			if(var10 == 3) {
				motionX += var7;
			}

			if(var10 == 4) {
				motionZ += var7;
			}

			if(var10 == 5) {
				motionZ -= var7;
			}

			int** var11 = s_field_855_j[var10];
			double var12 = (double)(var11[1][0] - var11[0][0]);
			double var14 = (double)(var11[1][2] - var11[0][2]);
			double var16 = sqrt(var12 * var12 + var14 * var14);
			double var18 = motionX * var12 + motionZ * var14;
			if(var18 < 0.0) {
				var12 = -var12;
				var14 = -var14;
			}

			double var20 = sqrt(motionX * motionX + motionZ * motionZ);
			motionX = var20 * var12 / var16;
			motionZ = var20 * var14 / var16;
			double var22 = 0.0;
			double var24 = (double)var1 + 0.5 + (double)var11[0][0] * 0.5;
			double var26 = (double)var3 + 0.5 + (double)var11[0][2] * 0.5;
			double var28 = (double)var1 + 0.5 + (double)var11[1][0] * 0.5;
			double var30 = (double)var3 + 0.5 + (double)var11[1][2] * 0.5;
			var12 = var28 - var24;
			var14 = var30 - var26;
			double var32;
			double var34;
			double var36;
			if(var12 == 0.0) {
				posX = (double)var1 + 0.5;
				var22 = posZ - (double)var3;
			} else if(var14 == 0.0) {
				posZ = (double)var3 + 0.5;
				var22 = posX - (double)var1;
			} else {
				var32 = posX - var24;
				var34 = posZ - var26;
				var36 = (var32 * var12 + var34 * var14) * 2.0;
				var22 = var36;
			}

			posX = var24 + var12 * var22;
			posZ = var26 + var14 * var22;
			setPosition(posX, posY + (double)yOffset, posZ);
			var32 = motionX;
			var34 = motionZ;
			if(m_riddenByEntity != NULL) {
				var32 *= 0.75;
				var34 *= 0.75;
			}

			if(var32 < -var4) {
				var32 = -var4;
			}

			if(var32 > var4) {
				var32 = var4;
			}

			if(var34 < -var4) {
				var34 = -var4;
			}

			if(var34 > var4) {
				var34 = var4;
			}

			moveEntity(var32, 0.0, var34);
			if(var11[0][1] != 0 && MathHelper::floor_double(posX) - var1 == var11[0][0] && MathHelper::floor_double(posZ) - var3 == var11[0][2]) {
				setPosition(posX, posY + (double)var11[0][1], posZ);
			} else if(var11[1][1] != 0 && MathHelper::floor_double(posX) - var1 == var11[1][0] && MathHelper::floor_double(posZ) - var3 == var11[1][2]) {
				setPosition(posX, posY + (double)var11[1][1], posZ);
			}

			if(m_riddenByEntity != NULL) {
				motionX *= (double)0.997F;
				motionY *= 0.0;
				motionZ *= (double)0.997F;
			} else {
				if(m_d == 2) {
					var36 = (double)MathHelper::sqrt_double(m_pushX * m_pushX + m_pushZ * m_pushZ);
					if(var36 > 0.01) {
						var6 = true;
						m_pushX /= var36;
						m_pushZ /= var36;
						double var38 = 0.04;
						motionX *= (double)0.8F;
						motionY *= 0.0;
						motionZ *= (double)0.8F;
						motionX += m_pushX * var38;
						motionZ += m_pushZ * var38;
					} else {
						motionX *= (double)0.9F;
						motionY *= 0.0;
						motionZ *= (double)0.9F;
					}
				}

				motionX *= (double)0.96F;
				motionY *= 0.0;
				motionZ *= (double)0.96F;
			}

			Vec3D* var46 = func_514_g(posX, posY, posZ);
			if(var46 != NULL && var9 != NULL) {
				double var37 = (var9->yCoord - var46->yCoord) * 0.05;
				var20 = sqrt(motionX * motionX + motionZ * motionZ);
				if(var20 > 0.0) {
					motionX = motionX / var20 * (var20 + var37);
					motionZ = motionZ / var20 * (var20 + var37);
				}

				setPosition(posX, var46->yCoord, posZ);
			}

			int var47 = MathHelper::floor_double(posX);
			int var48 = MathHelper::floor_double(posZ);
			if(var47 != var1 || var48 != var3) {
				var20 = sqrt(motionX * motionX + motionZ * motionZ);
				motionX = var20 * (double)(var47 - var1);
				motionZ = var20 * (double)(var48 - var3);
			}

			if(m_d == 2) {
				double var39 = (double)MathHelper::sqrt_double(m_pushX * m_pushX + m_pushZ * m_pushZ);
				if(var39 > 0.01 && motionX * motionX + motionZ * motionZ > 0.001) {
					m_pushX /= var39;
					m_pushZ /= var39;
					if(m_pushX * motionX + m_pushZ * motionZ < 0.0) {
						m_pushX = 0.0;
						m_pushZ = 0.0;
					} else {
						m_pushX = motionX;
						m_pushZ = motionZ;
					}
				}
			}
		} else {
			if(motionX < -var4) {
				motionX = -var4;
			}

			if(motionX > var4) {
				motionX = var4;
			}

			if(motionZ < -var4) {
				motionZ = -var4;
			}

			if(motionZ > var4) {
				motionZ = var4;
			}

			if(onGround) {
				motionX *= 0.5;
				motionY *= 0.5;
				motionZ *= 0.5;
			}

			moveEntity(motionX, motionY, motionZ);
			if(!onGround) {
				motionX *= (double)0.95F;
				motionY *= (double)0.95F;
				motionZ *= (double)0.95F;
			}
		}

		rotationPitch = 0.0F;
		double var43 = prevPosX - posX;
		double var44 = prevPosZ - posZ;
		if(var43 * var43 + var44 * var44 > 0.001) {
			rotationYaw = (float)(atan2(var44, var43) * 180.0 / 3.14159265358979323846);
			if(m_field_856_i) {
				rotationYaw += 180.0F;
			}
		}

		double var13;
		for(var13 = (double)(rotationYaw - prevRotationYaw); var13 >= 180.0; var13 -= 360.0) {
		}

		while(var13 < -180.0) {
			var13 += 360.0;
		}

		if(var13 < -170.0 || var13 >= 170.0) {
			rotationYaw += 180.0F;
			m_field_856_i = !m_field_856_i;
		}

		setRotation(rotationYaw, rotationPitch);
		std::vector<void*> var15 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.expands((double)0.2F, 0.0, (double)0.2F));
		if(var15.size() > 0) {
			for(size_t var45 = 0; var45 < var15.size(); ++var45) {
				Entity* var17 = (Entity*)var15[var45];
				if(var17 != m_riddenByEntity && var17->canBePushed() && dynamic_cast<EntityMinecart*>(var17) != NULL) {
					var17->applyEntityCollision(this);
				}
			}
		}

		if(m_riddenByEntity != NULL && m_riddenByEntity->isDead) {
			m_riddenByEntity = NULL;
		}

		if(var6 && rand->nextInt(4) == 0) {
			--m_fuel;
			if(m_fuel < 0) {
				m_pushX = m_pushZ = 0.0;
			}

			worldObj->spawnParticle("largesmoke", posX, posY + 0.8, posZ, 0.0, 0.0, 0.0);
		}

	}
}

Vec3D* EntityMinecart::func_515_a(double var1, double var3, double var5, double var7) {
	int var9 = MathHelper::floor_double(var1);
	int var10 = MathHelper::floor_double(var3);
	int var11i = MathHelper::floor_double(var5);
	if(worldObj->getBlockId(var9, var10 - 1, var11i) == Block::s_minecartTrack->blockID) {
		--var10;
	}

	if(worldObj->getBlockId(var9, var10, var11i) == Block::s_minecartTrack->blockID) {
		int var12 = worldObj->getBlockMetadata(var9, var10, var11i);
		var3 = (double)var10;
		if(var12 >= 2 && var12 <= 5) {
			var3 = (double)(var10 + 1);
		}

		int** var13 = s_field_855_j[var12];
		double var14 = (double)(var13[1][0] - var13[0][0]);
		double var16 = (double)(var13[1][2] - var13[0][2]);
		double var18 = sqrt(var14 * var14 + var16 * var16);
		var14 /= var18;
		var16 /= var18;
		var1 += var14 * var7;
		var5 += var16 * var7;
		if(var13[0][1] != 0 && MathHelper::floor_double(var1) - var9 == var13[0][0] && MathHelper::floor_double(var5) - var11i == var13[0][2]) {
			var3 += (double)var13[0][1];
		} else if(var13[1][1] != 0 && MathHelper::floor_double(var1) - var9 == var13[1][0] && MathHelper::floor_double(var5) - var11i == var13[1][2]) {
			var3 += (double)var13[1][1];
		}

		return func_514_g(var1, var3, var5);
	} else {
		return NULL;
	}
}

Vec3D* EntityMinecart::func_514_g(double var1, double var3, double var5) {
	int var7 = MathHelper::floor_double(var1);
	int var8 = MathHelper::floor_double(var3);
	int var9 = MathHelper::floor_double(var5);
	if(worldObj->getBlockId(var7, var8 - 1, var9) == Block::s_minecartTrack->blockID) {
		--var8;
	}

	if(worldObj->getBlockId(var7, var8, var9) == Block::s_minecartTrack->blockID) {
		int var10 = worldObj->getBlockMetadata(var7, var8, var9);
		var3 = (double)var8;
		if(var10 >= 2 && var10 <= 5) {
			var3 = (double)(var8 + 1);
		}

		int** var11 = s_field_855_j[var10];
		double var12 = 0.0;
		double var14 = (double)var7 + 0.5 + (double)var11[0][0] * 0.5;
		double var16 = (double)var8 + 0.5 + (double)var11[0][1] * 0.5;
		double var18 = (double)var9 + 0.5 + (double)var11[0][2] * 0.5;
		double var20 = (double)var7 + 0.5 + (double)var11[1][0] * 0.5;
		double var22 = (double)var8 + 0.5 + (double)var11[1][1] * 0.5;
		double var24 = (double)var9 + 0.5 + (double)var11[1][2] * 0.5;
		double var26 = var20 - var14;
		double var28 = (var22 - var16) * 2.0;
		double var30 = var24 - var18;
		if(var26 == 0.0) {
			var1 = (double)var7 + 0.5;
			var12 = var5 - (double)var9;
		} else if(var30 == 0.0) {
			var5 = (double)var9 + 0.5;
			var12 = var1 - (double)var7;
		} else {
			double var32 = var1 - var14;
			double var34 = var5 - var18;
			double var36 = (var32 * var26 + var34 * var30) * 2.0;
			var12 = var36;
		}

		var1 = var14 + var26 * var12;
		var3 = var16 + var28 * var12;
		var5 = var18 + var30 * var12;
		if(var28 < 0.0) {
			++var3;
		}

		if(var28 > 0.0) {
			var3 += 0.5;
		}

		return Vec3D::createVector(var1, var3, var5);
	} else {
		return NULL;
	}
}

void EntityMinecart::writeEntityToNBT(NBTTagCompound* var1) {
	var1->setInteger("Type", m_d);
	if(m_d == 2) {
		var1->setDouble("PushX", m_pushX);
		var1->setDouble("PushZ", m_pushZ);
		var1->setShort("Fuel", (short)m_fuel);
	} else if(m_d == 1) {
		NBTTagList* var2 = new NBTTagList();

		for(int var3 = 0; var3 < getSizeInventory(); ++var3) {
			if(m_cargoItems[var3] != NULL) {
				NBTTagCompound* var4 = new NBTTagCompound();
				var4->setByte("Slot", (unsigned char)var3);
				m_cargoItems[var3]->writeToNBT(var4);
				var2->setTag(var4);
			}
		}

		var1->setTag("Items", var2);
	}

}

void EntityMinecart::readEntityFromNBT(NBTTagCompound* var1) {
	m_d = var1->getInteger("Type");
	if(m_d == 2) {
		m_pushX = var1->getDouble("PushX");
		m_pushZ = var1->getDouble("PushZ");
		m_fuel = var1->getShort("Fuel");
	} else if(m_d == 1) {
		NBTTagList* var2 = var1->getTagList("Items");
		// Java reallocs cargoItems to getSizeInventory(); preserve the 36-slot
		// backing buffer instead and just clear it (getSizeInventory()==27 < 36,
		// and the existing 1-arg ctor already allocated 36).
		if(m_cargoItems == NULL) {
			m_cargoItems = new ItemStack*[36];
			m_cargoItems_count = 36;
		}
		for(int var6 = 0; var6 < m_cargoItems_count; ++var6) {
			m_cargoItems[var6] = NULL;
		}

		for(int var3 = 0; var3 < var2->tagCount(); ++var3) {
			NBTTagCompound* var4 = (NBTTagCompound*)var2->tagAt(var3);
			int var5 = var4->getByte("Slot") & 255;
			if(var5 >= 0 && var5 < getSizeInventory()) {
				m_cargoItems[var5] = new ItemStack(var4);
			}
		}
	}

}

float EntityMinecart::func_392_h_() {
	return 0.0F;
}

void EntityMinecart::applyEntityCollision(Entity* var1) {
	if(!worldObj->m_multiplayerWorld) {
		if(var1 != m_riddenByEntity) {
			if(dynamic_cast<EntityLiving*>(var1) != NULL && !(dynamic_cast<EntityPlayer*>(var1) != NULL) && m_d == 0 && motionX * motionX + motionZ * motionZ > 0.01 && m_riddenByEntity == NULL && var1->m_ridingEntity == NULL) {
				var1->mountEntity(this);
			}

			double var2 = var1->posX - posX;
			double var4 = var1->posZ - posZ;
			double var6 = var2 * var2 + var4 * var4;
			if(var6 >= (double)1.0E-4F) {
				var6 = (double)MathHelper::sqrt_double(var6);
				var2 /= var6;
				var4 /= var6;
				double var8 = 1.0 / var6;
				if(var8 > 1.0) {
					var8 = 1.0;
				}

				var2 *= var8;
				var4 *= var8;
				var2 *= (double)0.1F;
				var4 *= (double)0.1F;
				var2 *= (double)(1.0F - m_field_632_aO);
				var4 *= (double)(1.0F - m_field_632_aO);
				var2 *= 0.5;
				var4 *= 0.5;
				EntityMinecart* var1cart = dynamic_cast<EntityMinecart*>(var1);
				if(var1cart != NULL) {
					double var10 = var1->motionX + motionX;
					double var12 = var1->motionZ + motionZ;
					if(var1cart->m_d == 2 && m_d != 2) {
						motionX *= (double)0.2F;
						motionZ *= (double)0.2F;
						addVelocity(var1->motionX - var2, 0.0, var1->motionZ - var4);
						var1->motionX *= (double)0.7F;
						var1->motionZ *= (double)0.7F;
					} else if(var1cart->m_d != 2 && m_d == 2) {
						var1->motionX *= (double)0.2F;
						var1->motionZ *= (double)0.2F;
						var1->addVelocity(motionX + var2, 0.0, motionZ + var4);
						motionX *= (double)0.7F;
						motionZ *= (double)0.7F;
					} else {
						var10 /= 2.0;
						var12 /= 2.0;
						motionX *= (double)0.2F;
						motionZ *= (double)0.2F;
						addVelocity(var10 - var2, 0.0, var12 - var4);
						var1->motionX *= (double)0.2F;
						var1->motionZ *= (double)0.2F;
						var1->addVelocity(var10 + var2, 0.0, var12 + var4);
					}
				} else {
					addVelocity(-var2, 0.0, -var4);
					var1->addVelocity(var2 / 4.0, 0.0, var4 / 4.0);
				}
			}

		}
	}
}

int EntityMinecart::getSizeInventory() {
	return 27;
}

ItemStack* EntityMinecart::getStackInSlot(int var1) {
	return m_cargoItems[var1];
}

ItemStack* EntityMinecart::decrStackSize(int var1, int var2) {
	if(m_cargoItems[var1] != NULL) {
		ItemStack* var3 = NULL;
		if(m_cargoItems[var1]->stackSize <= var2) {
			var3 = m_cargoItems[var1];
			m_cargoItems[var1] = NULL;
			return var3;
		} else {
			var3 = m_cargoItems[var1]->splitStack(var2);
			if(m_cargoItems[var1]->stackSize == 0) {
				m_cargoItems[var1] = NULL;
			}

			return var3;
		}
	} else {
		return NULL;
	}
}

void EntityMinecart::setInventorySlotContents(int var1, ItemStack* var2) {
	m_cargoItems[var1] = var2;
	if(var2 != NULL && var2->stackSize > getInventoryStackLimit()) {
		var2->stackSize = getInventoryStackLimit();
	}

}

const std::string& EntityMinecart::getInvName() {
	// Interface returns a const-ref; back it with a function-local static so we
	// don't return the address of a temporary.
	static const std::string s_name = "Minecart";
	return s_name;
}

int EntityMinecart::getInventoryStackLimit() {
	return 64;
}

void EntityMinecart::onInventoryChanged() {
	// Java body is empty.
}

bool EntityMinecart::interact(EntityPlayer* var1) {
	if(m_d == 0) {
		if(m_riddenByEntity != NULL && dynamic_cast<EntityPlayer*>(m_riddenByEntity) != NULL && m_riddenByEntity != var1) {
			return true;
		}

		if(!worldObj->m_multiplayerWorld) {
			var1->mountEntity(this);
		}
	} else if(m_d == 1) {
		var1->displayGUIChest(this);
	} else if(m_d == 2) {
		ItemStack* var2 = var1->m_inventory->getCurrentItem();
		if(var2 != NULL && var2->itemID == Item::s_coal->shiftedIndex) {
			if(--var2->stackSize == 0) {
				var1->m_inventory->setInventorySlotContents(var1->m_inventory->m_currentItem, NULL);
			}

			m_fuel += 1200;
		}

		m_pushX = posX - var1->posX;
		m_pushZ = posZ - var1->posZ;
	}

	return true;
}

void EntityMinecart::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
	m_field_9414_l = var1;
	m_field_9413_m = var3;
	m_field_9412_n = var5;
	m_field_9411_o = (double)var7;
	m_field_9410_p = (double)var8;
	m_field_9415_k = var9 + 2;
	motionX = m_field_9409_q;
	motionY = m_field_9408_r;
	motionZ = m_field_9407_s;
}

void EntityMinecart::setVelocity(double var1, double var3, double var5) {
	m_field_9409_q = motionX = var1;
	m_field_9408_r = motionY = var3;
	m_field_9407_s = motionZ = var5;
}
