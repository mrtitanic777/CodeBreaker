#ifndef IWORLDACCESS_H
#define IWORLDACCESS_H

// Auto-generated from IWorldAccess.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Entity;

class IWorldAccess {
public:
	virtual ~IWorldAccess() {}
	virtual void markBlockAndNeighborsNeedsUpdate(int var1, int var2, int var3) = 0;
	virtual void markBlockRangeNeedsUpdate(int var1, int var2, int var3, int var4, int var5, int var6) = 0;
	virtual void loadRenderers() = 0;
	virtual void playSound(const std::string& var1, float var2, float var3, float var4, float var5, float var6) = 0;
	virtual void spawnParticle(const std::string& var1, float var2, float var3, float var4, float var5, float var6, float var7) = 0;
	virtual void playMusic(const std::string& var1, float var2, float var3, float var4, float var5) = 0;
	virtual void obtainEntitySkin(Entity* var1) = 0;
	virtual void releaseEntitySkin(Entity* var1) = 0;
	virtual void updateAllRenderers() = 0;
};

#endif // IWORLDACCESS_H