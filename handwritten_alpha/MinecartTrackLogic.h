#ifndef MINECARTTRACKLOGIC_H
#define MINECARTTRACKLOGIC_H

// Auto-generated from MinecartTrackLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

class BlockMinecartTrack;
class ChunkPosition;
class World;

class MinecartTrackLogic {
public:
	MinecartTrackLogic();
	MinecartTrackLogic(BlockMinecartTrack* var1, World* var2, int var3, int var4, int var5);
	virtual ~MinecartTrackLogic();

	virtual void func_792_a(bool var1);

public: // was private
	World* worldObj;
	int m_field_1165_c;
	int m_field_1164_d;
	int m_field_1163_e;
	int m_field_1162_f;
	std::vector<void*> /* PORT: determine element type */ m_field_1161_g;
	BlockMinecartTrack* m_field_1160_a;

	void func_789_a();
	void func_785_b();
	bool func_784_a(int var1, int var2, int var3);
	MinecartTrackLogic* func_795_a(ChunkPosition* var1);
	bool func_793_b(MinecartTrackLogic* var1);
	bool func_794_b(int var1, int var2, int var3);
	int func_790_c();
	bool func_787_c(MinecartTrackLogic* var1);
	void func_788_d(MinecartTrackLogic* var1);
	bool func_786_c(int var1, int var2, int var3);
	static int func_791_a(MinecartTrackLogic* var0);  // PORT: Java static helper (called as MinecartTrackLogic::func_791_a)

};

#endif // MINECARTTRACKLOGIC_H