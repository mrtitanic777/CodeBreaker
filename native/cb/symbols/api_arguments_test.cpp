// Tests of cb/symbols/api_arguments. The stack scan runs over a real Windows 95 module (formats/testdata/lz32.dll: its twelve
// exports each take a known number of argument bytes -- the values are what the Python module read from the same file) and
// over small modules written out byte by byte, so that each rule of the walk (the depth the stack is at, jumps into other
// functions, calls, imports, forwarded exports, the thunk compiler's stubs) can be checked by hand.
//
// DIVERGES from the Python module, by design:
//   * an operand the scan cannot read as a number (a constant that is hexadecimal digits without 0x) ends that path, or
//     counts as an unknown change of the stack; the Python raised ValueError there, which gave up on the whole export;
//   * a 16-bit export whose size is more than 32 bits is unknown; a name's ordinal beyond 32 bits is no ordinal.
#include "cb/symbols/api_arguments.hpp"

#include <format>
#include <map>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace aa = cb::symbols::api_arguments;
namespace pe = cb::formats::pe;
using cb::Bytes;

namespace {

constexpr std::uint32_t kBase = 0x10000000;
constexpr std::uint32_t kExportDir = 0x1800, kForwardText = 0x1900, kImportDir = 0x1A00, kIat = 0x1A80;

void put(Bytes& b, std::size_t at, std::uint64_t v, int width) {
    for (int k = 0; k < width; ++k) b[at + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(v >> (8 * k));
}
void put_text(Bytes& b, std::size_t at, std::string_view s) { std::copy(s.begin(), s.end(), b.begin() + static_cast<std::ptrdiff_t>(at)); }
std::size_t file_at(std::uint32_t rva) { return rva - 0x1000 + 0x400; }

// "c2 08 00" as bytes
Bytes hex(std::string_view text) {
    std::string digits;
    for (const char c : text) {
        if (c != ' ') digits += c;
    }
    return cb::crypto::from_hex(digits);
}
Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) out.insert(out.end(), p.begin(), p.end());
    return out;
}
// jmp / call rel32 at `from` to `to`
Bytes rel32(std::uint8_t opcode, std::uint32_t from, std::uint32_t to) {
    Bytes b(5, 0);
    b[0] = opcode;
    put(b, 1, to - (from + 5), 4);
    return b;
}
Bytes jmp(std::uint32_t from, std::uint32_t to) { return rel32(0xE9, from, to); }
Bytes call(std::uint32_t from, std::uint32_t to) { return rel32(0xE8, from, to); }
// call / jmp dword ptr [IAT slot k]
Bytes call_import(std::size_t k) {
    Bytes b = hex("ff 15");
    b.resize(6);
    put(b, 2, kBase + kIat + 4 * k, 4);
    return b;
}
Bytes jmp_import(std::size_t k) {
    Bytes b = call_import(k);
    b[1] = 0x25;
    return b;
}

// A module written out: its functions, what it exports (by code address or as another module's function) and what it imports.
struct Spec {
    struct Function {
        std::uint32_t rva;
        Bytes code;
    };
    struct Export {
        std::string name;
        std::uint32_t rva = 0;           // code address, unless `forward`
        std::string forward;             // "OTHER.Name"
    };
    std::string dll = "SYN.DLL";
    std::vector<Function> functions;
    std::vector<Export> exports;
    std::string import_dll;              // "OTHER.dll"
    std::vector<std::string> imports;    // names, or "#ordinal"
};

