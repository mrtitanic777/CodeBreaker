#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntitySignRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySignRenderer.h"
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
#include "SignModel.h"
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
#include "TileEntitySign.h"
#include "TileEntitySpecialRenderer.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

TileEntitySignRenderer::TileEntitySignRenderer()
	: signModel(NULL)
{
	this->signModel = new SignModel();
}

TileEntitySignRenderer::~TileEntitySignRenderer()
{
}

void TileEntitySignRenderer::a(TileEntitySign* var1, double var2, double var4, double var6, float var8)
{
	Block* var9 = var1->getBlockType();
	GL11::glPushMatrix();
	float var10 = 2.0F / 3.0F;
	float var12;
	if(var9 == Block::s_signPost) {
	GL11::glTranslatef((float)var2 + 0.5F, (float)var4 + 12.0F / 16.0F * var10, (float)var6 + 0.5F);
	float var11 = (float)(var1->getBlockMetadata() * 360) / 16.0F;
	GL11::glRotatef(-var11, 0.0F, 1.0F, 0.0F);
	this->signModel->signPost->showModel = true;
	} else {
	int var16 = var1->getBlockMetadata();
	var12 = 0.0F;
	if(var16 == 2) {
	var12 = 180.0F;
	}
	if(var16 == 4) {
	var12 = 90.0F;
	}
	if(var16 == 5) {
	var12 = -90.0F;
	}
	GL11::glTranslatef((float)var2 + 0.5F, (float)var4 + 12.0F / 16.0F * var10, (float)var6 + 0.5F);
	GL11::glRotatef(-var12, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, -(5.0F / 16.0F), -(7.0F / 16.0F));
	this->signModel->signPost->showModel = false;
	}
	this->bindTextureByName("/item/sign.png");
	GL11::glPushMatrix();
	GL11::glScalef(var10, -var10, -var10);
	this->signModel->renderSign();
	GL11::glPopMatrix();
	FontRenderer* var17 = this->getFontRenderer();
	var12 = (float)(1.0 / 60.0) * var10;
	GL11::glTranslatef(0.0F, 0.5F * var10, 0.07F * var10);
	GL11::glScalef(var12, -var12, var12);
	GL11::glNormal3f(0.0F, 0.0F, -1.0F * var12);
	GL11::glDepthMask(false);
	unsigned char var13 = 0;
	for(int var14 = 0; var14 < var1->signText.length; ++var14) {
	JavaString var15 = var1->signText[var14];
	if(var14 == var1->lineBeingEdited) {
	var15 = (JCat() + "> " + var15 + " <");
	var17->drawString(var15, -var17->getStringWidth(var15) / 2, var14 * 10 - var1->signText.length * 5, var13);
	} else {
	var17->drawString(var15, -var17->getStringWidth(var15) / 2, var14 * 10 - var1->signText.length * 5, var13);
	}
	}
	GL11::glDepthMask(true);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glPopMatrix();
}

void TileEntitySignRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8)
{
	this->a((TileEntitySign*)var1, var2, var4, var6, var8);
}

void TileEntitySignRenderer::initStatics()
{
}
