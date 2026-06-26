#ifndef CLIPPINGHELPERIMPLEMENTATION_H
#define CLIPPINGHELPERIMPLEMENTATION_H

// Auto-generated from ClippingHelperImplementation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelper.h"

class ClippingHelperImplementation : public ClippingHelper {
public:
	ClippingHelperImplementation();
	virtual ~ClippingHelperImplementation();

	static ClippingHelper* func_1155_a();

public: // was private
	static ClippingHelperImplementation* s_field_1694_e;
	float* /* PORT: was FloatBuffer */ m_field_1693_f;
	float* /* PORT: was FloatBuffer */ m_field_1692_g;
	float* /* PORT: was FloatBuffer */ m_field_1691_h;

	void func_1154_a(float** var1, int var2);
	void func_1153_b();

};

#endif // CLIPPINGHELPERIMPLEMENTATION_H