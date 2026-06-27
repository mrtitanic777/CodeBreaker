#ifndef NIBBLEARRAY_H
#define NIBBLEARRAY_H

// Auto-generated from NibbleArray.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class NibbleArray {
public:
	NibbleArray(int var1);
	NibbleArray(JArray<unsigned char > var1);
	virtual ~NibbleArray();

	JArray<unsigned char > data;

	virtual int getNibble(int var1, int var2, int var3);
	virtual void setNibble(int var1, int var2, int var3, int var4);
	virtual bool isValid();

	static void initStatics();
};

#endif // NIBBLEARRAY_H