#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CanvasIsomPreview.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CanvasIsomPreview.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "Canvas.h"
#include "Chunk.h"
#include "Class.h"
#include "Color.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumOS1.h"
#include "Explosion.h"
#include "File.h"
#include "Graphics.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "ImageObserver.h"
#include "InterruptedException.h"
#include "InventoryPlayer.h"
#include "IsoImageBuffer.h"
#include "Item.h"
#include "ItemStack.h"
#include "KeyEvent.h"
#include "KeyListener.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MouseEvent.h"
#include "MouseListener.h"
#include "MouseMotionListener.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OsMap.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "Runnable.h"
#include "RuntimeException.h"
#include "StepSound.h"
#include "TerrainTextureManager.h"
#include "Thread.h"
#include "ThreadRunIsoClient.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldIso.h"
#include "WorldProvider.h"

// --- Local AWT/2D shims for the desktop iso-preview tool ---------------------
// These java.awt types have no runtime stub (this preview canvas is the only
// consumer). Minimal local definitions keep the drawing logic intact while
// compiling for the C++03 target. Behavior is a no-op render harness.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // The shims below stand in for these java.awt / java.awt.* types the class
 *  // extended/used to actually render the isometric world preview. The C++ shims
 *  // are inert: Graphics2D / BufferStrategy / FontMetrics / AffineTransform / Color
 *  // perform no real drawing, so drawWorld() produces no visible output.
 *  import java.awt.Canvas;
 *  import java.awt.Color;
 *  import java.awt.Graphics;
 *  import java.awt.Graphics2D;
 *  import java.awt.Rectangle;
 *  import java.awt.RenderingHints;
 *  import java.awt.event.KeyEvent;
 *  import java.awt.event.KeyListener;
 *  import java.awt.event.MouseEvent;
 *  import java.awt.event.MouseListener;
 *  import java.awt.event.MouseMotionListener;
 *  import java.awt.geom.AffineTransform;
 *  import java.awt.image.BufferStrategy;
 *  import java.awt.image.ImageObserver;
 *  ---- end original Java ---- */
namespace {
	// Force a JCat concat chain to a copyable JavaString (variadic ctors like File(...)
	// take args by value, but JCat holds a non-copyable ostringstream).
	inline JavaString _isoStr(const std::string& s) { return JavaString(s); }
	struct _IsoRect { int x, y, width, height; _IsoRect() : x(0), y(0), width(0), height(0) {} };
	struct _IsoAffine { };
	struct _IsoFontMetrics { int stringWidth(const std::string&) { return 0; } };
	// Color values used by this tool (java.awt.Color presets / int-RGB ctors).
	static Color* _isoColorRed() { static Color c(255, 0, 0); return &c; }
	static Color* _isoColorWhite() { static Color c(255, 255, 255); return &c; }
}

typedef _IsoRect Rectangle;
typedef _IsoAffine AffineTransform;

// java.awt.image.BufferStrategy stub (Canvas::getBufferStrategy returns void*).
class BufferStrategy {
public:
	void* getDrawGraphics() { return NULL; }
	void show() {}
};

// java.awt.Graphics2D: extends the Graphics stub with the 2D calls this tool uses.
class Graphics2D : public Graphics {
public:
	AffineTransform* getTransform() { return NULL; }
	void setTransform(AffineTransform*) {}
	void setClip(int, int, int, int) {}
	void setRenderingHint(int, int) {}
	void translate(int, int) {}
	void translate(double, double) {}
	void scale(double, double) {}
	Rectangle* getClipBounds() { static Rectangle r; return &r; }
	_IsoFontMetrics getFontMetrics() { return _IsoFontMetrics(); }
};

// java.awt.RenderingHints key/value constants used below.
namespace RenderingHints {
	const int KEY_INTERPOLATION = 0;
	const int VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;
}

// java.lang.Thread subclass that runs a Runnable target (Thread stub lacks this ctor).
namespace {
	class _IsoThread : public Thread {
		Runnable* _target;
	public:
		_IsoThread(Runnable* t) : _target(t) {}
		virtual void run() { if (_target) _target->run(); }
	};
}

