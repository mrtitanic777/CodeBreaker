#ifndef EXPLOSION_H
#define EXPLOSION_H

// Auto-generated from Explosion.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: java.util.Set<ChunkPosition> -> std::vector<void*>* (entries are
// distinct `new ChunkPosition` objects -- Java HashSet used identity hashing, so a vector
// is behaviourally equivalent).

#include <vector>

class Entity;
class Random;
class World;

class Explosion {
public:
	Explosion();
	Explosion(World* var1, Entity* var2, double var3, double var5, double var7, float var9);
	virtual ~Explosion();

	bool m_field_12257_a;
	double m_field_12256_b;
	double m_field_12255_c;
	double m_field_12254_d;
	Entity* m_field_12253_e;
	float m_field_12252_f;
	std::vector<void*>* m_field_12251_g;

	virtual void func_12248_a();
	virtual void func_12247_b();

public: // was private
	Random* m_field_12250_h;
	World* m_field_12249_i;

};

#endif // EXPLOSION_H
