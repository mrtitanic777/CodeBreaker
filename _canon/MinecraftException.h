#ifndef MINECRAFTEXCEPTION_H
#define MINECRAFTEXCEPTION_H

// Auto-generated from MinecraftException.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RuntimeException.h"

class MinecraftException : public RuntimeException {
public:
	MinecraftException(const JavaString& var1);
	virtual ~MinecraftException();

	static void initStatics();
};

#endif // MINECRAFTEXCEPTION_H