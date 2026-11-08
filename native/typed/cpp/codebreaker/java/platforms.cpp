// codebreaker/java/platforms.cpp -- typed C++ port of codebreaker.java.platforms (see platforms.hpp).
//
// platforms.py -- target platform definitions for stub annotation.
//
// Each platform maps the converter's subsystem IDs (from stubs.py) to the
// platform-specific SDK, API, and library names that should fill that stub.
// The converter emits these as comments inside the stub body so the developer
// knows exactly what to reach for.

#include "codebreaker/java/platforms.hpp"

namespace codebreaker::java::platforms {

// The stdafx fragments below sit before PLATFORMS in the Python's order of use; here they come first so the
// tables that mention them are built after them (same translation unit, in order).

const py::str _STDAFX_COMMON_CRT = py::str(R"CB(#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <ctime>
#include <malloc.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
)CB");
// No platform, built on a Windows host (MSVC, mingw): the runtime's file and thread helpers (File.h,
// Class.h) call the Win32 kernel API, as they do on the 360 through <xtl.h>. Lean, and without GDI --
// GDI's Rectangle() function would hide java.awt.Rectangle.
const py::str _STDAFX_HOST_WINDOWS = py::str(R"CB(#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
#endif
)CB");
// Math.max/min are emitted as max/min (the XDK and MSVC's <windows.h> define them as macros). Without a
// platform on another compiler there are none: these give Java's overloads -- both sides widened to the
// wider of int, long long, float, double -- as functions, and only where no macro already exists.
const py::str _STDAFX_HOST_MINMAX = py::str(R"CB(#if !defined(max) && !defined(min)
template<class T> struct j2x_num { enum { r = 1 }; };                // char, short, int -> int
template<> struct j2x_num<long long> { enum { r = 2 }; };
template<> struct j2x_num<float> { enum { r = 3 }; };
template<> struct j2x_num<double> { enum { r = 4 }; };
template<int R> struct j2x_numtype { typedef int type; };
template<> struct j2x_numtype<2> { typedef long long type; };
template<> struct j2x_numtype<3> { typedef float type; };
template<> struct j2x_numtype<4> { typedef double type; };
template<class A, class B> struct j2x_wider {
	typedef typename j2x_numtype<((int)j2x_num<A>::r > (int)j2x_num<B>::r ? (int)j2x_num<A>::r : (int)j2x_num<B>::r)>::type type;
};
template<class A, class B> inline typename j2x_wider<A, B>::type max(A a, B b) {
	typedef typename j2x_wider<A, B>::type W; return (W)a > (W)b ? (W)a : (W)b;
}
template<class A, class B> inline typename j2x_wider<A, B>::type min(A a, B b) {
	typedef typename j2x_wider<A, B>::type W; return (W)a < (W)b ? (W)a : (W)b;
}
#endif
)CB");
const py::str _STDAFX_MSVC_WARNINGS = py::str(R"CB(#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion float/double
#pragma warning(disable: 4305) // truncation double to float
#pragma warning(disable: 4355) // 'this' in base-member init list
)CB");
const py::str _STDAFX_COMMON_TYPEDEFS = py::str(R"CB(typedef unsigned char byte;
#define null NULL
)CB");

const py::dict<py::str, Platform> PLATFORMS = {
    {py::str("xbox360"), Platform{
        py::str("Xbox 360"), py::str("Microsoft Xbox 360 (XDK / Visual Studio 2010)"),
        std::optional<py::str>(py::str("PlatformSDKs/WindowsSDK7.0")),
        true, false, py::str("hard crash (IP 0x0) on Xbox 360 (no NULL page protection)"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("D3D9"), py::str("d3d9.lib, d3dx9.lib"), py::str("IDirect3DDevice9, IDirect3DTexture9, vertex shaders (HLSL), D3DXMatrixPerspectiveFovLH")}},
            {py::str("audio"), Subsystem{py::str("XACT3"), py::str("xact3.lib, xact3wb.lib"), py::str("IXACT3Engine, IXACT3SoundBank, IXACT3WaveBank, IXACT3Cue")}},
            {py::str("input"), Subsystem{py::str("XInput2"), py::str("xinput2.lib"), py::str("XInputGetState, XINPUT_STATE, XINPUT_GAMEPAD")}},
            {py::str("io"), Subsystem{py::str("XDK File"), py::str("xbdm.lib"), py::str("CreateFile, ReadFile, WriteFile, XContentCreate")}},
            {py::str("compression"), Subsystem{py::str("LZX"), py::str("xcompress.lib"), py::str("XMemCreateDecompressionContext, XMemDecompress")}},
            {py::str("networking"), Subsystem{py::str("XNet"), py::str("xnetn.lib, xonline.lib"), py::str("XNetStartup, XNetCreateKey, WSAStartup, socket, connect")}},
            {py::str("nio"), Subsystem{py::str("D3D9"), py::str("d3d9.lib"), py::str("IDirect3DVertexBuffer9::Lock, direct memory (malloc/free)")}},
            {py::str("threading"), Subsystem{py::str("Win32"), py::str("kernel32.lib"), py::str("CreateThread, _beginthreadex, WaitForSingleObject")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(built-in)"), py::str("strftime, sprintf, _snprintf")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(built-in)"), py::str("typeid, dynamic_cast, manual factory registry")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(built-in)"), py::str("std::exception, std::runtime_error")}},
            {py::str("system"), Subsystem{py::str("XDK"), py::str("kernel32.lib"), py::str("QueryPerformanceCounter, QueryPerformanceFrequency, GetTickCount")}},
        }}},
    {py::str("pc_windows"), Platform{
        py::str("PC (Windows / DirectX)"), py::str("Windows PC with DirectX 9/11 (Visual Studio)"),
        std::optional<py::str>(py::str("PlatformSDKs/WindowsSDK7.0")),
        false, true, py::str("access violation (segfault) on Windows -- OS catches it but the process still crashes"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("D3D9/D3D11"), py::str("d3d9.lib or d3d11.lib, d3dx9.lib"), py::str("IDirect3DDevice9 or ID3D11Device, DXGI, HLSL shaders")}},
            {py::str("audio"), Subsystem{py::str("XAudio2"), py::str("xaudio2.lib"), py::str("IXAudio2, IXAudio2SourceVoice, X3DAudio")}},
            {py::str("input"), Subsystem{py::str("XInput"), py::str("xinput.lib"), py::str("XInputGetState, XINPUT_STATE, Raw Input (keyboard/mouse)")}},
            {py::str("io"), Subsystem{py::str("Win32"), py::str("kernel32.lib"), py::str("CreateFile, ReadFile, WriteFile, SHGetFolderPath")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("zlib.lib or miniz"), py::str("inflate, deflate, gzopen, gzread")}},
            {py::str("networking"), Subsystem{py::str("Winsock2"), py::str("ws2_32.lib"), py::str("WSAStartup, socket, connect, send, recv")}},
            {py::str("nio"), Subsystem{py::str("D3D9/D3D11"), py::str("d3d9.lib"), py::str("vertex buffers, malloc/free, VirtualAlloc")}},
            {py::str("threading"), Subsystem{py::str("Win32"), py::str("kernel32.lib"), py::str("CreateThread, _beginthreadex, InitializeCriticalSection")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(built-in)"), py::str("strftime, sprintf_s, std::locale")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(built-in)"), py::str("typeid, dynamic_cast, manual factory registry")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(built-in)"), py::str("std::exception, std::runtime_error")}},
            {py::str("system"), Subsystem{py::str("Win32"), py::str("kernel32.lib"), py::str("QueryPerformanceCounter, GetTickCount64, timeGetTime")}},
        }}},
    {py::str("pc_opengl"), Platform{
        py::str("PC (OpenGL / SDL)"), py::str("Cross-platform PC with OpenGL + SDL2"),
        std::nullopt,
        false, true, py::str("segfault on PC -- OS catches it but the process still crashes"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("OpenGL"), py::str("opengl32.lib, glew, SDL2"), py::str("glBegin, glEnd, glBindTexture, glDrawArrays, GLSL shaders")}},
            {py::str("audio"), Subsystem{py::str("OpenAL"), py::str("openal32.lib or SDL2_mixer"), py::str("alGenSources, alSourcePlay, alListener3f")}},
            {py::str("input"), Subsystem{py::str("SDL2"), py::str("SDL2.lib"), py::str("SDL_PollEvent, SDL_GetKeyboardState, SDL_GetMouseState")}},
            {py::str("io"), Subsystem{py::str("stdio"), py::str("(built-in)"), py::str("fopen, fread, fwrite, std::ifstream")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("zlib or miniz"), py::str("inflate, deflate, gzopen")}},
            {py::str("networking"), Subsystem{py::str("BSD sockets"), py::str("(built-in on Unix, ws2_32 on Win)"), py::str("socket, connect, send, recv, select")}},
            {py::str("nio"), Subsystem{py::str("OpenGL"), py::str("opengl32.lib"), py::str("glGenBuffers, glBufferData (VBO), malloc/free")}},
            {py::str("threading"), Subsystem{py::str("pthreads"), py::str("pthread or C++11 <thread>"), py::str("pthread_create, std::thread, std::mutex")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(built-in)"), py::str("strftime, sprintf, std::locale")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(built-in)"), py::str("typeid, dynamic_cast, manual factory registry")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(built-in)"), py::str("std::exception, std::runtime_error")}},
            {py::str("system"), Subsystem{py::str("SDL2"), py::str("SDL2.lib"), py::str("SDL_GetTicks, SDL_GetPerformanceCounter")}},
        }}},
    {py::str("ps3"), Platform{
        py::str("PlayStation 3"), py::str("Sony PlayStation 3 (SN Systems / GCC SNC)"),
        std::nullopt,
        true, false, py::str("hard crash on PS3 (no NULL page protection on Cell)"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("GCM/RSX"), py::str("libgcm_cmd.a, libgcm_sys.a"), py::str("cellGcmInit, cellGcmSetSurface, RSX vertex/fragment programs (Cg)")}},
            {py::str("audio"), Subsystem{py::str("MultiStream"), py::str("libmstream.a, libsnd3.a"), py::str("cellMSSystemConfigureSoundIO, cellSnd3Init, cellSnd3NoteOn")}},
            {py::str("input"), Subsystem{py::str("libpad"), py::str("libpad.a"), py::str("cellPadInit, cellPadGetData, CellPadData")}},
            {py::str("io"), Subsystem{py::str("Cell FS"), py::str("libfs.a"), py::str("cellFsOpen, cellFsRead, cellFsWrite, cellFsStat")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("libz.a (Cell zlib)"), py::str("inflate, deflate (same as zlib)")}},
            {py::str("networking"), Subsystem{py::str("libnet"), py::str("libnet.a, libnetctl.a"), py::str("cellNetCtlInit, sys_net_initialize_network, socket, connect")}},
            {py::str("nio"), Subsystem{py::str("GCM/RSX"), py::str("libgcm_cmd.a"), py::str("cellGcmMapMainMemory, RSX vertex arrays, malloc/free")}},
            {py::str("threading"), Subsystem{py::str("SPU/PPU"), py::str("libsysutil.a, libspurs.a"), py::str("sys_ppu_thread_create, cellSpursInit, sys_lwmutex_create")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(built-in)"), py::str("strftime, snprintf")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(built-in)"), py::str("typeid, dynamic_cast, manual factory registry")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(built-in)"), py::str("std::exception, std::runtime_error")}},
            {py::str("system"), Subsystem{py::str("Cell OS"), py::str("libsysutil.a"), py::str("sys_time_get_system_time, cellSysutilGetSystemParamInt")}},
        }}},
    {py::str("wii"), Platform{
        py::str("Nintendo Wii"), py::str("Nintendo Wii (devkitPPC / libogc or RVL_SDK)"),
        std::nullopt,
        true, false, py::str("hard crash on Wii (no NULL page protection on Broadway PPC)"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("GX"), py::str("libogc (GX)"), py::str("GX_Init, GX_Begin, GX_Position3f32, GX_LoadTexObj, GX_SetTevOp")}},
            {py::str("audio"), Subsystem{py::str("ASND/DSP"), py::str("libasnd.a"), py::str("ASND_Init, ASND_SetVoice, ASND_AddVoice")}},
            {py::str("input"), Subsystem{py::str("WPAD/PAD"), py::str("libwiiuse.a, libogc"), py::str("WPAD_Init, WPAD_ScanPads, WPAD_ButtonsDown, PAD_ScanPads")}},
            {py::str("io"), Subsystem{py::str("ISFS/FAT"), py::str("libfat.a, libogc"), py::str("ISFS_Open, ISFS_Read, fatInitDefault, fopen")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("libz.a (devkitPPC)"), py::str("inflate, deflate")}},
            {py::str("networking"), Subsystem{py::str("libnet"), py::str("libnet.a (libogc)"), py::str("net_init, net_socket, net_connect, net_send, net_recv")}},
            {py::str("nio"), Subsystem{py::str("GX"), py::str("libogc (GX)"), py::str("GX display lists, MEM2 heap (SYS_GetArena2Lo), memalign")}},
            {py::str("threading"), Subsystem{py::str("LWP"), py::str("libogc"), py::str("LWP_CreateThread, LWP_MutexInit, LWP_SemInit")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(newlib built-in)"), py::str("strftime, snprintf")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(limited on Wii)"), py::str("manual factory registry (RTTI may be disabled)")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(newlib)"), py::str("std::exception (if exceptions enabled; often disabled on Wii)")}},
            {py::str("system"), Subsystem{py::str("libogc"), py::str("libogc"), py::str("gettime, ticks_to_millisecs, SYS_GetArena1Size")}},
        }}},
    {py::str("psp"), Platform{
        py::str("PlayStation Portable"), py::str("Sony PSP (PSPSDK / devkitPSP)"),
        std::nullopt,
        false, false, py::str("hard crash on PSP (no NULL page protection on Allegrex MIPS)"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("GU/GE"), py::str("libpspgu.a, libpspge.a"), py::str("sceGuInit, sceGuStart, sceGuDrawArray, sceGuTexImage")}},
            {py::str("audio"), Subsystem{py::str("sceAudio"), py::str("libpspaudio.a"), py::str("sceAudioChReserve, sceAudioOutputBlocking, sceAudioSRCOutputBlocking")}},
            {py::str("input"), Subsystem{py::str("sceCtrl"), py::str("libpspctrl.a"), py::str("sceCtrlSetSamplingCycle, sceCtrlReadBufferPositive, SceCtrlData")}},
            {py::str("io"), Subsystem{py::str("sceIo"), py::str("libpspiofilemgr.a"), py::str("sceIoOpen, sceIoRead, sceIoWrite, sceIoLseek")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("libz.a"), py::str("inflate, deflate")}},
            {py::str("networking"), Subsystem{py::str("sceNet"), py::str("libpspnet.a, libpspwlan.a"), py::str("sceNetInit, sceNetAdhocInit, sceNetSocket")}},
            {py::str("nio"), Subsystem{py::str("GU/GE"), py::str("libpspgu.a"), py::str("sceGuGetMemory (VRAM alloc), memalign")}},
            {py::str("threading"), Subsystem{py::str("sceKernel"), py::str("libpspkernel.a"), py::str("sceKernelCreateThread, sceKernelStartThread, sceKernelCreateSema")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(newlib built-in)"), py::str("strftime, snprintf")}},
            {py::str("reflection"), Subsystem{py::str("RTTI"), py::str("(limited)"), py::str("manual factory registry")}},
            {py::str("exceptions"), Subsystem{py::str("C++ STL"), py::str("(newlib)"), py::str("std::exception (if enabled)")}},
            {py::str("system"), Subsystem{py::str("sceKernel"), py::str("libpsprtc.a"), py::str("sceKernelGetSystemTimeLow, sceRtcGetCurrentTick")}},
        }}},
    {py::str("nds"), Platform{
        py::str("Nintendo DS"), py::str("Nintendo DS (devkitARM / libnds)"),
        std::nullopt,
        false, false, py::str("hard crash on NDS (no NULL page protection on ARM7/ARM9)"),
        py::dict<py::str, Subsystem>{
            {py::str("rendering"), Subsystem{py::str("libnds 3D"), py::str("libnds"), py::str("glInit, glBegin, glVertex3f, glPolyFmt, glBindTexture (DS GL-like)")}},
            {py::str("audio"), Subsystem{py::str("maxmod"), py::str("libmm.a"), py::str("mmInit, mmLoadEffect, mmEffect, mmStart")}},
            {py::str("input"), Subsystem{py::str("libnds"), py::str("libnds"), py::str("scanKeys, keysDown, keysHeld, touchRead")}},
            {py::str("io"), Subsystem{py::str("libfat"), py::str("libfat.a"), py::str("fatInitDefault, fopen, fread, fwrite")}},
            {py::str("compression"), Subsystem{py::str("zlib"), py::str("libz.a (devkitARM)"), py::str("inflate, deflate")}},
            {py::str("networking"), Subsystem{py::str("dswifi"), py::str("libdswifi9.a"), py::str("Wifi_InitDefault, socket, connect (limited)")}},
            {py::str("nio"), Subsystem{py::str("libnds"), py::str("libnds"), py::str("glGetTexturePointer, malloc/free (limited RAM)")}},
            {py::str("threading"), Subsystem{py::str("N/A"), py::str("N/A"), py::str("single-threaded (cooperative; use IRQ handlers)")}},
            {py::str("text"), Subsystem{py::str("CRT"), py::str("(newlib)"), py::str("snprintf")}},
            {py::str("reflection"), Subsystem{py::str("N/A"), py::str("N/A"), py::str("manual factory registry (no RTTI)")}},
            {py::str("exceptions"), Subsystem{py::str("N/A"), py::str("N/A"), py::str("exceptions typically disabled on NDS")}},
            {py::str("system"), Subsystem{py::str("libnds"), py::str("libnds"), py::str("timerStart, timerElapsed, cpuStartTiming")}},
        }}},
};

