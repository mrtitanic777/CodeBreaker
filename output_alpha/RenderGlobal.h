#ifndef RENDERGLOBAL_H
#define RENDERGLOBAL_H

// Auto-generated from RenderGlobal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IWorldAccess.h"
#include <string>
#include <vector>

struct AxisAlignedBB;
class Entity;
class EntityPlayer;
class ICamera;
class ItemStack;
class Minecraft;
class MovingObjectPosition;
class RenderBlocks;
class RenderEngine;
class RenderList;
class TileEntity;
class Vec3D;
class World;
class WorldRenderer;

class RenderGlobal : public IWorldAccess {
public:
	RenderGlobal();
	RenderGlobal(Minecraft* var1, RenderEngine* var2);
	virtual ~RenderGlobal();

	std::vector<void*> /* PORT: determine element type */ m_field_1458_a;
	float m_field_1450_i;

	virtual void func_946_a(World* var1);
	virtual void func_958_a();
	virtual void func_951_a(Vec3D* var1, ICamera* var2, float var3);
	virtual const std::string& func_953_b();
	virtual const std::string& func_957_c();
	virtual int func_943_a(EntityPlayer* var1, int var2, double var3);
	virtual void func_944_a(int var1, double var2);
	virtual void func_945_d();
	virtual void func_4142_a(float var1);
	virtual void func_4141_b(float var1);
	virtual void func_6510_c(float var1);
	virtual bool func_948_a(EntityPlayer* var1, bool var2);
	virtual void func_959_a(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5);
	virtual void drawSelectionBox(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5);
	virtual void func_949_a(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void func_934_a(int var1, int var2, int var3);
	virtual void func_937_b(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void func_960_a(ICamera* var1, float var2);
	virtual void playRecord(const std::string& var1, int var2, int var3, int var4);
	virtual void playSound(const std::string& var1, double var2, double var4, double var6, float var8, float var9);
	virtual void spawnParticle(const std::string& var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void func_936_e();
	virtual void func_935_a(int var1, int var2, int var3, TileEntity* var4);

public: // was private
	World* worldObj;
	RenderEngine* m_renderEngine;
	std::vector<void*> /* PORT: determine element type */ m_field_1446_m;
	WorldRenderer* m_field_1445_n;
	WorldRenderer* m_field_1444_o;
	int m_field_1443_p;
	int m_field_1442_q;
	int m_field_1441_r;
	int m_field_1440_s;
	Minecraft* mc;
	RenderBlocks* m_field_1438_u;
	int* /* PORT: was IntBuffer */ m_field_1437_v;
	bool m_field_1436_w;
	int m_field_1435_x;
	int m_field_1434_y;
	int m_field_1433_z;
	int m_field_1432_A;
	int m_field_1431_B;
	int m_field_1430_C;
	int m_field_1429_D;
	int m_field_1428_E;
	int m_field_1427_F;
	int m_field_1426_G;
	int m_field_1425_H;
	int m_field_1424_I;
	int m_field_1423_J;
	int m_field_1422_K;
	int m_field_1421_L;
	int* m_field_1457_b;
	int* /* PORT: was IntBuffer */ m_field_1456_c;
	int m_field_1420_M;
	int m_field_1419_N;
	int m_field_1418_O;
	int m_field_1417_P;
	int m_field_1416_Q;
	std::vector<void*> /* PORT: determine element type */ m_field_1415_R;
	RenderList* m_field_1414_S;
	int m_field_1455_d;
	int m_field_1454_e;
	double m_field_1453_f;
	double m_field_1452_g;
	double m_field_1451_h;
	int m_field_1449_j;

	void func_950_f();
	void func_956_b(int var1, int var2, int var3);
	void func_962_a(int var1, int var2);
	int func_952_a(int var1, int var2, int var3, double var4);
	void drawOutlinedBoundingBox(AxisAlignedBB var1);

public:  // array-length companions
	int m_field_1445_n_count;
	int m_field_1444_o_count;
	int m_field_1457_b_count;
	int m_field_1414_S_count;

};

#endif // RENDERGLOBAL_H