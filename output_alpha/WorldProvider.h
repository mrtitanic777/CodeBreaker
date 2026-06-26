#ifndef WORLDPROVIDER_H
#define WORLDPROVIDER_H

// Auto-generated from WorldProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

class IChunkLoader;
class IChunkProvider;
class Vec3D;
class World;
class WorldChunkManager;

class WorldProvider {
public:
	WorldProvider();
	virtual ~WorldProvider();

	World* worldObj;
	WorldChunkManager* m_worldChunkMgr;
	bool m_field_4220_c;
	bool m_field_6479_d;
	bool m_field_6478_e;
	float* m_lightBrightnessTable;
	int m_field_4218_e;

	virtual void registerWorld(World* var1);
	virtual IChunkProvider* getChunkProvider();
	virtual IChunkLoader* getChunkLoader(std::string /* PORT: was java.io.File */* var1);
	virtual bool canCoordinateBeSpawn(int var1, int var2);
	virtual float calculateCelestialAngle(long long var1, float var3);
	virtual float* func_4097_b(float var1, float var2);
	virtual Vec3D* func_4096_a(float var1, float var2);
	virtual bool func_6477_d();
	static WorldProvider* func_4101_a(int var0);

public: // was protected
	virtual void generateLightBrightnessTable();
	virtual void registerWorldChunkManager();

public: // was private
	float* m_field_4217_f;

public:  // array-length companions
	int m_lightBrightnessTable_count;
	int m_field_4217_f_count;

};

#endif // WORLDPROVIDER_H