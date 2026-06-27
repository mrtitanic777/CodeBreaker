#ifndef GLALLOCATION_H
#define GLALLOCATION_H

// Auto-generated from GLAllocation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ByteBuffer;
class FloatBuffer;
class IntBuffer;

class GLAllocation {
public:
	virtual ~GLAllocation();

	static JList<void*> s_displayLists;
	static JList<void*> s_textureNames;

	static int generateDisplayLists(int var0);
	static void generateTextureNames(IntBuffer* var0);
	static void deleteTexturesAndDisplayLists();
	static ByteBuffer* createDirectByteBuffer(int var0);
	static IntBuffer* createDirectIntBuffer(int var0);
	static FloatBuffer* createDirectFloatBuffer(int var0);
};

#endif // GLALLOCATION_H