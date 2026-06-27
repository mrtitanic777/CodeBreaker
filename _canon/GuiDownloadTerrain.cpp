#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiDownloadTerrain.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiDownloadTerrain.h"
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
#include "Packet.h"
#include "Packet0KeepAlive.h"
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

GuiDownloadTerrain::GuiDownloadTerrain(NetClientHandler* var1)
	: netHandler(NULL)
	, updateCounter(0)
{
	this->updateCounter = 0;
	this->netHandler = var1;
}

GuiDownloadTerrain::~GuiDownloadTerrain()
{
}

void GuiDownloadTerrain::keyTyped(char var1, int var2)
{
}

void GuiDownloadTerrain::initGui()
{
	this->controlList.clear();
}

void GuiDownloadTerrain::updateScreen()
{
	++this->updateCounter;
	if(this->updateCounter % 20 == 0) {
	this->netHandler->addToSendQueue(new Packet0KeepAlive());
	}
	if(this->netHandler != NULL) {
	this->netHandler->processReadPackets();
	}
}

void GuiDownloadTerrain::actionPerformed(GuiButton* var1)
{
}

void GuiDownloadTerrain::drawScreen(int var1, int var2, float var3)
{
	this->drawBackground(0);
	this->drawCenteredString(this->fontRenderer, "Downloading terrain", this->width / 2, this->height / 2 - 50, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiDownloadTerrain::initStatics()
{
}
