// codebreaker/formats/xex_edit.cpp -- see xex_edit.hpp.

#include "codebreaker/formats/xex_edit.hpp"

#include <random>

#include "codebreaker/build/ppc_asm.hpp"
#include "codebreaker/formats/native_ask.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::formats::xex_edit {

namespace {

namespace na = native_ask;

// struct.unpack_from(">I", b, at)[0]
int64_t unpack_be32(const py::bytes& b, int64_t at) { return py::unpack_from1<int64_t>(">I", b, at); }

// tempfile.mkstemp(suffix=".xex") + os.close(fd): the path of a new, empty file in the temporary folder
py::str mkstemp_path(const py::str& suffix) {
    static const char* chars = "abcdefghijklmnopqrstuvwxyz0123456789_";
    static std::mt19937_64 rng{std::random_device{}()};
    for (;;) {
        std::string name = "tmp";
        for (int k = 0; k < 8; ++k) name += chars[rng() % 37];
        py::str path = py::os::path::join(py::tempfile::gettempdir(), py::str(name) + suffix);
        if (py::os::path::exists(path)) continue;
        py::file f = py::open(path, py::str("wb"));
        f.close();
        return path;
    }
}

py::str hex8(int64_t v) { return py::percent(py::str("%08X"), v); }

}  // namespace

const py::str DOC(
    "xex_edit.py -- change an Xbox 360 executable: its privilege flags, or its code at an address.\n"
    "\n"
    "    codebreaker xex-edit privileges <in.xex>                         the privilege bits that are set\n"
    "    codebreaker xex-edit privileges <in.xex> <+bit|-bit ...> <out>   set (+) or clear (-) bits; `pal50`\n"
    "                                                                     names bit 10, PAL-50 incompatible\n"
    "    codebreaker xex-edit nop   <in.xex> <address> [count] <out>      count instructions become nop\n"
    "    codebreaker xex-edit bytes <in.xex> <address> <hex> <out>        raw bytes over the code\n"
    "    codebreaker xex-edit asm   <in.xex> <address> \"<asm>\" <out>      PowerPC assembled there\n"
    "    codebreaker xex-edit hook  <in.xex> <address> <cave> \"<asm>\" <out>\n"
    "                                                                     `b cave` at address; at the cave\n"
    "                                                                     the asm, the displaced instruction,\n"
    "                                                                     and `b address+4`\n"
    "    codebreaker xex-edit asm-only \"<asm>\" [address]                  the bytes, nothing written\n"
    "\n"
    "Privileges live in the header's privilege entries (key 0x30000: bits 0-31; 0x30100: bits 32-63),\n"
    "changed where they stand. A code edit writes the module as an unencrypted XEX holding the whole\n"
    "image in one block (xex.basic) and changes the image there; that is what the recompiler, the Xbox\n"
    "360 runtime and emulators read. Several asm lines are separated by `;` or newlines. The header's\n"
    "hashes and signature are not redone: `codebreaker xex compress` / `machine` re-pack and re-sign.\n"
    "Only bit 10 is named here: the meaning of the other bits is not settled from a source this\n"
    "project trusts, so they are given by number.\n");
const py::dict<py::str, int64_t> NAMED_BITS = {{py::str("pal50"), 10}};
const py::bytes NOP("\x60\x00\x00\x00", 4);

std::tuple<py::list<int64_t>, py::dict<int64_t, int64_t>> privileges(const py::bytes& data) {
    return na::ask<std::tuple<py::list<int64_t>, py::dict<int64_t, int64_t>>>("xex_edit.privileges", data, na::no_own);
}

py::bytes set_privileges(const py::bytes& data, const py::list<std::tuple<int64_t, bool>>& changes) {
    return na::ask<py::bytes>("xex_edit.set_privileges", data, na::no_own, changes);
}

std::tuple<int64_t, bool> _parse_bit(const py::str& token) {
    if (token.empty()) throw py::IndexError("string index out of range");
    py::str sign = token.slice(0, 1), name = token.slice(1, token.size()).lower();
    if (!(sign == py::str("+") || sign == py::str("-"))) throw py::ValueError(py::percent(py::str("a change is +bit or -bit: %r"), token));
    int64_t bit = NAMED_BITS.contains(name) ? NAMED_BITS.at(name) : py::int_parse(name, 0);
    if (!(0 <= bit && bit < 64)) throw py::ValueError(py::percent(py::str("privilege bits are 0-63: %r"), token));
    return std::make_tuple(bit, sign == py::str("+"));
}

Image::Image(const py::str& path) {
    xex::Header h = xex::header(path);
    py::str tmp = mkstemp_path(py::str(".xex"));
    try {
        xex::basic(path, tmp);
        data = py::open_rb(tmp).read();
    } catch (...) {
        py::os::remove(tmp);
        throw;
    }
    py::os::remove(tmp);
    base = h.base;                                                  // (None when the header names no image base: offset() then fails)
    pe_offset = unpack_be32(data, 8);
}

int64_t Image::offset(int64_t address, int64_t size) const {
    if (!base) throw py::TypeError("unsupported operand type(s) for -: 'int' and 'NoneType'");
    int64_t off = pe_offset + address - *base;
    if (address < *base || off + size > py::len(data))
        throw py::ValueError(py::percent(py::str("%08X+%X is outside the image (%08X, %X bytes)"), address, size, *base, py::len(data) - pe_offset));
    return off;
}

