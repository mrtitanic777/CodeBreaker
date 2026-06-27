#ifndef GUIINGAME_H
#define GUIINGAME_H

// Auto-generated from GuiIngame.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Gui.h"

class Minecraft;
class Random;
class RenderItem;

class GuiIngame : public Gui {
public:
	GuiIngame(Minecraft* var1);
	virtual ~GuiIngame();

	JList<void*> chatMessageList;
	Random* rand;
	Minecraft* mc;
	JavaString clickedChatText;
	int updateCounter;
	JavaString recordPlayingName;
	int recordPlayingUpFor;
	float damagePartialTime;
	float prevVignetteBrightness;
	static RenderItem* s_itemRenderer;

	virtual void renderGameOverlay(float var1, bool var2, int var3, int var4);
	virtual void renderPumpkinBlur(int var1, int var2);
	virtual void renderVignette(float var1, int var2, int var3);
	virtual void renderPortalOverlay(float var1, int var2, int var3);
	virtual void renderInventorySlot(int var1, int var2, int var3, float var4);
	virtual void updateTick();
	virtual void addChatMessage(const JavaString& var1);
	virtual void setRecordPlaying(const JavaString& var1);
};

#endif // GUIINGAME_H