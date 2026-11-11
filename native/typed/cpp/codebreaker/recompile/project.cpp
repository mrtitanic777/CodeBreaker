// codebreaker/recompile/project.cpp -- typed C++ from codebreaker.recompile.project (see project.hpp).
#include "codebreaker/recompile/project.hpp"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "codebreaker/base/converted.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/formats/xex_imports.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/gpu.hpp"
#include "codebreaker/recompile/title.hpp"
#include "codebreaker/recompile/x360project.hpp"
#include "py_glob.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::project {

namespace converted = codebreaker::base::converted;

#include "codebreaker/recompile/project_text.inc"

namespace {

py::str default_root_() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}

// "%08X" / "%X" of an address
std::string hex_(int64_t v, int width) {
    char b[32];
    if (width) std::snprintf(b, sizeof b, "%0*llX", width, static_cast<unsigned long long>(v));
    else std::snprintf(b, sizeof b, "%llX", static_cast<unsigned long long>(v));
    return b;
}

std::string replace_all_(std::string s, const std::string& a, const std::string& b) {
    size_t i = 0;
    while ((i = s.find(a, i)) != std::string::npos) {
        s.replace(i, a.size(), b);
        i += b.size();
    }
    return s;
}

// f"{n:,}"
std::string commas_(int64_t n) {
    std::string d = std::to_string(n < 0 ? -n : n), o;
    for (size_t i = 0; i < d.size(); ++i) {
        if (i && (d.size() - i) % 3 == 0) o += ',';
        o += d[i];
    }
    return (n < 0 ? "-" : "") + o;
}

std::string join_(const std::vector<std::string>& lines) {
    std::string o;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i) o += '\n';
        o += lines[i];
    }
    return o;
}

// shutil.copy2(src, dst): the file and its modification time
void copy2_(const py::str& src, const py::str& dst) {
    py::shutil::copyfile(src, dst);
    std::error_code ec;
    auto t = std::filesystem::last_write_time(py::fspath_(src), ec);
    if (!ec) std::filesystem::last_write_time(py::fspath_(dst), t, ec);
}

// all text files are written UTF-8
void w_(const py::str& path, const std::string& utf8) { _write_if_changed(path, py::bytes(utf8)); }

}  // namespace

py::str ROOT;
py::str _REXGLUE;

void set_root(const py::str& root) {
    ROOT = root;
    _REXGLUE = py::os::path::join(ROOT, py::str("native"), py::str("rexglue"));
}

namespace {
const bool root_set_ = (set_root(default_root_()), true);
}

py::str _clean_name(const py::str& module) {
    py::str base = std::get<0>(py::os::path::splitext(module));
    for (const char* suf : {"XENON", "_xenon", "Xenon"}) base = base.replace(py::str(suf), py::str(""));
    std::u32string kept;
    for (char32_t c : base.u32())
        if (py::str(std::u32string(1, c)).isalnum() || c == U'_') kept.push_back(c);
    py::str r = py::str(kept).strip(py::str("_"));
    return r.empty() ? py::str("title") : r;
}

py::list<py::list<py::str>> _shards(const py::list<py::str>& parts, int64_t target) {
    py::list<py::list<py::str>> out;
    py::list<py::str> cur;
    int64_t sz = 0;
    for (const py::str& p : parts) {
        cur.append(p);
        sz += py::len(p);
        if (sz >= target) {
            out.append(cur);
            cur = py::list<py::str>();
            sz = 0;
        }
    }
    if (!cur.empty()) out.append(cur);
    return out;
}

bool _write_if_changed(const py::str& path, const py::bytes& data) {
    try {
        py::file f = py::open(path, py::str("rb"));
        py::bytes have = f.read();
        f.close();
        if (have == data) return false;
    } catch (const py::OSError&) {
    }
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
    f.close();
    return true;
}

