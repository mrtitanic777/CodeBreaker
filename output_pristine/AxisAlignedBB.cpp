#include "stdafx.h"
// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "AxisAlignedBB.h"
#include "Common.h"
#include "MovingObjectPosition.h"
#include "Vec3D.h"

AxisAlignedBB::AxisAlignedBB()
	: m_epsilon(0.0f)
	, minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
{
}

AxisAlignedBB::AxisAlignedBB(float var1, float var2, float var3, float var4, float var5, float var6)
	: m_epsilon(0.0f)
	, minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
{
		minX = var1;
		minY = var2;
		minZ = var3;
		maxX = var4;
		maxY = var5;
		maxZ = var6;
}

AxisAlignedBB::~AxisAlignedBB() {
	// No dynamic resources
}

AxisAlignedBB AxisAlignedBB::addCoord(float var1, float var2, float var3) {
		float var4 = minX;
		float var5 = minY;
		float var6 = minZ;
		float var7 = maxX;
		float var8 = maxY;
		float var9 = maxZ;
		if(var1 < 0.0F) {
			var4 += var1;
		}

		if(var1 > 0.0F) {
			var7 += var1;
		}

		if(var2 < 0.0F) {
			var5 += var2;
		}

		if(var2 > 0.0F) {
			var8 += var2;
		}

		if(var3 < 0.0F) {
			var6 += var3;
		}

		if(var3 > 0.0F) {
			var9 += var3;
		}

		return AxisAlignedBB(var4, var5, var6, var7, var8, var9);
}

AxisAlignedBB AxisAlignedBB::expand(float var1, float var2, float var3) {
		if(minY > maxY) {
			DbgPrintF("ERROR [IllegalArgumentException]: %s\n", "NOOOOOO!");
		} else {
			float var4 = minX - var1;
			float var5 = minY - var2;
			float var6 = minZ - var3;
			var1 += maxX;
			var2 += maxY;
			var3 += maxZ;
			return AxisAlignedBB(var4, var5, var6, var1, var2, var3);
		}
}

float AxisAlignedBB::calculateXOffset(AxisAlignedBB var1, float var2) {
		if(var1.maxY > minY && var1.minY < maxY) {
			if(var1.maxZ > minZ && var1.minZ < maxZ) {
				float var3;
				if(var2 > 0.0F && var1.maxX <= minX) {
					var3 = minX - var1.maxX;
					if(var3 < var2) {
						var2 = var3;
					}
				}

				if(var2 < 0.0F && var1.minX >= maxX) {
					var3 = maxX - var1.minX;
					if(var3 > var2) {
						var2 = var3;
					}
				}

				return var2;
			} else {
				return var2;
			}
		} else {
			return var2;
		}
}

float AxisAlignedBB::calculateYOffset(AxisAlignedBB var1, float var2) {
		if(var1.maxX > minX && var1.minX < maxX) {
			if(var1.maxZ > minZ && var1.minZ < maxZ) {
				float var3;
				if(var2 > 0.0F && var1.maxY <= minY) {
					var3 = minY - var1.maxY;
					if(var3 < var2) {
						var2 = var3;
					}
				}

				if(var2 < 0.0F && var1.minY >= maxY) {
					var3 = maxY - var1.minY;
					if(var3 > var2) {
						var2 = var3;
					}
				}

				return var2;
			} else {
				return var2;
			}
		} else {
			return var2;
		}
}

float AxisAlignedBB::calculateZOffset(AxisAlignedBB var1, float var2) {
		if(var1.maxX > minX && var1.minX < maxX) {
			if(var1.maxY > minY && var1.minY < maxY) {
				float var3;
				if(var2 > 0.0F && var1.maxZ <= minZ) {
					var3 = minZ - var1.maxZ;
					if(var3 < var2) {
						var2 = var3;
					}
				}

				if(var2 < 0.0F && var1.minZ >= maxZ) {
					var3 = maxZ - var1.minZ;
					if(var3 > var2) {
						var2 = var3;
					}
				}

				return var2;
			} else {
				return var2;
			}
		} else {
			return var2;
		}
}

