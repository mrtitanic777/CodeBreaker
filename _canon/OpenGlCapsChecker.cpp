#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from OpenGlCapsChecker.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "OpenGlCapsChecker.h"
#include "GLContext.h"

bool OpenGlCapsChecker::s_tryCheckOcclusionCapable = false;

OpenGlCapsChecker::~OpenGlCapsChecker()
{
}

bool OpenGlCapsChecker::checkARBOcclusion()
{
	return s_tryCheckOcclusionCapable && GLContext::getCapabilities().GL_ARB_occlusion_query;
}

void OpenGlCapsChecker::initStatics()
{
}
