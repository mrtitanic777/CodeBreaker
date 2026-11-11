// codebreaker/symbols/names.cpp -- typed C++ port of codebreaker.symbols.names (see names.hpp).

#include "codebreaker/symbols/names.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/source/module_writer.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/demangle.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/symbols/x86.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::names {

namespace {

constexpr int64_t _SMALLEST = 24;       // bytes; a body shorter than this is everybody's

bool is_end(const std::string& mn) {
    return mn == "ret" || mn == "retf" || mn == "retn" || mn == "jmp" || mn == "ljmp" || mn == "iret" || mn == "iretd";
}

using engine::native_engine::Cell;

bool is_none(const Cell& c) { return std::holds_alternative<std::monostate>(c); }

Cell cell_of(const std::optional<py::str>& s) { return s ? Cell(*s) : Cell(); }

unsigned char byte_at(const py::bytes& b, int64_t i) {
    if (i < 0 || i >= b.size()) throw py::IndexError("index out of range");
    return static_cast<unsigned char>(b.raw()[static_cast<size_t>(i)]);
}

bool by_u32(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

}  // namespace

py::str key(const py::str& module_path, int64_t address) {
    // The function store's text for an address (see verify/lanes/win32.py and win16.py).
    std::optional<py::str> lane = base::corpus::lane(module_path);
    if (lane && *lane == py::str("ne")) {
        auto so = decompile::describe::where(address);
        return verify::lanes::win16::key(std::get<0>(so), std::get<1>(so));
    }
    return verify::lanes::win32::LANES.at(py::str("pe")).key(address);
}

// ---- bodies ------------------------------------------------------------------------------------------

namespace {

// Digest of a function's bytes up to its last return or jump (what follows is padding), with the offsets in `masked`
// left out. None for a body with nothing of its own in it.
std::optional<py::str> _digest(const py::bytes& code, const std::set<int64_t>& masked, int64_t last) {
    std::string body = code.slice(0, last).raw();
    for (int64_t o : masked)
        if (o < last && o >= 0 && o < static_cast<int64_t>(body.size())) body[static_cast<size_t>(o)] = '\0';
    if (last < _SMALLEST) return std::nullopt;
    return py::hashlib::sha1(py::bytes(std::to_string(last) + ":" + body)).hexdigest().slice(0, 16);
}

py::dict<int64_t, py::str> _bodies32(const py::str& path) {
    std::shared_ptr<formats::pe_functions::PeCarve> carve = formats::pe_functions::carve(path);
    const formats::pe::PEFile& pe = *carve->pe;
    int64_t lo = static_cast<int64_t>(pe.image_base);
    int64_t extent = 0;
    for (const auto& s : pe.sections) extent = std::max(extent, s.vaddr + std::max(s.vsize, s.rawsize));
    int64_t hi = lo + extent;
    py::dict<int64_t, py::str> out;
    for (const auto& fkv : carve->funcs.items()) {
        int64_t start = std::get<0>(fkv);
        const formats::pe_functions::Func& f = std::get<1>(fkv);
        formats::pe_functions::SecOf sec = carve->_sec_of(start);
        if (!sec.data) continue;
        const int64_t base = *sec.base;
        const py::bytes& raw = *sec.data;
        int64_t end = f.end, last = 0, va = start, own = 0;
        std::set<int64_t> masked;
        while (va < end && carve->insn->contains(va)) {
            formats::pe_functions::Record rec = carve->insn->at(va);
            const formats::pe_functions::Instruction& ins = rec.ins;
            const std::string mnemonic = rec.mnemonic.utf8();
            int64_t at = va - start;
            struct Field {
                int64_t off, n;
                std::optional<int64_t> value;
            };
            Field fields[2] = {{ins.disp_offset, ins.disp_size, ins.disp & 0xFFFFFFFFLL}, {ins.imm_offset, ins.imm_size, std::nullopt}};
            for (const Field& fl : fields) {
                int64_t off = fl.off, n = fl.n;
                std::optional<int64_t> value = fl.value;
                if (!off || n != 4) continue;
                if (!value) {
                    value = py::unpack_from1<int64_t>("<I", raw, va - base + off);
                    if (mnemonic == "call" || mnemonic[0] == 'j') {             // relative: where it goes
                        value = (va + rec.size + *value) & 0xFFFFFFFFLL;
                        if (start <= *value && *value < end) continue;           // inside the function: the same anywhere
                        for (int64_t k = at + off; k < at + off + 4; k++) masked.insert(k);
                        continue;
                    }
                }
                if (lo <= *value && *value < hi) {                              // an address in the module
                    for (int64_t k = at + off; k < at + off + 4; k++) masked.insert(k);
                }
            }
            own += !runtime_library::_FRAME.match((rec.mnemonic + py::str(" ") + rec.op_str).strip()).has_value();
            va += rec.size;
            if (is_end(mnemonic)) last = va - start;
        }
        std::optional<py::str> digest = own >= 6 ? _digest(raw.slice(start - base, end - base), masked, last) : std::nullopt;
        if (digest) out.set(start, *digest);
    }
    return out;
}

py::dict<int64_t, py::str> _bodies16(const py::str& path) {
    static const py::re::Pattern near_target("0x[0-9a-f]+", 0);
    formats::ne::NEFile nef(path);
    py::dict<int64_t, formats::ne::Segment> segs;
    for (const auto& s : nef.segments()) segs.set(s.idx, s);
    py::dict<int64_t, py::list<int64_t>> starts;
    for (const auto& so : decompile::describe::functions(nef).keys())
        starts.setdefault(std::get<0>(so), py::list<int64_t>()).append(std::get<1>(so));
    py::dict<int64_t, py::str> out;
    for (const auto& kv : starts.items()) {
        int64_t seg = std::get<0>(kv);
        std::vector<int64_t> offs;
        for (int64_t o : std::get<1>(kv)) offs.push_back(o);
        const formats::ne::Segment& s = segs.at(seg);
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        std::set<int64_t> patched;                                   // every place the loader writes
        for (const auto& r : nef.relocations(s)) {
            int64_t width = r.src == 3 ? 4 : r.src == 2 ? 2 : r.src == 5 ? 2 : r.src == 0 ? 1 : 2;
            int64_t off = r.offset, hops = 0;
            while (off != 0xFFFF && off + 1 < code.size() && hops < 0x10000) {
                for (int64_t k = off; k < off + width; k++) patched.insert(k);
                if (r.additive) break;
                off = py::unpack_from1<int64_t>("<H", code, off);
                hops += 1;
            }
        }
        std::sort(offs.begin(), offs.end());
        for (size_t i = 0; i < offs.size(); i++) {
            int64_t start = offs[i];
            int64_t end = i + 1 < offs.size() ? offs[i + 1] : code.size();
            std::set<int64_t> masked;
            for (int64_t o : patched)
                if (start <= o && o < end) masked.insert(o - start);
            int64_t last = 0, own = 0;
            for (const x86::Lite& l : x86::disasm_lite(16, code.slice(start, end).raw(), start)) {
                py::str ops(l.op_str);
                if ((l.mnemonic == "call" || l.mnemonic[0] == 'j') && l.size == 3 && near_target.fullmatch(ops)) {
                    if (!(start <= py::int_parse(ops, 16) && py::int_parse(ops, 16) < end))    // a near call or jump out of the function
                        for (int64_t k = l.address - start + 1; k < l.address - start + 3; k++) masked.insert(k);
                }
                own += !runtime_library::_FRAME.match((py::str(l.mnemonic) + py::str(" ") + ops).strip()).has_value();
                if (is_end(l.mnemonic)) last = l.address + l.size - start;
            }
            std::optional<py::str> digest = own >= 6 ? _digest(code.slice(start, end), masked, last) : std::nullopt;
            if (digest) out.set(decompile::describe::address(seg, start), *digest);
        }
    }
    return out;
}

}  // namespace

py::dict<int64_t, py::str> bodies(const py::str& module_path) {
    // {address: digest} for the functions of a module that have a body worth telling apart.
    std::optional<py::str> lane = base::corpus::lane(module_path);
    if (lane && *lane == py::str("pe") && formats::pe::open_pe(module_path).machine == 0x014C) return _bodies32(module_path);
    if (lane && *lane == py::str("ne")) return _bodies16(module_path);
    return {};
}

// ---- debug symbols -------------------------------------------------------------------------------------

py::dict<std::tuple<int64_t, int64_t>, py::str> codeview(const py::bytes& data) {
    // {(segment, offset): name} from CodeView symbols appended to an executable (the NB09 form: public symbols and
    // procedures), or {}.
    const std::string& d = data.raw();
    auto find_nb09 = [&](int64_t from) -> int64_t {
        size_t p = d.find("NB09", static_cast<size_t>(std::max<int64_t>(from, 0)));
        return p == std::string::npos ? -1 : static_cast<int64_t>(p);
    };
    int64_t base = find_nb09(0);
    py::dict<std::tuple<int64_t, int64_t>, py::str> out;
    while (base >= 0 && out.empty()) {
        try {
            int64_t p = base + py::unpack_from1<int64_t>("<I", data, base + 4);
            auto [head, entry, count] = py::unpack_from<int64_t, int64_t, int64_t>("<HHI", data, p);
            int64_t n_entries = (12 <= entry && entry <= 32) ? count : 0;
            for (int64_t i = 0; i < n_entries; i++) {
                auto [kind, module, lfo, size] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<HHIi", data, p + head + i * entry);
                (void)module;
                if (kind != 0x129 && kind != 0x12A && kind != 0x134) continue;
                int64_t s = base + lfo + (kind == 0x134 ? 0 : 16);
                int64_t end = std::min<int64_t>(base + lfo + size, data.size());
                while (s + 4 <= end) {
                    auto [n, rec] = py::unpack_from<int64_t, int64_t>("<HH", data, s);
                    if (n < 2) break;
                    std::optional<int64_t> at;
                    if (rec == 0x0203) at = s + 4;
                    else if (rec == 0x0204 || rec == 0x0205) at = s + 28;
                    if (at) {
                        auto [off, seg] = py::unpack_from<int64_t, int64_t>("<IH", data, *at);
                        int64_t q = *at + (rec == 0x0203 ? 8 : 9);
                        py::str name = data.slice(q + 1, q + 1 + byte_at(data, q)).decode("latin1", "replace");
                        if (!name.empty() && seg) out.setdefault(std::make_tuple(seg, off), name);
                    }
                    s += 2 + n;
                }
            }
        } catch (const py::StructError&) {
            out = py::dict<std::tuple<int64_t, int64_t>, py::str>();
        } catch (const py::IndexError&) {
            out = py::dict<std::tuple<int64_t, int64_t>, py::str>();
        }
        base = find_nb09(base + 4);
    }
    return out;
}

py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> read_sym(const py::str& path) {
    // [(segment number, {offset: name})] from a .SYM file (the symbol file MAPSYM makes from a linker map: per
    // segment, each public name and its offset).
    py::bytes d = py::open_rb(path).read();
    py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> out;
    int64_t seg = py::unpack_from1<int64_t>("<H", d, 12) * 16, count = py::unpack_from1<int64_t>("<H", d, 10);
    for (int64_t k = 0; k < count; k++) {
        if (!seg || seg + 21 > d.size()) break;
        auto [nxt, nsym, table, number] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<HHHH", d, seg);
        int64_t wide = byte_at(d, seg + 14) & 1;
        py::dict<int64_t, py::str> symbols;
        for (int64_t i = 0; i < nsym; i++) {
            int64_t at = seg + py::unpack_from1<int64_t>("<H", d, seg + table + 2 * i);
            if (at + 5 > d.size()) break;
            int64_t value = wide ? py::unpack_from1<int64_t>("<I", d, at) : py::unpack_from1<int64_t>("<H", d, at);
            int64_t n = byte_at(d, at + (wide ? 4 : 2));
            int64_t from = at + (wide ? 5 : 3);
            symbols.set(value, d.slice(from, from + n).decode("latin1", "replace"));
        }
        out.append({number, symbols});
        if (nxt * 16 <= seg) break;
        seg = nxt * 16;
    }
    return out;
}

py::dict<int64_t, py::str> sym_objects(const py::str& path) {
    // {segment number: name} from a .SYM file. The linker names a segment after the object file its code came from
    // (_MSGBOX, _WINUTIL, ...), so where a module was linked one segment per object these are the names of its
    // maker's source files. {} when it cannot be read.
    try {
        py::bytes d = py::open_rb(path).read();
        py::dict<int64_t, py::str> out;
        int64_t seg = py::unpack_from1<int64_t>("<H", d, 12) * 16, count = py::unpack_from1<int64_t>("<H", d, 10);
        for (int64_t k = 0; k < count; k++) {
            if (!seg || seg + 21 > d.size()) break;
            auto [nxt, nsym, table, number] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<HHHH", d, seg);
            (void)nsym;
            (void)table;
            py::str name = d.slice(seg + 21, seg + 21 + byte_at(d, seg + 20)).decode("latin1", "replace");
            if (!name.empty()) out.set(number, name);
            if (nxt * 16 <= seg) break;
            seg = nxt * 16;
        }
        return out;
    } catch (const py::OSError&) {
    } catch (const py::StructError&) {
    } catch (const py::IndexError&) {
    }
    return {};
}

namespace {

const std::set<std::string>& plain_segments() {
    static const std::set<std::string> s = {"_TEXT", "TEXT", "DGROUP", "BSS", "_BSS", "RDATA", "_DATA", "DATA", "CONST", "IGROUP",
                                            "LGROUP", "_LGROUP", "_IGROUP", "EDATA", "IDATA", "RELOC", "RSRC", "CODE", "_CODE",
                                            "AUTO", "STACK", "HEAP", "_INIT", "INIT"};
    return s;
}

std::optional<py::dict<py::str, py::list<py::str>>> _sym_files;

// The .SYM files for a module in the reference folder (settings "reference", else the folder of that name beside the
// corpus), or [].
py::list<py::str> _sym_file(const py::str& stem) {
    if (!_sym_files) {
        py::dict<py::str, py::list<py::str>> found;
        py::str ref = base::corpus::reference();
        for (const auto& step : py::os::walk(ref)) {
            py::list<py::str> dirs = std::get<1>(step), fns = std::get<2>(step);
            std::vector<py::str> keep;
            for (const py::str& d : dirs)
                if (!(d.upper() == py::str("ALPHA") || d.upper() == py::str("MIPS") || d.upper() == py::str("PPC"))) keep.push_back(d);
            dirs.clear();                       // dirs[:] = [...]
            for (const py::str& d : keep) dirs.append(d);
            for (const py::str& fn : fns)
                if (fn.lower().endswith(py::str(".sym")))
                    found.setdefault(fn.slice(0, fn.size() - 4).upper(), py::list<py::str>()).append(py::os::path::join(std::get<0>(step), fn));
        }
        _sym_files = found;
    }
    return _sym_files->get(stem).value_or(py::list<py::str>());
}

py::str _plain(const py::str& name) { return std::get<0>(demangle::plain(name)).upper(); }

// {segment: {offset: name}} sorted by segment
py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> _by_segment(const py::dict<std::tuple<int64_t, int64_t>, py::str>& symbols) {
    py::dict<int64_t, py::dict<int64_t, py::str>> by;
    for (const auto& kv : symbols.items()) {
        const auto& so = std::get<0>(kv);
        by.setdefault(std::get<0>(so), py::dict<int64_t, py::str>()).set(std::get<1>(so), std::get<1>(kv));
    }
    std::vector<int64_t> keys;
    for (int64_t k : by.keys()) keys.push_back(k);
    std::sort(keys.begin(), keys.end());
    py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> out;
    for (int64_t k : keys) out.append({k, by.at(k)});
    return out;
}

py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> _read_sym(const py::str& path) {
    try {
        return read_sym(path);
    } catch (const py::OSError&) {
    } catch (const py::StructError&) {
    } catch (const py::IndexError&) {
    }
    return {};
}

std::map<std::string, std::pair<py::dict<int64_t, std::tuple<py::str, py::str>>, py::dict<int64_t, py::str>>> _debugged;

py::dict<int64_t, std::tuple<py::str, py::str>> _debug_names(const py::str& module_path, const py::dict<int64_t, py::str>& files) {
    std::optional<py::str> lane = base::corpus::lane(module_path);
    py::dict<int64_t, std::tuple<py::str, py::str>> out;
    py::bytes data = py::open_rb(module_path).read();
    bool ne = lane && *lane == py::str("ne");
    py::dict<py::str, std::tuple<int64_t, int64_t>> ne_exports;       // (NE) name -> (segment, offset)
    py::dict<py::str, int64_t> pe_exports;                            // (PE) name -> rva
    py::str stem;
    std::vector<int64_t> sections;
    int64_t image_base = 0;
    if (ne) {
        formats::ne::NEFile nef(module_path);
        py::dict<int64_t, std::tuple<int64_t, int64_t>> entries;
        for (const auto& e : nef.entries())
            if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && e.seg)
                entries.set(e.ordinal, std::make_tuple(*e.seg, e.offset.value_or(0)));
        auto res = nef.resident_names();
        auto nonres = nef.nonresident_names();
        for (int64_t i = 1; i < res.size(); i++)
            if (entries.contains(std::get<1>(res[i]))) ne_exports.set(std::get<0>(res[i]).upper(), entries.at(std::get<1>(res[i])));
        for (int64_t i = 1; i < nonres.size(); i++)
            if (entries.contains(std::get<1>(nonres[i]))) ne_exports.set(std::get<0>(nonres[i]).upper(), entries.at(std::get<1>(nonres[i])));
        stem = nef.module_name().upper();
    } else if (lane && *lane == py::str("pe")) {
        formats::pe::PEFile pe = formats::pe::open_pe(module_path);
        image_base = static_cast<int64_t>(pe.image_base);
        for (const auto& e : pe.exports())
            if (std::get<1>(e) && !std::get<1>(e)->empty()) pe_exports.set(_plain(*std::get<1>(e)), std::get<2>(e));
        stem = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).upper();
        for (const auto& s : pe.sections) sections.push_back(s.vaddr);
    } else {
        return out;
    }
    auto place = [&](int64_t seg, int64_t off) -> std::optional<int64_t> {
        if (ne) return decompile::describe::address(seg, off);
        if (0 < seg && seg <= static_cast<int64_t>(sections.size())) return image_base + sections[static_cast<size_t>(seg - 1)] + off;
        return std::nullopt;
    };
    struct Table {
        py::str where;
        py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> segments;
        py::dict<int64_t, py::str> objects;
    };
    std::vector<Table> tables;
    tables.push_back({py::str("CodeView symbols in the file"), _by_segment(codeview(data)), py::dict<int64_t, py::str>()});
    for (const py::str& p : _sym_file(stem)) tables.push_back({py::os::path::basename(p), _read_sym(p), sym_objects(p)});
    for (const Table& t : tables) {
        for (const auto& seg : t.segments) {
            int64_t number = std::get<0>(seg);
            const py::dict<int64_t, py::str>& symbols = std::get<1>(seg);
            int64_t agree = 0, named = 0;
            for (const auto& ov : symbols.items()) {
                int64_t off = std::get<0>(ov);
                py::str nm = _plain(std::get<1>(ov));
                if (ne) {
                    std::optional<std::tuple<int64_t, int64_t>> at = ne_exports.get(nm);
                    if (at) {
                        named += 1;
                        agree += (*at == std::make_tuple(number, off & 0xFFFF));
                    }
                } else {
                    std::optional<int64_t> at = pe_exports.get(nm);
                    if (at) {
                        named += 1;
                        std::optional<int64_t> p = place(number, off);
                        agree += (p && *p == image_base + *at);
                    }
                }
            }
            if (agree >= 2 && agree * 3 >= named * 2) {
                for (const auto& ov : symbols.items()) {
                    int64_t off = std::get<0>(ov);
                    std::optional<int64_t> address = place(number, ne ? (off & 0xFFFF) : off);
                    if (address) {
                        out.setdefault(*address, std::make_tuple(std::get<0>(demangle::plain(std::get<1>(ov))), t.where));
                        std::optional<py::str> object = t.objects.get(number);
                        py::str oname = object ? *object : py::str("");
                        if (!plain_segments().count(oname.upper().utf8()) && !oname.empty())
                            files.setdefault(*address, oname.lstrip(py::str("_")).lower());
                    }
                }
            }
        }
        if (!out.empty()) break;
    }
    return out;
}

