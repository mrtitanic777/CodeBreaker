#include "stdafx.h"
// Auto-generated from Minecraft.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Minecraft.h"
#include "Common.h"
#include "EffectRenderer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "LoadingScreenRenderer.h"
#include "MinecraftApplet.h"
#include "MouseHelper.h"
#include "OpenGlCapsChecker.h"
#include "PlayerController.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "Session.h"
#include "SoundManager.h"
#include "TextureLavaFX.h"
#include "TextureWaterFX.h"
#include "ThreadDownloadResources.h"
#include "Timer.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "LevelGenerator.h"
#include "ModelBiped.h"
#include "MovementInputFromOptions.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Textures.h"

Minecraft::Minecraft()
	: m_playerController(new PlayerControllerSP(this))
	, m_fullscreen(false)
	, m_displayWidth(0)
	, m_displayHeight(0)
	, m_glCapabilities(NULL)
	, m_timer(new Timer(20.0f))
	, theWorld(NULL)
	, m_renderGlobal(NULL)
	, thePlayer(NULL)
	, m_effectRenderer(NULL)
	, m_session(NULL)
	, m_mcCanvas(NULL)
	, m_appletMode(true)
	, m_renderEngine(NULL)
	, fontRenderer(NULL)
	, currentScreen(NULL)
	, m_loadingScreen(new LoadingScreenRenderer(this))
	, m_entityRenderer(new EntityRenderer(this))
	, m_downloadResourcesThread(NULL)
	, m_ticksRan(0)
	, m_leftClickCounter(0)
	, m_tempDisplayWidth(0)
	, m_tempDisplayHeight(0)
	, m_loadMapUser(NULL)
	, m_loadMapID(0)
	, m_t(NULL)
	, m_skipRenderWorld(false)
	, m_options(NULL)
	, m_mcApplet(NULL)
	, m_sndManager(NULL)
	, m_mouseHelper(NULL)
	, m_mcDataDir(NULL)
	, m_textureWaterFX(NULL)
	, m_textureLavaFX(NULL)
	, m_inventoryScreen(false)
	, m_prevFrameTime(0)
	, m_D(false)
{
}

Minecraft::Minecraft(void* /* PORT: was AWT Canvas */ var1, MinecraftApplet* var2, int var3, int var4, bool var5)
	: m_playerController(new PlayerControllerSP(this))
	, m_fullscreen(false)
	, m_displayWidth(0)
	, m_displayHeight(0)
	, m_glCapabilities(NULL)
	, m_timer(new Timer(20.0f))
	, theWorld(NULL)
	, m_renderGlobal(NULL)
	, thePlayer(NULL)
	, m_effectRenderer(NULL)
	, m_session(NULL)
	, m_mcCanvas(NULL)
	, m_appletMode(true)
	, m_renderEngine(NULL)
	, fontRenderer(NULL)
	, currentScreen(NULL)
	, m_loadingScreen(new LoadingScreenRenderer(this))
	, m_entityRenderer(new EntityRenderer(this))
	, m_downloadResourcesThread(NULL)
	, m_ticksRan(0)
	, m_leftClickCounter(0)
	, m_tempDisplayWidth(0)
	, m_tempDisplayHeight(0)
	, m_loadMapUser(NULL)
	, m_loadMapID(0)
	, m_t(NULL)
	, m_skipRenderWorld(false)
	, m_options(NULL)
	, m_mcApplet(NULL)
	, m_sndManager(NULL)
	, m_mouseHelper(NULL)
	, m_mcDataDir(NULL)
	, m_textureWaterFX(NULL)
	, m_textureLavaFX(NULL)
	, m_inventoryScreen(false)
	, m_prevFrameTime(0)
	, m_D(false)
{
		new ModelBiped(0.0F);
		m_objectMouseOver = NULL;
		m_sndManager = new SoundManager();
		m_server = NULL;
		m_textureWaterFX = new TextureWaterFX();
		m_textureLavaFX = new TextureLavaFX();
		running = false;
		m_debug = "";
		m_inventoryScreen = false;
		m_prevFrameTime = 0;
		m_D = false;
		m_tempDisplayWidth = var3;
		m_tempDisplayHeight = var4;
		m_fullscreen = var5;
		m_mcApplet = var2;
		new ThreadSleepForever(this, "Timer hack thread");
		m_mcCanvas = var1;
		m_displayWidth = var3;
		m_displayHeight = var4;
		m_fullscreen = var5;
}

