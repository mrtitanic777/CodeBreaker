#ifndef FRUSTRUM_H
#define FRUSTRUM_H

// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ICamera.h"

struct AxisAlignedBB;
class ClippingHelper;

class Frustrum : public ICamera {
public:
	Frustrum();
	virtual ~Frustrum();

	virtual void func_343_a(double var1, double var3, double var5);
	virtual bool func_344_a(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual bool func_342_a(AxisAlignedBB var1);

public: // was private
	ClippingHelper* m_field_593_a;
	double m_field_592_b;
	double m_field_595_c;
	double m_field_594_d;

};

#endif // FRUSTRUM_H