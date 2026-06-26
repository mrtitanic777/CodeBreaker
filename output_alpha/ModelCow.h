#ifndef MODELCOW_H
#define MODELCOW_H

// Auto-generated from ModelCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelQuadraped.h"

class ModelRenderer;

class ModelCow : public ModelQuadraped {
public:
	ModelCow();
	virtual ~ModelCow();

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	ModelRenderer* m_udders;
	ModelRenderer* m_horn1;
	ModelRenderer* m_horn2;

};

#endif // MODELCOW_H