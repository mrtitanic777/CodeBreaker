#include "stdafx.h"
// Hand-ported from Minecraft.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Ported against the converted Minecraft.h (obfuscated alpha field/method names).
// The auto-converted .cpp had ~62 errors clustered as:
//   - AWT/Swing leftovers in startGame()/getAppDir()/getOs()/the applet-launcher path
//     (Graphics/Color/Display/System/File/Frame/Canvas/MinecraftImpl/GameWindowListener),
//   - Tesselator copied BY VALUE (getInstance() returns a reference — must bind by ref),
//   - `running` referenced but never declared in the .h (made a file-scope static),
//   - ThreadDownloadResources / MinecraftApplet are empty stub headers (no class),
//   - std::string compared with `!= NULL`, a bogus `s_field_9240_E_count` symbol,
//     a malformed checkGLError body, and Mouse.getEventDWheel().
// Platform glue (always-fullscreen, hardcoded data dir, no AWT) mirrors the indev port.

#include "Minecraft.h"
#include "Common.h"
#include "WorldRenderer.h"
#include "BlockGrass.h"
#include "EffectRenderer.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "LoadingScreenRenderer.h"
#include "MinecraftApplet.h"
#include "ModelBiped.h"
#include "MouseHelper.h"
#include "MovingObjectPosition.h"
#include "OpenGlCapsChecker.h"
#include "PlayerController.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "Session.h"
#include "SoundManager.h"
#include "TextureLavaFX.h"
#include "TexturePackList.h"
#include "TextureWaterFX.h"
#include "ThreadDownloadResources.h"
#include "Timer.h"
#include "UnexpectedThrowable.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Block.h"
#include "InventoryPlayer.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "MovementInputFromOptions.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Vec3D.h"
#include "GLAllocation.h"
#include "GuiConflictWarning.h"
#include "GuiConnecting.h"
#include "GuiGameOver.h"
#include "GuiIngameMenu.h"
#include "GuiMainMenu.h"
#include "GuiUnused.h"
#include "PlayerControllerTest.h"
#include "RenderManager.h"
#include "Teleporter.h"
#include "WorldProvider.h"
#include "WorldProviderHell.h"

// PORT: `running` is referenced by run()/shutdown() but the converted .h never declared it
// (the indev .h had it as a member; the alpha .h doesn't). Minecraft is effectively a
// singleton on the 360, so a file-scope flag is equivalent and keeps the header unchanged.
// running is now a Minecraft member (declared in Minecraft.h override)

// PORT: `field_6316_m` (Java's "game is paused" flag) is referenced BARE throughout run()/
// runTick() but the converted .h never declared it either. File-scope storage resolves the
// unqualified name without touching the header. Defaults false (Java default for a boolean).
static bool field_6316_m = false;

// PORT: the converted code referenced a symbol `s_field_9240_E_count` (the Java array
// length of field_9240_E). The header allocates the timing arrays at length 512.
static const int FRAME_TIMES_COUNT = 512;

long long* Minecraft::s_field_9240_E = new long long[FRAME_TIMES_COUNT];
long long* Minecraft::s_field_9239_F = new long long[FRAME_TIMES_COUNT];
int Minecraft::s_field_9238_G = 0;
std::string /* PORT: was java.io.File */* Minecraft::s_minecraftDir = NULL;

Minecraft::Minecraft()
	: m_field_6327_b(NULL)
	, m_displayWidth(0)
	, m_displayHeight(0)
	, theWorld(NULL)
	, m_field_6323_f(NULL)
	, thePlayer(NULL)
	, m_field_6321_h(NULL)
	, m_field_6320_i(NULL)
	, m_field_6319_j("")
	, m_mcCanvas(NULL)
	, m_field_6317_l(true)
	, running(false)   // PORT: game-loop flag; set true in run(), cleared in shutdown()
	, m_renderEngine(NULL)
	, fontRenderer(NULL)
	, currentScreen(NULL)
	, m_loadingScreen(new LoadingScreenRenderer(this))
	, m_field_9243_r(new EntityRenderer(this))
	, m_field_6310_s("")
	, m_field_6309_t(0)
	, m_ingameGUI(NULL)
	, m_field_6307_v(false)
	, m_field_9242_w(new ModelBiped(0.0f))
	, m_objectMouseOver(NULL)
	, m_gameSettings(NULL)
	, m_sndManager(new SoundManager())
	, m_mouseHelper(NULL)
	, m_texturePackList(NULL)
	, m_field_6297_D(NULL)
	, m_field_6292_I("")
	, m_field_6289_L(false)
	, m_field_6288_M(false)
	, m_mcApplet(NULL)
	, m_a(false)
	, m_glCapabilities(NULL)
	, m_timer(new Timer(20.0f))
	, m_downloadResourcesThread(NULL)
	, m_ticksRan(0)
	, m_field_6282_S(0)
	, m_field_9236_T(0)
	, m_field_9235_U(0)
	, m_field_9234_V("")
	, m_field_9233_W(0)
	, m_field_9232_X(new TextureWaterFX())
	, m_field_9231_Y(new TextureLavaFX())
	, m_field_6291_J(false)
	, m_field_6290_K(-1LL)
	, m_field_6302_aa(0)
	, m_field_6287_N(GetTickCount())
	, m_field_6300_ab(0)
{
}

