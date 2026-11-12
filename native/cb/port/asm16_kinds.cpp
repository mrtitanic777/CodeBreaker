// cb/port/asm16_kinds.cpp -- reading a module into labels, code and data, and following what each register holds through
// it to a fixed point. (See asm16.hpp.)
// cb-unity: alone

#include <algorithm>
#include <deque>
#include <format>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"

namespace cb::port::asm16 {

namespace {

using Region_ = Region;

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

std::size_t reg_index(std::string_view n) {
    if (n.size() != 2) return 8;
    switch ((n[0] << 8) | n[1]) {
        case ('a' << 8) | 'x': return 0;
        case ('b' << 8) | 'x': return 1;
        case ('c' << 8) | 'x': return 2;
        case ('d' << 8) | 'x': return 3;
        case ('s' << 8) | 'i': return 4;
        case ('d' << 8) | 'i': return 5;
        case ('b' << 8) | 'p': return 6;
        case ('s' << 8) | 'p': return 7;
        default: return 8;
    }
}

// The last word of an operand (`SHORT lab`, `NEAR PTR f`: the label is the last word).
std::string last_word(std::string_view arg, const Line& l) {
    const auto words = text::split_ws(arg);
    if (words.empty()) throw Asm16Error("line " + std::to_string(l.no) + " (" + std::string(text::trim(l.text)) + "): no operand");
    return std::string(words.back());
}
std::string target_of(const Line& l) {
    if (l.args.empty()) throw Asm16Error("line " + std::to_string(l.no) + " (" + std::string(text::trim(l.text)) + "): no operand");
    return last_word(l.args[0], l);
}

std::string collapse_to(std::string_view s, std::size_t n) {   // spaces collapsed, trimmed, cut at n characters
    std::string out;
    bool in_space = false;
    for (const char c : s) {
        if (text::is_space(c)) {
            in_space = true;
        } else {
            if (in_space && !out.empty()) out.push_back(' ');
            in_space = false;
            out.push_back(c);
        }
    }
    return out.substr(0, n);
}

// Whether `text` has `word` as a whole word (ASCII case-insensitive).
bool has_word_ci(std::string_view text, std::string_view word) {
    for (std::size_t at = 0; at + word.size() <= text.size(); ++at) {
        if (!text::iequals(text.substr(at, word.size()), word)) continue;
        if ((at > 0 && is_word(text[at - 1])) || (at + word.size() < text.size() && is_word(text[at + word.size()]))) continue;
        return true;
    }
    return false;
}

// A C type without the words that say where a member lives or how far it reaches (far near huge extern static), trimmed.
std::string without_storage_words(std::string_view type) {
    std::string out;
    for (std::size_t i = 0; i < type.size();) {
        if (!is_word(type[i])) {
            out.push_back(type[i++]);
            continue;
        }
        std::size_t j = i;
        while (j < type.size() && is_word(type[j])) ++j;
        const std::string_view w = type.substr(i, j - i);
        if (w != "far" && w != "near" && w != "huge" && w != "extern" && w != "static") out.append(w);
        i = j;
    }
    return std::string(text::trim(out));
}

// A register used inside `[...]` of an operand: some bracket group (up to its `]`, or the end) holds the word.
bool word_in_brackets(std::string_view arg, std::string_view reg) {
    for (std::size_t at = arg.find('['); at != std::string_view::npos; at = arg.find('[', at + 1)) {
        std::size_t close = arg.find(']', at + 1);
        if (close == std::string_view::npos) close = arg.size();
        const std::string_view inside = arg.substr(at + 1, close - at - 1);
        for (std::size_t w = 0; w + reg.size() <= inside.size(); ++w) {
            if (!text::iequals(inside.substr(w, reg.size()), reg)) continue;
            if ((w > 0 && is_word(inside[w - 1])) || (w + reg.size() < inside.size() && is_word(inside[w + reg.size()]))) continue;
            return true;
        }
    }
    return false;
}

// `cs:` anywhere in the operand.
bool has_cs_colon(std::string_view x) {
    for (std::size_t at = 0; at + 2 <= x.size(); ++at) {
        if (!text::iequals(x.substr(at, 2), "cs")) continue;
        std::size_t i = at + 2;
        while (i < x.size() && text::is_space(x[i])) ++i;
        if (i < x.size() && x[i] == ':') return true;
    }
    return false;
}

// `cs:[... reg ...]`: the code segment read through the register.
bool cs_bracket(std::string_view x, std::string_view reg) {
    for (std::size_t at = 0; at + 2 <= x.size(); ++at) {
        if (!text::iequals(x.substr(at, 2), "cs")) continue;
        std::size_t i = at + 2;
        while (i < x.size() && text::is_space(x[i])) ++i;
        if (i >= x.size() || x[i] != ':') continue;
        ++i;
        while (i < x.size() && text::is_space(x[i])) ++i;
        if (i < x.size() && x[i] == '[' && word_in_brackets(x.substr(i), reg)) return true;
    }
    return false;
}

// `offset [seg:]name` and nothing else: the name.
std::optional<std::string> plain_offset_symbol(std::string_view v) {
    v = text::trim(v);
    if (v.size() < 7 || !text::iequals(v.substr(0, 6), "offset") || !text::is_space(v[6])) return std::nullopt;
    std::size_t i = 6;
    while (i < v.size() && text::is_space(v[i])) ++i;
    std::size_t w = i;
    while (w < v.size() && is_word(v[w])) ++w;
    if (w > i && w < v.size() && v[w] == ':') i = w + 1;
    std::size_t j = i;
    while (j < v.size() && is_word(v[j])) ++j;
    if (j == i || j != v.size()) return std::nullopt;
    return std::string(v.substr(i, j - i));
}

std::string type_text(const K& k) { return k.region == Region_::Any && k.to ? *k.to : std::string(); }

}  // namespace

// ---- what a register holds ------------------------------------------------------------------------------------------

OriginSet OriginSet::with(const Origin& o) const {
    std::set<Origin> s = get();
    s.insert(o);
    return OriginSet(std::move(s));
}

OriginSet OriginSet::united(const OriginSet& other) const {
    if (other.empty() || p_ == other.p_) return *this;
    if (empty()) return other;
    std::set<Origin> s = get();
    s.insert(other.get().begin(), other.get().end());
    return OriginSet(std::move(s));
}

bool OriginSet::contains_all(const OriginSet& other) const {
    return p_ == other.p_ || std::includes(get().begin(), get().end(), other.get().begin(), other.get().end());
}

K K::ptr(std::optional<std::string> to) {
    K k;
    k.kind = Kind::Ptr;
    if (to && !to->empty()) k.to = std::move(to);
    return k;
}

bool K::same_kind(const K& o) const {
    return kind == o.kind && to == o.to && region == o.region && ds_address == o.ds_address && owner == o.owner && old == o.old &&
           new_ == o.new_;
}

namespace {

bool same_pointee(const K& a, const K& b) { return a.to == b.to && a.region == b.region && a.ds_address == b.ds_address; }

}  // namespace

std::optional<K> join(const std::optional<K>& a, const std::optional<K>& b, const FirstMembers& first_members) {
    if (!a) return b;
    if (a->kind == Kind::Ptr && b && b->kind == Kind::Ptr && !same_pointee(*a, *b) && a->points_somewhere() && b->points_somewhere()) {
        // a pointer to a structure whose first member is a pointer: a pointer to such a pointer too -- a chain walked
        // from its head's place
        if (a->region == Region::Any && b->region == Region::Any) {
            for (const auto& [x, y] : {std::pair{&*a, &*b}, std::pair{&*b, &*a}}) {
                const auto fm = first_members.find(*y->to);
                if (fm != first_members.end() && fm->second == *x->to) {
                    K k = K::ptr(x->to);
                    k.origin = a->origin.united(b->origin);
                    return k;
                }
            }
        }
    }
    if (!b || a->same_kind(*b)) {
        if (!b || a->origin.contains_all(b->origin)) return a;
        K k = *a;
        k.origin = a->origin.united(b->origin);
        return k;
    }
    const OriginSet origin = a->origin.united(b->origin);
    if (a->kind == Kind::Val && b->kind == Kind::Val) return K::val(origin);
    if (a->kind == Kind::Ptr && b->kind == Kind::Ptr) {
        K k = same_pointee(*a, *b) ? *a : K::ptr();
        k.origin = origin;
        return k;
    }
    if (a->kind == Kind::Val || b->kind == Kind::Val) {
        const K& o = a->kind != Kind::Val ? *a : *b;
        K k = K::opq(origin);
        k.to = o.to;
        k.region = o.region;
        k.ds_address = o.ds_address;
        k.owner = o.owner;
        return k;
    }
    return K::opq(origin);
}

const K* State::reg(std::string_view name) const {
    const std::size_t i = reg_index(name);
    return i < 8 && regs[i] ? &*regs[i] : nullptr;
}

std::optional<K>& State::slot(std::string_view name) {
    const std::size_t i = reg_index(name);
    CB_ASSERT(i < 8, "a 16-bit register's name");
    return regs[i];
}

State merge(const State* a, const State& b, const FirstMembers& fm) {
    if (!a) return b;
    State s = *a;
    for (std::size_t i = 0; i < 8; ++i) s.regs[i] = join(a->regs[i], b.regs[i], fm);
    s.es = join(a->es, b.es, fm);
    s.ds = join(a->ds, b.ds, fm);
    if (a->stack.size() == b.stack.size())
        for (std::size_t i = 0; i < s.stack.size(); ++i) s.stack[i].k = join(a->stack[i].k, b.stack[i].k, fm);
    s.routines.insert(b.routines.begin(), b.routines.end());
    return s;
}

bool same_state(const State& a, const State& b) {
    auto same_opt = [](const std::optional<K>& x, const std::optional<K>& y, bool with_origin) {
        if (x.has_value() != y.has_value()) return false;
        return !x || (x->same_kind(*y) && (!with_origin || x->origin == y->origin));
    };
    for (std::size_t i = 0; i < 8; ++i)
        if (!same_opt(a.regs[i], b.regs[i], true)) return false;
    if (!same_opt(a.es, b.es, true) || !same_opt(a.ds, b.ds, true)) return false;
    if (a.stack.size() != b.stack.size() || a.sp_old != b.sp_old || a.sp_new != b.sp_new || a.routines != b.routines) return false;
    for (std::size_t i = 0; i < a.stack.size(); ++i)
        if (a.stack[i].old_size != b.stack[i].old_size || a.stack[i].new_size != b.stack[i].new_size ||
            !same_opt(a.stack[i].k, b.stack[i].k, false))
            return false;
    return true;
}

// ---- reading the module ---------------------------------------------------------------------------------------------

Port::Port(const translate::backends::assembly::Document& doc, const CTypes& ctypes, PortSettings settings)
    : ct_(ctypes), settings_(std::move(settings)) {
    source_ = translate::backends::assembly::module(doc).value();
    std::vector<Line> lines = parse(source_);
    c_names_all_.insert(settings_.c_names.begin(), settings_.c_names.end());
    for (const auto& [name, layout] : ct_.old_layouts) {   // (the structures that begin with a pointer)
        const auto f = ct_.field(name, 0);
        if (f && f->is_pointer && f->old_offset == 0 && f->old_size == 2) first_members_[name] = f->type + " *";
    }
    code_seg_ = code_segments(lines);
    std::optional<std::string> seg;
    bool proc_far = false;
    for (Line& l : lines) {
        const std::optional<Directive> d = l.directive;
        if (d && d->kind == "SEGMENT") {
            seg = *d->name;
            continue;
        }
        if (d && d->kind == "ENDS") {
            seg.reset();
            continue;
        }
        if (d && d->kind == "PUBLIC") {
            for (const std::string_view x : text::split(d->rest, ','))
                if (!text::trim(x).empty()) public_.emplace(text::trim(x));
            continue;
        }
        if (d && d->kind == "EXTRN") {
            for (const std::string_view x : text::split(d->rest, ',')) {
                const std::size_t colon = x.find(':');
                if (colon != std::string_view::npos)
                    extrn_[std::string(text::trim(x.substr(0, colon)))] = text::upper(text::trim(x.substr(colon + 1)));
            }
            continue;
        }
        if (d && (d->kind == "EQU" || d->kind == "=")) {
            equ_.values[*d->name] = std::string(text::trim(d->rest));
            continue;
        }
        if (seg && code_seg_.contains(*seg)) {
            l.seg = seg;
            if (d && d->kind == "LABEL") l.label = *d->name;
            if (d && d->kind == "PROC") {   // (NAME PROC [FAR]: the routine's label; its ret far)
                l.label = *d->name;
                l.directive.reset();
                proc_far = has_word_ci(d->rest, "far");
            }
            if (d && d->kind == "ENDP") proc_far = false;
            if (l.op && *l.op == "ret" && proc_far) l.op = "retf";
            if (l.label) labels_.insert_or_assign(*l.label, code_.size());
            auto is_ret_bytes = [&](const Line& prev) {
                if (!prev.directive || prev.directive->kind != "DB" || prev.op) return false;
                const std::string first = prev.directive->rest.substr(0, prev.directive->rest.find(';'));
                const auto n = number(first);
                return n && (*n == 0xCA || *n == 0xC2);
            };
            if (d && (d->kind == "DB" || d->kind == "DW") && !code_.empty() && is_ret_bytes(code_.back())) {
                Line& prev = code_.back();   // (DB 0CAh, DW n: a return written as its bytes)
                prev.op = number(prev.directive->rest) == 0xCA ? "retf" : "ret";
                prev.args = {std::string(text::trim(strip_comment(d->rest)))};
                prev.directive.reset();
                continue;
            }
            if (d && d->kind == "DB") {
                const auto n = number(strip_comment(d->rest));
                if (n && (*n == 0xCB || *n == 0xC3)) {
                    l.op = *n == 0xCB ? "retf" : "ret";
                    l.args.clear();
                    l.directive.reset();
                }
            }
            if (l.op || l.label || (d && (d->kind == "DB" || d->kind == "DW" || d->kind == "DD"))) code_.push_back(std::move(l));
        } else if (seg) {
            data_.emplace_back(*seg, std::move(l));
        }
    }
    for (const auto& [name, kind] : extrn_) {
        if (kind == "BYTE") equ_.sizes[name] = 1;
        else if (kind == "WORD") equ_.sizes[name] = 2;
        else if (kind == "DWORD") equ_.sizes[name] = 4;
        else if (kind == "QWORD") equ_.sizes[name] = 8;
        else if (kind == "TBYTE") equ_.sizes[name] = 10;
    }
    for (const auto& [segment, l] : data_) {
        const auto& d = l.directive;
        if (!d || !d->name) continue;
        if (d->kind == "DB") equ_.sizes[*d->name] = 1;
        else if (d->kind == "DW") equ_.sizes[*d->name] = 2;
        else if (d->kind == "DD") equ_.sizes[*d->name] = 4;
        else if (d->kind == "DQ") equ_.sizes[*d->name] = 8;
        else if (d->kind == "DT") equ_.sizes[*d->name] = 10;
    }
    std::map<std::string, std::string> lower;   // (the C names, by lower case: the last of two alike wins)
    for (const std::string& n : settings_.c_names) lower[text::lower(n)] = n;
    for (const std::string& p : public_) {
        if (!labels_.contains(p)) continue;
        const std::string n = p.starts_with('_') ? p.substr(1) : p;
        const auto it = lower.find(text::lower(n));
        cname[p] = it == lower.end() ? n : it->second;
    }
    for (const auto& [p, c] : cname)
        if (settings_.pascal.contains(c) || (!p.starts_with('_') && settings_.underscore)) pascal_.insert(p);
}

std::optional<Port::CodePlace> Port::code_label(const std::optional<std::string>& seg, std::int64_t value) const {
    std::optional<std::pair<const std::string*, std::int64_t>> best;
    for (const auto& [n, place] : offsets.labels) {
        if (n.starts_with('@')) continue;
        if (seg && place.seg == *seg && place.offset <= value && labels_.contains(n) && (!best || place.offset > best->second))
            best = {&n, place.offset};
    }
    if (!best) return std::nullopt;
    std::int64_t next = 1 << 20;
    for (const auto& [n, place] : offsets.labels)
        if (!n.starts_with('@') && seg && place.seg == *seg && place.offset > best->second) next = std::min<std::int64_t>(next, place.offset);
    if (value >= next) return std::nullopt;
    return CodePlace{*best->first, value - best->second};
}

void Port::place_data() {
    dline_.clear();
    std::size_t k = 0;
    const std::vector<ListedLine>& lines = offsets.lines;
    for (std::size_t i = 0; i < code_.size(); ++i) {
        const Line& l = code_[i];
        if (!l.directive || (l.directive->kind != "DB" && l.directive->kind != "DW" && l.directive->kind != "DD")) continue;
        const std::string want = collapse_to(strip_comment(l.text), 24);
        for (std::size_t j = k; j < lines.size(); ++j) {
            if (!(l.seg && lines[j].seg == *l.seg && lines[j].text.substr(0, 16) == want.substr(0, 16))) continue;
            std::optional<std::uint32_t> next;   // the next line of the segment at a higher offset
            for (std::size_t x = j + 1; x < lines.size(); ++x)
                if (lines[x].seg == *l.seg && lines[x].offset > lines[j].offset) {
                    next = lines[x].offset;
                    break;
                }
            dline_[i] = {lines[j].offset, next ? *next - lines[j].offset : lines[j].length};
            k = j + 1;
            break;
        }
    }
}

std::optional<Port::CodePlace> Port::data_address(const std::string& seg, std::int64_t v) const {
    std::optional<std::pair<std::string, std::int64_t>> best;
    for (const auto& [i, place] : dline_) {
        const auto [o, n] = place;
        if (code_[i].seg != seg) continue;
        if (o <= v && v < o + std::max<std::int64_t>(n, 1)) return CodePlace{std::format("@d_{:04x}", o), v - o};
        if (v < o && o <= v + 128 && (!best || o < best->second))   // (a table counted from a base before it)
            best = {std::format("@d_{:04x}", o), o};
    }
    if (best) return CodePlace{best->first, v - best->second};
    return std::nullopt;
}

std::set<std::string> Port::table_targets() const {
    std::set<std::string> out;
    for (const Line& l : code_) {
        if (!l.directive || l.directive->kind != "DW") continue;
        for (const std::string& v : split_ops(strip_comment(l.directive->rest))) {
            const auto n = number(v);
            if (n)
                if (const auto c = code_label(l.seg, *n); c && c->distance == 0) out.insert(c->label);
            if (const auto o = plain_offset_symbol(v); o && labels_.contains(*o)) out.insert(*o);
        }
    }
    return out;
}

bool Port::cs_address(std::size_t i, std::string_view r) const {
    for (std::size_t j = i + 1; j < std::min(i + 40, code_.size()); ++j) {
        const Line& l = code_[j];
        if (!l.op) continue;
        for (const std::string& x : l.args)
            if (cs_bracket(x, r)) return true;
        const std::string& op = *l.op;
        if (!l.args.empty() && (op == "mov" || op == "lea" || op == "pop" || op == "les" || op == "lds") && text::lower(l.args[0]) == r)
            return false;
        if (op == "ret" || op == "retf" || op == "jmp") return false;
    }
    return false;
}

bool Port::ds_base(std::size_t i, std::string_view r) const {
    for (std::size_t j = i + 1; j < std::min(i + 60, code_.size()); ++j) {
        const Line& l = code_[j];
        if (!l.op) continue;
        for (const std::string& x : l.args)
            if (word_in_brackets(x, r) && !has_cs_colon(x)) return true;
        const std::string& op = *l.op;
        if (!l.args.empty() && (op == "mov" || op == "lea" || op == "pop" || op == "les" || op == "lds" || op == "xchg") &&
            text::lower(l.args[0]) == r)
            return false;
        if (op == "ret" || op == "retf" || op == "jmp" || op == "call") return false;
    }
    return false;
}

// ---- before the kinds -----------------------------------------------------------------------------------------------

const std::vector<Operand>& Port::ops(std::size_t i) const {
    if (ops_cache_.size() != code_.size()) ops_cache_.assign(code_.size(), std::nullopt);
    std::optional<std::vector<Operand>>& slot = ops_cache_[i];
    if (!slot) {
        std::vector<Operand> v;
        for (const std::string& x : code_[i].args) v.push_back(operand(x, &equ_));
        slot = std::move(v);
    }
    return *slot;
}

void Port::rewrite_bytes_as_jumps() {
    static const std::map<std::int64_t, std::string> kJcc = {{0x70, "jo"},  {0x71, "jno"}, {0x72, "jb"},  {0x73, "jae"}, {0x74, "je"},
                                                             {0x75, "jne"}, {0x76, "jbe"}, {0x77, "ja"},  {0x78, "js"},  {0x79, "jns"},
                                                             {0x7C, "jl"},  {0x7D, "jge"}, {0x7E, "jle"}, {0x7F, "jg"},  {0xEB, "jmp"}};
    for (std::size_t i = 0; i + 1 < code_.size(); ++i) {
        Line& l = code_[i];
        Line& n = code_[i + 1];
        if (!(l.directive && n.directive && l.directive->kind == "DB" && n.directive->kind == "DB" && dline_.contains(i) && !n.label)) continue;
        const auto op = number(strip_comment(l.directive->rest));
        const auto d = number(strip_comment(n.directive->rest));
        if (!op || !d) continue;
        const auto jcc = kJcc.find(*op);
        if (jcc == kJcc.end() || !is_jump(jcc->second)) continue;
        const std::int64_t to = dline_[i].first + 2 + (*d >= 128 ? *d - 256 : *d);
        const auto c = code_label(l.seg, to);
        if (c && c->distance == 0) {
            l.op = jcc->second;
            l.args = {c->label};
            l.directive.reset();
            n.directive.reset();
            notes_.push_back(std::format("line {}: a jump written as its bytes - {} {}", l.no, jcc->second, c->label));
        }
    }
}

void Port::rewrite_bytes_as_instructions() {
    std::map<std::pair<std::string, std::uint32_t>, const ListedLine*> at;
    for (const ListedLine& x : offsets.lines) at[{x.seg, x.offset}] = &x;
    for (std::size_t i = 0; i + 1 < code_.size(); ++i) {   // (an instruction written as its bytes, running into the next
        Line& l = code_[i];                                 // line's: the instruction, the next gone)
        Line& n = code_[i + 1];
        if (!(l.directive && l.directive->kind == "DB" && dline_.contains(i) && n.op && !n.label)) continue;
        cb::Bytes db;
        for (const std::string& x : split_ops(strip_comment(l.directive->rest)))
            if (const auto v = number(x)) db.push_back(static_cast<std::uint8_t>(*v & 0xFF));
        const auto nxt = at.find({*l.seg, dline_[i].first + static_cast<std::uint32_t>(db.size())});
        if (nxt == at.end()) continue;
        cb::Bytes both = db;
        both.insert(both.end(), nxt->second->bytes.begin(), nxt->second->bytes.begin() + std::min<std::size_t>(4, nxt->second->bytes.size()));
        const auto ins = decode_direct(both);
        if (ins && db.size() < ins->length && ins->length == db.size() + nxt->second->bytes.size()) {
            l.op = ins->op;
            l.args = ins->args;
            l.directive.reset();
            n.op.reset();
            n.args.clear();
            notes_.push_back(std::format("line {}: an instruction written as its bytes, with the next line's - {} {}", l.no, ins->op,
                                         text::join(ins->args, ", ")));
        }
    }
}

// Before the kinds: the table lines of the code placed (the listing's offsets); a short jump written as its two bytes
// (DB 74h / DB 18h) the jump again, to the label where the listing puts its target.
void Port::prepare() {
    place_data();
    rewrite_bytes_as_jumps();
    rewrite_bytes_as_instructions();
    ops_cache_.clear();
}

// ---- the kinds, to a fixed point ------------------------------------------------------------------------------------

State Port::entry_state(const std::string& routine) const {
    State s;
    for (const std::string_view r : kReg16) {
        K k = K::opq(OriginSet({Origin{Origin::Source::In, routine, std::string(r), 0}}));
        s.regs[reg_index(r)] = std::move(k);
    }
    s.regs[7] = K{.kind = Kind::Frame};   // sp as it was at entry
    s.routines = {routine};
    return s;
}

void Port::flow_to(std::size_t j, const State& st, std::deque<std::size_t>& work, std::vector<char>& queued) {
    const std::optional<State>& old = before_[j];
    State merged = merge(old ? &*old : nullptr, st, first_members_);
    if (!old || !same_state(merged, *old)) {
        before_[j] = std::move(merged);
        if (!queued[j]) {
            queued[j] = 1;
            work.push_back(j);
        }
    }
}

void Port::analyse() {
    before_.assign(code_.size(), std::nullopt);
    std::deque<std::size_t> work;
    std::vector<char> queued(code_.size(), 0);
    auto seed = [&](const std::string& p) {
        const std::size_t i = labels_.at(p);
        before_[i] = entry_state(p);
        queued[i] = 1;
        work.push_back(i);
    };
    for (const auto& [p, c] : cname) {   // (a routine called from outside -- the C program, the other modules -- or by none:
        if (!c_names_all_.contains(c) && !used_outside.contains(p) && called_.contains(p)) continue;   // an entry; else its
        seed(p);                                                                                       // callers' states are what it is given)
    }
    for (const std::string& p : table_targets())   // (a routine called through a table of the code: an entry of its own)
        if (!before_[labels_.at(p)]) seed(p);
    std::size_t seen = 0;
    while (!work.empty()) {
        const std::size_t i = work.front();
        work.pop_front();
        queued[i] = 0;
        if (++seen > 200000) throw Asm16Error("the kinds do not settle");
        State st = *before_[i];
        std::vector<std::size_t> next;
        std::vector<Flow> flow;
        State pre_call;
        if (code_[i].op) {
            pre_call = st;   // (the state the callee is entered with: before)
            std::tie(st, flow) = step(i, std::move(st));
        } else {
            flow = {Flow{Flow::Kind::Next, {}}};
        }
        for (const Flow& f : flow) {
            switch (f.kind) {
                case Flow::Kind::Next: next.push_back(i + 1); break;
                case Flow::Kind::Ret: break;
                case Flow::Kind::Jump:
                    if (const auto t = labels_.find(f.label)) next.push_back(*t);
                    break;
                case Flow::Kind::Call: {
                    if (const auto t = labels_.find(f.label)) {
                        State cs = pre_call;
                        cs.stack.push_back({2, 8, K{.kind = Kind::Ret}});   // (the return address)
                        cs.sp_old -= 2;
                        cs.sp_new -= 8;
                        State rel;
                        rel.regs = cs.regs;
                        rel.es = cs.es;
                        rel.ds = cs.ds;
                        for (std::size_t r = 0; r < 8; ++r) {
                            const std::optional<K>& k = cs.regs[r];
                            if (!k || k->kind != Kind::Frame) continue;
                            if (r == 6) {   // (BP still the caller's frame: a local routine of its works on it)
                                FrameOwner owner;
                                if (k->owner) {
                                    owner = *k->owner;
                                } else {
                                    owner.routine = routine(i);
                                    for (const StackItem& x : pre_call.stack) owner.stack.emplace_back(x.old_size, x.new_size);
                                }
                                K fk{.kind = Kind::Frame, .owner = std::move(owner), .old = k->old, .new_ = k->new_};
                                rel.regs[r] = std::move(fk);
                            } else {
                                rel.regs[r] = K::opq();   // (a frame of the caller's is none of the callee's)
                            }
                        }
                        rel.regs[7] = K{.kind = Kind::Frame};
                        rel.routines = {f.label};
                        flow_to(*t, rel, work, queued);
                    }
                    next.push_back(i + 1);
                    break;
                }
            }
        }
        for (const std::size_t j : next)
            if (j < code_.size()) flow_to(j, st, work, queued);
    }
}

Mem Port::absolute(const Mem& m) {
    if (m.sym || !m.regs.empty() || m.disp < 0x100 || m.seg == "cs") return m;
    const auto v = data_map.empty() ? std::nullopt : data_var(m.disp);
    if (!v) return m;
    data_used_.insert(v->first);
    return Mem{.size = m.size, .sym = v->first, .disp = v->second, .text = m.text};
}

std::optional<std::pair<std::string, std::int64_t>> Port::data_var(std::int64_t addr) const {
    std::optional<std::size_t> best;
    for (std::size_t k = 0; k < data_map.size(); ++k) {
        if (data_map[k].first <= addr) best = k;
        else break;
    }
    if (!best) return std::nullopt;
    const auto& [a, n] = data_map[*best];
    const std::int64_t next = *best + 1 < data_map.size() ? data_map[*best + 1].first : static_cast<std::int64_t>(a) + 0x10000;
    if (addr < next) return std::pair{n, addr - a};
    return std::nullopt;
}

std::optional<std::string> Port::data_at(std::optional<std::int64_t> v) const {
    if (!v || *v < 0x100) return std::nullopt;
    for (const auto& [a, n] : data_map)
        if (a == *v) return n;
    return std::nullopt;
}

std::optional<std::string> Port::data_var_name(std::uint32_t addr) const {
    const auto v = data_var(addr);
    return v ? std::optional<std::string>(v->first) : std::nullopt;
}

// A memory operand at a register given an old address of the data segment: the variable there.
Mem Port::ds_mem(const Mem& m, const K& k) {
    const std::int64_t addr = static_cast<std::int64_t>(k.ds_address) + m.disp;
    const auto v = data_var(addr);
    if (!v) throw Asm16Error(std::format("the data segment's {:04x}h: no variable known there", addr));
    data_used_.insert(v->first);
    return Mem{.size = m.size, .sym = v->first, .disp = v->second, .text = m.text};
}

std::int64_t Port::retsize(const Rt& r) const { return far.contains(r) ? 4 : 2; }

std::map<std::int64_t, std::string> Port::param_place(const Rt& routine_) const {
    std::map<std::int64_t, std::string> out;
    if (!routine_) return out;
    const auto it = param_types.find(*routine_);
    if (it == param_types.end()) return out;
    std::int64_t at = retsize(routine_);
    for (const ParamType& p : it->second) {
        if (p.to && !p.to->empty()) out[at] = *p.to;
        at += p.size;
    }
    return out;
}

ElementSizes Port::element_sizes(const K& k) const {
    const std::string to = k.region == Region::Any && k.to ? *k.to : std::string();
    if (to.ends_with("*")) return {2, 8};
    if (to.starts_with("struct") || to.starts_with("union")) {
        const auto* o = ct_.old_layouts.find(to);
        const auto* n = ct_.new_layouts.find(to);
        if (o && n) return {o->size, n->size};
    }
    return {1, 1};
}

std::string Port::c_name(std::string_view s) { return std::string(s.starts_with('_') ? s.substr(1) : s); }

std::optional<Port::Callee> Port::callee(const std::string& t) const {
    if (settings_.asm_routines.contains(t)) return Callee{Callee::Kind::Asm, inner_name(t, c_names_all_), {}};
    if (const RuntimeCall* r = runtime_call(t)) return Callee{Callee::Kind::C, r->c_name, r->sig};
    const auto& funcs = settings_.c_funcs;
    if (t.starts_with('_')) {
        if (const CSig* sig = funcs.find(t.substr(1)); sig && !sig->pascal) return Callee{Callee::Kind::C, t.substr(1), *sig};
    }
    if (!t.starts_with('_')) {
        const std::string lt = text::lower(t);
        const std::string* found = nullptr;
        for (const auto& [k, sig] : funcs)
            if (text::lower(k) == lt) found = &k;   // (the last of two alike wins)
        if (found && funcs.at(*found).pascal) return Callee{Callee::Kind::C, *found, funcs.at(*found)};
    }
    return std::nullopt;
}

std::pair<std::vector<Port::Slot>, std::size_t> Port::slots(const CSig& sig) {
    std::vector<std::size_t> order(sig.args.size());
    for (std::size_t i = 0; i < order.size(); ++i) order[i] = i;
    if (sig.pascal) std::reverse(order.begin(), order.end());
    std::vector<Slot> out;
    std::size_t s = 0;
    for (const std::size_t j : order) {
        out.push_back({j, sig.args[j].size, sig.args[j].kind, s});
        s += static_cast<std::size_t>(sig.args[j].size / 2);
    }
    std::sort(out.begin(), out.end(), [](const Slot& a, const Slot& b) { return a.arg < b.arg; });
    return {out, s};
}

std::string Port::local(const std::string& label) const {
    const auto it = cname.find(label);
    return it != cname.end() && it->second == label ? "@" + label : label;
}

std::string Port::sym(const std::string& s) const {
    if (externs().contains(s) || public_.contains(s) || data_used_.contains(s)) {
        const std::string c = c_name(s);
        if (const auto a = aliases.find(c); a != aliases.end())
            return a->second.offset ? std::format("{}+{}", a->second.owner, a->second.offset) : a->second.owner;
        return c;
    }
    return s;
}

std::pair<K, std::optional<Field>> Port::kind_of_mem(const Mem& m0, const State& st) {
    const Mem m = absolute(m0);
    const bool size_word_or_none = !m.size || *m.size == 2 || *m.size == 4;
    if (m.sym && m.regs.size() == 1 && ct_.is_pointer(c_name(*m.sym))) {
        const VarType* v = ct_.var(c_name(*m.sym));
        if (v && v->declarator.find('[') != std::string::npos) {   // (table[bx]: an element of an array of pointers)
            const K* k = st.reg(m.regs[0]);
            if (!(k && k->wide())) return {K::ptr(ct_.pointee(c_name(*m.sym))), std::nullopt};
        }
    }
    if (m.sym && m.regs.empty()) {
        const std::string name = c_name(*m.sym);
        if (own_ptr_.contains({*m.sym, m.disp})) return {K::ptr(), std::nullopt};   // (a slot of the module's own data holding an address)
        if (ct_.is_pointer(name) && size_word_or_none && m.disp == 0) return {K::ptr(ct_.pointee(name)), std::nullopt};
        if (ct_.is_pointer(name) && ct_.far_ptrs.contains(name) && m.disp == 2 && (!m.size || *m.size == 2))
            return {K::seg(), std::nullopt};   // (the segment half of a far pointer variable)
        return {K::val(), std::nullopt};
    }
    if (m.regs.size() == 1 && !m.sym) {
        const K* b = st.reg(m.regs[0]);
        if (b && b->kind == Kind::Ptr && b->region == Region::DataSegment) return kind_of_mem(ds_mem(m, *b), st);
        const std::string bt = b ? type_text(*b) : std::string();
        if (b && b->kind == Kind::Ptr && bt.ends_with("*") && m.disp % 2 == 0 && (!m.size || *m.size == 2)) {
            // (an element of an array of pointers: one of them)
            return {K::ptr(std::string(text::trim(std::string_view(bt).substr(0, bt.size() - 1)))), std::nullopt};
        }
        if (b && b->kind == Kind::Ptr && (bt.starts_with("struct") || bt.starts_with("union")) && !bt.ends_with("*")) {
            if (auto f = ct_.field(bt, m.disp)) {
                if (f->is_pointer && m.disp == f->old_offset) {
                    const std::string to = without_storage_words(f->type);
                    return {K::ptr(to), std::move(f)};
                }
                if (f->is_pointer && m.disp == f->old_offset + 2 && f->old_size == 4) return {K::seg(), std::move(f)};
                return {K::val(), std::move(f)};
            }
        }
        if (b && b->kind == Kind::Frame && b->owner && b->old + m.disp < 0) return {K::val(), std::nullopt};   // (a local of the routine whose frame it is)
        if (b && b->kind == Kind::Frame && b->old + m.disp < 0) {
            const std::int64_t e = b->old + m.disp;   // (below the entry: what was pushed there, or room)
            std::int64_t o = 0;
            for (const StackItem& item : st.stack) {
                if (o - item.old_size <= e && e < o) {
                    if (item.k && item.old_size == 2 && item.new_size == 8) return {*item.k, std::nullopt};
                    return {K::val(), std::nullopt};
                }
                o -= item.old_size;
            }
            return {K::val(), std::nullopt};
        }
        if (b && b->kind == Kind::Frame) {
            const std::int64_t e = b->old + m.disp;
            std::vector<Rt> routines;
            if (b->owner) routines.push_back(b->owner->routine);
            else for (const std::string& r : st.routines) routines.push_back(r);
            for (const Rt& r : routines) {
                const auto place = param_place(r);
                const auto it = place.find(e);
                if (it != place.end() && size_word_or_none) return {K::ptr(it->second), std::nullopt};   // (an argument the C program passes a pointer in)
            }
            return {K::opq(OriginSet({Origin{Origin::Source::Param, m.regs[0], {}, e}})), std::nullopt};
        }
    }
    return {K::val(), std::nullopt};
}

// ---- one instruction's effect on the kinds --------------------------------------------------------------------------

std::pair<State, std::vector<Port::Flow>> Port::step(std::size_t i, State st) {
    const Line& l = code_[i];
    const std::string& op = *l.op;
    const std::vector<Operand>& a = ops(i);
    using OK = Operand::Kind;
    std::vector<Flow> flow = {Flow{Flow::Kind::Next, {}}};

    if ((op == "mov" || op == "pop") && !l.args.empty()) {
        const std::string first = text::lower(text::trim(l.args[0]));
        if (first == "es") st.es.reset();   // (another segment: no address's now)
        if (first == "ds") st.ds.reset();
    }
    auto seg_pair = [&](std::string_view name) -> std::optional<K>& { return name == "es" ? st.es : st.ds; };
    auto setr = [&](const std::string& r, const K& k) {
        if (is_reg8(r)) {
            const K* parent = st.reg(reg8_parent(r));
            st.slot(reg8_parent(r)) = K::val(parent ? parent->origin : OriginSet());
        } else if (is_reg16(r)) {
            st.slot(r) = k;
        }
    };
    auto get = [&](const Operand& o) -> K {
        if (o.kind == OK::Reg) {
            if (is_reg16(o.text)) {
                const K* k = st.reg(o.text);
                return k ? *k : K::val();
            }
            return is_reg8(o.text) ? K::val() : K::seg();
        }
        if (o.kind == OK::Off) {
            const VarType* v = ct_.var(c_name(o.text));
            return K::ptr(v ? std::optional<std::string>(v->base) : std::nullopt);
        }
        if (o.kind == OK::Mem) return kind_of_mem(o.mem, st).first;
        return K::val();
    };

    if (is_jump(op)) {
        flow = {Flow{Flow::Kind::Jump, target_of(l)}};   // (SHORT, NEAR PTR...: the label is the last word)
        if (op != "jmp") flow.push_back({Flow::Kind::Next, {}});
        return {std::move(st), std::move(flow)};
    }
    if (op == "call") {
        const std::string t = target_of(l);
        const bool inside = labels_.contains(t);
        if (!inside) {
            for (const char* r : {"ax", "cx", "dx", "bx"}) st.slot(r) = std::string_view(r) == "ax" ? K::opq() : K::val();
        }
        const std::optional<Callee> ce = inside ? std::nullopt : callee(t);
        if (ce && ce->kind == Callee::Kind::C && ce->sig.pascal) {   // (pascal: the callee took its arguments off)
            std::int64_t words = static_cast<std::int64_t>(slots(ce->sig).second);
            while (words > 0 && !st.stack.empty()) {
                const StackItem it = st.stack.back();
                st.stack.pop_back();
                st.sp_old += it.old_size;
                st.sp_new += it.new_size;
                words -= it.old_size / 2;
            }
        }
        if (ce && ce->kind == Callee::Kind::C && ce->sig.ret == CSig::Ret::Long) st.slot("dx") = K::val();
        if (ce && ce->kind == Callee::Kind::C && ce->sig.ret == CSig::Ret::Ptr) st.slot("ax") = K::ptr(ce->sig.to);   // (the address the C function gives: its type's)
        if (inside) {
            const std::optional<State> after = exit_of(t);
            const std::set<std::string> mods = writes(t);
            if (after) {
                for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "bp"}) {
                    if (!mods.contains(r)) continue;   // (what the routine never writes is the caller's still)
                    const K* k = after->reg(r);
                    if (!k) continue;
                    const bool from_entry = std::any_of(k->origin.get().begin(), k->origin.get().end(),
                                                        [](const Origin& o) { return o.source == Origin::Source::In; });
                    if (!from_entry) st.slot(r) = *k;
                }
            }
        }
        return {std::move(st), {Flow{Flow::Kind::Call, t}}};
    }
    if (op == "ret" || op == "retf" || op == "retn" || op == "iret") {
        std::set<Rt> rts;   // (the routines whose code this is)
        if (st.routines.empty()) rts.insert(routine(i));
        for (const std::string& r : st.routines) rts.insert(r);
        for (const Rt& rt : rts) {
            exits_new_[rt].push_back(st);
            if (op == "retf") far.insert(rt);   // (a far return: a far routine, its arguments 4 on)
        }
        return {std::move(st), {Flow{Flow::Kind::Ret, {}}}};
    }
    if (op == "push") {
        const K k = (!a.empty() && (a[0].kind == OK::Reg || a[0].kind == OK::Mem)) ? get(a[0]) : K::val();
        const bool gone = !a.empty() && a[0].kind == OK::Reg && is_segment_register(a[0].text);   // (a segment register: not pushed any more)
        st.stack.push_back({2, gone ? 0 : 8, k});
        st.sp_old -= 2;
        st.sp_new -= gone ? 0 : 8;
        return {std::move(st), std::move(flow)};
    }
    if (op == "pushf" || op == "pushfw") {
        st.stack.push_back({2, 8, K::val()});
        st.sp_old -= 2;
        st.sp_new -= 8;
        return {std::move(st), std::move(flow)};
    }
    if (op == "pop" || op == "popf" || op == "popfw") {
        StackItem item{2, 8, K::val()};
        if (!st.stack.empty()) {
            item = st.stack.back();
            st.stack.pop_back();
        }
        st.sp_old += 2;
        st.sp_new += item.new_size;
        if (op == "pop" && !a.empty() && a[0].kind == OK::Reg) setr(a[0].text, item.k ? *item.k : K::val());
        return {std::move(st), std::move(flow)};
    }
    if ((op == "mov" || op == "xchg" || op == "lea" || op == "les" || op == "lds") && !a.empty() && a[0].kind == OK::Reg) {
        if (a.size() < 2) throw Asm16Error("line " + std::to_string(l.no) + ": `" + op + "` with one operand");
        const std::string d = a[0].text;
        if (op == "xchg") {
            const K k0 = get(a[0]);
            const K k1 = get(a[1]);
            setr(d, k1);
            if (a[1].kind == OK::Reg) setr(a[1].text, k0);
            return {std::move(st), std::move(flow)};
        }
        if (op == "lea") {
            std::optional<std::string> to;
            if (a[1].kind == OK::Mem && a[1].mem.regs.size() == 1 && !a[1].mem.sym) {
                const K* kb = st.reg(a[1].mem.regs[0]);
                if (kb && kb->kind == Kind::Ptr && kb->region == Region::Any && kb->points_to_struct())
                    to = ct_.member_type(*kb->to, a[1].mem.disp);   // (the address of a member: its type's)
            }
            setr(d, K::ptr(to));
            return {std::move(st), std::move(flow)};
        }
        if (op == "les" || op == "lds") {
            const K k = get(a[1]);
            setr(d, k.wide() ? k : K::ptr());
            seg_pair(op == "les" ? "es" : "ds") = K::seg(OriginSet({Origin{Origin::Source::SegOf, d, {}, 0}}));
            return {std::move(st), std::move(flow)};
        }
        if (a[1].kind == OK::Reg && (a[1].text == "es" || a[1].text == "ds") && is_reg16(d) && seg_pair(a[1].text)) {
            const K& seg_k = *seg_pair(a[1].text);
            const auto partner = std::find_if(seg_k.origin.get().begin(), seg_k.origin.get().end(),
                                              [](const Origin& o) { return o.source == Origin::Source::SegOf; });
            const K* pk = partner != seg_k.origin.get().end() ? st.reg(partner->a) : nullptr;
            if (pk && pk->wide()) {   // (the segment of an address: the address, for its test against 0)
                const K copy = *pk;
                setr(d, copy);
                return {std::move(st), std::move(flow)};
            }
        }
        const Operand& src = a[1];
        const std::optional<std::int64_t> imm = src.kind == OK::Imm ? number(l.args[1]) : std::nullopt;
        if (src.kind == OK::Reg && src.text == "sp") {
            K k{.kind = Kind::Frame};
            k.old = st.sp_old;
            k.new_ = st.sp_new;
            setr(d, k);
        } else if (is_segment_register(d) || (src.kind == OK::Reg && is_segment_register(src.text))) {
            if (is_reg16(d)) setr(d, K::seg());
        } else if (d == "cx") {
            setr(d, K::val());   // (cx addresses nothing on the 8086: a count)
        } else if (imm && (d == "bx" || d == "si" || d == "di") &&
                   ((cs_address(i, d) && (data_address(*l.seg, *imm) || code_label(l.seg, *imm))) ||
                    (data_address(*l.seg, *imm) && *imm >= 0x100))) {
            // (an address register given a number in a table of the code, or just before one: its address)
            setr(d, K::code_ptr());
        } else if (imm && (d == "bx" || d == "si" || d == "di") && !data_map.empty() &&
                   std::any_of(data_map.begin(), data_map.end(), [&](const auto& p) { return p.first == *imm; }) && ds_base(i, d)) {
            setr(d, K::data_ptr(static_cast<std::uint32_t>(*imm)));   // (an old address of the data segment: a variable's)
        } else {
            const K k = get(src);
            setr(d, k.kind == Kind::Seg && src.kind == OK::Mem ? K::val() : k);   // (a far address's segment: a number)
        }
        return {std::move(st), std::move(flow)};
    }
    if (op == "sub" && !a.empty() && a[0].is_reg("sp")) {
        const std::int64_t n = l.args.size() > 1 ? number(l.args[1]).value_or(0) : 0;
        st.stack.push_back({n, n, K::val()});
        st.sp_old -= n;
        st.sp_new -= n;
        return {std::move(st), std::move(flow)};
    }
    if (op == "add" && !a.empty() && a[0].is_reg("sp")) {
        std::int64_t n = l.args.size() > 1 ? number(l.args[1]).value_or(0) : 0;
        st.sp_old += n;
        while (n > 0 && !st.stack.empty()) {
            const StackItem item = st.stack.back();
            st.stack.pop_back();
            n -= item.old_size;
            st.sp_new += item.new_size;
        }
        return {std::move(st), std::move(flow)};
    }
    if (is_string_op(op) || op == "cbw" || op == "cwd" || op == "mul" || op == "imul" || op == "div" || op == "idiv" || op == "lahf") {
        if (op == "lodsb" || op == "lodsw" || op == "cbw" || op == "lahf") st.slot("ax") = K::val();
        if (op == "cwd" || op == "mul" || op == "imul" || op == "div" || op == "idiv") {
            st.slot("ax") = K::val();
            st.slot("dx") = K::val();
        }
        if (l.prefix) st.slot("cx") = K::val();
        return {std::move(st), std::move(flow)};
    }
    if (op == "int") {
        st.slot("ax") = K::val();
        return {std::move(st), std::move(flow)};
    }
    static const std::set<std::string_view> kArith = {"add", "sub", "inc", "dec", "neg", "and", "or",  "xor", "adc", "sbb",
                                                      "not", "shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr"};
    if (!a.empty() && a[0].kind == OK::Reg && kArith.contains(op)) {
        const std::string d = a[0].text;
        std::optional<K> k;   // (what the register held before)
        if (is_reg16(d)) {
            if (const K* kk = st.reg(d)) k = *kk;
        } else {
            k = K::val();
        }
        if (is_reg8(d)) {
            setr(d, K::val());
        } else if (op == "xor" && a.size() > 1 && a[1].same_register(a[0])) {
            setr(d, K::val());
        } else if (op == "sub" && k && k->wide() && a.size() > 1 && a[1].kind == OK::Imm && data_at(number(l.args[1]))) {
            setr(d, K::val());   // (an address less a variable's: how far into it)
        } else if ((op == "add" || op == "sub") && k && k->wide()) {
            const K s2 = a.size() > 1 ? get(a[1]) : K::val();
            if (op == "sub" && s2.wide()) {
                setr(d, k->kind == Kind::Opq && s2.kind == Kind::Opq ? K::opq(k->origin.united(s2.origin)) : K::val());
            } else if (k->kind == Kind::Frame) {
                const std::int64_t n = l.args.size() > 1 ? number(l.args[1]).value_or(0) : 0;
                K moved{.kind = Kind::Frame, .owner = k->owner};
                moved.old = k->old + (op == "add" ? n : -n);
                moved.new_ = k->new_ + (op == "add" ? n : -n);
                setr(d, moved);
            } else {
                setr(d, *k);
            }
        } else if ((op == "inc" || op == "dec") && k && k->wide()) {
            setr(d, *k);
        } else if ((op == "and" || op == "or") && a.size() > 1 && a[1].same_register(a[0])) {
            // (a test of itself)
        } else {
            setr(d, K::val());
        }
        if (op == "add" && a.size() > 1 && k && k->kind == Kind::Val) {
            const K s2 = get(a[1]);
            if (s2.wide() && s2.kind != Kind::Frame) setr(d, s2);   // (a value added to a pointer: the pointer)
        }
        return {std::move(st), std::move(flow)};
    }
    return {std::move(st), std::move(flow)};
}