Bytes build_pe(const Spec& spec) {
    Bytes f(0x1400, 0);
    f[0] = 'M', f[1] = 'Z';
    put(f, 0x3C, 0x100, 4);
    put_text(f, 0x100, std::string_view("PE\0\0", 4));
    put(f, 0x104, 0x014C, 2);
    put(f, 0x106, 1, 2);
    put(f, 0x114, 0xE0, 2);
    put(f, 0x116, 0x2102, 2);
    const std::size_t opt = 0x118;
    put(f, opt, 0x10B, 2);
    put(f, opt + 28, kBase, 4);
    put(f, opt + 32, 0x1000, 4), put(f, opt + 36, 0x200, 4);
    const std::size_t dd = opt + 92;
    put(f, dd, 16, 4);
    put(f, dd + 4, kExportDir, 4), put(f, dd + 8, 0x200, 4);
    if (!spec.imports.empty()) put(f, dd + 12, kImportDir, 4), put(f, dd + 16, 40, 4);
    const std::size_t sec = opt + 0xE0;
    put_text(f, sec, ".text");
    put(f, sec + 8, 0x1000, 4), put(f, sec + 12, 0x1000, 4), put(f, sec + 16, 0x1000, 4), put(f, sec + 20, 0x400, 4);
    put(f, sec + 36, 0x60000020, 4);
    for (const Spec::Function& fn : spec.functions) std::copy(fn.code.begin(), fn.code.end(), f.begin() + static_cast<std::ptrdiff_t>(file_at(fn.rva)));
    // exports: ordinals 1..n, every one named
    const std::size_t n = spec.exports.size();
    const std::size_t ex = file_at(kExportDir);
    put(f, ex + 12, kForwardText, 4);
    put(f, ex + 16, 1, 4), put(f, ex + 20, n, 4), put(f, ex + 24, n, 4);
    const std::uint32_t functions = kExportDir + 0x30, names = functions + 4 * static_cast<std::uint32_t>(n);
    const std::uint32_t ordinals = names + 4 * static_cast<std::uint32_t>(n);
    put(f, ex + 28, functions, 4), put(f, ex + 32, names, 4), put(f, ex + 36, ordinals, 4);
    std::uint32_t text = kForwardText + 0x10;
    put_text(f, file_at(kForwardText), spec.dll);
    for (std::size_t i = 0; i < n; ++i) {
        const Spec::Export& e = spec.exports[i];
        std::uint32_t target = e.rva;
        if (!e.forward.empty()) {
            target = text;
            put_text(f, file_at(text), e.forward);
            text += static_cast<std::uint32_t>(e.forward.size()) + 1;
        }
        put(f, file_at(functions) + 4 * i, target, 4);
        put(f, file_at(names) + 4 * i, text, 4);
        put_text(f, file_at(text), e.name);
        text += static_cast<std::uint32_t>(e.name.size()) + 1;
        put(f, file_at(ordinals) + 2 * i, i, 2);
    }
    // imports from one module
    if (!spec.imports.empty()) {
        const std::size_t im = file_at(kImportDir);
        put(f, im, kImportDir + 0x40, 4), put(f, im + 12, kImportDir + 0x100, 4), put(f, im + 16, kIat, 4);
        put_text(f, file_at(kImportDir + 0x100), spec.import_dll);
        for (std::size_t k = 0; k < spec.imports.size(); ++k) {
            const std::string& name = spec.imports[k];
            std::uint32_t entry;
            if (name.starts_with('#')) {
                entry = 0x80000000u | static_cast<std::uint32_t>(std::stoul(name.substr(1)));
            } else {
                entry = kImportDir + 0x120 + 0x20 * static_cast<std::uint32_t>(k);
                put_text(f, file_at(entry) + 2, name);
            }
            put(f, file_at(kImportDir + 0x40) + 4 * k, entry, 4);
            put(f, file_at(kIat) + 4 * k, entry, 4);
        }
    }
    return f;
}

std::shared_ptr<const aa::ExportingModule> module_of(const Spec& spec) {
    return std::make_shared<const aa::ExportingModule>(pe::Image::parse(build_pe(spec), spec.dll));
}

// The corpus: modules by stem. Asking for a module counts.
class Corpus : public aa::ModuleSource {
public:
    std::map<std::string, std::shared_ptr<const aa::ExportingModule>> modules;
    std::map<std::string, int> asked;
    std::shared_ptr<const aa::ExportingModule> find(std::string_view stem) override {
        ++asked[std::string(stem)];
        const auto it = modules.find(std::string(stem));
        return it == modules.end() ? nullptr : it->second;
    }
};

}  // namespace

