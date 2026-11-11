// codebreaker/build/floppy.cpp -- C++ from codebreaker.build.floppy (see floppy.hpp).

#include "codebreaker/build/floppy.hpp"

#include <algorithm>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/dos.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::floppy {

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) {
        path = py::os::path::join(path, p);
    }
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// subprocess.CalledProcessError (check=True)
struct CalledProcessError : py::Exception {
    explicit CalledProcessError(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "CalledProcessError"; }
};

py::str latin1(const py::bytes& b) { return b.decode("latin1"); }

// struct.unpack_from's complaints about a buffer too small for `size` bytes at `offset`, worded as CPython does
// (the runtime's unpack_from says only "unpack_from requires a larger buffer")
void need_buffer(const py::bytes& buf, int64_t offset, int64_t size) {
    int64_t n = py::len(buf);
    if (offset < 0) {
        if (offset + n < 0) {
            throw py::StructError(py::percent(py::str("offset %d out of range for %d-byte buffer"), offset, n));
        }
        offset += n;
    }
    if (n - offset < size) {
        throw py::StructError(py::percent(py::str("unpack_from requires a buffer of at least %d bytes for unpacking %d bytes at offset %d "
                                                  "(actual buffer size is %d)"),
                                          size + offset, size, offset, n));
    }
}

// struct.unpack_from(fmt, buf, at)[0] for one little-endian integer of `size` bytes
int64_t unpack1(const char* fmt, int64_t size, const py::bytes& buf, int64_t at) {
    need_buffer(buf, at, size);
    return py::unpack_from1<int64_t>(fmt, buf, at);
}

// struct.unpack_from("<H", fat, at)[0]
int64_t word_at(const py::bytes& buf, int64_t at) { return unpack1("<H", 2, buf, at); }

// bytes[i]: IndexError("index out of range") as CPython says it (the runtime's text repeats the word "index")
int64_t byte_at(const py::bytes& b, int64_t i) {
    if (i >= py::len(b) || i < -py::len(b)) {
        throw py::IndexError("index out of range");
    }
    return b[i];
}

}  // namespace

py::str Entry::name() const {
    py::str base = latin1(raw.slice(0, 8).view()).strip();
    py::str ext = latin1(raw.slice(8, 11).view()).strip();
    return base + (py::truthy(ext) ? py::str(".") + ext : py::str(""));
}

bool Entry::is_file() const {
    return !(raw[11] & (_ATTR_LABEL | _ATTR_DIR)) && raw[11] != _LFN;
}

Bpb _bpb(const py::bytes& boot) {
    need_buffer(boot, 11, 13);
    auto [bps, spc, rsv, nfat, nroot, total, media, spf] =
        py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<HBHBHHBH", boot, 11);
    (void)media;
    Bpb b;
    b.bps = bps;
    b.spc = spc;
    b.rsv = rsv;
    b.nfat = nfat;
    b.nroot = nroot;
    b.total = total;
    b.spf = spf;
    b.root = (rsv + nfat * spf) * bps;
    b.data = (rsv + nfat * spf) * bps + nroot * 32;
    return b;
}

Disk read(const py::bytes& img) {
    Bpb b = _bpb(img);
    py::bytes fat = img.slice(b.rsv * b.bps, (b.rsv + b.spf) * b.bps);
    int64_t cluster = b.spc * b.bps;

    // the clusters of a file, from its first one
    auto chain = [&](int64_t c) {
        py::list<int64_t> out;
        while (2 <= c && c < 0xFF8) {
            out.append(c);
            int64_t v = word_at(fat, py::floordiv(c * 3, 2));
            c = (c & 1) ? (v >> 4) : (v & 0xFFF);
        }
        return out;
    };
    py::list<Entry> entries;
    for (int64_t i = 0; i < b.nroot; ++i) {
        py::bytes raw = img.slice(b.root + 32 * i, b.root + 32 * i + 32);
        if (byte_at(raw, 0) == 0) {
            break;
        }
        if (byte_at(raw, 0) == 0xE5) {
            continue;
        }
        Entry e(raw, std::nullopt);
        if (e.is_file()) {
            int64_t size = unpack1("<I", 4, raw, 28);
            int64_t first = unpack1("<H", 2, raw, 26);
            std::string joined;
            for (int64_t c : chain(first)) {
                joined += img.slice(b.data + (c - 2) * cluster, b.data + (c - 1) * cluster).raw();
            }
            e.data = py::bytes(std::move(joined)).slice(0, size);
        }
        entries.append(e);
    }
    return Disk(img.slice(0, 512), entries, b);
}

