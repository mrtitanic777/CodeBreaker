#ifndef SCALEDRESOLUTION_H
#define SCALEDRESOLUTION_H

// Auto-generated from ScaledResolution.java
// Target: Xbox 360 / C++03 / DirectX 9

struct ScaledResolution {
public:
	ScaledResolution();
	ScaledResolution(int var1, int var2);
	virtual ~ScaledResolution();

	virtual int getScaledWidth();
	virtual int getScaledHeight();

private:
	int m_scaledWidth;
	int m_scaledHeight;

};

#endif // SCALEDRESOLUTION_H