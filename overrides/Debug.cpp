#include "stdafx.h"
// Debug helpers (handwritten_alpha override). Originally ported from the indev
// reference; re-fitted to ALPHA's World API:
//   indev            -> alpha
//   m_xSpawn/Y/Z     -> spawnX/spawnY/spawnZ
//   setTileNoUpdate  -> setBlockWithNotify
//   m_random         -> rand
//   m_playerEntity   -> playerEntities[0]  (std::vector<void*>)
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
	if (level == NULL || level->playerEntities.empty()) {
		return NULL;
	}
	return (EntityPlayer*)level->playerEntities[0];
}

// Two starter chests pre-filled with test contents. The indev "spawn house" is
// gone in this Alpha build, so the chests are placed directly BESIDE the spawn
// point (spawnX +-1) -- i.e. right where the player appears -- instead of
// against the house's interior wall. Chest block = Block::s_crate (id 54).
// NOTE: not auto-called anywhere; wire it into the post-generate path if wanted.
void spawnDebugChests(World* level) {
	if (level == NULL || Block::s_crate == NULL) {
		return;
	}

	int chestX = level->spawnX;
	int chestZ = level->spawnZ;
	int chestId = Block::s_crate->blockID;
	// FIX("chests despawned"): place each chest ON THE SURFACE via getHeightValue(x,z). The old code used
	// spawnY (a fixed 64), but the generated terrain top is seed-dependent (e.g. surfaceY=66) -> the chest
	// was set 1-2 blocks UNDERGROUND, buried and invisible. getHeightValue gives the first free Y per column.
	int y1 = level->getHeightValue(chestX - 1, chestZ);
	int y2 = level->getHeightValue(chestX + 1, chestZ);

	// --- Chest 1 (left of spawn): one of every basic block + a few starter tools ---
	level->setBlockWithNotify(chestX - 1, y1, chestZ, chestId);
	TileEntityChest* chest1 = new TileEntityChest();
	level->setBlockTileEntity(chestX - 1, y1, chestZ, chest1);

	// NOTE: this Alpha v1.2.6 has NO blocks at ids 21-34 or 36 (s_blocksList holes), so an ItemStack of
	// one of those is an unregistered item -> opening/mining the chest used to IP-0 on its render. Skip any
	// id whose block isn't registered (compacting into contiguous slots) so the chest only holds real blocks.
	static const int ids1[] = { 1,2,3,4,5,6,7, 12,13, 14,15,16, 17,18,19,20, 21,22,23,24,25,26,27 };
	const int n1 = (int)(sizeof(ids1) / sizeof(ids1[0]));
	int out = 0;
	for (int i = 0; i < n1 && out < chest1->getSizeInventory(); ++i) {
		if (ids1[i] >= 256 || Block::s_blocksList[ids1[i]] == NULL) { continue; }   // skip gap/unregistered ids
		chest1->setInventorySlotContents(out++, new ItemStack(ids1[i], 16));
	}
	chest1->setInventorySlotContents(23, new ItemStack(261, 1));    // bow
	chest1->setInventorySlotContents(24, new ItemStack(262, 64));   // arrows
	chest1->setInventorySlotContents(25, new ItemStack(259, 1));    // flint & steel

	// --- Chest 2 (right of spawn): the remaining blocks ---
	level->setBlockWithNotify(chestX + 1, y2, chestZ, chestId);
	TileEntityChest* chest2 = new TileEntityChest();
	level->setBlockTileEntity(chestX + 1, y2, chestZ, chest2);

	static const int ids2[] = { 28,29,30,31,32,33,34,35,36, 37,38,39,40, 41,42,43,44,45,46,47,48,49,50, 54,56,57,58 };
	const int n2 = (int)(sizeof(ids2) / sizeof(ids2[0]));
	out = 0;
	for (int i = 0; i < n2 && out < chest2->getSizeInventory(); ++i) {
		if (ids2[i] >= 256 || Block::s_blocksList[ids2[i]] == NULL) { continue; }   // skip gap/unregistered ids
		int stackSize = (ids2[i] == 46) ? 64 : 16;   // 46 = TNT
		chest2->setInventorySlotContents(out++, new ItemStack(ids2[i], stackSize));
	}

	DbgPrintF("[DEBUG] Spawned debug chests beside spawn at (%d, ~%d, %d)\n", chestX, y1, chestZ);
}

// DEBUG: force one of each hostile (night) mob to spawn right next to the player in BROAD DAYLIGHT,
// bypassing the normal light/darkness spawn gate. Respects difficulty: nothing on Peaceful (==0).
void forceSpawnNightMobs(World* level) {
	if (level == NULL) {
		return;
	}
	if (level->difficultySetting == 0) {
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
