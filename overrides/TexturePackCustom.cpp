#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TexturePackCustom.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturePackCustom.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "BufferedReader.h"
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
#include "Exception.h"
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
#include "ZipFile.h"

TexturePackCustom::TexturePackCustom(File* var1)
	: texturePackZipFile(NULL)
	, texturePackName(0)
	, texturePackThumbnail(NULL)
	, texturePackFile(NULL)
{
	this->texturePackName = -1;

	this->texturePackFileName = var1->getName();
	this->texturePackFile = var1;
}

TexturePackCustom::~TexturePackCustom()
{
}

JavaString TexturePackCustom::trimDescription(const JavaString& var1_)
{
	JavaString var1 = var1_;
	if(var1 != NULL && var1.length() > 34) {
	var1 = var1.substr(0, (34) - (0));
	}
	return var1;
}

void TexturePackCustom::deleteTexturePackFile(Minecraft* var1)
{
	ZipFile* var2 = NULL;
	InputStream* var3 = NULL;
	try {
	var2 = new ZipFile(this->texturePackFile);
	try {
	var3 = var2->getInputStream(var2->getEntry("pack.txt"));
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  BufferedReader var4 = new BufferedReader(new InputStreamReader(var3));
	 *  ---- end original Java ---- */
	// InputStreamReader has no runtime stub; BufferedReader(...) accepts the stream directly.
	BufferedReader* var4 = new BufferedReader(var3);
	this->firstDescriptionLine = this->trimDescription(var4->readLine());
	this->secondDescriptionLine = this->trimDescription(var4->readLine());
	var4->close();
	var3->close();
	} catch (Exception) {
	}
	try {
	var3 = var2->getInputStream(var2->getEntry("pack.png"));
	this->texturePackThumbnail = ImageIO::read(var3);
	var3->close();
	} catch (Exception) {
	}
	var2->close();
	} catch (Exception var21) {
	var21.printStackTrace();
	}
	// finally: runs on all paths (no early return/throw escapes the try above).
	try {
	var3->close();
	} catch (Exception) {
	}
	try {
	var2->close();
	} catch (Exception) {
	}
}

void TexturePackCustom::bindTexturePack(Minecraft* var1)
{
	if(this->texturePackThumbnail != NULL) {
	var1->renderEngine->deleteTexture(this->texturePackName);
	}
	this->closeTexturePackFile();
}

void TexturePackCustom::unbindTexturePack(Minecraft* var1)
{
	if(this->texturePackThumbnail != NULL && this->texturePackName < 0) {
	this->texturePackName = var1->renderEngine->allocateAndSetupTexture(this->texturePackThumbnail);
	}
	if(this->texturePackThumbnail != NULL) {
	var1->renderEngine->bindTexture(this->texturePackName);
	} else {
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var1->renderEngine->getTexture("/gui/unknown_pack.png"));
	}
}

void TexturePackCustom::deleteTexturePack()
{
	try {
	this->texturePackZipFile = new ZipFile(this->texturePackFile);
	} catch (Exception) {
	}
}

void TexturePackCustom::closeTexturePackFile()
{
	try {
	this->texturePackZipFile->close();
	} catch (Exception) {
	}
	this->texturePackZipFile = NULL;
}

InputStream* TexturePackCustom::getResourceAsStream(const JavaString& var1)
{
	try {
	void* var2 = this->texturePackZipFile->getEntry(var1.substr(1));
	if(var2 != NULL) {
	return this->texturePackZipFile->getInputStream(var2);
	}
	} catch (Exception) {
	}
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  return TexturePackBase.class.getResourceAsStream(var1);
	 *  ---- end original Java ---- */
	return Class().getResourceAsStream(var1);
}
