// codebreaker/formats/xbox/nand.cpp -- C++ from codebreaker.formats.xbox.nand.

#include "codebreaker/formats/xbox/nand.hpp"

namespace codebreaker::formats::xbox::nand {

const int64_t PAGE = 0x200;
const int64_t SPARE = 0x10;
const int64_t PAGES_PER_BLOCK = 32;
const int64_t BLOCK = PAGE * PAGES_PER_BLOCK;
const int64_t ROOT_TYPE = 0x30;
const int64_t END = 0x1FFF;

bool is_nand(const py::str& path) {
    int64_t size = 0;
    py::bytes head;
    try {
        size = py::os::path::getsize(path);
        py::file f = py::open_rb(path);
        head = f.read(0x40);
        f.close();
    } catch (const py::OSError&) {
        return false;
    }
    return size % (PAGE + SPARE) == 0 && size >= 0x1000000 && head.slice_to(2) == py::bytes("\xFF\x4F", 2) &&
           head.contains(py::bytes("Microsoft"));
}

Flash::Flash(const py::bytes& raw_) : raw(raw_) {
    if (py::len(raw_) % (PAGE + SPARE)) {
        throw py::ValueError(py::percent("not a raw NAND dump with spare bytes (size %#x)", py::len(raw_)));
    }
    int64_t blocks = py::floordiv(py::floordiv(py::len(raw_), PAGE + SPARE), PAGES_PER_BLOCK);
    py::list<std::tuple<int64_t, int64_t>> roots;   // (sequence number, block)
    for (int64_t b : py::range(blocks)) {
        py::bytes s = _spare(b);
        if (s == py::bytes(std::string(static_cast<size_t>(SPARE), '\xFF'))) {
            continue;
        }
        if ((s[12] & 0x3F) == ROOT_TYPE) {
            roots.append({s[0] << 24 | s[3] << 16 | s[4] << 8 | s[6], b});
        } else {
            by_id.setdefault(s[2] << 8 | s[1], b);
        }
    }
    if (roots.empty()) {
        throw py::ValueError("no flash file system root block");
    }
    root = std::get<1>(py::max_of(roots));
    for (int64_t i : py::range(PAGES_PER_BLOCK)) {
        py::bytes p = _page(root * PAGES_PER_BLOCK + i);
        if (i % 2 == 0) {
            // struct.unpack(">256H", p): 256 big-endian 16-bit numbers
            if (py::len(p) != 512) {
                throw py::StructError("unpack requires a buffer of 512 bytes");
            }
            for (int64_t k : py::range(256)) {
                alloc.append(p[2 * k] << 8 | p[2 * k + 1]);
            }
            continue;
        }
        for (int64_t o : py::range(0, PAGE, 0x20)) {
            py::bytes e = p.slice(o, o + 0x20);
            if (e[0] == 0 || e[0] == 0xFF) {
                continue;
            }
            py::str name = e.slice_to(22).rstrip(py::bytes("\0", 1)).decode("ascii", "replace");
            // struct.unpack(">HII", e[22:])
            py::bytes t = e.slice_from(22);
            if (py::len(t) != 10) {
                throw py::StructError("unpack requires a buffer of 10 bytes");
            }
            int64_t start = t[0] << 8 | t[1];
            int64_t size = t[2] << 24 | t[3] << 16 | t[4] << 8 | t[5];
            int64_t time = t[6] << 24 | t[7] << 16 | t[8] << 8 | t[9];
            files.set(name, {start, size, time});
        }
    }
}

py::bytes Flash::_page(int64_t p) const {
    int64_t o = p * (PAGE + SPARE);
    return raw.slice(o, o + PAGE);
}

py::bytes Flash::_spare(int64_t block) const {
    int64_t o = block * PAGES_PER_BLOCK * (PAGE + SPARE) + PAGE;
    return raw.slice(o, o + SPARE);
}

py::list<int64_t> Flash::chain(const py::str& name) const {
    auto [start, size, _t] = files.at(name);
    py::list<int64_t> out;
    int64_t blk = start;
    while (blk != END && py::len(out) * BLOCK < size) {
        if (blk >= py::len(alloc) || blk >= 0x1FF0) {
            throw py::ValueError(py::percent("%s: a bad block number in its chain (%#x)", name, blk));
        }
        out.append(blk);
        blk = alloc[blk];
    }
    if (py::len(out) * BLOCK < size) {
        throw py::ValueError(py::percent("%s: its chain is short (%d blocks for %#x bytes)", name, py::len(out), size));
    }
    return out;
}

py::bytes Flash::read(const py::str& name) const {
    py::bytearray data;
    for (int64_t blk : chain(name)) {
        int64_t phys = by_id.get(blk, blk);
        for (int64_t i : py::range(PAGES_PER_BLOCK)) {
            data += _page(phys * PAGES_PER_BLOCK + i);
        }
    }
    return data.slice_to(std::get<1>(files.at(name))).view();
}

}  // namespace codebreaker::formats::xbox::nand
