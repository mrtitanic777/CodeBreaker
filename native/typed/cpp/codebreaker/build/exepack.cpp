// codebreaker/build/exepack.cpp -- C++ from codebreaker.build.exepack.

#include "codebreaker/build/exepack.hpp"

namespace codebreaker::build::exepack {

const py::bytes MESSAGE = py::bytes("Packed file is corrupt");
const int64_t FILL = 0xB0;
const int64_t COPY = 0xB2;

namespace {

// One command of the packed stream: a fill (the byte in `data`, `count` times) or a copy (`data`).
// (The Python keeps these as tuples of two or three items; here one record.)
struct Command {
    int64_t kind = 0;      // FILL or COPY
    py::bytes data;
    int64_t count = 0;     // fills only
};

// struct.unpack_from("<%dH" % count, module, offset): `count` little-endian words (negative offsets count
// from the end, as struct's do)
py::list<int64_t> unpack_words(const py::bytes& module, int64_t offset, int64_t count) {
    if (count < 0) {
        throw py::StructError("bad char in struct format");
    }
    int64_t n = py::len(module);
    if (offset < 0) {
        if (offset + n < 0) {
            throw py::StructError("offset out of range");
        }
        offset += n;
    }
    if (n - offset < 2 * count) {
        throw py::StructError("unpack_from requires a larger buffer");
    }
    py::list<int64_t> words;
    for (int64_t i = 0; i < count; ++i) {
        words.append(module.byte_(offset + 2 * i) | module.byte_(offset + 2 * i + 1) << 8);
    }
    return words;
}

py::bytes word(int64_t v) {   // struct.pack("<H", v)
    return py::pack("<H", v);
}

}  // namespace

Layout::Layout(const py::list<Fill>& fills_, const py::bytes& stub_code_, int64_t size_, int64_t words_)
    : fills(fills_), stub_code(stub_code_), size(size_), words(words_) {}

Layout layout(const py::bytes& module, int64_t cs, int64_t ip) {
    int64_t at = cs * 16;
    py::list<int64_t> words = unpack_words(module, at, py::floordiv(ip, 2));
    if (!(ip == 0x10 || ip == 0x12) || words[-1] != 0x4252 || (ip == 0x12 && words[7] != 1)) {
        throw py::ValueError("not the EXEPACK this writes (a header of eight or nine words ending in RB)");
    }
    int64_t own_size = words[3], paras = words[6];
    int64_t src = at, dst = paras * 16;
    py::list<Fill> fills;
    while (module[src - 1] == 0xFF) {
        src -= 1;
    }
    while (true) {
        int64_t command = module[src - 1];
        int64_t length = unpack_words(module, src - 3, 1)[0];
        src -= 3;
        if ((command & 0xFE) == FILL) {
            src -= 1;
            dst -= length;
            fills.append({dst, length, module[src]});
        } else if ((command & 0xFE) == COPY) {
            src -= length;
            dst -= length;
        } else {
            throw py::ValueError(py::percent("EXEPACK command %#x", command));
        }
        if (command & 1) {
            break;
        }
    }
    if (src != dst) {
        throw py::ValueError("the stored program's first bytes are not stored as they are");
    }
    int64_t table = module.find(MESSAGE, at, at + own_size);
    if (table < 0) {
        throw py::ValueError("EXEPACK message not found");
    }
    return Layout(py::sorted(fills), module.slice(at + ip, table + py::len(MESSAGE)), paras * 16, py::floordiv(ip, 2));
}

std::tuple<py::bytes, std::tuple<int64_t, int64_t, int64_t, int64_t>> pack(
    const py::bytes& image_in, const py::list<int64_t>& relocs, const std::tuple<int64_t, int64_t>& entry,
    const std::tuple<int64_t, int64_t>& stack, const py::list<Fill>& fills_in, const py::bytes& stub_code, int64_t words) {
    py::bytes image = image_in + py::bytes::zeros(py::mod(-py::len(image_in), 16));
    py::list<Fill> fills = py::sorted(fills_in);
    for (const Fill& f : fills) {
        auto [off, n, byte] = f;
        py::bytes run = py::repeat(py::bytes::from_list(py::list<int64_t>{byte}), n);
        if (image.slice(off, off + n) != run || !(0 < n && n <= 0xFFFF)) {
            throw py::ValueError(py::percent("a fill at %#x is not %d bytes of %02X", off, n, byte));
        }
    }
    // the commands, from the first fill to the end of the program
    py::list<Command> commands;
    int64_t at = !fills.empty() ? std::get<0>(fills[0]) : py::len(image);
    py::list<Fill> with_end = fills;
    with_end.append({py::len(image), 0, 0});   // (the Python's sentinel (len(image), 0, None): its byte is never read)
    for (const Fill& f : with_end) {
        auto [off, n, byte] = f;
        while (at < off) {
            int64_t k = py::min(off - at, int64_t(0xFFFF));
            commands.append(Command{COPY, image.slice(at, at + k), 0});
            at += k;
        }
        if (n) {
            commands.append(Command{FILL, py::bytes::from_list(py::list<int64_t>{byte}), n});
            at += n;
        }
    }
    py::bytearray stream(image.slice_to(!fills.empty() ? std::get<0>(fills[0]) : py::len(image)));
    for (int64_t i = 0; i < py::len(commands); ++i) {
        const Command& c = commands[i];
        int64_t last = i == 0 ? 1 : 0;   // read last: the lowest command
        if (c.kind == FILL) {
            stream += c.data + py::pack("<HB", c.count, FILL | last);
        } else {
            stream += c.data + py::pack("<HB", py::len(c.data), COPY | last);
        }
    }
    stream += py::repeat(py::bytes("\xFF", 1), py::mod(-py::len(stream), 16));
    py::list<py::list<int64_t>> lists;
    for (int64_t k = 0; k < 16; ++k) {
        lists.append(py::list<int64_t>());
    }
    for (int64_t r : relocs) {   // in the order given: LINK's was its fixups'
        lists[py::shr(r, 16)].append(r & 0xFFFF);
    }
    py::bytearray table_bytes;
    for (const py::list<int64_t>& l : lists) {
        if (py::len(l) > 0xFFFF) {
            throw py::StructError("ushort format requires 0 <= number <= 65535");
        }
        table_bytes += word(py::len(l));
        for (int64_t v : l) {
            table_bytes += word(v);
        }
    }
    py::bytes table = table_bytes.view();
    int64_t own = 2 * words + py::len(stub_code) + py::len(table);
    auto [cs, ip] = entry;
    auto [ss, sp] = stack;
    py::bytes head = py::pack("<7H", ip, cs, int64_t(0), own, sp, ss, py::floordiv(py::len(image), 16)) +
                     (words == 9 ? py::pack("<H", int64_t(1)) : py::bytes()) + py::bytes("RB");
    int64_t stub_cs = py::floordiv(py::len(stream), 16);
    return {stream.view() + head + stub_code + table,
            {stub_cs, 2 * words, py::floordiv(py::len(image), 16) + py::floordiv(own + 15, 16), 0x80}};
}

}  // namespace codebreaker::build::exepack
