#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Minecraft.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Common.h"           // DbgPrintF (boot markers)
#include "Minecraft.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BorderLayout.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "Canvas.h"
#include "Chunk.h"
#include "ChunkProviderClient.h"
#include "ChunkProviderLoadOrGenerate.h"   // s_deferPopulate (break the spawn-gen populate cascade)
#include "Class.h"
#include "Color.h"
#include "Component.h"
#include "Controllers.h"
#include "CraftingInventoryCB.h"
#include "CraftingInventoryPlayerCB.h"
#include "Cursor.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "DateFormat.h"
#include "Dimension.h"
#include "Display.h"
#include "DisplayMode.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "EnumOSMappingHelper.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "Frame.h"
#include "GL11.h"
#include "GLAllocation.h"
#include "GLU.h"
#include "GameSettings.h"
#include "GameWindowListener.h"
#include "Graphics.h"
#include "GuiButton.h"
#include "GuiChat.h"
#include "GuiConflictWarning.h"
#include "GuiConnecting.h"
#include "GuiGameOver.h"
#include "GuiIngame.h"
#include "GuiIngameMenu.h"
#include "GuiInventory.h"
#include "GuiMainMenu.h"
#include "GuiScreen.h"
#include "GuiUnused.h"
#include "HashEntry.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IInventory.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InterruptedException.h"
#include "InventoryCrafting.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "Keyboard.h"
#include "LWJGLException.h"
#include "LoadingScreenRenderer.h"
#include "LogoEffectRandomizer.h"
#include "MCHashTable.h"
#include "Material.h"
#include "MathHelper.h"
#include "MinecraftApplet.h"
#include "MinecraftError.h"
#include "MinecraftException.h"
#include "MinecraftImpl.h"
#include "MobSpawnerBase.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "Mouse.h"
#include "MouseHelper.h"
#include "Input.h"     // Input::poll() — the per-frame XInput read driving Keyboard/Mouse
#include "MovementInput.h"
#include "MovementInputFromOptions.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NetClientHandler.h"
#include "NetHandler.h"
#include "NetworkManager.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OpenGlCapsChecker.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "PlayerController.h"
#include "PlayerControllerTest.h"
#include "PositionTexureVertex.h"
#include "Random.h"
#include "Render.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Runnable.h"
#include "RuntimeException.h"
#include "ScaledResolution.h"
#include "ScreenShotHelper.h"
#include "Session.h"
#include "Slot.h"
#include "Socket.h"
#include "SoundManager.h"
#include "SoundPool.h"
#include "SoundPoolEntry.h"
#include "SoundSystem.h"
#include "StepSound.h"
#include "Teleporter.h"
#include "Tessellator.h"
#include "TextureCompassFX.h"
#include "TextureFlamesFX.h"
#include "TextureLavaFX.h"
#include "TextureLavaFlowFX.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "TexturePortalFX.h"
#include "TextureWatchFX.h"
#include "TextureWaterFX.h"
#include "TexturedQuad.h"
#include "TexureWaterFlowFX.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "ThreadSleepForever.h"
#include "Throwable.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "UnexpectedThrowable.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldClient.h"
#include "WorldProvider.h"
#include "WorldProviderHell.h"
#include "WorldRenderer.h"

JArray<long long > Minecraft::s_debugFrameTimes = JArray<long long >(512);
JArray<long long > Minecraft::s_debugTickTimes = JArray<long long >(512);
int Minecraft::s_debugTimingIndex = 0;
File* Minecraft::s_minecraftDir = NULL;

Minecraft::Minecraft(Component* var1, Canvas* var2, MinecraftApplet* var3, int var4, int var5, bool var6)
	: playerController(NULL)
	, a(0)
	, displayWidth(0)
	, displayHeight(0)
	, glCapabilities(NULL)
	, timer(NULL)
	, theWorld(NULL)
	, renderGlobal(NULL)
	, thePlayer(NULL)
	, effectRenderer(NULL)
	, session(NULL)
	, mcCanvas(NULL)
	, hideQuitButton(0)
	, isGamePaused(0)
	, renderEngine(NULL)
	, fontRenderer(NULL)
	, currentScreen(NULL)
	, loadingScreen(NULL)
	, entityRenderer(NULL)
	, downloadResourcesThread(NULL)
	, ticksRan(0)
	, leftClickCounter(0)
	, tempDisplayWidth(0)
	, tempDisplayHeight(0)
	, loadMapID(0)
	, ingameGUI(NULL)
	, skipRenderWorld(0)
	, modelBiped(NULL)
	, objectMouseOver(NULL)
	, gameSettings(NULL)
	, mcApplet(NULL)
	, sndManager(NULL)
	, mouseHelper(NULL)
	, texturePackList(NULL)
	, mcDataDir(NULL)
	, serverPort(0)
	, textureWaterFX(NULL)
	, textureLavaFX(NULL)
	, running(0)
	, isTakingScreenshot(0)
	, debugUpdateTime(0)
	, inGameHasFocus(0)
	, ticksSinceLastClick(0)
	, field_6288_M(0)
	, prevFrameTime(0)
	, joinPlayerCounter(0)
{
	this->sndManager = new SoundManager();
	this->running = true;
	this->debugUpdateTime = -1LL;
	this->prevFrameTime = System::currentTimeMillis();

	this->timer = new Timer(20.0F);
	this->hideQuitButton = true;
	this->loadingScreen = new LoadingScreenRenderer(this);
	this->entityRenderer = new EntityRenderer(this);
	this->modelBiped = new ModelBiped(0.0F);
	this->textureWaterFX = new TextureWaterFX();
	this->textureLavaFX = new TextureLavaFX();

	this->tempDisplayWidth = var4;
	this->tempDisplayHeight = var5;
	this->a = var6;
	this->mcApplet = var3;
	new ThreadSleepForever(this, "Timer hack thread");
	this->mcCanvas = var2;
	this->displayWidth = var4;
	this->displayHeight = var5;
	this->a = var6;
}

Minecraft::~Minecraft()
{
}

void Minecraft::setServer(const JavaString& var1, int var2)
{
	this->serverName = var1;
	this->serverPort = var2;
}

