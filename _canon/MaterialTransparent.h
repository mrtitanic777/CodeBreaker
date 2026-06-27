#ifndef MATERIALTRANSPARENT_H
#define MATERIALTRANSPARENT_H

// Auto-generated from MaterialTransparent.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Material.h"

class MaterialTransparent : public Material {
public:
	virtual ~MaterialTransparent();

	virtual bool isSolid();
	virtual bool getCanBlockGrass();
	virtual bool blocksMovement();

	static void initStatics();
};

#endif // MATERIALTRANSPARENT_H