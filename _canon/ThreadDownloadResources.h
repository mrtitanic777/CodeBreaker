#ifndef THREADDOWNLOADRESOURCES_H
#define THREADDOWNLOADRESOURCES_H

// Auto-generated from ThreadDownloadResources.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Thread.h"

class File;
class Minecraft;
class URL;

class ThreadDownloadResources : public Thread {
public:
	ThreadDownloadResources(File* var1, Minecraft* var2);
	virtual ~ThreadDownloadResources();

	File* resourcesFolder;
	Minecraft* mc;
	bool closing;

	virtual void run();
	virtual void reloadResources();
	virtual void loadResource(File* var1, const JavaString& var2);
	virtual void downloadAndInstallResource(URL* var1, const JavaString& var2, long long var3, int var5);
	virtual void downloadResource(URL* var1, File* var2, long long var3);
	virtual void closeMinecraft();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ThreadDownloadResources(const ThreadDownloadResources&);
	ThreadDownloadResources& operator=(const ThreadDownloadResources&);
};

#endif // THREADDOWNLOADRESOURCES_H