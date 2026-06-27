#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

// Auto-generated from RenderPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class Entity;
class EntityLiving;
class EntityPlayer;
class ModelBiped;

class RenderPlayer : public RenderLiving {
public:
	RenderPlayer();
	virtual ~RenderPlayer();

	ModelBiped* modelBipedMain;
	ModelBiped* modelArmorChestplate;
	ModelBiped* modelArmor;
	static JArray<JavaString > s_armorFilenamePrefix;

	virtual bool a(EntityPlayer* var1, int var2);
	virtual void a(EntityPlayer* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void a(EntityPlayer* var1, float var2);
	virtual void b(EntityPlayer* var1, float var2);
	virtual void drawFirstPersonHand();
	virtual void preRenderCallback(EntityLiving* var1, float var2);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);
	virtual void renderEquippedItems(EntityLiving* var1, float var2);
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERPLAYER_H