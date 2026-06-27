#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderFireball.h"
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
#include "EntityFireball.h"
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

RenderFireball::~RenderFireball()
{
}

void RenderFireball::a(EntityFireball* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	float var10 = 2.0F;
	GL11::glScalef(var10 / 1.0F, var10 / 1.0F, var10 / 1.0F);
	int var11 = Item::s_snowball->getIconIndex((ItemStack*)NULL);
	this->loadTexture("/gui/items.png");
	Tessellator* var12 = Tessellator::s_instance;
	float var13 = (float)(var11 % 16 * 16 + 0) / 256.0F;
	float var14 = (float)(var11 % 16 * 16 + 16) / 256.0F;
	float var15 = (float)(var11 / 16 * 16 + 0) / 256.0F;
	float var16 = (float)(var11 / 16 * 16 + 16) / 256.0F;
	float var17 = 1.0F;
	float var18 = 0.5F;
	float var19 = 0.25F;
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
}

void RenderFireball::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityFireball*)var1, var2, var4, var6, var8, var9);
}

void RenderFireball::initStatics()
{
}
