#ifndef NETWORKREADERTHREAD_H
#define NETWORKREADERTHREAD_H

// Auto-generated from NetworkReaderThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class NetworkManager;

class NetworkReaderThread : public Thread {
public:
	NetworkReaderThread(NetworkManager* var1, const JavaString& var2);
	virtual ~NetworkReaderThread();

	NetworkManager* netManager;

	virtual void run();
};

#endif // NETWORKREADERTHREAD_H