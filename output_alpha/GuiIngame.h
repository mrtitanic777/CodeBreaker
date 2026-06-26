#ifndef GUIINGAME_H
#define GUIINGAME_H

// Auto-generated from GuiIngame.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include <string>
#include <vector>

class ChatLine;
class Minecraft;
class Random;
class RenderItem;

class GuiIngame : public Gui {
public:
	GuiIngame();
	GuiIngame(Minecraft* var1);
	virtual ~GuiIngame();

	std::string m_field_933_a;
	float m_field_6446_b;

	virtual void renderGameOverlay(float var1, bool var2, int var3, int var4);
	virtual void func_555_a();
	virtual void addChatMessage(const std::string& var1);
	virtual void func_553_b(const std::string& var1);

public: // was private
	static RenderItem* s_itemRenderer;
	std::vector<ChatLine*> m_chatMessageList;
	Random* rand;
	Minecraft* mc;
	int m_updateCounter;
	std::string m_field_9420_i;
	int m_field_9419_j;
	float m_field_931_c;

	void func_4063_a(int var1, int var2);
	void func_4064_a(float var1, int var2, int var3);
	void func_4065_b(float var1, int var2, int var3);
	void func_554_a(int var1, int var2, int var3, float var4);

};

#endif // GUIINGAME_H