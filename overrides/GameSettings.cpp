#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GameSettings.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GameSettings.h"
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
#include "Graphics.h"
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

// PORT: local shims for java.io.FileReader / FileWriter / PrintWriter, which have no runtime
// stub header (see "runtime-stub gaps"). FileReader/FileWriter are constructed from a File*
// and handed to BufferedReader/PrintWriter; PrintWriter exposes the println/close used here.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  import java.io.BufferedReader;
 *  import java.io.File;
 *  import java.io.FileReader;
 *  import java.io.FileWriter;
 *  import java.io.PrintWriter;
 *  // loadOptions(): BufferedReader var1 = new BufferedReader(new FileReader(this.optionsFile));
 *  // saveOptions(): PrintWriter   var1 = new PrintWriter(new FileWriter(this.optionsFile));
 *  //               var1.println("music:" + this.musicVolume); ... var1.close();
 *  ---- end original Java ----
 *  These java.io types are faked: PrintWriter.println()/close() are no-ops, so saveOptions()
 *  walks every option but writes nothing to disk; FileReader/FileWriter do no real file IO. */
namespace {
	struct FileReader { FileReader(...) {} };
	struct FileWriter { FileWriter(...) {} };
	struct PrintWriter {
		PrintWriter(...) {}
		template<class T> void println(const T&) {}
		void println() {}
		void close(...) {}
	};
}

static JavaString _GameSettings_s_RENDER_DISTANCES_data[] = { "FAR", "NORMAL", "SHORT", "TINY" };
JArray<JavaString > GameSettings::s_RENDER_DISTANCES(_GameSettings_s_RENDER_DISTANCES_data, 4);
static JavaString _GameSettings_s_DIFFICULTY_LEVELS_data[] = { "Peaceful", "Easy", "Normal", "Hard" };
JArray<JavaString > GameSettings::s_DIFFICULTY_LEVELS(_GameSettings_s_DIFFICULTY_LEVELS_data, 4);

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // instance-field initializers on the GameSettings class:
 *  public float musicVolume = 1.0F;
 *  public float soundVolume = 1.0F;
 *  public float mouseSensitivity = 0.5F;
 *  public boolean invertMouse = false;
 *  public int renderDistance = 0;
 *  public boolean viewBobbing = true;
 *  public boolean anaglyph = false;
 *  public boolean limitFramerate = false;
 *  public boolean fancyGraphics = true;
 *  public String skin = "Default";
 *  public KeyBinding keyBindForward = new KeyBinding("Forward", 17);
 *  public KeyBinding keyBindLeft = new KeyBinding("Left", 30);
 *  public KeyBinding keyBindBack = new KeyBinding("Back", 31);
 *  public KeyBinding keyBindRight = new KeyBinding("Right", 32);
 *  public KeyBinding keyBindJump = new KeyBinding("Jump", 57);
 *  public KeyBinding keyBindInventory = new KeyBinding("Inventory", 23);
 *  public KeyBinding keyBindDrop = new KeyBinding("Drop", 16);
 *  public KeyBinding keyBindChat = new KeyBinding("Chat", 20);
 *  public KeyBinding keyBindToggleFog = new KeyBinding("Toggle fog", 33);
 *  public KeyBinding keyBindSneak = new KeyBinding("Sneak", 42);
 *  public KeyBinding[] keyBindings = new KeyBinding[]{this.keyBindForward, this.keyBindLeft, this.keyBindBack, this.keyBindRight, this.keyBindJump, this.keyBindSneak, this.keyBindDrop, this.keyBindInventory, this.keyBindChat, this.keyBindToggleFog};
 *  public int numberOfOptions = 10;
 *  public int difficulty = 2;
 *  public boolean thirdPersonView = false;
 *  public String lastServer = "";
 *  ---- end original Java ----
 *  ALL of the above Java field initializers are dropped: both ctor init lists below zero/NULL
 *  every member. So the defaults (volumes=1.0, sensitivity=0.5, viewBobbing/fancyGraphics=true,
 *  skin="Default", numberOfOptions=10, difficulty=2) are lost, every keyBind* is NULL, and the
 *  keyBindings[] array is never constructed -- getKeyBinding/loadOptions/saveOptions deref it. */