void Minecraft::startGame()
{
	DbgPrintF("[boot] startGame begin\n");
	if(this->mcCanvas != NULL) {
	Graphics* var1 = this->mcCanvas->getGraphics();
	if(var1 != NULL) {
	var1->setColor(Color::BLACK);
	var1->fillRect(0, 0, this->displayWidth, this->displayHeight);
	var1->dispose();
	}
	Display::setParent(this->mcCanvas);
	} else if(this->a) {
	Display::setFullscreen(true);
	this->displayWidth = Display::getDisplayMode().getWidth();
	this->displayHeight = Display::getDisplayMode().getHeight();
	if(this->displayWidth <= 0) {
	this->displayWidth = 1;
	}
	if(this->displayHeight <= 0) {
	this->displayHeight = 1;
	}
	} else {
	Display::setDisplayMode(new DisplayMode(this->displayWidth, this->displayHeight));
	}
	Display::setTitle("Minecraft Minecraft Alpha v1.2.6");
	try {
	Display::create();
	} catch (LWJGLException* var6) {
	var6->printStackTrace();
	try {
	Thread::sleep(1000LL);
	} catch (InterruptedException*) {
	}
	Display::create();
	}
	RenderManager::s_instance->itemRenderer = new ItemRenderer(this);
	this->mcDataDir = getMinecraftDir();
	this->gameSettings = new GameSettings(this, this->mcDataDir);
	this->texturePackList = new TexturePackList(this, this->mcDataDir);
	DbgPrintF("[boot] texturePackList ok\n");
	this->renderEngine = new RenderEngine(this->texturePackList, this->gameSettings);
	DbgPrintF("[boot] renderEngine ok\n");
	this->fontRenderer = new FontRenderer(this->gameSettings, "/font/default.png", this->renderEngine);
	DbgPrintF("[boot] fontRenderer ok\n");
	this->loadScreen();
	DbgPrintF("[boot] loadScreen ok\n");
	Keyboard::create();
	Mouse::create();
	this->mouseHelper = new MouseHelper(this->mcCanvas);
	try {
	Controllers::create();
	} catch (Exception* var4) {
	var4->printStackTrace();
	}
	this->checkGLError("Pre startup");
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	GL11::glShadeModel(GL11::GL_SMOOTH);
	GL11::glClearDepth(1.0);
	GL11::glEnable(GL11::GL_DEPTH_TEST);
	GL11::glDepthFunc(GL11::GL_LEQUAL);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glAlphaFunc(GL11::GL_GREATER, 0.1F);
	GL11::glCullFace(GL11::GL_BACK);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	this->checkGLError("Startup");
	this->glCapabilities = new OpenGlCapsChecker();
	DbgPrintF("[boot] glCaps ok\n");
	this->sndManager->loadSoundSettings(this->gameSettings);
	this->renderEngine->registerTextureFX(this->textureLavaFX);
	this->renderEngine->registerTextureFX(this->textureWaterFX);
	this->renderEngine->registerTextureFX(new TexturePortalFX());
	this->renderEngine->registerTextureFX(new TextureCompassFX(this));
	this->renderEngine->registerTextureFX(new TextureWatchFX(this));
	this->renderEngine->registerTextureFX(new TexureWaterFlowFX());
	this->renderEngine->registerTextureFX(new TextureLavaFlowFX());
	this->renderEngine->registerTextureFX(new TextureFlamesFX(0));
	this->renderEngine->registerTextureFX(new TextureFlamesFX(1));
	this->renderGlobal = new RenderGlobal(this, this->renderEngine);
	DbgPrintF("[boot] renderGlobal ok\n");
	GL11::glViewport(0, 0, this->displayWidth, this->displayHeight);
	this->effectRenderer = new EffectRenderer(this->theWorld, this->renderEngine);
	try {
	this->downloadResourcesThread = new ThreadDownloadResources(this->mcDataDir, this);
	this->downloadResourcesThread->start();
	} catch (Exception*) {
	}
	this->checkGLError("Post startup");
	this->ingameGUI = new GuiIngame(this);
	DbgPrintF("[boot] ingameGUI ok\n");
	if(this->serverName != NULL) {
	this->displayGuiScreen(new GuiConnecting(this, this->serverName, this->serverPort));
	} else {
	this->displayGuiScreen(new GuiMainMenu());
	DbgPrintF("[boot] main menu shown\n");
	}
}

void Minecraft::loadScreen()
{
	ScaledResolution* var1 = new ScaledResolution(this->displayWidth, this->displayHeight);
	int var2 = var1->getScaledWidth();
	int var3 = var1->getScaledHeight();
	DbgPrintF("[boot] ls: scaledres\n");
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT | GL11::GL_COLOR_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glOrtho(0.0, (double)var2, (double)var3, 0.0, 1000.0, 3000.0);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	GL11::glTranslatef(0.0F, 0.0F, -2000.0F);
	GL11::glViewport(0, 0, this->displayWidth, this->displayHeight);
	GL11::glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	DbgPrintF("[boot] ls: glstate done\n");
	Tessellator* var4 = Tessellator::s_instance;
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	GL11::glDisable(GL11::GL_FOG);
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->renderEngine->getTexture("/title/mojang.png"));
	DbgPrintF("[boot] ls: mojang tex bound\n");
	var4->startDrawingQuads();
	var4->setColorOpaque_I(16777215);
	// Draw in SCALED-resolution coords (var2 x var3) to match the scaled ortho above. Using the raw
	// displayWidth/displayHeight here put a full-pixel-size quad into a scaled ortho -> the mojang
	// screen was zoomed ~scaleFactor x and clipped.
	var4->addVertexWithUV(0.0, (double)var3, 0.0, 0.0, 0.0);
	var4->addVertexWithUV((double)var2, (double)var3, 0.0, 0.0, 0.0);
	var4->addVertexWithUV((double)var2, 0.0, 0.0, 0.0, 0.0);
	var4->addVertexWithUV(0.0, 0.0, 0.0, 0.0, 0.0);
	var4->draw();
	short var5 = 256;
	short var6 = 256;
	GL11::glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	var4->setColorOpaque_I(16777215);
	this->drawSplashTexture((var2 - var5) / 2, (var3 - var6) / 2, 0, 0, var5, var6);
	GL11::glDisable(GL11::GL_LIGHTING);
	GL11::glDisable(GL11::GL_FOG);
	GL11::glEnable(GL11::GL_ALPHA_TEST);
	GL11::glAlphaFunc(GL11::GL_GREATER, 0.1F);
	Display::swapBuffers();
	DbgPrintF("[boot] ls: post-swap\n");
}

void Minecraft::drawSplashTexture(int var1, int var2, int var3, int var4, int var5, int var6)
{
	float var7 = 0.00390625F;
	float var8 = 0.00390625F;
	Tessellator* var9 = Tessellator::s_instance;
	var9->startDrawingQuads();
	var9->addVertexWithUV((double)(var1 + 0), (double)(var2 + var6), 0.0, (double)((float)(var3 + 0) * var7), (double)((float)(var4 + var6) * var8));
	var9->addVertexWithUV((double)(var1 + var5), (double)(var2 + var6), 0.0, (double)((float)(var3 + var5) * var7), (double)((float)(var4 + var6) * var8));
	var9->addVertexWithUV((double)(var1 + var5), (double)(var2 + 0), 0.0, (double)((float)(var3 + var5) * var7), (double)((float)(var4 + 0) * var8));
	var9->addVertexWithUV((double)(var1 + 0), (double)(var2 + 0), 0.0, (double)((float)(var3 + 0) * var7), (double)((float)(var4 + 0) * var8));
	var9->draw();
}

