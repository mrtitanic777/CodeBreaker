#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetworkReaderThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetworkReaderThread.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "NetworkManager.h"
#include "Socket.h"
#include "Thread.h"

NetworkReaderThread::NetworkReaderThread(NetworkManager* var1, const JavaString& var2)
	: Thread(var2)
	, netManager(NULL)
{
	this->netManager = var1;
}

NetworkReaderThread::~NetworkReaderThread()
{
}

void NetworkReaderThread::run()
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var1) held the threadSyncObject monitor; below it is a plain scope
	 *  // (monitor behaviour dropped). Same applies to every synchronized block in run().
	 *  Object var1 = NetworkManager.threadSyncObject;
	 *  synchronized(var1) {
	 *     ++NetworkManager.numReadThreads;
	 *  }
	 *  ---- end original Java ---- */
	void* var1 = NetworkManager::s_threadSyncObject;
	{
	++NetworkManager::s_numReadThreads;
	}
	// PORT: Java try/finally -> the finally body runs on EVERY exit of the try. Exceptions are
	// off on the console (the try never throws), so the finally is inlined before each
	// continue/break and at fall-through, preserving the s_numReadThreads bookkeeping exactly.
	while(true) {
	bool var11 = false;
	{
	var11 = true;
	if(NetworkManager::isRunning_(this->netManager)) {
	if(!NetworkManager::isServerTerminating_(this->netManager)) {
	NetworkManager::readNetworkPacket(this->netManager);
	if(var11) { --NetworkManager::s_numReadThreads; }   // finally (continue path)
	continue;
	}
	var11 = false;
	if(var11) { --NetworkManager::s_numReadThreads; }   // finally (break path)
	break;
	}
	var11 = false;
	if(var11) { --NetworkManager::s_numReadThreads; }   // finally (break path)
	break;
	}
	}
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var1) held the threadSyncObject monitor; below it is a plain scope
	 *  // (monitor behaviour dropped). The finally body above also synchronized on
	 *  // threadSyncObject before --numReadThreads.
	 *  var1 = NetworkManager.threadSyncObject;
	 *  synchronized(var1) {
	 *     --NetworkManager.numReadThreads;
	 *  }
	 *  ---- end original Java ---- */
	var1 = NetworkManager::s_threadSyncObject;
	{
	--NetworkManager::s_numReadThreads;
	}
}
