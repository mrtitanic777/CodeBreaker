#ifndef LOADINGSCREENRENDERER_H
#define LOADINGSCREENRENDERER_H

// Auto-generated from LoadingScreenRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IProgressUpdate.h"
#include <string>

class InputStream;
class Minecraft;
class NBTTagCompound;

class LoadingScreenRenderer : public IProgressUpdate {
public:
	LoadingScreenRenderer(Minecraft* var1);
	LoadingScreenRenderer();
	virtual ~LoadingScreenRenderer();

	virtual void displayProgressMessage(const std::string& var1);
	virtual void displayLoadingString(const std::string& var1);
	virtual void setLoadingProgress(int var1);
	static NBTTagCompound* writeLevelTags(InputStream* var0);

private:
	std::string m_text;
	Minecraft* mc;
	std::string m_title;
	long long m_start;

};

#endif // LOADINGSCREENRENDERER_H