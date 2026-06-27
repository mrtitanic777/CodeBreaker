#ifndef IMAGEBUFFERDOWNLOAD_H
#define IMAGEBUFFERDOWNLOAD_H

// Auto-generated from ImageBufferDownload.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ImageBuffer.h"

class BufferedImage;

class ImageBufferDownload : public ImageBuffer {
public:
	virtual ~ImageBufferDownload();

	JArray<int > imageData;
	int imageWidth;
	int imageHeight;

	virtual BufferedImage* parseUserSkin(BufferedImage* var1);
	virtual void setAreaTransparent(int var1, int var2, int var3, int var4);
	virtual void setAreaOpaque(int var1, int var2, int var3, int var4);
	virtual bool hasTransparency(int var1, int var2, int var3, int var4);
};

#endif // IMAGEBUFFERDOWNLOAD_H