#ifndef THREADDOWNLOADIMAGE_H
#define THREADDOWNLOADIMAGE_H

// Auto-generated from ThreadDownloadImage.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class ImageBuffer;
class ThreadDownloadImageData;

class ThreadDownloadImage : public Thread {
public:
	ThreadDownloadImage(ThreadDownloadImageData* var1, const JavaString& var2, ImageBuffer* var3);
	virtual ~ThreadDownloadImage();

	JavaString location;
	ImageBuffer* buffer;
	ThreadDownloadImageData* imageData;

	virtual void run();
};

#endif // THREADDOWNLOADIMAGE_H