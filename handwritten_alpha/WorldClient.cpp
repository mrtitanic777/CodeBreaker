#include "stdafx.h"
// Auto-generated from WorldClient.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. WorldClient is the MP client world; SP never
// instantiates it. The two java.util.Set fields are modeled as
// std::vector<void*>* (see WorldClient.h). Packet sends are stubbed no-ops.

#include "WorldClient.h"
#include "Common.h"
#include "ChunkProviderClient.h"
#include "Entity.h"
#include "IChunkProvider.h"
#include "MCHashTable.h"
#include "NetClientHandler.h"
#include "TileEntity.h"
#include "IWorldAccess.h"
#include <string>
#include <vector>
#include <algorithm>
#include "World.h"
#include "WorldBlockPositionType.h"
#include "WorldProvider.h"

// --- small helpers for the std::vector<void*> "Set" stand-ins -------------
static bool _wcContains(std::vector<void*>* v, void* item) {
	if(v == NULL) return false;
	return std::find(v->begin(), v->end(), item) != v->end();
}
static void _wcRemove(std::vector<void*>* v, void* item) {
	if(v == NULL) return;
	v->erase(std::remove(v->begin(), v->end(), item), v->end());
}

WorldClient::WorldClient()
	: m_field_1057_z()
	, m_sendQueue(NULL)
	, m_C(NULL)
	, m_D(false)
	, m_field_1055_D(new MCHashTable())
	, m_F(new std::vector<void*>())
	, m_field_1053_F(new std::vector<void*>())
{
}

WorldClient::WorldClient(NetClientHandler* var1, long long var2, int var4)
	: World("MpServer", WorldProvider::func_4101_a(var4), var2)
	, m_field_1057_z()
	, m_sendQueue(NULL)
	, m_C(NULL)
	, m_D(false)
	, m_field_1055_D(new MCHashTable())
	, m_F(new std::vector<void*>())
	, m_field_1053_F(new std::vector<void*>())
{
		m_sendQueue = var1;
		m_spawnX = 8;
		m_spawnY = 64;
		m_spawnZ = 8;
}

WorldClient::~WorldClient() {
	m_sendQueue = NULL;
	m_C = NULL;
	m_field_1055_D = NULL;
	m_F = NULL;
	m_field_1053_F = NULL;
}

void WorldClient::tick() {
		++m_worldTime;
		int var1 = calculateSkylightSubtracted(1.0F);
		size_t var2;
		if(var1 != m_skylightSubtracted) {
			m_skylightSubtracted = var1;

			for(var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
				m_worldAccesses[var2]->func_936_e();
			}
		}

		for(var2 = 0; var2 < 10 && !m_field_1053_F->empty(); ++var2) {
			Entity* var3 = (Entity*)(*m_field_1053_F)[0];
			if(!_wcContains(&m_loadedEntityList, var3)) {
				entityJoinedWorld(var3);
			}
			// Java removed via iterator.next(); drop the head to advance.
			_wcRemove(m_field_1053_F, var3);
		}

		m_sendQueue->processReadPackets();

		for(var2 = 0; var2 < m_field_1057_z.size(); ++var2) {
			WorldBlockPositionType* var4 = (WorldBlockPositionType*)m_field_1057_z[var2];
			if(--var4->m_field_1206_d == 0) {
				World::setBlockAndMetadata(var4->m_field_1202_a, var4->m_field_1201_b, var4->m_field_1207_c, var4->m_field_1205_e, var4->m_field_1204_f);
				World::func_665_h(var4->m_field_1202_a, var4->m_field_1201_b, var4->m_field_1207_c);
				_vecRemoveAt(m_field_1057_z, (int)var2--);
			}
		}

}

void WorldClient::func_711_c(int var1, int var2, int var3, int var4, int var5, int var6) {
		for(size_t var7 = 0; var7 < m_field_1057_z.size(); ++var7) {
			WorldBlockPositionType* var8 = (WorldBlockPositionType*)m_field_1057_z[var7];
			if(var8->m_field_1202_a >= var1 && var8->m_field_1201_b >= var2 && var8->m_field_1207_c >= var3 && var8->m_field_1202_a <= var4 && var8->m_field_1201_b <= var5 && var8->m_field_1207_c <= var6) {
				_vecRemoveAt(m_field_1057_z, (int)var7--);
			}
		}

}

