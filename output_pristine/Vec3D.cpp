#include "stdafx.h"
// Auto-generated from Vec3D.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Vec3D.h"
#include "Common.h"
#include <string>
#include "Vec3.h"

Vec3D::Vec3D()
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
{
}

Vec3D::Vec3D(float var1, float var2, float var3)
	: m_xCoord(0)
	, m_yCoord(0)
	, m_zCoord(0)
{
		m_xCoord = var1;
		m_yCoord = var2;
		m_zCoord = var3;
}

Vec3D::~Vec3D() {
	// No dynamic resources
}

Vec3D* Vec3D::subtract(Vec3D* var1) {
		return new Vec3::0f(m_xCoord - var1->m_xCoord, m_yCoord - var1->m_yCoord, m_zCoord - var1->m_zCoord);
}

Vec3D* Vec3D::normalize() {
		float var1 = sqrtf(m_xCoord * m_xCoord + m_yCoord * m_yCoord + m_zCoord * m_zCoord);
		return new Vec3::0f(m_xCoord / var1, m_yCoord / var1, m_zCoord / var1);
}

Vec3D* Vec3D::addVector(float var1, float var2, float var3) {
		return new Vec3::0f(m_xCoord + var1, m_yCoord + var2, m_zCoord + var3);
}

float Vec3D::distance(Vec3D* var1) {
		float var2 = var1->m_xCoord - m_xCoord;
		float var3 = var1->m_yCoord - m_yCoord;
		float var4 = var1->m_zCoord - m_zCoord;
		return sqrtf(var2 * var2 + var3 * var3 + var4 * var4);
}

float Vec3D::squareDistanceTo(Vec3D* var1) {
		float var2 = var1->m_xCoord - m_xCoord;
		float var3 = var1->m_yCoord - m_yCoord;
		float var4 = var1->m_zCoord - m_zCoord;
		return var2 * var2 + var3 * var3 + var4 * var4;
}

Vec3D* Vec3D::getIntermediateWithXValue(Vec3D* var1, float var2) {
		float var3 = var1->m_xCoord - m_xCoord;
		float var4 = var1->m_yCoord - m_yCoord;
		float var5 = var1->m_zCoord - m_zCoord;
		if(var3 * var3 < 1.0E-7F) {
			return NULL;
		} else {
			var2 = (var2 - m_xCoord) / var3;
			return var2 >= 0.0F && var2 <= 1.0F ? new Vec3::0f(m_xCoord + var3 * var2, m_yCoord + var4 * var2, m_zCoord + var5 * var2) : NULL;
		}
}

Vec3D* Vec3D::getIntermediateWithYValue(Vec3D* var1, float var2) {
		float var3 = var1->m_xCoord - m_xCoord;
		float var4 = var1->m_yCoord - m_yCoord;
		float var5 = var1->m_zCoord - m_zCoord;
		if(var4 * var4 < 1.0E-7F) {
			return NULL;
		} else {
			var2 = (var2 - m_yCoord) / var4;
			return var2 >= 0.0F && var2 <= 1.0F ? new Vec3::0f(m_xCoord + var3 * var2, m_yCoord + var4 * var2, m_zCoord + var5 * var2) : NULL;
		}
}

Vec3D* Vec3D::getIntermediateWithZValue(Vec3D* var1, float var2) {
		float var3 = var1->m_xCoord - m_xCoord;
		float var4 = var1->m_yCoord - m_yCoord;
		float var5 = var1->m_zCoord - m_zCoord;
		if(var5 * var5 < 1.0E-7F) {
			return NULL;
		} else {
			var2 = (var2 - m_zCoord) / var5;
			return var2 >= 0.0F && var2 <= 1.0F ? new Vec3::0f(m_xCoord + var3 * var2, m_yCoord + var4 * var2, m_zCoord + var5 * var2) : NULL;
		}
}

const std::string& Vec3D::toString() {
		return "(" + m_xCoord + ", " + m_yCoord + ", " + m_zCoord + ")";
}
