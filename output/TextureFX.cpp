#include "stdafx.h"
// Auto-generated from TextureFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"
#include "Common.h"

TextureFX::TextureFX()
	: imageData(new unsigned char[1024])
	, iconIndex(0)
	, anaglyphEnabled(false)
	, textureId(0)
{
}

TextureFX::TextureFX(int var1)
	: imageData(new unsigned char[1024])
	, iconIndex(0)
	, anaglyphEnabled(false)
	, textureId(0)
{
		iconIndex = var1;
}

TextureFX::~TextureFX() {
	delete[] imageData;
	imageData = NULL;
}

void TextureFX::onTick() {
	// TODO: implement
}
