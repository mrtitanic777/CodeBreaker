# -*- coding: utf-8 -*-
r"""
platform_equivalents.py -- the authoritative Java-platform-API -> Xbox 360 XDK map.

The point of this module: NEVER hand-guess a platform port again. For every Java standard-library /
LWJGL / platform type the converter has to stub, this table names the REAL Xbox 360 XDK equivalent --
the exact function(s) and the .lib they live in -- plus whether the port already implements it (via the
hand-written reference layer) or still needs work.

Two consumers wire off this table:
  * the STUB EMITTER (runtime.py / stub headers)  -> every emitted stub gets a comment
        `// [360] equivalent: <api>  (<lib>)`   or   `// [360] no equivalent -- <why>`
    so the answer is right there in the file, not in someone's head.
  * the COVERAGE REPORT (platform_coverage.py)    -> PLATFORM_COVERAGE.md lists the XDK equivalent and
    STATUS for every platform symbol the game uses, up front.

STATUS values:
  'real'   the port implements it for real (reference layer / runtime) -- ships working.
  'stub'   still a no-op/default, but the XDK equivalent below is the drop-in to implement it.
  'dead'   genuinely no console equivalent / dead code path (applet, minecraft.net) -- stub is correct.

Keyed by the emitted class/holder BASENAME (no .h). `methods` narrows the equivalent to specific calls
when the class spans more than one XDK API. Grounded in docs/platform/reference/*, the XDK import table
(xboxkrnl/xam/xnet/d3d9/xact3/xinput2), and the coverage audit.
"""

# subsystem -> ordered list of entries. Each entry: (basename, dict)
#   xdk    : the XDK function(s) that implement this (the thing to resolve to)
#   lib    : the .lib / import module the xdk symbol lives in
#   status : 'real' | 'stub' | 'dead'
#   note   : one line of why / how (formats/gotchas)
#   methods: optional {method_name: 'xdk detail'} for per-method equivalents

