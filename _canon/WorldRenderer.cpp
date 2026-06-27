#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldRenderer.h"
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
#include "ChunkCache.h"
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
#include "ICamera.h"
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
#include "TileEntityRenderer.h"
#include "TileEntitySpecialRenderer.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

Tessellator* WorldRenderer::s_tessellator = Tessellator::s_instance;
int WorldRenderer::s_chunksUpdated = 0;

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  Java used java.util.Collection.addAll / removeAll on HashSet/List in updateRenderer():
 *      HashSet var21 = new HashSet();
 *      var21.addAll(this.tileEntityRenderers);
 *      ...
 *      HashSet var22 = new HashSet();
 *      var22.addAll(this.tileEntityRenderers);
 *      var22.removeAll(var21);
 *      this.tileEntities.addAll(var22);
 *      var21.removeAll(this.tileEntityRenderers);
 *      this.tileEntities.removeAll(var21);
 *  std::set / JList carry no addAll/removeAll, so these local helpers stand in for them
 *  (set = de-duplicating, list = positional). Behaviour-equivalent shim.
 *  ---- end original Java ---- */
// Java Collection.addAll/removeAll helpers. std::set / JList don't carry these, so
// supply faithful local equivalents (set = de-duplicating, list = positional).
static void _addAll(std::set<void*>& dst, const std::vector<void*>& src) {
	dst.insert(src.begin(), src.end());
}
static void _removeAll(std::set<void*>& dst, const std::vector<void*>& src) {
	for (std::vector<void*>::const_iterator it = src.begin(); it != src.end(); ++it) dst.erase(*it);
}
static void _removeAll(std::set<void*>& dst, const std::set<void*>& src) {
	for (std::set<void*>::const_iterator it = src.begin(); it != src.end(); ++it) dst.erase(*it);
}
static void _addAll(JList<void*>& dst, const std::set<void*>& src) {
	for (std::set<void*>::const_iterator it = src.begin(); it != src.end(); ++it) dst.push_back(*it);
}
static void _removeAll(JList<void*>& dst, const std::set<void*>& src) {
	for (std::set<void*>::const_iterator it = src.begin(); it != src.end(); ++it) dst.remove(*it);
}

