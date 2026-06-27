#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetworkManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetworkManager.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Exception.h"
#include "InterruptedException.h"
#include "NetHandler.h"
#include "NetworkMasterThread.h"
#include "NetworkReaderThread.h"
#include "NetworkWriterThread.h"
#include "Packet.h"
#include "Socket.h"
#include "Thread.h"
#include "Throwable.h"

void* NetworkManager::s_threadSyncObject = ((void*)new char);
int NetworkManager::s_numReadThreads = 0;
int NetworkManager::s_numWriteThreads = 0;

NetworkManager::NetworkManager(Socket* var1, const JavaString& var2, NetHandler* var3)
	: sendQueueLock(NULL)
	, field_12258_e(NULL)
	, networkSocket(NULL)
	, socketInputStream(NULL)
	, socketOutputStream(NULL)
	, isRunning(0)
	, netHandler(NULL)
	, isServerTerminating(0)
	, writeThread(NULL)
	, readThread(NULL)
	, isTerminating(0)
	, timeSinceLastRead(0)
	, sendQueueByteLength(0)
	, chunkDataSendCounter(0)
{
	this->field_12258_e = var1;
	this->networkSocket = (SocketAddress*)var1->getRemoteSocketAddress();
	this->netHandler = var3;
	var1->setTrafficClass(24);
	this->socketInputStream = new DataInputStream(var1->getInputStream());
	this->socketOutputStream = new DataOutputStream(var1->getOutputStream());
	this->readThread = new NetworkReaderThread(this,(JCat() + var2 + " read thread"));
	this->writeThread = new NetworkWriterThread(this,(JCat() + var2 + " write thread"));
	this->readThread->start();
	this->writeThread->start();
}

NetworkManager::~NetworkManager()
{
	delete readThread;
	readThread = NULL;
	delete socketInputStream;
	socketInputStream = NULL;
	delete socketOutputStream;
	socketOutputStream = NULL;
	delete writeThread;
	writeThread = NULL;
}

void NetworkManager::addToSendQueue(Packet* var1)
{
	if(!this->isServerTerminating) {
	void* var2 = this->sendQueueLock;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var2) {...} held the sendQueueLock monitor; below it is a plain
	 *  // scope with no locking (monitor behaviour dropped). The lists were also
	 *  // Collections.synchronizedList(new ArrayList()); the C++ uses a bare container.
	 *  Object var2 = this.sendQueueLock;
	 *  synchronized(var2) {
	 *     this.sendQueueByteLength += var1.getPacketSize() + 1;
	 *     if(var1.isChunkDataPacket) {
	 *        this.chunkDataPackets.add(var1);
	 *     } else {
	 *        this.dataPackets.add(var1);
	 *     }
	 *  }
	 *  ---- end original Java ---- */
	{
	this->sendQueueByteLength += var1->getPacketSize() + 1;
	if(var1->isChunkDataPacket) {
	this->chunkDataPackets.push_back(var1);
	} else {
	this->dataPackets.push_back(var1);
	}
	}
	}
}

void NetworkManager::sendPacket()
{
	try {
	bool var1 = true;
	Packet* var2;
	void* var3;
	if(!this->dataPackets.empty()) {
	var1 = false;
	var3 = this->sendQueueLock;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var3) held the sendQueueLock monitor; below is a plain scope
	 *  // (monitor behaviour dropped).
	 *  var3 = this.sendQueueLock;
	 *  synchronized(var3) {
	 *     var2 = (Packet)this.dataPackets.remove(0);
	 *     this.sendQueueByteLength -= var2.getPacketSize() + 1;
	 *  }
	 *  ---- end original Java ---- */
	{
	var2 = (Packet*)this->dataPackets.remove(0);
	this->sendQueueByteLength -= var2->getPacketSize() + 1;
	}
	Packet::writePacket(var2, this->socketOutputStream);
	}
	if((var1 || this->chunkDataSendCounter-- <= 0) && !this->chunkDataPackets.empty()) {
	var1 = false;
	var3 = this->sendQueueLock;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var3) held the sendQueueLock monitor; below is a plain scope
	 *  // (monitor behaviour dropped).
	 *  var3 = this.sendQueueLock;
	 *  synchronized(var3) {
	 *     var2 = (Packet)this.chunkDataPackets.remove(0);
	 *     this.sendQueueByteLength -= var2.getPacketSize() + 1;
	 *  }
	 *  ---- end original Java ---- */
	{
	var2 = (Packet*)this->chunkDataPackets.remove(0);
	this->sendQueueByteLength -= var2->getPacketSize() + 1;
	}
	Packet::writePacket(var2, this->socketOutputStream);
	this->chunkDataSendCounter = 50;
	}
	if(var1) {
	Thread::sleep(10LL);
	}
	} catch (InterruptedException* var8) {
	} catch (Exception* var9) {
	if(!this->isTerminating) {
	this->onNetworkError(var9);
	}
	}
}

void NetworkManager::readPacket()
{
	try {
	Packet* var1 = Packet::readPacket(this->socketInputStream);
	if(var1 != NULL) {
	this->readPackets.push_back(var1);
	} else {
	this->networkShutdown("End of stream");
	}
	} catch (Exception* var2) {
	if(!this->isTerminating) {
	this->onNetworkError(var2);
	}
	}
}

void NetworkManager::onNetworkError(Exception* var1)
{
	var1->printStackTrace();
	this->networkShutdown((JCat() + "Internal exception: " + var1->toString()));
}

void NetworkManager::networkShutdown(const JavaString& var1)
{
	if(this->isRunning) {
	this->isTerminating = true;
	this->terminationReason = var1;
	(new NetworkMasterThread(this))->start();
	this->isRunning = false;
	try {
	this->socketInputStream->close();
	this->socketInputStream = NULL;
	} catch (Throwable* var5) {
	}
	try {
	this->socketOutputStream->close();
	this->socketOutputStream = NULL;
	} catch (Throwable* var4) {
	}
	try {
	this->field_12258_e->close();
	this->field_12258_e = NULL;
	} catch (Throwable* var3) {
	}
	}
}

void NetworkManager::processReadPackets()
{
	if(this->sendQueueByteLength > 1048576) {
	this->networkShutdown("Send buffer overflow");
	}
	if(this->readPackets.empty()) {
	if(this->timeSinceLastRead++ == 1200) {
	this->networkShutdown("Timed out");
	}
	} else {
	this->timeSinceLastRead = 0;
	}
	int var1 = 100;
	while(!this->readPackets.empty() && var1-- >= 0) {
	Packet* var2 = (Packet*)this->readPackets.remove(0);
	var2->processPacket(this->netHandler);
	}
	if(this->isTerminating && this->readPackets.empty()) {
	this->netHandler->handleErrorMessage(this->terminationReason);
	}
}

bool NetworkManager::isRunning_(NetworkManager* var0)
{
	return var0->isRunning;
}

bool NetworkManager::isServerTerminating_(NetworkManager* var0)
{
	return var0->isServerTerminating;
}

void NetworkManager::readNetworkPacket(NetworkManager* var0)
{
	var0->readPacket();
}

void NetworkManager::sendNetworkPacket(NetworkManager* var0)
{
	var0->sendPacket();
}

Thread* NetworkManager::getReadThread(NetworkManager* var0)
{
	return var0->readThread;
}

Thread* NetworkManager::getWriteThread(NetworkManager* var0)
{
	return var0->writeThread;
}
