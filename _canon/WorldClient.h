#ifndef WORLDCLIENT_H
#define WORLDCLIENT_H

// Auto-generated from WorldClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <set>
#include "World.h"

class ChunkProviderClient;
class Entity;
class File;
class IChunkProvider;
class MCHashTable;
class NetClientHandler;
class TileEntity;

class WorldClient : public World {
public:
	WorldClient(NetClientHandler* var1, long long var2, int var4);
	virtual ~WorldClient();

	JList<void*> blockChangeList;
	NetClientHandler* sendQueue;
	ChunkProviderClient* C;
	bool D;
	MCHashTable* entitiesById;
	std::set<void*> F;
	std::set<void*> entitySpawnQueue;

	virtual void tick();
	virtual void invalidateBlockReceiveRegion(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual IChunkProvider* createChunkProvider(File* var1);
	virtual void findSpawnGround();
	virtual void updateBlocksAroundPlayers();
	virtual void scheduleBlockUpdate(int var1, int var2, int var3, int var4);
	virtual bool TickUpdates(bool var1);
	virtual void getServerSocket(int var1, int var2, bool var3);
	virtual bool entityJoinedWorld(Entity* var1);
	virtual void setEntityDead(Entity* var1);
	virtual void obtainEntitySkin(Entity* var1);
	virtual void releaseEntitySkin(Entity* var1);
	virtual void addEntityToWorld(int var1, Entity* var2);
	virtual Entity* getEntityByID(int var1);
	virtual Entity* removeEntityFromWorld(int var1);
	virtual bool setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual bool setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5);
	virtual bool setBlock(int var1, int var2, int var3, int var4);
	virtual bool setBlockAndMetadataNoQueue(int var1, int var2, int var3, int var4, int var5);
	virtual void markTileEntityForUpdate(int var1, int var2, int var3, TileEntity* var4);
	virtual void sendQuittingDisconnectingPacket();
};

#endif // WORLDCLIENT_H