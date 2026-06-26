#ifndef GUIDOWNLOADTERRAIN_H
#define GUIDOWNLOADTERRAIN_H

// Auto-generated from GuiDownloadTerrain.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"

class GuiButton;
class NetClientHandler;

class GuiDownloadTerrain : public GuiScreen {
public:
	GuiDownloadTerrain();
	GuiDownloadTerrain(NetClientHandler* var1);
	virtual ~GuiDownloadTerrain();

	virtual void initGui();
	virtual void updateScreen();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	NetClientHandler* m_netHandler;
	int m_updateCounter;

};

#endif // GUIDOWNLOADTERRAIN_H