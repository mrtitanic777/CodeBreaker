#ifndef OSMAP_H
#define OSMAP_H

// Auto-generated from OsMap.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class OsMap {
public:
	virtual ~OsMap();

	static JArray<int > s_osTypeOrdinals;
	static void initStatics();   // fills s_osTypeOrdinals in order (called by j2x_initAll)
};

#endif // OSMAP_H