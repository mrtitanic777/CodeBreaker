// decompile/c_text_flat.cpp -- a 32-bit module made compilable as a whole (x86 PE and PowerPC): the preamble every function's
// C is compiled after -- the types, the helpers, the globals as macros at their addresses, the declarations -- and each function
// written with what the module decides over all of its functions.
#include <algorithm>
#include <format>
#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "decompile/c_text_internal.hpp"
#include "decompile/conventions.hpp"
#include "translate/l2.hpp"

namespace cb::decompile::c_text {

namespace internal {

Json document_json(const FunctionNotes& notes) {
    if (notes.l2_exact_json) return json::parse(*notes.l2_exact_json);
    CB_ASSERT(notes.l2 != nullptr, "a function without an L2 document has none to give");
    return translate::l2::to_json(*notes.l2);
}

}  // namespace internal

namespace {

using internal::by_size;
using internal::document_json;
using internal::is_by_size_type;
using internal::ram_type;

// A name of the decompiler's for a value of a size C has no type for: `unkint5`, `uint6`, `xunknown7` ...
bool flat_is_odd_type_name(std::string_view name) {
    auto digits = [](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; }); };
    for (const std::string_view prefix : {"unkbyte", "unkint", "unkuint"})
        if (name.starts_with(prefix) && digits(name.substr(prefix.size()))) return true;
    for (const std::string_view prefix : {"int", "uint", "xunknown"}) {
        if (!name.starts_with(prefix) || name.size() != prefix.size() + 1) continue;
        const char c = name.back();
        if (c == '3' || c == '5' || c == '6' || c == '7') return true;
    }
    return false;
}

// Is `name;` (the whole word) in the text: a type the header declares already?
bool flat_declares_type(std::string_view types, std::string_view name) {
    const std::string needle = std::string(name) + ";";
    for (std::size_t at = types.find(needle); at != std::string_view::npos; at = types.find(needle, at + 1)) {
        const char before = at == 0 ? ' ' : types[at - 1];
        const bool word = (before >= '0' && before <= '9') || (before >= 'a' && before <= 'z') || (before >= 'A' && before <= 'Z') || before == '_';
        if (!word) return true;
    }
    return false;
}

// The helpers without the declarations of the library functions the module defines itself (a clash broke the whole header):
// `double <name>(...); ` for each name in `own`.
std::string flat_without_declared_libm(const std::string& helpers, const std::set<std::string>& own) {
    std::string out;
    std::size_t at = 0;
    while (at < helpers.size()) {
        const std::size_t found = helpers.find("double ", at);
        if (found == std::string::npos) break;
        out.append(helpers, at, found - at);
        std::size_t name_end = found + 7;
        while (name_end < helpers.size() && ((helpers[name_end] >= 'a' && helpers[name_end] <= 'z') || (helpers[name_end] >= '0' && helpers[name_end] <= '9')))
            ++name_end;
        const std::string name = helpers.substr(found + 7, name_end - found - 7);
        const auto libm = internal::libm_names();
        const std::size_t close = helpers.find(')', name_end);
        const bool declaration = std::find(libm.begin(), libm.end(), name) != libm.end() && name_end < helpers.size() && helpers[name_end] == '(' &&
                                 close != std::string::npos && close + 1 < helpers.size() && helpers[close + 1] == ';';
        if (declaration && own.contains(name)) {
            at = close + 2;
            if (at < helpers.size() && helpers[at] == ' ') ++at;   // (and the space after it)
        } else {
            out.append(helpers, found, 7);
            at = found + 7;
        }
    }
    out.append(helpers, std::min(at, helpers.size()), std::string::npos);
    return out;
}

std::string flat_trimmed(std::string_view s) { return std::string(text::trim(s)); }

std::string flat_name_of_alias(const std::string& name, std::int64_t width) { return name + "_" + std::to_string(width); }

bool flat_is_width(std::int64_t w) { return w == 1 || w == 2 || w == 4 || w == 8; }

}  // namespace

// ---- 32-bit ---------------------------------------------------------------------------------------------------