const std::pair<py::dict<int64_t, std::tuple<py::str, py::str>>, py::dict<int64_t, py::str>>& _debug(const py::str& module_path) {
    std::string key_ = py::os::path::abspath(module_path).utf8();
    auto it = _debugged.find(key_);
    if (it == _debugged.end()) {
        py::dict<int64_t, py::str> files;
        auto names = _debug_names(module_path, files);
        it = _debugged.emplace(key_, std::make_pair(names, files)).first;
    }
    return it->second;
}

}  // namespace

py::dict<int64_t, py::str> source_files(const py::str& module_path) {
    // {address: name of the object file the function was linked from}, for the functions the module's .SYM file
    // places in a segment named after an object -- and only where that file is shown to be for this build (see
    // debug_names). {} when there is no such file.
    return _debug(module_path).second;
}

py::dict<int64_t, std::tuple<py::str, py::str>> debug_names(const py::str& module_path) {
    // {address: (name, where it is from)} from debug symbols that are shown to be for this very build: a symbol file
    // is from some build of the module, and offsets move between builds, so a segment's names are used only where the
    // file's own exports sit at the offsets the symbols give them (at least two, and two in three of those the symbol
    // file names).
    return _debug(module_path).first;
}

// ---- the names of a module -------------------------------------------------------------------------------

