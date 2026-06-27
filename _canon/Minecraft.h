#ifndef MINECRAFT_H
#define MINECRAFT_H

// Auto-generated from Minecraft.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EnumOS2.h"
#include "Runnable.h"

class Canvas;
class Component;
class EffectRenderer;
class EntityPlayer;
class EntityPlayerSP;
class EntityRenderer;
class File;
class FontRenderer;
class GameSettings;
class GuiIngame;
class GuiScreen;
class LoadingScreenRenderer;
class MinecraftApplet;
class ModelBiped;
class MouseHelper;
class MovingObjectPosition;
class OpenGlCapsChecker;
class PlayerController;
class RenderEngine;
class RenderGlobal;
class Session;
class SoundManager;
class TextureLavaFX;
class TexturePackList;
class TextureWaterFX;
class ThreadDownloadResources;
class Timer;
class UnexpectedThrowable;
class World;

class Minecraft : public Runnable {
public:
	Minecraft(Component* var1, Canvas* var2, MinecraftApplet* var3, int var4, int var5, bool var6);
	virtual ~Minecraft();

	PlayerController* playerController;
	bool a;
	int displayWidth;
	int displayHeight;
	OpenGlCapsChecker* glCapabilities;
	Timer* timer;
	World* theWorld;
	RenderGlobal* renderGlobal;
	EntityPlayerSP* thePlayer;
	EffectRenderer* effectRenderer;
	Session* session;
	JavaString minecraftUri;
	Canvas* mcCanvas;
	bool hideQuitButton;
	bool isGamePaused;
	RenderEngine* renderEngine;
	FontRenderer* fontRenderer;
	GuiScreen* currentScreen;
	LoadingScreenRenderer* loadingScreen;
	EntityRenderer* entityRenderer;
	ThreadDownloadResources* downloadResourcesThread;
	int ticksRan;
	int leftClickCounter;
	int tempDisplayWidth;
	int tempDisplayHeight;
	JavaString loadMapUser;
	int loadMapID;
	GuiIngame* ingameGUI;
	bool skipRenderWorld;
	ModelBiped* modelBiped;
	MovingObjectPosition* objectMouseOver;
	GameSettings* gameSettings;
	MinecraftApplet* mcApplet;
	SoundManager* sndManager;
	MouseHelper* mouseHelper;
	TexturePackList* texturePackList;
	File* mcDataDir;
	JavaString serverName;
	int serverPort;
	TextureWaterFX* textureWaterFX;
	TextureLavaFX* textureLavaFX;
	bool running;
	JavaString debugFPS;
	bool isTakingScreenshot;
	long long debugUpdateTime;
	bool inGameHasFocus;
	int ticksSinceLastClick;
	bool field_6288_M;
	long long prevFrameTime;
	int joinPlayerCounter;
	static JArray<long long > s_debugFrameTimes;
	static JArray<long long > s_debugTickTimes;
	static int s_debugTimingIndex;
	static File* s_minecraftDir;

	virtual void displayUnexpectedThrowable(UnexpectedThrowable* var1) = 0;
	virtual void setServer(const JavaString& var1, int var2);
	virtual void startGame();
	virtual void loadScreen();
	virtual void drawSplashTexture(int var1, int var2, int var3, int var4, int var5, int var6);
	static File* getMinecraftDir();
	static File* getAppDir(const JavaString& var0);
	static int getOs();
	virtual void displayGuiScreen(GuiScreen* var1);
	virtual void checkGLError(const JavaString& var1);
	virtual void shutdownMinecraftApplet();
	virtual void run();
	virtual void screenshotListener();
	virtual void displayDebugInfo(long long var1);
	virtual void shutdown();
	virtual void setIngameFocus();
	virtual void setIngameNotInFocus();
	virtual void displayInGameMenu();
	virtual void clickBlockHeld(int var1, bool var2);
	virtual void clickMouse(int var1);
	virtual void toggleFullscreen();
	virtual void resize(int var1, int var2);
	virtual void clickMiddleMouseButton();
	virtual void runTick();
	virtual void forceReload();
	virtual bool isMultiplayerWorld();
	virtual void startWorld(const JavaString& var1);
	virtual void switchDimension();
	virtual void setWorld(World* var1);
	virtual void setWorldWithMessage(World* var1, const JavaString& var2);
	virtual void changeWorld(World* var1, const JavaString& var2, EntityPlayer* var3);
	virtual void loadWorldSpawnArea(const JavaString& var1);
	virtual void installResource(const JavaString& var1, File* var2);
	virtual OpenGlCapsChecker* getGlCapabilities();
	virtual JavaString getDebugInfoRenders();
	virtual JavaString getDebugInfoEntities();
	virtual JavaString getDebugInfoParticles();
	virtual void respawn();
	static void startMainThreadAndLogin(const JavaString& var0, const JavaString& var1);
	static void startMainThread(const JavaString& var0, const JavaString& var1, const JavaString& var2);
	static void main(JArray<JavaString > var0);

	static void initStatics();
};

#endif // MINECRAFT_H