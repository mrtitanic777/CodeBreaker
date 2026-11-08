// codebreaker/java/runtime.cpp -- typed C++ port of codebreaker.java.runtime (see runtime.hpp).
//
// The C++ runtime source text this module writes out (the raw string constants below, the stub calls in
// build_generated_) is carried over from the Python verbatim: the text constants as raw string literals
// split into adjacent pieces to stay under the compiler's per-literal limit, the stub calls statement by
// statement in the Python's order. The logic around them is ported by hand.
#include "codebreaker/java/runtime.hpp"

#include <algorithm>
#include <cstdio>

#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/platform_equivalents.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/platforms.hpp"
#include "codebreaker/java/stubs.hpp"

namespace codebreaker::java::runtime {

using MethodMap = py::dict<py::str, py::str>;

namespace {

// (name, (return type, default value)) / (name, value) -- the pairs of the Python's tables
Ret R(const char* name, const char* rtype, const char* rvalue) {
    return Ret{py::str(name), std::tuple<py::str, py::str>{py::str(rtype), py::str(rvalue)}};
}
Const C(const char* name, int64_t value) { return Const{py::str(name), value}; }

// list(a) + b
StrList _concat(const StrList& a, const StrList& b) {
    StrList out;
    out.extend(a);
    out.extend(b);
    return out;
}

// s.replace(old, new, 1)
py::str _replace_first(const py::str& s, const py::str& old, const py::str& repl) {
    int64_t i = s.find(old);
    if (i < 0) return s;
    return s.slice(0, i) + repl + s.slice_from(i + old.size());
}

// The module's state: what its body builds at import.
struct Store {
    py::dict<py::str, py::str> generated;
    py::dict<py::str, StubMembers> stub_members;
    py::dict<py::str, py::str> files;
    py::list<py::str> unwired;
    bool built = false;
    bool building = false;
};

Store& store() {
    static Store s;
    return s;
}

void ensure_built();

// _CANONICAL_DIR = _data_path('platform', 'canonical')
py::str _CANONICAL_DIR() {
    return _paths::data_path(py::list<py::str>{py::str("platform"), py::str("canonical")});
}

}  // namespace

const py::str GL11_H(
R"CBJRT(#ifndef GL11_H
#define GL11_H
// LWJGL org.lwjgl.opengl.GL11 stub -- symbols only, so render code COMPILES; the real
// pipeline is the platform layer (Xbox 360 = Direct3D 9). Values are real GL 1.1 enums.

namespace GL11 {
	const int GL_POINTS = 0x0000;
	const int GL_TRIANGLES = 0x0004;
	const int GL_ZERO = 0x0000;
	const int GL_FRONT = 0x0404;
	const int GL_BACK = 0x0405;
	const int GL_FRONT_AND_BACK = 0x0408;
	const int GL_LIGHTING = 0x0B50;
	const int GL_LIGHT0 = 0x4000;
	const int GL_LIGHT1 = 0x4001;
	const int GL_FOG = 0x0B60;
	const int GL_FOG_MODE = 0x0B65;
	const int GL_FOG_DENSITY = 0x0B62;
	const int GL_FOG_START = 0x0B63;
	const int GL_FOG_END = 0x0B64;
	const int GL_FOG_COLOR = 0x0B66;
	const int GL_DEPTH_TEST = 0x0B71;
	const int GL_CULL_FACE = 0x0B44;
	const int GL_ALPHA_TEST = 0x0BC0;
	const int GL_BLEND = 0x0BE2;
	const int GL_TEXTURE_2D = 0x0DE1;
	const int GL_NORMALIZE = 0x0BA1;
	const int GL_COLOR_MATERIAL = 0x0B57;
	const int GL_MODELVIEW = 0x1700;
	const int GL_PROJECTION = 0x1701;
	const int GL_SMOOTH = 0x1D01;
	const int GL_FLAT = 0x1D00;
	const int GL_EXP = 0x0800;
	const int GL_LINEAR = 0x2601;
	const int GL_NEAREST = 0x2600;
	const int GL_GREATER = 0x0204;
	const int GL_GEQUAL = 0x0206;
	const int GL_EQUAL = 0x0202;
	const int GL_LEQUAL = 0x0203;
	const int GL_SRC_ALPHA = 0x0302;
	const int GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	const int GL_ONE = 0x0001;
	const int GL_DST_COLOR = 0x0306;
	const int GL_SRC_COLOR = 0x0300;
	const int GL_ONE_MINUS_SRC_COLOR = 0x0301;
	const int GL_ONE_MINUS_DST_COLOR = 0x0307;
	const int GL_DST_ALPHA = 0x0304;
	const int GL_RGB = 0x1907;
	const int GL_RGBA = 0x1908;
	const int GL_UNSIGNED_BYTE = 0x1401;
	const int GL_BYTE = 0x1400;
	const int GL_FLOAT = 0x1406;
	const int GL_TEXTURE = 0x1702;
	const int GL_NORMAL_ARRAY = 0x8075;
	const int GL_TEXTURE_MAG_FILTER = 0x2800;
	const int GL_TEXTURE_MIN_FILTER = 0x2801;
	const int GL_TEXTURE_WRAP_S = 0x2802;
	const int GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_NEAREST_MIPMAP_LINEAR = 0x2702;
	const int GL_LINEAR_MIPMAP_LINEAR = 0x2703;
	const int GL_REPEAT = 0x2901;
	const int GL_CLAMP = 0x2900;
	const int GL_COLOR_BUFFER_BIT = 0x4000;
	const int GL_DEPTH_BUFFER_BIT = 0x0100;
	const int GL_COMPILE = 0x1300;
	const int GL_VERTEX_ARRAY = 0x8074;
	const int GL_COLOR_ARRAY = 0x8076;
	const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_AMBIENT = 0x1200;
	const int GL_DIFFUSE = 0x1201;
	const int GL_SPECULAR = 0x1202;
	const int GL_POSITION = 0x1203;
	const int GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	const int GL_PACK_ALIGNMENT = 0x0D05;
	const int GL_UNPACK_ALIGNMENT = 0x0CF5;
	const int GL_VENDOR = 0x1F00;
	const int GL_RENDERER = 0x1F01;
	const int GL_VERSION = 0x1F02;
	const int GL_POLYGON_OFFSET_FILL = 0x8037;
	const int GL_PROJECTION_MATRIX = 0x0BA7;
	const int GL_MODELVIEW_MATRIX = 0x0BA6;

	inline void glAlphaFunc(...) {}
	inline void glBindTexture(...) {}
	inline void glBlendFunc(...) {}
	inline void glCallList(...) {}
	inline void glCallLists(...) {}
	inline void glClear(...) {}
	inline void glClearColor(...) {}
	inline void glClearDepth(...) {}
	inline void glColor3f(...) {}
	inline void glColor4f(...) {}
	inline void glColorMask(...) {}
	inline void glColorMaterial(...) {}
	inline void glColorPointer(...) {}
	inline void glCullFace(...) {}
	inline void glDeleteTextures(...) {}
	inline void glDepthFunc(...) {}
	inline void glDepthMask(...) {}
	inline void glDisable(...) {}
	inline void glDisableClientState(...) {}
	inline void glDrawArrays(...) {}
	inline void glEnable(...) {}
	inline void glEnableClientState(...) {}
	inline void glEndList(...) {}
	inline void glFog(...) {}
	inline void glFogf(...) {}
	inline void glFogi(...) {}
	inline void glGenTextures(...) {}
	inline void glLight(...) {}
	inline void glLightModel(...) {}
	inline void glLineWidth(...) {}
	inline void glLoadIdentity(...) {}
	inline void glMatrixMode(...) {}
	inline void glMultMatrix(...) {}
	inline void glNewList(...) {}
	inline void glNormal3f(...) {}
	inline void glNormalPointer(...) {}
	inline void glOrtho(...) {}
	inline void glPixelStorei(...) {}
	inline void glPopMatrix(...) {}
	inline void glPushMatrix(...) {}
	inline void glReadPixels(...) {}
	inline void glRenderList(...) {}
	inline void glRotatef(...) {}
	inline void glScaled(...) {}
	inline void glScalef(...) {}
	inline void glShadeModel(...) {}
	inline void glSkyList(...) {}
	inline void glTexCoordPointer(...) {}
	inline void glTexImage2D(...) {}
	inline void glTexParameteri(...) {}
	inline void glTexSubImage2D(...) {}
	inline void glTranslatef(...) {}
	inline void glVertexPointer(...) {}
	inline void glViewport(...) {}
	inline int glGenLists(...) { return 1; }
	inline int glGenList(...) { return 1; }
	inline int glGetInteger(...) { return 0; }
	inline const char* glGetString(...) { return ""; }
	inline int glGetError() { return 0; }
	inline void glDeleteLists(...) {}
	inline void glPolygonOffset(...) {}
	inline void glGetFloat(...) {}
}
#endif // GL11_H
)CBJRT");

const py::str FILE_H(
R"CBJRT(#ifndef FILE_H
#define FILE_H
// java.io.File (stub)
#include <string>

class File {
public:
	File() {}
	File(const std::string&) {}
	File(File*, const std::string&) {}
	virtual ~File() {}
	std::string getName() { return std::string(); }
	std::string getPath() { return std::string(); }
	std::string getAbsolutePath() { return std::string(); }
	bool exists() { return false; }
	bool isDirectory() { return false; }
	bool isFile() { return false; }
	bool mkdir() { return false; }
	bool mkdirs() { return false; }
	bool renameTo(File*) { return false; }
	long long length() { return 0LL; }
};
#endif // FILE_H
)CBJRT");

const py::str HOST_IO_CPP(
R"CBJRT(#include "stdafx.h"
#include "JavaRT.h"
#include <cstdio>
#include "File.h"
#include "FileInputStream.h"

JavaString File::s_separator = JavaString("\\");

void FileInputStream::_load(const char* p) {
	FILE* f = p && p[0] ? std::fopen(p, "rb") : 0;
	if (!f) return;                                     // _data stays NULL -> read() == -1 (EOF)
	if (std::fseek(f, 0, SEEK_END) == 0) {
		long sz = std::ftell(f);
		if (sz > 0 && std::fseek(f, 0, SEEK_SET) == 0) {
			_data = new unsigned char[sz]();
			_size = (int)std::fread(_data, 1, (size_t)sz, f);
		}
	}
	std::fclose(f);
}

FileInputStream::FileInputStream(File* f) : _data(0), _size(0), _pos(0) { _load(f ? f->path.c_str() : ""); }

// Class.getResourceAsStream: the file under the current folder, or null when there is none (as Java,
// when the resource is not on the classpath)
InputStream* J2X_getResourceAsStream(const char* name) {
	while (name && (*name == '/' || *name == '\\')) ++name;
	if (!name || !*name) return 0;
	std::string path(name);
	FILE* f = std::fopen(path.c_str(), "rb");
	if (!f) {
		// a flattened asset folder (the console's Content keeps misc/foliagecolor.png at its root): the
		// file by its own name, as the 360's file layer does. FIXED BUG flattened-assets-black-foliage:
		// without it the foliage/grass colour tables loaded empty and every leaf drew black.
		std::string::size_type s = path.find_last_of("/\\");
		if (s == std::string::npos) return 0;
		path = path.substr(s + 1);
		f = std::fopen(path.c_str(), "rb");
		if (!f) return 0;
	}
	std::fclose(f);
	return new FileInputStream(path);
}
)CBJRT");

const py::str _PLATFORM_BANNER_OLD(
R"CBJRT(the real behaviour is the Xbox 360 platform layer)CBJRT");

const py::str _PLATFORM_BANNER_SDK_OLD(
R"CBJRT(// (Direct3D 9 / XInput / file IO / sound).)CBJRT");


namespace {

const py::str _COLOR_PREFIX(
R"CBJRT(#ifndef COLOR_H
#define COLOR_H
#include <string>
#include <math.h>
class Color {
    int _rgb;
public:
    Color(int rgb) : _rgb(rgb) {}
    Color(int rgba, bool) : _rgb(rgba) {}
    Color(...) : _rgb(0) {}
    virtual ~Color() {}
    virtual int getRGB()   { return _rgb; }
    virtual int getRed()   { return (_rgb >> 16) & 255; }
    virtual int getGreen() { return (_rgb >> 8) & 255; }
    virtual int getBlue()  { return _rgb & 255; }
    virtual int getAlpha() { return (_rgb >> 24) & 255; }
    static int HSBtoRGB(float hue, float saturation, float brightness) {
        int r=0,g=0,b=0;
        if (saturation == 0.0f) { r=g=b=(int)(brightness*255.0f+0.5f); }
        else {
            float h=(hue-(float)floorf(hue))*6.0f; float f=h-(float)floorf(h);
            float p=brightness*(1.0f-saturation); float q=brightness*(1.0f-saturation*f);
            float t=brightness*(1.0f-(saturation*(1.0f-f)));
            switch((int)h){
            case 0: r=(int)(brightness*255.0f+0.5f); g=(int)(t*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;
            case 1: r=(int)(q*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;
            case 2: r=(int)(p*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(t*255.0f+0.5f); break;
            case 3: r=(int)(p*255.0f+0.5f); g=(int)(q*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;
            case 4: r=(int)(t*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;
            case 5: r=(int)(brightness*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(q*255.0f+0.5f); break;
            }
        }
        return (int)(0xFF000000u | ((unsigned)r<<16) | ((unsigned)g<<8) | (unsigned)b);
    }
    static Color getHSBColor(float h, float s, float b) { return Color(HSBtoRGB(h,s,b)); }
)CBJRT");

const py::str _COLOR_SUFFIX(
R"CBJRT(};
#endif // COLOR_H
)CBJRT");

const py::str _T_BufferUtils_h(
R"CBJRT(#ifndef BUFFERUTILS_H
#define BUFFERUTILS_H
#include "ByteBuffer.h"
#include "IntBuffer.h"
#include "FloatBuffer.h"
// org.lwjgl.BufferUtils -- direct NIO buffer allocation.
// [360] NO XDK EQUIVALENT -- hand-implemented: plain heap (NioBacking) via ByteBuffer::allocateDirect.
// FIXED BUG bufferutils-null-screenshot: createByteBuffer returned NULL -> the F2 screenshot path
// NULL-dereferenced. as{Int,Float}Buffer are views over the same backing.
namespace BufferUtils {
	inline ByteBuffer*  createByteBuffer(int n)  { return ByteBuffer::allocateDirect(n); }
	inline IntBuffer*   createIntBuffer(int n)   { return ByteBuffer::allocateDirect(n * 4)->asIntBuffer(); }
	inline FloatBuffer* createFloatBuffer(int n) { return ByteBuffer::allocateDirect(n * 4)->asFloatBuffer(); }
}
#endif // BUFFERUTILS_H
)CBJRT");

const py::str _T_SimpleDateFormat_h(
R"CBJRT(#ifndef SIMPLEDATEFORMAT_H
#define SIMPLEDATEFORMAT_H
#include "DateFormat.h"   // SimpleDateFormat lives with DateFormat
#endif
)CBJRT");

const py::str _T_Runtime_h(
R"CBJRT(#ifndef RUNTIME_H
#define RUNTIME_H
// java.lang.Runtime -- memory reporters backed by the REAL 360 kernel query (not a JVM heap, not a stub).
// [360] equivalent: GlobalMemoryStatus (xboxkrnl.exe) fills MEMORYSTATUS.dwTotalPhys / dwAvailPhys over the
// console's unified 512 MB physical pool. (Lower level: MmQueryStatistics(&MM_STATISTICS) for per-pool page
// counts.) The F3 debug overlay reads these -> it shows the actual physical used/free.
#include <string>
#include "stdafx.h"
#include "JavaString.h"

class Runtime {
public:
	Runtime(...) {}
	virtual ~Runtime() {}
	virtual void gc(...) {}                 // [360] no equivalent -- no GC on the console (explicit frees); no-op
	virtual void runFinalization(...) {}    // [360] no equivalent -- no finalizers; no-op
	virtual long long freeMemory(...)  { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwAvailPhys; }
	virtual long long totalMemory(...) { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwTotalPhys; }
	virtual long long maxMemory(...)   { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwTotalPhys; }
	static Runtime* getRuntime() { static Runtime r; return &r; }
};
#endif // RUNTIME_H
)CBJRT");

const py::str _T_RandomAccessFile_h(
R"CBJRT(#ifndef RANDOMACCESSFILE_H
#define RANDOMACCESSFILE_H

// ============================ PLATFORM REFERENCE ============================
// Real java.io.RandomAccessFile for the Xbox 360 CodeBreaker port -- REPLACES the old symbol-only
// stub. The stub's length() returned 0, which made RegionFile's ctor build a ZERO-length
// sectorFree list and then call JList::set(0,..) on it -> out-of-bounds vector write ->
// world-gen hang the instant a chunk was loaded (RegionFileCache::getChunkInputStream).
//
// FIXED BUG raf-stub-worldgen-hang: back RandomAccessFile with real stdio (FILE*) so
// length()/seek()/readInt()/write() behave like Java's. Big-endian on the wire (Java
// DataOutput order; the 360 is big-endian too, but bytes are assembled explicitly so the
// format is host-independent). The ctor also creates the file's parent directories --
// File::mkdirs() is a no-op on this port, so the "region" folder would never exist and the
// underlying fopen("...\\region\\r.0.0.mcr","rw") would fail (-> length()==0 -> same crash).
// ==========================================================================
// NOTE: relies on <cstdio> and <xtl.h> (CreateDirectoryA), both pulled in by stdafx.h (PCH).

#include <cstdio>
#include <string>
#include "JavaString.h"
#include "JArray.h"
#include "File.h"

class RandomAccessFile {
	FILE* _fp;

	static void _ensureParentDirs(const std::string& p) {
		// Create every ancestor directory of the file path (backslash/forward-slash separated).
		// Skips a leading device/drive prefix like "game:\\" or "D:\\". CreateDirectoryA is a
		// harmless no-op when the directory already exists.
		std::string::size_type start = 0;
		std::string::size_type colon = p.find(':');
		if (colon != std::string::npos) start = colon + 1;
		for (std::string::size_type i = start; i < p.size(); ++i) {
			if ((p[i] == '\\' || p[i] == '/') && i > start) {
				std::string dir = p.substr(0, i);
				CreateDirectoryA(dir.c_str(), NULL);
			}
		}
	}

	void _open(const std::string& path) {
		_ensureParentDirs(path);
		// Java "rw": open read+write, DO NOT truncate, create if absent. "rb+" gives no-truncate
		// r/w but fails when the file is missing, so fall back to "wb+" (creates a fresh empty file).
		_fp = fopen(path.c_str(), "rb+");
		if (_fp == NULL) _fp = fopen(path.c_str(), "wb+");
	}

public:
	RandomAccessFile(File* f, const std::string& /*mode*/) : _fp(NULL) {
		if (f != NULL) _open(f->path);
	}
	RandomAccessFile(const std::string& path, const std::string& /*mode*/) : _fp(NULL) {
		_open(path);
	}
	virtual ~RandomAccessFile() { if (_fp) { fclose(_fp); _fp = NULL; } }

	// ---- positioning ----
	virtual void seek(long long pos) {
		if (_fp) { fflush(_fp); fseek(_fp, (long)pos, SEEK_SET); }
	}
	virtual long long getFilePointer() {
		return _fp ? (long long)ftell(_fp) : 0LL;
	}
	virtual long long length() {
		if (!_fp) return 0LL;
		fflush(_fp);
		long cur = ftell(_fp);
		fseek(_fp, 0, SEEK_END);
		long end = ftell(_fp);
		fseek(_fp, cur, SEEK_SET);
		return (long long)end;
	}
	virtual void setLength(long long /*n*/) { /* not needed by RegionFile; kept for API parity */ }
	virtual int skipBytes(int n) {
		if (!_fp || n <= 0) return 0;
		fseek(_fp, n, SEEK_CUR);
		return n;
	}

	// ---- writes (big-endian, matching java.io.DataOutput) ----
	virtual void write(int b) { if (_fp) fputc(b & 0xFF, _fp); }
	virtual void writeByte(int b) { if (_fp) fputc(b & 0xFF, _fp); }
	virtual void writeShort(int v) {
		if (!_fp) return;
		fputc((v >> 8) & 0xFF, _fp); fputc(v & 0xFF, _fp);
	}
	virtual void writeInt(int v) {
		if (!_fp) return;
		fputc((v >> 24) & 0xFF, _fp); fputc((v >> 16) & 0xFF, _fp);
		fputc((v >> 8) & 0xFF, _fp);  fputc(v & 0xFF, _fp);
	}
	virtual void writeLong(long long v) {
		if (!_fp) return;
		for (int s = 56; s >= 0; s -= 8) fputc((int)((v >> s) & 0xFF), _fp);
	}
	virtual void write(JArray<unsigned char> b) {
		if (_fp && b.length > 0) fwrite(b.data(), 1, (size_t)b.length, _fp);
	}
	virtual void write(JArray<unsigned char> b, int off, int len) {
		if (_fp && len > 0) fwrite(b.data() + off, 1, (size_t)len, _fp);
	}
	virtual void writeFully(JArray<unsigned char> b) { write(b); }

	// ---- reads (big-endian) ----
	virtual int read(JArray<unsigned char> b) {
		if (!_fp || b.length <= 0) return -1;
		size_t n = fread(b.data(), 1, (size_t)b.length, _fp);
		return n == 0 ? -1 : (int)n;
	}
	virtual int read(JArray<unsigned char> b, int off, int len) {
		if (!_fp || len <= 0) return -1;
		size_t n = fread(b.data() + off, 1, (size_t)len, _fp);
		return n == 0 ? -1 : (int)n;
	}
	virtual void readFully(JArray<unsigned char> b) {
		if (_fp && b.length > 0) fread(b.data(), 1, (size_t)b.length, _fp);
	}
	virtual int readByte() {                     // signed byte (-128..127); -1 at EOF
		if (!_fp) return -1;
		int c = fgetc(_fp);
		return c == EOF ? -1 : (int)(signed char)c;
	}
	virtual int readUnsignedByte() {
		if (!_fp) return 0;
		int c = fgetc(_fp);
		return c == EOF ? 0 : (c & 0xFF);
	}
	virtual int readShort() {
		int a = readUnsignedByte(), b = readUnsignedByte();
		return (short)((a << 8) | b);
	}
	virtual int readInt() {
		int a = readUnsignedByte(), b = readUnsignedByte(), c = readUnsignedByte(), d = readUnsignedByte();
		return (a << 24) | (b << 16) | (c << 8) | d;
	}
	virtual long long readLong() {
		long long v = 0;
		for (int i = 0; i < 8; ++i) v = (v << 8) | (long long)readUnsignedByte();
		return v;
	}

	virtual void close() { if (_fp) { fclose(_fp); _fp = NULL; } }
};
#endif // RANDOMACCESSFILE_H
)CBJRT");

const py::str _T_Stack_h(
R"CBJRT(#ifndef STACK_H
#define STACK_H
#include <vector>
class Stack {
	std::vector<void*> _v;
public:
	void* push(void* v) { _v.push_back(v); return v; }
	void* pop() { if (_v.empty()) return 0; void* v = _v.back(); _v.pop_back(); return v; }
	void* peek() const { return _v.empty() ? 0 : _v.back(); }
	bool isEmpty() const { return _v.empty(); }
	int size() const { return (int)_v.size(); }
	void* elementAt(int i) const { return (i >= 0 && i < (int)_v.size()) ? _v[i] : 0; }
};
#endif
)CBJRT");

const py::str _T_Entry_h(
R"CBJRT(#ifndef ENTRY_H
#define ENTRY_H
class Entry {
	void* _k; void* _v;
public:
	Entry() : _k(0), _v(0) {}
	Entry(void* k, void* v) : _k(k), _v(v) {}
	void* getKey() const { return _k; }
	void* getValue() const { return _v; }
	void* setValue(void* v) { void* o = _v; _v = v; return o; }
};
#endif // ENTRY_H
)CBJRT");

const py::str _T_InputStream_h(
R"CBJRT(#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H
// java.io.InputStream -- ONE canonical definition (see CodeBreaker runtime note: inputstream-odr).
#include <string>
#include "JavaString.h"
#include "JArray.h"
class InputStream {
public:
	InputStream(...) {}
	virtual ~InputStream() {}
	virtual int read() { return -1; }
	virtual int read(unsigned char* b, int off, int len) {
		int n = 0;
		for (; n < len; ++n) { int c = read(); if (c < 0) break; b[off + n] = (unsigned char)c; }
		return (n > 0) ? n : (len == 0 ? 0 : -1);
	}
	virtual int read(unsigned char* b, int len) { return read(b, 0, len); }
	// JArray forms (Java InputStream.read(byte[])) -> delegate to the typed virtual so byte-buffer
	// streams (FileInputStream) still fill them via their override:
	virtual int read(JArray<unsigned char>& b) { return read(b.data(), 0, b.length); }
	virtual int read(JArray<unsigned char>& b, int off, int len) { return read(b.data(), off, len); }
	virtual int available() { return 0; }
	virtual long long skip(long long) { return 0; }
	virtual void close() {}
	virtual void reset() {}
	virtual void mark(int) {}
};
#endif // INPUTSTREAM_H
)CBJRT");

const py::str _T_FileInputStream_h(
R"CBJRT(#ifndef FILEINPUTSTREAM_H
#define FILEINPUTSTREAM_H
#include <string>
#include "InputStream.h"
class File;
class FileInputStream : public InputStream {
	unsigned char* _data;
	int _size;
	int _pos;
	void _load(const char* p);
public:
	FileInputStream(const std::string& p) : _data(0), _size(0), _pos(0) { _load(p.c_str()); }
	FileInputStream(File* f);
	virtual ~FileInputStream() { if (_data) delete[] _data; }
	int read() { return (_pos < _size) ? (int)_data[_pos++] : -1; }
	int read(unsigned char* b, int off, int len) { int n = 0; while (n < len && _pos < _size) b[off + n++] = _data[_pos++]; return (n > 0) ? n : (len == 0 ? 0 : -1); }
	int read(unsigned char* b, int len) { return read(b, 0, len); }
	int available() { return _size - _pos; }
	void close() {}
};
#endif // FILEINPUTSTREAM_H
)CBJRT");

const py::str _T_GLContext_h(
R"CBJRT(#ifndef GLCONTEXT_H
#define GLCONTEXT_H
// org.lwjgl.opengl.GLContext + ContextCapabilities stub. Capability flags are public bools
// (the real values come from the Xbox 360 platform/D3D9 layer) so `getCapabilities().GL_ARB_x`
// compiles. Default false = feature absent.
struct ContextCapabilities {
	bool GL_ARB_occlusion_query, GL_ARB_vertex_buffer_object, GL_ARB_multitexture;
	bool GL_ARB_texture_env_combine, GL_EXT_blend_func_separate, GL_ARB_shader_objects;
	bool GL_NV_fog_distance, GL_EXT_texture_filter_anisotropic, GL_ARB_texture_non_power_of_two;
	bool OpenGL11, OpenGL12, OpenGL13, OpenGL14, OpenGL15, OpenGL20, OpenGL21;
	bool OpenGL30, OpenGL31, OpenGL32, OpenGL33, GL_ARB_compatibility;   // Indev logs these at boot
	ContextCapabilities() : GL_ARB_occlusion_query(false), GL_ARB_vertex_buffer_object(false),
		GL_ARB_multitexture(false), GL_ARB_texture_env_combine(false), GL_EXT_blend_func_separate(false),
		GL_ARB_shader_objects(false), GL_NV_fog_distance(false), GL_EXT_texture_filter_anisotropic(false),
		GL_ARB_texture_non_power_of_two(false), OpenGL11(false), OpenGL12(false), OpenGL13(false),
		OpenGL14(false), OpenGL15(false), OpenGL20(false), OpenGL21(false),
		OpenGL30(false), OpenGL31(false), OpenGL32(false), OpenGL33(false), GL_ARB_compatibility(false) {}
};
namespace GLContext {
	// J2X-BUGFIX glcontext-caps-ref: getCapabilities() returns a REFERENCE to the shared static, matching LWJGL
	// where the game does `GLContext.getCapabilities().GL_ARB_x` (member access via `.`). Returning a POINTER
	// made every faithful `.GL_ARB_x` site fail ("left of '.' must have class type ... use '->'").
	inline ContextCapabilities& getCapabilities() { static ContextCapabilities _c; return _c; }
}
#endif // GLCONTEXT_H
)CBJRT");

const py::str _T_Class_h(
R"CBJRT(#ifndef CLASS_H
#define CLASS_H
// java.lang.Class -- backed by C++ RTTI (std::type_info). Identity, ==, getName and map-key
// ordering are REAL: `X.class` (Class(typeid(X))) and `obj.getClass()` (Class(typeid(*obj)))
// agree for the same runtime type. newInstance/getConstructor have no reflection equivalent and
// return 0 -- the class-keyed registries (EntityList/Packet/TileEntity) supply explicit factories.
#include <string>
#include <typeinfo>
#include <vector>
#if defined(_XBOX)
#include <xtl.h>        // classreg CRITICAL_SECTION (thread-safe RTTI under MP net threads)
#elif defined(_WIN32)
#include <windows.h>
#endif
#include "URL.h"                                          // getResource returns a path-carrying URL
class InputStream;
InputStream* J2X_getResourceAsStream(const char* name);   // FileIO.cpp: "/x.png" -> game:\Content\x.png

// Program-lifetime registry of every Class ever materialised (shared by j2xClassOf below and
// Class::getSuperclass). Lets getSuperclass turn a base NAME (from _j2xBases) back into a Class*.
class Class;
inline std::vector<Class*>& _j2xClassReg() { static std::vector<Class*> reg; return reg; }
// FIXED BUG classreg-multithread-race: j2xClassOf/getSuperclass touch this shared registry from the
// main thread (RTTI on every tick) AND the MP network reader thread (packet reflection). Serialize
// the lookup+append with one program-wide CS. Uncontended in single-thread SP. (j2xPrepopulateClassReg,
// emitted into j2x_rtti.cpp and called at boot before threads start, keeps it read-only in practice.)
inline CRITICAL_SECTION* _j2xClassRegCS() {
	static CRITICAL_SECTION cs;
	static volatile long init = 0;
	if (init != 2) {
		if (InterlockedCompareExchange(&init, 1, 0) == 0) { InitializeCriticalSection(&cs); init = 2; }
		else { while (init != 2) Sleep(0); }
	}
	return &cs;
}

class Class {
	const std::type_info* _ti;
public:
	Class() : _ti(0) {}
	Class(const std::type_info& ti) : _ti(&ti) {}
	std::string getName() const {
		if (!_ti) return std::string();
		std::string n(_ti->name());
		// MSVC type_info::name() returns "class Foo" / "struct Foo"; Java getName() is the BARE class
		// name, and the class-keyed render registries (RenderManager / TileEntityRenderer) key by bare-
		// name strings ("EntityPig"). Strip the C++ keyword prefix (+ any namespace) so lookups MATCH.
		if      (n.compare(0,6,"class ")  == 0) n.erase(0,6);
		else if (n.compare(0,7,"struct ") == 0) n.erase(0,7);
		else if (n.compare(0,5,"enum ")   == 0) n.erase(0,5);
		std::string::size_type cc = n.rfind("::");
		if (cc != std::string::npos) n.erase(0, cc + 2);
		return n;
	}
	std::string getSimpleName() const { return getName(); }
	std::string getCanonicalName() const { return getName(); }
	std::string toString() const { return std::string("class ") + getName(); }
	bool equals(const Class& o) const { return *this == o; }
	bool operator==(const Class& o) const { return _ti == o._ti || (_ti && o._ti && *_ti == *o._ti); }
	bool operator!=(const Class& o) const { return !(*this == o); }
	// A Java `.class` literal maps to a STABLE Class* (j2xClassOf, below); `getClass()` stays a Class
	// value. These let `getClass() == Foo.class` (value == Class*) compare by TYPE, not pointer.
	bool operator==(const Class* o) const { return o && *this == *o; }
	bool operator!=(const Class* o) const { return !(o && *this == *o); }
	const std::type_info* rawType() const { return _ti; }
	bool operator<(const Class& o) const {                 // strict-weak order for std::map keys
		if (!_ti) return o._ti != 0;
		if (!o._ti) return false;
		return _ti->before(*o._ti) != 0;
	}
	int hashCode() const {
		std::string n = getName(); int h = 0;
		for (std::string::size_type i = 0; i < n.size(); ++i) h = h * 31 + (unsigned char)n[i];
		return h;
	}
	bool isInstance(...) const { return false; }
	// FIXED BUG isassignablefrom-stub-mob-explosion: was `return false`, so World::countEntities() counted
	// ZERO of EVERY creature type -> the SpawnerAnimals mob cap `count <= maxNumberOfCreature*chunks/256`
	// was ALWAYS satisfied -> unbounded mob spawning (thousands of entities, 2 fps, heap pressure). Now a
	// real Java IS-A via the ancestry registry (which includes INTERFACES, e.g. EntityZombie -> IMob):
	// `A.isAssignableFrom(B)` is true iff B is-a A. j2xClassOf() passes a Class* here.
	bool isAssignableFrom(const Class* o) const {
		return _ti && o && o->_ti && _j2xIsA(o->_ti->name(), _ti->name());
	}
	bool isAssignableFrom(const Class& o) const {
		return _ti && o._ti && _j2xIsA(o._ti->name(), _ti->name());
	}
	bool isAssignableFrom(...) const { return false; }   // fallback for any other call shape
	// FIXED BUG getsuperclass-stub-no-firstperson-hand: was `return 0`, so RenderManager::
	// getEntityClassRenderObject could NOT walk up the class hierarchy -> getEntityRenderObject(thePlayer)
	// (an EntityPlayerSP; RenderPlayer is registered for EntityPlayer) returned NULL -> the first-person
	// hand/arm (ItemRenderer: `if(var15 != NULL) drawFirstPersonHand()`) was silently skipped, and any
	// entity whose exact class isn't directly registered fell back to no/served-wrong renderer. Real
	// superclass via the ancestry registry: _j2xBases()[me][0] is the EXTENDS parent (pipeline emits the
	// parent edge before interfaces), resolved back to a Class* through _j2xClassReg().
	Class* getSuperclass() {
		if (!_ti) return 0;
		std::map<std::string, std::vector<std::string> >::iterator it = _j2xBases().find(_ti->name());
		if (it == _j2xBases().end() || it->second.empty()) return 0;
		const std::string& sup = it->second[0];                 // [0] = extends parent (interfaces follow)
		CRITICAL_SECTION* _cs = _j2xClassRegCS(); EnterCriticalSection(_cs);
		std::vector<Class*>& reg = _j2xClassReg();
		for (std::size_t i = 0; i < reg.size(); ++i) {
			const std::type_info* rt = reg[i]->rawType();
			if (rt && sup == rt->name()) { LeaveCriticalSection(_cs); return reg[i]; }
		}
		LeaveCriticalSection(_cs);
		return 0;                                                // superclass not materialised as a Class yet
	}
	Class getConstructor(...) { return *this; }   // dead reflection: chain `.newInstance()` compiles; returns NULL
	void* newInstance(...) { return 0; }
	void* getDeclaredFields(...) { return 0; }
	// Real resource resolution (was `return 0`, which made ImageIO::read(getResource("/x.png")) return
	// a NULL image -> the loader derefed NULL -> crash). Returns a URL carrying the path; ImageIO::
	// read(URL*) resolves it via game:\Content\... The variadic fallback keeps other call shapes.
	URL* getResource(const std::string& name) { return new URL(name); }
	URL* getResource(...) { return 0; }
	// Real resource resolution (was `return 0`, which made every RenderEngine texture load NULL ->
	// readTextureImage crashed on NULL->close()). Routes "/terrain.png","/title/mojang.png",... to
	// game:\Content\... via FileIO. const std::string& accepts JavaString (extends std::string) and
	// const char* literals; the variadic fallback keeps any other call shape compiling.
	InputStream* getResourceAsStream(const std::string& name) { return J2X_getResourceAsStream(name.c_str()); }
	InputStream* getResourceAsStream(...) { return 0; }
	operator Class*() { return this; }                     // usable where a Class* is expected
};

// A Java `.class` literal must be a STABLE, non-dangling Class object (Java Class objects are
// process-lifetime singletons). j2xClassOf returns a Class* into a program-lifetime registry keyed
// by std::type_info, so the SAME type always yields the SAME pointer -- `EntityList::addMapping(
// EntityPig.class, ...)` stores a valid pointer (NOT the address of a destroyed temporary, the old
// `Class(typeid(X))` dangling bug) and `a.class == b.class` is correct pointer identity.
inline Class* j2xClassOf(const std::type_info& ti) {
	CRITICAL_SECTION* _cs = _j2xClassRegCS(); EnterCriticalSection(_cs);
)CBJRT"
R"CBJRT(	std::vector<Class*>& reg = _j2xClassReg();             // shared registry (see getSuperclass); leaked = program-lifetime
	for (unsigned i = 0; i < reg.size(); ++i) {
		const std::type_info* rt = reg[i]->rawType();
		if (rt && *rt == ti) { LeaveCriticalSection(_cs); return reg[i]; }
	}
	Class* c = new Class(ti); reg.push_back(c); LeaveCriticalSection(_cs); return c;
}
// free cross-comparisons so `Foo.class == obj->getClass()` (Class* == Class value) also works
inline bool operator==(const Class* a, const Class& b) { return a && *a == b; }
inline bool operator!=(const Class* a, const Class& b) { return !(a && *a == b); }
#endif // CLASS_H
)CBJRT");

const py::str _T_Inflater_h(
R"CBJRT(#ifndef INFLATER_H
#define INFLATER_H
#include "JArray.h"
#include "Inflate.h"

// REAL java.util.zip.Inflater -- zlib (RFC-1950) / raw DEFLATE decompress via the bundled puff.c-based
// Inflate.cpp (the 360 SDK ships no zlib; XMemDecompress is LZX, a DIFFERENT format). setInput()/inflate()
// are what Packet51MapChunk feeds SMP terrain through. new Inflater() = zlib-wrapped; new Inflater(true) = raw.
class Inflater {
	const unsigned char* _in; int _inLen; bool _nowrap; int _lastOut; bool _done;
public:
	Inflater() : _in(0), _inLen(0), _nowrap(false), _lastOut(0), _done(false) {}
	Inflater(bool nowrap) : _in(0), _inLen(0), _nowrap(nowrap), _lastOut(0), _done(false) {}
	virtual ~Inflater() {}
	// BY-REFERENCE (JArray convention): a by-value JArray could deep-copy -> dangling _in / writes to a throwaway.
	virtual void setInput(JArray<unsigned char>& b) { _in = b.data(); _inLen = b.length; _done = false; }
	virtual void setInput(JArray<unsigned char>& b, int off, int len) { _in = b.data() + off; _inLen = len; _done = false; }
	virtual int inflate(JArray<unsigned char>& out) {
		if (!_in || _inLen <= 0 || out.length <= 0) { _lastOut = 0; _done = true; return 0; }
		int n = _nowrap ? rawInflate(_in, _inLen, out.data(), out.length)
		                : zlibInflate(_in, _inLen, out.data(), out.length);
		_lastOut = (n < 0) ? 0 : n; _done = true; return _lastOut;
	}
	virtual void end() {}
	virtual void reset() { _in = 0; _inLen = 0; _lastOut = 0; _done = false; }
	virtual bool finished() { return _done; }
	virtual bool needsInput() { return _in == 0; }
	virtual int getRemaining() { return 0; }
	virtual int getTotalOut() { return _lastOut; }
};
#endif // INFLATER_H
)CBJRT");

const py::str _T_SoftReference_h(
R"CBJRT(#ifndef SOFTREFERENCE_H
#define SOFTREFERENCE_H
#include <string>
#include "JavaString.h"
#include "Reference.h"
class SoftReference : public Reference {
	void* _referent;
public:
	SoftReference() : _referent(0) {}
	template<class T> SoftReference(T* r) : _referent((void*)r) {}
	template<class T, class Q> SoftReference(T* r, Q*) : _referent((void*)r) {}
	virtual ~SoftReference() {}
	virtual void* get(...) { return _referent; }   // strong ref: hand back the stored object
};
#endif
)CBJRT");

const py::str _T_InetAddress_h(
R"CBJRT(#ifndef INETADDRESS_H
#define INETADDRESS_H
#include <string>
#include "JavaString.h"

class InetAddress {
	std::string _host;
public:
	InetAddress() {}
	InetAddress(const JavaString& h) : _host(h.c_str()) {}
	virtual ~InetAddress() {}
	virtual JavaString getHostName(...) { return JavaString(_host); }
	virtual JavaString getHostAddress(...) { return JavaString(_host); }
	static InetAddress* getByName(const JavaString& h) { return new InetAddress(h); }
	static InetAddress* getLocalHost(...) { return new InetAddress(); }
};
#endif
)CBJRT");


}  // namespace

py::str _load_canonical(const py::str& header) {
    py::file f = py::open(py::os::path::join(_CANONICAL_DIR(), header), py::str("r"), py::str("utf-8"));
    py::str text = f.read_text();
    f.close();
    return text;
}

const py::str& RANDOM_H() { static const py::str v = _load_canonical(py::str("Random.h")); return v; }
const py::str& RUNNABLE_H() { static const py::str v = _load_canonical(py::str("Runnable.h")); return v; }
const py::str& THREAD_H() { static const py::str v = _load_canonical(py::str("Thread.h")); return v; }
const py::str& COMPARATOR_H() { static const py::str v = _load_canonical(py::str("Comparator.h")); return v; }
const py::str& FILENAMEFILTER_H() { static const py::str v = _load_canonical(py::str("FilenameFilter.h")); return v; }
const py::str& CANVAS_H() { static const py::str v = _load_canonical(py::str("Canvas.h")); return v; }
const py::str& APPLET_H() { static const py::str v = _load_canonical(py::str("Applet.h")); return v; }
const py::str& JAVASTRING_H() { static const py::str v = _load_canonical(py::str("JavaString.h")); return v; }
const py::str& JARRAY_H() { static const py::str v = _load_canonical(py::str("JArray.h")); return v; }
const py::str& JITERATOR_H() { static const py::str v = _load_canonical(py::str("JIterator.h")); return v; }
const py::str& JLIST_H() { static const py::str v = _load_canonical(py::str("JList.h")); return v; }
const py::str& JSET_H() { static const py::str v = _load_canonical(py::str("JSet.h")); return v; }
const py::str& JMAP_H() { static const py::str v = _load_canonical(py::str("JMap.h")); return v; }
const py::str& JAVART_H() { static const py::str v = _load_canonical(py::str("JavaRT.h")); return v; }
const py::str& COLLECTIONS_H() { static const py::str v = _load_canonical(py::str("Collections.h")); return v; }

py::str _guard(const py::str& name) { return name.upper().replace(py::str("."), py::str("_")) + py::str("_H"); }

const py::dict<py::str, py::str>& _EXT_PKG() {
    static const py::dict<py::str, py::str> v = {
        {py::str("GL11"), py::str("org.lwjgl.opengl")},
        {py::str("GL12"), py::str("org.lwjgl.opengl")},
        {py::str("GL15"), py::str("org.lwjgl.opengl")},
        {py::str("NVFogDistance"), py::str("org.lwjgl.opengl")},
        {py::str("GLU"), py::str("org.lwjgl.util.glu")},
        {py::str("GLContext"), py::str("org.lwjgl.opengl")},
        {py::str("ARBOcclusionQuery"), py::str("org.lwjgl.opengl")},
        {py::str("ARBVertexBufferObject"), py::str("org.lwjgl.opengl")},
        {py::str("Keyboard"), py::str("org.lwjgl.input")},
        {py::str("Mouse"), py::str("org.lwjgl.input")},
        {py::str("Controllers"), py::str("org.lwjgl.input")},
        {py::str("Display"), py::str("org.lwjgl.opengl")},
        {py::str("DisplayMode"), py::str("org.lwjgl.opengl")},
        {py::str("BufferUtils"), py::str("org.lwjgl")},
        {py::str("Sys"), py::str("org.lwjgl")},
        {py::str("File"), py::str("java.io")},
        {py::str("InputStream"), py::str("java.io")},
        {py::str("OutputStream"), py::str("java.io")},
        {py::str("DataInput"), py::str("java.io")},
        {py::str("DataOutput"), py::str("java.io")},
        {py::str("DataInputStream"), py::str("java.io")},
        {py::str("DataOutputStream"), py::str("java.io")},
        {py::str("BufferedInputStream"), py::str("java.io")},
        {py::str("BufferedOutputStream"), py::str("java.io")},
        {py::str("FileInputStream"), py::str("java.io")},
        {py::str("FileOutputStream"), py::str("java.io")},
        {py::str("BufferedReader"), py::str("java.io")},
        {py::str("FileReader"), py::str("java.io")},
        {py::str("FileWriter"), py::str("java.io")},
        {py::str("PrintWriter"), py::str("java.io")},
        {py::str("Socket"), py::str("java.net")},
        {py::str("URL"), py::str("java.net")},
        {py::str("URLConnection"), py::str("java.net")},
        {py::str("SoundSystem"), py::str("paulscode.sound")},
        {py::str("CodecJOrbis"), py::str("paulscode.sound.codecs")},
        {py::str("ImageIO"), py::str("javax.imageio")},
        {py::str("BufferedImage"), py::str("java.awt.image")},
        {py::str("ImageObserver"), py::str("java.awt.image")},
        {py::str("ZipFile"), py::str("java.util.zip")},
        {py::str("DecimalFormat"), py::str("java.text")},
        {py::str("DateFormat"), py::str("java.text")},
        {py::str("Class"), py::str("java.lang")},
        {py::str("Comparable"), py::str("java.lang")},
        {py::str("Comparator"), py::str("java.util")},
        {py::str("Canvas"), py::str("java.awt")},
        {py::str("Component"), py::str("java.awt")},
        {py::str("Frame"), py::str("java.awt")},
        {py::str("Panel"), py::str("java.awt")},
        {py::str("Graphics"), py::str("java.awt")},
        {py::str("Color"), py::str("java.awt")},
        {py::str("Cursor"), py::str("java.awt")},
        {py::str("Robot"), py::str("java.awt")},
        {py::str("FileDialog"), py::str("java.awt")},
        {py::str("Dimension"), py::str("java.awt")},
        {py::str("Applet"), py::str("java.applet")},
        {py::str("BorderLayout"), py::str("java.awt")},
        {py::str("GridLayout"), py::str("java.awt")},
        {py::str("FlowLayout"), py::str("java.awt")},
        {py::str("CardLayout"), py::str("java.awt")},
        {py::str("GridBagLayout"), py::str("java.awt")},
        {py::str("Point"), py::str("java.awt")},
        {py::str("MouseInfo"), py::str("java.awt")},
        {py::str("PointerInfo"), py::str("java.awt")},
        {py::str("Dialog"), py::str("java.awt")},
        {py::str("FloatBuffer"), py::str("java.nio")},
        {py::str("IntBuffer"), py::str("java.nio")},
        {py::str("ByteBuffer"), py::str("java.nio")}
    };
    return v;
}

const py::list<py::str>& _AWT_VOID() {
    static const py::list<py::str> v = {py::str("setFocusable"), py::str("setSize"), py::str("repaint"), py::str("requestFocus"), py::str("setVisible"), py::str("addNotify"), py::str("removeNotify"), py::str("setPreferredSize"), py::str("setMinimumSize"), py::str("setMaximumSize"), py::str("enableInputMethods"), py::str("setBackground"), py::str("setForeground"), py::str("validate"), py::str("setLayout"), py::str("add"), py::str("remove"), py::str("removeAll"), py::str("addKeyListener"), py::str("addMouseListener"), py::str("addMouseMotionListener"), py::str("addMouseWheelListener"), py::str("addFocusListener"), py::str("addComponentListener"), py::str("addWindowListener"), py::str("setFocusTraversalKeysEnabled"), py::str("setCursor"), py::str("transferFocus"), py::str("paint"), py::str("update"), py::str("doLayout"), py::str("invalidate"), py::str("revalidate"), py::str("pack")};
    return v;
}

const py::list<std::tuple<py::str, int64_t>>& _COLOR_RGB() {
    static const py::list<std::tuple<py::str, int64_t>> v = {C("BLACK", 4278190080), C("WHITE", 4294967295), C("GRAY", 4286611584), C("LIGHT_GRAY", 4290822336), C("DARK_GRAY", 4282400832), C("RED", 4294901760), C("GREEN", 4278255360), C("BLUE", 4278190335), C("YELLOW", 4294967040), C("CYAN", 4278255615), C("MAGENTA", 4294902015), C("ORANGE", 4294952960), C("PINK", 4294946735), C("black", 4278190080), C("white", 4294967295), C("gray", 4286611584), C("lightGray", 4290822336), C("darkGray", 4282400832), C("red", 4294901760), C("green", 4278255360), C("blue", 4278190335), C("yellow", 4294967040), C("cyan", 4278255615), C("magenta", 4294902015), C("orange", 4294952960), C("pink", 4294946735)};
    return v;
}

const py::dict<py::str, py::str>& _LWJGL_KEY_NAMES() {
    static const py::dict<py::str, py::str> v = [] {
        py::dict<py::str, py::str> d;
        for (const py::str& kv : py::split(py::str("0=NONE 1=ESCAPE 2=1 3=2 4=3 5=4 6=5 7=6 8=7 9=8 10=9 11=0 12=MINUS 13=EQUALS 14=BACK 15=TAB 16=Q 17=W 18=E 19=R 20=T 21=Y 22=U 23=I 24=O 25=P 26=LBRACKET 27=RBRACKET 28=RETURN 29=LCONTROL 30=A 31=S 32=D 33=F 34=G 35=H 36=J 37=K 38=L 39=SEMICOLON 40=APOSTROPHE 41=GRAVE 42=LSHIFT 43=BACKSLASH 44=Z 45=X 46=C 47=V 48=B 49=N 50=M 51=COMMA 52=PERIOD 53=SLASH 54=RSHIFT 55=MULTIPLY 56=LMENU 57=SPACE 58=CAPITAL 59=F1 60=F2 61=F3 62=F4 63=F5 64=F6 65=F7 66=F8 67=F9 68=F10 69=NUMLOCK 70=SCROLL 71=NUMPAD7 72=NUMPAD8 73=NUMPAD9 74=SUBTRACT 75=NUMPAD4 76=NUMPAD5 77=NUMPAD6 78=ADD 79=NUMPAD1 80=NUMPAD2 81=NUMPAD3 82=NUMPAD0 83=DECIMAL 87=F11 88=F12 100=F13 101=F14 102=F15 103=F16 104=F17 105=F18 112=KANA 113=F19 121=CONVERT 123=NOCONVERT 125=YEN 141=NUMPADEQUALS 144=CIRCUMFLEX 145=AT 146=COLON 147=UNDERLINE 148=KANJI 149=STOP 150=AX 151=UNLABELED 156=NUMPADENTER 157=RCONTROL 167=SECTION 179=NUMPADCOMMA 181=DIVIDE 183=SYSRQ 184=RMENU 196=FUNCTION 197=PAUSE 199=HOME 200=UP 201=PRIOR 203=LEFT 205=RIGHT 207=END 208=DOWN 209=NEXT 210=INSERT 211=DELETE 218=CLEAR 219=LMETA 220=RMETA 221=APPS 222=POWER 223=SLEEP"))) {
            py::list<py::str> two = py::split(kv, py::str("="), 1);   // kv.split('=', 1)
            d.set(two[0], two[1]);
        }
        return d;
    }();
    return v;
}

py::list<py::str> _ext_banner(const py::str& name) {
    // lazy import of platform_equivalents in the Python (it has no deps -> no cycle)
    std::optional<py::str> pkg = _EXT_PKG().get(name);
    py::str who = (pkg && !pkg->empty()) ? (*pkg + py::str(".") + name) : (py::str("the external Java type ") + name);
    py::list<py::str> banner{
        py::str(""),
        py::str("// ============================ EXTERNAL-LIBRARY STUB ============================"),
        py::str("// Stands in for ") + who + py::str("."),
        py::str("// This is NOT part of the decompiled Minecraft source -- it is an external Java"),
        py::str("// library. Symbol-only: methods are no-ops / return defaults so the converted game"),
        py::str("// code compiles and links; the real behaviour is the Xbox 360 platform layer"),
        py::str("// (Direct3D 9 / XInput / file IO / sound).")};
    // [360] equivalent: name the exact XDK API to resolve this stub to (from platform_equivalents.py) so nobody
    // hand-guesses the port. Absent line = we have no mapping yet for this symbol (add it to the table).
    py::str ann = console::platform_equivalents::annotation(name);
    if (!ann.empty()) {
        banner.append(py::str("// ------------------------------------------------------------------------------"));
        banner.append(ann);
    }
    banner.append(py::str("// =============================================================================="));
    return banner;
}

py::str _ns_stub(const py::str& name, const ConstList& consts, const StrList& void_fns, const RetList& ret_fns,
                 const StrList& includes, const StrList& fwds) {
    py::str g = _guard(name);
    StrList L{py::str("#ifndef ") + g, py::str("#define ") + g};
    L.extend(_ext_banner(name));
    L.append(py::str("#include <string>"));
    for (const py::str& h : includes) L.append(py::str("#include \"") + h + py::str(".h\""));
    for (const py::str& f : fwds) L.append(py::str("class ") + f + py::str(";"));
    L.append(py::str(""));
    L.append(py::str("namespace ") + name + py::str(" {"));
    for (const Const& kv : consts) {
        L.append(py::str("\tconst int ") + std::get<0>(kv) + py::str(" = ") + py::to_str(std::get<1>(kv)) + py::str(";"));
    }
    // every stub method carries a greppable J2X-STUB marker so a scan (`grep -rn J2X-STUB`) enumerates
    // the ENTIRE placeholder surface -- a no-op that turns out to be load-bearing (the glPolygonOffset
    // z-fight class) is never left silently undone.
    for (const py::str& f : void_fns) {
        L.append(py::str("\tinline void ") + f + py::str("(...) {}   // J2X-STUB[") + name +
                 py::str("]: no-op placeholder -- verify not load-bearing"));
    }
    for (const Ret& fr : ret_fns) {
        const py::str& f = std::get<0>(fr);
        const py::str& rt = std::get<0>(std::get<1>(fr));
        const py::str& rv = std::get<1>(std::get<1>(fr));
        L.append(py::str("\tinline ") + rt + py::str(" ") + f + py::str("(...) { return ") + rv +
                 py::str("; }   // J2X-STUB[") + name + py::str("]: returns default -- verify"));
    }
    L.append(py::str("}"));
    L.append(py::str("#endif // ") + g);
    L.append(py::str(""));
    return py::join(py::str("\n"), L);
}

py::dict<py::str, StubMembers>& STUB_MEMBERS() {
    ensure_built();
    return store().stub_members;
}

const py::re::Pattern& _EXTRA_PTR_METHOD() {
    // `Type* method(` / `static Type* method(` inside an `extra` line (static factories & pointer getters
    // defined verbatim rather than via ret_methods, e.g. Runtime::getRuntime, Toolkit::getDefaultToolkit).
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT((?:static\s+)?([A-Za-z_]\w*)\s*\*\s*(\w+)\s*\()PAT"));
    return p;
}

py::str _class_stub(const py::str& name, const StrList& void_methods, const RetList& ret_methods, bool ctors,
                    const StrList& bases, const StrList& includes, const StrList& fwds, const StrList& extra) {
    ensure_built();
    StubMembers rec;
    if (!bases.empty()) rec.parent = bases[0];
    for (const Ret& mr : ret_methods) rec.methods.set(std::get<0>(mr), std::get<0>(std::get<1>(mr)));
    for (const py::str& line : extra) {
        std::optional<py::re::Match> pm = _EXTRA_PTR_METHOD().search(line);
        if (pm && pm->group(1) != "void") rec.methods.set(pm->group(2), pm->group(1) + py::str("*"));
    }
    store().stub_members.set(name, rec);              // last def wins (mirrors _GENERATED overwrite)
    py::str g = _guard(name);
    StrList L{py::str("#ifndef ") + g, py::str("#define ") + g};
    L.extend(_ext_banner(name));
    L.append(py::str("#include <string>"));
    L.append(py::str("#include \"JavaString.h\""));
    for (const py::str& h : includes) L.append(py::str("#include \"") + h + py::str(".h\""));
    for (const py::str& f : fwds) L.append(py::str("class ") + f + py::str(";"));
    py::str decl = py::str("class ") + name;
    if (!bases.empty()) {
        StrList parts;
        for (const py::str& b : bases) parts.append(py::str("public ") + b);
        decl = decl + py::str(" : ") + py::join(py::str(", "), parts);
    }
    L.append(py::str(""));
    L.append(decl + py::str(" {"));
    L.append(py::str("public:"));
    if (ctors) L.append(py::str("\t") + name + py::str("(...) {}"));
    L.append(py::str("\tvirtual ~") + name + py::str("() {}"));
    // J2X-STUB marker on every stub method -> `grep -rn J2X-STUB` enumerates the full placeholder surface.
    for (const py::str& m : void_methods) {
        L.append(py::str("\tvirtual void ") + m + py::str("(...) {}   // J2X-STUB[") + name +
                 py::str("]: no-op placeholder -- verify not load-bearing"));
    }
    for (const Ret& mr : ret_methods) {
        const py::str& m = std::get<0>(mr);
        py::str rt = std::get<0>(std::get<1>(mr));
        py::str rv = std::get<1>(std::get<1>(mr));
        // A method that returns a Java String must return JavaString, not std::string -- else the
        // caller's `.startsWith()/.substring()/.trim()` (Java String methods) fail to compile.
        if (rt == "std::string") {
            rt = py::str("JavaString");
            rv = py::str("JavaString()");
        }
        L.append(py::str("\tvirtual ") + rt + py::str(" ") + m + py::str("(...) { return ") + rv +
                 py::str("; }   // J2X-STUB[") + name + py::str("]: returns default -- verify"));
    }
    for (const py::str& line : extra) L.append(py::str("\t") + line);
    L.append(py::str("};"));
    L.append(py::str("#endif // ") + g);
    L.append(py::str(""));
    return py::join(py::str("\n"), L);
}

py::str _buffer_stub(const py::str& name) {
    py::str g = _guard(name);
    StrList L{py::str("#ifndef ") + g, py::str("#define ") + g};
    L.extend(_ext_banner(name));
    L.extend(StrList{py::str("class FloatBuffer; class IntBuffer; class ByteBuffer;"), py::str(""),
                     py::str("class ") + name + py::str(" {"), py::str("public:"),
                     py::str("\t") + name + py::str("() {}"), py::str("\tvirtual ~") + name + py::str("() {}")});
    for (const char* m : {"clear", "flip", "rewind", "mark"}) {   // no-arg, return self
        L.append(py::str("\t") + name + py::str("& ") + py::str(m) + py::str("() { return *this; }"));
    }
    for (const char* m : {"put", "putInt", "putShort", "putLong", "putFloat", "putDouble", "putChar"}) {
        L.append(py::str("\t") + name + py::str("& ") + py::str(m) + py::str("(...) { return *this; }"));   // variadic, return self
    }
    L.extend(StrList{py::str("\tint get(...) { return 0; }"), py::str("\tint getInt(...) { return 0; }"),
                     py::str("\tshort getShort(...) { return 0; }"), py::str("\tlong long getLong(...) { return 0; }"),
                     py::str("\tfloat getFloat(...) { return 0.0F; }"), py::str("\tdouble getDouble(...) { return 0.0; }"),
                     py::str("\tchar getChar(...) { return 0; }")});
    for (const char* m : {"position", "limit"}) {                  // getter + setter (no clash)
        L.append(py::str("\tint ") + py::str(m) + py::str("() { return 0; }"));
        L.append(py::str("\t") + name + py::str("& ") + py::str(m) + py::str("(int) { return *this; }"));
    }
    L.extend(StrList{
        py::str("\tint remaining() { return 0; }"), py::str("\tint capacity() { return 0; }"),
        py::str("\tFloatBuffer* asFloatBuffer() { return 0; }"), py::str("\tIntBuffer* asIntBuffer() { return 0; }"),
        // ByteBuffer.order(ByteOrder) -> self (ptr; matches STUB_MEMBERS + chains into a ByteBuffer* local)
        py::str("\t") + name + py::str("* order(...) { return this; }"),
        // ByteBuffer.allocateDirect(n)
        py::str("\tstatic ") + name + py::str("* allocateDirect(...) { return new ") + name + py::str("(); }"),
        py::str("\tstatic ") + name + py::str("* allocate(...) { return new ") + name + py::str("(); }"),
        py::str("\toperator void*() { return 0; }"), py::str("};"), py::str("#endif // ") + g, py::str("")});
    return py::join(py::str("\n"), L);
}

namespace {

// _GENERATED['Keyboard.h'] after the getKeyName stub is replaced by the real name tables
// (and JavaString.h is included).
py::str _keyboard_with_names(const py::str& text) {
    std::vector<std::tuple<int64_t, py::str, py::str>> by_code;   // _LWJGL_KEY_NAMES sorted by int(key)
    for (const auto& kv : _LWJGL_KEY_NAMES().items()) {
        by_code.emplace_back(py::int_from_str(std::get<0>(kv)), std::get<0>(kv), std::get<1>(kv));
    }
    std::stable_sort(by_code.begin(), by_code.end(),
                     [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    StrList kn{py::str("\tinline JavaString getKeyName(int key) {"), py::str("\t\tswitch (key) {")};
    for (const auto& e : by_code) {
        kn.append(py::str("\t\tcase ") + std::get<1>(e) + py::str(": return JavaString(\"") + std::get<2>(e) + py::str("\");"));
    }
    kn.append(py::str("\t\t}"));
    kn.append(py::str("\t\treturn JavaString();   // null, as LWJGL"));
    kn.append(py::str("\t}"));
    kn.append(py::str("\tinline int getKeyIndex(const std::string& name) {"));
    for (const auto& e : by_code) {
        kn.append(py::str("\t\tif (name == \"") + std::get<2>(e) + py::str("\") return ") + std::get<1>(e) + py::str(";"));
    }
    kn.append(py::str("\t\treturn 0;   // KEY_NONE, as LWJGL"));
    kn.append(py::str("\t}"));
    return text
        .replace(py::str("\tinline std::string getKeyName(...) { return std::string(); }   // J2X-STUB[Keyboard]: returns default -- verify"),
                 py::join(py::str("\n"), kn))
        .replace(py::str("#include <string>"), py::str("#include <string>\n#include \"JavaString.h\""));
}

// java.awt.Color -- NOT a symbol-only stub: getHSBColor/HSBtoRGB/getRGB are REAL. The Alpha sky color
// is MobSpawnerBase::getSkyColorByTemp() -> Color::getHSBColor(h,s,b).getRGB(); a stub returning
// Color()/0 made the in-world SKY render BLACK. Standard java.awt HSB->RGB algorithm below.
py::str _color_h() {
    // Constants are static members named exactly as Java (Color is in RUNTIME_ENUM_LIKE
    // so `Color.BLACK` -> `Color::BLACK`, no s_ prefix). They are `static const int` (ARGB) -- a real
    // `static Color X;` member would need an out-of-line definition (none, since stubs are header-only)
    // and fail to LINK; an integral constant with an in-class initializer needs no definition, and the
    // variadic Color ctor / setColor accept the int.
    py::str out = _COLOR_PREFIX;
    for (const auto& nv : _COLOR_RGB()) {
        out += py::percent("    static const int %s = (int)0x%08X;\n", std::get<0>(nv), std::get<1>(nv));
    }
    return out + _COLOR_SUFFIX;
}

void build_generated_() {
    py::dict<py::str, py::str> G = store().generated;
    py::dict<py::str, StubMembers> SM = store().stub_members;
    // common java.awt Component/Container surface, shared by Canvas/Panel/Component/Applet stubs
    const StrList _AWT_VOID_ = _AWT_VOID();   // (the common java.awt surface, defined in the header)
    G.set(py::str("Keyboard.h"), _ns_stub(py::str("Keyboard"), ConstList{C("KEY_ESCAPE", 1), C("KEY_1", 2), C("KEY_2", 3), C("KEY_3", 4), C("KEY_4", 5), C("KEY_5", 6), C("KEY_6", 7), C("KEY_7", 8), C("KEY_8", 9), C("KEY_9", 10), C("KEY_0", 11), C("KEY_MINUS", 12), C("KEY_EQUALS", 13), C("KEY_BACK", 14), C("KEY_TAB", 15), C("KEY_Q", 16), C("KEY_W", 17), C("KEY_E", 18), C("KEY_R", 19), C("KEY_T", 20), C("KEY_Y", 21), C("KEY_U", 22), C("KEY_I", 23), C("KEY_O", 24), C("KEY_P", 25), C("KEY_LBRACKET", 26), C("KEY_RBRACKET", 27), C("KEY_RETURN", 28), C("KEY_LCONTROL", 29), C("KEY_A", 30), C("KEY_S", 31), C("KEY_D", 32), C("KEY_F", 33), C("KEY_G", 34), C("KEY_H", 35), C("KEY_J", 36), C("KEY_K", 37), C("KEY_L", 38), C("KEY_SEMICOLON", 39), C("KEY_APOSTROPHE", 40), C("KEY_GRAVE", 41), C("KEY_LSHIFT", 42), C("KEY_BACKSLASH", 43), C("KEY_Z", 44), C("KEY_X", 45), C("KEY_C", 46), C("KEY_V", 47), C("KEY_B", 48), C("KEY_N", 49), C("KEY_M", 50), C("KEY_COMMA", 51), C("KEY_PERIOD", 52), C("KEY_SLASH", 53), C("KEY_RSHIFT", 54), C("KEY_MULTIPLY", 55), C("KEY_LMENU", 56), C("KEY_SPACE", 57), C("KEY_CAPITAL", 58), C("KEY_F1", 59), C("KEY_F2", 60), C("KEY_F3", 61), C("KEY_F4", 62), C("KEY_F5", 63), C("KEY_F6", 64), C("KEY_F7", 65), C("KEY_F8", 66), C("KEY_F9", 67), C("KEY_F10", 68), C("KEY_NUMLOCK", 69), C("KEY_SCROLL", 70), C("KEY_F11", 87), C("KEY_F12", 88), C("KEY_NUMPAD0", 82), C("KEY_NUMPAD1", 79), C("KEY_NUMPAD2", 80), C("KEY_NUMPAD3", 81), C("KEY_NUMPAD4", 75), C("KEY_NUMPAD5", 76), C("KEY_NUMPAD6", 77), C("KEY_NUMPAD7", 71), C("KEY_NUMPAD8", 72), C("KEY_NUMPAD9", 73), C("KEY_ADD", 78), C("KEY_SUBTRACT", 74), C("KEY_DECIMAL", 83), C("KEY_RCONTROL", 157), C("KEY_RMENU", 184), C("KEY_HOME", 199), C("KEY_UP", 200), C("KEY_PRIOR", 201), C("KEY_LEFT", 203), C("KEY_RIGHT", 205), C("KEY_END", 207), C("KEY_DOWN", 208), C("KEY_NEXT", 209), C("KEY_INSERT", 210), C("KEY_DELETE", 211), C("CHAR_NONE", 0), C("KEY_NONE", 0), C("KEY_LMETA", 219), C("KEY_RMETA", 221), C("KEY_LWIN", 219), C("KEY_RWIN", 221), C("KEY_APPS", 221)}, StrList{py::str("create"), py::str("destroy"), py::str("enableRepeatEvents")}, RetList{R("getEventKey", "int", "0"), R("getEventKeyState", "bool", "false"), R("getEventCharacter", "char", "0"), R("isKeyDown", "bool", "false"), R("next", "bool", "false"), R("getKeyName", "std::string", "std::string()")}));
    // LWJGL 2's Keyboard.getKeyName: a key's constant without KEY_ (17 -> "W"), null for an unnamed code --
    // what the options screen prints for each binding. Taken from lwjgl-2.9.3 itself (getKeyName(0..255)).
    // FIXED BUG keyname-stub: it returned "", so every binding read "Forward: ". The 360 replaces Keyboard.h
    // with the Input reference layer, which has its own.
    G.set(py::str("Keyboard.h"), _keyboard_with_names(G.at(py::str("Keyboard.h"))));
    G.set(py::str("Mouse.h"), _ns_stub(py::str("Mouse"), ConstList{}, StrList{py::str("create"), py::str("destroy"), py::str("setNativeCursor"), py::str("setGrabbed"), py::str("setCursorPosition")}, RetList{R("getDX", "int", "0"), R("getDY", "int", "0"), R("getX", "int", "0"), R("getY", "int", "0"), R("getEventButton", "int", "0"), R("getEventButtonState", "bool", "false"), R("getEventDWheel", "int", "0"), R("getEventX", "int", "0"), R("getEventY", "int", "0"), R("isButtonDown", "bool", "false"), R("next", "bool", "false")}));
    G.set(py::str("DisplayMode.h"), _class_stub(py::str("DisplayMode"), StrList{}, RetList{R("getWidth", "int", "0"), R("getHeight", "int", "0"), R("getFrequency", "int", "0"), R("getBitsPerPixel", "int", "0")}));
    G.set(py::str("Display.h"), _ns_stub(py::str("Display"), ConstList{}, StrList{py::str("create"), py::str("destroy"), py::str("setDisplayMode"), py::str("setFullscreen"), py::str("setParent"), py::str("setTitle"), py::str("update"), py::str("swapBuffers"), py::str("setVSyncEnabled"), py::str("setIcon"), py::str("sync"), py::str("setResizable"), py::str("setLocation"), py::str("releaseContext"), py::str("makeCurrent")}, RetList{R("isActive", "bool", "false"), R("isCloseRequested", "bool", "false"), R("isCreated", "bool", "false"), R("isDirty", "bool", "false"), R("getWidth", "int", "0"), R("getHeight", "int", "0"), R("getDisplayMode", "DisplayMode", "DisplayMode()"), R("getDesktopDisplayMode", "DisplayMode", "DisplayMode()"), R("getAvailableDisplayModes", "void*", "0")}, StrList{py::str("DisplayMode")}));
    G.set(py::str("GL15.h"), _ns_stub(py::str("GL15"), ConstList{C("GL_ARRAY_BUFFER", 34962), C("GL_STREAM_DRAW", 35040), C("GL_SAMPLES_PASSED", 35092), C("GL_QUERY_RESULT", 34918), C("GL_QUERY_RESULT_AVAILABLE", 34919), C("GL_QUERY_COUNTER_BITS", 34916)}));
    G.set(py::str("GL12.h"), _ns_stub(py::str("GL12"), ConstList{C("GL_RESCALE_NORMAL", 32826), C("GL_CLAMP_TO_EDGE", 33071), C("GL_SMOOTH_POINT_SIZE_RANGE", 2834)}));
    G.set(py::str("NVFogDistance.h"), _ns_stub(py::str("NVFogDistance"), ConstList{C("GL_FOG_DISTANCE_MODE_NV", 34138), C("GL_EYE_RADIAL_NV", 34139), C("GL_EYE_PLANE_ABSOLUTE_NV", 34140)}));
    G.set(py::str("GLU.h"), _ns_stub(py::str("GLU"), ConstList{}, StrList{py::str("gluPerspective"), py::str("gluLookAt"), py::str("gluProject"), py::str("gluUnProject")}, RetList{R("gluErrorString", "std::string", "std::string()"), R("gluBuild2DMipmaps", "int", "0")}));
    G.set(py::str("ARBOcclusionQuery.h"), _ns_stub(py::str("ARBOcclusionQuery"), ConstList{}, StrList{py::str("glBeginQueryARB"), py::str("glEndQueryARB"), py::str("glGetQueryObjectuARB"), py::str("glGetQueryARB")}, RetList{R("glGenQueriesARB", "int", "0")}));
    G.set(py::str("ARBVertexBufferObject.h"), _ns_stub(py::str("ARBVertexBufferObject"), ConstList{}, StrList{py::str("glBindBufferARB"), py::str("glBufferDataARB"), py::str("glDeleteBuffersARB")}, RetList{R("glGenBuffersARB", "int", "0")}));
    G.set(py::str("BufferUtils.h"), _T_BufferUtils_h);
    G.set(py::str("ImageIO.h"), _ns_stub(py::str("ImageIO"), ConstList{}, StrList{py::str("write")}, RetList{R("read", "BufferedImage*", "0")}, StrList{}, StrList{py::str("BufferedImage")}));
    G.set(py::str("JOptionPane.h"), _ns_stub(py::str("JOptionPane"), ConstList{C("YES_OPTION", 0), C("NO_OPTION", 1), C("CANCEL_OPTION", 2), C("OK_OPTION", 0), C("ERROR_MESSAGE", 0), C("INFORMATION_MESSAGE", 1)}, StrList{py::str("showMessageDialog")}, RetList{R("showConfirmDialog", "int", "2"), R("showInputDialog", "void*", "0")}));
    for (const py::str& _b : StrList{py::str("FloatBuffer"), py::str("IntBuffer"), py::str("ByteBuffer")}) {
        G.set(_b + py::str(".h"), _buffer_stub(_b));
    }
    G.set(py::str("DataInput.h"), _load_canonical(py::str("DataInput.h")));
    G.set(py::str("DataOutput.h"), _load_canonical(py::str("DataOutput.h")));
    G.set(py::str("Regex.h"), _load_canonical(py::str("Regex.h")));
    G.set(py::str("Matcher.h"), _load_canonical(py::str("Matcher.h")));
    G.set(py::str("Pattern.h"), _load_canonical(py::str("Pattern.h")));
    SM.set(py::str("Matcher"), StubMembers{std::nullopt, MethodMap{{py::str("matches"), py::str("bool")}, {py::str("find"), py::str("bool")}, {py::str("lookingAt"), py::str("bool")}, {py::str("group"), py::str("JavaString")}, {py::str("groupCount"), py::str("int")}, {py::str("start"), py::str("int")}, {py::str("end"), py::str("int")}, {py::str("reset"), py::str("Matcher*")}}});
    SM.set(py::str("Pattern"), StubMembers{std::nullopt, MethodMap{{py::str("compile"), py::str("Pattern*")}, {py::str("matcher"), py::str("Matcher*")}, {py::str("matches"), py::str("bool")}, {py::str("pattern"), py::str("JavaString")}, {py::str("split"), py::str("JArray<JavaString>")}}});
    G.set(py::str("FileFilter.h"), _class_stub(py::str("FileFilter"), StrList{}, RetList{R("accept", "bool", "false")}, true, StrList{}, StrList{}, StrList{py::str("File")}));
    G.set(py::str("ByteArrayOutputStream.h"), _load_canonical(py::str("ByteArrayOutputStream.h")));
    G.set(py::str("Reader.h"), _class_stub(py::str("Reader"), StrList{py::str("close")}, RetList{R("read", "int", "-1")}));
    G.set(py::str("InputStreamReader.h"), _load_canonical(py::str("InputStreamReader.h")));
    SM.set(py::str("InputStreamReader"), StubMembers{std::optional<py::str>(py::str("Reader")), MethodMap{}});
    G.set(py::str("HttpURLConnection.h"), _class_stub(py::str("HttpURLConnection"), StrList{py::str("setDoInput"), py::str("setDoOutput"), py::str("connect"), py::str("disconnect"), py::str("setRequestMethod"), py::str("setRequestProperty")}, RetList{R("getResponseCode", "int", "-1"), R("getInputStream", "InputStream*", "0")}, true, StrList{py::str("URLConnection")}, StrList{py::str("URLConnection")}, StrList{py::str("InputStream")}));
    G.set(py::str("ZipEntry.h"), _class_stub(py::str("ZipEntry"), StrList{}, RetList{R("getName", "std::string", "std::string()"), R("getSize", "long long", "0"), R("isDirectory", "bool", "false")}));
    G.set(py::str("NodeList.h"), _class_stub(py::str("NodeList"), StrList{}, RetList{R("getLength", "int", "0"), R("item", "Node*", "0")}, true, StrList{}, StrList{}, StrList{py::str("Node")}));
    G.set(py::str("Node.h"), _class_stub(py::str("Node"), StrList{}, RetList{R("getNodeType", "int", "0"), R("getNodeName", "std::string", "std::string()"), R("getNodeValue", "std::string", "std::string()"), R("getChildNodes", "NodeList*", "0")}, true, StrList{}, StrList{}, StrList{py::str("NodeList")}));
    G.set(py::str("Element.h"), _class_stub(py::str("Element"), StrList{}, RetList{R("getAttribute", "std::string", "std::string()"), R("getElementsByTagName", "NodeList*", "0")}, true, StrList{py::str("Node")}, StrList{py::str("Node")}, StrList{py::str("NodeList")}));
    G.set(py::str("Document.h"), _class_stub(py::str("Document"), StrList{}, RetList{R("getElementsByTagName", "NodeList*", "0"), R("getDocumentElement", "Element*", "0")}, true, StrList{}, StrList{}, StrList{py::str("NodeList"), py::str("Element")}));
    G.set(py::str("DocumentBuilder.h"), _class_stub(py::str("DocumentBuilder"), StrList{}, RetList{R("parse", "Document*", "0")}, true, StrList{}, StrList{}, StrList{py::str("Document")}));
    G.set(py::str("DocumentBuilderFactory.h"), _class_stub(py::str("DocumentBuilderFactory"), StrList{}, RetList{R("newDocumentBuilder", "DocumentBuilder*", "0")}, true, StrList{}, StrList{}, StrList{py::str("DocumentBuilder")}, StrList{py::str("static DocumentBuilderFactory* newInstance(...) { return new DocumentBuilderFactory(); }")}));
    G.set(py::str("Date.h"), _load_canonical(py::str("Date.h")));
    G.set(py::str("SimpleDateFormat.h"), _T_SimpleDateFormat_h);
    G.set(py::str("GZIPInputStream.h"), _load_canonical(py::str("GZIPInputStream.h")));
    G.set(py::str("GZIPOutputStream.h"), _load_canonical(py::str("GZIPOutputStream.h")));
    G.set(py::str("Runtime.h"), _T_Runtime_h);
    G.set(py::str("ByteOrder.h"), _ns_stub(py::str("ByteOrder"), ConstList{C("BIG_ENDIAN", 1), C("LITTLE_ENDIAN", 0)}, StrList{}, RetList{R("nativeOrder", "int", "1")}));
    G.set(py::str("RandomAccessFile.h"), _T_RandomAccessFile_h);
    G.set(py::str("Writer.h"), _class_stub(py::str("Writer"), StrList{py::str("write"), py::str("flush"), py::str("close")}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("virtual void write(const JavaString&) {}"), py::str("virtual void write(int) {}"), py::str("Writer* append(...) { return this; }")}));
    G.set(py::str("StringWriter.h"), _class_stub(py::str("StringWriter"), StrList{}, RetList{R("toString", "std::string", "std::string()"), R("getBuffer", "void*", "0")}, true, StrList{py::str("Writer")}, StrList{py::str("Writer")}));
    G.set(py::str("PushbackReader.h"), _class_stub(py::str("PushbackReader"), StrList{py::str("unread"), py::str("close")}, RetList{R("read", "int", "-1")}));
    G.set(py::str("Stack.h"), _T_Stack_h);
    G.set(py::str("Properties.h"), _load_canonical(py::str("Properties.h")));
    SM.set(py::str("Properties"), StubMembers{std::nullopt, MethodMap{{py::str("getProperty"), py::str("std::string")}}});
    G.set(py::str("Entry.h"), _T_Entry_h);
    G.set(py::str("NumberFormat.h"), _load_canonical(py::str("NumberFormat.h")));
    SM.set(py::str("NumberFormat"), StubMembers{std::nullopt, MethodMap{{py::str("format"), py::str("std::string")}, {py::str("getInstance"), py::str("NumberFormat*")}, {py::str("getIntegerInstance"), py::str("NumberFormat*")}, {py::str("getNumberInstance"), py::str("NumberFormat*")}}});
    G.set(py::str("MessageDigest.h"), _load_canonical(py::str("MessageDigest.h")));
    SM.set(py::str("MessageDigest"), StubMembers{std::nullopt, MethodMap{{py::str("getInstance"), py::str("MessageDigest*")}, {py::str("digest"), py::str("JArray<unsigned char >")}}});
    G.set(py::str("RenderingHints.h"), _class_stub(py::str("RenderingHints"), StrList{py::str("put")}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static const int s_KEY_INTERPOLATION = 0;"), py::str("static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;"), py::str("static const int s_KEY_ANTIALIASING = 0;"), py::str("static const int s_VALUE_ANTIALIAS_OFF = 0;")}));
    G.set(py::str("CodecWav.h"), _class_stub(py::str("CodecWav")));
    G.set(py::str("DataFlavor.h"), _class_stub(py::str("DataFlavor")));
    G.set(py::str("Reference.h"), _class_stub(py::str("Reference"), StrList{}, RetList{R("get", "void*", "0")}));
    G.set(py::str("Logger.h"), _class_stub(py::str("Logger"), StrList{py::str("log"), py::str("info"), py::str("warning"), py::str("severe"), py::str("fine")}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static Logger* getLogger(...) { return new Logger(); }")}));
    G.set(py::str("Calendar.h"), _class_stub(py::str("Calendar"), StrList{py::str("set"), py::str("setTime"), py::str("add")}, RetList{R("get", "int", "0"), R("getTime", "void*", "0"), R("getTimeInMillis", "long long", "0LL")}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static Calendar* getInstance() { return new Calendar(); }"), py::str("static const int JANUARY = 0, FEBRUARY = 1, DECEMBER = 11, DATE = 5, MONTH = 2, YEAR = 1;")}));
    G.set(py::str("GregorianCalendar.h"), _class_stub(py::str("GregorianCalendar"), StrList{}, RetList{}, true, StrList{py::str("Calendar")}, StrList{py::str("Calendar")}));
    G.set(py::str("Clipboard.h"), _class_stub(py::str("Clipboard"), StrList{}, RetList{R("getContents", "Transferable*", "0")}, true, StrList{}, StrList{}, StrList{py::str("Transferable")}));
    G.set(py::str("Toolkit.h"), _class_stub(py::str("Toolkit"), StrList{}, RetList{}, true, StrList{}, StrList{py::str("Clipboard")}, StrList{}, StrList{py::str("static Toolkit* getDefaultToolkit() { return new Toolkit(); }"), py::str("Clipboard getSystemClipboard(...) { return Clipboard(); }")}));
    G.set(py::str("TextArea.h"), _class_stub(py::str("TextArea"), StrList{}, RetList{R("getText", "std::string", "std::string()")}));
    G.set(py::str("Transferable.h"), _class_stub(py::str("Transferable")));
    G.set(py::str("SoundSystemConfig.h"), _ns_stub(py::str("SoundSystemConfig"), ConstList{}, StrList{py::str("setCodec"), py::str("setNumberStreamingChannels"), py::str("addLibrary"), py::str("removeLibrary"), py::str("setSoundFilesPackage")}));
    G.set(py::str("Collection.h"), _class_stub(py::str("Collection"), StrList{py::str("add"), py::str("clear")}, RetList{R("size", "int", "0"), R("isEmpty", "bool", "true"), R("iterator", "void*", "0"), R("contains", "bool", "false")}));
    G.set(py::str("Locale.h"), _class_stub(py::str("Locale"), StrList{}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static const int US = 0, ENGLISH = 0, ROOT = 0;")}));
    G.set(py::str("Charset.h"), _class_stub(py::str("Charset"), StrList{}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static Charset* forName(...) { return new Charset(); }")}));
    G.set(py::str("Rectangle.h"), _class_stub(py::str("Rectangle"), StrList{}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("int x, y, width, height;")}));
    G.set(py::str("RenderingHints.h"), _class_stub(py::str("RenderingHints"), StrList{py::str("put")}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static const int s_KEY_INTERPOLATION = 0;"), py::str("static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;"), py::str("static const int s_KEY_ANTIALIASING = 0;"), py::str("static const int s_VALUE_ANTIALIAS_OFF = 0;")}));
    G.set(py::str("CodecWav.h"), _class_stub(py::str("CodecWav")));
    G.set(py::str("DataFlavor.h"), _class_stub(py::str("DataFlavor"), StrList{}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("static const int s_stringFlavor = 0;")}));
    G.set(py::str("Transferable.h"), _class_stub(py::str("Transferable"), StrList{}, RetList{R("isDataFlavorSupported", "bool", "false"), R("getTransferData", "JObject", "JObject()")}));
    G.set(py::str("TextArea.h"), _class_stub(py::str("TextArea"), StrList{py::str("setFont"), py::str("setText")}, RetList{R("getText", "std::string", "std::string()")}));
    G.set(py::str("InputStream.h"), _T_InputStream_h);
    G.set(py::str("OutputStream.h"), _load_canonical(py::str("OutputStream.h")));
    G.set(py::str("DataInputStream.h"), _load_canonical(py::str("DataInputStream.h")));
    G.set(py::str("DataOutputStream.h"), _load_canonical(py::str("DataOutputStream.h")));
    G.set(py::str("BufferedOutputStream.h"), _load_canonical(py::str("BufferedOutputStream.h")));
    G.set(py::str("BufferedInputStream.h"), _class_stub(py::str("BufferedInputStream"), StrList{}, RetList{}, true, StrList{py::str("InputStream")}, StrList{py::str("InputStream")}));
    G.set(py::str("FileInputStream.h"), _T_FileInputStream_h);
    G.set(py::str("FileOutputStream.h"), _load_canonical(py::str("FileOutputStream.h")));
    G.set(py::str("ZipFile.h"), _class_stub(py::str("ZipFile"), StrList{py::str("close")}, RetList{R("getEntry", "ZipEntry*", "0"), R("getInputStream", "InputStream*", "0"), R("size", "int", "0")}, true, StrList{}, StrList{}, StrList{py::str("InputStream"), py::str("ZipEntry")}));
    G.set(py::str("SocketAddress.h"), _class_stub(py::str("SocketAddress")));
    G.set(py::str("Socket.h"), _class_stub(py::str("Socket"), StrList{py::str("close"), py::str("setSoTimeout"), py::str("setTrafficClass"), py::str("setTcpNoDelay"), py::str("setKeepAlive"), py::str("connect")}, RetList{R("getInputStream", "InputStream*", "0"), R("getOutputStream", "OutputStream*", "0"), R("getRemoteSocketAddress", "SocketAddress*", "0"), R("getInetAddress", "InetAddress*", "0"), R("getPort", "int", "0"), R("isConnected", "bool", "false"), R("isClosed", "bool", "false")}, true, StrList{}, StrList{}, StrList{py::str("InputStream"), py::str("OutputStream"), py::str("SocketAddress"), py::str("InetAddress")}));
    G.set(py::str("DateFormat.h"), _load_canonical(py::str("DateFormat.h")));
    G.set(py::str("Dimension.h"), _class_stub(py::str("Dimension"), StrList{}, RetList{R("getWidth", "int", "0"), R("getHeight", "int", "0")}));
    for (const py::str& _li : StrList{py::str("KeyListener"), py::str("MouseListener"), py::str("MouseMotionListener"), py::str("MouseWheelListener"), py::str("WindowListener"), py::str("ComponentListener"), py::str("FocusListener")}) {
        G.set(_li + py::str(".h"), _class_stub(_li, StrList{py::str("keyPressed"), py::str("keyReleased"), py::str("keyTyped"), py::str("mouseClicked"), py::str("mousePressed"), py::str("mouseReleased"), py::str("mouseEntered"), py::str("mouseExited"), py::str("mouseMoved"), py::str("mouseDragged"), py::str("mouseWheelMoved"), py::str("windowClosing"), py::str("windowOpened"), py::str("windowClosed"), py::str("windowActivated"), py::str("windowDeactivated"), py::str("windowIconified"), py::str("windowDeiconified"), py::str("componentResized"), py::str("componentMoved"), py::str("componentShown"), py::str("componentHidden"), py::str("focusGained"), py::str("focusLost")}));
    }
    G.set(py::str("WindowAdapter.h"), _class_stub(py::str("WindowAdapter"), StrList{py::str("windowClosing"), py::str("windowOpened"), py::str("windowClosed"), py::str("windowActivated"), py::str("windowDeactivated"), py::str("windowIconified"), py::str("windowDeiconified")}));
    for (const py::str& _ev : StrList{py::str("KeyEvent"), py::str("MouseEvent"), py::str("WindowEvent"), py::str("ComponentEvent"), py::str("FocusEvent"), py::str("InputEvent")}) {
        G.set(_ev + py::str(".h"), _class_stub(_ev, StrList{py::str("consume")}, RetList{R("getKeyCode", "int", "0"), R("getKeyChar", "char", "0"), R("getX", "int", "0"), R("getY", "int", "0"), R("getButton", "int", "0"), R("getClickCount", "int", "0"), R("getWheelRotation", "int", "0"), R("getWindow", "void*", "0"), R("getComponent", "void*", "0"), R("isShiftDown", "bool", "false"), R("isControlDown", "bool", "false"), R("isAltDown", "bool", "false")}));
    }
    G.set(py::str("Panel.h"), _class_stub(py::str("Panel"), _AWT_VOID_, RetList{R("getWidth", "int", "J2X_COMPONENT_WIDTH()"), R("getHeight", "int", "J2X_COMPONENT_HEIGHT()"), R("getGraphics", "Graphics*", "0")}, true, StrList{py::str("Component")}, StrList{py::str("Component")}, StrList{py::str("Graphics")}));
    G.set(py::str("Comparable.h"), _class_stub(py::str("Comparable"), StrList{}, RetList{R("compareTo", "int", "0")}));
    for (const py::str& _lm : StrList{py::str("BorderLayout"), py::str("GridLayout"), py::str("FlowLayout"), py::str("CardLayout"), py::str("GridBagLayout")}) {
        G.set(_lm + py::str(".h"), _class_stub(_lm));
    }
    G.set(py::str("Frame.h"), _class_stub(py::str("Frame"), _concat(_AWT_VOID_, StrList{py::str("setTitle"), py::str("setResizable"), py::str("dispose"), py::str("show"), py::str("hide"), py::str("toFront"), py::str("setIconImage"), py::str("setMenuBar"), py::str("setLocationRelativeTo"), py::str("setLocation"), py::str("setUndecorated"), py::str("setState"), py::str("setExtendedState")}), RetList{R("getWidth", "int", "J2X_COMPONENT_WIDTH()"), R("getHeight", "int", "J2X_COMPONENT_HEIGHT()"), R("getGraphics", "Graphics*", "0"), R("getInsets", "void*", "0"), R("getTitle", "std::string", "std::string()")}, true, StrList{py::str("Component")}, StrList{py::str("Component")}, StrList{py::str("Graphics")}));
    G.set(py::str("Color.h"), _color_h());
    G.set(py::str("Controllers.h"), _ns_stub(py::str("Controllers"), ConstList{}, StrList{py::str("create"), py::str("destroy"), py::str("poll"), py::str("clearEvents")}, RetList{R("isCreated", "bool", "false"), R("getControllerCount", "int", "0"), R("getController", "void*", "0"), R("next", "bool", "false"), R("getEventSource", "void*", "0"), R("getEventControlIndex", "int", "0"), R("getEventButtonState", "bool", "false")}));
    G.set(py::str("Sys.h"), _ns_stub(py::str("Sys"), ConstList{}, StrList{py::str("openURL"), py::str("alert")}, RetList{R("getVersion", "std::string", "std::string()"), R("getTime", "long long", "0LL"), R("getTimerResolution", "long long", "1000LL")}));
    G.set(py::str("GLContext.h"), _T_GLContext_h);
    G.set(py::str("URLConnection.h"), _class_stub(py::str("URLConnection"), StrList{py::str("connect"), py::str("setDoInput"), py::str("setDoOutput"), py::str("setUseCaches"), py::str("setRequestProperty"), py::str("setConnectTimeout"), py::str("setReadTimeout"), py::str("disconnect")}, RetList{R("getInputStream", "InputStream*", "0"), R("getOutputStream", "OutputStream*", "0"), R("getContentLength", "int", "0"), R("getResponseCode", "int", "0"), R("getHeaderField", "std::string", "std::string()")}, true, StrList{}, StrList{}, StrList{py::str("InputStream"), py::str("OutputStream")}));
    G.set(py::str("CodecJOrbis.h"), _class_stub(py::str("CodecJOrbis"), StrList{py::str("initialize"), py::str("cleanup"), py::str("setAudioFormat"), py::str("reverseByteOrder")}, RetList{R("read", "void*", "0"), R("readAll", "void*", "0"), R("getAudioFormat", "void*", "0"), R("initialized", "bool", "false"), R("endOfStream", "bool", "false")}, true, StrList{}, StrList{py::str("URL"), py::str("URLConnection")}, StrList{}, StrList{py::str("URL* url;"), py::str("URLConnection urlConnection;")}));
    G.set(py::str("SoundSystem.h"), _class_stub(py::str("SoundSystem"), StrList{py::str("cleanup"), py::str("newSource"), py::str("newStreamingSource"), py::str("play"), py::str("setListenerOrientation"), py::str("setListenerPosition"), py::str("setPitch"), py::str("setVolume"), py::str("stop"), py::str("pause"), py::str("removeSource"), py::str("backgroundMusic"), py::str("quickPlay"), py::str("quickStream"), py::str("setPosition"), py::str("fadeOut"), py::str("fadeOutIn")}, RetList{R("playing", "bool", "false")}));
    G.set(py::str("BufferedReader.h"), _load_canonical(py::str("BufferedReader.h")));
    SM.set(py::str("BufferedReader"), StubMembers{std::nullopt, MethodMap{{py::str("readLine"), py::str("std::string")}}});
    G.set(py::str("FileReader.h"), _load_canonical(py::str("FileReader.h")));
    SM.set(py::str("FileReader"), StubMembers{std::optional<py::str>(py::str("Reader")), MethodMap{}});
    G.set(py::str("FileWriter.h"), _load_canonical(py::str("FileWriter.h")));
    SM.set(py::str("FileWriter"), StubMembers{std::optional<py::str>(py::str("Writer")), MethodMap{}});
    G.set(py::str("PrintWriter.h"), _load_canonical(py::str("PrintWriter.h")));
    G.set(py::str("Graphics.h"), _class_stub(py::str("Graphics"), StrList{py::str("drawImage"), py::str("dispose"), py::str("setColor"), py::str("fillRect"), py::str("drawString")}));
    G.set(py::str("Graphics.h"), _replace_first(G.at(py::str("Graphics.h")), py::str("};\n#endif"), py::str("};\ninline Graphics* j2xSharedGraphics() { static Graphics _g; return &_g; }\n#endif")));
    G.set(py::str("Cursor.h"), _class_stub(py::str("Cursor")));
    G.set(py::str("Class.h"), _T_Class_h);
    G.set(py::str("Robot.h"), _class_stub(py::str("Robot"), StrList{py::str("mouseMove")}));
    G.set(py::str("URL.h"), _class_stub(py::str("URL"), StrList{}, RetList{R("openConnection", "URLConnection*", "J2X_URL_OPEN_CONNECTION(this)"), R("openStream", "InputStream*", "J2X_URL_OPEN_STREAM(this)"), R("toString", "std::string", "std::string()"), R("getPath", "std::string", "std::string()"), R("getFile", "std::string", "std::string()"), R("getProtocol", "std::string", "std::string()"), R("getHost", "std::string", "std::string()"), R("getQuery", "std::string", "std::string()"), R("getPort", "int", "-1"), R("getDefaultPort", "int", "-1"), R("toURI", "void*", "0")}, false, StrList{}, StrList{}, StrList{py::str("InputStream"), py::str("URLConnection")}, StrList{py::str("std::string _j2xpath;"), py::str("explicit URL(...) {}   // explicit: a string reaches URL only through the path ctor below"), py::str("URL(const std::string& p) : _j2xpath(p) {}"), py::str("const std::string& j2xResourcePath() const { return _j2xpath; }")}));
    G.set(py::str("FileDialog.h"), _class_stub(py::str("FileDialog"), StrList{py::str("setVisible"), py::str("setMode")}, RetList{R("getFile", "std::string", "std::string()"), R("getDirectory", "std::string", "std::string()")}));
    G.set(py::str("DecimalFormat.h"), _load_canonical(py::str("DecimalFormat.h")));
    SM.set(py::str("DecimalFormat"), StubMembers{std::optional<py::str>(py::str("NumberFormat")), MethodMap{{py::str("format"), py::str("std::string")}}});
    G.set(py::str("ImageObserver.h"), _class_stub(py::str("ImageObserver")));
    G.set(py::str("Inflater.h"), _T_Inflater_h);
    G.set(py::str("Deflater.h"), _class_stub(py::str("Deflater"), StrList{py::str("setInput"), py::str("finish"), py::str("end"), py::str("setLevel"), py::str("reset")}, RetList{R("deflate", "int", "0"), R("finished", "bool", "false"), R("getTotalOut", "int", "0")}));
    G.set(py::str("InflaterInputStream.h"), _load_canonical(py::str("InflaterInputStream.h")));
    G.set(py::str("DeflaterOutputStream.h"), _load_canonical(py::str("DeflaterOutputStream.h")));
    G.set(py::str("ByteArrayInputStream.h"), _load_canonical(py::str("ByteArrayInputStream.h")));
    G.set(py::str("DataFormatException.h"), _class_stub(py::str("DataFormatException"), StrList{}, RetList{}, true, StrList{py::str("Exception")}, StrList{py::str("Exception")}));
    G.set(py::str("NoSuchAlgorithmException.h"), _class_stub(py::str("NoSuchAlgorithmException"), StrList{}, RetList{}, true, StrList{py::str("Exception")}, StrList{py::str("Exception")}));
    G.set(py::str("BigInteger.h"), _load_canonical(py::str("BigInteger.h")));
    SM.set(py::str("BigInteger"), StubMembers{std::nullopt, MethodMap{{py::str("toString"), py::str("std::string")}, {py::str("abs"), py::str("BigInteger*")}, {py::str("signum"), py::str("int")}}});
    G.set(py::str("SoftReference.h"), _T_SoftReference_h);
    G.set(py::str("InetAddress.h"), _T_InetAddress_h);
    G.set(py::str("StringReader.h"), _class_stub(py::str("StringReader"), StrList{}, RetList{}, true, StrList{py::str("InputStreamReader")}, StrList{py::str("InputStreamReader")}));
    G.set(py::str("Font.h"), _class_stub(py::str("Font"), StrList{}, RetList{R("getSize", "int", "0"), R("deriveFont", "Font*", "0")}));
    G.set(py::str("FontMetrics.h"), _class_stub(py::str("FontMetrics"), StrList{}, RetList{R("stringWidth", "int", "0"), R("getHeight", "int", "0"), R("charWidth", "int", "0")}));
    G.set(py::str("AffineTransform.h"), _class_stub(py::str("AffineTransform")));
    G.set(py::str("DataBufferInt.h"), _class_stub(py::str("DataBufferInt"), StrList{}, RetList{R("getData", "void*", "0")}));
    G.set(py::str("Graphics2D.h"), _class_stub(py::str("Graphics2D"), StrList{py::str("setClip"), py::str("setRenderingHint"), py::str("translate"), py::str("scale"), py::str("setColor"), py::str("fillRect"), py::str("drawImage"), py::str("setTransform"), py::str("drawString"), py::str("dispose"), py::str("rotate"), py::str("shear")}, RetList{R("getTransform", "AffineTransform*", "0"), R("getClipBounds", "Rectangle*", "0"), R("getFontMetrics", "FontMetrics*", "0")}, true, StrList{py::str("Graphics")}, StrList{py::str("Graphics"), py::str("FontMetrics")}, StrList{py::str("AffineTransform"), py::str("Rectangle")}));
    G.set(py::str("BufferStrategy.h"), _class_stub(py::str("BufferStrategy"), StrList{py::str("show")}, RetList{R("getDrawGraphics", "Graphics*", "0")}, true, StrList{}, StrList{py::str("Graphics")}, StrList{py::str("Graphics")}));
    G.set(py::str("LibraryLWJGLOpenAL.h"), _class_stub(py::str("LibraryLWJGLOpenAL")));
    const RetList _EXC_METHODS = RetList{R("getMessage", "std::string", "std::string()"), R("getLocalizedMessage", "std::string", "std::string()"), R("toString", "std::string", "std::string()")};
    G.set(py::str("Throwable.h"), _class_stub(py::str("Throwable"), StrList{py::str("printStackTrace")}, _EXC_METHODS));
    for (const std::tuple<py::str, py::str>& _it : std::vector<std::tuple<py::str, py::str>>{{py::str("Exception"), py::str("Throwable")}, {py::str("Error"), py::str("Throwable")}, {py::str("RuntimeException"), py::str("Exception")}, {py::str("IOException"), py::str("Exception")}, {py::str("InterruptedException"), py::str("Exception")}, {py::str("IllegalStateException"), py::str("RuntimeException")}, {py::str("IllegalArgumentException"), py::str("RuntimeException")}, {py::str("MalformedURLException"), py::str("IOException")}, {py::str("AWTException"), py::str("Exception")}, {py::str("LWJGLException"), py::str("Exception")}, {py::str("NoSuchFieldError"), py::str("Error")}, {py::str("FileNotFoundException"), py::str("IOException")}, {py::str("EOFException"), py::str("IOException")}, {py::str("SocketException"), py::str("IOException")}, {py::str("UnknownHostException"), py::str("IOException")}, {py::str("ConnectException"), py::str("SocketException")}, {py::str("BindException"), py::str("SocketException")}, {py::str("NumberFormatException"), py::str("IllegalArgumentException")}, {py::str("NullPointerException"), py::str("RuntimeException")}, {py::str("ArrayIndexOutOfBoundsException"), py::str("RuntimeException")}, {py::str("IndexOutOfBoundsException"), py::str("RuntimeException")}, {py::str("ClassCastException"), py::str("RuntimeException")}, {py::str("NoSuchElementException"), py::str("RuntimeException")}, {py::str("UnsupportedOperationException"), py::str("RuntimeException")}, {py::str("ConcurrentModificationException"), py::str("RuntimeException")}, {py::str("OutOfMemoryError"), py::str("Error")}, {py::str("NoSuchMethodException"), py::str("Exception")}, {py::str("CloneNotSupportedException"), py::str("Exception")}, {py::str("ClassNotFoundException"), py::str("Exception")}}) {
        const py::str& _e = std::get<0>(_it);
        const py::str& _base = std::get<1>(_it);
        G.set(_e + py::str(".h"), _class_stub(_e, StrList{}, RetList{}, true, StrList{_base}, StrList{_base}));
    }
    G.set(py::str("URL.h"), _replace_first(G.at(py::str("URL.h")), py::str("#include <string>\n"), py::str("#include <string>\n#ifndef J2X_URL_OPEN_CONNECTION\n#define J2X_URL_OPEN_CONNECTION(u) 0\n#define J2X_URL_OPEN_STREAM(u) 0\n#endif\n")));
    G.set(py::str("URI.h"), _class_stub(py::str("URI"), StrList{}, RetList{}, false, StrList{}, StrList{py::str("URL")}, StrList{}, StrList{py::str("std::string _j2xpath;"), py::str("URI(...) {}"), py::str("URI(const std::string& p) : _j2xpath(p) {}"), py::str("URL* toURL(...) { return new URL(_j2xpath); }"), py::str("JavaString getPath(...) { return JavaString(_j2xpath); }"), py::str("JavaString toString(...) { return JavaString(_j2xpath); }")}));
    G.set(py::str("File.h"), _load_canonical(py::str("File.h")));
    SM.set(py::str("File"), StubMembers{std::nullopt, MethodMap{{py::str("getName"), py::str("std::string")}, {py::str("getPath"), py::str("std::string")}, {py::str("getAbsolutePath"), py::str("std::string")}, {py::str("toString"), py::str("std::string")}, {py::str("getParent"), py::str("std::string")}, {py::str("exists"), py::str("bool")}, {py::str("isDirectory"), py::str("bool")}, {py::str("isFile"), py::str("bool")}, {py::str("canRead"), py::str("bool")}, {py::str("canWrite"), py::str("bool")}, {py::str("mkdir"), py::str("bool")}, {py::str("mkdirs"), py::str("bool")}, {py::str("renameTo"), py::str("bool")}, {py::str("delete_"), py::str("bool")}, {py::str("createNewFile"), py::str("bool")}, {py::str("length"), py::str("long long")}, {py::str("lastModified"), py::str("long long")}, {py::str("getParentFile"), py::str("File*")}, {py::str("getAbsoluteFile"), py::str("File*")}, {py::str("toURI"), py::str("URI")}, {py::str("listFiles"), py::str("JArray<File* >")}, {py::str("list"), py::str("void*")}}});
    G.set(py::str("Applet.h"), _class_stub(py::str("Applet"), StrList{py::str("validate"), py::str("setLayout"), py::str("add"), py::str("repaint"), py::str("requestFocus"), py::str("setVisible"), py::str("resize"), py::str("init"), py::str("start"), py::str("stop"), py::str("remove"), py::str("removeAll"), py::str("setSize"), py::str("setBackground"), py::str("setForeground"), py::str("addKeyListener"), py::str("addMouseListener"), py::str("addMouseMotionListener"), py::str("addFocusListener"), py::str("addComponentListener"), py::str("setFocusable"), py::str("setFocusTraversalKeysEnabled"), py::str("enableInputMethods")}, RetList{R("getParameter", "std::string", "std::string()"), R("getDocumentBase", "URL*", "0"), R("getCodeBase", "URL*", "0"), R("getWidth", "int", "J2X_COMPONENT_WIDTH()"), R("getHeight", "int", "J2X_COMPONENT_HEIGHT()"), R("getGraphics", "Graphics*", "0"), R("isActive", "bool", "false"), R("isDisplayable", "bool", "false"), R("getParent", "void*", "0")}, true, StrList{py::str("Component")}, StrList{py::str("Component")}, StrList{py::str("URL"), py::str("Graphics")}));
    G.set(py::str("Canvas.h"), _class_stub(py::str("Canvas"), _AWT_VOID_, RetList{R("getWidth", "int", "J2X_COMPONENT_WIDTH()"), R("getHeight", "int", "J2X_COMPONENT_HEIGHT()"), R("getGraphics", "Graphics*", "0"), R("getBufferStrategy", "BufferStrategy*", "0")}, true, StrList{py::str("Component")}, StrList{py::str("Component")}, StrList{py::str("Graphics"), py::str("BufferStrategy")}, StrList{py::str("void createBufferStrategy(...) {}")}));
    G.set(py::str("Component.h"), _class_stub(py::str("Component"), _AWT_VOID_, RetList{R("getWidth", "int", "J2X_COMPONENT_WIDTH()"), R("getHeight", "int", "J2X_COMPONENT_HEIGHT()"), R("getGraphics", "Graphics*", "0")}, true, StrList{}, StrList{py::str("Point")}, StrList{py::str("Graphics")}, StrList{py::str("Point* getLocationOnScreen() { static Point _p; return &_p; }")}));
    G.set(py::str("Component.h"), _replace_first(G.at(py::str("Component.h")), py::str("#include <string>\n"), py::str("#include <string>\n#ifndef J2X_COMPONENT_WIDTH\n#define J2X_COMPONENT_WIDTH() 0\n#define J2X_COMPONENT_HEIGHT() 0\n#endif\n")));
    G.set(py::str("FileDialog.h"), _class_stub(py::str("FileDialog"), StrList{py::str("setVisible"), py::str("setMode"), py::str("setDirectory"), py::str("setFile"), py::str("setFilenameFilter"), py::str("setLocationRelativeTo")}, RetList{R("getFile", "std::string", "std::string()"), R("getDirectory", "std::string", "std::string()")}));
    G.set(py::str("Point.h"), _class_stub(py::str("Point"), StrList{}, RetList{}, true, StrList{}, StrList{}, StrList{}, StrList{py::str("int x;"), py::str("int y;")}));
    G.set(py::str("PointerInfo.h"), _class_stub(py::str("PointerInfo"), StrList{}, RetList{}, true, StrList{}, StrList{py::str("Point")}, StrList{}, StrList{py::str("Point* getLocation() { static Point _p; return &_p; }")}));
    G.set(py::str("MouseInfo.h"), _ns_stub(py::str("MouseInfo"), ConstList{}, StrList{}, RetList{R("getPointerInfo", "PointerInfo", "PointerInfo()")}, StrList{py::str("PointerInfo")}));
    G.set(py::str("Dialog.h"), _class_stub(py::str("Dialog")));
    G.set(py::str("DataBuffer.h"), _class_stub(py::str("DataBuffer")));
    G.set(py::str("DataBufferInt.h"), _class_stub(py::str("DataBufferInt"), StrList{}, RetList{R("getData", "JArray<int >", "JArray<int >()")}, true, StrList{py::str("DataBuffer")}, StrList{py::str("DataBuffer"), py::str("JArray")}));
    G.set(py::str("Raster.h"), _class_stub(py::str("Raster"), StrList{}, RetList{R("getDataBuffer", "DataBuffer*", "0")}, true, StrList{}, StrList{}, StrList{py::str("DataBuffer")}));
    G.set(py::str("WritableRaster.h"), _class_stub(py::str("WritableRaster"), StrList{}, RetList{}, true, StrList{py::str("Raster")}, StrList{py::str("Raster")}));
    G.set(py::str("BufferedImage.h"), _class_stub(py::str("BufferedImage"), StrList{py::str("setRGB"), py::str("flush")}, RetList{R("getWidth", "int", "0"), R("getHeight", "int", "0"), R("getRGB", "int", "0"), R("getData", "void*", "0"), R("getRaster", "Raster", "Raster()"), R("getGraphics", "Graphics*", "j2xSharedGraphics()"), R("createGraphics", "Graphics*", "j2xSharedGraphics()")}, true, StrList{}, StrList{py::str("Raster"), py::str("Graphics")}));
    SM.set(py::str("JList"), StubMembers{std::nullopt, MethodMap{{py::str("iterator"), py::str("Iterator*")}}});
    SM.set(py::str("JSet"), StubMembers{std::nullopt, MethodMap{{py::str("iterator"), py::str("Iterator*")}}});
    SM.set(py::str("JMap"), StubMembers{std::nullopt, MethodMap{{py::str("entrySet"), py::str("JList")}, {py::str("values"), py::str("JList")}, {py::str("keySet"), py::str("JSet")}}});
    SM.set(py::str("ByteBuffer"), StubMembers{std::nullopt, MethodMap{{py::str("allocateDirect"), py::str("ByteBuffer*")}, {py::str("allocate"), py::str("ByteBuffer*")}, {py::str("order"), py::str("ByteBuffer*")}, {py::str("asIntBuffer"), py::str("IntBuffer*")}, {py::str("asFloatBuffer"), py::str("FloatBuffer*")}}});
}

void ensure_built() {
    Store& s = store();
    if (s.built || s.building) return;
    s.building = true;
    build_generated_();
    const py::dict<py::str, py::str>& G = s.generated;
    // filename -> content
    py::dict<py::str, py::str>& F = s.files;
    F.set(py::str("Random.h"), RANDOM_H());
    F.set(py::str("JavaString.h"), JAVASTRING_H());
    F.set(py::str("JArray.h"), JARRAY_H());
    F.set(py::str("JIterator.h"), JITERATOR_H());
    // java.util.Iterator maps to the `Iterator` struct, which lives in JIterator.h. Code that names the
    // type emits `#include "Iterator.h"` (RUNTIME_HEADERS), so provide it as a thin alias header.
    F.set(py::str("Iterator.h"), py::str("#ifndef ITERATOR_H\n#define ITERATOR_H\n#include \"JIterator.h\"\n#endif\n"));
    F.set(py::str("JList.h"), JLIST_H());
    F.set(py::str("JSet.h"), JSET_H());
    F.set(py::str("JMap.h"), JMAP_H());
    F.set(py::str("JavaRT.h"), JAVART_H());
    F.set(py::str("GL11.h"), GL11_H);
    F.set(py::str("Runnable.h"), RUNNABLE_H());
    F.set(py::str("Thread.h"), THREAD_H());
    F.set(py::str("Comparator.h"), COMPARATOR_H());
    F.set(py::str("Collections.h"), COLLECTIONS_H());
    F.set(py::str("File.h"), FILE_H);
    F.set(py::str("FilenameFilter.h"), FILENAMEFILTER_H());
    F.set(py::str("Canvas.h"), CANVAS_H());
    F.set(py::str("Applet.h"), APPLET_H());
    F.update(G);                                          // FILES.update(_GENERATED)
    s.built = true;
    s.building = false;
    // Fail fast at import if a canonical header was added but never wired (so it can't ship as a silent stub).
    s.unwired = unwired_canonicals();
    if (!s.unwired.empty()) {
        std::string msg = "[runtime] !! UNWIRED CANONICAL(S) -- exist but never _load_canonical'd (silent stub fallback): " +
                          py::join(py::str(", "), s.unwired).utf8() + "\n";
        std::fputs(msg.c_str(), stderr);
    }
}

}  // namespace

py::dict<py::str, py::str>& _GENERATED() {
    ensure_built();
    return store().generated;
}

py::dict<py::str, py::str>& FILES() {
    ensure_built();
    return store().files;
}

py::list<py::str> unwired_canonicals() {
    ensure_built();
    py::str dir = _CANONICAL_DIR();
    if (!py::os::path::isdir(dir)) return py::list<py::str>();
    py::list<py::str> bad;
    const py::dict<py::str, py::str>& files = store().files;
    for (const py::str& fn : py::sorted(py::os::listdir(dir))) {
        if (fn.endswith(py::str(".h")) && !files.contains(fn)) bad.append(fn);
    }
    return bad;
}

const py::list<py::str>& _UNWIRED() {
    ensure_built();
    return store().unwired;
}

py::str annotate_stub(const py::str& text, const py::str& class_name, const std::optional<py::str>& platform_id) {
    if (!text.contains(py::str("EXTERNAL-LIBRARY STUB")) || !platform_id || platform_id->empty()) return text;
    // (from codebreaker.java.stubs import classify_stub / platforms import get_stub_comment)
    py::str subsystem_id = std::get<0>(stubs::classify_stub(class_name));
    py::str comment = platforms::get_stub_comment(*platform_id, subsystem_id);
    py::str marker("// ==============================================================================");
    return _replace_first(text, marker, marker + py::str("\n") + comment);
}

py::dict<py::str, py::str> generate_files(const std::optional<emit::header::CppStd>& cpp_std,
                                          const std::optional<py::str>& platform_id) {
    // (from codebreaker.java.emit.std_transforms import apply_std_transforms; platforms import PLATFORMS,
    // generate_stdafx)
    bool has_platform = platform_id && !platform_id->empty();
    std::optional<platforms::Platform> plat;
    if (has_platform) plat = platforms::PLATFORMS.get(*platform_id);
    py::str plat_label = plat ? plat->label : py::str("Xbox 360");
    py::str std_label = cpp_std ? cpp_std->label : py::str("C++03");

    py::dict<py::str, py::str> out;

    if (has_platform) {
        out.set(py::str("stdafx.h"), platforms::generate_stdafx(*platform_id, std_label));
        // The PCH *create* source: Xbox 360 projects build <Project>.pch from stdafx.cpp (/Yc),
        // every other .cpp uses it (/Yu). Without this the vcxproj's stdafx.cpp Create block has no
        // file and all sources fail with C1083 "cannot open precompiled header". (Emitted for all
        // platforms -- it is just `#include "stdafx.h"`, harmless where no PCH is used.)
        out.set(py::str("stdafx.cpp"), py::str("#include \"stdafx.h\"\n"));
    } else {
        // Every emitted file includes stdafx.h; with no platform it is the generic one (the C/C++
        // standard library and the fixed-width typedefs), so a platform-less conversion compiles.
        out.set(py::str("stdafx.h"), platforms::generate_stdafx(py::str("generic"), std_label));
        // ... and the out-of-line pieces of the file layer that a platform's reference layer defines
        // (FileIO.reference.cpp on the 360), in standard C, so a program that opens a file still links
        // (WorldProvider, CanvasIsomPreview: undefined FileInputStream::FileInputStream(File*)).
        out.set(py::str("j2x_host_io.cpp"), HOST_IO_CPP);
    }
    if (platform_id && *platform_id == "pc_opengl") {
        out.set(py::str("j2x_host_io.cpp"), HOST_IO_CPP);   // the PC's files are the C library's
    }

    bool is_non_windows = platform_id && (*platform_id == "ps3" || *platform_id == "wii" || *platform_id == "psp" ||
                                          *platform_id == "nds");

    for (const auto& kv : FILES().items()) {
        const py::str& fname = std::get<0>(kv);
        py::str content = std::get<1>(kv);
        py::str class_name = fname.endswith(py::str(".h")) ? fname.slice(0, fname.size() - 2) : fname;
        if (has_platform && plat_label != "Xbox 360") {
            content = content.replace(_PLATFORM_BANNER_OLD, py::str("the real behaviour is the ") + plat_label +
                                                                py::str(" platform layer"));
            // plat is set here: plat_label differs from the default only for a known platform
            const py::dict<py::str, platforms::Subsystem>& subs = plat->subsystems;
            py::str sid = std::get<0>(stubs::classify_stub(class_name));
            std::optional<platforms::Subsystem> sub_info = subs.get(sid);
            if (sub_info) {
                content = content.replace(_PLATFORM_BANNER_SDK_OLD,
                                          py::str("// (") + sub_info->sdk + py::str(" / ") + sub_info->libs + py::str(")."));
            }
        }
        if ((is_non_windows || !has_platform) && (fname == "JavaRT.h" || fname == "Random.h")) {
            content = _patch_timer_for_platform(content, has_platform ? *platform_id : py::str("generic"));
        }
        if (has_platform) content = annotate_stub(content, class_name, platform_id);
        if (cpp_std) content = emit::std_transforms::apply_std_transforms(content, emit::std_transforms::CppStandard{cpp_std->year});
        out.set(fname, content);
    }
    return out;
}

const py::dict<py::str, py::dict<py::str, py::str>>& _TIMER_PATCHES() {
    static const py::dict<py::str, py::dict<py::str, py::str>> v = {
        // no platform: the C library's clocks (coarse, but portable C++03 -- no <windows.h> timers)
        {py::str("generic"), {
            {py::str("GetTickCount()"), py::str("((unsigned long)(clock() * 1000.0 / CLOCKS_PER_SEC))")},
            {py::str("currentTimeMillis"), py::str("inline long long currentTimeMillis() {\n"
                                                    "\t\treturn (long long)time(0) * 1000LL;\n"
                                                    "\t}")},
            {py::str("nanoTime"), py::str("inline long long nanoTime() {\n"
                                           "\t\treturn (long long)(clock() * (1000000000.0 / CLOCKS_PER_SEC));\n"
                                           "\t}")},
        }},
        {py::str("ps3"), {
            {py::str("GetTickCount()"), py::str("((unsigned long)(sys_time_get_system_time() / 1000))")},
            {py::str("currentTimeMillis"), py::str("inline long long currentTimeMillis() {\n"
                                                    "\t\treturn (long long)(sys_time_get_system_time() / 1000);\n"
                                                    "\t}")},
            {py::str("nanoTime"), py::str("inline long long nanoTime() {\n"
                                           "\t\treturn (long long)sys_time_get_system_time() * 1000LL;\n"
                                           "\t}")},
        }},
        {py::str("wii"), {
            {py::str("GetTickCount()"), py::str("((unsigned long)(ticks_to_millisecs(gettime())))")},
            {py::str("currentTimeMillis"), py::str("inline long long currentTimeMillis() {\n"
                                                    "\t\treturn (long long)ticks_to_millisecs(gettime());\n"
                                                    "\t}")},
            {py::str("nanoTime"), py::str("inline long long nanoTime() {\n"
                                           "\t\treturn (long long)ticks_to_nanosecs(gettime());\n"
                                           "\t}")},
        }},
        {py::str("psp"), {
            {py::str("GetTickCount()"), py::str("((unsigned long)(sceKernelGetSystemTimeLow() / 1000))")},
            {py::str("currentTimeMillis"), py::str("inline long long currentTimeMillis() {\n"
                                                    "\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n"
                                                    "\t\treturn (long long)(tick.tick / 1000);\n"
                                                    "\t}")},
            {py::str("nanoTime"), py::str("inline long long nanoTime() {\n"
                                           "\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n"
                                           "\t\treturn (long long)(tick.tick * 1000LL);\n"
                                           "\t}")},
        }},
        {py::str("nds"), {
            {py::str("GetTickCount()"), py::str("((unsigned long)(cpuGetTiming() / BUS_CLOCK * 1000))")},
            {py::str("currentTimeMillis"), py::str("inline long long currentTimeMillis() {\n"
                                                    "\t\treturn 0LL; // TODO: implement DS timer\n"
                                                    "\t}")},
            {py::str("nanoTime"), py::str("inline long long nanoTime() {\n"
                                           "\t\treturn 0LL; // TODO: implement DS timer\n"
                                           "\t}")},
        }},
    };
    return v;
}

const py::re::Pattern& _CURRENT_TIME_RE() {
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT(inline long long currentTimeMillis\(\)[^}]*\})PAT"));
    return p;
}

const py::re::Pattern& _NANO_TIME_RE() {
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT(inline long long nanoTime\(\)\s*\{[^}]*\})PAT"));
    return p;
}

const py::re::Pattern& _GET_TICK_COUNT() {
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT(\(long long\)GetTickCount\(\))PAT"));
    return p;
}

py::str _patch_timer_for_platform(const py::str& content0, const py::str& platform_id) {
    py::str content = content0;
    std::optional<py::dict<py::str, py::str>> patch = _TIMER_PATCHES().get(platform_id);
    if (!patch || patch->empty()) return content;
    if (patch->contains(py::str("currentTimeMillis"))) {
        content = _CURRENT_TIME_RE().sub(patch->at(py::str("currentTimeMillis")), content);
    }
    if (patch->contains(py::str("nanoTime"))) {
        content = _NANO_TIME_RE().sub(patch->at(py::str("nanoTime")), content);
    }
    if (patch->contains(py::str("GetTickCount()"))) {
        content = _GET_TICK_COUNT().sub(patch->at(py::str("GetTickCount()")), content);
    }
    return content;
}

}  // namespace codebreaker::java::runtime
