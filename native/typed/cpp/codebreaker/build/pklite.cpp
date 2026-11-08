// codebreaker/build/pklite.cpp -- C++17 port of codebreaker.build.pklite (see pklite.hpp).

#include "codebreaker/build/pklite.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace codebreaker::build::pklite {

const int64_t STUB_CODE = 0x44;
const int64_t MAX_DISTANCE = 0x1FFF;
const int64_t MAX_LENGTH = 25 + 0xFC;
const int64_t _FIRST_MARK = 0x8000;
const int64_t _MARK_EVERY = 0xA000;

namespace {

// struct.unpack_from("<%dH" % count, data, offset): `count` little-endian words (a negative offset
// counts from the end, as struct's does)
py::list<int64_t> unpack_words(const py::bytes& data, int64_t offset, int64_t count) {
    if (count < 0) throw py::StructError("bad char in struct format");
    int64_t n = data.size();
    if (offset < 0) {
        if (offset + n < 0) throw py::StructError("offset out of range");
        offset += n;
    }
    if (n - offset < 2 * count) throw py::StructError("unpack_from requires a larger buffer");
    py::list<int64_t> words;
    for (int64_t i = 0; i < count; ++i) words.append(data.byte_(offset + 2 * i) | data.byte_(offset + 2 * i + 1) << 8);
    return words;
}

}  // namespace

// ------------------------------------------------------------------------------------------ Bits_

Bits_::Bits_(const py::bytes& data, int64_t at) : d(data), si(at) {
    bp = word();
    dx = 16;
}

int64_t Bits_::word() {
    if (si + 2 > d.size()) throw Out_();
    int64_t w = py::unpack_from1<int64_t>("<H", d, si);
    si += 2;
    return w;
}

int64_t Bits_::byte() {
    if (si >= d.size()) throw Out_();
    si += 1;
    return d[si - 1];
}

int64_t Bits_::bit() {
    int64_t cf = bp & 1;
    bp >>= 1;
    dx -= 1;
    if (dx == 0) {
        bp = word();
        dx = 16;
    }
    return cf;
}

// ------------------------------------------------------------------------------------------ the codes

int64_t _length(const Bit& bit, const py::bytes& t) {
    // (Python evaluates  bit() << 1 | bit()  left to right: the bits are read one after the other)
    int64_t b0 = bit();
    int64_t b1 = bit();
    int64_t bx = b0 << 1 | b1;
    if (bx >= 2) return bx;
    bx = bx << 1 | bit();
    if (bx == 0) return t[0x209];
    bx = bx << 1 | bit();
    if (bx < 5) return t[0x209 + bx];
    bx = bx << 1 | bit();
    if (bx <= 0xC) return t[0x209 + bx];
    bx = (bx & 3) << 1 | bit();
    if (bx >= 5) {
        bx = bx << 1 | bit();
        if (bx > 0xC) {
            bx = (bx & 3) << 1 | bit();
            if (bx >= 5) bx = bx << 1 | bit();
            return t[0x21F + bx];
        }
    }
    return t[0x214 + bx];
}

int64_t _high(const Bit& bit, const py::bytes& t) {
    int64_t b0 = bit();
    int64_t b1 = bit();
    int64_t b2 = bit();
    int64_t bx = b0 << 2 | b1 << 1 | b2;
    if (bx < 2) return t[0x22F + bx];
    bx = bx << 1 | bit();
    if (bx < 8) return t[0x22F + bx];
    bx = bx << 1 | bit();
    if (bx < 0x17) return t[0x22F + bx];
    bx = bx << 1 | bit();
    return bx & 0xDF;
}

Codes _codes(const CodeReader& read, const py::bytes& t) {
    Codes out;
    std::function<void(const py::list<int64_t>&)> walk = [&](const py::list<int64_t>& prefix) {
        int64_t at = 0;
        Bit bit = [&]() -> int64_t {
            if (at >= prefix.size()) throw Out_();   // next(it) on an exhausted iterator
            return prefix[at++];
        };
        int64_t v;
        try {
            v = read(bit, t);
        } catch (const Out_&) {
            if (prefix.size() < 12) {
                py::list<int64_t> zero = py::list<int64_t>::from(prefix.vec());
                zero.append(0);
                walk(zero);
                py::list<int64_t> one = py::list<int64_t>::from(prefix.vec());
                one.append(1);
                walk(one);
            }
            return;
        }
        out.setdefault(v, py::list<int64_t>::from(prefix.vec()));
    };
    walk(py::list<int64_t>());
    return out;
}

Stub::Stub(const py::bytes& module, int64_t start)
    : head(module.slice(0, start)), code(module.slice(STUB_CODE, module.size())) {
    lengths = _codes(_length, code);
    highs = _codes(_high, code);
}

// ------------------------------------------------------------------------------------------ reading

