// codebreaker/formats/xbox/crypto/xell_unpack.cpp -- C++ from codebreaker.formats.xbox.crypto.xell_unpack.
//
// (The Python imports isfile from os.path and never uses it; it is left out.)
//
// The Python calls gzip.decompress; the runtime has no gzip, so it is ported here with CPython's
// semantics (codebreaker has no zlib in the C++ runtime): the member loop of gzip.decompress over a raw
// inflate (RFC 1951) and a CRC-32, with the same three failures -- BadGzipFile, EOFError, zlib.error.

#include "codebreaker/formats/xbox/crypto/xell_unpack.hpp"

#include <array>

namespace codebreaker::formats::xbox::crypto::xell_unpack {

const py::str XELL_DIR("XeLL");

namespace {

// ------------------------------------------------------------------------------------ crc32
uint32_t crc32_(const std::string& data) {
    static const std::array<uint32_t, 256> table = [] {
        std::array<uint32_t, 256> t{};
        for (uint32_t n = 0; n < 256; ++n) {
            uint32_t c = n;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            t[n] = c;
        }
        return t;
    }();
    uint32_t c = 0xFFFFFFFFu;
    for (char ch : data) c = table[(c ^ static_cast<uint8_t>(ch)) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

// ------------------------------------------------------------------------------------ inflate
// A raw deflate decoder (the shape of zlib's contrib/puff). Input that ends before the stream does is
// `Truncated` (zlib's decompressobj then reports no eof); a stream that is wrong is a ZlibError.
struct Truncated {};

constexpr int MAXBITS = 15;
constexpr int MAXLCODES = 286;
constexpr int MAXDCODES = 30;
constexpr int FIXLCODES = 288;

struct Huffman {
    std::array<int16_t, MAXBITS + 1> count{};
    std::array<int16_t, FIXLCODES> symbol{};
};

class Inflater {
    const std::string& in_;
    size_t pos_ = 0;
    int bitbuf_ = 0;
    int bitcnt_ = 0;

    [[noreturn]] static void bad(const char* why) { throw ZlibError(std::string("Error -3 while decompressing data: ") + why); }

    int bits(int need) {
        long val = bitbuf_;
        while (bitcnt_ < need) {
            if (pos_ >= in_.size()) throw Truncated();
            val |= static_cast<long>(static_cast<uint8_t>(in_[pos_++])) << bitcnt_;
            bitcnt_ += 8;
        }
        bitbuf_ = static_cast<int>(val >> need);
        bitcnt_ -= need;
        return static_cast<int>(val & ((1L << need) - 1));
    }

    int decode(const Huffman& h) {
        int code = 0, first = 0, index = 0;
        for (int len = 1; len <= MAXBITS; ++len) {
            code |= bits(1);
            int count = h.count[len];
            if (code - count < first) return h.symbol[index + (code - first)];
            index += count;
            first += count;
            first <<= 1;
            code <<= 1;
        }
        bad("invalid code");
    }

    // -> the number of codes left over (negative: over-subscribed)
    static int construct(Huffman& h, const short* length, int n) {
        for (int len = 0; len <= MAXBITS; ++len) h.count[len] = 0;
        for (int symbol = 0; symbol < n; ++symbol) h.count[length[symbol]]++;
        if (h.count[0] == n) return 0;
        int left = 1;
        for (int len = 1; len <= MAXBITS; ++len) {
            left <<= 1;
            left -= h.count[len];
            if (left < 0) return left;
        }
        short offs[MAXBITS + 1];
        offs[1] = 0;
        for (int len = 1; len < MAXBITS; ++len) offs[len + 1] = static_cast<short>(offs[len] + h.count[len]);
        for (int symbol = 0; symbol < n; ++symbol) {
            if (length[symbol] != 0) h.symbol[offs[length[symbol]]++] = static_cast<int16_t>(symbol);
        }
        return left;
    }

    void codes(const Huffman& lencode, const Huffman& distcode) {
        static const short lens[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
        static const short lext[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
        static const short dists[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
        static const short dext[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
        for (;;) {
            int symbol = decode(lencode);
            if (symbol < 256) {
                out.push_back(static_cast<char>(symbol));
            } else if (symbol > 256) {
                symbol -= 257;
                if (symbol >= 29) bad("invalid literal/length code");
                int len = lens[symbol] + bits(lext[symbol]);
                symbol = decode(distcode);
                size_t dist = static_cast<size_t>(dists[symbol] + bits(dext[symbol]));
                if (dist > out.size()) bad("invalid distance too far back");
                for (int k = 0; k < len; ++k) out.push_back(out[out.size() - dist]);
            } else {
                return;
            }
        }
    }

    void stored() {
        bitbuf_ = 0;
        bitcnt_ = 0;
        if (pos_ + 4 > in_.size()) throw Truncated();
        unsigned len = static_cast<uint8_t>(in_[pos_]) | (static_cast<unsigned>(static_cast<uint8_t>(in_[pos_ + 1])) << 8);
        unsigned nlen = static_cast<uint8_t>(in_[pos_ + 2]) | (static_cast<unsigned>(static_cast<uint8_t>(in_[pos_ + 3])) << 8);
        pos_ += 4;
        if (len != (~nlen & 0xFFFFu)) bad("invalid stored block lengths");
        if (pos_ + len > in_.size()) throw Truncated();
        out.append(in_, pos_, len);
        pos_ += len;
    }

    void fixed() {
        static const std::pair<Huffman, Huffman> tables = [] {
            std::pair<Huffman, Huffman> t;
            short lengths[FIXLCODES];
            int symbol = 0;
            for (; symbol < 144; ++symbol) lengths[symbol] = 8;
            for (; symbol < 256; ++symbol) lengths[symbol] = 9;
            for (; symbol < 280; ++symbol) lengths[symbol] = 7;
            for (; symbol < FIXLCODES; ++symbol) lengths[symbol] = 8;
            construct(t.first, lengths, FIXLCODES);
            for (symbol = 0; symbol < MAXDCODES; ++symbol) lengths[symbol] = 5;
            construct(t.second, lengths, MAXDCODES);
            return t;
        }();
        codes(tables.first, tables.second);
    }

    void dynamic() {
        static const short order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        short lengths[MAXLCODES + MAXDCODES];
        int nlen = bits(5) + 257;
        int ndist = bits(5) + 1;
        int ncode = bits(4) + 4;
        if (nlen > MAXLCODES || ndist > MAXDCODES) bad("too many length or distance symbols");
        int index = 0;
        for (; index < ncode; ++index) lengths[order[index]] = static_cast<short>(bits(3));
        for (; index < 19; ++index) lengths[order[index]] = 0;
        Huffman lencode, distcode;
        if (construct(lencode, lengths, 19) != 0) bad("invalid code lengths set");
        index = 0;
        while (index < nlen + ndist) {
            int symbol = decode(lencode);
            if (symbol < 16) {
                lengths[index++] = static_cast<short>(symbol);
            } else {
                int len = 0;
                if (symbol == 16) {
                    if (index == 0) bad("invalid bit length repeat");
                    len = lengths[index - 1];
                    symbol = 3 + bits(2);
                } else if (symbol == 17) {
                    symbol = 3 + bits(3);
                } else {
                    symbol = 11 + bits(7);
                }
                if (index + symbol > nlen + ndist) bad("invalid bit length repeat");
                while (symbol--) lengths[index++] = static_cast<short>(len);
            }
        }
        if (lengths[256] == 0) bad("invalid code -- missing end-of-block");
        int err = construct(lencode, lengths, nlen);
        if (err != 0 && (err < 0 || nlen != lencode.count[0] + lencode.count[1])) bad("invalid literal/lengths set");
        err = construct(distcode, lengths + nlen, ndist);
        if (err != 0 && (err < 0 || ndist != distcode.count[0] + distcode.count[1])) bad("invalid distances set");
        codes(lencode, distcode);
    }

  public:
    std::string out;

    explicit Inflater(const std::string& in) : in_(in) {}

    // inflates one raw deflate stream from the start of the input; -> true when the final block was reached
    bool run() {
        try {
            int last;
            do {
                last = bits(1);
                int type = bits(2);
                if (type == 0) stored();
                else if (type == 1) fixed();
                else if (type == 2) dynamic();
                else bad("invalid block type");
            } while (!last);
            return true;
        } catch (const Truncated&) {
            return false;
        }
    }

    // the input after the stream (the bits left in its last byte are dropped)
    std::string unused() const { return in_.substr(pos_); }
};

// ------------------------------------------------------------------------------------ gzip
constexpr int FHCRC = 2, FEXTRA = 4, FNAME = 8, FCOMMENT = 16;

// gzip._read_gzip_header: -> false at the end of the data (no header), else `pos` is past the header
bool read_gzip_header(const std::string& data, size_t& pos) {
    auto read_exact = [&](size_t n) {
        if (data.size() - pos < n) throw py::EOFError("Compressed file ended before the end-of-stream marker was reached");
        size_t at = pos;
        pos += n;
        return at;
    };
    if (pos >= data.size()) return false;
    std::string magic = data.substr(pos, 2);
    pos += magic.size();
    if (magic != "\x1f\x8b") throw BadGzipFile("Not a gzipped file");
    size_t h = read_exact(8);
    int method = static_cast<uint8_t>(data[h]);
    int flag = static_cast<uint8_t>(data[h + 1]);
    if (method != 8) throw BadGzipFile("Unknown compression method");
    if (flag & FEXTRA) {
        size_t e = read_exact(2);
        size_t extra_len = static_cast<uint8_t>(data[e]) | (static_cast<size_t>(static_cast<uint8_t>(data[e + 1])) << 8);
        read_exact(extra_len);
    }
    if (flag & FNAME) {
        while (pos < data.size() && data[pos++] != '\0') {
        }
    }
    if (flag & FCOMMENT) {
        while (pos < data.size() && data[pos++] != '\0') {
        }
    }
    if (flag & FHCRC) read_exact(2);
    return true;
}

uint32_t le32(const std::string& s, size_t at) {
    return static_cast<uint8_t>(s[at]) | (static_cast<uint32_t>(static_cast<uint8_t>(s[at + 1])) << 8) |
           (static_cast<uint32_t>(static_cast<uint8_t>(s[at + 2])) << 16) | (static_cast<uint32_t>(static_cast<uint8_t>(s[at + 3])) << 24);
}

}  // namespace

// Decompress a gzip compressed string in one shot.
py::bytes decompress(const py::bytes& data_in) {
    std::string members;
    std::string data = data_in.raw();
    for (;;) {
        size_t pos = 0;
        if (!read_gzip_header(data, pos)) return py::bytes(members);
        // a raw deflate stream, from after the header
        std::string body = data.substr(pos);
        Inflater inflater(body);
        bool eof = inflater.run();
        std::string unused = inflater.unused();
        if (!eof || unused.size() < 8) {
            throw py::EOFError("Compressed file ended before the end-of-stream marker was reached");
        }
        if (le32(unused, 0) != crc32_(inflater.out)) throw BadGzipFile("CRC check failed");
        if (le32(unused, 4) != static_cast<uint32_t>(inflater.out.size() & 0xffffffffu)) {
            throw BadGzipFile("Incorrect length of data produced");
        }
        members += inflater.out;
        // do.unused_data[8:].lstrip(b"\x00")
        size_t start = 8;
        while (start < unused.size() && unused[start] == '\0') ++start;
        data = unused.substr(start);
    }
}

// utilities
std::variant<py::bytes, py::str> read_file(const py::str& filename, bool text) {
    py::file f = py::open(filename, py::str(text ? "r" : "rb"));
    if (text) {
        py::str data = f.read_text();
        f.close();
        return data;
    }
    py::bytes data = f.read();
    f.close();
    return data;
}

void write_file(const py::str& filename, const std::variant<py::str, py::bytes, py::bytearray>& data) {
    bool is_str = std::holds_alternative<py::str>(data);
    py::file f = py::open(filename, py::str(is_str ? "w" : "wb"));
    if (is_str) f.write(std::get<py::str>(data));
    else if (std::holds_alternative<py::bytes>(data)) f.write(std::get<py::bytes>(data));
    else f.write(std::get<py::bytearray>(data));
    f.close();
}

void main() {
    // xell-1f.bin
    py::bytes xell_1f = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-1f.bin"))));
    py::bytes xell_1f_stage_1 = xell_1f.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-1.bin")), xell_1f_stage_1);
    py::bytes xell_1f_stage_2 = xell_1f.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-2-compressed.elf")), xell_1f_stage_2);
    xell_1f_stage_2 = decompress(xell_1f_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-2-decompressed.elf")), xell_1f_stage_2);

    // xell-2f.bin
    py::bytes xell_2f = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-2f.bin"))));
    py::bytes xell_2f_stage_1 = xell_2f.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-1.bin")), xell_2f_stage_1);
    py::bytes xell_2f_stage_2 = xell_2f.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-2-compressed.elf")), xell_2f_stage_2);
    xell_2f_stage_2 = decompress(xell_2f_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-2-decompressed.elf")), xell_2f_stage_2);

    // xell-gggggg.bin
    py::bytes xell_gggggg = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg.bin"))));
    py::bytes xell_gggggg_stage_1 = xell_gggggg.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-1.bin")), xell_gggggg_stage_1);
    py::bytes xell_gggggg_stage_2 = xell_gggggg.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-2-compressed.elf")), xell_gggggg_stage_2);
    xell_gggggg_stage_2 = decompress(xell_gggggg_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-2-decompressed.elf")), xell_gggggg_stage_2);
}

}  // namespace codebreaker::formats::xbox::crypto::xell_unpack
