#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CompressedStreamTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CompressedStreamTools.h"
#include "DataInput.h"
#include "DataInputStream.h"
#include "DataOutput.h"
#include "DataOutputStream.h"
#include "IOException.h"
#include "InputStream.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "OutputStream.h"

// --- Local runtime shims for java.io / java.util.zip stream classes that are
//     not provided as central runtime headers. They follow the same variadic
//     stub convention as DataInputStream/OutputStream. See "runtime-stub gaps".
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  These classes stand in for real java.util.zip / java.io stream types the
 *  original imported and relied on for actual gzip (de)compression and in-memory
 *  byte buffering. The shims below are inert: GZIP streams perform NO compression
 *  and ByteArrayOutputStream::toByteArray() returns an empty array, so compress()/
 *  decompress() do not round-trip data. Original imports being faked:
 *
 *  import java.io.ByteArrayInputStream;
 *  import java.io.ByteArrayOutputStream;
 *  import java.util.zip.GZIPInputStream;
 *  import java.util.zip.GZIPOutputStream;
 *
 *  Real semantics: new GZIPInputStream(in) inflates a gzip stream; new
 *  GZIPOutputStream(out) deflates to gzip; new ByteArrayInputStream(bytes) reads
 *  from the given array; new ByteArrayOutputStream() buffers writes and
 *  toByteArray() returns the accumulated bytes.
 *  ---- end original Java ---- */
class GZIPInputStream : public InputStream {
public:
	GZIPInputStream(...) {}
	virtual ~GZIPInputStream() {}
};
class GZIPOutputStream : public OutputStream {
public:
	GZIPOutputStream(...) {}
	virtual ~GZIPOutputStream() {}
};
class ByteArrayInputStream : public InputStream {
public:
	ByteArrayInputStream(...) {}
	virtual ~ByteArrayInputStream() {}
};
class ByteArrayOutputStream : public OutputStream {
public:
	ByteArrayOutputStream(...) {}
	virtual ~ByteArrayOutputStream() {}
	JArray<unsigned char> toByteArray() { return JArray<unsigned char>(0); }
};

// RAII guard: runs close() on a stream when leaving scope (faithful try/finally).
namespace {
template <typename T>
struct CloseGuard {
	T* s;
	CloseGuard(T* p) : s(p) {}
	~CloseGuard() { if (s) s->close(); }
};
}

CompressedStreamTools::~CompressedStreamTools()
{
}

NBTTagCompound* CompressedStreamTools::readCompressed(InputStream* var0)
{
	DataInputStream* var1 = new DataInputStream(new GZIPInputStream(var0));
	NBTTagCompound* var2;
	{
	CloseGuard<DataInputStream> _g(var1);
	var2 = read(var1);
	}
	return var2;
}

void CompressedStreamTools::writeGzippedCompoundToOutputStream(NBTTagCompound* var0, OutputStream* var1)
{
	DataOutputStream* var2 = new DataOutputStream(new GZIPOutputStream(var1));
	{
	CloseGuard<DataOutputStream> _g(var2);
	writeTag(var0, var2);
	}
}

NBTTagCompound* CompressedStreamTools::decompress(JArray<unsigned char > var0)
{
	DataInputStream* var1 = new DataInputStream(new GZIPInputStream(new ByteArrayInputStream(var0)));
	NBTTagCompound* var2;
	{
	CloseGuard<DataInputStream> _g(var1);
	var2 = read(var1);
	}
	return var2;
}

JArray<unsigned char > CompressedStreamTools::compress(NBTTagCompound* var0)
{
	ByteArrayOutputStream* var1 = new ByteArrayOutputStream();
	DataOutputStream* var2 = new DataOutputStream(new GZIPOutputStream(var1));
	{
	CloseGuard<DataOutputStream> _g(var2);
	writeTag(var0, var2);
	}
	return var1->toByteArray();
}

NBTTagCompound* CompressedStreamTools::read(DataInput* var0)
{
	NBTBase* var1 = NBTBase::readTag(var0);
	if((dynamic_cast<NBTTagCompound*>(var1) != NULL)) {
	return (NBTTagCompound*)var1;
	} else {
	throw new IOException("Root tag must be a named compound tag");
	}
}

void CompressedStreamTools::writeTag(NBTTagCompound* var0, DataOutput* var1)
{
	NBTBase::writeTag(var0, var1);
}
