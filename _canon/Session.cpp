#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Session.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Session.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
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

JList<void*> Session::s_registeredBlocksList;

Session::Session(const JavaString& var1, const JavaString& var2)
{
	this->inventory = var1;
	this->sessionId = var2;
}

Session::~Session()
{
}

void Session::initStatics()
{
	s_registeredBlocksList.push_back(Block::s_stone);
	s_registeredBlocksList.push_back(Block::s_cobblestone);
	s_registeredBlocksList.push_back(Block::s_brick);
	s_registeredBlocksList.push_back(Block::s_dirt);
	s_registeredBlocksList.push_back(Block::s_planks);
	s_registeredBlocksList.push_back(Block::s_wood);
	s_registeredBlocksList.push_back(Block::s_leaves);
	s_registeredBlocksList.push_back(Block::s_torchWood);
	s_registeredBlocksList.push_back(Block::s_stairSingle);
	s_registeredBlocksList.push_back(Block::s_glass);
	s_registeredBlocksList.push_back(Block::s_cobblestoneMossy);
	s_registeredBlocksList.push_back(Block::s_sapling);
	s_registeredBlocksList.push_back(Block::s_plantYellow);
	s_registeredBlocksList.push_back(Block::s_plantRed);
	s_registeredBlocksList.push_back(Block::s_mushroomBrown);
	s_registeredBlocksList.push_back(Block::s_mushroomRed);
	s_registeredBlocksList.push_back(Block::s_sand);
	s_registeredBlocksList.push_back(Block::s_gravel);
	s_registeredBlocksList.push_back(Block::s_sponge);
	s_registeredBlocksList.push_back(Block::s_cloth);
	s_registeredBlocksList.push_back(Block::s_oreCoal);
	s_registeredBlocksList.push_back(Block::s_oreIron);
	s_registeredBlocksList.push_back(Block::s_oreGold);
	s_registeredBlocksList.push_back(Block::s_blockSteel);
	s_registeredBlocksList.push_back(Block::s_blockGold);
	s_registeredBlocksList.push_back(Block::s_bookShelf);
	s_registeredBlocksList.push_back(Block::s_tnt);
	s_registeredBlocksList.push_back(Block::s_obsidian);
	System::out.println(s_registeredBlocksList.size());
}
