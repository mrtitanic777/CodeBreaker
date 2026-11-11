// codebreaker/formats/xbox/stfs.cpp -- C++ port of codebreaker.formats.xbox.stfs.

#include "codebreaker/formats/xbox/stfs.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <functional>
#include <vector>

#include "py_os.hpp"

namespace codebreaker::formats::xbox::stfs {

namespace {

// bytes(...) of one byte-string literal
py::bytes lit(const char* s, size_t n) { return py::bytes(s, n); }

// `"%d" % n` and friends
std::string d_(int64_t v) { return std::to_string(v); }
std::string hex_upper_(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llX", static_cast<unsigned long long>(v));
    return buf;
}
std::string upper_(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}
std::string u8_(const py::str& s) { return s.utf8(); }
void say_(const std::string& s) { py::print(py::str(s)); }

// bytes.decode("utf-16-be", "replace"): the runtime has no UTF-16 codec, so the one decoder this module
// needs is here, with CPython's treatment of an odd trailing byte and of unpaired surrogates (each an
// error replaced by one U+FFFD).
py::str utf16be_replace_(const py::bytes& b) {
    std::u32string out;
    const int64_t n = b.size();
    int64_t i = 0;
    while (i < n) {
        if (n - i < 2) {   // truncated data
            out.push_back(U'�');
            break;
        }
        int64_t ch = (b.byte_(i) << 8) | b.byte_(i + 1);
        i += 2;
        if (ch < 0xD800 || ch > 0xDFFF) {
            out.push_back(static_cast<char32_t>(ch));
            continue;
        }
        if (ch >= 0xDC00) {   // a low surrogate with no high one before it
            out.push_back(U'�');
            continue;
        }
        if (n - i < 2) {   // unexpected end of data
            out.push_back(U'�');
            break;
        }
        int64_t ch2 = (b.byte_(i) << 8) | b.byte_(i + 1);
        if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
            i += 2;
            out.push_back(static_cast<char32_t>(0x10000 + ((ch - 0xD800) << 10) + (ch2 - 0xDC00)));
        } else {   // a high surrogate not followed by a low one: it alone is the error
            out.push_back(U'�');
        }
    }
    return py::str(out);
}

py::bytes read_all_(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes data = f.read();
    f.close();
    return data;
}

// sha1(data).digest()
py::bytes sha1_(const py::bytes& data) { return py::hashlib::sha1(data).digest(); }

// Python's pow(base, exp, mod) for a possibly negative exponent given as (negative, magnitude)
BigInt pow_signed_(const BigInt& base, bool exp_neg, const BigInt& exp_mag, const BigInt& mod) {
    if (mod.is_zero()) throw py::ValueError("pow() 3rd argument cannot be 0");
    if (exp_neg) return BigInt::powmod(BigInt::modinv(base, mod), exp_mag, mod);
    return BigInt::powmod(base, exp_mag, mod);
}

// bytes with each of the (up to) 8-byte chunks' order reversed -- b"".join(b[i:i+8][::-1] ...)
py::bytes swap_qwords_(const py::bytes& b) {
    std::string out;
    for (int64_t i = 0; i < b.size(); i += 8) {
        std::string chunk = b.slice(i, i + 8).raw();
        std::reverse(chunk.begin(), chunk.end());
        out += chunk;
    }
    return py::bytes(out);
}

}  // namespace

const py::str DESCRIPTION("read/resign Xbox 360 STFS packages (CON/LIVE/PIRS)");

bool is_magic(const py::bytes& four) {
    return four == lit("CON ", 4) || four == lit("LIVE", 4) || four == lit("PIRS", 4);
}

int64_t _u24_le(const py::bytes& d, int64_t o) { return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16); }

