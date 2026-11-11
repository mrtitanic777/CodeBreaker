// codebreaker/formats/pe_functions.cpp -- see pe_functions.hpp.

#include "codebreaker/formats/pe_functions.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_set>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::pe_functions {

namespace {

// ---- the decoder: platforms::decoder(32, "x86", detail) ------------------------------------------------------------------
// md.disasm(data, address, count) as the raw array Capstone fills (freed with the run)
struct Run {
    std::shared_ptr<platforms::Decoder> md;
    cs_insn* insn = nullptr;
    size_t n = 0;
    Run(const uint8_t* p, size_t size, int64_t address, size_t count, bool detail) : md(platforms::decoder(32, py::str("x86"), detail)) {
        insn = md->raw(p, size, address, count, n);
    }
    ~Run() { md->release(insn, n); }
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
};

Instruction make_instruction(const cs_insn& c) {
    Instruction out;
    out.address = static_cast<int64_t>(c.address);
    out.size = c.size;
    out.mnemonic = py::str(std::string(c.mnemonic));
    out.op_str = py::str(std::string(c.op_str));
    if (c.detail) {
        const cs_x86& x = c.detail->x86;
        out.disp = x.disp;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        for (int k = 0; k < x.op_count; ++k) {
            const cs_x86_op& o = x.operands[k];
            Operand op;
            op.type = o.type;
            if (o.type == X86_OP_IMM) op.imm = o.imm;
            if (o.type == X86_OP_MEM) {
                op.base = o.mem.base;
                op.index = o.mem.index;
                op.disp = o.mem.disp;
            }
            out.operands.push_back(op);
        }
    }
    return out;
}

// "sub_%x" % t
py::str sub_name(int64_t t) {
    char buf[40];
    std::snprintf(buf, sizeof buf, "sub_%llx", static_cast<unsigned long long>(t));
    return py::str(std::string(buf));
}

// ---- the carve (native/formats/win/pe_functions.cc, which the Python asked) ----------------------------------------------
constexpr size_t kWindow = 1 << 16;

struct Sec {
    int64_t base;
    const std::string* data;
    int64_t end() const { return base + static_cast<int64_t>(data->size()); }
};

// the target of `call imm`: with detail, operands[0] is an immediate; without, the operand text "0x..." is a lone hex number
bool call_target(const cs_insn& c, bool detail, int64_t& t) {
    if (detail) {
        if (std::string(c.mnemonic) != "call") return false;
        if (!c.detail || c.detail->x86.op_count < 1 || c.detail->x86.operands[0].type != X86_OP_IMM) return false;
        t = c.detail->x86.operands[0].imm;
        return true;
    }
    if (std::string(c.mnemonic) != "call") return false;
    std::string op = c.op_str;
    if (op.size() < 3 || op[0] != '0' || op[1] != 'x') return false;
    char* end = nullptr;
    unsigned long long v = std::strtoull(op.c_str() + 2, &end, 16);
    if (*end != 0) return false;
    t = static_cast<int64_t>(v);
    return true;
}

struct Carved {
    std::vector<int64_t> addresses, targets;                                       // (empty without detail)
    std::vector<std::tuple<int64_t, int64_t, std::optional<py::str>>> funcs;       // (start, end, name)
};

// pe_functions.carve(sections, exports, image_base, entry, update_called): the linear sweep, the function starts
// (exports, the entry point, direct-call targets in code, standard prologues) and the extents start-to-next-start
Carved carve_core(const py::list<std::tuple<uint64_t, py::bytes, py::str>>& sections, const py::dict<int64_t, py::str>& exports,
                  int64_t image_base, int64_t entry, bool update_called) {
    std::vector<Sec> secs;
    for (const auto& s : sections) secs.push_back({static_cast<int64_t>(std::get<0>(s)), &std::get<1>(s).raw()});
    auto sec_of = [&](int64_t va) -> const Sec* {
        for (const Sec& s : secs)
            if (s.base <= va && va < s.end()) return &s;
        return nullptr;
    };
    std::unordered_set<int64_t> decoded, called, export_set;
    // the linear sweep of every code section, in windows
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data->size()), off = 0;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(s.data->data());
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, static_cast<int64_t>(kWindow)));
            Run run(p + off, w, s.base + off, 0, update_called);
            if (run.n == 0) {
                off += 1;
                continue;
            }
            for (size_t k = 0; k < run.n; ++k) {
                decoded.insert(static_cast<int64_t>(run.insn[k].address));
                int64_t t;
                if (call_target(run.insn[k], update_called, t)) called.insert(t);
            }
            off = static_cast<int64_t>(run.insn[run.n - 1].address) - s.base + run.insn[run.n - 1].size;
        }
    }
    std::set<int64_t> starts;
    for (int64_t e : exports.keys()) {
        starts.insert(e);
        export_set.insert(e);
    }
    if (entry) starts.insert(image_base + entry);
    for (int64_t t : called)
        if (sec_of(t)) starts.insert(t);
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data->size());
        const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data->data());
        for (int64_t i = 0; i < n - 4; ++i) {
            if (d[i] == 0x55 && d[i + 1] == 0x8b && d[i + 2] == 0xec) starts.insert(s.base + i);
            else if (d[i] == 0x8b && d[i + 1] == 0xff && d[i + 2] == 0x55 && d[i + 3] == 0x8b && d[i + 4] == 0xec) starts.insert(s.base + i);
        }
    }
    // a start the sweep did not decode (it began inside another instruction): decode from it until the sweep's own listing
    std::vector<int64_t> pending;
    for (int64_t s : starts)
        if (!decoded.count(s)) pending.push_back(s);
    for (int64_t start : pending) {
        if (!(export_set.count(start) || called.count(start) || start == image_base + entry)) continue;
        const Sec* sec = sec_of(start);
        if (!sec) continue;
        int64_t n = static_cast<int64_t>(sec->data->size()), off = start - sec->base;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(sec->data->data());
        bool stop = false;
        while (off < n && !stop) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, static_cast<int64_t>(kWindow)));
            Run run(p + off, w, sec->base + off, 0, update_called);
            if (run.n == 0) break;
            for (size_t k = 0; k < run.n; ++k) {
                int64_t addr = static_cast<int64_t>(run.insn[k].address);
                if (decoded.count(addr) && addr != start) {
                    stop = true;
                    break;
                }
                int64_t t;
                if (update_called && call_target(run.insn[k], update_called, t)) called.insert(t);
                decoded.insert(addr);
            }
            if (stop) break;
            off = static_cast<int64_t>(run.insn[run.n - 1].address) - sec->base + run.insn[run.n - 1].size;
        }
    }
    Carved out;
    std::vector<int64_t> order;
    for (int64_t s : starts)
        if (decoded.count(s)) order.push_back(s);
    if (update_called) {
        out.addresses.assign(decoded.begin(), decoded.end());
        std::sort(out.addresses.begin(), out.addresses.end());
        out.targets.assign(called.begin(), called.end());
        std::sort(out.targets.begin(), out.targets.end());
    }
    for (size_t i = 0; i < order.size(); ++i) {
        const Sec* s = sec_of(order[i]);
        int64_t end = s->end();
        if (i + 1 < order.size() && order[i + 1] < end) end = order[i + 1];
        out.funcs.emplace_back(order[i], end, exports.get(order[i]));
    }
    return out;
}

