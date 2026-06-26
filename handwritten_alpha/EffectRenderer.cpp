#include "stdafx.h"
// Auto-generated from EffectRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EffectRenderer.h"
#include "Common.h"
#include "Entity.h"
#include "EntityFX.h"
#include "Random.h"
#include "RenderEngine.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "Tesselator.h"
#include "Textures.h"
#include "EntityDiggingFX.h"

EffectRenderer::EffectRenderer()
	: worldObj(NULL)
	, m_field_1728_b(new std::vector<void*>[4])
	, m_field_1731_c(NULL)
	, rand(new Random())
{
}

EffectRenderer::EffectRenderer(World* var1, RenderEngine* var2)
	: worldObj(NULL)
	, m_field_1728_b(new std::vector<void*>[4])
	, m_field_1731_c(NULL)
	, rand(new Random())
{
		if(var1 != NULL) {
			worldObj = var1;
		}

		m_field_1731_c = var2;

		for(int var3 = 0; var3 < 4; ++var3) {
			m_field_1728_b[var3].clear();
		}

}

EffectRenderer::~EffectRenderer() {
	worldObj = NULL;
	delete[] m_field_1728_b;
	m_field_1728_b = NULL;
	m_field_1731_c = NULL;
	rand = NULL;
}

void EffectRenderer::func_1192_a(EntityFX* var1) {
		int var2 = var1->func_404_c();
		// PORT: cap each FX layer to prevent runaway particle accumulation (TNT-stack OOM/fps).
		// When full, drop the OLDEST particle (front) before adding. (matches reference port)
		if(var2 >= 0 && var2 < 4) {
			if(m_field_1728_b[var2].size() >= 4000) {
				_vecRemoveAt(m_field_1728_b[var2], 0);
			}
			m_field_1728_b[var2].push_back(var1);
		}
}

void EffectRenderer::func_1193_a() {
		for(int var1 = 0; var1 < 4; ++var1) {
			for(int var2 = 0; var2 < (int)m_field_1728_b[var1].size(); ++var2) {
				EntityFX* var3 = (EntityFX*)m_field_1728_b[var1][var2];
				var3->onUpdate();
				if(var3->isDead) {
					_vecRemoveAt(m_field_1728_b[var1], var2--);
				}
			}
		}

}

void EffectRenderer::func_1189_a(Entity* var1, float var2) {
		float var3 = cosf(var1->rotationYaw * 3.14159265358979323846f / 180.0F);
		float var4 = sinf(var1->rotationYaw * 3.14159265358979323846f / 180.0F);
		float var5 = -var4 * sinf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);
		float var6 = var3 * sinf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);
		float var7 = cosf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);
		EntityFX::s_field_660_l = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
		EntityFX::s_field_659_m = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
		EntityFX::s_field_658_n = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;

		for(int var8 = 0; var8 < 3; ++var8) {
			if(m_field_1728_b[var8].size() != 0) {
				int var9 = 0;
				if(var8 == 0) {
					var9 = Textures::load("game:\\Content\\particles.png");
				}

				if(var8 == 1) {
					var9 = Textures::load("game:\\Content\\terrain.png");
				}

				if(var8 == 2) {
					var9 = Textures::load("game:\\Content\\gui\\items.png");
				}

    g_pd3dDevice->SetTexture(0, Textures::getById(var9));
				Tesselator& var10 = Tesselator::getInstance();
				var10.init();

				for(int var11 = 0; var11 < (int)m_field_1728_b[var8].size(); ++var11) {
					EntityFX* var12 = (EntityFX*)m_field_1728_b[var8][var11];
					var12->func_406_a(&var10, var2, var3, var7, var4, var5, var6);
				}

				var10.flush();
			}
		}

}

void EffectRenderer::func_1187_b(Entity* var1, float var2) {
		unsigned char var3 = 3;
		if(m_field_1728_b[var3].size() != 0) {
			Tesselator& var4 = Tesselator::getInstance();

			for(int var5 = 0; var5 < (int)m_field_1728_b[var3].size(); ++var5) {
				EntityFX* var6 = (EntityFX*)m_field_1728_b[var3][var5];
				var6->func_406_a(&var4, var2, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
			}

		}
}

void EffectRenderer::func_1188_a(World* var1) {
		worldObj = var1;

		for(int var2 = 0; var2 < 4; ++var2) {
			m_field_1728_b[var2].clear();
		}

}

void EffectRenderer::func_1186_a(int var1, int var2, int var3) {
		int var4 = worldObj->getBlockId(var1, var2, var3);
		if(var4 != 0 && Block::blocksList[var4] != NULL) {   // PORT: guard unregistered block id (EntityDiggingFX derefs the Block)
			Block* var5 = Block::blocksList[var4];
			unsigned char var6 = 4;

			for(int var7 = 0; var7 < var6; ++var7) {
				for(int var8 = 0; var8 < var6; ++var8) {
					for(int var9 = 0; var9 < var6; ++var9) {
						double var10 = (double)var1 + ((double)var7 + 0.5f) / (double)var6;
						double var12 = (double)var2 + ((double)var8 + 0.5f) / (double)var6;
						double var14 = (double)var3 + ((double)var9 + 0.5f) / (double)var6;
						func_1192_a((new EntityDiggingFX(worldObj, var10, var12, var14, var10 - (double)var1 - 0.5f, var12 - (double)var2 - 0.5f, var14 - (double)var3 - 0.5f, var5))->func_4041_a(var1, var2, var3));
					}
				}
			}

		}
}

void EffectRenderer::func_1191_a(int var1, int var2, int var3, int var4) {
		int var5 = worldObj->getBlockId(var1, var2, var3);
		if(var5 != 0 && Block::blocksList[var5] != NULL) {   // PORT: guard unregistered block id
			Block* var6 = Block::blocksList[var5];
			float var7 = 0.1F;
			double var8 = (double)var1 + rand->nextDouble() * (var6->maxX - var6->m_field_370_bf - (double)(var7 * 2.0F)) + (double)var7 + var6->m_field_370_bf;
			double var10 = (double)var2 + rand->nextDouble() * (var6->maxY - var6->minY - (double)(var7 * 2.0F)) + (double)var7 + var6->minY;
			double var12 = (double)var3 + rand->nextDouble() * (var6->maxZ - var6->minZ - (double)(var7 * 2.0F)) + (double)var7 + var6->minZ;
			if(var4 == 0) {
				var10 = (double)var2 + var6->minY - (double)var7;
			}

			if(var4 == 1) {
				var10 = (double)var2 + var6->maxY + (double)var7;
			}

			if(var4 == 2) {
				var12 = (double)var3 + var6->minZ - (double)var7;
			}

			if(var4 == 3) {
				var12 = (double)var3 + var6->maxZ + (double)var7;
			}

			if(var4 == 4) {
				var8 = (double)var1 + var6->m_field_370_bf - (double)var7;
			}

			if(var4 == 5) {
				var8 = (double)var1 + var6->maxX + (double)var7;
			}

			func_1192_a((new EntityDiggingFX(worldObj, var8, var10, var12, 0.0f, 0.0f, 0.0f, var6))->func_4041_a(var1, var2, var3)->func_407_b(0.2F)->func_405_d(0.6F));
		}
}

const std::string& EffectRenderer::func_1190_b() {
		// PORT: original returned `"" + (count)` (Java string concat). Build the stats string
		// in a static so we can safely return by const-ref (matches the .h signature).
		static std::string s_stats;
		s_stats = toStr((int)(m_field_1728_b[0].size() + m_field_1728_b[1].size() + m_field_1728_b[2].size()));
		return s_stats;
}
