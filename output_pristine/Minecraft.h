#ifndef MINECRAFT_H
#define MINECRAFT_H

// Auto-generated from Minecraft.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class EffectRenderer;
class EntityPlayerSP;
class EntityRenderer;
class FontRenderer;
class GameSettings;
class GuiIngame;
class GuiScreen;
class LoadingScreenRenderer;
class MinecraftApplet;
class MouseHelper;
class MovingObjectPosition;
class OpenGlCapsChecker;
class PlayerController;
class RenderEngine;
class RenderGlobal;
class Session;
class SoundManager;
class TextureLavaFX;
class TextureWaterFX;
class ThreadDownloadResources;
class Timer;
class World;

class Minecraft {
public:
	Minecraft();
	Minecraft(void* /* PORT: was AWT Canvas */ var1, MinecraftApplet* var2, int var3, int var4, bool var5);
	virtual ~Minecraft();

	PlayerController* m_playerController;
	int m_displayWidth;
	int m_displayHeight;
	World* theWorld;
	RenderGlobal* m_renderGlobal;
	EntityPlayerSP* thePlayer;
	EffectRenderer* m_effectRenderer;
	Session* m_session;
	std::string m_minecraftUri;
	void* /* PORT: was AWT Canvas */ m_mcCanvas;
	bool m_appletMode;
	RenderEngine* m_renderEngine;
	FontRenderer* fontRenderer;
	GuiScreen* currentScreen;
	LoadingScreenRenderer* m_loadingScreen;
	EntityRenderer* m_entityRenderer;
	std::string m_loadMapUser;
	int m_loadMapID;
	GuiIngame* m_t;
	bool m_skipRenderWorld;
	MovingObjectPosition* m_objectMouseOver;
	GameSettings* m_options;
	SoundManager* m_sndManager;
	MouseHelper* m_mouseHelper;
	std::string /* PORT: was java.io.File */* m_mcDataDir;
	std::string m_debug;
	bool m_inventoryScreen;
	bool m_D;

	virtual void setServer(const std::string& var1, int var2);
	virtual void displayGuiScreen(GuiScreen* var1);
	virtual void shutdownMinecraftApplet();
	virtual void run();
	virtual void setIngameFocus();
	virtual void displayInGameMenu();
	virtual void toggleFullscreen();
	virtual void generateLevel(int var1, int var2, int var3, int var4);
	virtual void setLevel(World* var1);

private:
	bool m_fullscreen;
	OpenGlCapsChecker* m_glCapabilities;
	Timer* m_timer;
	ThreadDownloadResources* m_downloadResourcesThread;
	int m_ticksRan;
	int m_leftClickCounter;
	int m_tempDisplayWidth;
	int m_tempDisplayHeight;
	MinecraftApplet* m_mcApplet;
	std::string m_server;
	TextureWaterFX* m_textureWaterFX;
	TextureLavaFX* m_textureLavaFX;
	int m_prevFrameTime;

	void inputLock();
	void clickMouse(int var1);
	void resize(int var1, int var2);
	void runTick();

};

#endif // MINECRAFT_H