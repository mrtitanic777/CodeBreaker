#ifndef CHUNKPOSITION_H
#define CHUNKPOSITION_H

// Auto-generated from ChunkPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

class ChunkPosition {
public:
	ChunkPosition();
	ChunkPosition(int var1, int var2, int var3);
	virtual ~ChunkPosition();

	int m_x;
	int m_y;
	int m_z;

	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();

};

#endif // CHUNKPOSITION_H