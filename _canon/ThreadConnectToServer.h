#ifndef THREADCONNECTTOSERVER_H
#define THREADCONNECTTOSERVER_H

// Auto-generated from ThreadConnectToServer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class GuiConnecting;
class Minecraft;

class ThreadConnectToServer : public Thread {
public:
	ThreadConnectToServer(GuiConnecting* var1, Minecraft* var2, const JavaString& var3, int var4);
	virtual ~ThreadConnectToServer();

	Minecraft* mc;
	JavaString hostName;
	int port;
	GuiConnecting* connectingGui;

	virtual void run();

	static void initStatics();
};

#endif // THREADCONNECTTOSERVER_H