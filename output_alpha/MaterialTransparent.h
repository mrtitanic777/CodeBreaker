#ifndef MATERIALTRANSPARENT_H
#define MATERIALTRANSPARENT_H

// Auto-generated from MaterialTransparent.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"

class MaterialTransparent : public Material {
public:
	MaterialTransparent();
	virtual ~MaterialTransparent();

	virtual bool func_878_a();
	virtual bool getCanBlockGrass();
	virtual bool func_880_c();

};

#endif // MATERIALTRANSPARENT_H