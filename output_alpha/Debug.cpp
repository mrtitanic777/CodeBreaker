#include "stdafx.h"
// Debug helpers (handwritten_alpha override). Originally ported from the indev
// reference; re-fitted to ALPHA's World API:
//   indev            -> alpha
//   m_xSpawn/Y/Z     -> m_spawnX/m_spawnY/m_spawnZ
//   setTileNoUpdate  -> setBlockWithNotify
//   m_random         -> rand
//   m_playerEntity   -> m_playerEntities[0]  (std::vector<void*>)
//   spawnEntityInWorld -> entityJoinedWorld
// Target: Xbox 360 / C++03 / DirectX 9
#include "Debug.h"
#include "Common.h"
#include "World.h"
#include "Block.h"
#include "TileEntityChest.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "EntityLiving.h"
#include "Entity.h"
#include "Random.h"
#include "EntitySkeleton.h"
#include "EntityCreeper.h"
#include "EntitySpider.h"
#include "EntityZombie.h"

namespace Debug {

// Alpha keeps players in a vector<void*>; in singleplayer there is exactly one.
static EntityPlayer* getDebugPlayer(World* level) {
	if (level == NULL || level->m_playerEntities.empty()) {
		return NULL;
	}
	return (EntityPlayer*)level->m_playerEntities[0];
}

// Two starter chests pre-filled with test contents. The indev "spawn house" is
// gone in this Alpha build, so the chests are placed directly BESIDE the spawn
// point (m_spawnX +-1) -- i.e. right where the player appears -- instead of
// against the house's interior wall. Chest block = Block::s_crate (id 54).
// NOTE: not auto-called anywhere; wire it into the post-generate path if wanted.
void spawnDebugChests(World* level) {
	if (level == NULL || Block::s_crate == NULL) {
		return;
	}

	int chestX = level->m_spawnX;
	int chestY = level->m_spawnY;
	int chestZ = level->m_spawnZ;
	int chestId = Block::s_crate->blockID;

	// --- Chest 1 (left of spawn): one of every basic block + a few starter tools ---
	level->setBlockWithNotify(chestX - 1, chestY, chestZ, chestId);
	TileEntityChest* chest1 = new TileEntityChest();
	level->setBlockTileEntity(chestX - 1, chestY, chestZ, chest1);

	static const int ids1[] = { 1,2,3,4,5,6,7, 12,13, 14,15,16, 17,18,19,20, 21,22,23,24,25,26,27 };
	const int n1 = (int)(sizeof(ids1) / sizeof(ids1[0]));   // 23
	int slot;
	for (slot = 0; slot < n1 && slot < chest1->getSizeInventory(); ++slot) {
		chest1->setInventorySlotContents(slot, new ItemStack(ids1[slot], 16));
	}
	chest1->setInventorySlotContents(23, new ItemStack(261, 1));    // bow
	chest1->setInventorySlotContents(24, new ItemStack(262, 64));   // arrows
	chest1->setInventorySlotContents(25, new ItemStack(259, 1));    // flint & steel

	// --- Chest 2 (right of spawn): the remaining blocks ---
	level->setBlockWithNotify(chestX + 1, chestY, chestZ, chestId);
	TileEntityChest* chest2 = new TileEntityChest();
	level->setBlockTileEntity(chestX + 1, chestY, chestZ, chest2);

	static const int ids2[] = { 28,29,30,31,32,33,34,35,36, 37,38,39,40, 41,42,43,44,45,46,47,48,49,50, 54,56,57,58 };
	const int n2 = (int)(sizeof(ids2) / sizeof(ids2[0]));   // 27
	for (slot = 0; slot < n2 && slot < chest2->getSizeInventory(); ++slot) {
		int stackSize = (ids2[slot] == 46) ? 64 : 16;   // 46 = TNT
		chest2->setInventorySlotContents(slot, new ItemStack(ids2[slot], stackSize));
	}

	DbgPrintF("[DEBUG] Spawned debug chests beside spawn at (%d, %d, %d)\n", chestX, chestY, chestZ);
}

// DEBUG: force one of each hostile (night) mob to spawn right next to the player in BROAD DAYLIGHT,
// bypassing the normal light/darkness spawn gate. Respects difficulty: nothing on Peaceful (==0).
void forceSpawnNightMobs(World* level) {
	if (level == NULL) {
		return;
	}
	if (level->m_difficultySetting == 0) {
		DbgPrintF("[DEBUG] forceSpawnNightMobs: Peaceful (difficulty 0) -> no hostile mobs\n");
		return;
	}
	EntityPlayer* player = getDebugPlayer(level);
	if (player == NULL || level->rand == NULL) {
		return;
	}

	// Spawn the batch in a ROW directly IN FRONT of the player (computed from yaw) so they're in view.
	const int N = 4;
	float yawRad = player->rotationYaw * 0.017453292F;   // deg -> rad
	float fx = -sinf(yawRad), fz = cosf(yawRad);          // forward look vector (pitch 0)
	float sx =  cosf(yawRad), sz = sinf(yawRad);          // right (perpendicular)
	float px = (float)player->posX, py = (float)player->posY, pz = (float)player->posZ;
	const float lateral[N] = { -2.4F, -0.8F, 0.8F, 2.4F };

	for (int t = 0; t < N; ++t) {
		EntityLiving* mob = NULL;
		switch (t) {
			case 0: mob = new EntitySkeleton(level); break;
			case 1: mob = new EntityCreeper(level);  break;
			case 2: mob = new EntitySpider(level);   break;
			case 3: mob = new EntityZombie(level);   break;
		}
		if (mob == NULL) {
			continue;
		}
		float mx = px + fx * 4.0F + sx * lateral[t];
		float mz = pz + fz * 4.0F + sz * lateral[t];
		mob->setPositionAndRotation(mx, py, mz, level->rand->nextFloat() * 360.0F, 0.0F);
		level->entityJoinedWorld((Entity*)mob);
	}

	DbgPrintF("[DEBUG] forceSpawnNightMobs: spawned skeleton/creeper/spider/zombie near player\n");
}

} // namespace Debug
