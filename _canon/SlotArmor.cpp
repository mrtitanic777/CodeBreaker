#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SlotArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotArmor.h"
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
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiContainer.h"
#include "GuiIngame.h"
#include "GuiInventory.h"
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
#include "ItemArmor.h"
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

SlotArmor::SlotArmor(GuiInventory* var1, GuiContainer* var2, IInventory* var3, int var4, int var5, int var6, int var7)
	: SlotInventory(var2, var3, var4, var5, var6)
	, armorType(0)
	, guiInventory(NULL)
{
	this->guiInventory = var1;
	this->armorType = var7;
}

SlotArmor::~SlotArmor()
{
}

int SlotArmor::getSlotStackLimit()
{
	return 1;
}

bool SlotArmor::isItemValid(ItemStack* var1)
{
	if((dynamic_cast<ItemArmor*>(var1->getItem()) != NULL)) {
	return ((ItemArmor*)var1->getItem())->armorType == this->armorType;
	} else {
	System::out.println((JCat() + var1->getItem()->shiftedIndex + ", " + this->armorType));
	return var1->getItem()->shiftedIndex == Block::s_pumpkin->blockID ? this->armorType == 0 : false;
	}
}

int SlotArmor::getBackgroundIconIndex()
{
	return 15 + this->armorType * 16;
}

void SlotArmor::initStatics()
{
}
