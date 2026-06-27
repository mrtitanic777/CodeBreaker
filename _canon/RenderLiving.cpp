#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"
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
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Exception.h"
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

RenderLiving::RenderLiving(ModelBase* var1, float var2)
	: e(NULL)
	, renderPassModel(NULL)
{
	this->e = var1;
	this->shadowSize = var2;
}

RenderLiving::~RenderLiving()
{
}

void RenderLiving::setRenderPassModel(ModelBase* var1)
{
	this->renderPassModel = var1;
}

void RenderLiving::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	GL11::glDisable(GL11::GL_CULL_FACE);
	this->e->onGround = this->getSwingProgress(var1, var9);
	this->e->isChild = var1->ridingEntity != NULL || var1->field_9300_bu;
	if(this->renderPassModel != NULL) {
	this->renderPassModel->isChild = this->e->isChild;
	}
	try {
	float var10 = var1->prevRenderYawOffset + (var1->renderYawOffset - var1->prevRenderYawOffset) * var9;
	float var11 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var9;
	float var12 = var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9;
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	float var13 = this->handleRotationFloat(var1, var9);
	GL11::glRotatef(180.0F - var10, 0.0F, 1.0F, 0.0F);
	float var14;
	if(var1->deathTime > 0) {
	var14 = ((float)var1->deathTime + var9 - 1.0F) / 20.0F * 1.6F;
	var14 = MathHelper::sqrt_float(var14);
	if(var14 > 1.0F) {
	var14 = 1.0F;
	}
	GL11::glRotatef(var14 * this->getDeathMaxRotation(var1), 0.0F, 0.0F, 1.0F);
	}
	var14 = 1.0F / 16.0F;
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	GL11::glScalef(-1.0F, -1.0F, 1.0F);
	this->preRenderCallback(var1, var9);
	GL11::glTranslatef(0.0F, -24.0F * var14 - 0.0078125F, 0.0F);
	float var15 = var1->prevLimbSwingAmount + (var1->limbSwingAmount - var1->prevLimbSwingAmount) * var9;
	float var16 = var1->limbSwing - var1->limbSwingAmount * (1.0F - var9);
	if(var15 > 1.0F) {
	var15 = 1.0F;
	}
	this->loadDownloadableImageTexture(var1->skinUrl, var1->getEntityTexture());
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	this->e->render(var16, var15, var13, var11 - var10, var12, var14);
	for(int var17 = 0; var17 < 4; ++var17) {
	if(this->shouldRenderPass(var1, var17)) {
	this->renderPassModel->render(var16, var15, var13, var11 - var10, var12, var14);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	}
	}
	this->renderEquippedItems(var1, var9);
	float var25 = var1->getEntityBrightness(var9);
	int var18 = this->getColorMultiplier(var1, var25, var9);
	if(((var18 >> 24) & 255) > 0 || var1->hurtTime > 0 || var1->deathTime > 0) {
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	GL11::glDepthFunc(GL11::GL_EQUAL);
	if(var1->hurtTime > 0 || var1->deathTime > 0) {
	GL11::glColor4f(var25, 0.0F, 0.0F, 0.4F);
	this->e->render(var16, var15, var13, var11 - var10, var12, var14);
	for(int var19 = 0; var19 < 4; ++var19) {
	if(this->shouldRenderPass(var1, var19)) {
	GL11::glColor4f(var25, 0.0F, 0.0F, 0.4F);
	this->renderPassModel->render(var16, var15, var13, var11 - var10, var12, var14);
	}
	}
	}
	if(((var18 >> 24) & 255) > 0) {
	float var26 = (float)(var18 >> 16 & 255) / 255.0F;
	float var20 = (float)(var18 >> 8 & 255) / 255.0F;
	float var21 = (float)(var18 & 255) / 255.0F;
	float var22 = (float)(var18 >> 24 & 255) / 255.0F;
	GL11::glColor4f(var26, var20, var21, var22);
	this->e->render(var16, var15, var13, var11 - var10, var12, var14);
	for(int var23 = 0; var23 < 4; ++var23) {
	if(this->shouldRenderPass(var1, var23)) {
	GL11::glColor4f(var26, var20, var21, var22);
	this->renderPassModel->render(var16, var15, var13, var11 - var10, var12, var14);
	}
	}
	}
	GL11::glDepthFunc(GL11::GL_LEQUAL);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	}
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	} catch (Exception* var24) {
	var24->printStackTrace();
	}
	GL11::glEnable(GL11::GL_CULL_FACE);
	GL11::glPopMatrix();
}

float RenderLiving::getSwingProgress(EntityLiving* var1, float var2)
{
	return var1->getSwingProgress(var2);
}

float RenderLiving::handleRotationFloat(EntityLiving* var1, float var2)
{
	return (float)var1->ticksExisted + var2;
}

void RenderLiving::renderEquippedItems(EntityLiving* var1, float var2)
{
}

bool RenderLiving::shouldRenderPass(EntityLiving* var1, int var2)
{
	return false;
}

float RenderLiving::getDeathMaxRotation(EntityLiving* var1)
{
	return 90.0F;
}

int RenderLiving::getColorMultiplier(EntityLiving* var1, float var2, float var3)
{
	return 0;
}

void RenderLiving::preRenderCallback(EntityLiving* var1, float var2)
{
}

void RenderLiving::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityLiving*)var1, var2, var4, var6, var8, var9);
}

void RenderLiving::initStatics()
{
}
