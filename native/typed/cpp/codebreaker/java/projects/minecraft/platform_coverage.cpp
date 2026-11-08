// codebreaker/java/projects/minecraft/platform_coverage.cpp -- C++ from codebreaker.java.projects.minecraft.platform_coverage.

#include "codebreaker/java/projects/minecraft/platform_coverage.hpp"

#include <algorithm>
#include <exception>

#include "codebreaker/java/console/platform_equivalents.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/xdk_scan.hpp"
#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::projects::minecraft::platform_coverage {

namespace {

using py::str;
using StrList = py::list<py::str>;

StrList strs(std::initializer_list<const char*> xs) {
    StrList out;
    for (const char* x : xs) out.append(str(x));
    return out;
}

py::re::Pattern compile(const std::string& p) { return py::re::Pattern(str(p), 0); }

// '\n'.join(parts)
str join_lines(const StrList& parts) { return py::join(str("\n"), parts); }

}  // namespace

// ---------------------------------------------------------------------------
// Subsystem map: which platform holders/types belong to each, which reference
// files hold the real Alpha implementation, and the implementation-choice menu.
// ---------------------------------------------------------------------------
const py::list<Subsystem> SUBSYSTEMS = {
    Subsystem{
        str("video"),
        str("Video  --  OpenGL (LWJGL) -> Direct3D 9"),
        strs({"GL11", "GL12", "GL15", "GLU", "GLContext", "ARBOcclusionQuery", "ARBVertexBufferObject", "BufferUtils"}),
        strs({"GLAllocation", "Tessellator"}),
        strs({"GL11", "Shaders", "Mipmaps", "NIOBuffers", "ImageIO", "BufferedImage"}),
        strs({"A) REUSE the Alpha GL->D3D9 emulator (docs/platform/reference/GL11.reference.cpp) -- "
              "the GL API is identical between Alpha and Beta, so this is the recommended path; only the "
              "MISSING symbols below are new work.",
              "B) Write a fresh Direct3D 9 backend from scratch.",
              "C) Stub to no-ops (renders nothing / placeholder) -- for bring-up only."}),
        strs({"HALF-PIXEL OFFSET (Direct3D 9). D3D9 maps screen coordinates to pixel CENTERS but texture "
              "samples to the top-left CORNER, so any 2D/HUD/text drawn without a -0.5 screen-space shift "
              "lands half a texel off -> blurry / smeared, MOST visible on font atlases and text. OpenGL "
              "(the source) has no such offset, so the GL->D3D9 port introduces it. On the Xbox 360 the "
              "fix is the hardware render state `D3DRS_HALFPIXELOFFSET = TRUE` (applied once at device "
              "setup -- see Main.reference.cpp) rather than baking -0.5 into every GUI/font vertex. "
              "D3D10/11 removed the offset entirely; a D3D9 backend on PC still needs the -0.5 vertex "
              "shift. VERIFY ON HARDWARE with the in-game font/GUI."})},
    Subsystem{
        str("input"),
        str("Input  --  controller / keyboard / mouse"),
        strs({"Keyboard", "Mouse", "Controllers"}),
        strs({"MouseHelper", "MovementInput"}),
        strs({"Input", "OSK"}),
        strs({"A) XInput2 controller mapping + on-screen keyboard (docs/platform/reference/Input, OSK).",
              "B) Stub (no input) -- boots but is unplayable."}),
        StrList()},
    Subsystem{
        str("audio"),
        str("Audio  --  music / sound effects"),
        strs({"SoundSystem", "SoundSystemConfig"}),
        strs({"SoundSystem", "SoundManager", "CodecJOrbis", "SoundPool"}),
        strs({"Audio"}),
        strs({"A) XACT3 -- authored wave banks (.xwb) + sound banks (.xsb), cue-driven. The "
              "Audio.reference routes SoundManager -> XACT cues; the `codebreaker java xact` tool builds the banks "
              "from Content.md + the .ogg/.wav sources. RECOMMENDED (the Alpha port uses XACT3).",
              "B) XAudio2 -- low-level source voices; you own mixing/streaming/format decode yourself "
              "(more code, no authoring pipeline).",
              "C) Stub-silent (no audio)."}),
        StrList()},
    Subsystem{
        str("display"),
        str("Display / window / timing"),
        strs({"Display", "Sys"}),
        strs({"DisplayMode", "Canvas", "Frame", "Applet", "Panel", "Timer"}),
        strs({"Display"}),
        strs({"A) Fixed 1280x720 back buffer + vsync (docs/platform/reference/Display.reference).",
              "B) Stub."}),
        StrList()},
    Subsystem{
        str("fileio"),
        str("File I/O & world save / load"),
        StrList(),
        strs({"File", "RandomAccessFile", "FileInputStream", "FileOutputStream", "FileReader",
              "FileWriter", "RegionFile", "RegionFileCache", "ZipFile", "SaveHandler",
              "ChunkLoader", "McRegionChunkLoader"}),
        strs({"FileIO", "ZipFile", "Inflate"}),
        strs({"A) STFS / Save360 XContent -- persistent saves across boots (needs the writable Save: mount).",
              "B) In-memory only -- worlds play but do NOT persist across reboots.",
              "C) Stub -- no save/load at all."}),
        StrList()},
    Subsystem{
        str("network"),
        str("Networking (SMP)"),
        StrList(),
        strs({"Socket", "URL", "URLConnection", "HttpURLConnection", "NetworkManager",
              "NetClientHandler", "InetAddress"}),
        strs({"Socket"}),
        strs({"A) XNet insecure socket -- LAN SMP (docs/platform/reference/Socket.reference.cpp). "
              "Off-subnet needs a portproxy relay.",
              "B) Stub-offline -- singleplayer only."}),
        StrList()},
    Subsystem{
        str("memory"),
        str("Memory / allocation"),
        strs({"Runtime"}),
        StrList(),
        StrList(),
        strs({"A) Report the real Xbox 360 title memory budget (XMemGetAllocationInfo / retail 512MB).",
              "B) Fixed constants (freeMemory/totalMemory return a nominal budget) -- fine unless the "
              "game gates behaviour on free memory."}),
        StrList()},
    Subsystem{
        str("user"),
        str("User / profile / session"),
        StrList(),   // System.* is java.lang, provided by JavaRT.h prelude (not a platform gap)
        strs({"Session", "StatsSyncher", "MessageDigest"}),
        strs({"XboxUser"}),
        strs({"A) XUserGetName / gamertag + local profile (docs/platform/reference/XboxUser.reference).",
              "B) Stub (fixed \"Player\" identity)."}),
        StrList()},
};

