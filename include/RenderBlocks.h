#ifndef RENDERBLOCKS_H
#define RENDERBLOCKS_H

// Auto-generated from RenderBlocks.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class World;

class RenderBlocks {
public:
	RenderBlocks(World* var1);
	RenderBlocks();
	virtual ~RenderBlocks();

	virtual void renderBlockUsingTexture(Block* var1, int var2, int var3, int var4, int var5);
	virtual void renderBlockAllFaces(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockByRenderType(Block* var1, int var2, int var3, int var4);
	virtual void renderBlockOnInventory(Block* var1);

private:
	World* m_blockAccess;
	int m_overrideBlockTexture;
	bool m_flipTexture;

	void renderBlockTorch(Block* var1, float var2, float var3, float var4, float var5, float var6);
	void renderBlockPlant(Block* var1, int var2, float var3, float var4, float var5);
	void renderBlockCrops(Block* var1, int var2, float var3, float var4, float var5);
	float materialNotWater(int var1, int var2, int var3);
	void renderBlockBottom(Block* var1, float var2, float var3, float var4, int var5);
	void renderBlockTop(Block* var1, float var2, float var3, float var4, int var5);
	void renderBlockNorth(Block* var1, int var2, int var3, int var4, int var5);
	void renderBlockSouth(Block* var1, int var2, int var3, int var4, int var5);
	void renderBlockWest(Block* var1, int var2, int var3, int var4, int var5);
	void renderBlockEast(Block* var1, int var2, int var3, int var4, int var5);

};

#endif // RENDERBLOCKS_H