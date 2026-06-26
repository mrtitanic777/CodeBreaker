#include "stdafx.h"
// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Pathfinder.h"
#include "Common.h"
#include "Entity.h"
#include "Map.h"
#include "Path.h"
#include "PathEntity.h"
#include "World.h"
#include "Material.h"
#include "PathPoint.h"

Pathfinder::Pathfinder()
	: m_worldMap(NULL)
	, m_path(new Path())
	, m_pointMap(NULL)
	, m_pathOptions(new PathPoint[32])
{
}

Pathfinder::Pathfinder(World* var1)
	: m_worldMap(NULL)
	, m_path(new Path())
	, m_pointMap(NULL)
	, m_pathOptions(new PathPoint[32])
{
		m_worldMap = var1;
}

Pathfinder::~Pathfinder() {
	m_worldMap = NULL;
	m_path = NULL;
	m_pointMap = NULL;
	delete[] m_pathOptions;
	m_pathOptions = NULL;
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, Entity* var2, float var3) {
		return addToPath(var1, var2->posX, var2->boundingBox.minY, var2->posZ, 16.0F);
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5) {
		return addToPath(var1, (float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, 16.0F);
}

PathEntity* Pathfinder::addToPath(Entity* var1, float var2, float var3, float var4, float var5) {
		m_path->clearPath();
		m_pointMap->clear();
		PathPoint var6 = openPoint((int)var1->boundingBox.minX, (int)var1->boundingBox.minY, (int)var1->boundingBox.minZ);
		PathPoint var22 = openPoint((int)(var2 - var1->width / 2.0F), (int)var3, (int)(var4 - var1->width / 2.0F));
		PathPoint var24 = PathPoint((int)(var1->width + 1.0F), (int)(var1->height + 1.0F), (int)(var1->width + 1.0F));
		float var27 = var5;
		PathPoint var26 = var24;
		PathPoint var25 = var22;
		Entity var23 = var1;
		Pathfinder var20 = this;
		var6.m_totalPathDistance = 0.0F;
		var6.m_distanceToNext = var6.distanceTo(var22);
		var6.m_distanceToTarget = var6.m_distanceToNext;
		m_path->clearPath();
		m_path->addPoint(var6);
		PathPoint var7 = var6;

		PathEntity var10000;
		while(true) {
			if(var20.m_path.isPathEmpty()) {
				var10000 = var7 == var6 ? NULL : createEntityPath(var7);
				break;
			}

			PathPoint var8 = var20.m_path.b();
			if(var8.m_hash == var25.m_hash) {
				var10000 = createEntityPath(var25);
				break;
			}

			if(var8.distanceTo(var25) < var7.distanceTo(var25)) {
				var7 = var8;
			}

			var8.m_isFirst = true;
			int var15 = 0;
			unsigned char var16 = 0;
			if(var20.getVerticalOffset(var8.m_xCoord, var8.m_yCoord + 1, var8.m_zCoord, var26) > 0) {
				var16 = 1;
			}

			PathPoint var17 = var20.a(var23, var8.m_xCoord, var8.m_yCoord, var8.m_zCoord + 1, var26, var16);
			PathPoint var18 = var20.a(var23, var8.m_xCoord - 1, var8.m_yCoord, var8.m_zCoord, var26, var16);
			PathPoint var19 = var20.a(var23, var8.m_xCoord + 1, var8.m_yCoord, var8.m_zCoord, var26, var16);
			PathPoint var10 = var20.a(var23, var8.m_xCoord, var8.m_yCoord, var8.m_zCoord - 1, var26, var16);
			if(var17 != NULL && !var17.m_isFirst && var17.distanceTo(var25) < var27) {
				++var15;
				var20.m_pathOptions[0] = var17;
			}

			if(var18 != NULL && !var18.m_isFirst && var18.distanceTo(var25) < var27) {
				var20.m_pathOptions[var15++] = var18;
			}

			if(var19 != NULL && !var19.m_isFirst && var19.distanceTo(var25) < var27) {
				var20.m_pathOptions[var15++] = var19;
			}

			if(var10 != NULL && !var10.m_isFirst && var10.distanceTo(var25) < var27) {
				var20.m_pathOptions[var15++] = var10;
			}

			int var9 = var15;

			for(int var28 = 0; var28 < var9; ++var28) {
				PathPoint var11 = var20.m_pathOptions[var28];
				float var12 = var8.m_totalPathDistance + var8.distanceTo(var11);
				if(!var11.isAssigned() || var12 < var11.m_totalPathDistance) {
					var11.m_previous = var8;
					var11.m_totalPathDistance = var12;
					var11.m_distanceToNext = var11.distanceTo(var25);
					if(var11.isAssigned()) {
						var20.m_path.changeDistance(var11, var11.m_totalPathDistance + var11.m_distanceToNext);
					} else {
						var11.m_distanceToTarget = var11.m_totalPathDistance + var11.m_distanceToNext;
						var20.m_path.addPoint(var11);
					}
				}
			}
		}

		PathEntity var21 = var10000;
		return var21;
}

PathPoint Pathfinder::a(Entity* var1, int var2, int var3, int var4, const PathPoint& var5, int var6) {
		PathPoint var8 = NULL;
		if(getVerticalOffset(var2, var3, var4, var5) > 0) {
			var8 = openPoint(var2, var3, var4);
		}

		if(var8 == NULL && getVerticalOffset(var2, var3 + var6, var4, var5) > 0) {
			var8 = openPoint(var2, var3 + var6, var4);
		}

		if(var8 != NULL) {
			for(var6 = 0; var3 > 0; var8 = openPoint(var2, var3, var4)) {
				int var7 = getVerticalOffset(var2, var3 - 1, var4, var5);
				if(var7 <= 0) {
					break;
				}

				if(var7 < 0) {
					return NULL;
				}

				++var6;
				if(var6 >= 4) {
					return NULL;
				}

				--var3;
			}
		}

		return var8;
}

PathPoint Pathfinder::openPoint(int var1, int var2, int var3) {
		int var4 = var1 | var2 << 10 | var3 << 20;
		PathPoint var5 = (PathPoint)m_pointMap->get(var4);
		if(var5 == NULL) {
			var5 = PathPoint(var1, var2, var3);
			// PORT-TODO: m_pointMap->put(var4, var5);
		}

		return var5;
}

int Pathfinder::getVerticalOffset(int var1, int var2, int var3, const PathPoint& var4) {
		for(int var5 = var1; var5 < var1 + var4.m_xCoord; ++var5) {
			if(var5 < 0 || var5 >= m_worldMap->width) {
				return 0;
			}

			for(int var6 = var2; var6 < var2 + var4.m_yCoord; ++var6) {
				if(var6 < 0 || var6 >= m_worldMap->height) {
					return 0;
				}

				int var7 = var3;

				while(var7 < var3 + var4.m_zCoord) {
					if(var7 >= 0 && var7 < m_worldMap_count /* PORT: set this from allocation */) {
						Material* var8 = m_worldMap->getBlockMaterial(var1, var2, var3);
						if(var8->getIsSolid()) {
							return 0;
						}

						if(var8 != Material::s_water && var8 != Material::s_lava) {
							++var7;
							continue;
						}

						return -1;
					}

					return 0;
				}
			}
		}

		return 1;
}

PathEntity* Pathfinder::createEntityPath(const PathPoint& var0) {
		int var1 = 1;

		PathPoint var2;
		for(var2 = var0; var2.m_previous != NULL; var2 = var2.m_previous) {
			++var1;
		}

		PathPoint* var3 = new PathPoint[var1];
		var2 = var0;
		--var1;

		for(var3[var1] = var0; var2.m_previous != NULL; var3[var1] = var2) {
			var2 = var2.m_previous;
			--var1;
		}

		return new PathEntity(var3);
}
