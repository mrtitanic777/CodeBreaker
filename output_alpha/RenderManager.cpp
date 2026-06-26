#include "stdafx.h"
// Hand-written PORT override for RenderManager.java (Minecraft Alpha v1.2.6)
// -------------------------------------------------------------------------
// The Java RenderManager is the entity-renderer DISPATCH table: its ctor populates a Map
// keyed by the entity's Class -> a Render* instance, and func_855_a/func_854_a/func_853_a
// look up + delegate. Java resolves the lookup by reflection (getClass()/getSuperclass()).
// C++ has no Class objects, so this rebuilds the dispatch as a dynamic_cast chain
// (most-derived first) keyed by a stable string, exactly the way the indev override does it.
//
// The converter-generated RenderManager.h interface is preserved unchanged, so every
// converted caller (RenderGlobal / EntityRenderer / etc.) keeps working. Signatures, field
// names (m_xxx / func_XXX) and return types match output_alpha\RenderManager.h exactly.
//
// Only RenderManager's OWN methods are implemented here. Each "new RenderXxx(...)" below
// only needs the corresponding RenderXxx.h to declare that ctor; the RenderXxx.cpp bodies
// are separate translation units, so this file just needs the headers to compile.

#include "RenderManager.h"
#include "Common.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "RenderEngine.h"
#include "World.h"
#include "Tesselator.h"
#include "Render.h"

// --- entity types (for the dynamic_cast dispatch chain) ---
#include "EntitySpider.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntityCow.h"
#include "EntityChicken.h"
#include "EntityCreeper.h"
#include "EntitySkeleton.h"
#include "EntityZombie.h"
#include "EntitySlime.h"
#include "EntityZombieSimple.h"
#include "EntityGhast.h"
#include "EntityPainting.h"
#include "EntityArrow.h"
#include "EntitySnowball.h"
#include "EntityFireball.h"
#include "EntityItem.h"
#include "EntityTNTPrimed.h"
#include "EntityFallingSand.h"
#include "EntityMinecart.h"
#include "EntityBoat.h"
#include "EntityFish.h"

// --- renderers (mirror RenderManager.java's entityRenderMap.put(...) calls) ---
#include "RenderSpider.h"
#include "RenderPig.h"
#include "RenderSheep.h"
#include "RenderCow.h"
#include "RenderChicken.h"
#include "RenderCreeper.h"
#include "RenderBiped.h"
#include "RenderSlime.h"
#include "RenderPlayer.h"
#include "RenderZombieSimple.h"
#include "RenderGhast.h"
#include "RenderLiving.h"
#include "RenderEntity.h"
#include "RenderPainting.h"
#include "RenderArrow.h"
#include "RenderSnowball.h"
#include "RenderFireball.h"
#include "RenderItem.h"
#include "RenderTNTPrimed.h"
#include "RenderFallingSand.h"
#include "RenderMinecart.h"
#include "RenderBoat.h"
#include "RenderFish.h"

// --- models the renderers are constructed with ---
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelPig.h"
#include "ModelSheep1.h"
#include "ModelSheep2.h"
#include "ModelCow.h"
#include "ModelChicken.h"
#include "ModelSkeleton.h"
#include "ModelZombie.h"
#include "ModelSlime.h"

#include <map>
#include <string>

// Static members declared in RenderManager.h.
RenderManager* RenderManager::instance       = new RenderManager();
double         RenderManager::s_field_1232_b = 0.0;
double         RenderManager::s_field_1231_c = 0.0;
double         RenderManager::s_field_1230_d = 0.0;

