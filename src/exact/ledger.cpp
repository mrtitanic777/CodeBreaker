#include "exact/ledger.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "exact/omf_object.hpp"
#include "formats/omf.hpp"

namespace cb::exact::ledger {

namespace stdfs = std::filesystem;
namespace omf = formats::omf;

namespace {

constexpr std::string_view kLedgerSameFile = "/* SAMEFILE */";
constexpr std::string_view kLedgerFirstProcedure = "first procedure";
constexpr std::size_t kLedgerAgreeWindow = 256;   // how many bytes of a candidate start are held to the program
constexpr std::uint8_t kLedgerNearJump = 0xE9;
const Bytes kLedgerNoBytes;

bool ledger_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool ledger_is_blank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

// A name as the stubs are kept: upper-case, without its leading underscores.
std::string ledger_bare_upper(std::string_view name) {
    while (name.starts_with('_')) name.remove_prefix(1);
    return text::upper(name);
}

bool ledger_symbol_is_code(const SymbolTable& symbols, const std::string& name) {
    const SymPlace* p = symbols.find(name);
    return p != nullptr && (p->kind == SymKind::Code || p->kind == SymKind::Unknown);   // (a place whose kind is not said counts)
}

std::string ledger_note_text(const compare::Note& n) { return n.severity == compare::Severity::Warning ? "? " + n.text : n.text; }

// The place of a symbol as an address of the program, none when it lies outside one.
std::optional<std::int64_t> ledger_place_of(const SymbolTable& symbols, const std::string& name) {
    if (!ledger_symbol_is_code(symbols, name)) return std::nullopt;
    return symbols.at(name).address;
}

std::optional<std::uint32_t> ledger_as_address(std::int64_t a) {
    if (a < 0 || a > 0xFFFFFFFFll) return std::nullopt;
    return static_cast<std::uint32_t>(a);
}

// A function the compiler cut out of the module: its place in the program, and where in the code segment it lies.
struct LedgerPlace {
    std::int64_t offset = 0, length = 0;
    std::optional<std::int64_t> address;
};

// The module's start, or the start of one of its frames (push bp; mov bp, sp): of these the one whose bytes agree
// with the program at `at` - the file is named after a procedure of it, its first frame as a rule, and the main
// module has code before it. Gives how far into the code `at` is.
std::int64_t ledger_first_frame_offset(ByteView code, const std::set<std::int64_t>& holes, ByteView image, std::int64_t at) {
    static constexpr std::uint8_t kFrame[3] = {0x55, 0x8B, 0xEC};
    std::vector<std::int64_t> candidates{0};
    for (std::size_t i = 0; i + sizeof(kFrame) <= code.size();) {
        if (std::memcmp(code.data() + i, kFrame, sizeof(kFrame)) == 0) {
            candidates.push_back(static_cast<std::int64_t>(i));
            i += sizeof(kFrame);
        } else {
            ++i;
        }
    }
    const auto agree = [&](std::int64_t k0) -> std::int64_t {
        const std::int64_t b = at - k0;
        if (b < 0) return -1;
        std::int64_t n = 0;
        const std::size_t window = std::min(code.size(), kLedgerAgreeWindow);
        for (std::size_t x = 0; x < window; ++x) {
            if (holes.contains(static_cast<std::int64_t>(x))) continue;
            const std::uint64_t pos = static_cast<std::uint64_t>(b) + x;
            if (pos < image.size() && image[pos] == code[x]) ++n;
        }
        return n;
    };
    std::int64_t best = candidates.front(), best_agree = agree(best);
    for (std::size_t i = 1; i < candidates.size(); ++i) {   // (ties go to the earlier start)
        const std::int64_t a = agree(candidates[i]);
        if (a > best_agree) best = candidates[i], best_agree = a;
    }
    return best;
}

}  // namespace

std::set<std::string> stubs(std::string_view source) {
    // A function written `name(args) ... { }` after the marker. Every word is tried as a name: a word, blanks, '(',
    // up to the first ')', anything but '{' or ';' up to a '{', blanks, '}'.
    std::set<std::string> out;
    const std::size_t marker = source.find(kLedgerSameFile);
    if (marker == std::string_view::npos) return out;
    const std::string_view s = source.substr(marker);
    std::size_t i = 0;
    while (i < s.size()) {
        if (!ledger_is_word(s[i])) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < s.size() && ledger_is_word(s[e])) ++e;
        std::size_t p = e;
        while (p < s.size() && ledger_is_blank(s[p])) ++p;
        std::size_t next = e;   // where scanning goes on when this word is no stub
        if (p < s.size() && s[p] == '(') {
            const std::size_t close = s.find(')', p + 1);
            if (close != std::string_view::npos) {
                const std::size_t stop = s.find_first_of("{;", close + 1);
                if (stop != std::string_view::npos && s[stop] == '{') {
                    std::size_t q = stop + 1;
                    while (q < s.size() && ledger_is_blank(s[q])) ++q;
                    if (q < s.size() && s[q] == '}') {
                        out.insert(ledger_bare_upper(s.substr(i, e - i)));
                        next = q + 1;
                    }
                }
            }
        }
        i = next;
    }
    return out;
}

