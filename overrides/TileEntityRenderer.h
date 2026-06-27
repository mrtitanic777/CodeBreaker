#ifndef TILEENTITYRENDERER_H
#define TILEENTITYRENDERER_H

// Auto-generated from TileEntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class Class;
class EntityPlayer;
class FontRenderer;
class RenderEngine;
class TileEntity;
class TileEntitySpecialRenderer;
class World;

class TileEntityRenderer {
public:
	TileEntityRenderer();
	virtual ~TileEntityRenderer();

	std::map<std::string, void*> specialRendererMap;
	FontRenderer* fontRenderer;
	RenderEngine* renderEngine;
	World* worldObj;
	EntityPlayer* entityPlayer;
	float playerYaw;
	float playerPitch;
	double playerX;
	double playerY;
	double playerZ;
	static TileEntityRenderer* s_instance;
	static double s_staticPlayerX;
	static double s_staticPlayerY;
	static double s_staticPlayerZ;

	virtual TileEntitySpecialRenderer* getSpecialRendererForClass(Class* var1);
	virtual bool hasSpecialRenderer(TileEntity* var1);
	virtual TileEntitySpecialRenderer* getSpecialRendererForEntity(TileEntity* var1);
	virtual void setRenderingContext(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, float var5);
	virtual void renderTileEntity(TileEntity* var1, float var2);
	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8);
	virtual FontRenderer* getFontRenderer();
};

#endif // TILEENTITYRENDERER_H