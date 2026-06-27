#ifndef THREADSLEEPFOREVER_H
#define THREADSLEEPFOREVER_H

// Auto-generated from ThreadSleepForever.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class Minecraft;

class ThreadSleepForever : public Thread {
public:
	ThreadSleepForever(Minecraft* var1, const JavaString& var2);
	virtual ~ThreadSleepForever();

	Minecraft* mc;

	virtual void run();

	static void initStatics();
};

#endif // THREADSLEEPFOREVER_H