// {export_va -> name} of a PE: the name, or "Ord%d" of an export that has none (a dict: the last name given for an address)
py::dict<int64_t, py::str> exports_of(const pe::PEFile& pe) {
    py::dict<int64_t, py::str> exports;
    for (const auto& e : pe.exports()) {
        const std::optional<py::str>& n = std::get<1>(e);
        int64_t va = static_cast<int64_t>(pe.image_base) + std::get<2>(e);
        exports.set(va, (n && !n->empty()) ? *n : py::str("Ord" + std::to_string(std::get<0>(e))));
    }
    return exports;
}

}  // namespace

void set_capstone_dll(const py::str& path) { emulate::capstone_x86::load_capstone(path); }

// ---- _Insns --------------------------------------------------------------------------------------------------------------
Insns::Insns(const PeCarve* carve, const std::vector<int64_t>& addresses) : carve_(carve) {
    for (int64_t a : addresses) addresses_.add(a);
    sorted_ = addresses;
    std::sort(sorted_.begin(), sorted_.end());
}

bool Insns::contains(int64_t va) const { return addresses_.contains(va); }

int64_t Insns::size() const { return addresses_.size(); }

std::vector<int64_t> Insns::keys() const { return sorted_; }

Record Insns::at(int64_t va) const {
    SecOf sec = carve_->_sec_of(va);
    if (!addresses_.contains(va) || !sec.base) throw py::KeyError(std::to_string(va));
    const std::string& data = sec.data->raw();
    int64_t lo = va - *sec.base;
    if (lo < 0) lo = std::max<int64_t>(0, lo + static_cast<int64_t>(data.size()));     // (data[va - base:...] with a negative start)
    size_t from = static_cast<size_t>(std::min<int64_t>(lo, static_cast<int64_t>(data.size())));
    size_t len = std::min<size_t>(15, data.size() - from);
    Run run(reinterpret_cast<const uint8_t*>(data.data()) + from, len, va, 1, true);
    if (run.n == 0) throw py::StopIteration();
    Record rec;
    rec.ins = make_instruction(run.insn[0]);
    rec.mnemonic = rec.ins.mnemonic;
    rec.op_str = rec.ins.op_str;
    rec.size = rec.ins.size;
    if (rec.mnemonic == py::str("call") && !rec.ins.operands.empty() && rec.ins.operands[0].type == X86_OP_IMM)
        rec.tgt = rec.ins.operands[0].imm;
    return rec;
}

