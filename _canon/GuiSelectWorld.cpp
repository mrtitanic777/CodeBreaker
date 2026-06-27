#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiSelectWorld.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiSelectWorld.h"
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
#include "GuiDeleteWorld.h"
#include "GuiIngame.h"
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
#include "PlayerControllerSP.h"
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

GuiSelectWorld::GuiSelectWorld(GuiScreen* var1)
	: parentScreen(NULL)
	, selected(0)
{
	this->screenTitle = "Select world";
	this->selected = false;
	this->parentScreen = var1;
}

GuiSelectWorld::~GuiSelectWorld()
{
}

void GuiSelectWorld::initGui()
{
	File* var1 = Minecraft::getMinecraftDir();
	for(int var2 = 0; var2 < 5; ++var2) {
	NBTTagCompound* var3 = World::getWorldInfoNBT(var1,(JCat() + "World" + (var2 + 1)));
	if(var3 == NULL) {
	this->controlList.push_back(new GuiButton(var2,this->width / 2 - 100,this->height / 6 + 24 * var2,"- empty -"));
	} else {
	JavaString var4 = (JCat() + "World " + (var2 + 1));
	long long var5 = var3->getLong("SizeOnDisk");
	var4 = (JCat() + var4 + " (" + (float)(var5 / 1024LL * 100LL / 1024LL) / 100.0F + " MB)");
	this->controlList.push_back(new GuiButton(var2, this->width / 2 - 100, this->height / 6 + 24 * var2, var4));
	}
	}
	this->initGui2();
}

JavaString GuiSelectWorld::getWorldName(int var1)
{
	File* var2 = Minecraft::getMinecraftDir();
	return World::getWorldInfoNBT(var2,(JCat() + "World" + var1)) != NULL ? "World" + var1 : NULL;
}

void GuiSelectWorld::initGui2()
{
	this->controlList.push_back(new GuiButton(5,this->width / 2 - 100,this->height / 6 + 120 + 12,"Delete world..."));
	this->controlList.push_back(new GuiButton(6,this->width / 2 - 100,this->height / 6 + 168,"Cancel"));
}

void GuiSelectWorld::actionPerformed(GuiButton* var1)
{
	if(var1->enabled) {
	if(var1->id < 5) {
	this->selectWorld(var1->id + 1);
	} else if(var1->id == 5) {
	this->mc->displayGuiScreen(new GuiDeleteWorld(this));
	} else if(var1->id == 6) {
	this->mc->displayGuiScreen(this->parentScreen);
	}
	}
}

void GuiSelectWorld::selectWorld(int var1)
{
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	if(!this->selected) {
	this->selected = true;
	this->mc->playerController = new PlayerControllerSP(this->mc);
	this->mc->startWorld((JCat() + "World" + var1));
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	}
}

void GuiSelectWorld::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	this->drawCenteredString(this->fontRenderer, this->screenTitle, this->width / 2, 20, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiSelectWorld::initStatics()
{
}
