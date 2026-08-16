// NIOBuffers.h — real java.nio Byte/Float/IntBuffer for the Xbox 360 j2x port.  [Step 5]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// NIO buffer stubs (j2x/runtime.py _buffer_stub -> ByteBuffer.h / FloatBuffer.h / IntBuffer.h, where
// every method returns self / 0). The new code packs REAL data through these:
//   * Tessellator (Tessellator.cpp) does GLAllocation::createDirectByteBuffer(n) -> asIntBuffer()/
//     asFloatBuffer() VIEWS, then intBuffer->put(rawBuffer, 0, len), byteBuffer->position()/limit(),
//     floatBuffer->position(3), and hands the buffer to GL11::glVertexPointer/glColorPointer/
//     glTexCoordPointer.
//   * The GL11->D3D9 emulator (GL11.reference.cpp) reads vertex data back as
//     `((ByteBuffer*)buf)->base() + buf->position()` and treats position() as a BYTE offset.
//   * GLAllocation::generateTextureNames / RenderEngine drive a "singleIntBuffer":
//     clear() / put(int) / get(int) / get() / flip() / position() / limit().
//
// It is NOT compiled by the j2x build (which only syntax-checks the no-op stubs). Lives in the Xbox
// project. Grounded in the old port's GLAllocation.cpp (raw new[] arrays) and the new Tessellator's
// exact buffer usage. See docs/platform/04-textures-resources-fileio.md and 05b-step5-integration.md.
//
// DESIGN — one backing store, three typed views, byte-granular position:
//   * A refcounted Backing owns the contiguous bytes (`char* base`, `int cap` in bytes).
//   * Buffer (base class) holds `position`/`limit` IN BYTES + an element shift (Byte=0, Float/Int=2).
//     position()/limit()/base() are NON-VIRTUAL and BYTE-valued, so `(ByteBuffer*)floatBuffer`
//     (an upcast — all three derive from Buffer) reads the SAME byte position. This is exactly what
//     the GL emulator relies on: a FloatBuffer parked at float-element 3 reports position()==12.
//   * The TYPED API (put/get/position(int)) works in ELEMENTS (Java nio semantics): FloatBuffer::
//     position(3) sets the shared byte position to 12; IntBuffer::get(0) reads bytes [0..4).
//   * asIntBuffer()/asFloatBuffer() return NEW views over the SAME Backing (shared bytes) with their
//     own independent position/limit reset to the full range, matching java.nio's view semantics.
//   * Layout note: all three derive from Buffer via single inheritance with Buffer the ONLY base, so
//     they are layout-identical and the GL emulator's `(ByteBuffer*)floatBuffer` reinterpret keeps the
//     same object pointer; base()/position() (Buffer members) read correctly through any of them.
//
// BIG-ENDIAN NOTE: these buffers are RAW vertex/index scratch for the GPU (floatToRawIntBits packed
// ints, GL byte-order RGBA), NOT serialized NBT. The Tessellator writes ints and the D3D9 vertex
// path reads them back on the SAME machine, so native word order is correct here and a straight
// memcpy/reinterpret of the backing bytes is fine. Do NOT use these buffers for NBT/file I/O — that
// path is byte-at-a-time big-endian (DataInput/DataOutput, see FileIO.reference / 05b note).

#ifndef NIOBUFFERS_H
#define NIOBUFFERS_H

#include "stdafx.h"
#include "JArray.h"          // JArray<int> for IntBuffer::put(JArray<int>, off, len)

class IntBuffer;
class FloatBuffer;

// --- refcounted contiguous backing store (the "direct" memory) -----------------
struct NioBacking {
	char* bytes;
	int   cap;               // capacity in BYTES
	int   ref;
	explicit NioBacking(int nbytes) : bytes(nbytes > 0 ? new char[nbytes] : 0), cap(nbytes > 0 ? nbytes : 0), ref(1) {}
	void addRef() { ++ref; }
	void release() { if (--ref <= 0) { delete[] bytes; delete this; } }
private:
	~NioBacking() {}
	NioBacking(const NioBacking&); NioBacking& operator=(const NioBacking&);
};

