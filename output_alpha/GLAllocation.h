#ifndef GLALLOCATION_H
#define GLALLOCATION_H

// Auto-generated from GLAllocation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

class GLAllocation {
public:
	GLAllocation();
	virtual ~GLAllocation();

	static int generateDisplayLists(int var0);
	static void generateTextureNames(int* /* PORT: was IntBuffer */ var0);
	static void deleteTexturesAndDisplayLists();
	static unsigned char* /* PORT: was ByteBuffer */ createDirectByteBuffer(int var0);
	static int* /* PORT: was IntBuffer */ createDirectIntBuffer(int var0);
	static float* /* PORT: was FloatBuffer */ createDirectFloatBuffer(int var0);

public: // was private
	static std::vector<void*> /* PORT: determine element type */ s_displayLists;
	static std::vector<void*> /* PORT: determine element type */ s_textureNames;

};

#endif // GLALLOCATION_H