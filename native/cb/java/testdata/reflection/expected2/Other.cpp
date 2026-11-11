#include "stdafx.h"
#include "ReflectionFactory.h"

TileEntity* load() { return (TileEntity*)TileEntity_createByReflection(x); }
