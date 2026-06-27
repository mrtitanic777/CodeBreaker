#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiIngame.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiIngame.h"
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
#include "ChatLine.h"
#include "Chunk.h"
#include "Class.h"
#include "Color.h"
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
#include "GL12.h"
#include "GameSettings.h"
#include "Gui.h"
#include "GuiButton.h"
#include "GuiChat.h"
#include "GuiScreen.h"
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
#include "RenderHelper.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "ScaledResolution.h"
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

// PORT: runtime-stub gaps (see report).
//  - Keyboard::KEY_F3 missing from Keyboard.h (LWJGL F3 == 0x3D == 61).
//  - GL11::GL_ZERO missing from GL11.h (GL constant == 0x0000).
//  - java.lang.Runtime has no stub header; the debug overlay reads memory figures from it.
namespace {
	const int KEY_F3 = 61;
	const int GL_ZERO = 0x0000;
	struct _Runtime {
		long long maxMemory(...) { return 0LL; }
		long long totalMemory(...) { return 0LL; }
		long long freeMemory(...) { return 0LL; }
	};
	struct _RuntimeClass {
		_Runtime getRuntime(...) { return _Runtime(); }
	} Runtime;
}

RenderItem* GuiIngame::s_itemRenderer = new RenderItem();

GuiIngame::GuiIngame(Minecraft* var1)
	: rand(NULL)
	, mc(NULL)
	, updateCounter(0)
	, recordPlayingUpFor(0)
	, damagePartialTime(0)
	, prevVignetteBrightness(0)
{
	this->mc = var1;
}

GuiIngame::~GuiIngame()
{
}

