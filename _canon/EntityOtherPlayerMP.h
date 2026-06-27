#ifndef ENTITYOTHERPLAYERMP_H
#define ENTITYOTHERPLAYERMP_H

// Auto-generated from EntityOtherPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityPlayer.h"

class Entity;
class World;

class EntityOtherPlayerMP : public EntityPlayer {
public:
	EntityOtherPlayerMP(World* var1, const JavaString& var2);
	virtual ~EntityOtherPlayerMP();

	int otherPlayerMPPosRotationIncrements;
	double otherPlayerMPX;
	double otherPlayerMPY;
	double otherPlayerMPZ;
	double otherPlayerMPYaw;
	double otherPlayerMPPitch;
	float a;

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void onUpdate();
	virtual float getYOffsetForRender();
	virtual void onLivingUpdate();
	virtual bool isSneaking();

	static void initStatics();
};

#endif // ENTITYOTHERPLAYERMP_H