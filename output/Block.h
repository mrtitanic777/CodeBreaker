#ifndef BLOCK_H
#define BLOCK_H

// Auto-generated from Block.java
// Target: Xbox 360 / C++03 / DirectX 9

struct AxisAlignedBB;
class BlockFire;
class BlockFlower;
class BlockGrass;
class EntityPlayer;
class Material;
class MovingObjectPosition;
class Random;
class StepSound;
class Vec3D;
class World;

class Block {
public:
	Block();
	Block(int var1, Material* var2);
	Block(int var1, int var2, Material* var3);
	virtual ~Block();
	static void initStatics();  // run once at startup (was Java static {})

	static Block** blocksList;
	static bool* s_tickOnLoad;
	static bool* s_opaqueCubeLookup;
	static int* s_lightOpacity;
	static bool* s_isBlockFluid;
	static int* s_lightValue;
	static Block* s_stone;
	static BlockGrass* s_grass;
	static Block* s_dirt;
	static Block* s_cobblestone;
	static Block* s_planks;
	static Block* s_sapling;
	static Block* s_bedrock;
	static Block* s_waterMoving;
	static Block* s_waterStill;
	static Block* s_lavaMoving;
	static Block* s_lavaStill;
	static Block* s_sand;
	static Block* s_gravel;
	static Block* s_oreGold;
	static Block* s_oreIron;
	static Block* s_oreCoal;
	static Block* s_wood;
	static Block* s_leaves;
	static Block* s_sponge;
	static Block* s_glass;
	static Block* s_clothRed;
	static Block* s_clothOrange;
	static Block* s_clothYellow;
	static Block* s_clothChartreuse;
	static Block* s_clothGreen;
	static Block* s_clothSpringGreen;
	static Block* s_clothCyan;
	static Block* s_clothCapri;
	static Block* s_clothUltramarine;
	static Block* s_clothViolet;
	static Block* s_clothPurple;
	static Block* s_clothMagenta;
	static Block* s_clothRose;
	static Block* s_clothDarkGray;
	static Block* s_clothGray;
	static Block* s_clothWhite;
	static BlockFlower* s_plantYellow;
	static BlockFlower* s_plantRed;
	static BlockFlower* s_mushroomBrown;
	static BlockFlower* s_mushroomRed;
	static Block* s_blockGold;
	static Block* s_blockSteel;
	static Block* s_stairDouble;
	static Block* s_stairSingle;
	static Block* s_brick;
	static Block* s_tnt;
	static Block* s_bookShelf;
	static Block* s_cobblestoneMossy;
	static Block* s_obsidian;
	static Block* s_torch;
	static BlockFire* s_fire;
	static Block* s_waterSource;
	static Block* s_lavaSource;
	static Block* s_crate;
	static Block* s_cog;
	static Block* s_oreDiamond;
	static Block* s_blockDiamond;
	static Block* s_workbench;
	static Block* s_crops;
	static Block* s_tilledField;
	int blockIndexInTexture;
	int blockID;
	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;
	StepSound* stepSound;
	float blockParticleGravity;
	Material* material;

	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual float getBlockBrightness(World* var1, int var2, int var3, int var4);
	virtual bool shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5);
	virtual int getBlockTexture(World* var1, int var2, int var3, int var4, int var5);
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual int getBlockTextureFromSide(int var1);
	virtual AxisAlignedBB getSelectedBoundingBoxFromPool(int var1, int var2, int var3);
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual bool isCollidable();
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int tickRate();
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1);
	virtual float blockStrength(EntityPlayer* var1);
	virtual void dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5);
	virtual void dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6);
	virtual float getExplosionResistance();
	virtual MovingObjectPosition* collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6);
	virtual void onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4);
	virtual int getRenderBlockPass();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual bool onBlockPlaced(World* var1, float var2, float var3, float var4);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlaced(World* var1, int var2, int var3, int var4, int var5);

public: // was protected
	virtual Block* setLightOpacity(int var1);
	virtual Block* setResistance(float var1);
	virtual Block* setHardness(float var1);
	virtual void setTickOnLoad(bool var1);
	virtual void setBlockBounds(float var1, float var2, float var3, float var4, float var5, float var6);

public: // was private
	static StepSound* s_soundPowderFootstep;
	static StepSound* s_soundWoodFootstep;
	static StepSound* s_soundGravelFootstep;
	static StepSound* s_soundGrassFootstep;
	static StepSound* s_soundStoneFootstep;
	static StepSound* s_soundMetalFootstep;
	static StepSound* s_soundGlassFootstep;
	static StepSound* s_soundClothFootstep;
	static StepSound* s_soundSandFootstep;
	static bool* s_canBlockGrass;
	float m_hardness;
	float m_resistance;

	Block* setLightValue(float var1);
	bool isVecInsideYZBounds(Vec3D* var1);
	bool isVecInsideXZBounds(Vec3D* var1);
	bool isVecInsideXYBounds(Vec3D* var1);

};

#endif // BLOCK_H