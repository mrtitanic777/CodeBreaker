#ifndef RENDERBLOCKS_H
#define RENDERBLOCKS_H

// Auto-generated from RenderBlocks.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class IBlockAccess;
class Material;
class World;

class RenderBlocks {
public:
	RenderBlocks(IBlockAccess* var1);
	RenderBlocks();
	virtual ~RenderBlocks();

	virtual void renderBlockUsingTexture(Block* var1, int var2, int var3, int var4, int var5);
	virtual bool renderBlockByRenderType(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockTorch(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockLever(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockFire(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockRedstoneWire(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockMinecartTrack(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockLadder(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockReed(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockCrops(Block* var1, int var2, int var3, int var4);
	virtual void renderTorchAtAngle(Block* var1, double var2, double var4, double var6, double var8, double var10);
	virtual void func_1239_a(Block* var1, int var2, double var3, double var5, double var7);
	virtual void func_1245_b(Block* var1, int var2, double var3, double var5, double var7);
	virtual bool renderBlockFluids(Block* var1, int var2, int var3, int var4);
	virtual void func_1243_a(Block* var1, World* var2, int var3, int var4, int var5);
	virtual bool renderStandardBlock(Block* var1, int var2, int var3, int var4);
	virtual bool renderStandardBlockWithColorMultiplier(Block* var1, int var2, int var3, int var4, float var5, float var6, float var7);
	virtual bool renderBlockCactus(Block* var1, int var2, int var3, int var4);
	virtual bool func_1230_b(Block* var1, int var2, int var3, int var4, float var5, float var6, float var7);
	virtual bool renderBlockFence(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockStairs(Block* var1, int var2, int var3, int var4);
	virtual bool renderBlockDoor(Block* var1, int var2, int var3, int var4);
	virtual void renderBottomFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void renderTopFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void renderEastFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void renderWestFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void renderNorthFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void renderSouthFace(Block* var1, double var2, double var4, double var6, int var8);
	virtual void func_1238_a(Block* var1, float var2);
	virtual void func_1227_a(Block* var1);
	static bool func_1219_a(int var0);

public: // was private
	IBlockAccess* m_blockAccess;
	int m_overrideBlockTexture;
	bool m_flipTexture;
	bool m_renderAllFaces;

	float func_1224_a(int var1, int var2, int var3, Material* var4);

};

#endif // RENDERBLOCKS_H