// ---- what a routine writes, where it returns, which routine a line is in -------------------------------------------

std::set<std::string> Port::writes(const std::string& label) {
    // the recursive walk, sharing the labels already on the way
    struct Walker {
        Port& port;
        std::set<std::string> seen;
        std::set<std::string> walk(const std::string& lab) {
            if (const auto cached = port.writes_.find(lab); cached != port.writes_.end()) return cached->second;
            if (seen.contains(lab) || !port.labels_.contains(lab)) return {};
            seen.insert(lab);
            std::set<std::string> out;
            std::vector<std::size_t> todo = {port.labels_.at(lab)};
            std::set<std::size_t> done;
            std::map<std::string, int> pushed;
            while (!todo.empty()) {
                const std::size_t i = todo.back();
                todo.pop_back();
                if (done.contains(i) || i >= port.code_.size()) continue;
                done.insert(i);
                const Line& l = port.code_[i];
                std::vector<std::size_t> next = {i + 1};
                if (l.op) {
                    const std::string& op = *l.op;
                    const std::string a0 = l.args.empty() ? std::string() : text::lower(l.args[0]);
                    const std::string r16 = is_reg8(a0) ? std::string(reg8_parent(a0)) : a0;
                    static const std::set<std::string_view> kWriters = {"mov", "add", "sub", "and", "or", "xor", "adc", "sbb", "inc", "dec", "neg",
                                                                        "not", "shl", "shr", "sal", "sar", "rol", "ror", "rcl", "rcr", "lea", "les",
                                                                        "lds", "xchg"};
                    if (op == "push" && is_reg16(a0)) {
                        ++pushed[a0];
                    } else if (op == "pop" && is_reg16(a0)) {
                        if (!pushed[a0]) out.insert(a0);   // (a pop of what it did not push: a value given)
                    } else if (kWriters.contains(op) && is_reg16(r16) &&
                               !((op == "and" || op == "or") && l.args.size() == 2 && text::lower(l.args[0]) == text::lower(l.args[1]))) {
                        out.insert(r16);
                        if (op == "xchg" && l.args.size() == 2) {
                            const std::string b = text::lower(l.args[1]);
                            const std::string b16 = is_reg8(b) ? std::string(reg8_parent(b)) : b;
                            if (is_reg16(b16)) out.insert(b16);
                        }
                    }
                    if (op == "lodsb" || op == "lodsw" || op == "cbw" || op == "lahf" || op == "in") out.insert("ax");
                    if (op == "cwd" || op == "mul" || op == "imul" || op == "div" || op == "idiv") out.insert({"ax", "dx"});
                    if (is_string_op(op)) {
                        const std::string head = op.substr(0, 4);
                        if (head == "lods") out.insert("si");
                        else if (head == "stos" || head == "scas") out.insert("di");
                        else out.insert({"si", "di"});
                        if (l.prefix) out.insert("cx");
                    }
                    if (op.starts_with("loop")) out.insert("cx");
                    else if (op == "int") out.insert("ax");
                    if (op == "call") {
                        const std::string t = target_of(l);
                        if (port.labels_.contains(t)) {
                            const std::set<std::string> inner = walk(t);
                            out.insert(inner.begin(), inner.end());
                        } else {
                            out.insert({"ax", "bx", "cx", "dx"});
                        }
                    }
                    if (is_jump(op)) {
                        const std::string t = target_of(l);
                        const auto target = port.labels_.find(t);
                        if (op == "jmp") next.clear();
                        if (target) next.push_back(*target);
                    }
                    if (op == "ret" || op == "retf" || op == "retn" || op == "iret") next.clear();
                }
                todo.insert(todo.end(), next.begin(), next.end());
            }
            port.writes_[lab] = out;
            return out;
        }
    };
    Walker w{*this, {}};
    return w.walk(label);
}

