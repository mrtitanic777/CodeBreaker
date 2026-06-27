#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ThreadConnectToServer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ThreadConnectToServer.h"
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
#include "ConnectException.h"
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
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiConnectFailed.h"
#include "GuiConnecting.h"
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
#include "Packet2Handshake.h"
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
#include "UnknownHostException.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldClient.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

ThreadConnectToServer::ThreadConnectToServer(GuiConnecting* var1, Minecraft* var2, const JavaString& var3, int var4)
	: mc(NULL)
	, port(0)
	, connectingGui(NULL)
{
	this->connectingGui = var1;
	this->mc = var2;
	this->hostName = var3;
	this->port = var4;
}

ThreadConnectToServer::~ThreadConnectToServer()
{
}

void ThreadConnectToServer::run()
{
	try {
	GuiConnecting::setNetClientHandler(this->connectingGui, new NetClientHandler(this->mc, this->hostName, this->port));
	if(GuiConnecting::isCancelled(this->connectingGui)) {
	return;
	}
	GuiConnecting::getNetClientHandler(this->connectingGui)->addToSendQueue(new Packet2Handshake(this->mc->session->inventory));
	} catch (UnknownHostException*) {
	if(GuiConnecting::isCancelled(this->connectingGui)) {
	return;
	}
	this->mc->displayGuiScreen(new GuiConnectFailed("Failed to connect to the server",(JCat() + "Unknown host \'" + this->hostName + "\'")));
	} catch (ConnectException* var3) {
	if(GuiConnecting::isCancelled(this->connectingGui)) {
	return;
	}
	this->mc->displayGuiScreen(new GuiConnectFailed("Failed to connect to the server", var3->getMessage()));
	} catch (Exception* var4) {
	if(GuiConnecting::isCancelled(this->connectingGui)) {
	return;
	}
	var4->printStackTrace();
	this->mc->displayGuiScreen(new GuiConnectFailed("Failed to connect to the server", var4->toString()));
	}
}

void ThreadConnectToServer::initStatics()
{
}
