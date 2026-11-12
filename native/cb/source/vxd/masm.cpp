// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/vxd/masm.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::source::vxd::masm {

namespace le = formats::le;

// ---- the recovered driver, as the emitters work on it -----------------------------------------------------------------------

Emission::Emission(const Disassembly& recovered, const DdkNames& names)
    : driver_(&recovered.driver()), names_(&names), ddb_(recovered.ddb()), fixups_(recovered.fixups()),
      instructions_(recovered.instructions()), labels_(recovered.labels()), jump_tables_(recovered.jump_tables()) {
    for (const le::Object& object : driver_->objects()) {
        const ByteView bytes = recovered.code(object.index);
        if (code_.size() < object.index) code_.resize(object.index);
        code_[object.index - 1].assign(bytes.begin(), bytes.end());
    }
}

Emission::Emission(const le::Module& driver, const DdkNames& names, EmissionParts parts)
    : driver_(&driver), names_(&names), ddb_(std::move(parts.ddb)), fixups_(std::move(parts.fixups)), code_(std::move(parts.code)),
      instructions_(std::move(parts.instructions)), labels_(std::move(parts.labels)), jump_tables_(std::move(parts.jump_tables)) {}

std::optional<le::Fixup> Emission::fixup_at(const Place& at) const {
    const le::Fixup* f = fixups_.find(at);
    return f ? std::optional<le::Fixup>(*f) : std::nullopt;
}

ByteView Emission::code(std::uint32_t object) const {
    return object >= 1 && object <= code_.size() ? ByteView(code_[object - 1]) : ByteView();
}

Bytes& Emission::code_mut(std::uint32_t object) {
    CB_ASSERT(object >= 1 && object <= code_.size(), "no such object");
    return code_[object - 1];
}

const std::string* Emission::label_at(const Place& at) const {
    const auto it = labels_.find(at);
    return it == labels_.end() || it->second.empty() ? nullptr : &it->second;
}

const Instruction* Emission::instruction_at(const Place& at) const {
    const auto it = instructions_.find(at);
    return it == instructions_.end() ? nullptr : &it->second;
}

// ---- MASM text for one instruction ------------------------------------------------------------------------------------------

bool is_branch(std::string_view mnemonic) {
    static constexpr std::array<std::string_view, 27> names = {
        "call", "jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle",
        "jo", "jno", "js", "jns", "jp", "jnp", "jc", "jnc", "jcxz", "jecxz", "loop", "loope", "loopne"};
    return std::find(names.begin(), names.end(), mnemonic) != names.end();
}

