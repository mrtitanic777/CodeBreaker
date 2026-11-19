// cb/decompile/decompiler_lanes.cpp -- the decompiler's lanes: a 32-bit module (PE) and a PowerPC image through the description of the
// module, a virtual device driver, a DOS program, a NES cartridge; any executable as the text of one .c file; and the command.
#include <algorithm>
#include <cctype>

#include <nlohmann/json.hpp>

#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/c_text.hpp"
#include "cb/decompile/c_text_dos.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/decompiler_internal.hpp"
#include "cb/decompile/m6502.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/pe_functions.hpp"
#include "cb/symbols/c_library.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "cb/translate/l2.hpp"

namespace cb::decompile::decompiler {

using internal::hex;
using internal::hex8;
using internal::upper_hex;

namespace {

constexpr std::uint16_t kMachineX86 = 0x014C;
constexpr std::uint16_t kMachinePowerPc = 0x01F2;

// The functions the carve of an x86 module finds, by address.
std::vector<Va> lanes_carved_functions(const std::filesystem::path& module) {
    const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module);
    std::vector<Va> out;
    for (const auto& kv : carve.functions()) out.push_back(kv.first);
    return out;
}

// The 6502 as a function-finder reads it.
formats::Decoder lanes_decoder_6502() {
    return [](ByteView memory, std::uint32_t pc) -> std::optional<formats::DecodedInsn> {
        const std::optional<m6502::Insn> insn = m6502::decode(memory, pc, pc);
        if (!insn) return std::nullopt;
        formats::DecodedInsn out;
        out.size = insn->size;
        if (insn->target) out.target = *insn->target;
        if (insn->mnemonic == "JSR") out.flow = formats::Flow::Call;
        else if (insn->mnemonic == "JMP") out.flow = out.target ? formats::Flow::Jump : formats::Flow::Indirect;
        else if (insn->mnemonic == "RTS" || insn->mnemonic == "RTI") out.flow = formats::Flow::Return;
        else if (insn->mnemonic == "BRK") out.flow = formats::Flow::Halt;
        else out.flow = out.target ? formats::Flow::Branch : formats::Flow::Next;
        return out;
    };
}

// Decompile a description; then, while the output calls places not yet known as functions (`func_0x...`), add those inside the
// module's code and decompile them too.
template <class Described, class LineFor>
Functions lanes_grow(Decompiler& decompiler, Described& d, const LineFor& line_for, int rounds, std::span<const Va> only = {}) {
    Run first = decompiler.run(d.path, only);
    OrderedMap<std::string, std::string> out = first.texts;
    std::map<Va, std::string> by_va = first.by_va;
    std::map<std::string, core16::StructNote> structures;
    const auto take_structures = [&](const Run& r) {
        for (const auto& [name, notes] : r.notes)
            for (const core16::StructNote& s : notes.structs) structures.insert_or_assign(s.name, s);
    };
    take_structures(first);
    for (int round = 0; round < rounds; ++round) {
        std::set<Va> news;
        for (const auto& [name, text] : out)
            for (const Va va : placeholders(text, nullptr)) news.insert(va);
        for (auto it = news.begin(); it != news.end();) {
            bool inside = false;
            for (const auto& [object, span] : d.spans) inside = inside || (span.first <= *it && *it < span.first + span.second);
            if (d.names.contains(*it) || !inside) it = news.erase(it);
            else ++it;
        }
        if (news.empty()) break;
        for (const Va va : news) {
            const auto [name, line] = line_for(va);
            d.names[va] = name;
            d.lines.push_back(line);
        }
        d.write();
        std::set<Va> both = news;   // (made while these were unknown)
        for (const auto& [va, name] : by_va) {
            const std::string* text = out.find(name);
            for (const Va c : news)
                if (text != nullptr && text->find("func_0x" + hex8(c)) != std::string::npos) {
                    both.insert(va);
                    break;
                }
        }
        const Run more = decompiler.run(d.path, std::vector<Va>(both.begin(), both.end()));
        for (const auto& [name, text] : more.texts) out.insert_or_assign(name, text);
        for (const auto& [va, name] : more.by_va) by_va[va] = name;
        take_structures(more);
    }
    std::map<std::string, std::string> known;
    for (const auto& [va, name] : d.names) known[hex8(va)] = name;
    Functions result;
    for (const auto& [name, text] : out) result.texts.insert_or_assign(name, known.empty() ? text : by_their_names(text, known));
    for (const auto& [va, name] : by_va) result.order.push_back(name);
    result.by_va = std::move(by_va);
    result.structures = std::move(structures);
    return result;
}

}  // namespace

