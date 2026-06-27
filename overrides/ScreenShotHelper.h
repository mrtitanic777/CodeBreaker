#ifndef SCREENSHOTHELPER_H
#define SCREENSHOTHELPER_H

// Auto-generated from ScreenShotHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ByteBuffer;
class DateFormat;
class File;

class ScreenShotHelper {
public:
	virtual ~ScreenShotHelper();

	static DateFormat* s_dateFormat;
	static ByteBuffer* s_pixelBuffer;
	static JArray<unsigned char > s_pixelBytes;
	static JArray<int > s_pixelValues;

	static JavaString saveScreenshot(File* var0, int var1, int var2);
};

#endif // SCREENSHOTHELPER_H