namespace {

py::list<int64_t> _starts(const py::str& module_path) {
    std::optional<py::str> lane = base::corpus::lane(module_path);
    if (lane && *lane == py::str("ne")) {
        std::vector<int64_t> v;
        for (const auto& so : decompile::describe::functions(formats::ne::NEFile(module_path)).keys())
            v.push_back(decompile::describe::address(std::get<0>(so), std::get<1>(so)));
        std::sort(v.begin(), v.end());
        py::list<int64_t> out;
        for (int64_t a : v) out.append(a);
        return out;
    }
    if (lane && *lane == py::str("pe")) return decompile::describe::function_starts(module_path);
    return {};
}

}  // namespace

Named own_names(const py::str& module_path) {
    // {address: (name, source)} from what the module and its symbol files say themselves: exports, debug symbols,
    // the ordinal table.
    std::optional<py::str> lane = base::corpus::lane(module_path);
    Named out;
    py::str stem;
    py::dict<int64_t, int64_t> numbered;
    if (lane && *lane == py::str("ne")) {
        formats::ne::NEFile nef(module_path);
        py::dict<int64_t, int64_t> entries;
        for (const auto& e : nef.entries())
            if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && e.seg)
                entries.set(e.ordinal, decompile::describe::address(*e.seg, e.offset.value_or(0)));
        auto nonres = nef.nonresident_names();
        auto res = nef.resident_names();
        for (int64_t i = 1; i < nonres.size(); i++)
            if (entries.contains(std::get<1>(nonres[i]))) out.setdefault(entries.at(std::get<1>(nonres[i])), std::make_tuple(std::get<0>(nonres[i]), py::str("export")));
        for (int64_t i = 1; i < res.size(); i++)
            if (entries.contains(std::get<1>(res[i]))) out.setdefault(entries.at(std::get<1>(res[i])), std::make_tuple(std::get<0>(res[i]), py::str("export")));
        stem = nef.module_name().upper();
        numbered = entries;
    } else if (lane && *lane == py::str("pe") && formats::pe::open_pe(module_path).machine == 0x01F2) {
        return powerpc_names(module_path);
    } else if (lane && *lane == py::str("pe")) {
        formats::pe::PEFile pe = formats::pe::open_pe(module_path);
        for (const auto& e : pe.exports()) {
            int64_t va = static_cast<int64_t>(pe.image_base) + std::get<2>(e);
            numbered.set(std::get<0>(e), va);
            if (std::get<1>(e) && !std::get<1>(e)->empty()) out.setdefault(va, std::make_tuple(*std::get<1>(e), py::str("export")));
        }
        stem = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).upper();
    } else {
        return out;
    }
    for (const auto& kv : debug_names(module_path).items())
        out.setdefault(std::get<0>(kv), std::make_tuple(std::get<0>(std::get<1>(kv)),
                                                        py::percent(py::str("debug symbols (%s)"), std::get<1>(std::get<1>(kv)))));
    for (const auto& kv : numbered.items()) {
        int64_t o = std::get<0>(kv), address = std::get<1>(kv);
        if (out.contains(address)) continue;
        std::optional<py::str> ln = ordinals::linker_name(stem, o);
        if (ln && !ln->empty())
            out.set(address, std::make_tuple(*ln, py::percent(py::str("ordinal table (%s)"), ordinals::source(stem, o))));
    }
    return out;
}

