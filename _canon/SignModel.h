#ifndef SIGNMODEL_H
#define SIGNMODEL_H

// Auto-generated from SignModel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ModelRenderer;

class SignModel {
public:
	SignModel();
	virtual ~SignModel();

	ModelRenderer* signBoard;
	ModelRenderer* signPost;

	virtual void renderSign();

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	SignModel(const SignModel&);
	SignModel& operator=(const SignModel&);
};

#endif // SIGNMODEL_H