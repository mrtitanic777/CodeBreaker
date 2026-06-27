#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderEngine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderEngine.h"
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
#include "GLAllocation.h"
#include "GameSettings.h"
#include "Graphics.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "ImageBuffer.h"
#include "ImageIO.h"
#include "ImageObserver.h"
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
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "RuntimeException.h"
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

// --- Local shim (runtime-stub gap; see final report) ---
// GL_LINEAR_MIPMAP_LINEAR is absent from the GL11 stub.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // (org.lwjgl.opengl.GL11 constant used in setupTexture; not present in the GL11 stub header)
 *  GL11.GL_LINEAR_MIPMAP_LINEAR   // == 0x2703
 *  ---- end original Java ----
 *  Faithful value; reintroduced as a local constant so setupTexture's mipmap branch keeps the
 *  same GL_TEXTURE_MIN_FILTER value the original used. */
namespace {
	const int GL_LINEAR_MIPMAP_LINEAR_ = 0x2703;
}

bool RenderEngine::s_useMipmaps = false;

RenderEngine::RenderEngine(TexturePackList* var1, GameSettings* var2)
	: singleIntBuffer(NULL)
	, imageData(NULL)
	, options(NULL)
	, clampTexture(0)
	, blurTexture(0)
	, texturePackList(NULL)
{
	this->singleIntBuffer = GLAllocation::createDirectIntBuffer(1);
	this->imageData = GLAllocation::createDirectByteBuffer(1048576);

	this->texturePackList = var1;
	this->options = var2;
}

RenderEngine::~RenderEngine()
{
}

int RenderEngine::getTexture(const JavaString& var1)
{
	TexturePackBase* var2 = this->texturePackList->selectedTexturePack;
	std::map<std::string, int>::iterator var3it = this->textureMap.find(var1);
	if(var3it != this->textureMap.end()) {
	return var3it->second;
	} else {
	try {
	this->singleIntBuffer->clear();
	GLAllocation::generateTextureNames(this->singleIntBuffer);
	int var5 = this->singleIntBuffer->get(0);
	if(var1.startsWith("##")) {
	this->setupTexture(this->unwrapImageByColumns(this->readTextureImage(var2->getResourceAsStream(var1.substr(2)))), var5);
	} else if(var1.startsWith("%clamp%")) {
	this->clampTexture = true;
	this->setupTexture(this->readTextureImage(var2->getResourceAsStream(var1.substr(7))), var5);
	this->clampTexture = false;
	} else if(var1.startsWith("%blur%")) {
	this->blurTexture = true;
	this->setupTexture(this->readTextureImage(var2->getResourceAsStream(var1.substr(6))), var5);
	this->blurTexture = false;
	} else {
	this->setupTexture(this->readTextureImage(var2->getResourceAsStream(var1)), var5);
	}
	this->textureMap[var1] = Integer::valueOf(var5);
	return var5;
	} catch (IOException) {
	throw new RuntimeException("!!");
	}
	}
}

BufferedImage* RenderEngine::unwrapImageByColumns(BufferedImage* var1)
{
	if(var1 == NULL) return NULL;   // failed/missing decode -> no source image
	int var2 = var1->getWidth() / 16;
	BufferedImage* var3 = new BufferedImage(16, var1->getHeight() * var2, 2);
	// AWT Graphics is a no-op stub on console (getGraphics()==NULL -> drawImage would crash); copy
	// pixels directly. Java g.drawImage(var1,-col*16,col*H) stacks 16-wide columns vertically.
	int _h = var1->getHeight();
	for(int var5 = 0; var5 < var2; ++var5)
	for(int _y = 0; _y < _h; ++_y)
	for(int _x = 0; _x < 16; ++_x)
	var3->setRGB(_x, var5 * _h + _y, var1->getRGB(var5 * 16 + _x, _y));
	return var3;
}

int RenderEngine::allocateAndSetupTexture(BufferedImage* var1)
{
	this->singleIntBuffer->clear();
	GLAllocation::generateTextureNames(this->singleIntBuffer);
	int var2 = this->singleIntBuffer->get(0);
	this->setupTexture(var1, var2);
	this->textureNameToImageMap[var2] = (void*)var1;
	return var2;
}

