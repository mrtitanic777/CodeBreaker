#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CanvasMajongLogo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CanvasMajongLogo.h"
#include "BufferedImage.h"
#include "Canvas.h"
#include "Class.h"
#include "Dimension.h"
#include "Graphics.h"
#include "IOException.h"
#include "ImageIO.h"
#include "ImageObserver.h"
#include "PanelCrashReport.h"

CanvasMajongLogo::CanvasMajongLogo()
	: logo(NULL)
{
	try {
	this->logo = ImageIO::read(Class(typeid(PanelCrashReport)).getResource("/gui/logo->png"));
	} catch (IOException*) {
	}
	unsigned char var1 = 100;
	this->setPreferredSize(new Dimension(var1, var1));
	this->setMinimumSize(new Dimension(var1, var1));
}

CanvasMajongLogo::~CanvasMajongLogo()
{
}

void CanvasMajongLogo::paint(Graphics* var1)
{
	Canvas::paint(var1);
	var1->drawImage(this->logo, this->getWidth() / 2 - this->logo->getWidth() / 2, 32, (ImageObserver*)NULL);
}

void CanvasMajongLogo::initStatics()
{
}