str _read(const str& path) {
    return textutil::read_text(path);   // tolerant: errors='replace', missing -> ''
}

const py::re::Pattern& _STUB_MARKER() {
    static const py::re::Pattern p = compile("J2X-STUB\\[([a-z0-9-]+)\\]\\s*:\\s*([^\\n]*)");
    return p;
}

StubMarkers scan_stub_markers(const str& output_dir) {
    StubMarkers out;
    std::vector<str> names;
    for (const str& f : py::os::listdir(output_dir)) names.push_back(f);
    std::sort(names.begin(), names.end());
    for (const str& f : names) {
        if (!(f.endswith(str(".cpp")) || f.endswith(str(".h")))) continue;
        str text = _read(py::os::path::join(output_dir, f));
        if (!text.contains(str("J2X-STUB"))) continue;
        int64_t i = 1;
        for (const str& line : py::split(text, str("\n"))) {
            auto m = _STUB_MARKER().search(line);
            if (m) {
                out.setdefault(m->group(1), py::list<StubSite>()).append(StubSite(f, i, m->group(2).strip().slice(0, 100)));
            }
            ++i;
        }
    }
    return out;
}

const py::re::Pattern& _FN_DECL() {
    static const py::re::Pattern p = compile("\\b([A-Za-z_]\\w*)\\s*\\(");
    return p;
}
const py::re::Pattern& _CONST_DECL() {
    static const py::re::Pattern p = compile("\\b(?:const\\s+\\w[\\w:<>]*|#define)\\s+([A-Z][A-Z0-9_]{2,})\\b");
    return p;
}
const py::re::Pattern& _ENUM_CONST() {
    static const py::re::Pattern p = compile("\\b([A-Z][A-Z0-9_]{2,})\\s*=");
    return p;
}

