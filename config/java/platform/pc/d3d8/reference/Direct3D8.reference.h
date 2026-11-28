// Direct3D8.h -- the Direct3D 8 interface, as much of it as the GL layer's Direct3D 8 backend (GpuD3D8.cpp) and Display use, for
// every Windows compiler. The Windows SDK stopped shipping d3d8.h long ago (MinGW-w64 still has one, Visual C++ and clang-cl have
// none), and the runtime, d3d8.dll, is still in every 32-bit Windows (SysWOW64 on 64-bit Windows): the program loads it and asks
// for Direct3DCreate8 itself, so no import library is needed either.
//
// The interfaces are COM vtables: every method is listed in its place, in Direct3D 8's order, so the slots line up with the
// runtime's; the methods the backend never calls take no arguments here (their slot is all that matters). The values and
// structures are Direct3D 8's own (D3D_SDK_VERSION 220). All of it is in namespace d3d8, beside any other Direct3D header.
#ifndef DIRECT3D8_H
#define DIRECT3D8_H

#include <windows.h>
#include <unknwn.h>   // (IUnknown: windows.h leaves COM out under WIN32_LEAN_AND_MEAN, as the game's stdafx.h asks)

namespace d3d8 {

typedef DWORD D3DCOLOR;   // ARGB

// ---------------------------------------------------------------------------------------------------------------- values
enum { D3D_SDK_VERSION = 220 };
enum { D3DADAPTER_DEFAULT = 0 };
enum { D3DDEVTYPE_HAL = 1, D3DDEVTYPE_REF = 2 };
enum {
	D3DCREATE_FPU_PRESERVE = 0x02, D3DCREATE_MULTITHREADED = 0x04,
	D3DCREATE_SOFTWARE_VERTEXPROCESSING = 0x20, D3DCREATE_HARDWARE_VERTEXPROCESSING = 0x40
};
typedef DWORD D3DFORMAT;
enum { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_X8R8G8B8 = 22, D3DFMT_D24S8 = 75, D3DFMT_D24X8 = 77, D3DFMT_D16 = 80 };
typedef DWORD D3DPOOL;
enum { D3DPOOL_DEFAULT = 0, D3DPOOL_MANAGED = 1, D3DPOOL_SYSTEMMEM = 2 };
enum { D3DUSAGE_WRITEONLY = 0x08, D3DUSAGE_DYNAMIC = 0x200 };
enum { D3DLOCK_READONLY = 0x10, D3DLOCK_NOOVERWRITE = 0x1000, D3DLOCK_DISCARD = 0x2000 };
enum { D3DSWAPEFFECT_DISCARD = 1, D3DSWAPEFFECT_FLIP = 2, D3DSWAPEFFECT_COPY = 3, D3DSWAPEFFECT_COPY_VSYNC = 4 };
enum { D3DMULTISAMPLE_NONE = 0 };
enum { D3DPRESENTFLAG_LOCKABLE_BACKBUFFER = 0x1 };
enum { D3DPRESENT_INTERVAL_DEFAULT = 0 };
enum { D3DBACKBUFFER_TYPE_MONO = 0 };
enum { D3DCLEAR_TARGET = 1, D3DCLEAR_ZBUFFER = 2, D3DCLEAR_STENCIL = 4 };
enum { D3DPT_LINELIST = 2, D3DPT_TRIANGLELIST = 4 };
enum { D3DTS_VIEW = 2, D3DTS_PROJECTION = 3, D3DTS_TEXTURE0 = 16, D3DTS_WORLD = 256 };
enum {   // D3DRENDERSTATETYPE
	D3DRS_ZENABLE = 7, D3DRS_FILLMODE = 8, D3DRS_SHADEMODE = 9, D3DRS_ZWRITEENABLE = 14, D3DRS_ALPHATESTENABLE = 15,
	D3DRS_SRCBLEND = 19, D3DRS_DESTBLEND = 20, D3DRS_CULLMODE = 22, D3DRS_ZFUNC = 23, D3DRS_ALPHAREF = 24, D3DRS_ALPHAFUNC = 25,
	D3DRS_DITHERENABLE = 26, D3DRS_ALPHABLENDENABLE = 27, D3DRS_FOGENABLE = 28, D3DRS_SPECULARENABLE = 29, D3DRS_FOGCOLOR = 34,
	D3DRS_FOGTABLEMODE = 35, D3DRS_FOGSTART = 36, D3DRS_FOGEND = 37, D3DRS_FOGDENSITY = 38, D3DRS_RANGEFOGENABLE = 48,
	D3DRS_STENCILENABLE = 52, D3DRS_TEXTUREFACTOR = 60, D3DRS_CLIPPING = 136, D3DRS_LIGHTING = 137, D3DRS_AMBIENT = 139,
	D3DRS_FOGVERTEXMODE = 140, D3DRS_COLORVERTEX = 141, D3DRS_LOCALVIEWER = 142, D3DRS_NORMALIZENORMALS = 143,
	D3DRS_DIFFUSEMATERIALSOURCE = 145, D3DRS_SPECULARMATERIALSOURCE = 146, D3DRS_AMBIENTMATERIALSOURCE = 147,
	D3DRS_EMISSIVEMATERIALSOURCE = 148, D3DRS_COLORWRITEENABLE = 168, D3DRS_BLENDOP = 171
};
enum {   // D3DTEXTURESTAGESTATETYPE (Direct3D 8 keeps the sampler's states here too)
	D3DTSS_COLOROP = 1, D3DTSS_COLORARG1 = 2, D3DTSS_COLORARG2 = 3, D3DTSS_ALPHAOP = 4, D3DTSS_ALPHAARG1 = 5, D3DTSS_ALPHAARG2 = 6,
	D3DTSS_TEXCOORDINDEX = 11, D3DTSS_ADDRESSU = 13, D3DTSS_ADDRESSV = 14, D3DTSS_MAGFILTER = 16, D3DTSS_MINFILTER = 17,
	D3DTSS_MIPFILTER = 18, D3DTSS_TEXTURETRANSFORMFLAGS = 24, D3DTSS_RESULTARG = 28
};
enum { D3DTSS_TCI_PASSTHRU = 0x00000, D3DTSS_TCI_CAMERASPACEPOSITION = 0x20000 };
enum { D3DTTFF_DISABLE = 0, D3DTTFF_COUNT2 = 2, D3DTTFF_COUNT4 = 4, D3DTTFF_PROJECTED = 256 };
enum { D3DTADDRESS_WRAP = 1, D3DTADDRESS_CLAMP = 3 };
enum { D3DTEXF_NONE = 0, D3DTEXF_POINT = 1, D3DTEXF_LINEAR = 2 };
enum { D3DTOP_DISABLE = 1 };   // (the rest: GpuFixed.h's kTop*, the same in Direct3D 7 and 8)
enum { D3DTA_CURRENT = 1 };
enum { D3DZB_FALSE = 0, D3DZB_TRUE = 1 };
enum { D3DFILL_SOLID = 3 };
enum { D3DSHADE_FLAT = 1, D3DSHADE_GOURAUD = 2 };
enum { D3DCULL_NONE = 1, D3DCULL_CW = 2, D3DCULL_CCW = 3 };
enum { D3DCMP_NEVER = 1 };     // .. D3DCMP_ALWAYS = 8, in gpu::Compare's order
enum { D3DBLEND_ZERO = 1 };    // .. D3DBLEND_SRCALPHASAT = 11, in gpu::Blend's order
enum { D3DBLENDOP_ADD = 1 };
enum { D3DFOG_NONE = 0, D3DFOG_EXP = 1, D3DFOG_EXP2 = 2, D3DFOG_LINEAR = 3 };
enum { D3DMCS_MATERIAL = 0, D3DMCS_COLOR1 = 1 };
enum { D3DLIGHT_POINT = 1, D3DLIGHT_DIRECTIONAL = 3 };

// MAKE_HRESULT(1, 0x876, code)
const HRESULT D3DERR_DEVICELOST = (HRESULT)0x88760868L;
const HRESULT D3DERR_DEVICENOTRESET = (HRESULT)0x88760869L;

// ---------------------------------------------------------------------------------------------------------------- structures
struct D3DCOLORVALUE { float r, g, b, a; };
struct D3DVECTOR { float x, y, z; };
struct D3DMATRIX { float m[4][4]; };
struct D3DRECT { LONG x1, y1, x2, y2; };
struct D3DVIEWPORT8 { DWORD X, Y, Width, Height; float MinZ, MaxZ; };
struct D3DMATERIAL8 { D3DCOLORVALUE Diffuse, Ambient, Specular, Emissive; float Power; };
struct D3DLIGHT8 {
	DWORD Type;
	D3DCOLORVALUE Diffuse, Specular, Ambient;
	D3DVECTOR Position, Direction;
	float Range, Falloff, Attenuation0, Attenuation1, Attenuation2, Theta, Phi;
};
struct D3DLOCKED_RECT { INT Pitch; void* pBits; };
struct D3DSURFACE_DESC { D3DFORMAT Format; DWORD Type; DWORD Usage; D3DPOOL Pool; UINT Size; DWORD MultiSampleType; UINT Width, Height; };
struct D3DPRESENT_PARAMETERS {
	UINT BackBufferWidth, BackBufferHeight;
	D3DFORMAT BackBufferFormat;
	UINT BackBufferCount;
	DWORD MultiSampleType;
	DWORD SwapEffect;
	HWND hDeviceWindow;
	BOOL Windowed;
	BOOL EnableAutoDepthStencil;
	D3DFORMAT AutoDepthStencilFormat;
	DWORD Flags;
	UINT FullScreen_RefreshRateInHz;
	UINT FullScreen_PresentationInterval;
};

// ---------------------------------------------------------------------------------------------------------------- interfaces
#define D3D8_SLOT(name) virtual HRESULT STDMETHODCALLTYPE name() = 0   // (a method never called: its place only)

struct IDirect3DDevice8;

struct IDirect3DSurface8 : public IUnknown {
	D3D8_SLOT(GetDevice); D3D8_SLOT(SetPrivateData); D3D8_SLOT(GetPrivateData); D3D8_SLOT(FreePrivateData); D3D8_SLOT(GetContainer);
	virtual HRESULT STDMETHODCALLTYPE GetDesc(D3DSURFACE_DESC* desc) = 0;
	virtual HRESULT STDMETHODCALLTYPE LockRect(D3DLOCKED_RECT* locked, const RECT* rect, DWORD flags) = 0;
	virtual HRESULT STDMETHODCALLTYPE UnlockRect() = 0;
};

struct IDirect3DVertexBuffer8 : public IUnknown {
	D3D8_SLOT(GetDevice); D3D8_SLOT(SetPrivateData); D3D8_SLOT(GetPrivateData); D3D8_SLOT(FreePrivateData);
	D3D8_SLOT(SetPriority); D3D8_SLOT(GetPriority); D3D8_SLOT(PreLoad); D3D8_SLOT(GetType);
	virtual HRESULT STDMETHODCALLTYPE Lock(UINT offset, UINT size, BYTE** data, DWORD flags) = 0;
	virtual HRESULT STDMETHODCALLTYPE Unlock() = 0;
	D3D8_SLOT(GetDesc);
};

struct IDirect3DTexture8 : public IUnknown {   // (IDirect3DResource8, then IDirect3DBaseTexture8, then its own)
	D3D8_SLOT(GetDevice); D3D8_SLOT(SetPrivateData); D3D8_SLOT(GetPrivateData); D3D8_SLOT(FreePrivateData);
	D3D8_SLOT(SetPriority); D3D8_SLOT(GetPriority); D3D8_SLOT(PreLoad); D3D8_SLOT(GetType);
	D3D8_SLOT(SetLOD); D3D8_SLOT(GetLOD); D3D8_SLOT(GetLevelCount);
	D3D8_SLOT(GetLevelDesc); D3D8_SLOT(GetSurfaceLevel);
	virtual HRESULT STDMETHODCALLTYPE LockRect(UINT level, D3DLOCKED_RECT* locked, const RECT* rect, DWORD flags) = 0;
	virtual HRESULT STDMETHODCALLTYPE UnlockRect(UINT level) = 0;
	D3D8_SLOT(AddDirtyRect);
};

struct IDirect3DDevice8 : public IUnknown {
	virtual HRESULT STDMETHODCALLTYPE TestCooperativeLevel() = 0;
	D3D8_SLOT(GetAvailableTextureMem); D3D8_SLOT(ResourceManagerDiscardBytes); D3D8_SLOT(GetDirect3D); D3D8_SLOT(GetDeviceCaps);
	D3D8_SLOT(GetDisplayMode); D3D8_SLOT(GetCreationParameters); D3D8_SLOT(SetCursorProperties); D3D8_SLOT(SetCursorPosition);
	D3D8_SLOT(ShowCursor); D3D8_SLOT(CreateAdditionalSwapChain);
	virtual HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pp) = 0;
	virtual HRESULT STDMETHODCALLTYPE Present(const RECT* src, const RECT* dst, HWND window, const void* dirty) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT index, DWORD type, IDirect3DSurface8** surface) = 0;
	D3D8_SLOT(GetRasterStatus); D3D8_SLOT(SetGammaRamp); D3D8_SLOT(GetGammaRamp);
	virtual HRESULT STDMETHODCALLTYPE CreateTexture(UINT w, UINT h, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, IDirect3DTexture8** tex) = 0;
	D3D8_SLOT(CreateVolumeTexture); D3D8_SLOT(CreateCubeTexture);
	virtual HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT bytes, DWORD usage, DWORD fvf, D3DPOOL pool, IDirect3DVertexBuffer8** vb) = 0;
	D3D8_SLOT(CreateIndexBuffer); D3D8_SLOT(CreateRenderTarget); D3D8_SLOT(CreateDepthStencilSurface); D3D8_SLOT(CreateImageSurface);
	D3D8_SLOT(CopyRects); D3D8_SLOT(UpdateTexture); D3D8_SLOT(GetFrontBuffer); D3D8_SLOT(SetRenderTarget); D3D8_SLOT(GetRenderTarget);
	D3D8_SLOT(GetDepthStencilSurface);
	virtual HRESULT STDMETHODCALLTYPE BeginScene() = 0;
	virtual HRESULT STDMETHODCALLTYPE EndScene() = 0;
	virtual HRESULT STDMETHODCALLTYPE Clear(DWORD count, const D3DRECT* rects, DWORD flags, D3DCOLOR color, float z, DWORD stencil) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetTransform(DWORD state, const D3DMATRIX* m) = 0;
	D3D8_SLOT(GetTransform); D3D8_SLOT(MultiplyTransform);
	virtual HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT8* vp) = 0;
	D3D8_SLOT(GetViewport);
	virtual HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL8* m) = 0;
	D3D8_SLOT(GetMaterial);
	virtual HRESULT STDMETHODCALLTYPE SetLight(DWORD index, const D3DLIGHT8* light) = 0;
	D3D8_SLOT(GetLight);
	virtual HRESULT STDMETHODCALLTYPE LightEnable(DWORD index, BOOL on) = 0;
	D3D8_SLOT(GetLightEnable); D3D8_SLOT(SetClipPlane); D3D8_SLOT(GetClipPlane);
	virtual HRESULT STDMETHODCALLTYPE SetRenderState(DWORD state, DWORD value) = 0;
	D3D8_SLOT(GetRenderState); D3D8_SLOT(BeginStateBlock); D3D8_SLOT(EndStateBlock); D3D8_SLOT(ApplyStateBlock);
	D3D8_SLOT(CaptureStateBlock); D3D8_SLOT(DeleteStateBlock); D3D8_SLOT(CreateStateBlock); D3D8_SLOT(SetClipStatus);
	D3D8_SLOT(GetClipStatus); D3D8_SLOT(GetTexture);
	virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD stage, IDirect3DTexture8* tex) = 0;   // (an IDirect3DBaseTexture8: the same pointer)
	D3D8_SLOT(GetTextureStageState);
	virtual HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD stage, DWORD type, DWORD value) = 0;
	D3D8_SLOT(ValidateDevice); D3D8_SLOT(GetInfo); D3D8_SLOT(SetPaletteEntries); D3D8_SLOT(GetPaletteEntries);
	D3D8_SLOT(SetCurrentTexturePalette); D3D8_SLOT(GetCurrentTexturePalette);
	virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(DWORD type, UINT start, UINT primitives) = 0;
	D3D8_SLOT(DrawIndexedPrimitive);
	virtual HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(DWORD type, UINT primitives, const void* vertices, UINT stride) = 0;
	D3D8_SLOT(DrawIndexedPrimitiveUP); D3D8_SLOT(ProcessVertices); D3D8_SLOT(CreateVertexShader);
	virtual HRESULT STDMETHODCALLTYPE SetVertexShader(DWORD fvfOrHandle) = 0;   // (a fixed-function FVF code is a "shader" in Direct3D 8)
	D3D8_SLOT(GetVertexShader); D3D8_SLOT(DeleteVertexShader); D3D8_SLOT(SetVertexShaderConstant); D3D8_SLOT(GetVertexShaderConstant);
	D3D8_SLOT(GetVertexShaderDeclaration); D3D8_SLOT(GetVertexShaderFunction);
	virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT stream, IDirect3DVertexBuffer8* vb, UINT stride) = 0;
	D3D8_SLOT(GetStreamSource); D3D8_SLOT(SetIndices); D3D8_SLOT(GetIndices); D3D8_SLOT(CreatePixelShader); D3D8_SLOT(SetPixelShader);
	D3D8_SLOT(GetPixelShader); D3D8_SLOT(DeletePixelShader); D3D8_SLOT(SetPixelShaderConstant); D3D8_SLOT(GetPixelShaderConstant);
	D3D8_SLOT(GetPixelShaderFunction); D3D8_SLOT(DrawRectPatch); D3D8_SLOT(DrawTriPatch); D3D8_SLOT(DeletePatch);
};

