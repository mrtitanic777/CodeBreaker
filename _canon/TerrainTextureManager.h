#ifndef TERRAINTEXTUREMANAGER_H
#define TERRAINTEXTUREMANAGER_H

// Auto-generated from TerrainTextureManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class IsoImageBuffer;

class TerrainTextureManager {
public:
	TerrainTextureManager();
	virtual ~TerrainTextureManager();

	JArray<float > blockColors;
	JArray<int > imagePixels;
	JArray<int > blockDepth;
	JArray<int > waterDepth;
	JArray<int > waterBrightness;
	JArray<int > columnHeights;
	JArray<int > blockSideTextures;

	virtual void renderIsoTile(IsoImageBuffer* var1);
	virtual void blendWaterOverlay();

	static void initStatics();
};

#endif // TERRAINTEXTUREMANAGER_H