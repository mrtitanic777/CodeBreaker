#ifndef THREADDOWNLOADIMAGEDATA_H
#define THREADDOWNLOADIMAGEDATA_H

// Auto-generated from ThreadDownloadImageData.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class BufferedImage;
class ImageBuffer;

class ThreadDownloadImageData {
public:
	ThreadDownloadImageData(const JavaString& var1, ImageBuffer* var2);
	virtual ~ThreadDownloadImageData();

	BufferedImage* image;
	int referenceCount;
	int textureName;
	bool textureSetupComplete;

	static void initStatics();
};

#endif // THREADDOWNLOADIMAGEDATA_H