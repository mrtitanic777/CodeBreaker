#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderTNTPrimed.h"
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
#include "EntityTNTPrimed.h"
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

RenderTNTPrimed::RenderTNTPrimed()
	: blockRenderer(NULL)
{
	this->blockRenderer = new RenderBlocks();
	this->shadowSize = 0.5F;
}

RenderTNTPrimed::~RenderTNTPrimed()
{
}

void RenderTNTPrimed::a(EntityTNTPrimed* var1, double var2, double var4, double var6, float var8, float var9)
{
	GL11::glPushMatrix();
	GL11::glTranslatef((float)var2, (float)var4, (float)var6);
	float var10;
	if((float)var1->fuse - var9 + 1.0F < 10.0F) {
	var10 = 1.0F - ((float)var1->fuse - var9 + 1.0F) / 10.0F;
	if(var10 < 0.0F) {
	var10 = 0.0F;
	}
	if(var10 > 1.0F) {
	var10 = 1.0F;
	}
	var10 *= var10;
	var10 *= var10;
	float var11 = 1.0F + var10 * 0.3F;
	GL11::glScalef(var11, var11, var11);
	}
	var10 = (1.0F - ((float)var1->fuse - var9 + 1.0F) / 100.0F) * 0.8F;
	this->loadTexture("/terrain.png");
	this->blockRenderer->renderBlockAsItem(Block::s_tnt);
	if(var1->fuse / 5 % 2 == 0) {
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_DST_ALPHA);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, var10);
	this->blockRenderer->renderBlockAsItem(Block::s_tnt);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glEnable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	}
	GL11::glPopMatrix();
}

void RenderTNTPrimed::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityTNTPrimed*)var1, var2, var4, var6, var8, var9);
}

void RenderTNTPrimed::initStatics()
{
}
