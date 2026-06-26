#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

// Auto-generated from RenderManager.java
// Target: Xbox 360 / C++03 / DirectX 9

class Class;
class Entity;
class EntityPlayer;
class FontRenderer;
class GameSettings;
class ItemRenderer;
class Map;
class Render;
class RenderEngine;
class World;

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager();

	static RenderManager* instance;
	static double s_field_1232_b;
	static double s_field_1231_c;
	static double s_field_1230_d;
	RenderEngine* m_renderEngine;
	ItemRenderer* m_field_4236_f;
	World* worldObj;
	EntityPlayer* m_field_1226_h;
	float m_field_1225_i;
	float m_field_1224_j;
	GameSettings* m_options;
	double m_field_1222_l;
	double m_field_1221_m;
	double m_field_1220_n;

	virtual Render* func_4117_a(void* var1);
	virtual Render* func_855_a(Entity* var1);
	virtual void func_857_a(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, GameSettings* var5, float var6);
	virtual void func_854_a(Entity* var1, float var2);
	virtual void func_853_a(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void func_852_a(World* var1);
	virtual double func_851_a(double var1, double var3, double var5);
	virtual FontRenderer* getFontRenderer();

public: // was private
	std::map<std::string, void*>* m_entityRenderMap;
	FontRenderer* m_field_1218_p;


};

#endif // RENDERMANAGER_H