CB_TEST("symbols.api_arguments.stems", [] {
    CB_CHECK_EQ(aa::stem("USER32.dll"), "USER32");
    CB_CHECK_EQ(aa::stem("C:\\Windows\\System\\user32.DLL"), "USER32");
    CB_CHECK_EQ(aa::stem("c:/x/comctl32.ocx"), "COMCTL32");
    CB_CHECK_EQ(aa::stem("kernel"), "KERNEL");
    CB_CHECK_EQ(aa::stem("a.b.drv"), "A.B");
    CB_CHECK_EQ(aa::stem("setup.exe"), "SETUP");
    CB_CHECK_EQ(aa::stem("x.cpl"), "X");
    CB_CHECK_EQ(aa::stem("notes.txt"), "NOTES.TXT");
    CB_CHECK_EQ(aa::stem(""), "");
    CB_CHECK_EQ(aa::module_extensions().size(), std::size_t{5});
});

CB_TEST("symbols.api_arguments.every_export_of_a_real_module", [] {
    // lz32.dll of Windows 95: stdcall wrappers that thunk to the 16-bit LZEXPAND. Its imports are of KERNEL32 (not in this corpus).
    Corpus corpus;
    corpus.modules["LZ32"] = std::make_shared<const aa::ExportingModule>(
        pe::Image::open(cb::testing::data_path("formats/testdata/lz32.dll")));
    aa::StackScanner scanner(corpus);
    const std::map<std::string, std::uint32_t> expected = {
        {"CopyLZFile", 8},  {"GetExpandedNameA", 8}, {"GetExpandedNameW", 8}, {"LZClose", 4},     {"LZCopy", 8},   {"LZDone", 0},
        {"LZInit", 4},      {"LZOpenFileA", 12},     {"LZOpenFileW", 12},     {"LZRead", 12},     {"LZSeek", 12},  {"LZStart", 0}};
    for (const auto& [name, bytes] : expected) {
        const auto got = scanner.resolve("lz32.dll", name);
        CB_CHECK(got.has_value());
        CB_CHECK_EQ(got.value_or(999), bytes);
    }
    CB_CHECK(!scanner.resolve("lz32.dll", "NoSuchFunction").has_value());
    CB_CHECK(!scanner.resolve("lz32.dll", "#999").has_value());
    CB_CHECK(!scanner.resolve("lz32.dll", "").has_value());
    CB_CHECK(!scanner.resolve("kernel32.dll", "Sleep").has_value());   // not in the corpus
});

CB_TEST("symbols.api_arguments.the_module_reads_exports_imports_and_forwarders", [] {
    Spec spec;
    spec.functions = {{0x1000, hex("c2 08 00")}};
    spec.exports = {{"Plain", 0x1000, ""}, {"Fwd", 0, "OTHER.Func"}};
    spec.import_dll = "OTHER.dll";
    spec.imports = {"Target", "#7"};
    const auto m = module_of(spec);
    CB_CHECK_EQ(m->image_base(), std::uint64_t{kBase});
    CB_CHECK_EQ(m->export_by_name("Plain").value_or(0), std::uint32_t{0x1000});
    CB_CHECK(!m->export_by_name("plain").has_value());
    CB_CHECK_EQ(m->export_by_ordinal(1).value_or(0), std::uint32_t{0x1000});
    CB_CHECK(m->export_by_ordinal(2).has_value());
    CB_CHECK(!m->export_by_ordinal(3).has_value());
    CB_CHECK(!m->forwarder(0x1000).has_value());
    const auto fwd = m->forwarder(*m->export_by_name("Fwd"));
    CB_CHECK(fwd.has_value());
    CB_CHECK_EQ(*fwd, "OTHER.Func");
    CB_CHECK(m->import_at(kBase + kIat) != nullptr);
    CB_CHECK_EQ(m->import_at(kBase + kIat)->dll, "OTHER.dll");
    CB_CHECK_EQ(m->import_at(kBase + kIat)->name, "Target");
    CB_CHECK_EQ(m->import_at(kBase + kIat + 4)->name, "#7");
    CB_CHECK(m->import_at(kBase + kIat + 8) == nullptr);
    // bytes by address: as the file holds them, cut at its end, none outside a section
    CB_CHECK_EQ(m->bytes_at(0x1000, 16).size(), std::size_t{16});
    CB_CHECK_EQ(m->bytes_at(0x1000, 16)[0], std::uint8_t{0xC2});
    CB_CHECK(m->bytes_at(0x500, 16).empty());
    CB_CHECK(m->bytes_at(-4, 16).empty());
    CB_CHECK(m->bytes_at(0x2000, 16).empty());
    CB_CHECK_EQ(m->bytes_at(0x1FF8, 16).size(), std::size_t{8});
});