// =====================================================================================================
// STFS
// =====================================================================================================
STFS::STFS(py::bytes data_in) {
    if (!is_magic(data_in.slice(0, 4))) throw py::ValueError("not an STFS package (magic " + py::repr(data_in.slice(0, 4)).utf8() + ")");
    d = std::move(data_in);   // (a temporary is taken without a copy)
    const py::bytes& data = d;
    magic = data.slice(0, 4);

    header_size = py::unpack_from1<int64_t>(">I", data, 0x340);
    first_hash = (header_size + 0xFFF) & 0xFFFFF000;

    // STFS volume descriptor
    block_sep = data[0x37B];
    ft_block_count = py::unpack_from1<int64_t>("<H", data, 0x37C);
    ft_block_number = _u24_le(data, 0x37E);
    alloc_blocks = py::unpack_from1<int64_t>(">I", data, 0x395);
    unalloc_blocks = py::unpack_from1<int64_t>(">I", data, 0x399);

    sex = (~block_sep) & 1;   // 0 = female, 1 = male
    step = sex == 0 ? py::list<int64_t>{0xAB, 0x718F} : py::list<int64_t>{0xAC, 0x723A};
    top_level = alloc_blocks <= 0xAA ? 0 : (alloc_blocks <= 0x70E4 ? 1 : 2);

    // content metadata
    content_type = py::unpack_from1<int64_t>(">I", data, 0x344);
    title_id = data.slice(0x360, 0x364).hex().upper();
    profile_id = data.slice(0x371, 0x379).hex().upper();
    display_name = _wstr(0x411);
    title_name = _wstr(0x1691);

    _load_top_table();
    files = _read_file_listing();
}

// ---- util -------------------------------------------------------------------------------------------
py::str STFS::_wstr(int64_t off, int64_t maxlen) {
    py::str text = utf16be_replace_(d.slice(off, off + maxlen));
    const std::u32string& u = text.u32();
    size_t nul = u.find(U'\0');
    return nul == std::u32string::npos ? text : py::str(u.substr(0, nul));   // .split("\x00", 1)[0]
}

// ---- block address maths ----------------------------------------------------------------------------
int64_t STFS::_cbdbn(int64_t b) {
    int64_t r = (py::floordiv(b + 0xAA, 0xAA) << sex) + b;
    if (b < 0xAA) return r;
    r += py::floordiv(b + 0x70E4, 0x70E4) << sex;
    return b < 0x70E4 ? r : r + (int64_t(1) << sex);
}

int64_t STFS::_block_addr(int64_t b) { return (_cbdbn(b) << 0xC) + first_hash; }

int64_t STFS::_cl0(int64_t b) {
    if (b < 0xAA) return 0;
    int64_t num = py::floordiv(b, 0xAA) * step[0];
    num += (py::floordiv(b, 0x70E4) + 1) << sex;
    return py::floordiv(b, 0x70E4) == 0 ? num : num + (int64_t(1) << sex);
}

int64_t STFS::_cl1(int64_t b) {
    if (b < 0x70E4) return step[0];
    return (int64_t(1) << sex) + py::floordiv(b, 0x70E4) * step[1];
}

void STFS::_load_top_table() {
    int64_t true_block = top_level == 0 ? _cl0(0) : (top_level == 1 ? _cl1(0) : step[1]);
    int64_t base = (true_block << 0xC) + first_hash;
    int64_t addr = base + ((block_sep & 2) << 0xB);
    int64_t per = py::list<int64_t>{1, 0xAA, 0x70E4}[top_level];
    int64_t cnt = py::floordiv(alloc_blocks, per);
    if (alloc_blocks > 0x70E4 && py::mod(alloc_blocks, 0x70E4)) cnt += 1;
    else if (alloc_blocks > 0xAA && py::mod(alloc_blocks, 0xAA)) cnt += 1;
    top_entries = py::list<int64_t>();
    for (int64_t i = 0; i < cnt + 1; ++i) {
        py::bytes e = d.slice(addr + i * 0x18, addr + i * 0x18 + 0x18);
        if (e.size() < 0x18) break;
        top_entries.append(e[0x14]);   // status byte
    }
}

