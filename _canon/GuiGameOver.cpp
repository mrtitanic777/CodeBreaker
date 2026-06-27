#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiGameOver.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiGameOver.h"
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

GuiGameOver::~GuiGameOver()
{
}

void GuiGameOver::initGui()
{
	this->controlList.clear();
	this->controlList.push_back(new GuiButton(1,this->width / 2 - 100,this->height / 4 + 72,"Respawn"));
	this->controlList.push_back(new GuiButton(2,this->width / 2 - 100,this->height / 4 + 96,"Title menu"));
	if(this->mc->session == NULL) {
	((GuiButton*)this->controlList[1])->enabled = false;
	}
}

void GuiGameOver::keyTyped(char var1, int var2)
{
}

void GuiGameOver::actionPerformed(GuiButton* var1)
{
	if(var1->id == 0) {
	}
	if(var1->id == 1) {
	this->mc->thePlayer->respawnPlayer();
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	}
	if(var1->id == 2) {
	this->mc->setWorld((World*)NULL);
	this->mc->displayGuiScreen(new GuiMainMenu());
	}
}

void GuiGameOver::drawScreen(int var1, int var2, float var3)
{
	this->drawGradientRect(0, 0, this->width, this->height, 1615855616, -1602211792);
	GL11::glPushMatrix();
	GL11::glScalef(2.0F, 2.0F, 2.0F);
	this->drawCenteredString(this->fontRenderer, "Game over!", this->width / 2 / 2, 30, 16777215);
	GL11::glPopMatrix();
	this->drawCenteredString(this->fontRenderer,(JCat() + "Score: &e" + this->mc->thePlayer->getScore()),this->width / 2,100,16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

bool GuiGameOver::doesGuiPauseGame()
{
	return false;
}

void GuiGameOver::initStatics()
{
}
