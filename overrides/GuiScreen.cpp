#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include "Input.h"            // CONSOLE: Input::navUp/Down/... discrete menu nav + Mouse::setCursorPosition
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "Canvas.h"
#include "Chunk.h"
#include "Class.h"
#include "Component.h"
#include "Cursor.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GL11.h"
#include "GameSettings.h"
#include "Gui.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "Keyboard.h"
#include "LoadingScreenRenderer.h"
#include "Material.h"
#include "Minecraft.h"
#include "MinecraftApplet.h"
#include "MobSpawnerBase.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "Mouse.h"
#include "MouseHelper.h"
#include "MovementInput.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OpenGlCapsChecker.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "PlayerController.h"
#include "PositionTexureVertex.h"
#include "Random.h"
#include "Render.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Session.h"
#include "SoundManager.h"
#include "SoundPool.h"
#include "SoundPoolEntry.h"
#include "SoundSystem.h"
#include "StepSound.h"
#include "Tessellator.h"
#include "TextureLavaFX.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "TextureWaterFX.h"
#include "TexturedQuad.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

// --- local shims for java.awt.datatransfer clipboard classes (no runtime -----
// header; the Xbox 360 target has no system clipboard). Minimal faithful
// behavior: an empty clipboard so getClipboardString() returns "".
// See report's runtime-stub gaps.
struct DataFlavorRef { };
struct _DataFlavor { DataFlavorRef stringFlavor; };
static _DataFlavor DataFlavor;
class Transferable {
public:
	bool isDataFlavorSupported(const DataFlavorRef&) { return false; }
	void* getTransferData(const DataFlavorRef&) { return (void*)NULL; }
};
class Clipboard {
public:
	Transferable* getContents(void*) { return (Transferable*)NULL; }
};
class Toolkit {
public:
	Clipboard getSystemClipboard() { return Clipboard(); }
	static Toolkit getDefaultToolkit() { return Toolkit(); }
};

GuiScreen::~GuiScreen()
{
}

void GuiScreen::drawScreen(int var1, int var2, float var3)
{
	for(int var4 = 0; var4 < this->controlList.size(); ++var4) {
	GuiButton* var5 = (GuiButton*)this->controlList[var4];
	var5->drawButton(this->mc, var1, var2);
	}
}

