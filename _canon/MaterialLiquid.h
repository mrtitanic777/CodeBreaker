#ifndef MATERIALLIQUID_H
#define MATERIALLIQUID_H

// Auto-generated from MaterialLiquid.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Material.h"

class MaterialLiquid : public Material {
public:
	virtual ~MaterialLiquid();

	virtual bool getIsLiquid();
	virtual bool blocksMovement();
	virtual bool isSolid();

	static void initStatics();
};

#endif // MATERIALLIQUID_H