void RenderEngine::setupTexture(BufferedImage* var1, int var2)
{
	if(var1 == NULL) return;   // failed/missing decode -> leave slot blank, don't crash on getWidth()
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var2);
	if(s_useMipmaps) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR_MIPMAP_LINEAR);
	 *  ---- end original Java ----
	 *  GL11.GL_LINEAR_MIPMAP_LINEAR is missing from the GL11 stub; uses the local shim constant. */
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR_);
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MAG_FILTER, GL11::GL_LINEAR);
	} else {
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MIN_FILTER, GL11::GL_NEAREST);
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MAG_FILTER, GL11::GL_NEAREST);
	}
	if(this->blurTexture) {
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MIN_FILTER, GL11::GL_LINEAR);
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_MAG_FILTER, GL11::GL_LINEAR);
	}
	if(this->clampTexture) {
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_WRAP_S, GL11::GL_CLAMP);
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_WRAP_T, GL11::GL_CLAMP);
	} else {
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_WRAP_S, GL11::GL_REPEAT);
	GL11::glTexParameteri(GL11::GL_TEXTURE_2D, GL11::GL_TEXTURE_WRAP_T, GL11::GL_REPEAT);
	}
	int var3 = var1->getWidth();
	int var4 = var1->getHeight();
	JArray<int > var5 = JArray<int >(var3 * var4);
	JArray<unsigned char > var6 = JArray<unsigned char >(var3 * var4 * 4);
	var1->getRGB(0, 0, var3, var4, var5, 0, var3);
	int var7;
	int var8;
	int var9;
	int var10;
	int var11;
	int var12;
	int var13;
	int var14;
	for(var7 = 0; var7 < var5.length; ++var7) {
	var8 = (var5[var7] >> 24) & 255;
	var9 = (var5[var7] >> 16) & 255;
	var10 = (var5[var7] >> 8) & 255;
	var11 = var5[var7] & 255;
	if(this->options != NULL && this->options->anaglyph) {
	var12 = (var9 * 30 + var10 * 59 + var11 * 11) / 100;
	var13 = (var9 * 30 + var10 * 70) / 100;
	var14 = (var9 * 30 + var11 * 70) / 100;
	var9 = var12;
	var10 = var13;
	var11 = var14;
	}
	var6[var7 * 4 + 0] = (unsigned char)var9;
	var6[var7 * 4 + 1] = (unsigned char)var10;
	var6[var7 * 4 + 2] = (unsigned char)var11;
	var6[var7 * 4 + 3] = (unsigned char)var8;
	}
	this->imageData->clear();
	this->imageData->put(var6);
	this->imageData->position(0).limit(var6.length);
	GL11::glTexImage2D(GL11::GL_TEXTURE_2D, 0, GL11::GL_RGBA, var3, var4, 0, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)this->imageData);
	if(s_useMipmaps) {
	for(var7 = 1; var7 <= 4; ++var7) {
	var8 = var3 >> (var7 - 1);
	var9 = var3 >> var7;
	var10 = var4 >> var7;
	for(var11 = 0; var11 < var9; ++var11) {
	for(var12 = 0; var12 < var10; ++var12) {
	var13 = this->imageData->getInt((var11 * 2 + 0 + (var12 * 2 + 0) * var8) * 4);
	var14 = this->imageData->getInt((var11 * 2 + 1 + (var12 * 2 + 0) * var8) * 4);
	int var15 = this->imageData->getInt((var11 * 2 + 1 + (var12 * 2 + 1) * var8) * 4);
	int var16 = this->imageData->getInt((var11 * 2 + 0 + (var12 * 2 + 1) * var8) * 4);
	int var17 = this->weightedAverageColor(this->weightedAverageColor(var13, var14), this->weightedAverageColor(var15, var16));
	this->imageData->putInt((var11 + var12 * var9) * 4, var17);
	}
	}
	GL11::glTexImage2D(GL11::GL_TEXTURE_2D, var7, GL11::GL_RGBA, var9, var10, 0, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)this->imageData);
	}
	}
}

void RenderEngine::deleteTexture(int var1)
{
	this->textureNameToImageMap.erase(var1);
	this->singleIntBuffer->clear();
	this->singleIntBuffer->put(var1);
	this->singleIntBuffer->flip();
	GL11::glDeleteTextures(this->singleIntBuffer);
}

