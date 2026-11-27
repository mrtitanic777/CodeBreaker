//@template ../../templates/Display.template.cpp
//@define intro
// Display on SDL2 + Direct3D 9 (see Display.reference.h). The window is SDL's (input, focus, the AWT mouse are the same code as the
// OpenGL Display); the drawing surface is a Direct3D 9 device made on the window's HWND and handed to the shared Direct3D 9 layer
// (d3d9::glSetDevice + glInit), which the Xbox 360 build uses unchanged. A frame is one BeginScene..EndScene; update() presents it.
//@end
//@define device_notes
// The device is kept through the things that take a windowed Direct3D 9 device away: a resized window resets it to the new size,
// and a lost device (the lock screen, a UAC prompt, a display-mode change) waits until Windows hands it back, then resets it. Every
// resource the layer makes is in the managed pool, which a reset keeps, so textures and display lists survive both.
#define J2X_WITH_GDI   // (d3d9.h needs the GDI types stdafx.h keeps out of the game's files)
//@end
//@define gpu_include: #include "GpuD3D9.h"
//@define api_globals
IDirect3D9*       g_pD3D = 0;
IDirect3DDevice9* g_pd3dDevice = 0;

//@end
//@define device_state
	D3DPRESENT_PARAMETERS g_pp;
	bool g_inScene = false;
	bool g_needReset = false;     // the window changed size: reset the device to it before the next frame
//@end
//@define device_helpers
	void begin_scene() {
		if (g_pd3dDevice && !g_inScene && SUCCEEDED(g_pd3dDevice->BeginScene())) g_inScene = true;
	}
	void end_scene() {
		if (g_pd3dDevice && g_inScene) { g_pd3dDevice->EndScene(); g_inScene = false; }
	}

	// The present parameters for the window as it is now: its client size, a depth/stencil buffer, vsync as asked.
	void fill_present_parameters(int w, int h) {
		std::memset(&g_pp, 0, sizeof(g_pp));
		g_pp.Windowed = TRUE;
		g_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		g_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		g_pp.BackBufferWidth = (UINT)(w > 0 ? w : 1);
		g_pp.BackBufferHeight = (UINT)(h > 0 ? h : 1);
		g_pp.BackBufferCount = 1;
		g_pp.hDeviceWindow = g_hwnd;
		g_pp.EnableAutoDepthStencil = TRUE;
		g_pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		g_pp.PresentationInterval = g_vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Reset the device to the current window size; true when it is usable again. The layer re-applies its render state after.
	bool reset_device() {
		if (!g_pd3dDevice) return false;
		end_scene();
//@end
//@define resize_device
		fill_present_parameters(w, h);
		const HRESULT hr = g_pd3dDevice->Reset(&g_pp);
		if (FAILED(hr)) {
			DbgPrintF("[display] device Reset failed: 0x%08x\n", (unsigned)hr);
			return false;
		}
		d3d9::glSetDevice(g_pd3dDevice);   // the new backbuffer height (the layer's GL->D3D viewport flip)
		d3d9::glOnDeviceReset();
//@end
//@define resize_done
		g_needReset = false;
		begin_scene();
		return true;
//@end
//@define device_ready_fn

	// Before a frame: a lost device waits (false: skip drawing this frame), a resized window resets.
	bool device_ready() {
		if (!g_pd3dDevice) return false;
		const HRESULT coop = g_pd3dDevice->TestCooperativeLevel();
		if (coop == D3DERR_DEVICELOST) { end_scene(); SDL_Delay(10); return false; }
		if (coop == D3DERR_DEVICENOTRESET || g_needReset) return reset_device();
		return true;
	}
//@end
//@define resize_flag: 		g_needReset = true;
//@define fail: 		return;
//@define create_device_1
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!g_pD3D) {
		std::fprintf(stderr, "[display] Direct3DCreate9 failed (no Direct3D 9 on this system)\n");
		return;
//@end
//@define create_device_2
	fill_present_parameters(w, h);
	// Hardware vertex processing where the card has it, else software (a VM's or a remote desktop's adapter). FPU_PRESERVE keeps
	// the game's double-precision maths double: without it Direct3D drops the FPU to single precision for the whole process.
	const DWORD keep = D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED;
	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | keep, &g_pp, &g_pd3dDevice);
	if (FAILED(hr)) hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | keep, &g_pp, &g_pd3dDevice);
	if (FAILED(hr) || !g_pd3dDevice) {
		std::fprintf(stderr, "[display] CreateDevice failed: 0x%08x\n", (unsigned)hr);
		g_pd3dDevice = 0;
		return;
//@end
//@define create_device_3
	d3d9::glSetDevice(g_pd3dDevice);
	d3d9::glInit();
	d3d9::glViewport(0, 0, w, h);
//@end
//@define first_frame: 	begin_scene();
//@define destroy_device
	end_scene();
	if (g_pd3dDevice) g_pd3dDevice->Release();
	if (g_pD3D) g_pD3D->Release();
//@end
//@define destroy_cleared
	g_pd3dDevice = 0;
	g_pD3D = 0;
//@end
//@define resize_flag_deep: 				g_needReset = true;
//@define present_comment: // End the frame, present it, open the next. A lost device skips the present; device_ready() brings it back.
//@define present_body
	if (!g_pd3dDevice) return;
	d3d9::glFlush();   // the layer's pending batch and this frame's new display-list buffers
	end_scene();
	const HRESULT hr = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) DbgPrintF("[display] device lost; waiting for it\n");
	if (device_ready()) begin_scene();
