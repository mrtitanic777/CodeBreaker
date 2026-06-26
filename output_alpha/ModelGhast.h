#ifndef MODELGHAST_H
#define MODELGHAST_H

// Auto-generated from ModelGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelGhast : public ModelBase {
public:
	ModelGhast();
	virtual ~ModelGhast();

	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	ModelRenderer* body;
	ModelRenderer* m_tentacles;

public:  // array-length companions
	int m_tentacles_count;

};

#endif // MODELGHAST_H