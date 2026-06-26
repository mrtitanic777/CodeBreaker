#include "stdafx.h"
// Hand-ported from GuiScreen.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Base class for every menu screen. Must match output_alpha/GuiScreen.h exactly.
// Java AWT clipboard, LWJGL Mouse/Keyboard event polling are not available on the
// 360; those paths are stubbed with // PORT: notes. Real input is driven through
// the platform XInput poll (see Minecraft input glue), which calls
// mouseClicked / mouseMovedOrUp / keyTyped directly.

#include "GuiScreen.h"
#include "Common.h"
#include "FontRenderer.h"
#include "GuiButton.h"
#include "Minecraft.h"
#include <string>
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "RenderEngine.h"
#include "SoundManager.h"

GuiScreen::GuiScreen()
	: width(0)
	, height(0)
	, m_field_948_f(false)
	, mc(NULL)
	, fontRenderer(NULL)
	, m_selectedButton(NULL)
{
}

GuiScreen::~GuiScreen() {
	mc = NULL;
	fontRenderer = NULL;
	m_selectedButton = NULL;
}

void GuiScreen::drawScreen(int var1, int var2, float var3) {
	for(size_t var4 = 0; var4 < controlList.size(); ++var4) {
		GuiButton* var5 = (GuiButton*)controlList[var4];
		var5->drawButton(mc, var1, var2);
	}
}

void GuiScreen::keyTyped(char var1, int var2) {
	// var2 == 1 is the Escape key code (LWJGL KEY_ESCAPE); close the screen.
	if(var2 == 1) {
		mc->displayGuiScreen(NULL);
		mc->func_6259_e();   // setIngameFocus()
	}
}

const std::string& GuiScreen::getClipboardString() {
	// PORT: Java AWT system clipboard (Toolkit / Transferable / DataFlavor) does not
	// exist on the Xbox 360. There is no system clipboard to read from, so this
	// always returns an empty string. Returning a function-static keeps the
	// const-reference return contract valid (no dangling local / no NULL deref).
	static const std::string s_empty;
	return s_empty;
}

void GuiScreen::mouseClicked(int var1, int var2, int var3) {
	if(var3 == 0) {
		for(size_t var4 = 0; var4 < controlList.size(); ++var4) {
			GuiButton* var5 = (GuiButton*)controlList[var4];
			if(var5->mousePressed(mc, var1, var2)) {
				m_selectedButton = var5;
				mc->m_sndManager->func_337_a("random.click", 1.0F, 1.0F);   // playSoundFX
				actionPerformed(var5);
			}
		}
	}
}

void GuiScreen::mouseMovedOrUp(int var1, int var2, int var3) {
	if(m_selectedButton != NULL && var3 == 0) {
		m_selectedButton->mouseReleased(var1, var2);
		m_selectedButton = NULL;
	}
}

void GuiScreen::actionPerformed(GuiButton* var1) {
	// Base class: nothing. Subclasses override to handle their buttons.
}

void GuiScreen::setWorldAndResolution(Minecraft* var1, int var2, int var3) {
	mc = var1;
	fontRenderer = var1->fontRenderer;
	width = var2;
	height = var3;
	controlList.clear();
	initGui();
}

void GuiScreen::initGui() {
	// Base class: nothing. Subclasses add their buttons here.
}

void GuiScreen::handleInput() {
	// PORT: Java polled LWJGL Mouse.next()/Keyboard.next() event queues here and
	// dispatched each event to handleMouseInput()/handleKeyboardInput(). On the 360
	// input is delivered by the platform XInput poll, which drives the screen by
	// calling mouseClicked / mouseMovedOrUp / keyTyped directly, so there is no
	// per-frame event queue to drain here.
}

void GuiScreen::handleMouseInput() {
	// PORT: was Mouse.getEventButtonState()/getEventX()/getEventY()/getEventButton()
	// (LWJGL). No event-driven mouse on the 360; the XInput glue maps the cursor to
	// screen coords and calls mouseClicked()/mouseMovedOrUp() directly.
}

void GuiScreen::handleKeyboardInput() {
	// PORT: was Keyboard.getEventKeyState()/getEventKey()/getEventCharacter() (LWJGL),
	// including the F11 toggleFullscreen() shortcut. No keyboard event queue on the
	// 360; keyTyped() is invoked directly by the platform input glue. Fullscreen is
	// fixed on console, so the F11 path is dropped.
}

void GuiScreen::updateScreen() {
	// Base class: nothing. Subclasses tick their widgets here.
}

void GuiScreen::onGuiClosed() {
	// Base class: nothing. Subclasses release resources here.
}

void GuiScreen::drawDefaultBackground() {
	func_567_a(0);
}

void GuiScreen::func_567_a(int var1) {
	if(mc->theWorld != NULL) {
		// In-world pause overlay: darkening gradient (matches Java drawGradientRect).
		drawGradientRect(0, 0, width, height, -1072689136, -804253680);
	} else {
		drawBackground(var1);
	}
}

void GuiScreen::drawBackground(int var1) {
	// Lighting/fog are shader-based on Xbox 360 (Java did glDisable(GL_LIGHTING/GL_FOG)).
	Shaders::setFogEnabled(false);

	Tesselator& var2 = Tesselator::getInstance();
	g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/background.png")));
	var2.color4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var3 = 32.0F;
	var2.init();
	var2.colorI(4210752);
	var2.vertexUV(0.0F, (float)height, 0.0F, 0.0F, (float)height / var3 + (float)var1);
	var2.vertexUV((float)width, (float)height, 0.0F, (float)width / var3, (float)height / var3 + (float)var1);
	var2.vertexUV((float)width, 0.0F, 0.0F, (float)width / var3, (float)(0 + var1));
	var2.vertexUV(0.0F, 0.0F, 0.0F, 0.0F, (float)(0 + var1));
	var2.flush();
}

bool GuiScreen::doesGuiPauseGame() {
	return true;
}

void GuiScreen::deleteWorld(bool var1, int var2) {
	// PORT: not invoked by the base screen; subclasses (e.g. GuiYesNo flows) override
	// this to perform world deletion. Base implementation does nothing.
}
