// codebreaker/java/console/platform_equivalents.cpp -- C++ port of codebreaker.java.console.platform_equivalents
// (see platform_equivalents.hpp).
//
// Subsystems, in the Python's order: VIDEO (OpenGL/LWJGL -> Direct3D 9), INPUT (controller / keyboard), AUDIO
// (music / SFX), FILE IO + SAVE, COMPRESSION (zlib/gzip), NETWORKING, MEMORY, THREADING / TIME,
// USER / PROFILE / ACHIEVEMENTS, IMAGE / TEXTURE DECODE.
//
// NOTE (from the Python): the OPPORTUNITIES and platform REQUIREMENTS are no longer hardcoded here -- they are
// AUTO-DETECTED from the INSTALLED SDK by xdk_scan.py (each is claimed only because its XDK function is actually
// present in the scanned headers). See codebreaker/java/console/xdk_scan.py PATTERNS +
// platform_coverage.render_report. EQUIV below stays: it is the Java-symbol -> XDK-family INTENT map that drives
// the // [360] stub annotations.

#include "codebreaker/java/console/platform_equivalents.hpp"

namespace codebreaker::java::console::platform_equivalents {

const py::dict<py::str, py::dict<py::str, Equiv>> EQUIV = {    {py::str("video"), {
        {py::str("GL11"), Equiv{py::str("IDirect3DDevice9 (DrawPrimitiveUP / SetRenderState / SetTexture / SetTransform / SetSamplerState)"), py::str("d3d9.lib"), py::str("real"), py::str("GL11.reference.cpp is the GL->D3D9 emulator. HALF-PIXEL OFFSET: set D3DRS_HALFPIXELOFFSET."), std::nullopt}},
        {py::str("GL12"), Equiv{py::str("IDirect3DDevice9 render/sampler states"), py::str("d3d9.lib"), py::str("stub"), py::str("GL_RESCALE_NORMAL -> D3DRS_NORMALIZENORMALS; a few constants route through GL11. Minor."), std::nullopt}},
        {py::str("GL15"), Equiv{py::str("IDirect3DVertexBuffer9 / IDirect3DIndexBuffer9"), py::str("d3d9.lib"), py::str("stub"), py::str("Only referenced by the gated-off VBO path; inert until VBOs are enabled."), std::nullopt}},
        {py::str("GLU"), Equiv{py::str("D3DXMatrixPerspectiveFovRH / D3DXMatrixLookAtRH"), py::str("d3dx9.lib"), py::str("real"), py::str("gluPerspective is REAL (fixes the blank-3D bug)."), py::dict<py::str, py::str>{{py::str("gluPerspective"), py::str("D3DXMatrixPerspectiveFovRH -> SetTransform(D3DTS_PROJECTION)")}, {py::str("gluLookAt"), py::str("D3DXMatrixLookAtRH")}, {py::str("gluErrorString"), py::str("no equiv -- returns \"\" (log only)")}}}},
        {py::str("ARBVertexBufferObject"), Equiv{py::str("IDirect3DDevice9::CreateVertexBuffer + Lock/Unlock"), py::str("d3d9.lib"), py::str("stub"), py::str("Gated OFF (GLContext caps false) -> falls back to DrawPrimitiveUP. LANDMINE: enabling VBOs without implementing this blanks all terrain."), std::nullopt}},
        {py::str("ARBOcclusionQuery"), Equiv{py::str("IDirect3DQuery9(D3DQUERYTYPE_OCCLUSION) Issue/GetData"), py::str("d3d9.lib"), py::str("stub"), py::str("Gated OFF -> culling disabled (draws all in-frustum chunks). Correct visuals."), std::nullopt}},
        {py::str("NVFogDistance"), Equiv{py::str("D3DRS_FOGVERTEXMODE / D3DRS_FOGTABLEMODE"), py::str("d3d9.lib"), py::str("stub"), py::str("Gated OFF -> GL11 linear planar fog (real) is used instead."), std::nullopt}},
        {py::str("BufferUtils"), Equiv{py::str("plain heap buffer (malloc) -- NIO scratch, no XDK needed"), py::str("(CRT)"), py::str("stub"), py::str("createByteBuffer returns NULL -> F2 screenshot NULL-crashes. Trivial real fix: return a malloc buffer."), std::nullopt}},
        {py::str("Display"), Equiv{py::str("IDirect3DDevice9::Present + XInput poll"), py::str("d3d9.lib / xinput2.lib"), py::str("real"), py::str("Display.reference: update()=poll+Present, swapBuffers()=EndScene/Present/BeginScene."), std::nullopt}},
        {py::str("DisplayMode"), Equiv{py::str("D3DPRESENT_PARAMETERS BackBufferWidth/Height + XGetVideoMode"), py::str("d3d9.lib"), py::str("stub"), py::str("Boot uses ctor dims; the object is otherwise ignored by Display. Inert."), std::nullopt}},
        {py::str("Project"), Equiv{py::str("D3DXVec3Project / D3DXVec3Unproject"), py::str("d3dx9.lib"), py::str("stub"), py::str("gluProject/UnProject-equivalent; used by picking/particles if ever hit."), std::nullopt}},
    }},
    {py::str("input"), {
        {py::str("Keyboard"), Equiv{py::str("XamInputGetState(XINPUT_STATE) -> synthesized key events"), py::str("xinput2.lib"), py::str("real"), py::str("Input.reference: the XInput2 controller adapter maps pad -> Keyboard/Mouse events."), std::nullopt}},
        {py::str("Mouse"), Equiv{py::str("XamInputGetState right-stick/triggers -> mouse delta+buttons"), py::str("xinput2.lib"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("Controllers"), Equiv{py::str("XamInputGetState / XamInputGetCapabilities"), py::str("xinput2.lib"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("OSK"), Equiv{py::str("XamShowKeyboardUI (async, XOVERLAPPED) -> XGetOverlappedResult"), py::str("xam.lib"), py::str("real"), py::str("OSK.reference: show()/active()/poll(); the on-screen keyboard for text fields + chat."), std::nullopt}},
    }},
    {py::str("audio"), {
        {py::str("SoundManager"), Equiv{py::str("IXACT3Engine::CreateSoundBank/CreateInMemoryWaveBank + IXACT3SoundBank::Play (cue)"), py::str("xact3.lib"), py::str("real"), py::str("Audio.reference bridges paulscode SoundManager -> XACT3 cues."), std::nullopt}},
        {py::str("SoundSystem"), Equiv{py::str("IXACT3Engine (XACT3CreateEngine, DoWork per frame)"), py::str("xact3.lib"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("CodecJOrbis"), Equiv{py::str("XMA (XMACreateContext) if decoding at runtime; else ffmpeg->XMA at asset-build"), py::str("xmcore.lib"), py::str("dead"), py::str("Bypassed: ogg is pre-decoded to the XACT wavebank; no runtime ogg decode."), std::nullopt}},
        {py::str("CodecWav"), Equiv{py::str("(XACT wavebank owns PCM)"), py::str("xact3.lib"), py::str("dead"), std::nullopt, std::nullopt}},
        {py::str("LibraryLWJGLOpenAL"), Equiv{py::str("(XACT replaces OpenAL entirely)"), py::str("xact3.lib"), py::str("dead"), std::nullopt, std::nullopt}},
        {py::str("SoundSystemConfig"), Equiv{py::str("(XACT engine params)"), py::str("xact3.lib"), py::str("dead"), py::str("The real SoundSystem ignores this paulscode config object."), std::nullopt}},
    }},
    {py::str("fileio"), {
        {py::str("File"), Equiv{py::str("GetFileAttributes / CreateDirectory / MoveFile / DeleteFile"), py::str("xboxkrnl.exe"), py::str("real"), py::str("File.h is Win32-backed (recursive mkdirs for RegionFile <x36>\\<z36> nesting)."), std::nullopt}},
        {py::str("FileInputStream"), Equiv{py::str("CreateFile(GENERIC_READ) + ReadFile"), py::str("xboxkrnl.exe"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("FileOutputStream"), Equiv{py::str("CreateFile(CREATE_ALWAYS) + WriteFile"), py::str("xboxkrnl.exe"), py::str("stub"), py::str("STILL A NO-OP -> nothing is ever written to disk. The single biggest save/load gap."), std::nullopt}},
        {py::str("RandomAccessFile"), Equiv{py::str("CreateFile + SetFilePointer + ReadFile/WriteFile"), py::str("xboxkrnl.exe"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("FileReader"), Equiv{py::str("(CreateFile + ReadFile, char decode)"), py::str("xboxkrnl.exe"), py::str("stub"), py::str("NOT a Reader subclass -> options.txt never loads (falls to BufferedReader variadic ctor)."), std::nullopt}},
        {py::str("FileWriter"), Equiv{py::str("(CreateFile + WriteFile)"), py::str("xboxkrnl.exe"), py::str("stub"), py::str("no-op -> options.txt never saved."), std::nullopt}},
        {py::str("SaveHandler"), Equiv{py::str("XContentCreate / XContentClose / XContentFlush (STFS package on the HDD)"), py::str("xam.lib"), py::str("stub"), py::str("World saves should live in an STFS content package, mounted as a drive, flushed to persist."), std::nullopt}},
        {py::str("DataOutputStream"), Equiv{py::str("(pure big-endian byte codec -- no XDK)"), py::str("(none)"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("DataInputStream"), Equiv{py::str("(pure big-endian byte codec -- no XDK)"), py::str("(none)"), py::str("real"), std::nullopt, std::nullopt}},
    }},
    {py::str("compression"), {
        {py::str("Inflater"), Equiv{py::str("NO XDK zlib -- bundled puff.c (Inflate.cpp: zlibInflate/rawInflate)"), py::str("(bundled)"), py::str("real"), py::str("XMemDecompress (xcompress.lib) is LZX, NOT zlib -- WRONG format for Minecraft chunks. Use puff."), std::nullopt}},
        {py::str("InflaterInputStream"), Equiv{py::str("puff.c zlibInflate over the wrapped stream"), py::str("(bundled)"), py::str("stub"), std::nullopt, std::nullopt}},
        {py::str("GZIPInputStream"), Equiv{py::str("puff.c gzipInflate (Inflate.cpp)"), py::str("(bundled)"), py::str("stub"), py::str("level.dat/player.dat/chunk NBT read path -- still a no-op, so world load reads empty NBT."), std::nullopt}},
        {py::str("GZIPOutputStream"), Equiv{py::str("gzipDeflateStored (Inflate.cpp -- valid gzip via STORED blocks)"), py::str("(bundled)"), py::str("stub"), py::str("level.dat/player.dat write path -- no-op, so world never persists."), std::nullopt}},
        {py::str("Deflater"), Equiv{py::str("no Huffman encoder bundled; gzipDeflateStored gives valid (uncompressed) output"), py::str("(bundled)"), py::str("stub"), std::nullopt, std::nullopt}},
    }},
    {py::str("network"), {
        {py::str("Socket"), Equiv{py::str("XNetStartup(BYPASS_SECURITY) + socket/connect/send/recv (Winsock)"), py::str("xnet.lib"), py::str("real"), py::str("Socket.reference: real XNet TCP; setsockopt 0x5801/0x5802 to talk to a PC (insecure) server."), std::nullopt}},
        {py::str("InetAddress"), Equiv{py::str("XNetDnsLookup / XNetGetTitleXnAddr"), py::str("xnet.lib"), py::str("real"), std::nullopt, std::nullopt}},
        {py::str("URL"), Equiv{py::str("XHttpOpen / XHttpConnect / XHttpSendRequest / XHttpReceiveResponse"), py::str("xhttp.lib"), py::str("stub"), py::str("LIVE: NetClientHandler online-mode auth (joinserver) -> openStream NULL -> online-mode servers reject login."), std::nullopt}},
        {py::str("HttpURLConnection"), Equiv{py::str("XHttp*"), py::str("xhttp.lib"), py::str("dead"), py::str("Only the minecraft.net asset/skin downloader -- dead offline."), std::nullopt}},
    }},
    {py::str("memory"), {
        {py::str("Runtime"), Equiv{py::str("GlobalMemoryStatus (MEMORYSTATUS) / MmQueryStatistics"), py::str("xboxkrnl.exe"), py::str("real"), py::str("freeMemory/totalMemory/maxMemory -> physical pool. Replaces the diagnostic operator-new counters."), std::nullopt}},
    }},
    {py::str("concurrency"), {
        {py::str("Thread"), Equiv{py::str("CreateThread / ExCreateThread, Sleep, WaitForSingleObject; XSetThreadProcessor for affinity"), py::str("xboxkrnl.exe"), py::str("real"), py::str("Gated: the net reader/writer/connect threads get a real CreateThread; dead java daemons keep a no-op start() (real start() froze the menu)."), std::nullopt}},
    }},
    {py::str("user"), {
        {py::str("Session"), Equiv{py::str("XamUserGetName / XamUserGetSigninState (gamertag)"), py::str("xam.lib"), py::str("real"), py::str("Session username seeded from the signed-in gamertag (XboxUser)."), std::nullopt}},
        {py::str("StatFileWriter"), Equiv{py::str("XUserWriteAchievements (achievement unlock) -- NOT the minecraft.net stats sync"), py::str("xam.lib"), py::str("stub"), py::str("Local stats accumulate; the online sync is dead. Achievements go via the XLAST/SPA seam."), std::nullopt}},
    }},
    {py::str("image"), {
        {py::str("BufferedImage"), Equiv{py::str("D3DXCreateTextureFromFileInMemoryEx / D3DXLoadSurfaceFromMemory"), py::str("d3dx9.lib"), py::str("real"), py::str("Real ARGB int buffer; texture pixel ops + screenshots. getRaster()->empty (dead skin decode)."), std::nullopt}},
        {py::str("ImageIO"), Equiv{py::str("D3DXCreateTextureFromFileInMemoryEx (PNG decode on the GPU path)"), py::str("d3dx9.lib"), py::str("real"), std::nullopt, std::nullopt}},
    }},
};

std::optional<Equiv> lookup(const py::str& basename) {
    for (const py::dict<py::str, Equiv>& sub : EQUIV.values()) {
        if (sub.contains(basename)) {
            return sub.at(basename);
        }
    }
    return std::nullopt;
}

py::str kind(const Equiv& e) {
    if (e.status == "dead") {
        return "dead";
    }
    if (e.lib == "(none)" || e.lib == "(bundled)" || e.lib == "(CRT)") {
        return "handwritten";
    }
    return "xdk";
}

py::str annotation(const py::str& basename) {
    std::optional<Equiv> e = lookup(basename);
    if (!e.has_value()) {
        return "";
    }
    py::str k = kind(*e);
    if (k == "dead") {
        return py::percent("// [360] no equivalent -- dead on console: %s", e->note.value_or(py::str("")));
    }
    if (k == "handwritten") {
        return py::percent("// [360] NO XDK EQUIVALENT -- hand-implemented: %s", e->xdk.strip(py::str("()")));
    }
    py::str lib = e->lib;
    lib = !lib.empty() ? py::percent("  (%s)", lib) : py::str("");
    return py::percent("// [360] equivalent: %s%s", e->xdk, lib);
}

py::list<std::tuple<py::str, py::str, Equiv>> all_entries() {
    py::list<std::tuple<py::str, py::str, Equiv>> out;
    for (const auto& sub_items : EQUIV.items()) {
        for (const auto& name_entry : std::get<1>(sub_items).items()) {
            out.append(std::make_tuple(std::get<0>(sub_items), std::get<0>(name_entry), std::get<1>(name_entry)));
        }
    }
    return out;
}

}  // namespace codebreaker::java::console::platform_equivalents