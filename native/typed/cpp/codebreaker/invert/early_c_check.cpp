// codebreaker/invert/early_c_check.cpp -- C++17 port of codebreaker/invert/early_c_check.py (see early_c_check.hpp).
//
// Build with native/invert's early_c*.cc, flow16.cc, cs.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in, and the
// typed ports early_c, early_c_frame, compilers, formats/omf.

#include "codebreaker/invert/early_c_check.hpp"

#include <algorithm>
#include <filesystem>
#include <mutex>
#include <set>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "codebreaker/invert/early_c.hpp"
#include "codebreaker/invert/early_c_frame.hpp"
#include "cs.h"
#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::invert::early_c_check {

namespace {

// platforms.decoder(16) is native/invert's Capstone: load the DLL the way Python's `import capstone` finds it
void ensure_capstone() {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
}

}  // namespace

py::bytes external_branches(const py::bytes& code) {
    ensure_capstone();
    std::string mask(static_cast<size_t>(code.size()), '\0');
    for (const cbinv::CsIns& ins : cbinv::disasm16(reinterpret_cast<const uint8_t*>(code.raw().data()), code.raw().size(), 0)) {
        const std::string& mn = ins.mnemonic;
        py::str ops(ins.op_str);
        bool branch = mn == "call" || mn == "jmp" || mn.rfind("j", 0) == 0 || mn.rfind("loop", 0) == 0;
        if (branch && ops.startswith(py::str("0x"))) {
            int64_t target;
            try {
                target = py::int_parse(ops, 16);
            } catch (const py::ValueError&) {
                continue;
            }
            if (0 <= target && target < code.size()) continue;
            int64_t first = code[ins.address];
            int64_t width = (first == 0xEB || (0x70 <= first && first <= 0x7F)) ? 1 : (ins.size >= 4 || first == 0xE8 || first == 0xE9) ? 2 : 1;
            for (int64_t k = 0; k < width; ++k) mask[static_cast<size_t>(ins.address + ins.size - width + k)] = '\1';
        }
    }
    return py::bytes(mask);
}

py::str source(const py::bytes& code, const py::str& name) {
    ensure_capstone();
    std::optional<py::str> no_cfg = py::os::environ_::get(py::str("NO_CFG"));
    early_c::Recovered r = (no_cfg && !no_cfg->empty()) ? early_c::recover(code) : early_c::recover_cfg(code);
    int64_t size = std::get<0>(r);
    const py::list<py::str>& params = std::get<1>(r);
    py::list<py::str> statements = std::get<2>(r);
    const py::str& kind = std::get<3>(r);
    const py::list<int64_t>& local = std::get<4>(r);
    // sorted(set(re.findall(r"g_[0-9a-f]+", " ".join(statements))))
    std::string joined;
    for (int64_t i = 0; i < statements.size(); ++i) joined += (i ? " " : "") + statements[i].utf8();
    std::set<std::u32string> globals_set;
    for (const py::str& g : py::re::compile(py::str("g_[0-9a-f]+")).findall(py::str(joined)).vec()) globals_set.insert(g.u32());
    py::str signature = py::percent(py::str("%s(%s)"), name, py::join(py::str(", "), params));
    py::str declared = params.empty() ? py::str("") : py::percent(py::str("%s %s;"), kind, py::join(py::str(", "), params));
    auto [ordered, ignored] = early_c_frame::emit_local_decls(local, size, std::nullopt, std::optional<py::list<std::tuple<int64_t, int64_t>>>(early_c::detect_copies(statements)));
    (void)ignored;
    py::str locals_;
    for (const auto& d : ordered.vec()) locals_ = locals_ + early_c_frame::fmt_decl(std::get<0>(d), std::get<1>(d), std::get<2>(d)) + py::str(" ");
    py::list<py::str> lines;
    if (!locals_.strip().empty()) lines.append(locals_.strip());
    lines.extend(statements);
    py::str head;
    if (!globals_set.empty()) {
        py::list<py::str> gs;
        for (const auto& g : globals_set) gs.append(py::str(g));
        head = py::percent(py::str("int %s;\n"), py::join(py::str(", "), gs));
    }
    py::list<py::str> body;
    for (const py::str& s : lines.vec()) body.append(py::str("    ") + s);
    return py::percent(py::str("%s%s\n%s\n{\n%s\n}\n"), head, signature, declared, py::join(py::str("\n"), body));
}

