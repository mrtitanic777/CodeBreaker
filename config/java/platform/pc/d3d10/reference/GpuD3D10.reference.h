// GpuD3D10.h -- the GL layer's Direct3D 10 backend (GpuD3D10.cpp), the PC built with --graphics d3d10. The platform's Display
// makes the device, the swap chain and the views, and hands them over here; it calls setTargets again after every resize.
#ifndef GPU_D3D10_H
#define GPU_D3D10_H

#include "stdafx.h"
#include "GL11.h"
#include "Gpu.h"

struct ID3D10Device;
struct ID3D10RenderTargetView;
struct ID3D10DepthStencilView;
struct ID3D10Texture2D;

namespace j2xgl {
namespace gpu {
	bool attachD3D10(ID3D10Device* device);   // builds the shaders; false if they cannot be
	// The frame's colour target (and its texture, for reading the frame back) and depth target, w x h. Display calls it after the
	// swap chain is made and after every resize; null views detach (before ResizeBuffers, which needs them released).
	void setTargetsD3D10(ID3D10RenderTargetView* rtv, ID3D10DepthStencilView* dsv, ID3D10Texture2D* backbuffer, int w, int h);
	void frameDoneD3D10();   // a frame was presented (J2X_D3D_DEBUG: the debug layer's messages so far)
	void detachD3D10();   // releases everything the backend made (Display's destroy)
}
}

#endif // GPU_D3D10_H