Minecraft::~Minecraft() {
	m_playerController = NULL;
	m_glCapabilities = NULL;
	m_timer = NULL;
	theWorld = NULL;
	m_renderGlobal = NULL;
	thePlayer = NULL;
	m_effectRenderer = NULL;
	m_session = NULL;
	m_mcCanvas = NULL;
	m_renderEngine = NULL;
	fontRenderer = NULL;
	currentScreen = NULL;
	m_loadingScreen = NULL;
	m_entityRenderer = NULL;
	m_downloadResourcesThread = NULL;
	m_t = NULL;
	m_options = NULL;
	m_mcApplet = NULL;
	m_sndManager = NULL;
	m_mouseHelper = NULL;
	m_mcDataDir = NULL;
	m_textureWaterFX = NULL;
	m_textureLavaFX = NULL;
}

void Minecraft::setServer(const std::string& var1, int var2) {
		m_server = var1;
}

void Minecraft::displayGuiScreen(GuiScreen* var1) {
		if(!(dynamic_cast<GuiErrorScreen*>(currentScreen) != NULL)) {
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
				inputLock();
				ScaledResolution var2 = ScaledResolution(m_displayWidth, m_displayHeight);
				int var3 = var2.getScaledWidth();
				int var4 = var2.getScaledHeight();
				((GuiScreen*)var1).setWorldAndResolution(this, var3, var4);
				m_skipRenderWorld = false;
			} else {
				setIngameFocus();
			}
		}
}

void Minecraft::shutdownMinecraftApplet() {
  { // was try
			if(m_downloadResourcesThread != NULL) {
				m_downloadResourcesThread->closeMinecraft();
			}
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var5)
		}

  { // was try
			m_sndManager->closeMinecraft();
   // STUBBED: Mouse.destroy();  // no cursor on 360
   // STUBBED: Keyboard.destroy();  // XInput replaces Keyboard
  } { // was finally � cleanup code follows
   // PORT: D3D9 Release  // was: Display.destroy();
		}

}