namespace {

// One module whose functions are the cases of the walk, one to an RVA:
struct Cases {
    Spec spec;
    Corpus corpus;
    std::shared_ptr<const aa::ExportingModule> module;
    std::unique_ptr<aa::StackScanner> scanner;

    void add(std::uint32_t rva, const Bytes& code, const char* name = nullptr) {
        spec.functions.push_back({rva, code});
        if (name != nullptr) spec.exports.push_back({name, rva, ""});
    }
    void finish() {
        module = module_of(spec);
        corpus.modules["SYN"] = module;
        scanner = std::make_unique<aa::StackScanner>(corpus);
    }
    std::optional<std::uint32_t> scan(std::uint32_t rva) { return scanner->scan(*module, rva, aa::kMaxHops); }
};

}  // namespace

CB_TEST("symbols.api_arguments.scan_follows_the_stack_depth", [] {
    Cases c;
    c.add(0x1000, hex("c2 08 00"));                                   // ret 8
    c.add(0x1010, hex("c3"));                                         // ret
    c.add(0x1020, hex("55 8b ec 5d c2 10 00"));                       // push ebp / mov ebp, esp / pop ebp / ret 16
    c.add(0x1030, hex("83 ec 08 83 c4 08 c2 04 00"));                 // sub esp, 8 / add esp, 8 / ret 4
    c.add(0x1040, hex("55 8b ec 83 ec 10 c9 c2 04 00"));              // push ebp / mov ebp, esp / sub esp, 16 / leave / ret 4
    c.add(0x1050, hex("55 8b ec 83 ec 10 8b e5 5d c2 08 00"));        // ... / mov esp, ebp / pop ebp / ret 8
    c.add(0x1060, hex("55 8b ec 83 ec 10 8d 65 f4 5d c2 0c 00"));     // ... / lea esp, [ebp - 12] / pop ebp / ret 12 (depth 4 + 12 - 4...)
    c.add(0x1070, hex("c2 06 00"));                                   // ret 6: not a whole number of words
    c.add(0x1080, hex("cc"));                                         // int3: never returns
    c.add(0x1090, hex("60 61 c2 04 00"));                             // pushad / popad / ret 4
    c.add(0x10A0, hex("9c 9d c2 04 00"));                             // pushfd / popfd / ret 4
    c.add(0x10B0, hex("60 c2 04 00"));                                // pushad / ret 4: 32 bytes are still pushed, more than it returns
    c.add(0x10C0, hex("9c c2 08 00"));                                // pushfd / ret 8
    c.finish();
    CB_CHECK_EQ(c.scan(0x1000).value_or(999), std::uint32_t{8});
    CB_CHECK_EQ(c.scan(0x1010).value_or(999), std::uint32_t{0});
    CB_CHECK_EQ(c.scan(0x1020).value_or(999), std::uint32_t{16});
    CB_CHECK_EQ(c.scan(0x1030).value_or(999), std::uint32_t{4});
    CB_CHECK_EQ(c.scan(0x1040).value_or(999), std::uint32_t{4});
    CB_CHECK_EQ(c.scan(0x1050).value_or(999), std::uint32_t{8});
    // lea esp, [ebp - 12] with ebp at depth 4 puts the depth at 4 + 12 = 16; the pop makes it 12; ret 12 takes none of its own
    CB_CHECK_EQ(c.scan(0x1060).value_or(999), std::uint32_t{0});
    CB_CHECK(!c.scan(0x1070).has_value());
    CB_CHECK(!c.scan(0x1080).has_value());
    CB_CHECK_EQ(c.scan(0x1090).value_or(999), std::uint32_t{4});
    CB_CHECK_EQ(c.scan(0x10A0).value_or(999), std::uint32_t{4});
    CB_CHECK(!c.scan(0x10B0).has_value());
    CB_CHECK_EQ(c.scan(0x10C0).value_or(999), std::uint32_t{4});
    // an address that holds nothing
    CB_CHECK(!c.scan(0x1F00).has_value());
    CB_CHECK(!c.scan(0x300).has_value());
});

