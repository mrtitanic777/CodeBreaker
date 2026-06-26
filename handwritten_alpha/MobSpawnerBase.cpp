#include "stdafx.h"
// Auto-generated from MobSpawnerBase.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - s_biomeLookupTable is MobSpawnerBase** (BiomeGenBase[]); allocate 4096 biome slots.
//  - getBiomeFromLookup returns a single biome pointer (table element).
//  - biome monster/creature lists held entity .class literals (no C++03 reflection) -> NULL.
//  - java.awt.Color.getHSBColor(...).getRGB() -> local HSBtoRGB() helper (no Color class on 360).

#include "MobSpawnerBase.h"
#include "Common.h"
#include "BlockGrass.h"
#include "Class.h"
#include "EnumCreatureType.h"
#include <string>
#include <math.h>
#include "Block.h"
#include "MobSpawnerDesert.h"
#include "MobSpawnerHell.h"
#include "MobSpawnerSwamp.h"

// PORT: java.awt.Color.getHSBColor(h,s,b).getRGB() & 0xFFFFFF -> packed 0x00RRGGBB.
// Mirrors java.awt.Color.HSBtoRGB (same helper used in GuiIngame.cpp).
static int MobSpawnerBase_HSBtoRGB(float hue, float saturation, float brightness) {
	int r = 0, g = 0, b = 0;
	if(saturation == 0.0F) {
		r = g = b = (int)(brightness * 255.0F + 0.5F);
	} else {
		float h = (hue - (float)floorf(hue)) * 6.0F;
		float f = h - (float)floorf(h);
		float p = brightness * (1.0F - saturation);
		float q = brightness * (1.0F - saturation * f);
		float t = brightness * (1.0F - saturation * (1.0F - f));
		switch((int)h) {
		case 0:  r = (int)(brightness * 255.0F + 0.5F); g = (int)(t * 255.0F + 0.5F); b = (int)(p * 255.0F + 0.5F); break;
		case 1:  r = (int)(q * 255.0F + 0.5F); g = (int)(brightness * 255.0F + 0.5F); b = (int)(p * 255.0F + 0.5F); break;
		case 2:  r = (int)(p * 255.0F + 0.5F); g = (int)(brightness * 255.0F + 0.5F); b = (int)(t * 255.0F + 0.5F); break;
		case 3:  r = (int)(p * 255.0F + 0.5F); g = (int)(q * 255.0F + 0.5F); b = (int)(brightness * 255.0F + 0.5F); break;
		case 4:  r = (int)(t * 255.0F + 0.5F); g = (int)(p * 255.0F + 0.5F); b = (int)(brightness * 255.0F + 0.5F); break;
		default: r = (int)(brightness * 255.0F + 0.5F); g = (int)(p * 255.0F + 0.5F); b = (int)(q * 255.0F + 0.5F); break;
		}
	}
	return (r << 16) | (g << 8) | b;
}

MobSpawnerBase* MobSpawnerBase::s_rainforest = (new MobSpawnerBase())->func_4123_b(588342)->setBiomeName("Rainforest")->func_4124_a(2094168);
MobSpawnerBase* MobSpawnerBase::s_swampland = (new MobSpawnerSwamp())->func_4123_b(522674)->setBiomeName("Swampland")->func_4124_a(9154376);
MobSpawnerBase* MobSpawnerBase::s_seasonalForest = (new MobSpawnerBase())->func_4123_b(10215459)->setBiomeName("Seasonal Forest");
MobSpawnerBase* MobSpawnerBase::s_forest = (new MobSpawnerBase())->func_4123_b(353825)->setBiomeName("Forest")->func_4124_a(5159473);
MobSpawnerBase* MobSpawnerBase::s_savanna = (new MobSpawnerDesert())->func_4123_b(14278691)->setBiomeName("Savanna");
MobSpawnerBase* MobSpawnerBase::s_shrubland = (new MobSpawnerBase())->func_4123_b(10595616)->setBiomeName("Shrubland");
MobSpawnerBase* MobSpawnerBase::s_taiga = (new MobSpawnerBase())->func_4123_b(3060051)->setBiomeName("Taiga")->func_4122_b()->func_4124_a(8107825);
MobSpawnerBase* MobSpawnerBase::s_desert = (new MobSpawnerDesert())->func_4123_b(16421912)->setBiomeName("Desert");
MobSpawnerBase* MobSpawnerBase::s_plains = (new MobSpawnerDesert())->func_4123_b(16767248)->setBiomeName("Plains");
MobSpawnerBase* MobSpawnerBase::s_iceDesert = (new MobSpawnerDesert())->func_4123_b(16772499)->setBiomeName("Ice Desert")->func_4122_b()->func_4124_a(12899129);
MobSpawnerBase* MobSpawnerBase::s_tundra = (new MobSpawnerBase())->func_4123_b(5762041)->setBiomeName("Tundra")->func_4122_b()->func_4124_a(12899129);
MobSpawnerBase* MobSpawnerBase::s_hell = (new MobSpawnerHell())->func_4123_b(16711680)->setBiomeName("Hell");
// Java: BiomeGenBase[] biomeLookupTable = new BiomeGenBase[4096];
MobSpawnerBase** MobSpawnerBase::s_biomeLookupTable = new MobSpawnerBase*[4096];

