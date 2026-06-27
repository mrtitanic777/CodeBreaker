#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderArrow.h"
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
#include "EntityArrow.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
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

RenderArrow::~RenderArrow()
{
}

void RenderArrow::a(EntityArrow* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->loadTexture("/item/arrows.png");
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glRotatef(var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var9 - 90.0F, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(var1->prevRotationPitch + (var1->rotationPitch - var1->prevRotationPitch) * var9, 0.0F, 0.0F, 1.0F);
	Tessellator* var10 = Tessellator::s_instance;
	unsigned char var11 = 0;
	float var12 = 0.0F;
	float var13 = 0.5F;
	float var14 = (float)(0 + var11 * 10) / 32.0F;
	float var15 = (float)(5 + var11 * 10) / 32.0F;
	float var16 = 0.0F;
	float var17 = 0.15625F;
	float var18 = (float)(5 + var11 * 10) / 32.0F;
	float var19 = (float)(10 + var11 * 10) / 32.0F;
	float var20 = 0.05625F;
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	float var21 = (float)var1->arrowShake - var9;
	if(var21 > 0.0F) {
	float var22 = -MathHelper::sin(var21 * 3.0F) * var21;
	GL11::glRotatef(var22, 0.0F, 0.0F, 1.0F);
	}
	GL11::glRotatef(45.0F, 1.0F, 0.0F, 0.0F);
	GL11::glScalef(var20, var20, var20);
	GL11::glTranslatef(-4.0F, 0.0F, 0.0F);
	GL11::glNormal3f(var20, 0.0F, 0.0F);
	var10->startDrawingQuads();
	var10->addVertexWithUV(-7.0, -2.0, -2.0, (double)var16, (double)var18);
	var10->addVertexWithUV(-7.0, -2.0, 2.0, (double)var17, (double)var18);
	var10->addVertexWithUV(-7.0, 2.0, 2.0, (double)var17, (double)var19);
	var10->addVertexWithUV(-7.0, 2.0, -2.0, (double)var16, (double)var19);
	var10->draw();
	GL11::glNormal3f(-var20, 0.0F, 0.0F);
	var10->startDrawingQuads();
	var10->addVertexWithUV(-7.0, 2.0, -2.0, (double)var16, (double)var18);
	var10->addVertexWithUV(-7.0, 2.0, 2.0, (double)var17, (double)var18);
	var10->addVertexWithUV(-7.0, -2.0, 2.0, (double)var17, (double)var19);
	var10->addVertexWithUV(-7.0, -2.0, -2.0, (double)var16, (double)var19);
	var10->draw();
	for(int var23 = 0; var23 < 4; ++var23) {
	GL11::glRotatef(90.0F, 1.0F, 0.0F, 0.0F);
	GL11::glNormal3f(0.0F, 0.0F, var20);
	var10->startDrawingQuads();
	var10->addVertexWithUV(-8.0, -2.0, 0.0, (double)var12, (double)var14);
	var10->addVertexWithUV(8.0, -2.0, 0.0, (double)var13, (double)var14);
	var10->addVertexWithUV(8.0, 2.0, 0.0, (double)var13, (double)var15);
	var10->addVertexWithUV(-8.0, 2.0, 0.0, (double)var12, (double)var15);
	var10->draw();
	}
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	GL11::glPopMatrix();
}

void RenderArrow::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityArrow*)var1, var2, var4, var6, var8, var9);
}

void RenderArrow::initStatics()
{
}
