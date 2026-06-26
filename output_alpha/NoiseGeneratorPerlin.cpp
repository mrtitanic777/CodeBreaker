#include "stdafx.h"
// Auto-generated from NoiseGeneratorPerlin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorPerlin.h"
#include "Common.h"
#include "Random.h"

NoiseGeneratorPerlin::NoiseGeneratorPerlin()
	: m_permutations(0)
	, xCoord(0)
	, yCoord(0)
	, zCoord(0)
{
  // PORT: constructor delegation  // was: this(new Random());
}

NoiseGeneratorPerlin::NoiseGeneratorPerlin(Random* var1)
	: m_permutations(0)
	, xCoord(0)
	, yCoord(0)
	, zCoord(0)
{
		m_permutations = new int[512];
		m_permutations_count = 512;
		xCoord = var1->nextDouble() * 256.0f;
		yCoord = var1->nextDouble() * 256.0f;
		zCoord = var1->nextDouble() * 256.0f;

		int var2;
		for(var2 = 0; var2 < 256; m_permutations[var2] = var2++) {
		}

		for(var2 = 0; var2 < 256; ++var2) {
			int var3 = var1->nextInt(256 - var2) + var2;
			int var4 = m_permutations[var2];
			m_permutations[var2] = m_permutations[var3];
			m_permutations[var3] = var4;
			m_permutations[var2 + 256] = m_permutations[var2];
		}

}

NoiseGeneratorPerlin::~NoiseGeneratorPerlin() {
	delete[] m_permutations;
	m_permutations = NULL;
}

double NoiseGeneratorPerlin::generateNoise(double var1, double var3, double var5) {
		double var7 = var1 + xCoord;
		double var9 = var3 + yCoord;
		double var11 = var5 + zCoord;
		int var13 = (int)var7;
		int var14 = (int)var9;
		int var15 = (int)var11;
		if(var7 < (double)var13) {
			--var13;
		}

		if(var9 < (double)var14) {
			--var14;
		}

		if(var11 < (double)var15) {
			--var15;
		}

		int var16 = var13 & 255;
		int var17 = var14 & 255;
		int var18 = var15 & 255;
		var7 -= (double)var13;
		var9 -= (double)var14;
		var11 -= (double)var15;
		double var19 = var7 * var7 * var7 * (var7 * (var7 * 6.0f - 15.0f) + 10.0f);
		double var21 = var9 * var9 * var9 * (var9 * (var9 * 6.0f - 15.0f) + 10.0f);
		double var23 = var11 * var11 * var11 * (var11 * (var11 * 6.0f - 15.0f) + 10.0f);
		int var25 = m_permutations[var16] + var17;
		int var26 = m_permutations[var25] + var18;
		int var27 = m_permutations[var25 + 1] + var18;
		int var28 = m_permutations[var16 + 1] + var17;
		int var29 = m_permutations[var28] + var18;
		int var30 = m_permutations[var28 + 1] + var18;
		return lerp(var23, lerp(var21, lerp(var19, grad(m_permutations[var26], var7, var9, var11), grad(m_permutations[var29], var7 - 1.0f, var9, var11)), lerp(var19, grad(m_permutations[var27], var7, var9 - 1.0f, var11), grad(m_permutations[var30], var7 - 1.0f, var9 - 1.0f, var11))), lerp(var21, lerp(var19, grad(m_permutations[var26 + 1], var7, var9, var11 - 1.0f), grad(m_permutations[var29 + 1], var7 - 1.0f, var9, var11 - 1.0f)), lerp(var19, grad(m_permutations[var27 + 1], var7, var9 - 1.0f, var11 - 1.0f), grad(m_permutations[var30 + 1], var7 - 1.0f, var9 - 1.0f, var11 - 1.0f))));
}

double NoiseGeneratorPerlin::lerp(double var1, double var3, double var5) {
		return var3 + var1 * (var5 - var3);
}

double NoiseGeneratorPerlin::func_4110_a(int var1, double var2, double var4) {
		int var6 = var1 & 15;
		double var7 = (double)(1 - ((var6 & 8) >> 3)) * var2;
		double var9 = var6 < 4 ? 0.0f : (var6 != 12 && var6 != 14 ? var4 : var2);
		return ((var6 & 1) == 0 ? var7 : -var7) + ((var6 & 2) == 0 ? var9 : -var9);
}

double NoiseGeneratorPerlin::grad(int var1, double var2, double var4, double var6) {
		int var8 = var1 & 15;
		double var9 = var8 < 8 ? var2 : var4;
		double var11 = var8 < 4 ? var4 : (var8 != 12 && var8 != 14 ? var6 : var2);
		return ((var8 & 1) == 0 ? var9 : -var9) + ((var8 & 2) == 0 ? var11 : -var11);
}

double NoiseGeneratorPerlin::func_801_a(double var1, double var3) {
		return generateNoise(var1, var3, 0.0f);
}

