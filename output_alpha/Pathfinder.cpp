#include "stdafx.h"
// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Pathfinder.h"
#include "Common.h"
#include "Entity.h"
#include "IBlockAccess.h"
#include "MCHashTable.h"
#include "Path.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Material.h"
#include "World.h"

Pathfinder::Pathfinder()
	: m_worldMap(NULL)
	, m_path(new Path())
	, m_pointMap(new MCHashTable())
	, m_pathOptions(new PathPoint*[32])
{
}

Pathfinder::Pathfinder(IBlockAccess* var1)
	: m_worldMap(NULL)
	, m_path(new Path())
	, m_pointMap(new MCHashTable())
	, m_pathOptions(new PathPoint*[32])
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
		return createEntityPathTo(var1, var2->posX, var2->boundingBox.minY, var2->posZ, var3);
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5) {
		return createEntityPathTo(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), var5);
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, double var2, double var4, double var6, float var8) {
		m_path->clearPath();
		m_pointMap->clearMap();
		PathPoint* var9 = openPoint((int)floor(var1->boundingBox.minX), (int)floor(var1->boundingBox.minY), (int)floor(var1->boundingBox.minZ));
		PathPoint* var10 = openPoint((int)floor(var2 - (double)(var1->width / 2.0F)), (int)floor(var4), (int)floor(var6 - (double)(var1->width / 2.0F)));
		PathPoint* var11 = new PathPoint((int)floorf(var1->width + 1.0F), (int)floorf(var1->height + 1.0F), (int)floorf(var1->width + 1.0F));
		PathEntity* var12 = addToPath(var1, var9, var10, var11, var8);
		return var12;
}

PathEntity* Pathfinder::addToPath(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5) {
		var2->m_totalPathDistance = 0.0F;
		var2->m_distanceToNext = var2->distanceTo(var3);
		var2->m_distanceToTarget = var2->m_distanceToNext;
		m_path->clearPath();
		m_path->addPoint(var2);
		PathPoint* var6 = var2;

		while(!m_path->isPathEmpty()) {
			PathPoint* var7 = m_path->dequeue();
			if(var7->m_hash == var3->m_hash) {
				return createEntityPath(var2, var3);
			}

			if(var7->distanceTo(var3) < var6->distanceTo(var3)) {
				var6 = var7;
			}

			var7->m_isFirst = true;
			int var8 = findPathOptions(var1, var7, var4, var3, var5);

			for(int var9 = 0; var9 < var8; ++var9) {
				PathPoint* var10 = m_pathOptions[var9];
				float var11 = var7->m_totalPathDistance + var7->distanceTo(var10);
				if(!var10->isAssigned() || var11 < var10->m_totalPathDistance) {
					var10->m_previous = var7;
					var10->m_totalPathDistance = var11;
					var10->m_distanceToNext = var10->distanceTo(var3);
					if(var10->isAssigned()) {
						m_path->changeDistance(var10, var10->m_totalPathDistance + var10->m_distanceToNext);
					} else {
						var10->m_distanceToTarget = var10->m_totalPathDistance + var10->m_distanceToNext;
						m_path->addPoint(var10);
					}
				}
			}
		}

		if(var6 == var2) {
			return NULL;
		} else {
			return createEntityPath(var2, var6);
		}
}

int Pathfinder::findPathOptions(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5) {
		int var6 = 0;
		unsigned char var7 = 0;
		if(getVerticalOffset(var1, var2->xCoord, var2->yCoord + 1, var2->zCoord, var3) > 0) {
			var7 = 1;
		}

		PathPoint* var8 = getSafePoint(var1, var2->xCoord, var2->yCoord, var2->zCoord + 1, var3, var7);
		PathPoint* var9 = getSafePoint(var1, var2->xCoord - 1, var2->yCoord, var2->zCoord, var3, var7);
		PathPoint* var10 = getSafePoint(var1, var2->xCoord + 1, var2->yCoord, var2->zCoord, var3, var7);
		PathPoint* var11 = getSafePoint(var1, var2->xCoord, var2->yCoord, var2->zCoord - 1, var3, var7);
		if(var8 != NULL && !var8->m_isFirst && var8->distanceTo(var4) < var5) {
			m_pathOptions[var6++] = var8;
		}

		if(var9 != NULL && !var9->m_isFirst && var9->distanceTo(var4) < var5) {
			m_pathOptions[var6++] = var9;
		}

		if(var10 != NULL && !var10->m_isFirst && var10->distanceTo(var4) < var5) {
			m_pathOptions[var6++] = var10;
		}

		if(var11 != NULL && !var11->m_isFirst && var11->distanceTo(var4) < var5) {
			m_pathOptions[var6++] = var11;
		}

		return var6;
}

PathPoint* Pathfinder::getSafePoint(Entity* var1, int var2, int var3, int var4, PathPoint* var5, int var6) {
		PathPoint* var7 = NULL;
		if(getVerticalOffset(var1, var2, var3, var4, var5) > 0) {
			var7 = openPoint(var2, var3, var4);
		}

		if(var7 == NULL && getVerticalOffset(var1, var2, var3 + var6, var4, var5) > 0) {
			var7 = openPoint(var2, var3 + var6, var4);
			var3 += var6;
		}

		if(var7 != NULL) {
			int var8 = 0;

			for(bool var9 = false; var3 > 0; --var3) {
				int var10 = getVerticalOffset(var1, var2, var3 - 1, var4, var5);
				if(var10 <= 0) {
					break;
				}

				if(var10 < 0) {
					return NULL;
				}

				++var8;
				if(var8 >= 4) {
					return NULL;
				}
			}

			if(var3 > 0) {
				var7 = openPoint(var2, var3, var4);
			}
		}

		return var7;
}

PathPoint* Pathfinder::openPoint(int var1, int var2, int var3) {
		int var4 = var1 | var2 << 10 | var3 << 20;
		PathPoint* var5 = (PathPoint*)m_pointMap->lookup(var4);
		if(var5 == NULL) {
			var5 = new PathPoint(var1, var2, var3);
			m_pointMap->addKey(var4, var5);
		}

		return var5;
}

int Pathfinder::getVerticalOffset(Entity* var1, int var2, int var3, int var4, PathPoint* var5) {
		for(int var6 = var2; var6 < var2 + var5->xCoord; ++var6) {
			for(int var7 = var3; var7 < var3 + var5->yCoord; ++var7) {
				for(int var8 = var4; var8 < var4 + var5->zCoord; ++var8) {
					Material* var9 = m_worldMap->getBlockMaterial(var2, var3, var4);
					if(var9->func_880_c()) {
						return 0;
					}

					if(var9 == Material::s_water || var9 == Material::s_lava) {
						return -1;
					}
				}
			}
		}

		return 1;
}

PathEntity* Pathfinder::createEntityPath(PathPoint* var1, PathPoint* var2) {
		int var3 = 1;

		PathPoint* var4 = NULL;
		for(var4 = var2; var4->m_previous != NULL; var4 = var4->m_previous) {
			++var3;
		}

		PathPoint** var5 = new PathPoint*[var3];
		int var5_count = var3;
		var4 = var2;
		--var3;

		for(var5[var3] = var2; var4->m_previous != NULL; var5[var3] = var4) {
			var4 = var4->m_previous;
			--var3;
		}

		return new PathEntity(var5);
}
