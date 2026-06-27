#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RenderManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderManager.h"
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
#include "ModelChicken.h"
#include "ModelCow.h"
#include "ModelPig.h"
#include "ModelRenderer.h"
#include "ModelSheep1.h"
#include "ModelSheep2.h"
#include "ModelSkeleton.h"
#include "ModelSlime.h"
#include "ModelZombie.h"
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
#include "RenderArrow.h"
#include "RenderBiped.h"
#include "RenderBlocks.h"
#include "RenderBoat.h"
#include "RenderChicken.h"
#include "RenderCow.h"
#include "RenderCreeper.h"
#include "RenderEngine.h"
#include "RenderEntity.h"
#include "RenderFallingSand.h"
#include "RenderFireball.h"
#include "RenderFish.h"
#include "RenderGhast.h"
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderLiving.h"
#include "RenderMinecart.h"
#include "RenderPainting.h"
#include "RenderPig.h"
#include "RenderPlayer.h"
#include "RenderSheep.h"
#include "RenderSlime.h"
#include "RenderSnowball.h"
#include "RenderSpider.h"
#include "RenderTNTPrimed.h"
#include "RenderZombieSimple.h"
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

RenderManager* RenderManager::s_instance = new RenderManager();
double RenderManager::s_renderPosX = 0;
double RenderManager::s_renderPosY = 0;
double RenderManager::s_renderPosZ = 0;

