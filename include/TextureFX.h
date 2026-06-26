#ifndef TEXTUREFX_H
#define TEXTUREFX_H

// Auto-generated from TextureFX.java
// Target: Xbox 360 / C++03 / DirectX 9

class TextureFX {
public:
	TextureFX();
	TextureFX(int var1);
	virtual ~TextureFX();

	unsigned char* imageData;
	int iconIndex;
	bool anaglyphEnabled;
	int textureId;

	virtual void onTick();

};

#endif // TEXTUREFX_H