#ifndef IPROGRESSUPDATE_H
#define IPROGRESSUPDATE_H

// Auto-generated from IProgressUpdate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class IProgressUpdate {
public:

	virtual void displayProgressMessage(const JavaString& var1) = 0;
	virtual void displayLoadingString(const JavaString& var1) = 0;
	virtual void setLoadingProgress(int var1) = 0;
};

#endif // IPROGRESSUPDATE_H