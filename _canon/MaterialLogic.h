#ifndef MATERIALLOGIC_H
#define MATERIALLOGIC_H

// Auto-generated from MaterialLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Material.h"

class MaterialLogic : public Material {
public:
	virtual ~MaterialLogic();

	virtual bool isSolid();
	virtual bool getCanBlockGrass();
	virtual bool blocksMovement();

	static void initStatics();
};

#endif // MATERIALLOGIC_H