# -*- coding: utf-8 -*-
"""
platforms.py -- target platform definitions for stub annotation.

Each platform maps the converter's subsystem IDs (from stubs.py) to the
platform-specific SDK, API, and library names that should fill that stub.
The converter emits these as comments inside the stub body so the developer
knows exactly what to reach for.
"""

from collections import OrderedDict

PLATFORMS = OrderedDict([
    ('xbox360', {
        'label': 'Xbox 360',
        'description': 'Microsoft Xbox 360 (XDK / Visual Studio 2010)',
        'sdk_dir': 'PlatformSDKs/WindowsSDK7.0',
        'big_endian': True,
        'null_page_protected': False,
        'null_deref_msg': 'hard crash (IP 0x0) on Xbox 360 (no NULL page protection)',
        'subsystems': {
            'rendering':   {'sdk': 'D3D9',      'libs': 'd3d9.lib, d3dx9.lib',          'apis': 'IDirect3DDevice9, IDirect3DTexture9, vertex shaders (HLSL), D3DXMatrixPerspectiveFovLH'},
            'audio':       {'sdk': 'XACT3',     'libs': 'xact3.lib, xact3wb.lib',       'apis': 'IXACT3Engine, IXACT3SoundBank, IXACT3WaveBank, IXACT3Cue'},
            'input':       {'sdk': 'XInput2',   'libs': 'xinput2.lib',                   'apis': 'XInputGetState, XINPUT_STATE, XINPUT_GAMEPAD'},
            'io':          {'sdk': 'XDK File',   'libs': 'xbdm.lib',                     'apis': 'CreateFile, ReadFile, WriteFile, XContentCreate'},
            'compression': {'sdk': 'LZX',       'libs': 'xcompress.lib',                 'apis': 'XMemCreateDecompressionContext, XMemDecompress'},
            'networking':  {'sdk': 'XNet',      'libs': 'xnetn.lib, xonline.lib',        'apis': 'XNetStartup, XNetCreateKey, WSAStartup, socket, connect'},
            'nio':         {'sdk': 'D3D9',      'libs': 'd3d9.lib',                      'apis': 'IDirect3DVertexBuffer9::Lock, direct memory (malloc/free)'},
            'threading':   {'sdk': 'Win32',     'libs': 'kernel32.lib',                  'apis': 'CreateThread, _beginthreadex, WaitForSingleObject'},
            'text':        {'sdk': 'CRT',       'libs': '(built-in)',                    'apis': 'strftime, sprintf, _snprintf'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(built-in)',                    'apis': 'typeid, dynamic_cast, manual factory registry'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(built-in)',                    'apis': 'std::exception, std::runtime_error'},
            'system':      {'sdk': 'XDK',       'libs': 'kernel32.lib',                  'apis': 'QueryPerformanceCounter, QueryPerformanceFrequency, GetTickCount'},
        },
    }),
    ('pc_windows', {
        'label': 'PC (Windows / DirectX)',
        'description': 'Windows PC with DirectX 9/11 (Visual Studio)',
        'sdk_dir': 'PlatformSDKs/WindowsSDK7.0',
        'big_endian': False,
        'null_page_protected': True,
        'null_deref_msg': 'access violation (segfault) on Windows -- OS catches it but the process still crashes',
        'subsystems': {
            'rendering':   {'sdk': 'D3D9/D3D11','libs': 'd3d9.lib or d3d11.lib, d3dx9.lib',  'apis': 'IDirect3DDevice9 or ID3D11Device, DXGI, HLSL shaders'},
            'audio':       {'sdk': 'XAudio2',   'libs': 'xaudio2.lib',                   'apis': 'IXAudio2, IXAudio2SourceVoice, X3DAudio'},
            'input':       {'sdk': 'XInput',    'libs': 'xinput.lib',                    'apis': 'XInputGetState, XINPUT_STATE, Raw Input (keyboard/mouse)'},
            'io':          {'sdk': 'Win32',     'libs': 'kernel32.lib',                  'apis': 'CreateFile, ReadFile, WriteFile, SHGetFolderPath'},
            'compression': {'sdk': 'zlib',      'libs': 'zlib.lib or miniz',             'apis': 'inflate, deflate, gzopen, gzread'},
            'networking':  {'sdk': 'Winsock2',  'libs': 'ws2_32.lib',                    'apis': 'WSAStartup, socket, connect, send, recv'},
            'nio':         {'sdk': 'D3D9/D3D11','libs': 'd3d9.lib',                      'apis': 'vertex buffers, malloc/free, VirtualAlloc'},
            'threading':   {'sdk': 'Win32',     'libs': 'kernel32.lib',                  'apis': 'CreateThread, _beginthreadex, InitializeCriticalSection'},
            'text':        {'sdk': 'CRT',       'libs': '(built-in)',                    'apis': 'strftime, sprintf_s, std::locale'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(built-in)',                    'apis': 'typeid, dynamic_cast, manual factory registry'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(built-in)',                    'apis': 'std::exception, std::runtime_error'},
            'system':      {'sdk': 'Win32',     'libs': 'kernel32.lib',                  'apis': 'QueryPerformanceCounter, GetTickCount64, timeGetTime'},
        },
    }),
    ('pc_opengl', {
        'label': 'PC (OpenGL / SDL)',
        'description': 'Cross-platform PC with OpenGL + SDL2',
        'big_endian': False,
        'null_page_protected': True,
        'null_deref_msg': 'segfault on PC -- OS catches it but the process still crashes',
        'subsystems': {
            'rendering':   {'sdk': 'OpenGL',    'libs': 'opengl32.lib, glew, SDL2',      'apis': 'glBegin, glEnd, glBindTexture, glDrawArrays, GLSL shaders'},
            'audio':       {'sdk': 'OpenAL',    'libs': 'openal32.lib or SDL2_mixer',    'apis': 'alGenSources, alSourcePlay, alListener3f'},
            'input':       {'sdk': 'SDL2',      'libs': 'SDL2.lib',                      'apis': 'SDL_PollEvent, SDL_GetKeyboardState, SDL_GetMouseState'},
            'io':          {'sdk': 'stdio',     'libs': '(built-in)',                    'apis': 'fopen, fread, fwrite, std::ifstream'},
            'compression': {'sdk': 'zlib',      'libs': 'zlib or miniz',                 'apis': 'inflate, deflate, gzopen'},
            'networking':  {'sdk': 'BSD sockets','libs': '(built-in on Unix, ws2_32 on Win)', 'apis': 'socket, connect, send, recv, select'},
            'nio':         {'sdk': 'OpenGL',    'libs': 'opengl32.lib',                  'apis': 'glGenBuffers, glBufferData (VBO), malloc/free'},
            'threading':   {'sdk': 'pthreads',  'libs': 'pthread or C++11 <thread>',     'apis': 'pthread_create, std::thread, std::mutex'},
            'text':        {'sdk': 'CRT',       'libs': '(built-in)',                    'apis': 'strftime, sprintf, std::locale'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(built-in)',                    'apis': 'typeid, dynamic_cast, manual factory registry'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(built-in)',                    'apis': 'std::exception, std::runtime_error'},
            'system':      {'sdk': 'SDL2',      'libs': 'SDL2.lib',                      'apis': 'SDL_GetTicks, SDL_GetPerformanceCounter'},
        },
    }),
    ('ps3', {
        'label': 'PlayStation 3',
        'description': 'Sony PlayStation 3 (SN Systems / GCC SNC)',
        'big_endian': True,
        'null_page_protected': False,
        'null_deref_msg': 'hard crash on PS3 (no NULL page protection on Cell)',
        'subsystems': {
            'rendering':   {'sdk': 'GCM/RSX',   'libs': 'libgcm_cmd.a, libgcm_sys.a',   'apis': 'cellGcmInit, cellGcmSetSurface, RSX vertex/fragment programs (Cg)'},
            'audio':       {'sdk': 'MultiStream','libs': 'libmstream.a, libsnd3.a',      'apis': 'cellMSSystemConfigureSoundIO, cellSnd3Init, cellSnd3NoteOn'},
            'input':       {'sdk': 'libpad',    'libs': 'libpad.a',                      'apis': 'cellPadInit, cellPadGetData, CellPadData'},
            'io':          {'sdk': 'Cell FS',   'libs': 'libfs.a',                       'apis': 'cellFsOpen, cellFsRead, cellFsWrite, cellFsStat'},
            'compression': {'sdk': 'zlib',      'libs': 'libz.a (Cell zlib)',            'apis': 'inflate, deflate (same as zlib)'},
            'networking':  {'sdk': 'libnet',    'libs': 'libnet.a, libnetctl.a',         'apis': 'cellNetCtlInit, sys_net_initialize_network, socket, connect'},
            'nio':         {'sdk': 'GCM/RSX',   'libs': 'libgcm_cmd.a',                  'apis': 'cellGcmMapMainMemory, RSX vertex arrays, malloc/free'},
            'threading':   {'sdk': 'SPU/PPU',   'libs': 'libsysutil.a, libspurs.a',     'apis': 'sys_ppu_thread_create, cellSpursInit, sys_lwmutex_create'},
            'text':        {'sdk': 'CRT',       'libs': '(built-in)',                    'apis': 'strftime, snprintf'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(built-in)',                    'apis': 'typeid, dynamic_cast, manual factory registry'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(built-in)',                    'apis': 'std::exception, std::runtime_error'},
            'system':      {'sdk': 'Cell OS',   'libs': 'libsysutil.a',                  'apis': 'sys_time_get_system_time, cellSysutilGetSystemParamInt'},
        },
    }),
    ('wii', {
        'label': 'Nintendo Wii',
        'description': 'Nintendo Wii (devkitPPC / libogc or RVL_SDK)',
        'big_endian': True,
        'null_page_protected': False,
        'null_deref_msg': 'hard crash on Wii (no NULL page protection on Broadway PPC)',
        'subsystems': {
            'rendering':   {'sdk': 'GX',        'libs': 'libogc (GX)',                   'apis': 'GX_Init, GX_Begin, GX_Position3f32, GX_LoadTexObj, GX_SetTevOp'},
            'audio':       {'sdk': 'ASND/DSP',  'libs': 'libasnd.a',                     'apis': 'ASND_Init, ASND_SetVoice, ASND_AddVoice'},
            'input':       {'sdk': 'WPAD/PAD',  'libs': 'libwiiuse.a, libogc',           'apis': 'WPAD_Init, WPAD_ScanPads, WPAD_ButtonsDown, PAD_ScanPads'},
            'io':          {'sdk': 'ISFS/FAT',  'libs': 'libfat.a, libogc',              'apis': 'ISFS_Open, ISFS_Read, fatInitDefault, fopen'},
            'compression': {'sdk': 'zlib',      'libs': 'libz.a (devkitPPC)',            'apis': 'inflate, deflate'},
            'networking':  {'sdk': 'libnet',    'libs': 'libnet.a (libogc)',              'apis': 'net_init, net_socket, net_connect, net_send, net_recv'},
            'nio':         {'sdk': 'GX',        'libs': 'libogc (GX)',                   'apis': 'GX display lists, MEM2 heap (SYS_GetArena2Lo), memalign'},
            'threading':   {'sdk': 'LWP',       'libs': 'libogc',                        'apis': 'LWP_CreateThread, LWP_MutexInit, LWP_SemInit'},
            'text':        {'sdk': 'CRT',       'libs': '(newlib built-in)',              'apis': 'strftime, snprintf'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(limited on Wii)',              'apis': 'manual factory registry (RTTI may be disabled)'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(newlib)',                      'apis': 'std::exception (if exceptions enabled; often disabled on Wii)'},
            'system':      {'sdk': 'libogc',    'libs': 'libogc',                        'apis': 'gettime, ticks_to_millisecs, SYS_GetArena1Size'},
        },
    }),
    ('psp', {
        'label': 'PlayStation Portable',
        'description': 'Sony PSP (PSPSDK / devkitPSP)',
        'big_endian': False,
        'null_page_protected': False,
        'null_deref_msg': 'hard crash on PSP (no NULL page protection on Allegrex MIPS)',
        'subsystems': {
            'rendering':   {'sdk': 'GU/GE',     'libs': 'libpspgu.a, libpspge.a',        'apis': 'sceGuInit, sceGuStart, sceGuDrawArray, sceGuTexImage'},
            'audio':       {'sdk': 'sceAudio',  'libs': 'libpspaudio.a',                 'apis': 'sceAudioChReserve, sceAudioOutputBlocking, sceAudioSRCOutputBlocking'},
            'input':       {'sdk': 'sceCtrl',   'libs': 'libpspctrl.a',                  'apis': 'sceCtrlSetSamplingCycle, sceCtrlReadBufferPositive, SceCtrlData'},
            'io':          {'sdk': 'sceIo',     'libs': 'libpspiofilemgr.a',             'apis': 'sceIoOpen, sceIoRead, sceIoWrite, sceIoLseek'},
            'compression': {'sdk': 'zlib',      'libs': 'libz.a',                        'apis': 'inflate, deflate'},
            'networking':  {'sdk': 'sceNet',    'libs': 'libpspnet.a, libpspwlan.a',     'apis': 'sceNetInit, sceNetAdhocInit, sceNetSocket'},
            'nio':         {'sdk': 'GU/GE',     'libs': 'libpspgu.a',                    'apis': 'sceGuGetMemory (VRAM alloc), memalign'},
            'threading':   {'sdk': 'sceKernel', 'libs': 'libpspkernel.a',                'apis': 'sceKernelCreateThread, sceKernelStartThread, sceKernelCreateSema'},
            'text':        {'sdk': 'CRT',       'libs': '(newlib built-in)',              'apis': 'strftime, snprintf'},
            'reflection':  {'sdk': 'RTTI',      'libs': '(limited)',                     'apis': 'manual factory registry'},
            'exceptions':  {'sdk': 'C++ STL',   'libs': '(newlib)',                      'apis': 'std::exception (if enabled)'},
            'system':      {'sdk': 'sceKernel', 'libs': 'libpsprtc.a',                   'apis': 'sceKernelGetSystemTimeLow, sceRtcGetCurrentTick'},
        },
    }),
    ('nds', {
        'label': 'Nintendo DS',
        'description': 'Nintendo DS (devkitARM / libnds)',
        'big_endian': False,
        'null_page_protected': False,
        'null_deref_msg': 'hard crash on NDS (no NULL page protection on ARM7/ARM9)',
        'subsystems': {
            'rendering':   {'sdk': 'libnds 3D', 'libs': 'libnds',                        'apis': 'glInit, glBegin, glVertex3f, glPolyFmt, glBindTexture (DS GL-like)'},
            'audio':       {'sdk': 'maxmod',    'libs': 'libmm.a',                       'apis': 'mmInit, mmLoadEffect, mmEffect, mmStart'},
            'input':       {'sdk': 'libnds',    'libs': 'libnds',                         'apis': 'scanKeys, keysDown, keysHeld, touchRead'},
            'io':          {'sdk': 'libfat',    'libs': 'libfat.a',                       'apis': 'fatInitDefault, fopen, fread, fwrite'},
            'compression': {'sdk': 'zlib',      'libs': 'libz.a (devkitARM)',             'apis': 'inflate, deflate'},
            'networking':  {'sdk': 'dswifi',    'libs': 'libdswifi9.a',                   'apis': 'Wifi_InitDefault, socket, connect (limited)'},
            'nio':         {'sdk': 'libnds',    'libs': 'libnds',                         'apis': 'glGetTexturePointer, malloc/free (limited RAM)'},
            'threading':   {'sdk': 'N/A',       'libs': 'N/A',                            'apis': 'single-threaded (cooperative; use IRQ handlers)'},
            'text':        {'sdk': 'CRT',       'libs': '(newlib)',                       'apis': 'snprintf'},
            'reflection':  {'sdk': 'N/A',       'libs': 'N/A',                            'apis': 'manual factory registry (no RTTI)'},
            'exceptions':  {'sdk': 'N/A',       'libs': 'N/A',                            'apis': 'exceptions typically disabled on NDS'},
            'system':      {'sdk': 'libnds',    'libs': 'libnds',                         'apis': 'timerStart, timerElapsed, cpuStartTiming'},
        },
    }),
])


