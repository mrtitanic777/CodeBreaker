//@template Display.template.cpp
//@define intro
// Display on SDL2 + Direct3D @V@ (see Display.reference.h). The window is SDL's (input, focus, the AWT mouse are the same code as
//@hole intro_1
//@end
//@define device_notes: //@hole device_notes_1
//@define gpu_include: #include "GpuD3D@V@.h"
//@define api_includes
#include <d3d@V@.h>
//@hole api_includes_1
//@end
//@define device_state
	ID3D@V@Device* g_dev = 0;
//@hole device_state_1
	bool g_needResize = false;    // the window changed size: resize the buffers before the next frame
	bool g_removed = false;
//@end
//@define device_helpers
	template<class T> void release(T*& p) { if (p) { p->Release(); p = 0; } }

	// The device is released before the process ends, however it ends (the game's System.exit, the script's quit, the last thread
	// returning): the GPU's work finished and every object let go while the process can still run it. A process that exits with
//@hole device_helpers_1
	void release_at_exit() { Display::destroy(); }

//@hole device_helpers_2
	void resize_targets() {
		if (!g_swap) return;
//@end
//@define resize_device
		if (w < 1) w = 1;
		if (h < 1) h = 1;
//@hole resize_device_1
//@end
//@define resize_done: 		g_needResize = false;
//@define resize_flag: 		g_needResize = true;
//@define fatal_comment
// A window or device that cannot be made ends the program (exit code 3), after saying why: LWJGL's Display.create throws there,
// and the game has nothing to draw with.
//@end
//@define fail: 		std::exit(3);
//@define create_device_1: //@hole create_device_1_1
//@define create_device_2
//@hole create_device_2_1
		std::exit(3);
//@end
//@define create_device_3
//@hole create_device_3_1
		std::fprintf(stderr, "[display] the Direct3D @V@ renderer could not start\n");
		std::exit(3);
	}
	static bool registered = false;
	if (!registered) { registered = true; std::atexit(release_at_exit); }
	j2xgl::glInit();
	j2xgl::glViewport(0, 0, w, h);
//@end
//@define destroy_device: //@hole destroy_device_1
//@define resize_flag_deep: 				g_needResize = true;
//@define present_comment: // Present the frame. A removed device is said once; the game goes on (drawing nothing) rather than dying.
//@define present_body
	if (!g_swap) return;
	j2xgl::glFlush();   // the layer's pending batch and this frame's new display-list buffers
//@hole present_body_1
	if ((hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) && !g_removed) {
		g_removed = true;
		DbgPrintF("[display] the Direct3D @V@ device was removed (0x%08x): reason 0x%08x\n", (unsigned)hr, (unsigned)g_dev->GetDeviceRemovedReason());
	}
	if (g_needResize) resize_targets();
//@end
//@define save_frame_body
	std::vector<unsigned int> px;
	int w = 0, h = 0;
	if (!j2xgl::gpu::readFrame(px, w, h) || px.empty()) return;
	SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 24, SDL_PIXELFORMAT_BGR24);
	if (!s) return;
	for (int y = 0; y < h; ++y) {       // ARGB words -> B, G, R bytes
		unsigned char* dst = (unsigned char*)s->pixels + (size_t)y * s->pitch;
		for (int x = 0; x < w; ++x) {
			const unsigned int p = px[(size_t)y * w + x];
			dst[x * 3] = (unsigned char)p; dst[x * 3 + 1] = (unsigned char)(p >> 8); dst[x * 3 + 2] = (unsigned char)(p >> 16);
//@end
//@define save_frame_end
	SDL_SaveBMP(s, file);
	SDL_FreeSurface(s);
	std::fprintf(stderr, "[display] frame saved to %s\n", file);
//@end
//@define flush_for_shot: 	if (shoot || scripted_shot) j2xgl::glFlush();   // (a shot is the frame as drawn so far, the layer's pending batch in it)
//@define fullscreen_comment: // Full screen is the desktop-sized borderless window (the swap chain stays windowed: no exclusive mode to lose on alt-tab).
//@define vsync_fn: void setVSyncEnabled(bool on) { g_vsync = on; }   // (Present's sync interval, frame by frame)