static py::dict<py::str, StdafxConfig> _STDAFX_table() {
    py::dict<py::str, StdafxConfig> t;
    t.set(py::str("xbox360"), StdafxConfig{
        py::list<py::str>{py::str("#include <xtl.h>"), py::str("#include <xboxmath.h>")},
        py::list<py::str>{py::str("#include <d3d9.h>"), py::str("#include <d3dx9.h>"), py::str("#include <xaudio2.h>"), py::str("#include <xgraphics.h>")},
        py::list<py::str>{py::str("extern IDirect3D9*       g_pD3D;"), py::str("extern IDirect3DDevice9* g_pd3dDevice;")},
        _STDAFX_MSVC_WARNINGS,
        py::str(""),
        false,
        std::nullopt});
    t.set(py::str("pc_windows"), StdafxConfig{
        py::list<py::str>{py::str("#define WIN32_LEAN_AND_MEAN"), py::str("#include <windows.h>"), py::str("#include <mmsystem.h>")},
        py::list<py::str>{py::str("#include <d3d9.h>"), py::str("#include <d3dx9.h>")},
        py::list<py::str>{py::str("extern HWND              g_hWnd;"), py::str("extern HINSTANCE         g_hInstance;"), py::str("extern IDirect3D9*       g_pD3D;"), py::str("extern IDirect3DDevice9* g_pd3dDevice;")},
        _STDAFX_MSVC_WARNINGS,
        py::str("#pragma comment(lib, \"d3d9.lib\")\n#pragma comment(lib, \"d3dx9.lib\")\n"),
        false,
        std::nullopt});
    t.set(py::str("pc_opengl"), StdafxConfig{
        py::list<py::str>{_STDAFX_HOST_WINDOWS},
        py::list<py::str>{},
        py::list<py::str>{},
        py::str("#ifdef _MSC_VER\n") + _STDAFX_MSVC_WARNINGS + "#endif\n",
        py::str("// AWT components are as big as the SDL window (Display.cpp)\nint j2xDisplayWidth();\nint j2xDisplayHeight();\n#define J2X_COMPONENT_WIDTH() j2xDisplayWidth()\n#define J2X_COMPONENT_HEIGHT() j2xDisplayHeight()\n// the network: offline -- a connection that fails as Java's does with none (Network.cpp)\nclass URL; class URLConnection; class InputStream;\nURLConnection* j2xUrlOpenConnection(URL* u);\nInputStream* j2xUrlOpenStream(URL* u);\n#define J2X_URL_OPEN_CONNECTION(u) j2xUrlOpenConnection(u)\n#define J2X_URL_OPEN_STREAM(u) j2xUrlOpenStream(u)\n"),
        true,
        std::nullopt});
    t.set(py::str("ps3"), StdafxConfig{
        py::list<py::str>{py::str("#include <cell/cell_fs.h>"), py::str("#include <sys/process.h>"), py::str("#include <sys/ppu_thread.h>"), py::str("#include <sys/timer.h>"), py::str("#include <sys/sys_time.h>")},
        py::list<py::str>{py::str("#include <cell/gcm.h>"), py::str("#include <cell/snd3.h>"), py::str("#include <cell/pad.h>")},
        py::list<py::str>{},
        py::str(""),
        py::str(""),
        false,
        std::optional<py::str>(py::str("sys_time_get_system_time"))});
    t.set(py::str("wii"), StdafxConfig{
        py::list<py::str>{py::str("#include <gccore.h>"), py::str("#include <ogcsys.h>"), py::str("#include <wiiuse/wpad.h>")},
        py::list<py::str>{},
        py::list<py::str>{py::str("extern void* g_frameBuffer;"), py::str("extern GXRModeObj* g_rmode;")},
        py::str(""),
        py::str(""),
        false,
        std::nullopt});
    t.set(py::str("psp"), StdafxConfig{
        py::list<py::str>{py::str("#include <pspkernel.h>"), py::str("#include <pspdisplay.h>"), py::str("#include <pspgu.h>"), py::str("#include <pspge.h>"), py::str("#include <pspctrl.h>")},
        py::list<py::str>{},
        py::list<py::str>{},
        py::str(""),
        py::str(""),
        false,
        std::nullopt});
    t.set(py::str("nds"), StdafxConfig{
        py::list<py::str>{py::str("#include <nds.h>"), py::str("#include <fat.h>")},
        py::list<py::str>{},
        py::list<py::str>{},
        py::str(""),
        py::str(""),
        false,
        std::nullopt});
    return t;
}
const py::dict<py::str, StdafxConfig> _STDAFX = _STDAFX_table();

