// codebreaker/symbols/debug_build.cpp -- typed C++ port of codebreaker.symbols.debug_build.

#include "codebreaker/symbols/debug_build.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "codebreaker/formats/ne.hpp"
#include "json.hpp"
#include "py.hpp"
#include "py_json.hpp"
#include "value.hpp"

namespace codebreaker::symbols::debug_build {

namespace segment = source::ne_asm::segment;
namespace ne_mod = formats::ne;

const py::str _BR(
    "call|jmp|je|jne|jz|jnz|jl|jle|jg|jge|jb|jbe|ja|jae|js|jns|jo|jno|"
    "jp|jnp|jpe|jpo|jc|jnc|jcxz|jecxz|loop|loope|loopne|loopz|loopnz");

namespace {

// sorted(str keys) as Python orders them
bool str_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

}  // namespace

Sig norm_sig(const segment::Seg& sg, int64_t start) {
    static const py::re::Pattern ws = py::re::compile(py::str("\\s+"));
    static const py::re::Pattern branch = py::re::compile(py::str("^((?:") + _BR + py::str(")\\s+)0x[0-9a-f]+"));
    static const py::re::Pattern absmem = py::re::compile(py::str("\\[0x[0-9a-f]+\\]"));
    static const py::re::Pattern farptr = py::re::compile(py::str("0x[0-9a-f]+:0x[0-9a-f]+"));
    const auto rows = std::get<1>(sg.func_rows(start));
    Sig sig;
    for (const auto& oa : rows) {
        py::str a = ws.sub(py::str(" "), std::get<1>(oa)).strip();
        a = branch.sub(py::str("\\1X"), a);      // branch target
        a = absmem.sub(py::str("[X]"), a);       // absolute mem
        a = farptr.sub(py::str("X:X"), a);       // far ptr
        sig.append(a);
    }
    return sig;
}

py::dict<Sig, py::str> build_debug_index(const py::str& debug_exe, const py::str& debug_map,
                                         const py::dict<py::str, py::str>& ordinals) {
    const py::str mp = py::file::open(debug_map, py::str("r"), py::str("cp1252"), py::str("replace")).read_text();
    py::dict<int64_t, py::dict<int64_t, py::str>> names;   // seg -> {off -> NAME}
    for (const py::re::Match& m : py::re::finditer(py::str("^ ([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\\s+(\\w+)"), mp, py::re::M)) {
        const int64_t seg = py::int_parse(m.group(1), 16), off = py::int_parse(m.group(2), 16);
        names.setdefault(seg, py::dict<int64_t, py::str>()).set(off, m.group(3));
    }

    const ne_mod::NEFile ne(debug_exe);
    py::dict<Sig, py::str> sig2name;
    py::set<Sig> dup;
    int64_t i = 0;
    for (const ne_mod::Segment& s : ne.segments()) {
        ++i;
        if (!(s.kind == py::str("CODE")) || !names.contains(i)) continue;
        const segment::Seg sg(ne, i, ordinals);
        const py::dict<int64_t, py::str> in_seg = names.at(i);
        for (const int64_t st : in_seg.keys()) {
            if (!sg.valid.contains(st)) continue;
            const Sig sig = norm_sig(sg, st);
            if (sig.empty()) continue;
            if (sig2name.contains(sig) && !(sig2name.at(sig) == in_seg.at(st))) dup.add(sig);
            else sig2name.set(sig, in_seg.at(st));
        }
    }
    for (const Sig& sig : dup) sig2name.pop(sig, py::str(""));
    return sig2name;
}

std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> match_module(
    const py::str& retail_exe, const py::dict<Sig, py::str>& sig2name_in, const py::dict<py::str, py::str>& ordinals, int64_t min_len) {
    const ne_mod::NEFile ne(retail_exe);
    // {i: Seg(ne, i, ordinals) for the CODE segments}, in segment order
    std::vector<std::pair<int64_t, segment::Seg>> segs;
    int64_t idx = 0;
    for (const ne_mod::Segment& s : ne.segments()) {
        ++idx;
        if (s.kind == py::str("CODE")) segs.emplace_back(idx, segment::Seg(ne, idx, ordinals));
    }

    // export validation: drop any signature a retail export owns under another name
    py::dict<Sig, py::set<py::str>> export_sig;   // sig -> {export names}
    py::dict<py::str, std::tuple<int64_t, int64_t>> exp_at;   // UPPERNAME -> (seg, off)
    for (const auto& is : segs) {
        const int64_t i = is.first;
        const segment::Seg& sg = is.second;
        for (const int64_t st : sg.funcs.keys()) {
            const py::str nm = sg.name_at(st);
            if (!nm.empty() && !nm.lower().startswith(py::str("sub_"))) {
                export_sig.setdefault(norm_sig(sg, st), py::set<py::str>()).add(nm.upper());
                exp_at.set(nm.upper(), std::make_tuple(i, st));
            }
        }
    }
    py::dict<Sig, py::str> sig2name;
    for (const auto& kv : sig2name_in.items()) {
        const Sig& sig = std::get<0>(kv);
        const py::str& nm = std::get<1>(kv);
        const bool owned = export_sig.contains(sig) && !export_sig.at(sig).empty();
        if (!(owned && !export_sig.at(sig).contains(nm.upper()))) sig2name.set(sig, nm);
    }

    py::dict<py::str, py::str> namemap;
    int64_t agree = 0, disagree = 0;
    for (const auto& is : segs) {
        const int64_t i = is.first;
        const segment::Seg& sg = is.second;
        for (const int64_t st : sg.funcs.keys()) {
            const Sig sig = norm_sig(sg, st);
            std::optional<py::str> real;
            if (sig.size() >= min_len && sig2name.contains(sig)) real = sig2name.at(sig);
            if (!real || real->empty()) continue;
            // never shadow a real export at a different offset (case-insensitive)
            if (exp_at.contains(real->upper()) && !(exp_at.at(real->upper()) == std::make_tuple(i, st))) continue;
            const py::str cur = sg.name_at(st);
            if (!cur.empty() && !cur.lower().startswith(py::str("sub_"))) {
                agree += (cur.upper() == real->upper());
                disagree += (cur.upper() != real->upper());
            }
            namemap.set(py::percent(py::str("%d:%04x"), i, st), *real);
        }
    }

    // a real name lives at one address: drop every offset under a duplicated name
    py::dict<py::str, int64_t> cnt;
    for (const py::str& v : namemap.values()) cnt.set(v, cnt.get(v, 0) + 1);
    py::set<py::str> dupnames;
    for (const auto& kv : cnt.items()) {
        if (std::get<1>(kv) > 1) dupnames.add(std::get<0>(kv));
    }
    py::dict<py::str, py::str> kept;
    for (const auto& kv : namemap.items()) {
        if (!dupnames.contains(std::get<1>(kv))) kept.set(std::get<0>(kv), std::get<1>(kv));
    }
    return {kept, std::make_tuple(agree, disagree)};
}

py::dict<int64_t, py::dict<int64_t, py::str>> load_override(const py::str& names_json) {
    py::dict<int64_t, py::dict<int64_t, py::str>> override_;
    const py::str text = py::file::open(names_json, py::str("r"), py::str("cp1252")).read_text();
    py::V loaded;
    try {
        loaded = py::json_parse(text.utf8());
    } catch (const py::PyError& e) {
        throw JSONDecodeError(e.what());
    }
    if (!loaded.is_dict()) throw py::AttributeError("'" + std::string(loaded.is_list() ? "list" : "object") + "' object has no attribute 'items'");
    for (const auto& kv : loaded.items()) {
        const py::str k(kv.first.str());
        const py::list<py::str> parts = py::split(k, py::str(":"));
        if (parts.size() != 2) throw py::ValueError(parts.size() < 2 ? "not enough values to unpack (expected 2, got " + std::to_string(parts.size()) + ")"
                                                                   : "too many values to unpack (expected 2)");
        if (!kv.second.is_str()) throw py::TypeError("a name must be a JSON string");
        const int64_t si = py::int_parse(parts[0]), off = py::int_parse(parts[1], 16);
        override_.setdefault(si, py::dict<int64_t, py::str>()).set(off, py::str(kv.second.str()));
    }
    return override_;
}

std::tuple<py::dict<py::str, py::str>, std::tuple<int64_t, int64_t>> recover(
    const py::str& retail_exe, const py::str& debug_exe, const py::str& debug_map, const py::dict<py::str, py::str>& ordinals,
    const std::optional<py::str>& out_json) {
    const py::dict<Sig, py::str> idx = build_debug_index(debug_exe, debug_map, ordinals);
    const auto result = match_module(retail_exe, idx, ordinals);
    const py::dict<py::str, py::str>& namemap = std::get<0>(result);
    if (out_json && !out_json->empty()) {
        const py::str folder = py::os::path::dirname(*out_json);
        // os.makedirs("") is Python's FileNotFoundError ([WinError 3]); the runtime's makedirs says OSError for it
        if (folder.empty()) throw py::FileNotFoundError("[WinError 3] The system cannot find the path specified: '");
        py::os::makedirs(folder, 0777, true);
        // json.dump(namemap, open(out_json, "w"), indent=0, sort_keys=True)
        std::vector<py::str> keys = namemap.keys();
        std::sort(keys.begin(), keys.end(), str_less);
        std::string text;
        if (keys.empty()) {
            text = "{}";
        } else {
            text = "{\n";
            for (size_t k = 0; k < keys.size(); ++k) {
                if (k) text += ",\n";
                py::json::put(text, keys[k]);
                text += ": ";
                py::json::put(text, namemap.at(keys[k]));
            }
            text += "\n}";
        }
        const py::file f = py::file::open(*out_json, py::str("w"));
        f.write(py::str(text));
        f.close();
    }
    return result;
}

}  // namespace codebreaker::symbols::debug_build
