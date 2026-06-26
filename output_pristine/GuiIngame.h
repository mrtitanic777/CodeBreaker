#ifndef GUIINGAME_H
#define GUIINGAME_H

// Auto-generated from GuiIngame.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include <vector>

class Minecraft;
class Random;
class RenderItem;

class GuiIngame : public Gui {
public:
	GuiIngame();
	GuiIngame(Minecraft* var1);
	virtual ~GuiIngame();

	virtual void renderGameOverlay(float var1);
	virtual void addChatMessage();

private:
	static RenderItem* s_itemRenderer;
	std::vector<void*> /* PORT: determine element type */ m_chatMessageList;
	Random* rand;
	Minecraft* mc;
	int m_updateCounter;

};

#endif // GUIINGAME_H