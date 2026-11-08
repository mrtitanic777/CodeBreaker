// codebreaker/java/console/xdk_scan.cpp -- C++ port of codebreaker.java.console.xdk_scan (see the header).

#include "codebreaker/java/console/xdk_scan.hpp"

namespace codebreaker::java::console::xdk_scan {

const py::re::Pattern _FUNC_RE = py::re::compile(
    py::str("\\b((?:Xam[A-Z]|X[A-Z]|D3DX|Mm[A-Z]|Ke[A-Z]|Nt[A-Z]|Vd[A-Z])\\w+)\\s*\\("));

py::dict<py::str, py::str> scan_functions(const std::optional<py::str>& include_dir) {
    py::dict<py::str, py::str> funcs;
    if (!include_dir || include_dir->empty() || !py::os::path::isdir(*include_dir)) {
        return funcs;
    }
    for (const auto& walked : py::os::walk(*include_dir)) {
        const py::str& root = std::get<0>(walked);
        const py::list<py::str>& files = std::get<2>(walked);
        for (const py::str& fn : files) {
            if (!fn.endswith(py::str(".h"))) {
                continue;
            }
            py::str txt;
            try {
                txt = py::open(py::os::path::join(root, fn), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::Exception&) {
                continue;
            }
            for (const py::re::Match& m : _FUNC_RE.finditer(txt)) {
                funcs.setdefault(m.group(1), fn);
            }
        }
    }
    return funcs;
}

std::optional<py::str> sdk_include_dir(const std::optional<py::str>& sdk_root) {
    if (!sdk_root || sdk_root->empty()) {
        return std::nullopt;
    }
    const py::str cands[] = {py::os::path::join(*sdk_root, py::str("include"), py::str("xbox")),
                             py::os::path::join(*sdk_root, py::str("include")), *sdk_root};
    for (const py::str& cand : cands) {
        if (py::os::path::isdir(cand)) {
            return cand;
        }
    }
    return std::nullopt;
}

const py::list<Pattern> PATTERNS = {
    {py::str("XContentCreate"), py::str("XContent"), py::str("requirement"), py::str("save"), py::str("correctness"), std::nullopt,
     py::str("World saves must go through an STFS content package (not a raw file write)"),
     py::str("This SDK exports the XContent* content-package family -- the durable per-profile storage "
         "mechanism on the 360. CreateFile/WriteFile to a plain path is NOT committed to the "
         "device until the package is flushed (XContentFlush) after a save batch.")},
    {py::str("XShowDeviceSelectorUI"), py::str("XShowDeviceSelectorUI"), py::str("requirement"), py::str("save"), py::str("cert"), std::nullopt,
     py::str("Storage device must be selected before saving"),
     py::str("The SDK provides the device-selector UI; the chosen XCONTENTDEVICEID scopes "
         "XContentCreate. Cert requires picking a device and handling no-device / "
         "not-enough-space.")},
    {py::str("XNotifyGetNext"), py::str("XNotify"), py::str("requirement"), py::str("system"), py::str("cert"), std::nullopt,
     py::str("Pause/yield when the Guide overlay opens; handle sign-in & controller changes"),
     py::str("The SDK exposes the notification family (XNotifyGetNext). A title that keeps "
         "simulating/playing audio under the Guide overlay is a cert failure; also handle "
         "XN_SYS_SIGNINCHANGED and controller unplug.")},
    {py::str("XNetStartup"), py::str("XNet"), py::str("requirement"), py::str("network"), py::str("compat"), std::nullopt,
     py::str("Talking to a PC/Java server needs XNet security bypassed"),
     py::str("XNetStartup(XNET_STARTUP_BYPASS_SECURITY) + insecure setsockopt -- otherwise XNet "
         "encrypts all traffic and refuses non-Xbox peers, so a standard Minecraft (PC) server "
         "sees garbage.")},
    {py::str("XPhysicalAlloc"), py::str("XPhysical"), py::str("requirement"), py::str("video"), py::str("correctness"), std::nullopt,
     py::str("GPU resources should come from physical memory"),
     py::str("The SDK exposes XPhysicalAlloc/XPhysicalFree. The GPU reads physical (not paged) memory; "
         "allocating textures/vertex buffers from the general heap is the wrong path and feeds "
         "heap fragmentation.")},
    {py::str("XShowKeyboardUI"), py::str("XShowKeyboardUI"), py::str("equivalent"), py::str("input"), std::nullopt, std::nullopt,
     py::str("On-screen keyboard for text input"),
     py::str("XShowKeyboardUI (async) -- the OSK for server address / chat, since there is no PC "
         "keyboard.")},
    {py::str("XInputSetState"), py::str("XInputSetState"), py::str("opportunity"), py::str("input"), std::nullopt, py::str("S"),
     py::str("Controller rumble"),
     py::str("XInputSetState(XINPUT_VIBRATION) -- haptics on damage/explosions/mining.")},
    {py::str("XMPGetStatus"), py::str("XMP"), py::str("opportunity"), py::str("audio"), std::nullopt, py::str("M"),
     py::str("Custom soundtracks"),
     py::str("The XMP family lets the player's own dashboard music play over the game.")},
    {py::str("XMemCompress"), py::str("XMemCompress"), py::str("opportunity"), py::str("save"), std::nullopt, py::str("M"),
     py::str("LZX save compression"),
     py::str("XMemCompress/XMemDecompress (LZX) for our own saves -- smaller/faster than zlib. NOTE: "
         "MP server chunks stay zlib/puff; LZX is a different format.")},
    {py::str("XShowMessageBoxUI"), py::str("XShowMessageBoxUI"), py::str("opportunity"), py::str("system"), std::nullopt, py::str("S"),
     py::str("Native system dialogs"),
     py::str("XShowMessageBoxUI -- connect-failed/kicked prompts as the real 360 box.")},
    {py::str("XGetLanguage"), py::str("XGetLanguage"), py::str("opportunity"), py::str("i18n"), std::nullopt, py::str("S"),
     py::str("Console-language i18n"),
     py::str("XGetLanguage -> load the matching .lang instead of hardcoded en_US.")},
    {py::str("XGetVideoMode"), py::str("XGetVideoMode"), py::str("opportunity"), py::str("video"), std::nullopt, py::str("S"),
     py::str("Native display resolution"),
     py::str("XGetVideoMode -> size the render target to 480p/720p/1080p + widescreen.")},
    {py::str("XJSONCreateReader"), py::str("XJSON"), py::str("opportunity"), py::str("io"), std::nullopt, py::str("S"),
     py::str("Native JSON parser"),
     py::str("The XJSON family replaces the argo-JSON stub cluster (StringReader/J_*).")},
};

ScanResult detect(const std::optional<py::str>& sdk_root) {
    std::optional<py::str> inc = sdk_include_dir(sdk_root);
    py::dict<py::str, py::str> funcs = scan_functions(inc);
    py::list<Detection> found, missing;
    for (const Pattern& p : PATTERNS) {
        bool present = funcs.contains(p.needs);
        py::list<py::str> fam;
        for (const py::str& f : funcs.keys()) {
            if (f.startswith(p.family)) {
                fam.append(f);
            }
        }
        fam.sort();
        Detection rec;
        static_cast<Pattern&>(rec) = p;
        rec.present = present;
        rec.functions = fam.slice(0, 12);   // the actual functions detected (capped for display)
        rec.func_count = py::len(fam);
        rec.header = funcs.get(p.needs, py::str(""));
        (present ? found : missing).append(rec);
    }
    return ScanResult{inc, funcs, found, missing};
}

}  // namespace codebreaker::java::console::xdk_scan
