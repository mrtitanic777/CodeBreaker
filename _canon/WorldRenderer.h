#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

// Auto-generated from WorldRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class AxisAlignedBB;
class Entity;
class ICamera;
class Tessellator;
class World;

class WorldRenderer {
public:
	WorldRenderer(World* var1, JList<void*> var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~WorldRenderer();

	World* worldObj;
	int glRenderList;
	int posX;
	int posY;
	int posZ;
	int sizeWidth;
	int sizeHeight;
	int sizeDepth;
	int posXClamped;
	int posYClamped;
	int posZClamped;
	int posXMinus;
	int posYMinus;
	int posZMinus;
	bool isVisibleFromCamera;
	JArray<bool > skipRenderPass;
	int posXClip;
	int posYClip;
	int posZClip;
	float rendererRadius;
	bool needsUpdate;
	AxisAlignedBB* rendererBoundingBox;
	int chunkIndex;
	bool isVisible;
	bool isInFrustum;
	int glRenderListBase;
	bool isWaitingOnOcclusionQuery;
	bool isInitialized;
	JList<void*> tileEntityRenderers;
	JList<void*> tileEntities;
	static Tessellator* s_tessellator;
	static int s_chunksUpdated;

	virtual void setPosition(int var1, int var2, int var3);
	virtual void translateToOrigin();
	virtual void updateRenderer();
	virtual float distanceToEntitySquared(Entity* var1);
	virtual void markPassesDirty();
	virtual void setDontDraw();
	virtual int getGLCallList(int var1);
	virtual void updateInFrustum(ICamera* var1);
	virtual void callOcclusionQueryList();
	virtual bool skipAllRenderPasses();
	virtual void MarkDirty();
};

#endif // WORLDRENDERER_H