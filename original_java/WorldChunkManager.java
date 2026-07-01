package net.minecraft.src;

import java.util.Random;

public class WorldChunkManager {
	private NoiseGeneratorOctaves2 temperatureNoise;
	private NoiseGeneratorOctaves2 humidityNoise;
	private NoiseGeneratorOctaves2 noiseGen3;
	public double[] temperature;
	public double[] humidity;
	public double[] noiseArray;
	public MobSpawnerBase[] biomeCache;

	protected WorldChunkManager() {
	}

	public WorldChunkManager(World var1) {
		this.temperatureNoise = new NoiseGeneratorOctaves2(new Random(var1.randomSeed * 9871L), 4);
		this.humidityNoise = new NoiseGeneratorOctaves2(new Random(var1.randomSeed * 39811L), 4);
		this.noiseGen3 = new NoiseGeneratorOctaves2(new Random(var1.randomSeed * 543321L), 2);
	}

	public MobSpawnerBase getBiomeForChunk(ChunkCoordIntPair var1) {
		return this.getBiome(var1.chunkXPos, var1.chunkZPos);
	}

	public MobSpawnerBase getBiome(int var1, int var2) {
		return this.getBiomesGrid(var1, var2, 1, 1)[0];
	}

	public double getTemperature(int var1, int var2) {
		this.temperature = this.temperatureNoise.generateNoiseOctaves(this.temperature, (double)var1, (double)var2, 1, 1, (double)0.025F, (double)0.025F, 0.5D);
		return this.temperature[0];
	}

	public MobSpawnerBase[] getBiomesGrid(int var1, int var2, int var3, int var4) {
		this.biomeCache = this.loadBlockGeneratorData(this.biomeCache, var1, var2, var3, var4);
		return this.biomeCache;
	}

	public double[] getTemperatures(double[] var1, int var2, int var3, int var4, int var5) {
		if(var1 == null || var1.length < var4 * var5) {
			var1 = new double[var4 * var5];
		}

		var1 = this.temperatureNoise.generateNoiseOctaves(var1, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25D);
		this.noiseArray = this.noiseGen3.generateNoiseOctaves(this.noiseArray, (double)var2, (double)var3, var4, var4, 0.25D, 0.25D, 0.5882352941176471D);
		int var6 = 0;

		for(int var7 = 0; var7 < var4; ++var7) {
			for(int var8 = 0; var8 < var5; ++var8) {
				double var9 = this.noiseArray[var6] * 1.1D + 0.5D;
				double var11 = 0.01D;
				double var13 = 1.0D - var11;
				double var15 = (var1[var6] * 0.15D + 0.7D) * var13 + var9 * var11;
				var15 = 1.0D - (1.0D - var15) * (1.0D - var15);
				if(var15 < 0.0D) {
					var15 = 0.0D;
				}

				if(var15 > 1.0D) {
					var15 = 1.0D;
				}

				var1[var6] = var15;
				++var6;
			}
		}

		return var1;
	}

	public MobSpawnerBase[] loadBlockGeneratorData(MobSpawnerBase[] var1, int var2, int var3, int var4, int var5) {
		if(var1 == null || var1.length < var4 * var5) {
			var1 = new MobSpawnerBase[var4 * var5];
		}

		this.temperature = this.temperatureNoise.generateNoiseOctaves(this.temperature, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25D);
		this.humidity = this.humidityNoise.generateNoiseOctaves(this.humidity, (double)var2, (double)var3, var4, var4, (double)0.05F, (double)0.05F, 1.0D / 3.0D);
		this.noiseArray = this.noiseGen3.generateNoiseOctaves(this.noiseArray, (double)var2, (double)var3, var4, var4, 0.25D, 0.25D, 0.5882352941176471D);
		int var6 = 0;

		for(int var7 = 0; var7 < var4; ++var7) {
			for(int var8 = 0; var8 < var5; ++var8) {
				double var9 = this.noiseArray[var6] * 1.1D + 0.5D;
				double var11 = 0.01D;
				double var13 = 1.0D - var11;
				double var15 = (this.temperature[var6] * 0.15D + 0.7D) * var13 + var9 * var11;
				var11 = 0.002D;
				var13 = 1.0D - var11;
				double var17 = (this.humidity[var6] * 0.15D + 0.5D) * var13 + var9 * var11;
				var15 = 1.0D - (1.0D - var15) * (1.0D - var15);
				if(var15 < 0.0D) {
					var15 = 0.0D;
				}

				if(var17 < 0.0D) {
					var17 = 0.0D;
				}

				if(var15 > 1.0D) {
					var15 = 1.0D;
				}

				if(var17 > 1.0D) {
					var17 = 1.0D;
				}

				this.temperature[var6] = var15;
				this.humidity[var6] = var17;
				var1[var6++] = MobSpawnerBase.getBiomeFromLookup(var15, var17);
			}
		}

		return var1;
	}
}
