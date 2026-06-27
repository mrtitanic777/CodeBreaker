#ifndef RENDERLIST_H
#define RENDERLIST_H

// Auto-generated from RenderList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class IntBuffer;

class RenderList {
public:
	RenderList();
	virtual ~RenderList();

	int posX;
	int posY;
	int posZ;
	float cameraX;
	float cameraY;
	float cameraZ;
	IntBuffer* intBuffer;
	bool isInitialized;
	bool isBufferFlipped;

	virtual void setupRenderList(int var1, int var2, int var3, double var4, double var6, double var8);
	virtual bool isPositionMatching(int var1, int var2, int var3);
	virtual void addGLRenderList(int var1);
	virtual void callLists();
	virtual void resetList();

	static void initStatics();
};

#endif // RENDERLIST_H