CanvasIsomPreview::CanvasIsomPreview()
	: frameCount(0)
	, zoom(0)
	, showInfo(0)
	, worldObj(NULL)
	, dataFolder(NULL)
	, running(0)
	, offsetX(0)
	, offsetY(0)
	, lastMouseX(0)
	, lastMouseY(0)
{
	this->imageBuffers = JArray<JArray<IsoImageBuffer* > >(64);
	for(int var1 = 0; var1 < 64; ++var1) {
	this->imageBuffers[var1] = JArray<IsoImageBuffer* >(64);
	for(int var2 = 0; var2 < 64; ++var2) {
	this->imageBuffers[var1][var2] = new IsoImageBuffer((World*)NULL, var1, var2);
	}
	}
	this->addMouseListener(this);
	this->addMouseMotionListener(this);
	this->addKeyListener(this);
	this->setFocusable(true);
	this->requestFocus();
	this->setBackground(_isoColorRed());
}

CanvasIsomPreview::~CanvasIsomPreview()
{
}

File* CanvasIsomPreview::getDataFolder()
{
	if(this->dataFolder == NULL) {
	this->dataFolder = this->getMinecraftDir("minecraft");
	}
	return this->dataFolder;
}

File* CanvasIsomPreview::getMinecraftDir(const JavaString& var1)
{
	JavaString var2 = JavaString(System::getProperty("user.home"));
	if(var2.isEmpty()) { var2 = JavaString("."); }
	File* var3;
	switch(OsMap::s_osTypeOrdinals[getOSType()]) {
	case 1:
	case 2:
	var3 = new File(var2, _isoStr(JCat() + "." + var1 + '/'));
	break;
	case 3:
	{
	JavaString var4 = JavaString(System::getenv("APPDATA"));
	if(var4 != NULL) {
	var3 = new File(var4, _isoStr(JCat() + "." + var1 + '/'));
	} else {
	var3 = new File(var2, _isoStr(JCat() + "." + var1 + '/'));
	}
	break;
	}
	case 4:
	var3 = new File(var2, _isoStr(JCat() + "Library/Application Support/" + var1));
	break;
	default:
	var3 = new File(var2, _isoStr(JCat() + var1 + '/'));
	}
	if(!var3->exists() && !var3->mkdirs()) {
	throw new RuntimeException(_isoStr(JCat() + "The working directory could not be created: " + var3));
	} else {
	return var3;
	}
}

int CanvasIsomPreview::getOSType()
{
	JavaString var0 = JavaString(System::getProperty("os.name")).toLowerCase();
	return var0.contains("win") ? EnumOS1::windows : (var0.contains("mac") ? EnumOS1::macos : (var0.contains("solaris") ? EnumOS1::solaris : (var0.contains("sunos") ? EnumOS1::solaris : (var0.contains("linux") ? EnumOS1::linux : (var0.contains("unix") ? EnumOS1::linux : EnumOS1::unknown)))));
}

void CanvasIsomPreview::loadWorld(const JavaString& var1)
{
	this->offsetX = this->offsetY = 0;
	this->worldObj = new WorldIso(this, new File(this->dataFolder, "saves"), var1);
	this->worldObj->skylightSubtracted = 0;
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  // loadQueue is `Collections.synchronizedList(new LinkedList())` and every
	 *  // access goes through a `synchronized(var2)` monitor. The C++ drops the lock:
	 *  // loadQueue is a plain JList and the monitor becomes a bare { } scope block,
	 *  // so the cross-thread locking that protected loadQueue is no longer present.
	 *  List var2 = this.loadQueue;
	 *  synchronized(var2) {
	 *      this.loadQueue.clear();
	 *      for(int var3 = 0; var3 < 64; ++var3) {
	 *          for(int var4 = 0; var4 < 64; ++var4) {
	 *              this.imageBuffers[var3][var4].setup(this.worldObj, var3, var4);
	 *          }
	 *      }
	 *  }
	 *  ---- end original Java ---- */
	JList<void*> var2 = this->loadQueue;
	{
	this->loadQueue.clear();
	for(int var3 = 0; var3 < 64; ++var3) {
	for(int var4 = 0; var4 < 64; ++var4) {
	this->imageBuffers[var3][var4]->setup(this->worldObj, var3, var4);
	}
	}
	}
}

