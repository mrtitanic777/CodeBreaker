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
#include "GL11.h"       // GL11::glSetDevice / glInit / glViewport
#include "Common.h"     // DbgPrintF -- boot trace that surfaces on the 360/emulator; g_pd3dDevice extern
#include "Display.h"

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

void setDisplayMode(DisplayMode* mode) { if (mode) s_mode = *mode; }
void setDisplayMode(const DisplayMode& mode) { s_mode = mode; }

void create() {
	if (s_created) return;
	DbgPrintF("[J2X] Display create begin\n");
	s_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!s_d3d) { DbgPrintF("[J2X] Direct3DCreate9 FAILED (null)\n"); return; }

	int w = s_mode.getWidth()  > 0 ? s_mode.getWidth()  : 854;
	int h = s_mode.getHeight() > 0 ? s_mode.getHeight() : 480;
	DbgPrintF("[J2X] requested backbuffer %dx%d\n", w, h);

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.BackBufferWidth            = w;
	pp.BackBufferHeight           = h;
	pp.BackBufferFormat           = D3DFMT_X8R8G8B8;
	pp.BackBufferCount            = 1;
	pp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	pp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	pp.EnableAutoDepthStencil     = TRUE;
	pp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = 60;
	pp.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;

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
	GL11::glSetDevice(s_dev);
	GL11::glInit();
	GL11::glViewport(0, 0, w, h);
	s_dev->SetRenderState(D3DRS_HALFPIXELOFFSET, TRUE);   // pixel-exact 2D on 360 (HARDWARE-VERIFY)
	s_dev->BeginScene();
	s_created = true;
}

void swapBuffers() {
	if (s_dev) { s_dev->EndScene(); s_dev->Present(NULL, NULL, NULL, NULL); s_dev->BeginScene(); }
}

void update() {
	static int frame = 0;
	if (frame < 3) DbgPrintF("[J2X] render loop alive: presenting frame %d\n", frame);
	++frame;
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
