#ifndef TILEENTITYRENDERER_H
#define TILEENTITYRENDERER_H

// Auto-generated from TileEntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

class Class;
class EntityPlayer;
class FontRenderer;
class Map;
class RenderEngine;
class TileEntity;
class TileEntitySpecialRenderer;
class World;

class TileEntityRenderer {
public:
	TileEntityRenderer();
	virtual ~TileEntityRenderer();

	static TileEntityRenderer* instance;
	static double s_staticPlayerX;
	static double s_staticPlayerY;
	static double s_staticPlayerZ;
	RenderEngine* m_renderEngine;
	World* worldObj;
	EntityPlayer* m_entityPlayer;
	float m_playerYaw;
	float m_playerPitch;
	double m_playerX;
	double m_playerY;
	double m_playerZ;

	virtual TileEntitySpecialRenderer* getSpecialRendererForClass(void* var1);
	virtual bool hasSpecialRenderer(TileEntity* var1);
	virtual TileEntitySpecialRenderer* getSpecialRendererForEntity(TileEntity* var1);
	virtual void setRenderingContext(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, float var5);
	virtual void renderTileEntity(TileEntity* var1, float var2);
	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8);
	virtual FontRenderer* getFontRenderer();

public: // was private
	std::map<std::string, void*>* m_specialRendererMap;
	FontRenderer* fontRenderer;


};

#endif // TILEENTITYRENDERER_H