#include "cb/java/platforms.hpp"

#include <algorithm>
#include <format>
#include <vector>

namespace cb::java::platforms {

namespace {
constexpr std::string_view kCommonCrt = R"SDX(#include <cstdio>
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
)SDX";

constexpr std::string_view kHostWindows = R"SDX(#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
#endif
)SDX";

constexpr std::string_view kHostMinmax = R"SDX(#if !defined(max) && !defined(min)
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
)SDX";

constexpr std::string_view kMsvcWarnings = R"SDX(#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion float/double
#pragma warning(disable: 4305) // truncation double to float
#pragma warning(disable: 4355) // 'this' in base-member init list
)SDX";

constexpr std::string_view kCommonTypedefs = R"SDX(typedef unsigned char byte;
#define null NULL
)SDX";

// ---- generated from the Python tables, then reviewed ----
const std::vector<Platform>& table() {
    static const std::vector<Platform> kTable = {
        {PlatformId::Xbox360, "xbox360", "Xbox 360",
         "Microsoft Xbox 360 (XDK / Visual Studio 2010)",
         "PlatformSDKs/WindowsSDK7.0",
         true, false,
         "hard crash (IP 0x0) on Xbox 360 (no NULL page protection)",
         {{
             {"D3D9", "d3d9.lib, d3dx9.lib", "IDirect3DDevice9, IDirect3DTexture9, vertex shaders (HLSL), D3DXMatrixPerspectiveFovLH"},   // rendering
             {"XACT3", "xact3.lib, xact3wb.lib", "IXACT3Engine, IXACT3SoundBank, IXACT3WaveBank, IXACT3Cue"},   // audio
             {"XInput2", "xinput2.lib", "XInputGetState, XINPUT_STATE, XINPUT_GAMEPAD"},   // input
             {"XDK File", "xbdm.lib", "CreateFile, ReadFile, WriteFile, XContentCreate"},   // io
             {"LZX", "xcompress.lib", "XMemCreateDecompressionContext, XMemDecompress"},   // compression
             {"XNet", "xnetn.lib, xonline.lib", "XNetStartup, XNetCreateKey, WSAStartup, socket, connect"},   // networking
             {"D3D9", "d3d9.lib", "IDirect3DVertexBuffer9::Lock, direct memory (malloc/free)"},   // nio
             {"Win32", "kernel32.lib", "CreateThread, _beginthreadex, WaitForSingleObject"},   // threading
             {"CRT", "(built-in)", "strftime, sprintf, _snprintf"},   // text
             {"RTTI", "(built-in)", "typeid, dynamic_cast, manual factory registry"},   // reflection
             {"C++ STL", "(built-in)", "std::exception, std::runtime_error"},   // exceptions
             {"XDK", "kernel32.lib", "QueryPerformanceCounter, QueryPerformanceFrequency, GetTickCount"},   // system
         }}},
        {PlatformId::PcWindows, "pc_windows", "PC (Windows / DirectX)",
         "Windows PC with DirectX 9/11 (Visual Studio)",
         "PlatformSDKs/WindowsSDK7.0",
         false, true,
         "access violation (segfault) on Windows -- OS catches it but the process still crashes",
         {{
             {"D3D9/D3D11", "d3d9.lib or d3d11.lib, d3dx9.lib", "IDirect3DDevice9 or ID3D11Device, DXGI, HLSL shaders"},   // rendering
             {"XAudio2", "xaudio2.lib", "IXAudio2, IXAudio2SourceVoice, X3DAudio"},   // audio
             {"XInput", "xinput.lib", "XInputGetState, XINPUT_STATE, Raw Input (keyboard/mouse)"},   // input
             {"Win32", "kernel32.lib", "CreateFile, ReadFile, WriteFile, SHGetFolderPath"},   // io
             {"zlib", "zlib.lib or miniz", "inflate, deflate, gzopen, gzread"},   // compression
             {"Winsock2", "ws2_32.lib", "WSAStartup, socket, connect, send, recv"},   // networking
             {"D3D9/D3D11", "d3d9.lib", "vertex buffers, malloc/free, VirtualAlloc"},   // nio
             {"Win32", "kernel32.lib", "CreateThread, _beginthreadex, InitializeCriticalSection"},   // threading
             {"CRT", "(built-in)", "strftime, sprintf_s, std::locale"},   // text
             {"RTTI", "(built-in)", "typeid, dynamic_cast, manual factory registry"},   // reflection
             {"C++ STL", "(built-in)", "std::exception, std::runtime_error"},   // exceptions
             {"Win32", "kernel32.lib", "QueryPerformanceCounter, GetTickCount64, timeGetTime"},   // system
         }}},
        {PlatformId::PcOpengl, "pc_opengl", "PC (OpenGL / SDL)",
         "Cross-platform PC with OpenGL + SDL2",
         {},
         false, true,
         "segfault on PC -- OS catches it but the process still crashes",
         {{
             {"OpenGL", "opengl32.lib, glew, SDL2", "glBegin, glEnd, glBindTexture, glDrawArrays, GLSL shaders"},   // rendering
             {"OpenAL", "openal32.lib or SDL2_mixer", "alGenSources, alSourcePlay, alListener3f"},   // audio
             {"SDL2", "SDL2.lib", "SDL_PollEvent, SDL_GetKeyboardState, SDL_GetMouseState"},   // input
             {"stdio", "(built-in)", "fopen, fread, fwrite, std::ifstream"},   // io
             {"zlib", "zlib or miniz", "inflate, deflate, gzopen"},   // compression
             {"BSD sockets", "(built-in on Unix, ws2_32 on Win)", "socket, connect, send, recv, select"},   // networking
             {"OpenGL", "opengl32.lib", "glGenBuffers, glBufferData (VBO), malloc/free"},   // nio
             {"pthreads", "pthread or C++11 <thread>", "pthread_create, std::thread, std::mutex"},   // threading
             {"CRT", "(built-in)", "strftime, sprintf, std::locale"},   // text
             {"RTTI", "(built-in)", "typeid, dynamic_cast, manual factory registry"},   // reflection
             {"C++ STL", "(built-in)", "std::exception, std::runtime_error"},   // exceptions
             {"SDL2", "SDL2.lib", "SDL_GetTicks, SDL_GetPerformanceCounter"},   // system
         }}},
        {PlatformId::Ps3, "ps3", "PlayStation 3",
         "Sony PlayStation 3 (SN Systems / GCC SNC)",
         {},
         true, false,
         "hard crash on PS3 (no NULL page protection on Cell)",
         {{
             {"GCM/RSX", "libgcm_cmd.a, libgcm_sys.a", "cellGcmInit, cellGcmSetSurface, RSX vertex/fragment programs (Cg)"},   // rendering
             {"MultiStream", "libmstream.a, libsnd3.a", "cellMSSystemConfigureSoundIO, cellSnd3Init, cellSnd3NoteOn"},   // audio
             {"libpad", "libpad.a", "cellPadInit, cellPadGetData, CellPadData"},   // input
             {"Cell FS", "libfs.a", "cellFsOpen, cellFsRead, cellFsWrite, cellFsStat"},   // io
             {"zlib", "libz.a (Cell zlib)", "inflate, deflate (same as zlib)"},   // compression
             {"libnet", "libnet.a, libnetctl.a", "cellNetCtlInit, sys_net_initialize_network, socket, connect"},   // networking
             {"GCM/RSX", "libgcm_cmd.a", "cellGcmMapMainMemory, RSX vertex arrays, malloc/free"},   // nio
             {"SPU/PPU", "libsysutil.a, libspurs.a", "sys_ppu_thread_create, cellSpursInit, sys_lwmutex_create"},   // threading
             {"CRT", "(built-in)", "strftime, snprintf"},   // text
             {"RTTI", "(built-in)", "typeid, dynamic_cast, manual factory registry"},   // reflection
             {"C++ STL", "(built-in)", "std::exception, std::runtime_error"},   // exceptions
             {"Cell OS", "libsysutil.a", "sys_time_get_system_time, cellSysutilGetSystemParamInt"},   // system
         }}},
        {PlatformId::Wii, "wii", "Nintendo Wii",
         "Nintendo Wii (devkitPPC / libogc or RVL_SDK)",
         {},
         true, false,
         "hard crash on Wii (no NULL page protection on Broadway PPC)",
         {{
             {"GX", "libogc (GX)", "GX_Init, GX_Begin, GX_Position3f32, GX_LoadTexObj, GX_SetTevOp"},   // rendering
             {"ASND/DSP", "libasnd.a", "ASND_Init, ASND_SetVoice, ASND_AddVoice"},   // audio
             {"WPAD/PAD", "libwiiuse.a, libogc", "WPAD_Init, WPAD_ScanPads, WPAD_ButtonsDown, PAD_ScanPads"},   // input
             {"ISFS/FAT", "libfat.a, libogc", "ISFS_Open, ISFS_Read, fatInitDefault, fopen"},   // io
             {"zlib", "libz.a (devkitPPC)", "inflate, deflate"},   // compression
             {"libnet", "libnet.a (libogc)", "net_init, net_socket, net_connect, net_send, net_recv"},   // networking
             {"GX", "libogc (GX)", "GX display lists, MEM2 heap (SYS_GetArena2Lo), memalign"},   // nio
             {"LWP", "libogc", "LWP_CreateThread, LWP_MutexInit, LWP_SemInit"},   // threading
             {"CRT", "(newlib built-in)", "strftime, snprintf"},   // text
             {"RTTI", "(limited on Wii)", "manual factory registry (RTTI may be disabled)"},   // reflection
             {"C++ STL", "(newlib)", "std::exception (if exceptions enabled; often disabled on Wii)"},   // exceptions
             {"libogc", "libogc", "gettime, ticks_to_millisecs, SYS_GetArena1Size"},   // system
         }}},
        {PlatformId::Psp, "psp", "PlayStation Portable",
         "Sony PSP (PSPSDK / devkitPSP)",
         {},
         false, false,
         "hard crash on PSP (no NULL page protection on Allegrex MIPS)",
         {{
             {"GU/GE", "libpspgu.a, libpspge.a", "sceGuInit, sceGuStart, sceGuDrawArray, sceGuTexImage"},   // rendering
             {"sceAudio", "libpspaudio.a", "sceAudioChReserve, sceAudioOutputBlocking, sceAudioSRCOutputBlocking"},   // audio
             {"sceCtrl", "libpspctrl.a", "sceCtrlSetSamplingCycle, sceCtrlReadBufferPositive, SceCtrlData"},   // input
             {"sceIo", "libpspiofilemgr.a", "sceIoOpen, sceIoRead, sceIoWrite, sceIoLseek"},   // io
             {"zlib", "libz.a", "inflate, deflate"},   // compression
             {"sceNet", "libpspnet.a, libpspwlan.a", "sceNetInit, sceNetAdhocInit, sceNetSocket"},   // networking
             {"GU/GE", "libpspgu.a", "sceGuGetMemory (VRAM alloc), memalign"},   // nio
             {"sceKernel", "libpspkernel.a", "sceKernelCreateThread, sceKernelStartThread, sceKernelCreateSema"},   // threading
             {"CRT", "(newlib built-in)", "strftime, snprintf"},   // text
             {"RTTI", "(limited)", "manual factory registry"},   // reflection
             {"C++ STL", "(newlib)", "std::exception (if enabled)"},   // exceptions
             {"sceKernel", "libpsprtc.a", "sceKernelGetSystemTimeLow, sceRtcGetCurrentTick"},   // system
         }}},
        {PlatformId::Nds, "nds", "Nintendo DS",
         "Nintendo DS (devkitARM / libnds)",
         {},
         false, false,
         "hard crash on NDS (no NULL page protection on ARM7/ARM9)",
         {{
             {"libnds 3D", "libnds", "glInit, glBegin, glVertex3f, glPolyFmt, glBindTexture (DS GL-like)"},   // rendering
             {"maxmod", "libmm.a", "mmInit, mmLoadEffect, mmEffect, mmStart"},   // audio
             {"libnds", "libnds", "scanKeys, keysDown, keysHeld, touchRead"},   // input
             {"libfat", "libfat.a", "fatInitDefault, fopen, fread, fwrite"},   // io
             {"zlib", "libz.a (devkitARM)", "inflate, deflate"},   // compression
             {"dswifi", "libdswifi9.a", "Wifi_InitDefault, socket, connect (limited)"},   // networking
             {"libnds", "libnds", "glGetTexturePointer, malloc/free (limited RAM)"},   // nio
             {"N/A", "N/A", "single-threaded (cooperative; use IRQ handlers)"},   // threading
             {"CRT", "(newlib)", "snprintf"},   // text
             {"N/A", "N/A", "manual factory registry (no RTTI)"},   // reflection
             {"N/A", "N/A", "exceptions typically disabled on NDS"},   // exceptions
             {"libnds", "libnds", "timerStart, timerElapsed, cpuStartTiming"},   // system
         }}},
    };
    return kTable;
}