def get_platform(platform_id):
    """Return platform info dict or None."""
    return PLATFORMS.get(platform_id)



def get_stub_comment(platform_id, subsystem_id):
    """Return a comment string for a stub body, given a platform and subsystem.
    e.g. '// TODO: Implement with D3D9 (d3d9.lib) — IDirect3DDevice9, ...'"""
    plat = PLATFORMS.get(platform_id)
    if not plat:
        return '// TODO: Implement for target platform'
    subs = plat['subsystems'].get(subsystem_id)
    if not subs:
        return '// TODO: Implement for %s' % plat['label']
    return '// TODO [%s]: Implement with %s (%s) -- %s' % (
        plat['label'], subs['sdk'], subs['libs'], subs['apis'])


# ---------------------------------------------------------------------------
# Platform-specific stdafx.h generation
# ---------------------------------------------------------------------------

_STDAFX_COMMON_CRT = """\
#include <cstdio>
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
"""

# No platform, built on a Windows host (MSVC, mingw): the runtime's file and thread helpers (File.h,
# Class.h) call the Win32 kernel API, as they do on the 360 through <xtl.h>. Lean, and without GDI --
# GDI's Rectangle() function would hide java.awt.Rectangle.
_STDAFX_HOST_WINDOWS = """\
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
#endif
"""

