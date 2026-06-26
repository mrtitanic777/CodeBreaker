#include "stdafx.h"
#include "EntityPainting.h"

// Auto-generated from EntityPainting.java, hand-fixed for Xbox 360 / C++03 / DirectX 9
// Target: painting entity that hangs on a wall (LOGIC)

#include "Common.h"
#include "EnumArt.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Entity.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "Item.h"
#include "Material.h"
#include "AxisAlignedBB.h"
#include "MathHelper.h"
#include <string>
#include <vector>
#include <cmath>

EntityPainting::EntityPainting()
	: m_field_695_c(0)
	, m_field_691_a(0)
	, m_field_9322_d(0)
	, m_field_9321_e(0)
	, m_field_9320_f(0)
	, m_field_690_b(NULL)
{
}

EntityPainting::EntityPainting(World* var1)
	: Entity(var1)
	, m_field_695_c(0)
	, m_field_691_a(0)
	, m_field_9322_d(0)
	, m_field_9321_e(0)
	, m_field_9320_f(0)
	, m_field_690_b(NULL)
{
	yOffset = 0.0F;
	setSize(0.5F, 0.5F);
}

EntityPainting::EntityPainting(World* var1, int var2, int var3, int var4, int var5)
	: Entity(var1)
	, m_field_695_c(0)
	, m_field_691_a(0)
	, m_field_9322_d(0)
	, m_field_9321_e(0)
	, m_field_9320_f(0)
	, m_field_690_b(NULL)
{
	// PORT: constructor delegation -- inlined body of EntityPainting(World*)
	yOffset = 0.0F;
	setSize(0.5F, 0.5F);

	m_field_9322_d = var2;
	m_field_9321_e = var3;
	m_field_9320_f = var4;
	std::vector<EnumArt*> var6;
	EnumArt** var7 = EnumArt::values_;
	int var8 = EnumArt::values_count;

	for(int var9 = 0; var9 < var8; ++var9) {
		EnumArt* var10 = var7[var9];
		m_field_690_b = var10;
		func_412_b(var5);
		if(func_410_i()) {
			var6.push_back(var10);
		}
	}

	if(var6.size() > 0) {
		m_field_690_b = var6[rand->nextInt((int)var6.size())];
	}

	func_412_b(var5);
}

EntityPainting::~EntityPainting() {
	m_field_690_b = NULL;
}

void EntityPainting::func_412_b(int var1) {
	m_field_691_a = var1;
	prevRotationYaw = rotationYaw = (float)(var1 * 90);
	float var2 = (float)m_field_690_b->field_1623_z;
	float var3 = (float)m_field_690_b->field_1636_A;
	float var4 = (float)m_field_690_b->field_1623_z;
	if(var1 != 0 && var1 != 2) {
		var2 = 0.5F;
	} else {
		var4 = 0.5F;
	}

	var2 /= 32.0F;
	var3 /= 32.0F;
	var4 /= 32.0F;
	float var5 = (float)m_field_9322_d + 0.5F;
	float var6 = (float)m_field_9321_e + 0.5F;
	float var7 = (float)m_field_9320_f + 0.5F;
	float var8 = 9.0F / 16.0F;
	if(var1 == 0) {
		var7 -= var8;
	}

	if(var1 == 1) {
		var5 -= var8;
	}

	if(var1 == 2) {
		var7 += var8;
	}

	if(var1 == 3) {
		var5 += var8;
	}

	if(var1 == 0) {
		var5 -= func_411_c(m_field_690_b->field_1623_z);
	}

	if(var1 == 1) {
		var7 += func_411_c(m_field_690_b->field_1623_z);
	}

	if(var1 == 2) {
		var5 += func_411_c(m_field_690_b->field_1623_z);
	}

	if(var1 == 3) {
		var7 -= func_411_c(m_field_690_b->field_1623_z);
	}

	var6 += func_411_c(m_field_690_b->field_1636_A);
	setPosition((double)var5, (double)var6, (double)var7);
	float var9 = -(0.1F / 16.0F);
	boundingBox.setBounds((double)(var5 - var2 - var9), (double)(var6 - var3 - var9), (double)(var7 - var4 - var9), (double)(var5 + var2 + var9), (double)(var6 + var3 + var9), (double)(var7 + var4 + var9));
}