std::tuple<py::bytes, py::list<Token>, int64_t> _decode(const Stub& stub, const py::bytes& data, int64_t at) {
    Bits_ r(data, at);
    const py::bytes& t = stub.code;
    py::bytearray out;
    py::list<Token> tokens;
    Bit bit = [&]() { return r.bit(); };
    for (;;) {
        if (!r.bit()) {
            out.append(r.byte());
            tokens.append(Token::lit(out[-1]));
            continue;
        }
        int64_t n = _length(bit, t);
        if (n == 25) {
            int64_t b = r.byte();
            if (b == 0xFE) {
                tokens.append(Token::seg());
                continue;
            }
            if (b == 0xFF) {
                tokens.append(Token::end());
                return {py::bytes(out.raw()), tokens, r.si};
            }
            n += b;
        }
        int64_t hi = (n == 2 || r.bit()) ? 0 : _high(bit, t);
        int64_t d = hi << 8 | r.byte();
        for (int64_t i = 0; i < n; ++i) {
            int64_t v = out[out.size() - d];
            out.append(v);
        }
        tokens.append(Token::copy(n, d));
    }
}

int64_t stream_start(const py::bytes& module) {
    int64_t at = module.find(py::bytes("\x8c\xdb\x53\x81\xc3"));
    if (at < 0) throw py::ValueError("not PKLITE's decompressor (no start of the stream found)");
    return py::unpack_from1<int64_t>("<H", module, at + 5) * 16 - 0x100;
}

Stored read(const py::bytes& module) {
    int64_t start = stream_start(module);
    Stub stub(module, start);
    auto [image, tokens, end] = _decode(stub, module, start);
    py::list<std::tuple<int64_t, py::list<int64_t>>> relocs;
    int64_t at = end;
    for (;;) {
        int64_t n = module[at];
        at += 1;
        if (!n) break;
        int64_t seg = py::unpack_from1<int64_t>("<H", module, at);
        py::list<int64_t> offs = unpack_words(module, at + 2, n);
        relocs.append({seg, py::list<int64_t>::from(offs.vec())});
        at += 2 + 2 * n;
    }
    auto [ss, sp, cs, ip] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<4H", module, at);
    return Stored{stub, start, end, image, tokens, relocs, {cs, ip, ss, sp}, module.slice(at + 8, module.size())};
}

// ------------------------------------------------------------------------------------------ writing

Writer_::Writer_() : out(py::bytearray::zeros(2)) {}

void Writer_::bit(int64_t b) {
    word |= b << n;
    n += 1;
    if (n == 16) {
        py::pack_into("<H", out, slot, word);
        slot = out.size();
        n = 0;
        word = 0;
        out += py::bytes(std::string(2, '\0'));
    }
}

void Writer_::bits(const py::list<int64_t>& code) {
    for (int64_t i = 0; i < code.size(); ++i) bit(code[i]);
}

void Writer_::byte(int64_t b) { out.append(b); }

py::bytes Writer_::done() {
    py::pack_into("<H", out, slot, word);
    return py::bytes(out.raw());
}

py::bytes encode(const py::list<Token>& tokens, const Stub& stub) {
    Writer_ w;
    encode_into(w, tokens, stub);
    return w.done();
}

void encode_into(Writer_& w, const py::list<Token>& tokens, const Stub& stub) {
    for (int64_t i = 0; i < tokens.size(); ++i) {
        const Token t = tokens[i];
        if (t.kind == Token::Kind::Lit) {
            w.bit(0);
            w.byte(t.a);
            continue;
        }
        w.bit(1);
        if (t.kind == Token::Kind::Seg || t.kind == Token::Kind::End) {
            w.bits(stub.lengths.at(25));
            w.byte(t.kind == Token::Kind::Seg ? 0xFE : 0xFF);
            continue;
        }
        int64_t n = t.a, d = t.b;
        if (n >= 25) {
            w.bits(stub.lengths.at(25));
            w.byte(n - 25);
        } else {
            w.bits(stub.lengths.at(n));
        }
        if (n != 2) {
            if (d < 0x100) {
                w.bit(1);
            } else {
                w.bit(0);
                w.bits(stub.highs.at(d >> 8));
            }
        }
        w.byte(d & 0xFF);
    }
}

// ------------------------------------------------------------------------------------------ parsing

Longest _matcher(const py::bytes& data_bytes) {
    const std::string data = data_bytes.raw();
    const int64_t n = static_cast<int64_t>(data.size());
    // head[p]: the last q < p with p's 3 bytes (chain[p] is the same: the next candidate behind q)
    auto head = std::make_shared<std::vector<std::optional<int64_t>>>(static_cast<size_t>(n));
    auto chain = std::make_shared<std::vector<std::optional<int64_t>>>(static_cast<size_t>(n));
    py::dict<py::bytes, int64_t> latest;
    for (int64_t p = 0; p < n - 2; ++p) {
        py::bytes k = data_bytes.slice(p, p + 3);
        std::optional<int64_t> prev;
        if (latest.contains(k)) prev = latest.at(k);
        (*head)[static_cast<size_t>(p)] = (*chain)[static_cast<size_t>(p)] = prev;
        latest.set(k, p);
    }
    auto at = [data](int64_t i) { return static_cast<unsigned char>(data[static_cast<size_t>(i)]); };

    return [=](int64_t pos) -> std::tuple<int64_t, int64_t> {
        int64_t best = 0, bd = 0;
        int64_t lim = std::min(MAX_LENGTH, n - pos);
        std::optional<int64_t> q = lim >= 3 ? (*head)[static_cast<size_t>(pos)] : std::nullopt;
        while (q) {
            if (pos - *q > MAX_DISTANCE) q = pos - MAX_DISTANCE;
            int64_t l = 0;
            while (l < lim && at(*q + l) == at(pos + l)) ++l;
            if (l > best || l == lim) {
                best = l;
                bd = pos - *q;
            }
            if (pos - *q >= MAX_DISTANCE) break;
            q = (*chain)[static_cast<size_t>(*q)];
        }
        if (best >= 3) return {best, bd};
        if (lim >= 2) {
            for (int64_t d = 1; d <= std::min(pos, static_cast<int64_t>(255)); ++d) {
                if (at(pos - d) == at(pos) && at(pos - d + 1) == at(pos + 1)) return {2, d};
            }
        }
        return {0, 0};
    };
}

