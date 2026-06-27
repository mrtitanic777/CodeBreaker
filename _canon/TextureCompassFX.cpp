#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TextureCompassFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureCompassFX.h"
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

TextureCompassFX::TextureCompassFX(Minecraft* var1)
	: TextureFX(Item::s_compass->getIconIndex((ItemStack*)NULL))
	, mc(NULL)
	, compassRotation(0)
	, compassRotationDelta(0)
{
	this->compassIconPixels = JArray<int >(256);
	this->mc = var1;
	this->tileImage = 1;
	try {
	BufferedImage* var2 = ImageIO::read(Class(typeid(Minecraft)).getResource("/gui/items.png"));
	int var3 = this->iconIndex % 16 * 16;
	int var4 = this->iconIndex / 16 * 16;
	var2->getRGB(var3, var4, 16, 16, this->compassIconPixels, 0, 16);
	} catch (IOException* var5) {
	var5->printStackTrace();
	}
}

TextureCompassFX::~TextureCompassFX()
{
}

void TextureCompassFX::onTick()
{
	for(int var1 = 0; var1 < 256; ++var1) {
	int var2 = (this->compassIconPixels[var1] >> 24) & 255;
	int var3 = (this->compassIconPixels[var1] >> 16) & 255;
	int var4 = (this->compassIconPixels[var1] >> 8) & 255;
	int var5 = (this->compassIconPixels[var1] >> 0) & 255;
	if(this->anaglyphEnabled) {
	int var6 = (var3 * 30 + var4 * 59 + var5 * 11) / 100;
	int var7 = (var3 * 30 + var4 * 70) / 100;
	int var8 = (var3 * 30 + var5 * 70) / 100;
	var3 = var6;
	var4 = var7;
	var5 = var8;
	}
	this->imageData[var1 * 4 + 0] = (unsigned char)var3;
	this->imageData[var1 * 4 + 1] = (unsigned char)var4;
	this->imageData[var1 * 4 + 2] = (unsigned char)var5;
	this->imageData[var1 * 4 + 3] = (unsigned char)var2;
	}
	double var20 = 0.0;
	double var21;
	double var22;
	if(this->mc->theWorld != NULL && this->mc->thePlayer != NULL) {
	var21 = (double)this->mc->theWorld->spawnX - this->mc->thePlayer->posX;
	var22 = (double)this->mc->theWorld->spawnZ - this->mc->thePlayer->posZ;
	var20 = (double)(this->mc->thePlayer->rotationYaw - 90.0F) * 3.1415927F / 180.0 - atan2f(var22, var21);
	if(this->mc->theWorld->worldProvider->isHellWorld) {
	var20 = javaMathRandom() * (double)((float)3.1415927F) * 2.0;
	}
	}
	for(var21 = var20 - this->compassRotation; var21 < -3.1415927F; var21 += 3.1415927F * 2.0) {
	}
	while(var21 >= 3.1415927F) {
	var21 -= 3.1415927F * 2.0;
	}
	if(var21 < -1.0) {
	var21 = -1.0;
	}
	if(var21 > 1.0) {
	var21 = 1.0;
	}
	this->compassRotationDelta += var21 * 0.1;
	this->compassRotationDelta *= 0.8;
	this->compassRotation += this->compassRotationDelta;
	var22 = sinf(this->compassRotation);
	double var23 = cosf(this->compassRotation);
	int var9;
	int var10;
	int var11;
	int var12;
	int var13;
	int var14;
	int var15;
	short var16;
	int var17;
	int var18;
	int var19;
	for(var9 = -4; var9 <= 4; ++var9) {
	var10 = (int)(8.5 + var23 * (double)var9 * 0.3);
	var11 = (int)(7.5 - var22 * (double)var9 * 0.3 * 0.5);
	var12 = var11 * 16 + var10;
	var13 = 100;
	var14 = 100;
	var15 = 100;
	var16 = 255;
	if(this->anaglyphEnabled) {
	var17 = (var13 * 30 + var14 * 59 + var15 * 11) / 100;
	var18 = (var13 * 30 + var14 * 70) / 100;
	var19 = (var13 * 30 + var15 * 70) / 100;
	var13 = var17;
	var14 = var18;
	var15 = var19;
	}
	this->imageData[var12 * 4 + 0] = (unsigned char)var13;
	this->imageData[var12 * 4 + 1] = (unsigned char)var14;
	this->imageData[var12 * 4 + 2] = (unsigned char)var15;
	this->imageData[var12 * 4 + 3] = (unsigned char)var16;
	}
	for(var9 = -8; var9 <= 16; ++var9) {
	var10 = (int)(8.5 + var22 * (double)var9 * 0.3);
	var11 = (int)(7.5 + var23 * (double)var9 * 0.3 * 0.5);
	var12 = var11 * 16 + var10;
	var13 = var9 >= 0 ? 255 : 100;
	var14 = var9 >= 0 ? 20 : 100;
	var15 = var9 >= 0 ? 20 : 100;
	var16 = 255;
	if(this->anaglyphEnabled) {
	var17 = (var13 * 30 + var14 * 59 + var15 * 11) / 100;
	var18 = (var13 * 30 + var14 * 70) / 100;
	var19 = (var13 * 30 + var15 * 70) / 100;
	var13 = var17;
	var14 = var18;
	var15 = var19;
	}
	this->imageData[var12 * 4 + 0] = (unsigned char)var13;
	this->imageData[var12 * 4 + 1] = (unsigned char)var14;
	this->imageData[var12 * 4 + 2] = (unsigned char)var15;
	this->imageData[var12 * 4 + 3] = (unsigned char)var16;
	}
}

void TextureCompassFX::initStatics()
{
}
