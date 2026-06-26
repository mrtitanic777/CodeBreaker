#include "stdafx.h"
// Auto-generated from Explosion.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - dropped `#include "Set.h"`; field_12251_g is std::vector<void*>* (was java.util.Set).
//  - Java `field_12251_g = new HashSet()` (field initializer) -> allocate in ctors.
//  - Java Set.add -> ->push_back; ArrayList.addAll(set) -> copy the vector.

#include "Explosion.h"
#include "Common.h"
#include "BlockFire.h"
#include "Entity.h"
#include "Random.h"
#include <vector>
#include "World.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "Vec3D.h"
#include "ChunkPosition.h"

Explosion::Explosion()
	: m_field_12257_a(false)
	, m_field_12256_b(0)
	, m_field_12255_c(0)
	, m_field_12254_d(0)
	, m_field_12253_e(NULL)
	, m_field_12252_f(0)
	, m_field_12251_g(new std::vector<void*>())
	, m_field_12250_h(new Random())
	, m_field_12249_i(NULL)
{
}

Explosion::Explosion(World* var1, Entity* var2, double var3, double var5, double var7, float var9)
	: m_field_12257_a(false)
	, m_field_12256_b(0)
	, m_field_12255_c(0)
	, m_field_12254_d(0)
	, m_field_12253_e(NULL)
	, m_field_12252_f(0)
	, m_field_12251_g(new std::vector<void*>())
	, m_field_12250_h(new Random())
	, m_field_12249_i(NULL)
{
		m_field_12249_i = var1;
		m_field_12253_e = var2;
		m_field_12252_f = var9;
		m_field_12256_b = var3;
		m_field_12255_c = var5;
		m_field_12254_d = var7;
}

Explosion::~Explosion() {
	m_field_12250_h = NULL;
	m_field_12249_i = NULL;
	m_field_12253_e = NULL;
	delete m_field_12251_g;
	m_field_12251_g = NULL;
}

