#ifndef TEXTUREDQUAD_H
#define TEXTUREDQUAD_H

// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PositionTextureVertex.h"

struct TexturedQuad {
public:
	TexturedQuad();
	TexturedQuad(PositionTextureVertex* var1);
	TexturedQuad(PositionTextureVertex* var1, int var2, int var3, int var4, int var5);
	virtual ~TexturedQuad();

	PositionTextureVertex* m_vertexPositions;


private:

public:  // array-length companions
	int m_vertexPositions_count;

};

#endif // TEXTUREDQUAD_H