void GuiIngame::renderGameOverlay(float var1, bool var2, int var3, int var4)
{
	ScaledResolution* var5 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	int var6 = var5->getScaledWidth();
	int var7 = var5->getScaledHeight();
	FontRenderer* var8 = this->mc->fontRenderer;
	this->mc->entityRenderer->setupOverlayRendering();
	GL11::glEnable(GL11::GL_BLEND);
	if(this->mc->gameSettings->fancyGraphics) {
	this->renderVignette(this->mc->thePlayer->getEntityBrightness(var1), var6, var7);
	}
	ItemStack* var9 = this->mc->thePlayer->inventory->armorItemInSlot(3);
	if(!this->mc->gameSettings->thirdPersonView && var9 != NULL && var9->itemID == Block::s_pumpkin->blockID) {
	this->renderPumpkinBlur(var6, var7);
	}
	float var10 = this->mc->thePlayer->prevTimeInPortal + (this->mc->thePlayer->timeInPortal - this->mc->thePlayer->prevTimeInPortal) * var1;
	if(var10 > 0.0F) {
	this->renderPortalOverlay(var10, var6, var7);
	}
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/gui.png"));
	InventoryPlayer* var11 = this->mc->thePlayer->inventory;
	this->zLevel = -90.0F;
	this->drawTexturedModalRect(var6 / 2 - 91, var7 - 22, 0, 0, 182, 22);
	this->drawTexturedModalRect(var6 / 2 - 91 - 1 + var11->currentItem * 20, var7 - 22 - 1, 0, 22, 24, 22);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/icons.png"));
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_ONE_MINUS_DST_COLOR, GL11::GL_ONE_MINUS_SRC_COLOR);
	this->drawTexturedModalRect(var6 / 2 - 7, var7 / 2 - 7, 0, 0, 16, 16);
	GL11::glDisable(GL11::GL_BLEND);
	bool var12 = this->mc->thePlayer->hurtResistantTime / 3 % 2 == 1;
	if(this->mc->thePlayer->hurtResistantTime < 10) {
	var12 = false;
	}
	int var13 = this->mc->thePlayer->health;
	int var14 = this->mc->thePlayer->prevHealth;
	this->rand->setSeed((long long)(this->updateCounter * 312871));
	int var15;
	int var16;
	int var17;
	if(this->mc->playerController->shouldDrawHealthBar()) {
	var15 = this->mc->thePlayer->getPlayerArmorValue();
	int var18;
	for(var16 = 0; var16 < 10; ++var16) {
	var17 = var7 - 32;
	if(var15 > 0) {
	var18 = var6 / 2 + 91 - var16 * 8 - 9;
	if(var16 * 2 + 1 < var15) {
	this->drawTexturedModalRect(var18, var17, 34, 9, 9, 9);
	}
	if(var16 * 2 + 1 == var15) {
	this->drawTexturedModalRect(var18, var17, 25, 9, 9, 9);
	}
	if(var16 * 2 + 1 > var15) {
	this->drawTexturedModalRect(var18, var17, 16, 9, 9, 9);
	}
	}
	unsigned char var27 = 0;
	if(var12) {
	var27 = 1;
	}
	int var19 = var6 / 2 - 91 + var16 * 8;
	if(var13 <= 4) {
	var17 += this->rand->nextInt(2);
	}
	this->drawTexturedModalRect(var19, var17, 16 + var27 * 9, 0, 9, 9);
	if(var12) {
	if(var16 * 2 + 1 < var14) {
	this->drawTexturedModalRect(var19, var17, 70, 0, 9, 9);
	}
	if(var16 * 2 + 1 == var14) {
	this->drawTexturedModalRect(var19, var17, 79, 0, 9, 9);
	}
	}
	if(var16 * 2 + 1 < var13) {
	this->drawTexturedModalRect(var19, var17, 52, 0, 9, 9);
	}
	if(var16 * 2 + 1 == var13) {
	this->drawTexturedModalRect(var19, var17, 61, 0, 9, 9);
	}
	}
	if(this->mc->thePlayer->isInsideOfMaterial(Material::s_water)) {
	var16 = (int)ceilf((double)(this->mc->thePlayer->air - 2) * 10.0 / 300.0);
	var17 = (int)ceilf((double)this->mc->thePlayer->air * 10.0 / 300.0) - var16;
	for(var18 = 0; var18 < var16 + var17; ++var18) {
	if(var18 < var16) {
	this->drawTexturedModalRect(var6 / 2 - 91 + var18 * 8, var7 - 32 - 9, 16, 18, 9, 9);
	} else {
	this->drawTexturedModalRect(var6 / 2 - 91 + var18 * 8, var7 - 32 - 9, 25, 18, 9, 9);
	}
	}
	}
	}
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	GL11::glPushMatrix();
	GL11::glRotatef(180.0F, 1.0F, 0.0F, 0.0F);
	RenderHelper::enableStandardItemLighting();
	GL11::glPopMatrix();
	for(var15 = 0; var15 < 9; ++var15) {
	var16 = var6 / 2 - 90 + var15 * 20 + 2;
	var17 = var7 - 16 - 3;
	this->renderInventorySlot(var15, var16, var17, var1);
	}
	RenderHelper::disableStandardItemLighting();
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	JavaString var23;
	if(Keyboard::isKeyDown(KEY_F3)) {
	var8->drawStringWithShadow((JCat() + "Minecraft Alpha v1.2.6 (" + this->mc->debugFPS + ")"),2,2,16777215);
	var8->drawStringWithShadow(this->mc->getDebugInfoRenders(), 2, 12, 16777215);
	var8->drawStringWithShadow(this->mc->getDebugInfoEntities(), 2, 22, 16777215);
	var8->drawStringWithShadow(this->mc->getDebugInfoParticles(), 2, 32, 16777215);
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  long var24 = Runtime.getRuntime().maxMemory();
	 *  long var29 = Runtime.getRuntime().totalMemory();
	 *  long var30 = Runtime.getRuntime().freeMemory();
	 *  // The _Runtime shim (top of file) returns 0 for maxMemory/totalMemory/freeMemory
	 *  // since java.lang.Runtime has no console stub, so the "Used/Allocated memory"
	 *  // debug lines below compute from zeros (figures are not real).
	 *  ---- end original Java ---- */
	long long var24 = Runtime.getRuntime().maxMemory();
	long long var29 = Runtime.getRuntime().totalMemory();
	long long var30 = Runtime.getRuntime().freeMemory();
	long long var21 = var29 - var30;
	var23 = (JCat() + "Used memory: " + var21 * 100LL / var24 + "% (" + var21 / 1024LL / 1024LL + "MB) of " + var24 / 1024LL / 1024LL + "MB");
	this->drawString(var8, var23, var6 - var8->getStringWidth(var23) - 2, 2, 14737632);
	var23 = (JCat() + "Allocated memory: " + var29 * 100LL / var24 + "% (" + var29 / 1024LL / 1024LL + "MB)");
	this->drawString(var8, var23, var6 - var8->getStringWidth(var23) - 2, 12, 14737632);
	this->drawString(var8,(JCat() + "x: " + this->mc->thePlayer->posX),2,64,14737632);
	this->drawString(var8,(JCat() + "y: " + this->mc->thePlayer->posY),2,72,14737632);
	this->drawString(var8,(JCat() + "z: " + this->mc->thePlayer->posZ),2,80,14737632);
	} else {
	var8->drawStringWithShadow("Minecraft Alpha v1.2.6", 2, 2, 16777215);
	}
	if(this->recordPlayingUpFor > 0) {
	float var25 = (float)this->recordPlayingUpFor - var1;
	var16 = (int)(var25 * 256.0F / 20.0F);
	if(var16 > 255) {
	var16 = 255;
	}
	if(var16 > 0) {
	GL11::glPushMatrix();
	GL11::glTranslatef((float)(var6 / 2), (float)(var7 - 48), 0.0F);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	var17 = Color::HSBtoRGB(var25 / 50.0F, 0.7F, 0.6F) & 16777215;
	var8->drawString(this->recordPlayingName, -var8->getStringWidth(this->recordPlayingName) / 2, -4, var17 + (var16 << 24));
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glPopMatrix();
	}
	}
	unsigned char var26 = 10;
	bool var28 = false;
	if((dynamic_cast<GuiChat*>(this->mc->currentScreen) != NULL)) {
	var26 = 20;
	var28 = true;
	}
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	GL11::glPushMatrix();
	GL11::glTranslatef(0.0F, (float)(var7 - 48), 0.0F);
	for(var17 = 0; var17 < this->chatMessageList.size() && var17 < var26; ++var17) {
	if(((ChatLine*)this->chatMessageList[var17])->updateCounter < 200 || var28) {
	double var31 = (double)((ChatLine*)this->chatMessageList[var17])->updateCounter / 200.0;
	var31 = 1.0 - var31;
	var31 *= 10.0;
	if(var31 < 0.0) {
	var31 = 0.0;
	}
	if(var31 > 1.0) {
	var31 = 1.0;
	}
	var31 *= var31;
	int var20 = (int)(255.0 * var31);
	if(var28) {
	var20 = 255;
	}
	if(var20 > 0) {
	unsigned char var32 = 2;
	int var22 = -var17 * 9;
	var23 = ((ChatLine*)this->chatMessageList[var17])->message;
	this->drawRect(var32, var22 - 1, var32 + 320, var22 + 8, var20 / 2 << 24);
	GL11::glEnable(GL11::GL_BLEND);
	var8->drawStringWithShadow(var23, var32, var22, 16777215 + (var20 << 24));
	}
	}
	}
	GL11::glPopMatrix();
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glDisable(GL11::GL_BLEND);
}