std::optional<Record> Insns::get(int64_t va) const {
    if (!addresses_.contains(va)) return std::nullopt;
    return at(va);
}

std::vector<std::pair<int64_t, Record>> Insns::items() const {
    std::vector<std::pair<int64_t, Record>> out;
    for (int64_t va : sorted_) out.emplace_back(va, at(va));
    return out;
}

std::vector<Record> Insns::values() const {
    std::vector<Record> out;
    for (int64_t va : sorted_) out.push_back(at(va));
    return out;
}

// ---- PeCarve -------------------------------------------------------------------------------------------------------------
PeCarve::PeCarve(const py::str& path, std::function<void(int64_t)> progress_cb) {
    _progress_cb = progress_cb;
    pe = std::make_shared<pe::PEFile>(pe::open_pe(base::corpus::resolve(path)));
    sections = pe->code_sections();
    imports = pe->imports();
    exports = exports_of(*pe);
    // the linear sweep, the function starts and the extents
    Carved c = carve_core(sections, exports, static_cast<int64_t>(pe->image_base), pe->entry, true);
    for (int64_t t : c.targets) call_targets.add(t);
    insn = std::make_shared<Insns>(this, c.addresses);
    for (const auto& f : c.funcs) {
        Func fn;
        fn.start = std::get<0>(f);
        fn.end = std::get<1>(f);
        fn.name = std::get<2>(f);
        funcs.set(fn.start, fn);
        starts.add(fn.start);
    }
    if (progress_cb) {
        int64_t total = 0;
        for (const auto& s : sections) total += std::get<1>(s).size();
        progress_cb(total);
    }
}

SecOf PeCarve::_sec_of(int64_t va) const {
    for (const auto& s : sections) {
        int64_t base = static_cast<int64_t>(std::get<0>(s));
        if (base <= va && va < base + std::get<1>(s).size()) return {base, &std::get<1>(s)};
    }
    return {};
}