// --- common base: byte-granular position/limit shared across views -------------
// All three buffer types derive from Buffer so a FloatBuffer*/IntBuffer* upcasts to ByteBuffer*
// (the GL emulator does (ByteBuffer*)buf). base()/position()/limit() are byte-valued + NON-virtual.
class Buffer {
protected:
	NioBacking* _b;          // owning ref to the backing bytes
	int _posB;               // position in BYTES
	int _limB;               // limit    in BYTES
	int _shift;              // element size shift: byte=0, int/float=2 (1<<2 == 4)
	Buffer(NioBacking* b, int shift) : _b(b), _posB(0), _limB(b ? b->cap : 0), _shift(shift) { if (_b) _b->addRef(); }
public:
	virtual ~Buffer() { if (_b) _b->release(); }
	// base() = start of the backing bytes + the CURRENT byte position. The GL emulator reads vertex
	// data straight from base() with NO further position() arithmetic -- this folds the cursor in so
	// it is correct no matter which typed view it gets (a FloatBuffer parked at element 3 yields
	// start+12). This is the contract GL11.reference.cpp's bufBase() relies on (it casts to
	// ByteBuffer* and uses base() alone).
	char* base() const { return _b ? (_b->bytes + _posB) : 0; }
	char* baseStart() const { return _b ? _b->bytes : 0; }      // start of backing, ignoring position
	// position()/limit()/capacity() are ELEMENT-valued (java.nio semantics) -- e.g. GLAllocation
	// generateTextureNames iterates `for(i=buf->position(); i<buf->limit(); ++i) buf->get(i)` over an
	// IntBuffer, using element indices. For a ByteBuffer, element == byte.
	int   position() const { return _posB >> _shift; }
	int   positionBytes() const { return _posB; }
	int   capacity() const { return _b ? (_b->cap >> _shift) : 0; }
	int   limit() const { return _limB >> _shift; }
	int   remaining() const { return (_limB - _posB) >> _shift; }
};

// --- java.nio.ByteBuffer -------------------------------------------------------
class ByteBuffer : public Buffer {
public:
	using Buffer::position; using Buffer::limit;   // un-hide the 0-arg getters
	explicit ByteBuffer(NioBacking* b) : Buffer(b, 0) {}
	~ByteBuffer() {}

	// java.nio.ByteBuffer.allocateDirect(n): fresh backing of n bytes (GLAllocation calls this).
	static ByteBuffer* allocateDirect(int nbytes) { return new ByteBuffer(new NioBacking(nbytes)); }
	ByteBuffer* order(int /*byteOrder*/) { return this; }      // ByteOrder.nativeOrder() -> no-op (ptr: the converter chains ->order()->asIntBuffer() and assigns to ByteBuffer*)

	ByteBuffer& clear()  { _posB = 0; _limB = _b ? _b->cap : 0; return *this; }
	ByteBuffer& flip()   { _limB = _posB; _posB = 0; return *this; }
	ByteBuffer& rewind() { _posB = 0; return *this; }
	ByteBuffer& mark()   { return *this; }
	ByteBuffer& position(int p) { _posB = p; return *this; }    // ByteBuffer position == bytes
	ByteBuffer& limit(int l)    { _limB = l; return *this; }

	// relative byte put/get
	ByteBuffer& put(int v) { if (_b && _posB < _b->cap) _b->bytes[_posB++] = (char)v; return *this; }
	int get() { return (_b && _posB < _limB) ? (unsigned char)_b->bytes[_posB++] : 0; }
	int get(int idx) { return (_b && idx >= 0 && idx < _b->cap) ? (unsigned char)_b->bytes[idx] : 0; }
	// 4-byte int put/get (relative + absolute byte index). NATIVE order to round-trip with IntBuffer
	// over the same backing -- this is on-device scratch (GPU buffers), not serialized NBT.
	int getInt() { if (_b && _posB + 4 <= _limB) { int v = *(int*)(_b->bytes + _posB); _posB += 4; return v; } return 0; }
	int getInt(int idx) { return (_b && idx >= 0 && idx + 4 <= _b->cap) ? *(int*)(_b->bytes + idx) : 0; }
	ByteBuffer& putInt(int v) { if (_b && _posB + 4 <= _b->cap) { *(int*)(_b->bytes + _posB) = v; _posB += 4; } return *this; }
	ByteBuffer& putInt(int idx, int v) { if (_b && idx >= 0 && idx + 4 <= _b->cap) *(int*)(_b->bytes + idx) = v; return *this; }

	// bulk relative put/get over a JArray<unsigned char> (RenderEngine texture upload via put(bytes);
	// ScreenShotHelper readback via get(bytes)). Byte-granular, advances the shared byte position.
	ByteBuffer& put(const JArray<unsigned char>& src) {
		for (int i = 0; i < src.length && _b && _posB < _b->cap; ++i) _b->bytes[_posB++] = (char)src[i];
		return *this;
	}
	ByteBuffer& get(JArray<unsigned char>& dst) {
		for (int i = 0; i < dst.length && _b && _posB < _limB; ++i) dst[i] = (unsigned char)_b->bytes[_posB++];
		return *this;
	}

