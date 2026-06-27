#ifndef BUFFEREDIMAGE_H
#define BUFFEREDIMAGE_H
// java.awt.image.BufferedImage -- a real ARGB pixel buffer (see ImageIO.cpp, which decodes PNGs
// into one via D3DX). Replaces the converter's no-op stub.
#include "JArray.h"
class Graphics;                                        // java.awt.Graphics (AWT no-op stub)
class BufferedImage {
public:
	int w, h;
	JArray<int> pixels;                               // ARGB (0xAARRGGBB), row-major, w*h
	BufferedImage() : w(0), h(0) {}
	BufferedImage(int w_, int h_) : w(w_), h(h_), pixels(JArray<int>(w_ * h_)) {}
	int getWidth()  { return w; }
	int getHeight() { return h; }
	int getRGB(int x, int y) { return pixels[y * w + x]; }
	void getRGB(int sx, int sy, int rw, int rh, JArray<int>& a, int off, int scan) {
		for (int j = 0; j < rh; ++j)
			for (int i = 0; i < rw; ++i)
				a[off + j * scan + i] = pixels[(sy + j) * w + (sx + i)];
	}
	void setRGB(int x, int y, int rgb) { pixels[y * w + x] = rgb; }
	Graphics* getGraphics() { return 0; }              // AWT draw target -- no-op on console
};
#endif // BUFFEREDIMAGE_H
