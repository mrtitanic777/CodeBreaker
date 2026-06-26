#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

// Auto-generated from RenderManager.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class EntityPlayer;
class Map;
class RenderEngine;
class World;

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager();

	static RenderManager* instance;
	RenderEngine* m_renderEngine;
	World* worldObj;
	float m_playerViewY;

	virtual void cacheActiveRenderInfo(World* var1, RenderEngine* var2, EntityPlayer* var3, float var4);
	virtual void renderEntity(Entity* var1, float var2);
	virtual void a(Entity* var1, float var2, float var3, float var4, float var5, float var6);
	virtual void set(World* var1);
	virtual float getDistanceToCamera(float var1, float var2, float var3);

private:
	Map* m_entityRenderMap;
	float m_viewerPosX;
	float m_viewerPosY;
	float m_viewerPosZ;


};

#endif // RENDERMANAGER_H