int64_t STFS::_hash_addr(int64_t b) {
    int64_t a = (_cl0(b) << 0xC) + first_hash + py::mod(b, 0xAA) * 0x18;
    if (top_level == 0) {
        a += (block_sep & 2) << 0xB;
    } else if (top_level == 1) {
        a += (top_entries[py::floordiv(b, 0xAA)] & 0x40) << 6;
    } else {
        int64_t l1 = (top_entries[py::floordiv(b, 0x70E4)] & 0x40) << 6;
        int64_t pos = (_cl1(b) << 0xC) + first_hash + l1 + py::mod(b, 0xAA) * 0x18;
        a += (d[pos + 0x14] & 0x40) << 6;
    }
    return a;
}

int64_t STFS::_next_block(int64_t b) {
    int64_t a = _hash_addr(b);
    return (d[a + 0x15] << 16) | (d[a + 0x16] << 8) | d[a + 0x17];
}

// ---- file table -------------------------------------------------------------------------------------
py::bytes STFS::_read_chain(int64_t start, int64_t nblocks, int64_t size) {
    std::string out;
    int64_t cur = start;
    const int64_t count = nblocks != 0 ? nblocks : (int64_t(1) << 30);   // range(nblocks or 1 << 30)
    for (int64_t k = 0; k < count; ++k) {
        int64_t o = _block_addr(cur);
        out += d.slice(o, o + BLOCK).raw();
        cur = _next_block(cur);
        if (cur == 0xFFFFFF || cur == 0) break;
    }
    py::bytes all(out);
    return size != 0 ? all.slice(0, size) : all;
}

py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> STFS::_read_file_listing() {
    // {path: (start block, blocks, size)}: each file by its path in the package ("media/font.xpr").
    // An entry names its folder by the folder entry's index in the listing (0x32; -1 the root).
    struct Entry {   // (name, is folder, parent index, start, blocks, size)
        py::str name;
        bool is_folder;
        int64_t parent;
        int64_t start;
        int64_t blocks;
        int64_t size;
    };
    std::vector<std::optional<Entry>> entries;
    int64_t cur = ft_block_number;
    for (int64_t k = 0; k < ft_block_count; ++k) {
        py::bytes block = d.slice(_block_addr(cur), _block_addr(cur) + BLOCK);
        for (int64_t i = 0; i < BLOCK / 0x40; ++i) {
            py::bytes e = block.slice(i * 0x40, (i + 1) * 0x40);
            int64_t nlen = e[0x28] & 0x3F;
            if (nlen == 0) {
                entries.push_back(std::nullopt);   // (indices count every slot)
                continue;
            }
            entries.push_back(Entry{e.slice(0, nlen).decode("latin-1", "ignore"), static_cast<bool>(e[0x28] & 0x80),
                                    py::unpack_from1<int64_t>(">h", e, 0x32), _u24_le(e, 0x2F), _u24_le(e, 0x29),
                                    py::unpack_from1<int64_t>(">I", e, 0x34)});
        }
        cur = _next_block(cur);
        if (cur == 0xFFFFFF || cur == 0) break;
    }

    std::function<py::str(int64_t, int64_t)> path = [&](int64_t index, int64_t depth) -> py::str {
        const Entry* entry = (0 <= index && index < static_cast<int64_t>(entries.size()) && entries[static_cast<size_t>(index)])
                                 ? &*entries[static_cast<size_t>(index)]
                                 : nullptr;
        if (entry == nullptr || depth > 64) return py::str("");
        py::str parent = entry->parent >= 0 ? path(entry->parent, depth + 1) : py::str("");
        return !parent.u32().empty() ? parent + py::str("/") + entry->name : entry->name;
    };
    py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> out;
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        if (entry && !entry->is_folder) out[path(static_cast<int64_t>(i), 0)] = std::make_tuple(entry->start, entry->blocks, entry->size);
    }
    return out;
}

py::bytes STFS::read_file(const py::str& name) {
    auto [start, blocks, size] = files.at(name);
    if (size == 0) return py::bytes();   // an empty file's start block is the next file's: there is nothing to read
    return _read_chain(start, blocks, size);
}

int64_t STFS::extract_all(const py::str& outdir) {
    int64_t n = 0;
    for (const py::str& name : files) {
        py::str dest = py::os::path::join(outdir, name.replace(py::str("/"), py::os::sep).replace(py::str("\\"), py::os::sep));
        py::str parent = py::os::path::dirname(dest);
        py::os::makedirs(parent.u32().empty() ? outdir : parent, 0777, true);
        py::file f = py::open(dest, py::str("wb"));
        f.write(read_file(name));
        f.close();
        n += 1;
    }
    return n;
}