Rt Port::routine(std::size_t line) const {
    Rt best;
    std::size_t best_at = 0;
    for (const auto& [p, j] : labels_) {
        if (j <= line && (!best || j > best_at) && (cname.contains(p) || called_.contains(p))) {
            best = p;
            best_at = j;
        }
    }
    return best;
}

std::optional<State> Port::exit_of(const std::string& label) const {
    const auto a = exits_.find(label);
    const auto* ex = a != exits_.end() && !a->second.empty() ? &a->second : nullptr;   // (the round before's, whole; else this one's)
    if (!ex) {
        const auto b = exits_new_.find(label);
        if (b != exits_new_.end() && !b->second.empty()) ex = &b->second;
    }
    if (!ex) return std::nullopt;
    std::optional<State> s;
    for (const State& e : *ex) s = merge(s ? &*s : nullptr, e, first_members_);
    return s;
}

// ---- the whole analysis ---------------------------------------------------------------------------------------------

Port& Port::run() {
    prepare();
    called_.clear();
    for (const Line& l : code_)
        if (l.op && *l.op == "call") called_.insert(target_of(l));
    for (const std::string& t : table_targets()) called_.insert(t);
    for (std::size_t i = 0; i < code_.size(); ++i)
        if (code_[i].op && *code_[i].op == "retf")
            if (const Rt r = routine(i)) far.insert(r);
    for (int round = 0; round < 4; ++round) {   // (then again, knowing the slots that hold addresses)
        for (int pass = 0; pass < 4; ++pass) {  // (the callees' exits, and again: each round looks up the round before's, whole)
            exits_new_.clear();
            analyse();
            exits_ = exits_new_;
        }
        const std::set<std::pair<std::string, std::int64_t>> found = own_slots();
        if (found == own_ptr_) break;
        own_ptr_ = found;
    }
    find_params();
    return *this;
}