void Minecraft::run() {
		running = true;

  { // was try
			Minecraft var1 = this;
			if(m_mcCanvas != NULL) {
				Display.setParent(m_mcCanvas);
			} else if(m_fullscreen) {
    // PORT: always fullscreen on 360  // was: Display.setFullscreen(true);
				m_displayWidth = Display.getDisplayMode().getWidth();
				m_displayHeight = Display.getDisplayMode().getHeight();
			} else {
    // PORT: D3D9 present params  // was: Display.setDisplayMode(new DisplayMode(this.displayWidth, this.displayHeight));
			}

   // PORT: no window title on 360  // was: Display.setTitle("Minecraft Minecraft Indev");

			IntBuffer var24;
   { // was try
    // PORT: D3D9 CreateDevice  // was: Display.create();
				DbgPrintF("%s\n", "LWJGL version: " + Sys::getVersion());
    "Xbox 360" /* PORT: was glGetString */
    "Xbox 360" /* PORT: was glGetString */
    "Xbox 360" /* PORT: was glGetString */
    // PORT: D3D9 caps check  // was: ContextCapabilities var2 = GLContext::getCapabilities();
				DbgPrintF("%s\n", "OpenGL 3.0: " + var2.OpenGL30);
				DbgPrintF("%s\n", "OpenGL 3.1: " + var2.OpenGL31);
				DbgPrintF("%s\n", "OpenGL 3.2: " + var2.OpenGL32);
    // PORT: GL constant -> D3D9  // was: System.out.println("ARB_compatibility: " + var2.GL_ARB_compatibility);
				if(var2.OpenGL32) {
     // NIO asIntBuffer() � no-op for C array
     // PORT: use D3D9 GetTransform/GetViewport instead
					int var25 = var24[0];
					DbgPrintF("%s\n", "PROFILE MASK: " + Integer.toBinaryString(var25));
					DbgPrintF("%s\n", "CORE PROFILE: " + ((var25 & 1) != 0));
					DbgPrintF("%s\n", "COMPATIBILITY PROFILE: " + ((var25 & 2) != 0));
				}
			} { DbgPrintF("CATCH LWJGLException\n"); // was catch(LWJGLException var17)
    // Removed Java stream/exception call: var17.printStackTrace();

    { // was try
					Sleep(1000);
				} { DbgPrintF("CATCH InterruptedException\n"); // was catch(InterruptedException var16)
				}

    // PORT: D3D9 CreateDevice  // was: Display.create();
			}

   // STUBBED: Keyboard.create();  // XInput replaces Keyboard
   // STUBBED: Mouse.create();  // no cursor on 360
			m_mouseHelper = new MouseHelper(m_mcCanvas);

   { // was try
				Controllers::create();
			} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var15)
    // Removed Java stream/exception call: var15.printStackTrace();
			}

   g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
   // Shade model is shader-based on Xbox 360
   // PORT: D3D9 Clear()  // was: GL11.glClearDepth(1.0D);
   g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(255.0f * 0.1F));
   // PORT: GL constant -> D3D9  // was: GL11.glCullFace(GL11.GL_BACK);
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
			m_glCapabilities = new OpenGlCapsChecker();
			std::string var3 = "minecraft";
			std::string var26 = System.getProperty("user.home", ".");
			int[] var10001 = EnumOSMappingHelper.osValues;
			std::string var4 = System.getProperty("os.m_name").toLowerCase();
			File var27;
			switch(var10001[(var4/* PORT: contains check */ .count("win") ? EnumOS.windows : (var4/* PORT: contains check */ .count("mac") ? EnumOS.macos : (var4/* PORT: contains check */ .count("solaris") ? EnumOS.solaris : (var4/* PORT: contains check */ .count("sunos") ? EnumOS.solaris : (var4/* PORT: contains check */ .count("linux") ? EnumOS.linux : (var4/* PORT: contains check */ .count("unix") ? EnumOS.linux : EnumOS.unknown)))))).ordinal()]) {
			case 1:
			case 2:
				var27 = new File(var26, '.' + var3 + '/');
				break;
			case 3:
				var4 = System.getenv("APPDATA");
				if(var4 != NULL) {
					var27 = new File(var4, "." + var3 + '/');
				} else {
					var27 = new File(var26, '.' + var3 + '/');
				}
				break;
			case 4:
				var27 = new File(var26, "Library/Application Support/" + var3);
				break;
			default:
				var27 = new File(var26, var3 + '/');
			}

			if(!var27->exists() && !var27->mkdirs()) {
				DbgPrintF("ERROR [RuntimeException]: %s\n", "The working directory could not be created: " + var27);
			}

			m_mcDataDir = var27;
			m_options = new GameSettings(this, m_mcDataDir);
			m_sndManager->loadSoundSettings(m_options);
			m_renderEngine = new RenderEngine(m_options);
			m_// Animated texture FX registered at init  // was: registerTextureFX(m_textureLavaFX);
			m_// Animated texture FX registered at init  // was: registerTextureFX(m_textureWaterFX);
			m_// Animated texture FX registered at init  // was: registerTextureFX(new TextureWaterFlowFX());
			m_// Animated texture FX registered at init  // was: registerTextureFX(new TextureFlamesFX(0));
			m_// Animated texture FX registered at init  // was: registerTextureFX(new TextureFlamesFX(1));
			m_// Animated texture FX registered at init  // was: registerTextureFX(new TextureGearsFX(0));
			m_// Animated texture FX registered at init  // was: registerTextureFX(new TextureGearsFX(1));
			fontRenderer = new FontRenderer(m_options, "/default.png", m_renderEngine);
			var24 = new int[256];
   memset(var24, 0, sizeof(var24));
			m_renderGlobal = new RenderGlobal(this, m_renderEngine);
   { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
     g_pd3dDevice->SetViewport(&_vp); }
			if(m_server != NULL && m_session != NULL) {
				World* var31 = new World();
				var31->generate(8, 8, 8, new unsigned char[512]);
				setLevel(var31);
			} else if(theWorld == NULL) {
				displayGuiScreen(new GuiMainMenu());
			}

			m_effectRenderer = new EffectRenderer(theWorld, m_renderEngine);

   { // was try
				var1->m_downloadResourcesThread = new ThreadDownloadResources(var1->m_mcDataDir, var1);
				var1->m_downloadResourcesThread.start();
			} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var14)
			}

			m_t = new GuiIngame(this);
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var22)
   // Removed Java stream/exception call: var22.printStackTrace();
   // PORT: Java Swing/AWT: JOptionPane.showMessageDialog((Component)NULL, var22 /* PORT: toString */, "Failed to start Minecraft", 0);
			return;
		}

		long var23 = GetTickCount();
		int var28 = 0;

  { // was try
			while(running) {
				if(theWorld != NULL) {
					theWorld->updateLighting();
				}

    false /* PORT: XInput quit */  // was: if(this.m_mcCanvas == null && Display.isCloseRequested()) {
					running = false;
				}

    { // was try
					if(isGamePaused) {
						float var29 = m_timer->m_renderPartialTicks;
						m_timer->updateTimer();
						m_timer->m_renderPartialTicks = var29;
					} else {
						m_timer->updateTimer();
					}

					for(int var30 = 0; var30 < m_timer->m_elapsedTicks; ++var30) {
						++m_ticksRan;
						runTick();
					}

					m_sndManager->setListener(thePlayer, m_timer->m_renderPartialTicks);
     g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
					m_playerController->setPartialTime(m_timer->m_renderPartialTicks);
					m_entityRenderer->updateCameraAndRender(m_timer->m_renderPartialTicks);
     true /* PORT: always active on 360 */  // was: if(!Display.isActive()) {
						if(m_fullscreen) {
							toggleFullscreen();
						}

						Sleep(10);
					}

					if(m_mcCanvas != NULL && !m_fullscreen && (m_mcCanvas->getWidth() != m_displayWidth || m_mcCanvas->getHeight() != m_displayHeight)) {
						m_displayWidth = m_mcCanvas->getWidth();
						m_displayHeight = m_mcCanvas->getHeight();
						resize(m_displayWidth, m_displayHeight);
					}

					if(m_options->m_limitFramerate) {
						Sleep(5);
					}

					++var28;
					isGamePaused = currentScreen != NULL && currentScreen->doesGuiPauseGame();
				} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var18)
					displayGuiScreen(new GuiErrorScreen("Client error", "The game broke! [" + var18 + "]"));
     // Removed Java stream/exception call: var18.printStackTrace();
					return;
				}

				while(GetTickCount() >= var23 + 1000L) {
					m_debug = var28 + " fps, " + WorldRenderer.chunksUpdated + " chunk updates";
					WorldRenderer.chunksUpdated = 0;
					var23 += 1000L;
					var28 = 0;
				}
			}

			return;
		} { DbgPrintF("CATCH MinecraftError\n"); // was catch(MinecraftError var19)
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var20)
   // Removed Java stream/exception call: var20.printStackTrace();
			return;
  } { // was finally � cleanup code follows
			shutdownMinecraftApplet();
		}

}

