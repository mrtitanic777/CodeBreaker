#ifndef MAPGENBASE_H
#define MAPGENBASE_H

// Auto-generated from MapGenBase.java
// Target: Xbox 360 / C++03 / DirectX 9

class IChunkProvider;
class Random;
class World;

class MapGenBase {
public:
	MapGenBase();
	virtual ~MapGenBase();

	virtual void func_867_a(IChunkProvider* var1, World* var2, int var3, int var4, unsigned char* var5);

public: // was protected
	int m_field_1306_a;
	Random* m_field_1305_b;

	virtual void func_868_a(World* var1, int var2, int var3, int var4, int var5, unsigned char* var6);

};

#endif // MAPGENBASE_H