#ifndef WORLDBLOCKPOSITIONTYPE_H
#define WORLDBLOCKPOSITIONTYPE_H

// Auto-generated from WorldBlockPositionType.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class WorldClient;

class WorldBlockPositionType {
public:
	WorldBlockPositionType(WorldClient* var1, int var2, int var3, int var4, int var5, int var6);
	virtual ~WorldBlockPositionType();

	int xCoord;
	int yCoord;
	int zCoord;
	int countdown;
	int blockId;
	int metadata;
	WorldClient* worldClient;

	static void initStatics();
};

#endif // WORLDBLOCKPOSITIONTYPE_H