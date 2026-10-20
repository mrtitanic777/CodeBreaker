// javax.imageio.ImageIO on the PC: PNG decoded in the layer itself (Inflate.h), to ARGB pixels.
#ifndef IMAGEIO_H
#define IMAGEIO_H
#include "BufferedImage.h"
#include "InputStream.h"
class URL;
class File;

namespace ImageIO {
	BufferedImage* read(InputStream* in);
	BufferedImage* read(URL* u);
	BufferedImage* read(File* f);
	BufferedImage* decode(const unsigned char* data, unsigned size);   // PNG bytes -> image, or NULL
	inline bool write(...) { return false; }                            // screenshots: not yet
}
#endif // IMAGEIO_H
