// ImageIO.cpp + BufferedImage — javax.imageio.ImageIO / java.awt.image.BufferedImage -> D3DX.  [Step 3]
// REFERENCE (Xbox-side; not built by j2x). REPLACES the converter's no-op ImageIO/BufferedImage
// stubs. The new RenderEngine (override) does: ImageIO::read(getResourceAsStream(name)) -> a
// BufferedImage, then getRGB(...) -> int[] ARGB, then glTexImage2D. So BufferedImage must hold real
// pixels and ImageIO::read must actually decode the PNG. PNG decode uses D3DX (the XDK decodes PNG):
// load into a scratch A8R8G8B8 surface, lock, copy to the pixel array. D3D A8R8G8B8 == Java ARGB
// (0xAARRGGBB) 1:1 -> no swizzle.
//
// BufferedImage.h (real, replacing the stub):
//   class BufferedImage { public:
//     int w, h; JArray<int> pixels;                 // ARGB, row-major, w*h
//     BufferedImage(int w_, int h_): w(w_), h(h_), pixels(JArray<int>(w_*h_)) {}
//     int getWidth()  { return w; }
//     int getHeight() { return h; }
//     int getRGB(int x, int y) { return pixels[y*w + x]; }
//     void getRGB(int sx,int sy,int rw,int rh, JArray<int>& a, int off, int scan) {
//       for (int j=0;j<rh;++j) for (int i=0;i<rw;++i) a[off + j*scan + i] = pixels[(sy+j)*w + (sx+i)]; }
//     void setRGB(int x,int y,int rgb) { pixels[y*w + x] = rgb; }
//   };
//
// PREREQUISITE: a real InputStream (Step 5 file IO) whose read() returns the PNG bytes, and
// Class::getResourceAsStream(name) resolving `name` (e.g. "/terrain.png", "/gui/gui.png") to
// game:\Content\<name>. Until Step 5, ImageIO::readFile(path) below loads straight from a path.

#include "stdafx.h"
#include "BufferedImage.h"
#include "Common.h"          // g_pd3dDevice
#include "InputStream.h"
#include <vector>

namespace ImageIO {

// Decode PNG/JPG bytes in memory -> BufferedImage (ARGB pixels), via D3DX.
static BufferedImage* decode(const void* data, unsigned size) {
	if (!data || !size) return NULL;
	D3DXIMAGE_INFO info;
	if (FAILED(D3DXGetImageInfoFromFileInMemory(data, size, &info))) return NULL;
	IDirect3DSurface9* surf = NULL;
	if (FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(info.Width, info.Height,
	            D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL)) || !surf) return NULL;
	// decode into the surface (D3DX converts to A8R8G8B8)
	if (FAILED(D3DXLoadSurfaceFromFileInMemory(surf, NULL, NULL, data, size, NULL,
	            D3DX_DEFAULT, 0, &info))) { surf->Release(); return NULL; }
	BufferedImage* img = new BufferedImage((int)info.Width, (int)info.Height);
	D3DLOCKED_RECT lr;
	if (SUCCEEDED(surf->LockRect(&lr, NULL, D3DLOCK_READONLY))) {
		for (int y = 0; y < img->h; ++y)                          // A8R8G8B8 row == ARGB ints, 1:1
			memcpy(&img->pixels[y * img->w], (const unsigned char*)lr.pBits + y * lr.Pitch, img->w * 4);
		surf->UnlockRect();
	}
	surf->Release();
	return img;
}

// ImageIO.read(InputStream): pull all bytes from the stream, then decode.
BufferedImage* read(InputStream* in) {
	if (!in) return NULL;
	std::vector<unsigned char> bytes;
	unsigned char buf[4096];
	for (;;) {
		int n = in->read(buf, 0, (int)sizeof(buf));               // real InputStream (Step 5)
		if (n <= 0) break;
		bytes.insert(bytes.end(), buf, buf + n);
	}
	if (bytes.empty()) return NULL;
	return decode(&bytes[0], (unsigned)bytes.size());
}

// Convenience used before the Step-5 stream bridge exists: decode straight from a file path.
BufferedImage* readFile(const char* path) {
	HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h == INVALID_HANDLE_VALUE) return NULL;
	DWORD size = GetFileSize(h, NULL);
	std::vector<unsigned char> bytes(size);
	DWORD got = 0;
	ReadFile(h, size ? &bytes[0] : (void*)"", size, &got, NULL);
	CloseHandle(h);
	return (got == size && size) ? decode(&bytes[0], size) : NULL;
}

} // namespace ImageIO
