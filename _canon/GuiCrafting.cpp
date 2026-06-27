#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiCrafting.h"
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
#include "CraftingInventoryWorkbenchCB.h"
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
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Session.h"
#include "Slot.h"
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

GuiCrafting::GuiCrafting(InventoryPlayer* var1)
	: craftingInventory(NULL)
{
	this->craftingInventory = new CraftingInventoryWorkbenchCB();
	this->inventorySlots.push_back(new SlotCrafting(this, this->craftingInventory->craftMatrix, this->craftingInventory->craftResult, 0, 124, 35));
	int var2;
	int var3;
	for(var2 = 0; var2 < 3; ++var2) {
	for(var3 = 0; var3 < 3; ++var3) {
	this->inventorySlots.push_back(new SlotInventory(this, this->craftingInventory->craftMatrix, var3 + var2 * 3, 30 + var3 * 18, 17 + var2 * 18));
	}
	}
	for(var2 = 0; var2 < 3; ++var2) {
	for(var3 = 0; var3 < 9; ++var3) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var3 + (var2 + 1) * 9, 8 + var3 * 18, 84 + var2 * 18));
	}
	}
	for(var2 = 0; var2 < 9; ++var2) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var2, 8 + var2 * 18, 142));
	}
}

GuiCrafting::~GuiCrafting()
{
}

void GuiCrafting::onGuiClosed()
{
	GuiContainer::onGuiClosed();
	this->craftingInventory->onCraftGuiClosed(this->mc->thePlayer);
}

void GuiCrafting::drawGuiContainerForegroundLayer()
{
	this->fontRenderer->drawString("Crafting", 28, 6, 4210752);
	this->fontRenderer->drawString("Inventory",8,this->ySize - 96 + 2,4210752);
}

void GuiCrafting::drawGuiContainerBackgroundLayer(float var1)
{
	int var2 = this->mc->renderEngine->getTexture("/gui/crafting.png");
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	this->mc->renderEngine->bindTexture(var2);
	int var3 = (this->width - this->xSize) / 2;
	int var4 = (this->height - this->ySize) / 2;
	this->drawTexturedModalRect(var3, var4, 0, 0, this->xSize, this->ySize);
}

void GuiCrafting::initStatics()
{
}
