#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TextureWatchFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureWatchFX.h"
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
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "ImageIO.h"
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
#include "TextureFX.h"
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

TextureWatchFX::TextureWatchFX(Minecraft* var1)
	: TextureFX(Item::s_pocketSundial->getIconIndex((ItemStack*)NULL))
	, mc(NULL)
	, rotation(0)
	, rotationVelocity(0)
{
	this->itemPixels = JArray<int >(256);
	this->dialPixels = JArray<int >(256);
	this->mc = var1;
	this->tileImage = 1;
	try {
	BufferedImage* var2 = ImageIO::read(Class(typeid(Minecraft)).getResource("/gui/items.png"));
	int var3 = this->iconIndex % 16 * 16;
	int var4 = this->iconIndex / 16 * 16;
	var2->getRGB(var3, var4, 16, 16, this->itemPixels, 0, 16);
	var2 = ImageIO::read(Class(typeid(Minecraft)).getResource("/misc/dial.png"));
	var2->getRGB(0, 0, 16, 16, this->dialPixels, 0, 16);
	} catch (IOException* var5) {
	var5->printStackTrace();
	}
}

TextureWatchFX::~TextureWatchFX()
{
}

void TextureWatchFX::onTick()
{
	double var1 = 0.0;
	if(this->mc->theWorld != NULL && this->mc->thePlayer != NULL) {
	float var3 = this->mc->theWorld->getCelestialAngle(1.0F);
	var1 = (double)(-var3 * (float)3.1415927F * 2.0F);
	if(this->mc->theWorld->worldProvider->isHellWorld) {
	var1 = javaMathRandom() * (double)((float)3.1415927F) * 2.0;
	}
	}
	double var22;
	for(var22 = var1 - this->rotation; var22 < -3.1415927F; var22 += 3.1415927F * 2.0) {
	}
	while(var22 >= 3.1415927F) {
	var22 -= 3.1415927F * 2.0;
	}
	if(var22 < -1.0) {
	var22 = -1.0;
	}
	if(var22 > 1.0) {
	var22 = 1.0;
	}
	this->rotationVelocity += var22 * 0.1;
	this->rotationVelocity *= 0.8;
	this->rotation += this->rotationVelocity;
	double var5 = sinf(this->rotation);
	double var7 = cosf(this->rotation);
	for(int var9 = 0; var9 < 256; ++var9) {
	int var10 = (this->itemPixels[var9] >> 24) & 255;
	int var11 = (this->itemPixels[var9] >> 16) & 255;
	int var12 = (this->itemPixels[var9] >> 8) & 255;
	int var13 = (this->itemPixels[var9] >> 0) & 255;
	if(var11 == var13 && var12 == 0 && var13 > 0) {
	double var14 = -((double)(var9 % 16) / 15.0 - 0.5);
	double var16 = (double)(var9 / 16) / 15.0 - 0.5;
	int var18 = var11;
	int var19 = (int)((var14 * var7 + var16 * var5 + 0.5) * 16.0);
	int var20 = (int)((var16 * var7 - var14 * var5 + 0.5) * 16.0);
	int var21 = (var19 & 15) + (var20 & 15) * 16;
	var10 = (this->dialPixels[var21] >> 24) & 255;
	var11 = ((this->dialPixels[var21] >> 16) & 255) * var11 / 255;
	var12 = ((this->dialPixels[var21] >> 8) & 255) * var18 / 255;
	var13 = ((this->dialPixels[var21] >> 0) & 255) * var18 / 255;
	}
	if(this->anaglyphEnabled) {
	int var23 = (var11 * 30 + var12 * 59 + var13 * 11) / 100;
	int var15 = (var11 * 30 + var12 * 70) / 100;
	int var24 = (var11 * 30 + var13 * 70) / 100;
	var11 = var23;
	var12 = var15;
	var13 = var24;
	}
	this->imageData[var9 * 4 + 0] = (unsigned char)var11;
	this->imageData[var9 * 4 + 1] = (unsigned char)var12;
	this->imageData[var9 * 4 + 2] = (unsigned char)var13;
	this->imageData[var9 * 4 + 3] = (unsigned char)var10;
	}
}

void TextureWatchFX::initStatics()
{
}
