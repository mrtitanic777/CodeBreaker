#ifndef MODELGHAST_H
#define MODELGHAST_H

// Auto-generated from ModelGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ModelBase.h"

class ModelRenderer;

class ModelGhast : public ModelBase {
public:
	ModelGhast();
	virtual ~ModelGhast();

	ModelRenderer* body;
	JArray<ModelRenderer* > tentacles;

	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ModelGhast(const ModelGhast&);
	ModelGhast& operator=(const ModelGhast&);
};

#endif // MODELGHAST_H