RenderManager::RenderManager()
	: m_renderEngine(NULL)
	, m_field_4236_f(NULL)
	, worldObj(NULL)
	, m_field_1226_h(NULL)
	, m_field_1225_i(0.0F)
	, m_field_1224_j(0.0F)
	, m_options(NULL)
	, m_field_1222_l(0.0)
	, m_field_1221_m(0.0)
	, m_field_1220_n(0.0)
	, m_entityRenderMap(new std::map<std::string, void*>())
	, m_field_1218_p(NULL)
{
	std::map<std::string, void*>& m = *m_entityRenderMap;

	// Mirrors RenderManager.java's entityRenderMap.put(...) registrations, keyed by a stable
	// string instead of a Java Class object (resolved by _rmRenderKey below at lookup time).
	m["spider"]   = (void*)(new RenderSpider());
	m["pig"]      = (void*)(new RenderPig(new ModelPig(), new ModelPig(0.5F), 0.7F));
	m["sheep"]    = (void*)(new RenderSheep(new ModelSheep2(), new ModelSheep1(), 0.7F));
	m["cow"]      = (void*)(new RenderCow(new ModelCow(), 0.7F));
	m["chicken"]  = (void*)(new RenderChicken(new ModelChicken(), 0.3F));
	m["creeper"]  = (void*)(new RenderCreeper());
	m["skeleton"] = (void*)(new RenderBiped(new ModelSkeleton(), 0.5F));
	m["zombie"]   = (void*)(new RenderBiped(new ModelZombie(), 0.5F));
	m["slime"]    = (void*)(new RenderSlime(new ModelSlime(16), new ModelSlime(0), 0.25F));
	m["player"]   = (void*)(new RenderPlayer());
	m["giant"]    = (void*)(new RenderZombieSimple(new ModelZombie(), 0.5F, 6.0F));
	m["ghast"]    = (void*)(new RenderGhast());
	m["living"]   = (void*)(new RenderLiving(new ModelBiped(), 0.5F));
	m["entity"]   = (void*)(new RenderEntity());
	m["painting"] = (void*)(new RenderPainting());
	m["arrow"]    = (void*)(new RenderArrow());
	m["snowball"] = (void*)(new RenderSnowball());
	m["fireball"] = (void*)(new RenderFireball());
	m["item"]     = (void*)(new RenderItem());
	m["tnt"]      = (void*)(new RenderTNTPrimed());
	m["sand"]     = (void*)(new RenderFallingSand());
	m["minecart"] = (void*)(new RenderMinecart());
	m["boat"]     = (void*)(new RenderBoat());
	m["fish"]     = (void*)(new RenderFish());

	// Java: iterate the map and call render.setRenderManager(this) on every entry.
	for (std::map<std::string, void*>::iterator it = m.begin(); it != m.end(); ++it) {
		((Render*)it->second)->setRenderManager(this);
	}
}

RenderManager::~RenderManager() {
	// Renderers in the map are owned process-wide (the singleton lives for the whole run);
	// mirror the indev override and simply drop the borrowed back-references.
	m_entityRenderMap = NULL;
	m_renderEngine    = NULL;
	worldObj          = NULL;
	m_field_1226_h    = NULL;
	m_options         = NULL;
	m_field_1218_p    = NULL;
}

// Replaces Java's getClass()/getSuperclass() Map lookup. Most-derived classes are tested
// first so a subclass resolves to its own renderer before the EntityLiving/Entity fallbacks.
// EntityZombieSimple is tested before EntityZombie (in Java the "giant" is its own class).
static const char* _rmRenderKey(Entity* e) {
	if (dynamic_cast<EntitySpider*>(e))       return "spider";
	if (dynamic_cast<EntityCreeper*>(e))      return "creeper";
	if (dynamic_cast<EntityZombieSimple*>(e)) return "giant";
	if (dynamic_cast<EntitySkeleton*>(e))     return "skeleton";
	if (dynamic_cast<EntityZombie*>(e))       return "zombie";
	if (dynamic_cast<EntityGhast*>(e))        return "ghast";
	if (dynamic_cast<EntitySlime*>(e))        return "slime";
	if (dynamic_cast<EntityPig*>(e))          return "pig";
	if (dynamic_cast<EntitySheep*>(e))        return "sheep";
	if (dynamic_cast<EntityCow*>(e))          return "cow";
	if (dynamic_cast<EntityChicken*>(e))      return "chicken";
	if (dynamic_cast<EntityPlayer*>(e))       return "player";
	if (dynamic_cast<EntityPainting*>(e))     return "painting";
	if (dynamic_cast<EntityArrow*>(e))        return "arrow";
	if (dynamic_cast<EntitySnowball*>(e))     return "snowball";
	if (dynamic_cast<EntityFireball*>(e))     return "fireball";
	if (dynamic_cast<EntityItem*>(e))         return "item";
	if (dynamic_cast<EntityTNTPrimed*>(e))    return "tnt";
	if (dynamic_cast<EntityFallingSand*>(e))  return "sand";
	if (dynamic_cast<EntityMinecart*>(e))     return "minecart";
	if (dynamic_cast<EntityBoat*>(e))         return "boat";
	if (dynamic_cast<EntityFish*>(e))         return "fish";
	if (dynamic_cast<EntityLiving*>(e))       return "living";
	return "entity";
}

