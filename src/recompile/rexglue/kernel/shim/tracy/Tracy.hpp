// tracy/Tracy.hpp - SHIM (no-op) for the vendored DXBC translator.
//
// Tracy is a runtime profiler; the translator only annotates zones with it. In
// the vendored/standalone build there is no profiler, so every Tracy macro
// compiles to nothing.
#pragma once

#define ZoneScoped
#define ZoneScopedN(x)
#define ZoneScopedC(x)
#define ZoneScopedNC(x, c)
#define ZoneName(x, y)
#define ZoneText(x, y)
#define ZoneValue(x)
#define FrameMark
#define FrameMarkNamed(x)
#define FrameMarkStart(x)
#define FrameMarkEnd(x)
#define TracyMessage(x, y)
#define TracyMessageL(x)
#define TracyPlot(x, y)
#define TracyLockable(type, var) type var
#define TracyCZoneCtx int
#define TracyAlloc(p, s)
#define TracyFree(p)