//@end
//@define save_frame_body
	if (!g_pd3dDevice) return;
	IDirect3DSurface9* back = 0;
	IDirect3DSurface9* copy = 0;
	D3DSURFACE_DESC d;
	if (FAILED(g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back)) || !back) return;
	back->GetDesc(&d);
	if (SUCCEEDED(g_pd3dDevice->CreateOffscreenPlainSurface(d.Width, d.Height, d.Format, D3DPOOL_SYSTEMMEM, &copy, 0)) && copy &&
	    SUCCEEDED(g_pd3dDevice->GetRenderTargetData(back, copy))) {
		D3DLOCKED_RECT lr;
		if (SUCCEEDED(copy->LockRect(&lr, 0, D3DLOCK_READONLY))) {
			SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, (int)d.Width, (int)d.Height, 24, SDL_PIXELFORMAT_BGR24);
			if (s) {
				for (UINT y = 0; y < d.Height; ++y) {       // X8R8G8B8 rows (B, G, R, X in memory) -> B, G, R
					const unsigned char* src = (const unsigned char*)lr.pBits + (size_t)y * lr.Pitch;
					unsigned char* dst = (unsigned char*)s->pixels + (size_t)y * s->pitch;
					for (UINT x = 0; x < d.Width; ++x) { dst[x * 3] = src[x * 4]; dst[x * 3 + 1] = src[x * 4 + 1]; dst[x * 3 + 2] = src[x * 4 + 2]; }
				}
				SDL_SaveBMP(s, file);
				SDL_FreeSurface(s);
				std::fprintf(stderr, "[display] frame saved to %s\n", file);
			}
			copy->UnlockRect();
//@end
//@define save_frame_end
	if (copy) copy->Release();
	back->Release();
//@end
//@define flush_for_shot: 	if (shoot || scripted_shot) d3d9::glFlush();   // (a shot is the frame as drawn so far, the layer's pending batch in it)
//@define fullscreen_comment: // Full screen is the desktop-sized borderless window (Direct3D stays windowed: no exclusive mode to lose on alt-tab).
//@define vsync_fn
void setVSyncEnabled(bool on) {
	if (g_vsync == on) return;
	g_vsync = on;
	if (g_pd3dDevice) g_needReset = true;   // the present interval is a present parameter: it takes a reset
}
//@end
