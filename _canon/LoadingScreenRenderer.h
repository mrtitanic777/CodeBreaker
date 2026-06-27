#ifndef LOADINGSCREENRENDERER_H
#define LOADINGSCREENRENDERER_H

// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IProgressUpdate.h"

class Minecraft;

class LoadingScreenRenderer : public IProgressUpdate {
public:
	LoadingScreenRenderer(Minecraft* var1);
	virtual ~LoadingScreenRenderer();

	JavaString message;
	Minecraft* mc;
	JavaString title;
	long long systemTime;
	bool doneWorking;

	virtual void func_596_a(const JavaString& var1);
	virtual void displayProgressMessage(const JavaString& var1);
	virtual void func_597_c(const JavaString& var1);
	virtual void displayLoadingString(const JavaString& var1);
	virtual void setLoadingProgress(int var1);

	static void initStatics();
};

#endif // LOADINGSCREENRENDERER_H