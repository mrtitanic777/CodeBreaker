#ifndef CANVASISOMPREVIEW_H
#define CANVASISOMPREVIEW_H

// Auto-generated from CanvasIsomPreview.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Canvas.h"
#include "EnumOS1.h"
#include "KeyListener.h"
#include "MouseListener.h"
#include "MouseMotionListener.h"
#include "Runnable.h"

class File;
class Graphics;
class Graphics2D;
class IsoImageBuffer;
class KeyEvent;
class MouseEvent;
class World;

class CanvasIsomPreview : public Canvas, public KeyListener, public MouseListener, public MouseMotionListener, public Runnable {
public:
	CanvasIsomPreview();
	virtual ~CanvasIsomPreview();

	int frameCount;
	int zoom;
	bool showInfo;
	World* worldObj;
	File* dataFolder;
	bool running;
	JList<void*> loadQueue;
	JArray<JArray<IsoImageBuffer* > > imageBuffers;
	int offsetX;
	int offsetY;
	int lastMouseX;
	int lastMouseY;

	virtual File* getDataFolder();
	virtual File* getMinecraftDir(const JavaString& var1);
	static int getOSType();
	virtual void loadWorld(const JavaString& var1);
	virtual void setSkylightSubtracted(int var1);
	virtual void startThreads();
	virtual void stopThreads();
	virtual IsoImageBuffer* getImageBuffer(int var1, int var2);
	virtual void run();
	virtual void update(Graphics* var1);
	virtual void paint(Graphics* var1);
	virtual void render();
	virtual void drawWorld(Graphics2D* var1);
	virtual void mouseDragged(MouseEvent* var1);
	virtual void mouseMoved(MouseEvent* var1);
	virtual void mouseClicked(MouseEvent* var1);
	virtual void mouseEntered(MouseEvent* var1);
	virtual void mouseExited(MouseEvent* var1);
	virtual void mousePressed(MouseEvent* var1);
	virtual void mouseReleased(MouseEvent* var1);
	virtual void keyPressed(KeyEvent* var1);
	virtual void keyReleased(KeyEvent* var1);
	virtual void keyTyped(KeyEvent* var1);
	static bool getRunning(CanvasIsomPreview* var0);
};

#endif // CANVASISOMPREVIEW_H