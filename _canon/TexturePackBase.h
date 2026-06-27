#ifndef TEXTUREPACKBASE_H
#define TEXTUREPACKBASE_H

// Auto-generated from TexturePackBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class InputStream;
class Minecraft;

class TexturePackBase {
public:
	virtual ~TexturePackBase();

	JavaString texturePackFileName;
	JavaString firstDescriptionLine;
	JavaString secondDescriptionLine;
	JavaString texturePackID;

	virtual void deleteTexturePack();
	virtual void closeTexturePackFile();
	virtual void deleteTexturePackFile(Minecraft* var1);
	virtual void bindTexturePack(Minecraft* var1);
	virtual void unbindTexturePack(Minecraft* var1);
	virtual InputStream* getResourceAsStream(const JavaString& var1);

	static void initStatics();
};

#endif // TEXTUREPACKBASE_H