#include "gpu/d3d12_device.h"

#include "log.h"

#include <dxgi1_6.h>

#include <cstdlib>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace gpu
{
    namespace
    {
        ID3D12Device3* g_device = nullptr;
        ID3D12InfoQueue* g_infoQueue = nullptr;
        std::once_flag g_once;
        std::mutex g_messageMutex;
    }

    ID3D12Device3* SharedDevice()
    {
        std::call_once(g_once, [] {
            const char* debug = std::getenv("CB_D3D12_DEBUG");
            if (debug && debug[0] == '1')
            {
                ID3D12Debug* debugInterface = nullptr;
                if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
                {
                    debugInterface->EnableDebugLayer();
                    debugInterface->Release();
                    Log("GPU: D3D12 debug layer enabled");
                }
                else
                    Log("GPU: CB_D3D12_DEBUG=1 but the D3D12 debug layer is not installed (Graphics Tools)");
            }
            // The high-performance GPU: the default adapter (nullptr) is the first one Windows lists, which on a PC
            // with an integrated GPU or a virtual display adapter can be the slow one (the EDRAM emulation is heavy).
            ID3D12Device3* device = nullptr;
            IDXGIFactory6* factory = nullptr;
            if (SUCCEEDED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory))))
            {
                IDXGIAdapter1* adapter = nullptr;
                for (UINT i = 0; !device && factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                                                               IID_PPV_ARGS(&adapter)) == S_OK; i++)
                {
                    DXGI_ADAPTER_DESC1 desc = {};
                    adapter->GetDesc1(&desc);
                    if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) &&
                        SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
                    {
                        std::string name;
                        for (const wchar_t* c = desc.Description; *c; c++)
                            name += *c < 0x80 ? char(*c) : '?';
                        Log("GPU: adapter {} ({} MiB video memory)", name, desc.DedicatedVideoMemory >> 20);
                    }
                    adapter->Release();
                }
                factory->Release();
            }
            if (!device)
                CheckHr(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)),
                        "D3D12CreateDevice (ID3D12Device3)");
            // The EDRAM is emulated in pixel shaders through a rasterizer-ordered view (analysis/xenos/06b §4).
            D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
            CheckHr(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)),
                    "CheckFeatureSupport(D3D12_OPTIONS)");
            if (!options.ROVsSupported)
                Fatal("GPU: this Direct3D 12 device does not support rasterizer-ordered views, which the EDRAM "
                      "emulation needs");
            if (debug && debug[0] == '1' && SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&g_infoQueue))))
                g_infoQueue->SetMessageCountLimit(4096);
            g_device = device;
        });
        return g_device;
    }

    void LogDebugMessages()
    {
        if (!g_infoQueue)
            return;
        std::lock_guard lock(g_messageMutex);
        static std::set<std::string> seen;
        const UINT64 count = g_infoQueue->GetNumStoredMessages();
        for (UINT64 i = 0; i < count; i++)
        {
            SIZE_T size = 0;
            g_infoQueue->GetMessage(i, nullptr, &size);
            std::vector<uint8_t> storage(size);
            D3D12_MESSAGE* message = reinterpret_cast<D3D12_MESSAGE*>(storage.data());
            if (FAILED(g_infoQueue->GetMessage(i, message, &size)))
                continue;
            const std::string text(message->pDescription, message->DescriptionByteLength ? message->DescriptionByteLength - 1 : 0);
            if (seen.insert(text).second && seen.size() <= 200)
                Log("D3D12 debug (severity {}): {}", int(message->Severity), text);
        }
        g_infoQueue->ClearStoredMessages();
    }

    void CheckHr(HRESULT hr, const char* what)
    {
        if (SUCCEEDED(hr))
            return;
        LogDebugMessages();
        const HRESULT removed = g_device ? g_device->GetDeviceRemovedReason() : S_OK;
        Fatal("GPU: {} failed (HRESULT {:08X}, device removed reason {:08X})", what, uint32_t(hr), uint32_t(removed));
    }

    ID3D12Resource* CreateBuffer(uint64_t size, D3D12_HEAP_TYPE heap, D3D12_RESOURCE_FLAGS flags, const char* what)
    {
        D3D12_HEAP_PROPERTIES properties = { heap };
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = size;
        desc.Height = desc.DepthOrArraySize = desc.MipLevels = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = flags;
        const D3D12_RESOURCE_STATES state = heap == D3D12_HEAP_TYPE_UPLOAD ? D3D12_RESOURCE_STATE_GENERIC_READ
            : heap == D3D12_HEAP_TYPE_READBACK ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_COMMON;
        ID3D12Resource* resource = nullptr;
        CheckHr(SharedDevice()->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr,
                                                        IID_PPV_ARGS(&resource)), what);
        return resource;
    }
}
