#include "stdafx.h"
// Auto-generated from RedstoneUpdateInfo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RedstoneUpdateInfo.h"
#include "Common.h"

RedstoneUpdateInfo::RedstoneUpdateInfo()
	: m_x(0)
	, m_y(0)
	, m_z(0)
	, m_updateTime(0)
{
}

RedstoneUpdateInfo::RedstoneUpdateInfo(int var1, int var2, int var3, long long var4)
	: m_x(0)
	, m_y(0)
	, m_z(0)
	, m_updateTime(0)
{
		m_x = var1;
		m_y = var2;
		m_z = var3;
		m_updateTime = var4;
}

RedstoneUpdateInfo::~RedstoneUpdateInfo() {
	// No dynamic resources
}