// What one platform's stdafx.h is made of.
struct StdafxConfig {
    std::vector<std::string> system_headers;
    std::vector<std::string> sdk_headers;
    std::vector<std::string> globals;   // declarations of the platform globals (defined in main.cpp)
    std::string warnings;
    std::string extra;
    bool host_minmax = false;
};

StdafxConfig stdafx_config(PlatformId id) {
    switch (id) {
        case PlatformId::Xbox360:
            return {{"#include <xtl.h>", "#include <xboxmath.h>"},
                    {"#include <d3d9.h>", "#include <d3dx9.h>", "#include <xaudio2.h>", "#include <xgraphics.h>"},
                    {"extern IDirect3D9*       g_pD3D;", "extern IDirect3DDevice9* g_pd3dDevice;"},
                    std::string(kMsvcWarnings),
                    {},
                    false};
        case PlatformId::PcWindows:
            return {{"#define WIN32_LEAN_AND_MEAN", "#include <windows.h>", "#include <mmsystem.h>"},
                    {"#include <d3d9.h>", "#include <d3dx9.h>"},
                    {"extern HWND              g_hWnd;", "extern HINSTANCE         g_hInstance;",
                     "extern IDirect3D9*       g_pD3D;", "extern IDirect3DDevice9* g_pd3dDevice;"},
                    std::string(kMsvcWarnings),
                    "#pragma comment(lib, \"d3d9.lib\")\n#pragma comment(lib, \"d3dx9.lib\")\n",
                    false};
        case PlatformId::PcOpengl:
            // No GL or SDL header here: the game's own GL11::GL_* constants would collide with GL's macros. The
            // platform layer (codebreaker/java/platform/pc) includes them in its own .cpp files only.
            return {{std::string(kHostWindows)},
                    {},
                    {},
                    "#ifdef _MSC_VER\n" + std::string(kMsvcWarnings) + "#endif\n",
                    "// AWT components are as big as the SDL window (Display.cpp)\n"
                    "int j2xDisplayWidth();\nint j2xDisplayHeight();\n"
                    "#define J2X_COMPONENT_WIDTH() j2xDisplayWidth()\n"
                    "#define J2X_COMPONENT_HEIGHT() j2xDisplayHeight()\n"
                    "// the network: offline -- a connection that fails as Java's does with none (Network.cpp)\n"
                    "class URL; class URLConnection; class InputStream;\n"
                    "URLConnection* j2xUrlOpenConnection(URL* u);\nInputStream* j2xUrlOpenStream(URL* u);\n"
                    "#define J2X_URL_OPEN_CONNECTION(u) j2xUrlOpenConnection(u)\n"
                    "#define J2X_URL_OPEN_STREAM(u) j2xUrlOpenStream(u)\n",
                    true};
        case PlatformId::Ps3:
            return {{"#include <cell/cell_fs.h>", "#include <sys/process.h>", "#include <sys/ppu_thread.h>",
                     "#include <sys/timer.h>", "#include <sys/sys_time.h>"},
                    {"#include <cell/gcm.h>", "#include <cell/snd3.h>", "#include <cell/pad.h>"},
                    {}, {}, {}, false};
        case PlatformId::Wii:
            return {{"#include <gccore.h>", "#include <ogcsys.h>", "#include <wiiuse/wpad.h>"},
                    {},
                    {"extern void* g_frameBuffer;", "extern GXRModeObj* g_rmode;"},
                    {}, {}, false};
        case PlatformId::Psp:
            return {{"#include <pspkernel.h>", "#include <pspdisplay.h>", "#include <pspgu.h>", "#include <pspge.h>",
                     "#include <pspctrl.h>"},
                    {}, {}, {}, {}, false};
        case PlatformId::Nds:
            return {{"#include <nds.h>", "#include <fat.h>"}, {}, {}, {}, {}, false};
    }
    return {};
}

}  // namespace

