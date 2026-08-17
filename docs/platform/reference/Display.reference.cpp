// Display.cpp -- creates the D3D9 device and hands it to the GL emulator (see Display.reference.h).
//
// Device creation mirrors the Beta port's proven Main.cpp InitD3D (validated on real 360 hardware):
// Direct3DCreate9 + CreateDevice(adapter 0, HAL, HW vertex processing) + auto depth/stencil. The only
// change is WHEN it runs -- here, from the game's Display.create() call, instead of a Beta-specific
// Main. After the device is up we wire it into the GL11 emulator and init GL, then open the first scene.
//
// HARDWARE-VERIFY: exact 360 present params (backbuffer format/refresh), and D3DRS_HALFPIXELOFFSET for
// pixel-exact 2D (GUI/font). The input pump (Input::poll) is a no-op if the game links no Input layer.

#include "stdafx.h"     // xtl.h / d3d9.h / d3dx9.h
#include "GL11.h"       // d3d9::glSetDevice / glInit / glViewport
#include "Common.h"     // DbgPrintF -- boot trace that surfaces on the 360/emulator; g_pd3dDevice extern
#include "Display.h"
#include "SysNotify.h"  // system-notification queue: Guide stall, sign-in / controller changes

// Provided by the Input platform reference (controller -> Keyboard/Mouse event pump). Declared weakly
// here so Display.update() can pump input once per frame the way LWJGL's Display.update() does.
namespace Input { void poll(); }

// The app-global D3D device. Some platform companions (ImageIO / Mipmaps / Shaders) reference this
// `extern IDirect3DDevice9* g_pd3dDevice;` (declared in Common.h) for their own D3D work. Beta's Main.cpp
// used to DEFINE it; in the agnostic pipeline the generic entry has no such bootstrap, so Display (the
// device owner) defines it and keeps it pointed at the live device -- resolving the LNK2001 without
// making those companions depend on the GL emulator.
IDirect3DDevice9* g_pd3dDevice = 0;

namespace Display {

static IDirect3D9*       s_d3d = 0;
static IDirect3DDevice9* s_dev = 0;
static DisplayMode       s_mode(854, 480);   // sensible default until the game sets one
static bool              s_created = false;

// Ignored once the device is created (the entry creates it first): the backbuffer is fixed at creation,
// so a later setDisplayMode must NOT change the reported mode or the game's projection/viewport would
// desync from the actual backbuffer.
void setDisplayMode(DisplayMode* mode) { if (mode && !s_created) s_mode = *mode; }
void setDisplayMode(const DisplayMode& mode) { if (!s_created) s_mode = mode; }

void create() {
	if (s_created) return;
	DbgPrintF("[J2X] Display create begin\n");
	s_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!s_d3d) { DbgPrintF("[J2X] Direct3DCreate9 FAILED (null)\n"); return; }

	int w = s_mode.getWidth()  > 0 ? s_mode.getWidth()  : 854;
	int h = s_mode.getHeight() > 0 ? s_mode.getHeight() : 480;
#ifdef _XBOX
	// Xbox 360 HD REQUIRES a minimum 1280x720 frame buffer (a smaller one fails certification and
	// gets hardware-scaled, clipping the lower world). Force 720p and REPORT it as the display mode so
	// the game's projection/viewport (which reads getDisplayMode()) matches the backbuffer. Matches the
	// working hand-port's InitD3D. [J2X-BUGFIX fb-720p]
	if (w < 1280 || h < 720) { w = 1280; h = 720; s_mode = DisplayMode(w, h); }
#endif
	DbgPrintF("[J2X] backbuffer %dx%d\n", w, h);

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.BackBufferWidth            = w;
	pp.BackBufferHeight           = h;
	pp.BackBufferFormat           = D3DFMT_X8R8G8B8;
	pp.BackBufferCount            = 1;
	pp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	pp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow              = NULL;      // 360 has no window (match the working hand-port exactly)
	pp.Windowed                   = FALSE;
	pp.EnableAutoDepthStencil     = TRUE;
	pp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = 60;
	pp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = s_d3d->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
	                                 D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &s_dev);
	if (FAILED(hr)) {
		DbgPrintF("[J2X] CreateDevice FAILED hr=0x%08x\n", (unsigned)hr);
		s_d3d->Release(); s_d3d = 0; return;
	}
	DbgPrintF("[J2X] CreateDevice OK; device is up\n");

	// Hand the device to the GL emulator and initialize GL state, then set the viewport to the
	// backbuffer size and open the first scene (Display::update EndScene/Present/BeginScene per frame).
	g_pd3dDevice = s_dev;            // publish to the app-global the platform companions reference
	d3d9::glSetDevice(s_dev);
	d3d9::glInit();
	d3d9::glViewport(0, 0, w, h);
	s_dev->SetRenderState(D3DRS_HALFPIXELOFFSET, TRUE);   // pixel-exact 2D on 360 (HARDWARE-VERIFY)
	s_dev->BeginScene();
	s_created = true;
}

void swapBuffers() {
	d3d9::glFlush();   // [J2X-PERF] drain the coalesced entity batch before presenting the frame
	if (s_dev) { s_dev->EndScene(); s_dev->Present(NULL, NULL, NULL, NULL); s_dev->BeginScene(); }
}

void update() {
	static int frame = 0;
	if (frame < 3) DbgPrintF("[J2X] render loop alive: presenting frame %d\n", frame);
	++frame;
	// System notifications FIRST, before input and before the present. This is the one point a converted
	// game reaches every frame, so it is where the console's notification queue gets drained -- and while
	// the Guide overlay is up SysNotify::poll() does not return, which stalls the caller's game loop
	// (no ticks, no new audio, last frame left on screen). Simulating or playing audio under the Guide is
	// a cert failure; see SysNotify.h.
	SysNotify::poll();
	Input::poll(); swapBuffers();
}

void destroy() {
	if (s_dev) { s_dev->EndScene(); s_dev->Release(); s_dev = 0; }
	if (s_d3d) { s_d3d->Release(); s_d3d = 0; }
	g_pd3dDevice = 0;
	s_created = false;
}

bool isCloseRequested() { return false; }   // console title has no window close button
bool isActive()         { return true; }    // the title is always the foreground app
bool isCreated()        { return s_created; }
int  getWidth()         { return s_mode.getWidth(); }
int  getHeight()        { return s_mode.getHeight(); }
DisplayMode getDisplayMode()        { return s_mode; }
DisplayMode getDesktopDisplayMode() { return s_mode; }

} // namespace Display