py::bytes Image::read(int64_t address, int64_t size) const {
    int64_t off = offset(address, size);
    return data.slice(off, off + size);
}

void Image::write(int64_t address, const py::bytes& blob) {
    int64_t off = offset(address, blob.size());
    std::string s = data.raw();
    s.replace(size_t(off), size_t(blob.size()), blob.raw());     // self.data[off:off + len(blob)] = blob
    data = py::bytes(std::move(s));
}

void Image::save(const py::str& out) const {
    py::file f = py::open(out, py::str("wb"));
    f.write(data);
    f.close();
}

bool _relative_branch(int64_t word) {
    int64_t op = word >> 26;
    return (op == 18 && !(word & 2)) || (op == 16 && !(word & 2));
}

void hook(Image& image, int64_t address, int64_t cave, const py::str& asm_text) {
    py::bytes stolen = image.read(address, 4);
    int64_t word = unpack_be32(stolen, 0);
    if (_relative_branch(word))
        throw py::ValueError(py::percent(py::str("the instruction at %08X is a relative branch (%08X): it cannot be moved to the cave"), address, word));
    py::bytes body = build::ppc_asm::assemble(asm_text, cave);
    auto back = build::ppc_asm::assemble_one(py::percent(py::str("b 0x%X"), address + 4), cave + py::len(body) + 4);
    if (!back) throw py::TypeError("required argument is not an integer");
    image.write(cave, body + stolen + py::pack(">I", *back));
    auto jump = build::ppc_asm::assemble_one(py::percent(py::str("b 0x%X"), cave), address);
    if (!jump) throw py::TypeError("required argument is not an integer");
    image.write(address, py::pack(">I", *jump));
}

py::str _asm_text(const py::str& text) { return text.replace(py::str(";"), py::str("\n")); }

int64_t main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    if (argv.empty() || argv[0] == S("-h") || argv[0] == S("--help")) {
        py::print(DOC);
        return argv.empty() ? 2 : 0;
    }
    py::str op = argv[0];
    py::list<py::str> args;
    for (int64_t k = 1; k < argv.size(); ++k) args.append(argv[k]);
    try {
        if (op == S("asm-only")) {
            int64_t address = py::len(args) > 1 ? py::int_parse(args[1], 0) : 0;
            py::print(build::ppc_asm::assemble(_asm_text(args[0]), address).hex());
            return 0;
        }
        if (op == S("privileges")) {
            py::bytes data = py::open_rb(args[0]).read();
            if (py::len(args) == 1) {
                auto bits = std::get<0>(privileges(data));
                py::list<py::str> parts;
                for (int64_t b : bits) {
                    py::str named;
                    for (const auto& kv : NAMED_BITS.items())
                        if (std::get<1>(kv) == b) named = py::percent(S(" (%s)"), std::get<0>(kv));   // names = {v: k ...}: the last name of a bit
                    parts.append(py::percent(S("%d%s"), b, named));
                }
                py::str joined = py::join(S(", "), parts);
                py::print(S("privileges set: ") + (joined.empty() ? S("none") : joined));
                return 0;
            }
            py::list<std::tuple<int64_t, bool>> changes;
            for (int64_t k = 1; k < args.size() - 1; ++k) changes.append(_parse_bit(args[k]));
            data = set_privileges(data, changes);
            {
                py::file f = py::open(args[-1], S("wb"));
                f.write(data);
                f.close();
            }
            py::list<py::str> shown;
            auto now = privileges(data);
            for (int64_t b : std::get<0>(now)) shown.append(py::to_str(b));
            py::str joined = py::join(S(", "), shown);
            py::print(py::percent(S("privileges now: %s -> %s"), joined.empty() ? S("none") : joined, args[-1]));
            return 0;
        }
        if (!(op == S("nop") || op == S("bytes") || op == S("asm") || op == S("hook"))) {
            py::print(py::percent(S("unknown operation %r (privileges, nop, bytes, asm, hook, asm-only)"), op));
            return 2;
        }
        Image image(args[0]);
        int64_t address = py::int_parse(args[1], 0);
        py::str out = args[-1];
        if (op == S("nop")) {
            int64_t count = py::len(args) > 3 ? py::int_parse(args[2], 0) : 1;
            py::bytes nops;
            for (int64_t k = 0; k < count; ++k) nops = nops + NOP;                  // NOP * count
            image.write(address, nops);
        } else if (op == S("bytes")) {
            image.write(address, py::bytes::fromhex(args[2].replace(S(" "), S(""))));
        } else if (op == S("asm")) {
            image.write(address, build::ppc_asm::assemble(_asm_text(args[2]), address));
        } else {
            int64_t cave = py::int_parse(args[2], 0);              // (in order: the first of these to fail is the one reported)
            hook(image, address, cave, _asm_text(args[3]));
        }
        image.save(out);
        py::print(py::percent(S("%s at %08X -> %s"), op, address, out));
        return 0;
    } catch (const py::ValueError& e) {
        py::print(S("xex-edit: ") + py::str(e.what()));
        return 1;
    } catch (const py::IndexError& e) {
        py::print(S("xex-edit: ") + py::str(e.what()));
        return 1;
    } catch (const py::KeyError& e) {
        py::print(S("xex-edit: ") + py::str(e.what()));
        return 1;
    } catch (const build::ppc_asm::AsmError& e) {
        py::print(S("xex-edit: ") + py::str(e.what()));
        return 1;
    }
}

}  // namespace codebreaker::formats::xex_edit