void CanvasIsomPreview::setSkylightSubtracted(int var1)
{
	JList<void*> var2 = this->loadQueue;
	{
	this->worldObj->skylightSubtracted = var1;
	this->loadQueue.clear();
	for(int var3 = 0; var3 < 64; ++var3) {
	for(int var4 = 0; var4 < 64; ++var4) {
	this->imageBuffers[var3][var4]->setup(this->worldObj, var3, var4);
	}
	}
	}
}

void CanvasIsomPreview::startThreads()
{
	(new ThreadRunIsoClient(this))->start();
	for(int var1 = 0; var1 < 8; ++var1) {
	(new _IsoThread(this))->start();
	}
}

void CanvasIsomPreview::stopThreads()
{
	this->running = false;
}

IsoImageBuffer* CanvasIsomPreview::getImageBuffer(int var1, int var2)
{
	int var3 = var1 & 63;
	int var4 = var2 & 63;
	IsoImageBuffer* var5 = this->imageBuffers[var3][var4];
	if(var5->anotherInt == var1 && var5->anInt == var2) {
	return var5;
	} else {
	JList<void*> var6 = this->loadQueue;
	{
	this->loadQueue.remove(var5);
	}
	var5->resetData(var1, var2);
	return var5;
	}
}

void CanvasIsomPreview::run()
{
	TerrainTextureManager* var1 = new TerrainTextureManager();
	while(this->running) {
	IsoImageBuffer* var2 = NULL;
	JList<void*> var3 = this->loadQueue;
	{
	if(this->loadQueue.size() > 0) {
	var2 = (IsoImageBuffer*)this->loadQueue.remove(0);
	}
	}
	if(var2 != NULL) {
	if(this->frameCount - var2->counter < 2) {
	var1->renderIsoTile(var2);
	this->repaint();
	} else {
	var2->isLoaded = false;
	}
	}
	try {
	Thread::sleep(2LL);
	} catch (InterruptedException var5) {
	var5.printStackTrace();
	}
	}
}

void CanvasIsomPreview::update(Graphics* var1)
{
}

void CanvasIsomPreview::paint(Graphics* var1)
{
}

void CanvasIsomPreview::render()
{
	BufferStrategy* var1 = (BufferStrategy*)this->getBufferStrategy();
	if(var1 == NULL) {
	this->createBufferStrategy(2);
	} else {
	this->drawWorld((Graphics2D*)var1->getDrawGraphics());
	var1->show();
	}
}

