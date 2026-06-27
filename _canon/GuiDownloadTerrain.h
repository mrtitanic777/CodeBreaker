#ifndef GUIDOWNLOADTERRAIN_H
#define GUIDOWNLOADTERRAIN_H

// Auto-generated from GuiDownloadTerrain.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;
class NetClientHandler;

class GuiDownloadTerrain : public GuiScreen {
public:
	GuiDownloadTerrain(NetClientHandler* var1);
	virtual ~GuiDownloadTerrain();

	NetClientHandler* netHandler;
	int updateCounter;

	virtual void keyTyped(char var1, int var2);
	virtual void initGui();
	virtual void updateScreen();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIDOWNLOADTERRAIN_H