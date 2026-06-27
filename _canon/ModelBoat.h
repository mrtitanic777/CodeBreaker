#ifndef MODELBOAT_H
#define MODELBOAT_H

// Auto-generated from ModelBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelBoat : public ModelBase {
public:
	ModelBoat();
	virtual ~ModelBoat();

	JArray<ModelRenderer* > boatSides;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();
};

#endif // MODELBOAT_H