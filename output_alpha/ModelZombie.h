#ifndef MODELZOMBIE_H
#define MODELZOMBIE_H

// Auto-generated from ModelZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ModelBiped.h"

class ModelZombie : public ModelBiped {
public:
	ModelZombie();
	virtual ~ModelZombie();

	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELZOMBIE_H