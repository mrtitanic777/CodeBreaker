#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"
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
#include "GL12.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
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
#include "Session.h"
#include "Slot.h"
#include "SlotInventory.h"
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

RenderItem* GuiContainer::s_itemRenderer;

GuiContainer::GuiContainer()
	: xSize(0)
	, ySize(0)
{
	this->xSize = 176;
	this->ySize = 166;
	this->inventorySlots = JList<void*>();
}

GuiContainer::~GuiContainer()
{
}

void GuiContainer::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	int var4 = (this->width - this->xSize) / 2;
	int var5 = (this->height - this->ySize) / 2;
	this->drawGuiContainerBackgroundLayer(var3);
	GL11::glPushMatrix();
	GL11::glRotatef(180.0F, 1.0F, 0.0F, 0.0F);
	RenderHelper::enableStandardItemLighting();
	GL11::glPopMatrix();
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var4, (float)var5, 0.0F);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	for(int var6 = 0; var6 < this->inventorySlots.size(); ++var6) {
	SlotInventory* var7 = (SlotInventory*)this->inventorySlots[var6];
	this->drawSlotInventory(var7);
	if(var7->isAtCursorPos(var1, var2)) {
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	int var8 = var7->xPos;
	int var9 = var7->yPos;
	this->drawGradientRect(var8, var9, var8 + 16, var9 + 16, -2130706433, -2130706433);
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	}
	}
	InventoryPlayer* var10 = this->mc->thePlayer->inventory;
	if(var10->draggingItemStack != NULL) {
	GL11::glTranslatef(0.0F, 0.0F, 32.0F);
	s_itemRenderer->renderItemIntoGUI(this->fontRenderer, this->mc->renderEngine, var10->draggingItemStack, var1 - var4 - 8, var2 - var5 - 8);
	s_itemRenderer->renderItemOverlayIntoGUI(this->fontRenderer, this->mc->renderEngine, var10->draggingItemStack, var1 - var4 - 8, var2 - var5 - 8);
	}
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	RenderHelper::disableStandardItemLighting();
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_DEPTH_TEST);
	this->drawGuiContainerForegroundLayer();
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glPopMatrix();
}

void GuiContainer::drawGuiContainerForegroundLayer()
{
}

void GuiContainer::drawSlotInventory(SlotInventory* var1)
{
	IInventory* var2 = var1->inventory;
	int var3 = var1->slotIndex;
	int var4 = var1->xPos;
	int var5 = var1->yPos;
	ItemStack* var6 = var2->getStackInSlot(var3);
	if(var6 == NULL) {
	int var7 = var1->getBackgroundIconIndex();
	if(var7 >= 0) {
	GL11::glDisable(GL11::GL_LIGHTING);
	this->mc->renderEngine->bindTexture(this->mc->renderEngine->getTexture("/gui/items.png"));
	this->drawTexturedModalRect(var4, var5, var7 % 16 * 16, var7 / 16 * 16, 16, 16);
	GL11::glEnable(GL11::GL_LIGHTING);
	return;
	}
	}
	s_itemRenderer->renderItemIntoGUI(this->fontRenderer, this->mc->renderEngine, var6, var4, var5);
	s_itemRenderer->renderItemOverlayIntoGUI(this->fontRenderer, this->mc->renderEngine, var6, var4, var5);
}

Slot* GuiContainer::getSlotAtPosition(int var1, int var2)
{
	for(int var3 = 0; var3 < this->inventorySlots.size(); ++var3) {
	SlotInventory* var4 = (SlotInventory*)this->inventorySlots[var3];
	if(var4->isAtCursorPos(var1, var2)) {
	return var4;
	}
	}
	return NULL;
}

