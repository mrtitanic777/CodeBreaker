#ifndef UNEXPECTEDTHROWABLE_H
#define UNEXPECTEDTHROWABLE_H

// Auto-generated from UnexpectedThrowable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Throwable;

class UnexpectedThrowable {
public:
	UnexpectedThrowable(const JavaString& var1, Throwable* var2);
	virtual ~UnexpectedThrowable();

	JavaString description;
	Throwable* exception;

	static void initStatics();
};

#endif // UNEXPECTEDTHROWABLE_H