// PORT: Java Swing/AWT constructor (Component/Canvas/applet host) is not used on the 360.
// A converter postfix already commented out the original; left commented intentionally.
// Minecraft::Minecraft(void* var1, void* var2, MinecraftApplet* var3, int var4, int var5, bool var6) { ... }

Minecraft::~Minecraft() {
	m_field_6327_b = NULL;
	m_glCapabilities = NULL;
	m_timer = NULL;
	theWorld = NULL;
	m_field_6323_f = NULL;
	thePlayer = NULL;
	m_field_6321_h = NULL;
	m_field_6320_i = NULL;
	m_mcCanvas = NULL;
	m_renderEngine = NULL;
	fontRenderer = NULL;
	currentScreen = NULL;
	m_loadingScreen = NULL;
	m_field_9243_r = NULL;
	m_downloadResourcesThread = NULL;
	m_ingameGUI = NULL;
	m_field_9242_w = NULL;
	m_objectMouseOver = NULL;
	m_gameSettings = NULL;
	m_mcApplet = NULL;
	m_sndManager = NULL;
	m_mouseHelper = NULL;
	m_texturePackList = NULL;
	m_field_6297_D = NULL;
	m_field_9232_X = NULL;
	m_field_9231_Y = NULL;
}

void Minecraft::func_6258_a(const std::string& var1, int var2) {
		m_field_9234_V = var1;
		m_field_9233_W = var2;
}

void Minecraft::startGame() {
	// PORT: AWT canvas/Graphics path and LWJGL Display setup replaced by the fixed
	// 360 backbuffer. Always fullscreen at the console resolution.
		m_displayWidth = SCREEN_WIDTH;
		m_displayHeight = SCREEN_HEIGHT;
		if(m_displayWidth <= 0) {
			m_displayWidth = 1;
		}

		if(m_displayHeight <= 0) {
			m_displayHeight = 1;
		}

  // PORT: no window title on 360  // was: Display.setTitle("Minecraft Minecraft Alpha v1.2.6");
  // PORT: D3D9 device already created at boot  // was: Display.create();

		RenderManager::instance->m_field_4236_f = new ItemRenderer(this);
		m_field_6297_D = getMinecraftDir();
		m_gameSettings = new GameSettings(this, m_field_6297_D);
		m_texturePackList = new TexturePackList(this, m_field_6297_D);
		m_renderEngine = new RenderEngine(m_texturePackList, m_gameSettings);
		fontRenderer = new FontRenderer(m_gameSettings, "/font/default.png", m_renderEngine);
		loadScreen();
  // STUBBED: Keyboard.create();  // XInput replaces Keyboard
  // STUBBED: Mouse.create();  // no cursor on 360
		m_mouseHelper = new MouseHelper(m_mcCanvas);
  // STUBBED: Controllers.create();  // XInput needs no init

		checkGLError("Pre startup");
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  // Shade model is shader-based on Xbox 360
  // PORT: D3D9 Clear()  // was: GL11.glClearDepth(1.0);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(255.0f * 0.1F));
  // PORT: GL constant -> D3D9  // was: GL11.glCullFace(GL11.GL_BACK);
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
		checkGLError("Startup");
		m_glCapabilities = NULL;
		m_sndManager->func_340_a(m_gameSettings);
		// Animated texture FX registered at init
		m_field_6323_f = new RenderGlobal(this, m_renderEngine);
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
    g_pd3dDevice->SetViewport(&_vp); }
		m_field_6321_h = new EffectRenderer(theWorld, m_renderEngine);

  // PORT: resource-download thread (network) n/a on 360
  // was: m_downloadResourcesThread = new ThreadDownloadResources(m_field_6297_D, this); ...->start();

		checkGLError("Post startup");
		m_ingameGUI = new GuiIngame(this);
		if(!m_field_9234_V.empty()) {
			displayGuiScreen(new GuiConnecting(this, m_field_9234_V, m_field_9233_W));
		} else {
			displayGuiScreen(new GuiMainMenu());
		}

}

void Minecraft::loadScreen() {
		ScaledResolution var1 = ScaledResolution(m_displayWidth, m_displayHeight);
		int var2 = var1.getScaledWidth();
		int var3 = var1.getScaledHeight();
  g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), (float)((double)var2 + 0.5), (float)((double)var3 + 0.5), (float)(0.5), (float)(1000.0), (float)(3000.0));
    Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 2000.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
    g_pd3dDevice->SetViewport(&_vp); }
		Tesselator& var4 = Tesselator::getInstance();
  // Lighting is shader-based on Xbox 360 (disabled)
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  Shaders::setFogEnabled(false);
  g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/title/mojang.png")));
		var4.init();
		var4.colorI(16777215);
		var4.vertexUV(0.0f, (float)m_displayHeight, 0.0f, 0.0f, 0.0f);
		var4.vertexUV((float)m_displayWidth, (float)m_displayHeight, 0.0f, 0.0f, 0.0f);
		var4.vertexUV((float)m_displayWidth, 0.0f, 0.0f, 0.0f, 0.0f);
		var4.vertexUV(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		var4.flush();
		short var5 = 256;
		short var6 = 256;
  var4.color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var4.colorI(16777215);
		func_6274_a((m_displayWidth / 2 - var5) / 2, (m_displayHeight / 2 - var6) / 2, 0, 0, var5, var6);
  // Lighting is shader-based on Xbox 360 (disabled)
  Shaders::setFogEnabled(false);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(255.0f * 0.1F));
  // PORT: D3D9 present  // was: Display.swapBuffers();
  g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL); g_pd3dDevice->BeginScene();
}

