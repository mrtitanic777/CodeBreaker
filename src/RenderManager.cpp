#include "stdafx.h"
// Auto-generated from RenderManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderManager.h"
#include "Common.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "Map.h"
#include "RenderEngine.h"
#include "World.h"
#include "ModelBiped.h"
#include "RenderItem.h"
#include "Tesselator.h"
#include "EntityItem.h"

RenderManager* RenderManager::instance = new RenderManager();

RenderManager::RenderManager()
	: m_entityRenderMap(NULL)
	, m_renderEngine(NULL)
	, worldObj(NULL)
	, m_playerViewY(0)
	, m_viewerPosX(0)
	, m_viewerPosY(0)
	, m_viewerPosZ(0)
{
		// PORT-BLOCKED: m_entityRenderMap->put(EntitySpider.class, new RenderSpider());
		// PORT-BLOCKED: m_entityRenderMap->put(EntityPig.class, new RenderLiving(new ModelPig(), 0.7F));
		// PORT-BLOCKED: m_entityRenderMap->put(EntitySheep.class, new RenderSheep(new ModelSheep(), new ModelSheepFur(), 0.7F));
		// PORT-BLOCKED: m_entityRenderMap->put(EntityCreeper.class, new RenderCreeper());
		// PORT-BLOCKED: m_entityRenderMap->put(EntitySkeleton.class, new RenderLiving(new ModelSkeleton(), 0.5F));
		// PORT-BLOCKED: m_entityRenderMap->put(EntityZombie.class, new RenderLiving(new ModelZombie(), 0.5F));
		// PORT-BLOCKED: m_entityRenderMap->put(EntityPlayer.class, new RenderPlayer());
		// PORT-BLOCKED: m_entityRenderMap->put(EntityGiantZombie.class, new RenderGiantZombie(new ModelZombie(), 0.5F, 6.0F));
		// PORT-BLOCKED: m_entityRenderMap->put(EntityLiving.class, new RenderLiving(new ModelBiped(), 0.5F));
		// PORT-BLOCKED: m_entityRenderMap->put(Entity.class, new RenderEntity());
		// PORT-BLOCKED: m_entityRenderMap->put(EntityArrow.class, new RenderArrow());
		// PORT-BLOCKED: m_entityRenderMap->put(EntityItem.class, new RenderItem());
		// PORT-BLOCKED: m_entityRenderMap->put(EntityTNT.class, new RenderTNTPrimed());
		for (size_t _i1 = 0; _i1 < m_entityRenderMap->values().size(); ++_i1) {
			Render var2 = (Render*)var1.next();
			var2.setRenderManager(this);
		}

}

RenderManager::~RenderManager() {
	m_entityRenderMap = NULL;
	m_renderEngine = NULL;
	worldObj = NULL;
}

void RenderManager::cacheActiveRenderInfo(World* var1, RenderEngine* var2, EntityPlayer* var3, float var4) {
		worldObj = var1;
		m_renderEngine = var2;
		m_playerViewY = var3->prevRotationYaw + (var3->rotationYaw - var3->prevRotationYaw) * var4;
		m_viewerPosX = var3->lastTickPosX + (var3->posX - var3->lastTickPosX) * var4;
		m_viewerPosY = var3->lastTickPosY + (var3->posY - var3->lastTickPosY) * var4;
		m_viewerPosZ = var3->lastTickPosZ + (var3->posZ - var3->lastTickPosZ) * var4;
}

void RenderManager::renderEntity(Entity* var1, float var2) {
		float var3 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * var2;
		float var4 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * var2;
		float var5 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * var2;
		float var6 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
		float var7 = worldObj->getLightBrightness((int)var3, (int)(var4 + var1->getShadowSize()), (int)var5);
  Tesselator::getInstance().color3f(var7, var7, var7);
		m_a(var1, var3, var4, var5, var6, var2);
}

void RenderManager::a(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
		Class var8 = var1->getClass();
		Render var9 = (Render*)m_entityRenderMap[var8];
		// PORT-BLOCKED: if(var9 == NULL && var8 != Entity.class) {
			var9 = (Render*)m_entityRenderMap->get(var8.getSuperclass());
			// PORT-TODO: m_entityRenderMap->put(var8, var9);
		}

		if(var9 != NULL) {
			var9.doRender(var1, var2, var3, var4, var5, var6);
			var9.renderShadow(var1, var2, var3, var4, var6);
		}

}

void RenderManager::set(World* var1) {
		worldObj = var1;
}

float RenderManager::getDistanceToCamera(float var1, float var2, float var3) {
		var1 -= m_viewerPosX;
		var2 -= m_viewerPosY;
		var3 -= m_viewerPosZ;
		return var1 * var1 + var2 * var2 + var3 * var3;
}
