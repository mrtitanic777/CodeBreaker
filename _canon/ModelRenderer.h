#ifndef MODELRENDERER_H
#define MODELRENDERER_H

// Auto-generated from ModelRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class PositionTexureVertex;
class TexturedQuad;

class ModelRenderer {
public:
	ModelRenderer(int var1, int var2);
	virtual ~ModelRenderer();

	JArray<PositionTexureVertex* > corners;
	JArray<TexturedQuad* > faces;
	int textureOffsetX;
	int textureOffsetY;
	float offsetX;
	float offsetY;
	float offsetZ;
	float rotateAngleX;
	float rotateAngleY;
	float rotateAngleZ;
	bool compiled;
	int displayList;
	bool mirror;
	bool showModel;
	bool isHidden;

	virtual void addBox(float var1, float var2, float var3, int var4, int var5, int var6);
	virtual void addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7);
	virtual void setPosition(float var1, float var2, float var3);
	virtual void render(float var1);
	virtual void postRender(float var1);
	virtual void compileDisplayList(float var1);

	static void initStatics();
};

#endif // MODELRENDERER_H