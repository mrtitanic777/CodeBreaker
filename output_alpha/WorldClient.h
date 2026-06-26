#ifndef WORLDCLIENT_H
#define WORLDCLIENT_H

// Auto-generated from WorldClient.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. WorldClient is the client-side world used only in
// multiplayer; SP uses WorldServer/World directly. Kept compiling as a stub.
// The two java.util.Set fields (m_F, m_field_1053_F) become std::vector<void*>*
// because the engine's Set.h is an empty stub -- they are treated as plain
// entity lists here, which is sufficient for the (dead) MP path to compile.

#include "World.h"
#include <vector>

class ChunkProviderClient;
class Entity;
class IChunkProvider;
class MCHashTable;
class NetClientHandler;
class TileEntity;

class WorldClient : public World {
public:
	WorldClient();
	WorldClient(NetClientHandler* var1, long long var2, int var4);
	virtual ~WorldClient();

	virtual void tick();
	virtual void func_711_c(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void func_4076_b();
	virtual void scheduleBlockUpdate(int var1, int var2, int var3, int var4);
	virtual bool TickUpdates(bool var1);
	virtual void func_713_a(int var1, int var2, bool var3);
	virtual bool entityJoinedWorld(Entity* var1);
	virtual void setEntityDead(Entity* var1);
	virtual void func_712_a(int var1, Entity* var2);
	virtual Entity* func_709_b(int var1);
	virtual Entity* func_710_c(int var1);
	virtual bool setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual bool setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5);
	virtual bool setBlock(int var1, int var2, int var3, int var4);
	virtual bool func_714_c(int var1, int var2, int var3, int var4, int var5);
	virtual void func_698_b(int var1, int var2, int var3, TileEntity* var4);
	virtual void sendQuittingDisconnectingPacket();

public: // was protected
	virtual IChunkProvider* func_4081_a(std::string /* PORT: was java.io.File */* var1);
	virtual void func_4080_j();
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);

public: // was private
	std::vector<void*> /* PORT: WorldBlockPositionType* */ m_field_1057_z;
	NetClientHandler* m_sendQueue;
	ChunkProviderClient* m_C;
	bool m_D;
	MCHashTable* m_field_1055_D;
	std::vector<void*>* m_F;            // PORT: was java.util.Set<Entity*>
	std::vector<void*>* m_field_1053_F; // PORT: was java.util.Set<Entity*>

};

#endif // WORLDCLIENT_H
