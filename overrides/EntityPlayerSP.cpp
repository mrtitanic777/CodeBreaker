#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPlayerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayerSP.h"
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
#include "CraftingInventoryCB.h"
#include "CraftingInventoryWorkbenchCB.h"
#include "Cursor.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFX.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityPickupFX.h"
#include "EntityPlayer.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiChest.h"
#include "GuiCrafting.h"
#include "GuiEditSign.h"
#include "GuiFurnace.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryCrafting.h"
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
#include "Slot.h"
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
#include "TileEntityFurnace.h"
#include "TileEntitySign.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // field initializer on the EntityPlayerSP class:
 *  public int timeUntilPortal = 20;
 *  ---- end original Java ----
 *  The Java field initializer seeds timeUntilPortal to 20; here the init list seeds
 *  it to 0 instead (see timeUntilPortal(0) below), so the initial portal cooldown is lost. */
EntityPlayerSP::EntityPlayerSP(Minecraft* var1, World* var2, Session* var3, int var4)
	: EntityPlayer(var2)
	, movementInput(NULL)
	, mc(NULL)
	, timeUntilPortal(20)   // restored Java field initializer (was dropped: timeUntilPortal = 20)
	, inPortal(0)
	, timeInPortal(0)
	, prevTimeInPortal(0)
{
	this->mc = var1;
	this->dimension = var4;
	if(var3 != NULL && var3->inventory != NULL && var3->inventory.length() > 0) {
	this->skinUrl = (JCat() + "http://www.minecraft.net/skin/" + var3->inventory + ".png");
	System::out.println((JCat() + "Loading texture " + this->skinUrl));
	}
	this->field_771_i = var3->inventory;
}

EntityPlayerSP::~EntityPlayerSP()
{
}

void EntityPlayerSP::updateEntityActionState()
{
	EntityPlayer::updateEntityActionState();
	this->moveStrafing = this->movementInput->moveStrafe;
	this->moveForward = this->movementInput->moveForward;
	this->isJumping = this->movementInput->jump;
}

void EntityPlayerSP::onLivingUpdate()
{
	this->prevTimeInPortal = this->timeInPortal;
	if(this->inPortal) {
	if(this->timeInPortal == 0.0F) {
	this->mc->sndManager->playSoundFX("portal.trigger",1.0F,this->rand->nextFloat() * 0.4F + 0.8F);
	}
	this->timeInPortal += 0.0125F;
	if(this->timeInPortal >= 1.0F) {
	this->timeInPortal = 1.0F;
	this->timeUntilPortal = 10;
	this->mc->sndManager->playSoundFX("portal.travel",1.0F,this->rand->nextFloat() * 0.4F + 0.8F);
	this->mc->switchDimension();
	}
	this->inPortal = false;
	} else {
	if(this->timeInPortal > 0.0F) {
	this->timeInPortal -= 0.05F;
	}
	if(this->timeInPortal < 0.0F) {
	this->timeInPortal = 0.0F;
	}
	}
	if(this->timeUntilPortal > 0) {
	--this->timeUntilPortal;
	}
	this->movementInput->updatePlayerMoveState(this);
	if(this->movementInput->sneak && this->ySize < 0.2F) {
	this->ySize = 0.2F;
	}
	EntityPlayer::onLivingUpdate();
}

void EntityPlayerSP::resetMovementInput()
{
	this->movementInput->resetKeyState();
}

void EntityPlayerSP::setKeyState(int var1, bool var2)
{
	this->movementInput->setKeyState(var1, var2);
}

void EntityPlayerSP::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityPlayer::writeEntityToNBT(var1);
	var1->setInteger("Score", this->score);
}

void EntityPlayerSP::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityPlayer::readEntityFromNBT(var1);
	this->score = var1->getInteger("Score");
}

void EntityPlayerSP::displayGUIChest(IInventory* var1)
{
	this->mc->displayGuiScreen(new GuiChest(this->inventory, var1));
}

void EntityPlayerSP::displayGUIEditSign(TileEntitySign* var1)
{
	this->mc->displayGuiScreen(new GuiEditSign(var1));
}

void EntityPlayerSP::displayWorkbenchGUI()
{
	this->mc->displayGuiScreen(new GuiCrafting(this->inventory));
}

void EntityPlayerSP::displayGUIFurnace(TileEntityFurnace* var1)
{
	this->mc->displayGuiScreen(new GuiFurnace(this->inventory, var1));
}

void EntityPlayerSP::onItemPickup(Entity* var1, int var2)
{
	this->mc->effectRenderer->addEffect(new EntityPickupFX(this->mc->theWorld, var1, this, -0.5F));
}

int EntityPlayerSP::getPlayerArmorValue()
{
	return this->inventory->getTotalArmorValue();
}

void EntityPlayerSP::useCurrentItemOnEntity(Entity* var1)
{
	if(!var1->interact(this)) {
	ItemStack* var2 = this->getCurrentEquippedItem();
	if(var2 != NULL && (dynamic_cast<EntityLiving*>(var1) != NULL)) {
	var2->useItemOnEntity((EntityLiving*)var1);
	if(var2->stackSize <= 0) {
	var2->onItemDestroyedByUse(this);
	this->destroyCurrentEquippedItem();
	}
	}
	}
}

void EntityPlayerSP::sendChatMessage(const JavaString& var1)
{
}

void EntityPlayerSP::onUpdateClientPlayer()
{
}

bool EntityPlayerSP::isSneaking()
{
	return this->movementInput->sneak;
}

void EntityPlayerSP::setInPortal()
{
	if(this->timeUntilPortal > 0) {
	this->timeUntilPortal = 10;
	} else {
	this->inPortal = true;
	}
}

void EntityPlayerSP::setHealth(int var1)
{
	int var2 = this->health - var1;
	if(var2 <= 0) {
	this->health = var1;
	} else {
	this->lastDamage = var2;
	this->prevHealth = this->health;
	this->hurtResistantTime = this->maxHurtResistantTime;
	this->damageEntity(var2);
	this->hurtTime = this->maxHurtTime = 10;
	}
}

void EntityPlayerSP::respawnPlayer()
{
	this->mc->respawn();
}
