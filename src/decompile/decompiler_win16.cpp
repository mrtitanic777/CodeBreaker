// decompile/decompiler_win16.cpp -- a 16-bit Windows module decompiled in rounds, and what a 16-bit function's L2 document says.
//
// Function starts come first from what the file states (entry table, relocations, agreed near-call targets). Decompiling then
// shows calls to places not yet known as functions (`func_0x...`); those are added and the module is done again, until no new
// ones appear. Then the functions whose callers pass fewer arguments than they take are stated and done again, and the helpers
// that were made part of their callers get their own text.
#include <algorithm>

#include <nlohmann/json.hpp>

#include "build/watcom.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "decompile/decompiler_internal.hpp"
#include "translate/backends/c.hpp"
#include "translate/l2.hpp"

namespace cb::decompile::decompiler {

namespace ir = translate::ir;
using internal::hex8;
using internal::is_word_char;
using internal::upper_hex;

namespace {

// ---- L2 documents --------------------------------------------------------------------------------------------------------------

// The nodes of a function document's body, depth first.
std::vector<ir::Node*> body_nodes(ir::Node* doc) {
    std::vector<ir::Node*> out;
    const ir::MethodDecl* method = ir::get<ir::MethodDecl>(doc);
    if (method == nullptr) return out;
    for (ir::Node* statement : ir::statements(method->body))
        for (ir::Node* n : ir::walk(statement)) out.push_back(n);
    return out;
}

const std::string& symbol_name(const ir::SymP& sym) {
    static const std::string none;
    return sym ? sym->name : none;
}

// [(name, number of arguments)] of each call of a function by its name in an L2 document (`f(a, b)`).
std::vector<std::pair<std::string, std::int64_t>> win16_calls_of(ir::Node* doc) {
    std::vector<std::pair<std::string, std::int64_t>> out;
    for (ir::Node* n : body_nodes(doc)) {
        const ir::Call* call = ir::get<ir::Call>(n);
        if (call == nullptr) continue;
        const ir::Name* target = ir::get<ir::Name>(call->target);
        if (target != nullptr) out.emplace_back(symbol_name(target->sym), static_cast<std::int64_t>(call->args.size()));
    }
    return out;
}

// Every name an L2 document's body uses.
bool win16_names_include(ir::Node* doc, std::string_view name) {
    for (ir::Node* n : body_nodes(doc)) {
        const ir::Name* nm = ir::get<ir::Name>(n);
        if (nm != nullptr && symbol_name(nm->sym) == name) return true;
    }
    return false;
}

// The text after each place where `name(` stands as a call: the position of the character after the `(`.
std::vector<std::size_t> win16_call_sites(std::string_view text, std::string_view name) {
    std::vector<std::size_t> out;
    for (std::size_t at = text.find(name); at != std::string_view::npos; at = text.find(name, at + name.size())) {
        if (at != 0 && is_word_char(text[at - 1])) continue;
        std::size_t k = at + name.size();
        while (k < text.size() && text::is_space(text[k])) ++k;
        if (k < text.size() && text[k] == '(') out.push_back(k + 1);
    }
    return out;
}

// The text of a sequence of words joined by a space.
std::string win16_mods_text(ir::Node* doc) {
    const ir::MethodDecl* method = ir::get<ir::MethodDecl>(doc);
    return method == nullptr ? std::string() : text::join(method->mods, " ");
}

// The words (names and numbers) of a declaration, last first.
std::string win16_last_word(std::string_view declaration) {
    std::size_t end = declaration.size();
    while (end > 0 && !is_word_char(declaration[end - 1])) --end;
    std::size_t begin = end;
    while (begin > 0 && is_word_char(declaration[begin - 1])) --begin;
    return std::string(declaration.substr(begin, end - begin));
}

// The sizes of arguments declared so (`int2 a1`, `char *a2`): the analysis's own declarations, and a far pointer as its notes say
// -- `char *param_1` alone reads as a near one. A far pointer stated as one is `4p`: stated as a 4-byte number, 34:0146's string and
// 34:016A's copy were dereferenced as near pointers, into the data segment.
ArgumentSizes win16_argument_sizes(const std::vector<std::string>& params, const OrderedMap<std::string, std::int64_t>* pointers) {
    ArgumentSizes out;
    for (const std::string& p : params) {
        const std::string name = win16_last_word(p);
        const std::int64_t* bytes = pointers != nullptr ? pointers->find(name) : nullptr;
        const bool is_far = bytes != nullptr && *bytes == 4;
        const bool wide = internal::has_word(p, "int4") || internal::has_word(p, "uint4") || internal::has_word(p, "xunknown4") ||
                          internal::has_word(p, "float4") || internal::has_word(p, "undefined4");
        out.sizes.push_back(is_far || (p.find("__far") != std::string::npos && p.find('*') != std::string::npos) ? "4p" : wide ? "4" : "2");
    }
    return out;
}

}  // namespace

std::set<Va> placeholders(std::string_view text, ir::Node* doc) {
    std::set<Va> out;
    if (doc == nullptr) {   // (without a document: the calls of its text, `func_0x<8 hex> (`)
        for (const internal::Placeholder& p : internal::find_placeholders(text)) {
            std::size_t k = p.at + 7 + 8;
            while (k < text.size() && text::is_space(text[k])) ++k;
            if (k < text.size() && text[k] == '(') out.insert(*text::parse_int<Va>(p.digits, 16));
        }
        return out;
    }
    for (const auto& [name, arguments] : win16_calls_of(doc)) {
        const std::vector<internal::Placeholder> found = internal::find_placeholders(name);
        if (found.size() == 1 && found[0].at == 0 && name.size() == 15) out.insert(*text::parse_int<Va>(found[0].digits, 16));
    }
    return out;
}

bool calls(std::string_view text, ir::Node* doc, std::string_view name) {
    if (doc == nullptr) return !win16_call_sites(text, name).empty();
    for (const auto& [callee, arguments] : win16_calls_of(doc))
        if (callee == name) return true;
    return false;
}

bool mentions(std::string_view text, ir::Node* doc, std::string_view name) {
    if (doc != nullptr) return win16_names_include(doc, name);
    return text.find(name) != std::string_view::npos;
}

std::vector<std::int64_t> call_arguments(std::string_view text, std::string_view name, ir::Node* doc) {
    std::vector<std::int64_t> out;
    if (doc != nullptr) {
        for (const auto& [callee, arguments] : win16_calls_of(doc))
            if (callee == name) out.push_back(arguments);
        return out;
    }
    for (const std::size_t open : win16_call_sites(text, name)) {
        std::int64_t depth = 1, n = 0;
        bool seen = false;
        for (std::size_t k = open; k < text.size() && depth != 0; ++k) {
            const char ch = text[k];
            if (ch == '(' || ch == '[') ++depth;
            else if (ch == ')' || ch == ']') --depth;
            else if (ch == ',' && depth == 1) ++n;
            if (depth != 0 && !text::is_space(ch)) seen = true;
        }
        out.push_back(seen ? n + 1 : 0);
    }
    return out;
}

std::optional<ArgumentSizes> own_arguments(const translate::native_l2::Library& library, std::string_view text, std::string_view name,
                                           ir::Node* doc, const OrderedMap<std::string, std::int64_t>* pointers) {
    if (doc != nullptr) {
        std::vector<std::string> params;
        if (const ir::MethodDecl* method = ir::get<ir::MethodDecl>(doc)) {
            for (ir::Node* p : method->params) {
                const ir::Param* param = ir::get<ir::Param>(p);
                if (param == nullptr) throw InternalError("a parameter of a function document is not a Param");
                params.push_back(library.type_text(param->type ? translate::l2::type_to_json(param->type) : Json(nullptr), translate::backends::c::param_name(*param)));
            }
        }
        return win16_argument_sizes(params, pointers);
    }
    const std::string_view head = text.substr(0, text.find('{'));
    for (std::size_t at = head.find(name); at != std::string_view::npos; at = head.find(name, at + 1)) {
        if (at != 0 && is_word_char(head[at - 1])) continue;
        const std::size_t open = at + name.size();
        if (open >= head.size() || head[open] != '(') continue;
        const std::size_t close = head.find(')', open + 1);
        if (close == std::string_view::npos) continue;
        std::vector<std::string> params;
        for (const std::string_view p : text::split(head.substr(open + 1, close - open - 1), ',')) {
            const std::string_view trimmed = text::trim(p);
            if (!trimmed.empty() && trimmed != "void") params.emplace_back(trimmed);
        }
        return win16_argument_sizes(params, pointers);
    }
    return std::nullopt;
}

std::string by_their_names(std::string_view text, const std::map<std::string, std::string>& known) {
    std::string out;
    std::size_t from = 0;
    for (const internal::Placeholder& p : internal::find_placeholders(text)) {
        const auto name = known.find(p.digits);
        if (name == known.end()) continue;
        out.append(text.substr(from, p.at - from));
        out += name->second;
        from = p.at + 7 + 8;
    }
    out.append(text.substr(from));
    return out;
}

// ---- decompile16 ------------------------------------------------------------------------------------------------------------

namespace {

using Own = std::optional<ArgumentSizes>;

struct Win16Lost {
    Va va = 0;
    ArgumentSizes sizes;
};

ir::Node* win16_doc_of(const std::map<std::string, FunctionNotes>& notes, const std::string& name) {
    const auto it = notes.find(name);
    return it == notes.end() ? nullptr : it->second.l2;
}

const std::string& win16_text_of(const OrderedMap<std::string, std::string>& out, const std::string& name) {
    static const std::string none;
    const std::string* t = out.find(name);
    return t != nullptr ? *t : none;
}

// A cdecl function's own arguments' sizes, when they come to the bytes its instructions read through BP (describe.args_read);
// otherwise none -- its own text is wrong too (SETUPX's 25:0F96 once declared one word and read a far pointer). Its convention
// from its L2 document (the words before its name), or without one from its text.
Own win16_own_if_read(const translate::native_l2::Library& library, const OrderedMap<std::string, std::string>& out, Va va,
                      const std::string& name, const describe::NeDescription& d, const std::map<std::string, FunctionNotes>& notes) {
    const std::string& text = win16_text_of(out, name);
    ir::Node* doc = win16_doc_of(notes, name);
    const std::string head = doc != nullptr ? win16_mods_text(doc) : text.substr(0, text.find('{'));
    if (head.find("__cdecl16") == std::string::npos) return std::nullopt;
    const auto facts = notes.find(name);
    const Own own = own_arguments(library, text, name, doc, facts != notes.end() ? &facts->second.pointers : nullptr);
    if (!own || own->sizes.empty()) return std::nullopt;
    const describe::SegOff place = describe::where(va);
    const auto bytes = d.segment_bytes.find(place.seg);
    std::optional<std::int64_t> later;
    for (const auto& kv : d.names) {
        const describe::SegOff other = describe::where(kv.first);
        if (other.seg == place.seg && other.off > place.off) later = later ? std::min<std::int64_t>(*later, other.off) : other.off;
    }
    const std::int64_t length = bytes != d.segment_bytes.end() ? static_cast<std::int64_t>(bytes->second.size()) : 0;
    const describe::Distance kind = head.find("16far") != std::string::npos ? describe::Distance::Far : describe::Distance::Near;
    const std::optional<std::int64_t> read = bytes != d.segment_bytes.end()
                                                 ? describe::args_read(d.loaded(place.seg), place.off, later ? *later : length, kind)
                                                 : describe::args_read(describe::Code16(Bytes()), place.off, later ? *later : length, kind);
    std::int64_t sum = 0;
    for (const std::string& s : own->sizes) sum += *text::parse_int<std::int64_t>(text::trim(s, "p"), 10);
    if (!read || *read != sum) return std::nullopt;
    return own;
}

// The functions some call passes fewer arguments to than they themselves take (cdecl ones, whose own arguments are what they
// read: stated from a wrong text, the function was made worse).
std::vector<Win16Lost> win16_arguments_lost(const translate::native_l2::Library& library, const OrderedMap<std::string, std::string>& out,
                                            const std::map<Va, std::string>& by_va, const describe::NeDescription& d,
                                            const std::map<std::string, FunctionNotes>& notes) {
    std::vector<Win16Lost> lost;
    for (const auto& [va, name] : by_va) {
        const Own own = win16_own_if_read(library, out, va, name, d, notes);
        if (!own) continue;
        for (const auto& [other, t] : out) {
            ir::Node* doc = win16_doc_of(notes, other);
            if (other == name || !mentions(t, doc, name)) continue;
            bool fewer = false;
            for (const std::int64_t n : call_arguments(t, name, doc)) fewer = fewer || n < static_cast<std::int64_t>(own->sizes.size());
            if (fewer) {
                lost.push_back({va, *own});
                break;
            }
        }
    }
    return lost;
}

// The description's lines for these functions given their arguments' sizes (its 7th field, `4,2,2`: a far pointer one argument of
// four bytes, not two words -- as words, 34:0146's pointer was put back together as a near one, and SUWIN faulted in it).
void win16_state_arguments(describe::NeDescription& d, const std::vector<Win16Lost>& lost) {
    for (std::string& line : d.lines) {
        std::vector<std::string> parts;
        for (const std::string_view p : text::split(line, '\t')) parts.emplace_back(p);
        if (parts[0] != "F" || parts.size() < 5) continue;
        const std::optional<std::int64_t> va = text::parse_int<std::int64_t>(parts[1], 16);
        for (const Win16Lost& l : lost) {
            if (!va || l.va != *va) continue;
            while (parts.size() < 6) parts.push_back("0");
            parts.resize(6);
            parts.push_back(text::join(l.sizes.sizes, ","));
            line = text::join(parts, "\t");
            break;
        }
    }
    d.write();
}

std::map<std::string, std::string> win16_known_names(const std::map<Va, std::string>& names) {
    std::map<std::string, std::string> known;
    for (const auto& [va, name] : names) known[hex8(va)] = name;
    return known;
}

}  // namespace

// Decompile a 16-bit Windows module. Function starts come first from what the file states (entry table, relocations, agreed
// near-call targets). Decompiling then shows calls to places not yet known as functions (`func_0x...`); those are added and the
// module is done again, until no new ones appear.
c_text::Decompiled16 Decompiler::decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir_given) {
    return decompile16(module, outdir_given, 4);
}