// {label: bytes} of the module's own data (its data segments' labels).
std::map<std::string, std::int64_t> Port::own_data() const {
    std::map<std::string, std::int64_t> out;
    std::optional<std::string> cur;
    for (const auto& [segment, l] : data_) {
        const auto& d = l.directive;
        const bool is_data = d && (d->kind == "DB" || d->kind == "DW" || d->kind == "DD" || d->kind == "DQ" || d->kind == "DT");
        if (is_data && d->name) {
            cur = *d->name;
            out[*cur] = 0;
        } else if (d && d->kind == "LABEL") {
            cur = *d->name;
            out[*cur] = 0;
        }
        if (is_data && cur) {
            std::int64_t count = 0;
            bool dup = false;
            {   // `n DUP (...)`
                std::string_view r = d->rest;
                std::size_t i = 0;
                while (i < r.size() && text::is_space(r[i])) ++i;
                std::size_t j = i;
                while (j < r.size() && std::isdigit(static_cast<unsigned char>(r[j]))) ++j;
                std::size_t k = j;
                while (k < r.size() && text::is_space(r[k])) ++k;
                if (j > i && k > j && k + 3 <= r.size() && text::iequals(r.substr(k, 3), "dup")) {
                    count = *text::parse_int<std::int64_t>(r.substr(i, j - i));
                    dup = true;
                }
            }
            if (!dup) count = static_cast<std::int64_t>(split_ops(strip_comment(d->rest)).size());
            const std::int64_t unit = d->kind == "DB" ? 1 : d->kind == "DW" ? 2 : d->kind == "DD" ? 4 : d->kind == "DQ" ? 8 : 10;
            out[*cur] += count * unit;
        }
    }
    return out;
}