MobSpawnerBase::MobSpawnerBase()
	: m_biomeName("")
	, m_field_6503_n(0)
	, m_topBlock((unsigned char)Block::s_grass->blockID)
	, m_fillerBlock((unsigned char)Block::s_dirt->blockID)
	, m_field_6502_q(5169201)
	, m_biomeMonsters(NULL)
	, m_biomeCreatures(NULL)
	, m_biomeMonsters_count(0)
	, m_biomeCreatures_count(0)
{
	// PORT-BLOCKED: biomeMonsters was {EntitySpider,EntityZombie,EntitySkeleton,EntityCreeper}.class
	//              biomeCreatures was {EntitySheep,EntityPig,EntityChicken,EntityCow}.class
	// No .class literals / reflection on 360 -> lists left empty (spawning handled elsewhere).
}

MobSpawnerBase::~MobSpawnerBase() {
	// m_biomeMonsters / m_biomeCreatures are never allocated (PORT-BLOCKED) -> nothing to free.
	m_biomeMonsters = NULL;
	m_biomeCreatures = NULL;
}

void MobSpawnerBase::generateBiomeLookup() {
		for(int var0 = 0; var0 < 64; ++var0) {
			for(int var1 = 0; var1 < 64; ++var1) {
				s_biomeLookupTable[var0 + var1 * 64] = getBiome((float)var0 / 63.0F, (float)var1 / 63.0F);
			}
		}

		s_desert->m_topBlock = s_desert->m_fillerBlock = (unsigned char)Block::s_sand->blockID;
		s_iceDesert->m_topBlock = s_iceDesert->m_fillerBlock = (unsigned char)Block::s_sand->blockID;
}

MobSpawnerBase* MobSpawnerBase::func_4122_b() {
		return this;
}

MobSpawnerBase* MobSpawnerBase::setBiomeName(const std::string& var1) {
		m_biomeName = var1;
		return this;
}

MobSpawnerBase* MobSpawnerBase::func_4124_a(int var1) {
		m_field_6502_q = var1;
		return this;
}

MobSpawnerBase* MobSpawnerBase::func_4123_b(int var1) {
		m_field_6503_n = var1;
		return this;
}

MobSpawnerBase* MobSpawnerBase::getBiomeFromLookup(double var0, double var2) {
		int var4 = (int)(var0 * 63.0f);
		int var5 = (int)(var2 * 63.0f);
		return s_biomeLookupTable[var4 + var5 * 64];
}

MobSpawnerBase* MobSpawnerBase::getBiome(float var0, float var1) {
		var1 *= var0;
		return var0 < 0.1F ? s_tundra : (var1 < 0.2F ? (var0 < 0.5F ? s_tundra : (var0 < 0.95F ? s_savanna : s_desert)) : (var1 > 0.5F && var0 < 0.7F ? s_swampland : (var0 < 0.5F ? s_taiga : (var0 < 0.97F ? (var1 < 0.35F ? s_shrubland : s_forest) : (var1 < 0.45F ? s_plains : (var1 < 0.9F ? s_seasonalForest : s_rainforest))))));
}

int MobSpawnerBase::getSkyColorByTemp(float var1) {
		var1 /= 3.0F;
		if(var1 < -1.0F) {
			var1 = -1.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		return MobSpawnerBase_HSBtoRGB(224.0F / 360.0F - var1 * 0.05F, 0.5F + var1 * 0.1F, 1.0F);
}

void* MobSpawnerBase::getEntitiesForType(EnumCreatureType* var1) {
		return var1 == EnumCreatureType::s_monster ? m_biomeMonsters : (var1 == EnumCreatureType::s_creature ? m_biomeCreatures : NULL);
}

void MobSpawnerBase::initStatics() {
		generateBiomeLookup();
}
