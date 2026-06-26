#ifndef WORLDISO_H
#define WORLDISO_H

// Auto-generated from WorldIso.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "World.h"
#include <string>

class CanvasIsomPreview;
class IChunkProvider;

class WorldIso : public World {
public:
	WorldIso();
	WorldIso(CanvasIsomPreview* var1, std::string /* PORT: was java.io.File */* var2, const std::string& var3);
	virtual ~WorldIso();

public: // was protected
	virtual IChunkProvider* func_4081_a(std::string /* PORT: was java.io.File */* var1);

public: // was private
	CanvasIsomPreview* m_A;


};

#endif // WORLDISO_H