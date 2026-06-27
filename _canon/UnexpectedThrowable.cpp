#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from UnexpectedThrowable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "UnexpectedThrowable.h"
#include "Throwable.h"

UnexpectedThrowable::UnexpectedThrowable(const JavaString& var1, Throwable* var2)
	: exception(NULL)
{
	this->description = var1;
	this->exception = var2;
}

UnexpectedThrowable::~UnexpectedThrowable()
{
}

void UnexpectedThrowable::initStatics()
{
}