CB_TEST("symbols.api_arguments.scan_follows_jumps_and_calls_inside_the_module", [] {
    Cases c;
    c.add(0x1000, hex("c2 10 00"));                                   // FindWindowExA: ret 16
    // FindWindowA: pop ecx / push 0 / push 0 / push ecx / jmp FindWindowExA   -> 16 - 8 = 8
    c.add(0x1010, cat({hex("59 6a 00 6a 00 51"), jmp(0x1016, 0x1000)}));
    c.add(0x1020, hex("c2 08 00"));                                   // callee: ret 8
    c.add(0x1030, cat({hex("6a 01 6a 02"), call(0x1034, 0x1020), hex("c3")}));         // push 1 / push 2 / call callee / ret
    c.add(0x1040, cat({call(0x1040, 0x1040), hex("c2 04 00")}));      // calls itself: nothing is known of the callee, ret 4 is believed
    c.add(0x1050, hex("85 c0 74 03 c2 04 00 c2 08 00"));              // test eax, eax / je +3 / ret 4 / ret 8   (two answers)
    c.add(0x1060, hex("85 c0 74 03 c2 08 00 c2 08 00"));              // the two paths agree
    c.add(0x1070, cat({call(0x1070, 0x1F00), hex("c2 04 00")}));      // calls into nothing
    c.finish();
    CB_CHECK_EQ(c.scan(0x1010).value_or(999), std::uint32_t{8});
    CB_CHECK_EQ(c.scan(0x1030).value_or(999), std::uint32_t{0});
    CB_CHECK_EQ(c.scan(0x1040).value_or(999), std::uint32_t{4});
    CB_CHECK(!c.scan(0x1050).has_value());
    CB_CHECK_EQ(c.scan(0x1060).value_or(999), std::uint32_t{8});
    CB_CHECK_EQ(c.scan(0x1070).value_or(999), std::uint32_t{4});
});

CB_TEST("symbols.api_arguments.scan_reads_the_thunk_compilers_stubs", [] {
    Cases c;
    // mov cl, 3 / mov edx, 2 << 16 | 1 / jmp common                -- two argument words: 8 bytes
    c.add(0x1000, cat({hex("b1 03 ba 01 00 02 00"), jmp(0x1007, 0x1100)}));
    // mov cl, 5 / jmp dword ptr [eax]                              -- no count in the code: index * 4
    c.add(0x1020, hex("b1 05 ff 20"));
    // push 0 / mov cl, 5 / jmp dword ptr [eax]                     -- the stack is not at its entry depth: not trusted
    c.add(0x1030, hex("6a 00 b1 05 ff 20"));
    // mov cl, 5 / mov cl, 6 / jmp dword ptr [eax]                  -- the last cl is the index
    c.add(0x1040, hex("b1 05 b1 06 ff 20"));
    c.add(0x1100, hex("c3"));
    c.finish();
    CB_CHECK_EQ(c.scan(0x1000).value_or(999), std::uint32_t{8});
    CB_CHECK_EQ(c.scan(0x1020).value_or(999), std::uint32_t{20});
    CB_CHECK(!c.scan(0x1030).has_value());
    CB_CHECK_EQ(c.scan(0x1040).value_or(999), std::uint32_t{24});
});