std::span<const Platform> platforms() { return table(); }

const Platform* find_platform(std::string_view key) {
    for (const Platform& p : table()) {
        if (p.key == key) return &p;
    }
    return nullptr;
}

std::string stub_comment(std::string_view platform_key, std::string_view subsystem_key) {
    const Platform* plat = find_platform(platform_key);
    if (plat == nullptr) return "// TODO: Implement for target platform";
    const auto subsystem = stubs::subsystem_from_key(subsystem_key);
    if (!subsystem) return std::format("// TODO: Implement for {}", plat->label);
    const SubsystemApi& api = plat->api(*subsystem);
    return std::format("// TODO [{}]: Implement with {} ({}) -- {}", plat->label, api.sdk, api.libs, api.apis);
}

std::string generate_stdafx(std::string_view platform_key, std::string_view cpp_std_label) {
    const Platform* plat = find_platform(platform_key);
    const std::string_view label = plat != nullptr ? plat->label : platform_key;

    if (plat == nullptr) {
        return std::format("// stdafx.h -- precompiled header for {}\n#pragma once\n\n{}\n{}\n{}\n{}\n", label,
                           kHostWindows, kCommonCrt, kCommonTypedefs, kHostMinmax);
    }
    const StdafxConfig cfg = stdafx_config(plat->id);

    std::vector<std::string> lines = {
        std::format("// stdafx.h -- precompiled header for {} ({})", label, cpp_std_label),
        "// Auto-generated by CodeBreaker. Add project-specific includes below.",
        "#pragma once",
        "",
    };
    if (!cfg.warnings.empty()) lines.push_back(cfg.warnings);
    for (const std::string& h : cfg.system_headers) lines.push_back(h);
    lines.push_back("");
    if (!cfg.sdk_headers.empty()) {
        lines.push_back("// Platform SDK");
        for (const std::string& h : cfg.sdk_headers) lines.push_back(h);
        lines.push_back("");
    }
    if (!cfg.extra.empty()) lines.push_back(cfg.extra);
    lines.push_back("// C/C++ standard library");
    lines.push_back(std::string(kCommonCrt));
    lines.push_back(std::string(kCommonTypedefs));
    if (cfg.host_minmax) lines.push_back(std::string(kHostMinmax));
    if (!cfg.globals.empty()) {
        lines.push_back("// Platform globals (defined in main.cpp)");
        for (const std::string& g : cfg.globals) lines.push_back(g);
        lines.push_back("");
    }
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) out += '\n';
        out += lines[i];
    }
    return out;
}

}  // namespace cb::java::platforms
