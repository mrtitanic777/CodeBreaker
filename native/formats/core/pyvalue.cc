// pyvalue.cc -- the pickle subset written and read (pyvalue.h).
#include "pyvalue.h"

#include <cstring>

#include "text.h"

namespace cbf {

// ---- writing ------------------------------------------------------------------------------
void Out::integer(int64_t v) {
    if (v >= 0 && v < 0x100) {
        op('K');
        buf_ += static_cast<char>(v);
    } else if (v >= 0 && v < 0x10000) {
        op('M');
        buf_ += static_cast<char>(v);
        buf_ += static_cast<char>(v >> 8);
    } else if (v >= INT32_MIN && v <= INT32_MAX) {
        op('J');
        u32le(static_cast<uint32_t>(v));
    } else {                                    // LONG1: the fewest two's-complement bytes
        char b[8];
        int n = 8;
        uint64_t u = static_cast<uint64_t>(v);
        for (int k = 0; k < 8; ++k) b[k] = static_cast<char>(u >> (8 * k));
        while (n > 1) {
            bool top = (b[n - 1] & 0x80) != 0, next = (b[n - 2] & 0x80) != 0;
            if ((b[n - 1] == 0 && !next) || (b[n - 1] == static_cast<char>(0xFF) && next && top)) --n;
            else break;
        }
        op('\x8a');
        buf_ += static_cast<char>(n);
        raw(b, static_cast<size_t>(n));
    }
}

void Out::uinteger(uint64_t v) {
    if (v <= static_cast<uint64_t>(INT64_MAX)) {
        integer(static_cast<int64_t>(v));
        return;
    }
    char b[9];
    for (int k = 0; k < 8; ++k) b[k] = static_cast<char>(v >> (8 * k));
    b[8] = 0;
    op('\x8a');
    buf_ += static_cast<char>(9);
    raw(b, 9);
}

void Out::uinteger65(uint64_t low) {
    char b[9];
    for (int k = 0; k < 8; ++k) b[k] = static_cast<char>(low >> (8 * k));
    b[8] = 1;
    op('\x8a');
    buf_ += static_cast<char>(9);
    raw(b, 9);
}

void Out::text(const std::string &utf8) {
    if (utf8.size() < 0x100) {
        op('\x8c');
        buf_ += static_cast<char>(utf8.size());
    } else {
        op('X');
        u32le(static_cast<uint32_t>(utf8.size()));
    }
    buf_ += utf8;
}

void Out::latin1(Bytes b) { text(cbf::latin1(b)); }

void Out::bytes(Bytes b) {
    if (b.size() < 0x100) {
        op('C');
        buf_ += static_cast<char>(b.size());
    } else {
        op('B');
        u32le(static_cast<uint32_t>(b.size()));
    }
    raw(b.data(), static_cast<size_t>(b.size()));
}

// ---- reading ------------------------------------------------------------------------------
int64_t Value::as_int() const {
    if (kind == Int || kind == Bool) return i;
    if (kind == None) raise("TypeError", "an integer is required, not None");
    raise("TypeError", "an integer is required");
}

bool Value::truthy() const {
    switch (kind) {
    case None: return false;
    case Bool:
    case Int: return i != 0;
    case Str:
    case Bytes: return !s.empty();
    default: return !items.empty();
    }
}

const std::string &Value::as_str() const {
    if (kind != Str && kind != Bytes) raise("TypeError", "a str is required");
    return s;
}

cbf::Bytes Value::as_bytes() const {
    if (kind != Bytes && kind != Str) raise("TypeError", "a bytes-like object is required");
    return cbf::Bytes(reinterpret_cast<const uint8_t *>(s.data()), s.size());
}

const Value *Value::get(const char *key) const {
    if (kind != Dict) return nullptr;
    for (size_t k = 0; k + 1 < items.size(); k += 2)
        if (items[k].kind == Str && items[k].s == key) return &items[k + 1];
    return nullptr;
}

namespace {
struct Reader {
    const uint8_t *p, *e;
    std::vector<Value> stack;
    std::vector<size_t> marks;
    std::vector<Value> memo;

    uint64_t take(int n) {
        if (e - p < n) raise("TypeError", "a cut-off argument stream");
        uint64_t v = 0;
        for (int k = n - 1; k >= 0; --k) v = (v << 8) | p[k];
        p += n;
        return v;
    }
    std::string take_str(uint64_t n) {
        if (static_cast<uint64_t>(e - p) < n) raise("TypeError", "a cut-off argument stream");
        std::string s(reinterpret_cast<const char *>(p), static_cast<size_t>(n));
        p += n;
        return s;
    }
    Value &top() {
        if (stack.empty()) raise("TypeError", "a malformed argument stream");
        return stack.back();
    }
    std::vector<Value> pop_mark() {
        if (marks.empty()) raise("TypeError", "a malformed argument stream");
        size_t m = marks.back();
        marks.pop_back();
        std::vector<Value> out(std::make_move_iterator(stack.begin() + m), std::make_move_iterator(stack.end()));
        stack.resize(m);
        return out;
    }
    void push(Value v) { stack.push_back(std::move(v)); }
    static Value make(Value::Kind k) {
        Value v;
        v.kind = k;
        return v;
    }
    static Value make_int(int64_t i, Value::Kind k = Value::Int) {
        Value v;
        v.kind = k;
        v.i = i;
        return v;
    }

