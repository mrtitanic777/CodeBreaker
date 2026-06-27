#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiYesNo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiYesNo.h"
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

GuiYesNo::GuiYesNo(GuiScreen* var1, const JavaString& var2, const JavaString& var3, int var4)
	: parentScreen(NULL)
	, worldNumber(0)
{
	this->parentScreen = var1;
	this->message1 = var2;
	this->message2 = var3;
	this->worldNumber = var4;
}

GuiYesNo::~GuiYesNo()
{
}

void GuiYesNo::initGui()
{
	this->controlList.push_back(new GuiSmallButton(0,this->width / 2 - 155 + 0,this->height / 6 + 96,"Yes"));
	this->controlList.push_back(new GuiSmallButton(1,this->width / 2 - 155 + 160,this->height / 6 + 96,"No"));
}

void GuiYesNo::actionPerformed(GuiButton* var1)
{
	this->parentScreen->deleteWorld(var1->id == 0, this->worldNumber);
}

void GuiYesNo::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer, this->message1, this->width / 2, 70, 16777215);
	this->drawCenteredString(this->fontRenderer, this->message2, this->width / 2, 90, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiYesNo::initStatics()
{
}
