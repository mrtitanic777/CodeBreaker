#ifndef TEXTUREPACKCUSTOM_H
#define TEXTUREPACKCUSTOM_H

// Auto-generated from TexturePackCustom.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TexturePackBase.h"

class BufferedImage;
class File;
class InputStream;
class Minecraft;
class ZipFile;

class TexturePackCustom : public TexturePackBase {
public:
	TexturePackCustom(File* var1);
	virtual ~TexturePackCustom();

	ZipFile* texturePackZipFile;
	int texturePackName;
	BufferedImage* texturePackThumbnail;
	File* texturePackFile;

	virtual JavaString trimDescription(const JavaString& var1);
	virtual void deleteTexturePackFile(Minecraft* var1);
	virtual void bindTexturePack(Minecraft* var1);
	virtual void unbindTexturePack(Minecraft* var1);
	virtual void deleteTexturePack();
	virtual void closeTexturePackFile();
	virtual InputStream* getResourceAsStream(const JavaString& var1);
};

#endif // TEXTUREPACKCUSTOM_H