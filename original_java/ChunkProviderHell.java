package net.minecraft.src;

import java.util.Random;

public class ChunkProviderHell implements IChunkProvider {
	private Random rand;
	private NoiseGeneratorOctaves noiseGen1;
	private NoiseGeneratorOctaves noiseGen2;
	private NoiseGeneratorOctaves noiseGen3;
	private NoiseGeneratorOctaves noiseGen5;
	private NoiseGeneratorOctaves noiseGen6;
	public NoiseGeneratorOctaves noiseGenA;
	public NoiseGeneratorOctaves noiseGenB;
	private World worldObj;
	private double[] noiseArray;
	private double[] noiseData2 = new double[256];
	private double[] noiseData3 = new double[256];
	private double[] noiseData4 = new double[256];
	private MapGenBase caveGenerator = new MapGenCavesHell();
	double[] noiseData5;
	double[] noiseData6;
	double[] noiseData7;
	double[] noiseDataA;
	double[] noiseDataB;

	public ChunkProviderHell(World var1, long var2) {
		this.worldObj = var1;
		this.rand = new Random(var2);
		this.noiseGen1 = new NoiseGeneratorOctaves(this.rand, 16);
		this.noiseGen2 = new NoiseGeneratorOctaves(this.rand, 16);
		this.noiseGen3 = new NoiseGeneratorOctaves(this.rand, 8);
		this.noiseGen5 = new NoiseGeneratorOctaves(this.rand, 4);
		this.noiseGen6 = new NoiseGeneratorOctaves(this.rand, 4);
		this.noiseGenA = new NoiseGeneratorOctaves(this.rand, 10);
		this.noiseGenB = new NoiseGeneratorOctaves(this.rand, 16);
	}

	public void generateTerrain(int var1, int var2, byte[] var3) {
		byte var4 = 4;
		byte var5 = 32;
		int var6 = var4 + 1;
		byte var7 = 17;
		int var8 = var4 + 1;
		this.noiseArray = this.initializeNoiseField(this.noiseArray, var1 * var4, 0, var2 * var4, var6, var7, var8);

		for(int var9 = 0; var9 < var4; ++var9) {
			for(int var10 = 0; var10 < var4; ++var10) {
				for(int var11 = 0; var11 < 16; ++var11) {
					double var12 = 0.125D;
					double var14 = this.noiseArray[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 0];
					double var16 = this.noiseArray[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 0];
					double var18 = this.noiseArray[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 0];
					double var20 = this.noiseArray[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 0];
					double var22 = (this.noiseArray[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 1] - var14) * var12;
					double var24 = (this.noiseArray[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 1] - var16) * var12;
					double var26 = (this.noiseArray[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 1] - var18) * var12;
					double var28 = (this.noiseArray[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 1] - var20) * var12;

					for(int var30 = 0; var30 < 8; ++var30) {
						double var31 = 0.25D;
						double var33 = var14;
						double var35 = var16;
						double var37 = (var18 - var14) * var31;
						double var39 = (var20 - var16) * var31;

						for(int var41 = 0; var41 < 4; ++var41) {
							int var42 = var41 + var9 * 4 << 11 | 0 + var10 * 4 << 7 | var11 * 8 + var30;
							short var43 = 128;
							double var44 = 0.25D;
							double var46 = var33;
							double var48 = (var35 - var33) * var44;

							for(int var50 = 0; var50 < 4; ++var50) {
								int var51 = 0;
								if(var11 * 8 + var30 < var5) {
									var51 = Block.lavaMoving.blockID;
								}

								if(var46 > 0.0D) {
									var51 = Block.bloodStone.blockID;
								}

								var3[var42] = (byte)var51;
								var42 += var43;
								var46 += var48;
							}

							var33 += var37;
							var35 += var39;
						}

						var14 += var22;
						var16 += var24;
						var18 += var26;
						var20 += var28;
					}
				}
			}
		}

	}

