#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiConflictWarning.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiConflictWarning.h"
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

GuiConflictWarning::GuiConflictWarning()
	: updateCounter(0)
{
	this->updateCounter = 0;
}

GuiConflictWarning::~GuiConflictWarning()
{
}

void GuiConflictWarning::updateScreen()
{
	++this->updateCounter;
}

void GuiConflictWarning::initGui()
{
	this->controlList.clear();
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 120 + 12,"Back to title screen"));
}

void GuiConflictWarning::actionPerformed(GuiButton* var1)
{
	if(var1->enabled) {
	if(var1->id == 0) {
	this->mc->displayGuiScreen(new GuiMainMenu());
	}
	}
}

void GuiConflictWarning::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer,"Level save conflict",this->width / 2,this->height / 4 - 60 + 20,16777215);
	this->drawString(this->fontRenderer,"Minecraft detected a conflict in the level save data.",this->width / 2 - 140,this->height / 4 - 60 + 60 + 0,10526880);
	this->drawString(this->fontRenderer,"This could be caused by two copies of the game",this->width / 2 - 140,this->height / 4 - 60 + 60 + 18,10526880);
	this->drawString(this->fontRenderer,"accessing the same level.",this->width / 2 - 140,this->height / 4 - 60 + 60 + 27,10526880);
	this->drawString(this->fontRenderer,"To prevent level corruption, the current game has quit.",this->width / 2 - 140,this->height / 4 - 60 + 60 + 45,10526880);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiConflictWarning::initStatics()
{
}