void Minecraft::func_6274_a(int var1, int var2, int var3, int var4, int var5, int var6) {
		float var7 = 0.00390625F;
		float var8 = 0.00390625F;
		Tesselator& var9 = Tesselator::getInstance();
		var9.init();
		var9.vertexUV((float)(var1 + 0), (float)(var2 + var6), 0.0f, (float)((float)(var3 + 0) * var7), (float)((float)(var4 + var6) * var8));
		var9.vertexUV((float)(var1 + var5), (float)(var2 + var6), 0.0f, (float)((float)(var3 + var5) * var7), (float)((float)(var4 + var6) * var8));
		var9.vertexUV((float)(var1 + var5), (float)(var2 + 0), 0.0f, (float)((float)(var3 + var5) * var7), (float)((float)(var4 + 0) * var8));
		var9.vertexUV((float)(var1 + 0), (float)(var2 + 0), 0.0f, (float)((float)(var3 + 0) * var7), (float)((float)(var4 + 0) * var8));
		var9.flush();
}

std::string /* PORT: was java.io.File */* Minecraft::getMinecraftDir() {
		if(s_minecraftDir == NULL) {
			s_minecraftDir = getAppDir("minecraft");
		}

		return s_minecraftDir;
}

std::string /* PORT: was java.io.File */* Minecraft::getAppDir(const std::string& var0) {
	// PORT: the 360 has no user.home / APPDATA and no EnumOSMappingHelper switch. Like the
	// indev port, return a fixed data directory on the console cache drive. var0 is the app
	// subdir ("minecraft"). The directory is created lazily by callers via the File shim.
		(void)var0;
		std::string* var2 = new std::string("cache:\\.minecraft\\");
		return var2;
}

EnumOS2* Minecraft::getOs() {
	// PORT: fixed platform on the 360 — always report Windows-like (the only branch that
	// matters for getAppDir, which is itself hardcoded). Avoids System.getProperty.
		return EnumOS2::s_c;
}

void Minecraft::displayGuiScreen(GuiScreen* var1) {
		if(!(dynamic_cast<GuiUnused*>(currentScreen) != NULL)) {
			if(currentScreen != NULL) {
				currentScreen->onGuiClosed();
			}

			if(var1 == NULL && theWorld == NULL) {
				var1 = new GuiMainMenu();
			} else if(var1 == NULL && thePlayer->health <= 0) {
				var1 = new GuiGameOver();
			}

			currentScreen = (GuiScreen*)var1;
			if(var1 != NULL) {
				func_6273_f();
				ScaledResolution var2 = ScaledResolution(m_displayWidth, m_displayHeight);
				int var3 = var2.getScaledWidth();
				int var4 = var2.getScaledHeight();
				((GuiScreen*)var1)->setWorldAndResolution(this, var3, var4);
				m_field_6307_v = false;
			} else {
				func_6259_e();
			}

		}
}

void Minecraft::checkGLError(const std::string& var1) {
	// PORT: D3D9 has no glGetError equivalent here; nothing to report on the 360.
		(void)var1;
}

void Minecraft::func_6266_c() {
	// PORT: applet host n/a on 360 (m_mcApplet is always NULL; MinecraftApplet is a stub).

  { // was try
			if(m_downloadResourcesThread != NULL) {
				/* PORT: resource-download thread n/a on 360 */;
			}
		} if(false) { // was catch(Exception var8)
		}

  { // was try
			DbgPrintF("%s\n", "Stopping!");
			func_6261_a(NULL);

   { // was try
				GLAllocation::deleteTexturesAndDisplayLists();
			} if(false) { // was catch(Exception var6)
			}

			m_sndManager->closeMinecraft();
   // STUBBED: Mouse.destroy();  // no cursor on 360
   // STUBBED: Keyboard.destroy();  // XInput replaces Keyboard
  } { // was finally — cleanup code follows
   // PORT: D3D9 Release  // was: Display.destroy();
		}

		(void)0;
}

