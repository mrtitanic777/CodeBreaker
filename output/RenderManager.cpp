#include "stdafx.h"
// Hand-written PORT override for RenderManager.java
// -------------------------------------------------
// The Java RenderManager dispatches entity rendering by reflection: a Map keyed by the
// entity's Class, looked up via getClass()/getSuperclass(). C++ has no Class objects, so
// this rebuilds the dispatch as a dynamic_cast chain (most-derived first) that maps each
// entity to a string-keyed Render*. The converter-generated RenderManager.h interface is
// preserved unchanged, so all callers (RenderGlobal, etc.) keep working.

#include "RenderManager.h"
#include "Common.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "RenderEngine.h"
#include "World.h"
#include "Tesselator.h"
#include "Render.h"
// entity types (for dynamic_cast)
#include "EntitySpider.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntityCreeper.h"
#include "EntitySkeleton.h"
#include "EntityZombie.h"
#include "EntityGiantZombie.h"
#include "EntityArrow.h"
#include "EntityItem.h"
#include "EntityTNT.h"
// renderers + models
#include "RenderSpider.h"
#include "RenderLiving.h"
#include "RenderSheep.h"
#include "RenderCreeper.h"
#include "RenderPlayer.h"
#include "RenderGiantZombie.h"
#include "RenderEntity.h"
#include "RenderArrow.h"
#include "RenderItem.h"
#include "RenderTNTPrimed.h"
#include "ModelBiped.h"
#include "ModelPig.h"
#include "ModelSheep.h"
#include "ModelSheepFur.h"
#include "ModelSkeleton.h"
#include "ModelZombie.h"
#include <map>
#include <string>

RenderManager* RenderManager::instance = new RenderManager();

RenderManager::RenderManager()
	: m_renderEngine(NULL)
	, worldObj(NULL)
	, m_playerViewY(0)
	, m_entityRenderMap(new std::map<std::string, void*>())
	, m_viewerPosX(0)
	, m_viewerPosY(0)
	, m_viewerPosZ(0)
{
	std::map<std::string, void*>& m = *m_entityRenderMap;
	m["spider"]   = (void*)(new RenderSpider());
	m["pig"]      = (void*)(new RenderLiving(new ModelPig(), 0.7F));
	m["sheep"]    = (void*)(new RenderSheep(new ModelSheep(), new ModelSheepFur(), 0.7F));
	m["creeper"]  = (void*)(new RenderCreeper());
	m["skeleton"] = (void*)(new RenderLiving(new ModelSkeleton(), 0.5F));
	m["zombie"]   = (void*)(new RenderLiving(new ModelZombie(), 0.5F));
	m["player"]   = (void*)(new RenderPlayer());
	m["giant"]    = (void*)(new RenderGiantZombie(new ModelZombie(), 0.5F, 6.0F));
	m["living"]   = (void*)(new RenderLiving(new ModelBiped(), 0.5F));
	m["entity"]   = (void*)(new RenderEntity());
	m["arrow"]    = (void*)(new RenderArrow());
	m["item"]     = (void*)(new RenderItem());
	m["tnt"]      = (void*)(new RenderTNTPrimed());
	for (std::map<std::string, void*>::iterator it = m.begin(); it != m.end(); ++it) {
		((Render*)it->second)->setRenderManager(this);
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

// Replaces Java's getClass()/getSuperclass() lookup. Most-derived first so subclasses
// resolve to their own renderer before the EntityLiving/Entity fallbacks.
static const char* _rmRenderKey(Entity* e) {
	if (dynamic_cast<EntitySpider*>(e))      return "spider";
	if (dynamic_cast<EntityCreeper*>(e))     return "creeper";
	if (dynamic_cast<EntityGiantZombie*>(e)) return "giant";
	if (dynamic_cast<EntitySkeleton*>(e))    return "skeleton";
	if (dynamic_cast<EntityZombie*>(e))      return "zombie";
	if (dynamic_cast<EntityPig*>(e))         return "pig";
	if (dynamic_cast<EntitySheep*>(e))       return "sheep";
	if (dynamic_cast<EntityPlayer*>(e))      return "player";
	if (dynamic_cast<EntityArrow*>(e))       return "arrow";
	if (dynamic_cast<EntityItem*>(e))        return "item";
	if (dynamic_cast<EntityTNT*>(e))         return "tnt";
	if (dynamic_cast<EntityLiving*>(e))      return "living";
	return "entity";
}

void RenderManager::renderEntity(Entity* var1, float var2) {
	float var3 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * var2;
	float var4 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * var2;
	float var5 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * var2;
	float var6 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
	float var7 = worldObj->getLightBrightness((int)var3, (int)(var4 + var1->getShadowSize()), (int)var5);
	Tesselator::getInstance().color3f(var7, var7, var7);
	a(var1, var3, var4, var5, var6, var2);
}

void RenderManager::a(Entity* var1, float var2, float var3, float var4, float var5, float var6) {
	std::map<std::string, void*>::iterator it = m_entityRenderMap->find(_rmRenderKey(var1));
	Render* var9 = (it != m_entityRenderMap->end()) ? (Render*)it->second : NULL;
	if (var9 != NULL) {
		var9->doRender(var1, var2, var3, var4, var5, var6);
		var9->renderShadow(var1, var2, var3, var4, var6);
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
