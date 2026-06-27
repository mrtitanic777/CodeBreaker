#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

// Auto-generated from RenderManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class Class;
class Entity;
class EntityPlayer;
class FontRenderer;
class GameSettings;
class ItemRenderer;
class Render;
class RenderEngine;
class World;

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager();

	std::map<std::string, void*> entityRenderMap;
	FontRenderer* fontRenderer;
	RenderEngine* renderEngine;
	ItemRenderer* itemRenderer;
	World* worldObj;
	EntityPlayer* livingPlayer;
	float playerViewYaw;
	float playerViewPitch;
	GameSettings* options;
	double playerViewX;
	double playerViewY;
	double playerViewZ;
	static RenderManager* s_instance;
	static double s_renderPosX;
	static double s_renderPosY;
	static double s_renderPosZ;

	virtual Render* getEntityClassRenderObject(Class* var1);
	virtual Render* getEntityRenderObject(Entity* var1);
	virtual void cacheActiveRenderInfo(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, GameSettings* var5, float var6);
	virtual void renderEntity(Entity* var1, float var2);
	virtual void doRenderEntity(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void setWorld(World* var1);
	virtual double getDistanceToCamera(double var1, double var3, double var5);
	virtual FontRenderer* getFontRenderer();
};

#endif // RENDERMANAGER_H