void CanvasIsomPreview::drawWorld(Graphics2D* var1)
{
	++this->frameCount;
	AffineTransform* var2 = var1->getTransform();
	var1->setClip(0, 0, this->getWidth(), this->getHeight());
	var1->setRenderingHint(RenderingHints::KEY_INTERPOLATION, RenderingHints::VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
	var1->translate(this->getWidth() / 2, this->getHeight() / 2);
	var1->scale((double)this->zoom, (double)this->zoom);
	var1->translate(this->offsetX, this->offsetY);
	if(this->worldObj != NULL) {
	var1->translate(-(this->worldObj->spawnX + this->worldObj->spawnZ), -(-this->worldObj->spawnX + this->worldObj->spawnZ) + 64);
	}
	Rectangle* var3 = var1->getClipBounds();
	var1->setColor(new Color(-15724512));
	var1->fillRect(var3->x, var3->y, var3->width, var3->height);
	unsigned char var4 = 16;
	unsigned char var5 = 3;
	int var6 = var3->x / var4 / 2 - 2 - var5;
	int var7 = (var3->x + var3->width) / var4 / 2 + 1 + var5;
	int var8 = var3->y / var4 - 1 - var5 * 2;
	int var9 = (var3->y + var3->height + 16 + 128) / var4 + 1 + var5 * 2;
	int var10;
	for(var10 = var8; var10 <= var9; ++var10) {
	for(int var11 = var6; var11 <= var7; ++var11) {
	int var12 = var11 - (var10 >> 1);
	int var13 = var11 + (var10 + 1 >> 1);
	IsoImageBuffer* var14 = this->getImageBuffer(var12, var13);
	var14->counter = this->frameCount;
	if(!var14->isDirty) {
	if(!var14->isLoaded) {
	var14->isLoaded = true;
	this->loadQueue.push_back(var14);
	}
	} else {
	var14->isLoaded = false;
	if(!var14->isUpdating) {
	int var15 = var11 * var4 * 2 + (var10 & 1) * var4;
	int var16 = var10 * var4 - 128 - 16;
	var1->drawImage(var14->image, var15, var16, (ImageObserver*)NULL);
	}
	}
	}
	}
	if(this->showInfo) {
	var1->setTransform(var2);
	var10 = this->getHeight() - 32 - 4;
	var1->setColor(new Color(Integer::MIN_VALUE, true));
	var1->fillRect(4, this->getHeight() - 32 - 4, this->getWidth() - 8, 32);
	var1->setColor(_isoColorWhite());
	JavaString var17 = "F1 - F5: load levels   |   0-9: Set time of day   |   Space: return to spawn   |   Double click: zoom   |   Escape: hide this text";
	var1->drawString(var17, this->getWidth() / 2 - var1->getFontMetrics().stringWidth(var17) / 2, var10 + 20);
	}
	var1->dispose();
}

void CanvasIsomPreview::mouseDragged(MouseEvent* var1)
{
	int var2 = var1->getX() / this->zoom;
	int var3 = var1->getY() / this->zoom;
	this->offsetX += var2 - this->lastMouseX;
	this->offsetY += var3 - this->lastMouseY;
	this->lastMouseX = var2;
	this->lastMouseY = var3;
	this->repaint();
}

void CanvasIsomPreview::mouseMoved(MouseEvent* var1)
{
}

void CanvasIsomPreview::mouseClicked(MouseEvent* var1)
{
	if(var1->getClickCount() == 2) {
	this->zoom = 3 - this->zoom;
	this->repaint();
	}
}

void CanvasIsomPreview::mouseEntered(MouseEvent* var1)
{
}

void CanvasIsomPreview::mouseExited(MouseEvent* var1)
{
}

void CanvasIsomPreview::mousePressed(MouseEvent* var1)
{
	int var2 = var1->getX() / this->zoom;
	int var3 = var1->getY() / this->zoom;
	this->lastMouseX = var2;
	this->lastMouseY = var3;
}

void CanvasIsomPreview::mouseReleased(MouseEvent* var1)
{
}

void CanvasIsomPreview::keyPressed(KeyEvent* var1)
{
	if(var1->getKeyCode() == 48) {
	this->setSkylightSubtracted(11);
	}
	if(var1->getKeyCode() == 49) {
	this->setSkylightSubtracted(10);
	}
	if(var1->getKeyCode() == 50) {
	this->setSkylightSubtracted(9);
	}
	if(var1->getKeyCode() == 51) {
	this->setSkylightSubtracted(7);
	}
	if(var1->getKeyCode() == 52) {
	this->setSkylightSubtracted(6);
	}
	if(var1->getKeyCode() == 53) {
	this->setSkylightSubtracted(5);
	}
	if(var1->getKeyCode() == 54) {
	this->setSkylightSubtracted(3);
	}
	if(var1->getKeyCode() == 55) {
	this->setSkylightSubtracted(2);
	}
	if(var1->getKeyCode() == 56) {
	this->setSkylightSubtracted(1);
	}
	if(var1->getKeyCode() == 57) {
	this->setSkylightSubtracted(0);
	}
	if(var1->getKeyCode() == 112) {
	this->loadWorld("World1");
	}
	if(var1->getKeyCode() == 113) {
	this->loadWorld("World2");
	}
	if(var1->getKeyCode() == 114) {
	this->loadWorld("World3");
	}
	if(var1->getKeyCode() == 115) {
	this->loadWorld("World4");
	}
	if(var1->getKeyCode() == 116) {
	this->loadWorld("World5");
	}
	if(var1->getKeyCode() == 32) {
	this->offsetX = this->offsetY = 0;
	}
	if(var1->getKeyCode() == 27) {
	this->showInfo = !this->showInfo;
	}
	this->repaint();
}

void CanvasIsomPreview::keyReleased(KeyEvent* var1)
{
}

void CanvasIsomPreview::keyTyped(KeyEvent* var1)
{
}

bool CanvasIsomPreview::getRunning(CanvasIsomPreview* var0)
{
	return var0->running;
}