std::optional<Platform> get_platform(const py::str& platform_id) {
    // Return platform info dict or None.
    return PLATFORMS.get(platform_id);
}

py::str get_stub_comment(const py::str& platform_id, const py::str& subsystem_id) {
    // Return a comment string for a stub body, given a platform and subsystem.
    // e.g. '// TODO: Implement with D3D9 (d3d9.lib) -- IDirect3DDevice9, ...'
    std::optional<Platform> plat = PLATFORMS.get(platform_id);
    if (!plat) {
        return py::str("// TODO: Implement for target platform");
    }
    std::optional<Subsystem> subs = plat->subsystems.get(subsystem_id);
    if (!subs) {
        return py::str("// TODO: Implement for ") + plat->label;
    }
    return py::str("// TODO [") + plat->label + "]: Implement with " + subs->sdk + " (" + subs->libs + ") -- " + subs->apis;
}

py::str generate_stdafx(const py::str& platform_id, const py::str& cpp_std_label) {
    // Generate a platform-specific stdafx.h precompiled header.
    std::optional<Platform> plat = PLATFORMS.get(platform_id);
    py::str plat_label = plat ? plat->label : platform_id;
    std::optional<StdafxConfig> cfg = _STDAFX.get(platform_id);

    if (!cfg) {
        return py::str("// stdafx.h -- precompiled header for ") + plat_label + "\n#pragma once\n\n" + _STDAFX_HOST_WINDOWS +
               "\n" + _STDAFX_COMMON_CRT + "\n" + _STDAFX_COMMON_TYPEDEFS + "\n" + _STDAFX_HOST_MINMAX + "\n";
    }

    py::list<py::str> lines = {
        py::str("// stdafx.h -- precompiled header for ") + plat_label + " (" + cpp_std_label + ")",
        py::str("// Auto-generated by CodeBreaker. Add project-specific includes below."),
        py::str("#pragma once"),
        py::str(""),
    };

    if (py::len(cfg->warnings) != 0) {
        lines.append(cfg->warnings);
    }

    for (const py::str& h : cfg->system_headers) {
        lines.append(h);
    }
    lines.append(py::str(""));

    if (py::len(cfg->sdk_headers) != 0) {
        lines.append(py::str("// Platform SDK"));
        for (const py::str& h : cfg->sdk_headers) {
            lines.append(h);
        }
        lines.append(py::str(""));
    }

    if (py::len(cfg->extra) != 0) {
        lines.append(cfg->extra);
    }

    lines.append(py::str("// C/C++ standard library"));
    lines.append(_STDAFX_COMMON_CRT);

    lines.append(_STDAFX_COMMON_TYPEDEFS);

    if (cfg->host_minmax) {
        lines.append(_STDAFX_HOST_MINMAX);
    }

    if (py::len(cfg->globals) != 0) {
        lines.append(py::str("// Platform globals (defined in main.cpp)"));
        for (const py::str& g : cfg->globals) {
            lines.append(g);
        }
        lines.append(py::str(""));
    }

    return py::join(py::str("\n"), lines);
}

}  // namespace codebreaker::java::platforms
