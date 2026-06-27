#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"
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

Render::Render()
	: renderManager(NULL)
	, unusedModelBiped(NULL)
	, unusedRenderBlocks(NULL)
	, shadowSize(0)
	, shadowOpaque(0)
{
	this->unusedModelBiped = new ModelBiped();
	this->unusedRenderBlocks = new RenderBlocks();
	this->shadowSize = 0.0F;
	this->shadowOpaque = 1.0F;
}

Render::~Render()
{
}

void Render::loadTexture(const JavaString& var1)
{
	RenderEngine* var2 = this->renderManager->renderEngine;
	var2->bindTexture(var2->getTexture(var1));
}

void Render::loadDownloadableImageTexture(const JavaString& var1, const JavaString& var2)
{
	RenderEngine* var3 = this->renderManager->renderEngine;
	var3->bindTexture(var3->getTextureForDownloadableImage(var1, var2));
}

void Render::renderEntityOnFire(Entity* var1, double var2, double var4, double var6, float var8)
{
	GL11::glDisable(GL11::GL_LIGHTING);
	int var9 = Block::s_fire->blockIndexInTexture;
	int var10 = (var9 & 15) << 4;
	int var11 = var9 & 240;
	float var12 = (float)var10 / 256.0F;
	float var13 = ((float)var10 + 15.99F) / 256.0F;
	float var14 = (float)var11 / 256.0F;
	float var15 = ((float)var11 + 15.99F) / 256.0F;
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	float var16 = var1->width * 1.4F;
	GL11::glScalef(var16, var16, var16);
	this->loadTexture("/terrain.png");
	Tessellator* var17 = Tessellator::s_instance;
	float var18 = 1.0F;
	float var19 = 0.5F;
	float var20 = 0.0F;
	float var21 = var1->height / var1->width;
	GL11::glRotatef(-this->renderManager->playerViewYaw, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, 0.0F, -0.4F + (float)((int)var21) * 0.02F);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	var17->startDrawingQuads();
	while(var21 > 0.0F) {
	var17->addVertexWithUV((double)(var18 - var19), (double)(0.0F - var20), 0.0, (double)var13, (double)var15);
	var17->addVertexWithUV((double)(0.0F - var19), (double)(0.0F - var20), 0.0, (double)var12, (double)var15);
	var17->addVertexWithUV((double)(0.0F - var19), (double)(1.4F - var20), 0.0, (double)var12, (double)var14);
	var17->addVertexWithUV((double)(var18 - var19), (double)(1.4F - var20), 0.0, (double)var13, (double)var14);
	--var21;
	--var20;
	var18 *= 0.9F;
	GL11::glTranslatef(0.0F, 0.0F, -0.04F);
	}
	var17->draw();
	GL11::glPopMatrix();
	GL11::glEnable(GL11::GL_LIGHTING);
}

void Render::renderShadow(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	RenderEngine* var10 = this->renderManager->renderEngine;
	var10->bindTexture(var10->getTexture("%clamp%/misc/shadow.png"));
	World* var11 = this->getWorldFromRenderManager();
	GL11::glDepthMask(false);
	float var12 = this->shadowSize;
	double var13 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var9;
	double var15 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var9 + (double)var1->getYOffsetForRender();
	double var17 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var9;
	int var19 = MathHelper::floor_double(var13 - (double)var12);
	int var20 = MathHelper::floor_double(var13 + (double)var12);
	int var21 = MathHelper::floor_double(var15 - (double)var12);
	int var22 = MathHelper::floor_double(var15);
	int var23 = MathHelper::floor_double(var17 - (double)var12);
	int var24 = MathHelper::floor_double(var17 + (double)var12);
	double var25 = var2 - var13;
	double var27 = var4 - var15;
	double var29 = var6 - var17;
	Tessellator* var31 = Tessellator::s_instance;
	var31->startDrawingQuads();
	for(int var32 = var19; var32 <= var20; ++var32) {
	for(int var33 = var21; var33 <= var22; ++var33) {
	for(int var34 = var23; var34 <= var24; ++var34) {
	int var35 = var11->getBlockId(var32, var33 - 1, var34);
	if(var35 > 0 && var11->getBlockLightValue(var32, var33, var34) > 3) {
	this->renderShadowOnBlock(Block::s_blocksList[var35], var2, var4 + (double)var1->getYOffsetForRender(), var6, var32, var33, var34, var8, var12, var25, var27 + (double)var1->getYOffsetForRender(), var29);
	}
	}
	}
	}
	var31->draw();
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glDepthMask(true);
}

World* Render::getWorldFromRenderManager()
{
	return this->renderManager->worldObj;
}