void _sync_tree(const py::str& src, const py::str& dst,
                const std::function<py::set<py::str>(const py::str&, const py::list<py::str>&)>& ignore) {
    py::set<py::str> keep;
    py::os::walk_t walker(src);
    for (auto it = walker.begin(); it != walker.end(); ++it) {
        const py::str& dp = std::get<0>(it.cur);
        py::list<py::str> dirs = std::get<1>(it.cur);    // (shared with the walk: what is left in it is gone into)
        py::list<py::str> files = std::get<2>(it.cur);
        py::list<py::str> all;
        for (const py::str& d : dirs) all.append(d);
        for (const py::str& f : files) all.append(f);
        py::set<py::str> drop = ignore(dp, all);
        py::list<py::str> kept_dirs;
        for (const py::str& d : dirs)
            if (!drop.contains(d)) kept_dirs.append(d);
        dirs.clear();
        for (const py::str& d : kept_dirs) dirs.append(d);
        py::str rel = py::os::path::relpath(dp, src);
        py::os::makedirs(py::os::path::join(dst, rel), 0777, true);
        for (const py::str& f : files) {
            if (drop.contains(f)) continue;
            py::str target = py::os::path::normpath(py::os::path::join(dst, rel, f));
            keep.add(target);
            py::file fh = py::open(py::os::path::join(dp, f), py::str("rb"));
            py::bytes data = fh.read();
            fh.close();
            _write_if_changed(target, data);
        }
    }
    py::os::walk_t dst_walker(dst);
    for (auto it = dst_walker.begin(); it != dst_walker.end(); ++it) {
        const py::str& dp = std::get<0>(it.cur);
        py::list<py::str> files = std::get<2>(it.cur);
        for (const py::str& f : files) {
            py::str path = py::os::path::normpath(py::os::path::join(dp, f));
            if (!keep.contains(path)) py::os::remove(path);
        }
    }
}

bool _copy_rex_dxbc(const py::str& dst) {
    py::str src = py::os::path::join(_REXGLUE, py::str("dxbc"));
    if (!py::os::path::isdir(src) || !py::os::path::exists(py::os::path::join(src, py::str("CMakeLists.txt")))) return false;
    const py::set<py::str> skip_dirs{py::str("obj")};
    const py::set<py::str> skip_files{py::str("build_dxbc.sh"), py::str("linktest.cpp"), py::str("translate_probe.cpp"),
                                      py::str("client_test.cpp"), py::str("wired_main.cpp"), py::str("librex_dxbc.a")};
    auto ignore = [&](const py::str&, const py::list<py::str>& names) {
        py::set<py::str> drop;
        for (const py::str& n : names) {
            if (skip_dirs.contains(n) || skip_files.contains(n)) drop.add(n);
            else if (n.endswith(py::str(".a")) || n.endswith(py::str(".o")) || n.endswith(py::str(".exe"))) drop.add(n);
        }
        return drop;
    };
    _sync_tree(src, dst, ignore);
    return true;
}

bool _copy_rex_kernel(const py::str& dst) {
    py::str src = py::os::path::join(_REXGLUE, py::str("kernel"));
    if (!py::os::path::isdir(src) || !py::os::path::exists(py::os::path::join(src, py::str("CMakeLists.txt")))) return false;
    const py::set<py::str> skip{py::str("obj"), py::str("build_kernel.sh"), py::str("sweep.sh"), py::str("librex_kernel.a")};
    auto ignore = [&](const py::str&, const py::list<py::str>& names) {
        py::set<py::str> drop;
        for (const py::str& n : names)
            if (skip.contains(n) || n.endswith(py::str(".a")) || n.endswith(py::str(".o")) || n.endswith(py::str(".exe"))) drop.add(n);
        return drop;
    };
    _sync_tree(src, dst, ignore);
    return true;
}

