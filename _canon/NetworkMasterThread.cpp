#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetworkMasterThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetworkMasterThread.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "InterruptedException.h"
#include "NetHandler.h"
#include "NetworkManager.h"
#include "Socket.h"
#include "Thread.h"
#include "Throwable.h"

NetworkMasterThread::NetworkMasterThread(NetworkManager* var1)
	: netManager(NULL)
{
	this->netManager = var1;
}

NetworkMasterThread::~NetworkMasterThread()
{
}

void NetworkMasterThread::run()
{
	try {
	Thread::sleep(5000LL);
	if(NetworkManager::getReadThread(this->netManager)->isAlive()) {
	try {
	NetworkManager::getReadThread(this->netManager)->stop();
	} catch (Throwable*) {
	}
	}
	if(NetworkManager::getWriteThread(this->netManager)->isAlive()) {
	try {
	NetworkManager::getWriteThread(this->netManager)->stop();
	} catch (Throwable*) {
	}
	}
	} catch (InterruptedException* var4) {
	var4->printStackTrace();
	}
}

void NetworkMasterThread::initStatics()
{
}
