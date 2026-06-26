#ifndef MATERIALLIQUID_H
#define MATERIALLIQUID_H

// Auto-generated from MaterialLiquid.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"

class MaterialLiquid : public Material {
public:
	MaterialLiquid();
	virtual ~MaterialLiquid();

	virtual bool getIsLiquid();
	virtual bool func_880_c();
	virtual bool func_878_a();

};

#endif // MATERIALLIQUID_H