#ifndef CLIPPINGHELPERFACES_H
#define CLIPPINGHELPERFACES_H

// Auto-generated from ClippingHelperFaces.java
// Target: Xbox 360 / C++03 / DirectX 9

class ClippingHelperFaces {
public:
	ClippingHelperFaces();
	ClippingHelperFaces(float var1, float var2, float var3, float var4, float var5);
	virtual ~ClippingHelperFaces();

	virtual bool isVisible(float var1, float var2, float var3, float var4);
	virtual bool isVisible(float var1, float var2, float var3, float var4, float var5, float var6);

private:
	float posX;
	float posY;
	float posZ;
	float m_posXPlus;
	float m_posYPlus;
	float m_posZPlus;
	float m_clippingBoundingBox;

};

#endif // CLIPPINGHELPERFACES_H