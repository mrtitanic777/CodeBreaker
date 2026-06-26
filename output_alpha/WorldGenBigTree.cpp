#include "stdafx.h"
// Hand-ported from WorldGenBigTree.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
// Procedural large-tree generator. Core logic only (no GL).
//
// Fixes over the auto-converted output_alpha/WorldGenBigTree.cpp:
//   * static byte table s_field_882_a properly initialized to {2,0,0,1,2,1}
//   * Java int[][] foliage coord lists -> int** (heap rows of 4 ints)
//   * System.arraycopy -> manual row copy; array-length companion fields tracked
//   * undeclared `var2`/`var4` references resolved to the real locals/fields
//   * scratch 3-element coordinate vectors kept as local stack arrays

#include "WorldGenBigTree.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include <math.h>

// Java: static byte[] otherCoordPairs = {2, 0, 0, 1, 2, 1};
// Lazily-pointed file-scope storage so the static* member is never NULL.
static unsigned char s_otherCoordPairs[6] = { 2, 0, 0, 1, 2, 1 };
unsigned char* WorldGenBigTree::s_field_882_a = s_otherCoordPairs;

WorldGenBigTree::WorldGenBigTree()
	: m_field_881_b(new Random())
	, worldObj(NULL)
	, m_field_879_d(NULL)
	, m_field_878_e(0)
	, m_field_877_f(0)
	, m_field_876_g(0.618)
	, m_field_875_h(1.0)
	, m_field_874_i(0.381)
	, m_field_873_j(1.0)
	, m_field_872_k(1.0)
	, m_field_871_l(1)
	, m_field_870_m(12)
	, m_field_869_n(4)
	, m_field_868_o(NULL)
	, m_field_879_d_count(0)
	, m_field_868_o_count(0)
{
	// Java: int[] basePos = {0, 0, 0};
	m_field_879_d = new int[3];
	m_field_879_d_count = 3;
	m_field_879_d[0] = 0;
	m_field_879_d[1] = 0;
	m_field_879_d[2] = 0;
}

WorldGenBigTree::~WorldGenBigTree() {
	delete m_field_881_b;
	m_field_881_b = NULL;
	worldObj = NULL;
	delete[] m_field_879_d;
	m_field_879_d = NULL;
	if(m_field_868_o != NULL) {
		for(int i = 0; i < m_field_868_o_count; ++i) {
			delete[] m_field_868_o[i];
		}
		delete[] m_field_868_o;
		m_field_868_o = NULL;
	}
	m_field_868_o_count = 0;
}

