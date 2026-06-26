#include "stdafx.h"
// Auto-generated from Material::s_java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"
#include "Common.h"
#include "MaterialLiquid.h"
#include "MaterialLogic.h"
#include "MaterialTransparent.h"

Material* Material::s_air = new MaterialTransparent();
Material* Material::s_ground = new Material();
Material* Material::s_wood = (new Material())->setBurning();
Material* Material::s_rock = new Material();
Material* Material::s_iron = new Material();
Material* Material::s_water = new MaterialLiquid();
Material* Material::s_lava = new MaterialLiquid();
Material* Material::s_leaves = (new Material())->setBurning();
Material* Material::s_plants = new MaterialLogic();
Material* Material::s_sponge = new Material();
Material* Material::s_cloth = (new Material())->setBurning();
Material* Material::s_fire = new MaterialTransparent();
Material* Material::s_sand = new Material();
Material* Material::s_circuits = new MaterialLogic();
Material* Material::s_glass = new Material();
Material* Material::s_tnt = (new Material())->setBurning();
Material* Material::s_unused = new Material();
Material* Material::s_ice = new Material();
Material* Material::s_snow = new MaterialLogic();
Material* Material::s_builtSnow = new Material();
Material* Material::s_cactus = new Material();
Material* Material::s_clay = new Material();
Material* Material::s_pumpkin = new Material();
Material* Material::s_field_4260_x = new Material();

Material::Material()
	: m_canBurn(false)
{
}

Material::~Material() {
	// No dynamic resources
}

bool Material::getIsLiquid() {
		return false;
}

bool Material::func_878_a() {
		return true;
}

bool Material::getCanBlockGrass() {
		return true;
}

bool Material::func_880_c() {
		return true;
}

Material* Material::setBurning() {
		m_canBurn = true;
		return this;
}

bool Material::getBurning() {
		return m_canBurn;
}
