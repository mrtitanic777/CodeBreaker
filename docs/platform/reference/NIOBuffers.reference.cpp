// NIOBuffers.cpp — real java.nio Byte/Float/IntBuffer view creation.  [Step 5]
// REFERENCE (Xbox-side; not built by j2ctool). See NIOBuffers.reference.h for the full design.
//
// asIntBuffer()/asFloatBuffer() create a NEW typed view that shares the SAME backing bytes as the
// ByteBuffer (java.nio view semantics) -- the Tessellator does:
//     this->byteBuffer = GLAllocation::createDirectByteBuffer(n);
//     this->intBuffer  = this->byteBuffer->asIntBuffer();     // same bytes
//     this->floatBuffer= this->byteBuffer->asFloatBuffer();   // same bytes
// then writes ints via intBuffer and reads them as floats/colors through the GL emulator -- all over
// one contiguous block. The view's position/limit start at the full range (fresh, like java.nio).
//
// LIFETIME: the view holds its own ref on the shared NioBacking (Buffer ctor addRef's), so it stays
// valid as long as either the ByteBuffer or any view is alive. The Tessellator keeps all three for
// the life of the (singleton) tessellator, so nothing is freed mid-frame. createDirectByteBuffer'd
// buffers are effectively permanent scratch -- the old port also never freed them (raw new[]).

#include "stdafx.h"
#include "NIOBuffers.h"

IntBuffer* ByteBuffer::asIntBuffer() {
	// Share THIS buffer's backing bytes; new view, independent cursor over the full backing.
	return new IntBuffer(_b);
}

FloatBuffer* ByteBuffer::asFloatBuffer() {
	return new FloatBuffer(_b);
}

// ------------------------------------------------------------------------------
// INTEGRATION with the converted GLAllocation.cpp
// ------------------------------------------------------------------------------
// The converter emitted (j2xv7/GLAllocation.cpp) a LOCAL shim because the central stub had no real
// allocateDirect:
//     static ByteBuffer* _bb_allocateDirect(int) { return new ByteBuffer(); }   // <-- no bytes!
//     ...
//     ByteBuffer* GLAllocation::createDirectByteBuffer(int var0) {
//         (void)_ByteOrder::nativeOrder();
//         ByteBuffer* var1 = _bb_allocateDirect(var0);                            // <-- zero-size
//         return var1;
//     }
// With the real ByteBuffer, REPLACE that shim body so the buffer actually owns `var0` bytes:
//     ByteBuffer* GLAllocation::createDirectByteBuffer(int var0) {
//         return ByteBuffer::allocateDirect(var0);   // .order(nativeOrder()) is a no-op
//     }
// (createDirectIntBuffer/createDirectFloatBuffer already chain ->asIntBuffer()/asFloatBuffer() on
// that, so they need no change once createDirectByteBuffer is real.)
//
// `new ByteBuffer()` no longer compiles (the real ctor needs a NioBacking*), which is the desired
// forcing function: the zero-size shim CAN'T silently survive. If you prefer to keep the generated
// file untouched, instead make the shim real:
//     static ByteBuffer* _bb_allocateDirect(int n) { return ByteBuffer::allocateDirect(n); }