void Minecraft::run() {
		running = true;

  { // was try
			startGame();
		} if(false) { // was catch(Exception var15)
   // Dead handler: /EH is off and startGame() doesn't throw on the 360.
			func_4007_a(new UnexpectedThrowable("Failed to start game", NULL));
			return;
		}

  { // was try
   { // was try
				long var1 = GetTickCount();
				int var3 = 0;

				while(running && (m_mcApplet == NULL)) {
					{ static int _ri = 0; if (_ri < 3) { DbgPrintF("[RUN] iter%d theWorld=%p curScr=%p dw=%d dh=%d v6307=%d\n", _ri, (void*)theWorld, (void*)currentScreen, m_displayWidth, m_displayHeight, (int)m_field_6307_v); _ri++; } }
					AxisAlignedBB::clearBoundingBoxPool();
					Vec3D::initialize();
     if (false) { // PORT: LWJGL window state (Display) fixed on 360
						shutdown();
					}

					if(field_6316_m && theWorld != NULL) {
						float var4 = m_timer->m_renderPartialTicks;
						m_timer->updateTimer();
						m_timer->m_renderPartialTicks = var4;
					} else {
						m_timer->updateTimer();
					}

					long var19 = GetNanoTime();

					for(int var6 = 0; var6 < m_timer->m_elapsedTicks; ++var6) {
						++m_ticksRan;

      { // was try
							runTick();
						} if(false) { // was catch(MinecraftException var14)
							theWorld = NULL;
							func_6261_a(NULL);
							displayGuiScreen(new GuiConflictWarning());
						}
					}

					long var20 = GetNanoTime() - var19;
					checkGLError("Pre render");
					m_sndManager->func_338_a(thePlayer, m_timer->m_renderPartialTicks);
     g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
					if(theWorld != NULL && !theWorld->m_multiplayerWorld) {
						while(theWorld->func_6465_g()) {
						}
					}

					if(theWorld != NULL && theWorld->m_multiplayerWorld) {
						theWorld->func_6465_g();
					}

					if(m_gameSettings->m_limitFramerate) {
						Sleep(5);
					}

     // PORT: XInput poll; Display.update() handled by Present below.

					if(!m_field_6307_v) {
						if(m_field_6327_b != NULL) {
							m_field_6327_b->func_6467_a(m_timer->m_renderPartialTicks);
						}

						m_field_9243_r->func_4136_b(m_timer->m_renderPartialTicks);
					}

     if (false) { // PORT: LWJGL window state (Display) fixed on 360
						if(m_a) {
							toggleFullscreen();
						}

						Sleep(10);
					}

					(void)var20;
					Sleep(0);

					func_6248_s();
					if(m_mcCanvas != NULL && !m_a && (SCREEN_WIDTH != m_displayWidth || SCREEN_HEIGHT != m_displayHeight)) {
						m_displayWidth = SCREEN_WIDTH;
						m_displayHeight = SCREEN_HEIGHT;
						if(m_displayWidth <= 0) {
							m_displayWidth = 1;
						}

						if(m_displayHeight <= 0) {
							m_displayHeight = 1;
						}

						resize(m_displayWidth, m_displayHeight);
					}

					checkGLError("Post render");
					++var3;

					for(field_6316_m = !isMultiplayerWorld() && currentScreen != NULL && currentScreen->doesGuiPauseGame(); (long)GetTickCount() >= var1 + 1000L; var3 = 0) {
						m_field_6292_I = toStr(var3) + " fps, " + toStr(WorldRenderer::s_field_1762_b) + " chunk updates";
						WorldRenderer::s_field_1762_b = 0;
						var1 += 1000L;
					}
				}
			} if(false) { // was catch(MinecraftError var16)
			} if(false) { // was catch(Throwable var17)
				theWorld = NULL;
				func_4007_a(new UnexpectedThrowable("Unexpected error", NULL));
			}

  } { // was finally — cleanup code follows
		}
}

void Minecraft::func_6248_s() {
  // PORT: F2 screenshot (Keyboard) n/a on 360 — XInput has no equivalent binding here.
}

void Minecraft::func_6238_a(long long var1) {
		long var3 = 16666666L;
		if(m_field_6290_K == -1L) {
			m_field_6290_K = GetNanoTime();
		}

		long var5 = GetNanoTime();
		s_field_9239_F[s_field_9238_G & FRAME_TIMES_COUNT - 1] = var1;
		s_field_9240_E[s_field_9238_G++ & FRAME_TIMES_COUNT - 1] = var5 - m_field_6290_K;
		m_field_6290_K = var5;
  g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), (float)((double)this->m_displayWidth + 0.5), (float)((double)this->m_displayHeight + 0.5), (float)(0.5), (float)(1000.0), (float)(3000.0));
    Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 2000.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // Line width not supported on Xbox 360 GPU
  g_pd3dDevice->SetTexture(0, NULL);
		Tesselator& var7 = Tesselator::getInstance();
		var7.init( /* PORT: draw mode */ 7);
		int var8 = (int)(var3 / 200000L);
		var7.colorI(536870912);
		var7.vertex(0.0f, (float)(m_displayHeight - var8), 0.0f);
		var7.vertex(0.0f, (float)m_displayHeight, 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)m_displayHeight, 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)(m_displayHeight - var8), 0.0f);
		var7.colorI(538968064);
		var7.vertex(0.0f, (float)(m_displayHeight - var8 * 2), 0.0f);
		var7.vertex(0.0f, (float)(m_displayHeight - var8), 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)(m_displayHeight - var8), 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)(m_displayHeight - var8 * 2), 0.0f);
		var7.flush();
		long var9 = 0L;

		int var11;
		for(var11 = 0; var11 < FRAME_TIMES_COUNT; ++var11) {
			var9 += s_field_9240_E[var11];
		}

		var11 = (int)(var9 / 200000L / (long)FRAME_TIMES_COUNT);
		var7.init( /* PORT: draw mode */ 7);
		var7.colorI(541065216);
		var7.vertex(0.0f, (float)(m_displayHeight - var11), 0.0f);
		var7.vertex(0.0f, (float)m_displayHeight, 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)m_displayHeight, 0.0f);
		var7.vertex((float)FRAME_TIMES_COUNT, (float)(m_displayHeight - var11), 0.0f);
		var7.flush();
		var7.init( /* PORT: draw mode */ 1);

		for(int var12 = 0; var12 < FRAME_TIMES_COUNT; ++var12) {
			int var13 = (var12 - s_field_9238_G & FRAME_TIMES_COUNT - 1) * 255 / FRAME_TIMES_COUNT;
			int var14 = var13 * var13 / 255;
			var14 = var14 * var14 / 255;
			int var15 = var14 * var14 / 255;
			var15 = var15 * var15 / 255;
			if(s_field_9240_E[var12] > var3) {
				var7.colorI(-16777216 + var14 * 65536);
			} else {
				var7.colorI(-16777216 + var14 * 256);
			}

			long var16 = s_field_9240_E[var12] / 200000L;
			long var18 = s_field_9239_F[var12] / 200000L;
			var7.vertex((float)((float)var12 + 0.5F), (float)((float)((long)m_displayHeight - var16) + 0.5F), 0.0f);
			var7.vertex((float)((float)var12 + 0.5F), (float)((float)m_displayHeight + 0.5F), 0.0f);
			var7.colorI(-16777216 + var14 * 65536 + var14 * 256 + var14 * 1);
			var7.vertex((float)((float)var12 + 0.5F), (float)((float)((long)m_displayHeight - var16) + 0.5F), 0.0f);
			var7.vertex((float)((float)var12 + 0.5F), (float)((float)((long)m_displayHeight - (var16 - var18)) + 0.5F), 0.0f);
		}

		var7.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

