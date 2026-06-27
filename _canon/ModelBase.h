#ifndef MODELBASE_H
#define MODELBASE_H

// Auto-generated from ModelBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ModelBase {
public:
	ModelBase();
	virtual ~ModelBase();

	float onGround;
	bool isChild;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();
};

#endif // MODELBASE_H