Scaffolded scaffold(const py::str& xex_path, const py::str& out_dir, const std::optional<int64_t>& jobs) {
    namespace xex = codebreaker::formats::xex;
    auto [img, image_base, entry, sections] = disassemble::image(xex_path);
    (void)sections;
    bool is_xex = xex::is_xex(xex_path);
    std::optional<xex::Header> h;
    if (is_xex) h = xex::header(xex_path);
    py::str name = _clean_name(h && h->name && !h->name->empty() ? *h->name : py::os::path::basename(xex_path));
    // the import stubs get host functions, not recompiled bodies (the loader replaces their bytes)
    py::list<py::str> libraries;
    if (h && !h->imports.empty()) libraries = h->imports;
    else libraries = py::list<py::str>{py::str("xboxkrnl.exe"), py::str("xam.xex")};
    Thunks thunks;                                // {guest address: (library, ordinal)}
    py::ref<formats::powerpc_image::Image> image = formats::powerpc_image::read(is_xex ? xex::basefile(xex_path) : xex_path);
    for (const auto& kv : image->imports.items()) {
        int64_t addr = std::get<0>(kv);
        const py::str& symbol = std::get<1>(kv);
        py::list<py::str> bits = py::split(symbol.slice_from(6), py::str("_ord_"));
        if (bits.size() != 2) throw py::ValueError(bits.size() < 2 ? "not enough values to unpack (expected 2, got " + std::to_string(bits.size()) + ")"
                                                                   : "too many values to unpack (expected 2)");
        int64_t lib = py::int_parse(bits[0], 10), ordinal = py::int_parse(bits[1], 10);
        thunks.set(addr, std::make_tuple(lib < libraries.size() ? libraries[lib] : py::str("library") + py::to_str(lib), ordinal));
    }
    py::set<int64_t> skip;
    for (const auto& kv : thunks.items()) skip.add(std::get<0>(kv));
    auto [body, mapping, agg] = powerpc::recompile_image(xex_path, std::nullopt, jobs, skip);
    (void)body;
    (void)mapping;

    py::str inc = py::os::path::join(out_dir, py::str("include"));
    py::str src = py::os::path::join(out_dir, py::str("src"));
    py::str assets = py::os::path::join(out_dir, py::str("assets"));
    for (const py::str& d : {inc, src, assets}) py::os::makedirs(d, 0777, true);

    // --- include/ : the runtime header, the shared declarations, the config ---
    w_(py::os::path::join(inc, py::str("rex_ppc_min.h")), powerpc::runtime_header().utf8());
    w_(py::os::path::join(inc, name + py::str("_config.h")), _config_h(name, image_base, entry, py::len(img)).utf8());
    w_(py::os::path::join(inc, name + py::str(".h")), _recomp_header(name, agg.decls).utf8());
    w_(py::os::path::join(inc, py::str("rex_host.h")), _host_h(name).utf8());   // guest heap + host helpers

    // --- src/ : the recompiled functions, split into many translation units ---
    py::list<py::list<py::str>> shards = _shards(agg.parts);
    int64_t i = 0;
    for (const py::list<py::str>& group : shards) {
        std::string text = "#include \"" + name.utf8() + ".h\"\n\n";
        bool first = true;
        for (const py::str& p : group) {
            if (!first) text += '\n';
            text += p.utf8();
            first = false;
        }
        char idx[16];
        std::snprintf(idx, sizeof idx, "%04lld", static_cast<long long>(i));
        w_(py::os::path::join(src, name + py::str("_") + py::str(idx) + py::str(".cpp")), text);
        ++i;
    }
    // the import shims go in the table too: the CRT calls some imports through pointers
    // (TlsGetValue handing back KeTlsGetValue's thunk, then bctrl to it)
    {
        py::set<int64_t> all_chosen, declared;
        for (int64_t a : agg.chosen) all_chosen.add(a);
        for (int64_t a : agg.decls) declared.add(a);
        py::list<int64_t> chosen, undeclared;
        for (const auto& kv : thunks.items()) all_chosen.add(std::get<0>(kv));
        std::vector<int64_t> c(all_chosen.begin(), all_chosen.end()), u;
        for (const auto& kv : thunks.items())
            if (!declared.contains(std::get<0>(kv))) u.push_back(std::get<0>(kv));
        std::sort(c.begin(), c.end());
        std::sort(u.begin(), u.end());
        for (int64_t a : c) chosen.append(a);
        for (int64_t a : u) undeclared.append(a);
        w_(py::os::path::join(src, name + py::str("_init.cpp")), _init_cpp(name, chosen, undeclared).utf8());
    }
    w_(py::os::path::join(src, py::str("rex_runtime.cpp")), _runtime_cpp(name, entry).utf8());
    // the host-shim layer: one typed stub per imported kernel/XAM function
    w_(py::os::path::join(src, name + py::str("_imports.cpp")), _imports_cpp(name, thunks).utf8());

    // --- graphics/ : the native D3D12 GPU subsystem (gpu.py) ---
    py::os::makedirs(py::os::path::join(out_dir, py::str("graphics")), 0777, true);
    for (const auto& kv : gpu::gpu_files(name).items()) w_(py::os::path::join(out_dir, std::get<0>(kv)), std::get<1>(kv).utf8());
    w_(py::os::path::join(out_dir, py::str("GPU_DESIGN.md")), gpu::design_md(name).utf8());
    // the DXBC shader translator (graphics/rex_dxbc/): with it there, the ShaderTranslator's
    // __has_include finds it and shaders are translated to DXBC
    _copy_rex_dxbc(py::os::path::join(out_dir, py::str("graphics"), py::str("rex_dxbc")));
    // the kernel core (kernel/rex_kernel/): the recompiled code bridges onto its PPCContext
    // and kernel (rex_ppc_min.h finds it by itself)
    _copy_rex_kernel(py::os::path::join(out_dir, py::str("kernel"), py::str("rex_kernel")));

    // --- assets/ : the image (for a standalone boot) and the .xex (the kernel core decrypts,
    // decompresses and loads it itself at boot) ---
    {
        py::file f = py::open(py::os::path::join(assets, name + py::str(".bin")), py::str("wb"));
        f.write(img);
        f.close();
    }
    try {
        if (is_xex) copy2_(xex_path, py::os::path::join(assets, name + py::str(".xex")));
    } catch (const py::Exception&) {
    }

    // --- build glue, launcher, docs ---
    w_(py::os::path::join(out_dir, py::str("CMakeLists.txt")), _cmakelists(name).utf8());
    w_(py::os::path::join(out_dir, py::str("Play-") + name + py::str(".bat")), _launcher_bat(name).utf8());
    w_(py::os::path::join(out_dir, py::str("README.md")), _readme(name, agg, image_base, entry, shards.size()).utf8());

    Scaffolded r;
    r.agg = agg;
    r.name = name;
    r.dir = out_dir;
    r.shards = shards.size();
    r.imports = thunks.size();
    return r;
}

