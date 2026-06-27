#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ImageBufferDownload.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ImageBufferDownload.h"
#include "BufferedImage.h"
#include "Graphics.h"
#include "ImageBuffer.h"
#include "ImageObserver.h"

ImageBufferDownload::~ImageBufferDownload()
{
}

// BufferedImage's raster/DataBufferInt API has no runtime stub, so the original
//   imageData = ((DataBufferInt)img.getRaster().getDataBuffer()).getData();
// (which aliases the image's int[] pixel buffer) can't be expressed directly.
// Faithful equivalent: pull the pixels into imageData via getRGB, and write the
// mutated pixels back via setRGB so the returned image reflects the edits.
static void _readPixels(JArray<int>& dst, BufferedImage* img, int w, int h)
{
	dst = JArray<int>(w * h);
	for (int y = 0; y < h; ++y)
	for (int x = 0; x < w; ++x)
	dst[x + y * w] = img->getRGB(x, y);
}
static void _writePixels(BufferedImage* img, const JArray<int>& src, int w, int h)
{
	for (int y = 0; y < h; ++y)
	for (int x = 0; x < w; ++x)
	img->setRGB(x, y, src[x + y * w]);
}

BufferedImage* ImageBufferDownload::parseUserSkin(BufferedImage* var1)
{
	if(var1 == NULL) {
	return NULL;
	} else {
	this->imageWidth = 64;
	this->imageHeight = 32;
	BufferedImage* var2 = new BufferedImage(this->imageWidth, this->imageHeight, 2);
	Graphics* var3 = var2->getGraphics();
	var3->drawImage(var1, 0, 0, (ImageObserver*)NULL);
	var3->dispose();
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  this.imageData = ((DataBufferInt)var2.getRaster().getDataBuffer()).getData();
	 *  // Java aliased var2's backing int[] pixel buffer directly, so the
	 *  // setAreaOpaque/setAreaTransparent edits below mutated var2 in place.
	 *  // The C++ shim instead copies the pixels out via getRGB (_readPixels)
	 *  // and writes the edits back via setRGB (_writePixels at end of method).
	 *  ---- end original Java ---- */
	_readPixels(this->imageData, var2, this->imageWidth, this->imageHeight);
	this->setAreaOpaque(0, 0, 32, 16);
	this->setAreaTransparent(32, 0, 64, 32);
	this->setAreaOpaque(0, 16, 64, 32);
	bool var4 = false;
	int var5;
	int var6;
	int var7;
	for(var5 = 32; var5 < 64; ++var5) {
	for(var6 = 0; var6 < 16; ++var6) {
	var7 = this->imageData[var5 + var6 * 64];
	if(((var7 >> 24) & 255) < 128) {
	var4 = true;
	}
	}
	}
	if(!var4) {
	for(var5 = 32; var5 < 64; ++var5) {
	for(var6 = 0; var6 < 16; ++var6) {
	var7 = this->imageData[var5 + var6 * 64];
	if(((var7 >> 24) & 255) < 128) {
	var4 = true;
	}
	}
	}
	}
	_writePixels(var2, this->imageData, this->imageWidth, this->imageHeight);
	return var2;
	}
}

void ImageBufferDownload::setAreaTransparent(int var1, int var2, int var3, int var4)
{
	if(!this->hasTransparency(var1, var2, var3, var4)) {
	for(int var5 = var1; var5 < var3; ++var5) {
	for(int var6 = var2; var6 < var4; ++var6) {
	this->imageData[var5 + var6 * this->imageWidth] &= 16777215;
	}
	}
	}
}

void ImageBufferDownload::setAreaOpaque(int var1, int var2, int var3, int var4)
{
	for(int var5 = var1; var5 < var3; ++var5) {
	for(int var6 = var2; var6 < var4; ++var6) {
	this->imageData[var5 + var6 * this->imageWidth] |= -16777216;
	}
	}
}

bool ImageBufferDownload::hasTransparency(int var1, int var2, int var3, int var4)
{
	for(int var5 = var1; var5 < var3; ++var5) {
	for(int var6 = var2; var6 < var4; ++var6) {
	int var7 = this->imageData[var5 + var6 * this->imageWidth];
	if(((var7 >> 24) & 255) < 128) {
	return true;
	}
	}
	}
	return false;
}