void Render::renderShadowOnBlock(Block* var1, double var2, double var4, double var6, int var8, int var9, int var10, float var11, float var12, double var13, double var15, double var17)
{
	Tessellator* var19 = Tessellator::s_instance;
	if(var1->renderAsNormalBlock()) {
	double var20 = ((double)var11 - (var4 - ((double)var9 + var15)) / 2.0) * 0.5 * (double)this->getWorldFromRenderManager()->getLightBrightness(var8, var9, var10);
	if(var20 >= 0.0) {
	if(var20 > 1.0) {
	var20 = 1.0;
	}
	var19->setColorRGBA_F(1.0F, 1.0F, 1.0F, (float)var20);
	double var22 = (double)var8 + var1->field_370_bf + var13;
	double var24 = (double)var8 + var1->maxX + var13;
	double var26 = (double)var9 + var1->minY + var15 + 1.0 / 64.0;
	double var28 = (double)var10 + var1->minZ + var17;
	double var30 = (double)var10 + var1->maxZ + var17;
	float var32 = (float)((var2 - var22) / 2.0 / (double)var12 + 0.5);
	float var33 = (float)((var2 - var24) / 2.0 / (double)var12 + 0.5);
	float var34 = (float)((var6 - var28) / 2.0 / (double)var12 + 0.5);
	float var35 = (float)((var6 - var30) / 2.0 / (double)var12 + 0.5);
	var19->addVertexWithUV(var22, var26, var28, (double)var32, (double)var34);
	var19->addVertexWithUV(var22, var26, var30, (double)var32, (double)var35);
	var19->addVertexWithUV(var24, var26, var30, (double)var33, (double)var35);
	var19->addVertexWithUV(var24, var26, var28, (double)var33, (double)var34);
	}
	}
}

void Render::renderOffsetAABB(AxisAlignedBB* var0, double var1, double var3, double var5)
{
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	Tessellator* var7 = Tessellator::s_instance;
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	var7->startDrawingQuads();
	var7->setTranslationD(var1, var3, var5);
	var7->setNormal(0.0F, 0.0F, -1.0F);
	var7->addVertex(var0->minX, var0->maxY, var0->minZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var7->addVertex(var0->maxX, var0->minY, var0->minZ);
	var7->addVertex(var0->minX, var0->minY, var0->minZ);
	var7->setNormal(0.0F, 0.0F, 1.0F);
	var7->addVertex(var0->minX, var0->minY, var0->maxZ);
	var7->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var7->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var7->setNormal(0.0F, -1.0F, 0.0F);
	var7->addVertex(var0->minX, var0->minY, var0->minZ);
	var7->addVertex(var0->maxX, var0->minY, var0->minZ);
	var7->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var7->addVertex(var0->minX, var0->minY, var0->maxZ);
	var7->setNormal(0.0F, 1.0F, 0.0F);
	var7->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var7->addVertex(var0->minX, var0->maxY, var0->minZ);
	var7->setNormal(-1.0F, 0.0F, 0.0F);
	var7->addVertex(var0->minX, var0->minY, var0->maxZ);
	var7->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var7->addVertex(var0->minX, var0->maxY, var0->minZ);
	var7->addVertex(var0->minX, var0->minY, var0->minZ);
	var7->setNormal(1.0F, 0.0F, 0.0F);
	var7->addVertex(var0->maxX, var0->minY, var0->minZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var7->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var7->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var7->setTranslationD(0.0, 0.0, 0.0);
	var7->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
}

void Render::renderAABB(AxisAlignedBB* var0)
{
	Tessellator* var1 = Tessellator::s_instance;
	var1->startDrawingQuads();
	var1->addVertex(var0->minX, var0->maxY, var0->minZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var1->addVertex(var0->maxX, var0->minY, var0->minZ);
	var1->addVertex(var0->minX, var0->minY, var0->minZ);
	var1->addVertex(var0->minX, var0->minY, var0->maxZ);
	var1->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->minX, var0->minY, var0->minZ);
	var1->addVertex(var0->maxX, var0->minY, var0->minZ);
	var1->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var1->addVertex(var0->minX, var0->minY, var0->maxZ);
	var1->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var1->addVertex(var0->minX, var0->maxY, var0->minZ);
	var1->addVertex(var0->minX, var0->minY, var0->maxZ);
	var1->addVertex(var0->minX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->minX, var0->maxY, var0->minZ);
	var1->addVertex(var0->minX, var0->minY, var0->minZ);
	var1->addVertex(var0->maxX, var0->minY, var0->minZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->minZ);
	var1->addVertex(var0->maxX, var0->maxY, var0->maxZ);
	var1->addVertex(var0->maxX, var0->minY, var0->maxZ);
	var1->draw();
}

void Render::setRenderManager(RenderManager* var1)
{
	this->renderManager = var1;
}

void Render::doRenderShadowAndFire(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	if(this->renderManager->options->fancyGraphics && this->shadowSize > 0.0F) {
	double var10 = this->renderManager->getDistanceToCamera(var1->posX, var1->posY, var1->posZ);
	float var12 = (float)((1.0 - var10 / 256.0) * (double)this->shadowOpaque);
	if(var12 > 0.0F) {
	this->renderShadow(var1, var2, var4, var6, var12, var9);
	}
	}
	if(var1->fire > 0 || var1->field_9299_bv) {
	this->renderEntityOnFire(var1, var2, var4, var6, var9);
	}
}

FontRenderer* Render::getFontRendererFromRenderManager()
{
	return this->renderManager->getFontRenderer();
}

void Render::initStatics()
{
}
