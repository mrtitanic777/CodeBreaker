#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TexturePackList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturePackList.h"
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
#include "TexturePackCustom.h"
#include "TexturePackDefault.h"
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

TexturePackList::TexturePackList(Minecraft* var1, File* var2)
	: defaultTexturePack(NULL)
	, selectedTexturePack(NULL)
	, mc(NULL)
	, texturePackDir(NULL)
{
	// Java field initializer `defaultTexturePack = new TexturePackDefault()` -- this hand-written
	// override dropped it, so defaultTexturePack stayed NULL; updateAvailableTexturePacks() then left
	// selectedTexturePack == NULL and the ctor's selectedTexturePack->deleteTexturePack() below was a
	// virtual call through NULL (IP 0x0 access violation on boot). Restore it before any use.
	this->defaultTexturePack = new TexturePackDefault();
	this->mc = var1;
	this->texturePackDir = new File(var2, "texturepacks");
	if(!this->texturePackDir->exists()) {
	this->texturePackDir->mkdirs();
	}
	this->selectedTexturePackName = var1->gameSettings->skin;
	this->updateAvailableTexturePacks();
	this->selectedTexturePack->deleteTexturePack();
}

TexturePackList::~TexturePackList()
{
	delete texturePackDir;
	texturePackDir = NULL;
}

bool TexturePackList::setTexturePack(TexturePackBase* var1)
{
	if(var1 == this->selectedTexturePack) {
	return false;
	} else {
	this->selectedTexturePack->closeTexturePackFile();
	this->selectedTexturePackName = var1->texturePackFileName;
	this->selectedTexturePack = var1;
	this->mc->gameSettings->skin = this->selectedTexturePackName;
	this->mc->gameSettings->saveOptions();
	this->selectedTexturePack->deleteTexturePack();
	return true;
	}
}

void TexturePackList::updateAvailableTexturePacks()
{
	JList<void*> var1;
	this->selectedTexturePack = NULL;
	var1.push_back(this->defaultTexturePack);
	if(this->texturePackDir->exists() && this->texturePackDir->isDirectory()) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  File[] var2 = this.texturePackDir.listFiles();
	 *  File[] var3 = var2;
	 *  int var4 = var2.length;
	 *  ---- end original Java ---- */
	// File::listFiles() is a runtime stub returning void* (no real File[] enumeration); the
	// returned pointer is the raw buffer of a File* array. We adopt it as a JArray<File*>; the
	// stub currently yields null, so the loop is empty until a real listFiles() is provided.
	File** var2buf = (File**)this->texturePackDir->listFiles();
	JArray<File* > var2 = JArray<File* >(var2buf, 0);
	JArray<File* > var3 = var2;
	int var4 = var2.length;
	for(int var5 = 0; var5 < var4; ++var5) {
	File* var6 = var3[var5];
	if(var6->isFile() && JavaString(var6->getName()).toLowerCase().endsWith(".zip")) {
	JavaString var7 = (JCat() + var6->getName() + ":" + var6->length() + ":" + var6->lastModified());
	try {
	if(!this->texturePackCache.count(var7)) {
	TexturePackCustom* var8 = new TexturePackCustom(var6);
	var8->texturePackID = var7;
	this->texturePackCache[var7] = var8;
	var8->deleteTexturePackFile(this->mc);
	}
	TexturePackBase* var12 = (TexturePackBase*)this->texturePackCache[var7];
	if(var12->texturePackFileName.equals(this->selectedTexturePackName)) {
	this->selectedTexturePack = var12;
	}
	var1.push_back(var12);
	} catch (IOException var9) {
	var9.printStackTrace();
	}
	}
	}
	}
	if(this->selectedTexturePack == NULL) {
	this->selectedTexturePack = this->defaultTexturePack;
	}
	// Java: availableTexturePacks.removeAll(var1) -- drop every element also present in var1,
	// leaving only the now-stale packs. JList has no removeAll, so do it by hand.
	for(int var13 = 0; var13 < (int)var1.size(); ++var13) {
	this->availableTexturePacks.remove(var1[var13]);
	}
	// Java: for each remaining (stale) pack { unbind it; drop it from the cache }
	for(JList<void*>::iterator var10 = this->availableTexturePacks.begin(); var10 != this->availableTexturePacks.end(); ++var10) {
	TexturePackBase* var11 = (TexturePackBase*)(*var10);
	var11->bindTexturePack(this->mc);
	this->texturePackCache.erase(var11->texturePackID);
	}
	this->availableTexturePacks = var1;
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  public List availableTexturePacks() {
 *      return new ArrayList(this.availableTexturePacks);
 *  }
 *  ---- end original Java ---- */
// STUB: returns an empty list instead of a copy of this->availableTexturePacks.
JList<void*> TexturePackList::availableTexturePacks_()
{
	return JList<void*>();
}
