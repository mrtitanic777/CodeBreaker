#include "kernel/stfs_mount.h"
#include "gpu/presenter.h"

#include "gpu/d3d12_device.h"

#include "input.h"
#include "live/client.h"
#include "log.h"
#include "settings.h"
#include "transition/transition.h"

#include <windows.h>

#include <cstdlib>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <mutex>
#include <thread>

using Microsoft::WRL::ComPtr;

namespace gpu
{
    namespace
    {
        constexpr UINT kBackBufferCount = 2;
        constexpr DXGI_FORMAT kFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
        constexpr uint32_t kOutputs = 3; // one being written, one waiting to be shown, one shown

        // Fullscreen triangle sampling the guest output; the viewport does the letterboxing.
        constexpr char kShaderSource[] = R"(
struct Interpolants { float4 position : SV_Position; float2 uv : TEXCOORD0; };
Interpolants VertexMain(uint id : SV_VertexID)
{
    Interpolants o;
    o.uv = float2((id << 1) & 2, id & 2);
    o.position = float4(o.uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
    return o;
}
Texture2D frameTexture : register(t0);
SamplerState frameSampler : register(s0);
cbuffer Fade : register(b0) { float brightness; };
float4 PixelMain(Interpolants i) : SV_Target
{
    return float4(frameTexture.Sample(frameSampler, i.uv).rgb * brightness, 1.0);
}
)";

        // ---- Guest-output textures, shared by the renderer (writer) and the window (reader) ------------------
        // A texture is Free (may be written), Writing (acquired by the renderer), Pending (complete, not shown yet)
        // or Shown (the window draws it, also for repaints). Released textures remember the window fence value of
        // their last read, which the writer's queue waits for.
        enum class OutputState { kFree, kWriting, kPending, kShown };
        struct Output
        {
            ComPtr<ID3D12Resource> texture;
            uint32_t width = 0, height = 0;
            OutputState state = OutputState::kFree;
            uint64_t releaseValue = 0;     // window fence value after its last read
            ID3D12Fence* doneFence = nullptr;
            uint64_t doneValue = 0;        // written when doneFence reaches this
        };

        std::atomic<bool> g_active = false;
        // FadeToBlack: the picture's brightness goes from 1 to 0 in g_fadeMs from g_fadeStart (steady clock, ns);
        // -1: no fade. It stays black until ClearFade. g_fadeDone (manual reset) is set once black is on the screen.
        std::atomic<int64_t> g_fadeStart = -1;
        std::atomic<int64_t> g_fadeMs = 0;
        HANDLE g_fadeDone = nullptr;
        HANDLE g_firstPresent = nullptr; // set after the window's first Present (WaitForWindow)
        std::mutex g_outputMutex;
        Output g_outputs[kOutputs];
        HANDLE g_frameEvent = nullptr;

