#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

// Auto-generated from WorldRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "AxisAlignedBB.h"

class EntityLiving;
class ICamera;
class RenderBlocks;
class Tesselator;
class World;

class WorldRenderer {
public:
	WorldRenderer();
	WorldRenderer(World* var1, int var2, int var3, int var4, int var5, int var6);
	virtual ~WorldRenderer();

	static int chunksUpdated;
	bool isInFrustrum;
	bool needsUpdate;
	bool isVisible;
	bool isWaitingOnOcclusionQuery;
	int glOcclusionQuery;

	virtual void updateRenderer();
	virtual float distanceToEntitySquared(EntityLiving* var1);
	virtual void stopRendering();
	virtual int getGLCallListForPass(int* var1, int var2, int var3);
	virtual void updateInFrustrum(ICamera* var1);
	virtual void callOcclusionQueryList();

private:
	World* worldObj;
	int m_glRenderList;
	static Tesselator* s_tessellator;
	int posX;
	int posY;
	int posZ;
	int m_sizeWidth;
	int m_sizeHeight;
	int m_sizeDepth;
	bool* m_skipRenderPass;
	int m_posXPlus;
	int m_posYPlus;
	int m_posZPlus;
	AxisAlignedBB m_rendererBoundingBox;
	bool m_hasRendererBoundingBox;
	RenderBlocks* m_renderBlocks;

	void setDontDraw();

public:  // array-length companions
	int m_skipRenderPass_count;

};

#endif // WORLDRENDERER_H