#include "stdafx.h"
// Hand-ported from CompressedStreamTools.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: java.util.zip GZIPInputStream/GZIPOutputStream have no portable
//       equivalent here. Xbox 360 saves are stored UNCOMPRESSED, so the gzip
//       wrapping layer is dropped: the NBT tags are read/written directly via
//       the project's DataInput/DataOutput byte-buffer shims. The actual tag
//       I/O is driven by NBTBase::readTag / NBTBase::writeTag, exactly as in
//       the Java original (func_1141_a / func_1139_a).

#include "CompressedStreamTools.h"
#include "Common.h"
#include "DataInput.h"
#include "DataOutput.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "NBTTagCompound.h"
#include "NBTBase.h"

CompressedStreamTools::CompressedStreamTools()
{
}

CompressedStreamTools::~CompressedStreamTools() {
	// No dynamic resources
}

// PORT: InputStream is an empty stub on Xbox 360 (no in-memory data backing).
// The Java path wrapped it in GZIPInputStream + DataInputStream; here the
// stream-based load is not used (call sites in ChunkLoader/World are stubbed to
// the platform file I/O). Return NULL so a missing/unsupported stream is safe.
NBTTagCompound* CompressedStreamTools::func_1138_a(InputStream* var0) {
	(void)var0;
	return NULL;
}

// PORT: OutputStream is an empty stub on Xbox 360. The gzip+stream write path
// is unused (ChunkLoader/World save through platform file I/O instead).
void CompressedStreamTools::writeGzippedCompoundToOutputStream(NBTTagCompound* var0, OutputStream* var1) {
	(void)var0;
	(void)var1;
}

// PORT: read an NBT compound from a raw byte buffer. The Java version gunzipped
// the bytes first; here the buffer is treated as uncompressed NBT and read
// directly through the DataInput shim.
NBTTagCompound* CompressedStreamTools::func_1140_a(unsigned char* var0) {
	if(var0 == NULL) {
		return NULL;
	}

	// NOTE: DataInput stores a length; without an explicit size the readers are
	// bounds-checked against it. Length is unknown here, so use INT_MAX and rely
	// on the NBT End tag to terminate parsing (same as the Java stream, which
	// reads until the structure is complete).
	DataInput var1(var0, 0x7FFFFFFF);
	return func_1141_a(&var1);
}

// PORT: serialize an NBT compound to a freshly allocated raw byte buffer
// (uncompressed). Caller owns the returned buffer.
unsigned char* CompressedStreamTools::func_1142_a(NBTTagCompound* var0) {
	DataOutput var2;
	func_1139_a(var0, &var2);

	int var3 = (int)var2.buffer.size();
	unsigned char* var1 = new unsigned char[var3];
	for(int var4 = 0; var4 < var3; ++var4) {
		var1[var4] = var2.buffer[var4];
	}

	return var1;
}

NBTTagCompound* CompressedStreamTools::func_1141_a(DataInput* var0) {
	NBTBase* var1 = NBTBase::readTag(var0);
	if(dynamic_cast<NBTTagCompound*>(var1) != NULL) {
		return (NBTTagCompound*)var1;
	} else {
		DbgPrintF("ERROR [IOException]: %s\n", "Root tag must be a named compound tag");
		return NULL;
	}
}

void CompressedStreamTools::func_1139_a(NBTTagCompound* var0, DataOutput* var1) {
	NBTBase::writeTag(var0, var1);
}