py::str _config_h(const py::str& name, int64_t image_base, int64_t entry, int64_t image_size) {
    std::string n = name.utf8();
    return py::str("// " + n + "_config.h - generated by CodeBreaker\n#pragma once\n#include <cstdint>\n#define PPC_IMAGE_BASE  0x" + hex_(image_base, 8) +
                   "ull\n#define PPC_ENTRY_POINT 0x" + hex_(entry, 8) + "u\n#define PPC_IMAGE_SIZE  0x" + hex_(image_size, 0) +
                   "ull\n#define PPC_IMAGE_ASSET \"" + n + ".bin\"\n#define PPC_XEX_ASSET   \"" + n +
                   ".xex\"   // raw XEX (RexGlue runtime loads it at boot)\n");
}

py::str _recomp_header(const py::str& name, const py::list<int64_t>& decls) {
    std::string n = name.utf8();
    std::vector<std::string> lines = {"// " + n + ".h - generated by CodeBreaker.",
                                      "// Shared declarations for the recompiled title: the runtime, the",
                                      "// config, and a forward declaration of every recompiled function so",
                                      "// the src/*.cpp translation units can call across each other.",
                                      "#pragma once",
                                      "#include \"" + n + "_config.h\"",   // config first: the bridge's context.h may want it
                                      "#include \"rex_ppc_min.h\"",
                                      "",
                                      "// --- every recompiled guest function (defined across src/*.cpp) ---"};
    for (int64_t a : decls) lines.push_back("PPC_FUNC(sub_" + hex_(a, 8) + ");");
    for (const char* l : {"",
                          "// guest address -> recompiled function, for indirect (bctr) calls.",
                          "// In bridge mode the vendored RexGlue context.h already provides this",
                          "// (with the same {guest, host} field layout), so we only define it",
                          "// ourselves for the standalone build.",
                          "#ifndef REX_PPC_BRIDGE",
                          "struct PPCFuncMapping { size_t guest; PPCFunc* host; };",
                          "extern PPCFuncMapping PPCFuncMappings[];",
                          "#endif",
                          ""})
        lines.push_back(l);
    return py::str(join_(lines));
}