// =====================================================================================================
// writer (pack)
// =====================================================================================================
_WriteGeom::_WriteGeom(int64_t alloc_blocks_, int64_t first_hash_)
    : sex(1), step{0xAC, 0x723A}, alloc_blocks(alloc_blocks_), first_hash(first_hash_), top_level(alloc_blocks_ <= 0xAA ? 0 : 1) {}

int64_t _WriteGeom::cbdbn(int64_t b) {
    int64_t r = (py::floordiv(b + 0xAA, 0xAA) << sex) + b;
    if (b < 0xAA) return r;
    r += py::floordiv(b + 0x70E4, 0x70E4) << sex;
    return b < 0x70E4 ? r : r + (int64_t(1) << sex);
}

int64_t _WriteGeom::block_addr(int64_t b) { return (cbdbn(b) << 0xC) + first_hash; }

int64_t _WriteGeom::l0_backing(int64_t b) {
    if (b < 0xAA) return 0;
    return py::floordiv(b, 0xAA) * step[0] + 2;
}

int64_t _WriteGeom::l1_backing() { return step[0]; }

void _header_hash(py::bytearray buf, int64_t first_hash) {
    buf.set_slice(0x32C, 0x32C + 0x14, sha1_(buf.slice(0x344, first_hash).view()));
}