bool reaches(std::span<const std::uint32_t> starts, std::uint32_t code_end, ByteView image, std::uint32_t at, std::uint32_t length) {
    const auto next = std::upper_bound(starts.begin(), starts.end(), at);
    // the nearest of the next start and the code's end; the function's own end when neither is known
    std::uint64_t end = static_cast<std::uint64_t>(at) + length;
    bool known = false;
    if (next != starts.end()) end = *next, known = true;
    if (code_end != 0) end = known ? std::min<std::uint64_t>(end, code_end) : code_end, known = true;
    const std::uint64_t e = static_cast<std::uint64_t>(at) + length;
    if (e + (e & 1) == end + (end & 1)) return true;
    // or a pad of zeros (at most 16 bytes) up to it
    if (e >= image.size() || end <= e) return false;
    const std::uint64_t stop = std::min<std::uint64_t>(end, image.size());
    const std::uint64_t pad = stop - e;
    return pad > 0 && pad <= 16 && std::all_of(image.begin() + static_cast<std::ptrdiff_t>(e), image.begin() + static_cast<std::ptrdiff_t>(stop),
                                                [](std::uint8_t b) { return b == 0; });
}

std::vector<Row> check_file(const Project& project, const Toolchain& toolchain, const stdfs::path& file, std::optional<std::int64_t> at,
                            SymbolTable symbols, SymbolTable& learnt) {
    const Bytes source = fs::read_file(file);
    const std::string file_name = fs::to_text(file.filename());
    const ByteView image = project.image();
    const std::optional<std::uint32_t> here = at ? ledger_as_address(*at) : std::nullopt;

    modules::CompileOptions options;
    options.headers = project.headers();
    options.include = project.include();
    options.switches = project.switches();
    const modules::CompileResult compiled = toolchain.compile(source, options);
    if (!compiled.object) {
        Row r;
        r.file = file_name;
        r.address = here;
        r.verdict = Verdict::DoesNotCompile;
        const std::vector<std::string_view> said = text::lines(text::trim(compiled.console));
        if (!said.empty()) r.notes.emplace_back(said.back());
        return {r};
    }

    const omf::Object parsed = omf::read(*compiled.object);
    const ObjectFile object = object_of(parsed, *compiled.object);
    const std::set<std::string> same_file = stubs(as_text(source));
    const std::vector<std::uint32_t> starts = project.starts();
    const std::uint32_t code_end = project.code_end();
    const x86::Decoder16& decoder = project.decoder();
    const compare::Program program = project.program();
    const bool named_by_first_procedure = project.config_text("named_by").value_or("") == kLedgerFirstProcedure;

    std::vector<Row> out;
    SymbolTable got;   // what the functions bound or proved: names and their places
    const std::vector<std::string> segments = omf::code_segments(parsed);   // (#pragma alloc_text: more code segments than one)
    for (std::size_t ti = 0; ti < segments.size(); ++ti) {
        const std::string& segment = segments[ti];
        const std::vector<omf::FunctionSpan> fns = omf::functions(parsed, segment);
        std::optional<std::int64_t> first;
        if (ti == 0 && at && *at >= 0) first = *at;
        if (!first) {   // (an assembly module placed by its first name SYMBOLS knows)
            for (const omf::FunctionSpan& f : fns) {
                if (const auto place = ledger_place_of(symbols, f.name)) {
                    first = *place - f.offset;
                    break;
                }
            }
            if (!first && ti == 0 && at && *at < 0) continue;   // (nothing to place it by)
        }
        const bool together = project.whole(file) && first.has_value();   // (a whole source file: its text lies in one piece)
        if (together && ti == 0 && named_by_first_procedure && at && *at >= 0) {
            const Bytes* code_all = object.seg.find(segment);
            const std::uint32_t* filled = object.filled.find(segment);
            const ByteView code = code_all ? ByteView(*code_all).first(std::min<std::size_t>(filled ? *filled : 0, code_all->size())) : ByteView();
            first = *at - ledger_first_frame_offset(code, omf::holes(parsed, segment), image, *at);
        }
        std::vector<LedgerPlace> places;
        for (std::size_t k = 0; k < fns.size(); ++k) {
            LedgerPlace p{fns[k].offset, fns[k].length, std::nullopt};
            if (k == 0 && first) p.address = first;
            else if (together) p.address = *first - fns[0].offset + fns[k].offset;
            else p.address = ledger_place_of(symbols, fns[k].name);
            places.push_back(p);
        }
        const compare::Where where = [&](std::uint32_t x) -> std::int64_t {
            for (const LedgerPlace& p : places) {
                if (p.address && p.offset <= x && x < p.offset + p.length) return *p.address + x - p.offset;
            }
            return first ? *first - fns.front().offset + x : -1;
        };
        const Bytes* code_in_object = object.seg.find(segment);
        const Bytes& code = code_in_object ? *code_in_object : kLedgerNoBytes;
        for (std::size_t k = 0; k < fns.size(); ++k) {
            const omf::FunctionSpan& f = fns[k];
            if (same_file.contains(ledger_bare_upper(f.name))) continue;
            const auto length = static_cast<std::uint32_t>(f.length);
            Row row;
            row.file = file_name;
            row.function = f.name;
            row.length = length;
            const std::optional<std::uint32_t> a = places[k].address ? ledger_as_address(*places[k].address) : std::nullopt;
            if (!a) {
                row.verdict = Verdict::NoPlace;
                out.push_back(std::move(row));
                continue;
            }
            row.address = a;
            const compare::Subject subject{object, f.name, static_cast<std::uint32_t>(f.offset), length, *a, segment};
            compare::Report report = compare::function(subject, image, program, symbols, got, where, decoder);
            // the assembler's pad to an even length: the last byte is the next module's first
            if (report.bad.size() == 1 && report.bad[0] == length - 1 && k + 1 == fns.size() && static_cast<std::uint64_t>(f.offset) + length == code.size() &&
                code.size() % 2 == 0 && code.back() == 0 && length >= 4 && code[code.size() - 4] == kLedgerNearJump) {
                report.bad.clear();
                report.notes.push_back({compare::Severity::Warning, "the last byte is the pad to an even length; the next module begins there"});
            }
            if (!starts.empty() && !reaches(starts, code_end, image, *a, length))
                report.notes.push_back({compare::Severity::Error, std::format("ends at {:05x}, before the next function's start", static_cast<std::uint64_t>(*a) + length)});
            for (const compare::Note& n : report.notes) row.notes.push_back(ledger_note_text(n));
            const bool same = report.identical();
            row.verdict = same ? Verdict::Identical : Verdict::Differs;
            out.push_back(std::move(row));
            if (same) got.insert_or_assign(f.name, SymPlace{SymKind::Code, *a});
        }
    }
    if (std::ranges::all_of(out, [](const Row& r) { return !r.address || r.verdict == Verdict::Identical; })) {
        for (const auto& [name, place] : got)
            if (name.find(':') == std::string::npos) learnt.insert_or_assign(name, place);
    }
    return out;
}

