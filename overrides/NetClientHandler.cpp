#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetClientHandler.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetClientHandler.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "BufferedReader.h"
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
#include "EntityArrow.h"
#include "EntityBoat.h"
#include "EntityFX.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityList.h"
#include "EntityLiving.h"
#include "EntityMinecart.h"
#include "EntityOtherPlayerMP.h"
#include "EntityPickupFX.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EntitySnowball.h"
#include "EntityTNTPrimed.h"
#include "EnumOS2.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiConnectFailed.h"
#include "GuiDownloadTerrain.h"
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
#include "NetHandler.h"
#include "NetworkManager.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OpenGlCapsChecker.h"
#include "Packet.h"
#include "Packet10Flying.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet17AddToInventory.h"
#include "Packet18ArmAnimation.h"
#include "Packet1Login.h"
#include "Packet20NamedEntitySpawn.h"
#include "Packet21PickupSpawn.h"
#include "Packet22Collect.h"
#include "Packet23VehicleSpawn.h"
#include "Packet24MobSpawn.h"
#include "Packet255KickDisconnect.h"
#include "Packet28.h"
#include "Packet29DestroyEntity.h"
#include "Packet2Handshake.h"
#include "Packet30Entity.h"
#include "Packet34EntityTeleport.h"
#include "Packet38.h"
#include "Packet39.h"
#include "Packet3Chat.h"
#include "Packet4UpdateTime.h"
#include "Packet50PreChunk.h"
#include "Packet51MapChunk.h"
#include "Packet52MultiBlockChange.h"
#include "Packet53BlockChange.h"
#include "Packet59ComplexEntity.h"
#include "Packet5PlayerInventory.h"
#include "Packet60.h"
#include "Packet6SpawnPosition.h"
#include "Packet8.h"
#include "Packet9.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "PlayerController.h"
#include "PlayerControllerMP.h"
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

// PORT: local shims for java.net.InetAddress and java.io.InputStreamReader, which have no
// runtime stub header (see "runtime-stub gaps" in report). Minimal varargs surface to match
// how the surrounding stub classes (Socket, URL, BufferedReader) are modeled.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // java.net.InetAddress.getByName performs a real DNS lookup; the shim getByName(...) returns 0.
 *  // java.io.InputStreamReader is a real decoding reader; the shim is an empty placeholder.
 *  import java.io.InputStreamReader;
 *  import java.net.InetAddress;
 *
 *  public NetClientHandler(Minecraft var1, String var2, int var3) throws IOException, UnknownHostException {
 *     this.mc = var1;
 *     Socket var4 = new Socket(InetAddress.getByName(var2), var3);
 *     this.netManager = new NetworkManager(var4, "Client", this);
 *  }
 *
 *  // ... and inside handleHandshake(...):
 *     BufferedReader var3 = new BufferedReader(new InputStreamReader(var2.openStream()));
 *  ---- end original Java ---- */
namespace {
	struct _InetAddressClass {
		void* getByName(...) { return 0; }
	} InetAddress;
	struct InputStreamReader {
		InputStreamReader(...) {}
	};
}

NetClientHandler::NetClientHandler(Minecraft* var1, const JavaString& var2, int var3)
	: disconnected(0)
	, netManager(NULL)
	, mc(NULL)
	, worldClient(NULL)
	, receivedFirstUpdate(0)
	, rand(NULL)
{
	this->rand = new Random();

	this->mc = var1;
	Socket* var4 = new Socket(InetAddress.getByName(var2), var3);
	this->netManager = new NetworkManager(var4, "Client", this);
}

NetClientHandler::~NetClientHandler()
{
	delete netManager;
	netManager = NULL;
}

void NetClientHandler::processReadPackets()
{
	if(!this->disconnected) {
	this->netManager->processReadPackets();
	}
}

void NetClientHandler::handleLogin(Packet1Login* var1)
{
	this->mc->playerController = new PlayerControllerMP(this->mc, this);
	this->worldClient = new WorldClient(this, var1->mapSeed, var1->dimension);
	this->worldClient->multiplayerWorld = true;
	this->mc->setWorld(this->worldClient);
	this->mc->displayGuiScreen(new GuiDownloadTerrain(this));
	this->mc->thePlayer->entityId = var1->protocolVersion;
	System::out.println((JCat() + "clientEntityId: " + var1->protocolVersion));
}

