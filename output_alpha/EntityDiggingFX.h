#ifndef ENTITYDIGGINGFX_H
#define ENTITYDIGGINGFX_H

// Auto-generated from EntityDiggingFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Block;
class Tesselator;
class World;

class EntityDiggingFX : public EntityFX {
public:
	EntityDiggingFX();
	EntityDiggingFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12, Block* var14);
	virtual ~EntityDiggingFX();

	virtual EntityDiggingFX* func_4041_a(int var1, int var2, int var3);
	virtual int func_404_c();
	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);

public: // was private
	Block* m_field_4082_a;

};

#endif // ENTITYDIGGINGFX_H