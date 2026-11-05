// pyvalue.h -- the values that cross the C interface: answers out, arguments in.
//
// Both travel as Python pickle streams, protocol 4, restricted to plain values (None, bool,
// int, str, bytes, tuple, list, dict, set).  `Out` writes an answer as it is built -- no tree
// in between -- and `Value` is an argument read back.  Python's pickle module reads and writes
// these at C speed, and nothing here depends on Python itself.
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bytes.h"

namespace cbf {

class Out {
public:
    Out() { buf_.assign("\x80\x04", 2); }   // PROTO 4

    void none() { op('N'); }
    void boolean(bool v) { op(v ? '\x88' : '\x89'); }
    void integer(int64_t v);
    void uinteger(uint64_t v);
    void uinteger65(uint64_t low);            // 2**64 + low (a sum that carried past 64 bits)
    void text(const std::string &utf8);      // a str, given as UTF-8
    void text(const char *utf8) { text(std::string(utf8)); }
    void latin1(Bytes b);                     // a str decoded from latin1
    void bytes(Bytes b);

    // containers: begin, the items (a dict: key, value, key, value ...), end
    void begin_list() { op(']'); op('('); }
    void end_list() { op('e'); }
    void begin_tuple() { op('('); }
    void end_tuple() { op('t'); }
    void begin_dict() { op('}'); op('('); }
    void end_dict() { op('u'); }
    void begin_set() { op('\x8f'); op('('); }
    void end_set() { op('\x90'); }

    // one object met again: memo() right after writing it keeps it (-> its number), again(n)
    // writes that same object once more (one Python object, not a copy)
    size_t memo() {
        op('\x94');
        return memos_++;
    }
    void again(size_t n) {
        if (n < 0x100) {
            op('h');
            buf_ += static_cast<char>(n);
        } else {
            op('j');
            u32le(static_cast<uint32_t>(n));
        }
    }

    // shorthands
    void key(const char *k) { text(k); }
    void item(const char *k, int64_t v) { key(k); integer(v); }
    void item_text(const char *k, const std::string &utf8) { key(k); text(utf8); }

    const std::string &finish() {
        op('.');
        return buf_;
    }
    void reset() {
        buf_.assign("\x80\x04", 2);
        memos_ = 0;
    }

private:
    size_t memos_ = 0;
    void op(char c) { buf_ += c; }
    void raw(const void *p, size_t n) { buf_.append(static_cast<const char *>(p), n); }
    void u32le(uint32_t v) {
        char b[4] = {char(v), char(v >> 8), char(v >> 16), char(v >> 24)};
        raw(b, 4);
    }
    std::string buf_;
};

// An argument: one plain Python value.
struct Value {
    enum Kind { None, Bool, Int, Str, Bytes, Tuple, List, Dict, Set } kind = None;
    int64_t i = 0;                 // Bool, Int
    std::string s;                 // Str (UTF-8), Bytes
    std::vector<Value> items;      // Tuple, List, Set; a Dict as key, value, key, value ...

    bool is_none() const { return kind == None; }
    int64_t as_int() const;
    bool truthy() const;
    const std::string &as_str() const;
    cbf::Bytes as_bytes() const;
    const Value *get(const char *key) const;          // a Dict's value for a str key, or nullptr
    const Value &operator[](size_t k) const { return items.at(k); }
    size_t size() const { return kind == Dict ? items.size() / 2 : items.size(); }
};

// Read a pickle stream (the subset above) -> the value.  A stream outside the subset is a
// Fallback: the Python reader answers.
Value read_pickle(const uint8_t *p, size_t n);

}  // namespace cbf
