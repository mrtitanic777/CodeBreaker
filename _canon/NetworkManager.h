#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

// Auto-generated from NetworkManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class DataInputStream;
class DataOutputStream;
class Exception;
class NetHandler;
class Packet;
class Socket;
class SocketAddress;
class Thread;

class NetworkManager {
public:
	NetworkManager(Socket* var1, const JavaString& var2, NetHandler* var3);
	virtual ~NetworkManager();

	void* sendQueueLock;
	Socket* field_12258_e;
	SocketAddress* networkSocket;
	DataInputStream* socketInputStream;
	DataOutputStream* socketOutputStream;
	bool isRunning;
	JList<void*> readPackets;
	JList<void*> dataPackets;
	JList<void*> chunkDataPackets;
	NetHandler* netHandler;
	bool isServerTerminating;
	Thread* writeThread;
	Thread* readThread;
	bool isTerminating;
	JavaString terminationReason;
	int timeSinceLastRead;
	int sendQueueByteLength;
	int chunkDataSendCounter;
	static void* s_threadSyncObject;
	static int s_numReadThreads;
	static int s_numWriteThreads;

	virtual void addToSendQueue(Packet* var1);
	virtual void sendPacket();
	virtual void readPacket();
	virtual void onNetworkError(Exception* var1);
	virtual void networkShutdown(const JavaString& var1);
	virtual void processReadPackets();
	static bool isRunning_(NetworkManager* var0);
	static bool isServerTerminating_(NetworkManager* var0);
	static void readNetworkPacket(NetworkManager* var0);
	static void sendNetworkPacket(NetworkManager* var0);
	static Thread* getReadThread(NetworkManager* var0);
	static Thread* getWriteThread(NetworkManager* var0);

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	NetworkManager(const NetworkManager&);
	NetworkManager& operator=(const NetworkManager&);
};

#endif // NETWORKMANAGER_H