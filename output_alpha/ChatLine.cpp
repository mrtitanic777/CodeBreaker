#include "stdafx.h"
// Auto-generated from ChatLine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChatLine.h"
#include "Common.h"
#include <string>

ChatLine::ChatLine()
	: m_message("")
	, m_updateCounter(0)
{
}

ChatLine::ChatLine(const std::string& var1)
	: m_message("")
	, m_updateCounter(0)
{
		m_message = var1;
		m_updateCounter = 0;
}

ChatLine::~ChatLine() {
	// No dynamic resources
}
