#ifndef TEXTUREPACKDEFAULT_H
#define TEXTUREPACKDEFAULT_H

// Auto-generated from TexturePackDefault.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TexturePackBase.h"

class BufferedImage;
class Minecraft;

class TexturePackDefault : public TexturePackBase {
public:
	TexturePackDefault();
	virtual ~TexturePackDefault();

	int texturePackName;
	BufferedImage* packImage;

	virtual void bindTexturePack(Minecraft* var1);
	virtual void unbindTexturePack(Minecraft* var1);

	static void initStatics();
};

#endif // TEXTUREPACKDEFAULT_H