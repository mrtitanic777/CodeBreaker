#include "stdafx.h"
// Auto-generated from MinecartTrackLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MinecartTrackLogic.h"
#include "Common.h"
#include "BlockMinecartTrack.h"
#include "ChunkPosition.h"
#include "World.h"

MinecartTrackLogic::MinecartTrackLogic()
	: worldObj(NULL)
	, m_field_1165_c(0)
	, m_field_1164_d(0)
	, m_field_1163_e(0)
	, m_field_1162_f(0)
	, m_field_1160_a(NULL)
{
}

MinecartTrackLogic::MinecartTrackLogic(BlockMinecartTrack* var1, World* var2, int var3, int var4, int var5)
	: worldObj(NULL)
	, m_field_1165_c(0)
	, m_field_1164_d(0)
	, m_field_1163_e(0)
	, m_field_1162_f(0)
	, m_field_1160_a(NULL)
{
		m_field_1160_a = var1;
		m_field_1161_g.clear();
		worldObj = var2;
		m_field_1165_c = var3;
		m_field_1164_d = var4;
		m_field_1163_e = var5;
		m_field_1162_f = var2->getBlockMetadata(var3, var4, var5);
		func_789_a();
}

MinecartTrackLogic::~MinecartTrackLogic() {
	worldObj = NULL;
	m_field_1160_a = NULL;
}

void MinecartTrackLogic::func_789_a() {
  m_field_1161_g.clear();
		if(m_field_1162_f == 0) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1));
		} else if(m_field_1162_f == 1) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e));
		} else if(m_field_1162_f == 2) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c + 1, m_field_1164_d + 1, m_field_1163_e));
		} else if(m_field_1162_f == 3) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c - 1, m_field_1164_d + 1, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e));
		} else if(m_field_1162_f == 4) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e - 1));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1));
		} else if(m_field_1162_f == 5) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e + 1));
		} else if(m_field_1162_f == 6) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1));
		} else if(m_field_1162_f == 7) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1));
		} else if(m_field_1162_f == 8) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1));
		} else if(m_field_1162_f == 9) {
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e));
			m_field_1161_g.push_back(new ChunkPosition(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1));
		}

}

void MinecartTrackLogic::func_785_b() {
		for(size_t var1 = 0; var1 < m_field_1161_g.size(); ++var1) {
			MinecartTrackLogic* var2 = func_795_a((ChunkPosition*)m_field_1161_g[var1]);
			if(var2 != NULL && var2->func_793_b(this)) {
				delete (ChunkPosition*)m_field_1161_g[var1];  // PORT: free replaced entry (was Java List.set, GC'd)
				m_field_1161_g[var1] = new ChunkPosition(var2->m_field_1165_c, var2->m_field_1164_d, var2->m_field_1163_e);
			} else {
				_vecRemoveAt(m_field_1161_g, var1--);
			}
		}

}

bool MinecartTrackLogic::func_784_a(int var1, int var2, int var3) {
		return worldObj->getBlockId(var1, var2, var3) == m_field_1160_a->blockID ? true : (worldObj->getBlockId(var1, var2 + 1, var3) == m_field_1160_a->blockID ? true : worldObj->getBlockId(var1, var2 - 1, var3) == m_field_1160_a->blockID);
}

MinecartTrackLogic* MinecartTrackLogic::func_795_a(ChunkPosition* var1) {
		return worldObj->getBlockId(var1->m_x, var1->m_y, var1->m_z) == m_field_1160_a->blockID ? new MinecartTrackLogic(m_field_1160_a, worldObj, var1->m_x, var1->m_y, var1->m_z) : (worldObj->getBlockId(var1->m_x, var1->m_y + 1, var1->m_z) == m_field_1160_a->blockID ? new MinecartTrackLogic(m_field_1160_a, worldObj, var1->m_x, var1->m_y + 1, var1->m_z) : (worldObj->getBlockId(var1->m_x, var1->m_y - 1, var1->m_z) == m_field_1160_a->blockID ? new MinecartTrackLogic(m_field_1160_a, worldObj, var1->m_x, var1->m_y - 1, var1->m_z) : NULL));
}

bool MinecartTrackLogic::func_793_b(MinecartTrackLogic* var1) {
		for(size_t var2 = 0; var2 < m_field_1161_g.size(); ++var2) {
			ChunkPosition* var3 = (ChunkPosition*)m_field_1161_g[var2];
			if(var3->m_x == var1->m_field_1165_c && var3->m_z == var1->m_field_1163_e) {
				return true;
			}
		}

		return false;
}

bool MinecartTrackLogic::func_794_b(int var1, int var2, int var3) {
		for(size_t var4 = 0; var4 < m_field_1161_g.size(); ++var4) {
			ChunkPosition* var5 = (ChunkPosition*)m_field_1161_g[var4];
			if(var5->m_x == var1 && var5->m_z == var3) {
				return true;
			}
		}

		return false;
}

int MinecartTrackLogic::func_790_c() {
		int var1 = 0;
		if(func_784_a(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1)) {
			++var1;
		}

		if(func_784_a(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1)) {
			++var1;
		}

		if(func_784_a(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e)) {
			++var1;
		}

		if(func_784_a(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e)) {
			++var1;
		}

		return var1;
}

bool MinecartTrackLogic::func_787_c(MinecartTrackLogic* var1) {
		if(func_793_b(var1)) {
			return true;
		} else if(m_field_1161_g.size() == 2) {
			return false;
		} else if(m_field_1161_g.size() == 0) {
			return true;
		} else {
			ChunkPosition* var2 = (ChunkPosition*)m_field_1161_g[0];
			return var1->m_field_1164_d == m_field_1164_d && var2->m_y == m_field_1164_d ? true : true;
		}
}