// generateLeafNodeList
void WorldGenBigTree::func_521_a() {
	m_field_877_f = (int)((double)m_field_878_e * m_field_876_g);
	if(m_field_877_f >= m_field_878_e) {
		m_field_877_f = m_field_878_e - 1;
	}

	int var1 = (int)(1.382 + pow(m_field_872_k * (double)m_field_878_e / 13.0, 2.0));
	if(var1 < 1) {
		var1 = 1;
	}

	// Java: int[][] var2 = new int[var1 * heightAttenuation][4];
	int var2Cap = var1 * m_field_878_e;
	if(var2Cap < 1) {
		var2Cap = 1;
	}
	int** var2 = new int*[var2Cap];
	for(int vi = 0; vi < var2Cap; ++vi) {
		var2[vi] = new int[4];
		var2[vi][0] = 0;
		var2[vi][1] = 0;
		var2[vi][2] = 0;
		var2[vi][3] = 0;
	}

	int var3 = m_field_879_d[1] + m_field_878_e - m_field_869_n;
	int var4 = 1;
	int var5 = m_field_879_d[1] + m_field_877_f;
	int var6 = var3 - m_field_879_d[1];
	var2[0][0] = m_field_879_d[0];
	var2[0][1] = var3;
	var2[0][2] = m_field_879_d[2];
	var2[0][3] = var5;
	--var3;

	while(var6 >= 0) {
		int var7 = 0;
		float var8 = func_528_a(var6);
		if(var8 < 0.0F) {
			--var3;
			--var6;
		} else {
			double var9 = 0.5;
			for(var7 = 0; var7 < var1; ++var7) {
				double var11 = m_field_873_j * (double)var8 * ((double)m_field_881_b->nextFloat() + 0.328);
				double var13 = (double)m_field_881_b->nextFloat() * 2.0 * 3.14159;
				int var15 = (int)(var11 * sin(var13) + (double)m_field_879_d[0] + var9);
				int var16 = (int)(var11 * cos(var13) + (double)m_field_879_d[2] + var9);
				int var17arr[3];
				var17arr[0] = var15;
				var17arr[1] = var3;
				var17arr[2] = var16;
				int* var17 = var17arr;
				int var18arr[3];
				var18arr[0] = var15;
				var18arr[1] = var3 + m_field_869_n;
				var18arr[2] = var16;
				int* var18 = var18arr;
				if(func_524_a(var17, var18) == -1) {
					int var19arr[3];
					var19arr[0] = m_field_879_d[0];
					var19arr[1] = m_field_879_d[1];
					var19arr[2] = m_field_879_d[2];
					int* var19 = var19arr;
					double var20 = sqrt(pow((double)abs(m_field_879_d[0] - var17[0]), 2.0) + pow((double)abs(m_field_879_d[2] - var17[2]), 2.0));
					double var22 = var20 * m_field_874_i;
					if((double)var17[1] - var22 > (double)var5) {
						var19[1] = var5;
					} else {
						var19[1] = (int)((double)var17[1] - var22);
					}

					if(func_524_a(var19, var17) == -1) {
						var2[var4][0] = var15;
						var2[var4][1] = var3;
						var2[var4][2] = var16;
						var2[var4][3] = var19[1];
						++var4;
					}
				}
			}

			--var3;
			--var6;
		}
	}

	// Java: leafNodes = new int[var4][4]; System.arraycopy(var2, 0, leafNodes, 0, var4);
	// Release any prior list, then take ownership of the first var4 rows of var2.
	if(m_field_868_o != NULL) {
		for(int i = 0; i < m_field_868_o_count; ++i) {
			delete[] m_field_868_o[i];
		}
		delete[] m_field_868_o;
		m_field_868_o = NULL;
	}
	m_field_868_o = new int*[var4 > 0 ? var4 : 1];
	m_field_868_o_count = var4;
	for(int r = 0; r < var4; ++r) {
		m_field_868_o[r] = var2[r];
		var2[r] = NULL;
	}
	// Free the unused tail rows + the outer scratch array.
	for(int t = 0; t < var2Cap; ++t) {
		if(var2[t] != NULL) {
			delete[] var2[t];
		}
	}
	delete[] var2;
}

// genTreeLayer
void WorldGenBigTree::func_523_a(int var1, int var2, int var3, float var4, unsigned char var5, int var6) {
	int var7 = (int)((double)var4 + 0.618);
	unsigned char var8 = s_field_882_a[var5];
	unsigned char var9 = s_field_882_a[var5 + 3];
	int var10arr[3];
	var10arr[0] = var1;
	var10arr[1] = var2;
	var10arr[2] = var3;
	int* var10 = var10arr;
	int var11arr[3];
	var11arr[0] = 0;
	var11arr[1] = 0;
	var11arr[2] = 0;
	int* var11 = var11arr;
	int var12;

	var11[var5] = var10[var5];

	for(var12 = -var7; var12 <= var7; ++var12) {
		var11[var8] = var10[var8] + var12;

		for(int var13 = -var7; var13 <= var7; ++var13) {
			double var15 = sqrt(pow((double)abs(var12) + 0.5, 2.0) + pow((double)abs(var13) + 0.5, 2.0));
			if(var15 > (double)var4) {
				continue;
			}

			var11[var9] = var10[var9] + var13;
			int var14 = worldObj->getBlockId(var11[0], var11[1], var11[2]);
			if(var14 != 0 && var14 != 18) {
				continue;
			}

			worldObj->setBlock(var11[0], var11[1], var11[2], var6);
		}
	}
}

// leafSize / treeShape (foliage radius along the trunk)
float WorldGenBigTree::func_528_a(int var1) {
	if((double)var1 < (double)((float)m_field_878_e) * 0.3) {
		return -1.618F;
	} else {
		float var2 = (float)m_field_878_e / 2.0F;
		float var3 = (float)m_field_878_e / 2.0F - (float)var1;
		float var4;
		if(var3 == 0.0F) {
			var4 = var2;
		} else if((float)fabs((double)var3) >= var2) {
			var4 = 0.0F;
		} else {
			var4 = (float)sqrt(pow((double)fabs((double)var2), 2.0) - pow((double)fabs((double)var3), 2.0));
		}

		var4 *= 0.5F;
		return var4;
	}
}

