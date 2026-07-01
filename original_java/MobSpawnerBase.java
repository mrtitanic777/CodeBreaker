package net.minecraft.src;

import java.awt.Color;

public class MobSpawnerBase {
	public static final MobSpawnerBase rainforest = (new MobSpawnerBase()).setColor(588342).setBiomeName("Rainforest").setGrassColor(2094168);
	public static final MobSpawnerBase swampland = (new MobSpawnerSwamp()).setColor(522674).setBiomeName("Swampland").setGrassColor(9154376);
	public static final MobSpawnerBase seasonalForest = (new MobSpawnerBase()).setColor(10215459).setBiomeName("Seasonal Forest");
	public static final MobSpawnerBase forest = (new MobSpawnerBase()).setColor(353825).setBiomeName("Forest").setGrassColor(5159473);
	public static final MobSpawnerBase savanna = (new MobSpawnerDesert()).setColor(14278691).setBiomeName("Savanna");
	public static final MobSpawnerBase shrubland = (new MobSpawnerBase()).setColor(10595616).setBiomeName("Shrubland");
	public static final MobSpawnerBase taiga = (new MobSpawnerBase()).setColor(3060051).setBiomeName("Taiga").setEnableSnow().setGrassColor(8107825);
	public static final MobSpawnerBase desert = (new MobSpawnerDesert()).setColor(16421912).setBiomeName("Desert");
	public static final MobSpawnerBase plains = (new MobSpawnerDesert()).setColor(16767248).setBiomeName("Plains");
	public static final MobSpawnerBase iceDesert = (new MobSpawnerDesert()).setColor(16772499).setBiomeName("Ice Desert").setEnableSnow().setGrassColor(12899129);
	public static final MobSpawnerBase tundra = (new MobSpawnerBase()).setColor(5762041).setBiomeName("Tundra").setEnableSnow().setGrassColor(12899129);
	public static final MobSpawnerBase hell = (new MobSpawnerHell()).setColor(16711680).setBiomeName("Hell");
	public String biomeName;
	public int color;
	public byte topBlock = (byte)Block.grass.blockID;
	public byte fillerBlock = (byte)Block.dirt.blockID;
	public int grassColor = 5169201;
	protected Class[] biomeMonsters = new Class[]{EntitySpider.class, EntityZombie.class, EntitySkeleton.class, EntityCreeper.class};
	protected Class[] biomeCreatures = new Class[]{EntitySheep.class, EntityPig.class, EntityChicken.class, EntityCow.class};
	private static MobSpawnerBase[] biomeLookupTable = new MobSpawnerBase[4096];

	public static void generateBiomeLookup() {
		for(int var0 = 0; var0 < 64; ++var0) {
			for(int var1 = 0; var1 < 64; ++var1) {
				biomeLookupTable[var0 + var1 * 64] = getBiome((float)var0 / 63.0F, (float)var1 / 63.0F);
			}
		}

		desert.topBlock = desert.fillerBlock = (byte)Block.sand.blockID;
		iceDesert.topBlock = iceDesert.fillerBlock = (byte)Block.sand.blockID;
	}

	protected MobSpawnerBase setEnableSnow() {
		return this;
	}

	protected MobSpawnerBase setBiomeName(String var1) {
		this.biomeName = var1;
		return this;
	}

	protected MobSpawnerBase setGrassColor(int var1) {
		this.grassColor = var1;
		return this;
	}

	protected MobSpawnerBase setColor(int var1) {
		this.color = var1;
		return this;
	}

	public static MobSpawnerBase getBiomeFromLookup(double var0, double var2) {
		int var4 = (int)(var0 * 63.0D);
		int var5 = (int)(var2 * 63.0D);
		return biomeLookupTable[var4 + var5 * 64];
	}

	public static MobSpawnerBase getBiome(float var0, float var1) {
		var1 *= var0;
		return var0 < 0.1F ? tundra : (var1 < 0.2F ? (var0 < 0.5F ? tundra : (var0 < 0.95F ? savanna : desert)) : (var1 > 0.5F && var0 < 0.7F ? swampland : (var0 < 0.5F ? taiga : (var0 < 0.97F ? (var1 < 0.35F ? shrubland : forest) : (var1 < 0.45F ? plains : (var1 < 0.9F ? seasonalForest : rainforest))))));
	}

	public int getSkyColorByTemp(float var1) {
		var1 /= 3.0F;
		if(var1 < -1.0F) {
			var1 = -1.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		return Color.getHSBColor(224.0F / 360.0F - var1 * 0.05F, 0.5F + var1 * 0.1F, 1.0F).getRGB();
	}

	public Class[] getEntitiesForType(EnumCreatureType var1) {
		return var1 == EnumCreatureType.monster ? this.biomeMonsters : (var1 == EnumCreatureType.creature ? this.biomeCreatures : null);
	}

	static {
		generateBiomeLookup();
	}
}
