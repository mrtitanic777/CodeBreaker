#ifndef ENTITYFX_H
#define ENTITYFX_H

// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class NBTTagCompound;
class Tessellator;
class World;

class EntityFX : public Entity {
public:
	EntityFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityFX();

	int particleTextureIndex;
	float particleTextureJitterX;
	float particleTextureJitterY;
	int e;
	int particleMaxAge;
	float particleScale;
	float particleGravity;
	float particleRed;
	float particleGreen;
	float particleBlue;
	static double s_interpPosX;
	static double s_interpPosY;
	static double s_interpPosZ;

	virtual EntityFX* multiplyVelocity(float var1);
	virtual EntityFX* multipleParticleScaleBy(float var1);
	virtual void onUpdate();
	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual int getFXLayer();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

	static void initStatics();
};

#endif // ENTITYFX_H