#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRenderer.h"
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
#include "ClippingHelper.h"
#include "ClippingHelperImplementation.h"
#include "Component.h"
#include "Cursor.h"
#include "Display.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFX.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRainFX.h"
#include "EnumOS2.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "Frustrum.h"
#include "GL11.h"
#include "GLContext.h"
#include "GLU.h"
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
#include "Keyboard.h"
#include "LoadingScreenRenderer.h"
#include "Material.h"
#include "MathHelper.h"
#include "Minecraft.h"
#include "MinecraftApplet.h"
#include "MobSpawnerBase.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "Mouse.h"
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
#include "PlayerControllerTest.h"
#include "PositionTexureVertex.h"
#include "GLAllocation.h"
#include "Random.h"
#include "Render.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "RenderHelper.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "ScaledResolution.h"
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

// --- Local shims for symbols missing from the runtime stubs -------------------
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  import org.lwjgl.input.Keyboard;
 *  import org.lwjgl.opengl.GLContext;
 *  import org.lwjgl.opengl.NVFogDistance;
 *  // Keyboard.KEY_F1 is used in orientCamera/renderHand/updateCameraAndRender, e.g.:
 *  if(Keyboard.isKeyDown(Keyboard.KEY_F1)) { ... }
 *  // GLContext capability + NVFogDistance constants are used in setupFog():
 *  if(GLContext.getCapabilities().GL_NV_fog_distance) {
 *      GL11.glFogi(NVFogDistance.GL_FOG_DISTANCE_MODE_NV, NVFogDistance.GL_EYE_RADIAL_NV);
 *  }
 *  ---- end original Java ----
 *  KEY_F1 is hardcoded to LWJGL scancode 59; the NV_fog_distance capability is faked to
 *  ALWAYS false (_hasNvFogDistance), so the NVFogDistance branch in setupFog is dead code. */
namespace {
	// org.lwjgl.input.Keyboard.KEY_F1 (LWJGL scancode 59); absent from Keyboard stub.
	const int _KB_KEY_F1 = 59;
	// org.lwjgl.opengl.NVFogDistance extension constants (not on the D3D9 target).
	// Capability flag is false here, so the fog-distance branch is inert; constants
	// are provided only so the faithful structure compiles.
	const int _NV_GL_FOG_DISTANCE_MODE_NV = 0x855A;
	const int _NV_GL_EYE_RADIAL_NV = 0x855B;
	// GLContext capabilities lack GL_NV_fog_distance; the Xbox 360 D3D9 layer does
	// not expose this NVIDIA extension, so it is unconditionally unsupported.
	inline bool _hasNvFogDistance() { return false; }
}

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // instance-field initializers on the EntityRenderer class:
 *  private float farPlaneDistance = 0.0F;
 *  private Entity pointedEntity = null;
 *  private long prevFrameTime = System.currentTimeMillis();
 *  private Random random = new Random();
 *  FloatBuffer fogColorBuffer = GLAllocation.createDirectFloatBuffer(16);
 *  ---- end original Java ----
 *  The non-trivial Java field initializers are dropped: the init list below seeds
 *  prevFrameTime to 0 (not System.currentTimeMillis()), and leaves both random AND
 *  fogColorBuffer NULL -- never constructed. So addRainParticles() (random->nextInt)
 *  and setFogColorBuffer() (fogColorBuffer->clear) both null-deref. */
EntityRenderer::EntityRenderer(Minecraft* var1)
	: mc(NULL)
	, farPlaneDistance(0)
	, itemRenderer(NULL)
	, rendererUpdateCount(0)
	, pointedEntity(NULL)
	, prevFrameTime(0)
	, random(NULL)
	, field_1394_b(0)
	, field_1393_c(0)
	, fogColorBuffer(NULL)
	, fogColorRed(0)
	, fogColorGreen(0)
	, fogColorBlue(0)
	, prevFogLightBrightness(0)
	, fogLightBrightness(0)
{
	// restored Java field initializers (were dropped -> null-deref in addRainParticles/setFogColorBuffer)
	this->random = new Random();
	this->prevFrameTime = System::currentTimeMillis();
	this->fogColorBuffer = GLAllocation::createDirectFloatBuffer(16);
	this->mc = var1;
	this->itemRenderer = new ItemRenderer(var1);
}