File* Minecraft::getMinecraftDir()
{
	if(s_minecraftDir == NULL) {
	s_minecraftDir = getAppDir("minecraft");
	}
	return s_minecraftDir;
}

File* Minecraft::getAppDir(const JavaString& var0)
{
	// PORT (Xbox 360): the JVM path logic (user.home / getOs) yields "." here, which is not a valid
	// device -> File::mkdirs() fails -> the original `throw new RuntimeException(...)` fires. On this
	// target a guest C++ throw whose message is a JCat passed through the variadic exception ctor
	// reads a garbage std::string size and storms `new[huge]` (bad_alloc) -- the black-screen crash.
	// So pin the writable data dir to a real device next to the title (the same device the Content
	// assets load from) and NEVER throw: create it best-effort and return it regardless. (Durable
	// world saves additionally route through the STFS layer; a read-only device here just means
	// options.txt / screenshots don't persist.)
	std::string var1 = (JCat() + "game:\\" + var0);
	File* var2 = new File(var1);
	if(!var2->exists()) {
	var2->mkdirs();
	}
	return var2;
}

int Minecraft::getOs()
{
	JavaString var0 = System::getProperty("os.name").toLowerCase();
	return var0.contains("win") ? EnumOS2::c : (var0.contains("mac") ? EnumOS2::d : (var0.contains("solaris") ? EnumOS2::b : (var0.contains("sunos") ? EnumOS2::b : (var0.contains("linux") ? EnumOS2::a : (var0.contains("unix") ? EnumOS2::a : EnumOS2::unknown)))));
}

void Minecraft::displayGuiScreen(GuiScreen* var1)
{
	if(!((dynamic_cast<GuiUnused*>(this->currentScreen) != NULL))) {
	if(this->currentScreen != NULL) {
	this->currentScreen->onGuiClosed();
	}
	if(var1 == NULL && this->theWorld == NULL) {
	var1 = new GuiMainMenu();
	} else if(var1 == NULL && this->thePlayer->health <= 0) {
	var1 = new GuiGameOver();
	}
	this->currentScreen = (GuiScreen*)var1;
	if(var1 != NULL) {
	this->setIngameNotInFocus();
	ScaledResolution* var2 = new ScaledResolution(this->displayWidth, this->displayHeight);
	int var3 = var2->getScaledWidth();
	int var4 = var2->getScaledHeight();
	((GuiScreen*)var1)->setWorldAndResolution(this, var3, var4);
	this->skipRenderWorld = false;
	} else {
	this->setIngameFocus();
	}
	}
}

void Minecraft::checkGLError(const JavaString& var1)
{
	int var2 = GL11::glGetError();
	if(var2 != 0) {
	JavaString var3 = GLU::gluErrorString(var2);
	System::out.println("########## GL ERROR ##########");
	System::out.println((JCat() + "@ " + var1));
	System::out.println((JCat() + var2 + ": " + var3));
	System::exit(0);
	}
}

void Minecraft::shutdownMinecraftApplet()
{
	if(this->mcApplet != NULL) {
	this->mcApplet->clearReferences();
	}
	try {
	if(this->downloadResourcesThread != NULL) {
	this->downloadResourcesThread->closeMinecraft();
	}
	} catch (Exception*) {
	}
	// PORT: try/finally -> exceptions are off on the console, so run the try body then the
	// finally body unconditionally (no early exits in this body).
	{
	System::out.println("Stopping!");
	this->setWorld((World*)NULL);
	try {
	GLAllocation::deleteTexturesAndDisplayLists();
	} catch (Exception*) {
	}
	this->sndManager->closeMinecraft();
	Mouse::destroy();
	Keyboard::destroy();
	}
	{   // finally
	Display::destroy();
	}
	System::gc();
}

void Minecraft::run()
{
	this->running = true;
	try {
	this->startGame();
	} catch (Exception* var15) {
	var15->printStackTrace();
	this->displayUnexpectedThrowable(new UnexpectedThrowable("Failed to start game", var15));
	return;
	}
	{   // PORT: outer try had an empty finally -> plain block (exceptions are off)
	try {
	long long var1 = System::currentTimeMillis();
	int var3 = 0;
	while(this->running && (this->mcApplet == NULL || this->mcApplet->isActive())) {
	Input::setDisplaySize(this->displayWidth, this->displayHeight);   // resize() isn't reached on console (no AWT canvas) -> wire it here so the GUI cursor maps to real pixels + centers
	Input::poll();   // ONE XInput read per frame -> drives the virtual Keyboard/Mouse model (menu nav + look)
	AxisAlignedBB::clearBoundingBoxPool();
	Vec3D::initialize();
	if(this->mcCanvas == NULL && Display::isCloseRequested()) {
	this->shutdown();
	}
	if(this->isGamePaused && this->theWorld != NULL) {
	float var4 = this->timer->renderPartialTicks;
	this->timer->updateTimer();
	this->timer->renderPartialTicks = var4;
	} else {
	this->timer->updateTimer();
	}
	long long var19 = System::nanoTime();
	for(int var6 = 0; var6 < this->timer->elapsedTicks; ++var6) {
	++this->ticksRan;
	try {
	this->runTick();
	} catch (MinecraftException*) {
	this->theWorld = NULL;
	this->setWorld((World*)NULL);
	this->displayGuiScreen(new GuiConflictWarning());
	}
	}
	long long var20 = System::nanoTime() - var19;
	this->checkGLError("Pre render");
	this->sndManager->setListener(this->thePlayer, this->timer->renderPartialTicks);
	GL11::glEnable(GL11::GL_TEXTURE_2D);
	if(this->theWorld != NULL && !this->theWorld->multiplayerWorld) {
	// BOUNDED light drain: this was while(updatingLighting()){} = unbounded, draining the ENTIRE
	// queued-lighting backlog in ONE frame. The finite 864x864 world (and any chunk gen / block edit)
	// floods that queue, so the frame froze until it all resolved = the periodic hard-lock. Cap it so
	// the frame completes and lighting catches up over the next few frames.
	for(int _ld = 0; _ld < 64 && this->theWorld->updatingLighting(); ++_ld) {
	}
	}
	if(this->theWorld != NULL && this->theWorld->multiplayerWorld) {
	this->theWorld->updatingLighting();
	}
	if(this->gameSettings->limitFramerate) {
	Thread::sleep(5LL);
	}
	if(!Keyboard::isKeyDown(Keyboard::KEY_F7)) {
	Display::update();
	}
	if(!this->skipRenderWorld) {
	if(this->playerController != NULL) {
	this->playerController->updateBlockHitOverlay(this->timer->renderPartialTicks);
	}
	this->entityRenderer->updateCameraAndRender(this->timer->renderPartialTicks);
	}
	if(!Display::isActive()) {
	if(this->a) {
	this->toggleFullscreen();
	}
	Thread::sleep(10LL);
	}
	if(Keyboard::isKeyDown(Keyboard::KEY_F3)) {
	this->displayDebugInfo(var20);
	} else {
	this->debugUpdateTime = System::nanoTime();
	}
	Thread::yield();
	if(Keyboard::isKeyDown(Keyboard::KEY_F7)) {
	Display::update();
	}
	this->screenshotListener();
	if(this->mcCanvas != NULL && !this->a && (this->mcCanvas->getWidth() != this->displayWidth || this->mcCanvas->getHeight() != this->displayHeight)) {
	this->displayWidth = this->mcCanvas->getWidth();
	this->displayHeight = this->mcCanvas->getHeight();
	if(this->displayWidth <= 0) {
	this->displayWidth = 1;
	}
	if(this->displayHeight <= 0) {
	this->displayHeight = 1;
	}
	this->resize(this->displayWidth, this->displayHeight);
	}
	this->checkGLError("Post render");
	++var3;
	for(this->isGamePaused = !this->isMultiplayerWorld() && this->currentScreen != NULL && this->currentScreen->doesGuiPauseGame(); System::currentTimeMillis() >= var1 + 1000LL; var3 = 0) {
	this->debugFPS = (JCat() + var3 + " fps, " + WorldRenderer::s_chunksUpdated + " chunk updates");
	WorldRenderer::s_chunksUpdated = 0;
	var1 += 1000LL;
	}
	}
	} catch (MinecraftError*) {
	} catch (Throwable* var17) {
	this->theWorld = NULL;
	var17->printStackTrace();
	this->displayUnexpectedThrowable(new UnexpectedThrowable("Unexpected error", var17));
	}
	}
}