void Explosion::func_12248_a() {
		float var1 = m_field_12252_f;
		unsigned char var2 = 16;

		int var3;
		int var4;
		int var5;
		double var15;
		double var17;
		double var19;
		for(var3 = 0; var3 < var2; ++var3) {
			for(var4 = 0; var4 < var2; ++var4) {
				for(var5 = 0; var5 < var2; ++var5) {
					if(var3 == 0 || var3 == var2 - 1 || var4 == 0 || var4 == var2 - 1 || var5 == 0 || var5 == var2 - 1) {
						double var6 = (double)((float)var3 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
						double var8 = (double)((float)var4 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
						double var10 = (double)((float)var5 / ((float)var2 - 1.0F) * 2.0F - 1.0F);
						double var12 = sqrtf(var6 * var6 + var8 * var8 + var10 * var10);
						var6 /= var12;
						var8 /= var12;
						var10 /= var12;
						float var14 = m_field_12252_f * (0.7F + m_field_12249_i->rand->nextFloat() * 0.6F);
						var15 = m_field_12256_b;
						var17 = m_field_12255_c;
						var19 = m_field_12254_d;

						for(float var21 = 0.3F; var14 > 0.0F; var14 -= var21 * (12.0F / 16.0F)) {
							int var22 = (int)floor(var15);
							int var23 = (int)floor(var17);
							int var24 = (int)floor(var19);
							int var25 = m_field_12249_i->getBlockId(var22, var23, var24);
							if(var25 > 0) {
								var14 -= (Block::blocksList[var25]->func_227_a(m_field_12253_e) + 0.3F) * var21;
							}

							if(var14 > 0.0F) {
								m_field_12251_g->push_back(new ChunkPosition(var22, var23, var24));
							}

							var15 += var6 * (double)var21;
							var17 += var8 * (double)var21;
							var19 += var10 * (double)var21;
						}
					}
				}
			}
		}

		m_field_12252_f *= 2.0F;
		var3 = (int)floor(m_field_12256_b - (double)m_field_12252_f - 1.0f);
		var4 = (int)floor(m_field_12256_b + (double)m_field_12252_f + 1.0f);
		var5 = (int)floor(m_field_12255_c - (double)m_field_12252_f - 1.0f);
		int var29 = (int)floor(m_field_12255_c + (double)m_field_12252_f + 1.0f);
		int var7 = (int)floor(m_field_12254_d - (double)m_field_12252_f - 1.0f);
		int var30 = (int)floor(m_field_12254_d + (double)m_field_12252_f + 1.0f);
		std::vector<void*> var9 = m_field_12249_i->getEntitiesWithinAABBExcludingEntity(m_field_12253_e, AxisAlignedBB::getBoundingBoxFromPool((double)var3, (double)var5, (double)var7, (double)var4, (double)var29, (double)var30));
		Vec3D* var31 = Vec3D::createVector(m_field_12256_b, m_field_12255_c, m_field_12254_d);

		for(size_t var11 = 0; var11 < var9.size(); ++var11) {
			Entity* var33 = (Entity*)var9[var11];
			double var13 = var33->getDistance(m_field_12256_b, m_field_12255_c, m_field_12254_d) / (double)m_field_12252_f;
			if(var13 <= 1.0f) {
				var15 = var33->posX - m_field_12256_b;
				var17 = var33->posY - m_field_12255_c;
				var19 = var33->posZ - m_field_12254_d;
				double var39 = (double)sqrt(var15 * var15 + var17 * var17 + var19 * var19);
				var15 /= var39;
				var17 /= var39;
				var19 /= var39;
				double var40 = (double)m_field_12249_i->func_675_a(var31, var33->boundingBox);
				double var41 = (1.0f - var13) * var40;
				var33->canAttackEntity(m_field_12253_e, (int)((var41 * var41 + var41) / 2.0f * 8.0f * (double)m_field_12252_f + 1.0f));
				var33->motionX += var15 * var41;
				var33->motionY += var17 * var41;
				var33->motionZ += var19 * var41;
			}
		}

		m_field_12252_f = var1;
		// Java: ArrayList var32 = new ArrayList(); var32.addAll(this.field_12251_g);
		std::vector<void*> var32 = *m_field_12251_g;
		if(m_field_12257_a) {
			for(int var34 = (int)var32.size() - 1; var34 >= 0; --var34) {
				ChunkPosition* var35 = (ChunkPosition*)var32[var34];
				int var36 = var35->m_x;
				int var37 = var35->m_y;
				int var16 = var35->m_z;
				int var38 = m_field_12249_i->getBlockId(var36, var37, var16);
				int var18 = m_field_12249_i->getBlockId(var36, var37 - 1, var16);
				if(var38 == 0 && Block::s_opaqueCubeLookup[var18] && m_field_12250_h->nextInt(3) == 0) {
					m_field_12249_i->setBlockWithNotify(var36, var37, var16, Block::s_fire->blockID);
				}
			}
		}

}

void Explosion::func_12247_b() {
		m_field_12249_i->playSoundEffect(m_field_12256_b, m_field_12255_c, m_field_12254_d, "random.explode", 4.0F, (1.0F + (m_field_12249_i->rand->nextFloat() - m_field_12249_i->rand->nextFloat()) * 0.2F) * 0.7F);
		// Java: ArrayList var1 = new ArrayList(); var1.addAll(this.field_12251_g);
		std::vector<void*> var1 = *m_field_12251_g;

		for(int var2 = (int)var1.size() - 1; var2 >= 0; --var2) {
			ChunkPosition* var3 = (ChunkPosition*)var1[var2];
			int var4 = var3->m_x;
			int var5 = var3->m_y;
			int var6 = var3->m_z;
			int var7 = m_field_12249_i->getBlockId(var4, var5, var6);

			for(int var8 = 0; var8 < 1; ++var8) {
				double var9 = (double)((float)var4 + m_field_12249_i->rand->nextFloat());
				double var11 = (double)((float)var5 + m_field_12249_i->rand->nextFloat());
				double var13 = (double)((float)var6 + m_field_12249_i->rand->nextFloat());
				double var15 = var9 - m_field_12256_b;
				double var17 = var11 - m_field_12255_c;
				double var19 = var13 - m_field_12254_d;
				double var21 = (double)sqrt(var15 * var15 + var17 * var17 + var19 * var19);
				var15 /= var21;
				var17 /= var21;
				var19 /= var21;
				double var23 = 0.5f / (var21 / (double)m_field_12252_f + 0.1f);
				var23 *= (double)(m_field_12249_i->rand->nextFloat() * m_field_12249_i->rand->nextFloat() + 0.3F);
				var15 *= var23;
				var17 *= var23;
				var19 *= var23;
				m_field_12249_i->spawnParticle("explode", (var9 + m_field_12256_b * 1.0f) / 2.0f, (var11 + m_field_12255_c * 1.0f) / 2.0f, (var13 + m_field_12254_d * 1.0f) / 2.0f, var15, var17, var19);
				m_field_12249_i->spawnParticle("smoke", var9, var11, var13, var15, var17, var19);
			}

			if(var7 > 0) {
				Block::blocksList[var7]->dropBlockAsItemWithChance(m_field_12249_i, var4, var5, var6, m_field_12249_i->getBlockMetadata(var4, var5, var6), 0.3F);
				m_field_12249_i->setBlockWithNotify(var4, var5, var6, 0);
				Block::blocksList[var7]->onBlockDestroyedByExplosion(m_field_12249_i, var4, var5, var6);
			}
		}

}