IChunkProvider* WorldClient::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
		m_C = new ChunkProviderClient(this);
		return m_C;
}

void WorldClient::func_4076_b() {
		m_spawnX = 8;
		m_spawnY = 64;
		m_spawnZ = 8;
}

void WorldClient::func_4080_j() {
	// TODO: implement
}

void WorldClient::scheduleBlockUpdate(int var1, int var2, int var3, int var4) {
	// TODO: implement
}

bool WorldClient::TickUpdates(bool var1) {
		return false;
}

void WorldClient::func_713_a(int var1, int var2, bool var3) {
		if(var3) {
			m_C->func_538_d(var1, var2);
		} else {
			m_C->func_539_c(var1, var2);
		}

		if(!var3) {
			func_701_b(var1 * 16, 0, var2 * 16, var1 * 16 + 15, 128, var2 * 16 + 15);
		}

}

bool WorldClient::entityJoinedWorld(Entity* var1) {
		bool var2 = World::entityJoinedWorld(var1);
		m_F->push_back(var1);
		if(!var2) {
			m_field_1053_F->push_back(var1);
		}

		return var2;
}

void WorldClient::setEntityDead(Entity* var1) {
		World::setEntityDead(var1);
		_wcRemove(m_F, var1);
}

void WorldClient::obtainEntitySkin(Entity* var1) {
		World::obtainEntitySkin(var1);
		if(_wcContains(m_field_1053_F, var1)) {
			_wcRemove(m_field_1053_F, var1);
		}

}

void WorldClient::releaseEntitySkin(Entity* var1) {
		World::releaseEntitySkin(var1);
		if(_wcContains(m_F, var1)) {
			m_field_1053_F->push_back(var1);
		}

}

void WorldClient::func_712_a(int var1, Entity* var2) {
		Entity* var3 = func_709_b(var1);
		if(var3 != NULL) {
			setEntityDead(var3);
		}

		m_F->push_back(var2);
		var2->m_field_620_ab = var1;
		if(!entityJoinedWorld(var2)) {
			m_field_1053_F->push_back(var2);
		}

		m_field_1055_D->addKey(var1, var2);
}

Entity* WorldClient::func_709_b(int var1) {
		return (Entity*)m_field_1055_D->lookup(var1);
}

Entity* WorldClient::func_710_c(int var1) {
		Entity* var2 = (Entity*)m_field_1055_D->removeObject(var1);
		if(var2 != NULL) {
			_wcRemove(m_F, var2);
			setEntityDead(var2);
		}

		return var2;
}

bool WorldClient::setBlockMetadata(int var1, int var2, int var3, int var4) {
		int var5 = getBlockId(var1, var2, var3);
		int var6 = getBlockMetadata(var1, var2, var3);
		if(World::setBlockMetadata(var1, var2, var3, var4)) {
			m_field_1057_z.push_back(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
			return true;
		} else {
			return false;
		}
}

bool WorldClient::setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5) {
		int var6 = getBlockId(var1, var2, var3);
		int var7 = getBlockMetadata(var1, var2, var3);
		if(World::setBlockAndMetadata(var1, var2, var3, var4, var5)) {
			m_field_1057_z.push_back(new WorldBlockPositionType(this, var1, var2, var3, var6, var7));
			return true;
		} else {
			return false;
		}
}

bool WorldClient::setBlock(int var1, int var2, int var3, int var4) {
		int var5 = getBlockId(var1, var2, var3);
		int var6 = getBlockMetadata(var1, var2, var3);
		if(World::setBlock(var1, var2, var3, var4)) {
			m_field_1057_z.push_back(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
			return true;
		} else {
			return false;
		}
}

bool WorldClient::func_714_c(int var1, int var2, int var3, int var4, int var5) {
		func_711_c(var1, var2, var3, var1, var2, var3);
		if(World::setBlockAndMetadata(var1, var2, var3, var4, var5)) {
			notifyBlockChange(var1, var2, var3, var4);
			return true;
		} else {
			return false;
		}
}

void WorldClient::func_698_b(int var1, int var2, int var3, TileEntity* var4) {
		if(!m_D) {
			// PORT: 360 MP API TODO -- Packet59ComplexEntity (no-op).
		}
}

void WorldClient::sendQuittingDisconnectingPacket() {
		// PORT: 360 MP API TODO -- Packet255KickDisconnect("Quitting") (no-op).
}
