#ifndef MAPGENCAVESHELL_H
#define MAPGENCAVESHELL_H

// Auto-generated from MapGenCavesHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MapGenBase.h"

class World;

class MapGenCavesHell : public MapGenBase {
public:
	MapGenCavesHell();
	virtual ~MapGenCavesHell();

public: // was protected
	virtual void func_4129_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8);
	virtual void func_4128_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15);
	virtual void func_868_a(World* var1, int var2, int var3, int var4, int var5, unsigned char* var6);

};

#endif // MAPGENCAVESHELL_H