void Minecraft::setIngameFocus() {
  true /* PORT: always active on 360 */  // was: if(Display.isActive()) {
			if(!m_inventoryScreen) {
				m_inventoryScreen = true;
				m_mouseHelper->grabMouseCursor();
				displayGuiScreen((GuiScreen*)NULL);
				m_prevFrameTime = m_ticksRan + 10000;
			}
		}
}

void Minecraft::inputLock() {
		if(m_inventoryScreen) {
			if(thePlayer != NULL) {
				EntityPlayerSP var1 = thePlayer;
				var1->m_movementInput.resetKeyState();
			}

			m_inventoryScreen = false;

   { // was try
				Mouse.setNativeCursor((Cursor)NULL);
			} { DbgPrintF("CATCH LWJGLException\n"); // was catch(LWJGLException var2)
    // Removed Java stream/exception call: var2.printStackTrace();
			}
		}
}

void Minecraft::displayInGameMenu() {
		if(currentScreen == NULL) {
			displayGuiScreen(new GuiIngameMenu());
		}
}

void Minecraft::clickMouse(int var1) {
		if(var1 != 0 || m_leftClickCounter <= 0) {
			if(var1 == 0) {
				m_entityRenderer->m_itemRenderer.equippedItemRender();
			}

			ItemStack var2;
			int var3;
			World var5;
			if(var1 == 1) {
				var2 = thePlayer->m_inventory.getCurrentItem();
				if(var2 != NULL) {
					var3 = var2.stackSize;
					EntityPlayerSP var7 = thePlayer;
					var5 = theWorld;
					ItemStack var4 = var2.getItem().onItemRightClick(var2, var5, var7);
					if(var4 != var2 || var4 != NULL && var4.stackSize != var3) {
						thePlayer->m_inventory.m_mainInventory[thePlayer->m_inventory.m_currentItem] = var4;
						m_entityRenderer->m_itemRenderer.resetEquippedProgress();
						if(var4.stackSize == 0) {
							thePlayer->m_inventory.m_mainInventory[thePlayer->m_inventory.m_currentItem] = NULL;
						}
					}
				}
			}

			if(m_objectMouseOver == NULL) {
				if(var1 == 0 && !(dynamic_cast<PlayerControllerCreative*>(m_playerController) != NULL)) {
					m_leftClickCounter = 10;
				}

			} else {
				ItemStack var9;
				if(m_objectMouseOver.m_typeOfHit == 1) {
					if(var1 == 0) {
						Entity var14 = m_objectMouseOver.m_entityHit;
						EntityPlayerSP var12 = thePlayer;
						InventoryPlayer var11 = var12.m_inventory;
						var9 = var11.getStackInSlot(var11.m_currentItem);
						int var17 = var9 != NULL ? Item::itemsList[var9.itemID].getDamageVsEntity() : 1;
						if(var17 > 0) {
							var14.attackEntityFrom(var12, var17);
							var2 = var12.m_inventory.getCurrentItem();
							if(var2 != NULL && dynamic_cast<EntityLiving*>(var14) != NULL) {
								EntityLiving var8 = (EntityLiving*)var14;
								Item::itemsList[var2.itemID].hitEntity(var2);
								if(var2.stackSize <= 0) {
									var12.displayGUIInventory();
								}
							}
						}

						return;
					}
				} else if(m_objectMouseOver.m_typeOfHit == 0) {
					int var10 = m_objectMouseOver.m_blockX;
					var3 = m_objectMouseOver.m_blockY;
					int var13 = m_objectMouseOver.m_blockZ;
					int var15 = m_objectMouseOver.m_sideHit;
					Block var6 = Block::blocksList[theWorld->getBlockId(var10, var3, var13)];
					if(var1 == 0) {
						theWorld->extinguishFire(var10, var3, var13, m_objectMouseOver.m_sideHit);
						if(var6 != Block::s_bedrock) {
							m_playerController->clickBlock(var10, var3, var13);
							return;
						}
					} else {
						var9 = thePlayer->m_inventory.getCurrentItem();
						int var16 = theWorld->getBlockId(var10, var3, var13);
						if(var16 > 0 && Block::blocksList[var16].blockActivated(theWorld, var10, var3, var13, thePlayer)) {
							return;
						}

						if(var9 == NULL) {
							return;
						}

						var16 = var9.stackSize;
						int var18 = var15;
						var5 = theWorld;
						if(var9.getItem().onItemUse(var9, var5, var10, var3, var13, var18)) {
							m_entityRenderer->m_itemRenderer.equippedItemRender();
						}

						if(var9.stackSize == 0) {
							thePlayer->m_inventory.m_mainInventory[thePlayer->m_inventory.m_currentItem] = NULL;
							return;
						}

						if(var9.stackSize != var16) {
							m_entityRenderer->m_itemRenderer.equipAnimationSpeed();
						}
					}
				}

			}
		}
}