std::optional<py::str> PeCarve::resolve_call(const Instruction& ins) const {
    if (!(ins.mnemonic == py::str("call") || ins.mnemonic == py::str("jmp")) || ins.operands.empty()) return std::nullopt;
    const Operand& op = ins.operands[0];
    if (op.type == X86_OP_IMM) {
        int64_t t = op.imm;
        if (exports.contains(t)) return exports.at(t);
        if (funcs.contains(t) || _sec_of(t).base) return sub_name(t);
        return std::nullopt;
    }
    if (op.type == X86_OP_MEM) {
        // absolute [disp] with no base/index = an IAT slot (or a global / loader-connected thunk pointer). capstone
        // gives disp SIGNED, so mask to 32-bit before matching the IAT map. A hit is a named import; a miss is an
        // internal indirect (e.g. the per-module QT_Thunk pointer) and honestly stays unresolved.
        if (op.base == 0 && op.index == 0) {
            auto imp = imports.get(static_cast<uint64_t>(op.disp) & 0xFFFFFFFFull);
            if (imp) return std::get<0>(*imp) + py::str("!") + std::get<1>(*imp);
            return std::nullopt;
        }
    }
    return std::nullopt;
}

py::list<py::str> PeCarve::calls_in(int64_t start) const {
    py::list<py::str> out;
    if (!funcs.contains(start)) return out;
    const Func f = funcs.at(start);
    int64_t va = start;
    while (va < f.end) {
        std::optional<Record> rec = insn->get(va);
        if (!rec) {
            va += 1;
            continue;
        }
        auto nm = resolve_call(rec->ins);
        if (nm && !nm->empty()) out.append(*nm);
        va += rec->size;
    }
    return out;
}

py::list<std::tuple<int64_t, py::str, py::str>> PeCarve::disasm_func(int64_t start) const {
    py::list<std::tuple<int64_t, py::str, py::str>> rows;
    if (!funcs.contains(start)) return rows;
    const Func f = funcs.at(start);
    int64_t va = start;
    while (va < f.end) {
        std::optional<Record> rec = insn->get(va);
        if (!rec) {
            va += 1;
            continue;
        }
        rows.append(std::make_tuple(va, rec->mnemonic, rec->op_str));
        va += rec->size;
    }
    return rows;
}

py::list<std::tuple<int64_t, int64_t>> PeCarve::_image_ranges() const {
    if (imgr_) return *imgr_;
    py::list<std::tuple<int64_t, int64_t>> r;
    int64_t image_base = static_cast<int64_t>(pe->image_base);
    for (const auto& s : pe->sections)
        r.append(std::make_tuple(image_base + s.vaddr, image_base + s.vaddr + std::max(s.vsize, s.rawsize)));
    imgr_ = r;
    return r;
}

bool PeCarve::_in_image(int64_t va) const {
    for (const auto& r : _image_ranges()) {
        if (std::get<0>(r) <= va && va < std::get<1>(r)) return true;
    }
    return false;
}

const py::dict<int64_t, py::list<int64_t>>& PeCarve::data_refs() const {
    if (drefs_) return *drefs_;
    std::vector<int64_t> fstarts = funcs.keys();
    std::sort(fstarts.begin(), fstarts.end());
    py::dict<int64_t, int64_t> fend;
    for (int64_t s : fstarts) fend.set(s, funcs.at(s).end);
    py::dict<int64_t, py::set<int64_t>> out;
    for (const auto& kv : insn->items()) {
        int64_t addr = kv.first;
        const Record& rec = kv.second;
        // bisect.bisect_right(starts, addr) - 1
        int64_t i = static_cast<int64_t>(std::upper_bound(fstarts.begin(), fstarts.end(), addr) - fstarts.begin()) - 1;
        if (i < 0) continue;
        int64_t fs = fstarts[static_cast<size_t>(i)];
        if (addr >= fend.at(fs)) continue;
        const std::string mn = rec.mnemonic.utf8();
        bool branch = mn == "call" || mn == "jmp" || mn.compare(0, 1, "j") == 0;
        for (const Operand& op : rec.ins.operands) {
            std::optional<int64_t> va;
            if (op.type == X86_OP_IMM) {
                if (branch) continue;
                va = static_cast<int64_t>(static_cast<uint64_t>(op.imm) & 0xFFFFFFFFull);
            } else if (op.type == X86_OP_MEM) {
                if (op.base == 0 && op.index == 0) {
                    uint64_t cand = static_cast<uint64_t>(op.disp) & 0xFFFFFFFFull;
                    if (imports.contains(cand)) continue;
                    va = static_cast<int64_t>(cand);
                }
            }
            if (va && _in_image(*va)) out.setdefault(fs, py::set<int64_t>()).add(*va);
        }
    }
    py::dict<int64_t, py::list<int64_t>> result;
    for (const auto& kv : out.items()) {
        std::vector<int64_t> v(std::get<1>(kv).begin(), std::get<1>(kv).end());
        std::sort(v.begin(), v.end());
        result.set(std::get<0>(kv), py::list<int64_t>(v));
    }
    drefs_ = result;
    return *drefs_;
}

