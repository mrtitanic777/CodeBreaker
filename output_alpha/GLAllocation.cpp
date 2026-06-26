#include "stdafx.h"
// Hand-written PORT override for GLAllocation.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Java GLAllocation is pure LWJGL/GL glue: it wraps glGenLists / glGenTextures /
// glDeleteLists / glDeleteTextures and the NIO direct-buffer allocators
// (ByteBuffer.allocateDirect, asIntBuffer/asFloatBuffer).
//
// On Xbox 360 / Direct3D 9 there is NO display-list concept, and the NIO buffers
// are already mapped to raw C arrays elsewhere in the port. So this becomes a thin
// shim: the display-list / texture-name generators return dummy handles (and track
// them in the std::vector<void*> bookkeeping lists declared in the header), and the
// "createDirectXxxBuffer" allocators simply hand back raw heap arrays.
// Modeled on the RenderEngine shim pattern (platform glue with sensible stubs).

#include "GLAllocation.h"
#include "Common.h"

std::vector<void*> GLAllocation::s_displayLists;
std::vector<void*> GLAllocation::s_textureNames;

// Monotonic dummy handle counter so each generateDisplayLists() call hands back a
// distinct non-zero "list id" (mirrors glGenLists returning a base name).
static int s_nextDisplayList = 1;

GLAllocation::GLAllocation() {
}

GLAllocation::~GLAllocation() {
	// No dynamic resources owned here.
}

int GLAllocation::generateDisplayLists(int var0) {
	// PORT: D3D9 has no display lists. Hand back a dummy base handle and reserve
	// var0 ids so callers can index [base .. base+var0). Track (base, count) in the
	// bookkeeping list the same way Java stored Integer.valueOf(base)/valueOf(count).
	int var1 = s_nextDisplayList;
	s_nextDisplayList += (var0 > 0) ? var0 : 1;
	// Store the int handles cast into the void* slots (matches header's vector<void*>).
	s_displayLists.push_back(reinterpret_cast<void*>(static_cast<size_t>(var1)));
	s_displayLists.push_back(reinterpret_cast<void*>(static_cast<size_t>(var0)));
	return var1;
}

void GLAllocation::generateTextureNames(int* /* PORT: was IntBuffer */ var0) {
	// PORT: glGenTextures has no D3D9 equivalent here; texture creation/registry is
	// owned by the Textures class. NIO position()/limit() are no-ops for a C array, so
	// without a length we cannot iterate safely -> no-op stub.
	(void)var0;
}

void GLAllocation::deleteTexturesAndDisplayLists() {
	// PORT: no glDeleteLists / glDeleteTextures on D3D9 (resources are released by
	// their owning subsystems). Just clear the bookkeeping lists.
	s_displayLists.clear();
	s_textureNames.clear();
}

unsigned char* /* PORT: was ByteBuffer */ GLAllocation::createDirectByteBuffer(int var0) {
	// NIO ByteBuffer.allocateDirect(var0) -> raw byte array.
	if (var0 <= 0) {
		return NULL;
	}
	return new unsigned char[var0];
}

int* /* PORT: was IntBuffer */ GLAllocation::createDirectIntBuffer(int var0) {
	// NIO createDirectByteBuffer(var0 << 2).asIntBuffer() -> raw int array of var0 ints.
	if (var0 <= 0) {
		return NULL;
	}
	return new int[var0];
}

float* /* PORT: was FloatBuffer */ GLAllocation::createDirectFloatBuffer(int var0) {
	// NIO createDirectByteBuffer(var0 << 2).asFloatBuffer() -> raw float array of var0 floats.
	if (var0 <= 0) {
		return NULL;
	}
	return new float[var0];
}
