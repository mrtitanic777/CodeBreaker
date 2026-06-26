#ifndef TELEPORTER_H
#define TELEPORTER_H

// Auto-generated from Teleporter.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class Random;
class World;

class Teleporter {
public:
	Teleporter();
	virtual ~Teleporter();

	virtual void func_4107_a(World* var1, Entity* var2);
	virtual bool func_4106_b(World* var1, Entity* var2);
	virtual bool func_4108_c(World* var1, Entity* var2);

public: // was private
	Random* m_field_4232_a;

};

#endif // TELEPORTER_H