bool AxisAlignedBB::intersectsWith(AxisAlignedBB var1) {
		return var1.maxX >= minX && var1.minX <= maxX ? (var1.maxY >= minY && var1.minY <= maxY ? var1.maxZ >= minZ && var1.minZ <= maxZ : false) : false;
}

void AxisAlignedBB::offset(float var1, float var2, float var3) {
		minX += var1;
		minY += var2;
		minZ += var3;
		maxX += var1;
		maxY += var2;
		maxZ += var3;
}

AxisAlignedBB AxisAlignedBB::copy() {
		return AxisAlignedBB(minX, minY, minZ, maxX, maxY, maxZ);
}

MovingObjectPosition* AxisAlignedBB::calculateIntercept(Vec3D* var1, Vec3D* var2) {
		Vec3.0f var3 = var1->getIntermediateWithXValue(var2, minX);
		Vec3.0f var4 = var1->getIntermediateWithXValue(var2, maxX);
		Vec3.0f var5 = var1->getIntermediateWithYValue(var2, minY);
		Vec3.0f var6 = var1->getIntermediateWithYValue(var2, maxY);
		Vec3.0f var7 = var1->getIntermediateWithZValue(var2, minZ);
		var2 = var1->getIntermediateWithZValue(var2, maxZ);
		if(!isVecInYZ(var3)) {
			var3 = NULL;
		}

		if(!isVecInYZ(var4)) {
			var4 = NULL;
		}

		if(!isVecInXZ(var5)) {
			var5 = NULL;
		}

		if(!isVecInXZ(var6)) {
			var6 = NULL;
		}

		if(!isVecInXY(var7)) {
			var7 = NULL;
		}

		if(!isVecInXY(var2)) {
			var2 = NULL;
		}

		Vec3.0f var8 = NULL;
		if(var3 != NULL) {
			var8 = var3;
		}

		if(var4 != NULL && (var8 == NULL || var1->squareDistanceTo(var4) < var1->squareDistanceTo(var8))) {
			var8 = var4;
		}

		if(var5 != NULL && (var8 == NULL || var1->squareDistanceTo(var5) < var1->squareDistanceTo(var8))) {
			var8 = var5;
		}

		if(var6 != NULL && (var8 == NULL || var1->squareDistanceTo(var6) < var1->squareDistanceTo(var8))) {
			var8 = var6;
		}

		if(var7 != NULL && (var8 == NULL || var1->squareDistanceTo(var7) < var1->squareDistanceTo(var8))) {
			var8 = var7;
		}

		if(var2 != NULL && (var8 == NULL || var1->squareDistanceTo(var2) < var1->squareDistanceTo(var8))) {
			var8 = var2;
		}

		if(var8 == NULL) {
			return NULL;
		} else {
			unsigned char var9 = -1;
			if(var8 == var3) {
				var9 = 4;
			}

			if(var8 == var4) {
				var9 = 5;
			}

			if(var8 == var5) {
				var9 = 0;
			}

			if(var8 == var6) {
				var9 = 1;
			}

			if(var8 == var7) {
				var9 = 2;
			}

			if(var8 == var2) {
				var9 = 3;
			}

			return new MovingObjectPosition(0, 0, 0, var9, var8);
		}
}

bool AxisAlignedBB::isVecInYZ(Vec3D* var1) {
		return var1 == NULL ? false : var1->m_yCoord >= minY && var1->m_yCoord <= maxY && var1->m_zCoord >= minZ && var1->m_zCoord <= maxZ;
}

bool AxisAlignedBB::isVecInXZ(Vec3D* var1) {
		return var1 == NULL ? false : var1->m_xCoord >= minX && var1->m_xCoord <= maxX && var1->m_zCoord >= minZ && var1->m_zCoord <= maxZ;
}

bool AxisAlignedBB::isVecInXY(Vec3D* var1) {
		return var1 == NULL ? false : var1->m_xCoord >= minX && var1->m_xCoord <= maxX && var1->m_yCoord >= minY && var1->m_yCoord <= maxY;
}