py::str _init_cpp(const py::str& name, const py::list<int64_t>& chosen, const py::list<int64_t>& undeclared) {
    std::vector<std::string> lines = {"#include \"" + name.utf8() + ".h\"", ""};
    if (!undeclared.empty()) {   // import shims no recompiled code calls directly
        for (int64_t va : undeclared) lines.push_back("PPC_FUNC(sub_" + hex_(va, 8) + ");");
        lines.push_back("");
    }
    lines.push_back("// The function-pointer table: guest address -> recompiled function.");
    lines.push_back("PPCFuncMapping PPCFuncMappings[] = {");
    for (int64_t va : chosen) lines.push_back("\t{ 0x" + hex_(va, 8) + ", sub_" + hex_(va, 8) + " },");
    lines.push_back("\t{ 0, nullptr },");
    lines.push_back("};");
    lines.push_back("");
    return py::str(join_(lines));
}

const py::dict<py::str, py::list<py::str>>& HOST_IMPL() {
    static const py::dict<py::str, py::list<py::str>> table = [] {
        py::dict<py::str, py::list<py::str>> d;
        fill_host_impl(d);
        return d;
    }();
    return table;
}

py::str _host_h(const py::str& name) { return py::str(replace_all_(HOST_H_T, "@name@", name.utf8())); }

std::optional<RexExports> _REX_EXPORTS;

const RexExports& _rex_exports() {
    if (_REX_EXPORTS) return *_REX_EXPORTS;
    py::str root = py::os::path::join(_REXGLUE, py::str("kernel"));
    RexExports r;
    if (py::os::path::isdir(root)) {
        py::re::Pattern exp_re = py::re::compile("\\s*XE_EXPORT\\(\\s*(\\w+)\\s*,\\s*(0x[0-9A-Fa-f]+)\\s*,\\s*(\\w+)");
        for (const py::str& inc : py::glob::glob(py::os::path::join(root, py::str("src"), py::str("kernel"), py::str("*"), py::str("export_table.inc")))) {
            py::file f = py::open(inc, py::str("r"), py::str("utf-8"), py::str("replace"));
            for (const py::str& line : f.readlines_text()) {
                auto m = exp_re.match(line);
                if (m) r.ord2name.set(std::make_tuple(m->group(1).lower(), py::int_parse(m->group(2), 16)), m->group(3));
            }
            f.close();
        }
        py::re::Pattern imp_re = py::re::compile("EXPORT\\(__imp__(\\w+)");
        py::re::Pattern left_out = py::re::compile(
            "graphics[/\\\\](command_processor|graphics_system|trace_writer)|"
            "xboxkrnl[/\\\\](cert_monitor|debug_monitor|xboxkrnl_video|xboxkrnl_audio)|"
            "xam[/\\\\](xam_input|xam_ui|xam_nui)");   // (its CMakeLists)
        for (const py::str& cpp : py::glob::glob(py::os::path::join(root, py::str("src"), py::str("kernel"), py::str("**"), py::str("*.cpp")), true)) {
            if (left_out.search(cpp)) continue;
            py::str txt;
            try {
                py::file f = py::open(cpp, py::str("r"), py::str("utf-8"), py::str("replace"));
                txt = f.read_text();
                f.close();
            } catch (const py::Exception&) {
                continue;
            }
            for (const py::str& n : imp_re.findall(txt)) r.implemented.add(n);
        }
    }
    _REX_EXPORTS = r;
    return *_REX_EXPORTS;
}

