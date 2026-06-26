#include "stdafx.h"
// Auto-generated from GuiDeleteWorld.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiDeleteWorld.h"
#include "Common.h"
#include "GuiScreen.h"
#include <string>
#include "Minecraft.h"
#include "World.h"
#include "GuiButton.h"
#include "GuiYesNo.h"

GuiDeleteWorld::GuiDeleteWorld()
{
}

GuiDeleteWorld::GuiDeleteWorld(GuiScreen* var1)
	: GuiSelectWorld(var1)
{
		m_screenTitle = "Delete world";
}

GuiDeleteWorld::~GuiDeleteWorld() {
	// No dynamic resources
}

void GuiDeleteWorld::initGui2() {
		controlList.push_back(new GuiButton(6, width / 2 - 100, height / 6 + 168, "Cancel"));
}

void GuiDeleteWorld::selectWorld(int var1) {
		std::string var2 = getWorldName(var1);
		if(!var2.empty()) {
			mc->displayGuiScreen(new GuiYesNo(this, "Are you sure you want to delete this world?", "\'" + var2 + "\' will be lost forever!", var1));
		}

}

void GuiDeleteWorld::deleteWorld(bool var1, int var2) {
		if(var1) {
			std::string* var3 = Minecraft::getMinecraftDir();
			World::deleteWorld(var3, getWorldName(var2));
		}

		mc->displayGuiScreen(m_parentScreen);
}
