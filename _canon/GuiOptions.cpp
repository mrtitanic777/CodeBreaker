#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiOptions.h"
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
#include "GuiControls.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "GuiSlider.h"
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

GuiOptions::GuiOptions(GuiScreen* var1, GameSettings* var2)
	: parentScreen(NULL)
	, options(NULL)
{
	this->screenTitle = "Options";
	this->parentScreen = var1;
	this->options = var2;
}

GuiOptions::~GuiOptions()
{
}

void GuiOptions::initGui()
{
	for(int var1 = 0; var1 < this->options->numberOfOptions; ++var1) {
	int var2 = this->options->getOptionControlType(var1);
	if(var2 == 0) {
	this->controlList.push_back(new GuiSmallButton(var1, this->width / 2 - 155 + var1 % 2 * 160, this->height / 6 + 24 * (var1 >> 1), this->options->getOptionDisplayString(var1)));
	} else {
	this->controlList.push_back(new GuiSlider(var1, this->width / 2 - 155 + var1 % 2 * 160, this->height / 6 + 24 * (var1 >> 1), var1, this->options->getOptionDisplayString(var1), this->options->getOptionFloatValue(var1)));
	}
	}
	this->controlList.push_back(new GuiButton(100,this->width / 2 - 100,this->height / 6 + 120 + 12,"Controls..."));
	this->controlList.push_back(new GuiButton(200,this->width / 2 - 100,this->height / 6 + 168,"Done"));
}

void GuiOptions::actionPerformed(GuiButton* var1)
{
	if(var1->enabled) {
	if(var1->id < 100) {
	this->options->setOptionValue(var1->id, 1);
	var1->displayString = this->options->getOptionDisplayString(var1->id);
	}
	if(var1->id == 100) {
	this->mc->gameSettings->saveOptions();
	this->mc->displayGuiScreen(new GuiControls(this, this->options));
	}
	if(var1->id == 200) {
	this->mc->gameSettings->saveOptions();
	this->mc->displayGuiScreen(this->parentScreen);
	}
	}
}

void GuiOptions::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer, this->screenTitle, this->width / 2, 20, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiOptions::initStatics()
{
}