Named powerpc_names(const py::str& module_path) {
    // {address: (name, source)} for a PowerPC image: an image of the console exports nothing and carries no symbols,
    // so what names a function for certain is its run-time type information (formats/powerpc_image.py) -- a virtual
    // function of a named class -- and the entry point.
    static const py::re::Pattern nonword("\\W+", 0);
    auto img = formats::powerpc_image::read(module_path);
    Named out;
    if (img->entry) out.set(img->entry, std::make_tuple(py::str("entry"), py::str("entry point")));
    py::set<py::str> taken;
    taken.add(py::str("entry"));
    std::vector<std::pair<int64_t, std::tuple<py::str, int64_t>>> methods;
    for (const auto& kv : formats::powerpc_image::virtual_methods(*img).items()) methods.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(methods.begin(), methods.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        if (std::get<0>(a.second).u32() != std::get<0>(b.second).u32()) return std::get<0>(a.second).u32() < std::get<0>(b.second).u32();
        return std::get<1>(a.second) < std::get<1>(b.second);
    });
    for (const auto& m : methods) {
        int64_t address = m.first;
        const py::str& cls = std::get<0>(m.second);
        int64_t slot = std::get<1>(m.second);
        py::str name = py::percent(py::str("%s_vf%d"), nonword.sub(py::str("_"), cls), slot);
        if (!out.contains(address) && !taken.contains(name)) {
            taken.add(name);
            out.set(address, std::make_tuple(name, py::percent(py::str("run-time type information (%s, virtual function %d)"), cls, slot)));
        }
    }
    return out;
}

