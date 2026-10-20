#ifndef BUFFEREDIMAGE_H
#define BUFFEREDIMAGE_H
// java.awt.image.BufferedImage -- a real ARGB pixel buffer (see ImageIO.cpp, which decodes PNGs
// into one via D3DX). Replaces the converter's no-op stub.
#include "JArray.h"
#include "Raster.h"                                    // getRaster() chain (ImageBufferDownload skin decode)
#include "Graphics.h"                                  // java.awt.Graphics (all-no-op stub) + j2xSharedGraphics()
class BufferedImage {
public:
	int w, h;
	JArray<int> pixels;                               // ARGB (0xAARRGGBB), row-major, w*h
	BufferedImage() : w(0), h(0) {}
	BufferedImage(int w_, int h_) : w(w_), h(h_), pixels(JArray<int>(w_ * h_)) {}
	BufferedImage(int w_, int h_, int /*type*/) : w(w_), h(h_), pixels(JArray<int>(w_ * h_)) {}  // TYPE_INT_* ignored (always ARGB)
	int getWidth()  { return w; }
	int getHeight() { return h; }
	int getRGB(int x, int y) { return pixels[y * w + x]; }
	void getRGB(int sx, int sy, int rw, int rh, JArray<int>& a, int off, int scan) {
		for (int j = 0; j < rh; ++j)
			for (int i = 0; i < rw; ++i)
				a[off + j * scan + i] = pixels[(sy + j) * w + (sx + i)];
	}
	void setRGB(int x, int y, int rgb) { pixels[y * w + x] = rgb; }
	void setRGB(int sx, int sy, int rw, int rh, JArray<int>& a, int off, int scan) {   // block form (ScreenShotHelper/RenderEngine)
		for (int j = 0; j < rh; ++j)
			for (int i = 0; i < rw; ++i)
				pixels[(sy + j) * w + (sx + i)] = a[off + j * scan + i];
	}
	// J2X-BUGFIX 2026-08-13 null-graphics: return a SHARED no-op Graphics, not NULL -- RenderEngine's ctor
	// does `missingTextureImage.getGraphics().setColor(...)`, a virtual call that NULL-deref-crashed startGame.
	Graphics* getGraphics() { return j2xSharedGraphics(); }     // AWT draw target -- no-op on console
	Graphics* createGraphics() { return j2xSharedGraphics(); }
	void flush() {}
	// java.awt.image getRaster().getDataBuffer() -> (DataBufferInt*) -> getData(): ImageBufferDownload
	// (player-skin decode over the network -- dead on the 360) pulls the ARGB int[] this way. Compiles
	// via the converter's Raster/DataBufferInt stubs; returns empty (the skin path never runs offline).
	Raster getRaster() { return Raster(); }
};
#endif // BUFFEREDIMAGE_H