void Minecraft::toggleFullscreen() {
  { // was try
			m_fullscreen = !m_fullscreen;
			DbgPrintF("%s\n", "Toggle m_fullscreen!");
			if(m_fullscreen) {
    // PORT: D3D9 present params  // was: Display.setDisplayMode(Display.getDesktopDisplayMode());
				m_displayWidth = Display.getDisplayMode().getWidth();
				m_displayHeight = Display.getDisplayMode().getHeight();
			} else {
				if(m_mcCanvas != NULL) {
					m_displayWidth = m_mcCanvas->getWidth();
					m_displayHeight = m_mcCanvas->getHeight();
				} else {
					m_displayWidth = m_tempDisplayWidth;
					m_displayHeight = m_tempDisplayHeight;
				}

    // PORT: D3D9 present params  // was: Display.setDisplayMode(new DisplayMode(this.tempDisplayWidth, this.tempDisplayHeight));
			}

			inputLock();
   // PORT: always fullscreen on 360  // was: Display.setFullscreen(this.fullscreen);
   g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
			Sleep(1000);
			if(m_fullscreen) {
				setIngameFocus();
			}

			if(currentScreen != NULL) {
				inputLock();
				resize(m_displayWidth, m_displayHeight);
			}

			DbgPrintF("%s\n", "Size: " + m_displayWidth + ", " + m_displayHeight);
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var2)
   // Removed Java stream/exception call: var2.printStackTrace();
		}
}

void Minecraft::resize(int var1, int var2) {
		m_displayWidth = var1;
		m_displayHeight = var2;
		if(currentScreen != NULL) {
			ScaledResolution var3 = ScaledResolution(var1, var2);
			var2 = var3.getScaledWidth();
			var1 = var3.getScaledHeight();
			currentScreen->setWorldAndResolution(this, var2, var1);
		}

}

