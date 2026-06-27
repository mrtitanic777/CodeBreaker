#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PlayerControllerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerControllerMP.h"
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
#include "EntityClientPlayerMP.h"
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
#include "Packet14BlockDig.h"
#include "Packet15Place.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet7.h"
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

PlayerControllerMP::PlayerControllerMP(Minecraft* var1, NetClientHandler* var2)
	: PlayerController(var1)
	, currentBlockX(0)
	, currentBlockY(0)
	, currentBlockZ(0)
	, curBlockDamageMP(0)
	, prevBlockDamageMP(0)
	, stepSoundTickCounter(0)
	, blockHitDelay(0)
	, isHittingBlock(0)
	, netClientHandler(NULL)
	, currentPlayerItem(0)
{
	this->currentBlockX = -1;
	this->currentBlockY = -1;
	this->currentBlockZ = -1;
	this->curBlockDamageMP = 0.0F;
	this->prevBlockDamageMP = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->blockHitDelay = 0;
	this->isHittingBlock = false;
	this->currentPlayerItem = 0;
	this->netClientHandler = var2;
}

PlayerControllerMP::~PlayerControllerMP()
{
}

void PlayerControllerMP::flipPlayer(EntityPlayer* var1)
{
	var1->rotationYaw = -180.0F;
}

bool PlayerControllerMP::sendBlockRemoved(int var1, int var2, int var3, int var4)
{
	this->netClientHandler->addToSendQueue(new Packet14BlockDig(3, var1, var2, var3, var4));
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	this->mc->theWorld->getBlockMetadata(var1, var2, var3);
	bool var7 = PlayerController::sendBlockRemoved(var1, var2, var3, var4);
	ItemStack* var8 = this->mc->thePlayer->getCurrentEquippedItem();
	if(var8 != NULL) {
	var8->hitBlock(var5, var1, var2, var3);
	if(var8->stackSize == 0) {
	var8->onItemDestroyedByUse(this->mc->thePlayer);
	this->mc->thePlayer->destroyCurrentEquippedItem();
	}
	}
	return var7;
}

void PlayerControllerMP::clickBlock(int var1, int var2, int var3, int var4)
{
	this->isHittingBlock = true;
	this->netClientHandler->addToSendQueue(new Packet14BlockDig(0, var1, var2, var3, var4));
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	if(var5 > 0 && this->curBlockDamageMP == 0.0F) {
	if (Block::s_blocksList[var5] != NULL) { Block::s_blocksList[var5]->onBlockClicked(this->mc->theWorld, var1, var2, var3, this->mc->thePlayer); }
	}
	if(var5 > 0 && Block::s_blocksList[var5]->getPlayerRelativeBlockHardness(this->mc->thePlayer) >= 1.0F) {
	this->sendBlockRemoved(var1, var2, var3, var4);
	}
}

void PlayerControllerMP::resetBlockRemoving()
{
	if(this->isHittingBlock) {
	this->isHittingBlock = false;
	this->netClientHandler->addToSendQueue(new Packet14BlockDig(2, 0, 0, 0, 0));
	this->curBlockDamageMP = 0.0F;
	this->blockHitDelay = 0;
	}
}

void PlayerControllerMP::sendBlockRemoving(int var1, int var2, int var3, int var4)
{
	this->isHittingBlock = true;
	this->syncCurrentPlayItem();
	this->netClientHandler->addToSendQueue(new Packet14BlockDig(1, var1, var2, var3, var4));
	if(this->blockHitDelay > 0) {
	--this->blockHitDelay;
	} else {
	if(var1 == this->currentBlockX && var2 == this->currentBlockY && var3 == this->currentBlockZ) {
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	if(var5 == 0) {
	return;
	}
	Block* var6 = Block::s_blocksList[var5];
	this->curBlockDamageMP += var6->getPlayerRelativeBlockHardness(this->mc->thePlayer);
	if(fmodf(this->stepSoundTickCounter, 4.0F) == 0.0F && var6 != NULL) {
	this->mc->sndManager->playSound(var6->stepSound->getStepSound(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->getVolume() + 1.0F) / 8.0F, var6->stepSound->getPitch() * 0.5F);
	}
	++this->stepSoundTickCounter;
	if(this->curBlockDamageMP >= 1.0F) {
	this->sendBlockRemoved(var1, var2, var3, var4);
	this->curBlockDamageMP = 0.0F;
	this->prevBlockDamageMP = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->blockHitDelay = 5;
	}
	} else {
	this->curBlockDamageMP = 0.0F;
	this->prevBlockDamageMP = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->currentBlockX = var1;
	this->currentBlockY = var2;
	this->currentBlockZ = var3;
	}
	}
}

void PlayerControllerMP::updateBlockHitOverlay(float var1)
{
	if(this->curBlockDamageMP <= 0.0F) {
	this->mc->ingameGUI->damagePartialTime = 0.0F;
	this->mc->renderGlobal->damagePartialTime = 0.0F;
	} else {
	float var2 = this->prevBlockDamageMP + (this->curBlockDamageMP - this->prevBlockDamageMP) * var1;
	this->mc->ingameGUI->damagePartialTime = var2;
	this->mc->renderGlobal->damagePartialTime = var2;
	}
}

float PlayerControllerMP::getBlockReachDistance()
{
	return 4.0F;
}

void PlayerControllerMP::addPendingConnection(World* var1)
{
	PlayerController::addPendingConnection(var1);
}

void PlayerControllerMP::resetBlockRemovingSound()
{
	this->syncCurrentPlayItem();
	this->prevBlockDamageMP = this->curBlockDamageMP;
	this->mc->sndManager->playRandomMusic();
}

void PlayerControllerMP::syncCurrentPlayItem()
{
	ItemStack* var1 = this->mc->thePlayer->inventory->getCurrentItem();
	int var2 = 0;
	if(var1 != NULL) {
	var2 = var1->itemID;
	}
	if(var2 != this->currentPlayerItem) {
	this->currentPlayerItem = var2;
	this->netClientHandler->addToSendQueue(new Packet16BlockItemSwitch(0, this->currentPlayerItem));
	}
}

bool PlayerControllerMP::sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7)
{
	this->syncCurrentPlayItem();
	this->netClientHandler->addToSendQueue(new Packet15Place(var3 != NULL ? var3->itemID : -1, var4, var5, var6, var7));
	return PlayerController::sendPlaceBlock(var1, var2, var3, var4, var5, var6, var7);
}

bool PlayerControllerMP::sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3)
{
	this->syncCurrentPlayItem();
	this->netClientHandler->addToSendQueue(new Packet15Place(var3 != NULL ? var3->itemID : -1, -1, -1, -1, 255));
	return PlayerController::sendUseItem(var1, var2, var3);
}

EntityPlayer* PlayerControllerMP::createPlayer(World* var1)
{
	return new EntityClientPlayerMP(this->mc, var1, this->mc->session, this->netClientHandler);
}

void PlayerControllerMP::attackEntity(EntityPlayer* var1, Entity* var2)
{
	this->syncCurrentPlayItem();
	this->netClientHandler->addToSendQueue(new Packet7(var1->entityId, var2->entityId, 1));
	var1->attackTargetEntityWithCurrentItem(var2);
}

void PlayerControllerMP::interactWithEntity(EntityPlayer* var1, Entity* var2)
{
	this->syncCurrentPlayItem();
	this->netClientHandler->addToSendQueue(new Packet7(var1->entityId, var2->entityId, 0));
	var1->useCurrentItemOnEntity(var2);
}

void PlayerControllerMP::initStatics()
{
}
