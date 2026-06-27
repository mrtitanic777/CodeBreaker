#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiConnecting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiConnecting.h"
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
#include "ChunkProviderClient.h"
#include "Class.h"
#include "Component.h"
#include "Cursor.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
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
#include "HashEntry.h"
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
#include "MCHashTable.h"
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
#include "NetClientHandler.h"
#include "NetHandler.h"
#include "NetworkManager.h"
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
#include "Socket.h"
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
#include "ThreadConnectToServer.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldClient.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

GuiConnecting::GuiConnecting(Minecraft* var1, const JavaString& var2, int var3)
	: clientHandler(NULL)
	, cancelled(0)
{
	this->cancelled = false;
	var1->setWorld((World*)NULL);
	(new ThreadConnectToServer(this, var1, var2, var3))->start();
}

GuiConnecting::~GuiConnecting()
{
}

void GuiConnecting::updateScreen()
{
	if(this->clientHandler != NULL) {
	this->clientHandler->processReadPackets();
	}
}

void GuiConnecting::keyTyped(char var1, int var2)
{
}

void GuiConnecting::initGui()
{
	this->controlList.clear();
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 120 + 12,"Cancel"));
}

void GuiConnecting::actionPerformed(GuiButton* var1)
{
	if(var1->id == 0) {
	this->cancelled = true;
	if(this->clientHandler != NULL) {
	this->clientHandler->disconnect();
	}
	this->mc->displayGuiScreen(new GuiMainMenu());
	}
}

void GuiConnecting::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	if(this->clientHandler == NULL) {
	this->drawCenteredString(this->fontRenderer, "Connecting to the server...", this->width / 2, this->height / 2 - 50, 16777215);
	this->drawCenteredString(this->fontRenderer, "", this->width / 2, this->height / 2 - 10, 16777215);
	} else {
	this->drawCenteredString(this->fontRenderer, "Logging in...", this->width / 2, this->height / 2 - 50, 16777215);
	this->drawCenteredString(this->fontRenderer, this->clientHandler->connectionStatus, this->width / 2, this->height / 2 - 10, 16777215);
	}
	GuiScreen::drawScreen(var1, var2, var3);
}

NetClientHandler* GuiConnecting::setNetClientHandler(GuiConnecting* var0, NetClientHandler* var1)
{
	return var0->clientHandler = var1;
}

bool GuiConnecting::isCancelled(GuiConnecting* var0)
{
	return var0->cancelled;
}

NetClientHandler* GuiConnecting::getNetClientHandler(GuiConnecting* var0)
{
	return var0->clientHandler;
}

void GuiConnecting::initStatics()
{
}
