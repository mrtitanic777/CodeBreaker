#ifndef RENDERGLOBAL_H
#define RENDERGLOBAL_H

// Auto-generated from RenderGlobal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IWorldAccess.h"

class AxisAlignedBB;
class Entity;
class EntityPlayer;
class ICamera;
class IntBuffer;
class ItemStack;
class Minecraft;
class MovingObjectPosition;
class RenderBlocks;
class RenderEngine;
class RenderList;
class TileEntity;
class Vec3D;
class World;
class WorldRenderer;

class RenderGlobal : public IWorldAccess {
public:
	RenderGlobal(Minecraft* var1, RenderEngine* var2);
	virtual ~RenderGlobal();

	JList<void*> tileEntities;
	World* worldObj;
	RenderEngine* renderEngine;
	JList<void*> worldRenderersToUpdate;
	JArray<WorldRenderer* > sortedWorldRenderers;
	JArray<WorldRenderer* > worldRenderers;
	int renderChunksWide;
	int renderChunksTall;
	int renderChunksDeep;
	int glRenderListBase;
	Minecraft* mc;
	RenderBlocks* globalRenderBlocks;
	IntBuffer* glOcclusionQueryBase;
	bool occlusionEnabled;
	int cloudTickCounter;
	int starGLCallList;
	int glSkyList;
	int glSkyList2;
	int minBlockX;
	int minBlockY;
	int minBlockZ;
	int maxBlockX;
	int maxBlockY;
	int maxBlockZ;
	int renderDistance;
	int renderEntitiesStartupCounter;
	int countEntitiesTotal;
	int countEntitiesRendered;
	int countEntitiesHidden;
	JArray<int > field_1457_b;
	IntBuffer* occlusionResultBuffer;
	int renderersLoaded;
	int renderersFrustumCulled;
	int renderersOccluded;
	int renderersRendered;
	int renderersEmpty;
	JList<void*> glRenderLists;
	JArray<RenderList* > allRenderLists;
	int field_1455_d;
	int field_1454_e;
	double prevSortX;
	double prevSortY;
	double prevSortZ;
	float damagePartialTime;
	int frustumCheckOffset;

	virtual void renderStars();
	virtual void setWorldAndLoadRenderers(World* var1);
	virtual void loadRenderers();
	virtual void renderEntities(Vec3D* var1, ICamera* var2, float var3);
	virtual JavaString getDebugInfoRenders();
	virtual JavaString getDebugInfoEntities();
	virtual void markRenderersForNewPosition(int var1, int var2, int var3);
	virtual int sortAndRender(EntityPlayer* var1, int var2, double var3);
	virtual void checkOcclusionQueryResults(int var1, int var2);
	virtual int renderSortedRenderers(int var1, int var2, int var3, double var4);
	virtual void renderAllRenderLists(int var1, double var2);
	virtual void updateClouds();
	virtual void renderSky(float var1);
	virtual void renderClouds(float var1);
	virtual void renderCloudsFancy(float var1);
	virtual bool updateRenderers(EntityPlayer* var1, bool var2);
	virtual void drawBlockBreaking(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5);
	virtual void drawSelectionBox(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5);
	virtual void drawOutlinedBoundingBox(AxisAlignedBB* var1);
	virtual void markBlocksForUpdate(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void markBlockNeedsUpdate(int var1, int var2, int var3);
	virtual void markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void clipRenderersByFrustum(ICamera* var1, float var2);
	virtual void playRecord(const JavaString& var1, int var2, int var3, int var4);
	virtual void playSound(const JavaString& var1, double var2, double var4, double var6, float var8, float var9);
	virtual void spawnParticle(const JavaString& var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void updateAllRenderers();
	virtual void onTileEntityChanged(int var1, int var2, int var3, TileEntity* var4);
};

#endif // RENDERGLOBAL_H