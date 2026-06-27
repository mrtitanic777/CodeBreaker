#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderPainting.h"
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
#include "EntityPainting.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumArt.h"
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

// EnumArt per-value dimensions (sizeX, sizeY, offsetX, offsetY in the art atlas).
// The EnumArt runtime stub flattened the enum to plain ordinals and dropped these
// fields, so the canonical Alpha values are reconstructed here, indexed by ordinal
// (matching the EnumArt.h order).
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // EnumArt.java carried these fields per enum constant; method a() read them directly:
 *  //   EnumArt var10 = var1.art;
 *  //   ... this.getBlockPathWeight(var1, var10.sizeX, var10.sizeY, var10.offsetX, var10.offsetY);
 *  // Each EnumArt constant declared (title, sizeX, sizeY, offsetX, offsetY), e.g.
 *  //   Kebab("Kebab", 16, 16, 0, 0), Aztec("Aztec", 16, 16, 16, 0), ... Pool("Pool", 32, 16, 0, 32), ...
 *  ---- end original Java ----
 *  EnumArt was reduced to a bare ordinal int, so sizeX/sizeY/offsetX/offsetY are reconstructed
 *  in this _ART_DIMS table keyed by ordinal. Faithful values, but the enum field access is gone. */
namespace {
	struct _ArtDim { int sizeX, sizeY, offsetX, offsetY; };
	static const _ArtDim _ART_DIMS[] = {
		{ 16, 16,   0,   0 },  // Kebab
		{ 16, 16,  16,   0 },  // Aztec
		{ 16, 16,  32,   0 },  // Alban
		{ 16, 16,  48,   0 },  // Aztec2
		{ 16, 16,  64,   0 },  // Bomb
		{ 16, 16,  80,   0 },  // Plant
		{ 16, 16,  96,   0 },  // Wasteland
		{ 32, 16,   0,  32 },  // Pool
		{ 32, 16,  32,  32 },  // Courbet
		{ 32, 16,  64,  32 },  // Sea
		{ 32, 16,  96,  32 },  // Sunset
		{ 32, 16, 128,  32 },  // Creebet
		{ 16, 32,   0,  64 },  // Wanderer
		{ 16, 32,  16,  64 },  // Graham
		{ 32, 32,   0, 128 },  // Match
		{ 32, 32,  32, 128 },  // Bust
		{ 32, 32,  64, 128 },  // Stage
		{ 32, 32,  96, 128 },  // Void
		{ 32, 32, 128, 128 },  // SkullAndRoses
		{ 64, 32,   0,  96 },  // Fighters
		{ 64, 64,   0, 192 },  // Pointer
		{ 64, 64,  64, 192 },  // Pigscene
		{ 64, 64, 192,  64 },  // Skeleton
		{ 64, 64, 192, 192 }   // DonkeyKong
	};
	inline const _ArtDim& _artDim(int ordinal) {
		if (ordinal < 0 || ordinal >= (int)(sizeof(_ART_DIMS) / sizeof(_ART_DIMS[0]))) return _ART_DIMS[0];
		return _ART_DIMS[ordinal];
	}
}

RenderPainting::RenderPainting()
	: rand(NULL)
{
	this->rand = new Random();
}

RenderPainting::~RenderPainting()
{
}

void RenderPainting::a(EntityPainting* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->rand->setSeed(187LL);
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glRotatef(var8, 0.0F, 1.0F, 0.0F);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	this->loadTexture("/art/kz.png");
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  EnumArt var10 = var1.art;
	 *  float var11 = 1.0F / 16.0F;
	 *  GL11.glScalef(var11, var11, var11);
	 *  this.getBlockPathWeight(var1, var10.sizeX, var10.sizeY, var10.offsetX, var10.offsetY);
	 *  ---- end original Java ----
	 *  EnumArt is a plain ordinal int here; the dropped sizeX/sizeY/offsetX/offsetY enum fields are
	 *  recovered via the _ART_DIMS lookup (_artDim) above. */
	int var10 = var1->art;
	const _ArtDim& var10d = _artDim(var10);
	float var11 = 1.0F / 16.0F;
	GL11::glScalef(var11, var11, var11);
	this->getBlockPathWeight(var1, var10d.sizeX, var10d.sizeY, var10d.offsetX, var10d.offsetY);
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	GL11::glPopMatrix();
}