float EntityPainting::func_411_c(int var1) {
	return var1 == 32 ? 0.5F : (var1 == 64 ? 0.5F : 0.0F);
}

void EntityPainting::onUpdate() {
	if(m_field_695_c++ == 100 && !func_410_i()) {
		m_field_695_c = 0;
		setEntityDead();
		worldObj->entityJoinedWorld(new EntityItem(worldObj, posX, posY, posZ, new ItemStack(Item::s_painting)));
	}
}

bool EntityPainting::func_410_i() {
	if(worldObj->getCollidingBoundingBoxes(this, boundingBox).size() > 0) {
		return false;
	} else {
		int var1 = m_field_690_b->field_1623_z / 16;
		int var2 = m_field_690_b->field_1636_A / 16;
		int var3 = m_field_9322_d;
		int var4 = m_field_9321_e;
		int var5 = m_field_9320_f;
		if(m_field_691_a == 0) {
			var3 = (int)floor(posX - (double)((float)m_field_690_b->field_1623_z / 32.0F));
		}

		if(m_field_691_a == 1) {
			var5 = (int)floor(posZ - (double)((float)m_field_690_b->field_1623_z / 32.0F));
		}

		if(m_field_691_a == 2) {
			var3 = (int)floor(posX - (double)((float)m_field_690_b->field_1623_z / 32.0F));
		}

		if(m_field_691_a == 3) {
			var5 = (int)floor(posZ - (double)((float)m_field_690_b->field_1623_z / 32.0F));
		}

		var4 = (int)floor(posY - (double)((float)m_field_690_b->field_1636_A / 32.0F));

		for(int var6 = 0; var6 < var1; ++var6) {
			for(int var7 = 0; var7 < var2; ++var7) {
				Material* var8 = NULL;
				if(m_field_691_a != 0 && m_field_691_a != 2) {
					var8 = worldObj->getBlockMaterial(m_field_9322_d, var4 + var7, var5 + var6);
				} else {
					var8 = worldObj->getBlockMaterial(var3 + var6, var4 + var7, m_field_9320_f);
				}

				if(!var8->func_878_a()) {
					return false;
				}
			}
		}

		std::vector<void*> var9 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox);

		for(size_t var10 = 0; var10 < var9.size(); ++var10) {
			Entity* var11 = (Entity*)var9[var10];
			if(dynamic_cast<EntityPainting*>(var11) != NULL) {
				return false;
			}
		}

		return true;
	}
}

bool EntityPainting::canBeCollidedWith() {
	return true;
}

bool EntityPainting::canAttackEntity(Entity* var1, int var2) {
	setEntityDead();
	func_9281_M();
	worldObj->entityJoinedWorld(new EntityItem(worldObj, posX, posY, posZ, new ItemStack(Item::s_painting)));
	return true;
}

void EntityPainting::writeEntityToNBT(NBTTagCompound* var1) {
	var1->setByte("Dir", (unsigned char)m_field_691_a);
	var1->setString("Motive", m_field_690_b->field_1624_y);
	var1->setInteger("TileX", m_field_9322_d);
	var1->setInteger("TileY", m_field_9321_e);
	var1->setInteger("TileZ", m_field_9320_f);
}

void EntityPainting::readEntityFromNBT(NBTTagCompound* var1) {
	m_field_691_a = var1->getByte("Dir");
	m_field_9322_d = var1->getInteger("TileX");
	m_field_9321_e = var1->getInteger("TileY");
	m_field_9320_f = var1->getInteger("TileZ");
	std::string var2 = var1->getString("Motive");
	EnumArt** var3 = EnumArt::values_;
	int var4 = EnumArt::values_count;

	for(int var5 = 0; var5 < var4; ++var5) {
		EnumArt* var6 = var3[var5];
		if(var6->field_1624_y == var2) {
			m_field_690_b = var6;
		}
	}

	if(m_field_690_b == NULL) {
		m_field_690_b = EnumArt::s_Kebab;
	}

	func_412_b(m_field_691_a);
}
