#ifndef MINECRAFT_H
#define MINECRAFT_H

// Auto-generated from Minecraft.java  (handwritten_alpha override:
// adds the `running` member — the converter never declared it, but both
// Minecraft::run()/shutdown() and LoadingScreenRenderer read it.)
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class EffectRenderer;
class EntityPlayer;
class EntityPlayerSP;
class EntityRenderer;
struct EnumOS2;
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

class Minecraft {
public:
	Minecraft();
	Minecraft(void* /* PORT: was AWT Component */ var1, void* /* PORT: was AWT Canvas */ var2, MinecraftApplet* var3, int var4, int var5, bool var6);
	virtual ~Minecraft();

	PlayerController* m_field_6327_b;
	int m_displayWidth;
	int m_displayHeight;
	World* theWorld;
	RenderGlobal* m_field_6323_f;
	EntityPlayerSP* thePlayer;
	EffectRenderer* m_field_6321_h;
	Session* m_field_6320_i;
	std::string m_field_6319_j;
	void* /* PORT: was AWT Canvas */ m_mcCanvas;
	bool m_field_6317_l;
	bool running;   // PORT: game-loop flag (set in run(), read by LoadingScreenRenderer)
	RenderEngine* m_renderEngine;
	FontRenderer* fontRenderer;
	GuiScreen* currentScreen;
	LoadingScreenRenderer* m_loadingScreen;
	EntityRenderer* m_field_9243_r;
	std::string m_field_6310_s;
	int m_field_6309_t;
	GuiIngame* m_ingameGUI;
	bool m_field_6307_v;
	ModelBiped* m_field_9242_w;
	MovingObjectPosition* m_objectMouseOver;
	GameSettings* m_gameSettings;
	SoundManager* m_sndManager;
	MouseHelper* m_mouseHelper;
	TexturePackList* m_texturePackList;
	std::string /* PORT: was java.io.File */* m_field_6297_D;
	static long long* s_field_9240_E;
	static long long* s_field_9239_F;
	static int s_field_9238_G;
	std::string m_field_6292_I;
	bool m_field_6289_L;
	bool m_field_6288_M;

	virtual void func_4007_a(UnexpectedThrowable* var1);  // PORT: was pure-virtual (impl'd by the AWT/applet subclass); made concrete so the 360 boot can `new Minecraft()`
	virtual void func_6258_a(const std::string& var1, int var2);
	virtual void startGame();
	virtual void func_6274_a(int var1, int var2, int var3, int var4, int var5, int var6);
	static std::string /* PORT: was java.io.File */* getMinecraftDir();
	static std::string /* PORT: was java.io.File */* getAppDir(const std::string& var0);
	virtual void displayGuiScreen(GuiScreen* var1);
	virtual void func_6266_c();
	virtual void run();
	virtual void shutdown();
	virtual void func_6259_e();
	virtual void func_6273_f();
	virtual void func_6252_g();
	virtual void toggleFullscreen();
	virtual void runTick();
	virtual bool isMultiplayerWorld();
	virtual void func_6247_b(const std::string& var1);
	virtual void func_6237_k();
	virtual void func_6261_a(World* var1);
	virtual void func_6263_a(World* var1, const std::string& var2);
	virtual void func_6256_a(World* var1, const std::string& var2, EntityPlayer* var3);
	virtual void installResource(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
	virtual OpenGlCapsChecker* func_6251_l();
	virtual const std::string& func_6241_m();
	virtual const std::string& func_6262_n();
	virtual const std::string& func_6245_o();
	virtual void respawn();
	static void func_6269_a(const std::string& var0, const std::string& var1);
	static void func_6253_a(const std::string& var0, const std::string& var1, const std::string& var2);
	static void main(const std::string& var0);

public: // was protected
	MinecraftApplet* m_mcApplet;

public: // was private
	bool m_a;
	OpenGlCapsChecker* m_glCapabilities;
	Timer* m_timer;
	ThreadDownloadResources* m_downloadResourcesThread;
	int m_ticksRan;
	int m_field_6282_S;
	int m_field_9236_T;
	int m_field_9235_U;
	std::string m_field_9234_V;
	int m_field_9233_W;
	TextureWaterFX* m_field_9232_X;
	TextureLavaFX* m_field_9231_Y;
	static std::string /* PORT: was java.io.File */* s_minecraftDir;
	bool m_field_6291_J;
	long long m_field_6290_K;
	int m_field_6302_aa;
	long long m_field_6287_N;
	int m_field_6300_ab;

	void loadScreen();
	EnumOS2* getOs();
	void checkGLError(const std::string& var1);
	void func_6248_s();
	void func_6238_a(long long var1);
	void func_6254_a(int var1, bool var2);
	void clickMouse(int var1);
	void resize(int var1, int var2);
	void clickMiddleMouseButton();
	void forceReload();
	void func_6255_d(const std::string& var1);

};

#endif // MINECRAFT_H
