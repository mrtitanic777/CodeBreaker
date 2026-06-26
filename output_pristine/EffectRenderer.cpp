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

EffectRenderer::EffectRenderer()
	: worldObj(NULL)
	, m_fxLayers(new std::vector<EntityFX>[3])
	, m_renderEngine(NULL)
	, rand(new Random())
{
}

EffectRenderer::EffectRenderer(World* var1, RenderEngine* var2)
	: worldObj(NULL)
	, m_fxLayers(new std::vector<EntityFX>[3])
	, m_renderEngine(NULL)
	, rand(new Random())
{
		if(var1 != NULL) {
			worldObj = var1;
		}

		m_renderEngine = var2;

		for(int var3 = 0; var3 < 3; ++var3) {
			m_fxLayers[var3] = /* default vector */;
		}

}

EffectRenderer::~EffectRenderer() {
	worldObj = NULL;
	delete[] m_fxLayers;
	m_fxLayers = NULL;
	m_renderEngine = NULL;
	rand = NULL;
}

void EffectRenderer::addEffect(EntityFX* var1) {
		int var2 = var1->getFXLayer();
		m_fxLayers[var2].push_back(var1);
}

void EffectRenderer::updateEffects() {
		for(int var1 = 0; var1 < 3; ++var1) {
			for(int var2 = 0; var2 < m_fxLayers[var1].size(); ++var2) {
				EntityFX var3 = (EntityFX)m_fxLayers[var1][var2];
				var3.onEntityUpdate();
				if(var3.isDead) {
					m_fxLayers[var1].remove(var2--);
				}
			}
		}

}

void EffectRenderer::renderParticles(Entity* var1, float var2) {
		float var3 = -cosf(var1->rotationYaw * 3.14159265358979323846f / 180.0F);
		float var4 = -sinf(var1->rotationYaw * 3.14159265358979323846f / 180.0F);
		float var5 = -var4 * sinf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);
		float var6 = var3 * sinf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);
		float var11 = cosf(var1->rotationPitch * 3.14159265358979323846f / 180.0F);

		for(int var7 = 0; var7 < 2; ++var7) {
			if(m_fxLayers[var7].size() != 0) {
				int var8 = 0;
				if(var7 == 0) {
					var8 = m_Textures::load("game:\Content\particles.png");
				}

				if(var7 == 1) {
					var8 = m_Textures::load("game:\Content\terrain.png");
				}

    g_pd3dDevice->SetTexture(0, Textures::getById(var8));
				Tesselator var12 = Tesselator::getInstance();
				var12.init();

				for(int var9 = 0; var9 < m_fxLayers[var7].size(); ++var9) {
					EntityFX var10 = (EntityFX)m_fxLayers[var7][var9];
					var10.renderParticle(var12, var2, var3, var11, var4, var5, var6);
				}

				var12.flush();
			}
		}

}

void EffectRenderer::renderLitParticles(float var1) {
		if(m_fxLayers[2].size() != 0) {
			Tesselator var2 = Tesselator::getInstance();

			for(int var3 = 0; var3 < m_fxLayers[2].size(); ++var3) {
				EntityFX var4 = (EntityFX)m_fxLayers[2][var3];
				var4.renderParticle(var2, var1, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
			}

		}
}

void EffectRenderer::clearEffects(World* var1) {
		worldObj = var1;

		for(int var2 = 0; var2 < 3; ++var2) {
			m_fxLayers[var2].clear();
		}

}

void EffectRenderer::addBlockDestroyEffects(int var1, int var2, int var3) {
		int var4 = worldObj->getBlockId(var1, var2, var3);
		if(var4 != 0) {
			Block var11 = Block::blocksList[var4];

			for(int var5 = 0; var5 < 4; ++var5) {
				for(int var6 = 0; var6 < 4; ++var6) {
					for(int var7 = 0; var7 < 4; ++var7) {
						float var8 = (float)var1 + ((float)var5 + 0.5F) / 4.0F;
						float var9 = (float)var2 + ((float)var6 + 0.5F) / 4.0F;
						float var10 = (float)var3 + ((float)var7 + 0.5F) / 4.0F;
						addEffect(new EntityDiggingFX(worldObj, var8, var9, var10, var8 - (float)var1 - 0.5F, var9 - (float)var2 - 0.5F, var10 - (float)var3 - 0.5F, var11));
					}
				}
			}

		}
}

void EffectRenderer::addBlockHitEffects(int var1, int var2, int var3, int var4) {
		int var5 = worldObj->getBlockId(var1, var2, var3);
		if(var5 != 0) {
			Block var9 = Block::blocksList[var5];
			float var6 = (float)var1 + rand->nextFloat() * (var9.maxX - var9.minX - 0.2F) + 0.1F + var9.minX;
			float var7 = (float)var2 + rand->nextFloat() * (var9.maxY - var9.minY - 0.2F) + 0.1F + var9.minY;
			float var8 = (float)var3 + rand->nextFloat() * (var9.maxZ - var9.minZ - 0.2F) + 0.1F + var9.minZ;
			if(var4 == 0) {
				var7 = (float)var2 + var9.minY - 0.1F;
			}

			if(var4 == 1) {
				var7 = (float)var2 + var9.maxY + 0.1F;
			}

			if(var4 == 2) {
				var8 = (float)var3 + var9.minZ - 0.1F;
			}

			if(var4 == 3) {
				var8 = (float)var3 + var9.maxZ + 0.1F;
			}

			if(var4 == 4) {
				var6 = (float)var1 + var9.minX - 0.1F;
			}

			if(var4 == 5) {
				var6 = (float)var1 + var9.maxX + 0.1F;
			}

			addEffect((new EntityDiggingFX(worldObj, var6, var7, var8, 0.0F, 0.0F, 0.0F, var9)).multiplyVelocity(0.2F).multipleParticleScaleBy(0.6F));
		}
}

const std::string& EffectRenderer::getStatistics() {
		return "" + (m_fxLayers[0].size() + m_fxLayers[1].size() + m_fxLayers[2].size());
}