// A character of a source file's name in a version string (`lex.c`, `p0scanner.c`, `grammar.y`).
bool lanes_file_char(std::uint8_t c) { return std::isalnum(c) != 0 || c == '_' || c == '.' || c == '-'; }

// What a real-mode program has of the compiler's library: the functions that are not the program's, and what goes at the top of its C.
struct DosLibrary {
    std::set<Va> skipped;                 // the library's functions: described, so callers call them by name, but not decompiled
    std::vector<LibraryFunction> found;
    std::string header;
    std::map<std::string, std::vector<std::string>> pointers;   // name -> the C type of each pointer argument (c_text_dos::Layout)
    std::set<std::string> nothing;                              // the names whose header says they return nothing (void)
};

// Match the program's functions against the library signatures. A function that matches is named as the library names it, given the
// prototype its header declares (how many words each call passes, what it returns), and left out of what is decompiled: nobody wrote
// `strcpy` for this program. The description is rewritten to say so.
DosLibrary lanes_dos_library(Decompiler& decompiler, describe::DosDescription& d, const std::filesystem::path& checkout) {
    DosLibrary out;
    std::vector<Va> starts;
    for (const auto& [va, fn] : d.analysis->functions) starts.push_back(va);
    const std::map<Va, LibraryFunction> identified = decompiler.context().environment->dos_library(d.program, starts);
    if (identified.empty()) return out;
    symbols::c_library::CLibrary prototypes;
    try {
        prototypes = symbols::c_library::CLibrary::load(checkout / "data" / "symbols" / "c_library.json");
    } catch (const Error&) {   // (no table: the functions are named, their arguments left to the analysis)
    }
    std::set<std::string> used;
    std::map<std::string, std::vector<std::string>> by_library;   // library -> the names with no header of their own
    std::set<std::string> headers;
    for (const auto& [va, what] : identified) {
        std::string name = text::sanitize_identifier(what.name);
        if (name.empty() || !used.insert(name).second) name = (name.empty() ? "library" : name) + "_" + upper_hex(static_cast<std::int64_t>(va - d.base), 5), used.insert(name);
        const analyze::dos_functions::Function& fn = d.analysis->functions.at(static_cast<std::uint32_t>(va));
        const symbols::c_library::Prototype* p = prototypes.find(what.name);
        // a large or compact model's data pointers are far
        const std::uint32_t pointer_bytes = what.library.find("large") != std::string::npos || what.library.find("compact") != std::string::npos ? 4 : 2;
        std::string line = "F\t" + hex(va) + "\t" + name + "\t0\t__cdecl16" + std::string(analyze::dos_functions::name_of(fn.distance));
        if (p != nullptr) {
            headers.insert(p->header);
            const std::uint32_t result = p->result_bytes(pointer_bytes);
            line += "\t" + (result == 2 || result == 4 ? std::to_string(result) : std::string());
            const std::vector<std::string> sizes = p->argument_sizes(pointer_bytes);
            if (!sizes.empty()) line += "\t" + text::join(sizes, ",");
            if (!p->pointers.empty()) out.pointers[name] = p->pointers;
            if (p->result == 'v') out.nothing.insert(name);
        } else {
            by_library[what.library].push_back(name);
        }
        for (std::string& existing : d.lines) {
            if (!existing.starts_with("F\t" + hex(va) + "\t")) continue;
            existing = line;
            break;
        }
        d.names[va] = name;
        out.skipped.insert(va);
        out.found.push_back({name, what.library});
    }
    d.write();
    for (const std::string& h : headers) out.header += "#include <" + h + ">\n";
    // the compiler the program was linked with: the one most of its library functions are from. A function only another
    // compiler's library has a row for is that code -- said so, not claimed as that compiler's
    std::map<std::string, std::size_t> by_compiler;
    for (const LibraryFunction& f : out.found) ++by_compiler[f.library.substr(0, f.library.find(':'))];
    std::string linked;
    std::size_t most = 0;
    for (const auto& [compiler, n] : by_compiler)
        if (n > most) linked = compiler, most = n;
    for (const auto& [library, names] : by_library) {
        const bool own = library.substr(0, library.find(':')) == linked;
        out.header += (own ? "\n/* from " : "\n/* the same code as in ") + library + ", called by name: " + text::join(names, ", ") + " */";
    }
    return out;
}

