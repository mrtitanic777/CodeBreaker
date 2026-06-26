#include "stdafx.h"
// Auto-generated from MetadataChunkBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MetadataChunkBlock.h"
#include "Common.h"
#include "Light.h"
#include "Block.h"

MetadataChunkBlock::MetadataChunkBlock()
	: m_x(0)
	, m_y(0)
	, m_z(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
{
}

MetadataChunkBlock::MetadataChunkBlock(Light* var1, int var2, int var3, int var4, int var5, int var6, int var7)
	: m_x(0)
	, m_y(0)
	, m_z(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
{
		m_x = var2;
		m_y = var3;
		m_z = var4;
		maxX = var5;
		maxY = var6;
		maxZ = var7;
}

MetadataChunkBlock::~MetadataChunkBlock() {
	// No dynamic resources
}
