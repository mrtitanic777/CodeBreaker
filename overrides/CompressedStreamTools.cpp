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
#include "Inflate.h"   // CONSOLE: real gzip inflate for NBT (the GZIP*Stream shims below were inert no-ops)

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
// CONSOLE: real in-memory byte source (the stub returned nothing, which is why decompress() never
// round-tripped). Reads from a caller-owned buffer; does not own it.
class ByteArrayInputStream : public InputStream {
	const unsigned char* _b; int _len; int _pos;
public:
	ByteArrayInputStream(const unsigned char* b, int len) : _b(b), _len(len), _pos(0) {}
	virtual ~ByteArrayInputStream() {}
	virtual int read() { return _pos < _len ? (int)_b[_pos++] : -1; }
	virtual int read(unsigned char* dst, int off, int len) {
		if (_pos >= _len) return -1;
		int k = 0; while (k < len && _pos < _len) dst[off + k++] = _b[_pos++];
		return k;
	}
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
	// CONSOLE: NBT is gzip-compressed (RFC 1952). The GZIPInputStream shim above is inert, so the
	// original drained an empty stream and read() threw "Root tag must be a named compound tag" on
	// EVERY tile entity (Packet59ComplexEntity: chests/signs/furnaces) -> uncaught throw on the net
	// reader thread -> hard crash. Inflate for real, then parse the NBT from the raw bytes.
	int isize = 1 << 16;                               // default 64 KB
	if (var0.length >= 4) {
	const unsigned char* d = var0.data(); int dn = var0.length;
	int gz = d[dn - 4] | (d[dn - 3] << 8) | (d[dn - 2] << 16) | ((int)d[dn - 1] << 24);   // gzip ISIZE (LE)
	if (gz > 0 && gz <= (1 << 20)) isize = gz;        // trust the trailer within 1 MB, else fall back
	}
	JArray<unsigned char > raw(isize);
	int n = gzipInflate(var0.data(), var0.length, raw.data(), isize);
	if (n <= 0) {
	throw new IOException("Bad gzipped NBT");
	}
	ByteArrayInputStream bais(raw.data(), n);
	DataInputStream var1(&bais);                       // FILE-mode ctor drains bais into its own buffer
	return read(&var1);
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