Compilable32 compilable32(Services& services, const std::filesystem::path& module, std::span<const Va> functions,
                          const std::filesystem::path& outdir) {
    const translate::native_l2::Library& library = services.library();
    Decompiled got = services.raw(module, functions, outdir);
    OrderedMap<std::string, std::string> out = got.texts;
    const std::vector<std::string>& order = got.order;
    std::map<std::string, std::string> imports = services.import_dlls(module);
    // imports by number, under the names the description gave them: declared and linked too
    for (const auto& [name, numbered] : got.numbered) imports.try_emplace(name, numbered.module);
    std::map<Va, std::set<std::int64_t>> sizes;
    std::set<std::string> in_ecx, in_edx;
    std::map<std::string, std::map<Va, std::set<std::int64_t>>> own;   // function -> address of a global -> bytes of its uses there
    std::vector<StructNote> struct_notes;                              // the structures the decompiler recovered (its facts)
    std::map<std::string, std::map<std::string, std::int64_t>> sizes_of_structs;   // function -> the sizes of those recovered up to it
    std::map<std::string, translate::native_l2::ModuleNames> named;   // function -> what its tree names
    std::map<std::string, std::string> heading_of_doc;                 // function -> its signature, from the tree before it is written
    auto notes_of = [&](const std::string& n) -> FunctionNotes* {
        const auto it = got.notes.find(n);
        return it == got.notes.end() ? nullptr : &it->second;
    };
    std::map<std::string, std::optional<std::int64_t>> takes_of;
    for (const std::string& n : order) {
        FunctionNotes* facts = notes_of(n);
        if (facts != nullptr) struct_notes.insert(struct_notes.end(), facts->structs.begin(), facts->structs.end());
        sizes_of_structs[n] = internal::struct_sizes(struct_notes);
        if (facts == nullptr) continue;
        for (const core16::GlobalNote& g : facts->globals) {
            sizes[g.address].insert(g.bytes);
            own[n][g.address].insert(g.bytes);
        }
        if (facts->l2 != nullptr) {
            const Json doc = document_json(*facts);
            named[n] = library.module_names32(doc);
            // (how many arguments each function's definition takes, of those not __cdecl: a call that passes another number is
            // made through a pointer)
            takes_of[n] = library.takes32(library.signature(doc));
        }
    }
    // What the module decides over all of its functions, from their trees before any is written: a global the code calls
    // through holds a function pointer, whatever else it is compared with.
    std::set<std::string> called_through, names, called;
    for (const auto& [function, x] : named) {
        called_through.insert(x.called_through.begin(), x.called_through.end());
        names.insert(x.names.begin(), x.names.end());
        called.insert(x.calls.begin(), x.calls.end());
    }
    std::set<std::string> underscored;
    for (const std::string& n : names)
        if (internal::is_underscored_ram_name(n)) underscored.insert(n);
    std::vector<std::string> macros;
    std::set<std::string> pointers;   // (a pointer-typed global compared with a number the decompiler cast to an integer is compared as the same integer)
    std::map<std::string, std::map<std::string, std::string>> aliases;
    std::set<std::pair<std::string, std::string>> ramnames;   // (sorted)
    for (const std::string& n : names)
        if (const auto ram = internal::parse_ram_name(n)) ramnames.insert(*ram);
    for (const auto& [pre, addr] : ramnames) {
        const Va a = *text::parse_int<Va>(addr, 16);
        std::set<std::int64_t> szs;
        if (const auto it = sizes.find(a); it != sizes.end()) szs = it->second;
        if (szs.empty()) szs.insert(4);
        const std::int64_t smallest = *szs.begin();
        std::int64_t widest = 0;
        bool any_le8 = false;
        for (const std::int64_t s : szs) {
            if (s <= 8) {
                widest = any_le8 ? std::max(widest, s) : s;
                any_le8 = true;
            }
        }
        const std::int64_t size = (szs.size() > 1 && (pre[0] == 'c' || pre[0] == 'b')) ? smallest : (any_le8 ? widest : 4);
        // `pc` is the decompiler's prefix for a pointer to char; a global is a code pointer when something calls through it
        const std::string key = pre + "Ram" + addr;
        std::string ty = called_through.contains(key) ? "code *" : pre == "pc" ? "char *" : (pre[0] == 'i' ? "int" + std::to_string(size) : by_size(size));
        if (pre[0] == 'p' && pre.size() > 1 && ty != "code *") ty = ram_type(pre, size, false);   // a pointer, by the decompiler's own naming (piRam: int *)
        const bool is_pointer = ty.ends_with("*");
        macros.push_back("#define " + key + " (*(" + ty + " *)0x" + addr + ")");
        if (is_pointer) pointers.insert(key);
        if (underscored.contains("_" + key)) {
            // (the core's name for the same place read at another width: `_uRam0040c000`)
            macros.push_back("#define _" + key + " (*(" + ty + " *)0x" + addr + ")");
            if (is_pointer) pointers.insert("_" + key);
        }
        // A function that uses the place at one width other than the declared one (a byte stored where other functions read a word)
        // uses it at its own width: the declared width would store, or read, the bytes beside it too.
        if (is_by_size_type(ty) || (ty.size() == 4 && ty.starts_with("int") && ty[3] >= '0' && ty[3] <= '9')) {
            for (const std::string& n : order) {
                const auto mine_of = own.find(n);
                if (mine_of == own.end() || !mine_of->second.contains(a)) continue;
                const std::set<std::int64_t>& mine = mine_of->second.at(a);
                if (mine.size() != 1) continue;
                const std::int64_t w = *mine.begin();
                if (w == size || !flat_is_width(w)) continue;
                const std::string alias = flat_name_of_alias(key, w);
                aliases[n].try_emplace(key, alias);
                const std::string macro = "#define " + alias + " (*(" + (pre[0] == 'i' ? "int" + std::to_string(w) : by_size(w)) + " *)0x" + addr + ")";
                if (std::find(macros.begin(), macros.end(), macro) == macros.end()) macros.push_back(macro);
            }
        }
    }
    std::map<std::string, std::int64_t> takes;
    for (const std::string& n : order)
        if (const auto it = takes_of.find(n); it != takes_of.end() && it->second) takes[n] = *it->second;

    std::map<std::string, std::string> declared;
    std::map<std::string, ModuleFacts> written_of;
    // (the facts every function is given alike: held once, not copied into each one's)
    auto shared = std::make_shared<FlatShared>();
    shared->functions = std::set<std::string>(order.begin(), order.end());
    shared->called_through = called_through;
    shared->pointer_globals = pointers;
    shared->takes = takes;
    const std::shared_ptr<const FlatShared> shared_facts = shared;
    for (const std::string& n : order) {
        FunctionNotes none;
        FunctionNotes& facts = notes_of(n) != nullptr ? *notes_of(n) : none;
        // (each function's C written from its L2 document here, with the module's facts: the back end's passes of the 32-bit path
        // and its part of the module's decisions)
        ModuleFacts written;
        written.path = ModuleFacts::Path::Flat32;
        written.shared = shared_facts;
        written.struct_sizes = sizes_of_structs[n];
        written.without_notes = true;
        if (const auto it = aliases.find(n); it != aliases.end()) written.aliases = it->second;
        if (facts.l2 != nullptr) out.insert_or_assign(n, services.c_from_l2(out.at(n), facts, std::nullopt, &written).value_or(out.at(n)));
        if (facts.l2 != nullptr && !facts.from_l2) {
            // (a function the back end declines -- a node it does not write -- is one the decompiler could not make, as one whose
            // decompile failed: no C, so no declaration, and the build stands something in for it)
            out.insert_or_assign(n, "/* " + n + ": its C could not be written from L2 */");
        }
        if (facts.from_l2 && written.declared) declared[n] = *written.declared;
        // (a comment the core put before the function -- "WARNING: Removing unreachable block" -- is not part of it: left in, it
        // became the function's declaration and broke the header. The back end leaves them out; one with no C has no text.)
        out.insert_or_assign(n, facts.from_l2 ? flat_trimmed(out.at(n)) : std::string());
        // every function returns a value in EAX whether or not the core saw it used; the compiler used here has no word for "first
        // argument in ECX": the function is declared plainly, and a #pragma in the header says where its first argument is
        if (written.registers && *written.registers == "ecx") in_ecx.insert(n);
        else if (written.registers && *written.registers == "edx") in_edx.insert(n);
        written_of[n] = std::move(written);
    }
    // The documents as the back end left them (its passes rewrite the tree): the names of the types they use, and the parameters.
    std::map<std::string, Json> documents;
    for (const auto& [n, text] : declared) documents.emplace(n, document_json(got.notes.at(n)));

    std::vector<std::string> used;
    {
        std::vector<std::string> wanted(called.begin(), called.end());   // (sorted)
        for (const std::string& n : wanted)
            if (imports.contains(n) && !out.contains(n)) used.push_back(n);
    }
    std::vector<std::string> decls;
    for (const std::string& n : used) {
        std::optional<std::int64_t> pop;
        if (const auto it = got.numbered.find(n); it != got.numbered.end()) pop = it->second.pop;
        else if (const auto bytes = services.argument_bytes(imports.at(n), n)) pop = *bytes;
        decls.push_back(std::string("int ") + conventions::name_of(pop && *pop == 0 ? conventions::Convention::Cdecl : conventions::Convention::Stdcall) + " " + n + "();");
    }
    for (const std::string& n : order) {   // each function, declared before any use
        const auto it = declared.find(n);
        std::string head = it == declared.end() ? std::string() : it->second;
        if (!head.ends_with(")")) continue;
        if (internal::has(head, "__cdecl")) {
            // The caller removes the arguments, so nothing in the binary fixes how many there are: the decompiler recovers each call
            // on its own evidence and callers need not agree with the definition. Declared with the list left open, each call
            // compiles as it was recovered. (A function that removes its own arguments -- the default here -- states their size in
            // its return, and keeps its full declaration.)
            head = head.substr(0, head.find('(')) + "()";
        }
        decls.push_back(head + ";");
        if (in_ecx.contains(n)) decls.push_back("#pragma aux " + n + " \"_*\" parm routine [ecx] value [eax] modify [eax ecx edx];");
        else if (in_edx.contains(n)) decls.push_back("#pragma aux " + n + " \"_*\" parm routine [ecx] [edx] value [eax] modify [eax ecx edx];");
    }
    // the core's names for values of sizes C has no type for (5 bytes of a 64-bit product, 10 of a floating-point register): each is
    // given the next type that holds it. One left undeclared in a function's heading broke the header, and with it every function of
    // the module.
    std::vector<std::string> odd;
    std::set<std::string> types;
    for (const auto& [n, doc] : documents) {
        const std::set<std::string> of_one = library.type_names(doc);
        types.insert(of_one.begin(), of_one.end());
    }
    for (const std::string& name : types) {
        if (!flat_is_odd_type_name(name) || flat_declares_type(internal::types_text(), name)) continue;
        const std::size_t digits_at = name.find_first_of("0123456789");
        const std::int64_t size = *text::parse_int<std::int64_t>(std::string_view(name).substr(digits_at), 10);
        const bool is_signed = name.starts_with("unkint") || name.starts_with("int");
        odd.push_back(std::string("typedef ") + (size > 8 ? "long double" : is_signed ? (size > 4 ? "long long" : "int") : (size > 4 ? "unsigned long long" : "unsigned int")) +
                      " " + name + ";");
    }
    // A module that is itself a C runtime (crtdll, msvcrt, mfc40, opengl32 ...) has functions of its own called sqrt, ldexp, floor: its
    // declarations are the ones that hold, and the helpers' library prototypes of the same names step aside.
    std::set<std::string> own_names;
    for (const std::string& d : decls) {
        const std::size_t paren = d.find('(');
        if (paren == std::string::npos) continue;
        const std::vector<std::string_view> words = text::split_ws(std::string_view(d).substr(0, paren));
        if (words.empty()) continue;
        std::string_view last = words.back();
        while (last.starts_with('*')) last.remove_prefix(1);
        own_names.emplace(last);
    }
    const auto libm = internal::libm_names();
    const bool clash = std::any_of(libm.begin(), libm.end(), [&](std::string_view l) { return own_names.contains(std::string(l)); });
    const std::string helpers = clash ? flat_without_declared_libm(internal::x86_helpers(), own_names) : internal::x86_helpers();
    const std::string odd_text = text::join(odd, "\n");
    const std::vector<std::string> structs = internal::struct_decls(struct_notes, std::string(internal::types_text()) + odd_text);
    std::string preamble = std::string(internal::types_text()) + odd_text + "\n" + internal::lines_of(structs) + std::string(internal::helpers_text()) + helpers +
                           text::join(decls, "\n") + "\n" + text::join(macros, "\n") + "\n";

    // `p->field` for `*(T *)(p + N)`: which accesses through a parameter are a structure's fields is the module's decision
    // (struct_text.plan, from the trees and the headings as written), and the functions it names are written again with theirs.
    std::set<std::string> declared_names;
    for (const auto& [n, text] : declared) declared_names.insert(n);
    std::map<std::uint64_t, std::string> by_address;
    for (const auto& [va, n] : got.by_va) by_address.emplace(static_cast<std::uint64_t>(va), n);
    struct_text::Analysis analysis;
    analysis.recover = [&](std::string_view) { return services.recover_structures(module); };
    analysis.parameter_names = [&](const std::string& fn) { return library.parameter_names(documents.at(fn)); };
    analysis.struct_accesses = [&](const std::string& fn, const std::string& param) {
        std::vector<struct_text::Access> accesses;
        for (const translate::native_l2::StructAccess& a : library.struct_accesses(documents.at(fn), param))
            accesses.push_back({a.base, a.stars.size(), a.offset});
        return accesses;
    };
    const struct_text::Plan planned = struct_text::plan(module.generic_string(), declared_names, by_address, declared, analysis);
    std::set<std::string> used_structs;
    std::map<std::string, std::string> final_declared = declared;   // (each heading as last written: with its fields)
    for (const std::string& n : order) {
        const auto it = planned.find(n);
        if (it == planned.end()) continue;
        ModuleFacts written = written_of.at(n);
        written.struct_fields = it->second;
        FunctionNotes& facts = got.notes.at(n);
        out.insert_or_assign(n, flat_trimmed(services.c_from_l2(out.at(n), facts, std::nullopt, &written).value_or(out.at(n))));
        used_structs.insert(written.structs_used.begin(), written.structs_used.end());
        if (written.declared && !written.declared->empty()) final_declared[n] = *written.declared;
    }
    if (!planned.empty()) preamble += struct_text::declarations(planned, used_structs);

    Compilable32 result;
    result.preamble = std::move(preamble);
    result.order = order;
    result.by_va = got.by_va;
    result.imports = used;
    result.in_ecx = std::move(in_ecx);
    result.in_edx = std::move(in_edx);
    for (const std::string& n : order)
        if (const FunctionNotes* f = notes_of(n); f != nullptr && f->l2 != nullptr) result.docs[n] = f->l2;
    for (const auto& [n, head] : final_declared)
        if (head.ends_with(")")) result.declarations[n] = head + ";";
    result.texts = std::move(out);
    result.arena = got.arena;
    return result;
}

