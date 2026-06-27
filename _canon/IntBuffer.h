#ifndef INTBUFFER_H
#define INTBUFFER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.nio.IntBuffer.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
class FloatBuffer; class IntBuffer; class ByteBuffer;

class IntBuffer {
public:
	IntBuffer() {}
	virtual ~IntBuffer() {}
	IntBuffer& clear() { return *this; }
	IntBuffer& flip() { return *this; }
	IntBuffer& rewind() { return *this; }
	IntBuffer& mark() { return *this; }
	IntBuffer& put(...) { return *this; }
	IntBuffer& putInt(...) { return *this; }
	IntBuffer& putShort(...) { return *this; }
	IntBuffer& putLong(...) { return *this; }
	IntBuffer& putFloat(...) { return *this; }
	IntBuffer& putDouble(...) { return *this; }
	IntBuffer& putChar(...) { return *this; }
	int get(...) { return 0; }
	int getInt(...) { return 0; }
	short getShort(...) { return 0; }
	long long getLong(...) { return 0; }
	float getFloat(...) { return 0.0F; }
	double getDouble(...) { return 0.0; }
	char getChar(...) { return 0; }
	int position() { return 0; }
	IntBuffer& position(int) { return *this; }
	int limit() { return 0; }
	IntBuffer& limit(int) { return *this; }
	int remaining() { return 0; }
	int capacity() { return 0; }
	FloatBuffer* asFloatBuffer() { return 0; }
	IntBuffer* asIntBuffer() { return 0; }
	operator void*() { return 0; }
};
#endif // INTBUFFER_H
