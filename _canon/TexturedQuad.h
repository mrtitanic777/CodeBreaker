#ifndef TEXTUREDQUAD_H
#define TEXTUREDQUAD_H

// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class PositionTexureVertex;
class Tessellator;

class TexturedQuad {
public:
	TexturedQuad(JArray<PositionTexureVertex* > var1);
	TexturedQuad(JArray<PositionTexureVertex* > var1, int var2, int var3, int var4, int var5);
	virtual ~TexturedQuad();

	JArray<PositionTexureVertex* > vertexPositions;
	int nVertices;
	bool invertNormal;

	virtual void flipFace();
	virtual void draw(Tessellator* var1, float var2);

	static void initStatics();
};

#endif // TEXTUREDQUAD_H