py::str _imports_cpp(const py::str& name, const Thunks& thunks) {
    // knowledge.table("xbox360", "ordinal_names") (console_ordinals.json, through the knowledge base)
    // (knowledge.table records that the kind was read, for a tracked step; the dependency recorder, knowledge/reads, is not
    // linked here: nothing is tracking when this runs from the command line)
    py::dict<py::str, py::dict<int64_t, py::str>> ordinal_names = formats::xex_imports::_names();

    struct Rec {
        int64_t addr, ordv;
        py::str api;
    };
    std::map<std::string, std::vector<Rec>> by_lib;     // (sorted(by_lib): by code point, which is UTF-8 byte order)
    for (const auto& kv : thunks.items()) {
        int64_t addr = std::get<0>(kv);
        const py::str& lib = std::get<0>(std::get<1>(kv));
        int64_t ordv = std::get<1>(std::get<1>(kv));
        py::str api;
        auto names = ordinal_names.get(lib.lower());
        if (names && names->contains(ordv)) api = names->at(ordv);
        by_lib[lib.utf8()].push_back({addr, ordv, api});
    }

    // a few Vd* imports are the guest<->GPU seam -> route them to the native GPU
    // subsystem (graphics/) instead of a generic stub.
    py::dict<py::str, std::string> vd_routes;
    vd_routes.set(py::str("VdInitializeRingBuffer"), "xe_gpu::gpu().InitializeRingBuffer(ctx.r3.u32, ctx.r4.u32);");
    vd_routes.set(py::str("VdEnableRingBufferRPtrWriteBack"), "xe_gpu::gpu().EnableReadPointerWriteBack(ctx.r3.u32, ctx.r4.u32);");
    vd_routes.set(py::str("VdSwap"), "xe_gpu::gpu().Swap(ctx.r3.u32, 1280, 720); ctx.r3.s64 = 0;");
    vd_routes.set(py::str("VdQueryVideoFlags"), "ctx.r3.u64 = 0x00000007;   // widescreen + progressive (bootstrap)");

    const RexExports& rex = _rex_exports();

    auto rexname = [&](const py::str& lib, int64_t ordv) -> std::optional<py::str> {
        py::list<py::str> bits = py::split(lib, py::str("."));
        auto key = std::make_tuple(bits[0].lower(), ordv);
        return rex.ord2name.get(key);
    };

    std::set<std::string> imp_externs;           // __imp__<name> exports we call directly
    std::vector<std::string> body;
    int64_t n_rex = 0, n_host = 0, n_vd = 0, n_stub = 0;
    for (auto& entry : by_lib) {
        py::str lib(entry.first);
        std::vector<Rec> recs = entry.second;
        std::sort(recs.begin(), recs.end(), [](const Rec& a, const Rec& b) {
            if (a.addr != b.addr) return a.addr < b.addr;
            if (a.ordv != b.ordv) return a.ordv < b.ordv;
            return a.api < b.api;
        });
        std::string L = lib.utf8();
        body.push_back("// ---- " + L + "  (" + std::to_string(recs.size()) + " functions) ----");
        for (const Rec& rec : recs) {
            std::optional<py::str> rn = rexname(lib, rec.ordv);
            std::string label = !rec.api.empty() ? rec.api.utf8() : (rn && !rn->empty() ? rn->utf8() : "ordinal " + std::to_string(rec.ordv));
            std::string addr = hex_(rec.addr, 8);
            body.push_back("PPC_FUNC(sub_" + addr + ") {   // " + L + ": " + label);
            body.push_back("    REX_TRACE_IN(\"" + label + "\");");
            if (vd_routes.contains(rec.api)) {                       // (1) GPU seam -> our xe_gpu
                body.push_back("    " + vd_routes.at(rec.api));
                body.push_back("    (void)ctx; (void)base;");
                ++n_vd;
            } else if (rn && !rn->empty() && rex.implemented.contains(*rn)) {   // (2) real RexGlue kernel export
                imp_externs.insert(rn->utf8());
                body.push_back("    __imp__" + rn->utf8() + "(ctx, base);");
                ++n_rex;
            } else if (HOST_IMPL().contains(rec.api)) {              // (3) our own host impl
                for (const py::str& stmt : HOST_IMPL().at(rec.api)) body.push_back("    " + stmt.utf8());
                body.push_back("    (void)ctx; (void)base;");
                ++n_host;
            } else {                                                 // (4) log-once stub
                body.push_back("    static bool once = false;");
                body.push_back("    if (!once) { once = true; std::fprintf(stderr, \"[import] TODO " + L + ": " + label + "  (sub_" + addr + ")\\n\"); }");
                body.push_back("    (void)ctx; (void)base;");
                ++n_stub;
            }
            body.push_back("    REX_TRACE_OUT(\"" + label + "\");");
            body.push_back("}");
        }
        body.push_back("");
    }

    std::string pre = IMPORTS_PRELUDE_T;
    pre = replace_all_(pre, "@name@", name.utf8());
    pre = replace_all_(pre, "@N@", std::to_string(thunks.size()));
    pre = replace_all_(pre, "@REX@", std::to_string(n_rex));
    pre = replace_all_(pre, "@VD@", std::to_string(n_vd));
    pre = replace_all_(pre, "@HOST@", std::to_string(n_host));
    pre = replace_all_(pre, "@STUB@", std::to_string(n_stub));
    std::vector<std::string> lines = {pre, ""};
    if (!imp_externs.empty()) {
        lines.push_back("// Real RexGlue kernel exports (linked from kernel/rex_kernel). Calling");
        lines.push_back("// __imp__<name> runs the vendored kernel's actual implementation.");
        std::vector<std::string> sorted_names(imp_externs.begin(), imp_externs.end());
        // (sorted(set of str): by code point, which is UTF-8 byte order)
        std::sort(sorted_names.begin(), sorted_names.end());
        for (const std::string& nm : sorted_names) lines.push_back("extern \"C\" PPC_FUNC(__imp__" + nm + ");");
        lines.push_back("");
    }
    for (const std::string& b : body) lines.push_back(b);
    return py::str(join_(lines));
}

