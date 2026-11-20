#pragma once

// The one Direct3D 12 device of the process, shared by the renderer (command-processor thread) and the presenter
// (window thread). D3D12 devices are free-threaded; each user owns its queues and command lists.

#include <cstdint>

#include <d3d12.h>

namespace gpu
{
    // Created on first use. Fatal if no D3D12 device with the features the renderer needs exists.
    ID3D12Device3* SharedDevice();

    // Fatal with the HRESULT (and the device-removed reason when the device is gone).
    void CheckHr(HRESULT hr, const char* what);

    // CB_D3D12_DEBUG=1: the D3D12 debug layer is enabled; this copies its new messages into the log (each distinct
    // message once). Called after every renderer submission. No-op otherwise.
    void LogDebugMessages();

    // A committed buffer (`heap` upload, readback or default) in its natural initial state.
    ID3D12Resource* CreateBuffer(uint64_t size, D3D12_HEAP_TYPE heap, D3D12_RESOURCE_FLAGS flags, const char* what);
}
