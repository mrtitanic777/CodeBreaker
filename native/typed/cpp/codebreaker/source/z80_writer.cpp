// codebreaker/source/z80_writer.cpp -- typed C++ of codebreaker.source.z80_writer.

#include "codebreaker/source/z80_writer.hpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "codebreaker/build/z80_asm.hpp"

namespace codebreaker::source::z80_writer {

namespace z80 = codebreaker::decompile::z80;
namespace w = codebreaker::source::image_writer;
using Insn = z80::Insn;

std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> walk(const py::bytes& image, int64_t origin,
                                                                              const py::list<int64_t>& entries) {
    int64_t end = origin + py::len(image);
    py::bytearray mem = py::bytearray::zeros(0x10000);
    mem.set_slice(origin, end, image.slice_to(0x10000 - origin));
    const py::bytes& memory = mem;
    py::list<int64_t> inside_entries;
    for (int64_t a : entries) {
        if (origin <= a && a < end) {
            inside_entries.append(a);
        }
    }
    return w::walk<Insn>(
        inside_entries, [&](int64_t pc) { return z80::decode(memory, pc, pc); },
        [&](int64_t pc) { return origin <= pc && pc < end; }, [&](int64_t pc, const Insn& insn) { return pc + insn.size <= end; },
        [&](const Insn& insn) { return origin <= *insn.target && *insn.target < end && py::truthy(insn.flow); },
        [&](const Insn& insn) { return insn.flow == py::str("jump") || insn.flow == py::str("ret") || insn.flow == py::str("indirect"); },
        nullptr,
        [&](const Insn& insn) {
            return (insn.flow == py::str("call") || insn.flow == py::str("ccall")) && !insn.pattern.startswith(py::str("RST"));
        });
}

std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> walk(const py::bytes& image, int64_t origin,
                                                                              const py::dict<int64_t, py::str>& entries) {
    return walk(image, origin, py::list<int64_t>::from(entries.keys()));
}

Written write(const py::bytes& image, int64_t origin, const py::dict<int64_t, py::str>& entries, const py::str& folder, const py::str& stem,
              const py::dict<int64_t, py::str>& ports, const py::dict<int64_t, py::str>& names, const py::list<py::str>& header) {
    py::os::makedirs(folder, 0777, true);
    auto [code, targets, starts] = walk(image, origin, entries);
    int64_t end = origin + py::len(image);
    py::dict<int64_t, py::str> fnames = entries.copy();
    py::dict<int64_t, w::Function> functions = w::functions(starts, fnames, py::str("sub_%04X"));
    py::set<int64_t> covered = w::covered<Insn>(code);

    auto line_at = [&](int64_t a) { return origin <= a && a < end && !covered.contains(a); };
    py::dict<int64_t, py::str> labels = w::labels(targets, fnames, line_at, py::str("L_%04X"));
    const py::list<py::str> pointer{py::str("LD HL,nn"), py::str("LD DE,nn"), py::str("LD IX,nn"), py::str("LD IY,nn"), py::str("LD SP,nn")};
    py::dict<int64_t, py::str> used_names, used_ports;
    for (const Insn& insn : code.values()) {
        std::optional<int64_t> v;
        if (py::truthy(insn.values)) {
            v = insn.values[-1];
        }
        if ((insn.pattern == py::str("OUT (n),A") || insn.pattern == py::str("IN A,(n)")) && ports.contains(insn.values[0])) {
            used_ports.set(insn.values[0], ports.at(insn.values[0]));
        }
        if (!v.has_value()) {
            continue;
        }
        if (!(origin <= *v && *v < end) && names.contains(*v) && (insn.pattern.contains(py::str("nn")) || py::truthy(insn.flow))) {
            used_names.set(*v, names.at(*v));
        } else if (line_at(*v) && (pointer.contains(insn.pattern) || (insn.flow == py::str("") && insn.pattern.contains(py::str("(nn)"))))) {
            labels.setdefault(*v, code.contains(*v) ? fnames.get(*v, py::percent("L_%04X", *v)) : py::percent("D_%04X", *v));
        }
    }
    py::list<py::str> head{py::percent("; %s.asm -- written by CodeBreaker", stem)};
    for (const py::str& h : header) {
        head.append(py::str("; ") + h);
    }
    head.append(py::str(""));
    w::Assembly out = w::lines(head, py::str("sjasmplus"), stem);
    std::vector<int64_t> port_keys = used_ports.keys();
    std::sort(port_keys.begin(), port_keys.end());
    for (int64_t p : port_keys) {
        out.append(py::percent("%-16s EQU $%02X", used_ports.at(p), p));
    }
    std::vector<int64_t> name_keys = used_names.keys();
    std::sort(name_keys.begin(), name_keys.end());
    for (int64_t a : name_keys) {
        out.append(py::percent("%-16s EQU $%04X", used_names.at(a), a));
    }
    if (py::truthy(used_ports) || py::truthy(used_names)) {
        out.append(py::str(""));
    }
    out.append(py::percent("        ORG $%04X", origin));
    py::dict<int64_t, py::str> text_names = labels.copy();
    text_names.update(used_names);

    auto text = [&](const Insn& insn) -> py::str {
        py::str t = z80::text(insn, text_names);
        if (t.startswith(py::str(".db "))) {   // bytes an assembler would not write so (z80.text), in this syntax
            t = py::str("DEFB ") + t.slice_from(4);
        }
        if ((insn.pattern == py::str("OUT (n),A") || insn.pattern == py::str("IN A,(n)")) && used_ports.contains(insn.values[0])) {
            t = t.replace(py::percent("($%02X)", insn.values[0]), py::percent("(%s)", used_ports.at(insn.values[0])));
        }
        return t;
    };
    w::emit<Insn>(
        out, origin, end, [&](int64_t a) { return image[a - origin]; }, [](int64_t a) { return a; },
        [&](int64_t a, int64_t /*addr*/) { return code.get(a); }, labels, functions, text,
        [](const py::list<int64_t>& data) { return w::data_lines(data, py::str("DEFB"), py::str("DEFS %d,$%02X")); });
    py::str source = w::text(out) + py::str("\n");
    py::dict<py::str, w::Extra> more;
    more.set(py::str("bytes"), w::Extra(py::len(image)));
    Written r;
    static_cast<w::Result&>(r) = w::result<Insn>(folder, stem + py::str(".asm"), source, functions, code, nullptr, more);
    int64_t placed_at = 0;
    auto assemble = [&]() -> py::bytes {
        auto [at, built_bytes] = build::z80_asm::build_flat(source, folder);
        placed_at = at;
        return built_bytes;
    };
    std::optional<py::bytes> built = w::assembled(
        r, assemble, image, std::nullopt, py::str("block"),
        [&](const py::bytes& b) { return placed_at == origin && b == image; });
    if (built.has_value()) {
        r.built_image = built;
    }
    return r;
}

}  // namespace codebreaker::source::z80_writer