	public void replaceBlocksForBiome(int var1, int var2, byte[] var3) {
		byte var4 = 64;
		double var5 = 1.0D / 32.0D;
		this.noiseData2 = this.noiseGen5.generateNoiseOctaves(this.noiseData2, (double)(var1 * 16), (double)(var2 * 16), 0.0D, 16, 16, 1, var5, var5, 1.0D);
		this.noiseData3 = this.noiseGen5.generateNoiseOctaves(this.noiseData3, (double)(var2 * 16), 109.0134D, (double)(var1 * 16), 16, 1, 16, var5, 1.0D, var5);
		this.noiseData4 = this.noiseGen6.generateNoiseOctaves(this.noiseData4, (double)(var1 * 16), (double)(var2 * 16), 0.0D, 16, 16, 1, var5 * 2.0D, var5 * 2.0D, var5 * 2.0D);

		for(int var7 = 0; var7 < 16; ++var7) {
			for(int var8 = 0; var8 < 16; ++var8) {
				boolean var9 = this.noiseData2[var7 + var8 * 16] + this.rand.nextDouble() * 0.2D > 0.0D;
				boolean var10 = this.noiseData3[var7 + var8 * 16] + this.rand.nextDouble() * 0.2D > 0.0D;
				int var11 = (int)(this.noiseData4[var7 + var8 * 16] / 3.0D + 3.0D + this.rand.nextDouble() * 0.25D);
				int var12 = -1;
				byte var13 = (byte)Block.bloodStone.blockID;
				byte var14 = (byte)Block.bloodStone.blockID;

				for(int var15 = 127; var15 >= 0; --var15) {
					int var16 = (var7 * 16 + var8) * 128 + var15;
					if(var15 >= 127 - this.rand.nextInt(5)) {
						var3[var16] = (byte)Block.bedrock.blockID;
					} else if(var15 <= 0 + this.rand.nextInt(5)) {
						var3[var16] = (byte)Block.bedrock.blockID;
					} else {
						byte var17 = var3[var16];
						if(var17 == 0) {
							var12 = -1;
						} else if(var17 == Block.bloodStone.blockID) {
							if(var12 == -1) {
								if(var11 <= 0) {
									var13 = 0;
									var14 = (byte)Block.bloodStone.blockID;
								} else if(var15 >= var4 - 4 && var15 <= var4 + 1) {
									var13 = (byte)Block.bloodStone.blockID;
									var14 = (byte)Block.bloodStone.blockID;
									if(var10) {
										var13 = (byte)Block.gravel.blockID;
									}

									if(var10) {
										var14 = (byte)Block.bloodStone.blockID;
									}

									if(var9) {
										var13 = (byte)Block.slowSand.blockID;
									}

									if(var9) {
										var14 = (byte)Block.slowSand.blockID;
									}
								}

								if(var15 < var4 && var13 == 0) {
									var13 = (byte)Block.lavaMoving.blockID;
								}

								var12 = var11;
								if(var15 >= var4 - 1) {
									var3[var16] = var13;
								} else {
									var3[var16] = var14;
								}
							} else if(var12 > 0) {
								--var12;
								var3[var16] = var14;
							}
						}
					}
				}
			}
		}

	}

	public Chunk provideChunk(int var1, int var2) {
		this.rand.setSeed((long)var1 * 341873128712L + (long)var2 * 132897987541L);
		byte[] var3 = new byte[-Short.MIN_VALUE];
		this.generateTerrain(var1, var2, var3);
		this.replaceBlocksForBiome(var1, var2, var3);
		this.caveGenerator.generate(this, this.worldObj, var1, var2, var3);
		Chunk var4 = new Chunk(this.worldObj, var3, var1, var2);
		var4.generateSkylightMap();
		var4.generateBlocklightMap();
		return var4;
	}