void GuiIngame::renderPumpkinBlur(int var1, int var2)
{
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	GL11::glDepthMask(false);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("%blur%/misc/pumpkinblur.png"));
	Tessellator* var3 = Tessellator::s_instance;
	var3->startDrawingQuads();
	var3->addVertexWithUV(0.0, (double)var2, -90.0, 0.0, 1.0);
	var3->addVertexWithUV((double)var1, (double)var2, -90.0, 1.0, 1.0);
	var3->addVertexWithUV((double)var1, 0.0, -90.0, 1.0, 0.0);
	var3->addVertexWithUV(0.0, 0.0, -90.0, 0.0, 0.0);
	var3->draw();
	GL11::glDepthMask(true);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame::renderVignette(float var1, int var2, int var3)
{
	var1 = 1.0F - var1;
	if(var1 < 0.0F) {
	var1 = 0.0F;
	}
	if(var1 > 1.0F) {
	var1 = 1.0F;
	}
	this->prevVignetteBrightness = (float)((double)this->prevVignetteBrightness + (double)(var1 - this->prevVignetteBrightness) * 0.01);
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	GL11::glDepthMask(false);
	GL11::glBlendFunc(GL_ZERO, GL11::GL_ONE_MINUS_SRC_COLOR);
	GL11::glColor4f(this->prevVignetteBrightness, this->prevVignetteBrightness, this->prevVignetteBrightness, 1.0F);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("%blur%/misc/vignette.png"));
	Tessellator* var4 = Tessellator::s_instance;
	var4->startDrawingQuads();
	var4->addVertexWithUV(0.0, (double)var3, -90.0, 0.0, 1.0);
	var4->addVertexWithUV((double)var2, (double)var3, -90.0, 1.0, 1.0);
	var4->addVertexWithUV((double)var2, 0.0, -90.0, 1.0, 0.0);
	var4->addVertexWithUV(0.0, 0.0, -90.0, 0.0, 0.0);
	var4->draw();
	GL11::glDepthMask(true);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
}

