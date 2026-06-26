#ifndef CHUNKCOORDINTPAIR_H
#define CHUNKCOORDINTPAIR_H

// Auto-generated from ChunkCoordIntPair.java
// Target: Xbox 360 / C++03 / DirectX 9

class ChunkCoordIntPair {
public:
	ChunkCoordIntPair();
	ChunkCoordIntPair(int var1, int var2);
	virtual ~ChunkCoordIntPair();

	int m_chunkXPos;
	int m_chunkZPos;

	virtual int hashCode();
	virtual bool equals(void* /* PORT: was Object */ var1);

};

#endif // CHUNKCOORDINTPAIR_H