int64_t index(const py::str& module_path, const FunctionStorePtr& fs0, const std::optional<Named>& own0) {
    // Record a module's bodies in the store, each with the name the module itself gives it. -> number of bodies
    FunctionStorePtr fs = fs0 ? fs0 : store::functions::open();
    py::str sha = base::corpus::sha(module_path).value_or(py::str(""));
    Named own = own0 ? *own0 : own_names(module_path);
    fs->add_module(sha, py::os::path::basename(module_path), module_path, base::corpus::lane(module_path).value_or(py::str("")));
    py::dict<py::str, std::tuple<py::str, Cell, Cell>> rows;
    for (const auto& kv : bodies(module_path).items()) {
        std::optional<std::tuple<py::str, py::str>> o = own.get(std::get<0>(kv));
        rows.set(key(module_path, std::get<0>(kv)),
                 std::make_tuple(std::get<1>(kv), o ? Cell(std::get<0>(*o)) : Cell(), o ? Cell(std::get<1>(*o)) : Cell()));
    }
    fs->set_bodies(sha, rows);
    return rows.size();
}

std::optional<py::str> set_name(const py::str& module_path, int64_t address, const py::str& name, const FunctionStorePtr& fs0) {
    // Name the body of the function at `address`; every module holding that body then has the name. -> the body's
    // digest, or None when the function has no body worth naming.
    FunctionStorePtr fs = fs0 ? fs0 : store::functions::open();
    py::str sha = base::corpus::sha(module_path).value_or(py::str(""));
    if (fs->bodies(sha).empty()) index(module_path, fs);
    auto row = fs->bodies(sha).get(key(module_path, address));
    if (row) fs->name_body(std::get<0>(*row), name);
    return row ? std::optional<py::str>(std::get<0>(*row)) : std::nullopt;
}

