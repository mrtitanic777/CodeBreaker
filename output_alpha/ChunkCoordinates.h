#ifndef CHUNKCOORDINATES_H
#define CHUNKCOORDINATES_H

// Auto-generated from ChunkCoordinates.java
// Target: Xbox 360 / C++03 / DirectX 9

class ChunkCoordinates {
public:
	ChunkCoordinates();
	ChunkCoordinates(int var1, int var2);
	virtual ~ChunkCoordinates();

	int m_field_1518_a;
	int m_field_1517_b;

	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();

};

#endif // CHUNKCOORDINATES_H