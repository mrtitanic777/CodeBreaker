#ifndef MODELRENDERER_H
#define MODELRENDERER_H

// Auto-generated from ModelRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PositionTextureVertex.h"
#include "TexturedQuad.h"

class ModelRenderer {
public:
	ModelRenderer();
	ModelRenderer(int var1, int var2);
	virtual ~ModelRenderer();

	float m_rotateAngleX;
	float m_rotateAngleY;
	float m_rotateAngleZ;
	bool m_mirror;

	virtual void addBox(float var1, float var2, float var3, int var4, int var5, int var6, float var7);
	virtual void setRotationPoint(float var1, float var2, float var3);
	virtual void render(float var1);

private:
	PositionTextureVertex* m_corners;
	TexturedQuad* m_faces;
	int m_textureOffsetX;
	int m_textureOffsetY;
	float m_rotationPointX;
	float m_rotationPointY;
	float m_rotationPointZ;
	bool m_compiled;
	int m_displayList;
	bool m_showModel;
	bool m_isHidden;

public:  // array-length companions
	int m_corners_count;
	int m_faces_count;

};

#endif // MODELRENDERER_H