CB_TEST("symbols.api_arguments.resolve_follows_imports_and_forwarders", [] {
    // OTHER: Target ret 12, Four ret 4 (ordinal 2), Loop forwarded to SYN.Loop
    Spec other;
    other.dll = "OTHER.DLL";
    other.functions = {{0x1000, hex("c2 0c 00")}, {0x1010, hex("c2 04 00")}};
    other.exports = {{"Target", 0x1000, ""}, {"Four", 0x1010, ""}, {"Loop", 0, "SYN.Loop"}};
    // SYN: imports OTHER's Target and ordinal 2
    Spec syn;
    syn.import_dll = "OTHER.dll";
    syn.imports = {"Target", "#2", "Missing"};
    syn.functions = {
        {0x1000, jmp_import(0)},                                                       // jmp [Target]            -> 12
        {0x1010, cat({hex("6a 00 6a 00 6a 00"), call_import(0), hex("c2 00 00")})},    // push x3 / call [Target] / ret 0 -> 0
        {0x1030, call_import(1)},                                                      // call [ordinal 2] (nothing is pushed) / falls off
        {0x1040, jmp_import(2)},                                                       // jmp [Missing]
        {0x1050, hex("b1 02 ff 20")},                                                  // by cl
    };
    syn.exports = {{"Tail", 0x1000, ""}, {"CallsImport", 0x1010, ""}, {"Fwd", 0, "OTHER.Target"}, {"FwdOrdinal", 0, "OTHER.#2"},
                   {"Loop", 0, "OTHER.Loop"}, {"NoModule", 0, "GONE.Func"}, {"FwdMissing", 0, "OTHER.Absent"}, {"Missing", 0x1040, ""}};
    Corpus corpus;
    corpus.modules["OTHER"] = module_of(other);
    corpus.modules["SYN"] = module_of(syn);
    aa::StackScanner scanner(corpus);
    CB_CHECK_EQ(scanner.resolve("syn.dll", "Tail").value_or(999), std::uint32_t{12});
    CB_CHECK_EQ(scanner.resolve("SYN", "CallsImport").value_or(999), std::uint32_t{0});
    CB_CHECK_EQ(scanner.resolve("SYN", "Fwd").value_or(999), std::uint32_t{12});
    CB_CHECK_EQ(scanner.resolve("SYN", "FwdOrdinal").value_or(999), std::uint32_t{4});
    CB_CHECK_EQ(scanner.resolve("OTHER", "#2").value_or(999), std::uint32_t{4});   // an export asked for by number
    CB_CHECK(!scanner.resolve("SYN", "Loop").has_value());                         // SYN.Loop -> OTHER.Loop -> SYN.Loop ...
    CB_CHECK(!scanner.resolve("SYN", "NoModule").has_value());
    CB_CHECK(!scanner.resolve("SYN", "FwdMissing").has_value());
    CB_CHECK(!scanner.resolve("SYN", "Missing").has_value());
    CB_CHECK(!scanner.resolve("SYN", "#x").has_value());
    // forwarders count as hops
    CB_CHECK(scanner.resolve("SYN", "Fwd", 1).has_value());
    CB_CHECK(!scanner.resolve("SYN", "Fwd", 0).has_value());
});

CB_TEST("symbols.api_arguments.scan_remembers_what_it_found", [] {
    Cases c;
    c.add(0x1000, hex("c2 08 00"));
    c.finish();
    CB_CHECK_EQ(c.scan(0x1000).value_or(999), std::uint32_t{8});
    // the same module, the bytes no longer asked for again: the answer is in the memo
    CB_CHECK_EQ(c.scan(0x1000).value_or(999), std::uint32_t{8});
});

namespace {

class Memory : public aa::Win32Memory {
public:
    std::map<std::string, std::uint32_t> declared;
    std::map<std::pair<std::string, std::string>, aa::Known> scans;
    int remembered = 0;
    std::optional<std::uint32_t> header_bytes(std::string_view name) override {
        const auto it = declared.find(std::string(name));
        return it == declared.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
    }
    std::optional<aa::Known> scanned(std::string_view stem, std::string_view name) override {
        const auto it = scans.find({std::string(stem), std::string(name)});
        return it == scans.end() ? std::nullopt : std::optional<aa::Known>(it->second);
    }
    void remember(std::string_view stem, std::string_view name, std::optional<std::uint32_t> bytes) override {
        ++remembered;
        scans[{std::string(stem), std::string(name)}] = aa::Known{bytes};
    }
};

}  // namespace