// The words of the module's own data that are given an address (a whole register stored there): their (label, offset) --
// those alone, where no other access overlaps them.
std::set<std::pair<std::string, std::int64_t>> Port::own_slots() {
    const std::map<std::string, std::int64_t> own = own_data();
    std::map<std::string, std::set<std::pair<std::int64_t, std::int64_t>>> acc;
    std::set<std::pair<std::string, std::int64_t>> ptr;
    for (std::size_t i = 0; i < code_.size(); ++i) {
        const Line& l = code_[i];
        const std::optional<State>& st = before_[i];
        if (!st || !l.op) continue;
        const std::vector<Operand>& o = ops(i);
        for (std::size_t k = 0; k < o.size(); ++k) {
            if (o[k].kind == Operand::Kind::Off && own.contains(o[k].text)) acc[o[k].text].insert({o[k].disp, 0});
            if (o[k].kind != Operand::Kind::Mem || !o[k].mem.sym || !own.contains(*o[k].mem.sym) || !o[k].mem.regs.empty()) continue;
            const Mem& m = o[k].mem;
            const Operand* other = o.size() == 2 ? &o[1 - k] : nullptr;
            const std::int64_t size = m.size ? *m.size : (other && other->kind == Operand::Kind::Reg && is_reg8(other->text) ? 1 : 2);
            acc[*m.sym].insert({m.disp, size});
            if ((*l.op == "mov" || *l.op == "xchg") && k == 0 && other && other->kind == Operand::Kind::Reg && is_reg16(other->text)) {
                const K* kr = st->reg(other->text);
                if (kr && kr->wide() && kr->kind != Kind::Frame && size == 2) ptr.insert({*m.sym, m.disp});
            }
        }
    }
    std::set<std::pair<std::string, std::int64_t>> good;
    for (const auto& [sym_name, d] : ptr) {   // (none other reaches into the slot)
        bool alone = true;
        for (const auto& [o, sz] : acc[sym_name]) {
            const bool overlaps = o < d + 2 && d < o + std::max<std::int64_t>(sz, 1);
            if (overlaps && !((o == d && sz == 2) || (o == d && sz == 0))) alone = false;
        }
        if (alone && used_as_address(sym_name, d)) good.insert({sym_name, d});
    }
    return good;
}

