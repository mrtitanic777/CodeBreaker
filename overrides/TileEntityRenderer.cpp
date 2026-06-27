#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityRenderer.h"
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
#include "SignModel.h"
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
#include "TileEntityMobSpawnerRenderer.h"
#include "TileEntitySignRenderer.h"
#include "TileEntitySpecialRenderer.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

TileEntityRenderer* TileEntityRenderer::s_instance = new TileEntityRenderer();
double TileEntityRenderer::s_staticPlayerX = 0;
double TileEntityRenderer::s_staticPlayerY = 0;
double TileEntityRenderer::s_staticPlayerZ = 0;

TileEntityRenderer::TileEntityRenderer()
	: fontRenderer(NULL)
	, renderEngine(NULL)
	, worldObj(NULL)
	, entityPlayer(NULL)
	, playerYaw(0)
	, playerPitch(0)
	, playerX(0)
	, playerY(0)
	, playerZ(0)
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.specialRendererMap.put(TileEntitySign.class, new TileEntitySignRenderer());
	 *  this.specialRendererMap.put(TileEntityMobSpawner.class, new TileEntityMobSpawnerRenderer());
	 *  Iterator var1 = this.specialRendererMap.values().iterator();
	 *  while(var1.hasNext()) {
	 *      TileEntitySpecialRenderer var2 = (TileEntitySpecialRenderer)var1.next();
	 *      var2.setTileEntityRenderer(this);
	 *  }
	 *  ---- end original Java ---- */
	// Java keyed specialRendererMap by `TileEntityXxx.class`; the transpiler collapsed each
	// .class literal to a bare Class() stub. Restored faithfully by keying on the tile-entity
	// class NAME (the map is std::map<std::string,void*>).
	this->specialRendererMap["TileEntitySign"] = new TileEntitySignRenderer();
	this->specialRendererMap["TileEntityMobSpawner"] = new TileEntityMobSpawnerRenderer();
	// Java: for each renderer in specialRendererMap.values() { renderer.setTileEntityRenderer(this); }
	for(std::map<std::string, void*>::iterator var1 = this->specialRendererMap.begin(); var1 != this->specialRendererMap.end(); ++var1) {
	TileEntitySpecialRenderer* var2 = (TileEntitySpecialRenderer*)var1->second;
	var2->setTileEntityRenderer(this);
	}
}

TileEntityRenderer::~TileEntityRenderer()
{
}

TileEntitySpecialRenderer* TileEntityRenderer::getSpecialRendererForClass(Class* var1)
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  public TileEntitySpecialRenderer getSpecialRendererForClass(Class var1) {
	 *      TileEntitySpecialRenderer var2 = (TileEntitySpecialRenderer)this.specialRendererMap.get(var1);
	 *      if(var2 == null && var1 != TileEntity.class) {
	 *          var2 = this.getSpecialRendererForClass(var1.getSuperclass());
	 *          this.specialRendererMap.put(var1, var2);
	 *      }
	 *      return var2;
	 *  }
	 *  NOTE: Java keyed the map by the real Class object and stopped the recursion at
	 *  `var1 != TileEntity.class`. The port keys by Class::getName() and substitutes the
	 *  stop-condition with `var1->getSuperclass() != NULL` (class identity is lost).
	 *  ---- end original Java ---- */
	// Java keyed specialRendererMap by Class identity; here the key is the class name string
	// (Class::getName()). Preserves the original cache-and-walk-superclasses structure.
	if(var1 == NULL) {
	return NULL;
	}
	std::string var3 = var1->getName();
	TileEntitySpecialRenderer* var2 = (TileEntitySpecialRenderer*)this->specialRendererMap[var3];
	if(var2 == NULL && var1->getSuperclass() != NULL) {
	var2 = this->getSpecialRendererForClass(var1->getSuperclass());
	this->specialRendererMap[var3] = var2;
	}
	return var2;
}

bool TileEntityRenderer::hasSpecialRenderer(TileEntity* var1)
{
	return this->getSpecialRendererForEntity(var1) != NULL;
}

TileEntitySpecialRenderer* TileEntityRenderer::getSpecialRendererForEntity(TileEntity* var1)
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  return var1 == null ? null : this.getSpecialRendererForClass(var1.getClass());
	 *  NOTE: Java passes var1.getClass() (the entity's real runtime class) into the lookup.
	 *  The runtime model has no getClass(), so the port passes a fresh `new Class()` stub,
	 *  which never matches a registered key -> special renderers are effectively never found.
	 *  ---- end original Java ---- */
	// Java: getSpecialRendererForClass(var1.getClass()). TileEntity has no getClass() in the
	// runtime model (no RTTI shim), so identity is lost; structure preserved, resolution flagged.
	return var1 == NULL ? NULL : this->getSpecialRendererForClass(new Class());
}

void TileEntityRenderer::setRenderingContext(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, float var5)
{
	this->worldObj = var1;
	this->renderEngine = var2;
	this->entityPlayer = var4;
	this->fontRenderer = var3;
	this->playerYaw = var4->prevRotationYaw + (var4->rotationYaw - var4->prevRotationYaw) * var5;
	this->playerPitch = var4->prevRotationPitch + (var4->rotationPitch - var4->prevRotationPitch) * var5;
	this->playerX = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var5;
	this->playerY = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var5;
	this->playerZ = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var5;
}

void TileEntityRenderer::renderTileEntity(TileEntity* var1, float var2)
{
	if(var1->getDistanceFrom(this->playerX, this->playerY, this->playerZ) < 4096.0D) {
	float var3 = this->worldObj->getLightBrightness(var1->xCoord, var1->yCoord, var1->zCoord);
	GL11::glColor3f(var3, var3, var3);
	this->renderTileEntityAt(var1, (double)var1->xCoord - s_staticPlayerX, (double)var1->yCoord - s_staticPlayerY, (double)var1->zCoord - s_staticPlayerZ, var2);
	}
}

void TileEntityRenderer::renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8)
{
	TileEntitySpecialRenderer* var9 = this->getSpecialRendererForEntity(var1);
	if(var9 != NULL) {
	var9->renderTileEntityAt(var1, var2, var4, var6, var8);
	}
}

FontRenderer* TileEntityRenderer::getFontRenderer()
{
	return this->fontRenderer;
}
