#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GuiMainMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiMainMenu.h"
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
#include "GLU.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiMultiplayer.h"
#include "GuiOptions.h"
#include "GuiScreen.h"
#include "GuiSelectWorld.h"
#include "GuiTexturePacks.h"
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
#include "LogoEffectRandomizer.h"
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

// --- Local shims (runtime-stub gaps; see final report) ---
// java.io.InputStreamReader, java.util.Calendar, java.util.Date are not provided
// by the runtime headers. Minimal shims that preserve the call shapes used here.
#include <ctime>
namespace {
	class InputStreamReader {
	public:
		InputStreamReader(...) {}
	};
	class Date {
	public:
		Date() {}
	};
	class Calendar {
		std::tm _tm;
	public:
		// Field constants mirror java.util.Calendar (MONTH=2, DAY_OF_MONTH=5).
		Calendar() { std::time_t t = std::time(0); _tm = *std::localtime(&t); }
		static Calendar* getInstance() { return new Calendar(); }
		void setTime(Date*) {}                  // current time already captured in ctor
		int get(int field) const {
			if (field == 2) return _tm.tm_mon;        // Calendar.MONTH (0-based)
			if (field == 5) return _tm.tm_mday;       // Calendar.DAY_OF_MONTH
			return 0;
		}
	};
}

Random* GuiMainMenu::s_rand = new Random();

GuiMainMenu::GuiMainMenu()
	: updateCounter(0)
{
	try {
	JList<JavaString> var1;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  ArrayList var1 = new ArrayList();
	 *  BufferedReader var2 = new BufferedReader(new InputStreamReader(GuiMainMenu.class.getResourceAsStream("/title/splashes.txt")));
	 *  // The InputStreamReader shim (top of file) has an empty ctor and GuiMainMenu.class
	 *  // is reduced to an empty Class() token, so the splash-text resource is not actually
	 *  // read; the readLine() loop yields nothing and splashText stays "missingno".
	 *  ---- end original Java ---- */
	BufferedReader* var2 = new BufferedReader(new InputStreamReader(Class().getResourceAsStream("/title/splashes.txt")));
	JavaString var3 = "";
	while(true) {
	var3 = var2->readLine();
	if(var3 == NULL) {
	this->splashText = var1[s_rand->nextInt(var1.size())];
	break;
	}
	var3 = var3.trim();
	if(var3.length() > 0) {
	var1.push_back(var3);
	}
	}
	} catch (Exception var4) {
	}
}

GuiMainMenu::~GuiMainMenu()
{
}

void GuiMainMenu::updateScreen()
{
	++this->updateCounter;
	if(this->logoEffects != NULL) {
	for(int var1 = 0; var1 < this->logoEffects.length; ++var1) {
	for(int var2 = 0; var2 < this->logoEffects[var1].length; ++var2) {
	this->logoEffects[var1][var2]->updatePosition();
	}
	}
	}
}

void GuiMainMenu::keyTyped(char var1, int var2)
{
}

void GuiMainMenu::initGui()
{
	Calendar* var1 = Calendar::getInstance();
	var1->setTime(new Date());
	if(var1->get(2) + 1 == 11 && var1->get(5) == 9) {
	this->splashText = "Happy birthday, ez!";
	} else if(var1->get(2) + 1 == 6 && var1->get(5) == 1) {
	this->splashText = "Happy birthday, Notch!";
	} else if(var1->get(2) + 1 == 12 && var1->get(5) == 24) {
	this->splashText = "Merry X-mas!";
	} else if(var1->get(2) + 1 == 1 && var1->get(5) == 1) {
	this->splashText = "Happy new year!";
	}
	this->controlList.push_back(new GuiButton(1,this->width / 2 - 100,this->height / 4 + 48,"Singleplayer"));
	this->controlList.push_back(new GuiButton(2,this->width / 2 - 100,this->height / 4 + 72,"Multiplayer"));
	this->controlList.push_back(new GuiButton(3,this->width / 2 - 100,this->height / 4 + 96,"Mods and Texture Packs"));
	this->controlList.push_back(new GuiButton(0,this->width / 2 - 100,this->height / 4 + 120 + 12,"Options..."));
	if(this->mc->session == NULL) {
	((GuiButton*)this->controlList[1])->enabled = false;
	}
}

void GuiMainMenu::actionPerformed(GuiButton* var1)
{
	if(var1->id == 0) {
	this->mc->displayGuiScreen(new GuiOptions(this, this->mc->gameSettings));
	}
	if(var1->id == 1) {
	this->mc->displayGuiScreen(new GuiSelectWorld(this));
	}
	if(var1->id == 2) {
	this->mc->displayGuiScreen(new GuiMultiplayer(this));
	}
	if(var1->id == 3) {
	this->mc->displayGuiScreen(new GuiTexturePacks(this));
	}
}

