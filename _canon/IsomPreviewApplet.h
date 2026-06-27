#ifndef ISOMPREVIEWAPPLET_H
#define ISOMPREVIEWAPPLET_H

// Auto-generated from IsomPreviewApplet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Applet.h"

class CanvasIsomPreview;

class IsomPreviewApplet : public Applet {
public:
	IsomPreviewApplet();
	virtual ~IsomPreviewApplet();

	CanvasIsomPreview* isomPreview;

	virtual void start();
	virtual void stop();

	static void initStatics();
};

#endif // ISOMPREVIEWAPPLET_H