struct IDirect3D8 : public IUnknown {
	D3D8_SLOT(RegisterSoftwareDevice); D3D8_SLOT(GetAdapterCount); D3D8_SLOT(GetAdapterIdentifier); D3D8_SLOT(GetAdapterModeCount);
	D3D8_SLOT(EnumAdapterModes); D3D8_SLOT(GetAdapterDisplayMode); D3D8_SLOT(CheckDeviceType); D3D8_SLOT(CheckDeviceFormat);
	D3D8_SLOT(CheckDeviceMultiSampleType); D3D8_SLOT(CheckDepthStencilMatch); D3D8_SLOT(GetDeviceCaps); D3D8_SLOT(GetAdapterMonitor);
	virtual HRESULT STDMETHODCALLTYPE CreateDevice(UINT adapter, DWORD type, HWND focus, DWORD flags, D3DPRESENT_PARAMETERS* pp, IDirect3DDevice8** device) = 0;
};

#undef D3D8_SLOT

// d3d8.dll's one export: the runtime itself (0 when this Windows has no Direct3D 8, a 64-bit program among them).
typedef IDirect3D8* (WINAPI* Direct3DCreate8Proc)(UINT sdkVersion);
inline IDirect3D8* create() {
	HMODULE dll = LoadLibraryA("d3d8.dll");
	if (!dll) return 0;
	Direct3DCreate8Proc make = (Direct3DCreate8Proc)(void*)GetProcAddress(dll, "Direct3DCreate8");
	return make ? make(D3D_SDK_VERSION) : 0;
}

}  // namespace d3d8

#endif // DIRECT3D8_H
