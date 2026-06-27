#ifndef WORLDISO_H
#define WORLDISO_H

// Auto-generated from WorldIso.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "World.h"

class CanvasIsomPreview;
class File;
class IChunkProvider;

class WorldIso : public World {
public:
	WorldIso(CanvasIsomPreview* var1, File* var2, const JavaString& var3);
	virtual ~WorldIso();

	CanvasIsomPreview* A;

	virtual IChunkProvider* createChunkProvider(File* var1);

	static void initStatics();
};

#endif // WORLDISO_H