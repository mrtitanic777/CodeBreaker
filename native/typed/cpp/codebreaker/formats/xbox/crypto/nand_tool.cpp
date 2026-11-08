// codebreaker/formats/xbox/crypto/nand_tool.cpp -- typed C++ from codebreaker.formats.xbox.crypto.nand_tool.

#include "codebreaker/formats/xbox/crypto/nand_tool.hpp"

#include <algorithm>
#include <string>

namespace codebreaker::formats::xbox::crypto::nand_tool {

namespace {

// -- the three spare layouts: where each field lies in the 16 bytes --------------------------------------
// (a slot is a byte of the structure; PAIR_ID is block_id_0:4 | fs_unused_0:4, TYPE_ECC is
// fs_block_type:6 | ecc_3:2 -- the first field of a pair in the high bits)
enum class Slot {
    BLOCK_ID_1, PAIR_ID, FS_SEQUENCE_0, FS_SEQUENCE_1, FS_SEQUENCE_2, FS_SEQUENCE_3, BAD_BLOCK, FS_SIZE_1,
    FS_SIZE_0, FS_PAGE_COUNT, FS_UNUSED_1_0, FS_UNUSED_1_1, TYPE_ECC, ECC_2, ECC_1, ECC_0
};
using Layout = std::array<Slot, 16>;

constexpr Layout SMALLBLOCK_LAYOUT = {
    Slot::BLOCK_ID_1, Slot::PAIR_ID, Slot::FS_SEQUENCE_0, Slot::FS_SEQUENCE_1, Slot::FS_SEQUENCE_2,
    Slot::BAD_BLOCK, Slot::FS_SEQUENCE_3, Slot::FS_SIZE_1, Slot::FS_SIZE_0, Slot::FS_PAGE_COUNT,
    Slot::FS_UNUSED_1_0, Slot::FS_UNUSED_1_1, Slot::TYPE_ECC, Slot::ECC_2, Slot::ECC_1, Slot::ECC_0};

constexpr Layout BIGONSMALL_LAYOUT = {
    Slot::FS_SEQUENCE_0, Slot::BLOCK_ID_1, Slot::PAIR_ID, Slot::FS_SEQUENCE_1, Slot::FS_SEQUENCE_2,
    Slot::BAD_BLOCK, Slot::FS_SEQUENCE_3, Slot::FS_SIZE_1, Slot::FS_SIZE_0, Slot::FS_PAGE_COUNT,
    Slot::FS_UNUSED_1_0, Slot::FS_UNUSED_1_1, Slot::TYPE_ECC, Slot::ECC_2, Slot::ECC_1, Slot::ECC_0};

constexpr Layout BIGBLOCK_LAYOUT = {
    Slot::BAD_BLOCK, Slot::BLOCK_ID_1, Slot::PAIR_ID, Slot::FS_SEQUENCE_1, Slot::FS_SEQUENCE_2,
    Slot::FS_SEQUENCE_0, Slot::FS_SEQUENCE_3, Slot::FS_SIZE_1, Slot::FS_SIZE_0, Slot::FS_PAGE_COUNT,
    Slot::FS_UNUSED_1_0, Slot::FS_UNUSED_1_1, Slot::TYPE_ECC, Slot::ECC_2, Slot::ECC_1, Slot::ECC_0};

void put_slot(BlockSpare& s, Slot slot, BYTE v) {
    switch (slot) {
        case Slot::BLOCK_ID_1: s.block_id_1 = v; break;
        case Slot::PAIR_ID: s.block_id_0 = v >> 4; s.fs_unused_0 = v & 0xF; break;
        case Slot::FS_SEQUENCE_0: s.fs_sequence_0 = v; break;
        case Slot::FS_SEQUENCE_1: s.fs_sequence_1 = v; break;
        case Slot::FS_SEQUENCE_2: s.fs_sequence_2 = v; break;
        case Slot::FS_SEQUENCE_3: s.fs_sequence_3 = v; break;
        case Slot::BAD_BLOCK: s.bad_block = v; break;
        case Slot::FS_SIZE_1: s.fs_size_1 = v; break;
        case Slot::FS_SIZE_0: s.fs_size_0 = v; break;
        case Slot::FS_PAGE_COUNT: s.fs_page_count = v; break;
        case Slot::FS_UNUSED_1_0: s.fs_unused_1[0] = v; break;
        case Slot::FS_UNUSED_1_1: s.fs_unused_1[1] = v; break;
        case Slot::TYPE_ECC: s.fs_block_type = v >> 2; s.ecc_3 = v & 0x3; break;
        case Slot::ECC_2: s.ecc_2 = v; break;
        case Slot::ECC_1: s.ecc_1 = v; break;
        case Slot::ECC_0: s.ecc_0 = v; break;
    }
}

BYTE get_slot(const BlockSpare& s, Slot slot) {
    switch (slot) {
        case Slot::BLOCK_ID_1: return s.block_id_1;
        case Slot::PAIR_ID: return static_cast<BYTE>(((s.block_id_0 & 0xF) << 4) | (s.fs_unused_0 & 0xF));
        case Slot::FS_SEQUENCE_0: return s.fs_sequence_0;
        case Slot::FS_SEQUENCE_1: return s.fs_sequence_1;
        case Slot::FS_SEQUENCE_2: return s.fs_sequence_2;
        case Slot::FS_SEQUENCE_3: return s.fs_sequence_3;
        case Slot::BAD_BLOCK: return s.bad_block;
        case Slot::FS_SIZE_1: return s.fs_size_1;
        case Slot::FS_SIZE_0: return s.fs_size_0;
        case Slot::FS_PAGE_COUNT: return s.fs_page_count;
        case Slot::FS_UNUSED_1_0: return s.fs_unused_1[0];
        case Slot::FS_UNUSED_1_1: return s.fs_unused_1[1];
        case Slot::TYPE_ECC: return static_cast<BYTE>(((s.fs_block_type & 0x3F) << 2) | (s.ecc_3 & 0x3));
        case Slot::ECC_2: return s.ecc_2;
        case Slot::ECC_1: return s.ecc_1;
        case Slot::ECC_0: return s.ecc_0;
    }
    return 0;
}

// Type.from_buffer_copy(buffer)
template <class T> T from_buffer_(const py::bytes& buffer, const Layout& layout) {
    if (buffer.size() < T::SIZE) {
        throw py::ValueError("Buffer size too small (" + std::to_string(buffer.size()) + " instead of at least " +
                             std::to_string(T::SIZE) + " bytes)");
    }
    T t;
    for (int64_t i = 0; i < T::SIZE; ++i) put_slot(t, layout[static_cast<size_t>(i)], static_cast<BYTE>(buffer.byte_(i)));
    return t;
}

// bytes(structure)
py::bytes to_bytes_(const BlockSpare& s, const Layout& layout) {
    std::string out;
    for (Slot slot : layout) out.push_back(static_cast<char>(get_slot(s, slot)));
    return py::bytes(out);
}

}  // namespace

// ((self.block_id_0 << 8) & 0xF) + (self.block_id_1 & 0xFF)
int64_t BlockSpare::block_id() const { return ((static_cast<int64_t>(block_id_0) << 8) & 0xF) + (block_id_1 & 0xFF); }

// ((self.fs_size_0 << 8) & 0xFF) + (self.fs_size_1 & 0xFF)
int64_t BlockSpare::fs_size() const { return ((static_cast<int64_t>(fs_size_0) << 8) & 0xFF) + (fs_size_1 & 0xFF); }

SMALLBLOCK SMALLBLOCK::from_buffer_copy(const py::bytes& buffer) { return from_buffer_<SMALLBLOCK>(buffer, SMALLBLOCK_LAYOUT); }
py::bytes SMALLBLOCK::to_bytes() const { return to_bytes_(*this, SMALLBLOCK_LAYOUT); }

BIGONSMALL BIGONSMALL::from_buffer_copy(const py::bytes& buffer) { return from_buffer_<BIGONSMALL>(buffer, BIGONSMALL_LAYOUT); }
py::bytes BIGONSMALL::to_bytes() const { return to_bytes_(*this, BIGONSMALL_LAYOUT); }

BIGBLOCK BIGBLOCK::from_buffer_copy(const py::bytes& buffer) { return from_buffer_<BIGBLOCK>(buffer, BIGBLOCK_LAYOUT); }
py::bytes BIGBLOCK::to_bytes() const { return to_bytes_(*this, BIGBLOCK_LAYOUT); }

void BIGBLOCK::set_block_id(int64_t) { throw py::NotImplementedError("block_id can't be set yet!"); }
void BIGBLOCK::set_fs_size(int64_t) { throw py::NotImplementedError("fs_size can't be set yet!"); }

int64_t calc_page_ecc(const py::bytes& data_in, const py::bytes& spare) {
    // (the Python copies a bytes into a bytearray; reading one or the other is the same)
    const py::bytes* data = &data_in;

    int64_t val = 0;
    int64_t v = 0;
    int64_t idx = 0;
    for (int64_t bit = 0; bit < 0x1066; ++bit) {
        if (!(bit & 31)) {
            if (bit == 0x1000) {
                data = &spare;
                idx = 0;
            }
            v = py::unpack_from1<int64_t>("<I", *data, idx);
            v = ~v;
            idx += 4;
        }
        val ^= v & 1;
        v >>= 1;
        if (val & 1) {
            val ^= 0x6954559;
        }
        val >>= 1;
    }
    return ~val & 0xFFFFFFFF;
}

namespace {

// the body of fix_page_ecc: fill the ECC into the spare, in place
void fix_spare_(const py::bytes& data, const py::bytearray& spare) {
    int64_t val = calc_page_ecc(data, spare.view());
    spare[12] = (static_cast<int64_t>(spare[12]) & 0x3F) + ((val << 6) & 0xC0);
    spare[13] = (val >> 2) & 0xFF;
    spare[14] = (val >> 10) & 0xFF;
    spare[15] = (val >> 18) & 0xFF;
}

}  // namespace

std::tuple<py::bytes, py::bytearray> fix_page_ecc(const py::bytes& data, const py::bytes& spare) {
    py::bytearray s(spare);
    fix_spare_(data, s);
    return {data, s};
}

std::tuple<py::bytes, py::bytearray> fix_page_ecc(const py::bytes& data, const py::bytearray& spare) {
    fix_spare_(data, spare);
    return {data, spare};
}

std::tuple<py::bytearray, py::bytearray> fix_page_ecc(const py::bytearray& data, const py::bytes& spare) {
    py::bytearray s(spare);
    fix_spare_(data.view(), s);
    return {data, s};
}

std::tuple<py::bytearray, py::bytearray> fix_page_ecc(const py::bytearray& data, const py::bytearray& spare) {
    fix_spare_(data.view(), spare);
    return {data, spare};
}

// -- NANDImage ---------------------------------------------------------------------------------------------

StreamIO::StreamIO& NANDImage::stream_() {
    if (!stream) throw py::AttributeError("'NoneType' object has no attribute 'seek'");
    if (closed_) throw py::ValueError("I/O operation on closed file.");
    return *stream;
}

int64_t NANDImage::file_offset() const { return _file_offset; }

void NANDImage::set_file_offset(int64_t offset) {
    _flash_offset = file_to_flash_offset(offset);
    _file_offset = offset;
    stream_().seek(_file_offset);
}

int64_t NANDImage::flash_offset() const { return _flash_offset; }

void NANDImage::set_flash_offset(int64_t offset) {
    _file_offset = flash_to_file_offset(offset);
    _flash_offset = offset;
    stream_().seek(_file_offset);
}

NANDImage::NANDImage(const py::str& filename, int64_t mode_) {
    reset();

    mode = mode_;
    if (!py::os::path::exists(filename)) {
        throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(filename).utf8());
    }
    stream.emplace(filename);   // open(filename, "r+b")

