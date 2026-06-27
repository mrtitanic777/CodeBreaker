#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ThreadDownloadImageData.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ThreadDownloadImageData.h"
#include "BufferedImage.h"
#include "ImageBuffer.h"
#include "ThreadDownloadImage.h"

ThreadDownloadImageData::ThreadDownloadImageData(const JavaString& var1, ImageBuffer* var2)
	: image(NULL)
	, referenceCount(0)
	, textureName(0)
	, textureSetupComplete(0)
{
	this->referenceCount = 1;
	this->textureName = -1;
	this->textureSetupComplete = false;
	(new ThreadDownloadImage(this, var1, var2))->start();
}

ThreadDownloadImageData::~ThreadDownloadImageData()
{
}

void ThreadDownloadImageData::initStatics()
{
}
