#include "stdafx.h"
// Auto-generated from UnexpectedThrowable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "UnexpectedThrowable.h"
#include "Common.h"
#include "Throwable.h"
#include <string>

UnexpectedThrowable::UnexpectedThrowable()
	: m_description("")
	, m_exception(NULL)
{
}

UnexpectedThrowable::UnexpectedThrowable(const std::string& var1, Throwable* var2)
	: m_description("")
	, m_exception(NULL)
{
		m_description = var1;
		m_exception = var2;
}

UnexpectedThrowable::~UnexpectedThrowable() {
	m_exception = NULL;
}
