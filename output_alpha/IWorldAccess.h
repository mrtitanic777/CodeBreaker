#ifndef IWORLDACCESS_H
#define IWORLDACCESS_H

// Auto-generated from IWorldAccess.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Entity;
class TileEntity;

class IWorldAccess {
public:
	virtual ~IWorldAccess() {}
	virtual void func_934_a(int var1, int var2, int var3) = 0;
	virtual void func_937_b(int var1, int var2, int var3, int var4, int var5, int var6) = 0;
	virtual void playSound(const std::string& var1, double var2, double var4, double var6, float var8, float var9) = 0;
	virtual void spawnParticle(const std::string& var1, double var2, double var4, double var6, double var8, double var10, double var12) = 0;
	virtual void obtainEntitySkin(Entity* var1) = 0;
	virtual void releaseEntitySkin(Entity* var1) = 0;
	virtual void func_936_e() = 0;
	virtual void playRecord(const std::string& var1, int var2, int var3, int var4) = 0;
	virtual void func_935_a(int var1, int var2, int var3, TileEntity* var4) = 0;
};

#endif // IWORLDACCESS_H