#include "cb/decompile/frame16.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>
#include <unordered_map>
#include <utility>

#include <nlohmann/json.hpp>

#include "cb/core/capstone_api.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/l2tree.hpp"

namespace cb::decompile::frame16 {

namespace {

using inline_asm::Instruction;
using inline_asm::OperandKind;
using l2tree::is_kind;
using l2tree::member;
using l2tree::text;

// What the instructions of the function say about its frame.
struct Frame {
    std::int64_t size = 0;                                                       // the bytes the prologue takes (0: none found)
    std::map<std::int64_t, int> places;                                          // bp offset -> widest access
    std::set<std::int64_t> taken;                                                // bp offsets whose address is taken (lea)
    std::unordered_map<std::uint32_t, std::pair<std::int64_t, int>> writes;      // instruction address -> (bp offset, width) it writes
};

// the frame of the function in code[start:end]: ENTER n, or SUB SP,n as the third instruction (after PUSH BP / MOV BP,SP)
Frame read_frame(ByteView code, std::uint32_t start, std::uint32_t end, const inline_asm::Decoder& decoder) {
    const std::size_t from = std::min<std::size_t>(start, code.size());
    const std::size_t to = std::clamp<std::size_t>(end, from, code.size());
    const std::vector<Instruction> ins = (decoder ? decoder : inline_asm::Decoder(inline_asm::decode16))(code.subspan(from, to - from), start);
    Frame f;
    for (std::size_t k = 0; k < ins.size(); ++k) {
        const Instruction& i = ins[k];
        if (k == 0 && i.mnemonic == "enter" && !i.operands.empty()) {
            f.size = i.operands[0].imm;
        } else if (k == 2 && i.mnemonic == "sub" && i.op_str.starts_with("sp, ") && i.operands.size() > 1 &&
                   i.operands[1].kind == OperandKind::Imm) {
            f.size = i.operands[1].imm;
        }
        for (std::size_t n = 0; n < i.operands.size(); ++n) {
            const inline_asm::Operand& op = i.operands[n];
            if (op.kind != OperandKind::Mem || op.mem.base != static_cast<std::uint16_t>(X86_REG_BP) || op.mem.index != 0) continue;
            const std::int64_t off = op.mem.disp;
            if (off >= 0) continue;   // (a parameter's: the caller's)
            if (i.mnemonic == "lea") {
                f.taken.insert(off);
                continue;
            }
            const int width = op.size ? op.size : 2;
            int& widest = f.places[off];
            widest = std::max(widest, width);
            if (n == 0 && (op.access & inline_asm::kAccessWrite)) f.writes[i.address] = {off, width};
        }
        if (i.mnemonic == "ret" || i.mnemonic == "retf") break;
    }
    return f;
}

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

std::string_view symbol_name(const Json* node) { return text(member(node, "sym"), "name"); }

Json kind_only(std::string_view kind) {
    Json t = Json::object();
    t["kind"] = std::string(kind);
    return t;
}

// the type of a place of this many bytes (a bool kept in a word is an int2)
Json signed_width(int bytes) { return kind_only(bytes == 1 ? "u8" : bytes == 2 ? "i16" : "i32"); }
bool has_signed_width(int bytes) { return bytes == 1 || bytes == 2 || bytes == 4; }
// the type of a piece of a buffer this many bytes wide (3: as the back end writes x._1_3_)
std::optional<Json> unsigned_width(std::int64_t bytes) {
    switch (bytes) {
        case 1: return kind_only("u8");
        case 2:
        case 3: return kind_only("u16");
        case 4: return kind_only("u32");
        default: return std::nullopt;
    }
}

// the trailing digits of a class name: xunknown4 is 4 bytes, int12 is 12
std::int64_t class_size(std::string_view name) {
    std::size_t d = name.size();
    while (d > 0 && name[d - 1] >= '0' && name[d - 1] <= '9') --d;
    if (d == name.size() || !std::all_of(name.begin(), name.end(), is_word_char)) return 0;
    return text::parse_int<std::int64_t>(name.substr(d), 10).value_or(0);
}

// A type's size in bytes (an array's elements times their size; a far pointer 4; xunknownN N), or 0 where it is not known.
std::int64_t type_bytes(const Json* t) {
    const std::string_view kind = text(t, "kind");
    if (kind == "array") return type_bytes(member(t, "elem")) * l2tree::integer(t, "count").value_or(0);
    if (kind == "ptr") return text(t, "space") == "far" ? 4 : 2;
    if (kind == "class") return class_size(text(t, "name"));
    if (kind == "u8" || kind == "i8" || kind == "char" || kind == "bool") return 1;
    if (kind == "u16" || kind == "i16") return 2;
    if (kind == "u32" || kind == "i32") return 4;
    return 0;
}

bool is_far_pointer(const Json* t) { return text(t, "kind") == "ptr" && text(t, "space") == "far"; }

// x._2_2_ : 2 bytes of x from byte 2 (the decompiler's piece of a variable) -> (2, 2)
std::optional<std::pair<std::int64_t, std::int64_t>> piece_name(std::string_view name) {
    if (name.size() < 5 || name.front() != '_' || name.back() != '_') return std::nullopt;
    const auto parts = text::split(name.substr(1, name.size() - 2), '_');
    if (parts.size() != 2) return std::nullopt;
    const auto digits = [](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; }); };
    if (!digits(parts[0]) || !digits(parts[1])) return std::nullopt;
    const auto first = text::parse_int<std::int64_t>(parts[0], 10);
    const auto second = text::parse_int<std::int64_t>(parts[1], 10);
    if (!first || !second) return std::nullopt;
    return std::make_pair(*first, *second);
}

