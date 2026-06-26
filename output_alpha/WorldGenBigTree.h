#ifndef WORLDGENBIGTREE_H
#define WORLDGENBIGTREE_H

// Auto-generated from WorldGenBigTree.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenBigTree : public WorldGenerator {
public:
	WorldGenBigTree();
	virtual ~WorldGenBigTree();

	virtual void func_517_a(double var1, double var3, double var5);
	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	static unsigned char* s_field_882_a;
	Random* m_field_881_b;
	World* worldObj;
	int* m_field_879_d;
	int m_field_878_e;
	int m_field_877_f;
	double m_field_876_g;
	double m_field_875_h;
	double m_field_874_i;
	double m_field_873_j;
	double m_field_872_k;
	int m_field_871_l;
	int m_field_870_m;
	int m_field_869_n;
	int** m_field_868_o;

	void func_521_a();
	void func_523_a(int var1, int var2, int var3, float var4, unsigned char var5, int var6);
	float func_528_a(int var1);
	float func_526_b(int var1);
	void func_520_a(int var1, int var2, int var3);
	void func_522_a(int* var1, int* var2, int var3);
	void func_518_b();
	bool func_527_c(int var1);
	void func_529_c();
	void func_525_d();
	int func_524_a(int* var1, int* var2);
	bool func_519_e();

public:  // array-length companions
	int m_field_879_d_count;
	int m_field_868_o_count;

};

#endif // WORLDGENBIGTREE_H