void Minecraft::screenshotListener()
{
	if(Keyboard::isKeyDown(Keyboard::KEY_F2)) {
	if(!this->isTakingScreenshot) {
	if(Keyboard::isKeyDown(Keyboard::KEY_F1)) {
	this->ingameGUI->addChatMessage(ScreenShotHelper::saveScreenshot(s_minecraftDir, this->displayWidth, this->displayHeight));
	}
	this->isTakingScreenshot = true;
	}
	} else {
	this->isTakingScreenshot = false;
	}
}

void Minecraft::displayDebugInfo(long long var1)
{
	long long var3 = 16666666LL;
	if(this->debugUpdateTime == -1LL) {
	this->debugUpdateTime = System::nanoTime();
	}
	long long var5 = System::nanoTime();
	s_debugTickTimes[s_debugTimingIndex & s_debugFrameTimes.length - 1] = var1;
	s_debugFrameTimes[s_debugTimingIndex++ & s_debugFrameTimes.length - 1] = var5 - this->debugUpdateTime;
	this->debugUpdateTime = var5;
	GL11::glClear(GL11::GL_DEPTH_BUFFER_BIT);
	GL11::glMatrixMode(GL11::GL_PROJECTION);
	GL11::glLoadIdentity();
	GL11::glOrtho(0.0, (double)this->displayWidth, (double)this->displayHeight, 0.0, 1000.0, 3000.0);
	GL11::glMatrixMode(GL11::GL_MODELVIEW);
	GL11::glLoadIdentity();
	GL11::glTranslatef(0.0F, 0.0F, -2000.0F);
	GL11::glLineWidth(1.0F);
	GL11::glDisable(GL11::GL_TEXTURE_2D);
	Tessellator* var7 = Tessellator::s_instance;
	var7->startDrawing(7);
	int var8 = (int)(var3 / 200000LL);
	var7->setColorOpaque_I(536870912);
	var7->addVertex(0.0, (double)(this->displayHeight - var8), 0.0);
	var7->addVertex(0.0, (double)this->displayHeight, 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)this->displayHeight, 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)(this->displayHeight - var8), 0.0);
	var7->setColorOpaque_I(538968064);
	var7->addVertex(0.0, (double)(this->displayHeight - var8 * 2), 0.0);
	var7->addVertex(0.0, (double)(this->displayHeight - var8), 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)(this->displayHeight - var8), 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)(this->displayHeight - var8 * 2), 0.0);
	var7->draw();
	long long var9 = 0LL;
	int var11;
	for(var11 = 0; var11 < s_debugFrameTimes.length; ++var11) {
	var9 += s_debugFrameTimes[var11];
	}
	var11 = (int)(var9 / 200000LL / (long long)s_debugFrameTimes.length);
	var7->startDrawing(7);
	var7->setColorOpaque_I(541065216);
	var7->addVertex(0.0, (double)(this->displayHeight - var11), 0.0);
	var7->addVertex(0.0, (double)this->displayHeight, 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)this->displayHeight, 0.0);
	var7->addVertex((double)s_debugFrameTimes.length, (double)(this->displayHeight - var11), 0.0);
	var7->draw();
	var7->startDrawing(1);
	for(int var12 = 0; var12 < s_debugFrameTimes.length; ++var12) {
	int var13 = ((var12 - s_debugTimingIndex) & (s_debugFrameTimes.length - 1)) * 255 / s_debugFrameTimes.length;
	int var14 = var13 * var13 / 255;
	var14 = var14 * var14 / 255;
	int var15 = var14 * var14 / 255;
	var15 = var15 * var15 / 255;
	if(s_debugFrameTimes[var12] > var3) {
	var7->setColorOpaque_I(-16777216 + var14 * 65536);
	} else {
	var7->setColorOpaque_I(-16777216 + var14 * 256);
	}
	long long var16 = s_debugFrameTimes[var12] / 200000LL;
	long long var18 = s_debugTickTimes[var12] / 200000LL;
	var7->addVertex((double)((float)var12 + 0.5F), (double)((float)((long long)this->displayHeight - var16) + 0.5F), 0.0);
	var7->addVertex((double)((float)var12 + 0.5F), (double)((float)this->displayHeight + 0.5F), 0.0);
	var7->setColorOpaque_I(-16777216 + var14 * 65536 + var14 * 256 + var14 * 1);
	var7->addVertex((double)((float)var12 + 0.5F), (double)((float)((long long)this->displayHeight - var16) + 0.5F), 0.0);
	var7->addVertex((double)((float)var12 + 0.5F), (double)((float)((long long)this->displayHeight - (var16 - var18)) + 0.5F), 0.0);
	}
	var7->draw();
	GL11::glEnable(GL11::GL_TEXTURE_2D);
}

void Minecraft::shutdown()
{
	this->running = false;
}

