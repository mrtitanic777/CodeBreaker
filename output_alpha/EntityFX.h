#ifndef ENTITYFX_H
#define ENTITYFX_H

// Auto-generated from EntityFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class NBTTagCompound;
class Tesselator;
class World;

class EntityFX : public Entity {
public:
	EntityFX();
	EntityFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityFX();

	static double s_field_660_l;
	static double s_field_659_m;
	static double s_field_658_n;

	virtual EntityFX* func_407_b(float var1);
	virtual EntityFX* func_405_d(float var1);
	virtual void onUpdate();
	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual int func_404_c();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was protected
	int m_field_670_b;
	float m_field_669_c;
	float m_field_668_d;
	int m_e;
	int m_field_666_f;
	float m_field_665_g;
	float m_field_664_h;
	float m_field_663_i;
	float m_field_662_j;
	float m_field_661_k;

};

#endif // ENTITYFX_H