py::bytes make(const Disk& disk, const py::bytes& boot, const py::dict<py::str, py::bytes>& contents) {
    const Bpb& b = disk.bpb;
    if (_bpb(boot) != b) {
        throw py::ValueError("the boot sector's parameters are not the original floppy's");
    }
    py::bytearray img = py::bytearray::zeros(b.total * b.bps);
    img.set_slice(0, 512, boot);
    int64_t cluster = b.spc * b.bps;
    py::bytearray fat = py::bytearray::zeros(b.spf * b.bps);

    auto put = [&](int64_t c, int64_t v) {
        int64_t at = py::floordiv(c * 3, 2);
        int64_t w = word_at(fat.view(), at);
        w = (c & 1) ? ((w & 0x000F) | (v << 4)) : ((w & 0xF000) | v);
        py::pack_into("<H", fat, at, w);
    };
    put(0, 0xF00 | byte_at(boot, 21));   // the media byte
    put(1, 0xFFF);
    int64_t nxt = 2;
    int64_t i = 0;
    for (const Entry& e : disk.entries) {
        py::bytearray raw(e.raw.view());
        if (e.is_file()) {
            py::bytes data;
            if (contents.contains(e.name().upper())) {
                data = contents.get(e.name().upper(), py::bytes());
            } else if (e.data) {
                data = *e.data;
            } else {
                throw py::TypeError("object of type 'NoneType' has no len()");
            }
            int64_t n = py::floordiv(py::len(data) + cluster - 1, cluster);
            if (nxt + n - 2 > py::floordiv(b.total * b.bps - b.data, cluster)) {
                throw py::ValueError("the files do not fit on the floppy");
            }
            int64_t first = n ? nxt : 0;
            for (int64_t k = 0; k < n; ++k) {
                put(nxt + k, k < n - 1 ? nxt + k + 1 : 0xFFF);
            }
            int64_t at = b.data + (nxt - 2) * cluster;
            img.set_slice(at, at + py::len(data), data);
            nxt += n;
            py::pack_into("<HI", raw, 26, first, py::len(data));
        }
        img.set_slice(b.root + 32 * i, b.root + 32 * i + 32, raw.view());
        ++i;
    }
    for (int64_t k = 0; k < b.nfat; ++k) {
        int64_t at = (b.rsv + k * b.spf) * b.bps;
        img.set_slice(at, at + py::len(fat), fat.view());
    }
    return img.view();
}

std::tuple<py::bytes, py::str> _boot_sector(const py::str& design) {
    py::str src = py::os::path::join(design, py::str("recon"), py::str("bootdisk"), py::str("msboot.asm"));
    py::str nasm = py::settings::get_str(py::str("nasm"));
    if (!py::truthy(nasm)) {
        nasm = py::str("nasm");
    }
    if (py::os::path::isdir(nasm)) {
        nasm = py::os::path::join(nasm, py::str("nasm.exe"));
    }
    py::os::makedirs(state({py::str("bootfloppy")}), 0777, true);
    py::str out = state({py::str("bootfloppy"), py::str("msboot.bin")});
    // subprocess.run([nasm, "-f", "bin", src, "-o", out], check=True, capture_output=True)
    py::subprocess::RunOptions opts;
    opts.stdin_ = py::subprocess::Stream::Inherit;
    opts.text = false;
    py::list<py::str> cmd;
    for (const char* a : {"-f", "bin"}) {
        cmd.append(py::str(a));
    }
    py::list<py::str> argv;
    argv.append(nasm);
    for (const py::str& a : cmd) {
        argv.append(a);
    }
    argv.append(src);
    argv.append(py::str("-o"));
    argv.append(out);
    py::subprocess::CompletedProcess done = py::subprocess::run(argv, opts);
    if (done.returncode != 0) {
        throw CalledProcessError(py::percent(py::str("Command %s returned non-zero exit status %d."), py::repr(argv), done.returncode));
    }
    py::file f = py::open_rb(out);
    py::bytes data = f.read();
    f.close();
    return {data, src};
}

std::optional<py::str> _rebuilt(const py::str& src_dir, const py::str& name) {
    py::str d = py::os::path::join(src_dir, name);
    if (!py::os::path::isdir(d)) {
        return std::nullopt;
    }
    py::str stem = std::get<0>(py::os::path::splitext(name)).lower();
    // the program laid out as the original file is (dos_writer: file/<NAME>), else as built
    py::list<py::str> candidates;
    candidates.append(py::os::path::join(py::str("file"), name.upper()));
    candidates.append(name.lower());
    candidates.append(stem + py::str(".exe"));
    candidates.append(stem + py::str(".com"));
    candidates.append(stem + py::str(".bin"));
    for (const py::str& cand : candidates) {
        py::str p = py::os::path::join(d, cand);
        if (py::os::path::isfile(p)) {
            return p;
        }
    }
    return std::nullopt;
}

std::function<Reconstructed(const py::str&, const py::str&)>& dos_writer_reconstruct() {
    static std::function<Reconstructed(const py::str&, const py::str&)> hook = [](const py::str&, const py::str&) -> Reconstructed {
        throw py::NotImplementedError("source.dos_writer.reconstruct has no typed port: set floppy::dos_writer_reconstruct()");
    };
    return hook;
}