// The evidence that a slot holds an address: what is read from it is then used as one (a register's base, the si or di of
// a string instruction), or it is compared with an address.
bool Port::used_as_address(const std::string& sym_name, std::int64_t d) {
    for (std::size_t i = 0; i < code_.size(); ++i) {
        const Line& l = code_[i];
        const std::optional<State>& st = before_[i];
        if (!st || !l.op) continue;
        const std::vector<Operand>& o = ops(i);
        std::optional<std::size_t> hit;
        for (std::size_t k = 0; k < o.size(); ++k)
            if (o[k].kind == Operand::Kind::Mem && o[k].mem.sym == sym_name && o[k].mem.disp == d && o[k].mem.regs.empty()) {
                hit = k;
                break;
            }
        if (!hit) continue;
        if ((*l.op == "cmp" || *l.op == "sub") && o.size() == 2 && o[1 - *hit].kind == Operand::Kind::Reg) {
            const Operand& r = o[1 - *hit];
            const K* k = st->reg(r.text);
            std::set<std::size_t> seen;
            if ((k && k->kind == Kind::Ptr) || (is_reg16(r.text) && address_use(i + 1, r.text, 3, seen))) return true;   // (compared with an address)
        }
        if (*l.op == "mov" && *hit == 1 && o[0].kind == Operand::Kind::Reg && is_reg16(o[0].text)) {
            std::set<std::size_t> seen;
            if (address_use(i + 1, o[0].text, 3, seen)) return true;
        }
    }
    return false;
}

