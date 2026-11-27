//@template ../../templates/DisplayDxgi.template.cpp
//@set V 12
//@define intro_1
// the OpenGL and Direct3D 9/10/11 Displays); the drawing surface is a flip-model DXGI swap chain on the window's HWND, made on a
// Direct3D @V@ device's command queue and handed with them to the GL layer's Direct3D @V@ backend (GpuD3D@V@.cpp), which records
// each frame and presents it when update() is called.
//@end
//@define device_notes_1
// The device is the card's (feature level 11_0 and up), else WARP, Windows' own software rasterizer (a virtual machine or a
// remote desktop without a Direct3D @V@ card still runs the game). J2X_D3D_DEBUG=1 turns on Direct3D @V@'s debug layer (the
// Graphics Tools feature of Windows), whose messages go to the debugger. A resized window resizes the swap chain's buffers and
// the depth buffer; a removed device (a driver update or crash) is reported, as Direct3D @V@ cannot get it back without
// remaking every texture.
//@end
//@define api_includes_1: #include <dxgi1_4.h>
//@define device_state_1
	ID3D@V@CommandQueue* g_queue = 0;
	IDXGISwapChain3* g_swap = 0;
//@end
//@define device_helpers_1: 	// a live device leaves that to the driver as Windows tears the process down, and Direct3D @V@'s has been seen to hang there.
//@define device_helpers_2: 	// Resize the swap chain's buffers to the window (the backend waits for the GPU and remakes its targets).
//@define resize_device_1: 		if (!j2xgl::gpu::resizeD3D@V@(w, h)) DbgPrintF("[display] the resized targets could not be made\n");
//@define create_device_1_1
	if (std::getenv("J2X_D3D_DEBUG")) {   // Direct3D @V@'s validation (before the device is made)
		ID3D@V@Debug* debug = 0;
		if (SUCCEEDED(D3D@V@GetDebugInterface(__uuidof(ID3D@V@Debug), (void**)&debug)) && debug) { debug->EnableDebugLayer(); debug->Release(); }
//@end
//@define create_device_2_1
	IDXGIFactory4* factory = 0;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);
	if (FAILED(hr) || !factory) {
		std::fprintf(stderr, "[display] no DXGI 1.4 factory (0x%08x): Direct3D @V@ needs Windows 10 or later\n", (unsigned)hr);
//@end
//@define create_device_3_1
	// The card's device, else WARP (Windows' software rasterizer): the game runs where there is no Direct3D @V@ card at all.
	hr = D3D@V@CreateDevice(0, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D@V@Device), (void**)&g_dev);
	if (FAILED(hr)) {
		std::fprintf(stderr, "[display] no Direct3D @V@ hardware device (0x%08x); using WARP\n", (unsigned)hr);
		IDXGIAdapter* warp = 0;
		if (SUCCEEDED(factory->EnumWarpAdapter(__uuidof(IDXGIAdapter), (void**)&warp)) && warp) {
			hr = D3D@V@CreateDevice(warp, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D@V@Device), (void**)&g_dev);
			warp->Release();
		}
	}
	D3D@V@_COMMAND_QUEUE_DESC qd;
	std::memset(&qd, 0, sizeof(qd));
	qd.Type = D3D@V@_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(hr) || !g_dev || FAILED(g_dev->CreateCommandQueue(&qd, __uuidof(ID3D@V@CommandQueue), (void**)&g_queue))) {
		std::fprintf(stderr, "[display] D3D@V@CreateDevice failed: 0x%08x\n", (unsigned)hr);
		factory->Release();
		std::exit(3);
	}
	DXGI_SWAP_CHAIN_DESC1 sd;
	std::memset(&sd, 0, sizeof(sd));
	sd.Width = (UINT)w; sd.Height = (UINT)h;
	sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;                              // (the backend's frames)
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   // (Direct3D @V@ presents only by flipping)
	IDXGISwapChain1* chain = 0;
	hr = factory->CreateSwapChainForHwnd(g_queue, g_hwnd, &sd, 0, 0, &chain);
	if (SUCCEEDED(hr) && chain) {
		chain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&g_swap);
		chain->Release();
	}
	factory->MakeWindowAssociation(g_hwnd, DXGI_MWA_NO_ALT_ENTER);   // (Alt+Enter is the game's own F11, not DXGI's exclusive full screen)
	factory->Release();
	if (!g_swap) {
		std::fprintf(stderr, "[display] CreateSwapChainForHwnd failed: 0x%08x\n", (unsigned)hr);
		std::exit(3);
	}
	if (!j2xgl::gpu::attachD3D@V@(g_dev, g_queue, g_swap, w, h)) {
//@end
//@define destroy_device_1
	j2xgl::gpu::detachD3D@V@();
	release(g_swap); release(g_queue); release(g_dev);
//@end
//@define present_body_1: 	const HRESULT hr = (HRESULT)j2xgl::gpu::presentD3D@V@(g_vsync ? 1 : 0);
