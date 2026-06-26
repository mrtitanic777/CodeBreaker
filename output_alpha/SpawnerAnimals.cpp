#include "stdafx.h"
// Auto-generated from SpawnerAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - dropped `#include "Set.h"`; s_eligibleChunksForSpawning is std::vector<void*>* (was Set).
//  - EnumCreatureType.values()/length -> values_/values_count; field names field_4278_c /
//    maxNumberOfEntityType (no m_ prefix in the enum struct).
//  - PORT-BLOCKED: the vanilla spawn loop builds an EntityLiving via Java reflection
//    (var33[var8].getConstructor(World.class).newInstance(world)) from a Class[] returned by
//    biome.getEntitiesForType(). The port has no .class literals / reflection, and those biome
//    entity-class lists are NULL (see MobSpawnerBase) -> getEntitiesForType() returns NULL, so
//    the vanilla inner loop would `continue` anyway. We keep the eligible-chunk bookkeeping and
//    the per-creature-type gate; actual instantiation is left to the project's other spawn path
//    (Debug::forceSpawnNightMobs + the live spawn system).

#include "SpawnerAnimals.h"
#include "Common.h"
#include "ChunkPosition.h"
#include <vector>
#include "World.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "Material.h"
#include "ChunkCoordIntPair.h"
#include "EnumCreatureType.h"
#include "MobSpawnerBase.h"
#include "WorldChunkManager.h"

std::vector<void*>* SpawnerAnimals::s_eligibleChunksForSpawning = new std::vector<void*>();

SpawnerAnimals::SpawnerAnimals()
{
}

SpawnerAnimals::~SpawnerAnimals() {
	// No dynamic resources
}

ChunkPosition* SpawnerAnimals::getRandomSpawningPointInChunk(World* var0, int var1, int var2) {
		int var3 = var1 + var0->rand->nextInt(16);
		int var4 = var0->rand->nextInt(128);
		int var5 = var2 + var0->rand->nextInt(16);
		return new ChunkPosition(var3, var4, var5);
}

int SpawnerAnimals::performSpawning(World* var0) {
		// free last pass's eligible-chunk entries, then rebuild.
		for(size_t _d = 0; _d < s_eligibleChunksForSpawning->size(); ++_d) {
			delete (ChunkCoordIntPair*)(*s_eligibleChunksForSpawning)[_d];
		}
		s_eligibleChunksForSpawning->clear();

		int var1;
		for(var1 = 0; var1 < (int)var0->m_playerEntities.size(); ++var1) {
			EntityPlayer* var2 = (EntityPlayer*)var0->m_playerEntities[var1];
			int var3 = (int)floor(var2->posX / 16.0f);
			int var4 = (int)floor(var2->posZ / 16.0f);
			unsigned char var5 = 8;

			for(int var6 = -var5; var6 <= var5; ++var6) {
				for(int var7 = -var5; var7 <= var5; ++var7) {
					s_eligibleChunksForSpawning->push_back(new ChunkCoordIntPair(var6 + var3, var7 + var4));
				}
			}
		}

		var1 = 0;

		for(int var28 = 0; var28 < EnumCreatureType::values_count; ++var28) {
			EnumCreatureType* var29 = EnumCreatureType::values_[var28];
			if(var0->countEntities(var29->field_4278_c) <= var29->maxNumberOfEntityType * (int)s_eligibleChunksForSpawning->size() / 256) {
				// Vanilla iterates the eligible chunks and reflectively instantiates a random
				// entity from biome.getEntitiesForType(var29). That Class[] is unavailable in the
				// port (always NULL), so there is nothing to instantiate here.
				for(size_t var30 = 0; var30 < s_eligibleChunksForSpawning->size(); ++var30) {
					ChunkCoordIntPair* var31 = (ChunkCoordIntPair*)(*s_eligibleChunksForSpawning)[var30];
					if(var0->rand->nextInt(50) != 0) {
						continue;
					}

					MobSpawnerBase* var32 = var0->func_4075_a()->func_4074_a(var31);
					void* var33 = var32->getEntitiesForType(var29);
					if(var33 == NULL) {
						// PORT-BLOCKED: no entity .class list / reflection on 360 -> cannot spawn here.
						continue;
					}

					// PORT-BLOCKED: var34 = (EntityLiving)var33[i].getConstructor(World.class)
					//               .newInstance(var0);  -- Java reflection, not portable to C++03.
					// (Unreachable in practice because var33 is always NULL above.)
				}
			}
		}

		return var1;
}
