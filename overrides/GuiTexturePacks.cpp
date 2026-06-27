#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiTexturePacks.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiTexturePacks.h"
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
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GL11.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "GuiSmallButton.h"
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
#include "Sys.h"
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

GuiTexturePacks::GuiTexturePacks(GuiScreen* var1)
	: parentScreen(NULL)
	, scrollOffset(0)
	, listTop(0)
	, listBottom(0)
	, listLeft(0)
	, listRight(0)
	, mousePrevY(0)
	, refreshTimer(0)
{
	this->listTop = 32;
	this->listBottom = this->height - 55 + 4;
	this->listRight = this->width;
	this->mousePrevY = -2;

	this->refreshTimer = -1;

	this->parentScreen = var1;
}

GuiTexturePacks::~GuiTexturePacks()
{
}

void GuiTexturePacks::initGui()
{
	this->controlList.push_back(new GuiSmallButton(5, this->width / 2 - 154, this->height - 48, "Open texture pack folder"));
	this->controlList.push_back(new GuiSmallButton(6,this->width / 2 + 4,this->height - 48,"Done"));
	this->mc->texturePackList->updateAvailableTexturePacks();
	this->texturePackDir = (new File(this->mc->mcDataDir, "texturepacks"))->getAbsolutePath();
	this->listTop = 32;
	this->listBottom = this->height - 58 + 4;
	this->listLeft = 0;
	this->listRight = this->width;
}

void GuiTexturePacks::actionPerformed(GuiButton* var1)
{
	if(var1->enabled) {
	if(var1->id == 5) {
	JavaString _url = (JCat() + "file://" + this->texturePackDir);
	Sys::openURL(_url);
	}
	if(var1->id == 6) {
	this->mc->renderEngine->refreshTextures();
	this->mc->displayGuiScreen(this->parentScreen);
	}
	}
}

void GuiTexturePacks::mouseClicked(int var1, int var2, int var3)
{
	GuiScreen::mouseClicked(var1, var2, var3);
}

void GuiTexturePacks::mouseMovedOrUp(int var1, int var2, int var3)
{
	GuiScreen::mouseMovedOrUp(var1, var2, var3);
}

