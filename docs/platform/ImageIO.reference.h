#ifndef IMAGEIO_H
#define IMAGEIO_H
// javax.imageio.ImageIO -> D3DX PNG decode (see ImageIO.cpp). Replaces the no-op stub.
#include "BufferedImage.h"
#include "InputStream.h"
class URL;
namespace ImageIO {
	BufferedImage* read(InputStream* in);             // decode the stream's bytes (PNG) -> pixels
	BufferedImage* readFile(const char* path);        // decode straight from a game:\Content path
	inline BufferedImage* read(URL*) { return 0; }    // URL form -- stub (skins/resources)
	inline bool write(...) { return false; }          // save (screenshots) -- no-op for now
}
#endif // IMAGEIO_H
