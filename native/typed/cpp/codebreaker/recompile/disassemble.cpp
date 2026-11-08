// codebreaker/recompile/disassemble.cpp -- typed C++ port of codebreaker.recompile.disassemble.

#include "codebreaker/recompile/disassemble.hpp"

#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

#include <capstone/capstone.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "codebreaker/decompile/vmx128.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"

namespace codebreaker::recompile::disassemble {

const py::set<py::str> _ENDERS{py::str("b"), py::str("ba"), py::str("blr"), py::str("bctr"), py::str("bctrl"), py::str("rfi"), py::str("rfid")};

namespace {

// ---- the PowerPC capstone decoder (platforms.decoder(32, arch="ppc", detail=True)), one handle a thread ---------
struct CsLib {
    HMODULE module = nullptr;
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
    bool tried = false;
    bool ok = false;
};

CsLib& cs_lib() {
    static CsLib lib;
    return lib;
}
std::mutex& mu() {
    static std::mutex m;
    return m;
}
std::wstring& dll_path() {
    static std::wstring p;
    return p;
}

template <class F> bool sym_(HMODULE m, const char* name, F& out) {
    FARPROC p = GetProcAddress(m, name);
    out = reinterpret_cast<F>(p);
    return p != nullptr;
}

// load the DLL once: Capstone is "installed" when it does
const CsLib& cs_ready() {
    std::lock_guard<std::mutex> lock(mu());
    CsLib& lib = cs_lib();
    if (lib.tried) return lib;
    lib.tried = true;
    if (dll_path().empty()) return lib;
    lib.module = LoadLibraryW(dll_path().c_str());
    if (!lib.module) return lib;
    lib.ok = sym_(lib.module, "cs_open", lib.open) && sym_(lib.module, "cs_option", lib.option) && sym_(lib.module, "cs_disasm", lib.disasm) &&
             sym_(lib.module, "cs_free", lib.free_);
    return lib;
}

// this thread's handle, or 0 when there is no decoder
csh handle_() {
    static thread_local csh h = 0;
    if (h) return h;
    const CsLib& lib = cs_ready();
    if (!lib.ok) return 0;
    if (lib.open(CS_ARCH_PPC, static_cast<cs_mode>(CS_MODE_32 | CS_MODE_BIG_ENDIAN), &h) != CS_ERR_OK) {
        h = 0;
        return 0;
    }
    lib.option(h, CS_OPT_DETAIL, CS_OPT_ON);
    return h;
}

bool in_group(const cs_detail* d, uint8_t group) {
    for (int i = 0; i < d->groups_count; ++i) {
        if (d->groups[i] == group) return true;
    }
    return false;
}

}  // namespace

void set_capstone_dll(const py::str& path) {
    const std::string u = path.utf8();
    const int n = MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(n > 0 ? n - 1 : 0), L'\0');
    if (n > 0) MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, &w[0], n);
    std::lock_guard<std::mutex> lock(mu());
    dll_path() = w;
    cs_lib().tried = false;
}

bool _md() { return handle_() != 0; }

bool is_vmx128(const py::bytes& raw4) {
    const int64_t op = raw4[0] >> 2;
    return op == 4 || op == 5 || op == 6;
}

std::optional<Insn> _decode_one(const py::bytes& code, int64_t off, int64_t vaddr) {
    const csh h = handle_();
    if (!h || off + WORD > code.size()) return std::nullopt;
    const py::bytes four = code.slice(off, off + WORD);
    cs_insn* insn = nullptr;
    const size_t n = cs_lib().disasm(h, reinterpret_cast<const uint8_t*>(four.raw().data()), static_cast<size_t>(four.size()),
                                     static_cast<uint64_t>(vaddr + off), 1, &insn);
    if (n == 0) return std::nullopt;
    Insn out;
    out.address = static_cast<int64_t>(insn->address);
    out.mnemonic = py::str(std::string(insn->mnemonic));
    out.op_str = py::str(std::string(insn->op_str));
    if (insn->detail) {
        out.jump = in_group(insn->detail, CS_GRP_JUMP);
        out.call = in_group(insn->detail, CS_GRP_CALL);
        const cs_ppc& ppc = insn->detail->ppc;
        for (int i = 0; i < ppc.op_count; ++i) {
            if (ppc.operands[i].type == PPC_OP_IMM) out.imm.append(static_cast<int64_t>(ppc.operands[i].imm));
        }
    }
    cs_lib().free_(insn, n);
    return out;
}