void GuiTexturePacks::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	if(this->refreshTimer <= 0) {
	this->mc->texturePackList->updateAvailableTexturePacks();
	this->refreshTimer += 20;
	}
	JList<void*> var4 = this->mc->texturePackList->availableTexturePacks_();
	int var5;
	if(Mouse::isButtonDown(0)) {
	if(this->mousePrevY == -1) {
	if(var2 >= this->listTop && var2 <= this->listBottom) {
	var5 = this->width / 2 - 110;
	int var6 = this->width / 2 + 110;
	int var7 = (var2 - this->listTop + this->scrollOffset - 2) / 36;
	if(var1 >= var5 && var1 <= var6 && var7 >= 0 && var7 < var4.size() && this->mc->texturePackList->setTexturePack((TexturePackBase*)var4[var7])) {
	this->mc->renderEngine->refreshTextures();
	}
	this->mousePrevY = var2;
	} else {
	this->mousePrevY = -2;
	}
	} else if(this->mousePrevY >= 0) {
	this->scrollOffset -= var2 - this->mousePrevY;
	this->mousePrevY = var2;
	}
	} else {
	if(this->mousePrevY >= 0 && this->mousePrevY == var2) {
	}
	this->mousePrevY = -1;
	}
	var5 = var4.size() * 36 - (this->listBottom - this->listTop - 4);
	if(var5 < 0) {
	var5 /= 2;
	}
	if(this->scrollOffset < 0) {
	this->scrollOffset = 0;
	}
	if(this->scrollOffset > var5) {
	this->scrollOffset = var5;
	}
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_FOG);
	Tessellator* var16 = Tessellator::s_instance;
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/background.png"));
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var17 = 32.0F;
	var16->startDrawingQuads();
	var16->setColorOpaque_I(2105376);
	var16->addVertexWithUV((double)this->listLeft, (double)this->listBottom, 0.0, (double)((float)this->listLeft / var17), (double)((float)(this->listBottom + this->scrollOffset) / var17));
	var16->addVertexWithUV((double)this->listRight, (double)this->listBottom, 0.0, (double)((float)this->listRight / var17), (double)((float)(this->listBottom + this->scrollOffset) / var17));
	var16->addVertexWithUV((double)this->listRight, (double)this->listTop, 0.0, (double)((float)this->listRight / var17), (double)((float)(this->listTop + this->scrollOffset) / var17));
	var16->addVertexWithUV((double)this->listLeft, (double)this->listTop, 0.0, (double)((float)this->listLeft / var17), (double)((float)(this->listTop + this->scrollOffset) / var17));
	var16->draw();
	for(int var8 = 0; var8 < var4.size(); ++var8) {
	TexturePackBase* var9 = (TexturePackBase*)var4[var8];
	int var10 = this->width / 2 - 92 - 16;
	int var11 = 36 + var8 * 36 - this->scrollOffset;
	unsigned char var12 = 32;
	unsigned char var13 = 32;
	if(var9 == this->mc->texturePackList->selectedTexturePack) {
	int var14 = this->width / 2 - 110;
	int var15 = this->width / 2 + 110;
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	var16->startDrawingQuads();
	var16->setColorOpaque_I(8421504);
	var16->addVertexWithUV((double)var14, (double)(var11 + var12 + 2), 0.0, 0.0, 1.0);
	var16->addVertexWithUV((double)var15, (double)(var11 + var12 + 2), 0.0, 1.0, 1.0);
	var16->addVertexWithUV((double)var15, (double)(var11 - 2), 0.0, 1.0, 0.0);
	var16->addVertexWithUV((double)var14, (double)(var11 - 2), 0.0, 0.0, 0.0);
	var16->setColorOpaque_I(0);
	var16->addVertexWithUV((double)(var14 + 1), (double)(var11 + var12 + 1), 0.0, 0.0, 1.0);
	var16->addVertexWithUV((double)(var15 - 1), (double)(var11 + var12 + 1), 0.0, 1.0, 1.0);
	var16->addVertexWithUV((double)(var15 - 1), (double)(var11 - 1), 0.0, 1.0, 0.0);
	var16->addVertexWithUV((double)(var14 + 1), (double)(var11 - 1), 0.0, 0.0, 0.0);
	var16->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	}
	var9->unbindTexturePack(this->mc);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	var16->startDrawingQuads();
	var16->setColorOpaque_I(16777215);
	var16->addVertexWithUV((double)var10, (double)(var11 + var12), 0.0, 0.0, 1.0);
	var16->addVertexWithUV((double)(var10 + var13), (double)(var11 + var12), 0.0, 1.0, 1.0);
	var16->addVertexWithUV((double)(var10 + var13), (double)var11, 0.0, 1.0, 0.0);
	var16->addVertexWithUV((double)var10, (double)var11, 0.0, 0.0, 0.0);
	var16->draw();
	this->drawString(this->fontRenderer, var9->texturePackFileName, var10 + var13 + 2, var11 + 1, 16777215);
	this->drawString(this->fontRenderer, var9->firstDescriptionLine, var10 + var13 + 2, var11 + 12, 8421504);
	this->drawString(this->fontRenderer, var9->secondDescriptionLine, var10 + var13 + 2, var11 + 12 + 10, 8421504);
	}
	unsigned char var18 = 4;
	this->drawGradientOverlay(0, this->listTop, 255, 255);
	this->drawGradientOverlay(this->listBottom, this->height, 255, 255);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	GL11::glShadeModel(GL11::GL_SMOOTH);
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	var16->startDrawingQuads();
	var16->setColorRGBA_I(0, 0);
	var16->addVertexWithUV((double)this->listLeft, (double)(this->listTop + var18), 0.0, 0.0, 1.0);
	var16->addVertexWithUV((double)this->listRight, (double)(this->listTop + var18), 0.0, 1.0, 1.0);
	var16->setColorRGBA_I(0, 255);
	var16->addVertexWithUV((double)this->listRight, (double)this->listTop, 0.0, 1.0, 0.0);
	var16->addVertexWithUV((double)this->listLeft, (double)this->listTop, 0.0, 0.0, 0.0);
	var16->draw();
	var16->startDrawingQuads();
	var16->setColorRGBA_I(0, 255);
	var16->addVertexWithUV((double)this->listLeft, (double)this->listBottom, 0.0, 0.0, 1.0);
	var16->addVertexWithUV((double)this->listRight, (double)this->listBottom, 0.0, 1.0, 1.0);
	var16->setColorRGBA_I(0, 0);
	var16->addVertexWithUV((double)this->listRight, (double)(this->listBottom - var18), 0.0, 1.0, 0.0);
	var16->addVertexWithUV((double)this->listLeft, (double)(this->listBottom - var18), 0.0, 0.0, 0.0);
	var16->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	GL11::glShadeModel(GL11::GL_FLAT);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glDisable(GL11::GL_BLEND);
	this->drawCenteredString(this->fontRenderer, "Select Texture Pack", this->width / 2, 16, 16777215);
	this->drawCenteredString(this->fontRenderer, "(Place texture pack files here)", this->width / 2 - 77, this->height - 26, 8421504);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiTexturePacks::updateScreen()
{
	GuiScreen::updateScreen();
	--this->refreshTimer;
}

void GuiTexturePacks::drawGradientOverlay(int var1, int var2, int var3, int var4)
{
	Tessellator* var5 = Tessellator::s_instance;
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/background.png"));
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var6 = 32.0F;
	var5->startDrawingQuads();
	var5->setColorRGBA_I(4210752, var4);
	var5->addVertexWithUV(0.0, (double)var2, 0.0, 0.0, (double)((float)var2 / var6));
	var5->addVertexWithUV((double)this->width, (double)var2, 0.0, (double)((float)this->width / var6), (double)((float)var2 / var6));
	var5->setColorRGBA_I(4210752, var3);
	var5->addVertexWithUV((double)this->width, (double)var1, 0.0, (double)((float)this->width / var6), (double)((float)var1 / var6));
	var5->addVertexWithUV(0.0, (double)var1, 0.0, 0.0, (double)((float)var1 / var6));
	var5->draw();
}
