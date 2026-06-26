#ifndef MATERIAL_H
#define MATERIAL_H

// Auto-generated from Material.java
// Target: Xbox 360 / C++03 / DirectX 9

class Material {
public:
	Material();
	virtual ~Material();

	static Material* s_air;
	static Material* s_ground;
	static Material* s_wood;
	static Material* s_rock;
	static Material* s_iron;
	static Material* s_water;
	static Material* s_lava;
	static Material* s_leaves;
	static Material* s_plants;
	static Material* s_sponge;
	static Material* s_cloth;
	static Material* s_fire;
	static Material* s_sand;
	static Material* s_circuits;
	static Material* s_glass;
	static Material* s_tnt;
	static Material* s_unused;
	static Material* s_ice;
	static Material* s_snow;
	static Material* s_builtSnow;
	static Material* s_cactus;
	static Material* s_clay;
	static Material* s_pumpkin;
	static Material* s_field_4260_x;

	virtual bool getIsLiquid();
	virtual bool func_878_a();
	virtual bool getCanBlockGrass();
	virtual bool func_880_c();
	virtual bool getBurning();

public: // was private
	bool m_canBurn;

	Material* setBurning();

};

#endif // MATERIAL_H