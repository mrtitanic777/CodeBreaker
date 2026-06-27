#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EffectRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EffectRenderer.h"
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
#include "Entity.h"
#include "EntityDiggingFX.h"
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

EffectRenderer::EffectRenderer(World* var1, RenderEngine* var2)
	: worldObj(NULL)
	, renderer(NULL)
	, rand(NULL)
{
	this->fxLayers = JArray<JList<void*> >(4);
	this->rand = new Random();
	if(var1 != NULL) {
	this->worldObj = var1;
	}
	this->renderer = var2;
	for(int var3 = 0; var3 < 4; ++var3) {
	this->fxLayers[var3] = JList<void*>();
	}
}

EffectRenderer::~EffectRenderer()
{
}

void EffectRenderer::addEffect(EntityFX* var1)
{
	int var2 = var1->getFXLayer();
	this->fxLayers[var2].push_back(var1);
}

void EffectRenderer::updateEffects()
{
	for(int var1 = 0; var1 < 4; ++var1) {
	for(int var2 = 0; var2 < this->fxLayers[var1].size(); ++var2) {
	EntityFX* var3 = (EntityFX*)this->fxLayers[var1][var2];
	var3->onUpdate();
	if(var3->isDead) {
	this->fxLayers[var1].remove(var2--);
	}
	}
	}
}

void EffectRenderer::renderParticles(Entity* var1, float var2)
{
	float var3 = MathHelper::cos(var1->rotationYaw * (float)3.1415927F / 180.0F);
	float var4 = MathHelper::sin(var1->rotationYaw * (float)3.1415927F / 180.0F);
	float var5 = -var4 * MathHelper::sin(var1->rotationPitch * (float)3.1415927F / 180.0F);
	float var6 = var3 * MathHelper::sin(var1->rotationPitch * (float)3.1415927F / 180.0F);
	float var7 = MathHelper::cos(var1->rotationPitch * (float)3.1415927F / 180.0F);
	EntityFX::s_interpPosX = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
	EntityFX::s_interpPosY = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
	EntityFX::s_interpPosZ = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;
	for(int var8 = 0; var8 < 3; ++var8) {
	if(this->fxLayers[var8].size() != 0) {
	int var9 = 0;
	if(var8 == 0) {
	var9 = this->renderer->getTexture("/particles.png");
	}
	if(var8 == 1) {
	var9 = this->renderer->getTexture("/terrain.png");
	}
	if(var8 == 2) {
	var9 = this->renderer->getTexture("/gui/items.png");
	}
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var9);
	Tessellator* var10 = Tessellator::s_instance;
	var10->startDrawingQuads();
	for(int var11 = 0; var11 < this->fxLayers[var8].size(); ++var11) {
	EntityFX* var12 = (EntityFX*)this->fxLayers[var8][var11];
	var12->renderParticle(var10, var2, var3, var7, var4, var5, var6);
	}
	var10->draw();
	}
	}
}

void EffectRenderer::renderLitParticles(Entity* var1, float var2)
{
	unsigned char var3 = 3;
	if(this->fxLayers[var3].size() != 0) {
	Tessellator* var4 = Tessellator::s_instance;
	for(int var5 = 0; var5 < this->fxLayers[var3].size(); ++var5) {
	EntityFX* var6 = (EntityFX*)this->fxLayers[var3][var5];
	var6->renderParticle(var4, var2, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
	}
	}
}

void EffectRenderer::clearEffects(World* var1)
{
	this->worldObj = var1;
	for(int var2 = 0; var2 < 4; ++var2) {
	this->fxLayers[var2].clear();
	}
}

void EffectRenderer::addBlockDestroyEffects(int var1, int var2, int var3)
{
	int var4 = this->worldObj->getBlockId(var1, var2, var3);
	if(var4 != 0) {
	Block* var5 = Block::s_blocksList[var4];
	unsigned char var6 = 4;
	for(int var7 = 0; var7 < var6; ++var7) {
	for(int var8 = 0; var8 < var6; ++var8) {
	for(int var9 = 0; var9 < var6; ++var9) {
	double var10 = (double)var1 + ((double)var7 + 0.5) / (double)var6;
	double var12 = (double)var2 + ((double)var8 + 0.5) / (double)var6;
	double var14 = (double)var3 + ((double)var9 + 0.5) / (double)var6;
	this->addEffect((new EntityDiggingFX(this->worldObj, var10, var12, var14, var10 - (double)var1 - 0.5, var12 - (double)var2 - 0.5, var14 - (double)var3 - 0.5, var5))->applyColorMultiplier(var1, var2, var3));
	}
	}
	}
	}
}

void EffectRenderer::addBlockHitEffects(int var1, int var2, int var3, int var4)
{
	int var5 = this->worldObj->getBlockId(var1, var2, var3);
	if(var5 != 0) {
	Block* var6 = Block::s_blocksList[var5];
	float var7 = 0.1F;
	double var8 = (double)var1 + this->rand->nextDouble() * (var6->maxX - var6->field_370_bf - (double)(var7 * 2.0F)) + (double)var7 + var6->field_370_bf;
	double var10 = (double)var2 + this->rand->nextDouble() * (var6->maxY - var6->minY - (double)(var7 * 2.0F)) + (double)var7 + var6->minY;
	double var12 = (double)var3 + this->rand->nextDouble() * (var6->maxZ - var6->minZ - (double)(var7 * 2.0F)) + (double)var7 + var6->minZ;
	if(var4 == 0) {
	var10 = (double)var2 + var6->minY - (double)var7;
	}
	if(var4 == 1) {
	var10 = (double)var2 + var6->maxY + (double)var7;
	}
	if(var4 == 2) {
	var12 = (double)var3 + var6->minZ - (double)var7;
	}
	if(var4 == 3) {
	var12 = (double)var3 + var6->maxZ + (double)var7;
	}
	if(var4 == 4) {
	var8 = (double)var1 + var6->field_370_bf - (double)var7;
	}
	if(var4 == 5) {
	var8 = (double)var1 + var6->maxX + (double)var7;
	}
	this->addEffect((new EntityDiggingFX(this->worldObj, var8, var10, var12, 0.0, 0.0, 0.0, var6))->applyColorMultiplier(var1, var2, var3)->multiplyVelocity(0.2F)->multipleParticleScaleBy(0.6F));
	}
}

JavaString EffectRenderer::getStatistics()
{
	return (JCat() + "" + (this->fxLayers[0].size() + this->fxLayers[1].size() + this->fxLayers[2].size()));
}

void EffectRenderer::initStatics()
{
}
