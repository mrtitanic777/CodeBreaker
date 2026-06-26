#include "stdafx.h"
// Hand-written PORT override for TileEntityRenderer.java (Minecraft Alpha v1.2.6)
// -----------------------------------------------------------------------------
// The Java TileEntityRenderer is the tile-entity special-renderer DISPATCH table: its ctor
// populates a Map keyed by the TileEntity's Class -> a TileEntitySpecialRenderer instance,
// and getSpecialRendererForClass/ForEntity look up + delegate. Java resolves the lookup by
// reflection (getClass()/getSuperclass()). C++ has no Class objects, so this rebuilds the
// dispatch as a dynamic_cast chain keyed by a stable string, exactly the way RenderManager's
// hand-written override does it (see handwritten_alpha\RenderManager.cpp).
//
// The converter-generated TileEntityRenderer.h interface is preserved unchanged, so every
// converted caller keeps working. Signatures, field names and return types match
// output_alpha\TileEntityRenderer.h exactly.
//
// Alpha registers only two special renderers (sign + mob spawner); there is no chest special
// renderer in this version. Each "new TileEntityXxxRenderer()" only needs the corresponding
// header to declare its ctor; the .cpp bodies are separate translation units.

#include "TileEntityRenderer.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "FontRenderer.h"
#include "RenderEngine.h"
#include "World.h"
#include "Tesselator.h"
#include "TileEntitySpecialRenderer.h"

// --- tile-entity types (for the dynamic_cast dispatch chain) ---
#include "TileEntity.h"
#include "TileEntitySign.h"
#include "TileEntityMobSpawner.h"

// --- special renderers (mirror TileEntityRenderer.java's specialRendererMap.put(...) calls) ---
#include "TileEntitySignRenderer.h"
#include "TileEntityMobSpawnerRenderer.h"

#include <map>
#include <string>

// Static members declared in TileEntityRenderer.h.
TileEntityRenderer* TileEntityRenderer::instance = new TileEntityRenderer();
double TileEntityRenderer::s_staticPlayerX = 0.0;
double TileEntityRenderer::s_staticPlayerY = 0.0;
double TileEntityRenderer::s_staticPlayerZ = 0.0;

TileEntityRenderer::TileEntityRenderer()
	: m_renderEngine(NULL)
	, worldObj(NULL)
	, m_entityPlayer(NULL)
	, m_playerYaw(0.0F)
	, m_playerPitch(0.0F)
	, m_playerX(0.0)
	, m_playerY(0.0)
	, m_playerZ(0.0)
	, m_specialRendererMap(new std::map<std::string, void*>())
	, fontRenderer(NULL)
{
	std::map<std::string, void*>& m = *m_specialRendererMap;

	// Mirrors TileEntityRenderer.java's specialRendererMap.put(...) registrations, keyed by a
	// stable string instead of a Java Class object (resolved by _teRenderKey at lookup time).
	m["sign"]    = (void*)(new TileEntitySignRenderer());
	m["spawner"] = (void*)(new TileEntityMobSpawnerRenderer());

	// Java: iterate the map and call renderer.setTileEntityRenderer(this) on every entry.
	for (std::map<std::string, void*>::iterator it = m.begin(); it != m.end(); ++it) {
		((TileEntitySpecialRenderer*)it->second)->setTileEntityRenderer(this);
	}
}

TileEntityRenderer::~TileEntityRenderer() {
	// Renderers in the map are owned process-wide (the singleton lives for the whole run);
	// mirror RenderManager's override and simply drop the borrowed back-references.
	m_specialRendererMap = NULL;
	fontRenderer = NULL;
	m_renderEngine = NULL;
	worldObj = NULL;
	m_entityPlayer = NULL;
}

// Replaces Java's getClass()/getSuperclass() Map lookup. Resolves a tile entity to the stable
// string key its special renderer was registered under. Returns NULL when no renderer applies
// (the common case: most tile entities have no special renderer).
static const char* _teRenderKey(TileEntity* te) {
	if (dynamic_cast<TileEntitySign*>(te))        return "sign";
	if (dynamic_cast<TileEntityMobSpawner*>(te))  return "spawner";
	return NULL;
}

// Java: getSpecialRendererForClass(Class) -> TileEntitySpecialRenderer. Reflection-based
// superclass walk. The C++ dispatch is done up-front in _teRenderKey (string keys are already
// resolved per entity), so here we just look the resolved key up in the map. The void*
// parameter is treated as a key-string pointer (matches how getSpecialRendererForEntity
// forwards it). Returns NULL when the key is unknown.
TileEntitySpecialRenderer* TileEntityRenderer::getSpecialRendererForClass(void* var1) {
	if (var1 == NULL) {
		return NULL;
	}
	const char* key = (const char*)var1;
	std::map<std::string, void*>::iterator it = m_specialRendererMap->find(key);
	return (it != m_specialRendererMap->end()) ? (TileEntitySpecialRenderer*)it->second : NULL;
}

bool TileEntityRenderer::hasSpecialRenderer(TileEntity* var1) {
	return getSpecialRendererForEntity(var1) != NULL;
}

// Java: getSpecialRendererForEntity(TileEntity) -> getSpecialRendererForClass(entity.getClass()).
// Here we resolve the entity to its string key and look the renderer up directly.
TileEntitySpecialRenderer* TileEntityRenderer::getSpecialRendererForEntity(TileEntity* var1) {
	if (var1 == NULL) {
		return NULL;
	}
	const char* key = _teRenderKey(var1);
	return (key != NULL) ? getSpecialRendererForClass((void*)key) : NULL;
}

void TileEntityRenderer::setRenderingContext(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, float var5) {
	worldObj = var1;
	m_renderEngine = var2;
	m_entityPlayer = var4;
	fontRenderer = var3;
	if (var4 != NULL) {
		m_playerYaw   = var4->prevRotationYaw   + (var4->rotationYaw   - var4->prevRotationYaw)   * var5;
		m_playerPitch = var4->prevRotationPitch + (var4->rotationPitch - var4->prevRotationPitch) * var5;
		m_playerX = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var5;
		m_playerY = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var5;
		m_playerZ = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var5;
	}
}

void TileEntityRenderer::renderTileEntity(TileEntity* var1, float var2) {
	if (var1 == NULL) {
		return;  // PORT: guard against null tile entities
	}
	if (var1->getDistanceFrom(m_playerX, m_playerY, m_playerZ) < 4096.0) {
		float var3 = (worldObj != NULL)
			? worldObj->getLightBrightness(var1->xCoord, var1->yCoord, var1->zCoord)
			: 1.0F;
		Tesselator::getInstance().color3f(var3, var3, var3);
		renderTileEntityAt(var1,
		                   (double)var1->xCoord - s_staticPlayerX,
		                   (double)var1->yCoord - s_staticPlayerY,
		                   (double)var1->zCoord - s_staticPlayerZ,
		                   var2);
	}
}

void TileEntityRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8) {
	TileEntitySpecialRenderer* var9 = getSpecialRendererForEntity(var1);
	if (var9 != NULL) {
		var9->renderTileEntityAt(var1, var2, var4, var6, var8);
	}
}

FontRenderer* TileEntityRenderer::getFontRenderer() {
	return fontRenderer;
}