void MinecartTrackLogic::func_788_d(MinecartTrackLogic* var1) {
		m_field_1161_g.push_back(new ChunkPosition(var1->m_field_1165_c, var1->m_field_1164_d, var1->m_field_1163_e));
		bool var2 = func_794_b(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1);
		bool var3 = func_794_b(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1);
		bool var4 = func_794_b(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e);
		bool var5 = func_794_b(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e);
		unsigned char var6 = -1;
		if(var2 || var3) {
			var6 = 0;
		}

		if(var4 || var5) {
			var6 = 1;
		}

		if(var3 && var5 && !var2 && !var4) {
			var6 = 6;
		}

		if(var3 && var4 && !var2 && !var5) {
			var6 = 7;
		}

		if(var2 && var4 && !var3 && !var5) {
			var6 = 8;
		}

		if(var2 && var5 && !var3 && !var4) {
			var6 = 9;
		}

		if(var6 == 0) {
			if(worldObj->getBlockId(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e - 1) == m_field_1160_a->blockID) {
				var6 = 4;
			}

			if(worldObj->getBlockId(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e + 1) == m_field_1160_a->blockID) {
				var6 = 5;
			}
		}

		if(var6 == 1) {
			if(worldObj->getBlockId(m_field_1165_c + 1, m_field_1164_d + 1, m_field_1163_e) == m_field_1160_a->blockID) {
				var6 = 2;
			}

			if(worldObj->getBlockId(m_field_1165_c - 1, m_field_1164_d + 1, m_field_1163_e) == m_field_1160_a->blockID) {
				var6 = 3;
			}
		}

		if(var6 < 0) {
			var6 = 0;
		}

		worldObj->setBlockMetadataWithNotify(m_field_1165_c, m_field_1164_d, m_field_1163_e, var6);
}

bool MinecartTrackLogic::func_786_c(int var1, int var2, int var3) {
		MinecartTrackLogic* var4 = func_795_a(new ChunkPosition(var1, var2, var3));
		if(var4 == NULL) {
			return false;
		} else {
			var4->func_785_b();
			return var4->func_787_c(this);
		}
}

void MinecartTrackLogic::func_792_a(bool var1) {
		bool var2 = func_786_c(m_field_1165_c, m_field_1164_d, m_field_1163_e - 1);
		bool var3 = func_786_c(m_field_1165_c, m_field_1164_d, m_field_1163_e + 1);
		bool var4 = func_786_c(m_field_1165_c - 1, m_field_1164_d, m_field_1163_e);
		bool var5 = func_786_c(m_field_1165_c + 1, m_field_1164_d, m_field_1163_e);
		unsigned char var6 = -1;
		if((var2 || var3) && !var4 && !var5) {
			var6 = 0;
		}

		if((var4 || var5) && !var2 && !var3) {
			var6 = 1;
		}

		if(var3 && var5 && !var2 && !var4) {
			var6 = 6;
		}

		if(var3 && var4 && !var2 && !var5) {
			var6 = 7;
		}

		if(var2 && var4 && !var3 && !var5) {
			var6 = 8;
		}

		if(var2 && var5 && !var3 && !var4) {
			var6 = 9;
		}

		if(var6 == -1) {
			if(var2 || var3) {
				var6 = 0;
			}

			if(var4 || var5) {
				var6 = 1;
			}

			if(var1) {
				if(var3 && var5) {
					var6 = 6;
				}

				if(var4 && var3) {
					var6 = 7;
				}

				if(var5 && var2) {
					var6 = 9;
				}

				if(var2 && var4) {
					var6 = 8;
				}
			} else {
				if(var2 && var4) {
					var6 = 8;
				}

				if(var5 && var2) {
					var6 = 9;
				}

				if(var4 && var3) {
					var6 = 7;
				}

				if(var3 && var5) {
					var6 = 6;
				}
			}
		}

		if(var6 == 0) {
			if(worldObj->getBlockId(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e - 1) == m_field_1160_a->blockID) {
				var6 = 4;
			}

			if(worldObj->getBlockId(m_field_1165_c, m_field_1164_d + 1, m_field_1163_e + 1) == m_field_1160_a->blockID) {
				var6 = 5;
			}
		}

		if(var6 == 1) {
			if(worldObj->getBlockId(m_field_1165_c + 1, m_field_1164_d + 1, m_field_1163_e) == m_field_1160_a->blockID) {
				var6 = 2;
			}

			if(worldObj->getBlockId(m_field_1165_c - 1, m_field_1164_d + 1, m_field_1163_e) == m_field_1160_a->blockID) {
				var6 = 3;
			}
		}

		if(var6 < 0) {
			var6 = 0;
		}

		m_field_1162_f = var6;
		func_789_a();
		worldObj->setBlockMetadataWithNotify(m_field_1165_c, m_field_1164_d, m_field_1163_e, var6);

		for(size_t var7 = 0; var7 < m_field_1161_g.size(); ++var7) {
			MinecartTrackLogic* var8 = func_795_a((ChunkPosition*)m_field_1161_g[var7]);
			if(var8 != NULL) {
				var8->func_785_b();
				if(var8->func_787_c(this)) {
					var8->func_788_d(this);
				}
			}
		}

}

int MinecartTrackLogic::func_791_a(MinecartTrackLogic* var0) {
		return var0->func_790_c();
}
