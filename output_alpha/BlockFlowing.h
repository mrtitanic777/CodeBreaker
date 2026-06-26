#ifndef BLOCKFLOWING_H
#define BLOCKFLOWING_H

// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluids.h"

class Material;
class Random;
class World;

class BlockFlowing : public BlockFluids {
public:
	BlockFlowing();
	BlockFlowing(int var1, Material* var2);
	virtual ~BlockFlowing();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);

public: // was protected
	virtual int func_296_f(World* var1, int var2, int var3, int var4, int var5);

public: // was private
	int m_field_460_a;
	bool* m_field_459_b;
	int* m_field_461_c;

	void func_15233_j(World* var1, int var2, int var3, int var4);
	void func_299_g(World* var1, int var2, int var3, int var4, int var5);
	int func_300_a(World* var1, int var2, int var3, int var4, int var5, int var6);
	bool* func_297_k(World* var1, int var2, int var3, int var4);
	bool func_295_l(World* var1, int var2, int var3, int var4);
	bool func_298_m(World* var1, int var2, int var3, int var4);

public:  // array-length companions
	int m_field_459_b_count;
	int m_field_461_c_count;

};

#endif // BLOCKFLOWING_H