ModuleNames of_module(const py::str& module_path, const FunctionStorePtr& fs0) {
    // Every function and import of a module with its best name and where the name is from.
    FunctionStorePtr fs = fs0 ? fs0 : store::functions::open();
    std::optional<py::str> lane = base::corpus::lane(module_path);
    py::str sha = base::corpus::sha(module_path).value_or(py::str(""));
    Named own = own_names(module_path);
    if (fs->bodies(sha).empty()) index(module_path, fs, own);
    auto library = runtime_library::identify(module_path);
    auto from_bodies = fs->names_from_bodies(sha);
    ModuleNames result;
    for (int64_t address : _starts(module_path)) {
        py::str k = key(module_path, address);
        auto shared_cells = from_bodies.get(k);
        std::optional<std::tuple<py::str, py::str>> shared;
        if (shared_cells) shared = std::make_tuple(engine::native_engine::cell_str(std::get<0>(*shared_cells)), engine::native_engine::cell_str(std::get<1>(*shared_cells)));
        py::str name, source;
        auto o = own.get(address);
        if (o && std::get<1>(*o) == py::str("export")) {
            name = std::get<0>(*o);
            source = std::get<1>(*o);
        } else if (shared && !std::get<1>(*shared).startswith(py::str("the same body"))) {
            name = std::get<0>(*shared);
            source = py::percent(py::str("named body (%s)"), std::get<1>(*shared));
        } else if (o) {
            name = std::get<0>(*o);
            source = std::get<1>(*o);
        } else if (library.contains(address)) {
            name = std::get<0>(library.at(address));
            source = py::str("runtime library: ") + std::get<1>(library.at(address));
        } else if (shared) {
            name = std::get<0>(*shared);
            source = std::get<1>(*shared);
        } else {
            name = py::str("sub_") + k.replace(py::str(":"), py::str("_"));
            source = py::str("(none)");
        }
        result.functions.append(FunctionName{address, k, name, source});
    }
    if (lane && *lane == py::str("ne")) {
        formats::ne::NEFile nef(module_path);
        py::list<py::str> refs = nef.module_refs();
        std::set<std::string> seen;
        for (const auto& seg : nef.segments()) {
            for (const auto& r : nef.relocations(seg)) {
                if (r.kind == py::str("IMPORTNAME") && r.module && 0 < *r.module && *r.module <= refs.size()) {
                    ImportName item{refs[*r.module - 1].upper(), std::nullopt, nef.imported_name(r.name_off.value_or(0)), py::str("imported by name")};
                    std::string id = item.module.utf8() + "\x01" + item.name.utf8();
                    if (!seen.count(id)) {
                        seen.insert(id);
                        result.imports.append(item);
                    }
                }
            }
        }
        auto found = ordinals::imports16(module_path);
        std::vector<std::pair<std::tuple<py::str, int64_t>, std::optional<py::str>>> items;
        for (const auto& kv : found.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
            if (std::get<0>(a.first).u32() != std::get<0>(b.first).u32()) return std::get<0>(a.first).u32() < std::get<0>(b.first).u32();
            return std::get<1>(a.first) < std::get<1>(b.first);
        });
        for (const auto& it : items) {
            const py::str& module = std::get<0>(it.first);
            int64_t o = std::get<1>(it.first);
            const std::optional<py::str>& nm = it.second;
            bool named = nm && !nm->empty();
            result.imports.append(ImportName{module, o, named ? *nm : py::percent(py::str("%s.%d"), module, o),
                                             named ? py::percent(py::str("%s's exports (%s)"), module, ordinals::source(module, o)) : py::str("(none)")});
        }
    } else if (lane && *lane == py::str("pe") && formats::pe::open_pe(module_path).machine == 0x01F2) {
        static const py::re::Pattern console("import(\\d+)_ord_(\\d+)", 0);
        std::vector<std::pair<py::str, py::str>> items;
        for (const auto& kv : source::module_writer::console_import_names(module_path).items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
            if (a.first.u32() != b.first.u32()) return a.first.u32() < b.first.u32();
            return a.second.u32() < b.second.u32();
        });
        for (const auto& it : items) {
            auto m = console.fullmatch(it.first);
            result.imports.append(ImportName{m ? py::percent(py::str("library %s"), m->group(1)) : py::str("?"),
                                             m ? std::optional<int64_t>(py::int_parse(m->group(2), 10)) : std::nullopt, it.second,
                                             py::str("the console's system libraries (symbols/console_ordinals.json)")});
        }
    } else if (lane && *lane == py::str("pe")) {
        std::set<std::pair<std::string, std::string>> uniq;
        for (const auto& kv : formats::pe::open_pe(module_path).imports().items())
            uniq.insert({std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()});
        std::vector<std::pair<py::str, py::str>> items;
        for (const auto& u : uniq) items.emplace_back(py::str(u.first), py::str(u.second));
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
            if (a.first.u32() != b.first.u32()) return a.first.u32() < b.first.u32();
            return a.second.u32() < b.second.u32();
        });
        for (const auto& it : items) {
            py::str module = std::get<0>(py::os::path::splitext(it.first)).upper();
            const py::str& name = it.second;
            if (name.startswith(py::str("#")) && name.slice(1, name.size()).isdigit()) {
                int64_t n = py::int_parse(name.slice(1, name.size()), 10);
                std::optional<py::str> real = ordinals::name(module, n);
                bool has = real && !real->empty();
                result.imports.append(ImportName{module, n, has ? *real : py::percent(py::str("%s.%s"), module, name.slice(1, name.size())),
                                                 has ? py::percent(py::str("%s's exports (%s)"), module, ordinals::source(module, n)) : py::str("(none)")});
            } else {
                result.imports.append(ImportName{module, std::nullopt, name, py::str("imported by name")});
            }
        }
    }
    return result;
}