EQUIV = {

  # ============================ VIDEO -- OpenGL (LWJGL) -> Direct3D 9 ============================
  'video': {
    'GL11':   {'xdk': 'IDirect3DDevice9 (DrawPrimitiveUP / SetRenderState / SetTexture / SetTransform / '
                      'SetSamplerState)', 'lib': 'd3d9.lib', 'status': 'real',
               'note': 'GL11.reference.cpp is the GL->D3D9 emulator. HALF-PIXEL OFFSET: set D3DRS_HALFPIXELOFFSET.'},
    'GL12':   {'xdk': 'IDirect3DDevice9 render/sampler states', 'lib': 'd3d9.lib', 'status': 'stub',
               'note': 'GL_RESCALE_NORMAL -> D3DRS_NORMALIZENORMALS; a few constants route through GL11. Minor.'},
    'GL15':   {'xdk': 'IDirect3DVertexBuffer9 / IDirect3DIndexBuffer9', 'lib': 'd3d9.lib', 'status': 'stub',
               'note': 'Only referenced by the gated-off VBO path; inert until VBOs are enabled.'},
    'GLU':    {'xdk': 'D3DXMatrixPerspectiveFovRH / D3DXMatrixLookAtRH', 'lib': 'd3dx9.lib', 'status': 'real',
               'methods': {'gluPerspective': 'D3DXMatrixPerspectiveFovRH -> SetTransform(D3DTS_PROJECTION)',
                           'gluLookAt': 'D3DXMatrixLookAtRH', 'gluErrorString': 'no equiv -- returns "" (log only)'},
               'note': 'gluPerspective is REAL (fixes the blank-3D bug).'},
    'ARBVertexBufferObject': {'xdk': 'IDirect3DDevice9::CreateVertexBuffer + Lock/Unlock', 'lib': 'd3d9.lib',
               'status': 'stub', 'note': 'Gated OFF (GLContext caps false) -> falls back to DrawPrimitiveUP. '
               'LANDMINE: enabling VBOs without implementing this blanks all terrain.'},
    'ARBOcclusionQuery': {'xdk': 'IDirect3DQuery9(D3DQUERYTYPE_OCCLUSION) Issue/GetData', 'lib': 'd3d9.lib',
               'status': 'stub', 'note': 'Gated OFF -> culling disabled (draws all in-frustum chunks). Correct visuals.'},
    'NVFogDistance': {'xdk': 'D3DRS_FOGVERTEXMODE / D3DRS_FOGTABLEMODE', 'lib': 'd3d9.lib', 'status': 'stub',
               'note': 'Gated OFF -> GL11 linear planar fog (real) is used instead.'},
    'BufferUtils': {'xdk': 'plain heap buffer (malloc) -- NIO scratch, no XDK needed', 'lib': '(CRT)', 'status': 'stub',
               'note': 'createByteBuffer returns NULL -> F2 screenshot NULL-crashes. Trivial real fix: return a malloc buffer.'},
    'Display': {'xdk': 'IDirect3DDevice9::Present + XInput poll', 'lib': 'd3d9.lib / xinput2.lib', 'status': 'real',
               'note': 'Display.reference: update()=poll+Present, swapBuffers()=EndScene/Present/BeginScene.'},
    'DisplayMode': {'xdk': 'D3DPRESENT_PARAMETERS BackBufferWidth/Height + XGetVideoMode', 'lib': 'd3d9.lib',
               'status': 'stub', 'note': 'Boot uses ctor dims; the object is otherwise ignored by Display. Inert.'},
    'Project': {'xdk': 'D3DXVec3Project / D3DXVec3Unproject', 'lib': 'd3dx9.lib', 'status': 'stub',
               'note': 'gluProject/UnProject-equivalent; used by picking/particles if ever hit.'},
  },

  # ============================ INPUT -- controller / keyboard ============================
  'input': {
    'Keyboard': {'xdk': 'XamInputGetState(XINPUT_STATE) -> synthesized key events', 'lib': 'xinput2.lib',
               'status': 'real', 'note': 'Input.reference: the XInput2 controller adapter maps pad -> Keyboard/Mouse events.'},
    'Mouse':   {'xdk': 'XamInputGetState right-stick/triggers -> mouse delta+buttons', 'lib': 'xinput2.lib', 'status': 'real'},
    'Controllers': {'xdk': 'XamInputGetState / XamInputGetCapabilities', 'lib': 'xinput2.lib', 'status': 'real'},
    'OSK':     {'xdk': 'XamShowKeyboardUI (async, XOVERLAPPED) -> XGetOverlappedResult', 'lib': 'xam.lib',
               'status': 'real', 'note': 'OSK.reference: show()/active()/poll(); the on-screen keyboard for text fields + chat.'},
  },

  # ============================ AUDIO -- music / SFX ============================
  'audio': {
    'SoundManager': {'xdk': 'IXACT3Engine::CreateSoundBank/CreateInMemoryWaveBank + IXACT3SoundBank::Play (cue)',
               'lib': 'xact3.lib', 'status': 'real', 'note': 'Audio.reference bridges paulscode SoundManager -> XACT3 cues.'},
    'SoundSystem': {'xdk': 'IXACT3Engine (XACT3CreateEngine, DoWork per frame)', 'lib': 'xact3.lib', 'status': 'real'},
    'CodecJOrbis': {'xdk': 'XMA (XMACreateContext) if decoding at runtime; else ffmpeg->XMA at asset-build',
               'lib': 'xmcore.lib', 'status': 'dead', 'note': 'Bypassed: ogg is pre-decoded to the XACT wavebank; no runtime ogg decode.'},
    'CodecWav': {'xdk': '(XACT wavebank owns PCM)', 'lib': 'xact3.lib', 'status': 'dead'},
    'LibraryLWJGLOpenAL': {'xdk': '(XACT replaces OpenAL entirely)', 'lib': 'xact3.lib', 'status': 'dead'},
    'SoundSystemConfig': {'xdk': '(XACT engine params)', 'lib': 'xact3.lib', 'status': 'dead',
               'note': 'The real SoundSystem ignores this paulscode config object.'},
  },

  # ============================ FILE IO + SAVE ============================
  'fileio': {
    'File':   {'xdk': 'GetFileAttributes / CreateDirectory / MoveFile / DeleteFile', 'lib': 'xboxkrnl.exe', 'status': 'real',
               'note': 'File.h is Win32-backed (recursive mkdirs for RegionFile <x36>\\<z36> nesting).'},
    'FileInputStream':  {'xdk': 'CreateFile(GENERIC_READ) + ReadFile', 'lib': 'xboxkrnl.exe', 'status': 'real'},
    'FileOutputStream': {'xdk': 'CreateFile(CREATE_ALWAYS) + WriteFile', 'lib': 'xboxkrnl.exe', 'status': 'stub',
               'note': 'STILL A NO-OP -> nothing is ever written to disk. The single biggest save/load gap.'},
    'RandomAccessFile': {'xdk': 'CreateFile + SetFilePointer + ReadFile/WriteFile', 'lib': 'xboxkrnl.exe', 'status': 'real'},
    'FileReader': {'xdk': '(CreateFile + ReadFile, char decode)', 'lib': 'xboxkrnl.exe', 'status': 'stub',
               'note': 'NOT a Reader subclass -> options.txt never loads (falls to BufferedReader variadic ctor).'},
    'FileWriter': {'xdk': '(CreateFile + WriteFile)', 'lib': 'xboxkrnl.exe', 'status': 'stub', 'note': 'no-op -> options.txt never saved.'},
    # ---- SAVE persistence container (Xbox profile-bound storage) ----
    'SaveHandler': {'xdk': 'XContentCreate / XContentClose / XContentFlush (STFS package on the HDD)', 'lib': 'xam.lib',
               'status': 'stub', 'note': 'World saves should live in an STFS content package, mounted as a drive, flushed to persist.'},
    'DataOutputStream': {'xdk': '(pure big-endian byte codec -- no XDK)', 'lib': '(none)', 'status': 'real'},
    'DataInputStream':  {'xdk': '(pure big-endian byte codec -- no XDK)', 'lib': '(none)', 'status': 'real'},
  },

  # ============================ COMPRESSION (zlib/gzip) ============================
  'compression': {
    'Inflater': {'xdk': 'NO XDK zlib -- bundled puff.c (Inflate.cpp: zlibInflate/rawInflate)', 'lib': '(bundled)',
               'status': 'real', 'note': 'XMemDecompress (xcompress.lib) is LZX, NOT zlib -- WRONG format for Minecraft chunks. Use puff.'},
    'InflaterInputStream': {'xdk': 'puff.c zlibInflate over the wrapped stream', 'lib': '(bundled)', 'status': 'stub'},
    'GZIPInputStream':  {'xdk': 'puff.c gzipInflate (Inflate.cpp)', 'lib': '(bundled)', 'status': 'stub',
               'note': 'level.dat/player.dat/chunk NBT read path -- still a no-op, so world load reads empty NBT.'},
    'GZIPOutputStream': {'xdk': 'gzipDeflateStored (Inflate.cpp -- valid gzip via STORED blocks)', 'lib': '(bundled)',
               'status': 'stub', 'note': 'level.dat/player.dat write path -- no-op, so world never persists.'},
    'Deflater': {'xdk': 'no Huffman encoder bundled; gzipDeflateStored gives valid (uncompressed) output', 'lib': '(bundled)',
               'status': 'stub'},
  },

  # ============================ NETWORKING ============================
  'network': {
    'Socket': {'xdk': 'XNetStartup(BYPASS_SECURITY) + socket/connect/send/recv (Winsock)', 'lib': 'xnet.lib',
               'status': 'real', 'note': 'Socket.reference: real XNet TCP; setsockopt 0x5801/0x5802 to talk to a PC (insecure) server.'},
    'InetAddress': {'xdk': 'XNetDnsLookup / XNetGetTitleXnAddr', 'lib': 'xnet.lib', 'status': 'real'},
    'URL': {'xdk': 'XHttpOpen / XHttpConnect / XHttpSendRequest / XHttpReceiveResponse', 'lib': 'xhttp.lib', 'status': 'stub',
               'note': 'LIVE: NetClientHandler online-mode auth (joinserver) -> openStream NULL -> online-mode servers reject login.'},
    'HttpURLConnection': {'xdk': 'XHttp*', 'lib': 'xhttp.lib', 'status': 'dead', 'note': 'Only the minecraft.net asset/skin downloader -- dead offline.'},
  },

  # ============================ MEMORY ============================
  'memory': {
    'Runtime': {'xdk': 'GlobalMemoryStatus (MEMORYSTATUS) / MmQueryStatistics', 'lib': 'xboxkrnl.exe', 'status': 'real',
               'note': 'freeMemory/totalMemory/maxMemory -> physical pool. Replaces the diagnostic operator-new counters.'},
  },

  # ============================ THREADING / TIME ============================
  'concurrency': {
    'Thread': {'xdk': 'CreateThread / ExCreateThread, Sleep, WaitForSingleObject; XSetThreadProcessor for affinity',
               'lib': 'xboxkrnl.exe', 'status': 'real', 'note': 'Gated: the net reader/writer/connect threads get a real CreateThread; '
               'dead java daemons keep a no-op start() (real start() froze the menu).'},
  },

  # ============================ USER / PROFILE / ACHIEVEMENTS ============================
  'user': {
    'Session': {'xdk': 'XamUserGetName / XamUserGetSigninState (gamertag)', 'lib': 'xam.lib', 'status': 'real',
               'note': 'Session username seeded from the signed-in gamertag (XboxUser).'},
    'StatFileWriter': {'xdk': 'XUserWriteAchievements (achievement unlock) -- NOT the minecraft.net stats sync', 'lib': 'xam.lib',
               'status': 'stub', 'note': 'Local stats accumulate; the online sync is dead. Achievements go via the XLAST/SPA seam.'},
  },

  # ============================ IMAGE / TEXTURE DECODE ============================
  'image': {
    'BufferedImage': {'xdk': 'D3DXCreateTextureFromFileInMemoryEx / D3DXLoadSurfaceFromMemory', 'lib': 'd3dx9.lib',
               'status': 'real', 'note': 'Real ARGB int buffer; texture pixel ops + screenshots. getRaster()->empty (dead skin decode).'},
    'ImageIO': {'xdk': 'D3DXCreateTextureFromFileInMemoryEx (PNG decode on the GPU path)', 'lib': 'd3dx9.lib', 'status': 'real'},
  },
}