GameSettings::GameSettings(Minecraft* var1, File* var2)
	: musicVolume(0)
	, soundVolume(0)
	, mouseSensitivity(0)
	, invertMouse(0)
	, renderDistance(0)
	, viewBobbing(0)
	, anaglyph(0)
	, limitFramerate(0)
	, fancyGraphics(0)
	, keyBindForward(NULL)
	, keyBindLeft(NULL)
	, keyBindBack(NULL)
	, keyBindRight(NULL)
	, keyBindJump(NULL)
	, keyBindInventory(NULL)
	, keyBindDrop(NULL)
	, keyBindChat(NULL)
	, keyBindToggleFog(NULL)
	, keyBindSneak(NULL)
	, mc(NULL)
	, optionsFile(NULL)
	, numberOfOptions(0)
	, difficulty(0)
	, thirdPersonView(0)
{
	// restored Java instance field initializers (were dropped -> defaults lost, keyBind*/keyBindings
	// NULL -> null-deref on input handling and options load/save)
	this->musicVolume = 1.0F;
	this->soundVolume = 1.0F;
	this->mouseSensitivity = 0.5F;
	this->invertMouse = false;
	this->renderDistance = 0;
	this->viewBobbing = true;
	this->anaglyph = false;
	this->limitFramerate = false;
	this->fancyGraphics = true;
	this->skin = "Default";
	this->keyBindForward = new KeyBinding("Forward", 17);
	this->keyBindLeft = new KeyBinding("Left", 30);
	this->keyBindBack = new KeyBinding("Back", 31);
	this->keyBindRight = new KeyBinding("Right", 32);
	this->keyBindJump = new KeyBinding("Jump", 57);
	this->keyBindInventory = new KeyBinding("Inventory", 23);
	this->keyBindDrop = new KeyBinding("Drop", 16);
	this->keyBindChat = new KeyBinding("Chat", 20);
	this->keyBindToggleFog = new KeyBinding("Toggle fog", 33);
	this->keyBindSneak = new KeyBinding("Sneak", 42);
	this->keyBindings = (JArrayInit<KeyBinding* >(), this->keyBindForward, this->keyBindLeft, this->keyBindBack, this->keyBindRight, this->keyBindJump, this->keyBindSneak, this->keyBindDrop, this->keyBindInventory, this->keyBindChat, this->keyBindToggleFog);
	this->numberOfOptions = 10;
	this->difficulty = 2;
	this->thirdPersonView = false;
	this->lastServer = "";
	this->mc = var1;
	this->optionsFile = new File(var2, "options.txt");
	this->loadOptions();
}

GameSettings::GameSettings()
	: musicVolume(0)
	, soundVolume(0)
	, mouseSensitivity(0)
	, invertMouse(0)
	, renderDistance(0)
	, viewBobbing(0)
	, anaglyph(0)
	, limitFramerate(0)
	, fancyGraphics(0)
	, keyBindForward(NULL)
	, keyBindLeft(NULL)
	, keyBindBack(NULL)
	, keyBindRight(NULL)
	, keyBindJump(NULL)
	, keyBindInventory(NULL)
	, keyBindDrop(NULL)
	, keyBindChat(NULL)
	, keyBindToggleFog(NULL)
	, keyBindSneak(NULL)
	, mc(NULL)
	, optionsFile(NULL)
	, numberOfOptions(0)
	, difficulty(0)
	, thirdPersonView(0)
{
}

GameSettings::~GameSettings()
{
	delete optionsFile;
	optionsFile = NULL;
}

JavaString GameSettings::getKeyBinding(int var1)
{
	return (JCat() + this->keyBindings[var1]->keyDescription + ": " + Keyboard::getKeyName(this->keyBindings[var1]->keyCode));
}