// ---- PowerPC --------------------------------------------------------------------------------------------------

ModulePPC compilable_powerpc(Services& services, const std::filesystem::path& module, const std::filesystem::path& outdir) {
    const translate::native_l2::Library& library = services.library();
    describe::PowerPcFacts image = services.powerpc_facts(module);
    // (the L2 documents kept, and each function's C written from its own here, by the C back end with the PowerPC path's passes)
    std::vector<Va> starts;
    Decompiled got = services.raw(module, starts, outdir);
    OrderedMap<std::string, std::string> out = got.texts;
    const std::vector<std::string>& order = got.order;
    std::map<Va, std::set<std::int64_t>> sizes;
    std::set<std::string> novalue;
    std::map<std::string, translate::native_l2::ModuleNames> named;
    std::map<std::string, std::string> declared;
    const std::set<std::string> order_set(order.begin(), order.end());
    for (const std::string& n : order) {
        FunctionNotes none;
        FunctionNotes& facts = got.notes.contains(n) ? got.notes.at(n) : none;
        for (const core16::GlobalNote& g : facts.globals) sizes[g.address].insert(g.bytes);
        ModuleFacts written;
        written.path = ModuleFacts::Path::PowerPc;
        written.functions = order_set;
        if (facts.l2 != nullptr) out.insert_or_assign(n, services.c_from_l2(out.at(n), facts, std::nullopt, &written).value_or(out.at(n)));
        if (!facts.from_l2) {
            // (a function the back end declines -- a node it does not write -- is one the decompiler could not make, as one whose
            // decompile failed: no C, so no declaration, and the build leaves it to the original)
            if (facts.l2 != nullptr) out.insert_or_assign(n, "/* " + n + ": its C could not be written from L2 */");
            out.insert_or_assign(n, flat_trimmed(out.at(n)));
            continue;
        }
        named[n] = library.module_names32(document_json(facts));   // (what its tree names, as its text does)
        if (written.declared) declared[n] = *written.declared;
        out.insert_or_assign(n, flat_trimmed(out.at(n)));
        if (written.novalue) novalue.insert(n);   // (every function leaves something in r3 and callers are free to use it: a `void` result is uint8)
    }
    std::set<std::string> names, called_through, called;
    for (const auto& [function, x] : named) {
        names.insert(x.names.begin(), x.names.end());
        called_through.insert(x.called_through.begin(), x.called_through.end());
        called.insert(x.calls.begin(), x.calls.end());
    }
    std::vector<std::string> macros;
    std::set<std::pair<std::string, std::string>> ramnames;
    for (const std::string& n : names)
        if (const auto ram = internal::parse_ram_name(n)) ramnames.insert(*ram);
    for (const auto& [pre, addr] : ramnames) {
        const Va a = *text::parse_int<Va>(addr, 16);
        std::int64_t size = 4;
        bool seen = false;
        if (const auto it = sizes.find(a); it != sizes.end()) {
            for (const std::int64_t x : it->second) {
                if (x <= 8 && (!seen || x > size)) {
                    size = x;
                    seen = true;
                }
            }
        }
        macros.push_back("#define " + pre + "Ram" + addr + " (*(" + ram_type(pre, size, called_through.contains(pre + "Ram" + addr)) + " *)0x" + addr + ")");
    }
    // a call to a place in the original the decompiler was not told is a function: called there, in the original, which is in memory
    // beside the rebuilt image when it is judged
    Va lo_image = INT64_MAX, hi_image = INT64_MIN;
    for (const describe::PowerPcFacts::Span& s : image.spans) {
        lo_image = std::min(lo_image, s.va);
        hi_image = std::max(hi_image, s.va + static_cast<Va>(s.length));
    }
    std::set<std::string> func_addrs;
    for (const std::string& n : names) {
        if (n.size() != 15 || !n.starts_with("func_0x")) continue;
        const std::string digits = n.substr(7);
        if (std::all_of(digits.begin(), digits.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); })) func_addrs.insert(digits);
    }
    for (const std::string& addr : func_addrs) {
        const Va v = *text::parse_int<Va>(addr, 16);
        if (lo_image <= v && v < hi_image && !out.contains("func_0x" + addr)) macros.push_back("#define func_0x" + addr + " ((code *)0x" + addr + ")");
    }
    std::vector<std::string> used, decls;
    for (const std::string& n : called) {   // (`import<library>_ord_<n>`)
        const std::size_t ord = n.find("_ord_");
        if (!n.starts_with("import") || ord == std::string::npos || ord == 6) continue;
        const std::string_view library_number = std::string_view(n).substr(6, ord - 6), ordinal = std::string_view(n).substr(ord + 5);
        auto all_digits = [](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; }); };
        if (all_digits(library_number) && all_digits(ordinal)) used.push_back(n);
    }
    for (const std::string& n : used) decls.push_back("uint8 " + n + "();");
    for (const std::string& n : order) {
        const auto it = declared.find(n);
        if (it == declared.end() || !internal::has(it->second, "(")) continue;
        decls.push_back(it->second.substr(0, it->second.find('(')) + "();");
    }
    // The structures recovered for each function (its facts), declared before the prototypes: a function returning a pointer to one
    // names it in its prototype, which every body includes.
    const std::string types = text::replace_all(internal::types_text(), "typedef int code();", "typedef uint8 code();") + std::string(internal::helpers_text()) +
                              std::string(internal::powerpc_text());
    std::vector<StructNote> all_structs;
    for (const std::string& n : order)
        if (const auto it = got.notes.find(n); it != got.notes.end()) all_structs.insert(all_structs.end(), it->second.structs.begin(), it->second.structs.end());
    const std::vector<std::string> structs = internal::struct_decls(all_structs, types);
    const std::string header = types + internal::lines_of(structs) + text::join(decls, "\n") + "\n" + text::join(macros, "\n") + "\n";
    ModulePPC m;
    OrderedMap<std::uint64_t, std::string> by_address;
    for (const auto& [va, n] : got.by_va) by_address.insert_or_assign(static_cast<std::uint64_t>(va), n);
    m.made = translate::l2::Made::of(text::lower(module.filename().string()), header, out, order, by_address);
    m.novalue = std::move(novalue);
    std::map<std::string, std::int64_t> by_name;
    for (const auto& [address, name] : image.imports) by_name[name] = address;
    for (const std::string& n : used)
        if (const auto it = by_name.find(n); it != by_name.end()) m.externs[n] = it->second;
    for (const auto& [function, x] : named)
        for (const auto& [callee, arguments] : x.arity) m.calls[callee].insert(arguments);
    std::set<std::string> extern_names;
    for (const auto& [k, v] : m.externs) extern_names.insert(k);
    m.argc = call_counts(m.calls, extern_names);
    m.image = std::move(image);
    return m;
}

}  // namespace cb::decompile::c_text