int RenderEngine::getTextureForDownloadableImage(const JavaString& var1, const JavaString& var2)
{
	ThreadDownloadImageData* var3 = (ThreadDownloadImageData*)this->urlToImageDataMap[var1];
	if(var3 != NULL && var3->image != NULL && !var3->textureSetupComplete) {
	if(var3->textureName < 0) {
	var3->textureName = this->allocateAndSetupTexture(var3->image);
	} else {
	this->setupTexture(var3->image, var3->textureName);
	}
	var3->textureSetupComplete = true;
	}
	return var3 != NULL && var3->textureName >= 0 ? var3->textureName : this->getTexture(var2);
}

ThreadDownloadImageData* RenderEngine::obtainImageData(const JavaString& var1, ImageBuffer* var2)
{
	ThreadDownloadImageData* var3 = (ThreadDownloadImageData*)this->urlToImageDataMap[var1];
	if(var3 == NULL) {
	this->urlToImageDataMap[var1] = new ThreadDownloadImageData(var1, var2);
	} else {
	++var3->referenceCount;
	}
	return var3;
}

void RenderEngine::releaseImageData(const JavaString& var1)
{
	ThreadDownloadImageData* var2 = (ThreadDownloadImageData*)this->urlToImageDataMap[var1];
	if(var2 != NULL) {
	--var2->referenceCount;
	if(var2->referenceCount == 0) {
	if(var2->textureName >= 0) {
	this->deleteTexture(var2->textureName);
	}
	this->urlToImageDataMap.erase(var1);
	}
	}
}

void RenderEngine::registerTextureFX(TextureFX* var1)
{
	this->textureList.push_back(var1);
	var1->onTick();
}

void RenderEngine::updateDynamicTextures()
{
	int var1;
	TextureFX* var2;
	int var3;
	int var4;
	int var5;
	int var6;
	int var7;
	int var8;
	int var9;
	int var10;
	int var11;
	int var12;
	for(var1 = 0; var1 < this->textureList.size(); ++var1) {
	var2 = (TextureFX*)this->textureList[var1];
	var2->anaglyphEnabled = this->options->anaglyph;
	var2->onTick();
	this->imageData->clear();
	this->imageData->put(var2->imageData);
	this->imageData->position(0).limit(var2->imageData.length);
	var2->bindImage(this);
	for(var3 = 0; var3 < var2->tileSize; ++var3) {
	for(var4 = 0; var4 < var2->tileSize; ++var4) {
	GL11::glTexSubImage2D(GL11::GL_TEXTURE_2D, 0, var2->iconIndex % 16 * 16 + var3 * 16, var2->iconIndex / 16 * 16 + var4 * 16, 16, 16, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)this->imageData);
	if(s_useMipmaps) {
	for(var5 = 1; var5 <= 4; ++var5) {
	var6 = 16 >> (var5 - 1);
	var7 = 16 >> var5;
	for(var8 = 0; var8 < var7; ++var8) {
	for(var9 = 0; var9 < var7; ++var9) {
	var10 = this->imageData->getInt((var8 * 2 + 0 + (var9 * 2 + 0) * var6) * 4);
	var11 = this->imageData->getInt((var8 * 2 + 1 + (var9 * 2 + 0) * var6) * 4);
	var12 = this->imageData->getInt((var8 * 2 + 1 + (var9 * 2 + 1) * var6) * 4);
	int var13 = this->imageData->getInt((var8 * 2 + 0 + (var9 * 2 + 1) * var6) * 4);
	int var14 = this->averageColor(this->averageColor(var10, var11), this->averageColor(var12, var13));
	this->imageData->putInt((var8 + var9 * var7) * 4, var14);
	}
	}
	GL11::glTexSubImage2D(GL11::GL_TEXTURE_2D, var5, var2->iconIndex % 16 * var7, var2->iconIndex / 16 * var7, var7, var7, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, this->imageData);
	}
	}
	}
	}
	}
	for(var1 = 0; var1 < this->textureList.size(); ++var1) {
	var2 = (TextureFX*)this->textureList[var1];
	if(var2->bindTextureId > 0) {
	this->imageData->clear();
	this->imageData->put(var2->imageData);
	this->imageData->position(0).limit(var2->imageData.length);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var2->bindTextureId);
	GL11::glTexSubImage2D(GL11::GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)this->imageData);
	if(s_useMipmaps) {
	for(var3 = 1; var3 <= 4; ++var3) {
	var4 = 16 >> (var3 - 1);
	var5 = 16 >> var3;
	for(var6 = 0; var6 < var5; ++var6) {
	for(var7 = 0; var7 < var5; ++var7) {
	var8 = this->imageData->getInt((var6 * 2 + 0 + (var7 * 2 + 0) * var4) * 4);
	var9 = this->imageData->getInt((var6 * 2 + 1 + (var7 * 2 + 0) * var4) * 4);
	var10 = this->imageData->getInt((var6 * 2 + 1 + (var7 * 2 + 1) * var4) * 4);
	var11 = this->imageData->getInt((var6 * 2 + 0 + (var7 * 2 + 1) * var4) * 4);
	var12 = this->averageColor(this->averageColor(var8, var9), this->averageColor(var10, var11));
	this->imageData->putInt((var6 + var7 * var5) * 4, var12);
	}
	}
	GL11::glTexSubImage2D(GL11::GL_TEXTURE_2D, var3, 0, 0, var5, var5, GL11::GL_RGBA, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)this->imageData);
	}
	}
	}
	}
}