namespace {

// argv[argv.index(flag) + 1]
py::str value_of(const py::list<py::str>& argv, const char* flag) { return argv[argv.index(py::str(flag)) + 1]; }

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    py::str image = argv.contains(py::str("--image"))
                        ? value_of(argv, "--image")
                        : py::os::path::join(codebreaker::base::corpus::root(), py::str("setup"), py::str("media"), py::str("boot.img"));
    py::file imgf = py::open_rb(image);
    py::bytes imgdata = imgf.read();
    imgf.close();
    Disk disk = read(imgdata);
    if (argv.contains(py::str("--extract"))) {
        py::str out = value_of(argv, "--extract");
        py::os::makedirs(out, 0777, true);
        int64_t files = 0;
        for (const Entry& e : disk.entries) {
            if (e.is_file()) {
                py::file f = py::open(py::os::path::join(out, e.name()), py::str("wb"));
                f.write(*e.data);
                f.close();
                files += 1;
            }
        }
        py::print(py::percent(py::str("%d files written to %s"), files, out));
        return 0;
    }
    py::str original = state({py::str("bootfloppy"), py::str("original")});
    py::str src_dir = state({py::str("bootfloppy"), py::str("src")});
    py::os::makedirs(original, 0777, true);
    auto [boot, boot_src] = _boot_sector(codebreaker::base::corpus::design_dir());
    py::print(py::percent(py::str("boot sector: %s (%s the original's)"), py::os::path::basename(boot_src),
                          boot == disk.boot ? py::str("the same as") : py::str("DIFFERENT from")));
    py::dict<py::str, py::bytes> contents;
    py::list<py::str> kept;
    for (const Entry& e : disk.entries) {
        if (!e.is_file()) {
            continue;
        }
        py::str path = py::os::path::join(original, e.name());
        if (!py::os::path::isfile(path)) {
            py::file f = py::open(path, py::str("wb"));
            f.write(*e.data);
            f.close();
        }
        if (!formats::dos::kind(path)) {
            bool text = true;
            for (int64_t c : *e.data) {
                if (!((32 <= c && c < 127) || c == 9 || c == 10 || c == 13 || c == 26)) {
                    text = false;
                    break;
                }
            }
            py::print(py::percent(py::str("  %-13s %s"), e.name(), text ? py::str("text: as it is") : py::str("not a real-mode program: the original's")));
            if (!text) {
                kept.append(e.name());
            }
            continue;
        }
        std::optional<py::str> built = _rebuilt(src_dir, e.name());
        if (!built) {
            Reconstructed r = dos_writer_reconstruct()(path, py::os::path::join(src_dir, e.name()));
            if (r.same || r.equivalent) {
                built = (r.file && py::truthy(*r.file)) ? *r.file : r.image;
            }
        }
        if (!built) {
            py::print(py::percent(py::str("  %-13s does not build back: the original's"), e.name()));
            kept.append(e.name());
            continue;
        }
        py::file f = py::open_rb(*built);
        contents.set(e.name().upper(), f.read());
        f.close();
        py::print(py::percent(py::str("  %-13s rebuilt from source (%s)"), e.name(), py::os::path::relpath(*built, src_dir)));
    }
    // a program the original stores packed by a packer CodeBreaker does not write (PKLITE ...)
    // is rebuilt unpacked, and bigger: when the disk is too small for them all, the biggest of
    // them go on as the original stores them, until everything fits
    py::list<py::str> unpacked;
    for (const py::str& n : contents.keys()) {
        std::optional<py::str> p = formats::dos::packer(py::os::path::join(original, n));
        if (p && *p != py::str("EXEPACK") && *p != py::str("moves itself") && *p != py::str("PKLITE")) {
            unpacked.append(n);
        }
    }
    std::stable_sort(unpacked.vec().begin(), unpacked.vec().end(), [&](const py::str& a, const py::str& b) {
        return -py::len(contents.get(a, py::bytes())) < -py::len(contents.get(b, py::bytes()));
    });
    py::bytes img;
    while (true) {
        try {
            img = make(disk, boot, contents);
            break;
        } catch (const py::ValueError& e) {
            if (py::str(e.what()).find(py::str("do not fit")) < 0 || !py::truthy(unpacked)) {
                throw;
            }
            py::str n = unpacked.pop(0);
            contents.pop(n);
            kept.append(py::percent(py::str("%s (stored packed by %s; rebuilt unpacked it does not fit)"), n,
                                    *formats::dos::packer(py::os::path::join(original, n))));
        }
    }
    py::str out = argv.contains(py::str("--out")) ? value_of(argv, "--out") : state({py::str("bootfloppy"), py::str("boot95.img")});
    py::file outf = py::open(out, py::str("wb"));
    outf.write(img);
    outf.close();
    py::print(py::percent(py::str("%s: %d programs rebuilt from source; the original's: %s"), out, py::len(contents),
                          py::truthy(kept) ? py::join(py::str(", "), kept) : py::str("none")));
    return 0;
}

}  // namespace codebreaker::build::floppy
