#ifndef WORLDGENBIGTREE_H
#define WORLDGENBIGTREE_H

// Auto-generated from WorldGenBigTree.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenBigTree : public WorldGenerator {
public:
	WorldGenBigTree();   // synthesized: Java field initializers (override had no ctor)
	virtual ~WorldGenBigTree();

	Random* rand;
	World* worldObj;
	JArray<int > basePos;
	int heightLimit;
	int trunkHeight;
	double heightAttenuation;
	double branchDensity;
	double branchSlope;
	double scaleWidth;
	double leafDensity;
	int trunkSize;
	int heightLimitLimit;
	int leafDistanceLimit;
	JArray<JArray<int > > leafNodes;
	static JArray<unsigned char > s_axisCoordPairs;

	virtual void generateLeafNodeList();
	virtual void placeLeafCrossSection(int var1, int var2, int var3, float var4, unsigned char var5, int var6);
	virtual float leafShapeRadius(int var1);
	virtual float layerLeafSize(int var1);
	virtual void generateLeafNode(int var1, int var2, int var3);
	virtual void placeBlockLine(JArray<int > var1, JArray<int > var2, int var3);
	virtual void generateLeaves();
	virtual bool leafNodeNeedsBase(int var1);
	virtual void generateTrunk();
	virtual void generateLeafNodeBases();
	virtual int checkBlockLine(JArray<int > var1, JArray<int > var2);
	virtual bool validateTreeLocation();
	virtual void setScale(double var1, double var3, double var5);
	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);
};

#endif // WORLDGENBIGTREE_H