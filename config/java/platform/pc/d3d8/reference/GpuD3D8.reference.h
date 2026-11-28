// GpuD3D8.h -- the GL layer's Direct3D 8 backend (GpuD3D8.cpp), the PC built with --graphics d3d8 (a 32-bit program: Windows
// has Direct3D 8 for 32-bit programs only). The platform's Display makes the device and hands it over here; the backend opens
// and closes the device's scene itself, around what it draws.
#ifndef GPU_D3D8_H
#define GPU_D3D8_H

#include "stdafx.h"
#include "GL11.h"
#include "Gpu.h"

namespace d3d8 { struct IDirect3DDevice8; }

namespace j2xgl {
namespace gpu {
	bool attachD3D8(d3d8::IDirect3DDevice8* device, int w, int h);   // the device and its back buffer's size
	void resizedD3D8(int w, int h);    // after a Reset to a new back buffer size
	void endSceneD3D8();               // before Present, Reset or the device going away: the open scene closed
	void detachD3D8();                 // releases what the backend made (Display's destroy)
}
}

#endif // GPU_D3D8_H
