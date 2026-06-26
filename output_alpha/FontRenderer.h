#ifndef FONTRENDERER_H
#define FONTRENDERER_H

// Auto-generated from FontRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class GameSettings;
class RenderEngine;

class FontRenderer {
public:
	FontRenderer();
	FontRenderer(GameSettings* var1, const std::string& var2, RenderEngine* var3);
	virtual ~FontRenderer();

	int m_field_1308_a;

	virtual void drawStringWithShadow(const std::string& var1, int var2, int var3, int var4);
	virtual void drawString(const std::string& var1, int var2, int var3, int var4);
	virtual void renderString(const std::string& var1, int var2, int var3, int var4, bool var5);
	virtual int getStringWidth(const std::string& var1);

public: // was private
	int* m_charWidth;
	int m_field_1310_c;
	int* /* PORT: was IntBuffer */ m_buffer;

public:  // array-length companions
	int m_charWidth_count;

};

#endif // FONTRENDERER_H