std::tuple<std::optional<py::str>, std::optional<bool>, py::str> reconstruct(const py::bytes& image, int64_t off, int64_t size, bool verify) {
    using R = std::tuple<std::optional<py::str>, std::optional<bool>, py::str>;
    py::bytes code = image.slice(off, off + size);
    if (code.size() < 4) return R(std::nullopt, false, py::str("too small to be a function"));
    py::str name = py::percent(py::str("f%05x"), off);
    py::str text;
    try {
        text = source(code, name);
    } catch (const py::Exception& e) {
        return R(std::nullopt, false, py::str("the model failed: ") + py::str(e.what()));
    }
    if (!verify) return R(text, std::nullopt, py::str("not compiled"));
    std::optional<py::bytes> obj = compilers::mc_compile(text);
    if (!obj || obj->size() == 0) return R(text, false, py::str("does not compile"));
    auto functions = formats::omf::function_bytes(*obj);
    std::optional<std::tuple<py::bytes, py::set<int64_t>>> built;
    for (const auto& kv : functions.items()) {
        if (std::get<0>(kv).upper().lstrip(py::str("_")) == name.upper()) {
            built = std::get<1>(kv);
            break;
        }
    }
    if (!built && !functions.empty()) built = std::get<1>(functions.items()[0]);
    if (!built) return R(text, false, py::str("no function in the object file"));
    const py::bytes& got = std::get<0>(*built);
    std::string mask(static_cast<size_t>(got.size()), '\0');
    for (int64_t i : std::get<1>(*built))
        if (0 <= i && i < static_cast<int64_t>(mask.size())) mask[static_cast<size_t>(i)] = '\1';
    py::bytes ext = external_branches(got);
    for (int64_t i = 0; i < ext.size(); ++i)
        if (ext[i]) mask[static_cast<size_t>(i)] = '\1';
    bool same = got.size() == code.size();
    if (same)
        for (int64_t i = 0; i < code.size(); ++i)
            if (!(mask[static_cast<size_t>(i)] || got[i] == code[i])) {
                same = false;
                break;
            }
    int64_t fixed = static_cast<int64_t>(std::count(mask.begin(), mask.end(), '\1'));
    return R(text, same,
             same ? py::percent(py::str("identical (%d bytes the linker fills in)"), fixed)
                  : py::percent(py::str("different (%d bytes against %d)"), got.size(), code.size()));
}

void main(const py::list<py::str>& argv) {
    py::file f = py::open(argv[1], py::str("rb"));
    py::bytes image = f.read();
    f.close();
    int64_t batch = -1;
    for (int64_t i = 0; i < argv.size(); ++i)
        if (argv[i] == py::str("--batch")) {
            batch = i;
            break;
        }
    if (batch >= 0) {
        py::file lf = py::open(argv[batch + 1]);
        py::V work = py::json_parse(lf.read().decode("utf-8").utf8());
        lf.close();
        int64_t same = 0;
        for (const py::V& item : work.seq()) {
            auto [text, ok, note] = reconstruct(image, py::int_parse(py::str(item.get("off").str()), 16), item.get("size").as_int());
            (void)text;
            same += ok.has_value() && *ok;
            py::print(py::percent(py::str("  %-16s %-34s %s"), py::str(item.get("name").str()), note, py::str(ok && *ok ? "OK" : "")));
        }
        py::print(py::percent(py::str("\n%d of %d functions give the original's bytes back"), same, static_cast<int64_t>(work.size())));
    } else {
        auto [text, ok, note] = reconstruct(image, py::int_parse(argv[2], 16), py::int_parse(argv[3]));
        py::print(py::str("same:"), py::str(!ok ? "None" : *ok ? "True" : "False"), py::str("|"), note);
        py::print(py::str(""));
        py::print(text ? *text : py::str("None"));
    }
}

}  // namespace codebreaker::invert::early_c_check