    // seek to the end
    file_seek(0, SEEK_END_);
    // get size with spare data
    file_size = file_tell();
    // get number of pages
    num_pages = py::floordiv(file_size, 528);
    // get size without spare data
    flash_size = num_pages * 512;
    // seek back to the start
    file_seek(0);
}

NANDImage::NANDImage(const py::bytes& data, int64_t mode_) {
    reset();

    mode = mode_;
    stream.emplace(data);   // BytesIO(data)

    file_seek(0, SEEK_END_);
    file_size = file_tell();
    num_pages = py::floordiv(file_size, 528);
    flash_size = num_pages * 512;
    file_seek(0);
}

NANDImage::NANDImage(const py::bytearray& data, int64_t mode_) : NANDImage(data.view(), mode_) {}

NANDImage& NANDImage::__enter__() { return *this; }

void NANDImage::__exit__() {
    flush();   // self.stream.flush()
    stream_().close();
    closed_ = true;
}

void NANDImage::reset() {
    mode = MODE_FILE;
    stream.reset();
    closed_ = false;
    file_size = 0;
    flash_size = 0;
    num_pages = 0;
}

// stream primitives
int64_t NANDImage::seek(int64_t offset, int64_t whence) {
    return mode == MODE_FILE ? file_seek(offset, whence) : flash_seek(offset, whence);
}

