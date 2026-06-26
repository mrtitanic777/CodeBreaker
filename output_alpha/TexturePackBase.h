#ifndef TEXTUREPACKBASE_H
#define TEXTUREPACKBASE_H

// Auto-generated from TexturePackBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class InputStream;
class Minecraft;

class TexturePackBase {
public:
	TexturePackBase();
	virtual ~TexturePackBase();

	std::string m_texturePackFileName;
	std::string m_firstDescriptionLine;
	std::string m_secondDescriptionLine;
	std::string m_field_6488_d;

	virtual void func_6482_a();
	virtual void closeTexturePackFile();
	virtual void func_6485_a(Minecraft* var1);
	virtual void func_6484_b(Minecraft* var1);
	virtual void func_6483_c(Minecraft* var1);
	virtual InputStream* func_6481_a(const std::string& var1);

};

#endif // TEXTUREPACKBASE_H