void NetClientHandler::handlePickupSpawn(Packet21PickupSpawn* var1)
{
	double var2 = (double)var1->xPosition / 32.0;
	double var4 = (double)var1->yPosition / 32.0;
	double var6 = (double)var1->zPosition / 32.0;
	EntityItem* var8 = new EntityItem(this->worldClient, var2, var4, var6, new ItemStack(var1->itemId, var1->count));
	var8->motionX = (double)var1->rotation / 128.0;
	var8->motionY = (double)var1->pitch / 128.0;
	var8->motionZ = (double)var1->roll / 128.0;
	var8->serverPosX = var1->xPosition;
	var8->serverPosY = var1->yPosition;
	var8->serverPosZ = var1->zPosition;
	this->worldClient->addEntityToWorld(var1->entityId, var8);
}

void NetClientHandler::handleVehicleSpawn(Packet23VehicleSpawn* var1)
{
	double var2 = (double)var1->xPosition / 32.0;
	double var4 = (double)var1->yPosition / 32.0;
	double var6 = (double)var1->zPosition / 32.0;
	void* var8 = NULL;
	if(var1->type == 10) {
	var8 = new EntityMinecart(this->worldClient, var2, var4, var6, 0);
	}
	if(var1->type == 11) {
	var8 = new EntityMinecart(this->worldClient, var2, var4, var6, 1);
	}
	if(var1->type == 12) {
	var8 = new EntityMinecart(this->worldClient, var2, var4, var6, 2);
	}
	if(var1->type == 90) {
	var8 = new EntityFish(this->worldClient, var2, var4, var6);
	}
	if(var1->type == 60) {
	var8 = new EntityArrow(this->worldClient, var2, var4, var6);
	}
	if(var1->type == 61) {
	var8 = new EntitySnowball(this->worldClient, var2, var4, var6);
	}
	if(var1->type == 1) {
	var8 = new EntityBoat(this->worldClient, var2, var4, var6);
	}
	if(var1->type == 50) {
	var8 = new EntityTNTPrimed(this->worldClient, var2, var4, var6);
	}
	if(var8 != NULL) {
	((Entity*)var8)->serverPosX = var1->xPosition;
	((Entity*)var8)->serverPosY = var1->yPosition;
	((Entity*)var8)->serverPosZ = var1->zPosition;
	((Entity*)var8)->rotationYaw = 0.0F;
	((Entity*)var8)->rotationPitch = 0.0F;
	((Entity*)var8)->entityId = var1->entityId;
	this->worldClient->addEntityToWorld(var1->entityId, (Entity*)var8);
	}
}

void NetClientHandler::handleEntityVelocity(Packet28* var1)
{
	Entity* var2 = this->getEntityByID(var1->entityId);
	if(var2 != NULL) {
	var2->setVelocity((double)var1->velocityX / 8000.0, (double)var1->velocityY / 8000.0, (double)var1->velocityZ / 8000.0);
	}
}

void NetClientHandler::handleNamedEntitySpawn(Packet20NamedEntitySpawn* var1)
{
	double var2 = (double)var1->xPosition / 32.0;
	double var4 = (double)var1->yPosition / 32.0;
	double var6 = (double)var1->zPosition / 32.0;
	float var8 = (float)(var1->rotation * 360) / 256.0F;
	float var9 = (float)(var1->pitch * 360) / 256.0F;
	EntityOtherPlayerMP* var10 = new EntityOtherPlayerMP(this->mc->theWorld, var1->name);
	var10->serverPosX = var1->xPosition;
	var10->serverPosY = var1->yPosition;
	var10->serverPosZ = var1->zPosition;
	int var11 = var1->currentItem;
	if(var11 == 0) {
	var10->inventory->mainInventory[var10->inventory->currentItem] = NULL;
	} else {
	var10->inventory->mainInventory[var10->inventory->currentItem] = new ItemStack(var11);
	}
	var10->setPositionAndRotation(var2, var4, var6, var8, var9);
	this->worldClient->addEntityToWorld(var1->entityId, var10);
}

void NetClientHandler::handleEntityTeleport(Packet34EntityTeleport* var1)
{
	Entity* var2 = this->getEntityByID(var1->entityId);
	if(var2 != NULL) {
	var2->serverPosX = var1->xPosition;
	var2->serverPosY = var1->yPosition;
	var2->serverPosZ = var1->zPosition;
	double var3 = (double)var2->serverPosX / 32.0;
	double var5 = (double)var2->serverPosY / 32.0 + 1.0 / 64.0;
	double var7 = (double)var2->serverPosZ / 32.0;
	float var9 = (float)(var1->yaw * 360) / 256.0F;
	float var10 = (float)(var1->pitch * 360) / 256.0F;
	var2->setPositionAndRotation2(var3, var5, var7, var9, var10, 3);
	}
}