    Value run() {
        while (p < e) {
            uint8_t c = *p++;
            switch (c) {
            case 0x80: take(1); break;                                       // PROTO
            case 0x95: take(8); break;                                       // FRAME
            case '.': {                                                      // STOP
                if (stack.size() != 1) raise("TypeError", "a malformed argument stream");
                return std::move(stack.back());
            }
            case 'N': push(make(Value::None)); break;
            case 0x88: push(make_int(1, Value::Bool)); break;
            case 0x89: push(make_int(0, Value::Bool)); break;
            case 'K': push(make_int(static_cast<int64_t>(take(1)))); break;
            case 'M': push(make_int(static_cast<int64_t>(take(2)))); break;
            case 'J': push(make_int(static_cast<int32_t>(take(4)))); break;
            case 0x8a: {                                                     // LONG1
                int n = static_cast<int>(take(1));
                if (n > 8) raise("TypeError", "an int past 64 bits");
                uint64_t u = n ? take(n) : 0;
                if (n && n < 8 && (u >> (8 * n - 1)) & 1) u |= ~0ULL << (8 * n);
                push(make_int(static_cast<int64_t>(u)));
                break;
            }
            case 0x8c: { Value v = make(Value::Str); v.s = take_str(take(1)); push(std::move(v)); break; }
            case 'X': { Value v = make(Value::Str); v.s = take_str(take(4)); push(std::move(v)); break; }
            case 0x8d: { Value v = make(Value::Str); v.s = take_str(take(8)); push(std::move(v)); break; }
            case 'C': { Value v = make(Value::Bytes); v.s = take_str(take(1)); push(std::move(v)); break; }
            case 'B': { Value v = make(Value::Bytes); v.s = take_str(take(4)); push(std::move(v)); break; }
            case 0x8e: { Value v = make(Value::Bytes); v.s = take_str(take(8)); push(std::move(v)); break; }
            case ')': push(make(Value::Tuple)); break;
            case 0x85:
            case 0x86:
            case 0x87: {                                                     // TUPLE1..3
                size_t n = c - 0x84;
                if (stack.size() < n) raise("TypeError", "a malformed argument stream");
                Value v = make(Value::Tuple);
                v.items.assign(std::make_move_iterator(stack.end() - n), std::make_move_iterator(stack.end()));
                stack.resize(stack.size() - n);
                push(std::move(v));
                break;
            }
            case '(': marks.push_back(stack.size()); break;
            case 't': { Value v = make(Value::Tuple); v.items = pop_mark(); push(std::move(v)); break; }
            case ']': push(make(Value::List)); break;
            case '}': push(make(Value::Dict)); break;
            case 0x8f: push(make(Value::Set)); break;
            case 'a': {                                                      // APPEND
                Value x = std::move(top());
                stack.pop_back();
                top().items.push_back(std::move(x));
                break;
            }
            case 'e':                                                        // APPENDS
            case 'u':                                                        // SETITEMS
            case 0x90: {                                                     // ADDITEMS
                std::vector<Value> xs = pop_mark();
                for (auto &x : xs) top().items.push_back(std::move(x));
                break;
            }
            case 's': {                                                      // SETITEM
                if (stack.size() < 3) raise("TypeError", "a malformed argument stream");
                Value v = std::move(stack.back());
                stack.pop_back();
                Value k = std::move(stack.back());
                stack.pop_back();
                top().items.push_back(std::move(k));
                top().items.push_back(std::move(v));
                break;
            }
            case 0x91: { Value v = make(Value::Set); v.items = pop_mark(); push(std::move(v)); break; }   // FROZENSET
            case 0x94: memo.push_back(top()); break;                         // MEMOIZE
            case 'q': { size_t k = static_cast<size_t>(take(1)); if (memo.size() <= k) memo.resize(k + 1); memo[k] = top(); break; }
            case 'r': { size_t k = static_cast<size_t>(take(4)); if (memo.size() <= k) memo.resize(k + 1); memo[k] = top(); break; }
            case 'h': { size_t k = static_cast<size_t>(take(1)); if (k >= memo.size()) raise("TypeError", "a malformed argument stream"); push(memo[k]); break; }
            case 'j': { size_t k = static_cast<size_t>(take(4)); if (k >= memo.size()) raise("TypeError", "a malformed argument stream"); push(memo[k]); break; }
            default: raise("TypeError", "an argument this library does not read (pickle opcode " + std::to_string(c) + ")");
            }
        }
        raise("TypeError", "a cut-off argument stream");
    }
};
}  // namespace

Value read_pickle(const uint8_t *p, size_t n) {
    if (!p || !n) return Reader::make(Value::Tuple);
    Reader r{p, p + n, {}, {}, {}};
    return r.run();
}

}  // namespace cbf