CB_TEST("symbols.api_arguments.win32_the_headers_first_then_the_code_once", [] {
    Spec spec;
    spec.functions = {{0x1000, hex("c2 0c 00")}, {0x1010, hex("c2 06 00")}};
    spec.exports = {{"Declared", 0x1000, ""}, {"Scanned", 0x1000, ""}, {"Odd", 0x1010, ""}};
    Corpus corpus;
    corpus.modules["SYN"] = module_of(spec);
    aa::StackScanner scanner(corpus);
    Memory memory;
    memory.declared["Declared"] = 20;      // the headers say 20; the code would say 12
    aa::Win32Arguments sizes(memory, scanner);
    CB_CHECK_EQ(sizes.stack_bytes("syn.dll", "Declared").value_or(999), std::uint32_t{20});
    CB_CHECK_EQ(memory.remembered, 0);      // the code was not asked
    CB_CHECK_EQ(corpus.asked["SYN"], 0);
    CB_CHECK_EQ(sizes.stack_bytes("SYN.DLL", "Scanned").value_or(999), std::uint32_t{12});
    CB_CHECK_EQ(memory.remembered, 1);
    CB_CHECK_EQ(corpus.asked["SYN"], 1);
    CB_CHECK_EQ(sizes.stack_bytes("syn", "Scanned").value_or(999), std::uint32_t{12});   // remembered: no second look
    CB_CHECK_EQ(memory.remembered, 1);
    CB_CHECK_EQ(corpus.asked["SYN"], 1);
    // an answer that could not be found is remembered as well
    CB_CHECK(!sizes.stack_bytes("syn", "Nonesuch").has_value());
    CB_CHECK(!sizes.stack_bytes("syn", "Nonesuch").has_value());
    CB_CHECK_EQ(memory.remembered, 2);
    CB_CHECK(memory.scans.at({"SYN", "Nonesuch"}).bytes == std::nullopt);
    // argument counts
    CB_CHECK_EQ(sizes.argc("syn", "Scanned").value_or(999), std::uint32_t{3});
    CB_CHECK_EQ(sizes.argc("syn", "Declared").value_or(999), std::uint32_t{5});
    memory.declared["Six"] = 6;
    CB_CHECK(!sizes.argc("syn", "Six").has_value());            // not a whole number of words
    CB_CHECK(!sizes.argc("syn", "Odd").has_value());            // the code says 6 (not decidable: no multiple of 4)
    CB_CHECK_EQ(sizes.header_bytes("Declared").value_or(999), std::uint32_t{20});
});

namespace {

class Exports16 : public aa::Win16Exports {
public:
    std::map<std::pair<std::string, std::uint32_t>, Found> found;
    int asked = 0;
    std::optional<Found> find(std::string_view module, std::uint32_t ordinal) override {
        ++asked;
        const auto it = found.find({std::string(module), ordinal});
        return it == found.end() ? std::nullopt : std::optional<Found>(it->second);
    }
};

class Memory16 : public aa::Win16Memory {
public:
    std::map<std::string, std::optional<std::uint32_t>> kept;
    std::map<std::string, std::string> sources;
    std::optional<aa::Known> remembered(std::string_view key) override {
        const auto it = kept.find(std::string(key));
        return it == kept.end() ? std::nullopt : std::optional<aa::Known>(aa::Known{it->second});
    }
    std::map<std::string, std::optional<std::uint32_t>> table() override { return kept; }
    void remember(std::string_view key, std::optional<std::uint32_t> bytes, std::string_view source) override {
        kept[std::string(key)] = bytes;
        sources[std::string(key)] = std::string(source);
    }
};

aa::Win16Exports::Found far_return(std::int64_t pop, std::string file = "user.exe") {
    return {aa::ExportReturn{std::nullopt, true, pop}, std::move(file)};
}

}  // namespace

