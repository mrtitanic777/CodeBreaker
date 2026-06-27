#ifndef TEXTUREPACKLIST_H
#define TEXTUREPACKLIST_H

// Auto-generated from TexturePackList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class File;
class Minecraft;
class TexturePackBase;

class TexturePackList {
public:
	TexturePackList(Minecraft* var1, File* var2);
	virtual ~TexturePackList();

	JList<void*> availableTexturePacks;
	TexturePackBase* defaultTexturePack;
	TexturePackBase* selectedTexturePack;
	std::map<std::string, void*> texturePackCache;
	Minecraft* mc;
	File* texturePackDir;
	JavaString selectedTexturePackName;

	virtual bool setTexturePack(TexturePackBase* var1);
	virtual void updateAvailableTexturePacks();
	virtual JList<void*> availableTexturePacks_();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	TexturePackList(const TexturePackList&);
	TexturePackList& operator=(const TexturePackList&);
};

#endif // TEXTUREPACKLIST_H