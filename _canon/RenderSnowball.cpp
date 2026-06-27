#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderSnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderSnowball.h"
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
#include "EntitySnowball.h"
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

RenderSnowball::~RenderSnowball()
{
}

void RenderSnowball::a(EntitySnowball* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	GL11::glEnable(GL12::GL_RESCALE_NORMAL);
	GL11::glScalef(0.5F, 0.5F, 0.5F);
	int var10 = Item::s_snowball->getIconIndex((ItemStack*)NULL);
	this->loadTexture("/gui/items.png");
	Tessellator* var11 = Tessellator::s_instance;
	float var12 = (float)(var10 % 16 * 16 + 0) / 256.0F;
	float var13 = (float)(var10 % 16 * 16 + 16) / 256.0F;
	float var14 = (float)(var10 / 16 * 16 + 0) / 256.0F;
	float var15 = (float)(var10 / 16 * 16 + 16) / 256.0F;
	float var16 = 1.0F;
	float var17 = 0.5F;
	float var18 = 0.25F;
	GL11::glRotatef(180.0F - this->renderManager->playerViewYaw, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-this->renderManager->playerViewPitch, 1.0F, 0.0F, 0.0F);
	var11->startDrawingQuads();
	var11->setNormal(0.0F, 1.0F, 0.0F);
	var11->addVertexWithUV((double)(0.0F - var17), (double)(0.0F - var18), 0.0, (double)var12, (double)var15);
	var11->addVertexWithUV((double)(var16 - var17), (double)(0.0F - var18), 0.0, (double)var13, (double)var15);
	var11->addVertexWithUV((double)(var16 - var17), (double)(1.0F - var18), 0.0, (double)var13, (double)var14);
	var11->addVertexWithUV((double)(0.0F - var17), (double)(1.0F - var18), 0.0, (double)var12, (double)var14);
	var11->draw();
	GL11::glDisable(GL12::GL_RESCALE_NORMAL);
	GL11::glPopMatrix();
}

void RenderSnowball::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntitySnowball*)var1, var2, var4, var6, var8, var9);
}

void RenderSnowball::initStatics()
{
}