int64_t NANDImage::tell() { return mode == MODE_FILE ? file_tell() : flash_tell(); }

py::bytes NANDImage::read(int64_t num) { return mode == MODE_FILE ? file_read(num) : flash_read(num); }

int64_t NANDImage::write(const py::bytes& data) { return mode == MODE_FILE ? file_write(data) : flash_write(data); }

void NANDImage::flush() {
    stream_();   // (BytesIO / the file: flush; the stream writes through, so only the closed check remains)
}

// extended I/O functions

// offset translation functions
int64_t NANDImage::file_to_flash_offset(int64_t file_offset_) const {
    if (file_offset_ < 0) {
        file_offset_ = flash_size + file_offset_;
    }
    return (py::floordiv(file_offset_, 528) * 512) + py::mod(file_offset_, 528);
}

int64_t NANDImage::flash_to_file_offset(int64_t flash_offset_) const {
    if (flash_offset_ < 0) {
        flash_offset_ = file_size + flash_offset_;
    }
    return (py::floordiv(flash_offset_, 512) * 528) + py::mod(flash_offset_, 512);
}

int64_t NANDImage::file_offset_to_page(int64_t offset) const { return py::floordiv(offset, 528) + 1; }

int64_t NANDImage::flash_offset_to_page(int64_t offset) const { return py::floordiv(offset, 512) + 1; }