void RenderPainting::getBlockPathWeight(EntityPainting* var1, int var2, int var3, int var4, int var5)
{
	float var6 = (float)(-var2) / 2.0F;
	float var7 = (float)(-var3) / 2.0F;
	float var8 = -0.5F;
	float var9 = 0.5F;
	for(int var10 = 0; var10 < var2 / 16; ++var10) {
	for(int var11 = 0; var11 < var3 / 16; ++var11) {
	float var12 = var6 + (float)((var10 + 1) * 16);
	float var13 = var6 + (float)(var10 * 16);
	float var14 = var7 + (float)((var11 + 1) * 16);
	float var15 = var7 + (float)(var11 * 16);
	this->setLightmap(var1, (var12 + var13) / 2.0F, (var14 + var15) / 2.0F);
	float var16 = (float)(var4 + var2 - var10 * 16) / 256.0F;
	float var17 = (float)(var4 + var2 - (var10 + 1) * 16) / 256.0F;
	float var18 = (float)(var5 + var3 - var11 * 16) / 256.0F;
	float var19 = (float)(var5 + var3 - (var11 + 1) * 16) / 256.0F;
	float var20 = 12.0F / 16.0F;
	float var21 = 13.0F / 16.0F;
	float var22 = 0.0F;
	float var23 = 1.0F / 16.0F;
	float var24 = 12.0F / 16.0F;
	float var25 = 13.0F / 16.0F;
	float var26 = 0.001953125F;
	float var27 = 0.001953125F;
	float var28 = 385.0F / 512.0F;
	float var29 = 385.0F / 512.0F;
	float var30 = 0.0F;
	float var31 = 1.0F / 16.0F;
	Tessellator* var32 = Tessellator::s_instance;
	var32->startDrawingQuads();
	var32->setNormal(0.0F, 0.0F, -1.0F);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var8, (double)var17, (double)var18);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var8, (double)var16, (double)var18);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var8, (double)var16, (double)var19);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var8, (double)var17, (double)var19);
	var32->setNormal(0.0F, 0.0F, 1.0F);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var9, (double)var20, (double)var22);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var9, (double)var21, (double)var22);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var9, (double)var21, (double)var23);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var9, (double)var20, (double)var23);
	var32->setNormal(0.0F, -1.0F, 0.0F);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var8, (double)var24, (double)var26);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var8, (double)var25, (double)var26);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var9, (double)var25, (double)var27);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var9, (double)var24, (double)var27);
	var32->setNormal(0.0F, 1.0F, 0.0F);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var9, (double)var24, (double)var26);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var9, (double)var25, (double)var26);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var8, (double)var25, (double)var27);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var8, (double)var24, (double)var27);
	var32->setNormal(-1.0F, 0.0F, 0.0F);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var9, (double)var29, (double)var30);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var9, (double)var29, (double)var31);
	var32->addVertexWithUV((double)var12, (double)var15, (double)var8, (double)var28, (double)var31);
	var32->addVertexWithUV((double)var12, (double)var14, (double)var8, (double)var28, (double)var30);
	var32->setNormal(1.0F, 0.0F, 0.0F);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var8, (double)var29, (double)var30);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var8, (double)var29, (double)var31);
	var32->addVertexWithUV((double)var13, (double)var15, (double)var9, (double)var28, (double)var31);
	var32->addVertexWithUV((double)var13, (double)var14, (double)var9, (double)var28, (double)var30);
	var32->draw();
	}
	}
}

void RenderPainting::setLightmap(EntityPainting* var1, float var2, float var3)
{
	int var4 = MathHelper::floor_double(var1->posX);
	int var5 = MathHelper::floor_double(var1->posY + (double)(var3 / 16.0F));
	int var6 = MathHelper::floor_double(var1->posZ);
	if(var1->direction == 0) {
	var4 = MathHelper::floor_double(var1->posX + (double)(var2 / 16.0F));
	}
	if(var1->direction == 1) {
	var6 = MathHelper::floor_double(var1->posZ - (double)(var2 / 16.0F));
	}
	if(var1->direction == 2) {
	var4 = MathHelper::floor_double(var1->posX - (double)(var2 / 16.0F));
	}
	if(var1->direction == 3) {
	var6 = MathHelper::floor_double(var1->posZ + (double)(var2 / 16.0F));
	}
	float var7 = this->renderManager->worldObj->getLightBrightness(var4, var5, var6);
	GL11::glColor3f(var7, var7, var7);
}

void RenderPainting::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityPainting*)var1, var2, var4, var6, var8, var9);
}
