#ifndef CLIPPINGHELPERIMPLEMENTATION_H
#define CLIPPINGHELPERIMPLEMENTATION_H

// Auto-generated from ClippingHelperImplementation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ClippingHelper.h"

class FloatBuffer;

class ClippingHelperImplementation : public ClippingHelper {
public:
	virtual ~ClippingHelperImplementation();

	FloatBuffer* projectionBuffer;
	FloatBuffer* modelviewBuffer;
	FloatBuffer* modelviewProjectionBuffer;
	static ClippingHelperImplementation* s_instance;

	static ClippingHelper* getInstance();
	virtual void normalize(JArray<JArray<float > > var1, int var2);
	virtual void init();
};

#endif // CLIPPINGHELPERIMPLEMENTATION_H