	private double[] initializeNoiseField(double[] var1, int var2, int var3, int var4, int var5, int var6, int var7) {
		if(var1 == null) {
			var1 = new double[var5 * var6 * var7];
		}

		double var8 = 684.412D;
		double var10 = 2053.236D;
		this.noiseDataA = this.noiseGenA.generateNoiseOctaves(this.noiseDataA, (double)var2, (double)var3, (double)var4, var5, 1, var7, 1.0D, 0.0D, 1.0D);
		this.noiseDataB = this.noiseGenB.generateNoiseOctaves(this.noiseDataB, (double)var2, (double)var3, (double)var4, var5, 1, var7, 100.0D, 0.0D, 100.0D);
		this.noiseData5 = this.noiseGen3.generateNoiseOctaves(this.noiseData5, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8 / 80.0D, var10 / 60.0D, var8 / 80.0D);
		this.noiseData6 = this.noiseGen1.generateNoiseOctaves(this.noiseData6, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
		this.noiseData7 = this.noiseGen2.generateNoiseOctaves(this.noiseData7, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
		int var12 = 0;
		int var13 = 0;
		double[] var14 = new double[var6];

		int var15;
		for(var15 = 0; var15 < var6; ++var15) {
			var14[var15] = Math.cos((double)var15 * Math.PI * 6.0D / (double)var6) * 2.0D;
			double var16 = (double)var15;
			if(var15 > var6 / 2) {
				var16 = (double)(var6 - 1 - var15);
			}

			if(var16 < 4.0D) {
				var16 = 4.0D - var16;
				var14[var15] -= var16 * var16 * var16 * 10.0D;
			}
		}

		for(var15 = 0; var15 < var5; ++var15) {
			for(int var36 = 0; var36 < var7; ++var36) {
				double var17 = (this.noiseDataA[var13] + 256.0D) / 512.0D;
				if(var17 > 1.0D) {
					var17 = 1.0D;
				}

				double var19 = 0.0D;
				double var21 = this.noiseDataB[var13] / 8000.0D;
				if(var21 < 0.0D) {
					var21 = -var21;
				}

				var21 = var21 * 3.0D - 3.0D;
				if(var21 < 0.0D) {
					var21 /= 2.0D;
					if(var21 < -1.0D) {
						var21 = -1.0D;
					}

					var21 /= 1.4D;
					var21 /= 2.0D;
					var17 = 0.0D;
				} else {
					if(var21 > 1.0D) {
						var21 = 1.0D;
					}

					var21 /= 6.0D;
				}

				var17 += 0.5D;
				var21 = var21 * (double)var6 / 16.0D;
				++var13;

				for(int var23 = 0; var23 < var6; ++var23) {
					double var24 = 0.0D;
					double var26 = var14[var23];
					double var28 = this.noiseData6[var12] / 512.0D;
					double var30 = this.noiseData7[var12] / 512.0D;
					double var32 = (this.noiseData5[var12] / 10.0D + 1.0D) / 2.0D;
					if(var32 < 0.0D) {
						var24 = var28;
					} else if(var32 > 1.0D) {
						var24 = var30;
					} else {
						var24 = var28 + (var30 - var28) * var32;
					}

					var24 -= var26;
					double var34;
					if(var23 > var6 - 4) {
						var34 = (double)((float)(var23 - (var6 - 4)) / 3.0F);
						var24 = var24 * (1.0D - var34) + -10.0D * var34;
					}

					if((double)var23 < var19) {
						var34 = (var19 - (double)var23) / 4.0D;
						if(var34 < 0.0D) {
							var34 = 0.0D;
						}

						if(var34 > 1.0D) {
							var34 = 1.0D;
						}

						var24 = var24 * (1.0D - var34) + -10.0D * var34;
					}

					var1[var12] = var24;
					++var12;
				}
			}
		}

		return var1;
	}

	public boolean chunkExists(int var1, int var2) {
		return true;
	}

	public void populate(IChunkProvider var1, int var2, int var3) {
		BlockSand.fallInstantly = true;
		int var4 = var2 * 16;
		int var5 = var3 * 16;

		int var6;
		int var7;
		int var8;
		int var9;
		for(var6 = 0; var6 < 8; ++var6) {
			var7 = var4 + this.rand.nextInt(16) + 8;
			var8 = this.rand.nextInt(120) + 4;
			var9 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenHellLava(Block.lavaStill.blockID)).generate(this.worldObj, this.rand, var7, var8, var9);
		}

		var6 = this.rand.nextInt(this.rand.nextInt(10) + 1) + 1;

		int var10;
		for(var7 = 0; var7 < var6; ++var7) {
			var8 = var4 + this.rand.nextInt(16) + 8;
			var9 = this.rand.nextInt(120) + 4;
			var10 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenFire()).generate(this.worldObj, this.rand, var8, var9, var10);
		}

		var6 = this.rand.nextInt(this.rand.nextInt(10) + 1);

		for(var7 = 0; var7 < var6; ++var7) {
			var8 = var4 + this.rand.nextInt(16) + 8;
			var9 = this.rand.nextInt(120) + 4;
			var10 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenLightStone1()).generate(this.worldObj, this.rand, var8, var9, var10);
		}

		for(var7 = 0; var7 < 10; ++var7) {
			var8 = var4 + this.rand.nextInt(16) + 8;
			var9 = this.rand.nextInt(128);
			var10 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenLightStone2()).generate(this.worldObj, this.rand, var8, var9, var10);
		}

		if(this.rand.nextInt(1) == 0) {
			var7 = var4 + this.rand.nextInt(16) + 8;
			var8 = this.rand.nextInt(128);
			var9 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenFlowers(Block.mushroomBrown.blockID)).generate(this.worldObj, this.rand, var7, var8, var9);
		}

		if(this.rand.nextInt(1) == 0) {
			var7 = var4 + this.rand.nextInt(16) + 8;
			var8 = this.rand.nextInt(128);
			var9 = var5 + this.rand.nextInt(16) + 8;
			(new WorldGenFlowers(Block.mushroomRed.blockID)).generate(this.worldObj, this.rand, var7, var8, var9);
		}

		BlockSand.fallInstantly = false;
	}

	public boolean saveChunks(boolean var1, IProgressUpdate var2) {
		return true;
	}

	public boolean unloadQueuedChunks() {
		return false;
	}

	public boolean canSave() {
		return true;
	}
}