int64_t NANDImage::flash_calc_page_offset(int64_t offset) const { return offset - (py::floordiv(offset, 512) * 512); }

int64_t NANDImage::calc_page_offset(int64_t num) const { return (num - 1) * 528; }

int64_t NANDImage::calc_spare_offset(int64_t num) const {
    int64_t so = ((num - 1) * 528) - 16;
    return (num - 1) == 0 ? 512 : so;
}

// file primitives
int64_t NANDImage::file_tell() { return stream_().tell(); }

int64_t NANDImage::file_seek(int64_t offset, int64_t whence) {
    int64_t no = stream_().seek(offset, whence);
    set_file_offset(no);
    return no;
}

py::bytes NANDImage::file_read(int64_t num) {
    py::bytes data;
    if (num > 0) {
        data = stream_().read(num);
    } else {
        data = stream_().read();
    }
    set_file_offset(file_tell());
    return data;
}

int64_t NANDImage::file_write(const py::bytes& data) {
    int64_t nw = stream_().write(data);
    set_file_offset(file_tell());
    return nw;
}

// flash primitives
int64_t NANDImage::flash_tell() { return file_to_flash_offset(file_tell()); }

int64_t NANDImage::flash_seek(int64_t offset, int64_t whence) {
    int64_t no = 0;  // promise
    if (whence == SEEK_SET_) {
        if (offset >= 0) {
            // no = self.file_seek(self.flash_to_file_offset(offset), SEEK_SET)
            set_flash_offset(offset);
            no = flash_tell();
        } else if (offset < 0) {
            no = file_seek(flash_to_file_offset(flash_size - offset), SEEK_SET_);
        }
    } else if (whence == SEEK_CUR_) {
        if (offset >= 0) {
            // no = self.file_seek(self.flash_to_file_offset(offset), SEEK_CUR)
            set_flash_offset(flash_offset() + offset);
            no = flash_tell();
        } else if (offset < 0) {
            no = file_seek(flash_to_file_offset(flash_offset() - offset), SEEK_CUR_);
        }
    } else if (whence == SEEK_END_) {
        if (offset == 0) {
            no = file_seek(0, SEEK_END_);
        } else if (offset < 0) {
            no = file_seek(flash_to_file_offset(flash_size - offset), SEEK_END_);
        }
    }
    // self.file_offset = no
    return file_to_flash_offset(no);
}

