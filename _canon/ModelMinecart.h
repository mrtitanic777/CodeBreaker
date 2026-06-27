#ifndef MODELMINECART_H
#define MODELMINECART_H

// Auto-generated from ModelMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelMinecart : public ModelBase {
public:
	ModelMinecart();
	virtual ~ModelMinecart();

	JArray<ModelRenderer* > sideModels;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();
};

#endif // MODELMINECART_H