py::set<int64_t> classify(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries) {
    py::set<int64_t> covered;
    if (!_md()) return covered;
    const int64_t n = code.size();
    py::set<int64_t> seen;
    std::vector<int64_t> work;
    for (int64_t e : entries) work.push_back(e - base_vaddr);
    auto cover = [&](int64_t off) {
        for (int64_t k = off; k < std::min(off + WORD, n); ++k) covered.add(k);
    };
    while (!work.empty()) {
        const int64_t off = work.back();
        work.pop_back();
        if (seen.contains(off) || !(0 <= off && off < n) || off % WORD) continue;
        seen.add(off);
        const std::optional<Insn> ins = _decode_one(code, off, base_vaddr);
        if (!ins) {
            if (is_vmx128(code.slice(off, off + WORD))) {     // vector code: falls through, never branches
                cover(off);
                work.push_back(off + WORD);
            }
            continue;
        }
        cover(off);
        if (ins->jump || ins->call) {
            for (int64_t imm : ins->imm) {
                if (0 <= imm - base_vaddr && imm - base_vaddr < n) work.push_back(imm - base_vaddr);
            }
        }
        if (!_ENDERS.contains(ins->mnemonic)) work.push_back(off + WORD);
    }
    return covered;
}

py::list<std::tuple<int64_t, int64_t>> data_spans(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries, int64_t minrun) {
    const py::set<int64_t> covered = classify(code, base_vaddr, entries);
    py::list<std::tuple<int64_t, int64_t>> out;
    int64_t i = 0;
    const int64_t n = code.size();
    while (i < n) {
        if (covered.contains(i)) {
            i += 1;
            continue;
        }
        int64_t j = i;
        while (j < n && !covered.contains(j)) j += 1;
        if (j - i >= minrun) out.append(std::make_tuple(i, j));
        i = j;
    }
    return out;
}

py::list<std::tuple<int64_t, py::bytes, py::str, py::str>> listing(const py::bytes& code, int64_t base_vaddr, int64_t start,
                                                                  std::optional<int64_t> count) {
    py::list<std::tuple<int64_t, py::bytes, py::str, py::str>> out;
    int64_t off = start - (((start % WORD) + WORD) % WORD), emitted = 0;   // start - start % WORD (floor modulo)
    while (off + WORD <= code.size()) {
        const py::bytes raw = code.slice(off, off + WORD);
        int64_t word = 0;
        for (int64_t b : raw) word = (word << 8) | b;                // int.from_bytes(raw, "big")
        const auto vec = decompile::vmx128::decode(word);            // first: Capstone reads these as other instructions (ISA 2.07/3.0)
        if (vec) {
            out.append(std::make_tuple(base_vaddr + off, raw, std::get<0>(*vec), std::get<1>(*vec)));
        } else {
            const std::optional<Insn> ins = _decode_one(code, off, base_vaddr);
            if (ins) {
                out.append(std::make_tuple(base_vaddr + off, raw, ins->mnemonic, ins->op_str));
            } else {
                out.append(std::make_tuple(base_vaddr + off, raw, py::str(".long"), py::percent(py::str("0x%08X"), word)));
            }
        }
        off += WORD;
        emitted += 1;
        if (count && emitted >= *count) return out;
    }
    return out;
}

