//@template ../../templates/DisplayDxgi.template.cpp
//@set V 11
//@define intro_1
// the OpenGL and Direct3D 9 Displays); the drawing surface is a DXGI swap chain on the window's HWND with a Direct3D @V@ device,
// handed to the GL layer's Direct3D @V@ backend (GpuD3D@V@.cpp). A frame is drawn, then update() presents it.
//@end
//@define device_notes_1
// The device is the card's (feature level 11_0 down to 10_0), else WARP, Windows' own software rasterizer (a virtual machine or
// a remote desktop without a Direct3D 10 card still runs the game; J2X_D3D_DEBUG=1 makes it with the debug layer, whose
// errors and warnings the backend prints). A resized window resizes the swap chain's buffers and the
// depth buffer; a removed device (a driver update or crash) is reported, as Direct3D @V@ cannot get it back without remaking
// every texture.
//@end
//@define api_includes_1: #include <dxgi.h>
//@define device_state_1
	ID3D@V@DeviceContext* g_ctx = 0;
	IDXGISwapChain* g_swap = 0;
	ID3D@V@Texture2D* g_back = 0;
	ID3D@V@RenderTargetView* g_rtv = 0;
	ID3D@V@Texture2D* g_depth = 0;
	ID3D@V@DepthStencilView* g_dsv = 0;
	int g_bw = 0, g_bh = 0;       // the swap chain's buffer size
//@end
//@define device_helpers_1: 	// a live device leaves that to the driver as Windows tears the process down, and Direct3D 12's has been seen to hang there.
//@define device_helpers_2
	// The views on the swap chain's buffer and a depth buffer of its size, handed to the backend.
	bool make_targets(int w, int h) {
		if (FAILED(g_swap->GetBuffer(0, __uuidof(ID3D@V@Texture2D), (void**)&g_back)) || !g_back) return false;
		if (FAILED(g_dev->CreateRenderTargetView(g_back, 0, &g_rtv))) return false;
		D3D@V@_TEXTURE2D_DESC d;
		std::memset(&d, 0, sizeof(d));
		d.Width = (UINT)w; d.Height = (UINT)h; d.MipLevels = 1; d.ArraySize = 1;
		d.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; d.SampleDesc.Count = 1;
		d.Usage = D3D@V@_USAGE_DEFAULT; d.BindFlags = D3D@V@_BIND_DEPTH_STENCIL;
		if (FAILED(g_dev->CreateTexture2D(&d, 0, &g_depth)) || FAILED(g_dev->CreateDepthStencilView(g_depth, 0, &g_dsv))) return false;
		g_bw = w; g_bh = h;
		j2xgl::gpu::setTargetsD3D@V@(g_rtv, g_dsv, g_back, w, h);
		return true;
	}
	void drop_targets() {
		j2xgl::gpu::setTargetsD3D@V@(0, 0, 0, 0, 0);
		release(g_rtv); release(g_back); release(g_dsv); release(g_depth);
	}
	// Resize the swap chain's buffers to the window (ResizeBuffers needs every view on them released first).
//@end
//@define resize_device_1
		drop_targets();
		const HRESULT hr = g_swap->ResizeBuffers(0, (UINT)w, (UINT)h, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) DbgPrintF("[display] ResizeBuffers failed: 0x%08x\n", (unsigned)hr);
		if (!make_targets(w, h)) DbgPrintF("[display] the resized targets could not be made\n");
//@end
//@define create_device_1_1
	DXGI_SWAP_CHAIN_DESC sd;
	std::memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)w; sd.BufferDesc.Height = (UINT)h;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	static const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL got = D3D_FEATURE_LEVEL_10_0;
	const UINT flags = std::getenv("J2X_D3D_DEBUG") ? D3D@V@_CREATE_DEVICE_DEBUG : 0;   // (Direct3D's validation, the Graphics Tools feature)
	// The card's device, else WARP (Windows' software rasterizer): the game runs where there is no Direct3D 10 card at all.
	HRESULT hr = D3D@V@CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, levels, 3, D3D@V@_SDK_VERSION, &sd, &g_swap, &g_dev, &got, &g_ctx);
	if (FAILED(hr)) {
		std::fprintf(stderr, "[display] no Direct3D @V@ hardware device (0x%08x); using WARP\n", (unsigned)hr);
		hr = D3D@V@CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_WARP, 0, flags, levels, 3, D3D@V@_SDK_VERSION, &sd, &g_swap, &g_dev, &got, &g_ctx);
//@end
//@define create_device_2_1
	if (FAILED(hr) || !g_dev || !g_swap) {
		std::fprintf(stderr, "[display] D3D@V@CreateDeviceAndSwapChain failed: 0x%08x\n", (unsigned)hr);
//@end
//@define create_device_3_1
	IDXGIFactory* factory = 0;   // (Alt+Enter is the game's own F11, not DXGI's exclusive full screen)
	if (SUCCEEDED(g_swap->GetParent(__uuidof(IDXGIFactory), (void**)&factory)) && factory) {
		factory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);
		factory->Release();
	}
	if (!j2xgl::gpu::attachD3D@V@(g_dev, g_ctx) || !make_targets(w, h)) {
//@end
//@define destroy_device_1
	drop_targets();
	j2xgl::gpu::detachD3D@V@();
	release(g_swap); release(g_ctx); release(g_dev);
//@end
//@define present_body_1
	const HRESULT hr = g_swap->Present(g_vsync ? 1 : 0, 0);
	j2xgl::gpu::frameDoneD3D@V@();
//@end
