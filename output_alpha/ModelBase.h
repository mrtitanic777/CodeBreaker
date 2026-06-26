#ifndef MODELBASE_H
#define MODELBASE_H

// Auto-generated from ModelBase.java
// Target: Xbox 360 / C++03 / DirectX 9

class ModelBase {
public:
	ModelBase();
	virtual ~ModelBase();

	float m_field_1244_k;
	bool m_field_1243_l;

	virtual void render(float var1, float var2, float var3, float var4, float var5, float var6);
	virtual void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // MODELBASE_H