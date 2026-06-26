#ifndef MODELBOAT_H
#define MODELBOAT_H

// Auto-generated from ModelBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBase.h"

class ModelRenderer;

class ModelBoat : public ModelBase {
public:
	ModelBoat();
	virtual ~ModelBoat();

	ModelRenderer* m_field_1287_a;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

public:  // array-length companions
	int m_field_1287_a_count;

};

#endif // MODELBOAT_H