std::vector<Row> merge(const std::vector<Attempt>& per) {
    std::vector<Row> rows;
    if (per.empty()) return rows;
    const Attempt* base = &per.front();   // (one that compiled, if any did: the longest)
    for (const Attempt& a : per)
        if (a.rows.size() > base->rows.size()) base = &a;
    for (std::size_t k = 0; k < base->rows.size(); ++k) {
        std::vector<std::string> by;
        const Row* identical = nullptr;
        const Row* compiled = nullptr;
        for (const Attempt& a : per) {
            if (k >= a.rows.size()) continue;
            const Row& r = a.rows[k];
            if (r.verdict == Verdict::Identical) {
                by.push_back(a.label);
                if (!identical) identical = &r;
            }
            if (r.verdict != Verdict::DoesNotCompile && !compiled) compiled = &r;
        }
        // the first that compiled says what is wrong
        Row best = identical ? *identical : compiled ? *compiled : base->rows[k];
        best.by = std::move(by);
        rows.push_back(std::move(best));
    }
    return rows;
}

std::vector<Row> run(const Project& project, bool learn, bool verbose) {
    if (const pascal_join::Project* pascal = project.compilands()) {   // (a Pascal program in pieces: exact/pascal_join)
        std::vector<Row> rows;
        const std::optional<std::string> label = project.config_text("toolchain");
        for (const pascal_join::Row& r : pascal_join::run(*pascal, verbose)) {
            Row row{r.label, r.procedure, r.address, r.length, r.verdict, r.notes, {}};
            if (r.verdict == Verdict::Identical && label) row.by.push_back(*label);
            rows.push_back(std::move(row));
        }
        return rows;
    }
    const std::vector<LabelledToolchain> toolchains = project.toolchains();
    const SymbolTable symbols = project.symbols();
    SymbolTable learnt;
    std::vector<Row> rows;
    for (const SourceFile& source : project.sources()) {
        std::vector<Attempt> per;
        for (const LabelledToolchain& tc : toolchains)
            per.push_back({tc.label, check_file(project, *tc.toolchain, source.path, source.at, symbols, learnt)});
        std::vector<Row> merged = merge(per);
        if (verbose) {
            for (const Row& r : merged) {
                std::string notes;
                for (std::size_t i = 0; i < std::min<std::size_t>(2, r.notes.size()); ++i) notes += (i ? "; " : "") + r.notes[i];
                out("{:<10} {:<24} {} {} {}\n", r.file, r.function.value_or("-"), pascal_join::verdict_text(r.verdict), text::join(r.by, ","), notes);
            }
        }
        rows.insert(rows.end(), std::make_move_iterator(merged.begin()), std::make_move_iterator(merged.end()));
    }
    if (learn && !learnt.empty()) {
        SymbolTable all = project.symbols();
        for (const auto& [name, place] : learnt) all.try_emplace(name, place);   // (a name SYMBOLS has keeps its place)
        project.save_symbols(all);
    }
    return rows;
}