void GuiScreen::keyTyped(char var1, int var2)
{
	if(var2 == 1) {
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	this->mc->setIngameFocus();
	}
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  public static String getClipboardString() {
 *      try {
 *          Transferable var0 = Toolkit.getDefaultToolkit().getSystemClipboard().getContents((Object)null);
 *          if(var0 != null && var0.isDataFlavorSupported(DataFlavor.stringFlavor)) {
 *              String var1 = (String)var0.getTransferData(DataFlavor.stringFlavor);
 *              return var1;
 *          }
 *      } catch (Exception var2) {
 *      }
 *      return null;
 *  }
 *  // The Toolkit/Clipboard/DataFlavor/Transferable shims at top of file fake the
 *  // java.awt.datatransfer API: getSystemClipboard() returns an empty Clipboard,
 *  // so this always falls through and returns "" (Xbox 360 has no system clipboard).
 *  ---- end original Java ---- */
JavaString GuiScreen::getClipboardString()
{
	try {
	Transferable* var0 = Toolkit::getDefaultToolkit().getSystemClipboard().getContents((void*)NULL);
	if(var0 != NULL && var0->isDataFlavorSupported(DataFlavor.stringFlavor)) {
	JavaString var1 = *(JavaString*)var0->getTransferData(DataFlavor.stringFlavor);
	return var1;
	}
	} catch (Exception) {
	}
	return std::string();
}

void GuiScreen::mouseClicked(int var1, int var2, int var3)
{
	if(var3 == 0) {
	for(int var4 = 0; var4 < this->controlList.size(); ++var4) {
	GuiButton* var5 = (GuiButton*)this->controlList[var4];
	if(var5->mousePressed(this->mc, var1, var2)) {
	this->selectedButton = var5;
	this->mc->sndManager->playSoundFX("random.click", 1.0F, 1.0F);
	this->actionPerformed(var5);
	}
	}
	}
}

void GuiScreen::mouseMovedOrUp(int var1, int var2, int var3)
{
	if(this->selectedButton != NULL && var3 == 0) {
	this->selectedButton->mouseReleased(var1, var2);
	this->selectedButton = NULL;
	}
}

void GuiScreen::actionPerformed(GuiButton* var1)
{
}

void GuiScreen::setWorldAndResolution(Minecraft* var1, int var2, int var3)
{
	this->mc = var1;
	this->fontRenderer = var1->fontRenderer;
	this->width = var2;
	this->height = var3;
	this->controlList.clear();
	this->initGui();
}

void GuiScreen::initGui()
{
}

void GuiScreen::handleInput()
{
	this->handleControllerMenuNav();   // CONSOLE: Indev-style discrete d-pad button selection + cursor snap
	while(Mouse::next()) {
	this->handleMouseInput();
	}
	while(Keyboard::next()) {
	this->handleKeyboardInput();
	}
}

// CONSOLE (controller): discrete menu navigation. The d-pad / left stick move `navSelected` between the
// ENABLED+visible buttons in controlList (wrap-around, skipping disabled), and we snap the virtual cursor
// onto the selected button's center. That makes the EXISTING paths do the work: GuiButton::drawButton
// highlights the button under the cursor (yellow text + hover tile), and an A press -> a left-click at the
// cursor (Input::poll) -> GuiScreen::mouseClicked -> actionPerformed. Screens with no buttons (slot GUIs)
// fall through untouched and use the right-stick free pointer. Mirrors the working Indev port's selectedButton.
static bool _navButtonOK(JList<void*>& list, int i) {
	if (i < 0 || i >= (int)list.size()) return false;
	GuiButton* b = (GuiButton*)list[i];
	return b != NULL && b->enabled && b->enabled2;
}
void GuiScreen::handleControllerMenuNav()
{
	int n = (int)this->controlList.size();
	if (n <= 0) { this->navSelected = -1; return; }   // no buttons -> nothing to navigate

	bool up = Input::navUp(), down = Input::navDown();
	bool left = Input::navLeft(), right = Input::navRight();
	int step = (down || right) ? 1 : ((up || left) ? -1 : 0);

	// Ensure a valid selection (first navigable button) -- handles first entry and a selection that
	// became disabled/removed.
	if (!_navButtonOK(this->controlList, this->navSelected)) {
		this->navSelected = -1;
		for (int i = 0; i < n; ++i) { if (_navButtonOK(this->controlList, i)) { this->navSelected = i; break; } }
	}
	if (this->navSelected < 0) return;   // no navigable buttons on this screen

	if (step != 0) {
		int i = this->navSelected;
		for (int guard = 0; guard < n; ++guard) {
			i += step;
			if (i < 0) i = n - 1; else if (i >= n) i = 0;
			if (_navButtonOK(this->controlList, i)) { this->navSelected = i; break; }
		}
	}

	// Snap the cursor onto the selected button center. Cursor is DISPLAY pixels (top-down); button coords
	// are GUI-scaled. This is the inverse of EntityRenderer's mouseX/Y mapping (var10/var11):
	//   guiX = cursorX * width/displayWidth ; guiY = height - getY*height/displayHeight - 1.
	if (this->width > 0 && this->height > 0 && this->mc != NULL) {
		GuiButton* b = (GuiButton*)this->controlList[this->navSelected];
		int bx = b->xPosition + b->width / 2;
		int by = b->yPosition + b->height / 2;
		int cx = bx * this->mc->displayWidth / this->width;
		int cy = by * this->mc->displayHeight / this->height;
		Mouse::setCursorPosition(cx, cy);
	}
}

void GuiScreen::handleMouseInput()
{
	int var1;
	int var2;
	if(Mouse::getEventButtonState()) {
	var1 = Mouse::getEventX() * this->width / this->mc->displayWidth;
	var2 = this->height - Mouse::getEventY() * this->height / this->mc->displayHeight - 1;
	this->mouseClicked(var1, var2, Mouse::getEventButton());
	} else {
	var1 = Mouse::getEventX() * this->width / this->mc->displayWidth;
	var2 = this->height - Mouse::getEventY() * this->height / this->mc->displayHeight - 1;
	this->mouseMovedOrUp(var1, var2, Mouse::getEventButton());
	}
}

void GuiScreen::handleKeyboardInput()
{
	if(Keyboard::getEventKeyState()) {
	if(Keyboard::getEventKey() == Keyboard::KEY_F11) {
	this->mc->toggleFullscreen();
	return;
	}
	this->keyTyped(Keyboard::getEventCharacter(), Keyboard::getEventKey());
	}
}

void GuiScreen::updateScreen()
{
}

void GuiScreen::onGuiClosed()
{
}

void GuiScreen::drawDefaultBackground()
{
	this->createEntityInWorld(0);
}

void GuiScreen::createEntityInWorld(int var1)
{
	if(this->mc->theWorld != NULL) {
	this->drawGradientRect(0, 0, this->width, this->height, -1072689136, -804253680);
	} else {
	this->drawBackground(var1);
	}
}

void GuiScreen::drawBackground(int var1)
{
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_FOG);
	Tessellator* var2 = Tessellator::s_instance;
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/background.png"));
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var3 = 32.0F;
	var2->startDrawingQuads();
	var2->setColorOpaque_I(4210752);
	var2->addVertexWithUV(0.0, (double)this->height, 0.0, 0.0, (double)((float)this->height / var3 + (float)var1));
	var2->addVertexWithUV((double)this->width, (double)this->height, 0.0, (double)((float)this->width / var3), (double)((float)this->height / var3 + (float)var1));
	var2->addVertexWithUV((double)this->width, 0.0, 0.0, (double)((float)this->width / var3), (double)(0 + var1));
	var2->addVertexWithUV(0.0, 0.0, 0.0, 0.0, (double)(0 + var1));
	var2->draw();
}

bool GuiScreen::doesGuiPauseGame()
{
	return true;
}

void GuiScreen::deleteWorld(bool var1, int var2)
{
}