void Minecraft::shutdown() {
		running = false;
}

void Minecraft::func_6259_e() {
  if (true) { // PORT: LWJGL window state (Display) fixed on 360
			if(!m_field_6289_L) {
				m_field_6289_L = true;
				m_mouseHelper->func_774_a();
				displayGuiScreen(NULL);
				m_field_6302_aa = m_ticksRan + 10000;
			}
		}
}

void Minecraft::func_6273_f() {
		if(m_field_6289_L) {
			if(thePlayer != NULL) {
				thePlayer->func_458_k();
			}

			m_field_6289_L = false;
			m_mouseHelper->func_773_b();
		}
}

void Minecraft::func_6252_g() {
		if(currentScreen == NULL) {
			displayGuiScreen(new GuiIngameMenu());
		}
}

void Minecraft::func_6254_a(int var1, bool var2) {
		if(!m_field_6327_b->m_field_1064_b) {
			if(var1 != 0 || m_field_6282_S <= 0) {
				if(var2 && m_objectMouseOver != NULL && m_objectMouseOver->m_typeOfHit == 0 && var1 == 0) {
					int var3 = m_objectMouseOver->m_blockX;
					int var4 = m_objectMouseOver->m_blockY;
					int var5 = m_objectMouseOver->m_blockZ;
					m_field_6327_b->sendBlockRemoving(var3, var4, var5, m_objectMouseOver->m_sideHit);
					m_field_6321_h->func_1191_a(var3, var4, var5, m_objectMouseOver->m_sideHit);
				} else {
					m_field_6327_b->func_6468_a();
				}

			}
		}
}

void Minecraft::clickMouse(int var1) {
		if(var1 != 0 || m_field_6282_S <= 0) {
			if(var1 == 0) {
				thePlayer->func_457_w();
			}

			bool var2 = true;
			if(m_objectMouseOver == NULL) {
				if(var1 == 0 && !(dynamic_cast<PlayerControllerTest*>(m_field_6327_b) != NULL)) {
					m_field_6282_S = 10;
				}
			} else if(m_objectMouseOver->m_typeOfHit == 1) {
				if(var1 == 0) {
					m_field_6327_b->func_6472_b(thePlayer, m_objectMouseOver->m_entityHit);
				}

				if(var1 == 1) {
					m_field_6327_b->func_6475_a(thePlayer, m_objectMouseOver->m_entityHit);
				}
			} else if(m_objectMouseOver->m_typeOfHit == 0) {
				int var3 = m_objectMouseOver->m_blockX;
				int var4 = m_objectMouseOver->m_blockY;
				int var5 = m_objectMouseOver->m_blockZ;
				int var6 = m_objectMouseOver->m_sideHit;
				Block* var7 = Block::blocksList[theWorld->getBlockId(var3, var4, var5)];
				if(var1 == 0) {
					theWorld->onBlockHit(var3, var4, var5, m_objectMouseOver->m_sideHit);
					if(var7 != Block::s_bedrock || thePlayer->m_field_9371_f >= 100) {
						m_field_6327_b->clickBlock(var3, var4, var5, m_objectMouseOver->m_sideHit);
					}
				} else {
					ItemStack* var8 = thePlayer->m_inventory->getCurrentItem();
					int var9 = var8 != NULL ? var8->stackSize : 0;
					if(m_field_6327_b->sendPlaceBlock(thePlayer, theWorld, var8, var3, var4, var5, var6)) {
						var2 = false;
						thePlayer->func_457_w();
					}

					if(var8 == NULL) {
						return;
					}

					if(var8->stackSize == 0) {
						thePlayer->m_inventory->m_mainInventory[thePlayer->m_inventory->m_currentItem] = NULL;
					} else if(var8->stackSize != var9) {
						m_field_9243_r->m_field_1395_a->func_9449_b();
					}
				}
			}

			if(var2 && var1 == 1) {
				ItemStack* var10 = thePlayer->m_inventory->getCurrentItem();
				if(var10 != NULL && m_field_6327_b->sendUseItem(thePlayer, theWorld, var10)) {
					m_field_9243_r->m_field_1395_a->func_9450_c();
				}
			}

		}
}