// Whether register r, from line j0 on (into the routines called, depth levels), is used as an address before it is given
// another value.
bool Port::address_use(std::size_t j0, std::string_view r, int depth, std::set<std::size_t>& seen) {
    if (seen.contains(j0)) return false;
    seen.insert(j0);
    for (std::size_t j = j0; j < std::min(j0 + 60, code_.size()); ++j) {
        const Line& l2 = code_[j];
        if (!l2.op) continue;
        const std::string& op = *l2.op;
        for (const std::string& x : l2.args)
            if (word_in_brackets(x, r)) return true;   // (used as a base)
        if (is_string_op(op)) {
            const std::string head = op.substr(0, 4);
            if ((r == "si" && (head == "lods" || head == "movs" || head == "cmps")) ||
                (r == "di" && (head == "stos" || head == "movs" || head == "cmps" || head == "scas")))
                return true;   // (the string instruction's address)
        }
        if (op == "call" && depth) {
            const std::string t = target_of(l2);
            if (const auto target = labels_.find(t); target && address_use(*target, r, depth - 1, seen)) return true;
        }
        if (!l2.args.empty() && text::lower(l2.args[0]) == r && (op == "mov" || op == "lea" || op == "les" || op == "lds" || op == "xor"))
            return false;   // (given another value; a pop gives back the one pushed)
        if (op == "ret" || op == "retf" || op == "jmp") return false;
    }
    return false;
}