py::bytes NANDImage::flash_read(int64_t num) {
    int64_t strt_page = flash_offset_to_page(flash_offset());
    int64_t strt_offs = flash_calc_page_offset(flash_offset());
    int64_t stop_page = flash_offset_to_page(flash_offset() + num);
    int64_t stop_offs = flash_calc_page_offset(flash_offset() + num);

    //print("\nFlash Read:")
    //print(f"\tFlash Size:   0x{self.flash_size:04X}")
    //print(f"\tStart Page:   {strt_page}")
    //print(f"\tStart Offset: {strt_offs}")
    //print(f"\tStop Page:    {stop_page}")
    //print(f"\tStop Offset:  {stop_offs}")

    std::string bio;  // BytesIO()
    if (strt_page == stop_page) {  // only one page
        bio += get_page(strt_page).slice(strt_offs, stop_offs).raw();
    }

    for (int64_t page_num = strt_page; page_num < stop_page; ++page_num) {
        py::bytes tmp_page = get_page(page_num);
        if (page_num == strt_page) {  // first page
            bio += tmp_page.slice_from(strt_offs).raw();
        } else if (page_num == stop_page) {  // last page (never reached: the loop stops before stop_page)
            bio += tmp_page.slice_to(stop_offs).raw();
        } else {  // between first and last
            bio += tmp_page.raw();
        }
    }
    // self.flash_offset = self.flash_tell()
    return py::bytes(bio);
}

int64_t NANDImage::flash_write(const py::bytes& data) {
    int64_t strt_page = flash_offset_to_page(flash_offset());
    int64_t strt_offs = flash_calc_page_offset(flash_offset());
    int64_t stop_page = flash_offset_to_page(flash_offset() + py::len(data));
    int64_t stop_offs = flash_calc_page_offset(flash_offset() + py::len(data));

    // print("\nFlash Write:")
    // print(f"\tFlash Size:   0x{self.flash_size:04X}")
    // print(f"\tStart Page:   {strt_page}")
    // print(f"\tStart Offset: {strt_offs}")
    // print(f"\tStop Page:    {stop_page}")
    // print(f"\tStop Offset:  {stop_offs}")

    int64_t nw = 0;
    int64_t bio_pos = 0;  // BytesIO(data).read(n): n < 0 reads the rest
    auto bio_read = [&](int64_t n) -> py::bytes {
        int64_t have = py::len(data) - bio_pos;
        int64_t take = n < 0 ? have : std::min(n, have);
        py::bytes r = data.slice(bio_pos, bio_pos + take);
        bio_pos += take;
        return r;
    };
    if (strt_page == stop_page) {  // only one page
        int64_t chunk_size = stop_offs - strt_offs;
        py::bytearray tmp_page(get_page(strt_page));
        // pack_into(f"{chunk_size}s", tmp_page, strt_offs, bio.read(chunk_size))
        tmp_page.set_slice(strt_offs, strt_offs + chunk_size, bio_read(chunk_size));
        // (the page is edited but never written back, as in the Python)
    }

    for (int64_t page_num = strt_page; page_num < stop_page; ++page_num) {
        py::bytearray tmp_page(get_page(page_num));
        if (page_num == strt_page) {  // first page
            int64_t chunk_size = 512 - strt_offs;
            // pack_into(f"{chunk_size}s", tmp_page, strt_offs, bio.read(512 - strt_offs))
            tmp_page.set_slice(strt_offs, strt_offs + chunk_size, bio_read(512 - strt_offs));
        } else if (page_num == stop_page) {  // last page (never reached: the loop stops before stop_page)
            int64_t chunk_size = 512 - stop_offs;
            // pack_into(f"{chunk_size}s", tmp_page, 0, bio.read(chunk_size))
            tmp_page.set_slice(0, chunk_size, bio_read(chunk_size));
        } else {  // between first and last
            // pack_into(f"512s", tmp_page, 0, bio.read(512))
            tmp_page.set_slice(0, 512, bio_read(512));
        }
        nw += set_page(page_num, tmp_page.view());
    }
    // self.flash_offset = self.flash_tell()
    return nw;
}

