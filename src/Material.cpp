#include "stdafx.h"
// Auto-generated from Material.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"
#include "Common.h"
#include "MaterialLiquid.h"
#include "MaterialLogic.h"
#include "MaterialTransparent.h"

Material* Material::s_air = new MaterialTransparent();
Material* Material::s_ground = new Material();
Material* Material::s_wood = new Material();
Material* Material::s_rock = new Material();
Material* Material::s_iron = new Material();
Material* Material::s_water = new MaterialLiquid();
Material* Material::s_lava = new MaterialLiquid();
Material* Material::s_leaves = new Material();
Material* Material::s_plants = new MaterialLogic();
Material* Material::s_sponge = new Material();
Material* Material::s_cloth = new Material();
Material* Material::s_fire = new MaterialTransparent();
Material* Material::s_sand = new Material();
Material* Material::s_circuits = new MaterialLogic();
Material* Material::s_glass = new Material();
Material* Material::s_tnt = new Material();

Material::Material()
{
}

Material::~Material() {
	// No dynamic resources
}

bool Material::getIsLiquid() {
		return false;
}

bool Material::liquidSolidCheck() {
		return !getIsLiquid() && !isSolid();
}

bool Material::isSolid() {
		return true;
}

bool Material::getCanBlockGrass() {
		return true;
}

bool Material::getIsSolid() {
		return true;
}
