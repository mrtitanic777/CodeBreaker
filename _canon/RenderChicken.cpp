#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderChicken.h"
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
#include "EntityChicken.h"
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

RenderChicken::RenderChicken(ModelBase* var1, float var2)
	: RenderLiving(var1, var2)
{
}

RenderChicken::~RenderChicken()
{
}

void RenderChicken::a(EntityChicken* var1, double var2, double var4, double var6, float var8, float var9)
{
	RenderLiving::a(var1, var2, var4, var6, var8, var9);
}

float RenderChicken::a(EntityChicken* var1, float var2)
{
	float var3 = var1->prevWingRotation + (var1->wingRotation - var1->prevWingRotation) * var2;
	float var4 = var1->prevDestPos + (var1->destPos - var1->prevDestPos) * var2;
	return (MathHelper::sin(var3) + 1.0F) * var4;
}

float RenderChicken::handleRotationFloat(EntityLiving* var1, float var2)
{
	return this->a((EntityChicken*)var1, var2);
}

void RenderChicken::a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityChicken*)var1, var2, var4, var6, var8, var9);
}

void RenderChicken::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	this->a((EntityChicken*)var1, var2, var4, var6, var8, var9);
}

void RenderChicken::initStatics()
{
}