WorldRenderer::WorldRenderer(World* var1, JList<void*> var2, int var3, int var4, int var5, int var6, int var7)
	: worldObj(NULL)
	, glRenderList(0)
	, posX(0)
	, posY(0)
	, posZ(0)
	, sizeWidth(0)
	, sizeHeight(0)
	, sizeDepth(0)
	, posXClamped(0)
	, posYClamped(0)
	, posZClamped(0)
	, posXMinus(0)
	, posYMinus(0)
	, posZMinus(0)
	, isVisibleFromCamera(0)
	, posXClip(0)
	, posYClip(0)
	, posZClip(0)
	, rendererRadius(0)
	, needsUpdate(0)
	, rendererBoundingBox(NULL)
	, chunkIndex(0)
	, isVisible(0)
	, isInFrustum(0)
	, glRenderListBase(0)
	, isWaitingOnOcclusionQuery(0)
	, isInitialized(0)
{
	this->worldObj = var1;
	this->tileEntities = var2;
	this->sizeWidth = this->sizeHeight = this->sizeDepth = var6;
	this->rendererRadius = MathHelper::sqrt_float((float)(this->sizeWidth * this->sizeWidth + this->sizeHeight * this->sizeHeight + this->sizeDepth * this->sizeDepth)) / 2.0F;
	this->glRenderList = var7;
	this->posX = -999;
	this->setPosition(var3, var4, var5);
	this->needsUpdate = false;
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::setPosition(int var1, int var2, int var3)
{
	if(var1 != this->posX || var2 != this->posY || var3 != this->posZ) {
	this->markPassesDirty();
	this->posX = var1;
	this->posY = var2;
	this->posZ = var3;
	this->posXClip = var1 + this->sizeWidth / 2;
	this->posYClip = var2 + this->sizeHeight / 2;
	this->posZClip = var3 + this->sizeDepth / 2;
	this->posXMinus = var1 & 1023;
	this->posYMinus = var2;
	this->posZMinus = var3 & 1023;
	this->posXClamped = var1 - this->posXMinus;
	this->posYClamped = var2 - this->posYMinus;
	this->posZClamped = var3 - this->posZMinus;
	float var4 = 2.0F;
	this->rendererBoundingBox = AxisAlignedBB::getBoundingBox((double)((float)var1 - var4), (double)((float)var2 - var4), (double)((float)var3 - var4), (double)((float)(var1 + this->sizeWidth) + var4), (double)((float)(var2 + this->sizeHeight) + var4), (double)((float)(var3 + this->sizeDepth) + var4));
	GL11::glNewList(this->glRenderList + 2, GL11::GL_COMPILE);
	RenderItem::renderAABB(AxisAlignedBB::getBoundingBoxFromPool((double)((float)this->posXMinus - var4), (double)((float)this->posYMinus - var4), (double)((float)this->posZMinus - var4), (double)((float)(this->posXMinus + this->sizeWidth) + var4), (double)((float)(this->posYMinus + this->sizeHeight) + var4), (double)((float)(this->posZMinus + this->sizeDepth) + var4)));
	GL11::glEndList();
	this->MarkDirty();
	}
}

void WorldRenderer::translateToOrigin()
{
	GL11::glTranslatef((float)this->posXMinus, (float)this->posYMinus, (float)this->posZMinus);
}

void WorldRenderer::updateRenderer()
{
	if(this->needsUpdate) {
	++s_chunksUpdated;
	int var1 = this->posX;
	int var2 = this->posY;
	int var3 = this->posZ;
	int var4 = this->posX + this->sizeWidth;
	int var5 = this->posY + this->sizeHeight;
	int var6 = this->posZ + this->sizeDepth;
	for(int var7 = 0; var7 < 2; ++var7) {
	this->skipRenderPass[var7] = true;
	}
	Chunk::s_isLit = false;
	std::set<void*> var21;
	_addAll(var21, this->tileEntityRenderers);
	this->tileEntityRenderers.clear();
	unsigned char var8 = 1;
	ChunkCache* var9 = new ChunkCache(this->worldObj, var1 - var8, var2 - var8, var3 - var8, var4 + var8, var5 + var8, var6 + var8);
	RenderBlocks* var10 = new RenderBlocks(var9);
	for(int var11 = 0; var11 < 2; ++var11) {
	bool var12 = false;
	bool var13 = false;
	bool var14 = false;
	for(int var15 = var2; var15 < var5; ++var15) {
	for(int var16 = var3; var16 < var6; ++var16) {
	for(int var17 = var1; var17 < var4; ++var17) {
	int var18 = var9->getBlockId(var17, var15, var16);
	if(var18 > 0) {
	if(!var14) {
	var14 = true;
	GL11::glNewList(this->glRenderList + var11, GL11::GL_COMPILE);
	GL11::glPushMatrix();
	this->translateToOrigin();
	float var19 = 1.000001F;
	GL11::glTranslatef((float)(-this->sizeDepth) / 2.0F, (float)(-this->sizeHeight) / 2.0F, (float)(-this->sizeDepth) / 2.0F);
	GL11::glScalef(var19, var19, var19);
	GL11::glTranslatef((float)this->sizeDepth / 2.0F, (float)this->sizeHeight / 2.0F, (float)this->sizeDepth / 2.0F);
	s_tessellator->startDrawingQuads();
	s_tessellator->setTranslationD((double)(-this->posX), (double)(-this->posY), (double)(-this->posZ));
	}
	if(var11 == 0 && Block::s_isBlockContainer[var18]) {
	TileEntity* var23 = var9->getBlockTileEntity(var17, var15, var16);
	if(TileEntityRenderer::s_instance->hasSpecialRenderer(var23)) {
	this->tileEntityRenderers.push_back(var23);
	}
	}
	Block* var24 = Block::s_blocksList[var18];
	int var20 = var24->getRenderBlockPass();
	if(var20 != var11) {
	var12 = true;
	} else if(var20 == var11) {
	var13 |= var10->renderBlockByRenderType(var24, var17, var15, var16);
	}
	}
	}
	}
	}
	if(var14) {
	s_tessellator->draw();
	GL11::glPopMatrix();
	GL11::glEndList();
	s_tessellator->setTranslationD(0.0, 0.0, 0.0);
	} else {
	var13 = false;
	}
	if(var13) {
	this->skipRenderPass[var11] = false;
	}
	if(!var12) {
	break;
	}
	}
	std::set<void*> var22;
	_addAll(var22, this->tileEntityRenderers);
	_removeAll(var22, var21);
	_addAll(this->tileEntities, var22);
	_removeAll(var21, this->tileEntityRenderers);
	_removeAll(this->tileEntities, var21);
	this->isWaitingOnOcclusionQuery = Chunk::s_isLit;
	this->isInitialized = true;
	}
}

float WorldRenderer::distanceToEntitySquared(Entity* var1)
{
	float var2 = (float)(var1->posX - (double)this->posXClip);
	float var3 = (float)(var1->posY - (double)this->posYClip);
	float var4 = (float)(var1->posZ - (double)this->posZClip);
	return var2 * var2 + var3 * var3 + var4 * var4;
}

void WorldRenderer::markPassesDirty()
{
	for(int var1 = 0; var1 < 2; ++var1) {
	this->skipRenderPass[var1] = true;
	}
	this->isVisibleFromCamera = false;
	this->isInitialized = false;
}

void WorldRenderer::setDontDraw()
{
	this->markPassesDirty();
	this->worldObj = NULL;
}

int WorldRenderer::getGLCallList(int var1)
{
	return !this->isVisibleFromCamera ? -1 : (!this->skipRenderPass[var1] ? this->glRenderList + var1 : -1);
}

void WorldRenderer::updateInFrustum(ICamera* var1)
{
	this->isVisibleFromCamera = var1->isBoundingBoxInFrustum(this->rendererBoundingBox);
}

void WorldRenderer::callOcclusionQueryList()
{
	GL11::glCallList(this->glRenderList + 2);
}

bool WorldRenderer::skipAllRenderPasses()
{
	return !this->isInitialized ? false : this->skipRenderPass[0] && this->skipRenderPass[1];
}

void WorldRenderer::MarkDirty()
{
	this->needsUpdate = true;
}
