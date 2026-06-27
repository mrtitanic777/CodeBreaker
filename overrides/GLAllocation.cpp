#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from GLAllocation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GLAllocation.h"
#include "ByteBuffer.h"
#include "FloatBuffer.h"
#include "GL11.h"
#include "IntBuffer.h"

// Local shims for runtime symbols not present in the central stubs (see report):
//  - GL11::glDeleteLists      (GL11.h has glGenLists/glGenTextures/glDeleteTextures only)
//  - ByteOrder.nativeOrder()  (no-op; the platform ByteBuffer is already native order)
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // GL11.glDeleteLists, used in deleteTexturesAndDisplayLists():
 *  GL11.glDeleteLists(((Integer)displayLists.get(var0)).intValue(), ((Integer)displayLists.get(var0 + 1)).intValue());
 *  // ByteBuffer.allocateDirect(...).order(ByteOrder.nativeOrder()), used in createDirectByteBuffer():
 *  ByteBuffer var1 = ByteBuffer.allocateDirect(var0).order(ByteOrder.nativeOrder());
 *  ---- end original Java ----
 *  glDeleteLists stays a no-op. allocateDirect now routes to the REAL platform ByteBuffer
 *  (NIOBuffers.h ByteBuffer::allocateDirect -> n-byte direct backing); nativeOrder is a no-op. */
namespace { inline void _gl11_glDeleteLists(int, int) {} }
static ByteBuffer* _bb_allocateDirect(int n) { return ByteBuffer::allocateDirect(n); }   // real direct buffer (n bytes)
namespace _ByteOrder { inline int nativeOrder() { return 0; } }
// box/unbox: the Java source kept List<Integer>; here the list element type is void*, so each
// int is boxed into the pointer and read back out (no Integer object type exists in the runtime).
static inline void* _boxInt(int v) { return (void*)(long long)v; }
static inline int _unboxInt(void* p) { return (int)(long long)p; }

JList<void*> GLAllocation::s_displayLists;
JList<void*> GLAllocation::s_textureNames;

GLAllocation::~GLAllocation()
{
}

int GLAllocation::generateDisplayLists(int var0)
{
	int var1 = GL11::glGenLists(var0);
	s_displayLists.push_back(_boxInt(var1));
	s_displayLists.push_back(_boxInt(var0));
	return var1;
}

void GLAllocation::generateTextureNames(IntBuffer* var0)
{
	GL11::glGenTextures(var0);
	for(int var1 = var0->position(); var1 < var0->limit(); ++var1) {
	s_textureNames.push_back(_boxInt(var0->get(var1)));
	}
}

void GLAllocation::deleteTexturesAndDisplayLists()
{
	for(int var0 = 0; var0 < (int)s_displayLists.size(); var0 += 2) {
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  GL11.glDeleteLists(((Integer)displayLists.get(var0)).intValue(), ((Integer)displayLists.get(var0 + 1)).intValue());
	 *  ---- end original Java ----
	 *  _gl11_glDeleteLists is a no-op shim; the display lists are never freed on the GPU. */
	_gl11_glDeleteLists(_unboxInt(s_displayLists[var0]), _unboxInt(s_displayLists[var0 + 1]));
	}
	IntBuffer* var2 = createDirectIntBuffer(s_textureNames.size());
	var2->flip();
	GL11::glDeleteTextures(var2);
	for(int var1 = 0; var1 < (int)s_textureNames.size(); ++var1) {
	var2->put(_unboxInt(s_textureNames[var1]));
	}
	var2->flip();
	GL11::glDeleteTextures(var2);
	s_displayLists.clear();
	s_textureNames.clear();
}

ByteBuffer* GLAllocation::createDirectByteBuffer(int var0)
{
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  ByteBuffer var1 = ByteBuffer.allocateDirect(var0).order(ByteOrder.nativeOrder());
	 *  ---- end original Java ---- */
	// Java: ByteBuffer.allocateDirect(var0).order(ByteOrder.nativeOrder())
	// order() returns the same buffer; native order is the default here, so it is a no-op.
	(void)_ByteOrder::nativeOrder();
	ByteBuffer* var1 = _bb_allocateDirect(var0);
	return var1;
}

IntBuffer* GLAllocation::createDirectIntBuffer(int var0)
{
	return createDirectByteBuffer(var0 << 2)->asIntBuffer();
}

FloatBuffer* GLAllocation::createDirectFloatBuffer(int var0)
{
	return createDirectByteBuffer(var0 << 2)->asFloatBuffer();
}
