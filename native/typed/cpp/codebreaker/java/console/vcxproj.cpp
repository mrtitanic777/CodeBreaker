// codebreaker/java/console/vcxproj.cpp -- typed C++ port of codebreaker.java.console.vcxproj.

#include "codebreaker/java/console/vcxproj.hpp"

#include "codebreaker/java/projects/minecraft/achievements.hpp"
#include "json.hpp"
#include "value.hpp"

namespace codebreaker::java::console::vcxproj {

namespace achievements = ::codebreaker::java::projects::minecraft::achievements;

// Ordered (regex, filter) classification: FIRST match wins, so put SPECIFIC before GENERAL.
// Purely name-based -- the decompiled corpus has consistent class-name prefixes. Filters are NESTED
// (backslash = one Solution-Explorer sub-folder level) so big subsystems break into readable sub-folders
// instead of one giant flat list. EVERY file resolves to a filter (fallback 'Game (misc)') -- a file must
// NEVER land at the project root, which is why the pipeline regenerates this on every conversion.
// The (basename regex -> filter path) rules are game-specific and come from the active target
// profile. EVERY file must resolve to a filter (fallback 'Game (misc)'), never the project root.
// (The active profile at start is GENERIC: no rules.  See the header.)
py::list<std::tuple<py::str, py::str>> _FILTER_RULES = py::list<std::tuple<py::str, py::str>>{};

// Re-derive the .filters classification rules when the active target profile changes
// (profiles.on_change). Copied so an edition switch retargets the Solution-Explorer layout.
void _reseed(const py::list<std::tuple<py::str, py::str>>& vcxproj_filters) {
    py::list<std::tuple<py::str, py::str>> copy;
    for (const auto& rule : vcxproj_filters) copy.append(rule);
    _FILTER_RULES = copy;
}

py::str _classify(const py::str& basename, const py::dict<py::str, py::str>& parent_of) {
    for (const auto& [pat, filt] : _FILTER_RULES) {
        if (py::re::match(pat, basename)) {
            return filt;
        }
    }
    if (!parent_of.empty()) {
        py::set<py::str> seen;
        std::optional<py::str> cur = parent_of.contains(basename) ? std::optional<py::str>(parent_of.at(basename)) : std::nullopt;
        while (cur.has_value() && !cur->empty() && !seen.contains(*cur)) {
            seen.add(*cur);
            for (const auto& [pat, filt] : _FILTER_RULES) {
                if (py::re::match(pat, *cur)) {
                    return filt;
                }
            }
            cur = parent_of.contains(*cur) ? std::optional<py::str>(parent_of.at(*cur)) : std::nullopt;
        }
    }
    return py::str("Game (misc)");
}

const py::re::Pattern& _CLASS_DECL() {
    static const py::re::Pattern p = py::re::compile(py::str("^\\s*class\\s+(\\w+)\\s*:\\s*(?:public\\s+|private\\s+|protected\\s+)?(\\w+)"), py::re::M);
    return p;
}

// Scan the emitted .h headers for `class X : public Y` and return {X: Y} (primary base only).
// This recovers the converted codebase's real inheritance graph so filters follow it -- no naming
// convention required. Forward decls (`class Foo;`) and root classes (`class Foo {`) are skipped
// (no `:` base). Works standalone, since the headers are right there in the output directory.
py::dict<py::str, py::str> _build_parent_map(const py::str& proj_dir) {
    py::dict<py::str, py::str> parent_of;
    py::list<py::str> headers;
    try {
        for (const py::str& f : py::os::listdir(proj_dir)) {
            if (f.endswith(py::str(".h"))) {
                headers.append(f);
            }
        }
    } catch (const py::OSError&) {
        return parent_of;
    }
    for (const py::str& fn : headers) {
        py::str text;
        try {
            py::file fh = py::open(py::os::path::join(proj_dir, fn), py::str("r"), std::optional<py::str>(py::str("utf-8")), std::optional<py::str>(py::str("replace")));
            text = fh.read_text();
            fh.close();
        } catch (const py::OSError&) {
            continue;
        }
        for (const auto& [child, base] : _CLASS_DECL().findall_groups<py::str, py::str>(text)) {
            parent_of.setdefault(child, base);
        }
    }
    return parent_of;
}

py::str _guid(const py::str& name) {
    py::str h = py::hashlib::md5((py::str("j2ctool-filter-") + name).encode("utf-8", "strict")).hexdigest();   // stable per filter name
    return py::percent("{%s-%s-%s-%s-%s}", h.slice_to(8), h.slice(8, 12), h.slice(12, 16), h.slice(16, 20), h.slice(20, 32));
}

namespace {

// s.rsplit(ch, 1)[0]: everything before the last ch (all of s when it has none)
py::str before_last_(const py::str& s, const py::str& ch) {
    int64_t at = s.rfind(ch);
    return at < 0 ? s : s.slice_to(at);
}

// XML-escape filter/file names (& < >)
py::str esc_(const py::str& v) {
    return v.replace(py::str("&"), py::str("&amp;")).replace(py::str("<"), py::str("&lt;")).replace(py::str(">"), py::str("&gt;"));
}

}  // namespace

// Emit <proj_name>.vcxproj.filters grouping every .cpp/.h into subsystem virtual folders, so
// Solution Explorer shows folder icons (Blocks, Entities, Rendering, World & Generation, Network,
// GUI, Runtime, Platform, ...) instead of two flat lists. Deterministic (stable filter GUIDs).
std::tuple<py::str, int64_t> write_filters(const py::str& proj_dir, const py::str& proj_name,
                                           const py::list<py::str>& cpps, const py::list<py::str>& hs) {
    py::dict<py::str, py::str> parent_of = _build_parent_map(proj_dir);   // real inheritance graph of the output
    py::dict<py::str, py::str> file_filter;
    py::set<py::str> filters;
    py::list<py::str> all;
    for (const py::str& fn : cpps) all.append(fn);
    for (const py::str& fn : hs) all.append(fn);
    for (const py::str& fn : all) {
        py::str filt;
        if (fn.rfind(py::str("\\")) >= 0) {                              // subdir'd overlay file -> folder = its subdir path
            filt = before_last_(fn, py::str("\\"));
        } else {
            filt = _classify(before_last_(fn, py::str(".")), parent_of);
        }
        file_filter.set(fn, filt);
        py::list<py::str> parts = py::split(filt, py::str("\\"));          // register each nesting level
        for (int64_t i = 0; i < py::len(parts); i++) {
            py::list<py::str> head;
            for (int64_t j = 0; j <= i; j++) head.append(parts[j]);
            filters.add(py::join(py::str("\\"), head));
        }
    }

    py::list<py::str> L;
    L.append(py::str("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
    L.append(py::str("<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">"));
    L.append(py::str("  <ItemGroup>"));
    for (const py::str& filt : py::sorted(filters)) {
        L.append(py::str("    <Filter Include=\"") + esc_(filt) + py::str("\">"));
        L.append(py::str("      <UniqueIdentifier>") + _guid(filt) + py::str("</UniqueIdentifier>"));
        L.append(py::str("    </Filter>"));
    }
    L.append(py::str("  </ItemGroup>"));
    L.append(py::str("  <ItemGroup>"));
    for (const py::str& fn : cpps) {
        L.append(py::str("    <ClCompile Include=\"") + esc_(fn) + py::str("\"><Filter>") + esc_(file_filter.at(fn)) + py::str("</Filter></ClCompile>"));
    }
    L.append(py::str("  </ItemGroup>"));
    L.append(py::str("  <ItemGroup>"));
    for (const py::str& fn : hs) {
        L.append(py::str("    <ClInclude Include=\"") + esc_(fn) + py::str("\"><Filter>") + esc_(file_filter.at(fn)) + py::str("</Filter></ClInclude>"));
    }
    L.append(py::str("  </ItemGroup>"));
    L.append(py::str("</Project>"));
    py::str path = py::os::path::join(proj_dir, proj_name + py::str(".vcxproj.filters"));
    {
        py::file f = py::open(path, py::str("w"), std::optional<py::str>(py::str("utf-8")));
        f.write(py::join(py::str("\n"), L) + py::str("\n"));
        f.close();
    }
    return std::make_tuple(path, static_cast<int64_t>(py::len(filters)));
}

// No hardcoded project path -- callers pass the actual output/project dir. Fall back to CWD so a
// bare invocation operates on the current directory rather than a stale, non-existent path.

// The .vcxproj to operate on: <name>.vcxproj if given, else the single .vcxproj in the dir.
py::str _find_vcxproj(const py::str& proj_dir, const std::optional<py::str>& name) {
    if (name.has_value() && !name->empty()) {
        return py::os::path::join(proj_dir, *name + py::str(".vcxproj"));
    }
    py::list<py::str> cands;
    for (const py::str& f : py::os::listdir(proj_dir)) {
        if (f.endswith(py::str(".vcxproj"))) {
            cands.append(f);
        }
    }
    return !cands.empty() ? py::os::path::join(proj_dir, cands[0]) : py::os::path::join(proj_dir, py::str("MinecraftAlpha.vcxproj"));
}

namespace {

// type(x).__name__ of a parsed JSON value
const char* json_type_name_(const py::V& v) {
    if (v.is_none()) return "NoneType";
    if (v.is_bool()) return "bool";
    if (v.is_int()) return "int";
    if (v.is_float()) return "float";
    if (v.is_str()) return "str";
    if (v.is_list()) return "list";
    return "dict";
}

// JSONDecodeError's text: "<msg>: line L column C (char N)", N counted in characters
py::str decode_error_(const py::str& doc, const py::str& msg, int64_t pos) {
    const std::u32string& u = doc.u32();
    int64_t lineno = 1;
    int64_t last_nl = -1;
    for (int64_t i = 0; i < pos && i < static_cast<int64_t>(u.size()); i++) {
        if (u[static_cast<size_t>(i)] == U'\n') {
            lineno++;
            last_nl = i;
        }
    }
    return py::percent("%s: line %d column %d (char %d)", msg, lineno, pos - last_nl, pos);
}

// json.load of the text: py::json_parse reads it, and says "<msg> at char <byte offset>" where Python's
// JSONDecodeError says "<msg>: line L column C (char N)" (a BOM is refused first, as json.loads does)
py::V json_load_(const py::str& text) {
    if (!text.empty() && text.u32()[0] == U'﻿') {
        throw py::ValueError(decode_error_(text, py::str("Unexpected UTF-8 BOM (decode using utf-8-sig)"), 0).utf8());
    }
    try {
        return py::json_parse(text.utf8());
    } catch (const py::PyError& e) {
        const std::string what = e.what();
        const size_t at = what.rfind(" at char ");
        if (e.type != "ValueError" || at == std::string::npos) throw;
        const size_t byte_pos = static_cast<size_t>(std::stoull(what.substr(at + 9)));
        const std::string utf8 = text.utf8();
        int64_t pos = 0;
        for (size_t i = 0; i < byte_pos && i < utf8.size(); i++) {
            if ((static_cast<unsigned char>(utf8[i]) & 0xC0) != 0x80) pos++;
        }
        std::string msg = what.substr(0, at);
        if (msg == "Invalid control character") msg += " at";   // (Python's wording)
        throw py::ValueError(decode_error_(text, py::str(msg), pos).utf8());
    }
}

// bool(x) of a parsed JSON value
bool json_truthy_(const py::V& v) {
    if (v.is_none()) return false;
    if (v.is_bool()) return v.as_int() != 0;
    if (v.is_int()) return v.as_int() != 0;
    if (v.is_float()) return v.as_float() != 0.0;
    if (v.is_str()) return !v.str().empty();
    return v.size() != 0;
}

}  // namespace

// The project's self-issued Title ID from platform.config.json, as (canonical_8_hex or None, note).
// ONE source of truth: the XEX (<ImageXex><TitleID>, stamped here) and the XLAST/SPA achievement config
// are both keyed to it and must match, or achievement unlocks never bind to a profile.
std::tuple<std::optional<py::str>, py::str> _title_id_for(const py::str& proj_dir) {
    py::str cfg_path = py::os::path::join(proj_dir, py::str("platform.config.json"));
    if (!py::os::path::isfile(cfg_path)) {
        return std::make_tuple(std::nullopt, py::str(""));
    }
    py::V raw;
    try {
        py::file f = py::open(cfg_path, py::str("r"), std::optional<py::str>(py::str("utf-8")));
        py::str text = f.read_text();
        f.close();
        py::V cfg = json_load_(text);
        if (!cfg.is_dict()) {
            throw py::AttributeError(std::string("'") + json_type_name_(cfg) + "' object has no attribute 'get'");   // json.load(f).get(...) of a non-object
        }
        raw = cfg.get("title_id");
    } catch (const py::ValueError& e) {
        return std::make_tuple(std::nullopt, py::percent("title id: could not read platform.config.json (%s)", py::str(e.what())));
    } catch (const py::OSError& e) {
        return std::make_tuple(std::nullopt, py::percent("title id: could not read platform.config.json (%s)", py::str(e.what())));
    } catch (const py::PyError& e) {
        if (e.type != "ValueError") throw;
        return std::make_tuple(std::nullopt, py::percent("title id: could not read platform.config.json (%s)", py::str(e.what())));
    }
    if (!json_truthy_(raw)) {
        return std::make_tuple(std::nullopt, py::str("title id: UNSET in platform.config.json -- the XEX keeps 0x00000000, so "
                                                     "achievements cannot bind to a profile (set it, then re-run vcxproj)"));
    }
    // normalize_title_id(raw) is str(raw).strip().lower() against 1-8 hex digits: only a str or an int
    // can be one (the str() of a float, bool, list or dict never is)
    std::optional<py::str> tid;
    if (raw.is_str()) tid = achievements::normalize_title_id(py::str(raw.str()));
    else if (raw.is_int()) tid = achievements::normalize_title_id(py::str(std::to_string(raw.as_int())));
    if (!tid.has_value() || tid->empty()) {
        return std::make_tuple(std::nullopt, py::str("title id: ") + py::str(py::repr(raw)) + py::str(" in platform.config.json is not 1-8 hex digits -- ignored"));
    }
    return std::make_tuple(tid, py::str(""));
}

// Recursively collect .cpp/.h under proj_dir as RELATIVE (backslash) paths, skipping build/output dirs.
// FIXED BUG overlay-subdir-not-compiled: os.listdir only saw the project ROOT, so subdir'd overlay files -- a
// whole added subsystem like xuimenu/ (Ui/Gfx/MenuApp/...) -- were never listed in the .vcxproj and silently
// never compiled, defeating the overlays.py "ADDED .cpp are compiled" guarantee. Walk recursively instead.
std::tuple<py::list<py::str>, py::list<py::str>> _enum_sources(const py::str& proj_dir) {
    const py::set<py::str> SKIP = py::set<py::str>{py::str("debug"), py::str("release"), py::str("release_ltcg"), py::str("profile"), py::str("fastcap"), py::str("codeanalysis"), py::str("report"), py::str(".vs"), py::str("_decompiled"), py::str("x64"), py::str("obj"), py::str("bin"), py::str(".git"), py::str("ipch")};
    py::list<py::str> cpps, hs;
    for (const auto& [root, dirs, files] : py::os::walk(proj_dir)) {
        py::list<py::str> items;
        for (const py::str& d : dirs) {
            if (!SKIP.contains(d.lower())) {
                items.append(d);
            }
        }
        dirs.set_slice(py::none, py::none, items);
        for (const py::str& fn : files) {
            py::str rel = py::os::path::relpath(py::os::path::join(root, fn), proj_dir).replace(py::str("/"), py::str("\\"));
            if (fn.endswith(py::str(".cpp"))) {
                cpps.append(rel);
            } else if (fn.endswith(py::str(".h"))) {
                hs.append(rel);
            }
        }
    }
    return std::make_tuple(py::sorted(cpps), py::sorted(hs));
}

void run(const std::optional<py::str>& proj_dir_, const std::optional<py::str>& name, bool write_filters_too) {
    const py::str proj_dir = (proj_dir_.has_value() && !proj_dir_->empty()) ? *proj_dir_ : py::os::getcwd();   // the folder the command is run in (read at call time)
    const py::str vcx = _find_vcxproj(proj_dir, name);
    const py::str proj_name = std::get<0>(py::os::path::splitext(py::os::path::basename(vcx)));

    py::str text;
    {
        py::file f = py::open(vcx, py::str("r"), std::optional<py::str>(py::str("utf-8")));
        text = f.read_text();
        f.close();
    }
    const int64_t raw_bytes = py::len(text);

    {
        py::list<py::str> kept;
        for (const py::str& ln : py::split(text, py::str("\n"))) {
            if (ln.strip() != py::str("")) kept.append(ln);
        }
        text = py::join(py::str("\n"), kept);
    }

    std::optional<py::str> stdafx_block;
    {
        std::optional<py::re::Match> m = py::re::search(py::str("<ClCompile Include=\"stdafx\\.cpp\">.*?</ClCompile>"), text, py::re::S);
        if (m) stdafx_block = m->group(0);
    }

    text = py::re::sub(py::str("<ItemGroup>\\s*.*?</ItemGroup>"), [](const py::re::Match& mt) {
        py::str block = mt.group(0);
        return (block.find(py::str("<ClCompile")) >= 0 || block.find(py::str("<ClInclude")) >= 0) ? py::str("") : block;
    }, text, 0, py::re::S);

    auto [cpps, hs] = _enum_sources(proj_dir);

    auto cl_compile = [&](const py::str& fn) -> py::str {
        if (fn == py::str("stdafx.cpp") && stdafx_block.has_value() && !stdafx_block->empty()) {
            return py::str("    ") + *stdafx_block + py::str("\n");
        }
        if (fn.rfind(py::str("\\")) >= 0) {   // subdir'd overlay subsystem (e.g. xuimenu) -- carries its OWN stdafx.h, not the game PCH
            return py::str("    <ClCompile Include=\"") + fn + py::str("\">\n"
                           "      <PrecompiledHeader>NotUsing</PrecompiledHeader>\n"
                           "      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
                           "    </ClCompile>\n");
        }
        return py::str("    <ClCompile Include=\"") + fn + py::str("\" />\n");
    };

    py::str compile_grp = py::str("  <ItemGroup>\n");
    for (const py::str& f : cpps) compile_grp = compile_grp + cl_compile(f);
    compile_grp = compile_grp + py::str("  </ItemGroup>\n");
    py::str include_grp = py::str("  <ItemGroup>\n");
    for (const py::str& f : hs) include_grp = include_grp + py::str("    <ClInclude Include=\"") + f + py::str("\" />\n");
    include_grp = include_grp + py::str("  </ItemGroup>\n");

    // FIXED BUG assets-in-project-root: assets are emitted DIRECTLY into Debug\Content (= game:\Content next to
    // the Debug .xex) by the asset/audio/achievement tools -- so there is NO Content\ at the project root.
    // FIXED BUG release-ltcg-no-content: game: maps to $(OutDir), which DIFFERS per config (Debug\, Release\,
    // Release_LTCG\...), so a Release/LTCG build's game:\Content was EMPTY (assets only in Debug\Content). The
    // old StageContent copied $(ProjectDir)Content (never exists) -> inert. Emit a DeployContent target that
    // mirrors the canonical Debug\Content into any NON-Debug config's OutDir (Debug already has it). /D copies
    // only changed files (skips the big audio banks on rebuilds).
    // FIXED BUG release-ltcg-no-media: the XUI front-end loads from game:\Media (skin/*.xur scenes, fonts,
    // strings, panorama) which is ALSO emitted only into Debug\Media -> a Release/LTCG build's game:\Media was
    // EMPTY -> the menu held a solid McDie colour. Mirror Debug\Media the same way. Both Execs are now guarded by
    // a per-folder Exists() so a project with only Content (no XUI) deploys cleanly and one with Media gets both.
    text = py::re::sub(py::str("\\s*<Target Name=\"StageContent\".*?</Target>"), py::str(""), text, 0, py::re::S);
    text = py::re::sub(py::str("\\s*<Target Name=\"DeployContent\".*?</Target>"), py::str(""), text, 0, py::re::S);
    const py::str deploy_target = py::str(
        "  <Target Name=\"DeployContent\" AfterTargets=\"Build\" Condition=\"'$(Configuration)' != 'Debug'\">\n"
        "    <Exec Condition=\"Exists('$(ProjectDir)Debug\\Content')\" "
        "Command=\"xcopy /E /I /Y /Q /D &quot;$(ProjectDir)Debug\\Content&quot; &quot;$(OutDir)Content\\&quot;\" />\n"
        "    <Exec Condition=\"Exists('$(ProjectDir)Debug\\Media')\" "
        "Command=\"xcopy /E /I /Y /Q /D &quot;$(ProjectDir)Debug\\Media&quot; &quot;$(OutDir)Media\\&quot;\" />\n"
        "  </Target>\n");

    text = text.replace(py::str("</Project>"), compile_grp + include_grp + deploy_target + py::str("</Project>"));

    // TITLE ID -> <ImageXex><TitleID>: the ImageXex task stamps this into the XEX (it is passed to the
    // task, not to the visible imagexex command line). It MUST equal the id in the XLAST/SPA achievement
    // config or unlocks never bind to a profile, so both read the ONE value in platform.config.json.
    //
    // FIXED BUG titleid-one-config-only: the project template carries a single <ImageXex> block, and it
    // sits inside the Release_LTCG ItemDefinitionGroup -- so filling that one left EVERY other
    // configuration (Debug, Release, Profile, ...) building a 0x00000000 XEX, and a Release build silently
    // could not deliver achievements. ItemDefinitionGroup with NO Condition applies to all configurations,
    // so emit one of those (regenerated each run -> idempotent) AND fill any per-config block that exists.
    auto [tid, tid_msg_in] = _title_id_for(proj_dir);
    py::str tid_msg = tid_msg_in;
    if (tid.has_value() && !tid->empty()) {
        text = py::re::sub(py::str("\\s*<ItemDefinitionGroup Label=\"J2XTitleId\">.*?</ItemDefinitionGroup>"),
                           py::str(""), text, 0, py::re::S);                      // drop ours from a previous run
        auto [text2, n_existing] = py::re::subn(py::str("(<TitleID>)\\s*(</TitleID>)"),
                                                py::str("\\g<1>0x") + *tid + py::str("\\g<2>"), text);   // per-config blocks (LTCG)
        text = text2;
        const py::str shared = py::str("  <ItemDefinitionGroup Label=\"J2XTitleId\">\n"
                                       "    <ImageXex>\n"
                                       "      <TitleID>0x") + *tid + py::str("</TitleID>\n"
                                       "    </ImageXex>\n"
                                       "  </ItemDefinitionGroup>\n");
        std::optional<py::re::Match> m_idg = py::re::search(py::str("[ \\t]*<ItemDefinitionGroup"), text);
        if (m_idg) {
            text = text.slice_to(m_idg->start()) + shared + text.slice_from(m_idg->start());
        } else {                                                                  // no groups at all -> before </Project>
            text = text.replace(py::str("</Project>"), shared + py::str("</Project>"));
        }
        tid_msg = py::percent("title id: 0x%s (all configurations; %d per-config block(s) filled)", *tid, n_existing);
    }

    {
        py::file f = py::open(vcx, py::str("w"), std::optional<py::str>(py::str("utf-8")));
        f.write(text);
        f.close();
    }
    py::str msg = py::percent("vcxproj: %d bytes -> %d bytes; %d .cpp, %d .h listed%s",
                              raw_bytes, py::len(text), py::len(cpps), py::len(hs),
                              stdafx_block.has_value() && !stdafx_block->empty() ? py::str("") : py::str("  (WARNING: stdafx.cpp PCH block not found)"));
    if (!tid_msg.empty()) {
        msg = msg + py::str("\n") + tid_msg;
    }
    if (write_filters_too) {
        auto [fp, nfilt] = write_filters(proj_dir, proj_name, cpps, hs);
        msg = msg + py::str("\n") + py::percent("filters: %s (%d subsystem folders)", py::os::path::basename(fp), nfilt);
    }
    py::print(msg);
}

}  // namespace codebreaker::java::console::vcxproj
