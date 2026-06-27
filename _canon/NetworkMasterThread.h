#ifndef NETWORKMASTERTHREAD_H
#define NETWORKMASTERTHREAD_H

// Auto-generated from NetworkMasterThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class NetworkManager;

class NetworkMasterThread : public Thread {
public:
	NetworkMasterThread(NetworkManager* var1);
	virtual ~NetworkMasterThread();

	NetworkManager* netManager;

	virtual void run();

	static void initStatics();
};

#endif // NETWORKMASTERTHREAD_H