namespace {

bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_hex_letter(char c) { return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

// pushal and popal are capstone's names for what MASM calls pushad and popad.
std::string_view masm_mnemonic(std::string_view mnemonic) {
    if (mnemonic == "pushal") return "pushad";
    if (mnemonic == "popal") return "popad";
    return mnemonic;
}

bool is_string_operation(std::string_view mnemonic) {
    static constexpr std::array<std::string_view, 21> names = {
        "movsb", "movsw", "movsd", "stosb", "stosw", "stosd", "lodsb", "lodsw", "lodsd", "scasb", "scasw",
        "scasd", "cmpsb", "cmpsw", "cmpsd", "insb", "insw", "insd", "outsb", "outsw", "outsd"};
    const std::size_t space = mnemonic.rfind(' ');   // "rep movsb"
    const std::string_view last = space == std::string_view::npos ? mnemonic : mnemonic.substr(space + 1);
    return std::find(names.begin(), names.end(), last) != names.end();
}

// jmp and jcc have a 2-byte rel8 (SHORT) and a 5/6-byte rel32 (NEAR) form; MASM picks the short one when the target fits, which
// drifts from a compiler that emitted the long form. Writing the size pins it to the original's encoding. call is always near;
// jcxz and loop are always short: neither takes a size.
bool is_shortable(std::string_view mnemonic) {
    static constexpr std::array<std::string_view, 31> names = {
        "jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle", "jo", "jno", "js", "jns",
        "jp", "jnp", "jc", "jnc", "jpe", "jpo", "jna", "jnae", "jnb", "jnbe", "jng", "jnge", "jnl", "jnle"};
    return std::find(names.begin(), names.end(), mnemonic) != names.end();
}

}  // namespace

std::string masm_operands(std::string_view operands) {
    std::string out;
    for (std::size_t i = 0; i < operands.size();) {
        if (operands[i] == '0' && i + 2 < operands.size() + 0 && operands[i + 1] == 'x' && is_hex_digit(operands[i + 2])) {
            std::size_t j = i + 2;
            while (j < operands.size() && is_hex_digit(operands[j])) ++j;
            const std::string_view digits = operands.substr(i + 2, j - i - 2);
            if (is_hex_letter(digits.front())) out += '0';
            out += digits;
            out += 'h';
            i = j;
        } else {
            out += operands[i++];
        }
    }
    return out;
}

std::optional<std::string> masm_line(const Emission& code, std::uint32_t object, const Instruction& insn) {
    const std::string_view mnemonic = masm_mnemonic(insn.mnemonic);
    // A far absolute jmp or call is `ljmp seg:off` in capstone, neither MASM nor NASM, and mostly appears where data was walked as
    // code: byte-preserve it rather than emit what cannot be assembled.
    if (insn.mnemonic == "ljmp" || insn.mnemonic == "lcall") return std::nullopt;
    if (is_string_operation(mnemonic)) return "        " + std::string(text::rtrim(mnemonic));   // `lodsb al,[esi]` -> `lodsb`
    const auto [operands, resolved] = operand_text(insn, object, [&](const Place& p) -> std::optional<std::string> {
        const std::string* label = code.label_at(p);
        return label ? std::optional<std::string>(*label) : std::nullopt;
    });
    if (is_branch(mnemonic) && !resolved && insn.direct_target()) return std::nullopt;   // a direct branch to an unlabelled address
    std::string ops = masm_operands(operands);
    if (is_shortable(mnemonic) && resolved) ops = (insn.size == 2 ? "SHORT " : "NEAR PTR ") + ops;
    return std::string(text::rtrim(std::format("        {:<6} {}", mnemonic, ops)));
}

std::string db_line(ByteView bytes) {
    std::string out = "        db  ";
    for (std::size_t i = 0; i < bytes.size(); ++i) out += std::format("{}0{:02X}h", i ? ", " : "", bytes[i]);
    return out;
}

bool has_absolute_memory(const Instruction& insn) {
    const std::string_view ops = insn.operands;
    for (std::size_t open = ops.find('['); open != std::string_view::npos; open = ops.find('[', open + 1)) {
        std::size_t i = open + 1;
        while (i < ops.size() && text::is_space(ops[i])) ++i;
        if (i + 2 < ops.size() && ops[i] == '0' && ops[i + 1] == 'x' && is_hex_digit(ops[i + 2])) {
            i += 2;
            while (i < ops.size() && is_hex_digit(ops[i])) ++i;
        } else if (i < ops.size() && ops[i] >= '0' && ops[i] <= '9') {
            while (i < ops.size() && ops[i] >= '0' && ops[i] <= '9') ++i;
        } else {
            continue;
        }
        while (i < ops.size() && text::is_space(ops[i])) ++i;
        if (i < ops.size() && ops[i] == ']') return true;
    }
    return false;
}

bool has_segment_prefix(const Instruction& insn) {
    if (insn.raw.empty()) return false;
    const std::uint8_t first = insn.raw.front();
    return first == 0x2E || first == 0x26 || first == 0x36 || first == 0x3E || first == 0x64 || first == 0x65;
}

// ---- the object, as MASM source ---------------------------------------------------------------------------------------------

Body object_body(const Emission& code, std::uint32_t object, const BodyOptions& options) {
    const ByteView bytes = code.code(object);
    const std::uint32_t n = static_cast<std::uint32_t>(bytes.size());   // the page bytes: may be less than vsize (a BSS tail)
    const auto [lo, hi] = options.range.value_or(std::make_pair(std::uint32_t{0}, n));
    Body body;
    std::vector<std::string>& lines = body.lines;
    const auto define_extra = [&](std::uint32_t at) {
        lines.push_back("public " + options.extra_labels.at(at));
        lines.push_back(options.extra_labels.at(at) + " label byte");
    };
    std::uint32_t a = lo;
    while (a < hi) {
        if (options.extra_labels.contains(a)) define_extra(a);
        if (const std::string* label = code.label_at({object, a})) lines.push_back(*label + ":");
        const Instruction* insn = code.instruction_at({object, a});
        if (insn == nullptr) {   // data bytes, up to the next instruction or label
            std::uint32_t b = a;
            while (b < hi && code.instruction_at({object, b}) == nullptr && !(b > a && code.label_at({object, b})) &&
                   !(b > a && options.extra_labels.contains(b))) {
                ++b;
            }
            for (std::uint32_t i = a; i < b; i += 16) lines.push_back(db_line(bytes.subspan(i, std::min<std::uint32_t>(16, b - i))));
            a = b;
            continue;
        }
        // An extra label (the DDB's) can land STRICTLY INSIDE a decoded instruction when data was walked as code. Stepping by the
        // instruction's size would skip the offset and the label would never be defined: emit the instruction as raw db, split at each
        // interior label so the label sits on its byte.
        std::vector<std::uint32_t> cuts = {a};
        for (const auto& [at, name] : options.extra_labels) {
            if (a < at && at < a + insn->size) cuts.push_back(at);
        }
        if (cuts.size() > 1) {
            cuts.push_back(a + insn->size);
            for (std::size_t k = 0; k + 1 < cuts.size(); ++k) {
                if (options.extra_labels.contains(cuts[k])) define_extra(cuts[k]);
                lines.push_back(db_line(bytes.subspan(cuts[k], cuts[k + 1] - cuts[k])));
            }
            a += insn->size;
            continue;
        }
        if (options.probe) {   // public, so it lands in the COFF symbol table
            lines.push_back(std::format("public Q{:x}", a));
            lines.push_back(std::format("Q{:x}:", a));
        }
        std::optional<std::string> line;
        if (!insn->fixup && !insn->service && !options.force_raw.contains(a)) line = masm_line(code, object, *insn);
        if (!line) {
            lines.push_back(db_line(insn->raw));
        } else {
            lines.push_back(*line);
            body.line_map[options.base_line + lines.size()] = a;
        }
        a += insn->size;
    }
    return body;
}

ObjectSource emit_object(const Emission& code, std::uint32_t object, std::string_view segment_name, const std::set<std::uint32_t>& force_raw,
                         bool probe, const std::optional<std::pair<std::uint32_t, std::uint32_t>>& range) {
    std::vector<std::string> lines = {".386p", std::format("{} segment dword public use32 'CODE'", segment_name), ""};
    BodyOptions options;
    options.force_raw = force_raw;
    options.base_line = lines.size();
    options.probe = probe;
    options.range = range;
    Body body = object_body(code, object, options);
    lines.insert(lines.end(), body.lines.begin(), body.lines.end());
    lines.push_back(std::format("{} ends", segment_name));
    lines.push_back("        end");
    lines.push_back("");
    return {text::join(lines, "\r\n"), std::move(body.line_map)};
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> split_ranges(const Emission& code, std::uint32_t object, std::size_t max_instructions) {
    std::vector<std::uint32_t> offsets;
    for (const auto& [at, insn] : code.instructions()) {
        if (at.object == object) offsets.push_back(static_cast<std::uint32_t>(at.offset));
    }
    std::vector<std::pair<std::uint32_t, std::uint32_t>> ranges;
    std::uint32_t lo = 0;
    std::size_t count = 0;
    for (const std::uint32_t a : offsets) {   // the map is ordered by place: the offsets ascend
        const std::string* label = code.label_at({object, a});
        if (count >= max_instructions && label != nullptr && label->starts_with("sub_") && a > lo) {
            ranges.emplace_back(lo, a);
            lo = a;
            count = 0;
        }
        ++count;
    }
    ranges.emplace_back(lo, static_cast<std::uint32_t>(code.code(object).size()));
    return ranges;
}

std::set<std::uint32_t> cross_range_force(const Emission& code, std::uint32_t object, std::uint32_t lo, std::uint32_t hi) {
    std::set<std::uint32_t> out;
    for (const auto& [at, insn] : code.instructions()) {
        if (at.object != object || at.offset < lo || at.offset >= hi || !is_branch(insn.mnemonic)) continue;
        const auto target = insn.direct_target();
        if (target && !(lo <= *target && *target < hi)) out.insert(static_cast<std::uint32_t>(at.offset));
    }
    return out;
}

// ---- the assemblers ---------------------------------------------------------------------------------------------------------

namespace {

namespace stdfs = std::filesystem;

constexpr std::string_view kNasmProbe = "bits 32\r\nglobal Q7X\r\nQ7X:\r\n mov eax, ebx\r\n";
constexpr std::size_t kLargeSource = 60000;   // characters: the assemblers get longer to finish

bool file_exists(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::exists(p, ec);
}

std::string text_of(const Bytes& b) { return text::from_latin1(b); }

Assembled run_assembler(const std::vector<std::string>& argv, const stdfs::path& work, const stdfs::path& object_path, std::chrono::seconds timeout,
                        std::string_view tool) {
    Assembled out;
    try {
        const process::ProcessResult r = process::run({.argv = argv, .cwd = work, .err = process::Capture::Merge, .timeout = timeout});
        out.log = text_of(r.out);
        if (file_exists(object_path)) out.object = fs::read_file(object_path);
    } catch (const Error& e) {
        out.log = std::format("{}: {}", tool, e.what());
    }
    return out;
}

Assembled assemble_with_nasm(const stdfs::path& nasm, std::string_view masm_text) {
    fs::ScopedTempDir work("cbvxdnasm");
    fs::write_file(work.path() / "m.asm", as_bytes(masm_text));
    return run_assembler({fs::to_text(nasm), "--masm", "-f", "win32", "m.asm", "-o", "m.obj"}, work.path(), work.path() / "m.obj",
                         std::chrono::seconds(masm_text.size() > kLargeSource ? 300 : 120), "nasm");
}

Assembled assemble_with_ml(const Toolchain& tools, std::string_view masm_text) {
    fs::ScopedTempDir work("cbvxdml");
    fs::write_file(work.path() / "m.asm", as_bytes(masm_text));
    return run_assembler({fs::to_text(*tools.native_ml), "-coff", "-c", "-DBLD_COFF", "-DIS_32", "-DMASM6", "-I" + fs::to_text(tools.include_dir), "-Fom.obj", "m.asm"},
                         work.path(), work.path() / "m.obj", std::chrono::seconds(masm_text.size() > kLargeSource ? 600 : 180), "ml");
}

// A program on PATH (with the .exe a Windows program has), nullopt when there is none.
std::optional<stdfs::path> find_on_path(std::string_view name) {
    const auto path = fs::get_env("PATH");
    if (!path) return std::nullopt;
    for (const std::string_view dir : text::split(*path, ';')) {
        if (dir.empty()) continue;
        for (const char* extension : {".exe", ""}) {
            const stdfs::path candidate = fs::from_text(dir) / (std::string(name) + extension);
            std::error_code ec;
            if (stdfs::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

bool force_period_ml() { return fs::get_env("CB_FORCE_ML").has_value(); }

}  // namespace

std::optional<stdfs::path> probe_nasm(const std::vector<stdfs::path>& candidates) {
    for (const stdfs::path& candidate : candidates) {
        if (candidate.empty() || !file_exists(candidate)) continue;
        try {
            fs::ScopedTempDir work("cbvxdprobe");
            fs::write_file(work.path() / "p.asm", as_bytes(kNasmProbe));
            const Assembled made = run_assembler({fs::to_text(candidate), "--masm", "-f", "win32", "p.asm", "-o", "p.obj"}, work.path(), work.path() / "p.obj",
                                                 std::chrono::seconds(30), "nasm");
            if (made.object && std::search(made.object->begin(), made.object->end(), std::string_view("Q7X").begin(), std::string_view("Q7X").end()) != made.object->end()) {
                return candidate;   // --masm accepted, COFF written, the case of PUBLIC names kept
            }
        } catch (const Error&) {
            // a candidate that cannot be probed is not the one
        }
    }
    return std::nullopt;
}

Toolchain Toolchain::standard(const DdkFolders& ddk, const stdfs::path& checkout_root, const std::optional<std::string>& nasm_setting,
                              PeriodAssembler period) {
    Toolchain tools;
    tools.period = std::move(period);
    if (const auto inc = DdkFolders::first_existing(ddk.inc)) tools.include_dir = *inc;
    if (force_period_ml()) return tools;   // the authentic period assembler, not a native one
    std::vector<stdfs::path> candidates;
    if (nasm_setting) candidates.push_back(fs::from_text(*nasm_setting));
    if (const auto env = fs::get_env("NASM_MASM")) candidates.push_back(fs::from_text(*env));
    for (const char* name : {"nasm-masm", "nasm"}) {
        if (const auto found = find_on_path(name)) candidates.push_back(*found);
    }
    tools.nasm = probe_nasm(candidates);
    const stdfs::path nt = checkout_root / "toolchains" / "period" / "sdks" / "nt351-ddk" / "DDK" / "BIN" / "I386" / "FREE" / "ML.EXE";
    if (file_exists(nt)) tools.native_ml = nt;   // the Win95 DDK's own ML is a Phar Lap TNT program: only the period assembler runs it
    return tools;
}

Assembled assemble_coff(const Toolchain& tools, std::string_view masm_text) {
    if (tools.nasm) return assemble_with_nasm(*tools.nasm, masm_text);
    if (tools.native_ml) return assemble_with_ml(tools, masm_text);
    if (tools.period) {
        const std::string one(masm_text);
        auto made = tools.period(std::span<const std::string>(&one, 1));
        if (!made.empty()) return std::move(made.front());
    }
    return {std::nullopt, "no assembler: no nasm --masm, no MASM 6.11d and no period assembler"};
}

std::vector<Assembled> assemble_coff_batch(const Toolchain& tools, std::span<const std::string> texts) {
    if (!tools.nasm && !tools.native_ml && tools.period) return tools.period(texts);
    std::vector<Assembled> out;
    out.reserve(texts.size());
    for (const std::string& t : texts) out.push_back(assemble_coff(tools, t));   // a native tool has no cold start: no batching
    return out;
}

std::vector<Assembled> assemble_macro_batch(const Toolchain& tools, std::span<const std::string> texts) {
    if (tools.native_ml) {
        std::vector<Assembled> out;
        for (const std::string& t : texts) out.push_back(assemble_with_ml(tools, t));
        return out;
    }
    if (tools.period) return tools.period(texts);
    return std::vector<Assembled>(texts.size(), Assembled{std::nullopt, "no assembler: no MASM 6.11d and no period assembler"});
}

CoffText coff_text(ByteView object) {
    io::ByteReader r(object);
    r.seek(2);
    const std::uint16_t section_count = r.u16();
    r.seek(8);
    const std::uint32_t symbol_pointer = r.u32();
    const std::uint32_t symbol_count = r.u32();
    const std::uint64_t string_table = std::uint64_t{symbol_pointer} + std::uint64_t{symbol_count} * 18;
    struct Section {
        std::size_t number;
        std::uint32_t raw_size, raw_pointer, flags;
    };
    std::vector<Section> sections;
    for (std::size_t i = 0; i < section_count; ++i) {
        r.seek(20 + i * 40 + 16);
        const std::uint32_t raw_size = r.u32(), raw_pointer = r.u32();
        r.seek(20 + i * 40 + 36);
        sections.push_back({i + 1, raw_size, raw_pointer, r.u32()});
    }
    if (sections.empty()) throw FormatError("COFF", 0, "the object has no sections");
    const auto code = std::find_if(sections.begin(), sections.end(), [](const Section& s) { return (s.flags & 0x20) && s.raw_size > 0; });
    const Section chosen = code != sections.end() ? *code : *std::max_element(sections.begin(), sections.end(), [](const Section& a, const Section& b) { return a.raw_size < b.raw_size; });
    CoffText out;
    const ByteView section_bytes = io::checked_range(object, chosen.raw_pointer, chosen.raw_size);
    out.text.assign(section_bytes.begin(), section_bytes.end());
    for (std::uint32_t i = 0; i < symbol_count;) {
        const std::uint64_t at = std::uint64_t{symbol_pointer} + std::uint64_t{i} * 18;
        const ByteView name8 = io::checked_range(object, at, 8);
        std::string name;
        if (name8[0] == 0 && name8[1] == 0 && name8[2] == 0 && name8[3] == 0) {
            const std::uint32_t offset = static_cast<std::uint32_t>(name8[4] | (name8[5] << 8) | (name8[6] << 16) | (static_cast<std::uint32_t>(name8[7]) << 24));
            const std::uint64_t from = string_table + offset;
            if (from > object.size()) throw TruncatedError("COFF", from, 1, 0);
            const auto end = std::find(object.begin() + static_cast<std::ptrdiff_t>(from), object.end(), std::uint8_t{0});
            name = text::from_latin1(ByteView(object.data() + from, static_cast<std::size_t>(end - (object.begin() + static_cast<std::ptrdiff_t>(from)))));
        } else {
            std::size_t len = 8;
            while (len > 0 && name8[len - 1] == 0) --len;
            name = text::from_latin1(name8.first(len));
        }
        io::ByteReader sym(object);
        sym.seek(at + 8);
        const std::uint32_t value = sym.u32();
        const std::int16_t section = static_cast<std::int16_t>(sym.u16());
        sym.u16();   // type
        sym.u8();    // storage class
        const std::uint8_t aux = sym.u8();
        if (section == static_cast<std::int16_t>(chosen.number)) out.symbols[name] = value;
        i += 1 + aux;
    }
    return out;
}

// ---- the byte-exact test ----------------------------------------------------------------------------------------------------

namespace {

bool equal_range(ByteView a, std::size_t a_at, ByteView b, std::size_t b_at, std::size_t n) {
    return a_at + n <= a.size() && b_at + n <= b.size() && std::equal(a.begin() + static_cast<std::ptrdiff_t>(a_at), a.begin() + static_cast<std::ptrdiff_t>(a_at + n), b.begin() + static_cast<std::ptrdiff_t>(b_at));
}

// Instructions the assembler printed an error on: the source lines of `m<N>.asm(LINE): error` and `m.asm:LINE: error`, as offsets.
std::set<std::uint32_t> failed_instructions(std::string_view log, const std::map<std::size_t, std::uint32_t>& line_map) {
    static const Regex error_line(R"(m\d*\.asm[:(](\d+)\)?:?\s*(?:error|fatal))", RegexFlags::IgnoreCase);
    std::set<std::uint32_t> bad;
    error_line.for_each_match(log, [&](const RegexMatch& m) {
        if (const auto line = text::parse_int<std::size_t>(m.group(1), 10)) {
            if (const auto it = line_map.find(*line); it != line_map.end()) bad.insert(it->second);
        }
        return true;
    });
    return bad;
}

bool is_subset(const std::set<std::uint32_t>& a, const std::set<std::uint32_t>& b) { return std::includes(b.begin(), b.end(), a.begin(), a.end()); }

}  // namespace

RangeResult converge_range(const Emission& code, std::uint32_t object, const Toolchain& tools, std::uint32_t lo, std::uint32_t hi, ByteView original,
                           std::size_t max_repair, const std::optional<FirstPass>& first) {
    std::vector<std::uint32_t> offsets;
    for (const auto& [at, insn] : code.instructions()) {
        if (at.object == object && lo <= at.offset && at.offset < hi) offsets.push_back(static_cast<std::uint32_t>(at.offset));
    }
    const std::set<std::uint32_t> in_range(offsets.begin(), offsets.end());
    std::set<std::uint32_t> force = cross_range_force(code, object, lo, hi);   // branches out of the range are byte-preserved
    const std::size_t span = hi - lo;
    const auto result_of = [&](bool match, const Bytes& text) {
        std::size_t forced = 0;
        for (const std::uint32_t f : force) forced += in_range.contains(f);
        return RangeResult{in_range.size() - forced, forced, match, force, Bytes(text.begin(), text.begin() + static_cast<std::ptrdiff_t>(std::min(text.size(), span)))};
    };
    for (std::size_t pass = 0; pass < max_repair; ++pass) {
        Assembled made;
        std::map<std::size_t, std::uint32_t> line_map;
        if (pass == 0 && first) {
            made = first->assembled;   // the batched first pass, assembled with the initial force
            line_map = first->line_map;
        } else {
            ObjectSource source = emit_object(code, object, "_LTEXT", force, true, std::make_pair(lo, hi));
            made = assemble_coff(tools, source.text);
            line_map = std::move(source.line_map);
        }
        if (!made.object) {
            const std::set<std::uint32_t> bad = failed_instructions(made.log, line_map);
            if (bad.empty() || is_subset(bad, force)) break;   // to the whole-range fallback
            force.insert(bad.begin(), bad.end());
            continue;
        }
        const CoffText coff = coff_text(*made.object);
        std::set<std::uint32_t> drifted;
        for (const std::uint32_t a : offsets) {
            if (force.contains(a)) continue;
            const auto sym = coff.symbols.find(std::format("Q{:x}", a));
            if (sym == coff.symbols.end()) continue;
            const std::size_t length = code.instruction_at({object, a})->size;
            if (!equal_range(coff.text, sym->second, original, a, length)) drifted.insert(a);
        }
        const bool exact = coff.text.size() >= span && equal_range(coff.text, 0, original, lo, span);
        if (exact || drifted.empty()) return result_of(exact, coff.text);
        force.insert(drifted.begin(), drifted.end());
    }
    force.insert(in_range.begin(), in_range.end());   // the fallback: byte-preserve the whole range
    const ObjectSource source = emit_object(code, object, "_LTEXT", force, false, std::make_pair(lo, hi));
    const Assembled made = assemble_coff(tools, source.text);
    Bytes text;
    if (made.object) text = coff_text(*made.object).text;
    const bool exact = made.object && text.size() >= span && equal_range(text, 0, original, lo, span);
    RangeResult fallback{0, in_range.size(), exact, force, {}};
    if (made.object) fallback.text.assign(text.begin(), text.begin() + static_cast<std::ptrdiff_t>(std::min(text.size(), span)));
    return fallback;
}

namespace {

std::vector<std::pair<std::uint32_t, std::uint32_t>> ranges_of(const Emission& code, std::uint32_t object, std::size_t max_instructions) {
    std::size_t count = 0;
    for (const auto& [at, insn] : code.instructions()) count += at.object == object;
    return count <= max_instructions ? std::vector<std::pair<std::uint32_t, std::uint32_t>>{{0, static_cast<std::uint32_t>(code.code(object).size())}}
                                     : split_ranges(code, object, max_instructions);
}

ObjectReport report_of(std::uint32_t object, ByteView original, const std::vector<RangeResult>& parts) {
    ObjectReport r;
    r.object = object;
    r.match = std::all_of(parts.begin(), parts.end(), [](const RangeResult& p) { return p.match; });
    r.original_length = original.size();
    for (const RangeResult& p : parts) {
        r.symbolic += p.symbolic;
        r.forced_raw += p.raw;
        r.force_raw.insert(p.force.begin(), p.force.end());
        append(r.assembled, p.text);
    }
    r.assembled_length = r.assembled.size();
    r.files = parts.size();
    r.all_raw = r.symbolic == 0 && r.symbolic + r.forced_raw > 0;
    r.assembled.resize(std::min(r.assembled.size(), original.size()));
    r.original.assign(original.begin(), original.end());
    return r;
}

}  // namespace

ObjectReport compare_object(const Emission& code, std::uint32_t object, const Toolchain& tools, std::size_t max_repair, std::size_t max_instructions) {
    const ByteView original = code.code(object);
    std::vector<RangeResult> parts;
    for (const auto& [lo, hi] : ranges_of(code, object, max_instructions)) parts.push_back(converge_range(code, object, tools, lo, hi, original, max_repair));
    return report_of(object, original, parts);
}

ModuleReport compare_module(const Emission& code, const Toolchain& tools, std::size_t max_repair, std::size_t max_instructions) {
    struct Range {
        std::uint32_t object, lo, hi;
        std::map<std::size_t, std::uint32_t> line_map;
    };
    std::vector<Range> ranges;
    std::vector<std::string> first_texts;   // each range's first-pass (probe) source
    std::vector<std::uint32_t> objects;
    for (const le::Object& o : code.driver().objects()) {
        if (!o.executable()) continue;
        objects.push_back(o.index);
        for (const auto& [lo, hi] : ranges_of(code, o.index, max_instructions)) {
            ObjectSource source = emit_object(code, o.index, "_LTEXT", cross_range_force(code, o.index, lo, hi), true, std::make_pair(lo, hi));
            ranges.push_back({o.index, lo, hi, std::move(source.line_map)});
            first_texts.push_back(std::move(source.text));
        }
    }
    std::vector<Assembled> firsts = assemble_coff_batch(tools, first_texts);
    std::map<std::uint32_t, std::vector<RangeResult>> per_object;
    for (std::size_t i = 0; i < ranges.size(); ++i) {
        const Range& r = ranges[i];
        per_object[r.object].push_back(converge_range(code, r.object, tools, r.lo, r.hi, code.code(r.object), max_repair,
                                                      FirstPass{std::move(firsts[i]), r.line_map}));
    }
    ModuleReport report;
    for (const std::uint32_t object : objects) report.objects.push_back(report_of(object, code.code(object), per_object[object]));
    report.byte_exact = std::all_of(report.objects.begin(), report.objects.end(), [](const ObjectReport& r) { return r.match; });
    for (const ObjectReport& r : report.objects) {
        report.symbolic += r.symbolic;
        report.forced_raw += r.forced_raw;
        report.all_raw_objects += r.all_raw;
    }
    return report;
}

// ---- the command ------------------------------------------------------------------------------------------------------------

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "masm",
        "assemble a driver's objects and compare them with the original",
        {cli::Positional{"file", "a VxD (LE)"},
         cli::Positional{"object", "one object (from 1); default: every executable object", false},
         cli::Option{"--module", "", "which module of a container", cli::Arg::Value, "NAME"}},
        [&](const cli::Args& a) -> int {
            auto modules = le::open_modules(fs::from_text(a.required("file")));
            if (modules.empty()) throw FormatError("LE", 0, "the file holds no driver");
            std::size_t chosen = 0;
            if (const auto name = a.value("--module")) {
                const auto found = std::find_if(modules.begin(), modules.end(), [&](const auto& m) { return m.first == *name; });
                if (found != modules.end()) chosen = static_cast<std::size_t>(found - modules.begin());
            }
            const Emission code(Disassembly::run(modules[chosen].second, env.names), env.names);
            if (const auto object = a.integer<std::uint32_t>("object")) {
                const ObjectReport r = compare_object(code, *object, env.tools);
                out("obj {}: {} (symbolic={} forced_raw={} diffs={})\n", *object, r.match ? "BYTE-EXACT" : "MISMATCH", r.symbolic, r.forced_raw, r.match ? 0 : 1);
                return r.match ? kExitOk : kExitFailed;
            }
            const ModuleReport report = compare_module(code, env.tools);
            for (const ObjectReport& r : report.objects) {
                out("  obj {:<2}  {:5} bytes  symbolic={:<5} forced_raw={:<3}  {}\n", r.object, r.original_length, r.symbolic, r.forced_raw,
                    r.match ? "BYTE-EXACT" : "MISMATCH (1 diffs)");
            }
            out("== module {}: {} (symbolic={} forced_raw={}) ==\n", fs::to_text(fs::from_text(a.required("file")).filename()),
                report.byte_exact ? "BYTE-EXACT" : "NOT byte-exact", report.symbolic, report.forced_raw);
            return report.byte_exact ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
    const cli::Cli program("masm", {command});
    std::vector<std::string> line = {"masm"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::masm