        // Signalled by the window's queue after each draw.
        ID3D12Fence* ReadFence()
        {
            static ID3D12Fence* fence = [] {
                ID3D12Fence* f = nullptr;
                CheckHr(SharedDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&f)), "presenter CreateFence");
                return f;
            }();
            return fence;
        }

        void WaitForFence(ID3D12Fence* fence, uint64_t value)
        {
            if (fence->GetCompletedValue() < value)
                CheckHr(fence->SetEventOnCompletion(value, nullptr), "SetEventOnCompletion"); // blocks until reached
        }

        class Presenter
        {
        public:
            void Run(uint32_t width, uint32_t height)
            {
                CreateWindowAndDevice(width, height);
                MSG msg;
                for (;;)
                {
                    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
                    {
                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                    }
                    if (resizePending_)
                        Resize();
                    // While fading, a frame per vblank (Present waits for it); else only new frames and repaints.
                    const bool fading = g_fadeStart >= 0 && WaitForSingleObject(g_fadeDone, 0) != WAIT_OBJECT_0;
                    if (TakePending() || redrawPending_ || fading)
                        Draw();
                    redrawPending_ = false;
                    MsgWaitForMultipleObjects(1, &g_frameEvent, FALSE, fading ? 0 : INFINITE, QS_ALLINPUT);
                }
            }

            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
            {
                auto* self = reinterpret_cast<Presenter*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
                // F11 (or Alt+Enter) switches between the window and borderless full screen (not a key of the
                // game's keyboard controller).
                const bool repeat = (lParam & (1 << 30)) != 0;
                if (self && !self->hosted_ && !repeat && ((message == WM_KEYDOWN && wParam == VK_F11) ||
                                        (message == WM_SYSKEYDOWN && wParam == VK_RETURN && (lParam & (1 << 29)))))
                {
                    self->ToggleFullscreen();
                    return 0;
                }
                if ((message == WM_KEYUP || message == WM_SYSKEYUP) && (wParam == VK_F11 || wParam == VK_RETURN) &&
                    self && self->swallowKeyUp_)
                {
                    self->swallowKeyUp_ = false;
                    return 0;
                }
                if (input::HandleWindowMessage(hwnd, message, wParam, lParam))
                    return message == WM_SETCURSOR ? TRUE : 0;
                switch (message)
                {
                case WM_SIZE:
                    if (self && wParam != SIZE_MINIMIZED)
                        self->resizePending_ = true;
                    return 0;
                case WM_PAINT:
                    if (self)
                        self->redrawPending_ = true;
                    ValidateRect(hwnd, nullptr);
                    return 0;
                case WM_CLOSE:
                    Log("Window closed");
                    live::SignOutAllForExit(); // the player quits: party seats are given up at once
                    kernel::stfs_mount::WaitForAllWrites(); // content packages still being written back
                    logging::Flush();
                    ExitProcess(0);
                }
                return DefWindowProcW(hwnd, message, wParam, lParam);
            }

            bool hosted_ = false; // a child of the dashboard's window (NATIVE_DASH_HOST_WINDOW)

        private:
            // Borderless full screen on the window's monitor; the windowed size and place come back after.
            void ToggleFullscreen()
            {
                swallowKeyUp_ = true;
                if (!fullscreen_)
                {
                    windowed_.length = sizeof(windowed_);
                    GetWindowPlacement(hwnd_, &windowed_);
                    MONITORINFO monitor{ sizeof(monitor) };
                    GetMonitorInfoW(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST), &monitor);
                    SetWindowLongPtrW(hwnd_, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                    SetWindowPos(hwnd_, HWND_TOP, monitor.rcMonitor.left, monitor.rcMonitor.top,
                                 monitor.rcMonitor.right - monitor.rcMonitor.left,
                                 monitor.rcMonitor.bottom - monitor.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
                    fullscreen_ = true;
                }
                else
                {
                    SetWindowLongPtrW(hwnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
                    SetWindowPlacement(hwnd_, &windowed_);
                    SetWindowPos(hwnd_, nullptr, 0, 0, 0, 0,
                                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
                    fullscreen_ = false;
                }
                Log("Presenter: {}", fullscreen_ ? "full screen" : "windowed");
            }

            struct FrameContext
            {
                ComPtr<ID3D12CommandAllocator> allocator;
                uint64_t fenceValue = 0;
            };

            void CreateWindowAndDevice(uint32_t width, uint32_t height)
            {
                WNDCLASSEXW wc{ sizeof(wc) };
                wc.lpfnWndProc = WindowProc;
                wc.hInstance = GetModuleHandleW(nullptr);
                wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512)) /* IDC_ARROW */;
                // The program's icon (its resource 1) in the title bar and task bar.
                wc.hIcon = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(1));
                wc.hIconSm = static_cast<HICON>(LoadImageW(wc.hInstance, MAKEINTRESOURCEW(1), IMAGE_ICON,
                    GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0));
                // Black from the start: nothing else shows before the first frame.
                wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
                wc.lpszClassName = L"CodeBreakerX360";
                RegisterClassExW(&wc);
                // Started by a dashboard that shows the title in its own window (NATIVE_DASH_HOST_WINDOW = that
                // window's handle, Native Dashboard's games.h): a child filling it, as the console has one screen.
                HWND host = nullptr;
                const char* hostText = std::getenv("NATIVE_DASH_HOST_WINDOW");
                if (hostText && *hostText)
                    host = reinterpret_cast<HWND>(uintptr_t(std::strtoull(hostText, nullptr, 10)));
                if (host && !IsWindow(host))
                {
                    Log("Presenter: NATIVE_DASH_HOST_WINDOW {} is not a window: a window of its own", hostText);
                    host = nullptr;
                }
                if (host)
                {
                    RECT client;
                    GetClientRect(host, &client);
                    hosted_ = true;
                    hwnd_ = CreateWindowExW(0, wc.lpszClassName, L"Xbox 360 title", WS_CHILD | WS_CLIPSIBLINGS, 0, 0,
                        client.right, client.bottom, host, nullptr, wc.hInstance, nullptr);
                    Log("Presenter: inside the dashboard's window ({}x{})", client.right, client.bottom);
                }
                else
                {
                    RECT rect{ 0, 0, LONG(width), LONG(height) };
                    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
                    hwnd_ = CreateWindowExW(0, wc.lpszClassName, L"Xbox 360 Dashboard", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr,
                        wc.hInstance, nullptr);
                }
                if (!hwnd_)
                    Fatal("Presenter: cannot create the window (error {})", GetLastError());
                SetWindowLongPtrW(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
                input::SetWindow(hwnd_);

                ComPtr<IDXGIFactory4> factory;
                CheckHr(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)), "CreateDXGIFactory2");
                device_ = SharedDevice(); // shared with the renderer (d3d12_device.h)
                fence_ = ReadFence();
                D3D12_COMMAND_QUEUE_DESC queueDesc{ D3D12_COMMAND_LIST_TYPE_DIRECT };
                CheckHr(device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue_)), "presenter CreateCommandQueue");

                DXGI_SWAP_CHAIN_DESC1 sc{};
                sc.Width = width;
                sc.Height = height;
                sc.Format = kFormat;
                sc.SampleDesc.Count = 1;
                sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sc.BufferCount = kBackBufferCount;
                sc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                ComPtr<IDXGISwapChain1> swapChain;
                CheckHr(factory->CreateSwapChainForHwnd(queue_.Get(), hwnd_, &sc, nullptr, nullptr, &swapChain),
                    "CreateSwapChainForHwnd");
                CheckHr(swapChain.As(&swapChain_), "IDXGISwapChain3");
                factory->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER);

                D3D12_DESCRIPTOR_HEAP_DESC rtvDesc{ D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kBackBufferCount };
                CheckHr(device_->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap_)), "presenter RTV heap");
                rtvSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                D3D12_DESCRIPTOR_HEAP_DESC srvDesc{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kOutputs,
                    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE };
                CheckHr(device_->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(&srvHeap_)), "presenter SRV heap");
                srvSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                CreateBackBufferViews();

                for (FrameContext& f : frames_)
                    CheckHr(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&f.allocator)),
                        "presenter allocator");
                CheckHr(device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frames_[0].allocator.Get(), nullptr,
                    IID_PPV_ARGS(&list_)), "presenter command list");
                list_->Close();
                CreatePipeline();

                ShowWindow(hwnd_, hosted_ ? SW_SHOW : SW_SHOWDEFAULT);
                if (hosted_)
                    SetFocus(hwnd_); // the keyboard (the dashboard's window forwards it while active)
                redrawPending_ = true; // a black frame until the title's first one
                Log("Presenter: window {}x{}", width, height);
                if (Settings().fullscreen)
                {
                    ToggleFullscreen();
                    swallowKeyUp_ = false;
                }
            }

            void CreateBackBufferViews()
            {
                D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
                for (UINT i = 0; i < kBackBufferCount; i++)
                {
                    CheckHr(swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i])), "GetBuffer");
                    device_->CreateRenderTargetView(backBuffers_[i].Get(), nullptr, handle);
                    handle.ptr += rtvSize_;
                }
            }

            ComPtr<ID3DBlob> Compile(const char* entry, const char* target)
            {
                ComPtr<ID3DBlob> code, errors;
                if (FAILED(D3DCompile(kShaderSource, sizeof(kShaderSource) - 1, "presenter", nullptr, nullptr, entry,
                        target, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &errors)))
                    Fatal("Presenter: shader {} failed to compile: {}", entry,
                        errors ? static_cast<const char*>(errors->GetBufferPointer()) : "?");
                return code;
            }

            void CreatePipeline()
            {
                D3D12_DESCRIPTOR_RANGE range{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0 };
                D3D12_ROOT_PARAMETER parameters[2]{};
                parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                parameters[0].DescriptorTable = { 1, &range };
                parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS; // the fade's brightness
                parameters[1].Constants = { 0, 0, 1 };
                parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                D3D12_STATIC_SAMPLER_DESC sampler{};
                sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
                sampler.AddressU = sampler.AddressV = sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                sampler.MaxLOD = D3D12_FLOAT32_MAX;
                sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                D3D12_ROOT_SIGNATURE_DESC rs{ 2, parameters, 1, &sampler };
                ComPtr<ID3DBlob> blob, errors;
                CheckHr(D3D12SerializeRootSignature(&rs, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &errors),
                    "presenter root signature");
                CheckHr(device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
                    IID_PPV_ARGS(&rootSignature_)), "presenter CreateRootSignature");

                ComPtr<ID3DBlob> vs = Compile("VertexMain", "vs_5_0"), ps = Compile("PixelMain", "ps_5_0");
                D3D12_GRAPHICS_PIPELINE_STATE_DESC pso{};
                pso.pRootSignature = rootSignature_.Get();
                pso.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };
                pso.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };
                pso.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
                pso.SampleMask = UINT_MAX;
                pso.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
                pso.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
                pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                pso.NumRenderTargets = 1;
                pso.RTVFormats[0] = kFormat;
                pso.SampleDesc.Count = 1;
                CheckHr(device_->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&pipeline_)), "presenter pipeline");
            }

            void Resize()
            {
                resizePending_ = false;
                RECT client;
                GetClientRect(hwnd_, &client);
                if (client.right <= 0 || client.bottom <= 0)
                    return;
                WaitForFence(fence_, fenceValue_); // the back buffers must be idle (rare: window resizes only)
                for (auto& b : backBuffers_)
                    b.Reset();
                CheckHr(swapChain_->ResizeBuffers(kBackBufferCount, UINT(client.right), UINT(client.bottom), kFormat, 0),
                    "ResizeBuffers");
                CreateBackBufferViews();
                redrawPending_ = true;
            }

            // Takes the newest pending guest output; the one shown before is released with its last read's fence value.
            bool TakePending()
            {
                {
                    std::lock_guard lock(g_outputMutex);
                    int pending = -1;
                    for (uint32_t i = 0; i < kOutputs; i++)
                        if (g_outputs[i].state == OutputState::kPending)
                            pending = int(i);
                    if (pending < 0)
                        return false;
                    if (shown_ >= 0)
                    {
                        g_outputs[shown_].state = OutputState::kFree;
                        g_outputs[shown_].releaseValue = lastRead_[shown_];
                    }
                    Output& o = g_outputs[pending];
                    o.state = OutputState::kShown;
                    shown_ = pending;
                    shownTexture_ = o.texture;
                    shownWidth_ = o.width;
                    shownHeight_ = o.height;
                    doneFence_ = o.doneFence;
                    doneValue_ = o.doneValue;
                }
                // A texture replaced after its release (size change) gets a new view; its old one is no longer read
                // (AcquireGuestOutput waited for the release value before replacing it).
                if (srvResource_[shown_] != shownTexture_.Get())
                {
                    D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
                    srv.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
                    srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                    srv.Texture2D.MipLevels = 1;
                    D3D12_CPU_DESCRIPTOR_HANDLE handle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
                    handle.ptr += SIZE_T(shown_) * srvSize_;
                    device_->CreateShaderResourceView(shownTexture_.Get(), &srv, handle);
                    srvResource_[shown_] = shownTexture_.Get();
                }
                return true;
            }

            void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
            {
                D3D12_RESOURCE_BARRIER barrier{ D3D12_RESOURCE_BARRIER_TYPE_TRANSITION };
                barrier.Transition = { resource, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, before, after };
                list_->ResourceBarrier(1, &barrier);
            }

            void Draw()
            {
                RECT client;
                GetClientRect(hwnd_, &client);
                const float windowW = float(client.right), windowH = float(client.bottom);
                if (windowW <= 0 || windowH <= 0)
                {
                    if (g_fadeStart >= 0)
                        SetEvent(g_fadeDone); // minimized: nothing to fade
                    return;
                }
                FrameContext& frame = frames_[frameIndex_];
                frameIndex_ = (frameIndex_ + 1) % kBackBufferCount;
                WaitForFence(fence_, frame.fenceValue); // its allocator's previous commands (normally long finished)
                const UINT index = swapChain_->GetCurrentBackBufferIndex();
                CheckHr(frame.allocator->Reset(), "presenter allocator Reset");
                CheckHr(list_->Reset(frame.allocator.Get(), pipeline_.Get()), "presenter list Reset");
                Transition(backBuffers_[index].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
                D3D12_CPU_DESCRIPTOR_HANDLE rtv = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
                rtv.ptr += SIZE_T(index) * rtvSize_;
                const float black[4] = { 0, 0, 0, 1 };
                list_->ClearRenderTargetView(rtv, black, 0, nullptr);
                // No frame from the title yet: black.
                ID3D12Resource* texture = shown_ >= 0 ? shownTexture_.Get() : nullptr;
                const float brightness = Brightness();
                if (texture)
                {
                    Transition(texture, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                    // Letterbox: largest rectangle with the frame's aspect ratio.
                    const float scale = std::min(windowW / float(shownWidth_), windowH / float(shownHeight_));
                    const float w = float(shownWidth_) * scale, h = float(shownHeight_) * scale;
                    D3D12_VIEWPORT viewport{ (windowW - w) / 2, (windowH - h) / 2, w, h, 0, 1 };
                    D3D12_RECT scissor{ 0, 0, client.right, client.bottom };
                    list_->OMSetRenderTargets(1, &rtv, FALSE, nullptr);
                    list_->SetGraphicsRootSignature(rootSignature_.Get());
                    ID3D12DescriptorHeap* heaps[] = { srvHeap_.Get() };
                    list_->SetDescriptorHeaps(1, heaps);
                    D3D12_GPU_DESCRIPTOR_HANDLE table = srvHeap_->GetGPUDescriptorHandleForHeapStart();
                    table.ptr += UINT64(shown_) * srvSize_;
                    list_->SetGraphicsRootDescriptorTable(0, table);
                    list_->SetGraphicsRoot32BitConstants(1, 1, &brightness, 0);
                    list_->RSSetViewports(1, &viewport);
                    list_->RSSetScissorRects(1, &scissor);
                    list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    list_->DrawInstanced(3, 1, 0, 0);
                    Transition(texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COMMON);
                }
                Transition(backBuffers_[index].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
                CheckHr(list_->Close(), "presenter list Close");
                if (texture)
                    CheckHr(queue_->Wait(doneFence_, doneValue_), "presenter queue Wait"); // the renderer finished it
                ID3D12CommandList* lists[] = { list_.Get() };
                queue_->ExecuteCommandLists(1, lists);
                CheckHr(queue_->Signal(fence_, ++fenceValue_), "presenter Signal");
                frame.fenceValue = fenceValue_;
                if (shown_ >= 0)
                    lastRead_[shown_] = fenceValue_;
                const HRESULT hr = swapChain_->Present(1, 0);
                if (FAILED(hr))
                    Fatal("Presenter: Present failed (HRESULT {:08X}, device removed reason {:08X})", uint32_t(hr),
                        uint32_t(device_->GetDeviceRemovedReason()));
                if (g_firstPresent)
                    SetEvent(g_firstPresent);
                // A process that started this one covers the screen until its boot animation is ready
                // (native/x360/system/transition).
                if (texture)
                    transition::FramePresented();
                // The fade is done once a black frame is on the screen (the second Present after it returns).
                blackPresents_ = brightness == 0.0f ? blackPresents_ + 1 : 0;
                if (blackPresents_ == 2)
                    SetEvent(g_fadeDone);
            }

            static float Brightness()
            {
                const int64_t start = g_fadeStart;
                if (start < 0)
                    return 1.0f;
                const int64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count();
                const float t = std::clamp(float(now - start) / (float(std::max<int64_t>(g_fadeMs, 1)) * 1e6f), 0.0f, 1.0f);
                return 1.0f - t * t * (3.0f - 2.0f * t); // smooth start and end
            }

            HWND hwnd_ = nullptr;
            int blackPresents_ = 0;
            ID3D12Device* device_ = nullptr;
            ComPtr<ID3D12CommandQueue> queue_;
            ComPtr<IDXGISwapChain3> swapChain_;
            ComPtr<ID3D12DescriptorHeap> rtvHeap_, srvHeap_;
            UINT rtvSize_ = 0, srvSize_ = 0;
            ComPtr<ID3D12Resource> backBuffers_[kBackBufferCount];
            FrameContext frames_[kBackBufferCount];
            uint32_t frameIndex_ = 0;
            ComPtr<ID3D12GraphicsCommandList> list_;
            ID3D12Fence* fence_ = nullptr;
            uint64_t fenceValue_ = 0;
            ComPtr<ID3D12RootSignature> rootSignature_;
            ComPtr<ID3D12PipelineState> pipeline_;

            int shown_ = -1;
            ComPtr<ID3D12Resource> shownTexture_;
            uint32_t shownWidth_ = 0, shownHeight_ = 0;
            ID3D12Fence* doneFence_ = nullptr;
            uint64_t doneValue_ = 0;
            uint64_t lastRead_[kOutputs] = {};
            ID3D12Resource* srvResource_[kOutputs] = {};
            bool resizePending_ = false;
            bool redrawPending_ = false;
            bool fullscreen_ = false;
            bool swallowKeyUp_ = false;
            WINDOWPLACEMENT windowed_{};
        };
    }

    void StartPresenter(uint32_t width, uint32_t height)
    {
        const char* headless = std::getenv("CB_HEADLESS");
        if (headless && headless[0] == '1')
        {
            Log("Presenter: headless (CB_HEADLESS=1)");
            return;
        }
        g_frameEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        g_fadeDone = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        g_firstPresent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
        g_active = true;
        std::thread([width, height] { Presenter().Run(width, height); }).detach();
    }

    bool PresenterActive()
    {
        return g_active;
    }

    void WaitForWindow(std::chrono::milliseconds timeout)
    {
        if (!g_active || !g_firstPresent)
            return;
        const auto start = std::chrono::steady_clock::now();
        const DWORD result = WaitForSingleObject(g_firstPresent, DWORD(timeout.count()));
        Log("Presenter: window ready after {} ms{}", std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count(), result == WAIT_OBJECT_0 ? "" : " (timed out)");
    }

    void FadeToBlack(std::chrono::milliseconds duration)
    {
        if (!g_active)
            return;
        ResetEvent(g_fadeDone);
        g_fadeMs = std::max<int64_t>(duration.count(), 1);
        g_fadeStart = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        SetEvent(g_frameEvent);
        if (WaitForSingleObject(g_fadeDone, DWORD(duration.count()) + 2000) != WAIT_OBJECT_0)
            Log("Presenter: the fade to black did not reach the screen in time");
    }

    void ClearFade()
    {
        if (!g_active)
            return;
        g_fadeStart = -1;
        SetEvent(g_frameEvent);
    }

    GuestOutput AcquireGuestOutput(uint32_t width, uint32_t height)
    {
        std::lock_guard lock(g_outputMutex);
        int chosen = -1;
        for (uint32_t i = 0; i < kOutputs; i++)
            if (g_outputs[i].state == OutputState::kFree &&
                (chosen < 0 || g_outputs[i].releaseValue < g_outputs[chosen].releaseValue))
                chosen = int(i);
        if (chosen < 0)
            Fatal("Presenter: no free guest-output texture (a previous one was acquired and never submitted)");
        Output& o = g_outputs[chosen];
        if (!o.texture || o.width != width || o.height != height)
        {
            WaitForFence(ReadFence(), o.releaseValue); // the window no longer reads the old texture
            D3D12_HEAP_PROPERTIES heap{ D3D12_HEAP_TYPE_DEFAULT };
            D3D12_RESOURCE_DESC desc{ D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0, width, height, 1, 1,
                DXGI_FORMAT_R10G10B10A2_TYPELESS, { 1, 0 }, D3D12_TEXTURE_LAYOUT_UNKNOWN,
                D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS };
            o.texture.Reset();
            CheckHr(SharedDevice()->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON,
                nullptr, IID_PPV_ARGS(&o.texture)), "guest output texture");
            o.width = width;
            o.height = height;
        }
        o.state = OutputState::kWriting;
        return { uint32_t(chosen), o.texture.Get(), ReadFence(), o.releaseValue };
    }

    void SubmitGuestOutput(uint32_t index, ID3D12Fence* fence, uint64_t value)
    {
        {
            std::lock_guard lock(g_outputMutex);
            Output& o = g_outputs[index];
            if (o.state != OutputState::kWriting)
                Fatal("Presenter: guest output {} submitted without being acquired", index);
            for (Output& other : g_outputs)
                if (other.state == OutputState::kPending)
                    other.state = OutputState::kFree; // never shown; its last read is unchanged
            o.state = g_active ? OutputState::kPending : OutputState::kFree;
            o.doneFence = fence;
            o.doneValue = value;
        }
        if (g_active)
            SetEvent(g_frameEvent);
    }
}