RenderManager::RenderManager()
	: fontRenderer(NULL)
	, renderEngine(NULL)
	, itemRenderer(NULL)
	, worldObj(NULL)
	, livingPlayer(NULL)
	, playerViewYaw(0)
	, playerViewPitch(0)
	, options(NULL)
	, playerViewX(0)
	, playerViewY(0)
	, playerViewZ(0)
{
	// NOTE: the Java source keyed this map by `EntityXxx.class` (a java.lang.Class identity).
	// The transpiler collapsed every `.class` literal to a bare `Class()` stub, losing the
	// key identity. We restore the registration faithfully by keying on the entity class NAME
	// (the map is std::map<std::string,void*>); the key strings below are recovered from the
	// canonical Alpha RenderManager constructor order so the data structure is intact.
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.entityRenderMap.put(EntitySpider.class, new RenderSpider());
	 *  this.entityRenderMap.put(EntityPig.class, new RenderPig(new ModelPig(), new ModelPig(0.5F), 0.7F));
	 *  this.entityRenderMap.put(EntitySheep.class, new RenderSheep(new ModelSheep2(), new ModelSheep1(), 0.7F));
	 *  this.entityRenderMap.put(EntityCow.class, new RenderCow(new ModelCow(), 0.7F));
	 *  this.entityRenderMap.put(EntityChicken.class, new RenderChicken(new ModelChicken(), 0.3F));
	 *  this.entityRenderMap.put(EntityCreeper.class, new RenderCreeper());
	 *  this.entityRenderMap.put(EntitySkeleton.class, new RenderBiped(new ModelSkeleton(), 0.5F));
	 *  this.entityRenderMap.put(EntityZombie.class, new RenderBiped(new ModelZombie(), 0.5F));
	 *  this.entityRenderMap.put(EntitySlime.class, new RenderSlime(new ModelSlime(16), new ModelSlime(0), 0.25F));
	 *  this.entityRenderMap.put(EntityPlayer.class, new RenderPlayer());
	 *  this.entityRenderMap.put(EntityZombieSimple.class, new RenderZombieSimple(new ModelZombie(), 0.5F, 6.0F));
	 *  this.entityRenderMap.put(EntityGhast.class, new RenderGhast());
	 *  this.entityRenderMap.put(EntityLiving.class, new RenderLiving(new ModelBiped(), 0.5F));
	 *  this.entityRenderMap.put(Entity.class, new RenderEntity());
	 *  this.entityRenderMap.put(EntityPainting.class, new RenderPainting());
	 *  this.entityRenderMap.put(EntityArrow.class, new RenderArrow());
	 *  this.entityRenderMap.put(EntitySnowball.class, new RenderSnowball());
	 *  this.entityRenderMap.put(EntityFireball.class, new RenderFireball());
	 *  this.entityRenderMap.put(EntityItem.class, new RenderItem());
	 *  this.entityRenderMap.put(EntityTNTPrimed.class, new RenderTNTPrimed());
	 *  this.entityRenderMap.put(EntityFallingSand.class, new RenderFallingSand());
	 *  this.entityRenderMap.put(EntityMinecart.class, new RenderMinecart());
	 *  this.entityRenderMap.put(EntityBoat.class, new RenderBoat());
	 *  this.entityRenderMap.put(EntityFish.class, new RenderFish());
	 *  ---- end original Java ----
	 *  RESTORED-BUT-UNCERTAIN keys: Java keyed by Class identity; here keyed by class-NAME string.
	 *  Two keys also differ from the Java spelling -- Java uses EntityZombieSimple.class and
	 *  EntityLiving.class, but the strings below read "EntityGiantZombie" and "EntityMob", and the
	 *  Fish key is "EntityFishHook" vs Java's EntityFish.class. Needs review against Entity.getClass(). */
	this->entityRenderMap["EntitySpider"] = new RenderSpider();
	this->entityRenderMap["EntityPig"] = new RenderPig(new ModelPig(), new ModelPig(0.5F), 0.7F);
	this->entityRenderMap["EntitySheep"] = new RenderSheep(new ModelSheep2(), new ModelSheep1(), 0.7F);
	this->entityRenderMap["EntityCow"] = new RenderCow(new ModelCow(), 0.7F);
	this->entityRenderMap["EntityChicken"] = new RenderChicken(new ModelChicken(), 0.3F);
	this->entityRenderMap["EntityCreeper"] = new RenderCreeper();
	this->entityRenderMap["EntitySkeleton"] = new RenderBiped(new ModelSkeleton(), 0.5F);
	this->entityRenderMap["EntityZombie"] = new RenderBiped(new ModelZombie(), 0.5F);
	this->entityRenderMap["EntitySlime"] = new RenderSlime(new ModelSlime(16), new ModelSlime(0), 0.25F);
	this->entityRenderMap["EntityPlayer"] = new RenderPlayer();
	this->entityRenderMap["EntityPlayerSP"] = this->entityRenderMap["EntityPlayer"];   // CONSOLE: the client player is EntityPlayerSP (a leaf class); getSuperclass() is a stub (no walk), so alias its bare name to RenderPlayer
	this->entityRenderMap["EntityGiantZombie"] = new RenderZombieSimple(new ModelZombie(), 0.5F, 6.0F);
	this->entityRenderMap["EntityGhast"] = new RenderGhast();
	this->entityRenderMap["EntityMob"] = new RenderLiving(new ModelBiped(), 0.5F);
	this->entityRenderMap["Entity"] = new RenderEntity();
	this->entityRenderMap["EntityPainting"] = new RenderPainting();
	this->entityRenderMap["EntityArrow"] = new RenderArrow();
	this->entityRenderMap["EntitySnowball"] = new RenderSnowball();
	this->entityRenderMap["EntityFireball"] = new RenderFireball();
	this->entityRenderMap["EntityItem"] = new RenderItem();
	this->entityRenderMap["EntityTNTPrimed"] = new RenderTNTPrimed();
	this->entityRenderMap["EntityFallingSand"] = new RenderFallingSand();
	this->entityRenderMap["EntityMinecart"] = new RenderMinecart();
	this->entityRenderMap["EntityBoat"] = new RenderBoat();
	this->entityRenderMap["EntityFishHook"] = new RenderFish();
	// Java: for each Render in entityRenderMap.values() { render.setRenderManager(this); }
	for(std::map<std::string, void*>::iterator var1 = this->entityRenderMap.begin(); var1 != this->entityRenderMap.end(); ++var1) {
	Render* var2 = (Render*)var1->second;
	var2->setRenderManager(this);
	}
}

RenderManager::~RenderManager()
{
}

