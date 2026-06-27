#ifndef SOUNDPOOLENTRY_H
#define SOUNDPOOLENTRY_H

// Auto-generated from SoundPoolEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class URL;

class SoundPoolEntry {
public:
	SoundPoolEntry(const JavaString& var1, URL* var2);
	virtual ~SoundPoolEntry();

	JavaString soundName;
	URL* soundUrl;

	static void initStatics();
};

#endif // SOUNDPOOLENTRY_H