void GameSettings::setKeyBinding(int var1, int var2)
{
	this->keyBindings[var1]->keyCode = var2;
	this->saveOptions();
}

void GameSettings::setOptionFloatValue(int var1, float var2)
{
	if(var1 == 0) {
	this->musicVolume = var2;
	this->mc->sndManager->onSoundOptionsChanged();
	}
	if(var1 == 1) {
	this->soundVolume = var2;
	this->mc->sndManager->onSoundOptionsChanged();
	}
	if(var1 == 3) {
	this->mouseSensitivity = var2;
	}
}

void GameSettings::setOptionValue(int var1, int var2)
{
	if(var1 == 2) {
	this->invertMouse = !this->invertMouse;
	}
	if(var1 == 4) {
	this->renderDistance = (this->renderDistance + var2) & 3;
	}
	if(var1 == 5) {
	this->viewBobbing = !this->viewBobbing;
	}
	if(var1 == 6) {
	this->anaglyph = !this->anaglyph;
	this->mc->renderEngine->refreshTextures();
	}
	if(var1 == 7) {
	this->limitFramerate = !this->limitFramerate;
	}
	if(var1 == 8) {
	this->difficulty = (this->difficulty + var2) & 3;
	}
	if(var1 == 9) {
	this->fancyGraphics = !this->fancyGraphics;
	this->mc->renderGlobal->loadRenderers();
	}
	this->saveOptions();
}

int GameSettings::getOptionControlType(int var1)
{
	return var1 == 0 ? 1 : (var1 == 1 ? 1 : (var1 == 3 ? 1 : 0));
}

float GameSettings::getOptionFloatValue(int var1)
{
	return var1 == 0 ? this->musicVolume : (var1 == 1 ? this->soundVolume : (var1 == 3 ? this->mouseSensitivity : 0.0F));
}

JavaString GameSettings::getOptionDisplayString(int var1)
{
	// PORT: faithful expansion of the original nested-ternary String builder. Each Java
	// `"literal" + value` concatenation is rendered via JCat (C++ can't `+` a literal with
	// an int/float/String); structure and values are preserved exactly.
	if(var1 == 0) {
		return this->musicVolume > 0.0F
			? JavaString((std::string)(JCat() + "Music: " + (int)(this->musicVolume * 100.0F) + "%"))
			: JavaString("Music: OFF");
	}
	if(var1 == 1) {
		return this->soundVolume > 0.0F
			? JavaString((std::string)(JCat() + "Sound: " + (int)(this->soundVolume * 100.0F) + "%"))
			: JavaString("Sound: OFF");
	}
	if(var1 == 2) {
		return JavaString(this->invertMouse ? "Invert mouse: ON" : "Invert mouse: OFF");
	}
	if(var1 == 3) {
		if(this->mouseSensitivity == 0.0F) return JavaString("Sensitivity: *yawn*");
		if(this->mouseSensitivity == 1.0F) return JavaString("Sensitivity: HYPERSPEED!!!");
		return JavaString((std::string)(JCat() + "Sensitivity: " + (int)(this->mouseSensitivity * 200.0F) + "%"));
	}
	if(var1 == 4) {
		return JavaString((std::string)(JCat() + "Render distance: " + s_RENDER_DISTANCES[this->renderDistance]));
	}
	if(var1 == 5) {
		return JavaString(this->viewBobbing ? "View bobbing: ON" : "View bobbing: OFF");
	}
	if(var1 == 6) {
		return JavaString(this->anaglyph ? "3d anaglyph: ON" : "3d anaglyph: OFF");
	}
	if(var1 == 7) {
		return JavaString(this->limitFramerate ? "Limit framerate: ON" : "Limit framerate: OFF");
	}
	if(var1 == 8) {
		return JavaString((std::string)(JCat() + "Difficulty: " + s_DIFFICULTY_LEVELS[this->difficulty]));
	}
	if(var1 == 9) {
		return JavaString(this->fancyGraphics ? "Graphics: FANCY" : "Graphics: FAST");
	}
	return JavaString("");
}

