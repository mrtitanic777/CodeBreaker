#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

// Auto-generated from LevelGenerator.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class BlockFlower;
class IProgressUpdate;
class Random;
class World;

class LevelGenerator {
public:
	LevelGenerator();
	LevelGenerator(IProgressUpdate* var1);
	virtual ~LevelGenerator();

	bool m_islandGen;
	bool m_floatingGen;
	bool m_flatGen;
	int m_levelType;

	virtual World* generate(const std::string& var1, int var2, int var3, int var4);

public: // was private
	IProgressUpdate* m_guiLoading;
	int width;
	int m_depth;
	int height;
	Random* rand;
	unsigned char* m_blocksByteArray;
	int m_waterLevel;
	int m_groundLevel;
	int m_phaseBar;
	int m_phases;
	float m_phaseBareLength;
	int* m_floodFillBlocks;

	void generateHouse(World* var0);
	void growGrassOnDirt(World* var1);
	void growTrees(World* var1);
	void populateFlowersAndMushrooms(World* var1, BlockFlower* var2, int var3);
	int populateOre(int var1, int var2, int var3, int var4);
	void liquidThemeSpawner();
	void loadingBar();
	void setNextPhase(float var1);
	void lavaGen();
	long long floodFill(int var1, int var2, int var3, int var4, int var5);

public:  // array-length companions
	int m_blocksByteArray_count;
	int m_floodFillBlocks_count;

};

#endif // LEVELGENERATOR_H