# NOTE: the OPPORTUNITIES and platform REQUIREMENTS are no longer hardcoded here -- they are AUTO-DETECTED
# from the INSTALLED SDK by xdk_scan.py (each is claimed only because its XDK function is actually present in
# the scanned headers). See j2x/xdk_scan.py PATTERNS + platform_coverage.render_report. EQUIV below stays: it
# is the Java-symbol -> XDK-family INTENT map that drives the // [360] stub annotations.



def lookup(basename):
    """Return the equivalent dict for an emitted class basename, or None."""
    for sub in EQUIV.values():
        if basename in sub:
            return sub[basename]
    return None


def kind(e):
    """Classify an equivalent so the emitted comment (and the report) can distinguish the three cases:
       'xdk'         -> there IS an XDK function to call; resolve the stub to it.
       'handwritten' -> NO XDK equivalent exists -> we implement it ourselves (bundled puff.c, a pure C++
                        byte codec, a CRT call). These are the bespoke pieces that must never be lost.
       'dead'        -> no console equivalent and a dead code path; the stub is correct as-is."""
    if e.get('status') == 'dead':
        return 'dead'
    if e.get('lib') in ('(none)', '(bundled)', '(CRT)'):
        return 'handwritten'
    return 'xdk'


def annotation(basename):
    """The one-line C++ comment dropped next to a stub -- THREE distinct, greppable markers so nothing is
    ever missed:
        // [360] equivalent: <XDK api>  (<lib>)               -> resolve the stub to the named XDK call
        // [360] NO XDK EQUIVALENT -- hand-implemented: ...    -> bespoke platform code (grep to find them ALL)
        // [360] no equivalent -- dead on console: ...         -> genuinely dead path; stub is correct
    Returns '' if we have no mapping (emitter leaves the stub untouched)."""
    e = lookup(basename)
    if not e:
        return ''
    k = kind(e)
    if k == 'dead':
        return '// [360] no equivalent -- dead on console: %s' % e.get('note', '')
    if k == 'handwritten':
        return '// [360] NO XDK EQUIVALENT -- hand-implemented: %s' % e['xdk'].strip('()')
    lib = e.get('lib', '')
    lib = ('  (%s)' % lib) if lib else ''
    return '// [360] equivalent: %s%s' % (e['xdk'], lib)


def all_entries():
    """Flatten to (subsystem, basename, entry) for the coverage report."""
    for sub, items in EQUIV.items():
        for name, e in items.items():
            yield sub, name, e
