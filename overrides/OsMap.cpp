#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from OsMap.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "OsMap.h"
#include "EnumOS1.h"

/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // The array length and the static{} initializer block below were dropped:
 *  // the C++ only allocates the array; it never fills osTypeOrdinals[] with the
 *  // per-OS ordinal->id mapping (linux=1, solaris=2, windows=3, macos=4), each
 *  // guarded by a try/catch(NoSuchFieldError) in the original.
 *  static final int[] osTypeOrdinals = new int[EnumOS1.values().length];
 *
 *  static {
 *      try {
 *          osTypeOrdinals[EnumOS1.linux.ordinal()] = 1;
 *      } catch (NoSuchFieldError) {
 *      }
 *
 *      try {
 *          osTypeOrdinals[EnumOS1.solaris.ordinal()] = 2;
 *      } catch (NoSuchFieldError) {
 *      }
 *
 *      try {
 *          osTypeOrdinals[EnumOS1.windows.ordinal()] = 3;
 *      } catch (NoSuchFieldError) {
 *      }
 *
 *      try {
 *          osTypeOrdinals[EnumOS1.macos.ordinal()] = 4;
 *      } catch (NoSuchFieldError) {
 *      }
 *  }
 *  ---- end original Java ---- */
JArray<int > OsMap::s_osTypeOrdinals;
// restored Java static{} block (was dropped -> osTypeOrdinals all zero). EnumOS1 constants ARE
// their ordinals here, so Java's .ordinal() is dropped. Run in order by j2x_initAll().
void OsMap::initStatics()
{
	OsMap::s_osTypeOrdinals = JArray<int >(5);
	OsMap::s_osTypeOrdinals[EnumOS1::linux] = 1;
	OsMap::s_osTypeOrdinals[EnumOS1::solaris] = 2;
	OsMap::s_osTypeOrdinals[EnumOS1::windows] = 3;
	OsMap::s_osTypeOrdinals[EnumOS1::macos] = 4;
}

OsMap::~OsMap()
{
}
