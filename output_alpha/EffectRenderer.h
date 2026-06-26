#ifndef EFFECTRENDERER_H
#define EFFECTRENDERER_H

// Auto-generated from EffectRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

class Entity;
class EntityFX;
class Random;
class RenderEngine;
class World;

class EffectRenderer {
public:
	EffectRenderer();
	EffectRenderer(World* var1, RenderEngine* var2);
	virtual ~EffectRenderer();

	virtual void func_1192_a(EntityFX* var1);
	virtual void func_1193_a();
	virtual void func_1189_a(Entity* var1, float var2);
	virtual void func_1187_b(Entity* var1, float var2);
	virtual void func_1188_a(World* var1);
	virtual void func_1186_a(int var1, int var2, int var3);
	virtual void func_1191_a(int var1, int var2, int var3, int var4);
	virtual const std::string& func_1190_b();

public: // was protected
	World* worldObj;

public: // was private
	std::vector<void*>* /* PORT: determine element type */ m_field_1728_b;
	RenderEngine* m_field_1731_c;
	Random* rand;

public:  // array-length companions
	int m_field_1728_b_count;

};

#endif // EFFECTRENDERER_H