namespace {

py::str build_(const py::str& folder, const py::str& out_path, const py::bytes& tpl) {
    if (!is_magic(tpl.slice(0, 4))) throw py::ValueError("template is not an STFS package");
    int64_t first_hash = (py::unpack_from1<int64_t>(">I", tpl, 0x340) + 0xFFF) & 0xFFFFF000;
    py::bytearray header(tpl.slice(0, first_hash));

    std::vector<std::pair<py::str, py::bytes>> inner;
    if (py::os::path::isdir(folder)) {
        py::list<py::str> names = py::sorted(py::os::listdir(folder));
        for (int64_t i = 0; i < names.size(); ++i) {
            py::str p = py::os::path::join(folder, names[i]);
            if (py::os::path::isfile(p)) inner.emplace_back(names[i], read_all_(p));
        }
    } else {
        py::bytes data = read_all_(folder);
        inner.emplace_back(py::os::path::basename(folder), data);
    }
    if (inner.empty()) throw py::ValueError("no files to pack in " + py::repr(folder).utf8());

    // assign data blocks: file table at block 0.., then each file's content
    int64_t ft_nblocks = std::max<int64_t>(1, py::floordiv(static_cast<int64_t>(inner.size()) + 0x3F, 0x40));
    struct FileEntry {
        py::str name;
        int64_t start;
        int64_t nb;
        int64_t size;
    };
    std::vector<FileEntry> entries;
    std::vector<py::bytes> contents;
    int64_t cur = ft_nblocks;
    for (const auto& [nm, data] : inner) {
        int64_t nb = std::max<int64_t>(1, py::floordiv(data.size() + 0xFFF, 0x1000));
        entries.push_back(FileEntry{nm, cur, nb, data.size()});
        for (int64_t i = 0; i < nb; ++i) contents.push_back(data.slice(i * 0x1000, (i + 1) * 0x1000).ljust(0x1000, py::bytes(std::string(1, '\0'))));
        cur += nb;
    }
    int64_t alloc_blocks = cur;

    // file-table block(s)
    py::bytearray ftbuf = py::bytearray::zeros(ft_nblocks * 0x1000);
    for (int64_t i = 0; i < static_cast<int64_t>(entries.size()); ++i) {
        const FileEntry& en = entries[static_cast<size_t>(i)];
        py::bytes b = en.name.encode("latin1", "ignore").slice(0, 0x28);
        ftbuf.set_slice(i * 0x40, i * 0x40 + b.size(), b);
        ftbuf[i * 0x40 + 0x28] = b.size();
        py::pack_into("<I", ftbuf, i * 0x40 + 0x29, en.nb);
        ftbuf[i * 0x40 + 0x2B] = 0;
        py::pack_into("<I", ftbuf, i * 0x40 + 0x2C, en.nb);
        ftbuf[i * 0x40 + 0x2E] = 0;
        py::pack_into("<I", ftbuf, i * 0x40 + 0x2F, en.start);
        ftbuf[i * 0x40 + 0x31] = 0;
        py::pack_into(">H", ftbuf, i * 0x40 + 0x32, 0xFFFF);   // root
        py::pack_into(">I", ftbuf, i * 0x40 + 0x34, en.size);
    }
    std::vector<py::bytes> blocks;
    for (int64_t i = 0; i < ft_nblocks; ++i) blocks.push_back(ftbuf.slice(i * 0x1000, (i + 1) * 0x1000).view());
    for (const auto& c : contents) blocks.push_back(c);

    // next-block chains
    py::dict<int64_t, int64_t> nextb;
    for (int64_t j = 0; j < ft_nblocks; ++j) nextb[j] = j < ft_nblocks - 1 ? j + 1 : 0xFFFFFF;
    for (const auto& en : entries)
        for (int64_t i = 0; i < en.nb; ++i) nextb[en.start + i] = i < en.nb - 1 ? en.start + i + 1 : 0xFFFFFF;

    _WriteGeom g(alloc_blocks, first_hash);
    int64_t max_back = 0;
    for (int64_t b = 0; b < alloc_blocks; ++b) max_back = std::max({max_back, g.cbdbn(b), g.l0_backing(b) + 1});
    if (g.top_level == 1) max_back = std::max(max_back, g.l1_backing() + 1);
    py::bytearray buf = py::bytearray::zeros(first_hash + (max_back + 2) * 0x1000);
    buf.set_slice(0, header.size(), header.view());
    buf.set_slice(0, 4, tpl.slice(0, 4) == lit("CON ", 4) ? lit("CON ", 4) : tpl.slice(0, 4));

    for (int64_t b = 0; b < alloc_blocks; ++b) {
        int64_t off = g.block_addr(b);
        buf.set_slice(off, off + 0x1000, blocks[static_cast<size_t>(b)]);
    }

    // L0 hash tables (+ male backup copy)
    int64_t ngroups = py::floordiv(alloc_blocks + 0xA9, 0xAA);
    std::vector<py::bytes> l0_hashes;
    for (int64_t grp = 0; grp < ngroups; ++grp) {
        int64_t base = (g.l0_backing(grp * 0xAA) << 0xC) + first_hash;
        for (int64_t k = 0; k < 0xAA; ++k) {
            int64_t b = grp * 0xAA + k;
            if (b >= alloc_blocks) break;
            int64_t ea = base + k * 0x18;
            buf.set_slice(ea, ea + 0x14, sha1_(blocks[static_cast<size_t>(b)]));
            buf[ea + 0x14] = 0xC0;
            int64_t nb = nextb.get(b, 0xFFFFFF);
            buf[ea + 0x15] = (nb >> 16) & 0xFF;
            buf[ea + 0x16] = (nb >> 8) & 0xFF;
            buf[ea + 0x17] = nb & 0xFF;
        }
        py::bytes table = buf.slice(base, base + 0x1000).view();
        buf.set_slice(base + 0x1000, base + 0x2000, table);
        l0_hashes.push_back(sha1_(table));
    }

    py::bytes top_hash;
    if (g.top_level == 1) {
        int64_t l1 = (g.l1_backing() << 0xC) + first_hash;
        for (int64_t i = 0; i < static_cast<int64_t>(l0_hashes.size()); ++i) {
            int64_t ea = l1 + i * 0x18;
            buf.set_slice(ea, ea + 0x14, l0_hashes[static_cast<size_t>(i)]);
            buf[ea + 0x14] = 0x80;
        }
        py::bytes table = buf.slice(l1, l1 + 0x1000).view();
        buf.set_slice(l1 + 0x1000, l1 + 0x2000, table);
        top_hash = sha1_(table);
    } else {
        top_hash = l0_hashes[0];
    }

    // volume descriptor + sizes (male layout: block separation 0)
    buf[0x379] = 0x24;
    buf[0x37B] = 0x00;
    py::pack_into("<H", buf, 0x37C, ft_nblocks);
    buf[0x37E] = 0;
    buf[0x37F] = 0;
    buf[0x380] = 0;
    buf.set_slice(0x381, 0x381 + 0x14, top_hash);
    py::pack_into(">I", buf, 0x395, alloc_blocks);
    py::pack_into(">I", buf, 0x399, int64_t(0));
    py::pack_into(">Q", buf, 0x34C, alloc_blocks * 0x1000);

    _header_hash(buf, first_hash);
    py::file f = py::open(out_path, py::str("wb"));
    f.write(buf.view());
    f.close();
    return out_path;
}

}  // namespace

