#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

// Auto-generated from RenderPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"
#include <string>

class Entity;
class EntityLiving;
class EntityPlayer;
class ModelBiped;

class RenderPlayer : public RenderLiving {
public:
	RenderPlayer();
	virtual ~RenderPlayer();

	virtual void a(EntityPlayer* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void drawFirstPersonHand();
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was protected
	virtual bool a(EntityPlayer* var1, int var2);
	virtual void a(EntityPlayer* var1, float var2);
	virtual void b(EntityPlayer* var1, float var2);
	virtual void func_6330_a(EntityLiving* var1, float var2);
	virtual bool func_166_a(EntityLiving* var1, int var2);
	virtual void func_6331_b(EntityLiving* var1, float var2);

public: // was private
	ModelBiped* m_field_209_f;
	ModelBiped* m_field_208_g;
	ModelBiped* m_field_207_h;
	static std::string* s_armorFilenamePrefix;

};

#endif // RENDERPLAYER_H