bool _put_off(const std::tuple<int64_t, int64_t>& cur, const std::tuple<int64_t, int64_t>& nxt) {
    return std::get<0>(nxt) >= std::get<0>(cur) + 2 || (std::get<0>(nxt) == std::get<0>(cur) + 1 && std::get<1>(cur) > 128);
}

py::list<Token> parse(const py::bytes& image) {
    const py::bytes data = image;
    Longest longest = _matcher(data);
    py::list<Token> tokens;
    int64_t pos = 0, mark = _FIRST_MARK;
    std::tuple<int64_t, int64_t> cur = longest(0);
    while (pos < data.size()) {
        if (pos >= mark - MAX_LENGTH) {
            tokens.append(Token::seg());
            mark += _MARK_EVERY;
        }
        auto [n, d] = cur;
        std::tuple<int64_t, int64_t> nxt = pos + 1 < data.size() ? longest(pos + 1) : std::make_tuple<int64_t, int64_t>(0, 0);
        if (n >= 2 && !_put_off(cur, nxt)) {
            tokens.append(Token::copy(n, d));
            pos += n;
            cur = pos < data.size() ? longest(pos) : std::make_tuple<int64_t, int64_t>(0, 0);
        } else {
            tokens.append(Token::lit(data[pos]));
            pos += 1;
            cur = nxt;
        }
    }
    tokens.append(Token::end());
    return tokens;
}

py::bytes _relocation_table(const py::list<std::tuple<int64_t, int64_t>>& relocs) {
    py::bytearray out;
    int64_t i = 0;
    while (i < relocs.size()) {
        int64_t seg = std::get<0>(relocs[i]);
        int64_t j = i;
        while (j < relocs.size() && std::get<0>(relocs[j]) == seg && j - i < 255) ++j;
        out += py::pack("<BH", j - i, seg);
        for (int64_t k = i; k < j; ++k) out += py::pack("<H", std::get<1>(relocs[k]));
        i = j;
    }
    return py::bytes(out.raw() + std::string(1, '\0'));
}

std::tuple<py::bytes, Header> pack(const py::bytes& image, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                                   const std::tuple<int64_t, int64_t, int64_t, int64_t>& regs, const py::bytes& head,
                                   int64_t memory) {
    py::list<Token> tokens = parse(image);
    Stub stub(head, head.size());
    auto [cs, ip, ss, sp] = regs;
    py::bytes body = encode(tokens, stub) + _relocation_table(relocs) + py::pack("<4H", ss, sp, cs, ip);
    py::bytearray module(head);
    module += body;
    int64_t paras = (module.size() + 15) / 16;
    // the memory it needs, relative to where it is loaded: the program and what it asks for
    // beyond itself, less the program prefix -- and never so little that the expanding program
    // would overrun the stream before it is read (it is moved to just below the decompressor)
    int64_t need = (image.size() + 15) / 16 + memory - 0x10;
    int64_t out = 0, reach = 0;
    Writer_ w;
    for (int64_t i = 0; i < tokens.size(); ++i) {                      // how far the output runs ahead of the input
        const Token t = tokens[i];
        reach = std::max(reach, out - w.out.size());
        out += t.kind == Token::Kind::Lit ? 1 : t.kind == Token::Kind::Copy ? t.a : 0;
        encode_into(w, py::list<Token>({t}), stub);
    }
    int64_t safe = paras + (std::max(reach, static_cast<int64_t>(0)) + 15) / 16 + 0x25;
    int64_t top = std::max(need, safe);
    py::pack_into("<H", module, 1, top);               // mov ax, top
    py::pack_into("<H", module, 4, paras);             // mov dx, paras
    Header header;
    header.cs = 0xFFF0;
    header.ip = 0x100;
    header.ss = module.size() / 16 + 9;
    header.sp = 0x200;
    header.min_alloc = top + 0x10 - module.size() / 16;
    header.relocs = py::list<std::tuple<int64_t, int64_t>>({std::make_tuple<int64_t, int64_t>(7, 0)});
    return {py::bytes(module.raw()), header};
}

}  // namespace codebreaker::build::pklite
