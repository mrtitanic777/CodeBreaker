// GpuD3D9.h -- the GL layer's Direct3D 9 backend (GpuD3D9.cpp): the Xbox 360, and the PC built with --graphics d3d9. The
// platform's Display makes the device and hands it over here; the frame's scene is the backend's, opened and closed through
// sceneBegin / sceneEnd so it can step out of it to make a resource (the 360 makes resources only out of a scene).
#ifndef GPU_D3D9_H
#define GPU_D3D9_H

#include "stdafx.h"
#include "GL11.h"     // (the d3d9 name the 360's platform files call these by)
#include "Gpu.h"

#if !defined(_XBOX) && !defined(DIRECT3D_VERSION)
// The PC: Direct3D stays out of the game's own headers (it needs GDI, and wingdi.h's Polygon() or Rectangle() would hide a game
// class of that name). The backend and the Display include it through Common.h.
struct IDirect3DDevice9;
#endif

namespace j2xgl {
	void glSetDevice(IDirect3DDevice9* dev);   // after CreateDevice and after every Reset (reads the backbuffer size)
	IDirect3DDevice9* device();
	void sceneBegin();                         // the frame's scene (Display: around each frame)
	void sceneEnd();
	bool inScene();
}

#endif // GPU_D3D9_H