void NetClientHandler::handleEntity(Packet30Entity* var1)
{
	Entity* var2 = this->getEntityByID(var1->entityId);
	if(var2 != NULL) {
	var2->serverPosX += var1->xPosition;
	var2->serverPosY += var1->yPosition;
	var2->serverPosZ += var1->zPosition;
	double var3 = (double)var2->serverPosX / 32.0;
	double var5 = (double)var2->serverPosY / 32.0 + 1.0 / 64.0;
	double var7 = (double)var2->serverPosZ / 32.0;
	float var9 = var1->rotating ? (float)(var1->yaw * 360) / 256.0F : var2->rotationYaw;
	float var10 = var1->rotating ? (float)(var1->pitch * 360) / 256.0F : var2->rotationPitch;
	var2->setPositionAndRotation2(var3, var5, var7, var9, var10, 3);
	}
}

void NetClientHandler::handleDestroyEntity(Packet29DestroyEntity* var1)
{
	this->worldClient->removeEntityFromWorld(var1->entityId);
}

void NetClientHandler::handleFlying(Packet10Flying* var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	double var3 = var2->posX;
	double var5 = var2->posY;
	double var7 = var2->posZ;
	float var9 = var2->rotationYaw;
	float var10 = var2->rotationPitch;
	if(var1->moving) {
	var3 = var1->xPosition;
	var5 = var1->yPosition;
	var7 = var1->zPosition;
	}
	if(var1->rotating) {
	var9 = var1->yaw;
	var10 = var1->pitch;
	}
	var2->ySize = 0.0F;
	var2->motionX = var2->motionY = var2->motionZ = 0.0;
	var2->setPositionAndRotation(var3, var5, var7, var9, var10);
	var1->xPosition = var2->posX;
	var1->yPosition = var2->boundingBox->minY;
	var1->zPosition = var2->posZ;
	var1->stance = var2->posY;
	this->netManager->addToSendQueue(var1);
	if(!this->receivedFirstUpdate) {
	this->mc->thePlayer->prevPosX = this->mc->thePlayer->posX;
	this->mc->thePlayer->prevPosY = this->mc->thePlayer->posY;
	this->mc->thePlayer->prevPosZ = this->mc->thePlayer->posZ;
	this->receivedFirstUpdate = true;
	this->mc->displayGuiScreen((GuiScreen*)NULL);
	}
}

void NetClientHandler::handlePreChunk(Packet50PreChunk* var1)
{
	this->worldClient->getServerSocket(var1->xPosition, var1->yPosition, var1->mode);
}

void NetClientHandler::handleMultiBlockChange(Packet52MultiBlockChange* var1)
{
	Chunk* var2 = this->worldClient->getChunkFromChunkCoords(var1->xPosition, var1->zPosition);
	int var3 = var1->xPosition * 16;
	int var4 = var1->zPosition * 16;
	for(int var5 = 0; var5 < var1->size; ++var5) {
	short var6 = var1->coordinateArray[var5];
	int var7 = var1->typeArray[var5] & 255;
	unsigned char var8 = var1->metadataArray[var5];
	int var9 = (var6 >> 12) & 15;
	int var10 = (var6 >> 8) & 15;
	int var11 = var6 & 255;
	var2->setBlockIDWithMetadata(var9, var11, var10, var7, var8);
	this->worldClient->invalidateBlockReceiveRegion(var9 + var3, var11, var10 + var4, var9 + var3, var11, var10 + var4);
	this->worldClient->markBlockRangeNeedsUpdate(var9 + var3, var11, var10 + var4, var9 + var3, var11, var10 + var4);
	}
}

void NetClientHandler::handleMapChunk(Packet51MapChunk* var1)
{
	this->worldClient->invalidateBlockReceiveRegion(var1->xPosition, var1->yPosition, var1->zPosition, var1->xPosition + var1->xSize - 1, var1->yPosition + var1->ySize - 1, var1->zPosition + var1->zSize - 1);
	this->worldClient->setChunkData(var1->xPosition, var1->yPosition, var1->zPosition, var1->xSize, var1->ySize, var1->zSize, var1->chunk);
}