void Minecraft::toggleFullscreen() {
  { // was try
			m_a = !m_a;
			DbgPrintF("%s\n", "Toggle fullscreen!");
			if(m_a) {
    // PORT: D3D9 present params  // was: Display.setDisplayMode(Display.getDesktopDisplayMode());
				m_displayWidth = SCREEN_WIDTH;
				m_displayHeight = SCREEN_HEIGHT;
				if(m_displayWidth <= 0) {
					m_displayWidth = 1;
				}

				if(m_displayHeight <= 0) {
					m_displayHeight = 1;
				}
			} else {
				if(m_mcCanvas != NULL) {
					m_displayWidth = SCREEN_WIDTH;
					m_displayHeight = SCREEN_HEIGHT;
				} else {
					m_displayWidth = m_field_9236_T;
					m_displayHeight = m_field_9235_U;
				}

				if(m_displayWidth <= 0) {
					m_displayWidth = 1;
				}

				if(m_displayHeight <= 0) {
					m_displayHeight = 1;
				}

    // PORT: D3D9 present params  // was: Display.setDisplayMode(new DisplayMode(this.field_9236_T, this.field_9235_U));
			}

			func_6273_f();
   // PORT: always fullscreen on 360  // was: Display.setFullscreen(this.a);
   g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL); g_pd3dDevice->BeginScene();
			Sleep(1000);
			if(m_a) {
				func_6259_e();
			}

			if(currentScreen != NULL) {
				func_6273_f();
				resize(m_displayWidth, m_displayHeight);
			}

			DbgPrintF("%s\n", ("Size: " + toStr(m_displayWidth) + ", " + toStr(m_displayHeight)).c_str());
		} if(false) { // was catch(Exception var2)
		}

}

void Minecraft::resize(int var1, int var2) {
		if(var1 <= 0) {
			var1 = 1;
		}

		if(var2 <= 0) {
			var2 = 1;
		}

		m_displayWidth = var1;
		m_displayHeight = var2;
		if(currentScreen != NULL) {
			ScaledResolution var3 = ScaledResolution(var1, var2);
			int var4 = var3.getScaledWidth();
			int var5 = var3.getScaledHeight();
			currentScreen->setWorldAndResolution(this, var4, var5);
		}

}

void Minecraft::clickMiddleMouseButton() {
		if(m_objectMouseOver != NULL) {
			int var1 = theWorld->getBlockId(m_objectMouseOver->m_blockX, m_objectMouseOver->m_blockY, m_objectMouseOver->m_blockZ);
			if(var1 == Block::s_grass->blockID) {
				var1 = Block::s_dirt->blockID;
			}

			if(var1 == Block::s_stairDouble->blockID) {
				var1 = Block::s_stairSingle->blockID;
			}

			if(var1 == Block::s_bedrock->blockID) {
				var1 = Block::s_stone->blockID;
			}

			thePlayer->m_inventory->setCurrentItem(var1, dynamic_cast<PlayerControllerTest*>(m_field_6327_b) != NULL);
		}

}

void Minecraft::runTick() {
		m_ingameGUI->func_555_a();
		m_field_9243_r->func_910_a(1.0F);
		if(thePlayer != NULL) {
			thePlayer->func_6420_o();
		}

		if(!field_6316_m && theWorld != NULL) {
			m_field_6327_b->func_6474_c();
		}

  g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/terrain.png")));
		if(!field_6316_m) {
			m_renderEngine->func_1067_a();
		}

		if(currentScreen == NULL && thePlayer != NULL && thePlayer->health <= 0) {
			displayGuiScreen(NULL);
		}

		if(currentScreen != NULL) {
			m_field_6302_aa = m_ticksRan + 10000;
		}

		if(currentScreen != NULL) {
			currentScreen->handleInput();
			if(currentScreen != NULL) {
				currentScreen->updateScreen();
			}
		}

	// PORT: the Java Mouse/Keyboard event-poll loop (LWJGL) is replaced by XInput handled
	// in GuiScreen::handleInput / EntityPlayerSP. The original blocking next()/event loops
	// are not portable; in-game button handling lives in the controller input path.
		if(currentScreen != NULL) {
			currentScreen->handleMouseInput();
		}

		if(theWorld != NULL) {
			if(thePlayer != NULL) {
				++m_field_6300_ab;
				if(m_field_6300_ab == 30) {
					m_field_6300_ab = 0;
					theWorld->func_705_f(thePlayer);
				}
			}

			theWorld->m_difficultySetting = m_gameSettings->m_difficulty;
			if(theWorld->m_multiplayerWorld) {
				theWorld->m_difficultySetting = 3;
			}

			if(!field_6316_m) {
				m_field_9243_r->func_911_a();
			}

			if(!field_6316_m) {
				m_field_6323_f->func_945_d();
			}

			if(!field_6316_m) {
				theWorld->func_633_c();
			}

			if(!field_6316_m || isMultiplayerWorld()) {
				theWorld->tick();
			}

			if(!field_6316_m && theWorld != NULL) {
				theWorld->randomDisplayUpdates((int)floor(thePlayer->posX), (int)floor(thePlayer->posY), (int)floor(thePlayer->posZ));
			}

			if(!field_6316_m) {
				m_field_6321_h->func_1193_a();
			}
		}

		m_field_6287_N = GetTickCount();
}

