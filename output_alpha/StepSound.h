#ifndef STEPSOUND_H
#define STEPSOUND_H

// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class StepSound {
public:
	StepSound();
	StepSound(const std::string& var1, float var2, float var3);
	virtual ~StepSound();

	std::string m_field_1678_a;
	float m_field_1677_b;
	float m_field_1679_c;

	virtual float func_1147_b();
	virtual float func_1144_c();
	virtual std::string func_1146_a();
	virtual std::string func_1145_d();

};

#endif // STEPSOUND_H