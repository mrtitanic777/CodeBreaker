#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderMinecart.h"
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
#include "EntityMinecart.h"
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
#include "ModelMinecart.h"
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

RenderMinecart::RenderMinecart()
	: a(NULL)
{
	this->shadowSize = 0.5F;
	this->a = new ModelMinecart();
}

RenderMinecart::~RenderMinecart()
{
	delete a;
	a = NULL;
}

void RenderMinecart::a_(EntityMinecart* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	double var10 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var9;
	double var12 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var9;
	double var14 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var9;
	double var16 = (double)0.3F;
	Vec3D* var18 = var1->getPosOnRail(var10, var12, var14);
	float var19 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9;
	if(var18 != NULL) {
	Vec3D* var20 = var1->getPosOffsetFromRail(var10, var12, var14, var16);
	Vec3D* var21 = var1->getPosOffsetFromRail(var10, var12, var14, -var16);
	if(var20 == NULL) {
	var20 = var18;
	}
	if(var21 == NULL) {
	var21 = var18;
	}
	var2 += var18->xCoord - var10;
	var4 += (var20->yCoord + var21->yCoord) / 2.0 - var12;
	var6 += var18->zCoord - var14;
	Vec3D* var22 = var21->addVector(-var20->xCoord, -var20->yCoord, -var20->zCoord);
	if(var22->lengthVector() != 0.0) {
	var22 = var22->normalize();
	var8 = (float)(atan2f(var22->zCoord, var22->xCoord) * 180.0 / 3.1415927F);
	var19 = (float)(atanf(var22->yCoord) * 73.0);
	}
	}
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glRotatef(180.0F - var8, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-var19, 0.0F, 0.0F, 1.0F);
	float var23 = (float)var1->b - var9;
	float var24 = (float)var1->a - var9;
	if(var24 < 0.0F) {
	var24 = 0.0F;
	}
	if(var23 > 0.0F) {
	GL11::glRotatef(MathHelper::sin(var23) * var23 * var24 / 10.0F * (float)var1->c, 1.0F, 0.0F, 0.0F);
	}
	if(var1->d != 0) {
	this->loadTexture("/terrain.png");
	float var25 = 12.0F / 16.0F;
	GL11::glScalef(var25, var25, var25);
	GL11::glTranslatef(0.0F, 5.0F / 16.0F, 0.0F);
	GL11::glRotatef(90.0F, 0.0F, 1.0F, 0.0F);
	if(var1->d == 1) {
	(new RenderBlocks())->renderBlockAsItem(Block::s_crate);
	} else if(var1->d == 2) {
	(new RenderBlocks())->renderBlockAsItem(Block::s_stoneOvenIdle);
	}
	GL11::glRotatef(-90.0F, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, -(5.0F / 16.0F), 0.0F);
	GL11::glScalef(1.0F / var25, 1.0F / var25, 1.0F / var25);
	}
	this->loadTexture("/item/cart.png");
	GL11::glScalef(-1.0F, -1.0F, 1.0F);
	this->a->render(0.0F, 0.0F, -0.1F, 0.0F, 0.0F, 1.0F / 16.0F);
	GL11::glPopMatrix();
}

void RenderMinecart::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a_((EntityMinecart*)var1, var2, var4, var6, var8, var9);
}

void RenderMinecart::initStatics()
{
}
