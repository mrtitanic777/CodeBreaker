package net.minecraft.src;

import java.util.Random;

public class NoiseGenerator2 {
	private static int[][] grad = new int[][]{{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1}, {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};
	private int[] permutations;
	public double xOffset;
	public double yOffset;
	public double zOffset;
	private static final double F2 = 0.5D * (Math.sqrt(3.0D) - 1.0D);
	private static final double G2 = (3.0D - Math.sqrt(3.0D)) / 6.0D;

	public NoiseGenerator2() {
		this(new Random());
	}

	public NoiseGenerator2(Random var1) {
		this.permutations = new int[512];
		this.xOffset = var1.nextDouble() * 256.0D;
		this.yOffset = var1.nextDouble() * 256.0D;
		this.zOffset = var1.nextDouble() * 256.0D;

		int var2;
		for(var2 = 0; var2 < 256; this.permutations[var2] = var2++) {
		}

		for(var2 = 0; var2 < 256; ++var2) {
			int var3 = var1.nextInt(256 - var2) + var2;
			int var4 = this.permutations[var2];
			this.permutations[var2] = this.permutations[var3];
			this.permutations[var3] = var4;
			this.permutations[var2 + 256] = this.permutations[var2];
		}

	}

	private static int fastFloor(double var0) {
		return var0 > 0.0D ? (int)var0 : (int)var0 - 1;
	}

	private static double dot(int[] var0, double var1, double var3) {
		return (double)var0[0] * var1 + (double)var0[1] * var3;
	}

	public void generateNoise(double[] var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12) {
		int var14 = 0;

		for(int var15 = 0; var15 < var6; ++var15) {
			double var16 = (var2 + (double)var15) * var8 + this.xOffset;

			for(int var18 = 0; var18 < var7; ++var18) {
				double var19 = (var4 + (double)var18) * var10 + this.yOffset;
				double var27 = (var16 + var19) * F2;
				int var29 = fastFloor(var16 + var27);
				int var30 = fastFloor(var19 + var27);
				double var31 = (double)(var29 + var30) * G2;
				double var33 = (double)var29 - var31;
				double var35 = (double)var30 - var31;
				double var37 = var16 - var33;
				double var39 = var19 - var35;
				byte var41;
				byte var42;
				if(var37 > var39) {
					var41 = 1;
					var42 = 0;
				} else {
					var41 = 0;
					var42 = 1;
				}

				double var43 = var37 - (double)var41 + G2;
				double var45 = var39 - (double)var42 + G2;
				double var47 = var37 - 1.0D + 2.0D * G2;
				double var49 = var39 - 1.0D + 2.0D * G2;
				int var51 = var29 & 255;
				int var52 = var30 & 255;
				int var53 = this.permutations[var51 + this.permutations[var52]] % 12;
				int var54 = this.permutations[var51 + var41 + this.permutations[var52 + var42]] % 12;
				int var55 = this.permutations[var51 + 1 + this.permutations[var52 + 1]] % 12;
				double var56 = 0.5D - var37 * var37 - var39 * var39;
				double var21;
				if(var56 < 0.0D) {
					var21 = 0.0D;
				} else {
					var56 *= var56;
					var21 = var56 * var56 * dot(grad[var53], var37, var39);
				}

				double var58 = 0.5D - var43 * var43 - var45 * var45;
				double var23;
				if(var58 < 0.0D) {
					var23 = 0.0D;
				} else {
					var58 *= var58;
					var23 = var58 * var58 * dot(grad[var54], var43, var45);
				}

				double var60 = 0.5D - var47 * var47 - var49 * var49;
				double var25;
				if(var60 < 0.0D) {
					var25 = 0.0D;
				} else {
					var60 *= var60;
					var25 = var60 * var60 * dot(grad[var55], var47, var49);
				}

				int var10001 = var14++;
				var1[var10001] += 70.0D * (var21 + var23 + var25) * var12;
			}
		}

	}
}