CB_TEST("symbols.api_arguments.win16_sizes_come_from_how_the_export_returns", [] {
    Exports16 exports;
    exports.found[{"USER", 91}] = far_return(10);                                                          // retf 10
    exports.found[{"USER", 92}] = far_return(-1);                                                          // retf: nothing popped known
    exports.found[{"USER", 93}] = {aa::ExportReturn{std::nullopt, false, 4}, "user.exe"};                  // a near return
    exports.found[{"USER", 471}] = {aa::ExportReturn{6u, true, 10}, "USER.EXE"};                           // forwards: its own answer wins
    exports.found[{"USER", 94}] = {std::nullopt, "user.exe"};                                              // no code for it
    exports.found[{"GDI", 5}] = far_return(0, "gdi.exe");
    Memory16 memory;
    aa::Win16Arguments sizes(memory, exports);
    CB_CHECK_EQ(sizes.arg_bytes("user", 91).value_or(999), std::uint32_t{10});
    CB_CHECK_EQ(memory.sources.at("USER.91"), "user.exe's code");
    CB_CHECK(!sizes.arg_bytes("USER", 92).has_value());
    CB_CHECK(!sizes.arg_bytes("USER", 93).has_value());
    CB_CHECK_EQ(sizes.arg_bytes("USER", 471).value_or(999), std::uint32_t{6});
    CB_CHECK_EQ(memory.sources.at("USER.471"), "user.exe's code");
    CB_CHECK(!sizes.arg_bytes("USER", 94).has_value());
    CB_CHECK_EQ(sizes.arg_bytes("GDI", 5).value_or(999), std::uint32_t{0});
    // no such module: the module's name is the note
    CB_CHECK(!sizes.arg_bytes("nomod", 1).has_value());
    CB_CHECK_EQ(memory.sources.at("NOMOD.1"), "NOMOD's code");
    // asked once, remembered, including what is not known
    const int asked = exports.asked;
    CB_CHECK_EQ(sizes.arg_bytes("USER", 91).value_or(999), std::uint32_t{10});
    CB_CHECK(!sizes.arg_bytes("USER", 92).has_value());
    CB_CHECK(!sizes.arg_bytes("nomod", 1).has_value());
    CB_CHECK_EQ(exports.asked, asked);
    // as the emulator names the API
    CB_CHECK_EQ(sizes.of("USER.@91").value_or(999), std::uint32_t{10});
    CB_CHECK_EQ(sizes.of("USER_91").value_or(999), std::uint32_t{10});
    CB_CHECK_EQ(sizes.of("USER.91").value_or(999), std::uint32_t{10});
    CB_CHECK(!sizes.of("GetDlgItem").has_value());
    CB_CHECK(!sizes.of("").has_value());
});

CB_TEST("symbols.api_arguments.win16_table_text_is_sorted_json", [] {
    Exports16 exports;
    exports.found[{"USER", 91}] = far_return(10);
    exports.found[{"GDI", 5}] = far_return(0);
    Memory16 memory;
    aa::Win16Arguments sizes(memory, exports);
    CB_CHECK_EQ(sizes.table_text(), "{}");
    (void)sizes.arg_bytes("USER", 91);
    (void)sizes.arg_bytes("USER", 7);
    (void)sizes.arg_bytes("GDI", 5);
    CB_CHECK_EQ(sizes.table_text(), "{\"GDI.5\": 0, \"USER.7\": null, \"USER.91\": 10}");
});

CB_TEST("symbols.api_arguments.api_names", [] {
    const auto a = aa::parse_api_name("USER.@171");
    CB_CHECK(a.has_value());
    CB_CHECK_EQ(a->module, "USER");
    CB_CHECK_EQ(a->ordinal, std::uint32_t{171});
    CB_CHECK_EQ(aa::parse_api_name("USER_171")->ordinal, std::uint32_t{171});
    CB_CHECK_EQ(aa::parse_api_name("A_1_2")->module, "A_1");
    CB_CHECK_EQ(aa::parse_api_name("a.@5")->module, "a");
    CB_CHECK(!aa::parse_api_name("USER._171").has_value());
    CB_CHECK(!aa::parse_api_name("x").has_value());
    CB_CHECK(!aa::parse_api_name("USER.171x").has_value());
    CB_CHECK(!aa::parse_api_name("_5").has_value());
    CB_CHECK(!aa::parse_api_name("KERNEL..5").has_value());
    CB_CHECK(!aa::parse_api_name("USER.@").has_value());
    CB_CHECK(!aa::parse_api_name("USER.99999999999").has_value());
    CB_CHECK(!aa::parse_api_name("").has_value());
});
