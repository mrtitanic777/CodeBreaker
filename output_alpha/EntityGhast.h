#ifndef ENTITYGHAST_H
#define ENTITYGHAST_H

// Auto-generated from EntityGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFlying.h"
#include "IMobs.h"
#include <string>

class Entity;
class World;

class EntityGhast : public EntityFlying, public IMobs {
public:
	EntityGhast();
	EntityGhast(World* var1);
	virtual ~EntityGhast();

	int m_field_4121_a;
	double m_field_4120_b;
	double m_field_4127_c;
	double m_field_4126_d;
	int m_field_4125_e;
	int m_field_4124_f;

	virtual bool getCanSpawnHere();
	virtual int func_6391_i();

public: // was protected
	virtual void func_418_b_();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();
	virtual float getSoundVolume();

public: // was private
	Entity* m_field_4123_g;
	int m_field_4122_h;

	bool func_4050_a(double var1, double var3, double var5, double var7);

};

#endif // ENTITYGHAST_H