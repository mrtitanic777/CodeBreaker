#ifndef GUISELECTWORLD_H
#define GUISELECTWORLD_H

// Auto-generated from GuiSelectWorld.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiSelectWorld : public GuiScreen {
public:
	GuiSelectWorld();
	GuiSelectWorld(GuiScreen* var1);
	virtual ~GuiSelectWorld();

	virtual void initGui();
	virtual void initGui2();
	virtual void selectWorld(int var1);
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	GuiScreen* m_parentScreen;
	std::string m_screenTitle;

	virtual const std::string& getWorldName(int var1);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	bool m_selected;
	std::string m_worldNameBuf; // PORT: backing storage for getWorldName ref return

};

#endif // GUISELECTWORLD_H