// The two functions Microsoft C's start-up code calls around the library's own: its CRT0.ASM is
//     call _cinit / call _setargv / call _setenvp / ... / call main / ... / call exit
// so in a program whose start-up calls _setargv and _setenvp, the function it calls just before them is _cinit (the library's,
// not decompiled) and the first it calls after them is main.
void lanes_dos_startup(describe::DosDescription& d, DosLibrary& library) {
    const analyze::dos_functions::Analysis& a = *d.analysis;
    const auto entry = std::find_if(a.functions.begin(), a.functions.end(),
                                    [](const auto& f) { return f.second.evidence == analyze::dos_functions::Evidence::Entry; });
    if (entry == a.functions.end()) return;
    const auto next = std::next(entry);
    const Va start = entry->first, end = next == a.functions.end() ? d.base + static_cast<Va>(d.image.size()) : static_cast<Va>(next->first);
    const ByteView image(d.image);
    std::vector<Va> calls;   // the start-up code's call targets, in order
    for (Va va = start; va + 3 <= end; ++va) {
        const std::size_t off = static_cast<std::size_t>(va - d.base);
        if (off + 5 > image.size() || off >= a.bytes.size() || a.bytes[off] != analyze::dos_functions::ByteClass::Code) continue;
        if (image[off] == 0xE8) {
            const auto rel = static_cast<std::int16_t>(image[off + 1] | (image[off + 2] << 8));
            const Va cs = static_cast<Va>(entry->second.cs);
            calls.push_back(cs + ((va - cs + 3 + rel) & 0xFFFF));
        } else if (image[off] == 0x9A) {
            calls.push_back(static_cast<Va>(image[off + 3] | (image[off + 4] << 8)) * 16 + (image[off + 1] | (image[off + 2] << 8)));
        }
    }
    auto name_of = [&](Va va) -> std::string {
        const auto n = d.names.find(va);
        return n == d.names.end() ? std::string() : n->second;
    };
    const auto argv = std::find_if(calls.begin(), calls.end(), [&](Va va) { return name_of(va) == "_setargv"; });
    if (argv == calls.end() || std::next(argv) == calls.end() || name_of(*std::next(argv)) != "_setenvp") return;
    auto rename = [&](Va va, const std::string& name) {
        if (!a.functions.contains(static_cast<std::uint32_t>(va)) || !name_of(va).starts_with("sub_")) return false;
        for (std::string& line : d.lines)
            if (line.starts_with("F\t" + hex(va) + "\t")) {
                std::vector<std::string> f;
                for (const std::string_view p : text::split(line, '\t')) f.emplace_back(p);
                f[2] = name;
                line = text::join(f, "\t");
            }
        d.names[va] = name;
        return true;
    };
    if (argv != calls.begin() && rename(*std::prev(argv), "_cinit")) {
        library.skipped.insert(*std::prev(argv));
        library.found.push_back({"_cinit", "Microsoft C start-up code (CRT0DAT.ASM)"});
    }
    // main (int argc, char **argv, char **envp): an int and two data pointers -- far ones in the compact and large models
    bool far_data = false;
    for (const LibraryFunction& f : library.found) far_data = far_data || f.library.find("large-model") != std::string::npos || f.library.find("compact-model") != std::string::npos;
    for (auto it = std::next(argv, 2); it != calls.end(); ++it)
        if (name_of(*it).starts_with("sub_")) {
            if (rename(*it, "main"))
                for (std::string& line : d.lines)
                    if (line.starts_with("F\t" + hex(*it) + "\t")) {
                        const std::vector<std::string_view> f = text::split(line, '\t');
                        if (f.size() >= 5)
                            line = std::format("F\t{}\tmain\t0\t{}\t2\t{}", f[1], f[4], far_data ? "2,4p,4p" : "2,2,2");
                    }
            break;
        }
    d.write();
}

