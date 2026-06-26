#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

// Auto-generated from WorldRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "AxisAlignedBB.h"
#include <vector>

class Entity;
class ICamera;
class Tesselator;
class World;

class WorldRenderer {
public:
	WorldRenderer();
	WorldRenderer(World* var1, std::vector<void*> /* PORT: determine element type */ var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~WorldRenderer();

	World* worldObj;
	static int s_field_1762_b;
	int m_field_1761_c;
	int m_field_1760_d;
	int m_field_1759_e;
	int m_field_1758_f;
	int m_field_1757_g;
	int m_field_1756_h;
	int m_field_1755_i;
	int m_field_1754_j;
	int m_field_1753_k;
	int m_field_1752_l;
	int m_field_1751_m;
	int m_field_1750_n;
	bool m_field_1749_o;
	bool* m_field_1748_p;
	int m_field_1746_q;
	int m_field_1743_r;
	int m_field_1741_s;
	float m_field_1740_t;
	bool needsUpdate;
	AxisAlignedBB m_field_1736_v;
	bool m_hasField_1736_v;
	int m_field_1735_w;
	bool m_field_1734_x;
	bool m_field_1733_y;
	int m_field_1732_z;
	bool m_field_1747_A;
	std::vector<void*> /* PORT: determine element type */ m_field_1745_B;

	virtual void func_1197_a(int var1, int var2, int var3);
	virtual void func_1198_a();
	virtual float func_1202_a(Entity* var1);
	virtual void func_1195_b();
	virtual void func_1204_c();
	virtual int func_1200_a(int var1);
	virtual void func_1199_a(ICamera* var1);
	virtual void func_1201_d();
	virtual bool func_1196_e();
	virtual void MarkDirty();

public: // was private
	int m_field_1744_C;
	static Tesselator* s_field_1742_D;
	bool m_field_1739_E;
	std::vector<void*> /* PORT: determine element type */ m_field_1737_F;

	void func_1203_g();

public:  // array-length companions
	int m_field_1748_p_count;

};

#endif // WORLDRENDERER_H