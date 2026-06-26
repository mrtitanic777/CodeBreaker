#include "stdafx.h"
// Auto-generated from GuiScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include "Common.h"
#include "FontRenderer.h"
#include "GuiButton.h"
#include "Minecraft.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "RenderEngine.h"

GuiScreen::GuiScreen()
	: mc(NULL)
	, width(0)
	, height(0)
	, m_allowUserInput(false)
	, fontRenderer(NULL)
{
}

GuiScreen::~GuiScreen() {
	mc = NULL;
	fontRenderer = NULL;
}

void GuiScreen::drawScreen(int var1, int var2, float var3) {
		for(size_t var5 = 0; var5 < controlList.size(); ++var5) {
			GuiButton var4 = (GuiButton*)controlList[var5];
			var4.drawButton(mc, var1, var2);
		}

}

void GuiScreen::keyTyped(char var1, int var2) {
		if(var2 == 1) {
			mc->displayGuiScreen((GuiScreen*)NULL);
			mc->setIngameFocus();
		}

}

void GuiScreen::drawSlotInventory(int var1, int var2, int var3) {
		if(var3 == 0) {
			for(var3 = 0; var3 < controlList.size(); ++var3) {
				GuiButton var4 = (GuiButton*)controlList[var3];
				if(var4.mousePressed(var1, var2)) {
					mc->m_sndManager.playSoundFX("random.click", 1.0F, 1.0F);
					actionPerformed(var4);
				}
			}
		}

}

void GuiScreen::actionPerformed(GuiButton* var1) {
	// TODO: implement
}

void GuiScreen::setWorldAndResolution(Minecraft* var1, int var2, int var3) {
		mc = var1;
		fontRenderer = var1->fontRenderer;
		width = var2;
		height = var3;
		initGui();
}

void GuiScreen::initGui() {
	// TODO: implement
}

void GuiScreen::handleMouseInput() {
  // PORT: XInput button  // was: if(Mouse.getEventButtonState()) {
   // PORT-BLOCKED: int var1 = Mouse.getEventX() * this.width / this.mc->displayWidth;
   // PORT-BLOCKED: int var2 = this.height - Mouse.getEventY() * this.height / this.mc->displayHeight - 1;
   // PORT-BLOCKED: this.drawSlotInventory(var1, var2, Mouse.getEventButton());
  // PORT-BLOCKED: } else {
   // PORT-BLOCKED: Mouse.getEventX();
   // PORT-BLOCKED: Mouse.getEventY();
   // PORT-BLOCKED: Mouse.getEventButton();
  // PORT-BLOCKED: }
}

void GuiScreen::handleKeyboardInput() {
  // PORT: XInputGetKeystroke  // was: if(Keyboard.getEventKeyState()) {
   // PORT-BLOCKED: if(Keyboard.getEventKey() == Keyboard.KEY_F11) {
    // PORT-BLOCKED: this.mc->toggleFullscreen();
    // PORT-BLOCKED: return;
   // PORT-BLOCKED: }
// PORT-BLOCKED: 
   // PORT-BLOCKED: this.keyTyped(Keyboard.getEventCharacter(), Keyboard.getEventKey());
  // PORT-BLOCKED: }

}

void GuiScreen::updateScreen() {
	// TODO: implement
}

void GuiScreen::onGuiClosed() {
	// TODO: implement
}

void GuiScreen::drawDefaultBackground() {
		bool var1 = false;
		if(mc->theWorld != NULL) {
			drawGradientRect(0, 0, width, height, 1610941696, -1607454624);
		} else {
   // Lighting is shader-based on Xbox 360 (disabled)
   Shaders::setFogEnabled(false);
			Tesselator var2 = Tesselator::getInstance();
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/dirt.png")));
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			var2.init();
			var2.colorI(4210752);
			var2.vertexUV(0.0F, (float)height, 0.0F, 0.0F, (float)height / 32.0F);
			var2.vertexUV((float)width, (float)height, 0.0F, (float)width / 32.0F, (float)height / 32.0F);
			var2.vertexUV((float)width, 0.0F, 0.0F, (float)width / 32.0F, 0.0F);
			var2.vertexUV(0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
			var2.flush();
		}
}

bool GuiScreen::doesGuiPauseGame() {
		return true;
}
