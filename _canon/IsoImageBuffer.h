#ifndef ISOIMAGEBUFFER_H
#define ISOIMAGEBUFFER_H

// Auto-generated from IsoImageBuffer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class BufferedImage;
class World;

class IsoImageBuffer {
public:
	IsoImageBuffer(World* var1, int var2, int var3);
	virtual ~IsoImageBuffer();

	BufferedImage* image;
	World* worldObj;
	int anotherInt;
	int anInt;
	bool isDirty;
	bool isUpdating;
	int counter;
	bool isLoaded;

	virtual void resetData(int var1, int var2);
	virtual void setup(World* var1, int var2, int var3);

	static void initStatics();
};

#endif // ISOIMAGEBUFFER_H