#ifndef MAPGENCAVES_H
#define MAPGENCAVES_H

// Auto-generated from MapGenCaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MapGenBase.h"

class World;

class MapGenCaves : public MapGenBase {
public:
	MapGenCaves();
	virtual ~MapGenCaves();

public: // was protected
	virtual void func_870_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8);
	virtual void func_869_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15);
	virtual void func_868_a(World* var1, int var2, int var3, int var4, int var5, unsigned char* var6);

};

#endif // MAPGENCAVES_H