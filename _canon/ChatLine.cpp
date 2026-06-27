#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChatLine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChatLine.h"

ChatLine::ChatLine(const JavaString& var1)
	: updateCounter(0)
{
	this->message = var1;
	this->updateCounter = 0;
}

ChatLine::~ChatLine()
{
}

void ChatLine::initStatics()
{
}