// The words of arguments each of a real-mode program's own functions is called with, as its callers remove them: Microsoft C's
// cdecl caller pushes the arguments and takes them off again right after the call (`add sp, N`, or `pop cx` for one word, two
// for two), so the instruction after a call says how many words that call passed. A function's description then states that
// many (the most any caller passes), and every call of it is written with them -- the decompiler reading a call's pushes on its
// own misses those pushed before a branch, and writes one far pointer as one argument here and as two words there.
// Whether the instruction ending at `off` is a push: `push reg` / `push sreg`, `push imm8`, `push imm16`, `push word [...]`
// (FF /6, by its addressing form's length). Read backwards, so a guess -- it only ever turns a count into "not known".
bool lanes_push_before(ByteView image, std::size_t off) {
    auto at = [&](std::size_t back) -> int { return off >= back ? image[off - back] : -1; };
    const int b1 = at(1);
    if ((b1 >= 0x50 && b1 <= 0x57) || b1 == 0x06 || b1 == 0x0E || b1 == 0x16 || b1 == 0x1E) return true;
    if (at(2) == 0x6A || at(3) == 0x68) return true;
    auto push_modrm = [](int m, int mod) { return m >= 0 && (m & 0x38) == 0x30 && (m >> 6) == mod; };
    if (at(2) == 0xFF && push_modrm(at(1), 0) && (at(1) & 7) != 6) return true;            // push word [bx]
    if (at(3) == 0xFF && push_modrm(at(2), 1)) return true;                                // push word [bp+d8]
    if (at(4) == 0xFF && (push_modrm(at(3), 2) || (push_modrm(at(3), 0) && (at(3) & 7) == 6))) return true;   // [bp+d16], [a16]
    return false;
}

void lanes_dos_arguments(describe::DosDescription& d, const std::set<Va>& library) {
    const analyze::dos_functions::Analysis& a = *d.analysis;
    const ByteView image(d.image);
    std::map<Va, std::pair<std::size_t, std::size_t>> words;   // the fewest and the most words a call of it removes
    auto cs_of = [&](Va linear) -> std::optional<Va> {
        auto it = a.functions.upper_bound(static_cast<std::uint32_t>(linear));
        if (it == a.functions.begin()) return std::nullopt;
        return static_cast<Va>((--it)->second.cs);
    };
    for (std::size_t off = 0; off + 3 <= image.size() && off < a.bytes.size(); ++off) {
        if (a.bytes[off] != analyze::dos_functions::ByteClass::Code) continue;
        const Va here = d.base + static_cast<Va>(off);
        std::optional<Va> target;
        std::size_t length = 0;
        if (image[off] == 0xE8) {   // call near rel16, in the segment it runs in
            const std::optional<Va> cs = cs_of(here);
            if (!cs) continue;
            const auto rel = static_cast<std::int16_t>(image[off + 1] | (image[off + 2] << 8));
            target = *cs + ((here - *cs + 3 + rel) & 0xFFFF);
            length = 3;
        } else if (image[off] == 0x9A && off + 5 <= image.size()) {   // call far ptr16:16 (the reader has relocated its segment: linear)
            const std::uint32_t o = image[off + 1] | (image[off + 2] << 8);
            const std::uint32_t s = image[off + 3] | (image[off + 4] << 8);
            target = static_cast<Va>(s) * 16 + o;
            length = 5;
        }
        if (!target || !a.functions.contains(static_cast<std::uint32_t>(*target)) || library.contains(*target)) continue;
        const std::size_t next = off + length;
        std::size_t passed = 0;
        if (next + 3 <= image.size() && image[next] == 0x83 && image[next + 1] == 0xC4 && image[next + 2] < 0x80) passed = image[next + 2] / 2;
        else if (next + 4 <= image.size() && image[next] == 0x81 && image[next + 1] == 0xC4) passed = (image[next + 2] | (image[next + 3] << 8)) / 2;
        else if (next < image.size() && image[next] == 0x59) passed = next + 1 < image.size() && image[next + 1] == 0x59 ? 2 : 1;   // pop cx
        // (none removed right after it, but a push right before it: Microsoft C left this call's words for a later `add sp`
        // to remove with the next call's -- the call says nothing of how many it passed)
        if (passed == 0 && lanes_push_before(image, off)) continue;
        const auto [w, first] = words.try_emplace(*target, passed, passed);
        if (!first) w->second = {std::min(w->second.first, passed), std::max(w->second.second, passed)};
    }
    for (std::string& line : d.lines) {
        if (!line.starts_with("F\t")) continue;
        const std::vector<std::string_view> f = text::split(line, '\t');
        if (f.size() != 5 || (f[3] != "0" && f[3] != "-1")) continue;   // (a function whose return removes its arguments says so already)
        const std::optional<std::int64_t> va = text::parse_int<std::int64_t>(f[1], 16);
        const auto w = va ? words.find(*va) : words.end();
        if (w == words.end() || w->second.second == 0) continue;   // (no words: nothing to say -- an empty list is "not known")
        // its callers remove the arguments, so its return removes none (the decompiler states the sizes of such a function only);
        // callers that disagree (an error reporter given a message number and what the message names) pass the words all of
        // them pass and then each its own: `...` (the most any passes, given to all, has the others pass words they never pushed)
        std::vector<std::string> sizes(w->second.first, "2");
        if (w->second.first != w->second.second) sizes.emplace_back("...");
        line = std::format("F\t{}\t{}\t0\t{}\t\t{}", f[1], f[2], f[4], text::join(sizes, ","));
    }
    d.write();
}

