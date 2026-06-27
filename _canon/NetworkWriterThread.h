#ifndef NETWORKWRITERTHREAD_H
#define NETWORKWRITERTHREAD_H

// Auto-generated from NetworkWriterThread.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class NetworkManager;

class NetworkWriterThread : public Thread {
public:
	NetworkWriterThread(NetworkManager* var1, const JavaString& var2);
	virtual ~NetworkWriterThread();

	NetworkManager* netManager;

	virtual void run();
};

#endif // NETWORKWRITERTHREAD_H