void Minecraft::setIngameFocus()
{
	if(Display::isActive()) {
	if(!this->inGameHasFocus) {
	this->inGameHasFocus = true;
	this->mouseHelper->grabMouseCursor();
	this->displayGuiScreen((GuiScreen*)NULL);
	this->ticksSinceLastClick = this->ticksRan + 10000;
	}
	}
}

void Minecraft::setIngameNotInFocus()
{
	if(this->inGameHasFocus) {
	if(this->thePlayer != NULL) {
	this->thePlayer->resetMovementInput();
	}
	this->inGameHasFocus = false;
	this->mouseHelper->ungrabMouseCursor();
	}
}

void Minecraft::displayInGameMenu()
{
	if(this->currentScreen == NULL) {
	this->displayGuiScreen(new GuiIngameMenu());
	}
}

void Minecraft::clickBlockHeld(int var1, bool var2)
{
	if(!this->playerController->isCreativeMode) {
	if(var1 != 0 || this->leftClickCounter <= 0) {
	if(var2 && this->objectMouseOver != NULL && this->objectMouseOver->typeOfHit == 0 && var1 == 0) {
	int var3 = this->objectMouseOver->blockX;
	int var4 = this->objectMouseOver->blockY;
	int var5 = this->objectMouseOver->blockZ;
	this->playerController->sendBlockRemoving(var3, var4, var5, this->objectMouseOver->sideHit);
	this->effectRenderer->addBlockHitEffects(var3, var4, var5, this->objectMouseOver->sideHit);
	} else {
	this->playerController->resetBlockRemoving();
	}
	}
	}
}

void Minecraft::clickMouse(int var1)
{
	if(var1 != 0 || this->leftClickCounter <= 0) {
	if(var1 == 0) {
	this->thePlayer->swingItem();
	}
	bool var2 = true;
	if(this->objectMouseOver == NULL) {
	if(var1 == 0 && !((dynamic_cast<PlayerControllerTest*>(this->playerController) != NULL))) {
	this->leftClickCounter = 10;
	}
	} else if(this->objectMouseOver->typeOfHit == 1) {
	if(var1 == 0) {
	this->playerController->attackEntity(this->thePlayer, this->objectMouseOver->entityHit);
	}
	if(var1 == 1) {
	this->playerController->interactWithEntity(this->thePlayer, this->objectMouseOver->entityHit);
	}
	} else if(this->objectMouseOver->typeOfHit == 0) {
	int var3 = this->objectMouseOver->blockX;
	int var4 = this->objectMouseOver->blockY;
	int var5 = this->objectMouseOver->blockZ;
	int var6 = this->objectMouseOver->sideHit;
	Block* var7 = Block::s_blocksList[this->theWorld->getBlockId(var3, var4, var5)];
	if(var1 == 0) {
	this->theWorld->onBlockHit(var3, var4, var5, this->objectMouseOver->sideHit);
	if(var7 != Block::s_bedrock || this->thePlayer->blockBreakingPower >= 100) {
	this->playerController->clickBlock(var3, var4, var5, this->objectMouseOver->sideHit);
	}
	} else {
	ItemStack* var8 = this->thePlayer->inventory->getCurrentItem();
	int var9 = var8 != NULL ? var8->stackSize : 0;
	if(this->playerController->sendPlaceBlock(this->thePlayer, this->theWorld, var8, var3, var4, var5, var6)) {
	var2 = false;
	this->thePlayer->swingItem();
	}
	if(var8 == NULL) {
	return;
	}
	if(var8->stackSize == 0) {
	this->thePlayer->inventory->mainInventory[this->thePlayer->inventory->currentItem] = NULL;
	} else if(var8->stackSize != var9) {
	this->entityRenderer->itemRenderer->resetEquippedProgress();
	}
	}
	}
	if(var2 && var1 == 1) {
	ItemStack* var10 = this->thePlayer->inventory->getCurrentItem();
	if(var10 != NULL && this->playerController->sendUseItem(this->thePlayer, this->theWorld, var10)) {
	this->entityRenderer->itemRenderer->resetEquippedProgress2();
	}
	}
	}
}

void Minecraft::toggleFullscreen()
{
	try {
	this->a = !this->a;
	System::out.println("Toggle fullscreen!");
	if(this->a) {
	Display::setDisplayMode(Display::getDesktopDisplayMode());
	this->displayWidth = Display::getDisplayMode().getWidth();
	this->displayHeight = Display::getDisplayMode().getHeight();
	if(this->displayWidth <= 0) {
	this->displayWidth = 1;
	}
	if(this->displayHeight <= 0) {
	this->displayHeight = 1;
	}
	} else {
	if(this->mcCanvas != NULL) {
	this->displayWidth = this->mcCanvas->getWidth();
	this->displayHeight = this->mcCanvas->getHeight();
	} else {
	this->displayWidth = this->tempDisplayWidth;
	this->displayHeight = this->tempDisplayHeight;
	}
	if(this->displayWidth <= 0) {
	this->displayWidth = 1;
	}
	if(this->displayHeight <= 0) {
	this->displayHeight = 1;
	}
	Display::setDisplayMode(new DisplayMode(this->tempDisplayWidth, this->tempDisplayHeight));
	}
	this->setIngameNotInFocus();
	Display::setFullscreen(this->a);
	Display::update();
	Thread::sleep(1000LL);
	if(this->a) {
	this->setIngameFocus();
	}
	if(this->currentScreen != NULL) {
	this->setIngameNotInFocus();
	this->resize(this->displayWidth, this->displayHeight);
	}
	System::out.println((JCat() + "Size: " + this->displayWidth + ", " + this->displayHeight));
	} catch (Exception* var2) {
	var2->printStackTrace();
	}
}

void Minecraft::resize(int var1, int var2)
{
	if(var1 <= 0) {
	var1 = 1;
	}
	if(var2 <= 0) {
	var2 = 1;
	}
	this->displayWidth = var1;
	this->displayHeight = var2;
	Input::setDisplaySize(var1, var2);   // keep the controller's virtual GUI cursor mapped to real pixels
	if(this->currentScreen != NULL) {
	ScaledResolution* var3 = new ScaledResolution(var1, var2);
	int var4 = var3->getScaledWidth();
	int var5 = var3->getScaledHeight();
	this->currentScreen->setWorldAndResolution(this, var4, var5);
	}
}

void Minecraft::clickMiddleMouseButton()
{
	if(this->objectMouseOver != NULL) {
	int var1 = this->theWorld->getBlockId(this->objectMouseOver->blockX, this->objectMouseOver->blockY, this->objectMouseOver->blockZ);
	if(var1 == Block::s_grass->blockID) {
	var1 = Block::s_dirt->blockID;
	}
	if(var1 == Block::s_stairDouble->blockID) {
	var1 = Block::s_stairSingle->blockID;
	}
	if(var1 == Block::s_bedrock->blockID) {
	var1 = Block::s_stone->blockID;
	}
	this->thePlayer->inventory->setCurrentItem(var1, (dynamic_cast<PlayerControllerTest*>(this->playerController) != NULL));
	}
}

