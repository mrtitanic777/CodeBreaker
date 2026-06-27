#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityClientPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityClientPlayerMP.h"
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
#include "MathHelper.h"
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
#include "Packet10Flying.h"
#include "Packet11PlayerPosition.h"
#include "Packet12PlayerLook.h"
#include "Packet13PlayerLookMove.h"
#include "Packet18ArmAnimation.h"
#include "Packet21PickupSpawn.h"
#include "Packet3Chat.h"
#include "Packet5PlayerInventory.h"
#include "Packet9.h"
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

EntityClientPlayerMP::EntityClientPlayerMP(Minecraft* var1, World* var2, Session* var3, NetClientHandler* var4)
	: EntityPlayerSP(var1, var2, var3, 0)
	, sendQueue(NULL)
	, inventorySyncTimer(0)
	, oldPosX(0)
	, oldMinY(0)
	, oldPosY(0)
	, oldPosZ(0)
	, oldRotationYaw(0)
	, oldRotationPitch(0)
	, lastSentInventory(NULL)
	, wasOnGround(0)
	, wasSwinging(0)
	, positionUpdateTicks(0)
{
	this->inventorySyncTimer = 0;
	this->lastSentInventory = new InventoryPlayer((EntityPlayer*)NULL);
	this->wasOnGround = false;
	this->wasSwinging = false;
	this->positionUpdateTicks = 0;
	this->sendQueue = var4;
}

EntityClientPlayerMP::~EntityClientPlayerMP()
{
}

bool EntityClientPlayerMP::canAttackEntity(Entity* var1, int var2)
{
	return false;
}

void EntityClientPlayerMP::heal(int var1)
{
}

void EntityClientPlayerMP::onUpdate()
{
	if(this->worldObj->blockExists(MathHelper::floor_double(this->posX), 64, MathHelper::floor_double(this->posZ))) {
	EntityPlayerSP::onUpdate();
	this->onUpdateWalkingPlayer();
	}
}

void EntityClientPlayerMP::onUpdateClientPlayer()
{
}

void EntityClientPlayerMP::onUpdateWalkingPlayer()
{
	if(this->inventorySyncTimer++ == 20) {
	this->sendInventoryChanged();
	this->inventorySyncTimer = 0;
	}
	bool var1 = this->isSneaking();
	if(var1 != this->wasSwinging) {
	if(var1) {
	this->sendQueue->addToSendQueue(new Packet18ArmAnimation(this, 104));
	} else {
	this->sendQueue->addToSendQueue(new Packet18ArmAnimation(this, 105));
	}
	this->wasSwinging = var1;
	}
	double var2 = this->posX - this->oldPosX;
	double var4 = this->boundingBox->minY - this->oldMinY;
	double var6 = this->posY - this->oldPosY;
	double var8 = this->posZ - this->oldPosZ;
	double var10 = (double)(this->rotationYaw - this->oldRotationYaw);
	double var12 = (double)(this->rotationPitch - this->oldRotationPitch);
	bool var14 = var4 != 0.0 || var6 != 0.0 || var2 != 0.0 || var8 != 0.0;
	bool var15 = var10 != 0.0 || var12 != 0.0;
	if(this->ridingEntity != NULL) {
	if(var15) {
	this->sendQueue->addToSendQueue(new Packet11PlayerPosition(this->motionX, -999.0, -999.0, this->motionZ, this->onGround));
	} else {
	this->sendQueue->addToSendQueue(new Packet13PlayerLookMove(this->motionX, -999.0, -999.0, this->motionZ, this->rotationYaw, this->rotationPitch, this->onGround));
	}
	var14 = false;
	} else if(var14 && var15) {
	this->sendQueue->addToSendQueue(new Packet13PlayerLookMove(this->posX, this->boundingBox->minY, this->posY, this->posZ, this->rotationYaw, this->rotationPitch, this->onGround));
	this->positionUpdateTicks = 0;
	} else if(var14) {
	this->sendQueue->addToSendQueue(new Packet11PlayerPosition(this->posX, this->boundingBox->minY, this->posY, this->posZ, this->onGround));
	this->positionUpdateTicks = 0;
	} else if(var15) {
	this->sendQueue->addToSendQueue(new Packet12PlayerLook(this->rotationYaw, this->rotationPitch, this->onGround));
	this->positionUpdateTicks = 0;
	} else {
	this->sendQueue->addToSendQueue(new Packet10Flying(this->onGround));
	if(this->wasOnGround == this->onGround && this->positionUpdateTicks <= 20) {
	++this->positionUpdateTicks;
	} else {
	this->positionUpdateTicks = 0;
	}
	}
	this->wasOnGround = this->onGround;
	if(var14) {
	this->oldPosX = this->posX;
	this->oldMinY = this->boundingBox->minY;
	this->oldPosY = this->posY;
	this->oldPosZ = this->posZ;
	}
	if(var15) {
	this->oldRotationYaw = this->rotationYaw;
	this->oldRotationPitch = this->rotationPitch;
	}
}

void EntityClientPlayerMP::sendInventoryChanged()
{
	if(!this->inventory->compareInventory(this->lastSentInventory)) {
	this->sendQueue->addToSendQueue(new Packet5PlayerInventory(-1, this->inventory->mainInventory));
	this->sendQueue->addToSendQueue(new Packet5PlayerInventory(-2, this->inventory->craftingInventory));
	this->sendQueue->addToSendQueue(new Packet5PlayerInventory(-3, this->inventory->armorInventory));
	this->lastSentInventory = this->inventory->copyInventory();
	}
}

void EntityClientPlayerMP::joinEntityItemWithWorld(EntityItem* var1)
{
	Packet21PickupSpawn* var2 = new Packet21PickupSpawn(var1);
	this->sendQueue->addToSendQueue(var2);
	var1->posX = (double)var2->xPosition / 32.0;
	var1->posY = (double)var2->yPosition / 32.0;
	var1->posZ = (double)var2->zPosition / 32.0;
	var1->motionX = (double)var2->rotation / 128.0;
	var1->motionY = (double)var2->pitch / 128.0;
	var1->motionZ = (double)var2->roll / 128.0;
}

void EntityClientPlayerMP::sendChatMessage(const JavaString& var1)
{
	this->sendQueue->addToSendQueue(new Packet3Chat(var1));
}

void EntityClientPlayerMP::swingItem()
{
	EntityPlayerSP::swingItem();
	this->sendQueue->addToSendQueue(new Packet18ArmAnimation(this, 1));
}

void EntityClientPlayerMP::respawnPlayer()
{
	this->sendInventoryChanged();
	this->sendQueue->addToSendQueue(new Packet9());
}

void EntityClientPlayerMP::damageEntity(int var1)
{
	this->health -= var1;
}

void EntityClientPlayerMP::initStatics()
{
}