# Math.max/min are emitted as max/min (the XDK and MSVC's <windows.h> define them as macros). Without a
# platform on another compiler there are none: these give Java's overloads -- both sides widened to the
# wider of int, long long, float, double -- as functions, and only where no macro already exists.
_STDAFX_HOST_MINMAX = """\
#if !defined(max) && !defined(min)
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
"""

_STDAFX_MSVC_WARNINGS = """\
#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion float/double
#pragma warning(disable: 4305) // truncation double to float
#pragma warning(disable: 4355) // 'this' in base-member init list
"""

_STDAFX_COMMON_TYPEDEFS = """\
typedef unsigned char byte;
#define null NULL
"""

_STDAFX = {
    'xbox360': {
        'system_headers': ['#include <xtl.h>', '#include <xboxmath.h>'],
        'sdk_headers': ['#include <d3d9.h>', '#include <d3dx9.h>',
                        '#include <xaudio2.h>', '#include <xgraphics.h>'],
        'globals': [
            'extern IDirect3D9*       g_pD3D;',
            'extern IDirect3DDevice9* g_pd3dDevice;',
        ],
        'warnings': _STDAFX_MSVC_WARNINGS,
    },
    'pc_windows': {
        'system_headers': [
            '#define WIN32_LEAN_AND_MEAN',
            '#include <windows.h>',
            '#include <mmsystem.h>',
        ],
        'sdk_headers': ['#include <d3d9.h>', '#include <d3dx9.h>'],
        'globals': [
            'extern HWND              g_hWnd;',
            'extern HINSTANCE         g_hInstance;',
            'extern IDirect3D9*       g_pD3D;',
            'extern IDirect3DDevice9* g_pd3dDevice;',
        ],
        'warnings': _STDAFX_MSVC_WARNINGS,
        'extra': '#pragma comment(lib, "d3d9.lib")\n#pragma comment(lib, "d3dx9.lib")\n',
    },
    # No GL or SDL header here: the game's own GL11::GL_* constants would collide with GL's macros. The
    # platform layer (codebreaker/java/platform/pc) includes them in its own .cpp files only.
    'pc_opengl': {
        'system_headers': [_STDAFX_HOST_WINDOWS],
        'sdk_headers': [],
        'extra': ('// AWT components are as big as the SDL window (Display.cpp)\n'
                  'int j2xDisplayWidth();\nint j2xDisplayHeight();\n'
                  '#define J2X_COMPONENT_WIDTH() j2xDisplayWidth()\n'
                  '#define J2X_COMPONENT_HEIGHT() j2xDisplayHeight()\n'
                  '// the network: offline -- a connection that fails as Java\'s does with none (Network.cpp)\n'
                  'class URL; class URLConnection; class InputStream;\n'
                  'URLConnection* j2xUrlOpenConnection(URL* u);\nInputStream* j2xUrlOpenStream(URL* u);\n'
                  '#define J2X_URL_OPEN_CONNECTION(u) j2xUrlOpenConnection(u)\n'
                  '#define J2X_URL_OPEN_STREAM(u) j2xUrlOpenStream(u)\n'),
        'host_minmax': True,
        'warnings': (
            '#ifdef _MSC_VER\n'
            + _STDAFX_MSVC_WARNINGS
            + '#endif\n'
        ),
    },
    'ps3': {
        'system_headers': [
            '#include <cell/cell_fs.h>',
            '#include <sys/process.h>',
            '#include <sys/ppu_thread.h>',
            '#include <sys/timer.h>',
            '#include <sys/sys_time.h>',
        ],
        'sdk_headers': [
            '#include <cell/gcm.h>',
            '#include <cell/snd3.h>',
            '#include <cell/pad.h>',
        ],
        'globals': [],
        'warnings': '',
        'timer_impl': 'sys_time_get_system_time',
    },
    'wii': {
        'system_headers': [
            '#include <gccore.h>',
            '#include <ogcsys.h>',
            '#include <wiiuse/wpad.h>',
        ],
        'sdk_headers': [],
        'globals': [
            'extern void* g_frameBuffer;',
            'extern GXRModeObj* g_rmode;',
        ],
        'warnings': '',
    },
    'psp': {
        'system_headers': [
            '#include <pspkernel.h>',
            '#include <pspdisplay.h>',
            '#include <pspgu.h>',
            '#include <pspge.h>',
            '#include <pspctrl.h>',
        ],
        'sdk_headers': [],
        'globals': [],
        'warnings': '',
    },
    'nds': {
        'system_headers': [
            '#include <nds.h>',
            '#include <fat.h>',
        ],
        'sdk_headers': [],
        'globals': [],
        'warnings': '',
    },
}