void Minecraft::runTick()
{
	static int s_tk = 0; ++s_tk; DbgPrintF("[T%d] begin\n", s_tk);   // CONSOLE: per-tick markers for the "few seconds then crash" diagnostic
	this->ingameGUI->updateTick();
	this->entityRenderer->getMouseOver(1.0F);
	if(this->thePlayer != NULL) {
	this->thePlayer->onUpdateClientPlayer();
	}
	if(!this->isGamePaused && this->theWorld != NULL) {
	this->playerController->resetBlockRemovingSound();
	}
	GL11::glBindTexture(GL11::GL_TEXTURE_2D, this->renderEngine->getTexture("/terrain.png"));
	if(!this->isGamePaused) {
	this->renderEngine->updateDynamicTextures();
	}
	{ static bool _dtm = false; if(!_dtm){ _dtm = true; DbgPrintF("[tick] updateDynamicTextures ok\n"); } }
	if(this->currentScreen == NULL && this->thePlayer != NULL && this->thePlayer->health <= 0) {
	this->displayGuiScreen((GuiScreen*)NULL);
	}
	if(this->currentScreen != NULL) {
	this->ticksSinceLastClick = this->ticksRan + 10000;
	}
	if(this->currentScreen != NULL) {
	this->currentScreen->handleInput();
	if(this->currentScreen != NULL) {
	this->currentScreen->updateScreen();
	}
	}
	if(this->currentScreen == NULL || this->currentScreen->allowUserInput) {
	while(true) {
	while(true) {
	while(true) {
	long long var1;
	do {
	if(!Mouse::next()) {
	if(this->leftClickCounter > 0) {
	--this->leftClickCounter;
	}
	while(true) {
	while(true) {
	do {
	if(!Keyboard::next()) {
	if(this->currentScreen == NULL) {
	if(Mouse::isButtonDown(0) && (float)(this->ticksRan - this->ticksSinceLastClick) >= this->timer->ticksPerSecond / 4.0F && this->inGameHasFocus) {
	this->clickMouse(0);
	this->ticksSinceLastClick = this->ticksRan;
	}
	if(Mouse::isButtonDown(1) && (float)(this->ticksRan - this->ticksSinceLastClick) >= this->timer->ticksPerSecond / 4.0F && this->inGameHasFocus) {
	this->clickMouse(1);
	this->ticksSinceLastClick = this->ticksRan;
	}
	}
	this->clickBlockHeld(0, this->currentScreen == NULL && Mouse::isButtonDown(0) && this->inGameHasFocus);
	goto label238_brk;
	}
	this->thePlayer->setKeyState(Keyboard::getEventKey(), Keyboard::getEventKeyState());
	} while(!Keyboard::getEventKeyState());
	if(Keyboard::getEventKey() == Keyboard::KEY_F11) {
	this->toggleFullscreen();
	} else {
	if(this->currentScreen != NULL) {
	this->currentScreen->handleKeyboardInput();
	} else {
	if(Keyboard::getEventKey() == Keyboard::KEY_ESCAPE) {
	this->displayInGameMenu();
	}
	if(Keyboard::getEventKey() == Keyboard::KEY_S && Keyboard::isKeyDown(Keyboard::KEY_F3)) {
	this->forceReload();
	}
	if(Keyboard::getEventKey() == Keyboard::KEY_F5) {
	this->gameSettings->thirdPersonView = !this->gameSettings->thirdPersonView;
	}
	if(Keyboard::getEventKey() == this->gameSettings->keyBindInventory->keyCode) {
	this->displayGuiScreen(new GuiInventory(this->thePlayer->inventory, this->thePlayer->inventory->craftingInventory));
	}
	if(Keyboard::getEventKey() == this->gameSettings->keyBindDrop->keyCode) {
	this->thePlayer->dropPlayerItemWithRandomChoice(this->thePlayer->inventory->decrStackSize(this->thePlayer->inventory->currentItem, 1), false);
	}
	if(this->isMultiplayerWorld() && Keyboard::getEventKey() == this->gameSettings->keyBindChat->keyCode) {
	this->displayGuiScreen(new GuiChat());
	}
	}
	for(int var4 = 0; var4 < 9; ++var4) {
	if(Keyboard::getEventKey() == Keyboard::KEY_1 + var4) {
	this->thePlayer->inventory->currentItem = var4;
	}
	}
	if(Keyboard::getEventKey() == this->gameSettings->keyBindToggleFog->keyCode) {
	this->gameSettings->setOptionValue(4, !Keyboard::isKeyDown(Keyboard::KEY_LSHIFT) && !Keyboard::isKeyDown(Keyboard::KEY_RSHIFT) ? 1 : -1);
	}
	}
	}
	}
	}
	var1 = System::currentTimeMillis() - this->prevFrameTime;
	} while(var1 > 200LL);
	int var3 = Mouse::getEventDWheel();
	if(var3 != 0) {
	this->thePlayer->inventory->changeCurrentItem(var3);
	}
	if(this->currentScreen == NULL) {
	if(!this->inGameHasFocus && Mouse::getEventButtonState()) {
	this->setIngameFocus();
	} else {
	if(Mouse::getEventButton() == 0 && Mouse::getEventButtonState()) {
	this->clickMouse(0);
	this->ticksSinceLastClick = this->ticksRan;
	}
	if(Mouse::getEventButton() == 1 && Mouse::getEventButtonState()) {
	this->clickMouse(1);
	this->ticksSinceLastClick = this->ticksRan;
	}
	if(Mouse::getEventButton() == 2 && Mouse::getEventButtonState()) {
	this->clickMiddleMouseButton();
	}
	}
	} else if(this->currentScreen != NULL) {
	this->currentScreen->handleMouseInput();
	}
	}
	}
	}
	label238_brk:;
	}
	if(this->theWorld != NULL) {
	if(this->thePlayer != NULL) {
	++this->joinPlayerCounter;
	if(this->joinPlayerCounter == 30) {
	this->joinPlayerCounter = 0;
	this->theWorld->joinEntityInSurroundings(this->thePlayer);
	}
	}
	this->theWorld->difficultySetting = this->gameSettings->difficulty;
	if(this->theWorld->multiplayerWorld) {
	this->theWorld->difficultySetting = 3;
	}
	if(!this->isGamePaused) {
	this->entityRenderer->updateRenderer();
	}
	if(!this->isGamePaused) {
	this->renderGlobal->updateClouds();
	}
	if(!this->isGamePaused) {
	DbgPrintF("[T%d] updateEntities\n", s_tk);
	this->theWorld->updateEntities();
	}
	if(!this->isGamePaused || this->isMultiplayerWorld()) {
	DbgPrintF("[T%d] worldTick\n", s_tk);
	this->theWorld->tick();
	DbgPrintF("[T%d] worldTick ok\n", s_tk);
	}
	if(!this->isGamePaused && this->theWorld != NULL) {
	this->theWorld->randomDisplayUpdates(MathHelper::floor_double(this->thePlayer->posX), MathHelper::floor_double(this->thePlayer->posY), MathHelper::floor_double(this->thePlayer->posZ));
	}
	if(!this->isGamePaused) {
	this->effectRenderer->updateEffects();
	}
	}
	this->prevFrameTime = System::currentTimeMillis();
}