c_text::Decompiled16 Decompiler::decompile16(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir_given, int rounds) {
    const std::string stem = text::lower(fs::to_text(module.filename()));
    const std::filesystem::path folder = outdir_given ? *outdir_given : scratch_for({"decompiler16", stem});
    auto d = std::make_shared<describe::NeDescription>(describe::describe16(formats::ne::Module::open_file(module), folder,
                                                                           context_.environment->knowledge(), describe::Policy::from_environment()));
    const RunOptions kept{.l2 = true, .write = false};
    Run got = run(d->path, {}, kept);
    OrderedMap<std::string, std::string> out = got.texts;
    std::map<Va, std::string> by_va = got.by_va;
    std::map<std::string, FunctionNotes> notes = got.notes;   // {name: its facts}
    const translate::native_l2::Library& lib = library();
    auto update = [&](const Run& more) {
        for (const auto& [name, text] : more.texts) out.insert_or_assign(name, text);
        for (const auto& [name, n] : more.notes) notes[name] = n;
        for (const auto& [va, name] : more.by_va) by_va[va] = name;
    };
    for (int round = 0; round < rounds; ++round) {
        std::set<Va> news;
        for (const auto& [name, text] : out)
            for (const Va va : placeholders(text, win16_doc_of(notes, name))) news.insert(va);
        for (auto it = news.begin(); it != news.end();) {
            if (d->names.contains(*it) || !d->inside(*it)) it = news.erase(it);
            else ++it;
        }
        if (news.empty()) break;
        for (const Va va : news) {
            const describe::SegOff place = describe::where(va);
            const std::string name = "sub_" + std::to_string(place.seg) + "_" + upper_hex(place.off, 4);
            d->names[va] = name;
            d->where_map[va] = place;
            d->lines.push_back(describe::model_line(*d, va, name));
        }
        d->write();
        // the new ones -- and again every function that calls one: it was made while the callee was unknown, a default near
        // function (a far call's pushed CS read as its first argument, the last one lost); now its prototype is known
        std::set<Va> both = news;
        for (const auto& [va, name] : by_va)
            for (const Va c : news)
                if (mentions(win16_text_of(out, name), win16_doc_of(notes, name), "func_0x" + hex8(c))) {
                    both.insert(va);
                    break;
                }
        update(run(d->path, std::vector<Va>(both.begin(), both.end()), kept));
    }
    // A function its callers clean up after (cdecl) has its arguments left to the analysis, which decides at each call on its own:
    // SETUPX's allocator 24:0F1E takes a size, and 11 of its 25 callers were made to pass none (sub_3_7E58's block got whatever
    // the stack held: Setup's SU99405). Where a call passes fewer than the function itself takes, the function's own argument
    // words are stated, and it and its callers are done again. (Stated for every such function, all 578 of SETUPX's texts
    // changed, their pointers made words, and new faults.)
    const std::vector<Win16Lost> lost = win16_arguments_lost(lib, out, by_va, *d, notes);
    if (!lost.empty()) {
        win16_state_arguments(*d, lost);
        std::set<std::string> lost_names;
        for (const Win16Lost& l : lost) lost_names.insert(by_va.at(l.va));
        std::set<Va> callers;
        for (const auto& [va, name] : by_va)
            for (const std::string& c : lost_names)
                if (c != name && calls(win16_text_of(out, name), win16_doc_of(notes, name), c)) {
                    callers.insert(va);
                    break;
                }
        std::set<Va> both = callers;
        for (const Win16Lost& l : lost) both.insert(l.va);
        const Run more = run(d->path, std::vector<Va>(both.begin(), both.end()), kept);
        // (and of what is done again, only the callers that passed too few are taken: the rest -- the stated functions themselves,
        // callers that passed all -- kept as they were. Taken whole, the analysis's second look made other mistakes: 25:0F96 lost
        // one of its own two arguments, 30:00F6 wrote through a pointer it had read right before)
        std::set<std::string> short_callers;
        for (const Va va : callers) {
            const std::string& n = by_va.at(va);
            bool any = false;
            for (const Win16Lost& l : lost) {
                const std::string& callee = by_va.at(l.va);
                if (callee == n) continue;
                for (const std::int64_t k : call_arguments(win16_text_of(out, n), callee, win16_doc_of(notes, n)))
                    if (k < static_cast<std::int64_t>(l.sizes.sizes.size())) any = true;
            }
            if (any) short_callers.insert(n);
        }
        // (and one whose own arguments the second look changed is not taken either: 25:0F96, two words become one, faulted -- its
        // calls passed in full, its own frame read wrong)
        std::vector<std::string> taken;
        for (const auto& [n, t] : more.texts) {
            if (!short_callers.contains(n)) continue;
            const auto mf = more.notes.find(n);
            const auto of = notes.find(n);
            const Own now = own_arguments(lib, t, n, mf != more.notes.end() ? mf->second.l2 : nullptr, mf != more.notes.end() ? &mf->second.pointers : nullptr);
            const Own was = own_arguments(lib, win16_text_of(out, n), n, win16_doc_of(notes, n), of != notes.end() ? &of->second.pointers : nullptr);
            if (now == was) taken.push_back(n);
        }
        for (const std::string& n : taken) {
            out.insert_or_assign(n, more.texts.at(n));
            notes[n] = more.notes.at(n);
        }
    }
    // a helper made part of its callers (describe.register_helper: `inline`) is still a function of the module, at its place, for
    // whatever else it reaches: its own text, from the description with it as an ordinary function
    if (!d->inlined.empty()) {
        describe::Description own;
        own.path = d->path.parent_path() / "description_own.txt";
        for (const std::string& line : d->lines) {
            std::vector<std::string> p;
            for (const std::string_view f : text::split(line, '\t')) p.emplace_back(f);
            std::string made = line;
            if (p[0] == "F" && p.size() > 4 && text::trim(p[4]) == "inline") {
                const Va va = *text::parse_int<Va>(p[1], 16);
                if (d->inlined.contains(va)) made = describe::model_line(*d, va, p[2]);
            }
            own.lines.push_back(std::move(made));
        }
        own.write();
        update(run(own.path, std::vector<Va>(d->inlined.begin(), d->inlined.end()), kept));
    }
    // A function found only because another called it was decompiled after its callers, whose text still calls it `func_0x<address>`
    // -- a name nothing has. The callers are given its name.
    const std::map<std::string, std::string> known = win16_known_names(d->names);
    if (!known.empty()) {
        OrderedMap<std::string, std::string> renamed;
        for (const auto& [name, text] : out) renamed.insert_or_assign(name, by_their_names(text, known));
        out = std::move(renamed);
    }
    for (auto& [name, facts] : notes) {   // (its L2 document's calls by those names too)
        if (facts.l2 == nullptr || known.empty()) continue;
        const std::string dumped = json::dump(translate::l2::to_json(*facts.l2), json::Style::Compact);
        const std::string changed = by_their_names(dumped, known);
        if (changed == dumped) continue;
        std::lock_guard<std::mutex> hold(arena_lock_);
        facts.l2 = translate::l2::from_json(*arena_, json::parse(changed));
    }
    d->notes.clear();
    for (const auto& [name, text] : out)
        if (const auto n = notes.find(name); n != notes.end()) d->notes[name] = n->second;
    c_text::Decompiled16 result;
    result.texts = std::move(out);
    for (const auto& [va, name] : by_va) result.order.push_back(name);
    result.by_va = std::move(by_va);
    result.description = d;
    result.arena = arena_;
    return result;
}

}  // namespace cb::decompile::decompiler
