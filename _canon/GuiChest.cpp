#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiChest.h"
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
#include "GuiContainer.h"
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

GuiChest::GuiChest(IInventory* var1, IInventory* var2)
	: lowerPlayerInventory(NULL)
	, upperChestInventory(NULL)
	, numRows(0)
{
	this->numRows = 0;
	this->lowerPlayerInventory = var1;
	this->upperChestInventory = var2;
	this->allowUserInput = false;
	short var3 = 222;
	int var4 = var3 - 108;
	this->numRows = var2->getSizeInventory() / 9;
	this->ySize = var4 + this->numRows * 18;
	int var5 = (this->numRows - 4) * 18;
	int var6;
	int var7;
	for(var6 = 0; var6 < this->numRows; ++var6) {
	for(var7 = 0; var7 < 9; ++var7) {
	this->inventorySlots.push_back(new SlotInventory(this, var2, var7 + var6 * 9, 8 + var7 * 18, 18 + var6 * 18));
	}
	}
	for(var6 = 0; var6 < 3; ++var6) {
	for(var7 = 0; var7 < 9; ++var7) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var7 + (var6 + 1) * 9, 8 + var7 * 18, 103 + var6 * 18 + var5));
	}
	}
	for(var6 = 0; var6 < 9; ++var6) {
	this->inventorySlots.push_back(new SlotInventory(this, var1, var6, 8 + var6 * 18, 161 + var5));
	}
}

GuiChest::~GuiChest()
{
}

void GuiChest::drawGuiContainerForegroundLayer()
{
	this->fontRenderer->drawString(this->upperChestInventory->getInvName(), 8, 6, 4210752);
	this->fontRenderer->drawString(this->lowerPlayerInventory->getInvName(), 8, this->ySize - 96 + 2, 4210752);
}

void GuiChest::drawGuiContainerBackgroundLayer(float var1)
{
	int var2 = this->mc->renderEngine->getTexture("/gui/container.png");
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	this->mc->renderEngine->bindTexture(var2);
	int var3 = (this->width - this->xSize) / 2;
	int var4 = (this->height - this->ySize) / 2;
	this->drawTexturedModalRect(var3, var4, 0, 0, this->xSize, this->numRows * 18 + 17);
	this->drawTexturedModalRect(var3, var4 + this->numRows * 18 + 17, 0, 126, this->xSize, 96);
}

void GuiChest::initStatics()
{
}