void GuiIngame::renderPortalOverlay(float var1, int var2, int var3)
{
	var1 *= var1;
	var1 *= var1;
	var1 = var1 * 0.8F + 0.2F;
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	GL11::glDepthMask(false);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, var1);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/terrain.png"));
	float var4 = (float)(Block::s_portal->blockIndexInTexture % 16) / 16.0F;
	float var5 = (float)(Block::s_portal->blockIndexInTexture / 16) / 16.0F;
	float var6 = (float)(Block::s_portal->blockIndexInTexture % 16 + 1) / 16.0F;
	float var7 = (float)(Block::s_portal->blockIndexInTexture / 16 + 1) / 16.0F;
	Tessellator* var8 = Tessellator::s_instance;
	var8->startDrawingQuads();
	var8->addVertexWithUV(0.0, (double)var3, -90.0, (double)var4, (double)var7);
	var8->addVertexWithUV((double)var2, (double)var3, -90.0, (double)var6, (double)var7);
	var8->addVertexWithUV((double)var2, 0.0, -90.0, (double)var6, (double)var5);
	var8->addVertexWithUV(0.0, 0.0, -90.0, (double)var4, (double)var5);
	var8->draw();
	GL11::glDepthMask(true);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
}

void GuiIngame::renderInventorySlot(int var1, int var2, int var3, float var4)
{
	ItemStack* var5 = this->mc->thePlayer->inventory->mainInventory[var1];
	if(var5 != NULL) {
	float var6 = (float)var5->animationsToGo - var4;
	if(var6 > 0.0F) {
	GL11::glPushMatrix();
	float var7 = 1.0F + var6 / 5.0F;
	GL11::glTranslatef((float)(var2 + 8), (float)(var3 + 12), 0.0F);
	GL11::glScalef(1.0F / var7, (var7 + 1.0F) / 2.0F, 1.0F);
	GL11::glTranslatef((float)(-(var2 + 8)), (float)(-(var3 + 12)), 0.0F);
	}
	s_itemRenderer->renderItemIntoGUI(this->mc->fontRenderer, this->mc->renderEngine, var5, var2, var3);
	if(var6 > 0.0F) {
	GL11::glPopMatrix();
	}
	s_itemRenderer->renderItemOverlayIntoGUI(this->mc->fontRenderer, this->mc->renderEngine, var5, var2, var3);
	}
}

void GuiIngame::updateTick()
{
	if(this->recordPlayingUpFor > 0) {
	--this->recordPlayingUpFor;
	}
	++this->updateCounter;
	for(int var1 = 0; var1 < this->chatMessageList.size(); ++var1) {
	++((ChatLine*)this->chatMessageList[var1])->updateCounter;
	}
}

void GuiIngame::addChatMessage(const JavaString& var1_)
{
	// PORT: Java reassigns the parameter (var1 = var1.substring(var2)); C++ ref param is const,
	// so work on a local mutable copy.
	JavaString var1 = var1_;
	while(this->mc->fontRenderer->getStringWidth(var1) > 320) {
	int var2;
	for(var2 = 1; var2 < var1.length() && this->mc->fontRenderer->getStringWidth(var1.substr(0, (var2 + 1) - (0))) <= 320; ++var2) {
	}
	this->addChatMessage(var1.substr(0, (var2) - (0)));
	var1 = var1.substr(var2);
	}
	// PORT: Java List.add(0, e) -> JList::add(index, value); element boxed to void*.
	this->chatMessageList.add(0, (void*)new ChatLine(var1));
	while(this->chatMessageList.size() > 50) {
	this->chatMessageList.remove((int)(this->chatMessageList.size() - 1));
	}
}

void GuiIngame::setRecordPlaying(const JavaString& var1)
{
	this->recordPlayingName = (JCat() + "Now playing: " + var1);
	this->recordPlayingUpFor = 60;
}