py::str _runtime_cpp(const py::str& name, int64_t) { return py::str(replace_all_(RUNTIME_CPP_T, "@name@", name.utf8())); }

py::str _cmakelists(const py::str& name) {
    std::string t = replace_all_(CMAKELISTS_T, "@NAME@", name.upper().utf8());
    return py::str(replace_all_(t, "@name@", name.utf8()));
}

py::str _launcher_bat(const py::str& name) { return py::str(replace_all_(LAUNCHER_BAT_T, "@name@", name.utf8())); }

py::str _readme(const py::str& name, const powerpc::Agg& agg, int64_t image_base, int64_t entry, int64_t shards) {
    double pct = 100.0 * static_cast<double>(agg.translated) / static_cast<double>(std::max<int64_t>(1, agg.total));
    char pb[64];
    std::snprintf(pb, sizeof pb, "%.1f", pct);
    std::string t = README_T;
    t = replace_all_(t, "@name@", name.utf8());
    t = replace_all_(t, "@IB@", hex_(image_base, 8));
    t = replace_all_(t, "@ENTRY@", hex_(entry, 8));
    t = replace_all_(t, "@FUNCS@", commas_(agg.funcs));
    t = replace_all_(t, "@TRANS@", commas_(agg.translated));
    t = replace_all_(t, "@TOTAL@", commas_(agg.total));
    t = replace_all_(t, "@PCT@", pb);
    t = replace_all_(t, "@SHARDS@", std::to_string(shards));
    return py::str(t);
}

namespace {

// argv[argv.index(flag) + 1]: None when the flag is not there; IndexError when it is the last argument
std::optional<py::str> opt_(const py::list<py::str>& argv, const char* flag) {
    py::str f(flag);
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == f) {
            if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
            return argv[i + 1];
        }
    }
    return std::nullopt;
}

bool has_(const py::list<py::str>& argv, const char* flag) { return argv.contains(py::str(flag)); }

