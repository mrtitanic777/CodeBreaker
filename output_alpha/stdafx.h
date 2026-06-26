// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// C4355: 'this' used in base-member init list -- intentional/safe here (see note in
// the other stdafx template). Silence project-wide.
#pragma warning(disable: 4355)

#include <xtl.h>
#include <xboxmath.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xaudio2.h>
#include <xgraphics.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <malloc.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>

#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion float/double
#pragma warning(disable: 4305) // truncation double to float

// Xbox 360 equivalents of Java standard library
typedef unsigned char byte;
#define null NULL

// Forward declarations for globals
extern IDirect3D9*       g_pD3D;
extern IDirect3DDevice9* g_pd3dDevice;
