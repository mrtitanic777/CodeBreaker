#ifndef RENDERBIPED_H
#define RENDERBIPED_H

// Auto-generated from RenderBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class ModelBiped;

class RenderBiped : public RenderLiving {
public:
	RenderBiped(ModelBiped* var1, float var2);
	virtual ~RenderBiped();

	ModelBiped* modelBipedMain;

	virtual void renderEquippedItems(EntityLiving* var1, float var2);

	static void initStatics();
};

#endif // RENDERBIPED_H