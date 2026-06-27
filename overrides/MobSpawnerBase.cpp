#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MobSpawnerBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MobSpawnerBase.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Color.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumCreatureType.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerDesert.h"
#include "MobSpawnerHell.h"
#include "MobSpawnerSwamp.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

MobSpawnerBase* MobSpawnerBase::s_rainforest = (new MobSpawnerBase())->setColor(588342)->setBiomeName("Rainforest")->setGrassColor(2094168);
MobSpawnerBase* MobSpawnerBase::s_swampland = (new MobSpawnerSwamp())->setColor(522674)->setBiomeName("Swampland")->setGrassColor(9154376);
MobSpawnerBase* MobSpawnerBase::s_seasonalForest = (new MobSpawnerBase())->setColor(10215459)->setBiomeName("Seasonal Forest");
MobSpawnerBase* MobSpawnerBase::s_forest = (new MobSpawnerBase())->setColor(353825)->setBiomeName("Forest")->setGrassColor(5159473);
MobSpawnerBase* MobSpawnerBase::s_savanna = (new MobSpawnerDesert())->setColor(14278691)->setBiomeName("Savanna");
MobSpawnerBase* MobSpawnerBase::s_shrubland = (new MobSpawnerBase())->setColor(10595616)->setBiomeName("Shrubland");
MobSpawnerBase* MobSpawnerBase::s_taiga = (new MobSpawnerBase())->setColor(3060051)->setBiomeName("Taiga")->setEnableSnow()->setGrassColor(8107825);
MobSpawnerBase* MobSpawnerBase::s_desert = (new MobSpawnerDesert())->setColor(16421912)->setBiomeName("Desert");
MobSpawnerBase* MobSpawnerBase::s_plains = (new MobSpawnerDesert())->setColor(16767248)->setBiomeName("Plains");
MobSpawnerBase* MobSpawnerBase::s_iceDesert = (new MobSpawnerDesert())->setColor(16772499)->setBiomeName("Ice Desert")->setEnableSnow()->setGrassColor(12899129);
MobSpawnerBase* MobSpawnerBase::s_tundra = (new MobSpawnerBase())->setColor(5762041)->setBiomeName("Tundra")->setEnableSnow()->setGrassColor(12899129);
MobSpawnerBase* MobSpawnerBase::s_hell = (new MobSpawnerHell())->setColor(16711680)->setBiomeName("Hell");
JArray<MobSpawnerBase* > MobSpawnerBase::s_biomeLookupTable = JArray<MobSpawnerBase* >(4096);

MobSpawnerBase::~MobSpawnerBase()
{
}

void MobSpawnerBase::generateBiomeLookup()
{
	for(int var0 = 0; var0 < 64; ++var0) {
	for(int var1 = 0; var1 < 64; ++var1) {
	s_biomeLookupTable[var0 + var1 * 64] = getBiome((float)var0 / 63.0F, (float)var1 / 63.0F);
	}
	}
	s_desert->topBlock = s_desert->fillerBlock = (unsigned char)Block::s_sand->blockID;
	s_iceDesert->topBlock = s_iceDesert->fillerBlock = (unsigned char)Block::s_sand->blockID;
}

MobSpawnerBase* MobSpawnerBase::setEnableSnow()
{
	return this;
}

MobSpawnerBase* MobSpawnerBase::setBiomeName(const JavaString& var1)
{
	this->biomeName = var1;
	return this;
}

MobSpawnerBase* MobSpawnerBase::setGrassColor(int var1)
{
	this->grassColor = var1;
	return this;
}

MobSpawnerBase* MobSpawnerBase::setColor(int var1)
{
	this->color = var1;
	return this;
}

MobSpawnerBase* MobSpawnerBase::getBiomeFromLookup(double var0, double var2)
{
	int var4 = (int)(var0 * 63.0);
	int var5 = (int)(var2 * 63.0);
	return s_biomeLookupTable[var4 + var5 * 64];
}

MobSpawnerBase* MobSpawnerBase::getBiome(float var0, float var1)
{
	var1 *= var0;
	return var0 < 0.1F ? s_tundra : (var1 < 0.2F ? (var0 < 0.5F ? s_tundra : (var0 < 0.95F ? s_savanna : s_desert)) : (var1 > 0.5F && var0 < 0.7F ? s_swampland : (var0 < 0.5F ? s_taiga : (var0 < 0.97F ? (var1 < 0.35F ? s_shrubland : s_forest) : (var1 < 0.45F ? s_plains : (var1 < 0.9F ? s_seasonalForest : s_rainforest))))));
}

int MobSpawnerBase::getSkyColorByTemp(float var1)
{
	var1 /= 3.0F;
	if(var1 < -1.0F) {
	var1 = -1.0F;
	}
	if(var1 > 1.0F) {
	var1 = 1.0F;
	}
	return Color::getHSBColor(224.0F / 360.0F - var1 * 0.05F, 0.5F + var1 * 0.1F, 1.0F).getRGB();
}

JArray<Class* > MobSpawnerBase::getEntitiesForType(int var1)
{
	// Java returns null for unknown types; the JArray equivalent is an empty (null-buffer)
	// array. The ternary is unrolled so every branch has type JArray<Class*>.
	if(var1 == EnumCreatureType::monster) {
	return this->biomeMonsters;
	} else if(var1 == EnumCreatureType::creature) {
	return this->biomeCreatures;
	} else {
	return JArray<Class* >();
	}
}