def generate_stdafx(platform_id, cpp_std_label='C++03'):
    """Generate a platform-specific stdafx.h precompiled header."""
    plat = PLATFORMS.get(platform_id, {})
    plat_label = plat.get('label', platform_id)
    cfg = _STDAFX.get(platform_id)

    if cfg is None:
        return (
            '// stdafx.h -- precompiled header for %s\n'
            '#pragma once\n\n'
            '%s\n%s\n%s\n%s\n'
        ) % (plat_label, _STDAFX_HOST_WINDOWS, _STDAFX_COMMON_CRT, _STDAFX_COMMON_TYPEDEFS, _STDAFX_HOST_MINMAX)

    lines = [
        '// stdafx.h -- precompiled header for %s (%s)' % (plat_label, cpp_std_label),
        '// Auto-generated by j2ctool. Add project-specific includes below.',
        '#pragma once',
        '',
    ]

    if cfg.get('warnings'):
        lines.append(cfg['warnings'])

    for h in cfg['system_headers']:
        lines.append(h)
    lines.append('')

    if cfg['sdk_headers']:
        lines.append('// Platform SDK')
        for h in cfg['sdk_headers']:
            lines.append(h)
        lines.append('')

    if cfg.get('extra'):
        lines.append(cfg['extra'])

    lines.append('// C/C++ standard library')
    lines.append(_STDAFX_COMMON_CRT)

    lines.append(_STDAFX_COMMON_TYPEDEFS)

    if cfg.get('host_minmax'):
        lines.append(_STDAFX_HOST_MINMAX)

    if cfg.get('globals'):
        lines.append('// Platform globals (defined in main.cpp)')
        for g in cfg['globals']:
            lines.append(g)
        lines.append('')

    return '\n'.join(lines)
