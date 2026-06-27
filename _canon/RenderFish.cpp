#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderFish.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderFish.h"
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
#include "GL11.h"
#include "GL12.h"
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

RenderFish::~RenderFish()
{
}

void RenderFish::a(EntityFish* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	GL11::glScalef(0.5F, 0.5F, 0.5F);
	unsigned char var10 = 1;
	unsigned char var11 = 2;
	this->loadTexture("/particles.png");
	Tessellator* var12 = Tessellator::s_instance;
	float var13 = (float)(var10 * 8 + 0) / 128.0F;
	float var14 = (float)(var10 * 8 + 8) / 128.0F;
	float var15 = (float)(var11 * 8 + 0) / 128.0F;
	float var16 = (float)(var11 * 8 + 8) / 128.0F;
	float var17 = 1.0F;
	float var18 = 0.5F;
	float var19 = 0.5F;
	GL11::glRotatef(180.0F - this->renderManager->playerViewYaw, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-this->renderManager->playerViewPitch, 1.0F, 0.0F, 0.0F);
	var12->startDrawingQuads();
	var12->setNormal(0.0F, 1.0F, 0.0F);
	var12->addVertexWithUV((double)(0.0F - var18), (double)(0.0F - var19), 0.0, (double)var13, (double)var16);
	var12->addVertexWithUV((double)(var17 - var18), (double)(0.0F - var19), 0.0, (double)var14, (double)var16);
	var12->addVertexWithUV((double)(var17 - var18), (double)(1.0F - var19), 0.0, (double)var14, (double)var15);
	var12->addVertexWithUV((double)(0.0F - var18), (double)(1.0F - var19), 0.0, (double)var13, (double)var15);
	var12->draw();
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	GL11::glPopMatrix();
	if(var1->angler != NULL) {
	float var20 = (var1->angler->prevRotationYaw + (var1->angler->rotationYaw - var1->angler->prevRotationYaw) * var9) * (float)3.1415927F / 180.0F;
	float var21 = (var1->angler->prevRotationPitch + (var1->angler->rotationPitch - var1->angler->prevRotationPitch) * var9) * (float)3.1415927F / 180.0F;
	double var22 = (double)MathHelper::sin(var20);
	double var24 = (double)MathHelper::cos(var20);
	double var26 = (double)MathHelper::sin(var21);
	double var28 = (double)MathHelper::cos(var21);
	double var30 = var1->angler->prevPosX + (var1->angler->posX - var1->angler->prevPosX) * (double)var9 - var24 * 0.7 - var22 * 0.5 * var28;
	double var32 = var1->angler->prevPosY + (var1->angler->posY - var1->angler->prevPosY) * (double)var9 - var26 * 0.5;
	double var34 = var1->angler->prevPosZ + (var1->angler->posZ - var1->angler->prevPosZ) * (double)var9 - var22 * 0.7 + var24 * 0.5 * var28;
	if(this->renderManager->options->thirdPersonView) {
	var20 = (var1->angler->prevRenderYawOffset + (var1->angler->renderYawOffset - var1->angler->prevRenderYawOffset) * var9) * (float)3.1415927F / 180.0F;
	var22 = (double)MathHelper::sin(var20);
	var24 = (double)MathHelper::cos(var20);
	var30 = var1->angler->prevPosX + (var1->angler->posX - var1->angler->prevPosX) * (double)var9 - var24 * 0.35 - var22 * 0.85;
	var32 = var1->angler->prevPosY + (var1->angler->posY - var1->angler->prevPosY) * (double)var9 - 0.45;
	var34 = var1->angler->prevPosZ + (var1->angler->posZ - var1->angler->prevPosZ) * (double)var9 - var22 * 0.35 + var24 * 0.85;
	}
	double var36 = var1->prevPosX + (var1->posX - var1->prevPosX) * (double)var9;
	double var38 = var1->prevPosY + (var1->posY - var1->prevPosY) * (double)var9 + 0.25;
	double var40 = var1->prevPosZ + (var1->posZ - var1->prevPosZ) * (double)var9;
	double var42 = (double)((float)(var30 - var36));
	double var44 = (double)((float)(var32 - var38));
	double var46 = (double)((float)(var34 - var40));
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	GL11::glDisable(GL11::GL_LIGHTING);
	var12->startDrawing(3);
	var12->setColorOpaque_I(0);
	unsigned char var48 = 16;
	for(int var49 = 0; var49 <= var48; ++var49) {
	float var50 = (float)var49 / (float)var48;
	var12->addVertex(var2 + var42 * (double)var50, var4 + var44 * (double)(var50 * var50 + var50) * 0.5 + 0.25, var6 + var46 * (double)var50);
	}
	var12->draw();
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	}
}

void RenderFish::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityFish*)var1, var2, var4, var6, var8, var9);
}

void RenderFish::initStatics()
{
}
