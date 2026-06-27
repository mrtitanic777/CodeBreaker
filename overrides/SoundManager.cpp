#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from SoundManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SoundManager.h"
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
#include "RenderManager.h"
#include "Session.h"
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
#include "Throwable.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  import paulscode.sound.SoundSystem;
 *  import paulscode.sound.SoundSystemConfig;
 *  import paulscode.sound.codecs.CodecJOrbis;
 *  import paulscode.sound.codecs.CodecWav;
 *  import paulscode.sound.libraries.LibraryLWJGLOpenAL;
 *  // SoundSystemConfig is the real paulscode static config registry; the methods used below are:
 *  //   SoundSystemConfig.addLibrary(LibraryLWJGLOpenAL.class);
 *  //   SoundSystemConfig.setCodec("ogg", CodecJOrbis.class);
 *  //   SoundSystemConfig.setCodec("mus", CodecMus.class);
 *  //   SoundSystemConfig.setCodec("wav", CodecWav.class);
 *  // sndSystem.backgroundMusic("BgMusic", var1.soundUrl, var1.soundName, false);  streams a looping track
 *  ---- end original Java ---- */
// PORT: paulscode SoundSystem support classes have no runtime stub header (see
// "runtime-stub gaps"). SoundSystemConfig is a static config registry in Java; modeled here
// as a single config object with varargs no-op methods. The LibraryLWJGLOpenAL / CodecWav
// `.class` literals are passed as Class() (same as the ogg/mus codecs the generator emitted).
namespace {
	struct _SoundSystemConfig {
		void addLibrary(...) {}
		void setCodec(...) {}
	} SoundSystemConfig;
	// SoundSystem stub lacks backgroundMusic(); provide it as a faithful no-op free helper
	// (the real method streams a looping track on the named channel).
	inline void _sndBackgroundMusic(SoundSystem*, ...) {}
}

SoundSystem* SoundManager::s_sndSystem = NULL;
bool SoundManager::s_loaded = false;

SoundManager::~SoundManager()
{
}

void SoundManager::loadSoundSettings(GameSettings* var1)
{
	this->soundPoolStreaming->stripDigitsFromName = false;
	this->options = var1;
	if(!s_loaded && (var1 == NULL || var1->soundVolume != 0.0F || var1->musicVolume != 0.0F)) {
	this->tryToSetLibraryAndCodecs();
	}
}

void SoundManager::tryToSetLibraryAndCodecs()
{
	try {
	float var1 = this->options->soundVolume;
	float var2 = this->options->musicVolume;
	this->options->soundVolume = 0.0F;
	this->options->musicVolume = 0.0F;
	this->options->saveOptions();
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  SoundSystemConfig.addLibrary(LibraryLWJGLOpenAL.class);
	 *  SoundSystemConfig.setCodec("ogg", CodecJOrbis.class);
	 *  SoundSystemConfig.setCodec("mus", CodecMus.class);
	 *  SoundSystemConfig.setCodec("wav", CodecWav.class);
	 *  ---- end original Java ---- */
	SoundSystemConfig.addLibrary(Class());
	SoundSystemConfig.setCodec("ogg", Class());
	SoundSystemConfig.setCodec("mus", Class());
	SoundSystemConfig.setCodec("wav", Class());
	s_sndSystem = new SoundSystem();
	this->options->soundVolume = var1;
	this->options->musicVolume = var2;
	this->options->saveOptions();
	} catch (Throwable var3) {
	var3.printStackTrace();
	System::err.println("error linking with the LibraryJavaSound plug-in");
	}
	s_loaded = true;
}

void SoundManager::onSoundOptionsChanged()
{
	if(!s_loaded && (this->options->soundVolume != 0.0F || this->options->musicVolume != 0.0F)) {
	this->tryToSetLibraryAndCodecs();
	}
	if(this->options->musicVolume == 0.0F) {
	s_sndSystem->stop("BgMusic");
	} else {
	s_sndSystem->setVolume("BgMusic", this->options->musicVolume);
	}
}

void SoundManager::closeMinecraft()
{
	if(s_loaded) {
	s_sndSystem->cleanup();
	}
}

void SoundManager::addSound(const JavaString& var1, File* var2)
{
	this->soundPoolSounds->addSound(var1, var2);
}

void SoundManager::addStreaming(const JavaString& var1, File* var2)
{
	this->soundPoolStreaming->addSound(var1, var2);
}

void SoundManager::addMusic(const JavaString& var1, File* var2)
{
	this->soundPoolMusic->addSound(var1, var2);
}

