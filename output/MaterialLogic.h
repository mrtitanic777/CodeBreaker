#ifndef MATERIALLOGIC_H
#define MATERIALLOGIC_H

// Auto-generated from MaterialLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"

class MaterialLogic : public Material {
public:
	MaterialLogic();
	virtual ~MaterialLogic();

	virtual bool isSolid();
	virtual bool getCanBlockGrass();
	virtual bool getIsSolid();

};

#endif // MATERIALLOGIC_H