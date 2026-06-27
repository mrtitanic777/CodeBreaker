#ifndef GAMEWINDOWLISTENER_H
#define GAMEWINDOWLISTENER_H

// Auto-generated from GameWindowListener.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WindowAdapter.h"

class Minecraft;
class Thread;
class WindowEvent;

class GameWindowListener : public WindowAdapter {
public:
	GameWindowListener(Minecraft* var1, Thread* var2);
	virtual ~GameWindowListener();

	Minecraft* mc;
	Thread* mcThread;

	virtual void windowClosing(WindowEvent* var1);

	static void initStatics();
};

#endif // GAMEWINDOWLISTENER_H