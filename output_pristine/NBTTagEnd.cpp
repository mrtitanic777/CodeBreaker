#include "stdafx.h"
// Auto-generated from NBTTagEnd.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NBTTagEnd.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>

NBTTagEnd::NBTTagEnd()
{
}

NBTTagEnd::~NBTTagEnd() {
	// No dynamic resources
}

void NBTTagEnd::readTagContents(DataInput* var1) {
	// TODO: implement
}

void NBTTagEnd::writeTagContents(DataOutput* var1) {
	// TODO: implement
}

unsigned char NBTTagEnd::getType() {
		return (unsigned char)0;
}

const std::string& NBTTagEnd::toString() {
		return "END";
}