std::tuple<py::bytes, int64_t, int64_t, py::list<std::tuple<py::str, int64_t, int64_t, bool>>> image(const py::str& path) {
    const auto img = formats::powerpc_image::read(formats::xex::is_xex(path) ? formats::xex::basefile(path) : path);
    if (img->spans.empty()) throw py::ValueError("max() iterable argument is empty");
    int64_t top = 0;
    bool first = true;
    for (const auto& s : img->spans) {
        const int64_t end = std::get<0>(s) + std::get<2>(s);
        if (first || end > top) top = end;
        first = false;
    }
    std::string flat(static_cast<size_t>(top - img->base), '\0');
    for (const auto& s : img->spans) {
        const int64_t va = std::get<0>(s), off = std::get<1>(s), n = std::get<2>(s);
        const py::bytes piece = img->data.slice(off, off + n);
        // flat[va - base:va - base + n] = piece (a shorter piece shortens the image, as a bytearray slice assignment does)
        const int64_t lo = std::min<int64_t>(std::max<int64_t>(va - img->base, 0), static_cast<int64_t>(flat.size()));
        const int64_t hi = std::min<int64_t>(std::max<int64_t>(va - img->base + n, lo), static_cast<int64_t>(flat.size()));
        flat.replace(static_cast<size_t>(lo), static_cast<size_t>(hi - lo), piece.raw());
    }
    py::list<std::tuple<py::str, int64_t, int64_t, bool>> sections;
    for (const auto& s : img->sections) {
        const py::str& name = std::get<0>(s);
        sections.append(std::make_tuple(name, std::get<1>(s), std::get<2>(s), (std::get<3>(s) & 0x20000000) != 0 || name == py::str(".text")));
    }
    return {py::bytes(flat), img->base, img->entry ? img->entry : std::get<0>(img->code), sections};
}

std::tuple<py::bytes, int64_t, int64_t> code_section(const py::str& path) {
    const auto img = formats::powerpc_image::read(formats::xex::is_xex(path) ? formats::xex::basefile(path) : path);
    const int64_t lo = std::get<0>(img->code), hi = std::get<1>(img->code);
    return {img->bytes_at(lo, hi - lo), lo, img->entry ? img->entry : lo};
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(py::str("disassemble.py -- Xbox 360 PowerPC as (mnemonic, operands) text, for the recompiler."));   // __doc__.split("\n\n")[0]
        return 2;
    }
    if (!_md()) {
        py::print(py::str("capstone is not installed (pip install capstone)"));
        return 2;
    }
    int64_t limit = 60;
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == py::str("--limit")) {
            if (i + 1 >= argv.size()) throw py::IndexError("list index out of range");
            limit = py::int_parse(argv[i + 1]);
            break;
        }
    }
    const auto [body, base, entry] = code_section(argv[0]);
    py::print(py::percent(py::str("%s  code at 0x%08X, entry 0x%08X, %d bytes"), py::os::path::basename(argv[0]), base, entry, body.size()));
    const py::set<int64_t> covered = classify(body, base, py::list<int64_t>{entry, base});
    for (const auto& row : listing(body, base, 0, limit)) {
        const int64_t va = std::get<0>(row);
        py::print(py::percent(py::str("  %s %08X  %s  %-10s %s"), covered.contains(va - base) ? py::str(" ") : py::str("d"), va,
                              std::get<1>(row).hex(), std::get<2>(row), std::get<3>(row)));
    }
    int64_t data = 0;
    for (const auto& se : data_spans(body, base, py::list<int64_t>{entry, base})) data += std::get<1>(se) - std::get<0>(se);
    py::print(py::percent(py::str("  --- %d bytes, %d read as data (%.1f%%)"), body.size(), data,
                          100.0 * static_cast<double>(data) / static_cast<double>(std::max<int64_t>(1, body.size()))));
    return 0;
}

}  // namespace codebreaker::recompile::disassemble
