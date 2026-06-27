#ifndef THREADRUNISOCLIENT_H
#define THREADRUNISOCLIENT_H

// Auto-generated from ThreadRunIsoClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class CanvasIsomPreview;

class ThreadRunIsoClient : public Thread {
public:
	ThreadRunIsoClient(CanvasIsomPreview* var1);
	virtual ~ThreadRunIsoClient();

	CanvasIsomPreview* canvas;

	virtual void run();

	static void initStatics();
};

#endif // THREADRUNISOCLIENT_H