// source/ne_asm/idiom.cpp -- a 16-bit NE code segment as cMacros source (see idiom.hpp).
// cb-unity: alone
#include "source/ne_asm/idiom.hpp"
#include "source/ne_asm/cmacros_shim.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <format>
#include <iterator>
#include <map>
#include <tuple>

#include "build/x86asm/x86asm.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "formats/omf.hpp"
#include "source/ne_asm/reachability.hpp"
#include "naming/debug_build.hpp"

namespace cb::source::ne_asm::idiom {

namespace stdfs = std::filesystem;
using segment::Row;
using segment::Seg;

namespace {

bool idiom_is_jump(std::string_view op) { return op == "jmp"; }
bool idiom_is_call(std::string_view op) { return op == "call"; }

const std::vector<std::string>& idiom_preamble() {
    // OPTION M510: MASM 5.10 compatibility enables the cMacros `&' paste AND makes .386 segments default to use16, so 16-bit code keeps its encoding
    // while 32-bit-operand (66/67-prefix) routines still assemble. .386p INSIDE the open segment: sBegin already fixed _TEXT as use16; .386p then
    // enables the 386 registers and instructions KERNEL's 32-bit-operand routines use (they get 66/67 prefixes in a use16 segment).
    static const std::vector<std::string> lines = {
        "; idiomatic recovered source -- Microsoft SDK cMacros (ne_idiom.py)",
        "OPTION M510",
        "?PLM = 1",
        "?WIN = 1",
        ".xlist",
        "include cmacros.inc",
        ".list",
        "",
        "sBegin  CODE",
        ".386p",
        "        assumes cs,CODE",
        "        assumes ds,nothing",
        "        assumes es,nothing",
        ""};
    return lines;
}

enum class Target { Function, Local, Extern };

}  // namespace

std::string_view backend_name(Backend backend) { return backend == Backend::Ml ? "ml" : "nasm"; }

const recon::Assemble& Assemblers::of(Backend backend) const {
    const recon::Assemble& assemble = backend == Backend::Ml ? ml : nasm;
    if (!assemble) throw UsageError(std::format("the {} assembler is not set up", backend_name(backend)));
    return assemble;
}

recon::Assemble nasm_assembler() {
    return [](std::string_view source) {
        // x86asm in MASM mode, in memory: `include cmacros.inc' is redirected to the shim, which is handed over as an include file
        build::x86asm::Request request;
        request.cwd = "/m";
        request.args = {"--masm", "-f", "obj", "-I", "/inc/", "-l", "m.lst", "-o", "m.obj", "/m/m.asm"};
        request.files["/m/m.asm"] = Bytes(source.begin(), source.end());
        request.files["/inc/cmacros_shim.inc"] = Bytes(kCmacrosShim.begin(), kCmacrosShim.end());
        recon::Assembled made;
        try {
            const build::x86asm::Result r = build::x86asm::assemble(request);
            made.log = r.messages;
            if (const auto at = r.files.find("m.obj"); at != r.files.end()) made.object = at->second;
            if (const auto at = r.files.find("m.lst"); at != r.files.end()) made.listing.assign(at->second.begin(), at->second.end());
        } catch (const Error& e) {
            made.log = e.what();
        }
        return made;
    };
}

recon::Assemble ml_assembler(const build::DosRunner& runner, MlTools tools) {
    return [&runner, tools = std::move(tools)](std::string_view source) {
        // the programs and the include files in a folder with no space in its path
        fs::make_dirs(tools.stage / "inc");
        std::error_code ec;
        for (const auto& [from, name] : {std::pair<stdfs::path, const char*>{tools.ml_folder / "ML.EXE", "ML.EXE"}, {tools.ml_folder / "ML.ERR", "ML.ERR"},
                                         {tools.extender, "DOSXNT.EXE"}})
            if (!stdfs::exists(tools.stage / name, ec) && stdfs::exists(from, ec)) stdfs::copy_file(from, tools.stage / name, ec);
        for (const auto& entry : stdfs::directory_iterator(tools.includes, ec))
            if (text::iequals(fs::to_text(entry.path().extension()), ".inc") && !stdfs::exists(tools.stage / "inc" / entry.path().filename(), ec))
                stdfs::copy_file(entry.path(), tools.stage / "inc" / entry.path().filename(), ec);
        build::DosRunSpec spec;
        spec.mounts = {{"c", tools.stage}};
        spec.commands = {"c:\\ml.exe /c /Ic:\\inc /Fom.obj /Flm.lst m.asm > a.txt"};
        spec.artifacts = {"m.obj", "m.lst", "a.txt"};
        spec.files["m.asm"] = std::string(source);
        spec.timeout = std::chrono::seconds(300);
        const build::DosRunResult ran = runner.run(spec);
        recon::Assembled made;
        const auto text_of = [&](const char* name) {
            const auto at = ran.made.find(name);
            return at == ran.made.end() ? std::string() : text::from_latin1(at->second);
        };
        if (const auto at = ran.made.find("m.obj"); at != ran.made.end()) made.object = at->second;
        made.listing = text_of("m.lst");
        made.log = text_of("a.txt");
        if (ran.work) {
            std::error_code ignored;
            stdfs::remove_all(*ran.work, ignored);   // (one folder a run, not left behind)
        }
        return made;
    };
}

std::string procedure_name(const Seg& seg, std::uint32_t offset) {
    std::string name = seg.name_at(offset);
    for (char& c : name)
        if (!(std::isalnum(static_cast<unsigned char>(c)) && c < 0x80) && c != '_' && c != '$' && c != '?' && c != '@') c = '_';
    if (!name.empty() && name.front() >= '0' && name.front() <= '9') name = "_" + name;
    return name.empty() ? std::format("sub_{:04x}", offset) : name;
}

Emitted emit(const Seg& seg, std::uint32_t start, std::uint32_t end, const std::optional<std::vector<std::uint32_t>>& given_functions,
             const std::set<std::uint32_t>& force_db, const std::set<std::uint32_t>& data_off) {
    std::vector<std::uint32_t> functions;
    if (given_functions) {
        functions = *given_functions;
    } else {
        for (const auto& [at, function] : seg.functions())
            if (at >= start && at < end) functions.push_back(at);
    }
    std::sort(functions.begin(), functions.end());
    // a segment the carve found no function starts in (code that does not open with `push bp'): the whole range is one function, so it is written
    // rather than dropped to an empty segment
    if (functions.empty()) functions.push_back(start);
    const std::set<std::uint32_t> function_starts(functions.begin(), functions.end());
    std::map<std::uint32_t, std::uint32_t> function_end;
    for (std::size_t i = 0; i < functions.size(); ++i) function_end[functions[i]] = i + 1 < functions.size() ? functions[i + 1] : end;

    std::vector<const Row*> rows;
    for (const Row& row : seg.rows())
        if (row.offset >= start && row.offset < end) rows.push_back(&row);
    std::map<std::uint32_t, std::size_t> row_index;
    std::vector<std::uint32_t> row_starts;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        row_index[rows[i]->offset] = i;
        row_starts.push_back(rows[i]->offset);
    }
    std::sort(row_starts.begin(), row_starts.end());
    const ByteView code = seg.code();

