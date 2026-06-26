#ifndef TEXTUREPACKLIST_H
#define TEXTUREPACKLIST_H

// Auto-generated from TexturePackList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

class Map;
class Minecraft;
class TexturePackBase;

class TexturePackList {
public:
	TexturePackList();
	TexturePackList(Minecraft* var1, std::string /* PORT: was java.io.File */* var2);
	virtual ~TexturePackList();

	TexturePackBase* m_selectedTexturePack;

	virtual bool setTexturePack(TexturePackBase* var1);
	virtual void func_6532_a();
	virtual std::vector<void*> /* PORT: determine element type */ availableTexturePacks();

public: // was private
	std::vector<void*> /* PORT: determine element type */ m_availableTexturePacks;
	TexturePackBase* m_defaultTexturePack;
	std::map<std::string, void*>* m_field_6538_d;
	Minecraft* mc;
	std::string /* PORT: was java.io.File */* m_texturePackDir;
	std::string m_field_6535_g;

};

#endif // TEXTUREPACKLIST_H