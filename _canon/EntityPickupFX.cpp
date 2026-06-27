#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPickupFX.h"
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
#include "EntityFX.h"
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
#include "GL11.h"
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

EntityPickupFX::EntityPickupFX(World* var1, Entity* var2, Entity* var3, float var4)
	: EntityFX(var1, var2->posX, var2->posY, var2->posZ, var2->motionX, var2->motionY, var2->motionZ)
	, thePlayer(NULL)
	, targetEntity(NULL)
	, age(0)
	, maxAge(0)
	, targetYOffset(0)
{
	this->age = 0;
	this->maxAge = 0;
	this->thePlayer = var2;
	this->targetEntity = var3;
	this->maxAge = 3;
	this->targetYOffset = var4;
}

EntityPickupFX::~EntityPickupFX()
{
}

void EntityPickupFX::renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
	float var8 = ((float)this->age + var2) / (float)this->maxAge;
	var8 *= var8;
	double var9 = this->thePlayer->posX;
	double var11 = this->thePlayer->posY;
	double var13 = this->thePlayer->posZ;
	double var15 = this->targetEntity->lastTickPosX + (this->targetEntity->posX - this->targetEntity->lastTickPosX) * (double)var2;
	double var17 = this->targetEntity->lastTickPosY + (this->targetEntity->posY - this->targetEntity->lastTickPosY) * (double)var2 + (double)this->targetYOffset;
	double var19 = this->targetEntity->lastTickPosZ + (this->targetEntity->posZ - this->targetEntity->lastTickPosZ) * (double)var2;
	double var21 = var9 + (var15 - var9) * (double)var8;
	double var23 = var11 + (var17 - var11) * (double)var8;
	double var25 = var13 + (var19 - var13) * (double)var8;
	int var27 = MathHelper::floor_double(var21);
	int var28 = MathHelper::floor_double(var23 + (double)(this->yOffset / 2.0F));
	int var29 = MathHelper::floor_double(var25);
	float var30 = this->worldObj->getLightBrightness(var27, var28, var29);
	var21 -= s_interpPosX;
	var23 -= s_interpPosY;
	var25 -= s_interpPosZ;
	GL11::glColor4f(var30, var30, var30, 1.0F);
	RenderManager::s_instance->doRenderEntity(this->thePlayer, (double)((float)var21), (double)((float)var23), (double)((float)var25), this->thePlayer->rotationYaw, var2);
}

void EntityPickupFX::onUpdate()
{
	++this->age;
	if(this->age == this->maxAge) {
	this->setEntityDead();
	}
}

int EntityPickupFX::getFXLayer()
{
	return 3;
}

void EntityPickupFX::initStatics()
{
}