// An old offset in the module's own data: the new one (the slots that hold addresses 8 bytes now).
std::int64_t Port::own_place(const std::string& sym_name, std::int64_t disp) const {
    std::int64_t before_it = 0;
    for (const auto& [s2, d] : own_ptr_)
        if (s2 == sym_name && d < disp) ++before_it;
    return disp + 6 * before_it;
}

// {routine: [(old offset from the entry's sp, old size)]} of the arguments each reads, in order.
void Port::find_params() {
    std::map<Rt, std::map<std::int64_t, std::int64_t>> acc;
    for (std::size_t i = 0; i < code_.size(); ++i) {
        const Line& l = code_[i];
        const std::optional<State>& st = before_[i];
        if (!st || !l.op) continue;
        for (const Operand& o : ops(i)) {
            if (o.kind != Operand::Kind::Mem || o.mem.regs.size() != 1) continue;
            const K* k = st->reg(o.mem.regs[0]);
            if (!k || k->kind != Kind::Frame) continue;
            const std::int64_t e = k->old + o.mem.disp;
            const Rt r = k->owner ? k->owner->routine : routine(i);
            std::int64_t size = o.mem.size ? *o.mem.size : (*l.op == "les" || *l.op == "lds" ? 4 : 2);
            if (*l.op == "les" || *l.op == "lds") size = 4;
            std::int64_t& slot_size = acc[r][e];
            slot_size = std::max(slot_size, size);
        }
    }
    params_.clear();
    for (const auto& [r, d] : acc) {
        const std::int64_t rs = retsize(r);
        std::vector<Param> out;
        for (const auto& [e, size] : d) {
            if (e < rs) continue;
            if (!out.empty() && e < out.back().old_offset + out.back().size) continue;   // (the upper half of one read whole)
            out.push_back({e, size});
        }
        params_[r] = std::move(out);
    }
}

std::pair<std::size_t, std::int64_t> Port::slot(const Rt& r, std::int64_t e) const {
    const auto it = params_.find(r);
    if (it != params_.end())
        for (std::size_t k = 0; k < it->second.size(); ++k) {
            const Param& p = it->second[k];
            if (p.old_offset <= e && e < p.old_offset + p.size) return {k, e - p.old_offset};
        }
    throw Asm16Error(std::format("{}: no argument at sp+{}", r ? *r : std::string("(no routine)"), e));
}

}  // namespace cb::port::asm16
