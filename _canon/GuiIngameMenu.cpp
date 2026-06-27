#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiIngameMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiIngameMenu.h"
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
#include "GuiMainMenu.h"
#include "GuiOptions.h"
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
#include "LoadingScreenRenderer.h"
#include "LogoEffectRandomizer.h"
#include "Material.h"
#include "MathHelper.h"
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

GuiIngameMenu::GuiIngameMenu()
	: updateCounter2(0)
	, updateCounter(0)
{
	this->updateCounter2 = 0;
	this->updateCounter = 0;
}

GuiIngameMenu::~GuiIngameMenu()
{
}

void GuiIngameMenu::initGui()
{
	this->updateCounter2 = 0;
	this->controlList.clear();
	this->controlList.push_back(new GuiButton(1,this->width / 2 - 100,this->height / 4 + 48,"Save and quit to title"));
	if(this->mc->isMultiplayerWorld()) {
	((GuiButton*)this->controlList[0])->displayString = "Disconnect";
	}
	this->controlList.push_back(new GuiButton(4,this->width / 2 - 100,this->height / 4 + 24,"Back to game"));
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 96,"Options..."));
}

void GuiIngameMenu::actionPerformed(GuiButton* var1)
{
	if(var1->id == 0) {
	this->mc->displayGuiScreen(new GuiOptions(this, this->mc->gameSettings));
	}
	if(var1->id == 1) {
	if(this->mc->isMultiplayerWorld()) {
	this->mc->theWorld->sendQuittingDisconnectingPacket();
	}
	this->mc->setWorld((World*)NULL);
	this->mc->displayGuiScreen(new GuiMainMenu());
	}
	if(var1->id == 4) {
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	this->mc->setIngameFocus();
	}
}

void GuiIngameMenu::updateScreen()
{
	GuiScreen::updateScreen();
	++this->updateCounter;
}

void GuiIngameMenu::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	bool var4 = !this->mc->theWorld->quickSaveWorld(this->updateCounter2++);
	if(var4 || this->updateCounter < 20) {
	float var5 = ((float)(this->updateCounter % 10) + var3) / 10.0F;
	var5 = MathHelper::sin(var5 * (float)3.1415927F * 2.0F) * 0.2F + 0.8F;
	int var6 = (int)(255.0F * var5);
	this->drawString(this->fontRenderer, "Saving level..", 8, this->height - 16, var6 << 16 | var6 << 8 | var6);
	}
	this->drawCenteredString(this->fontRenderer, "Game menu", this->width / 2, 40, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiIngameMenu::initStatics()
{
}