// foliage leaf-layer radius
float WorldGenBigTree::func_526_b(int var1) {
	return var1 >= 0 && var1 < m_field_869_n ? (var1 != 0 && var1 != m_field_869_n - 1 ? 3.0F : 2.0F) : -1.0F;
}

// generateLeafNode
void WorldGenBigTree::func_520_a(int var1, int var2, int var3) {
	int var4 = var2;

	for(int var5 = var2 + m_field_869_n; var4 < var5; ++var4) {
		float var6 = func_526_b(var4 - var2);
		func_523_a(var1, var4, var3, var6, (unsigned char)1, 18);
	}
}

// placeBlockLine
void WorldGenBigTree::func_522_a(int* var1, int* var2, int var3) {
	int var4arr[3];
	var4arr[0] = 0;
	var4arr[1] = 0;
	var4arr[2] = 0;
	int* var4 = var4arr;
	unsigned char var5 = 0;

	unsigned char var6;
	for(var6 = 0; var5 < 3; ++var5) {
		var4[var5] = var2[var5] - var1[var5];
		if(abs(var4[var5]) > abs(var4[var6])) {
			var6 = var5;
		}
	}

	if(var4[var6] != 0) {
		unsigned char var7 = s_field_882_a[var6];
		unsigned char var8 = s_field_882_a[var6 + 3];
		int var9;
		if(var4[var6] > 0) {
			var9 = 1;
		} else {
			var9 = -1;
		}

		double var10 = (double)var4[var7] / (double)var4[var6];
		double var12 = (double)var4[var8] / (double)var4[var6];
		int var14arr[3];
		var14arr[0] = 0;
		var14arr[1] = 0;
		var14arr[2] = 0;
		int* var14 = var14arr;
		int var15 = 0;

		for(int var16 = var4[var6] + var9; var15 != var16; var15 += var9) {
			var14[var6] = (int)floor((double)(var1[var6] + var15) + 0.5);
			var14[var7] = (int)floor((double)var1[var7] + (double)var15 * var10 + 0.5);
			var14[var8] = (int)floor((double)var1[var8] + (double)var15 * var12 + 0.5);
			worldObj->setBlock(var14[0], var14[1], var14[2], var3);
		}
	}
}

// generateLeaves
void WorldGenBigTree::func_518_b() {
	int var1 = 0;

	for(int var2 = m_field_868_o_count; var1 < var2; ++var1) {
		int var3 = m_field_868_o[var1][0];
		int var4 = m_field_868_o[var1][1];
		int var5 = m_field_868_o[var1][2];
		func_520_a(var3, var4, var5);
	}
}

// leafNodeNeedsBase
bool WorldGenBigTree::func_527_c(int var1) {
	return (double)var1 >= (double)m_field_878_e * 0.2;
}

// generateTrunk
void WorldGenBigTree::func_529_c() {
	int var1 = m_field_879_d[0];
	int var2 = m_field_879_d[1];
	int var3 = m_field_879_d[1] + m_field_877_f;
	int var4 = m_field_879_d[2];
	int var5arr[3];
	var5arr[0] = var1;
	var5arr[1] = var2;
	var5arr[2] = var4;
	int* var5 = var5arr;
	int var6arr[3];
	var6arr[0] = var1;
	var6arr[1] = var3;
	var6arr[2] = var4;
	int* var6 = var6arr;
	func_522_a(var5, var6, 17);
	if(m_field_871_l == 2) {
		++var5[0];
		++var6[0];
		func_522_a(var5, var6, 17);
		++var5[2];
		++var6[2];
		func_522_a(var5, var6, 17);
		var5[0] += -1;
		var6[0] += -1;
		func_522_a(var5, var6, 17);
	}
}