int RenderEngine::averageColor(int var1, int var2)
{
	int var3 = ((var1 & -16777216) >> 24) & 255;
	int var4 = ((var2 & -16777216) >> 24) & 255;
	return ((var3 + var4) >> 1 << 24) + (((var1 & 16711422) + (var2 & 16711422)) >> 1);
}

int RenderEngine::weightedAverageColor(int var1, int var2)
{
	int var3 = ((var1 & -16777216) >> 24) & 255;
	int var4 = ((var2 & -16777216) >> 24) & 255;
	short var5 = 255;
	if(var3 + var4 == 0) {
	var3 = 1;
	var4 = 1;
	var5 = 0;
	}
	int var6 = ((var1 >> 16) & 255) * var3;
	int var7 = ((var1 >> 8) & 255) * var3;
	int var8 = (var1 & 255) * var3;
	int var9 = ((var2 >> 16) & 255) * var4;
	int var10 = ((var2 >> 8) & 255) * var4;
	int var11 = (var2 & 255) * var4;
	int var12 = (var6 + var9) / (var3 + var4);
	int var13 = (var7 + var10) / (var3 + var4);
	int var14 = (var8 + var11) / (var3 + var4);
	return (var5 << 24) | (var12 << 16) | (var13 << 8) | var14;
}

void RenderEngine::refreshTextures()
{
	TexturePackBase* var1 = this->texturePackList->selectedTexturePack;
	BufferedImage* var4;
	for(std::map<int, void*>::iterator it = this->textureNameToImageMap.begin(); it != this->textureNameToImageMap.end(); ++it) {
	int var3 = it->first;
	var4 = (BufferedImage*)this->textureNameToImageMap[var3];
	this->setupTexture(var4, var3);
	}
	ThreadDownloadImageData* var7;
	for(std::map<std::string, void*>::iterator it = this->urlToImageDataMap.begin(); it != this->urlToImageDataMap.end(); ++it) {
	var7 = (ThreadDownloadImageData*)it->second;
	var7->textureSetupComplete = false;
	}
	for(std::map<std::string, int>::iterator it = this->textureMap.begin(); it != this->textureMap.end(); ++it) {
	JavaString var8 = JavaString(it->first);
	try {
	if(var8.startsWith("##")) {
	var4 = this->unwrapImageByColumns(this->readTextureImage(var1->getResourceAsStream(var8.substr(2))));
	} else if(var8.startsWith("%clamp%")) {
	this->clampTexture = true;
	var4 = this->readTextureImage(var1->getResourceAsStream(var8.substr(7)));
	} else if(var8.startsWith("%blur%")) {
	this->blurTexture = true;
	var4 = this->readTextureImage(var1->getResourceAsStream(var8.substr(6)));
	} else {
	var4 = this->readTextureImage(var1->getResourceAsStream(var8));
	}
	int var5 = this->textureMap[var8];
	this->setupTexture(var4, var5);
	this->blurTexture = false;
	this->clampTexture = false;
	} catch (IOException var6) {
	var6.printStackTrace();
	}
	}
}

BufferedImage* RenderEngine::readTextureImage(InputStream* var1)
{
	if(var1 == NULL) return NULL;   // missing resource -> no stream; don't deref/close NULL
	BufferedImage* var2 = ImageIO::read(var1);
	var1->close();
	return var2;
}

void RenderEngine::bindTexture(int var1)
{
	if(var1 >= 0) {
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var1);
	}
}