// page I/O
py::bytes NANDImage::get_page(int64_t num) {
    file_seek(calc_page_offset(num));
    return file_read(512);
}

py::bytes NANDImage::get_spare(int64_t num) {
    file_seek(calc_spare_offset(num));
    return file_read(16);
}

int64_t NANDImage::set_page(int64_t num, const py::bytes& data) {
    if (!(1 <= num && num <= num_pages)) throw py::AssertionError("Page number out of range");
    if (!(py::len(data) == 512)) throw py::AssertionError("Invalid page size");
    py::bytes spare_data = get_spare(num);
    file_seek(calc_page_offset(num));
    auto [page_data, spare_fixed] = fix_page_ecc(data, spare_data);
    int64_t nw = file_write(page_data);
    nw += file_write(spare_fixed.view());
    return nw;
}

int64_t NANDImage::set_spare(int64_t num, const py::bytes& data) {
    if (!(1 <= num && num <= num_pages)) throw py::AssertionError("Page number out of range");
    if (!(py::len(data) == 16)) throw py::AssertionError("Invalid spare size");
    file_seek(calc_spare_offset(num));
    return file_write(data);
}

// ---- image build / ECC batch operations (nandpro / xeBuild class) ----------

int64_t _stored_ecc(const py::bytes& spare) {
    return ((spare[12] >> 6) & 3) | (spare[13] << 2) | (spare[14] << 10) | (spare[15] << 18);
}

py::bytes strip_spare(const py::bytes& img) {
    std::string out;
    for (int64_t o = 0; o < py::len(img) - FULL + 1; o += FULL) {
        out += img.slice(o, o + PAGE).raw();
    }
    return py::bytes(out);
}

py::bytes add_spare(const py::bytes& raw, const std::optional<py::bytes>& meta) {
    py::bytearray out;
    int64_t npages = py::floordiv(py::len(raw), PAGE);
    for (int64_t i = 0; i < npages; ++i) {
        py::bytes page = raw.slice(i * PAGE, (i + 1) * PAGE);
        // `if meta`: a given, non-empty template
        py::bytearray spare = (meta && !meta->empty()) ? py::bytearray(meta->slice(i * SPARE, i * SPARE + SPARE))
                                                       : py::bytearray::zeros(SPARE);
        fix_page_ecc(page, spare);
        out += page + spare.view();
    }
    return py::bytes(out.view());
}

py::list<int64_t> verify_ecc(const py::bytes& img) {
    py::list<int64_t> bad;
    int64_t i = 0;
    for (int64_t o = 0; o < py::len(img) - FULL + 1; o += FULL, ++i) {
        py::bytes page = img.slice(o, o + PAGE);
        py::bytes spare = img.slice(o + PAGE, o + FULL);
        // only the low 26 bits of the computed value are stored in the spare
        if ((calc_page_ecc(page, spare) & 0x3FFFFFF) != _stored_ecc(spare)) {
            bad.append(i);
        }
    }
    return bad;
}

py::bytes fix_all_ecc(const py::bytes& img) {
    py::bytearray out(img);
    for (int64_t o = 0; o < out.size() - FULL + 1; o += FULL) {
        py::bytearray page = out.slice(o, o + PAGE);
        py::bytearray spare = out.slice(o + PAGE, o + FULL);
        fix_page_ecc(page, spare);
        out.set_slice(o + PAGE, o + FULL, spare.view());
    }
    return py::bytes(out.view());
}