py::str build(const py::str& folder, const py::str& out_path, const py::bytes& template_bytes) {
    return build_(folder, out_path, template_bytes);
}

py::str build(const py::str& folder, const py::str& out_path, const py::str& template_path) {
    return build_(folder, out_path, read_all_(template_path));
}

// =====================================================================================================
// resign (CON)
// =====================================================================================================
const py::bytes _SHA1_DER = py::bytes::fromhex(py::str("3021300906052b0e03021a05000414"));

BigInt _bn(const py::bytes& b) {
    // swapped = b"".join(b[i:i + 8][::-1] for i in range(0, len(b), 8)); int.from_bytes(swapped, "little")
    return BigInt::from_bytes(swap_qwords_(b), "little");
}

std::tuple<py::bytes, std::tuple<int64_t, BigInt>> _con_sign(const py::bytes& region, const py::bytes& kv) {
    int64_t e = py::unpack_from1<int64_t>(">I", kv, KV_PRV_OFF + 4);
    BigInt n = _bn(kv.slice(KV_PRV_OFF + 0x10, KV_PRV_OFF + 0x10 + 0x80));
    BigInt p = _bn(kv.slice(KV_PRV_OFF + 0x90, KV_PRV_OFF + 0x90 + 0x40));
    BigInt q = _bn(kv.slice(KV_PRV_OFF + 0xD0, KV_PRV_OFF + 0xD0 + 0x40));

    // d = pow(e, -1, (p - 1) * (q - 1)) -- p or q being 0 makes a factor -1, so track the sign
    const BigInt one(1);
    bool pn = p.is_zero(), qn = q.is_zero();
    BigInt pm = pn ? one : p - one, qm = qn ? one : q - one;
    bool phi_neg = pn != qn;
    BigInt phi = pm * qm;
    if (phi.is_zero()) throw py::ValueError("pow() 3rd argument cannot be 0");
    BigInt inv = BigInt::modinv(BigInt(static_cast<uint64_t>(e)), phi);
    bool d_neg = phi_neg && !inv.is_zero();   // (a negative modulus gives a result in (modulus, 0])
    BigInt d = d_neg ? phi - inv : inv;

    py::bytes digest_info = _SHA1_DER + sha1_(region);
    py::bytes pad(std::string(static_cast<size_t>(std::max<int64_t>(0, 0x80 - digest_info.size() - 3)), '\xFF'));
    py::bytes block = lit("\x00\x01", 2) + pad + lit("\x00", 1) + digest_info;
    py::bytes sig = pow_signed_(BigInt::from_bytes(block, "big"), d_neg, d, n).to_bytes(0x80, "big");
    std::string rev = sig.raw();
    std::reverse(rev.begin(), rev.end());
    return {py::bytes(rev), {e, n}};
}

bool _con_verify(const py::bytes& region, const py::bytes& sig, const py::bytes& modulus_bnqw, int64_t e) {
    BigInt n = _bn(modulus_bnqw);
    std::string rev = sig.raw();
    std::reverse(rev.begin(), rev.end());
    if (e < 0) throw py::ValueError("pow() negative exponent is not supported here");
    py::bytes block = BigInt::powmod(BigInt::from_bytes(py::bytes(rev), "big"), BigInt(static_cast<uint64_t>(e)), n).to_bytes(0x80, "big");
    py::bytes digest_info = _SHA1_DER + sha1_(region);
    return block.endswith(digest_info) && block.slice(0, 2) == lit("\x00\x01", 2);
}