void Minecraft::forceReload() {
		DbgPrintF("%s\n", "FORCING RELOAD!");
		m_sndManager = new SoundManager();
		m_sndManager->func_340_a(m_gameSettings);
  // PORT: resource-download thread n/a on 360  // was: m_downloadResourcesThread->reloadResources();
}

bool Minecraft::isMultiplayerWorld() {
		return theWorld != NULL && theWorld->m_multiplayerWorld;
}

void Minecraft::func_6247_b(const std::string& var1) {
		func_6261_a(NULL);
		(void)0;
		// PORT: World's ctor takes std::string* (the converter mapped java.io.File -> std::string*),
		// so build the saves-dir path string and pass a pointer to it (was `new File(dir, "saves")`).
		std::string* var2dir = new std::string((getMinecraftDir() ? *getMinecraftDir() : std::string()) + "saves\\");
		World* var2 = new World(var2dir, var1);
		if(var2->m_field_1033_r) {
			func_6263_a(var2, "Generating level");
		} else {
			func_6263_a(var2, "Loading level");
		}

}

void Minecraft::func_6237_k() {
		if(thePlayer->m_dimension == -1) {
			thePlayer->m_dimension = 0;
		} else {
			thePlayer->m_dimension = -1;
		}

		theWorld->setEntityDead(thePlayer);
		thePlayer->isDead = false;
		double var1 = thePlayer->posX;
		double var3 = thePlayer->posZ;
		double var5 = 8.0f;
		World* var7 = NULL;
		if(thePlayer->m_dimension == -1) {
			var1 /= var5;
			var3 /= var5;
			thePlayer->setLocationAndAngles(var1, thePlayer->posY, var3, thePlayer->rotationYaw, thePlayer->rotationPitch);
			theWorld->func_4084_a(thePlayer, false);
			var7 = new World(theWorld, new WorldProviderHell());
			func_6256_a(var7, "Entering the Nether", thePlayer);
		} else {
			var1 *= var5;
			var3 *= var5;
			thePlayer->setLocationAndAngles(var1, thePlayer->posY, var3, thePlayer->rotationYaw, thePlayer->rotationPitch);
			theWorld->func_4084_a(thePlayer, false);
			var7 = new World(theWorld, new WorldProvider());
			func_6256_a(var7, "Leaving the Nether", thePlayer);
		}

		thePlayer->worldObj = theWorld;
		thePlayer->setLocationAndAngles(var1, thePlayer->posY, var3, thePlayer->rotationYaw, thePlayer->rotationPitch);
		theWorld->func_4084_a(thePlayer, false);
		(new Teleporter())->func_4107_a(theWorld, thePlayer);
}

void Minecraft::func_6261_a(World* var1) {
		func_6263_a(var1, "");
}

void Minecraft::func_6263_a(World* var1, const std::string& var2) {
		func_6256_a(var1, var2, NULL);
}

