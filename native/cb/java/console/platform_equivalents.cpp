#include "cb/java/console/platform_equivalents.hpp"

#include <algorithm>
#include <array>

#include "cb/core/text.hpp"

namespace cb::java::console::platform_equivalents {

namespace {

// Subsystems, in the order the coverage report lists them: VIDEO (OpenGL/LWJGL -> Direct3D 9), INPUT (controller
// / keyboard), AUDIO (music / SFX), FILE IO + SAVE, COMPRESSION (zlib/gzip), NETWORKING, MEMORY, THREADING / TIME,
// USER / PROFILE / ACHIEVEMENTS, IMAGE / TEXTURE DECODE.
//
// NOTE: the OPPORTUNITIES and platform REQUIREMENTS are not hardcoded here -- they are auto-detected from the
// INSTALLED SDK by xdk_scan (each is claimed only because its XDK function is actually present in the scanned
// headers). This table is the Java-symbol -> XDK-family INTENT map that drives the // [360] stub annotations.

constexpr MethodEquiv kGluMethods[] = {
    {"gluPerspective", "D3DXMatrixPerspectiveFovRH -> SetTransform(D3DTS_PROJECTION)"},
    {"gluLookAt", "D3DXMatrixLookAtRH"},
    {"gluErrorString", "no equiv -- returns \"\" (log only)"},
};

constexpr Equiv kVideo[] = {
    {"GL11", "IDirect3DDevice9 (DrawPrimitiveUP / SetRenderState / SetTexture / SetTransform / SetSamplerState)", "d3d9.lib", Status::Real, "GL11.reference.cpp is the GL->D3D9 emulator. HALF-PIXEL OFFSET: set D3DRS_HALFPIXELOFFSET.", {}},
    {"GL12", "IDirect3DDevice9 render/sampler states", "d3d9.lib", Status::Stub, "GL_RESCALE_NORMAL -> D3DRS_NORMALIZENORMALS; a few constants route through GL11. Minor.", {}},
    {"GL15", "IDirect3DVertexBuffer9 / IDirect3DIndexBuffer9", "d3d9.lib", Status::Stub, "Only referenced by the gated-off VBO path; inert until VBOs are enabled.", {}},
    {"GLU", "D3DXMatrixPerspectiveFovRH / D3DXMatrixLookAtRH", "d3dx9.lib", Status::Real, "gluPerspective is REAL (fixes the blank-3D bug).", kGluMethods},
    {"ARBVertexBufferObject", "IDirect3DDevice9::CreateVertexBuffer + Lock/Unlock", "d3d9.lib", Status::Stub, "Gated OFF (GLContext caps false) -> falls back to DrawPrimitiveUP. LANDMINE: enabling VBOs without implementing this blanks all terrain.", {}},
    {"ARBOcclusionQuery", "IDirect3DQuery9(D3DQUERYTYPE_OCCLUSION) Issue/GetData", "d3d9.lib", Status::Stub, "Gated OFF -> culling disabled (draws all in-frustum chunks). Correct visuals.", {}},
    {"NVFogDistance", "D3DRS_FOGVERTEXMODE / D3DRS_FOGTABLEMODE", "d3d9.lib", Status::Stub, "Gated OFF -> GL11 linear planar fog (real) is used instead.", {}},
    {"BufferUtils", "plain heap buffer (malloc) -- NIO scratch, no XDK needed", "(CRT)", Status::Stub, "createByteBuffer returns NULL -> F2 screenshot NULL-crashes. Trivial real fix: return a malloc buffer.", {}},
    {"Display", "IDirect3DDevice9::Present + XInput poll", "d3d9.lib / xinput2.lib", Status::Real, "Display.reference: update()=poll+Present, swapBuffers()=EndScene/Present/BeginScene.", {}},
    {"DisplayMode", "D3DPRESENT_PARAMETERS BackBufferWidth/Height + XGetVideoMode", "d3d9.lib", Status::Stub, "Boot uses ctor dims; the object is otherwise ignored by Display. Inert.", {}},
    {"Project", "D3DXVec3Project / D3DXVec3Unproject", "d3dx9.lib", Status::Stub, "gluProject/UnProject-equivalent; used by picking/particles if ever hit.", {}},
};
constexpr Equiv kInput[] = {
    {"Keyboard", "XamInputGetState(XINPUT_STATE) -> synthesized key events", "xinput2.lib", Status::Real, "Input.reference: the XInput2 controller adapter maps pad -> Keyboard/Mouse events.", {}},
    {"Mouse", "XamInputGetState right-stick/triggers -> mouse delta+buttons", "xinput2.lib", Status::Real, "", {}},
    {"Controllers", "XamInputGetState / XamInputGetCapabilities", "xinput2.lib", Status::Real, "", {}},
    {"OSK", "XamShowKeyboardUI (async, XOVERLAPPED) -> XGetOverlappedResult", "xam.lib", Status::Real, "OSK.reference: show()/active()/poll(); the on-screen keyboard for text fields + chat.", {}},
};
constexpr Equiv kAudio[] = {
    {"SoundManager", "IXACT3Engine::CreateSoundBank/CreateInMemoryWaveBank + IXACT3SoundBank::Play (cue)", "xact3.lib", Status::Real, "Audio.reference bridges paulscode SoundManager -> XACT3 cues.", {}},
    {"SoundSystem", "IXACT3Engine (XACT3CreateEngine, DoWork per frame)", "xact3.lib", Status::Real, "", {}},
    {"CodecJOrbis", "XMA (XMACreateContext) if decoding at runtime; else ffmpeg->XMA at asset-build", "xmcore.lib", Status::Dead, "Bypassed: ogg is pre-decoded to the XACT wavebank; no runtime ogg decode.", {}},
    {"CodecWav", "(XACT wavebank owns PCM)", "xact3.lib", Status::Dead, "", {}},
    {"LibraryLWJGLOpenAL", "(XACT replaces OpenAL entirely)", "xact3.lib", Status::Dead, "", {}},
    {"SoundSystemConfig", "(XACT engine params)", "xact3.lib", Status::Dead, "The real SoundSystem ignores this paulscode config object.", {}},
};
constexpr Equiv kFileio[] = {
    {"File", "GetFileAttributes / CreateDirectory / MoveFile / DeleteFile", "xboxkrnl.exe", Status::Real, "File.h is Win32-backed (recursive mkdirs for RegionFile <x36>\\<z36> nesting).", {}},
    {"FileInputStream", "CreateFile(GENERIC_READ) + ReadFile", "xboxkrnl.exe", Status::Real, "", {}},
    {"FileOutputStream", "CreateFile(CREATE_ALWAYS) + WriteFile", "xboxkrnl.exe", Status::Stub, "STILL A NO-OP -> nothing is ever written to disk. The single biggest save/load gap.", {}},
    {"RandomAccessFile", "CreateFile + SetFilePointer + ReadFile/WriteFile", "xboxkrnl.exe", Status::Real, "", {}},
    {"FileReader", "(CreateFile + ReadFile, char decode)", "xboxkrnl.exe", Status::Stub, "NOT a Reader subclass -> options.txt never loads (falls to BufferedReader variadic ctor).", {}},
    {"FileWriter", "(CreateFile + WriteFile)", "xboxkrnl.exe", Status::Stub, "no-op -> options.txt never saved.", {}},
    {"SaveHandler", "XContentCreate / XContentClose / XContentFlush (STFS package on the HDD)", "xam.lib", Status::Stub, "World saves should live in an STFS content package, mounted as a drive, flushed to persist.", {}},
    {"DataOutputStream", "(pure big-endian byte codec -- no XDK)", "(none)", Status::Real, "", {}},
    {"DataInputStream", "(pure big-endian byte codec -- no XDK)", "(none)", Status::Real, "", {}},
};
constexpr Equiv kCompression[] = {
    {"Inflater", "NO XDK zlib -- bundled puff.c (Inflate.cpp: zlibInflate/rawInflate)", "(bundled)", Status::Real, "XMemDecompress (xcompress.lib) is LZX, NOT zlib -- WRONG format for Minecraft chunks. Use puff.", {}},
    {"InflaterInputStream", "puff.c zlibInflate over the wrapped stream", "(bundled)", Status::Stub, "", {}},
    {"GZIPInputStream", "puff.c gzipInflate (Inflate.cpp)", "(bundled)", Status::Stub, "level.dat/player.dat/chunk NBT read path -- still a no-op, so world load reads empty NBT.", {}},
    {"GZIPOutputStream", "gzipDeflateStored (Inflate.cpp -- valid gzip via STORED blocks)", "(bundled)", Status::Stub, "level.dat/player.dat write path -- no-op, so world never persists.", {}},
    {"Deflater", "no Huffman encoder bundled; gzipDeflateStored gives valid (uncompressed) output", "(bundled)", Status::Stub, "", {}},
};
constexpr Equiv kNetwork[] = {
    {"Socket", "XNetStartup(BYPASS_SECURITY) + socket/connect/send/recv (Winsock)", "xnet.lib", Status::Real, "Socket.reference: real XNet TCP; setsockopt 0x5801/0x5802 to talk to a PC (insecure) server.", {}},
    {"InetAddress", "XNetDnsLookup / XNetGetTitleXnAddr", "xnet.lib", Status::Real, "", {}},
    {"URL", "XHttpOpen / XHttpConnect / XHttpSendRequest / XHttpReceiveResponse", "xhttp.lib", Status::Stub, "LIVE: NetClientHandler online-mode auth (joinserver) -> openStream NULL -> online-mode servers reject login.", {}},
    {"HttpURLConnection", "XHttp*", "xhttp.lib", Status::Dead, "Only the minecraft.net asset/skin downloader -- dead offline.", {}},
};
constexpr Equiv kMemory[] = {
    {"Runtime", "GlobalMemoryStatus (MEMORYSTATUS) / MmQueryStatistics", "xboxkrnl.exe", Status::Real, "freeMemory/totalMemory/maxMemory -> physical pool. Replaces the diagnostic operator-new counters.", {}},
};
constexpr Equiv kConcurrency[] = {
    {"Thread", "CreateThread / ExCreateThread, Sleep, WaitForSingleObject; XSetThreadProcessor for affinity", "xboxkrnl.exe", Status::Real, "Gated: the net reader/writer/connect threads get a real CreateThread; dead java daemons keep a no-op start() (real start() froze the menu).", {}},
};
constexpr Equiv kUser[] = {
    {"Session", "XamUserGetName / XamUserGetSigninState (gamertag)", "xam.lib", Status::Real, "Session username seeded from the signed-in gamertag (XboxUser).", {}},
    {"StatFileWriter", "XUserWriteAchievements (achievement unlock) -- NOT the minecraft.net stats sync", "xam.lib", Status::Stub, "Local stats accumulate; the online sync is dead. Achievements go via the XLAST/SPA seam.", {}},
};
constexpr Equiv kImage[] = {
    {"BufferedImage", "D3DXCreateTextureFromFileInMemoryEx / D3DXLoadSurfaceFromMemory", "d3dx9.lib", Status::Real, "Real ARGB int buffer; texture pixel ops + screenshots. getRaster()->empty (dead skin decode).", {}},
    {"ImageIO", "D3DXCreateTextureFromFileInMemoryEx (PNG decode on the GPU path)", "d3dx9.lib", Status::Real, "", {}},
};

constexpr Subsystem kSubsystems[] = {
    {"video", kVideo},       {"input", kInput},     {"audio", kAudio},   {"fileio", kFileio},
    {"compression", kCompression}, {"network", kNetwork}, {"memory", kMemory}, {"concurrency", kConcurrency},
    {"user", kUser},         {"image", kImage},
};

}  // namespace

std::span<const Subsystem> subsystems() { return kSubsystems; }

const Equiv* lookup(std::string_view basename) {
    for (const Subsystem& sub : kSubsystems) {
        for (const Equiv& e : sub.entries) {
            if (e.basename == basename) return &e;
        }
    }
    return nullptr;
}

std::string_view status_name(Status s) {
    switch (s) {
        case Status::Real: return "real";
        case Status::Stub: return "stub";
        case Status::Dead: return "dead";
    }
    return "";
}

std::optional<Status> parse_status(std::string_view name) {
    if (name == "real") return Status::Real;
    if (name == "stub") return Status::Stub;
    if (name == "dead") return Status::Dead;
    return std::nullopt;
}

Kind kind(Status status, std::string_view lib) {
    if (status == Status::Dead) return Kind::Dead;
    if (lib == "(none)" || lib == "(bundled)" || lib == "(CRT)") return Kind::Handwritten;
    return Kind::Xdk;
}

Kind kind(const Equiv& e) { return kind(e.status, e.lib); }

std::string_view kind_name(Kind k) {
    switch (k) {
        case Kind::Xdk: return "xdk";
        case Kind::Handwritten: return "handwritten";
        case Kind::Dead: return "dead";
    }
    return "";
}

std::string annotation(std::string_view basename) {
    const Equiv* e = lookup(basename);
    if (e == nullptr) return "";
    switch (kind(*e)) {
        case Kind::Dead:
            return "// [360] no equivalent -- dead on console: " + std::string(e->note);
        case Kind::Handwritten:
            return "// [360] NO XDK EQUIVALENT -- hand-implemented: " + std::string(text::trim(e->xdk, "()"));
        case Kind::Xdk:
            break;
    }
    std::string out = "// [360] equivalent: " + std::string(e->xdk);
    if (!e->lib.empty()) out += "  (" + std::string(e->lib) + ")";
    return out;
}

}  // namespace cb::java::console::platform_equivalents
