#include "stdafx.h"
// Auto-generated from WorldIso.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: dropped `#include "CanvasIsomPreview.h"` (no such class on 360 -
// it was a java.awt.Canvas-based map preview). m_A is only stored as an opaque pointer,
// so the forward declaration in WorldIso.h is sufficient.

#include "WorldIso.h"
#include "Common.h"
#include "IChunkProvider.h"
#include <string>
#include "World.h"
#include "ChunkLoader.h"
#include "ChunkProviderIso.h"

WorldIso::WorldIso()
	: m_A(NULL)
{
}

WorldIso::WorldIso(CanvasIsomPreview* var1, std::string /* PORT: was java.io.File */* var2, const std::string& var3)
	: World(var2, var3)
	, m_A(NULL)
{
		m_A = var1;
}

WorldIso::~WorldIso() {
	m_A = NULL;
}

IChunkProvider* WorldIso::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
		return new ChunkProviderIso(this, new ChunkLoader(var1, false));
}