void GuiMainMenu::drawScreen(int var1, int var2, float var3)
{
	this->drawDefaultBackground();
	Tessellator* var4 = Tessellator::s_instance;
	this->drawLogo(var3);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/gui/logo.png"));
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	var4->setColorOpaque_I(16777215);
	GL11::glPushMatrix();
	GL11::glTranslatef((float)(this->width / 2 + 90), 70.0F, 0.0F);
	GL11::glRotatef(-20.0F, 0.0F, 0.0F, 1.0F);
	float var5 = 1.8F - MathHelper::abs(MathHelper::sin((float)(System::currentTimeMillis() % 1000L) / 1000.0F * (float)3.1415927F * 2.0F) * 0.1F);
	var5 = var5 * 100.0F / (float)(this->fontRenderer->getStringWidth(this->splashText) + 32);
	GL11::glScalef(var5, var5, var5);
	this->drawCenteredString(this->fontRenderer, this->splashText, 0, -8, 16776960);
	GL11::glPopMatrix();
	this->drawString(this->fontRenderer, "Minecraft Alpha v1.2.6", 2, 2, 5263440);
	JavaString var6 = "Copyright Mojang Specifications. Do not distribute.";
	this->drawString(this->fontRenderer, var6, this->width - this->fontRenderer->getStringWidth(var6) - 2, this->height - 10, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiMainMenu::drawLogo(float var1)
{
	int var3;
	if(this->logoEffects == NULL) {
	int var0a = this->minecraftLogo[0].length();
	int var0b = this->minecraftLogo.length;
	this->logoEffects = JArray<JArray<LogoEffectRandomizer* > >(var0a);
	for(int var0c = 0; var0c < var0a; ++var0c) {
	this->logoEffects[var0c] = JArray<LogoEffectRandomizer* >(var0b);
	}
	for(int var2 = 0; var2 < this->logoEffects.length; ++var2) {
	for(var3 = 0; var3 < this->logoEffects[var2].length; ++var3) {
	this->logoEffects[var2][var3] = new LogoEffectRandomizer(this, var2, var3);
	}
	}
	}
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glPushMatrix();
	GL11::glLoadIdentity();
	ScaledResolution* var14 = new ScaledResolution(this->mc->displayWidth, this->mc->displayHeight);
	var3 = 120 * var14->scaleFactor;
	GLU::gluPerspective(70.0F, (float)this->mc->displayWidth / (float)var3, 0.05F, 100.0F);
	GL11::glViewport(0, this->mc->displayHeight - var3, this->mc->displayWidth, var3);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glPushMatrix();
	GL11::glLoadIdentity();
	GL11::glDisable(GL11::GL_CULL_FACE);
	GL11::glCullFace(GL11::GL_BACK);
	GL11::glDepthMask(true);
	RenderBlocks* var4 = new RenderBlocks();
	for(int var5 = 0; var5 < 3; ++var5) {
	GL11::glPushMatrix();
	GL11::glTranslatef(0.4F, 0.6F, -13.0F);
	if(var5 == 0) {
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	GL11::glTranslatef(0.0F, -0.4F, 0.0F);
	GL11::glScalef(0.98F, 1.0F, 1.0F);
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_ALPHA, GL11::GL_ONE_MINUS_SRC_ALPHA);
	}
	if(var5 == 1) {
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	}
	if(var5 == 2) {
	GL11::glEnable(GL11::GL_BLEND);
	GL11::glBlendFunc(GL11::GL_SRC_COLOR, GL11::GL_ONE);
	}
	GL11::glScalef(1.0F, -1.0F, 1.0F);
	GL11::glRotatef(15.0F, 1.0F, 0.0F, 0.0F);
	GL11::glScalef(0.89F, 1.0F, 0.4F);
	GL11::glTranslatef((float)(-this->minecraftLogo[0].length()) * 0.5F, (float)(-this->minecraftLogo.length) * 0.5F, 0.0F);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/terrain.png"));
	if(var5 == 0) {
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->mc->renderEngine->getTexture("/title/black.png"));
	}
	for(int var6 = 0; var6 < this->minecraftLogo.length; ++var6) {
	for(int var7 = 0; var7 < this->minecraftLogo[var6].length(); ++var7) {
	char var8 = this->minecraftLogo[var6][var7];
	if(var8 != 32) {
	GL11::glPushMatrix();
	LogoEffectRandomizer* var9 = this->logoEffects[var7][var6];
	float var10 = (float)(var9->prevPosY + (var9->posY - var9->prevPosY) * (double)var1);
	float var11 = 1.0F;
	float var12 = 1.0F;
	float var13 = 0.0F;
	if(var5 == 0) {
	var11 = var10 * 0.04F + 1.0F;
	var12 = 1.0F / var11;
	var10 = 0.0F;
	}
	GL11::glTranslatef((float)var7, (float)var6, var10);
	GL11::glScalef(var11, var11, var11);
	GL11::glRotatef(var13, 0.0F, 1.0F, 0.0F);
	var4->renderBlockWithAlpha(Block::s_stone, var12);
	GL11::glPopMatrix();
	}
	}
	}
	GL11::glPopMatrix();
	}
	GL11::glDisable(GL11::GL_BLEND);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glPopMatrix();
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glPopMatrix();
	GL11::glViewport(0, 0, this->mc->displayWidth, this->mc->displayHeight);
	GL11::glEnable(GL11::GL_CULL_FACE);
}

Random* GuiMainMenu::getRand()
{
	return s_rand;
}
