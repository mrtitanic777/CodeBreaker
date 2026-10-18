// DisplayMode.h -- real org.lwjgl.opengl.DisplayMode for the j2ctool Xbox 360 target.
//
// The converter's stub discards the constructor args and getWidth/getHeight return 0, so a game that
// does `Display.setDisplayMode(new DisplayMode(1024, 768))` then reads `getDisplayMode().getWidth()`
// (RubyDung) got 0x0 -> a zero-size backbuffer / viewport. This carries the real width/height so the
// resolution the game asks for actually reaches the D3D device.

#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

class DisplayMode {
public:
	int m_width, m_height, m_freq, m_bpp;
	DisplayMode() : m_width(0), m_height(0), m_freq(60), m_bpp(32) {}
	DisplayMode(int w, int h) : m_width(w), m_height(h), m_freq(60), m_bpp(32) {}
	DisplayMode(int w, int h, int bpp, int freq) : m_width(w), m_height(h), m_freq(freq), m_bpp(bpp) {}
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getFrequency() const { return m_freq; }
	int getBitsPerPixel() const { return m_bpp; }
	bool isFullscreenCapable() const { return true; }
};

#endif // DISPLAYMODE_H
