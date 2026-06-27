#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiInventory.h"
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
#include "CraftingInventoryCB.h"
#include "CraftingInventoryPlayerCB.h"
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
#include "GuiContainer.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryCrafting.h"
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
#include "SlotArmor.h"
#include "SlotCrafting.h"
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

GuiInventory::GuiInventory(IInventory* var1, JArray<ItemStack* > var2)
	: craftingInventory(NULL)
	, xSize_lo(0)
	, ySize_lo(0)
{
	this->allowUserInput = true;
	this->craftingInventory = new CraftingInventoryPlayerCB(var2);
	this->inventorySlots.push_back(new SlotCrafting(this, this->craftingInventory->craftMatrix, this->craftingInventory->craftResult, 0, 144, 36));
	int var3;
	int var4;
	for(var3 = 0; var3 < 2; ++var3) {
	for(var4 = 0; var4 < 2; ++var4) {
	this->inventorySlots.push_back(new SlotInventory(this, this->craftingInventory->craftMatrix, var4 + var3 * 2, 88 + var4 * 18, 26 + var3 * 18));
	}
	}
	for(var3 = 0; var3 < 4; ++var3) {
	this->inventorySlots.push_back(new SlotArmor(this, this, var1, var1->getSizeInventory() - 1 - var3, 8, 8 + var3 * 18, var3));
	}
	for(var3 = 0; var3 < 3; ++var3) {
	for(var4 = 0; var4 < 9; ++var4) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var4 + (var3 + 1) * 9, 8 + var4 * 18, 84 + var3 * 18));
	}
	}
	for(var3 = 0; var3 < 9; ++var3) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var3, 8 + var3 * 18, 142));
	}
}

GuiInventory::~GuiInventory()
{
	delete craftingInventory;
	craftingInventory = NULL;
}

void GuiInventory::drawGuiContainerForegroundLayer()
{
	this->fontRenderer->drawString("Crafting", 86, 16, 4210752);
}

void GuiInventory::drawScreen(int var1, int var2, float var3)
{
	GuiContainer::drawScreen(var1, var2, var3);
	this->xSize_lo = (float)var1;
	this->ySize_lo = (float)var2;
}

void GuiInventory::drawGuiContainerBackgroundLayer(float var1)
{
	int var2 = this->mc->renderEngine->getTexture("/gui/inventory.png");
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	this->mc->renderEngine->bindTexture(var2);
	int var3 = (this->width - this->xSize) / 2;
	int var4 = (this->height - this->ySize) / 2;
	this->drawTexturedModalRect(var3, var4, 0, 0, this->xSize, this->ySize);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	GL11::glEnable(GL11::GL_COLOR_MATERIAL);
	GL11::glPushMatrix();
	GL11::glTranslatef((float)(var3 + 51), (float)(var4 + 75), 50.0F);
	float var5 = 30.0F;
	GL11::glScalef(-var5, var5, var5);
	GL11::glRotatef(180.0F, 0.0F, 0.0F, 1.0F);
	float var6 = this->mc->thePlayer->renderYawOffset;
	float var7 = this->mc->thePlayer->rotationYaw;
	float var8 = this->mc->thePlayer->rotationPitch;
	float var9 = (float)(var3 + 51) - this->xSize_lo;
	float var10 = (float)(var4 + 75 - 50) - this->ySize_lo;
	GL11::glRotatef(135.0F, 0.0F, 1.0F, 0.0F);
	RenderHelper::enableStandardItemLighting();
	GL11::glRotatef(-135.0F, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-((float)atanf((double)(var10 / 40.0F))) * 20.0F, 1.0F, 0.0F, 0.0F);
	this->mc->thePlayer->renderYawOffset = (float)atanf((double)(var9 / 40.0F)) * 20.0F;
	this->mc->thePlayer->rotationYaw = (float)atanf((double)(var9 / 40.0F)) * 40.0F;
	this->mc->thePlayer->rotationPitch = -((float)atanf((double)(var10 / 40.0F))) * 20.0F;
	GL11::glTranslatef(0.0F, this->mc->thePlayer->yOffset, 0.0F);
	RenderManager::s_instance->doRenderEntity(this->mc->thePlayer, 0.0, 0.0, 0.0, 0.0F, 1.0F);
	this->mc->thePlayer->renderYawOffset = var6;
	this->mc->thePlayer->rotationYaw = var7;
	this->mc->thePlayer->rotationPitch = var8;
	GL11::glPopMatrix();
	RenderHelper::disableStandardItemLighting();
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
}

void GuiInventory::initStatics()
{
}