// `line` with each whole-word occurrence of `name` replaced
std::string replace_word(std::string_view line, std::string_view name, std::string_view with) {
    std::string out;
    std::size_t i = 0;
    while (i < line.size()) {
        const std::size_t at = line.find(name, i);
        if (at == std::string_view::npos) break;
        const bool before = at == 0 || !is_word_char(line[at - 1]);
        const bool after = at + name.size() >= line.size() || !is_word_char(line[at + name.size()]);
        out += line.substr(i, at - i);
        if (before && after) {
            out += with;
            i = at + name.size();
        } else {
            out += line.substr(at, 1);
            i = at + 1;
        }
    }
    out += line.substr(i);
    return out;
}

// the buffer's bytes at `off`, as a value of type `width`: *(width __ss *)((u8[n])local + off)
Json piece_of_buffer(const std::string& buffer_name, std::int64_t buffer_size, std::int64_t off, const Json& width) {
    Json arr = Json::object();
    arr["node"] = "Name";
    Json sym = Json::object();
    sym["kind"] = "local";
    sym["name"] = buffer_name;
    arr["sym"] = std::move(sym);
    Json arr_type = Json::object();
    arr_type["kind"] = "array";
    arr_type["elem"] = kind_only("u8");
    arr_type["count"] = buffer_size;
    arr["type"] = std::move(arr_type);

    Json addr = arr;
    if (off) {
        addr = Json::object();
        addr["node"] = "Binary";
        addr["op"] = "+";
        addr["left"] = std::move(arr);
        Json lit = Json::object();
        lit["node"] = "Lit";
        lit["value"] = off;
        lit["text"] = std::to_string(off);
        addr["right"] = std::move(lit);
    }
    Json cast = Json::object();
    cast["node"] = "Cast";
    Json ptr = Json::object();
    ptr["kind"] = "ptr";
    ptr["space"] = "near";
    ptr["seg"] = "ss";
    ptr["to"] = width;
    cast["type"] = std::move(ptr);
    cast["x"] = std::move(addr);

    Json deref = Json::object();
    deref["node"] = "Unary";
    deref["op"] = "*";
    deref["x"] = std::move(cast);
    deref["type"] = width;
    return deref;
}

}  // namespace

