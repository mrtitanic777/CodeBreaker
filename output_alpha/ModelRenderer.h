#ifndef MODELRENDERER_H
#define MODELRENDERER_H

// Auto-generated from ModelRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturedQuad.h"

class PositionTexureVertex;

class ModelRenderer {
public:
	ModelRenderer();
	ModelRenderer(int var1, int var2);
	virtual ~ModelRenderer();

	float m_offsetX;
	float m_offsetY;
	float m_offsetZ;
	float m_rotateAngleX;
	float m_rotateAngleY;
	float m_rotateAngleZ;
	bool m_mirror;
	bool m_field_1403_h;
	bool m_field_1402_i;

	virtual void func_921_a(float var1, float var2, float var3, int var4, int var5, int var6);
	virtual void addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7);
	virtual void setPosition(float var1, float var2, float var3);
	virtual void render(float var1);
	virtual void func_926_b(float var1);

public: // was private
	PositionTexureVertex* m_corners;
	TexturedQuad* m_faces;
	int m_textureOffsetX;
	int m_textureOffsetY;
	bool m_compiled;
	int m_displayList;

	void compileDisplayList(float var1);

public:  // array-length companions
	int m_corners_count;
	int m_faces_count;

};

#endif // MODELRENDERER_H