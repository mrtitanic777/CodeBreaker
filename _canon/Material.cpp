#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Material.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Material.h"
#include "MaterialLiquid.h"
#include "MaterialLogic.h"
#include "MaterialTransparent.h"

Material* Material::s_air;
Material* Material::s_ground;
Material* Material::s_wood;
Material* Material::s_rock;
Material* Material::s_iron;
Material* Material::s_water;
Material* Material::s_lava;
Material* Material::s_leaves;
Material* Material::s_plants;
Material* Material::s_sponge;
Material* Material::s_cloth;
Material* Material::s_fire;
Material* Material::s_sand;
Material* Material::s_circuits;
Material* Material::s_glass;
Material* Material::s_tnt;
Material* Material::s_unused;
Material* Material::s_ice;
Material* Material::s_snow;
Material* Material::s_builtSnow;
Material* Material::s_cactus;
Material* Material::s_clay;
Material* Material::s_pumpkin;
Material* Material::s_portal;

Material::~Material()
{
}

bool Material::getIsLiquid()
{
	return false;
}

bool Material::isSolid()
{
	return true;
}

bool Material::getCanBlockGrass()
{
	return true;
}

bool Material::blocksMovement()
{
	return true;
}

Material* Material::setBurning()
{
	this->canBurn = true;
	return this;
}

bool Material::getBurning()
{
	return this->canBurn;
}

void Material::initStatics()
{
	Material::s_air = new MaterialTransparent();
	Material::s_ground = new Material();
	Material::s_wood = (new Material())->setBurning();
	Material::s_rock = new Material();
	Material::s_iron = new Material();
	Material::s_water = new MaterialLiquid();
	Material::s_lava = new MaterialLiquid();
	Material::s_leaves = (new Material())->setBurning();
	Material::s_plants = new MaterialLogic();
	Material::s_sponge = new Material();
	Material::s_cloth = (new Material())->setBurning();
	Material::s_fire = new MaterialTransparent();
	Material::s_sand = new Material();
	Material::s_circuits = new MaterialLogic();
	Material::s_glass = new Material();
	Material::s_tnt = (new Material())->setBurning();
	Material::s_unused = new Material();
	Material::s_ice = new Material();
	Material::s_snow = new MaterialLogic();
	Material::s_builtSnow = new Material();
	Material::s_cactus = new Material();
	Material::s_clay = new Material();
	Material::s_pumpkin = new Material();
	Material::s_portal = new Material();
}