int64_t main(const py::list<py::str>& argv) {
    // python -m codebreaker names <module> [--set ADDRESS NAME] | --shared | --ordinals FILE...
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker names <module> [--set ADDRESS NAME] | --shared | --ordinals FILE..."));
        return 2;
    }
    if (argv[0] == py::str("--ordinals")) {
        ordinals::collect(argv.slice(1, argv.size()));
        return 0;
    }
    FunctionStorePtr fs = store::functions::open();
    if (argv[0] == py::str("--shared")) {
        auto cells = fs->shared_bodies();
        py::print(py::percent(py::str("%d bodies recorded, %d distinct; %d bodies are in two or more modules (%d functions are such copies)"),
                              engine::native_engine::cell_int(cells.at(0)), engine::native_engine::cell_int(cells.at(1)),
                              engine::native_engine::cell_int(cells.at(2)), engine::native_engine::cell_int(cells.at(3))));
        return 0;
    }
    std::optional<py::str> found_path = base::corpus::retail(argv[0]);
    if (!found_path || found_path->empty()) throw py::SystemExit(py::percent(py::str("no module %r in the corpus"), argv[0]));
    py::str path = *found_path;
    std::optional<py::str> lane = base::corpus::lane(path);
    if (argv.contains(py::str("--set"))) {
        int64_t set_at = argv.index(py::str("--set"));
        py::str text = argv[set_at + 1], name = argv[set_at + 2];       // the address as the listing prints it
        int64_t address;
        if (text.contains(py::str(":"))) {
            auto parts = py::split(text, py::str(":"));
            address = decompile::describe::address(py::int_parse(parts[0], 10), py::int_parse(parts[1], 16));
        } else {
            address = py::int_parse(text, 16);
        }
        std::optional<py::str> digest = set_name(path, address, name, fs);
        py::print(digest ? py::percent(py::str("body %s is now %s"), *digest, name) : py::str("that function has no body worth naming"));
        return digest ? 0 : 1;
    }
    ModuleNames found = of_module(path, fs);
    std::vector<std::pair<py::str, int64_t>> count;
    auto bump = [](std::vector<std::pair<py::str, int64_t>>& v, const py::str& k) {
        for (auto& p : v)
            if (p.first == k) { p.second += 1; return; }
        v.emplace_back(k, 1);
    };
    for (const FunctionName& f : found.functions) {
        py::str shown = f.name.startswith(py::str("?")) ? demangle::readable(f.name) : f.name;
        py::print(py::percent(py::str("%-10s %-48s %s"), f.key, shown.slice(0, 48), f.source));
        bump(count, std::get<0>(std::get<0>(f.source.partition(py::str(" ("))).partition(py::str(":"))));
    }
    std::vector<std::pair<py::str, int64_t>> icount;
    for (const ImportName& i : found.imports) {
        py::str shown = i.name.startswith(py::str("?")) ? demangle::readable(i.name) : i.name;
        py::print(py::percent(py::str("import %-12s %-6s %-40s %s"), i.module, i.ordinal ? py::to_str(*i.ordinal) : py::str(""), shown.slice(0, 40), i.source));
        bump(icount, !i.ordinal ? py::str("by name") : i.source != py::str("(none)") ? py::str("by number, named") : py::str("by number, no name"));
    }
    auto sort_desc = [](std::vector<std::pair<py::str, int64_t>> v) {
        std::stable_sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return -a.second < -b.second; });
        py::list<py::str> parts;
        for (const auto& p : v) parts.append(py::percent(py::str("%s %d"), p.first, p.second));
        return py::join(py::str(", "), parts);
    };
    py::print(py::percent(py::str("%s: %d functions -- %s"), py::os::path::basename(path).lower(), found.functions.size(), sort_desc(count)));
    {
        std::vector<std::pair<py::str, int64_t>> v = icount;
        std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });   // sorted(icount.items())
        py::list<py::str> parts;
        for (const auto& p : v) parts.append(py::percent(py::str("%s %d"), p.first, p.second));
        py::print(py::percent(py::str("%d imports -- %s"), found.imports.size(), py::join(py::str(", "), parts)));
    }
    runtime_library::Compiler info = runtime_library::compiler(path);
    py::print((lane && (*lane == py::str("pe") || *lane == py::str("ne")))
                  ? py::percent(py::str("built with: %s (linker %s)"), info.compiler, info.linker.empty() ? py::str("?") : info.linker)
                  : py::str(""));
    return 0;
}

}  // namespace codebreaker::symbols::names