// What a real-mode program says of its source files (c_text_dos::Layout): the version string each file left in the data
// segment (`@(#)lex.c:1.9`), at its offset in that segment, and the code segment each function is in.
c_text_dos::Layout lanes_dos_layout(const describe::DosDescription& d) {
    c_text_dos::Layout layout;
    const ByteView image(d.image);
    // the data segment: what the program's own code loads DS with (`mov ax, DGROUP` / `mov ds, ax`, as Microsoft C's start-up
    // code and every far function that needs its data begin), the segment most often loaded so
    std::map<std::uint32_t, std::size_t> loaded;
    for (std::size_t at = 0; at + 5 <= image.size(); ++at)
        if (image[at] == 0xB8 && image[at + 3] == 0x8E && image[at + 4] == 0xD8) ++loaded[image[at + 1] | (image[at + 2] << 8)];
    std::optional<Va> ds;
    std::size_t most = 0;
    for (const auto& [segment, n] : loaded) {
        const Va linear = static_cast<Va>(segment) * 16;
        if (n > most && linear >= d.base && linear < d.base + static_cast<Va>(image.size())) ds = linear, most = n;
    }
    if (!ds && d.program.ds) ds = static_cast<Va>(*d.program.ds) * 16;
    if (!ds) return layout;
    if (*ds >= d.base && *ds < d.base + static_cast<Va>(image.size())) {
        const std::size_t from = static_cast<std::size_t>(*ds - d.base);
        layout.data_segment = std::format("0x{:x}", *ds / 16);
        layout.data.assign(reinterpret_cast<const char*>(image.data() + from), std::min<std::size_t>(0x10000, image.size() - from));
    }
    for (std::size_t at = 0; at + 6 < image.size(); ++at) {
        if (image[at] != '@' || image[at + 1] != '(' || image[at + 2] != '#' || image[at + 3] != ')') continue;
        std::size_t e = at + 4;
        while (e < image.size() && (lanes_file_char(image[e]))) ++e;
        const std::string file(reinterpret_cast<const char*>(image.data() + at + 4), e - at - 4);
        const Va linear = d.base + static_cast<Va>(at);
        if (e < image.size() && image[e] == ':' && file.find('.') != std::string::npos && linear >= *ds && linear - *ds < 0x10000)
            layout.files.emplace_back(static_cast<std::uint32_t>(linear - *ds), file);
        at = e;
    }
    for (const auto& [va, fn] : d.analysis->functions) {
        const auto name = d.names.find(va);
        if (name != d.names.end()) layout.segment_of[name->second] = fn.cs;
    }
    return layout;
}

// ---- a PE module, 32-bit or PowerPC --------------------------------------------------------------------------------------------


// A PE module described to the native program. A PowerPC image says where its bytes sit, which stubs are imports and which runs are
// part of their callers.
describe::PeDescription Decompiler::describe_module(const std::filesystem::path& module, const formats::pe::Image& image, std::span<const Va> starts,
                                                    const std::filesystem::path& outdir) {
    describe::PeDescription description;
    if (image.header().machine != kMachinePowerPc) {
        description = describe::describe32(module, image, starts, outdir, context_.environment->knowledge());
    } else {
        describe::PowerPcFacts ppc = context_.environment->powerpc_facts(module);
        std::map<Va, std::string> names;
        for (const Va va : starts)
            if (const auto it = ppc.names.find(va); it != ppc.names.end()) names[va] = it->second;
        ppc.names = std::move(names);
        description = describe::describe32(module, image, starts, outdir, context_.environment->knowledge(), &ppc);
    }
    context_.environment->described(module, description);
    return description;
}

