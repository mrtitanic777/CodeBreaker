#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ScreenShotHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ScreenShotHelper.h"
#include "BufferUtils.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "DateFormat.h"
#include "Exception.h"
#include "File.h"
#include "GL11.h"
#include "ImageIO.h"

// --- local shims for runtime-stub gaps ---------------------------------------
// GL_UNPACK_ALIGNMENT now lives in the real platform GL11.h, so it is NOT re-added here (doing so
// is a same-TU redefinition once GL11.h is included). java.text.SimpleDateFormat and java.util.Date
// have no runtime header, so provide minimal stand-ins. See report's runtime-stub gaps.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  import java.text.DateFormat;
 *  import java.text.SimpleDateFormat;
 *  import java.util.Date;
 *  import org.lwjgl.opengl.GL11;   // provides GL11.GL_UNPACK_ALIGNMENT
 *  ...
 *  private static DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH.mm.ss");
 *  ...
 *  String var4 = "" + dateFormat.format(new Date());   // real timestamp string
 *  ---- end original Java ----
 *  Date and SimpleDateFormat are minimal stand-ins -- crucially SimpleDateFormat.format(Date) is a
 *  NO-OP returning "" instead of a formatted timestamp, so screenshot filenames lose their date/time
 *  prefix. Needs review. (GL_UNPACK_ALIGNMENT comes from the platform GL11.h now.) */

class Date {
public:
	Date() {}
};
class SimpleDateFormat : public DateFormat {
public:
	SimpleDateFormat(const JavaString&) {}
	std::string format(Date*) { return std::string(); }
};

DateFormat* ScreenShotHelper::s_dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH.mm.ss");
ByteBuffer* ScreenShotHelper::s_pixelBuffer = NULL;
JArray<unsigned char > ScreenShotHelper::s_pixelBytes;
JArray<int > ScreenShotHelper::s_pixelValues;

ScreenShotHelper::~ScreenShotHelper()
{
}

JavaString ScreenShotHelper::saveScreenshot(File* var0, int var1, int var2)
{
	try {
	File* var3 = new File(var0, "screenshots");
	var3->mkdir();
	if(s_pixelBuffer == NULL || s_pixelBuffer->capacity() < var1 * var2 * 3) {
	s_pixelBuffer = BufferUtils::createByteBuffer(var1 * var2 * 3);
	s_pixelBytes = JArray<unsigned char >(var1 * var2 * 3);
	s_pixelValues = JArray<int >(var1 * var2);
	}
	GL11::glPixelStorei(GL11::GL_PACK_ALIGNMENT, 1);
	GL11::glPixelStorei(GL11::GL_UNPACK_ALIGNMENT, 1);
	s_pixelBuffer->clear();
	GL11::glReadPixels(0, 0, var1, var2, GL11::GL_RGB, GL11::GL_UNSIGNED_BYTE, (ByteBuffer*)s_pixelBuffer);
	s_pixelBuffer->clear();
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  String var4 = "" + dateFormat.format(new Date());
	 *  ---- end original Java ----
	 *  STUB: the SimpleDateFormat stand-in's format() returns "" (see shim above), so var4 is empty
	 *  and screenshots are named ".png" / "_2.png" ... instead of "yyyy-MM-dd_HH.mm.ss.png". */
	JavaString var4 = (JCat() + "" + s_dateFormat->format(new Date()));
	int var6 = 1;
	while(true) {
	JavaString var5suffix("");
	if(var6 != 1) { std::string s = (JCat() + "_" + var6); var5suffix = JavaString(s); }
	// Materialize the concat to a JavaString before the variadic File ctor: a
	// JCat temporary cannot be copied (non-copyable ostringstream member).
	std::string var5name = (JCat() + var4 + var5suffix + ".png");
	File* var5 = new File(var3, JavaString(var5name));
	if(!var5->exists()) {
	s_pixelBuffer->get(s_pixelBytes);
	for(int var7 = 0; var7 < var1; ++var7) {
	for(int var8 = 0; var8 < var2; ++var8) {
	int var9 = var7 + (var2 - var8 - 1) * var1;
	int var10 = s_pixelBytes[var9 * 3 + 0] & 255;
	int var11 = s_pixelBytes[var9 * 3 + 1] & 255;
	int var12 = s_pixelBytes[var9 * 3 + 2] & 255;
	int var13 = -16777216 | var10 << 16 | var11 << 8 | var12;
	s_pixelValues[var7 + var8 * var1] = var13;
	}
	}
	BufferedImage* var15 = new BufferedImage(var1, var2, 1);
	var15->setRGB(0, 0, var1, var2, s_pixelValues, 0, var1);
	ImageIO::write(var15, "png", var5);
	return (JCat() + "Saved screenshot as " + var5->getName());
	}
	++var6;
	}
	} catch (Exception var14) {
	var14.printStackTrace();
	return (JCat() + "Failed to save: " + var14.toString());
	}
}