void Minecraft::forceReload()
{
	System::out.println("FORCING RELOAD!");
	this->sndManager = new SoundManager();
	this->sndManager->loadSoundSettings(this->gameSettings);
	this->downloadResourcesThread->reloadResources();
}

bool Minecraft::isMultiplayerWorld()
{
	return this->theWorld != NULL && this->theWorld->multiplayerWorld;
}

void Minecraft::startWorld(const JavaString& var1)
{
	this->setWorld((World*)NULL);
	System::gc();
	World* var2 = new World(new File(getMinecraftDir(), "saves"), var1);
	if(var2->isNewWorld) {
	this->setWorldWithMessage(var2, "Generating level");
	} else {
	this->setWorldWithMessage(var2, "Loading level");
	}
}

void Minecraft::switchDimension()
{
	if(this->thePlayer->dimension == -1) {
	this->thePlayer->dimension = 0;
	} else {
	this->thePlayer->dimension = -1;
	}
	this->theWorld->setEntityDead(this->thePlayer);
	this->thePlayer->isDead = false;
	double var1 = this->thePlayer->posX;
	double var3 = this->thePlayer->posZ;
	double var5 = 8.0;
	World* var7;
	if(this->thePlayer->dimension == -1) {
	var1 /= var5;
	var3 /= var5;
	this->thePlayer->setLocationAndAngles(var1, this->thePlayer->posY, var3, this->thePlayer->rotationYaw, this->thePlayer->rotationPitch);
	this->theWorld->updateEntityWithOptionalForce(this->thePlayer, false);
	var7 = new World(this->theWorld, new WorldProviderHell());
	this->changeWorld(var7, "Entering the Nether", this->thePlayer);
	} else {
	var1 *= var5;
	var3 *= var5;
	this->thePlayer->setLocationAndAngles(var1, this->thePlayer->posY, var3, this->thePlayer->rotationYaw, this->thePlayer->rotationPitch);
	this->theWorld->updateEntityWithOptionalForce(this->thePlayer, false);
	var7 = new World(this->theWorld, new WorldProvider());
	this->changeWorld(var7, "Leaving the Nether", this->thePlayer);
	}
	this->thePlayer->worldObj = this->theWorld;
	this->thePlayer->setLocationAndAngles(var1, this->thePlayer->posY, var3, this->thePlayer->rotationYaw, this->thePlayer->rotationPitch);
	this->theWorld->updateEntityWithOptionalForce(this->thePlayer, false);
	(new Teleporter())->placeInPortal(this->theWorld, this->thePlayer);
}

void Minecraft::setWorld(World* var1)
{
	this->setWorldWithMessage(var1, "");
}

void Minecraft::setWorldWithMessage(World* var1, const JavaString& var2)
{
	this->changeWorld(var1, var2, (EntityPlayer*)NULL);
}

void Minecraft::changeWorld(World* var1, const JavaString& var2, EntityPlayer* var3)
{
	this->loadingScreen->func_596_a(var2);
	this->loadingScreen->displayLoadingString("");
	this->sndManager->playStreaming((JavaString)NULL, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
	if(this->theWorld != NULL) {
	this->theWorld->saveWorldIndicator(this->loadingScreen);
	}
	this->theWorld = var1;
	System::out.println((JCat() + "Player is " + this->thePlayer));
	if(var1 != NULL) {
	this->playerController->addPendingConnection(var1);
	if(!this->isMultiplayerWorld()) {
	if(var3 == NULL) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.thePlayer = (EntityPlayerSP)var1.findEntityByClass(EntityPlayerSP.class);
	 *  // The EntityPlayerSP.class type token is reduced to an empty Class(), so the
	 *  // world is not actually filtered by the EntityPlayerSP type when locating the
	 *  // existing player entity (class-dispatch argument is a placeholder).
	 *  ---- end original Java ---- */
	this->thePlayer = (EntityPlayerSP*)var1->findEntityByClass(Class());
	}
	} else if(this->thePlayer != NULL) {
	this->thePlayer->preparePlayerToSpawn();
	if(var1 != NULL) {
	var1->entityJoinedWorld(this->thePlayer);
	}
	}
	if(!var1->multiplayerWorld) {
	this->loadWorldSpawnArea(var2);
	}
	DbgPrintF("[world] changeWorld: spawnArea returned\n");
	System::out.println((JCat() + "Player is now " + this->thePlayer));
	if(this->thePlayer == NULL) {
	this->thePlayer = (EntityPlayerSP*)this->playerController->createPlayer(var1);
	this->thePlayer->preparePlayerToSpawn();
	this->playerController->flipPlayer(this->thePlayer);
	}
	DbgPrintF("[world] changeWorld: player ready (%p)\n", (void*)this->thePlayer);
	if(this->thePlayer != NULL && var1 != NULL) {
	int _px = (int)this->thePlayer->posX, _py = (int)this->thePlayer->posY, _pz = (int)this->thePlayer->posZ;
	DbgPrintF("[spawn] pos=(%d,%d,%d) spawn=(%d,%d,%d) surfaceY=%d feetBlk=%d headBlk=%d light(head)=%d skySub=%d\n",
		_px, _py, _pz, var1->spawnX, var1->spawnY, var1->spawnZ,
		var1->getHeightValue(_px, _pz), var1->getBlockId(_px, _py, _pz), var1->getBlockId(_px, _py + 1, _pz),
		var1->getBlockLightValue(_px, _py + 1, _pz), var1->skylightSubtracted);
	}
	this->thePlayer->movementInput = new MovementInputFromOptions(this->gameSettings);
	if(this->renderGlobal != NULL) {
	this->renderGlobal->setWorldAndLoadRenderers(var1);
	}
	DbgPrintF("[world] changeWorld: renderers set\n");
	if(this->effectRenderer != NULL) {
	this->effectRenderer->clearEffects(var1);
	}
	this->playerController->onPlayerCreated(this->thePlayer);
	if(var3 != NULL) {
	var1->func_6464_c();
	}
	var1->loadPlayerData(this->thePlayer);
	if(var1->isNewWorld) {
	var1->saveWorldIndicator(this->loadingScreen);
	}
	} else {
	this->thePlayer = NULL;
	}
	System::gc();
	this->prevFrameTime = 0LL;
	DbgPrintF("[world] changeWorld: DONE (entering world)\n");
}