// The native program's own output for a PE module, before it is made compilable: the module described, the program run on it.
c_text::Decompiled Decompiler::raw(const std::filesystem::path& module, std::span<const Va> functions, const std::filesystem::path& outdir) {
    const std::uint16_t machine = formats::pe::Image::open(module).header().machine;
    std::vector<Va> starts(functions.begin(), functions.end());
    if (starts.empty() && machine != kMachineX86)   // (a PowerPC image states its starts)
        starts = machine == kMachinePowerPc ? context_.environment->powerpc_functions(module) : lanes_carved_functions(module);
    ModuleRun r = run_module(module, starts, outdir, RunOptions{.l2 = true, .write = false});
    c_text::Decompiled out;
    out.arena = arena_;
    out.texts = std::move(r.run.texts);
    out.order = std::move(r.run.order);
    out.by_va = std::move(r.run.by_va);
    out.notes = std::move(r.run.notes);
    out.numbered = std::move(r.numbered);
    return out;
}

Decompiler::ModuleRun Decompiler::run_module(const std::filesystem::path& module, std::span<const Va> starts, const std::filesystem::path& outdir,
                                             const RunOptions& options) {
    const formats::pe::Image image = formats::pe::Image::open(module);
    describe::PeDescription description = describe_module(module, image, starts, outdir);
    ModuleRun out;
    out.run = run(description.path, {}, options);
    out.numbered = std::move(description.by_number);
    return out;
}

c_text::Compilable32 Decompiler::compilable32(const std::filesystem::path& module, const std::optional<std::vector<Va>>& functions,
                                              const std::optional<std::filesystem::path>& outdir) {
    const std::vector<Va> starts = functions ? *functions : lanes_carved_functions(module);
    const std::filesystem::path folder = outdir ? *outdir : scratch_for({"decompiler", text::lower(fs::to_text(module.filename()))});
    return c_text::compilable32(*this, module, starts, folder);
}

// ---- a virtual device driver ----------------------------------------------------------------------------------------------------

std::vector<LeMember> Decompiler::decompile_le(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir_given) {
    const std::filesystem::path outdir = outdir_given ? *outdir_given : scratch_for({"decompiler_le", text::lower(fs::to_text(module.filename()))});
    std::vector<LeMember> results;
    for (const auto& [name, image] : formats::le::open_modules(module)) {
        auto d = std::make_shared<describe::LeDescription>(
            describe::describe_le(image, name, outdir / text::sanitize_identifier(name), context_.environment->knowledge()));
        const auto line = [](Va va) -> std::pair<std::string, std::string> {
            const std::int64_t object = (va - describe::kLeBase) / describe::kLeStride + 1;
            const std::int64_t offset = (va - describe::kLeBase) % describe::kLeStride;
            const std::string n = "sub_" + std::to_string(object) + "_" + upper_hex(offset, 5);
            return {n, "F\t" + hex(va) + "\t" + n + "\t-1"};
        };
        LeMember member;
        member.member = d->device;
        member.functions = lanes_grow(*this, *d, line, 4);
        member.description = d;
        results.push_back(std::move(member));
    }
    return results;
}

// ---- a DOS program -------------------------------------------------------------------------------------------------------------