EntityRenderer::~EntityRenderer()
{
	delete itemRenderer;
	itemRenderer = NULL;
}

void EntityRenderer::updateRenderer()
{
	this->prevFogLightBrightness = this->fogLightBrightness;
	float var1 = this->mc->theWorld->getLightBrightness(MathHelper::floor_double(this->mc->thePlayer->posX), MathHelper::floor_double(this->mc->thePlayer->posY), MathHelper::floor_double(this->mc->thePlayer->posZ));
	float var2 = (float)(3 - this->mc->gameSettings->renderDistance) / 3.0F;
	float var3 = var1 * (1.0F - var2) + var2;
	this->fogLightBrightness += (var3 - this->fogLightBrightness) * 0.1F;
	++this->rendererUpdateCount;
	this->itemRenderer->updateEquippedItem();
	if(this->mc->field_6288_M) {
	this->addRainParticles();
	}
}

void EntityRenderer::getMouseOver(float var1)
{
	if(this->mc->thePlayer != NULL) {
	double var2 = (double)this->mc->playerController->getBlockReachDistance();
	this->mc->objectMouseOver = this->mc->thePlayer->rayTrace(var2, var1);
	double var4 = var2;
	Vec3D* var6 = this->mc->thePlayer->getPosition(var1);
	if(this->mc->objectMouseOver != NULL) {
	var4 = this->mc->objectMouseOver->hitVec->distanceTo(var6);
	}
	if((dynamic_cast<PlayerControllerTest*>(this->mc->playerController) != NULL)) {
	var2 = 32.0;
	} else {
	if(var4 > 3.0) {
	var4 = 3.0;
	}
	var2 = var4;
	}
	Vec3D* var7 = this->mc->thePlayer->getLook(var1);
	Vec3D* var8 = var6->addVector(var7->xCoord * var2, var7->yCoord * var2, var7->zCoord * var2);
	this->pointedEntity = NULL;
	float var9 = 1.0F;
	JList<void*> var10 = this->mc->theWorld->getEntitiesWithinAABBExcludingEntity(this->mc->thePlayer, this->mc->thePlayer->boundingBox->addCoord(var7->xCoord * var2, var7->yCoord * var2, var7->zCoord * var2)->expands((double)var9, (double)var9, (double)var9));
	double var11 = 0.0;
	for(int var13 = 0; var13 < var10.size(); ++var13) {
	Entity* var14 = (Entity*)var10[var13];
	if(var14->canBeCollidedWith()) {
	float var15 = var14->getShadowSize();
	AxisAlignedBB* var16 = var14->boundingBox->expands((double)var15, (double)var15, (double)var15);
	MovingObjectPosition* var17 = var16->calculateIntercept(var6, var8);
	if(var16->isVecInside(var6)) {
	if(0.0 < var11 || var11 == 0.0) {
	this->pointedEntity = var14;
	var11 = 0.0;
	}
	} else if(var17 != NULL) {
	double var18 = var6->distanceTo(var17->hitVec);
	if(var18 < var11 || var11 == 0.0) {
	this->pointedEntity = var14;
	var11 = var18;
	}
	}
	}
	}
	if(this->pointedEntity != NULL && !((dynamic_cast<PlayerControllerTest*>(this->mc->playerController) != NULL))) {
	this->mc->objectMouseOver = new MovingObjectPosition(this->pointedEntity);
	}
	}
}

float EntityRenderer::getFOVModifier(float var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	float var3 = 70.0F;
	if(var2->isInsideOfMaterial(Material::s_water)) {
	var3 = 60.0F;
	}
	if(var2->health <= 0) {
	float var4 = (float)var2->deathTime + var1;
	var3 /= (1.0F - 500.0F / (var4 + 500.0F)) * 2.0F + 1.0F;
	}
	return var3;
}

