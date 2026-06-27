#ifndef REDSTONEUPDATEINFO_H
#define REDSTONEUPDATEINFO_H

// Auto-generated from RedstoneUpdateInfo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class RedstoneUpdateInfo {
public:
	RedstoneUpdateInfo(int var1, int var2, int var3, long long var4);
	virtual ~RedstoneUpdateInfo();

	int x;
	int y;
	int z;
	long long updateTime;

	static void initStatics();
};

#endif // REDSTONEUPDATEINFO_H