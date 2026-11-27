// GpuD3D12.h -- the GL layer's Direct3D 12 backend (GpuD3D12.cpp), the PC built with --graphics d3d12. The platform's Display
// makes the device, the command queue and the swap chain, and hands them over here; the backend owns everything recorded for a
// frame (the command list, its allocators and fence, the descriptor heaps, the swap chain's buffers and the depth buffer).
#ifndef GPU_D3D12_H
#define GPU_D3D12_H

#include "stdafx.h"
#include "GL11.h"
#include "Gpu.h"

struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGISwapChain3;

namespace j2xgl {
namespace gpu {
	// Builds the shaders, the root signature and the frame's resources on the swap chain's w x h buffers; false if they cannot be.
	bool attachD3D12(ID3D12Device* device, ID3D12CommandQueue* queue, IDXGISwapChain3* swapChain, int w, int h);
	// Ends the frame: runs what was recorded, presents it (syncInterval as IDXGISwapChain::Present's) and starts the next one.
	// Present's HRESULT (a removed device shows there).
	long presentD3D12(unsigned syncInterval);
	// The window changed size: waits for the GPU, resizes the swap chain's buffers and the depth buffer (Display, after present).
	bool resizeD3D12(int w, int h);
	void detachD3D12();   // waits for the GPU and releases everything the backend made (Display's destroy)
}
}

#endif // GPU_D3D12_H
