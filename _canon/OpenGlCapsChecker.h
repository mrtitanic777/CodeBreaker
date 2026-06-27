#ifndef OPENGLCAPSCHECKER_H
#define OPENGLCAPSCHECKER_H

// Auto-generated from OpenGlCapsChecker.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class OpenGlCapsChecker {
public:
	virtual ~OpenGlCapsChecker();

	static bool s_tryCheckOcclusionCapable;

	virtual bool checkARBOcclusion();

	static void initStatics();
};

#endif // OPENGLCAPSCHECKER_H