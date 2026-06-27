#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetworkWriterThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetworkWriterThread.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "NetworkManager.h"
#include "Socket.h"
#include "Thread.h"

NetworkWriterThread::NetworkWriterThread(NetworkManager* var1, const JavaString& var2)
	: Thread(var2)
	, netManager(NULL)
{
	this->netManager = var1;
}

NetworkWriterThread::~NetworkWriterThread()
{
}

void NetworkWriterThread::run()
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var1) held the threadSyncObject monitor; below it is a plain scope
	 *  // (monitor behaviour dropped). Same applies to every synchronized block in run(),
	 *  // including the one inside the finally (the RAII _Finally guard below).
	 *  Object var1 = NetworkManager.threadSyncObject;
	 *  synchronized(var1) {
	 *     ++NetworkManager.numWriteThreads;
	 *  }
	 *  ---- end original Java ---- */
	void* var1 = NetworkManager::s_threadSyncObject;
	{
	++NetworkManager::s_numWriteThreads;
	}
	while(true) {
	bool var11 = false;
	// PORT: Java try/finally -> RAII guard so the finally body runs on every exit
	// path of the protected block (normal fall-through AND the `break`).
	struct _Finally {
		bool& v11;
		_Finally(bool& v) : v11(v) {}
		/* ---- ORIGINAL JAVA (stubbed below) ----
		 *  // This RAII destructor stands in for the Java finally block, whose
		 *  // synchronized(var5) on threadSyncObject is dropped to a plain scope.
		 *  } finally {
		 *     if(var11) {
		 *        Object var5 = NetworkManager.threadSyncObject;
		 *        synchronized(var5) {
		 *           --NetworkManager.numWriteThreads;
		 *        }
		 *     }
		 *  }
		 *  ---- end original Java ---- */
		~_Finally() {
			if(v11) {
			void* var5 = NetworkManager::s_threadSyncObject;
			(void)var5;
			{
			--NetworkManager::s_numWriteThreads;
			}
			}
		}
	} _finallyGuard(var11);   // NOT '_finally': MSVC treats _finally as the __finally SEH keyword
	{
	var11 = true;
	if(!NetworkManager::isRunning_(this->netManager)) {
	var11 = false;
	break;
	}
	NetworkManager::sendNetworkPacket(this->netManager);
	}
	}
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // synchronized(var1) held the threadSyncObject monitor; below it is a plain scope
	 *  // (monitor behaviour dropped).
	 *  var1 = NetworkManager.threadSyncObject;
	 *  synchronized(var1) {
	 *     --NetworkManager.numWriteThreads;
	 *  }
	 *  ---- end original Java ---- */
	var1 = NetworkManager::s_threadSyncObject;
	{
	--NetworkManager::s_numWriteThreads;
	}
}
