#ifndef FONTRENDERER_H
#define FONTRENDERER_H

// Auto-generated from FontRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class GameSettings;
class IntBuffer;
class RenderEngine;

class FontRenderer {
public:
	FontRenderer(GameSettings* var1, const JavaString& var2, RenderEngine* var3);
	virtual ~FontRenderer();

	JArray<int > charWidth;
	int fontTextureName;
	int baseDisplayList;
	IntBuffer* buffer;

	virtual void drawStringWithShadow(const JavaString& var1, int var2, int var3, int var4);
	virtual void drawString(const JavaString& var1, int var2, int var3, int var4);
	virtual void renderString(const JavaString& var1, int var2, int var3, int var4, bool var5);
	virtual int getStringWidth(const JavaString& var1);

	static void initStatics();
};

#endif // FONTRENDERER_H