void Minecraft::loadWorldSpawnArea(const JavaString& var1)
{
	this->loadingScreen->func_596_a(var1);
	this->loadingScreen->displayLoadingString("Building terrain");
	short var2 = 128;
	int var3 = 0;
	int var4 = var2 * 2 / 16 + 1;
	var4 *= var4;
	DbgPrintF("[world] spawnArea BEGIN (generate %d columns)\n", var4);
	ChunkProviderLoadOrGenerate::s_deferPopulate = true;   // break the populate cascade during pre-gen
	for(int var5 = -var2; var5 <= var2; var5 += 16) {
	int var6 = this->theWorld->spawnX;
	int var7 = this->theWorld->spawnZ;
	if(this->thePlayer != NULL) {
	var6 = (int)this->thePlayer->posX;
	var7 = (int)this->thePlayer->posZ;
	}
	for(int var8 = -var2; var8 <= var2; var8 += 16) {
	if(var3 % 40 == 0) DbgPrintF("[world] gen %d/%d at chunk(%d,%d)\n", var3, var4, (var6+var5)>>4, (var7+var8)>>4);
	this->loadingScreen->setLoadingProgress(var3++ * 100 / var4);
	this->theWorld->getBlockId(var6 + var5, 64, var7 + var8);
	for(int _ld = 0; _ld < 64 && this->theWorld->updatingLighting(); ++_ld) {   // BOUNDED (was unbounded per-column -> quadratic pre-gen stall); rest drains in-game
	}
	}
	}
	ChunkProviderLoadOrGenerate::s_deferPopulate = false;   // resume normal lazy population during play
	DbgPrintF("[world] spawnArea: terrain built (getBlockId loop done)\n");
	this->loadingScreen->displayLoadingString("Simulating world for a bit");
	bool var9 = true;
	this->theWorld->loadAllChunks();
	DbgPrintF("[world] spawnArea: loadAllChunks done\n");
}

void Minecraft::installResource(const JavaString& var1_, File* var2)
{
	JavaString var1 = var1_;   // Java reassigns this param below; take a mutable copy
	int var3 = var1.indexOf("/");
	JavaString var4 = var1.substr(0, (var3) - (0));
	var1 = var1.substr(var3 + 1);
	if(var4.equalsIgnoreCase("sound")) {
	this->sndManager->addSound(var1, var2);
	} else if(var4.equalsIgnoreCase("newsound")) {
	this->sndManager->addSound(var1, var2);
	} else if(var4.equalsIgnoreCase("streaming")) {
	this->sndManager->addStreaming(var1, var2);
	} else if(var4.equalsIgnoreCase("music")) {
	this->sndManager->addMusic(var1, var2);
	} else if(var4.equalsIgnoreCase("newmusic")) {
	this->sndManager->addMusic(var1, var2);
	}
}

OpenGlCapsChecker* Minecraft::getGlCapabilities()
{
	return this->glCapabilities;
}

JavaString Minecraft::getDebugInfoRenders()
{
	return this->renderGlobal->getDebugInfoRenders();
}

JavaString Minecraft::getDebugInfoEntities()
{
	return this->renderGlobal->getDebugInfoEntities();
}

JavaString Minecraft::getDebugInfoParticles()
{
	return (JCat() + "P: " + this->effectRenderer->getStatistics() + ". T: " + this->theWorld->getDebugLoadedEntities());
}

void Minecraft::respawn()
{
	if(!this->theWorld->worldProvider->isSurfaceWorld()) {
	this->switchDimension();
	}
	this->theWorld->findSpawnGround();
	this->theWorld->releaseUnloadedEntities();
	int var1 = 0;
	if(this->thePlayer != NULL) {
	var1 = this->thePlayer->entityId;
	this->theWorld->setEntityDead(this->thePlayer);
	}
	this->thePlayer = (EntityPlayerSP*)this->playerController->createPlayer(this->theWorld);
	this->thePlayer->preparePlayerToSpawn();
	this->playerController->flipPlayer(this->thePlayer);
	this->theWorld->loadPlayerData(this->thePlayer);
	this->thePlayer->movementInput = new MovementInputFromOptions(this->gameSettings);
	this->thePlayer->entityId = var1;
	this->playerController->onPlayerCreated(this->thePlayer);
	this->loadWorldSpawnArea("Respawning");
	if((dynamic_cast<GuiGameOver*>(this->currentScreen) != NULL)) {
	this->displayGuiScreen((GuiScreen*)NULL);
	}
}

void Minecraft::startMainThreadAndLogin(const JavaString& var0, const JavaString& var1)
{
	startMainThread(var0, var1, (JavaString)NULL);
}

void Minecraft::startMainThread(const JavaString& var0, const JavaString& var1, const JavaString& var2)
{
	bool var3 = false;
	Frame* var5 = new Frame("Minecraft");
	Canvas* var6 = new Canvas();
	var5->setLayout(new BorderLayout());
	var5->add(var6, "Center");
	var6->setPreferredSize(new Dimension(854, 480));
	var5->pack();
	var5->setLocationRelativeTo((Component*)NULL);
	MinecraftImpl* var7 = new MinecraftImpl(var5, var6, (MinecraftApplet*)NULL, 854, 480, var3, var5);
	Thread* var8 = new Thread(var7, "Minecraft main thread");
	var8->setPriority(10);
	var7->hideQuitButton = false;
	var7->minecraftUri = "www.minecraft.net";
	if(var0 != NULL && var1 != NULL) {
	var7->session = new Session(var0, var1);
	} else {
	var7->session = new Session((JCat() + "Player" + System::currentTimeMillis() % 1000LL),"");
	}
	if(var2 != NULL) {
	JArray<JavaString > var9 = var2.split(":");
	var7->setServer(var9[0], Integer::parseInt(var9[1]));
	}
	var5->setVisible(true);
	var5->addWindowListener(new GameWindowListener(var7, var8));
	var8->start();
}

void Minecraft::main(JArray<JavaString > var0)
{
	JavaString var1 = (JCat() + "Player" + System::currentTimeMillis() % 1000LL);
	if(var0.length > 0) {
	var1 = var0[0];
	}
	JavaString var2 = "-";
	if(var0.length > 1) {
	var2 = var0[1];
	}
	var1 = (JCat() + "Player" + System::currentTimeMillis() % 1000LL);
	var1 = "Player524";
	startMainThreadAndLogin(var1, var2);
}
