#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LoadingScreenRenderer.h"
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
#include "Display.h"
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
#include "IProgressUpdate.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "Material.h"
#include "Minecraft.h"
#include "MinecraftApplet.h"
#include "MinecraftError.h"
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

LoadingScreenRenderer::LoadingScreenRenderer(Minecraft* var1)
	: mc(NULL)
	, systemTime(0)
	, doneWorking(0)
{
	this->message = "";
	this->title = "";
	this->systemTime = System::currentTimeMillis();
	this->doneWorking = false;
	this->mc = var1;
}

LoadingScreenRenderer::~LoadingScreenRenderer()
{
}

void LoadingScreenRenderer::func_596_a(const JavaString& var1)
{
	this->doneWorking = false;
	this->func_597_c(var1);
}

void LoadingScreenRenderer::displayProgressMessage(const JavaString& var1)
{
	this->doneWorking = true;
	this->func_597_c(this->title);
}

void LoadingScreenRenderer::func_597_c(const JavaString& var1)
{
	if(!this->mc->running) {
	if(!this->doneWorking) {
	throw new MinecraftError();
	}
	} else {
	this->title = var1;
	ScaledResolution* var2 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	int var3 = var2->getScaledWidth();
	int var4 = var2->getScaledHeight();
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glOrtho(0.0, (double)var3, (double)var4, 0.0, 100.0, 300.0);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	GL11::glTranslatef(0.0F, 0.0F, -200.0F);
	}
}

void LoadingScreenRenderer::displayLoadingString(const JavaString& var1)
{
	if(!this->mc->running) {
	if(!this->doneWorking) {
	throw new MinecraftError();
	}
	} else {
	this->systemTime = 0LL;
	this->message = var1;
	this->setLoadingProgress(-1);
	this->systemTime = 0LL;
	}
}

void LoadingScreenRenderer::setLoadingProgress(int var1)
{
	if(!this->mc->running) {
	if(!this->doneWorking) {
	throw new MinecraftError();
	}
	} else {
	long long var2 = System::currentTimeMillis();
	if(var2 - this->systemTime >= 20LL) {
	this->systemTime = var2;
	ScaledResolution* var4 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	int var5 = var4->getScaledWidth();
	int var6 = var4->getScaledHeight();
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glOrtho(0.0, (double)var5, (double)var6, 0.0, 100.0, 300.0);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	GL11::glTranslatef(0.0F, 0.0F, -200.0F);
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT | GL11::GL_COLOR_BUFFER_BIT);
	Tessellator* var7 = Tessellator::s_instance;
	int var8 = this->mc->renderEngine->getTexture("/gui/background.png");
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, var8);
	float var9 = 32.0F;
	var7->startDrawingQuads();
	var7->setColorOpaque_I(4210752);
	var7->addVertexWithUV(0.0, (double)var6, 0.0, 0.0, (double)((float)var6 / var9));
	var7->addVertexWithUV((double)var5, (double)var6, 0.0, (double)((float)var5 / var9), (double)((float)var6 / var9));
	var7->addVertexWithUV((double)var5, 0.0, 0.0, (double)((float)var5 / var9), 0.0);
	var7->addVertexWithUV(0.0, 0.0, 0.0, 0.0, 0.0);
	var7->draw();
	if(var1 >= 0) {
	unsigned char var10 = 100;
	unsigned char var11 = 2;
	int var12 = var5 / 2 - var10 / 2;
	int var13 = var6 / 2 + 16;
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	var7->startDrawingQuads();
	var7->setColorOpaque_I(8421504);
	var7->addVertex((double)var12, (double)var13, 0.0);
	var7->addVertex((double)var12, (double)(var13 + var11), 0.0);
	var7->addVertex((double)(var12 + var10), (double)(var13 + var11), 0.0);
	var7->addVertex((double)(var12 + var10), (double)var13, 0.0);
	var7->setColorOpaque_I(8454016);
	var7->addVertex((double)var12, (double)var13, 0.0);
	var7->addVertex((double)var12, (double)(var13 + var11), 0.0);
	var7->addVertex((double)(var12 + var1), (double)(var13 + var11), 0.0);
	var7->addVertex((double)(var12 + var1), (double)var13, 0.0);
	var7->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	}
	this->mc->fontRenderer->drawStringWithShadow(this->title, (var5 - this->mc->fontRenderer->getStringWidth(this->title)) / 2, var6 / 2 - 4 - 16, 16777215);
	this->mc->fontRenderer->drawStringWithShadow(this->message, (var5 - this->mc->fontRenderer->getStringWidth(this->message)) / 2, var6 / 2 - 4 + 8, 16777215);
	Display::update();
	try {
	Thread::yield();
	} catch (Exception*) {
	}
	}
	}
}

void LoadingScreenRenderer::initStatics()
{
}
