#include "stdafx.h"
// Auto-generated from GuiSelectWorld.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiSelectWorld.h"
#include "Common.h"
#include "GuiButton.h"
#include <string>
#include "GuiScreen.h"
#include "Minecraft.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "GuiDeleteWorld.h"
#include "PlayerControllerSP.h"

GuiSelectWorld::GuiSelectWorld()
	: m_parentScreen(NULL)
	, m_screenTitle("Select world")
	, m_selected(false)
{
}

GuiSelectWorld::GuiSelectWorld(GuiScreen* var1)
	: m_parentScreen(NULL)
	, m_screenTitle("Select world")
	, m_selected(false)
{
		m_parentScreen = var1;
}

GuiSelectWorld::~GuiSelectWorld() {
	m_parentScreen = NULL;
}

void GuiSelectWorld::initGui() {
		std::string* var1 = Minecraft::getMinecraftDir();

		for(int var2 = 0; var2 < 5; ++var2) {
			NBTTagCompound* var3 = World::func_629_a(var1, "World" + toStr(var2 + 1));
			if(var3 == NULL) {
				controlList.push_back(new GuiButton(var2, width / 2 - 100, height / 6 + 24 * var2, "- empty -"));
			} else {
				std::string var4 = "World " + toStr(var2 + 1);
				long var5 = (long)var3->getLong("SizeOnDisk");
				var4 = var4 + " (" + toStr((float)(var5 / 1024L * 100L / 1024L) / 100.0F) + " MB)";
				controlList.push_back(new GuiButton(var2, width / 2 - 100, height / 6 + 24 * var2, var4));
			}
		}

		initGui2();
}

const std::string& GuiSelectWorld::getWorldName(int var1) {
		std::string* var2 = Minecraft::getMinecraftDir();
		std::string name = "World" + toStr(var1);
		m_worldNameBuf = (World::func_629_a(var2, name) != NULL) ? name : std::string("");
		return m_worldNameBuf;
}

void GuiSelectWorld::initGui2() {
		controlList.push_back(new GuiButton(5, width / 2 - 100, height / 6 + 120 + 12, "Delete world..."));
		controlList.push_back(new GuiButton(6, width / 2 - 100, height / 6 + 168, "Cancel"));
}

void GuiSelectWorld::actionPerformed(GuiButton* var1) {
		if(var1->m_enabled) {
			if(var1->m_id < 5) {
				selectWorld(var1->m_id + 1);
			} else if(var1->m_id == 5) {
				mc->displayGuiScreen(new GuiDeleteWorld(this));
			} else if(var1->m_id == 6) {
				mc->displayGuiScreen(m_parentScreen);
			}

		}
}

void GuiSelectWorld::selectWorld(int var1) {
		mc->displayGuiScreen(NULL);
		if(!m_selected) {
			m_selected = true;
			mc->m_field_6327_b = new PlayerControllerSP(mc);
			mc->func_6247_b("World" + toStr(var1));
			mc->displayGuiScreen(NULL);
		}
}

void GuiSelectWorld::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_screenTitle, width / 2, 20, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