// Java: func_4117_a(Class) -> Render. Reflection-based superclass walk. The C++ dispatch is
// done up-front in _rmRenderKey (string keys are already resolved per entity), so here we
// just look the resolved key up in the map. The Class* parameter is treated as a key string
// pointer (matches how func_855_a forwards it). Falls back to the "entity" renderer.
Render* RenderManager::func_4117_a(void* var1) {
	if (var1 == NULL) {
		return NULL;
	}
	const char* key = (const char*)var1;
	std::map<std::string, void*>::iterator it = m_entityRenderMap->find(key);
	if (it != m_entityRenderMap->end()) {
		return (Render*)it->second;
	}
	// Java caches the resolved Entity.class fallback into the map; we just return it.
	std::map<std::string, void*>::iterator fb = m_entityRenderMap->find("entity");
	return (fb != m_entityRenderMap->end()) ? (Render*)fb->second : NULL;
}

// Java: func_855_a(Entity) -> func_4117_a(entity.getClass()). Here we resolve the entity to
// its string key and look the Render* up directly.
Render* RenderManager::func_855_a(Entity* var1) {
	if (var1 == NULL) {
		return NULL;
	}
	std::map<std::string, void*>::iterator it = m_entityRenderMap->find(_rmRenderKey(var1));
	return (it != m_entityRenderMap->end()) ? (Render*)it->second : NULL;
}

// Java: func_857_a(World, RenderEngine, FontRenderer, EntityPlayer, GameSettings, float) =
// cacheActiveRenderInfo. Stores the per-frame render context and the interpolated viewer
// position / orientation.
void RenderManager::func_857_a(World* var1, RenderEngine* var2, FontRenderer* var3,
                               EntityPlayer* var4, GameSettings* var5, float var6) {
	worldObj       = var1;
	m_renderEngine = var2;
	m_options      = var5;
	m_field_1226_h = var4;
	m_field_1218_p = var3;
	if (var4 != NULL) {
		m_field_1225_i = var4->prevRotationYaw   + (var4->rotationYaw   - var4->prevRotationYaw)   * var6;
		m_field_1224_j = var4->prevRotationPitch + (var4->rotationPitch - var4->prevRotationPitch) * var6;
		m_field_1222_l = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var6;
		m_field_1221_m = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var6;
		m_field_1220_n = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var6;
	}
}

// Java: func_854_a(Entity, float) = renderEntity. Interpolates the entity's position/yaw,
// fetches its brightness, sets the vertex colour and forwards to func_853_a, offsetting by
// the static viewer origin (field_1232_b/c/d).
void RenderManager::func_854_a(Entity* var1, float var2) {
	if (var1 == NULL) {
		return;  // PORT: guard against unregistered / null entities
	}
	double var3  = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
	double var5  = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
	double var7  = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;
	float  var9  = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
	float  var10 = var1->getEntityBrightness(var2);
	Tesselator::getInstance().color3f(var10, var10, var10);
	func_853_a(var1,
	           var3 - s_field_1232_b,
	           var5 - s_field_1231_c,
	           var7 - s_field_1230_d,
	           var9, var2);
}

// Java: func_853_a(Entity, double, double, double, float, float) = renderEntityWithPosYaw.
// Resolves the Render* for the entity and delegates the actual draw + shadow/fire pass.
void RenderManager::func_853_a(Entity* var1, double var2, double var4, double var6,
                               float var8, float var9) {
	Render* var10 = func_855_a(var1);
	if (var10 != NULL) {
		var10->doRender(var1, var2, var4, var6, var8, var9);
		var10->doRenderShadowAndFire(var1, var2, var4, var6, var8, var9);
	}
}

// Java: func_852_a(World) = set(World).
void RenderManager::func_852_a(World* var1) {
	worldObj = var1;
}

// Java: func_851_a(double, double, double) = getDistanceToCamera (squared distance from the
// interpolated viewer position).
double RenderManager::func_851_a(double var1, double var3, double var5) {
	double var7  = var1 - m_field_1222_l;
	double var9  = var3 - m_field_1221_m;
	double var11 = var5 - m_field_1220_n;
	return var7 * var7 + var9 * var9 + var11 * var11;
}

// Java: getFontRenderer() -> field_1218_p.
FontRenderer* RenderManager::getFontRenderer() {
	return m_field_1218_p;
}