void NetClientHandler::handleBlockChange(Packet53BlockChange* var1)
{
	this->worldClient->setBlockAndMetadataNoQueue(var1->xPosition, var1->yPosition, var1->zPosition, var1->type, var1->metadata);
}

void NetClientHandler::handleKickDisconnect(Packet255KickDisconnect* var1)
{
	this->netManager->networkShutdown("Got kicked");
	this->disconnected = true;
	this->mc->setWorld((World*)NULL);
	this->mc->displayGuiScreen(new GuiConnectFailed("Disconnected by server", var1->reason));
}

void NetClientHandler::handleErrorMessage(const JavaString& var1)
{
	if(!this->disconnected) {
	this->disconnected = true;
	this->mc->setWorld((World*)NULL);
	this->mc->displayGuiScreen(new GuiConnectFailed("Connection lost", var1));
	}
}

void NetClientHandler::addToSendQueue(Packet* var1)
{
	if(!this->disconnected) {
	this->netManager->addToSendQueue(var1);
	}
}

void NetClientHandler::handleCollect(Packet22Collect* var1)
{
	Entity* var2 = this->getEntityByID(var1->collectedEntityId);
	void* var3 = (EntityLiving*)this->getEntityByID(var1->collectorEntityId);
	if(var3 == NULL) {
	var3 = this->mc->thePlayer;
	}
	if(var2 != NULL) {
	this->worldClient->playSoundAtEntity(var2,"random.pop",0.2F,((this->rand->nextFloat() - this->rand->nextFloat()) * 0.7F + 1.0F) * 2.0F);
	this->mc->effectRenderer->addEffect(new EntityPickupFX(this->mc->theWorld, var2, (Entity*)var3, -0.5F));
	this->worldClient->removeEntityFromWorld(var1->collectedEntityId);
	}
}

void NetClientHandler::handleBlockItemSwitch(Packet16BlockItemSwitch* var1)
{
	Entity* var2 = this->getEntityByID(var1->unused);
	if(var2 != NULL) {
	EntityPlayer* var3 = (EntityPlayer*)var2;
	int var4 = var1->id;
	if(var4 == 0) {
	var3->inventory->mainInventory[var3->inventory->currentItem] = NULL;
	} else {
	var3->inventory->mainInventory[var3->inventory->currentItem] = new ItemStack(var4);
	}
	}
}

void NetClientHandler::handleChat(Packet3Chat* var1)
{
	this->mc->ingameGUI->addChatMessage(var1->message);
}

void NetClientHandler::handleArmAnimation(Packet18ArmAnimation* var1)
{
	Entity* var2 = this->getEntityByID(var1->entityId);
	if(var2 != NULL) {
	if(var1->animate == 1) {
	EntityPlayer* var3 = (EntityPlayer*)var2;
	var3->swingItem();
	} else if(var1->animate == 100) {
	var2->field_9300_bu = true;
	} else if(var1->animate == 101) {
	var2->field_9300_bu = false;
	} else if(var1->animate == 102) {
	var2->field_9299_bv = true;
	} else if(var1->animate == 103) {
	var2->field_9299_bv = false;
	} else if(var1->animate == 104) {
	var2->isSneakingRender = true;
	} else if(var1->animate == 105) {
	var2->isSneakingRender = false;
	} else if(var1->animate == 2) {
	var2->performHurtAnimation();
	}
	}
}

void NetClientHandler::handleAddToInventory(Packet17AddToInventory* var1)
{
	this->mc->thePlayer->inventory->addItemStackToInventory(new ItemStack(var1->id, var1->count, var1->durability));
}

void NetClientHandler::handleHandshake(Packet2Handshake* var1)
{
	if(var1->username.equals("-")) {
	this->addToSendQueue(new Packet1Login(this->mc->session->inventory, "Password", 6));
	} else {
	try {
	JavaString _urlStr = (JCat() + "http://www.minecraft.net/game/joinserver.jsp?user=" + this->mc->session->inventory + "&sessionId=" + this->mc->session->sessionId + "&serverId=" + var1->username);
	URL* var2 = new URL(_urlStr);
	BufferedReader* var3 = new BufferedReader(new InputStreamReader(var2->openStream()));
	JavaString var4 = var3->readLine();
	var3->close();
	if(var4.equalsIgnoreCase("ok")) {
	this->addToSendQueue(new Packet1Login(this->mc->session->inventory, "Password", 6));
	} else {
	this->netManager->networkShutdown((JCat() + "Failed to login: " + var4));
	}
	} catch (Exception var5) {
	var5.printStackTrace();
	this->netManager->networkShutdown((JCat() + "Internal client error: " + var5.toString()));
	}
	}
}

