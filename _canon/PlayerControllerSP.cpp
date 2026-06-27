#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PlayerControllerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerControllerSP.h"
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

PlayerControllerSP::PlayerControllerSP(Minecraft* var1)
	: PlayerController(var1)
	, currentBlockX(0)
	, currentBlockY(0)
	, currentBlockZ(0)
	, curBlockDamage(0)
	, prevBlockDamage(0)
	, stepSoundTickCounter(0)
	, blockHitDelay(0)
{
	this->currentBlockX = -1;
	this->currentBlockY = -1;
	this->currentBlockZ = -1;
	this->curBlockDamage = 0.0F;
	this->prevBlockDamage = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->blockHitDelay = 0;
}

PlayerControllerSP::~PlayerControllerSP()
{
}

void PlayerControllerSP::flipPlayer(EntityPlayer* var1)
{
	var1->rotationYaw = -180.0F;
}

bool PlayerControllerSP::sendBlockRemoved(int var1, int var2, int var3, int var4)
{
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	int var6 = this->mc->theWorld->getBlockMetadata(var1, var2, var3);
	bool var7 = PlayerController::sendBlockRemoved(var1, var2, var3, var4);
	ItemStack* var8 = this->mc->thePlayer->getCurrentEquippedItem();
	bool var9 = this->mc->thePlayer->canHarvestBlock(Block::s_blocksList[var5]);
	if(var8 != NULL) {
	var8->hitBlock(var5, var1, var2, var3);
	if(var8->stackSize == 0) {
	var8->onItemDestroyedByUse(this->mc->thePlayer);
	this->mc->thePlayer->destroyCurrentEquippedItem();
	}
	}
	if(var7 && var9) {
	if (Block::s_blocksList[var5] != NULL) { Block::s_blocksList[var5]->harvestBlock(this->mc->theWorld, var1, var2, var3, var6); }
	}
	return var7;
}

void PlayerControllerSP::clickBlock(int var1, int var2, int var3, int var4)
{
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	if(var5 > 0 && this->curBlockDamage == 0.0F) {
	if (Block::s_blocksList[var5] != NULL) { Block::s_blocksList[var5]->onBlockClicked(this->mc->theWorld, var1, var2, var3, this->mc->thePlayer); }
	}
	if(var5 > 0 && Block::s_blocksList[var5]->getPlayerRelativeBlockHardness(this->mc->thePlayer) >= 1.0F) {
	this->sendBlockRemoved(var1, var2, var3, var4);
	}
}

void PlayerControllerSP::resetBlockRemoving()
{
	this->curBlockDamage = 0.0F;
	this->blockHitDelay = 0;
}

void PlayerControllerSP::sendBlockRemoving(int var1, int var2, int var3, int var4)
{
	if(this->blockHitDelay > 0) {
	--this->blockHitDelay;
	} else {
	if(var1 == this->currentBlockX && var2 == this->currentBlockY && var3 == this->currentBlockZ) {
	int var5 = this->mc->theWorld->getBlockId(var1, var2, var3);
	if(var5 == 0) {
	return;
	}
	Block* var6 = Block::s_blocksList[var5];
	this->curBlockDamage += var6->getPlayerRelativeBlockHardness(this->mc->thePlayer);
	if(fmodf(this->stepSoundTickCounter, 4.0F) == 0.0F && var6 != NULL) {
	this->mc->sndManager->playSound(var6->stepSound->getStepSound(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->getVolume() + 1.0F) / 8.0F, var6->stepSound->getPitch() * 0.5F);
	}
	++this->stepSoundTickCounter;
	if(this->curBlockDamage >= 1.0F) {
	this->sendBlockRemoved(var1, var2, var3, var4);
	this->curBlockDamage = 0.0F;
	this->prevBlockDamage = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->blockHitDelay = 5;
	}
	} else {
	this->curBlockDamage = 0.0F;
	this->prevBlockDamage = 0.0F;
	this->stepSoundTickCounter = 0.0F;
	this->currentBlockX = var1;
	this->currentBlockY = var2;
	this->currentBlockZ = var3;
	}
	}
}

void PlayerControllerSP::updateBlockHitOverlay(float var1)
{
	if(this->curBlockDamage <= 0.0F) {
	this->mc->ingameGUI->damagePartialTime = 0.0F;
	this->mc->renderGlobal->damagePartialTime = 0.0F;
	} else {
	float var2 = this->prevBlockDamage + (this->curBlockDamage - this->prevBlockDamage) * var1;
	this->mc->ingameGUI->damagePartialTime = var2;
	this->mc->renderGlobal->damagePartialTime = var2;
	}
}

float PlayerControllerSP::getBlockReachDistance()
{
	return 4.0F;
}

void PlayerControllerSP::addPendingConnection(World* var1)
{
	PlayerController::addPendingConnection(var1);
}

void PlayerControllerSP::resetBlockRemovingSound()
{
	this->prevBlockDamage = this->curBlockDamage;
	this->mc->sndManager->playRandomMusic();
}

void PlayerControllerSP::initStatics()
{
}