void write(const Project& project, const std::vector<Row>& rows) {
    const bool several = project.toolchains().size() > 1;
    std::string out_text = std::format("# file, function, bytes, compared with {}, the toolchains that give it (codebreaker exact ledger)\n",
                                       fs::to_text(stdfs::path(project.exe()).filename()));
    for (const Row& r : rows) {
        out_text += std::format("{} {} {} {}", r.file, r.function.value_or("-"), r.length, pascal_join::verdict_text(r.verdict));
        if (!r.by.empty() && several) out_text += " " + text::join(r.by, ",");
        out_text += '\n';
    }
    fs::write_text(project.path("LEDGER"), out_text);
}

int command(const Project& project, const std::vector<std::string>& args) {
    const auto has = [&](std::string_view flag) { return std::ranges::find(args, flag) != args.end(); };
    const std::vector<std::shared_ptr<const Project>> sets = project.sets();
    std::vector<Row> rows;
    for (const auto& part : sets) {   // (each set of sources with its own toolchain)
        std::vector<Row> r = run(*part, has("--learn"), has("-v"));
        if (sets.size() > 1) {
            const auto same = std::ranges::count_if(r, [](const Row& x) { return x.verdict == Verdict::Identical; });
            stdfs::path rel = part->src().lexically_relative(project.dir());
            if (rel.empty()) rel = part->src();
            out("   {} ({}): {} functions, {} identical\n", fs::to_text(rel), part->config_text("toolchain").value_or("default"), r.size(), same);
        }
        rows.insert(rows.end(), std::make_move_iterator(r.begin()), std::make_move_iterator(r.end()));
    }
    if (has("--write")) write(project, rows);
    const auto count = [&](Verdict v) { return std::ranges::count_if(rows, [&](const Row& r) { return r.verdict == v; }); };
    const auto equivalent = count(Verdict::Equivalent);
    out("{} functions, {} identical, {}{} differ, {} no place, {} files do not compile\n", rows.size(), count(Verdict::Identical),
        equivalent ? std::format("{} the same code by another compiler state's habits, ", equivalent) : std::string(), count(Verdict::Differs),
        count(Verdict::NoPlace), count(Verdict::DoesNotCompile));
    const std::vector<LabelledToolchain> toolchains = project.toolchains();
    if (toolchains.size() > 1) {
        for (const LabelledToolchain& tc : toolchains) {
            const auto gives = std::ranges::count_if(rows, [&](const Row& r) { return std::ranges::find(r.by, tc.label) != r.by.end(); });
            const auto alone = std::ranges::count_if(rows, [&](const Row& r) { return r.by.size() == 1 && r.by[0] == tc.label; });
            out("   {:<10} gives {} ({} of them it alone)\n", tc.label, gives, alone);
        }
    }
    return 0;
}

}  // namespace cb::exact::ledger
