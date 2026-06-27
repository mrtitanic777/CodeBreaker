#ifndef MINECARTTRACKLOGIC_H
#define MINECARTTRACKLOGIC_H

// Auto-generated from MinecartTrackLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class BlockMinecartTrack;
class ChunkPosition;
class World;

class MinecartTrackLogic {
public:
	MinecartTrackLogic(BlockMinecartTrack* var1, World* var2, int var3, int var4, int var5);
	virtual ~MinecartTrackLogic();

	World* worldObj;
	int trackX;
	int trackY;
	int trackZ;
	int trackMetadata;
	JList<void*> connectingTracks;
	BlockMinecartTrack* theTrack;

	virtual void setConnections();
	virtual void refreshConnectedTracks();
	virtual bool isMinecartTrack(int var1, int var2, int var3);
	virtual MinecartTrackLogic* getRailLogic(ChunkPosition* var1);
	virtual bool isConnectedToTrack(MinecartTrackLogic* var1);
	virtual bool hasConnection(int var1, int var2, int var3);
	virtual int getAdjacentTracks();
	virtual bool canConnectTo(MinecartTrackLogic* var1);
	virtual void connectTo(MinecartTrackLogic* var1);
	virtual bool canConnectFrom(int var1, int var2, int var3);
	virtual void refreshTrackShape(bool var1);
	static int getNumberOfAdjacentTracks(MinecartTrackLogic* var0);

	static void initStatics();
};

#endif // MINECARTTRACKLOGIC_H