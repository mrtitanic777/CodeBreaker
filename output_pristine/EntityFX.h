#ifndef ENTITYFX_H
#define ENTITYFX_H

// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include <string>

class NBTTagCompound;
class Tesselator;
class World;

class EntityFX : public Entity {
public:
	EntityFX();
	EntityFX(World* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual ~EntityFX();

	virtual EntityFX* multiplyVelocity(float var1);
	virtual EntityFX* multipleParticleScaleBy(float var1);
	virtual void onEntityUpdate();
	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual int getFXLayer();

protected:
	float motionX1;
	float motionY1;
	float motionZ1;
	int P;
	float m_particleTextureJitterX;
	float m_particleTextureJitterY;
	int particleAge;
	int particleMaxAge;
	float particleScale;
	float particleGravity;
	float particleRed;
	float particleGreen;
	float particleBlue;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual void readEntityFromNBT(NBTTagCompound* var1);

};

#endif // ENTITYFX_H