py::list<int64_t> PeCarve::xrefs_to(int64_t va) const {
    if (!drev_) {
        py::dict<int64_t, py::list<int64_t>> rev;
        for (const auto& kv : data_refs().items()) {
            for (int64_t v : std::get<1>(kv)) rev.setdefault(v, py::list<int64_t>()).append(std::get<0>(kv));
        }
        drev_ = rev;
    }
    std::vector<int64_t> v = drev_->contains(va) ? drev_->at(va).vec() : std::vector<int64_t>();
    std::sort(v.begin(), v.end());
    return py::list<int64_t>(v);
}

py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> PeCarve::string_refs(const py::dict<int64_t, py::str>& va2text) const {
    py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> out;
    for (const auto& kv : data_refs().items()) {
        py::list<std::tuple<int64_t, py::str>> hits;
        for (int64_t v : std::get<1>(kv))
            if (va2text.contains(v)) hits.append(std::make_tuple(v, va2text.at(v)));
        if (!hits.empty()) out.set(std::get<0>(kv), hits);
    }
    return out;
}

// ---- module level --------------------------------------------------------------------------------------------------------
namespace {

// settings.data_root(): WT_DATA_ROOT when it holds era dirs (or is a directory), else the checkout's parent
py::str data_root_() {
    static const char* const eras[] = {"win95", "win98", "win3.1", "win1.0"};
    auto env = py::os::environ_::get(py::str("WT_DATA_ROOT"));
    if (env && !env->empty()) {
        for (const char* e : eras)
            if (py::os::path::isdir(py::os::path::join(*env, py::str(e)))) return *env;
        if (py::os::path::isdir(*env)) return *env;
    }
    return py::os::path::dirname(py::settings::checkout_root());
}

}  // namespace

void ensure_target() {
    std::vector<py::str> roots;
    auto env = py::os::environ_::get(py::str("WT_DATA_ROOT"));
    if (env && !env->empty()) roots.push_back(py::os::path::join(*env, py::str("win95")));
    roots.push_back(data_root_());
    roots.push_back(py::os::getcwd());
    for (const py::str& r : roots) {
        if (py::os::path::isdir(py::os::path::join(r, py::str("files"))) || py::os::path::isdir(py::os::path::join(r, py::str("src")))) {
            py::os::chdir(r);
            return;
        }
    }
}

std::shared_ptr<PeCarve> carve(const py::str& module) {
    static py::dict<py::str, std::shared_ptr<PeCarve>> kept;       // _CARVE
    ensure_target();
    if (!kept.contains(module)) kept.set(module, std::make_shared<PeCarve>(base::corpus::resolve(module)));
    return kept.at(module);
}

py::list<std::tuple<int64_t, py::str>> disasm_whole32(const py::bytes& code, int64_t base, std::function<void(int64_t)> progress_cb) {
    py::list<std::tuple<int64_t, py::str>> rows;
    const std::string& raw = code.raw();
    int64_t n = static_cast<int64_t>(raw.size()), pos = 0;
    const uint8_t* p = reinterpret_cast<const uint8_t*>(raw.data());
    while (pos < n) {
        Run run(p + pos, static_cast<size_t>(std::min<int64_t>(15, n - pos)), base + pos, 1, false);
        if (run.n == 0) {
            rows.append(std::make_tuple(base + pos, py::str("(bad)")));
            pos += 1;
        } else {
            std::string mn = run.insn[0].mnemonic, op = run.insn[0].op_str;
            rows.append(std::make_tuple(base + pos, py::str(op.empty() ? mn : mn + " " + op)));
            pos += run.insn[0].size;
        }
    }
    if (progress_cb) progress_cb(static_cast<int64_t>(raw.size()));
    return rows;
}

