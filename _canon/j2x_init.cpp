#include "stdafx.h"
#include "JavaRT.h"
#include "j2x_init.h"

#include "MathHelper.h"
#include "Material.h"
#include "Block.h"
#include "Item.h"
#include "CraftingManager.h"
#include "EntityList.h"
#include "TileEntity.h"
#include "MobSpawnerBase.h"
#include "OsMap.h"
#include "Packet.h"
#include "ColorizerFoliage.h"
#include "ColorizerGrass.h"
#include "EntityPigZombie.h"
#include "EntitySkeleton.h"
#include "GuiContainer.h"
#include "RenderHelper.h"
#include "Session.h"

void j2x_initAll()
{
	MathHelper::initStatics();
	Material::initStatics();
	Block::initStatics();
	Item::initStatics();
	CraftingManager::initStatics();
	EntityList::initStatics();
	TileEntity::initStatics();
	MobSpawnerBase::initStatics();
	OsMap::initStatics();
	Packet::initStatics();
	ColorizerFoliage::initStatics();
	ColorizerGrass::initStatics();
	EntityPigZombie::initStatics();
	EntitySkeleton::initStatics();
	GuiContainer::initStatics();
	RenderHelper::initStatics();
	Session::initStatics();
}