    // pass 1: every direct branch target, in range or not
    std::map<std::uint32_t, Target> targets;
    for (const Row* row : rows) {
        const auto branch = reachability::direct_branch(row->text);
        if (!branch) continue;
        if (function_starts.contains(branch->target)) targets[branch->target] = Target::Function;
        else if (branch->target >= start && branch->target < end) targets.emplace(branch->target, Target::Local);
        else targets[branch->target] = Target::Extern;
    }
    const auto label_of = [&](std::uint32_t target) -> std::optional<std::string> {
        const auto kind = targets.find(target);
        if (kind == targets.end()) return std::nullopt;
        return kind->second == Target::Local ? std::format("L_{:04x}", target) : procedure_name(seg, target);   // a function, or a near ref across ranges
    };
    std::set<std::uint32_t> externs, local_labels;
    for (const auto& [target, kind] : targets) {
        if (kind == Target::Extern) externs.insert(target);
        if (kind == Target::Local) local_labels.insert(target);
    }

    // pass 2: each function
    std::vector<std::string> body;
    Emitted emitted;
    const auto no_label = [](std::uint32_t) -> std::optional<std::string> { return std::nullopt; };
    if (functions.front() > start) recon::emit_db(body, code.subspan(start, functions.front() - start), start, "data/lead-in");
    for (const std::uint32_t s : functions) {
        const std::uint32_t e = function_end[s];
        const std::string name = procedure_name(seg, s);
        // near or far by the terminating instruction (retf, ret far)
        bool far = false;
        for (const Row* row : rows) {
            if (row->offset < s || row->offset >= e) continue;
            const std::string_view t = text::trim(row->text);
            if (t.starts_with("retf") || t.starts_with("ret f") || t.starts_with("lret")) far = true;
        }
        body.push_back(std::format("cProc   {},<{}>", name, far ? "FAR" : "NEAR"));
        body.push_back("cBegin  nogen");
        // Every byte of [s, e) is walked: a covered offset emits its instruction (live, symbolic, or db), and a gap is db up to the next covered offset,
        // so no byte is dropped even where objdump misdecodes a run.
        std::uint32_t pos = s;
        while (pos < e) {
            if (local_labels.contains(pos)) body.push_back(std::format("L_{:04x}:", pos));
            const auto here = row_index.find(pos);
            if (here == row_index.end()) {   // uncovered bytes: db
                const auto after = std::upper_bound(row_starts.begin(), row_starts.end(), pos);
                std::uint32_t next = std::min(after != row_starts.end() ? *after : e, e);
                if (next <= pos) next = e;
                recon::emit_db(body, code.subspan(pos, next - pos), pos, "data/uncovered");
                pos = next;
                continue;
            }
            const std::size_t i = here->second;
            const std::uint32_t off = rows[i]->offset;
            const std::string& instruction = rows[i]->text;
            const std::uint32_t next = std::min(i + 1 < rows.size() ? rows[i + 1]->offset : e, e);
            const ByteView raw = code.subspan(off, next - off);
            const auto branch = reachability::direct_branch(instruction);
            const bool far_instruction = !raw.empty() && (raw[0] == 0x9A || raw[0] == 0xEA);
            if (force_db.contains(off) || data_off.contains(off) || far_instruction || instruction.find("(bad)") != std::string::npos || raw.size() > 8) {
                // pinned as data: a diverger, a safe data region, a far seg:off pointer, or a run objdump folded into one row
                recon::emit_db(body, raw, off, recon::to_masm(instruction, no_label));
                pos = next;
                continue;
            }
            std::optional<std::string> line;
            if (branch) {
                if (const auto label = label_of(branch->target)) {
                    const bool is_short = raw.size() == 2;   // eb / 7x rel8
                    if (idiom_is_jump(branch->op)) line = is_short ? "jmp     short " + *label : "jmp     " + *label;
                    else if (idiom_is_call(branch->op)) line = "call    " + *label;
                    else line = std::format("{:<7} {}", branch->op, is_short ? "short " + *label : *label);   // a jcc: rel8 short, rel16 needs .386
                    if (targets[branch->target] == Target::Extern)   // the displacement bytes are unresolved
                        for (std::uint32_t k = off + 1; k < next; ++k) emitted.unresolved.insert(k);
                }
            }
            body.push_back(std::format("        {:<30} ; {:04x}", line ? *line : recon::to_masm(instruction, no_label), off));
            pos = next;
        }
        body.push_back("cEnd    nogen");
        body.push_back("");
    }
    std::vector<std::string> all = idiom_preamble();
    all.insert(all.end(), body.begin(), body.end());
    for (const std::uint32_t t : externs) all.push_back("        externNP " + procedure_name(seg, t));
    all.push_back("sEnd    CODE");
    all.push_back("        end");
    all.push_back("");
    emitted.source = text::join(all, "\n");
    return emitted;
}