void SoundManager::playRandomMusic()
{
	if(s_loaded && this->options->musicVolume != 0.0F) {
	if(!s_sndSystem->playing("BgMusic") && !s_sndSystem->playing("streaming")) {
	if(this->nextMusicDelay > 0) {
	--this->nextMusicDelay;
	return;
	}
	SoundPoolEntry* var1 = this->soundPoolMusic->getRandomSound();
	if(var1 != NULL) {
	this->nextMusicDelay = this->rand->nextInt(12000) + 12000;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  sndSystem.backgroundMusic("BgMusic", var1.soundUrl, var1.soundName, false);
	 *  ---- end original Java ---- */
	_sndBackgroundMusic(s_sndSystem, "BgMusic", var1->soundUrl, var1->soundName, false);
	s_sndSystem->setVolume("BgMusic", this->options->musicVolume);
	s_sndSystem->play("BgMusic");
	}
	}
	}
}

void SoundManager::setListener(EntityLiving* var1, float var2)
{
	if(s_loaded && this->options->soundVolume != 0.0F) {
	if(var1 != NULL) {
	float var3 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
	double var4 = var1->prevPosX + (var1->posX - var1->prevPosX) * (double)var2;
	double var6 = var1->prevPosY + (var1->posY - var1->prevPosY) * (double)var2;
	double var8 = var1->prevPosZ + (var1->posZ - var1->prevPosZ) * (double)var2;
	float var10 = MathHelper::cos(-var3 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var11 = MathHelper::sin(-var3 * ((float)3.1415927F / 180.0F) - (float)3.1415927F);
	float var12 = -var11;
	float var13 = 0.0F;
	float var14 = -var10;
	float var15 = 0.0F;
	float var16 = 1.0F;
	float var17 = 0.0F;
	s_sndSystem->setListenerPosition((float)var4, (float)var6, (float)var8);
	s_sndSystem->setListenerOrientation(var12, var13, var14, var15, var16, var17);
	}
	}
}

void SoundManager::playStreaming(const JavaString& var1, float var2, float var3, float var4, float var5, float var6)
{
	if(s_loaded && this->options->soundVolume != 0.0F) {
	JavaString var7 = "streaming";
	if(s_sndSystem->playing("streaming")) {
	s_sndSystem->stop("streaming");
	}
	if(var1 != NULL) {
	SoundPoolEntry* var8 = this->soundPoolStreaming->getRandomSoundFromSoundPool(var1);
	if(var8 != NULL && var5 > 0.0F) {
	if(s_sndSystem->playing("BgMusic")) {
	s_sndSystem->stop("BgMusic");
	}
	float var9 = 16.0F;
	s_sndSystem->newStreamingSource(true, var7, var8->soundUrl, var8->soundName, false, var2, var3, var4, 2, var9 * 4.0F);
	s_sndSystem->setVolume(var7, 0.5F * this->options->soundVolume);
	s_sndSystem->play(var7);
	}
	}
	}
}

void SoundManager::playSound(const JavaString& var1, float var2, float var3, float var4, float var5, float var6)
{
	if(s_loaded && this->options->soundVolume != 0.0F) {
	SoundPoolEntry* var7 = this->soundPoolSounds->getRandomSoundFromSoundPool(var1);
	if(var7 != NULL && var5 > 0.0F) {
	this->soundCounter = (this->soundCounter + 1) % 256;
	JavaString var8 = (JCat() + "sound_" + this->soundCounter);
	float var9 = 16.0F;
	if(var5 > 1.0F) {
	var9 *= var5;
	}
	s_sndSystem->newSource(var5 > 1.0F, var8, var7->soundUrl, var7->soundName, false, var2, var3, var4, 2, var9);
	s_sndSystem->setPitch(var8, var6);
	if(var5 > 1.0F) {
	var5 = 1.0F;
	}
	s_sndSystem->setVolume(var8, var5 * this->options->soundVolume);
	s_sndSystem->play(var8);
	}
	}
}

void SoundManager::playSoundFX(const JavaString& var1, float var2, float var3)
{
	if(s_loaded && this->options->soundVolume != 0.0F) {
	SoundPoolEntry* var4 = this->soundPoolSounds->getRandomSoundFromSoundPool(var1);
	if(var4 != NULL) {
	this->soundCounter = (this->soundCounter + 1) % 256;
	JavaString var5 = (JCat() + "sound_" + this->soundCounter);
	s_sndSystem->newSource(false, var5, var4->soundUrl, var4->soundName, false, 0.0F, 0.0F, 0.0F, 0, 0.0F);
	if(var2 > 1.0F) {
	var2 = 1.0F;
	}
	var2 *= 0.25F;
	s_sndSystem->setPitch(var5, var3);
	s_sndSystem->setVolume(var5, var2 * this->options->soundVolume);
	s_sndSystem->play(var5);
	}
	}
}
