package net.minecraft.src;

import java.util.Arrays;

public class WorldChunkManagerHell extends WorldChunkManager {
	private MobSpawnerBase biome;
	private double fixedTemperature;
	private double fixedHumidity;

	public WorldChunkManagerHell(MobSpawnerBase var1, double var2, double var4) {
		this.biome = var1;
		this.fixedTemperature = var2;
		this.fixedHumidity = var4;
	}

	public MobSpawnerBase getBiomeForChunk(ChunkCoordIntPair var1) {
		return this.biome;
	}

	public MobSpawnerBase getBiome(int var1, int var2) {
		return this.biome;
	}

	public double getTemperature(int var1, int var2) {
		return this.fixedTemperature;
	}

	public MobSpawnerBase[] getBiomesGrid(int var1, int var2, int var3, int var4) {
		this.biomeCache = this.loadBlockGeneratorData(this.biomeCache, var1, var2, var3, var4);
		return this.biomeCache;
	}

	public double[] getTemperatures(double[] var1, int var2, int var3, int var4, int var5) {
		if(var1 == null || var1.length < var4 * var5) {
			var1 = new double[var4 * var5];
		}

		Arrays.fill(var1, 0, var4 * var5, this.fixedTemperature);
		return var1;
	}

	public MobSpawnerBase[] loadBlockGeneratorData(MobSpawnerBase[] var1, int var2, int var3, int var4, int var5) {
		if(var1 == null || var1.length < var4 * var5) {
			var1 = new MobSpawnerBase[var4 * var5];
			this.temperature = new double[var4 * var5];
			this.humidity = new double[var4 * var5];
		}

		Arrays.fill(var1, 0, var4 * var5, this.biome);
		Arrays.fill(this.humidity, 0, var4 * var5, this.fixedHumidity);
		Arrays.fill(this.temperature, 0, var4 * var5, this.fixedTemperature);
		return var1;
	}
}