const py::str DESCRIPTION(
    "Xbox 360 NAND image build: ecc / unecc / verify / fix ECC "
    "(528-byte page + spare, nandpro/xeBuild class)");

void main(const py::list<py::str>& a) {
    const py::list<py::str> ops{py::str("info"), py::str("verify"), py::str("unecc"), py::str("ecc"), py::str("fix")};
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help") || !ops.contains(a[0])) {
        py::print(py::str(
            "usage:\n"
            "  nandtool info   <img>              page/geometry summary\n"
            "  nandtool verify <img>              check every page's ECC\n"
            "  nandtool unecc  <img> <out>        strip spare -> raw 512-page data\n"
            "  nandtool ecc    <raw> <out>        add spare + ECC -> flash image\n"
            "  nandtool fix    <img> <out>        recompute all ECC in place"));
        return;
    }
    py::str op = a[0];
    py::bytes data = py::open_rb(a[1]).read();
    if (op == py::str("info")) {
        int64_t full = py::floordiv(py::len(data), FULL);
        py::print(py::percent(py::str("file size   : 0x%X (%d bytes)"), py::len(data), py::len(data)));
        if (py::mod(py::len(data), FULL) == 0) {
            py::print(py::percent(py::str("layout      : %d pages x 528 (512 data + 16 spare)"), full));
            py::print(py::percent(py::str("flash size  : 0x%X (%d MB)"), full * PAGE,
                                  py::floordiv(full * PAGE, 1024 * 1024)));
            py::list<int64_t> bad = verify_ecc(data);
            py::str shown = bad.empty() ? py::percent(py::str("all %d pages OK"), full)
                                        : py::percent(py::str("%d bad pages (e.g. %s)"), py::len(bad), bad.slice(0, 8));
            py::print(py::percent(py::str("ECC         : %s"), shown));
        } else if (py::mod(py::len(data), PAGE) == 0) {
            py::print(py::percent(py::str("layout      : %d raw pages x 512 (no spare) \xE2\x80\x94 use `ecc` to build"),
                                  py::floordiv(py::len(data), PAGE)));
        } else {
            py::print(py::str("layout      : not a whole number of 512/528 pages"));
        }
        return;
    }
    if (op == py::str("verify")) {
        py::list<int64_t> bad = verify_ecc(data);
        py::print(py::percent(py::str("pages       : %d"), py::floordiv(py::len(data), FULL)));
        py::str shown = bad.empty() ? py::str("all pages valid")
                                    : py::percent(py::str("%d bad: %s%s"), py::len(bad), bad.slice(0, 16),
                                                  py::len(bad) > 16 ? py::str(" ...") : py::str(""));
        py::print(py::percent(py::str("ECC result  : %s"), shown));
        throw py::SystemExit(static_cast<int64_t>(bad.empty() ? 0 : 1));
    }
    std::optional<py::str> out;
    if (py::len(a) > 2) out = a[2];
    if (!out || out->empty()) {
        py::print(py::percent(py::str("usage: nandtool %s <in> <out>"), op));
        throw py::SystemExit(static_cast<int64_t>(2));
    }
    if (op == py::str("unecc")) {
        py::open(*out, py::str("wb")).write(strip_spare(data));
        py::print(py::percent(py::str("stripped %d pages -> %s (%d bytes raw)"), py::floordiv(py::len(data), FULL), *out,
                              py::floordiv(py::len(data), FULL) * PAGE));
    } else if (op == py::str("ecc")) {
        py::bytes res = add_spare(data);
        py::open(*out, py::str("wb")).write(res);
        py::print(py::percent(py::str("built %d pages with ECC -> %s (%d bytes)"), py::floordiv(py::len(data), PAGE), *out,
                              py::len(res)));
    } else if (op == py::str("fix")) {
        py::bytes res = fix_all_ecc(data);
        py::open(*out, py::str("wb")).write(res);
        py::print(py::percent(py::str("recomputed ECC for %d pages -> %s"), py::floordiv(py::len(data), FULL), *out));
    }
}

}  // namespace codebreaker::formats::xbox::crypto::nand_tool