void Minecraft::runTick() {
		m_t->addChatMessage();
		if(!isGamePaused && theWorld != NULL) {
			m_playerController->onUpdate();
		}

  g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/terrain.png")));
		if(!isGamePaused) {
			m_// PORT: animated texture update);
		}

		if(currentScreen == NULL && thePlayer != NULL && thePlayer->health <= 0) {
			displayGuiScreen((GuiScreen*)NULL);
		}

		if(currentScreen == NULL || currentScreen->m_allowUserInput) {
			label286:
			while(true) {
				while(true) {
					int var1;
					int var2;
     // PORT: XInput poll loop  // was: while(Mouse.next()) {
      // PORT-BLOCKED: var1 = Mouse.getEventDWheel();
      // PORT-BLOCKED: if(var1 != 0) {
       // PORT-BLOCKED: var2 = var1;
       // PORT-BLOCKED: InventoryPlayer var5 = this.thePlayer->inventory;
       // PORT-BLOCKED: if(var1 > 0) {
        // PORT-BLOCKED: var2 = 1;
       // PORT-BLOCKED: }
// PORT-BLOCKED: 
       // PORT-BLOCKED: if(var2 < 0) {
        // PORT-BLOCKED: var2 = -1;
       // PORT-BLOCKED: }
// PORT-BLOCKED: 
       // PORT-BLOCKED: for(var5.currentItem -= var2; var5.currentItem < 0; var5.currentItem += 9) {
       // PORT-BLOCKED: }
// PORT-BLOCKED: 
       // PORT-BLOCKED: while(var5.currentItem >= 9) {
        // PORT-BLOCKED: var5.currentItem -= 9;
       // PORT-BLOCKED: }
      // PORT-BLOCKED: }
// PORT-BLOCKED: 
      // PORT-BLOCKED: if(this.currentScreen == null) {
       // PORT-BLOCKED: if(!this.inventoryScreen && Mouse.getEventButtonState()) {
        // PORT-BLOCKED: this.setIngameFocus();
       // PORT-BLOCKED: } else {
        // PORT-BLOCKED: if(Mouse.getEventButton() == 0 && Mouse.getEventButtonState()) {
         // PORT-BLOCKED: this.clickMouse(0);
         // PORT-BLOCKED: this.prevFrameTime = this.ticksRan;
        // PORT-BLOCKED: }
// PORT-BLOCKED: 
        // PORT-BLOCKED: if(Mouse.getEventButton() == 1 && Mouse.getEventButtonState()) {
         // PORT-BLOCKED: this.clickMouse(1);
         // PORT-BLOCKED: this.prevFrameTime = this.ticksRan;
        // PORT-BLOCKED: }
// PORT-BLOCKED: 
        // PORT-BLOCKED: if(Mouse.getEventButton() == 2 && Mouse.getEventButtonState() && this.objectMouseOver != null) {
         // PORT-BLOCKED: var2 = this.theWorld->getBlockId(this.objectMouseOver.blockX, this.objectMouseOver.blockY, this.objectMouseOver.blockZ);
         // PORT-BLOCKED: if(var2 == Block::s_grass->blockID) {
          // PORT-BLOCKED: var2 = Block::s_dirt->blockID;
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(var2 == Block::s_stairDouble->blockID) {
          // PORT-BLOCKED: var2 = Block::s_stairSingle->blockID;
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(var2 == Block::s_bedrock->blockID) {
          // PORT-BLOCKED: var2 = Block::s_stone->blockID;
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: this.thePlayer->inventory.getFirstEmptyStack(var2);
        // PORT-BLOCKED: }
       // PORT-BLOCKED: }
      // PORT-BLOCKED: } else if(this.currentScreen != null) {
       // PORT-BLOCKED: this.currentScreen->handleMouseInput();
      // PORT-BLOCKED: }
     // PORT-BLOCKED: }

					if(m_leftClickCounter > 0) {
						--m_leftClickCounter;
					}

					while(true) {
						while(true) {
							do {
								bool var3;
        // PORT: XInputGetKeystroke loop  // was: if(!Keyboard.next()) {
         // PORT-BLOCKED: if(this.currentScreen == null) {
          // PORT-BLOCKED: if(Mouse.isButtonDown(0) && (float)(this.ticksRan - this.prevFrameTime) >= this.timer.ticksPerSecond / 4.0F && this.inventoryScreen) {
           // PORT-BLOCKED: this.clickMouse(0);
           // PORT-BLOCKED: this.prevFrameTime = this.ticksRan;
          // PORT-BLOCKED: }
// PORT-BLOCKED: 
          // PORT-BLOCKED: if(Mouse.isButtonDown(1) && (float)(this.ticksRan - this.prevFrameTime) >= this.timer.ticksPerSecond / 4.0F && this.inventoryScreen) {
           // PORT-BLOCKED: this.clickMouse(1);
           // PORT-BLOCKED: this.prevFrameTime = this.ticksRan;
          // PORT-BLOCKED: }
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: var3 = this.currentScreen == null && Mouse.isButtonDown(0) && this.inventoryScreen;
         // PORT-BLOCKED: boolean var9 = false;
         // PORT-BLOCKED: if(!this.playerController.isInTestMode && this.leftClickCounter <= 0) {
          // PORT-BLOCKED: if(var3 && this.objectMouseOver != null && this.objectMouseOver.typeOfHit == 0) {
           // PORT-BLOCKED: var2 = this.objectMouseOver.blockX;
           // PORT-BLOCKED: int var8 = this.objectMouseOver.blockY;
           // PORT-BLOCKED: int var4 = this.objectMouseOver.blockZ;
           // PORT-BLOCKED: this.playerController.sendBlockRemoving(var2, var8, var4, this.objectMouseOver.sideHit);
           // PORT-BLOCKED: this.effectRenderer.addBlockHitEffects(var2, var8, var4, this.objectMouseOver.sideHit);
          // PORT-BLOCKED: } else {
           // PORT-BLOCKED: this.playerController.resetBlockRemoving();
          // PORT-BLOCKED: }
         // PORT-BLOCKED: }
         // PORT-BLOCKED: goto label286;
        // PORT-BLOCKED: }

								EntityPlayerSP var10000 = thePlayer;
        // PORT: XInputGetKeystroke  // was: int var10001 = Keyboard.getEventKey();
        int var10001 = 0; // PORT: stub for blocked declaration
        // PORT: XInputGetKeystroke  // was: var3 = Keyboard.getEventKeyState();
								var2 = var10001;
								EntityPlayerSP var6 = var10000;
								var6.m_movementInput.checkKeyForMovementInput(var2, var3);
       // PORT: XInputGetKeystroke  // was: } while(!Keyboard.getEventKeyState());

       // PORT: XInputGetKeystroke  // was: if(Keyboard.getEventKey() == Keyboard.KEY_F11) {
        // PORT-BLOCKED: this.toggleFullscreen();
       // PORT-BLOCKED: } else {
        // PORT-BLOCKED: if(this.currentScreen != null) {
         // PORT-BLOCKED: this.currentScreen->handleKeyboardInput();
        // PORT-BLOCKED: } else {
         // PORT-BLOCKED: if(Keyboard.getEventKey() == Keyboard.KEY_ESCAPE) {
          // PORT-BLOCKED: this.displayInGameMenu();
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(Keyboard.getEventKey() == Keyboard.KEY_F7) {
          // PORT-BLOCKED: this.entityRenderer.grabLargeScreenshot();
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(this.playerController instanceof PlayerControllerCreative) {
          // PORT-BLOCKED: if(Keyboard.getEventKey() == this.options.keyBindLoad.keyCode) {
           // PORT-BLOCKED: this.thePlayer->preparePlayerToSpawn();
          // PORT-BLOCKED: }
// PORT-BLOCKED: 
          // PORT-BLOCKED: if(Keyboard.getEventKey() == this.options.keyBindSave.keyCode) {
           // PORT-BLOCKED: this.theWorld->setSpawnLocation((int)this.thePlayer->posX, (int)this.thePlayer->posY, (int)this.thePlayer->posZ, this.thePlayer->rotationYaw);
           // PORT-BLOCKED: this.thePlayer->preparePlayerToSpawn();
          // PORT-BLOCKED: }
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(Keyboard.getEventKey() == Keyboard.KEY_F5) {
          // PORT-BLOCKED: this.D = !this.D;
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(Keyboard.getEventKey() == this.options.keyBindInventory.keyCode) {
          // PORT-BLOCKED: this.displayGuiScreen(new GuiInventory(this.thePlayer->inventory));
         // PORT-BLOCKED: }
// PORT-BLOCKED: 
         // PORT-BLOCKED: if(Keyboard.getEventKey() == this.options.keyBindDrop.keyCode) {
          // PORT-BLOCKED: this.thePlayer->dropPlayerItemWithRandomChoice(this.thePlayer->inventory.decrStackSize(this.thePlayer->inventory.currentItem, 1), false);
         // PORT-BLOCKED: }
        // PORT-BLOCKED: }
// PORT-BLOCKED: 
        // PORT-BLOCKED: for(var1 = 0; var1 < 9; ++var1) {
         // PORT-BLOCKED: if(Keyboard.getEventKey() == var1 + 2) {
          // PORT-BLOCKED: this.thePlayer->inventory.currentItem = var1;
         // PORT-BLOCKED: }
        // PORT-BLOCKED: }
// PORT-BLOCKED: 
        // PORT-BLOCKED: if(Keyboard.getEventKey() == this.options.keyBindToggleFog.keyCode) {
         // PORT-BLOCKED: this.options.setOptionValue(4, !Keyboard.isKeyDown(Keyboard.KEY_LSHIFT) && !Keyboard.isKeyDown(Keyboard.KEY_RSHIFT) ? 1 : -1);
        // PORT-BLOCKED: }
       // PORT-BLOCKED: }
						}
					}
				}
			}
		}

		if(currentScreen != NULL) {
			m_prevFrameTime = m_ticksRan + 10000;
		}

		if(currentScreen != NULL) {
			GuiScreen var7 = currentScreen;

   // PORT: XInput poll loop  // was: while(Mouse.next()) {
    // PORT-BLOCKED: var7.handleMouseInput();
   // PORT-BLOCKED: }

   // PORT: XInputGetKeystroke loop  // was: while(Keyboard.next()) {
    // PORT-BLOCKED: var7.handleKeyboardInput();
   // PORT-BLOCKED: }

			if(currentScreen != NULL) {
				currentScreen->updateScreen();
			}
		}

		if(theWorld != NULL) {
			theWorld->m_difficultySetting = m_options->m_difficulty;
			if(!isGamePaused) {
				m_entityRenderer->updateRenderer();
			}

			if(!isGamePaused) {
				m_renderGlobal->updateClouds();
			}

			if(!isGamePaused) {
				theWorld->updateEntities();
			}

			if(!isGamePaused) {
				theWorld->tick();
			}

			if(!isGamePaused) {
				theWorld->randomDisplayUpdates((int)thePlayer->posX, (int)thePlayer->posY, (int)thePlayer->posZ);
			}

			if(!isGamePaused) {
				m_effectRenderer->updateEffects();
			}
		}

}

void Minecraft::generateLevel(int var1, int var2, int var3, int var4) {
		setLevel((World*)NULL);
		System.gc();
		std::string var5 = m_session != NULL ? m_session->username : "anonymous";
		LevelGenerator* var6 = new LevelGenerator(m_loadingScreen);
		var6->m_islandGen = var3 == 1;
		var6->m_floatingGen = var3 == 2;
		var6->m_flatGen = var3 == 3;
		var6->m_levelType = var4;
		var1 = 128 << var1;
		var3 = var1;
		short var8 = 64;
		if(var2 == 1) {
			var1 /= 2;
			var3 <<= 1;
		} else if(var2 == 2) {
			var1 /= 2;
			var3 = var1;
			var8 = 256;
		}

		World* var7 = var6->generate(var5, var1, var3, var8);
		setLevel(var7);
}

void Minecraft::setLevel(World* var1) {
		if(theWorld != NULL) {
			theWorld->setLevel();
		}

  { // was try
			// PORT-TODO: replace Java I/O — BufferedReader var2
			// PORT-TODO: atoi(var2.readLine(.c_str()));
			// PORT-TODO: var2.close();
			if(m_mcApplet->getDocumentBase() /* PORT: toString */.startsWith("http://www.minecraft.net/") || m_mcApplet->getDocumentBase() /* PORT: toString */.startsWith("http://minecraft.net/")) {
				theWorld = var1;
			}
		} { DbgPrintF("CATCH Throwable\n"); // was catch(Throwable var3)
		}

		if(var1 != NULL) {
			var1->load();
			m_playerController->onWorldChange(var1);
			// PORT-BLOCKED: thePlayer = (EntityPlayerSP*)var1->findSubclassOf(EntityPlayerSP.class);
			var1->m_playerEntity = thePlayer;
			if(thePlayer == NULL) {
				thePlayer = new EntityPlayerSP(this, var1, m_session);
				thePlayer->preparePlayerToSpawn();
				if(var1 != NULL) {
					var1->spawnEntityInWorld(thePlayer);
					var1->m_playerEntity = thePlayer;
				}
			}

			if(thePlayer != NULL) {
				thePlayer->m_movementInput = new MovementInputFromOptions(m_options);
				m_playerController->onRespawn(thePlayer);
			}

			if(m_renderGlobal != NULL) {
				m_renderGlobal->changeWorld(var1);
			}

			if(m_effectRenderer != NULL) {
				m_effectRenderer->clearEffects(var1);
			}

			m_textureWaterFX->textureId = 0;
			m_textureLavaFX->textureId = 0;
			int var4 = m_Textures::load("game:\Content\water.png");
			if(var1->m_defaultFluid == Block::s_waterMoving->blockID) {
				m_textureWaterFX->textureId = var4;
			} else {
				m_textureLavaFX->textureId = var4;
			}
		}

		System.gc();
}