// generateLeafNodeBases
void WorldGenBigTree::func_525_d() {
	int var1 = 0;
	int var2 = m_field_868_o_count;

	int var3arr[3];
	var3arr[0] = m_field_879_d[0];
	var3arr[1] = m_field_879_d[1];
	var3arr[2] = m_field_879_d[2];
	int* var3 = var3arr;

	for(; var1 < var2; ++var1) {
		int* var4 = m_field_868_o[var1];
		int var5arr[3];
		var5arr[0] = var4[0];
		var5arr[1] = var4[1];
		var5arr[2] = var4[2];
		int* var5 = var5arr;
		var3[1] = var4[3];
		int var6 = var3[1] - m_field_879_d[1];
		if(func_527_c(var6)) {
			func_522_a(var3, var5, 17);
		}
	}
}

// checkBlockLine -- returns -1 if clear, else distance to first obstruction
int WorldGenBigTree::func_524_a(int* var1, int* var2) {
	int var3arr[3];
	var3arr[0] = 0;
	var3arr[1] = 0;
	var3arr[2] = 0;
	int* var3 = var3arr;
	unsigned char var4 = 0;

	unsigned char var5;
	for(var5 = 0; var4 < 3; ++var4) {
		var3[var4] = var2[var4] - var1[var4];
		if(abs(var3[var4]) > abs(var3[var5])) {
			var5 = var4;
		}
	}

	if(var3[var5] == 0) {
		return -1;
	} else {
		unsigned char var6 = s_field_882_a[var5];
		unsigned char var7 = s_field_882_a[var5 + 3];
		int var8;
		if(var3[var5] > 0) {
			var8 = 1;
		} else {
			var8 = -1;
		}

		double var9 = (double)var3[var6] / (double)var3[var5];
		double var11 = (double)var3[var7] / (double)var3[var5];
		int var13arr[3];
		var13arr[0] = 0;
		var13arr[1] = 0;
		var13arr[2] = 0;
		int* var13 = var13arr;
		int var14 = 0;

		int var15;
		for(var15 = var3[var5] + var8; var14 != var15; var14 += var8) {
			var13[var5] = var1[var5] + var14;
			var13[var6] = (int)((double)var1[var6] + (double)var14 * var9);
			var13[var7] = (int)((double)var1[var7] + (double)var14 * var11);
			int var16 = worldObj->getBlockId(var13[0], var13[1], var13[2]);
			if(var16 != 0 && var16 != 18) {
				break;
			}
		}

		return var14 == var15 ? -1 : abs(var14);
	}
}

// validTreeLocation
bool WorldGenBigTree::func_519_e() {
	int var1arr[3];
	var1arr[0] = m_field_879_d[0];
	var1arr[1] = m_field_879_d[1];
	var1arr[2] = m_field_879_d[2];
	int* var1 = var1arr;
	int var2arr[3];
	var2arr[0] = m_field_879_d[0];
	var2arr[1] = m_field_879_d[1] + m_field_878_e - 1;
	var2arr[2] = m_field_879_d[2];
	int* var2 = var2arr;
	int var3 = worldObj->getBlockId(m_field_879_d[0], m_field_879_d[1] - 1, m_field_879_d[2]);
	if(var3 != 2 && var3 != 3) {
		return false;
	} else {
		int var4 = func_524_a(var1, var2);
		if(var4 == -1) {
			return true;
		} else if(var4 < 6) {
			return false;
		} else {
			m_field_878_e = var4;
			return true;
		}
	}
}

// setScale
void WorldGenBigTree::func_517_a(double var1, double var3, double var5) {
	m_field_870_m = (int)(var1 * 12.0);
	if(var1 > 0.5) {
		m_field_869_n = 5;
	}

	m_field_873_j = var3;
	m_field_872_k = var5;
}

bool WorldGenBigTree::generate(World* var1, Random* var2, int var3, int var4, int var5) {
	worldObj = var1;
	long long var6 = var2->nextLong();
	m_field_881_b->setSeed(var6);
	m_field_879_d[0] = var3;
	m_field_879_d[1] = var4;
	m_field_879_d[2] = var5;
	if(m_field_878_e == 0) {
		m_field_878_e = 5 + m_field_881_b->nextInt(m_field_870_m);
	}

	if(!func_519_e()) {
		return false;
	} else {
		func_521_a();
		func_518_b();
		func_529_c();
		func_525_d();
		return true;
	}
}
