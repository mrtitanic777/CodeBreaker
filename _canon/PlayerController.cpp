#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PlayerController.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"
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

PlayerController::PlayerController(Minecraft* var1)
	: mc(NULL)
	, isCreativeMode(0)
{
	this->isCreativeMode = false;
	this->mc = var1;
}

PlayerController::~PlayerController()
{
}

void PlayerController::addPendingConnection(World* var1)
{
}

void PlayerController::clickBlock(int var1, int var2, int var3, int var4)
{
	this->sendBlockRemoved(var1, var2, var3, var4);
}

bool PlayerController::sendBlockRemoved(int var1, int var2, int var3, int var4)
{
	this->mc->effectRenderer->addBlockDestroyEffects(var1, var2, var3);
	World* var5 = this->mc->theWorld;
	Block* var6 = Block::s_blocksList[var5->getBlockId(var1, var2, var3)];
	int var7 = var5->getBlockMetadata(var1, var2, var3);
	bool var8 = var5->setBlockWithNotify(var1, var2, var3, 0);
	if(var6 != NULL && var8) {
	this->mc->sndManager->playSound(var6->stepSound->getBreakSound(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->getVolume() + 1.0F) / 2.0F, var6->stepSound->getPitch() * 0.8F);
	var6->onBlockDestroyedByPlayer(var5, var1, var2, var3, var7);
	}
	return var8;
}

void PlayerController::sendBlockRemoving(int var1, int var2, int var3, int var4)
{
}

void PlayerController::resetBlockRemoving()
{
}

void PlayerController::updateBlockHitOverlay(float var1)
{
}

float PlayerController::getBlockReachDistance()
{
	return 5.0F;
}

bool PlayerController::sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3)
{
	int var4 = var3->stackSize;
	ItemStack* var5 = var3->useItemRightClick(var2, var1);
	if(var5 != var3 || var5 != NULL && var5->stackSize != var4) {
	var1->inventory->mainInventory[var1->inventory->currentItem] = var5;
	if(var5->stackSize == 0) {
	var1->inventory->mainInventory[var1->inventory->currentItem] = NULL;
	}
	return true;
	} else {
	return false;
	}
}

void PlayerController::flipPlayer(EntityPlayer* var1)
{
}

void PlayerController::resetBlockRemovingSound()
{
}

bool PlayerController::shouldDrawHealthBar()
{
	return true;
}

void PlayerController::onPlayerCreated(EntityPlayer* var1)
{
}

bool PlayerController::sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7)
{
	int var8 = var2->getBlockId(var4, var5, var6);
	return var8 > 0 && Block::s_blocksList[var8]->blockActivated(var2, var4, var5, var6, var1) ? true : (var3 == NULL ? false : var3->useItem(var1, var2, var4, var5, var6, var7));
}

EntityPlayer* PlayerController::createPlayer(World* var1)
{
	return new EntityPlayerSP(this->mc, var1, this->mc->session, var1->worldProvider->dimensionId);
}

void PlayerController::interactWithEntity(EntityPlayer* var1, Entity* var2)
{
	var1->useCurrentItemOnEntity(var2);
}

void PlayerController::attackEntity(EntityPlayer* var1, Entity* var2)
{
	var1->attackTargetEntityWithCurrentItem(var2);
}

void PlayerController::initStatics()
{
}
