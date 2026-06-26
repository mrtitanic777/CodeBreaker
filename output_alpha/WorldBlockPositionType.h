#ifndef WORLDBLOCKPOSITIONTYPE_H
#define WORLDBLOCKPOSITIONTYPE_H

// Auto-generated from WorldBlockPositionType.java
// Target: Xbox 360 / C++03 / DirectX 9

class WorldClient;

class WorldBlockPositionType {
public:
	WorldBlockPositionType();
	WorldBlockPositionType(WorldClient* var1, int var2, int var3, int var4, int var5, int var6);
	virtual ~WorldBlockPositionType();


public: // was private
	int m_field_1202_a;
	int m_field_1201_b;
	int m_field_1207_c;
	int m_field_1206_d;
	int m_field_1205_e;
	int m_field_1204_f;
	WorldClient* m_field_1203_g;

};

#endif // WORLDBLOCKPOSITIONTYPE_H