py::bytes resign(const std::variant<py::bytes, py::str>& package, const std::variant<py::bytes, py::str>& kv_in,
                 const std::optional<py::str>& out_path, bool bind_console) {
    py::bytearray data(std::holds_alternative<py::bytes>(package) ? std::get<py::bytes>(package) : read_all_(std::get<py::str>(package)));
    py::bytes kv = std::holds_alternative<py::bytes>(kv_in) ? std::get<py::bytes>(kv_in) : read_all_(std::get<py::str>(kv_in));
    if (!is_magic(data.slice(0, 4).view()))
        throw py::ValueError("not an STFS package (magic " + py::repr(data.slice(0, 4).view()).utf8() + ")");
    if (kv.size() < 0x4000)
        throw py::ValueError("keyvault too small (" + d_(kv.size()) + " bytes); expected a decrypted KV_dec.bin of 0x4000 bytes");
    bool converting = data.slice(0, 4).view() != lit("CON ", 4);
    if (converting) {
        // LIVE/PIRS -> CON: flip the magic and reset the license descriptor to a single Unrestricted entry
        // (the state console content uses). The cert copy below fills 0x04..0x1AC and the signature fills
        // 0x1AC..0x22C, so the whole Microsoft-signature block is overwritten.
        data.set_slice(0x00, 0x04, lit("CON ", 4));
        data.set_slice(0x22C, 0x32C, py::bytes(std::string(0x100, '\0')));   // 0x10 license entries
        data.set_slice(0x22C, 0x234, py::bytes(std::string(8, '\xFF')));     // license[0] = Unrestricted
    }
    // 1) console certificate -> header 0x04 (also sets cert size 0x1A8 at 0x04)
    data.set_slice(0x04, 0x04 + KV_CERT_LEN, kv.slice(KV_CERT_OFF, KV_CERT_OFF + KV_CERT_LEN));
    // optional: bind the content to this console (metadata console id @0x36C)
    if (bind_console) data.set_slice(0x36C, 0x371, kv.slice(KV_CERT_OFF + 2, KV_CERT_OFF + 7));
    // 2) header hash @0x32C over the current metadata (inside the signed region).
    //    Hash 0x344 up to the first hash table, but never past end-of-file.
    int64_t first_hash = (py::unpack_from1<int64_t>(">I", data.view(), 0x340) + 0xFFF) & 0xFFFFF000;
    first_hash = std::min(first_hash, data.size());
    data.set_slice(0x32C, 0x32C + 0x14, sha1_(data.slice(0x344, first_hash).view()));
    // 3) sign header[0x22C:0x344] with the console private key
    py::bytes region = data.slice(0x22C, 0x344).view();
    auto [sig, key] = _con_sign(region, kv);
    data.set_slice(0x1AC, 0x1AC + 0x80, sig);
    // 4) self-check: signature must verify against the certificate's own modulus
    py::bytes cert_mod = data.slice(0x2C, 0x2C + 0x80).view();
    int64_t cert_exp = py::unpack_from1<int64_t>(">I", data.view(), 0x28);
    if (!_con_verify(region, sig, cert_mod, cert_exp)) throw py::RuntimeError("internal error: resigned signature failed self-verify");
    std::optional<py::str> dest;
    if (out_path && !out_path->u32().empty()) dest = out_path;
    else if (std::holds_alternative<py::str>(package) && !std::get<py::str>(package).u32().empty()) dest = std::get<py::str>(package);
    if (dest) {
        py::file f = py::open(*dest, py::str("wb"));
        f.write(data.view());
        f.close();
    }
    return data.view();
}