std::vector<std::string> lay_out(Json& doc, ByteView code, std::uint32_t start, std::uint32_t end, const inline_asm::Decoder& decoder) {
    const Frame frame = read_frame(code, start, end, decoder);
    if (!frame.size) return {"no frame"};
    Json* body_json = member(&doc, "body");
    if (!body_json || !body_json->is_array() || body_json->empty()) return {"no local the frame places"};
    Json::array_t& body = body_json->get_ref<Json::array_t&>();

    std::vector<Json*> locals;                        // the declarations, in the order the tree has them
    OrderedMap<std::string, Json*> by_name;           // a name met twice keeps its place and takes the last declaration
    for (Json& s : body) {
        if (!is_kind(s, "Local")) continue;
        locals.push_back(&s);
        if (const std::string_view name = symbol_name(&s); !name.empty()) by_name.insert_or_assign(std::string(name), &s);
    }

    // each local's place in the frame (insertion order is the order the passes below report in)
    OrderedMap<std::string, std::int64_t> place;
    for (const auto& [name, s] : by_name) {
        if (const auto at = l2tree::integer(member(s, "at"), "stack")) place[name] = *at + 2;
    }
    l2tree::walk(std::as_const(*body_json), [&](const Json& st) {
        if (!is_kind(st, "ExprStmt")) return;
        const Json* expr = member(&st, "expr");
        if (!is_kind(expr, "Assign")) return;
        const Json* target = member(expr, "target");
        const std::string name(symbol_name(target));
        if (!is_kind(target, "Name") || !by_name.contains(name) || place.contains(name)) return;
        std::set<std::pair<std::int64_t, int>> hits;   // what the statement's instructions write
        for (std::uint32_t a : l2tree::origin_offsets(st)) {
            if (auto w = frame.writes.find(a); w != frame.writes.end()) hits.insert(w->second);
        }
        if (hits.size() == 1) place[name] = hits.begin()->first;
    });
    if (place.empty()) return {"no local the frame places"};

    // the original's places: a buffer (its address taken) runs to the next place above it
    std::set<std::int64_t> point_set(frame.taken);
    for (const auto& entry : frame.places) point_set.insert(entry.first);
    for (const auto& entry : place) point_set.insert(entry.second);
    const std::vector<std::int64_t> points(point_set.begin(), point_set.end());
    std::map<std::int64_t, std::int64_t> span;   // place -> the bytes up to the next one
    for (std::size_t k = 0; k < points.size(); ++k) span[points[k]] = (k + 1 < points.size() ? points[k + 1] : 0) - points[k];

    // the places inside a buffer: from it up to the next taken place or a place written as a variable of its own (one
    // assigned by a statement: a C variable, not a piece)
    std::set<std::int64_t> own;
    for (const auto& [name, p] : place) {
        const Json* at = member(by_name.at(name), "at");
        if (!at || (at->is_string() && at->get_ref<const std::string&>() == "none")) own.insert(p);
    }
    std::map<std::int64_t, std::int64_t> buffers;   // buffer's place -> its bytes
    for (std::int64_t b : frame.taken) {
        std::int64_t top = 0;
        for (std::int64_t x : points) {
            if (x > b && (frame.taken.contains(x) || own.contains(x))) top = std::min(top, x);
        }
        buffers[b] = top - b;
    }

    std::vector<std::string> said;
    OrderedMap<std::string, std::pair<std::int64_t, std::int64_t>> piece_of;   // local -> (buffer, offset in it)
    for (const auto& [name, p] : place) {
        for (const auto& [b, n] : buffers) {
            if (!(b <= p && p < b + n)) continue;
            const auto inside = std::count_if(place.begin(), place.end(), [&](const auto& e) { return b <= e.second && e.second < b + n; });
            const auto s = span.find(b);
            if (p == b && inside == 1 && s != span.end() && s->second == n) continue;   // the buffer's only local, at its start
            piece_of.insert_or_assign(name, std::make_pair(b, p - b));
        }
    }

    // a buffer one of whose pieces holds a far pointer -- declared so, or used so, or the decompiler's `_x` (x and the word
    // above it, joined: a far pointer's halves) -- is left as it was: the back end writes a far pointer's __far on a
    // declaration, and inside a cast `*(T __far **)` would lose it and move two bytes for four (SUWIN 2:410C passed
    // SETUPX.2 a pointer's offset and 0)
    std::set<std::int64_t> held;
    for (const auto& [name, where] : piece_of) {
        if (is_far_pointer(member(by_name.at(name), "type"))) held.insert(where.first);
    }
    l2tree::walk(std::as_const(*body_json), [&](const Json& n) {
        if (!is_kind(n, "Name")) return;
        const std::string name(symbol_name(&n));
        if (const auto* where = piece_of.find(name); where && is_far_pointer(member(&n, "type"))) {
            held.insert(where->first);
        } else if (name.starts_with("_") && !by_name.contains(name)) {
            if (const auto* joined = piece_of.find(name.substr(1))) held.insert(joined->first);
        }
    });
    // In a frame with an array local the buffers are left too, as is one another local overlaps: the back end lays such a
    // frame out itself (backends/c._buffer_fields, after this pass was written): an array the function reads past the end
    // of made to reach over what it reads, the words a callee fills above an array whose address is passed on read as its
    // fields, two buffers or more kept in the frame's order and distances as one packed structure (codebreaker_frame). Cut
    // into pieces under it, the bytes were no longer shared -- SUWIN 4:503E's axStack_52 + 0x3c reached over the pieces at
    // [bp-30h], VERX's VERQUERYVALUE lost the words its callee fills -- or C 8 at /Ox placed them elsewhere (SUWIN 6:6562,
    // which matched byte for byte, no longer did).
    const auto has_pieces = [&](std::int64_t b) {
        return std::any_of(piece_of.begin(), piece_of.end(), [&](const auto& e) { return e.second.first == b; });
    };
    for (const auto& [name, p] : place) {
        const Json* t = member(by_name.at(name), "type");
        const std::int64_t size = type_bytes(t) ? type_bytes(t) : 2;
        for (const auto& [b, n] : buffers) {
            if (!has_pieces(b)) continue;
            if (text(t, "kind") == "array" || (!piece_of.contains(name) && p < b + n && b < p + size && p != b)) held.insert(b);
        }
    }
    std::vector<std::string> given_up;
    for (const auto& [name, where] : piece_of) {
        if (held.contains(where.first)) given_up.push_back(name);
    }
    for (const std::string& name : given_up) piece_of.erase(name);
    if (!held.empty()) {
        std::string places;
        for (std::int64_t b : held) places += (places.empty() ? "" : ", ") + std::format("[bp{}]", b);
        said.push_back("a buffer left as it was: a far pointer in it, an array in the frame, or a local over it (" + places + ")");
    }

    // rewrite: pieces read through one array; scalars typed by their width
    std::map<std::int64_t, std::string> names;   // the arrays that stand for buffers, by place
    for (const auto& [b, n] : buffers) {
        if (has_pieces(b)) names[b] = std::format("local_{:x}", -b);
    }

    // (a piece of one first: x._2_2_, 2 bytes at x + 2 -- written `->_2_2_` of the cast, it did not compile)
    l2tree::walk(*body_json, [&](Json& n) {
        if (!is_kind(n, "Select")) return;
        const Json* obj = member(&n, "obj");
        const auto piece = piece_name(symbol_name(&n));
        const std::string name(symbol_name(obj));
        const auto* where = piece_of.find(name);
        if (!piece || !is_kind(obj, "Name") || !where) return;
        const auto width = unsigned_width(piece->second);
        if (!width) return;
        n = piece_of_buffer(names.at(where->first), buffers.at(where->first), where->second + piece->first, *width);
    });
    for (const auto& [name, where] : piece_of) {
        const auto& [b, off] = where;
        const Json* declared = member(by_name.at(name), "type");   // (at its declared type)
        const Json width = declared ? *declared : Json(nullptr);
        std::vector<Json*> uses;
        l2tree::walk(*body_json, [&](Json& n) {
            if (is_kind(n, "Name") && symbol_name(&n) == name) uses.push_back(&n);
        });
        for (Json* n : uses) *n = piece_of_buffer(names.at(b), buffers.at(b), off, width);
        std::vector<Json*> fragments;   // (and in an _asm block: the array, at the offset)
        l2tree::walk(*body_json, [&](Json& n) {
            if (is_kind(n, "MachineFragment")) fragments.push_back(&n);
        });
        const std::string with = names.at(b) + (off ? std::format("[{}]", off) : std::string());
        for (Json* n : fragments) {
            Json lines = Json::array();
            if (const Json* old = member(n, "asm"); old && old->is_array()) {
                for (const Json& ln : *old) lines.push_back(ln.is_string() ? Json(replace_word(ln.get_ref<const std::string&>(), name, with)) : ln);
            }
            (*n)["asm"] = std::move(lines);
        }
        said.push_back(std::format("{}: {}'s bytes {}..", name, names.at(b), off));
    }
    for (const auto& [name, p] : place) {
        if (piece_of.contains(name)) continue;
        const auto w = frame.places.find(p);
        // (a bool only: a local of the decompiler's unknown type, xunknownN, takes the type its uses give it in the back
        // end -- `xStack_16`, a pointer, made an int2 was dereferenced, SUWIN 9:0924)
        if (w != frame.places.end() && has_signed_width(w->second) && text(member(by_name.at(name), "type"), "kind") == "bool") {
            (*by_name.at(name))["type"] = signed_width(w->second);
            said.push_back(std::format("{}: {} bytes at [bp{}]", name, w->second, p));
        }
    }

    // declarations: the frame's, nearest the frame pointer first; the rest as they were, after
    std::vector<std::pair<std::int64_t, Json>> decl;
    for (const auto& [b, name] : names) {
        Json local = Json::object();
        local["node"] = "Local";
        Json sym = Json::object();
        sym["kind"] = "local";
        sym["name"] = name;
        local["sym"] = std::move(sym);
        Json type = Json::object();
        type["kind"] = "array";
        type["elem"] = kind_only("u8");
        type["count"] = buffers.at(b);
        local["type"] = std::move(type);
        Json at = Json::object();
        at["stack"] = b - 2;
        local["at"] = std::move(at);
        decl.emplace_back(b, std::move(local));
    }
    for (Json* s : locals) {
        const std::string name(symbol_name(s));
        if (piece_of.contains(name)) continue;
        const auto* p = place.find(name);
        decl.emplace_back(p ? *p : 1, *s);
    }
    std::stable_sort(decl.begin(), decl.end(), [](const auto& a, const auto& b) {
        const auto key = [](std::int64_t p) { return p <= 0 ? -p : std::int64_t{1}; };
        return key(a.first) < key(b.first);
    });
    Json declared = Json::array();
    for (auto& [at, s] : decl) declared.push_back(std::move(s));
    for (const Json& s : body) {
        if (!is_kind(s, "Local")) declared.push_back(s);
    }
    *body_json = std::move(declared);
    return said;
}

}  // namespace cb::decompile::frame16
