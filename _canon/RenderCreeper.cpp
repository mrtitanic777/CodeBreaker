#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderCreeper.h"
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
#include "EntityCreeper.h"
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
#include "ModelCreeper.h"
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
#include "RenderLiving.h"
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

RenderCreeper::RenderCreeper()
	: RenderLiving(new ModelCreeper(), 0.5F)
{
}

RenderCreeper::~RenderCreeper()
{
}

void RenderCreeper::a(EntityCreeper* var1, float var2)
{
	float var4 = var1->getCreeperFlashIntensity(var2);
	float var5 = 1.0F + MathHelper::sin(var4 * 100.0F) * var4 * 0.01F;
	if(var4 < 0.0F) {
	var4 = 0.0F;
	}
	if(var4 > 1.0F) {
	var4 = 1.0F;
	}
	var4 *= var4;
	var4 *= var4;
	float var6 = (1.0F + var4 * 0.4F) * var5;
	float var7 = (1.0F + var4 * 0.1F) / var5;
	GL11::glScalef(var6, var7, var6);
}

int RenderCreeper::a(EntityCreeper* var1, float var2, float var3)
{
	float var5 = var1->getCreeperFlashIntensity(var3);
	if((int)(var5 * 10.0F) % 2 == 0) {
	return 0;
	} else {
	int var6 = (int)(var5 * 0.2F * 255.0F);
	if(var6 < 0) {
	var6 = 0;
	}
	if(var6 > 255) {
	var6 = 255;
	}
	short var7 = 255;
	short var8 = 255;
	short var9 = 255;
	return (var6 << 24) | (var7 << 16) | (var8 << 8) | var9;
	}
}

void RenderCreeper::preRenderCallback(EntityLiving* var1, float var2)
{
	this->a((EntityCreeper*)var1, var2);
}

int RenderCreeper::getColorMultiplier(EntityLiving* var1, float var2, float var3)
{
	return this->a((EntityCreeper*)var1, var2, var3);
}

void RenderCreeper::initStatics()
{
}