// =====================================================================================================
// CLI
// =====================================================================================================
void main(const py::list<py::str>& argv) {
    std::vector<py::str> a;
    for (int64_t i = 0; i < argv.size(); ++i) a.push_back(argv[i]);
    auto A = [&](size_t i) { return a[i]; };
    if (a.empty() || u8_(a[0]) == "-h" || u8_(a[0]) == "--help") {
        say_("usage: stfs {info|list|extract} <package> [outdir]");
        say_("       stfs pack   <folder> <out.stfs> <template>");
        say_("       stfs resign <package> <KV_dec.bin> [out] [--bind]");
        say_("              resigns CON, and converts LIVE/PIRS -> CON");
        say_("              --bind also stamps the console id into the metadata");
        return;
    }
    const std::string op = u8_(a[0]);
    if (op == "pack") {
        if (a.size() < 4) {
            say_("usage: stfs pack <folder> <out.stfs> <template-package>");
            throw SystemExit(2);
        }
        py::str out = build(A(1), A(2), A(3));
        STFS r(read_all_(out));
        say_("packed " + d_(r.files.size()) + " files -> " + u8_(out) + "  (reads back: " + d_(r.files.size()) + " files)");
        return;
    }
    if (op == "resign") {
        bool bind = false;
        std::vector<py::str> b;
        for (const auto& x : a) {
            if (u8_(x) == "--bind") bind = true;
            else b.push_back(x);
        }
        if (b.size() < 3) {
            say_("usage: stfs resign <package> <KV_dec.bin> [out] [--bind]");
            throw SystemExit(2);
        }
        py::str out = b.size() > 3 ? b[3] : b[1];
        py::bytes src_magic = read_all_(b[1]).slice(0, 4);
        resign(std::variant<py::bytes, py::str>(b[1]), std::variant<py::bytes, py::str>(b[2]), out, bind);
        py::bytes kv = read_all_(b[2]);
        std::string cid = upper_(kv.slice(KV_CERT_OFF + 2, KV_CERT_OFF + 7).hex().utf8());
        say_("resigned " + u8_(py::os::path::basename(b[1])));
        if (src_magic != lit("CON ", 4)) say_("  converted  : " + src_magic.decode("latin-1").utf8() + " -> CON");
        say_("  console id : " + cid + "  (from " + u8_(py::os::path::basename(b[2])) + ")" + (bind ? "  [bound @0x36C]" : ""));
        say_("  signature  : verified against console certificate");
        say_("  written    : " + u8_(out));
        return;
    }
    if (a.size() < 2) {
        say_("usage: stfs " + op + " <package> [args]");
        throw SystemExit(2);
    }
    STFS s(read_all_(a[1]));
    if (op == "info") {
        say_("package     : " + u8_(py::os::path::basename(a[1])));
        say_("magic       : " + s.magic.decode("latin-1").utf8());
        say_("title       : " + u8_(py::repr(s.title_name)));
        say_("display name: " + u8_(py::repr(s.display_name)));
        say_("title id    : " + u8_(s.title_id) + "   content type: 0x" + hex_upper_(s.content_type));
        say_("profile id  : " + u8_(s.profile_id));
        say_("layout      : " + std::string(s.sex == 0 ? "female" : "male") + "  top-level=" + d_(s.top_level) + "  alloc_blocks=" + d_(s.alloc_blocks));
        say_("files       : " + d_(s.files.size()));
    } else if (op == "list") {
        py::list<py::str> names = py::sorted(py::list<py::str>::from(s.files.keys()));
        int64_t total = 0;
        for (int64_t i = 0; i < names.size(); ++i) {
            int64_t size = std::get<2>(s.files.at(names[i]));
            std::string num = d_(size);
            say_("  " + std::string(num.size() < 10 ? 10 - num.size() : 0, ' ') + num + "  " + u8_(names[i]));
        }
        for (const auto& v : s.files.values()) total += std::get<2>(v);
        say_("total: " + d_(s.files.size()) + " files, " + d_(total) + " bytes");
    } else if (op == "extract") {
        py::str outdir = a.size() > 2 ? a[2] : py::str(u8_(std::get<0>(py::os::path::splitext(a[1]))) + "_extracted");
        say_("extracted " + d_(s.extract_all(outdir)) + " files -> " + u8_(outdir));
    } else {
        say_("unknown op " + u8_(py::repr(a[0])) + " (info|list|extract)");
        throw SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::stfs