DosProgramC Decompiler::decompile_dos(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir_given) {
    const std::filesystem::path outdir = outdir_given ? *outdir_given : scratch_for({"decompiler_dos", text::lower(fs::to_text(module.filename()))});
    const formats::dos::Program program = formats::dos::read_program(fs::read_file(module), fs::to_text(module.filename()));
    auto d = std::make_shared<describe::DosDescription>(describe::describe_dos(program, outdir));
    const describe::Code16 code(d->image);
    const auto line = [&](Va va) -> std::pair<std::string, std::string> {
        const describe::Return r = describe::returns(code, va - d->base);
        const std::string n = "sub_" + upper_hex(va - d->base, 5);
        return {n, "F\t" + hex(va) + "\t" + n + "\t-1\t__cdecl16" + std::string(r.kind ? describe::distance_name(*r.kind) : "near")};
    };
    // the compiler's own library is named, not decompiled
    DosLibrary library = lanes_dos_library(*this, *d, context_.settings.checkout);
    lanes_dos_startup(*d, library);
    lanes_dos_arguments(*d, library.skipped);
    std::vector<Va> own;
    for (const auto& [va, fn] : d->analysis->functions)
        if (!library.skipped.contains(va)) own.push_back(va);
    Functions grown = lanes_grow(*this, *d, line, 8, library.skipped.empty() ? std::span<const Va>() : std::span<const Va>(own));
    const symbols::dos_interrupts::ServiceTable table =
        symbols::dos_interrupts::ServiceTable::load(context_.settings.checkout / "data" / "symbols" / "dos_interrupts.json");
    DosProgramC result;
    result.functions.order = std::move(grown.order);
    result.functions.by_va = std::move(grown.by_va);
    result.functions.structures = std::move(grown.structures);
    for (const auto& [name, text] : grown.texts) {
        std::vector<symbols::dos_interrupts::Interrupt> calls;
        if (const auto it = d->interrupts.find(name); it != d->interrupts.end())
            for (const analyze::dos_functions::Interrupt& i : it->second)
                calls.push_back({i.number, i.ah ? std::optional<std::uint32_t>(*i.ah) : std::nullopt, i.al ? std::optional<std::uint32_t>(*i.al) : std::nullopt, i.address});
        result.functions.texts.insert_or_assign(name, table.annotate(text, calls));
    }
    result.description = d;
    result.library = library.found;
    result.header = library.header;
    result.library_pointers = library.pointers;
    result.library_void = library.nothing;
    // each structure field's C type, as wide as the decompiler found it (`uint1` for a byte the code tests): the C pass declares
    // the structures again from what the statements reach, and every field it knows nothing of is a word
    for (const auto& [name, s] : result.functions.structures)
        for (const core16::StructField& f : s.fields) {
            static const std::set<std::string> scalars = {"int1", "uint1", "xunknown1", "int2", "uint2", "xunknown2", "int4", "uint4", "xunknown4"};
            const std::string type = scalars.contains(f.type) ? f.type : f.bytes == 1 || f.bytes == 2 || f.bytes == 4 ? "xunknown" + std::to_string(f.bytes) : "";
            if (!type.empty() && f.offset >= 0) result.structure_fields[name][static_cast<std::uint32_t>(f.offset)] = type;
        }
    return result;
}

// ---- any executable as C --------------------------------------------------------------------------------------------------------