void say_(const std::string& s) { py::print(py::str(s)); }

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0].startswith(py::str("-"))) {
        say_(MAIN_DOC);
        return 2;
    }
    std::optional<py::str> out = opt_(argv, "-o");
    py::str runtime = opt_(argv, "--runtime").value_or(py::str("x360"));
    if (runtime == py::str("x360")) {
        std::optional<py::str> files = opt_(argv, "--title-files");
        std::optional<py::str> name = opt_(argv, "--name");
        py::str xex_path = argv[0];
        std::optional<title::Title> found;
        bool is_xex_file = false;
        if (py::os::path::isfile(xex_path)) {
            py::file f = py::open(xex_path, py::str("rb"));
            is_xex_file = f.read(4) == py::bytes("XEX2");
            f.close();
        }
        if (!is_xex_file) {
            // a package, disc image, Games-on-Demand folder or folder of files: its default.xex, and its files
            // unpacked into the project's assets/ (once)
            found = title::locate(xex_path);
            if (!found) {
                say_(xex_path.utf8() + ": no Xbox 360 title found (a default.xex in a package, disc image or folder)");
                return 1;
            }
            if (!name || name->empty()) name = found->name;
            py::str staging = py::os::path::join(converted::root(), py::str("_unpacking"));
            py::os::makedirs(staging, 0777, true);
            xex_path = py::os::path::join(staging, py::str("default.xex"));
            py::file f = py::open(xex_path, py::str("wb"));
            f.write(found->read_xex());
            f.close();
        }
        std::optional<converted::Project> proj;
        if (!out) proj = converted::project(xex_path, name);
        if (found) {
            py::str target = py::os::path::join(proj ? proj->root : *out, py::str("assets"));
            int64_t n = found->unpack(target);
            say_(found->name->utf8() + ": " +
                 (n ? std::to_string(n) + " files unpacked from " + found->source.utf8() : std::string("files unpacked before")));
        }
        std::optional<py::str> scaffold_name = name;
        if (!scaffold_name || scaffold_name->empty()) scaffold_name = proj ? std::optional<py::str>(proj->name()) : std::nullopt;
        x360project::ScaffoldInfo info = x360project::scaffold(xex_path, proj ? proj->root : *out, scaffold_name, files);
        if (proj) {
            proj->record("recompile-project",
                         py::list<py::str>{py::os::path::join(proj->root, py::str("CMakeLists.txt")), proj->src, proj->assets,
                                           py::os::path::join(proj->reports, py::str("recompile.log"))},
                         py::str(std::to_string(info.functions) + " functions recompiled to C++ (XenonRecomp) for CodeBreaker's Xbox 360 runtime.\n"
                                 "Build: Build-" + info.name.utf8() + ".bat   Play: Play-" + info.name.utf8() + ".bat (keyboard or controller)"));
        }
        say_(info.name.utf8() + ": " + std::to_string(info.functions) + " functions, " + std::to_string(info.switches) + " switch tables, " +
             std::to_string(info.unimplemented) + " unimplemented instructions");
        say_("  -> " + info.dir.utf8());
        say_("  build: " + py::os::path::join(info.dir, py::str("Build-") + info.name + py::str(".bat")).utf8());
        say_("  play:  " + py::os::path::join(info.dir, py::str("Play-") + info.name + py::str(".bat")).utf8());
        if (has_(argv, "--build") || has_(argv, "--check")) {
            auto [ok, lines] = x360project::build(info.dir, info.name);
            say_(std::string("build: ") + (ok ? "ok" : "FAILED"));
            if (!ok) {
                std::vector<std::string> shown;
                for (const py::str& line : lines) shown.push_back("  " + line.slice(0, 300).utf8());
                say_(join_(shown));
                return 1;
            }
        }
        if (has_(argv, "--check")) {
            int64_t i = 0;
            for (; i < argv.size(); ++i)
                if (argv[i] == py::str("--check")) break;
            int64_t seconds = (i + 1 < argv.size() && argv[i + 1].isdecimal()) ? py::int_parse(argv[i + 1], 10) : 60;
            x360project::CheckResult r = x360project::check(info.dir, info.name, seconds);
            say_("check (" + std::to_string(seconds) + " s headless): " + std::to_string(r.frames) + " frames presented");
            std::vector<std::pair<py::str, int64_t>> imps;
            for (const auto& kv : r.unimplemented.items()) imps.emplace_back(std::get<0>(kv), std::get<1>(kv));
            std::stable_sort(imps.begin(), imps.end(), [](const auto& a, const auto& b) { return -a.second < -b.second; });
            for (const auto& kv : imps) say_("  unimplemented import " + kv.first.utf8() + " (" + std::to_string(kv.second) + " calls)");
            if (!r.fatal.empty()) say_("  stopped: " + r.fatal.slice(0, 400).utf8());
            say_("  log: " + r.log.utf8());
        }
        return 0;
    }
    if (!out) {
        say_(MAIN_DOC);
        return 2;
    }
    std::optional<int64_t> jobs;
    if (auto j = opt_(argv, "-j")) jobs = py::int_parse(*j, 10);
    Scaffolded agg = scaffold(argv[0], *out, jobs);
    double pct = 100.0 * static_cast<double>(agg.agg.translated) / static_cast<double>(std::max<int64_t>(1, agg.agg.total));
    char pb[64];
    std::snprintf(pb, sizeof pb, "%.1f", pct);
    say_(agg.name.utf8() + ": " + std::to_string(agg.agg.funcs) + " functions in " + std::to_string(agg.shards) + " source files, " + pb + "% of " +
         std::to_string(agg.agg.total) + " instructions translated, " + std::to_string(agg.imports) + " imports");
    say_("  -> " + agg.dir.utf8());
    std::string d = agg.dir.utf8();
    say_("  build: cmake -S \"" + d + "\" -B \"" + d + "/build\" && cmake --build \"" + d + "/build\"");
    return 0;
}

}  // namespace codebreaker::recompile::project
