#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.nio.ByteBuffer.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
class FloatBuffer; class IntBuffer; class ByteBuffer;

class ByteBuffer {
public:
	ByteBuffer() {}
	virtual ~ByteBuffer() {}
	ByteBuffer& clear() { return *this; }
	ByteBuffer& flip() { return *this; }
	ByteBuffer& rewind() { return *this; }
	ByteBuffer& mark() { return *this; }
	ByteBuffer& put(...) { return *this; }
	ByteBuffer& putInt(...) { return *this; }
	ByteBuffer& putShort(...) { return *this; }
	ByteBuffer& putLong(...) { return *this; }
	ByteBuffer& putFloat(...) { return *this; }
	ByteBuffer& putDouble(...) { return *this; }
	ByteBuffer& putChar(...) { return *this; }
	int get(...) { return 0; }
	int getInt(...) { return 0; }
	short getShort(...) { return 0; }
	long long getLong(...) { return 0; }
	float getFloat(...) { return 0.0F; }
	double getDouble(...) { return 0.0; }
	char getChar(...) { return 0; }
	int position() { return 0; }
	ByteBuffer& position(int) { return *this; }
	int limit() { return 0; }
	ByteBuffer& limit(int) { return *this; }
	int remaining() { return 0; }
	int capacity() { return 0; }
	FloatBuffer* asFloatBuffer() { return 0; }
	IntBuffer* asIntBuffer() { return 0; }
	operator void*() { return 0; }
};
#endif // BYTEBUFFER_H