Reconciled reconcile_range(const Seg& seg, std::uint32_t start, std::uint32_t end, const Assemblers& assemblers, Backend backend,
                           const std::optional<std::vector<std::uint32_t>>& functions, std::size_t max_passes) {
    const recon::Assemble& assemble = assemblers.of(backend);
    const ByteView shipped = seg.code().subspan(start, end - start);
    const segdiff::Mask shipped_mask;   // (a carved segment keeps no relocation mask of its own)
    const std::vector<recon::DataSpan> data_spans = recon::safe_data_spans(seg, start, end);
    std::set<std::uint32_t> data_off;
    for (const recon::DataSpan& span : data_spans)
        for (const Row& row : seg.rows())
            if (row.offset >= span.start && row.offset < span.end) data_off.insert(row.offset);
    std::vector<std::uint32_t> rows;
    for (const Row& row : seg.rows())
        if (row.offset >= start && row.offset < end && !data_off.contains(row.offset)) rows.push_back(row.offset);
    std::sort(rows.begin(), rows.end());
    std::map<std::uint32_t, std::uint32_t> spans;
    for (std::size_t i = 0; i < rows.size(); ++i) spans[rows[i]] = i + 1 < rows.size() ? rows[i + 1] : end;
    std::vector<std::uint32_t> function_list;
    if (functions) {
        for (const std::uint32_t f : *functions)
            if (f >= start && f < end) function_list.push_back(f);
    } else {
        for (const auto& [at, function] : seg.functions())
            if (at >= start && at < end) function_list.push_back(at);
    }
    std::sort(function_list.begin(), function_list.end());
    // the function [s, e) holding an offset, for writing a whole function as db
    const auto function_of = [&](std::uint32_t at) -> std::optional<std::pair<std::uint32_t, std::uint32_t>> {
        const auto after = std::upper_bound(function_list.begin(), function_list.end(), at);
        if (after == function_list.begin()) return std::nullopt;
        const auto it = after - 1;
        return std::pair{*it, after != function_list.end() ? *after : end};
    };

    Reconciled result;
    std::set<std::uint32_t> errored;
    for (std::size_t pass = 0; pass < max_passes; ++pass) {
        const Emitted emitted = emit(seg, start, end, functions, result.force_db, data_off);
        result.source = emitted.source;
        const std::vector<std::string_view> source_lines = text::lines(result.source);
        const recon::Assembled made = assemble(result.source);
        // an error of the assembler is localised to the original offset in the `; NNNN` of its source line, and just that instruction is db: it is
        // data the assembler cannot encode
        std::set<std::uint32_t> errs;
        const std::string messages = made.log + "\n" + made.listing;
        for (const std::size_t line : backend == Backend::Nasm ? recon::nasm_error_lines(messages) : recon::masm_error_lines(messages))
            if (line >= 1 && line <= source_lines.size())
                if (const auto at = recon::comment_offset(source_lines[line - 1])) errs.insert(*at);
        if (!made.object || made.object->empty() || !errs.empty()) {
            std::set<std::uint32_t> fresh;
            std::set_difference(errs.begin(), errs.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
            if (!fresh.empty()) {
                result.force_db.insert(fresh.begin(), fresh.end());
                errored.insert(fresh.begin(), fresh.end());
                continue;
            }
            if (result.force_db.size() >= rows.size()) break;
            result.force_db = std::set<std::uint32_t>(rows.begin(), rows.end());
            continue;
        }
        const formats::omf::CodeSegment ours = formats::omf::code_segment(*made.object);
        segdiff::Mask our_mask = ours.fixup_mask;
        for (const std::uint32_t u : emitted.unresolved) our_mask.insert(u - start);
        result.verdict = segdiff::diff(ours.code, our_mask, shipped, shipped_mask);
        if (result.verdict->exact) break;
        const recon::Divergers diverged =
            backend == Backend::Nasm
                ? recon::divergers_of(recon::nasm_listing_pairs(made.listing), ours.code, our_mask, shipped, shipped_mask, spans, start)
                : recon::divergers_of(recon::masm_listing_pairs(made.listing), ours.code, our_mask, shipped, shipped_mask, spans, start);
        std::set<std::uint32_t> fresh;
        std::set_difference(diverged.length.begin(), diverged.length.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
        if (!fresh.empty()) {   // phase 1: the instructions of another length, first
            result.force_db.insert(fresh.begin(), fresh.end());
            continue;
        }
        std::set_difference(diverged.re_encode.begin(), diverged.re_encode.end(), result.force_db.begin(), result.force_db.end(), std::inserter(fresh, fresh.end()));
        if (!fresh.empty()) {   // phase 2: the same-length re-encodings
            result.force_db.insert(fresh.begin(), fresh.end());
            continue;
        }
        // The last resort: the listing cannot place the difference (a misdecoded region, such as an inline jump table objdump read as code). The whole
        // function holding the first difference is db -- pure db is byte-exact in a contiguous emission -- which realigns everything after it.
        if (result.verdict->first_diff) {
            if (const auto function = function_of(start + static_cast<std::uint32_t>(*result.verdict->first_diff))) {
                std::set<std::uint32_t> whole;
                for (const std::uint32_t o : rows)
                    if (o >= function->first && o < function->second) whole.insert(o);
                std::set<std::uint32_t> more;
                std::set_difference(whole.begin(), whole.end(), result.force_db.begin(), result.force_db.end(), std::inserter(more, more.end()));
                if (!more.empty()) {
                    result.force_db.insert(whole.begin(), whole.end());
                    continue;
                }
            }
        }
        break;
    }
    std::set<std::uint32_t> kept;
    std::set_difference(result.force_db.begin(), result.force_db.end(), errored.begin(), errored.end(), std::inserter(kept, kept.end()));
    result.force_db = std::move(kept);
    result.code_instructions = static_cast<std::size_t>(std::count_if(rows.begin(), rows.end(), [&](std::uint32_t o) { return !errored.contains(o); }));
    return result;
}

Verified verify_range(const Seg& seg, std::uint32_t start, std::uint32_t end, const Assemblers& assemblers, Backend backend,
                      const std::optional<std::vector<std::uint32_t>>& functions) {
    Reconciled r = reconcile_range(seg, start, end, assemblers, backend, functions);
    Verified out;
    out.ok = r.verdict && r.verdict->exact;
    out.bytes = end - start;
    out.code_instructions = r.code_instructions;
    out.pinned = r.force_db.size();
    out.live = r.code_instructions - r.force_db.size();
    out.source = std::move(r.source);
    if (!out.ok) out.verdict = r.verdict;
    return out;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "idiom",
        "a range of a code segment as SDK cMacros source, byte-exact",
        {cli::Positional{"module", "a corpus name or a path (default KRNL386.EXE)", false},
         cli::Positional{"seg", "the code segment (from 1; default 1)", false},
         cli::Positional{"lo", "the first offset (default 0x04a4)", false},
         cli::Positional{"hi", "the end offset, or `whole` (default 0x4fa)", false},
         cli::Option{"--backend", "", "the assembler: ml (the period ML under DOS) or nasm (x86asm, the built-in NASM MASM mode)", cli::Arg::Value, "ml|nasm", "ml"},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const std::string module_name = a.value_or("module", "KRNL386.EXE");
            const std::size_t seg_index = a.integer<std::size_t>("seg").value_or(1);
            const std::uint32_t lo = a.integer<std::uint32_t>("lo").value_or(0x04a4);
            const std::string hi_text = a.value_or("hi", "0x4fa");
            const std::string backend_text = a.value_or("backend", "ml");
            if (backend_text != "ml" && backend_text != "nasm") throw UsageError("--backend is ml or nasm");
            const Backend backend = backend_text == "ml" ? Backend::Ml : Backend::Nasm;
            const stdfs::path path = env.corpus.retail(module_name).value_or(fs::from_text(module_name));
            const formats::ne::Module module = fixups::open_module(path);
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            // real internal names, where a names.json exists for the module (`debug-names`): a pure label rename, byte-exactness is unaffected
            std::map<std::size_t, segment::NameOverrides> overrides;
            const std::string tag = text::lower(fs::to_text(fs::from_text(module_name).stem()));
            const stdfs::path names_json = env.state / "idiomatic" / tag / "names.json";
            std::error_code ec;
            if (stdfs::is_regular_file(names_json, ec)) {
                overrides = naming::debug_build::parse_overrides(fs::read_text(names_json));
                std::size_t count = 0;
                for (const auto& [seg, names] : overrides) count += names.size();
                out("applying {} recovered real names from {}\n", count, fs::to_text(names_json));
            }
            const auto& segments = module.segments();
            const auto found = std::find_if(segments.begin(), segments.end(), [&](const formats::ne::Segment& s) { return s.index == seg_index && !s.is_data(); });
            if (found == segments.end()) {
                out("no code segment {}\n", seg_index);
                return kExitFailed;
            }
            const auto named = overrides.find(seg_index);
            const Seg seg(module, seg_index, ordinals, named != overrides.end() ? named->second : segment::NameOverrides{}, env.objdump);
            const std::uint32_t hi = hi_text == "whole" ? static_cast<std::uint32_t>(seg.code().size())
                                                         : text::parse_int<std::uint32_t>(hi_text, 0).value_or(0);
            const Verified r = verify_range(seg, lo, hi, env.assemblers, backend);
            out("=== {} seg{} [0x{:x},0x{:x}) ===\n", module_name, seg_index, lo, hi);
            if (r.ok) {
                const double percent = r.code_instructions != 0 ? 100.0 * static_cast<double>(r.live) / static_cast<double>(r.code_instructions) : 100.0;
                out("*** BYTE-EXACT ({} bytes) -- {}/{} instructions live idiomatic, {} db-pinned ({:.1f}% live) ***\n", r.bytes, r.live, r.code_instructions, r.pinned, percent);
                if (const auto save = fs::get_env("NE_SAVE"); save && !save->empty()) {
                    const stdfs::path target = fs::from_text(*save);
                    if (!target.parent_path().empty()) fs::make_dirs(target.parent_path());
                    fs::write_text(target, text::replace_all(r.source, "\n", "\r\n"));
                    out("saved idiomatic source -> {} ({} lines)\n", *save, std::count(r.source.begin(), r.source.end(), '\n'));
                }
            } else {
                out("NOT exact: ok=false n={} n_code={} db={} live={}", r.bytes, r.code_instructions, r.pinned, r.live);
                if (r.verdict)
                    out(" res={{exact: false, length_match: {}, our_len: {}, shipped_len: {}, n_diff: {}, first_diff: {}}}", r.verdict->length_match, r.verdict->our_len,
                        r.verdict->shipped_len, r.verdict->n_diff, r.verdict->first_diff ? std::to_string(*r.verdict->first_diff) : "none");
                out("\n");
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("idiom", {command});
    std::vector<std::string> line = {"idiom"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::idiom