void NetClientHandler::disconnect()
{
	this->disconnected = true;
	this->netManager->networkShutdown("Closed");
}

void NetClientHandler::handleMobSpawn(Packet24MobSpawn* var1)
{
	double var2 = (double)var1->xPosition / 32.0;
	double var4 = (double)var1->yPosition / 32.0;
	double var6 = (double)var1->zPosition / 32.0;
	float var8 = (float)(var1->yaw * 360) / 256.0F;
	float var9 = (float)(var1->pitch * 360) / 256.0F;
	EntityLiving* var10 = (EntityLiving*)EntityList::createEntity(var1->type, this->mc->theWorld);
	var10->serverPosX = var1->xPosition;
	var10->serverPosY = var1->yPosition;
	var10->serverPosZ = var1->zPosition;
	var10->entityId = var1->entityId;
	var10->setPositionAndRotation(var2, var4, var6, var8, var9);
	var10->noAi = true;
	this->worldClient->addEntityToWorld(var1->entityId, var10);
}

void NetClientHandler::handleUpdateTime(Packet4UpdateTime* var1)
{
	this->mc->theWorld->setWorldTime(var1->time);
}

void NetClientHandler::handlePlayerInventory(Packet5PlayerInventory* var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	if(var1->type == -1) {
	var2->inventory->mainInventory = var1->stacks;
	}
	if(var1->type == -2) {
	var2->inventory->craftingInventory = var1->stacks;
	}
	if(var1->type == -3) {
	var2->inventory->armorInventory = var1->stacks;
	}
}

void NetClientHandler::handleComplexEntity(Packet59ComplexEntity* var1)
{
	if(var1->entityNBT->getInteger("x") == var1->xPosition) {
	if(var1->entityNBT->getInteger("y") == var1->yPosition) {
	if(var1->entityNBT->getInteger("z") == var1->zPosition) {
	TileEntity* var2 = this->worldClient->getBlockTileEntity(var1->xPosition, var1->yPosition, var1->zPosition);
	if(var2 != NULL) {
	try {
	var2->readFromNBT(var1->entityNBT);
	} catch (Exception) {
	}
	this->worldClient->markBlockRangeNeedsUpdate(var1->xPosition, var1->yPosition, var1->zPosition, var1->xPosition, var1->yPosition, var1->zPosition);
	}
	}
	}
	}
}

void NetClientHandler::handleSpawnPosition(Packet6SpawnPosition* var1)
{
	this->worldClient->spawnX = var1->xPosition;
	this->worldClient->spawnY = var1->yPosition;
	this->worldClient->spawnZ = var1->zPosition;
}

void NetClientHandler::handleAttachEntity(Packet39* var1)
{
	void* var2 = this->getEntityByID(var1->entityId);
	Entity* var3 = this->getEntityByID(var1->vehicleEntityId);
	if(var1->entityId == this->mc->thePlayer->entityId) {
	var2 = this->mc->thePlayer;
	}
	if(var2 != NULL) {
	((Entity*)var2)->mountEntity(var3);
	}
}

void NetClientHandler::handleEntityStatus(Packet38* var1)
{
	Entity* var2 = this->getEntityByID(var1->entityId);
	if(var2 != NULL) {
	var2->handleHealthUpdate(var1->entityStatus);
	}
}

Entity* NetClientHandler::getEntityByID(int var1)
{
	return (Entity*)(var1 == this->mc->thePlayer->entityId ? this->mc->thePlayer : this->worldClient->getEntityByID(var1));
}

void NetClientHandler::handleHealth(Packet8* var1)
{
	this->mc->thePlayer->setHealth(var1->healthMP);
}

void NetClientHandler::handleRespawn(Packet9* var1)
{
	this->mc->respawn();
}

void NetClientHandler::handleExplosion(Packet60* var1)
{
	Explosion* var2 = new Explosion(this->mc->theWorld, (Entity*)NULL, var1->explosionX, var1->explosionY, var1->explosionZ, var1->explosionSize);
	var2->destroyedBlockPositions = var1->destroyedBlockPositions;
	var2->doExplosionB();
}
