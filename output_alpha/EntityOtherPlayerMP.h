#ifndef ENTITYOTHERPLAYERMP_H
#define ENTITYOTHERPLAYERMP_H

// Auto-generated from EntityOtherPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayer.h"
#include <string>

class Entity;
class World;

class EntityOtherPlayerMP : public EntityPlayer {
public:
	EntityOtherPlayerMP();
	EntityOtherPlayerMP(World* var1, const std::string& var2);
	virtual ~EntityOtherPlayerMP();

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void onUpdate();
	virtual float func_392_h_();
	virtual void onLivingUpdate();
	virtual bool func_381_o();

public: // was private
	int m_field_785_bg;
	double m_field_784_bh;
	double m_field_783_bi;
	double m_field_782_bj;
	double m_field_780_bk;
	double m_field_786_bl;
	float m_a;

};

#endif // ENTITYOTHERPLAYERMP_H