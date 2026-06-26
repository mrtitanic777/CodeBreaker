#ifndef POSITIONTEXUREVERTEX_H
#define POSITIONTEXUREVERTEX_H

// Auto-generated from PositionTexureVertex.java
// Target: Xbox 360 / C++03 / DirectX 9

class Vec3D;

class PositionTexureVertex {
public:
	PositionTexureVertex();
	PositionTexureVertex(float var1, float var2, float var3, float var4, float var5);
	PositionTexureVertex(PositionTexureVertex* var1, float var2, float var3);
	PositionTexureVertex(Vec3D* var1, float var2, float var3);
	virtual ~PositionTexureVertex();

	Vec3D* m_vector3D;
	float m_texturePositionX;
	float m_texturePositionY;

	virtual PositionTexureVertex* setTexturePosition(float var1, float var2);

};

#endif // POSITIONTEXUREVERTEX_H