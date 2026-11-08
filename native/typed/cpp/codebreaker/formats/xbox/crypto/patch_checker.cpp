// codebreaker/formats/xbox/crypto/patch_checker.cpp -- C++ port of codebreaker.formats.xbox.crypto.patch_checker.

#include "codebreaker/formats/xbox/crypto/patch_checker.hpp"

#include <cstdio>

#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::patch_checker {

namespace {

using codebreaker::formats::xbox::crypto::StreamIO::Endian;
using StreamIOStream = codebreaker::formats::xbox::crypto::StreamIO::StreamIO;

// range(start, stop) of the Python
struct Range {
    int64_t start;
    int64_t stop;
};

// f"0x{value:08X}"
py::str hex08(int64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%08llX", static_cast<unsigned long long>(value));
    return py::str(buf);
}

// `with StreamIO(...) as psio:` -- closes the stream when the block is left, by an exception too.
struct Closing {
    StreamIOStream& s;
    explicit Closing(StreamIOStream& stream) : s(stream) { s.__enter__(); }
    ~Closing() { s.__exit__(); }
};

template <class Data> void eval_patches_(const Data& patch_data) {
    py::list<Range> patches;
    {
        StreamIOStream psio(patch_data, Endian::BIG);
        Closing with(psio);
        while (true) {
            int64_t addr = psio.read_uint32();
            if (addr == 0xFFFFFFFF) {
                break;
            }
            int64_t size = psio.read_uint32();
            py::bytes patch = psio.read_ubytes(size * 4);
            patches.append(Range{addr, addr + py::len(patch)});
        }
    }

    for (int64_t i = 0; i < py::len(patches); i++) {
        py::list<Range> tmp = patches.copy();   // list(patches) (a py::list is a shared reference)
        Range patch_range = tmp.pop(i);

        Range x = patch_range;
        for (int64_t j = 0; j < py::len(tmp); j++) {
            Range y = tmp[j];
            if (is_overlapping(x.start, x.stop + 1, y.start, y.stop + 1)) {
                py::print(py::str("Patch Conflict!"));
                py::print(py::str("Patch A: ") + hex08(x.start) + py::str(" - ") + hex08(x.stop));
                py::print(py::str("Patch B: ") + hex08(y.start) + py::str(" - ") + hex08(y.stop));
            }
        }
    }
}

}  // namespace

bool is_overlapping(int64_t x1, int64_t x2, int64_t y1, int64_t y2) {
    return x1 <= y2 && y1 <= x2;
}

void eval_patches(const py::bytes& patch_data) { eval_patches_(patch_data); }
void eval_patches(const py::bytearray& patch_data) { eval_patches_(patch_data); }

py::bytes read_file(const py::str& filename) {
    py::file f = py::open(filename, py::str("rb"));
    py::bytes data = f.read();
    f.close();
    return data;
}

void write_file(const py::str& filename, const py::bytes& data) {
    py::file f = py::open(filename, py::str("wb"));
    f.write(data);
    f.close();
}

void write_file(const py::str& filename, const py::bytearray& data) {
    py::file f = py::open(filename, py::str("wb"));
    f.write(data);
    f.close();
}

int64_t main() {
    eval_patches(read_file(py::str("Output/NonZero/RGL_flash.bin")));
    eval_patches(read_file(py::str("Output/NonZero/RGL_hdd.bin")));

    eval_patches(read_file(py::str("Output/Zero/VRGL_flash.bin")));
    eval_patches(read_file(py::str("Output/Zero/VRGL_hdd.bin")));

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::patch_checker
