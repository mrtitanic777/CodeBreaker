// cb-unity: alone
// cb/java/runtime_texts.cpp -- the verbatim C++ texts of the Java runtime (see runtime.hpp). Compiled as plain data.
#include "cb/java/runtime_texts.hpp"

namespace cb::java::runtime::verbatim {

const std::string_view kGl11Header =
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
)CBJRT";

const std::string_view kBufferUtilsHeader =
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
)CBJRT";

const std::string_view kSimpleDateFormatHeader =
R"CBJRT(#ifndef SIMPLEDATEFORMAT_H
#define SIMPLEDATEFORMAT_H
#include "DateFormat.h"   // SimpleDateFormat lives with DateFormat
#endif
)CBJRT";

const std::string_view kRuntimeHeader =
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
)CBJRT";

const std::string_view kRandomAccessFileHeader =
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
)CBJRT";

const std::string_view kStackHeader =
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
)CBJRT";

const std::string_view kEntryHeader =
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
)CBJRT";

const std::string_view kInputStreamHeader =
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
)CBJRT";

const std::string_view kFileInputStreamHeader =
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
)CBJRT";

const std::string_view kColorPrefix =
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
)CBJRT";

const std::string_view kColorSuffix =
R"CBJRT(};
#endif // COLOR_H
)CBJRT";

const std::string_view kGLContextHeader =
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
)CBJRT";

const std::string_view kClassHeader =
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
	std::vector<Class*>& reg = _j2xClassReg();             // shared registry (see getSuperclass); leaked = program-lifetime
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
)CBJRT";

const std::string_view kInflaterHeader =
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
)CBJRT";

const std::string_view kSoftReferenceHeader =
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
)CBJRT";

const std::string_view kInetAddressHeader =
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
)CBJRT";

const std::string_view kHostIoCpp =
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
)CBJRT";

}  // namespace cb::java::runtime::verbatim
