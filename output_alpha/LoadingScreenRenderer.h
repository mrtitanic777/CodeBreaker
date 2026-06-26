#ifndef LOADINGSCREENRENDERER_H
#define LOADINGSCREENRENDERER_H

// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IProgressUpdate.h"
#include <string>

class Minecraft;

class LoadingScreenRenderer : public IProgressUpdate {
public:
	LoadingScreenRenderer();
	LoadingScreenRenderer(Minecraft* var1);
	virtual ~LoadingScreenRenderer();

	virtual void func_596_a(const std::string& var1);
	virtual void func_594_b(const std::string& var1);
	virtual void func_597_c(const std::string& var1);
	virtual void displayLoadingString(const std::string& var1);
	virtual void setLoadingProgress(int var1);

public: // was private
	std::string m_field_1004_a;
	Minecraft* mc;
	std::string m_field_1007_c;
	long long m_field_1006_d;
	bool m_field_1005_e;

};

#endif // LOADINGSCREENRENDERER_H