void EntityRenderer::hurtCameraEffect(float var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	float var3 = (float)var2->hurtTime - var1;
	float var4;
	if(var2->health <= 0) {
	var4 = (float)var2->deathTime + var1;
	GL11::glRotatef(40.0F - 8000.0F / (var4 + 200.0F), 0.0F, 0.0F, 1.0F);
	}
	if(var3 >= 0.0F) {
	var3 /= (float)var2->maxHurtTime;
	var3 = MathHelper::sin(var3 * var3 * var3 * var3 * (float)3.1415927F);
	var4 = var2->attackedAtYaw;
	GL11::glRotatef(-var4, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(-var3 * 14.0F, 0.0F, 0.0F, 1.0F);
	GL11::glRotatef(var4, 0.0F, 1.0F, 0.0F);
	}
}

void EntityRenderer::setupViewBobbing(float var1)
{
	if(!this->mc->gameSettings->thirdPersonView) {
	EntityPlayerSP* var2 = this->mc->thePlayer;
	float var3 = var2->distanceWalkedModified - var2->prevDistanceWalkedModified;
	float var4 = var2->distanceWalkedModified + var3 * var1;
	float var5 = var2->prevCameraYaw + (var2->cameraYaw - var2->prevCameraYaw) * var1;
	float var6 = var2->field_9329_Q + (var2->field_9328_R - var2->field_9329_Q) * var1;
	GL11::glTranslatef(MathHelper::sin(var4 * (float)3.1415927F) * var5 * 0.5F, -abs(MathHelper::cos(var4 * (float)3.1415927F) * var5), 0.0F);
	GL11::glRotatef(MathHelper::sin(var4 * (float)3.1415927F) * var5 * 3.0F, 0.0F, 0.0F, 1.0F);
	GL11::glRotatef(abs(MathHelper::cos(var4 * (float)3.1415927F + 0.2F) * var5) * 5.0F, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(var6, 1.0F, 0.0F, 0.0F);
	}
}

void EntityRenderer::orientCamera(float var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	double var3 = var2->prevPosX + (var2->posX - var2->prevPosX) * (double)var1;
	double var5 = var2->prevPosY + (var2->posY - var2->prevPosY) * (double)var1;
	double var7 = var2->prevPosZ + (var2->posZ - var2->prevPosZ) * (double)var1;
	if(this->mc->gameSettings->thirdPersonView) {
	double var9 = 4.0;
	float var11 = var2->rotationYaw;
	float var12 = var2->rotationPitch;
	if(Keyboard::isKeyDown(_KB_KEY_F1)) {
	var12 += 180.0F;
	var9 += 2.0;
	}
	double var13 = (double)(-MathHelper::sin(var11 / 180.0F * (float)3.1415927F) * MathHelper::cos(var12 / 180.0F * (float)3.1415927F)) * var9;
	double var15 = (double)(MathHelper::cos(var11 / 180.0F * (float)3.1415927F) * MathHelper::cos(var12 / 180.0F * (float)3.1415927F)) * var9;
	double var17 = (double)(-MathHelper::sin(var12 / 180.0F * (float)3.1415927F)) * var9;
	for(int var19 = 0; var19 < 8; ++var19) {
	float var20 = (float)((var19 & 1) * 2 - 1);
	float var21 = (float)((var19 >> 1 & 1) * 2 - 1);
	float var22 = (float)((var19 >> 2 & 1) * 2 - 1);
	var20 *= 0.1F;
	var21 *= 0.1F;
	var22 *= 0.1F;
	MovingObjectPosition* var23 = this->mc->theWorld->rayTraceBlocks(Vec3D::createVector(var3 + (double)var20, var5 + (double)var21, var7 + (double)var22), Vec3D::createVector(var3 - var13 + (double)var20 + (double)var22, var5 - var17 + (double)var21, var7 - var15 + (double)var22));
	if(var23 != NULL) {
	double var24 = var23->hitVec->distanceTo(Vec3D::createVector(var3, var5, var7));
	if(var24 < var9) {
	var9 = var24;
	}
	}
	}
	if(Keyboard::isKeyDown(_KB_KEY_F1)) {
	GL11::glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
	}
	GL11::glRotatef(var2->rotationPitch - var12, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(var2->rotationYaw - var11, 0.0F, 1.0F, 0.0F);
	GL11::glTranslatef(0.0F, 0.0F, (float)(-var9));
	GL11::glRotatef(var11 - var2->rotationYaw, 0.0F, 1.0F, 0.0F);
	GL11::glRotatef(var12 - var2->rotationPitch, 1.0F, 0.0F, 0.0F);
	} else {
	GL11::glTranslatef(0.0F, 0.0F, -0.1F);
	}
	GL11::glRotatef(var2->prevRotationPitch + (var2->rotationPitch - var2->prevRotationPitch) * var1, 1.0F, 0.0F, 0.0F);
	GL11::glRotatef(var2->prevRotationYaw + (var2->rotationYaw - var2->prevRotationYaw) * var1 + 180.0F, 0.0F, 1.0F, 0.0F);
}

void EntityRenderer::setupCameraTransform(float var1, int var2)
{
	this->farPlaneDistance = (float)(256 >> this->mc->gameSettings->renderDistance);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	float var3 = 0.07F;
	if(this->mc->gameSettings->anaglyph) {
	GL11::glTranslatef((float)(-(var2 * 2 - 1)) * var3, 0.0F, 0.0F);
	}
	GLU::gluPerspective(this->getFOVModifier(var1), (float)this->mc->displayWidth / (float)this->mc->displayHeight, 0.05F, this->farPlaneDistance);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	if(this->mc->gameSettings->anaglyph) {
	GL11::glTranslatef((float)(var2 * 2 - 1) * 0.1F, 0.0F, 0.0F);
	}
	this->hurtCameraEffect(var1);
	if(this->mc->gameSettings->viewBobbing) {
	this->setupViewBobbing(var1);
	}
	float var4 = this->mc->thePlayer->prevTimeInPortal + (this->mc->thePlayer->timeInPortal - this->mc->thePlayer->prevTimeInPortal) * var1;
	if(var4 > 0.0F) {
	float var5 = 5.0F / (var4 * var4 + 5.0F) - var4 * 0.04F;
	var5 *= var5;
	GL11::glRotatef(var4 * var4 * 1500.0F, 0.0F, 1.0F, 1.0F);
	GL11::glScalef(1.0F / var5, 1.0F, 1.0F);
	GL11::glRotatef(-var4 * var4 * 1500.0F, 0.0F, 1.0F, 1.0F);
	}
	this->orientCamera(var1);
}

void EntityRenderer::renderHand(float var1, int var2)
{
	GL11::glLoadIdentity();
	if(this->mc->gameSettings->anaglyph) {
	GL11::glTranslatef((float)(var2 * 2 - 1) * 0.1F, 0.0F, 0.0F);
	}
	GL11::glPushMatrix();
	this->hurtCameraEffect(var1);
	if(this->mc->gameSettings->viewBobbing) {
	this->setupViewBobbing(var1);
	}
	if(!this->mc->gameSettings->thirdPersonView && !Keyboard::isKeyDown(_KB_KEY_F1)) {
	this->itemRenderer->renderItemInFirstPerson(var1);
	}
	GL11::glPopMatrix();
	if(!this->mc->gameSettings->thirdPersonView) {
	this->itemRenderer->renderOverlays(var1);
	this->hurtCameraEffect(var1);
	}
	if(this->mc->gameSettings->viewBobbing) {
	this->setupViewBobbing(var1);
	}
}

void EntityRenderer::updateCameraAndRender(float var1)
{
	if(!Display::isActive()) {
	if(System::currentTimeMillis() - this->prevFrameTime > 500LL) {
	this->mc->displayInGameMenu();
	}
	} else {
	this->prevFrameTime = System::currentTimeMillis();
	}
	if(this->mc->inGameHasFocus) {
	this->mc->mouseHelper->updateMouseDelta();
	float var2 = this->mc->gameSettings->mouseSensitivity * 0.6F + 0.2F;
	float var3 = var2 * var2 * var2 * 8.0F;
	float var4 = (float)this->mc->mouseHelper->deltaX * var3;
	float var5 = (float)this->mc->mouseHelper->deltaY * var3;
	unsigned char var6 = 1;
	if(this->mc->gameSettings->invertMouse) {
	var6 = -1;
	}
	this->mc->thePlayer->setAngles(var4, var5 * (float)var6);
	}
	if(!this->mc->skipRenderWorld) {
	ScaledResolution* var7 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	int var8 = var7->getScaledWidth();
	int var9 = var7->getScaledHeight();
	int var10 = Mouse::getX() * var8 / this->mc->displayWidth;
	int var11 = var9 - Mouse::getY() * var9 / this->mc->displayHeight - 1;
	if(this->mc->theWorld != NULL) {
	this->renderWorld(var1);
	if(!Keyboard::isKeyDown(_KB_KEY_F1)) {
	this->mc->ingameGUI->renderGameOverlay(var1, this->mc->currentScreen != NULL, var10, var11);
	}
	} else {
	GL11::glViewport(0, 0, this->mc->displayWidth, this->mc->displayHeight);
	GL11::glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT | GL11::GL_COLOR_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	this->setupOverlayRendering();
	}
	if(this->mc->currentScreen != NULL) {
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	this->mc->currentScreen->drawScreen(var10, var11, var1);
	}
	}
}

void EntityRenderer::renderWorld(float var1)
{
	this->getMouseOver(var1);
	EntityPlayerSP* var2 = this->mc->thePlayer;
	RenderGlobal* var3 = this->mc->renderGlobal;
	EffectRenderer* var4 = this->mc->effectRenderer;
	double var5 = var2->lastTickPosX + (var2->posX - var2->lastTickPosX) * (double)var1;
	double var7 = var2->lastTickPosY + (var2->posY - var2->lastTickPosY) * (double)var1;
	double var9 = var2->lastTickPosZ + (var2->posZ - var2->lastTickPosZ) * (double)var1;
	for(int var11 = 0; var11 < 2; ++var11) {
	if(this->mc->gameSettings->anaglyph) {
	if(var11 == 0) {
	GL11::glColorMask(false, true, true, false);
	} else {
	GL11::glColorMask(true, false, false, false);
	}
	}
	GL11::glViewport(0, 0, this->mc->displayWidth, this->mc->displayHeight);
	this->updateFogColor(var1);
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT | GL11::GL_COLOR_BUFFER_BIT);
	GL11::glEnable(GL11::GL_CULL_FACE);
	this->setupCameraTransform(var1, var11);
	ClippingHelperImplementation::getInstance();
	if(this->mc->gameSettings->renderDistance < 2) {
	this->setupFog(-1);
	var3->renderSky(var1);
	}
	GL11::glEnable(GL11::GL_FOG);
	this->setupFog(1);
	Frustrum* var12 = new Frustrum();
	var12->setPosition(var5, var7, var9);
	this->mc->renderGlobal->clipRenderersByFrustum(var12, var1);
	this->mc->renderGlobal->updateRenderers(var2, false);
	this->setupFog(0);
	GL11::glEnable(GL11::GL_FOG);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/terrain.png"));
	RenderHelper::disableStandardItemLighting();
	var3->sortAndRender(var2, 0, (double)var1);
	RenderHelper::enableStandardItemLighting();
	var3->renderEntities(var2->getPosition(var1), var12, var1);
	var4->renderLitParticles(var2, var1);
	RenderHelper::disableStandardItemLighting();
	this->setupFog(0);
	var4->renderParticles(var2, var1);
	if(this->mc->objectMouseOver != NULL && var2->isInsideOfMaterial(Material::s_water)) {
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	var3->drawBlockBreaking(var2, this->mc->objectMouseOver, 0, var2->inventory->getCurrentItem(), var1);
	var3->drawSelectionBox(var2, this->mc->objectMouseOver, 0, var2->inventory->getCurrentItem(), var1);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	}
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	this->setupFog(0);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glDisable(GL11::GL_CULL_FACE);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/terrain.png"));
	if(this->mc->gameSettings->fancyGraphics) {
	GL11::glColorMask(false, false, false, false);
	int var13 = var3->sortAndRender(var2, 1, (double)var1);
	GL11::glColorMask(true, true, true, true);
	if(this->mc->gameSettings->anaglyph) {
	if(var11 == 0) {
	GL11::glColorMask(false, true, true, false);
	} else {
	GL11::glColorMask(true, false, false, false);
	}
	}
	if(var13 > 0) {
	var3->renderAllRenderLists(1, (double)var1);
	}
	} else {
	var3->sortAndRender(var2, 1, (double)var1);
	}
	GL11::glDepthMask(true);
	GL11::glEnable(GL11::GL_CULL_FACE);
	GL11::glDisable(GL11::GL_BLEND);
	if(this->mc->objectMouseOver != NULL && !var2->isInsideOfMaterial(Material::s_water)) {
	GL11::glDisable(GL11::GL_ALPHA_TEST);
	var3->drawBlockBreaking(var2, this->mc->objectMouseOver, 0, var2->inventory->getCurrentItem(), var1);
	var3->drawSelectionBox(var2, this->mc->objectMouseOver, 0, var2->inventory->getCurrentItem(), var1);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	}
	GL11::glDisable(GL11::GL_FOG);
	if(this->pointedEntity != NULL) {
	}
	this->setupFog(0);
	GL11::glEnable(GL11::GL_FOG);
	var3->renderClouds(var1);
	GL11::glDisable(GL11::GL_FOG);
	this->setupFog(1);
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	this->renderHand(var1, var11);
	if(!this->mc->gameSettings->anaglyph) {
	return;
	}
	}
	GL11::glColorMask(true, true, true, false);
}

void EntityRenderer::addRainParticles()
{
	if(this->mc->gameSettings->fancyGraphics) {
	EntityPlayerSP* var1 = this->mc->thePlayer;
	World* var2 = this->mc->theWorld;
	int var3 = MathHelper::floor_double(var1->posX);
	int var4 = MathHelper::floor_double(var1->posY);
	int var5 = MathHelper::floor_double(var1->posZ);
	unsigned char var6 = 16;
	for(int var7 = 0; var7 < 150; ++var7) {
	int var8 = var3 + this->random->nextInt(var6) - this->random->nextInt(var6);
	int var9 = var5 + this->random->nextInt(var6) - this->random->nextInt(var6);
	int var10 = var2->func_696_e(var8, var9);
	int var11 = var2->getBlockId(var8, var10 - 1, var9);
	if(var10 <= var4 + var6 && var10 >= var4 - var6) {
	float var12 = this->random->nextFloat();
	float var13 = this->random->nextFloat();
	if(var11 > 0) {
	this->mc->effectRenderer->addEffect(new EntityRainFX(var2, (double)((float)var8 + var12), (double)((float)var10 + 0.1F) - Block::s_blocksList[var11]->minY, (double)((float)var9 + var13)));
	}
	}
	}
	}
}

void EntityRenderer::setupOverlayRendering()
{
	ScaledResolution* var1 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	int var2 = var1->getScaledWidth();
	int var3 = var1->getScaledHeight();
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glOrtho(0.0, (double)var2, (double)var3, 0.0, 1000.0, 3000.0);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	GL11::glTranslatef(0.0F, 0.0F, -2000.0F);
}

void EntityRenderer::updateFogColor(float var1)
{
	World* var2 = this->mc->theWorld;
	EntityPlayerSP* var3 = this->mc->thePlayer;
	float var4 = 1.0F / (float)(4 - this->mc->gameSettings->renderDistance);
	var4 = 1.0F - (float)powf((double)var4, 0.25);
	Vec3D* var5 = var2->getSkyColor(this->mc->thePlayer, var1);
	float var6 = (float)var5->xCoord;
	float var7 = (float)var5->yCoord;
	float var8 = (float)var5->zCoord;
	Vec3D* var9 = var2->getCloudColour(var1);
	this->fogColorRed = (float)var9->xCoord;
	this->fogColorGreen = (float)var9->yCoord;
	this->fogColorBlue = (float)var9->zCoord;
	this->fogColorRed += (var6 - this->fogColorRed) * var4;
	this->fogColorGreen += (var7 - this->fogColorGreen) * var4;
	this->fogColorBlue += (var8 - this->fogColorBlue) * var4;
	if(var3->isInsideOfMaterial(Material::s_water)) {
	this->fogColorRed = 0.02F;
	this->fogColorGreen = 0.02F;
	this->fogColorBlue = 0.2F;
	} else if(var3->isInsideOfMaterial(Material::s_lava)) {
	this->fogColorRed = 0.6F;
	this->fogColorGreen = 0.1F;
	this->fogColorBlue = 0.0F;
	}
	float var10 = this->prevFogLightBrightness + (this->fogLightBrightness - this->prevFogLightBrightness) * var1;
	this->fogColorRed *= var10;
	this->fogColorGreen *= var10;
	this->fogColorBlue *= var10;
	if(this->mc->gameSettings->anaglyph) {
	float var11 = (this->fogColorRed * 30.0F + this->fogColorGreen * 59.0F + this->fogColorBlue * 11.0F) / 100.0F;
	float var12 = (this->fogColorRed * 30.0F + this->fogColorGreen * 70.0F) / 100.0F;
	float var13 = (this->fogColorRed * 30.0F + this->fogColorBlue * 70.0F) / 100.0F;
	this->fogColorRed = var11;
	this->fogColorGreen = var12;
	this->fogColorBlue = var13;
	}
	GL11::glClearColor(this->fogColorRed, this->fogColorGreen, this->fogColorBlue, 0.0F);
}

void EntityRenderer::setupFog(int var1)
{
	EntityPlayerSP* var2 = this->mc->thePlayer;
	GL11::glFog(GL11::GL_FOG_COLOR, this->setFogColorBuffer(this->fogColorRed, this->fogColorGreen, this->fogColorBlue, 1.0F));
	GL11::glNormal3f(0.0F, -1.0F, 0.0F);
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var3;
	float var4;
	float var5;
	float var6;
	float var7;
	float var8;
	if(var2->isInsideOfMaterial(Material::s_water)) {
	GL11::glFogi(GL11::GL_FOG_MODE, GL11::GL_EXP);
	GL11::glFogf(GL11::GL_FOG_DENSITY, 0.1F);
	var3 = 0.4F;
	var4 = 0.4F;
	var5 = 0.9F;
	if(this->mc->gameSettings->anaglyph) {
	var6 = (var3 * 30.0F + var4 * 59.0F + var5 * 11.0F) / 100.0F;
	var7 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
	var8 = (var3 * 30.0F + var5 * 70.0F) / 100.0F;
	}
	} else if(var2->isInsideOfMaterial(Material::s_lava)) {
	GL11::glFogi(GL11::GL_FOG_MODE, GL11::GL_EXP);
	GL11::glFogf(GL11::GL_FOG_DENSITY, 2.0F);
	var3 = 0.4F;
	var4 = 0.3F;
	var5 = 0.3F;
	if(this->mc->gameSettings->anaglyph) {
	var6 = (var3 * 30.0F + var4 * 59.0F + var5 * 11.0F) / 100.0F;
	var7 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
	var8 = (var3 * 30.0F + var5 * 70.0F) / 100.0F;
	}
	} else {
	GL11::glFogi(GL11::GL_FOG_MODE, GL11::GL_LINEAR);
	GL11::glFogf(GL11::GL_FOG_START, this->farPlaneDistance * 0.25F);
	GL11::glFogf(GL11::GL_FOG_END, this->farPlaneDistance);
	if(var1 < 0) {
	GL11::glFogf(GL11::GL_FOG_START, 0.0F);
	GL11::glFogf(GL11::GL_FOG_END, this->farPlaneDistance * 0.8F);
	}
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  if(GLContext.getCapabilities().GL_NV_fog_distance) {
	 *      GL11.glFogi(NVFogDistance.GL_FOG_DISTANCE_MODE_NV, NVFogDistance.GL_EYE_RADIAL_NV);
	 *  }
	 *  ---- end original Java ----
	 *  _hasNvFogDistance() is the shim that always returns false, so this radial-fog
	 *  branch never executes on the D3D9 target (NVIDIA extension not available). */
	if(_hasNvFogDistance()) {
	GL11::glFogi(_NV_GL_FOG_DISTANCE_MODE_NV, _NV_GL_EYE_RADIAL_NV);
	}
	if(this->mc->theWorld->worldProvider->isHellWorld) {
	GL11::glFogf(GL11::GL_FOG_START, 0.0F);
	}
	}
	GL11::glEnable(GL11::GL_COLOR_MATERIAL);
	GL11::glColorMaterial(GL11::GL_FRONT, GL11::GL_AMBIENT);
}

FloatBuffer* EntityRenderer::setFogColorBuffer(float var1, float var2, float var3, float var4)
{
	this->fogColorBuffer->clear();
	this->fogColorBuffer->put(var1).put(var2).put(var3).put(var4);
	this->fogColorBuffer->flip();
	return this->fogColorBuffer;
}