PeSeg::PeSeg(const py::str& module_, std::optional<py::str> section, std::shared_ptr<PeCarve> carve_, std::function<void(int64_t)> progress_cb) {
    module = module_;
    path = base::corpus::resolve(module);
    pe = std::make_shared<pe::PEFile>(pe::open_pe(path));
    base = pe->image_base;
    auto secs = pe->code_sections();
    if (secs.empty()) throw py::ValueError(module.utf8() + ": no executable section");
    std::tuple<uint64_t, py::bytes, py::str> pick;
    if (!section) {                                // default: the largest .text-like
        bool found = false;
        for (const auto& s : secs)
            if (std::get<2>(s) == py::str(".text")) {
                pick = s;
                found = true;
                break;
            }
        if (!found) {
            pick = secs[0];
            for (const auto& s : secs)
                if (std::get<1>(s).size() > std::get<1>(pick).size()) pick = s;      // max(): the first of the longest
        }
    } else {
        bool found = false;
        for (const auto& s : secs)
            if (std::get<2>(s) == *section) {
                pick = s;
                found = true;
                break;
            }
        if (!found) throw py::ValueError(module.utf8() + ": no section " + py::repr(*section).utf8());
    }
    va = static_cast<int64_t>(std::get<0>(pick));
    code = std::get<1>(pick);
    name = std::get<2>(pick);
    secva = va;
    rows = disasm_whole32(code, 0, progress_cb);
    int64_t idx = 0;
    for (const auto& r : rows) off2idx.set(std::get<0>(r), idx++);

    // carve functions with PeCarve (capstone, export-table driven), then keep those that fall inside THIS section and
    // rebase VA -> section offset. Reuse a caller-supplied carve when given (avoids re-sweeping every section of a
    // multi-section module).
    if (!carve_) carve_ = std::make_shared<PeCarve>(path);
    int64_t lo = va, hi = va + code.size();
    for (const auto& kv : carve_->funcs.items()) {
        int64_t fva = std::get<0>(kv);
        const Func& f = std::get<1>(kv);
        if (lo <= fva && fva < hi) {
            int64_t s = fva - va;
            int64_t e = std::min(f.end, hi) - va;
            Func nf;
            nf.start = s;
            nf.end = e;
            nf.name = f.name;
            funcs.set(s, nf);
            if (f.name && !f.name->empty()) export_names.set(s, *f.name);
        }
    }
    _carve = carve_;

    // base-relocation mask, section-relative (each HIGHLOW covers 4 bytes)
    int64_t rvbase = va - static_cast<int64_t>(base);
    for (int64_t rva : pe->base_relocs()) {
        int64_t o = rva - rvbase;
        if (0 <= o && o < code.size()) {
            for (int64_t k = 0; k < 4; ++k)
                if (o + k < code.size()) mask.add(o + k);
        }
    }
    // fixups: resolved call sites for annotate (site_off -> "DLL!name"); filled by annotate
}

py::dict<int64_t, std::tuple<int64_t, std::optional<py::str>>> function_spans(const py::str& path) {
    pe::PEFile p = pe::open_pe(base::corpus::resolve(path));
    py::dict<int64_t, py::str> exports = exports_of(p);
    Carved c = carve_core(p.code_sections(), exports, static_cast<int64_t>(p.image_base), p.entry, false);
    py::dict<int64_t, std::tuple<int64_t, std::optional<py::str>>> out;
    for (const auto& f : c.funcs) out.set(std::get<0>(f), std::make_tuple(std::get<1>(f), std::get<2>(f)));
    return out;
}

}  // namespace codebreaker::formats::pe_functions
