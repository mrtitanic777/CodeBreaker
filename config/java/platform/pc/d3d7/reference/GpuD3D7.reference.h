// GpuD3D7.h -- the GL layer's Direct3D 7 backend (GpuD3D7.cpp), the PC built with --graphics d3d7 (a 32-bit program: Windows
// has Direct3D 7 for 32-bit programs only). The platform's Display makes DirectDraw, the render target and the device, and hands
// them over here; the backend opens and closes the device's scene itself, around what it draws.
#ifndef GPU_D3D7_H
#define GPU_D3D7_H

#include "stdafx.h"
#include "GL11.h"
#include "Gpu.h"

struct IDirectDraw7;
struct IDirect3D7;
struct IDirect3DDevice7;
struct IDirectDrawSurface7;

namespace j2xgl {
namespace gpu {
	// DirectDraw (textures are its surfaces), Direct3D (vertex buffers), the device, its render target (w x h); systemMemory: the
	// software device, whose textures and buffers live in system memory.
	bool attachD3D7(IDirectDraw7* dd, IDirect3D7* d3d, IDirect3DDevice7* device, IDirectDrawSurface7* target, int w, int h, bool systemMemory);
	void setTargetD3D7(IDirectDrawSurface7* target, int w, int h);   // after the Display made new targets (a resized window)
	void endSceneD3D7();   // before the frame is shown, the targets change or the device goes away: the open scene closed
	void detachD3D7();     // releases what the backend made (Display's destroy)
}
}

#endif // GPU_D3D7_H
