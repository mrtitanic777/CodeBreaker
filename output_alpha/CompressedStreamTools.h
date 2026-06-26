#ifndef COMPRESSEDSTREAMTOOLS_H
#define COMPRESSEDSTREAMTOOLS_H

// Auto-generated from CompressedStreamTools.java
// Target: Xbox 360 / C++03 / DirectX 9

class DataInput;
class DataOutput;
class InputStream;
class NBTTagCompound;
class OutputStream;

class CompressedStreamTools {
public:
	CompressedStreamTools();
	virtual ~CompressedStreamTools();

	static NBTTagCompound* func_1138_a(InputStream* var0);
	static void writeGzippedCompoundToOutputStream(NBTTagCompound* var0, OutputStream* var1);
	static NBTTagCompound* func_1140_a(unsigned char* var0);
	static unsigned char* func_1142_a(NBTTagCompound* var0);
	static NBTTagCompound* func_1141_a(DataInput* var0);
	static void func_1139_a(NBTTagCompound* var0, DataOutput* var1);

};

#endif // COMPRESSEDSTREAMTOOLS_H