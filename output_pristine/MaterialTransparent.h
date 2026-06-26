#ifndef MATERIALTRANSPARENT_H
#define MATERIALTRANSPARENT_H

// Auto-generated from MaterialTransparent.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"

class MaterialTransparent : public Material {
public:
	MaterialTransparent();
	virtual ~MaterialTransparent();

	virtual bool isSolid();
	virtual bool getCanBlockGrass();
	virtual bool getIsSolid();

};

#endif // MATERIALTRANSPARENT_H