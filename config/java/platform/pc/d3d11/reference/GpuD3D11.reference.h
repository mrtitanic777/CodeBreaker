// GpuD3D11.h -- the GL layer's Direct3D 11 backend (GpuD3D11.cpp), the PC built with --graphics d3d11. The platform's Display
// makes the device, the swap chain and the views, and hands them over here; it calls setTargets again after every resize.
#ifndef GPU_D3D11_H
#define GPU_D3D11_H

#include "stdafx.h"
#include "GL11.h"
#include "Gpu.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace j2xgl {
namespace gpu {
	bool attachD3D11(ID3D11Device* device, ID3D11DeviceContext* context);   // builds the shaders; false if they cannot be
	// The frame's colour target (and its texture, for reading the frame back) and depth target, w x h. Display calls it after the
	// swap chain is made and after every resize; null views detach (before ResizeBuffers, which needs them released).
	void setTargetsD3D11(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, ID3D11Texture2D* backbuffer, int w, int h);
	void detachD3D11();   // releases everything the backend made (Display's destroy)
}
}

#endif // GPU_D3D11_H
