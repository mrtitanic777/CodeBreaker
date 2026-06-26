#ifndef IPROGRESSUPDATE_H
#define IPROGRESSUPDATE_H

// Auto-generated from IProgressUpdate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class IProgressUpdate {
public:
	virtual ~IProgressUpdate() {}
	virtual void func_594_b(const std::string& var1) = 0;
	virtual void displayLoadingString(const std::string& var1) = 0;
	virtual void setLoadingProgress(int var1) = 0;
};

#endif // IPROGRESSUPDATE_H