	// VIEWS over the SAME bytes (share backing; independent position/limit) -- java.nio semantics.
	IntBuffer*   asIntBuffer();
	FloatBuffer* asFloatBuffer();

	operator void*() { return base(); }                        // legacy interop (truthiness / raw ptr)
};

// --- java.nio.IntBuffer (view over a ByteBuffer's bytes) -----------------------
class IntBuffer : public Buffer {
public:
	using Buffer::position; using Buffer::limit;   // un-hide the 0-arg getters
	explicit IntBuffer(NioBacking* b) : Buffer(b, 2) {}
	~IntBuffer() {}

	IntBuffer& clear()  { _posB = 0; _limB = _b ? _b->cap : 0; return *this; }
	IntBuffer& flip()   { _limB = _posB; _posB = 0; return *this; }
	IntBuffer& rewind() { _posB = 0; return *this; }
	IntBuffer& mark()   { return *this; }
	IntBuffer& position(int p) { _posB = p << 2; return *this; }
	IntBuffer& limit(int l)    { _limB = l << 2; return *this; }

	// relative put(int)
	IntBuffer& put(int v) {
		if (_b && _posB + 4 <= _b->cap) { *(int*)(_b->bytes + _posB) = v; _posB += 4; }
		return *this;
	}
	// bulk put from a JArray<int> (Tessellator: intBuffer->put(rawBuffer, 0, rawBufferIndex)).
	// Native int copy -- scratch GPU data on the SAME machine, not serialized NBT (see header note).
	IntBuffer& put(const JArray<int>& src, int off, int len) {
		for (int i = 0; i < len && _posB + 4 <= (_b ? _b->cap : 0); ++i) {
			*(int*)(_b->bytes + _posB) = src[off + i]; _posB += 4;
		}
		return *this;
	}
	// relative get()
	int get() {
		if (_b && _posB + 4 <= _limB) { int v = *(int*)(_b->bytes + _posB); _posB += 4; return v; }
		return 0;
	}
	// absolute get(index) (GLAllocation: var0->get(var1); RenderEngine: singleIntBuffer->get(0)).
	int get(int idx) {
		int o = idx << 2;
		return (_b && o + 4 <= _b->cap) ? *(int*)(_b->bytes + o) : 0;
	}
	operator void*() { return base(); }
};

// --- java.nio.FloatBuffer (view over a ByteBuffer's bytes) ---------------------
class FloatBuffer : public Buffer {
public:
	using Buffer::position; using Buffer::limit;   // un-hide the 0-arg getters
	explicit FloatBuffer(NioBacking* b) : Buffer(b, 2) {}
	~FloatBuffer() {}

	FloatBuffer& clear()  { _posB = 0; _limB = _b ? _b->cap : 0; return *this; }
	FloatBuffer& flip()   { _limB = _posB; _posB = 0; return *this; }
	FloatBuffer& rewind() { _posB = 0; return *this; }
	FloatBuffer& mark()   { return *this; }
	FloatBuffer& position(int p) { _posB = p << 2; return *this; }   // float element -> byte
	FloatBuffer& limit(int l)    { _limB = l << 2; return *this; }

	FloatBuffer& put(float v) {
		if (_b && _posB + 4 <= _b->cap) { *(float*)(_b->bytes + _posB) = v; _posB += 4; }
		return *this;
	}
	FloatBuffer& put(const JArray<float>& src, int off, int len) {
		for (int i = 0; i < len && _posB + 4 <= (_b ? _b->cap : 0); ++i) {
			*(float*)(_b->bytes + _posB) = src[off + i]; _posB += 4;
		}
		return *this;
	}
	FloatBuffer& get(JArray<float>& dst) { for (int i = 0; i < dst.length; ++i) dst[i] = get(); return *this; }
	float get() {
		if (_b && _posB + 4 <= _limB) { float v = *(float*)(_b->bytes + _posB); _posB += 4; return v; }
		return 0.0F;
	}
	float get(int idx) {
		int o = idx << 2;
		return (_b && o + 4 <= _b->cap) ? *(float*)(_b->bytes + o) : 0.0F;
	}
	operator void*() { return base(); }
};

#endif // NIOBUFFERS_H
