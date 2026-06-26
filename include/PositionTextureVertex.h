#ifndef POSITIONTEXTUREVERTEX_H
#define POSITIONTEXTUREVERTEX_H

// Auto-generated from PositionTextureVertex.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Vec3D.h"

struct PositionTextureVertex {
public:
	PositionTextureVertex();
	PositionTextureVertex(float var1, float var2, float var3, float var4, float var5);
	PositionTextureVertex(const PositionTextureVertex& var1, float var2, float var3);
	PositionTextureVertex(const Vec3D& var1, float var2, float var3);
	virtual ~PositionTextureVertex();

	Vec3D m_vector3D;
	bool m_hasVector3D;
	float m_texturePositionX;
	float m_texturePositionY;

	virtual PositionTextureVertex setTexturePosition(float var1, float var2);

private:

};

#endif // POSITIONTEXTUREVERTEX_H