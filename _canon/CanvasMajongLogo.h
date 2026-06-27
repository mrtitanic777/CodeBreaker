#ifndef CANVASMAJONGLOGO_H
#define CANVASMAJONGLOGO_H

// Auto-generated from CanvasMajongLogo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Canvas.h"

class BufferedImage;
class Graphics;

class CanvasMajongLogo : public Canvas {
public:
	CanvasMajongLogo();
	virtual ~CanvasMajongLogo();

	BufferedImage* logo;

	virtual void paint(Graphics* var1);

	static void initStatics();
};

#endif // CANVASMAJONGLOGO_H