void GameSettings::loadOptions()
{
	try {
	if(!this->optionsFile->exists()) {
	return;
	}
	BufferedReader* var1 = new BufferedReader(new FileReader(this->optionsFile));
	JavaString var2 = "";
	while(true) {
	var2 = var1->readLine();
	if(var2 == NULL) {
	var1->close();
	break;
	}
	JArray<JavaString > var3 = var2.split(":");
	if(var3[0].equals("music")) {
	this->musicVolume = this->parseFloat(var3[1]);
	}
	if(var3[0].equals("sound")) {
	this->soundVolume = this->parseFloat(var3[1]);
	}
	if(var3[0].equals("mouseSensitivity")) {
	this->mouseSensitivity = this->parseFloat(var3[1]);
	}
	if(var3[0].equals("invertYMouse")) {
	this->invertMouse = var3[1].equals("true");
	}
	if(var3[0].equals("viewDistance")) {
	this->renderDistance = Integer::parseInt(var3[1]);
	}
	if(var3[0].equals("bobView")) {
	this->viewBobbing = var3[1].equals("true");
	}
	if(var3[0].equals("anaglyph3d")) {
	this->anaglyph = var3[1].equals("true");
	}
	if(var3[0].equals("limitFramerate")) {
	this->limitFramerate = var3[1].equals("true");
	}
	if(var3[0].equals("difficulty")) {
	this->difficulty = Integer::parseInt(var3[1]);
	}
	if(var3[0].equals("fancyGraphics")) {
	this->fancyGraphics = var3[1].equals("true");
	}
	if(var3[0].equals("skin")) {
	this->skin = var3[1];
	}
	if(var3[0].equals("lastServer")) {
	this->lastServer = var3[1];
	}
	for(int var4 = 0; var4 < this->keyBindings.length; ++var4) {
	if(var3[0].equals((JCat() + "key_" + this->keyBindings[var4]->keyDescription))) {
	this->keyBindings[var4]->keyCode = Integer::parseInt(var3[1]);
	}
	}
	}
	} catch (Exception var5) {
	System::out.println("Failed to load options");
	var5.printStackTrace();
	}
}

float GameSettings::parseFloat(const JavaString& var1)
{
	return (var1 == "true") ? 1.0F : ((var1 == "false") ? 0.0F : Float::parseFloat(var1));
}

void GameSettings::saveOptions()
{
	try {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  PrintWriter var1 = new PrintWriter(new FileWriter(this.optionsFile));
	 *  var1.println("music:" + this.musicVolume);
	 *  ... (one println per option) ...
	 *  var1.close();
	 *  ---- end original Java ----
	 *  PrintWriter is the local no-op shim: every println below is discarded and close() does
	 *  nothing, so no options file is actually written. */
	PrintWriter* var1 = new PrintWriter(new FileWriter(this->optionsFile));
	var1->println((JCat() + "music:" + this->musicVolume));
	var1->println((JCat() + "sound:" + this->soundVolume));
	var1->println((JCat() + "invertYMouse:" + this->invertMouse));
	var1->println((JCat() + "mouseSensitivity:" + this->mouseSensitivity));
	var1->println((JCat() + "viewDistance:" + this->renderDistance));
	var1->println((JCat() + "bobView:" + this->viewBobbing));
	var1->println((JCat() + "anaglyph3d:" + this->anaglyph));
	var1->println((JCat() + "limitFramerate:" + this->limitFramerate));
	var1->println((JCat() + "difficulty:" + this->difficulty));
	var1->println((JCat() + "fancyGraphics:" + this->fancyGraphics));
	var1->println((JCat() + "skin:" + this->skin));
	var1->println((JCat() + "lastServer:" + this->lastServer));
	for(int var2 = 0; var2 < this->keyBindings.length; ++var2) {
	var1->println((JCat() + "key_" + this->keyBindings[var2]->keyDescription + ":" + this->keyBindings[var2]->keyCode));
	}
	var1->close();
	} catch (Exception var3) {
	System::out.println("Failed to save options");
	var3.printStackTrace();
	}
}