void NoiseGeneratorPerlin::func_805_a(double* var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15, double var17) {
		int var10001;
		int var19;
		int var22;
		double var31;
		double var35;
		int var37;
		double var38;
		int var40;
		int var41;
		double var42;
		int var75;
		if(var9 == 1) {
			bool var64 = false;
			bool var65 = false;
			bool var21 = false;
			bool var68 = false;
			double var70 = 0.0f;
			double var73 = 0.0f;
			var75 = 0;
			double var77 = 1.0f / var17;

			for(int var30 = 0; var30 < var8; ++var30) {
				var31 = (var2 + (double)var30) * var11 + xCoord;
				int var78 = (int)var31;
				if(var31 < (double)var78) {
					--var78;
				}

				int var34 = var78 & 255;
				var31 -= (double)var78;
				var35 = var31 * var31 * var31 * (var31 * (var31 * 6.0f - 15.0f) + 10.0f);

				for(var37 = 0; var37 < var10; ++var37) {
					var38 = (var6 + (double)var37) * var15 + zCoord;
					var40 = (int)var38;
					if(var38 < (double)var40) {
						--var40;
					}

					var41 = var40 & 255;
					var38 -= (double)var40;
					var42 = var38 * var38 * var38 * (var38 * (var38 * 6.0f - 15.0f) + 10.0f);
					var19 = m_permutations[var34] + 0;
					int var66 = m_permutations[var19] + var41;
					int var67 = m_permutations[var34 + 1] + 0;
					var22 = m_permutations[var67] + var41;
					var70 = lerp(var35, func_4110_a(m_permutations[var66], var31, var38), grad(m_permutations[var22], var31 - 1.0f, 0.0f, var38));
					var73 = lerp(var35, grad(m_permutations[var66 + 1], var31, 0.0f, var38 - 1.0f), grad(m_permutations[var22 + 1], var31 - 1.0f, 0.0f, var38 - 1.0f));
					double var79 = lerp(var42, var70, var73);
					var10001 = var75++;
					var1[var10001] += var79 * var77;
				}
			}

		} else {
			var19 = 0;
			double var20 = 1.0f / var17;
			var22 = -1;
			bool var23 = false;
			bool var24 = false;
			bool var25 = false;
			bool var26 = false;
			bool var27 = false;
			bool var28 = false;
			double var29 = 0.0f;
			var31 = 0.0f;
			double var33 = 0.0f;
			var35 = 0.0f;

			for(var37 = 0; var37 < var8; ++var37) {
				var38 = (var2 + (double)var37) * var11 + xCoord;
				var40 = (int)var38;
				if(var38 < (double)var40) {
					--var40;
				}

				var41 = var40 & 255;
				var38 -= (double)var40;
				var42 = var38 * var38 * var38 * (var38 * (var38 * 6.0f - 15.0f) + 10.0f);

				for(int var44 = 0; var44 < var10; ++var44) {
					double var45 = (var6 + (double)var44) * var15 + zCoord;
					int var47 = (int)var45;
					if(var45 < (double)var47) {
						--var47;
					}

					int var48 = var47 & 255;
					var45 -= (double)var47;
					double var49 = var45 * var45 * var45 * (var45 * (var45 * 6.0f - 15.0f) + 10.0f);

					for(int var51 = 0; var51 < var9; ++var51) {
						double var52 = (var4 + (double)var51) * var13 + yCoord;
						int var54 = (int)var52;
						if(var52 < (double)var54) {
							--var54;
						}

						int var55 = var54 & 255;
						var52 -= (double)var54;
						double var56 = var52 * var52 * var52 * (var52 * (var52 * 6.0f - 15.0f) + 10.0f);
						if(var51 == 0 || var55 != var22) {
							var22 = var55;
							int var69 = m_permutations[var41] + var55;
							int var71 = m_permutations[var69] + var48;
							int var72 = m_permutations[var69 + 1] + var48;
							int var74 = m_permutations[var41 + 1] + var55;
							var75 = m_permutations[var74] + var48;
							int var76 = m_permutations[var74 + 1] + var48;
							var29 = lerp(var42, grad(m_permutations[var71], var38, var52, var45), grad(m_permutations[var75], var38 - 1.0f, var52, var45));
							var31 = lerp(var42, grad(m_permutations[var72], var38, var52 - 1.0f, var45), grad(m_permutations[var76], var38 - 1.0f, var52 - 1.0f, var45));
							var33 = lerp(var42, grad(m_permutations[var71 + 1], var38, var52, var45 - 1.0f), grad(m_permutations[var75 + 1], var38 - 1.0f, var52, var45 - 1.0f));
							var35 = lerp(var42, grad(m_permutations[var72 + 1], var38, var52 - 1.0f, var45 - 1.0f), grad(m_permutations[var76 + 1], var38 - 1.0f, var52 - 1.0f, var45 - 1.0f));
						}

						double var58 = lerp(var56, var29, var31);
						double var60 = lerp(var56, var33, var35);
						double var62 = lerp(var49, var58, var60);
						var10001 = var19++;
						var1[var10001] += var62 * var20;
					}
				}
			}

		}
}
