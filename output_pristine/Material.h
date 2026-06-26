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

	virtual bool getIsLiquid();
	virtual bool liquidSolidCheck();
	virtual bool isSolid();
	virtual bool getCanBlockGrass();
	virtual bool getIsSolid();

};

#endif // MATERIAL_H