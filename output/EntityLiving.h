#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include <string>

class NBTTagCompound;
class World;

class EntityLiving : public Entity {
public:
	EntityLiving();
	EntityLiving(World* var1);
	virtual ~EntityLiving();

	float renderYawOffset;
	float prevRenderYawOffset;
	int health;
	int prevHealth;
	int hurtTime;
	int m_maxHurtTime;
	float attackedAtYaw;
	int deathTime;
	int attackTime;
	float m_prevCameraPitch;
	float m_cameraPitch;
	float m_ae;
	float m_limbYaw;
	float m_limbSwing;

	virtual const std::string& getTexture();
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual void onEntityUpdate();
	virtual void heal(int var1);
	virtual bool attackEntityFrom(Entity* var1, int var2);
	virtual void onDeath(Entity* var1);
	virtual bool isEntityAlive();
	virtual void onLivingUpdate();
	virtual bool getCanSpawnHere(float var1, float var2, float var3);

public: // was protected
	std::string texture;
	int entityAge;
	float moveStrafing;
	float moveForward;
	bool isJumping;
	float moveSpeed;

	virtual float getEyeHeight();
	virtual void setSize(float var1, float var2);
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual void fall(float var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual void updatePlayerActionState();

public: // was private
	int heartsHalvesLife;
	float m_rotationYawHead;
	float m_prevRotationYawHead;
	int scoreValue;
	int m_livingSoundTime;
	float randomYawVelocity;
	float defaultPitch;

};

#endif // ENTITYLIVING_H