// Any executable as C: the text, the functions decompiled, the functions found. UnsupportedError for a file that is not an
// executable this can read.
AsC Decompiler::as_c(const std::filesystem::path& module) {
    OrderedMap<std::string, std::string> texts;
    std::vector<std::string> order;
    std::string header;
    std::size_t total = 0;
    std::size_t library = 0;
    bool dos = false;   // a real-mode program: its C made compilable as a whole (c_text_dos)
    c_text_dos::Layout dos_layout;   // and its source files, where it names them
    const std::string stem = text::lower(fs::to_text(module.filename()));
    std::optional<formats::detect::Lane> kind;
    const bool nes = formats::nes::probe(module);
    if (!nes) kind = formats::detect::kind(module);
    using formats::detect::Lane;
    if (kind == Lane::Ne) {
        c_text::Decompiled16 r = decompile16(module, std::nullopt);
        texts = std::move(r.texts);
        order = std::move(r.order);
        total = r.description->names.size();
    } else if (kind == Lane::Pe && formats::pe::Image::open(module).header().machine == kMachineX86) {
        c_text::Compilable32 m = compilable32(module);
        // (one module: the functions named in the order, then those of the texts it has no place for)
        OrderedMap<std::uint64_t, std::string> placed;
        for (const auto& [va, name] : m.by_va) placed.insert_or_assign(static_cast<std::uint64_t>(va), name);
        const translate::l2::Made made = translate::l2::Made::of(stem, m.preamble, m.texts, m.order, placed);
        texts = made.texts();
        order = made.order();
        header = made.header();
        total = made.by_va().size();
    } else if (kind == Lane::Pe) {   // PowerPC: as the decompiler gives it
        const std::uint16_t machine = formats::pe::Image::open(module).header().machine;
        const std::vector<Va> starts =
            machine == kMachinePowerPc ? context_.environment->powerpc_functions(module) : lanes_carved_functions(module);
        Run r = run_module(module, starts, scratch_for({"decompiler", stem})).run;
        texts = std::move(r.texts);
        order = std::move(r.order);
        total = starts.size();
    } else if (nes) {   // the 6502, as the decompiler gives it
        const formats::nes::Rom rom = formats::nes::read(module);
        const std::vector<std::uint32_t> starts = rom.functions(lanes_decoder_6502());
        const describe::NesDescription description = describe::describe_nes(rom, starts, scratch_for({"decompiler_nes", stem}));
        Run r = run(description.path);
        texts = std::move(r.texts);
        order = std::move(r.order);
        total = description.names.size();
    } else if (kind == Lane::Le) {
        for (const LeMember& member : decompile_le(module, std::nullopt)) {
            total += member.description->names.size();
            for (const std::string& n : member.functions.order) {
                const std::string key = member.member + "." + n;
                texts.insert_or_assign(key, "/* driver " + member.member + " */\n" + member.functions.texts.at(n));
                order.push_back(key);
            }
        }
    } else if (kind == Lane::Dos) {
        const Bytes data = fs::read_file(module);
        const auto parts = formats::dos::chain(data);
        dos = true;
        if (parts.empty()) {
            DosProgramC r = decompile_dos(module, std::nullopt);
            dos_layout = lanes_dos_layout(*r.description);
            dos_layout.library_pointers = r.library_pointers;
            dos_layout.library_void = r.library_void;
            dos_layout.structure_fields = r.structure_fields;
            texts = std::move(r.functions.texts);
            order = std::move(r.functions.order);
            total = r.description->names.size();
            header = std::move(r.header);
            library = r.library.size();
        } else {   // programs one after another: each its own
            std::size_t k = 1;
            for (const auto& [at, size] : parts) {
                const std::string part_name = stem + ".part" + std::to_string(k);
                const std::filesystem::path part = scratch_for({"decompiler_dos", part_name, part_name + ".exe"});
                fs::make_dirs(part.parent_path());
                fs::write_file(part, ByteView(data).subspan(static_cast<std::size_t>(at), static_cast<std::size_t>(size)));
                DosProgramC r = decompile_dos(part, std::nullopt);
                total += r.description->names.size();
                for (const std::string& n : r.functions.order) {
                    const std::string key = "part" + std::to_string(k) + "." + n;
                    texts.insert_or_assign(key, "/* part " + std::to_string(k) + " of " + std::to_string(parts.size()) + ": offset " +
                                                    upper_hex(static_cast<std::int64_t>(at), 1) + "h, " + std::to_string(size) + " bytes */\n" +
                                                    r.functions.texts.at(n));
                    order.push_back(key);
                }
                ++k;
            }
        }
    } else {
        throw UsageError(fs::to_text(module) + ": not an executable this can read");
    }
    std::string text = "/* " + fs::to_text(module.filename()) + " -- decompiled by CodeBreaker; not built, not judged. */\n\n";
    if (!header.empty()) text += header + "\n\n";
    bool first = true;
    for (const std::string& n : order) {
        const std::string* body = texts.find(n);
        if (body == nullptr) continue;
        if (!first) text += "\n\n";
        first = false;
        text += text::trim(*body);
    }
    text += "\n";
    if (dos) text = c_text_dos::compilable(text, c_text::header16(), dos_layout).text;
    return {std::move(text), order.size(), total, library};
}

// ---- the command ---------------------------------------------------------------------------------------------------------------

int command(Decompiler& decompiler, const base::Corpus& corpus, const base::converted::Store& converted, std::span<const std::string> args) {
    if (args.empty()) {
        out_text(
            "codebreaker decompile <module> [--out FILE]\n\n"
            "Decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows module, a\n"
            "virtual device driver, a DOS program, a PowerPC image, a NES cartridge -- and write its functions as C. "
            "Nothing is built or judged; `module` does that.\n");
        flush_out();
        return 2;
    }
    const std::optional<std::filesystem::path> path = corpus.retail(args[0]);
    if (!path) throw UsageError("no module '" + args[0] + "'");
    const AsC c = decompiler.as_c(*path);
    std::optional<base::converted::Project> project;
    std::filesystem::path out;
    const auto flag = std::find(args.begin(), args.end(), "--out");
    if (flag != args.end()) {
        if (flag + 1 == args.end()) throw UsageError("--out needs a file name");
        out = fs::from_text(*(flag + 1));
    } else {   // the binary's project folder: Converted/<name>/src/
        project = converted.project(*path);
        out = project->src() / (text::lower(fs::to_text(path->stem())) + ".c");
    }
    fs::write_text(out, c.text);
    if (project)
        project->record("decompile", {out}, std::to_string(c.decompiled) + " of " + std::to_string(c.found) + " functions as C (readable; not built or judged)");
    out_text(fs::to_text(path->filename()) + ": " + std::to_string(c.decompiled) + " of " + std::to_string(c.found) + " functions -> " + fs::to_text(out) + "\n");
    flush_out();
    return 0;
}

}  // namespace cb::decompile::decompiler
