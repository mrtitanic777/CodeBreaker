#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MinecraftApplet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MinecraftApplet.h"
#include "Applet.h"
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
#include "CanvasMinecraftApplet.h"
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
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InterruptedException.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "LoadingScreenRenderer.h"
#include "Material.h"
#include "Minecraft.h"
#include "MinecraftAppletImpl.h"
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

// --- Local shims for runtime-stub gaps (see final report) ---
// getParameter() returns std::string; Java treats it as a String (nullable + has
// String methods). Wrap it so `!= NULL` / equalsIgnoreCase work as in Java.
static JavaString _appletParam(const std::string& s) { return JavaString(s); }
// URL stub has no getPort(); Java URL.getPort() returns -1 when no explicit port.
static int _urlGetPort(URL*) { return -1; }
// Thread stub lacks Thread(Runnable*, String) and join(long). Adapt locally: the
// stub Thread runs inline via start(), so we keep the target/name and ignore timeout.
class Runnable; // fwd (Runnable.h pulled via Thread.h)
static Thread* _makeThread(Runnable* /*target*/, const std::string& name) { return new Thread(name); }
static void _threadJoin(Thread* t, long long /*millis*/) { if (t) t->join(); }

MinecraftApplet::~MinecraftApplet()
{
}

void MinecraftApplet::init()
{
	this->mcCanvas = new CanvasMinecraftApplet(this);
	bool var1 = false;
	if(_appletParam(this->getParameter("fullscreen")) != NULL) {
	var1 = _appletParam(this->getParameter("fullscreen")).equalsIgnoreCase("true");
	}
	this->mc = new MinecraftAppletImpl(this, (Component*)(void*)this, this->mcCanvas, this, this->getWidth(), this->getHeight(), var1);
	this->mc->minecraftUri = this->getDocumentBase()->getHost();
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  if(this.getDocumentBase().getPort() > 0) {
	 *      this.mc.minecraftUri = this.mc.minecraftUri + ":" + this.getDocumentBase().getPort();
	 *  }
	 *  // The URL stub has no getPort(); _urlGetPort() always returns -1, so this
	 *  // port-appending branch is never taken (real applet would append the doc port).
	 *  ---- end original Java ---- */
	if(_urlGetPort(this->getDocumentBase()) > 0) {
	this->mc->minecraftUri = (JCat() + this->mc->minecraftUri + ":" + _urlGetPort(this->getDocumentBase()));
	}
	if(_appletParam(this->getParameter("username")) != NULL && _appletParam(this->getParameter("sessionid")) != NULL) {
	this->mc->session = new Session(this->getParameter("username"), this->getParameter("sessionid"));
	System::out.println((JCat() + "Setting user: " + this->mc->session->inventory + ", " + this->mc->session->sessionId));
	if(_appletParam(this->getParameter("mppass")) != NULL) {
	this->mc->session->mpPassword = this->getParameter("mppass");
	}
	} else {
	this->mc->session = new Session("Player", "");
	}
	if(_appletParam(this->getParameter("loadmap_user")) != NULL && _appletParam(this->getParameter("loadmap_id")) != NULL) {
	this->mc->loadMapUser = this->getParameter("loadmap_user");
	this->mc->loadMapID = Integer::parseInt(this->getParameter("loadmap_id"));
	} else if(_appletParam(this->getParameter("server")) != NULL && _appletParam(this->getParameter("port")) != NULL) {
	this->mc->setServer(this->getParameter("server"), Integer::parseInt(this->getParameter("port")));
	}
	this->mc->hideQuitButton = true;
	this->setLayout(new BorderLayout());
	this->add(this->mcCanvas, "Center");
	this->mcCanvas->setFocusable(true);
	this->validate();
}

void MinecraftApplet::startMainThread()
{
	if(this->mcThread == NULL) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.mcThread = new Thread(this.mc, "Minecraft main thread");
	 *  this.mcThread.start();
	 *  // The Thread stub lacks Thread(Runnable, String); _makeThread() builds a
	 *  // name-only Thread and DROPS the Runnable target (this.mc), so start() will
	 *  // not run Minecraft.run() on a real worker thread as the original did.
	 *  ---- end original Java ---- */
	this->mcThread = _makeThread(this->mc, "Minecraft main thread");
	this->mcThread->start();
	}
}

void MinecraftApplet::start()
{
	if(this->mc != NULL) {
	this->mc->isGamePaused = false;
	}
}

void MinecraftApplet::stop()
{
	if(this->mc != NULL) {
	this->mc->isGamePaused = true;
	}
}

void MinecraftApplet::destroy()
{
	this->shutdown();
}

void MinecraftApplet::shutdown()
{
	if(this->mcThread != NULL) {
	this->mc->shutdown();
	try {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.mcThread.join(10000L);
	 *  // The Thread stub lacks join(long); _threadJoin() ignores the 10000ms
	 *  // timeout and calls plain join() (no bounded wait as the original had).
	 *  ---- end original Java ---- */
	_threadJoin(this->mcThread, 10000L);
	} catch (InterruptedException var4) {
	try {
	this->mc->shutdownMinecraftApplet();
	} catch (Exception var3) {
	var3.printStackTrace();
	}
	}
	this->mcThread = NULL;
	}
}

void MinecraftApplet::clearReferences()
{
	this->mcCanvas = NULL;
	this->mc = NULL;
	this->mcThread = NULL;
	try {
	this->removeAll();
	this->validate();
	} catch (Exception var2) {
	}
}
