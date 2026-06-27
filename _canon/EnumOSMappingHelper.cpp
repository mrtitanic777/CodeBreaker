#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EnumOSMappingHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EnumOSMappingHelper.h"
#include "EnumOS2.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  public static final int[] osMappings = new int[EnumOS2.values().length];
 *
 *  static {
 *      try {
 *          osMappings[EnumOS2.a.ordinal()] = 1;
 *      } catch (NoSuchFieldError var4) {
 *      }
 *
 *      try {
 *          osMappings[EnumOS2.b.ordinal()] = 2;
 *      } catch (NoSuchFieldError var3) {
 *      }
 *
 *      try {
 *          osMappings[EnumOS2.c.ordinal()] = 3;
 *      } catch (NoSuchFieldError var2) {
 *      }
 *
 *      try {
 *          osMappings[EnumOS2.d.ordinal()] = 4;
 *      } catch (NoSuchFieldError var1) {
 *      }
 *  }
 *  ---- end original Java ----
 *  NOTE: the Java static initializer that populates osMappings[a..d] = 1..4 is
 *  dropped here; the array is left zero-initialized (only allocated, never filled). */
JArray<int > EnumOSMappingHelper::s_osMappings = JArray<int >(5);
// restored Java static{} block (was dropped -> osMappings all zero -> OS detection broken).
// EnumOS2 constants ARE their ordinals here, so Java's .ordinal() is dropped.
namespace {
	struct _InitOsMappings {
		_InitOsMappings() {
			EnumOSMappingHelper::s_osMappings[EnumOS2::a] = 1;
			EnumOSMappingHelper::s_osMappings[EnumOS2::b] = 2;
			EnumOSMappingHelper::s_osMappings[EnumOS2::c] = 3;
			EnumOSMappingHelper::s_osMappings[EnumOS2::d] = 4;
		}
	};
	_InitOsMappings _initOsMappings_instance;
}

EnumOSMappingHelper::~EnumOSMappingHelper()
{
}