void Minecraft::func_6256_a(World* var1, const std::string& var2, EntityPlayer* var3) {
		m_loadingScreen->func_596_a(var2);
		m_loadingScreen->displayLoadingString("");
		m_sndManager->func_331_a("", 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
		if(theWorld != NULL) {
			theWorld->func_651_a(m_loadingScreen);
		}

		theWorld = var1;
		DbgPrintF("%s\n", "Player is set");
		if(var1 != NULL) {
			m_field_6327_b->func_717_a(var1);
			if(!isMultiplayerWorld()) {
				if(var3 == NULL) {
					// PORT-BLOCKED: thePlayer = (EntityPlayerSP*)var1->func_4085_a(EntityPlayerSP.class);
				}
			} else if(thePlayer != NULL) {
				thePlayer->preparePlayerToSpawn();
				if(var1 != NULL) {
					var1->entityJoinedWorld(thePlayer);
				}
			}

			if(!var1->m_multiplayerWorld) {
				func_6255_d(var2);
			}

			DbgPrintF("%s\n", "Player is now set");
			if(thePlayer == NULL) {
				thePlayer = (EntityPlayerSP*)m_field_6327_b->func_4087_b(var1);
				thePlayer->preparePlayerToSpawn();
				m_field_6327_b->flipPlayer(thePlayer);
			}

			thePlayer->m_field_787_a = new MovementInputFromOptions(m_gameSettings);
			if(m_field_6323_f != NULL) {
				m_field_6323_f->func_946_a(var1);
			}

			if(m_field_6321_h != NULL) {
				m_field_6321_h->func_1188_a(var1);
			}

			m_field_6327_b->func_6473_b(thePlayer);
			if(var3 != NULL) {
				var1->func_6464_c();
			}

			var1->func_608_a(thePlayer);
			if(var1->m_field_1033_r) {
				var1->func_651_a(m_loadingScreen);
			}
		} else {
			thePlayer = NULL;
		}

		(void)0;
		m_field_6287_N = 0L;
}

void Minecraft::func_6255_d(const std::string& var1) {
		m_loadingScreen->func_596_a(var1);
		m_loadingScreen->displayLoadingString("Building terrain");
		short var2 = 128;
		int var3 = 0;
		int var4 = var2 * 2 / 16 + 1;
		var4 *= var4;

		for(int var5 = -var2; var5 <= var2; var5 += 16) {
			int var6 = theWorld->m_spawnX;
			int var7 = theWorld->m_spawnZ;
			if(thePlayer != NULL) {
				var6 = (int)thePlayer->posX;
				var7 = (int)thePlayer->posZ;
			}

			for(int var8 = -var2; var8 <= var2; var8 += 16) {
				m_loadingScreen->setLoadingProgress(var3++ * 100 / var4);
				theWorld->getBlockId(var6 + var5, 64, var7 + var8);

				while(theWorld->func_6465_g()) {
				}
			}
		}

		m_loadingScreen->displayLoadingString("Simulating world for a bit");
		bool var9 = true;
		(void)var9;
		theWorld->func_656_j();
}

void Minecraft::installResource(const std::string& var1in, std::string /* PORT: was java.io.File */* var2) {
		std::string var1 = var1in;
		int var3 = javaIndexOf(var1, "/");
		std::string var4 = var1.substr(0, (var3) - (0));
		var1 = var1.substr(var3 + 1);
		if(javaToLowerCase(var4) == "sound") {
			m_sndManager->func_6372_a(var1, var2);
		} else if(javaToLowerCase(var4) == "newsound") {
			m_sndManager->func_6372_a(var1, var2);
		} else if(javaToLowerCase(var4) == "streaming") {
			m_sndManager->addStreaming(var1, var2);
		} else if(javaToLowerCase(var4) == "music") {
			m_sndManager->addMusic(var1, var2);
		} else if(javaToLowerCase(var4) == "newmusic") {
			m_sndManager->addMusic(var1, var2);
		}

}

OpenGlCapsChecker* Minecraft::func_6251_l() {
		return m_glCapabilities;
}

const std::string& Minecraft::func_6241_m() {
		return m_field_6323_f->func_953_b();
}

const std::string& Minecraft::func_6262_n() {
		return m_field_6323_f->func_957_c();
}

const std::string& Minecraft::func_6245_o() {
	// PORT: header mandates a by-reference return; back it with a file-scope buffer
	// (single-threaded UI consumes it immediately).
		static std::string s_buf;
		s_buf = "P: " + m_field_6321_h->func_1190_b() + ". T: " + theWorld->func_687_d();
		return s_buf;
}

void Minecraft::respawn() {
		if(!theWorld->m_worldProvider->func_6477_d()) {
			func_6237_k();
		}

		theWorld->func_4076_b();
		theWorld->func_9424_o();
		int var1 = 0;
		if(thePlayer != NULL) {
			var1 = thePlayer->m_field_620_ab;
			theWorld->setEntityDead(thePlayer);
		}

		thePlayer = (EntityPlayerSP*)m_field_6327_b->func_4087_b(theWorld);
		thePlayer->preparePlayerToSpawn();
		m_field_6327_b->flipPlayer(thePlayer);
		theWorld->func_608_a(thePlayer);
		thePlayer->m_field_787_a = new MovementInputFromOptions(m_gameSettings);
		thePlayer->m_field_620_ab = var1;
		m_field_6327_b->func_6473_b(thePlayer);
		func_6255_d("Respawning");
		if(dynamic_cast<GuiGameOver*>(currentScreen) != NULL) {
			displayGuiScreen(NULL);
		}

}

void Minecraft::func_6269_a(const std::string& var0, const std::string& var1) {
		func_6253_a(var0, var1, "");
}

void Minecraft::func_6253_a(const std::string& var0, const std::string& var1, const std::string& var2) {
	// PORT: the Java AWT applet launcher (Frame/Canvas/Dimension/MinecraftImpl/Thread/
	// GameWindowListener) is not used on the 360 — the game is launched directly from
	// Main.cpp which constructs Minecraft and calls run(). Kept as a no-op so the
	// obfuscated entry points still link.
		(void)var0;
		(void)var1;
		(void)var2;
}

void Minecraft::main(const std::string& var0) {
	// PORT: Java `public static void main(String[])` — the 360 entry point is Main.cpp.
		(void)var0;
		std::string var1 = "Player524";
		std::string var2 = "-";
		func_6269_a(var1, var2);
}

// PORT: was pure-virtual, implemented by the Java applet/Frame subclass to pop a crash
// dialog. On the 360 there's no such subclass (Main.cpp does `new Minecraft()` directly),
// so provide a concrete fallback: log and return (the title is already wedged if hit).
void Minecraft::func_4007_a(UnexpectedThrowable* var1) {
	DbgPrintF("[FATAL] Minecraft::func_4007_a: unexpected throwable during run()\n");
	(void)var1;
}