py::set<str> _provided_symbols(const str& header_text) {
    py::set<str> syms;
    for (const py::re::Match& m : _FN_DECL().finditer(header_text)) syms.add(m.group(1));
    for (const py::re::Match& m : _CONST_DECL().finditer(header_text)) syms.add(m.group(1));
    for (const py::re::Match& m : _ENUM_CONST().finditer(header_text)) syms.add(m.group(1));
    for (const char* k : {"if", "for", "while", "return", "sizeof", "switch"}) syms.discard(str(k));
    return syms;
}

py::dict<str, int64_t> _used_holder_symbols(const str& game_text, const str& holder) {
    py::dict<str, int64_t> out;
    for (const py::re::Match& m : compile("\\b" + py::re::escape(holder).utf8() + "\\s*::\\s*([A-Za-z_]\\w*)").finditer(game_text)) {
        str k = m.group(1);
        out.set(k, out.get(k, 0) + 1);
    }
    return out;
}

int64_t _type_usage_count(const str& game_text, const str& typ) {
    return py::len(compile("(?<![\\w:])" + py::re::escape(typ).utf8() + "\\b").findall(game_text));
}

py::list<Coverage> analyze(const str& output_dir, const std::optional<str>& reference_dir, const std::optional<py::set<str>>& runtime_files_) {
    py::set<str> runtime_files = runtime_files_ ? *runtime_files_ : py::set<str>();
    StrList files;
    for (const str& f : py::os::listdir(output_dir))
        if (f.endswith(str(".cpp")) || f.endswith(str(".h"))) files.append(f);
    // game code = everything except the emitted runtime/stub headers
    StrList game_texts;
    for (const str& f : files)
        if (!runtime_files.contains(f)) game_texts.append(_read(py::os::path::join(output_dir, f)));
    str game_blob = join_lines(game_texts);

    auto stub_provides = [&](const str& holder) { return _provided_symbols(_read(py::os::path::join(output_dir, holder + str(".h")))); };

    auto reference_provides = [&](const StrList& names) {
        py::set<str> s;
        if (!reference_dir || reference_dir->empty()) return s;
        for (const str& n : names) {
            s.update(_provided_symbols(_read(py::os::path::join(*reference_dir, n + str(".reference.h")))));
            s.update(_provided_symbols(_read(py::os::path::join(*reference_dir, n + str(".reference.cpp")))));
        }
        return s;
    };

    py::list<Coverage> results;
    for (const Subsystem& sub : SUBSYSTEMS) {
        py::set<str> ref_syms = reference_provides(sub.reference);
        bool ref_exists = !sub.reference.empty() && !ref_syms.empty();
        py::list<SymbolUse> symbols;   // (symbol, count, status)
        for (const str& holder : sub.holders) {
            py::set<str> provided = stub_provides(holder);
            auto used = _used_holder_symbols(game_blob, holder).items();
            std::stable_sort(used.begin(), used.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
            for (const auto& kv : used) {
                const str& sym = std::get<0>(kv);
                str status;
                if (ref_syms.contains(sym)) status = str("REUSABLE");
                else if (provided.contains(sym)) status = str("STUB");
                else status = str("MISSING");
                symbols.append(SymbolUse(holder + str("::") + sym, std::get<1>(kv), status));
            }
        }
        py::list<TypeUse> types;   // (type, count, has_ref)
        for (const str& typ : sub.types) {
            int64_t cnt = _type_usage_count(game_blob, typ);
            if (cnt != 0) types.append(TypeUse(typ, cnt, sub.reference.contains(typ) || ref_syms.contains(typ)));
        }
        Coverage c;
        c.sub = sub;
        c.symbols = symbols;
        c.types = types;
        c.ref_exists = ref_exists;
        for (const SymbolUse& s : symbols) {
            if (std::get<2>(s) == "MISSING") c.missing.append(s);
            if (std::get<2>(s) == "STUB") c.stub.append(s);
            if (std::get<2>(s) == "REUSABLE") c.reusable.append(s);
        }
        results.append(c);
    }
    return results;
}

// ---------------------------------------------------------------------------
StrList _render_stub_markers(const StubMarkers& markers) {
    if (markers.empty()) return StrList();
    int64_t total = 0;
    for (const auto& kv : markers.items()) total += py::len(std::get<1>(kv));
    StrList L;
    L.append(str("---"));
    L.append(str(""));
    L.append(py::percent(str("## J2X-STUB markers -- the explicit placeholder surface (%d)"), total));
    L.append(str(""));
    L.append(str("Every no-op / default-return in the emitted stubs and the hand-written references is marked "
                 "`// J2X-STUB[tag]: ...` at the exact function. `grep -rn \"J2X-STUB\" .` is the full audit list; "
                 "grouped by tag here. A no-op is only safe if its effect is cosmetic -- verify the load-bearing "
                 "ones (the glPolygonOffset z-fight class of bug lived in an unmarked no-op)."));
    L.append(str(""));
    L.append(str("| Tag | Count | Example site |"));
    L.append(str("|---|---:|---|"));
    auto tags = markers.keys();
    std::stable_sort(tags.begin(), tags.end(), [&](const str& a, const str& b) { return py::len(markers.at(a)) > py::len(markers.at(b)); });
    for (const str& tag : tags) {
        const py::list<StubSite>& sites = markers.at(tag);
        str ex = py::percent(str("`%s:%d`"), std::get<0>(sites[0]), std::get<1>(sites[0]));
        L.append(py::percent(str("| `%s` | %d | %s -- %s |"), tag, py::len(sites), ex, std::get<2>(sites[0])));
    }
    L.append(str(""));
    return L;
}

str render_report(const py::list<Coverage>& results, const std::optional<str>& reference_dir, const std::optional<StubMarkers>& stub_markers,
                  const std::optional<console::xdk_scan::ScanResult>& sdk_caps) {
    bool have_ref = reference_dir && !reference_dir->empty() && py::os::path::isdir(*reference_dir);
    StrList L;
    auto add = [&](std::initializer_list<str> xs) {
        for (const str& x : xs) L.append(x);
    };
    add({str("# PLATFORM_COVERAGE.md -- platform-layer coverage & decisions"), str(""),
         str("Every platform subsystem the converted game touches, and what still has to be answered to"),
         str("make it run on real hardware. The converter emits **no-op stubs** for all of these; this"),
         str("report says, per symbol, whether the real implementation already exists in the Alpha"),
         str("`docs/platform/reference/` layer (**REUSABLE**), is only a placeholder (**STUB**), or is"),
         str("**MISSING** (new to this version -> new work)."), str("")});
    if (!have_ref) {
        add({str("> NOTE: the Alpha reference layer was not found, so nothing could be marked REUSABLE. "
                 "Pass its path (`--reference docs/platform/reference`) to classify against the real impls."),
             str("")});
    }

    // scoreboard
    add({str("## Scoreboard"), str(""), str("| Subsystem | Reference impl | REUSABLE | STUB | MISSING |"), str("|---|---|---:|---:|---:|")});
    for (const Coverage& r : results) {
        L.append(py::percent(str("| %s | %s | %d | %d | %d |"), r.sub.name.slice(0, r.sub.name.find(str("  --")) < 0 ? r.sub.name.size() : r.sub.name.find(str("  --"))).strip(),
                             r.ref_exists ? str("yes") : str("no"), py::len(r.reusable), py::len(r.stub), py::len(r.missing)));
    }
    L.append(str(""));
    int64_t total_missing = 0;
    for (const Coverage& r : results) total_missing += py::len(r.missing);
    L.append(py::percent(str("**%d symbol(s) are MISSING** (used by the game, absent from both the stub and the Alpha "
                             "reference) -- those are the concrete new work items, listed per subsystem below."),
                         total_missing));
    L.append(str(""));

    namespace pe = console::platform_equivalents;

    // --- Platform REQUIREMENTS -- AUTO-DETECTED from the installed SDK (xdk_scan), NOT hardcoded. Each is
    //     claimed ONLY because its XDK function was actually found in the scanned SDK. ---------------
    std::vector<console::xdk_scan::Detection> reqs, opps;
    if (sdk_caps) {
        for (const auto& r : sdk_caps->found) {
            if (r.kind == "requirement") reqs.push_back(r);
            if (r.kind == "opportunity") opps.push_back(r);
        }
    }
    if (sdk_caps && sdk_caps->include && !sdk_caps->include->empty()) {
        add({str("---"), str(""), str("## Xbox 360 platform requirements -- auto-detected from the installed SDK"), str(""),
             py::percent(str("Rules the console/XDK imposes that are NOT a single stub-swap. **Each row is here because its API "
                             "was found in the scanned SDK** (`%s`, %d functions) -- derived from the SDK, not from memory. Ignore "
                             "one and the feature silently breaks or the title fails cert."),
                         *sdk_caps->include, py::len(sdk_caps->funcs)),
             str(""), str("| Requirement | Detected in SDK | Severity | If ignored |"), str("|---|---|:-:|---|")});
        for (const auto& r : reqs) {
            str fam = r.func_count > 1 ? py::percent(str("  (+%d in family)"), r.func_count - 1) : str("");
            L.append(py::percent(str("| **%s** | `%s`%s -- %s | %s | %s |"), r.title, r.needs, fam, r.header, r.sev.value().upper(), r.why));
        }
        L.append(str(""));
    } else {
        add({str("---"), str(""), str("## Xbox 360 platform requirements"), str(""),
             str("> No SDK was scanned, so requirements could not be auto-detected. (They are derived from the SDK "
                 "headers, not hardcoded -- supply the SDK to populate this.)"),
             str("")});
    }

    // --- Xbox 360 XDK equivalents (the authoritative Java-symbol -> XDK-API map) -------------------
    add({str("---"), str(""), str("## Xbox 360 XDK equivalents -- the real API for every platform symbol"), str(""),
         str("For each Java/LWJGL platform type the port stubs, the exact XDK function to resolve it to "
             "(this same line is emitted in the stub header as `// [360] equivalent:`). "
             "**real** = the port implements it; **stub** = still a no-op but the API named is the drop-in; "
             "**dead** = no console equivalent."),
         str(""), str("| Subsystem | Java symbol | Xbox 360 XDK API | Lib | Status |"), str("|---|---|---|---|---|")});
    auto ord = [](const str& status) -> int64_t { return status == "real" ? 0 : status == "stub" ? 1 : status == "dead" ? 2 : 3; };
    std::vector<str> subs = pe::EQUIV.keys();
    auto sub_index = [&](const str& s) -> int64_t {
        for (size_t i = 0; i < subs.size(); ++i)
            if (subs[i] == s) return static_cast<int64_t>(i);
        throw py::ValueError(py::repr(s).utf8() + " is not in list");
    };
    std::vector<std::tuple<str, str, pe::Equiv>> entries;
    for (const auto& e : pe::all_entries()) entries.push_back(e);
    std::stable_sort(entries.begin(), entries.end(), [&](const auto& a, const auto& b) {
        return std::make_tuple(sub_index(std::get<0>(a)), ord(std::get<2>(a).status), std::get<1>(a)) <
               std::make_tuple(sub_index(std::get<0>(b)), ord(std::get<2>(b).status), std::get<1>(b));
    });
    for (const auto& ent : entries) {
        const str& subk = std::get<0>(ent);
        const str& name = std::get<1>(ent);
        const pe::Equiv& e = std::get<2>(ent);
        str badge = e.status == "real" ? str("REAL") : e.status == "stub" ? str("stub") : e.status == "dead" ? str("dead") : e.status;
        L.append(py::percent(str("| %s | `%s` | %s | %s | %s |"), subk, name, e.xdk, e.lib, badge));
    }
    L.append(str(""));

    // --- Untapped XDK capabilities -- AUTO-DETECTED in the scanned SDK (not hardcoded) --------------
    if (sdk_caps && !opps.empty()) {
        add({str("---"), str(""), str("## Untapped XDK capabilities -- 360-native wins, auto-detected in the SDK"), str(""),
             str("Found in the scanned SDK headers; not stubs to fill -- features/optimizations worth adopting."), str(""),
             str("| Feature | Detected in SDK | Effort | Why it helps |"), str("|---|---|:-:|---|")});
        for (const auto& o : opps) {
            L.append(py::percent(str("| **%s** | `%s` -- %s | %s | %s |"), o.title, o.needs, o.header, o.effort ? *o.effort : str("?"), o.why));
        }
        L.append(str(""));
    }

    for (const Coverage& r : results) {
        const Subsystem& sub = r.sub;
        if (r.symbols.empty() && r.types.empty() && sub.notes.empty()) continue;
        add({str("---"), str(""), py::percent(str("## %s"), sub.name), str("")});
        if (!sub.reference.empty()) {
            StrList refs;
            for (const str& n : sub.reference) refs.append(py::percent(str("%s.reference.*"), n));
            L.append(str("**Reference implementation:** ") + str("`") + py::join(str("`, `"), refs) + str("`"));
        } else {
            L.append(str("**Reference implementation:** none (platform-native only)"));
        }
        L.append(str(""));
        L.append(str("**Choose an implementation:**"));
        for (const str& opt : sub.options) L.append(py::percent(str("- %s"), opt));
        L.append(str(""));
        if (!sub.notes.empty()) {
            add({str("**Known considerations (verify these even when the symbols are all covered):**"), str("")});
            for (const str& note : sub.notes) L.append(py::percent(str("- \xE2\x9A\xA0\xEF\xB8\x8F %s"), note));
            L.append(str(""));
        }
        if (!r.missing.empty()) {
            add({py::percent(str("### MISSING -- new work for this version (%d)"), py::len(r.missing)), str(""), str("| Symbol | Used | Note |"), str("|---|---:|---|")});
            for (const SymbolUse& s : r.missing)
                L.append(py::percent(str("| `%s` | %d | not in the stub OR the Alpha reference -- implement it |"), std::get<0>(s), std::get<1>(s)));
            L.append(str(""));
        }
        if (!r.reusable.empty()) {
            add({py::percent(str("### REUSABLE -- already implemented in the Alpha reference (%d)"), py::len(r.reusable)), str(""),
                 str("These compile+work as soon as the reference file replaces the stub. Top by use:"), str(""), str("| Symbol | Used |"),
                 str("|---|---:|")});
            for (const SymbolUse& s : r.reusable.slice(0, 20)) L.append(py::percent(str("| `%s` | %d |"), std::get<0>(s), std::get<1>(s)));
            if (py::len(r.reusable) > 20) L.append(py::percent(str("| \xE2\x80\xA6 (%d more) | |"), py::len(r.reusable) - 20));
            L.append(str(""));
        }
        if (!r.stub.empty()) {
            add({py::percent(str("### STUB -- placeholder only, verify it is not load-bearing (%d)"), py::len(r.stub)), str(""),
                 str("A no-op here is fine if the effect is cosmetic, but silently wrong if it matters "
                     "(the glPolygonOffset z-fight class). Review the high-use ones:"),
                 str(""), str("| Symbol | Used |"), str("|---|---:|")});
            for (const SymbolUse& s : r.stub.slice(0, 15)) L.append(py::percent(str("| `%s` | %d |"), std::get<0>(s), std::get<1>(s)));
            L.append(str(""));
        }
        if (!r.types.empty()) {
            add({str("### Platform types referenced"), str(""), str("| Type | Used | In reference? |"), str("|---|---:|---|")});
            for (const TypeUse& t : r.types)
                L.append(py::percent(str("| `%s` | %d | %s |"), std::get<0>(t), std::get<1>(t), std::get<2>(t) ? str("yes") : str("no")));
            L.append(str(""));
        }
    }
    StrList tail = _render_stub_markers(stub_markers ? *stub_markers : StubMarkers());
    for (const str& x : tail) L.append(x);
    return join_lines(L) + str("\n");
}

std::tuple<str, py::list<Coverage>> write_report(const str& output_dir, const std::optional<str>& reference_dir, const std::optional<py::set<str>>& runtime_files,
                                                 const std::optional<str>& sdk_root_) {
    py::list<Coverage> results = analyze(output_dir, reference_dir, runtime_files);
    StubMarkers markers = scan_stub_markers(output_dir);
    // AUTO-DETECT platform requirements/capabilities from the installed SDK (not hardcoded). Best-effort:
    // if no SDK is resolvable, the report says so rather than inventing requirements.
    std::optional<console::xdk_scan::ScanResult> sdk_caps;
    try {
        std::optional<str> sdk_root = sdk_root_;
        if (!sdk_root || sdk_root->empty()) {
            std::optional<console::sdk::SdkInfo> r = console::sdk::detect(std::optional<str>(str("xbox360")));
            sdk_root = r ? std::optional<str>(r->root) : std::nullopt;
        }
        if (sdk_root && !sdk_root->empty()) sdk_caps = console::xdk_scan::detect(sdk_root);
    } catch (const std::exception&) {
        sdk_caps.reset();
    }
    str path = py::os::path::join(output_dir, str("PLATFORM_COVERAGE.md"));
    textutil::write_text(path, render_report(results, reference_dir, markers, sdk_caps));
    return std::make_tuple(path, results);
}

}  // namespace codebreaker::java::projects::minecraft::platform_coverage
