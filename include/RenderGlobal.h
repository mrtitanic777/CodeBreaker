#ifndef RENDERGLOBAL_H
#define RENDERGLOBAL_H

// Auto-generated from RenderGlobal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IWorldAccess.h"
#include <string>
#include <vector>

class Entity;
class EntityPlayer;
class ICamera;
class ItemStack;
class Minecraft;
struct MovingObjectPosition;
class RenderBlocks;
class RenderEngine;
struct Vec3D;
class World;
class WorldRenderer;

class RenderGlobal : public IWorldAccess {
public:
	RenderGlobal();
	RenderGlobal(Minecraft* var1, RenderEngine* var2);
	virtual ~RenderGlobal();

	float m_damagePartialTime;

	virtual void changeWorld(World* var1);
	virtual void loadRenderers();
	virtual void renderEntities(const Vec3D& var1, ICamera* var2, float var3);
	virtual const std::string& getDebugInfoRenders();
	virtual const std::string& getDebugInfoEntities();
	virtual int sortAndRender(EntityPlayer* var1, int var2);
	virtual void renderAllRenderLists();
	virtual void updateClouds();
	virtual void renderSky(float var1);
	virtual void oobGroundRenderer();
	virtual void oobWaterRenderer();
	virtual void updateRenderers(EntityPlayer* var1);
	virtual void drawBlockBreaking(const MovingObjectPosition& var1, int var2, const ItemStack& var3);
	virtual void drawSelectionBox(const MovingObjectPosition& var1, int var2);
	virtual void markBlockAndNeighborsNeedsUpdate(int var1, int var2, int var3);
	virtual void markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void clipRenderersByFrustrum(ICamera* var1);
	virtual void playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6);
	virtual void spawnParticle(const std::string& var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void playMusic(const std::string& var1, float var2, float var3, float var4, float var5);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void updateAllRenderers();

private:
	World* worldObj;
	RenderEngine* m_renderEngine;
	int m_glGenList;
	int* /* PORT: was IntBuffer */ m_renderIntBuffer;
	std::vector<void*> /* PORT: determine element type */ m_worldRenderersToUpdate;
	WorldRenderer* m_sortedWorldRenderers;
	WorldRenderer* m_worldRenderers;
	int m_renderChunksWide;
	int m_renderChunksTall;
	int m_renderChunksDeep;
	int m_glRenderListBase;
	Minecraft* mc;
	RenderBlocks* m_globalRenderBlocks;
	int* /* PORT: was IntBuffer */ m_glOcclusionQueryBase;
	bool m_occlusionEnabled;
	int m_cloudOffsetX;
	int m_glSkyList;
	int m_countEntitiesTotal;
	int m_countEntitiesRendered;
	int m_countEntitiesHidden;
	int* m_dummyBuf50k;
	int* /* PORT: was IntBuffer */ m_occlusionResult;
	int m_renderersLoaded;
	int m_renderersBeingClipped;
	int m_renderersBeingOccluded;
	int m_renderersBeingRendered;
	float m_prevSortX;
	float m_prevSortY;
	float m_prevSortZ;

	void checkOcclusionQueryResult(int var1, int var2);
	int renderSortedRenderers(int var1, int var2, int var3);
	void oobGroundRenderHeight();
	void oobWaterRenderHeight();
	void markBlocksForUpdate(int var1, int var2, int var3, int var4, int var5, int var6);

};

#endif // RENDERGLOBAL_H