void GuiContainer::mouseClicked(int var1, int var2, int var3)
{
	if(var3 == 0 || var3 == 1) {
	Slot* var4 = this->getSlotAtPosition(var1, var2);
	InventoryPlayer* var5 = this->mc->thePlayer->inventory;
	int var7;
	if(var4 != NULL) {
	ItemStack* var6 = var4->getStack();
	if(var6 != NULL || var5->draggingItemStack != NULL) {
	if(var6 != NULL && var5->draggingItemStack == NULL) {
	var7 = var3 == 0 ? var6->stackSize : (var6->stackSize + 1) / 2;
	var5->draggingItemStack = var4->inventory->decrStackSize(var4->slotIndex, var7);
	if(var6->stackSize == 0) {
	var4->putStack((ItemStack*)NULL);
	}
	var4->onPickupFromSlot();
	} else if(var6 == NULL && var5->draggingItemStack != NULL && var4->isItemValid(var5->draggingItemStack)) {
	var7 = var3 == 0 ? var5->draggingItemStack->stackSize : 1;
	if(var7 > var4->getSlotStackLimit()) {
	var7 = var4->getSlotStackLimit();
	}
	var4->putStack(var5->draggingItemStack->splitStack(var7));
	if(var5->draggingItemStack->stackSize == 0) {
	var5->draggingItemStack = NULL;
	}
	} else if(var6 != NULL && var5->draggingItemStack != NULL) {
	if(var4->isItemValid(var5->draggingItemStack)) {
	if(var6->itemID != var5->draggingItemStack->itemID) {
	if(var5->draggingItemStack->stackSize <= var4->getSlotStackLimit()) {
	var4->putStack(var5->draggingItemStack);
	var5->draggingItemStack = var6;
	}
	} else if(var6->itemID == var5->draggingItemStack->itemID) {
	if(var3 == 0) {
	var7 = var5->draggingItemStack->stackSize;
	if(var7 > var4->getSlotStackLimit() - var6->stackSize) {
	var7 = var4->getSlotStackLimit() - var6->stackSize;
	}
	if(var7 > var5->draggingItemStack->getMaxStackSize() - var6->stackSize) {
	var7 = var5->draggingItemStack->getMaxStackSize() - var6->stackSize;
	}
	var5->draggingItemStack->splitStack(var7);
	if(var5->draggingItemStack->stackSize == 0) {
	var5->draggingItemStack = NULL;
	}
	var6->stackSize += var7;
	} else if(var3 == 1) {
	var7 = 1;
	if(var7 > var4->getSlotStackLimit() - var6->stackSize) {
	var7 = var4->getSlotStackLimit() - var6->stackSize;
	}
	if(var7 > var5->draggingItemStack->getMaxStackSize() - var6->stackSize) {
	var7 = var5->draggingItemStack->getMaxStackSize() - var6->stackSize;
	}
	var5->draggingItemStack->splitStack(var7);
	if(var5->draggingItemStack->stackSize == 0) {
	var5->draggingItemStack = NULL;
	}
	var6->stackSize += var7;
	}
	}
	} else if(var6->itemID == var5->draggingItemStack->itemID && var5->draggingItemStack->getMaxStackSize() > 1) {
	var7 = var6->stackSize;
	if(var7 > 0 && var7 + var5->draggingItemStack->stackSize <= var5->draggingItemStack->getMaxStackSize()) {
	var5->draggingItemStack->stackSize += var7;
	var6->splitStack(var7);
	if(var6->stackSize == 0) {
	var4->putStack((ItemStack*)NULL);
	}
	var4->onPickupFromSlot();
	}
	}
	}
	}
	var4->onSlotChanged();
	} else if(var5->draggingItemStack != NULL) {
	int var9 = (this->width - this->xSize) / 2;
	var7 = (this->height - this->ySize) / 2;
	if(var1 < var9 || var2 < var7 || var1 >= var9 + this->xSize || var2 >= var7 + this->xSize) {
	EntityPlayerSP* var8 = this->mc->thePlayer;
	if(var3 == 0) {
	var8->dropPlayerItem(var5->draggingItemStack);
	var5->draggingItemStack = NULL;
	}
	if(var3 == 1) {
	var8->dropPlayerItem(var5->draggingItemStack->splitStack(1));
	if(var5->draggingItemStack->stackSize == 0) {
	var5->draggingItemStack = NULL;
	}
	}
	}
	}
	}
}

void GuiContainer::mouseMovedOrUp(int var1, int var2, int var3)
{
	if(var3 == 0) {
	}
}

void GuiContainer::keyTyped(char var1, int var2)
{
	if(var2 == 1 || var2 == this->mc->gameSettings->keyBindInventory->keyCode) {
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	}
}

void GuiContainer::onGuiClosed()
{
	if(this->mc->thePlayer != NULL) {
	InventoryPlayer* var1 = this->mc->thePlayer->inventory;
	if(var1->draggingItemStack != NULL) {
	this->mc->thePlayer->dropPlayerItem(var1->draggingItemStack);
	var1->draggingItemStack = NULL;
	}
	}
}

bool GuiContainer::doesGuiPauseGame()
{
	return false;
}

void GuiContainer::initStatics()
{
	GuiContainer::s_itemRenderer = new RenderItem();
}