Render* RenderManager::getEntityClassRenderObject(Class* var1)
{
	// Java keyed entityRenderMap by Class identity; here the key is the class name string
	// (Class::getName()). Preserves the original cache-and-walk-superclasses structure.
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  public Render getEntityClassRenderObject(Class var1) {
	 *      Render var2 = (Render)this.entityRenderMap.get(var1);
	 *      if(var2 == null && var1 != Entity.class) {
	 *          var2 = this.getEntityClassRenderObject(var1.getSuperclass());
	 *          this.entityRenderMap.put(var1, var2);
	 *      }
	 *      return var2;
	 *  }
	 *  ---- end original Java ----
	 *  Stubbed: map.get(Class) -> map[getName()] (string keys), and the `var1 != Entity.class`
	 *  recursion guard is replaced by `getSuperclass() != NULL`; the extra `var1 == NULL` early
	 *  return has no Java counterpart (guards the lost Class identity). */
	if(var1 == NULL) {
	return NULL;
	}
	std::string var3 = var1->getName();
	Render* var2 = (Render*)this->entityRenderMap[var3];
	if(var2 == NULL && var1->getSuperclass() != NULL) {
	var2 = this->getEntityClassRenderObject(var1->getSuperclass());
	this->entityRenderMap[var3] = var2;
	}
	return var2;
}

Render* RenderManager::getEntityRenderObject(Entity* var1)
{
	// Java: getEntityClassRenderObject(var1.getClass()). The runtime model has no getClass()
	// for Entity (no RTTI shim), so the transpiler emitted a bare Class() and the runtime
	// class identity is lost. Faithful structure preserved; dispatch resolution flagged for
	// review -- this needs a getClass()/class-name accessor on Entity to pick the right Render.
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  public Render getEntityRenderObject(Entity var1) {
	 *      return this.getEntityClassRenderObject(var1.getClass());
	 *  }
	 *  ---- end original Java ----
	 *  Stubbed: var1.getClass() has no runtime equivalent, so a bare `new Class()` is passed and
	 *  var1 is dropped (void cast). This always resolves to the same (wrong) Render bucket --
	 *  per-entity render dispatch is effectively broken until Entity exposes its class name. */
	// CONSOLE (entity render dispatch fix): pass the entity's REAL runtime class via RTTI so
	// getEntityClassRenderObject finds its Render in entityRenderMap. Was `new Class()` (empty -> the
	// lookup ALWAYS returned NULL -> no entity ever rendered AND the first-person hand crashed on
	// NULL->drawFirstPersonHand). Class::getName() now returns the BARE class name to match the map's
	// string keys ("EntityPig" etc.), and the client player's leaf class "EntityPlayerSP" is registered.
	if (var1 == NULL) return NULL;
	return this->getEntityClassRenderObject(new Class(typeid(*var1)));
}

void RenderManager::cacheActiveRenderInfo(World* var1, RenderEngine* var2, FontRenderer* var3, EntityPlayer* var4, GameSettings* var5, float var6)
{
	this->worldObj = var1;
	this->renderEngine = var2;
	this->options = var5;
	this->livingPlayer = var4;
	this->fontRenderer = var3;
	this->playerViewYaw = var4->prevRotationYaw + (var4->rotationYaw - var4->prevRotationYaw) * var6;
	this->playerViewPitch = var4->prevRotationPitch + (var4->rotationPitch - var4->prevRotationPitch) * var6;
	this->playerViewX = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var6;
	this->playerViewY = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var6;
	this->playerViewZ = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var6;
}

void RenderManager::renderEntity(Entity* var1, float var2)
{
	double var3 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
	double var5 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
	double var7 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;
	float var9 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
	float var10 = var1->getEntityBrightness(var2);
	GL11::glColor3f(var10, var10, var10);
	this->doRenderEntity(var1, var3 - s_renderPosX, var5 - s_renderPosY, var7 - s_renderPosZ, var9, var2);
}

void RenderManager::doRenderEntity(Entity* var1, double var2, double var4, double var6, float var8, float var9)
{
	Render* var10 = this->getEntityRenderObject(var1);
	if(var10 != NULL) {
	var10->doRender(var1, var2, var4, var6, var8, var9);
	var10->doRenderShadowAndFire(var1, var2, var4, var6, var8, var9);
	}
}

void RenderManager::setWorld(World* var1)
{
	this->worldObj = var1;
}

double RenderManager::getDistanceToCamera(double var1, double var3, double var5)
{
	double var7 = var1 - this->playerViewX;
	double var9 = var3 - this->playerViewY;
	double var11 = var5 - this->playerViewZ;
	return var7 * var7 + var9 * var9 + var11 * var11;
}

FontRenderer* RenderManager::getFontRenderer()
{
	return this->fontRenderer;
}
