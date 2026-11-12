// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/vxd/link.hpp"

#include <algorithm>
#include <format>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/source/vxd/le_writer.hpp"

namespace cb::source::vxd::link {

namespace le = formats::le;
namespace stdfs = std::filesystem;

namespace {

// the last `n` characters of a log: what the stage that failed said last
std::string tail(std::string_view log, std::size_t n) { return std::string(log.size() > n ? log.substr(log.size() - n) : log); }

Linked run_linker(const stdfs::path& program, const std::vector<std::string>& arguments, ByteView object, std::string_view def_text, const stdfs::path& work,
                  const std::map<std::string, std::string>& env_overrides, const char* name) {
    fs::write_file(work / "m.obj", object);
    fs::write_file(work / "m.def", as_bytes(def_text));
    Linked out;
    try {
        std::vector<std::string> argv = {fs::to_text(program)};
        argv.insert(argv.end(), arguments.begin(), arguments.end());
        const process::ProcessResult r = process::run({.argv = argv, .cwd = work, .env = env_overrides, .err = process::Capture::Merge, .timeout = std::chrono::seconds(120)});
        out.log = text::from_latin1(r.out);
    } catch (const Error& e) {
        out.log = std::format("{} exec failed: {}", name, e.what());
        return out;
    }
    std::error_code ec;
    if (stdfs::exists(work / "m.vxd", ec)) out.vxd = fs::read_file(work / "m.vxd");
    return out;
}

std::string label_of(const Emission& code, const Place& at) {
    const std::string* label = code.label_at(at);
    return label ? *label : std::string();
}

const le::InternalFixup* internal_at(const Emission& code, const Place& at) {
    const le::Fixup* f = code.fixups().find(at);
    return f ? std::get_if<le::InternalFixup>(f) : nullptr;
}

std::size_t internal_count(const le::Module& m) {
    std::size_t n = 0;
    for (const auto& [at, f] : m.fixups()) n += std::holds_alternative<le::InternalFixup>(f);
    return n;
}

}  // namespace

LinkFn standard_link(stdfs::path link_exe) {
    return [link_exe = std::move(link_exe)](ByteView object, std::string_view def_text, const stdfs::path& work) {
        return run_linker(link_exe, {"/VXD", "/NOD", "/OUT:m.vxd", "/DEF:m.def", "m.obj"}, object, def_text, work, {}, "link");
    };
}

LinkFn link_260(stdfs::path msvc20_folder, stdfs::path stage_folder, std::uint32_t align) {
    return [msvc20 = std::move(msvc20_folder), stage = std::move(stage_folder), align](ByteView object, std::string_view def_text, const stdfs::path& work) {
        fs::make_dirs(stage);
        for (const char* file : {"LINK.EXE", "DBI.DLL", "LINK.ERR", "IMAGEHLP.DLL"}) {
            std::error_code ec;
            if (!stdfs::exists(stage / file, ec)) stdfs::copy_file(msvc20 / file, stage / file, ec);
        }
        const std::string path_now = fs::get_env("PATH").value_or("");
        const std::map<std::string, std::string> env = {{"PATH", fs::to_text(stage) + ";" + path_now}};   // the self-spawn and the DLLs
        return run_linker(stage / "LINK.EXE", {"/VXD", "/NOD", std::format("/ALIGN:0x{:x}", align), "/OUT:m.vxd", "/DEF:m.def", "m.obj"}, object, def_text, work, env,
                          "link260");
    };
}

// ---- the source and the .DEF ------------------------------------------------------------------------------------------------

const std::string_view kVxdDefSegments =
    "    _LPTEXT     CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LTEXT      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LDATA      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TEXT       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _DATA       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    CONST       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TLS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _BSS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _ITEXT      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _IDATA      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _PTEXT      CLASS 'PCODE'   NONDISCARDABLE\r\n"
    "    _PDATA      CLASS 'PDATA'   NONDISCARDABLE SHARED\r\n"
    "    _STEXT      CLASS 'SCODE'   RESIDENT\r\n"
    "    _SDATA      CLASS 'SCODE'   RESIDENT\r\n"
    "    _16ICODE    CLASS '16ICODE' PRELOAD DISCARDABLE\r\n"
    "    _RCODE      CLASS 'RCODE'\r\n";

std::string segment_attributes(std::uint32_t flags) {
    std::vector<std::string> a;
    if (flags & 0x40) a.push_back("PRELOAD");
    a.push_back(flags & 0x10 ? "DISCARDABLE" : "NONDISCARDABLE");
    if (flags & 0x20) a.push_back("SHARED");
    if (flags & 0x8000) a.push_back("IOPL");
    if (flags & 0x4000) a.push_back("CONFORMING");
    if (flags & 0x200) a.push_back("RESIDENT");
    return text::join(a, " ");
}

std::vector<std::string> link_body(const Emission& code, std::uint32_t object, const std::map<std::uint32_t, std::string>& extra_labels) {
    const ByteView bytes = code.code(object);
    const std::uint32_t n = static_cast<std::uint32_t>(bytes.size());
    std::vector<std::string> lines;
    // the label an internal fix-up at `k` points to, or none
    const auto relocation = [&](std::uint32_t k) -> std::string {
        const le::InternalFixup* f = internal_at(code, {object, k});
        return f != nullptr && f->target ? label_of(code, {f->object, *f->target}) : std::string();
    };
    std::uint32_t a = 0;
    while (a < n) {
        if (const auto extra = extra_labels.find(a); extra != extra_labels.end()) {
            lines.push_back("public " + extra->second);
            lines.push_back(extra->second + " label byte");
        }
        if (const std::string* label = code.label_at({object, a})) lines.push_back(*label + ":");
        const Instruction* row = code.instruction_at({object, a});
        if (row == nullptr) {   // data
            if (const std::string label = relocation(a); !label.empty()) {
                lines.push_back("        dd  offset " + label);
                a += 4;
                continue;
            }
            std::uint32_t b = a;   // raw bytes to the next label, fix-up or instruction
            while (b < n && code.instruction_at({object, b}) == nullptr && !(b > a && code.label_at({object, b})) && !(b > a && code.fixups().contains({object, b})) &&
                   !(b > a && extra_labels.contains(b))) {
                ++b;
            }
            for (std::uint32_t i = a; i < b; i += 16) lines.push_back(masm::db_line(bytes.subspan(i, std::min<std::uint32_t>(16, b - i))));
            a = b;
            continue;
        }
        const std::uint32_t size = row->size;
        const std::uint8_t op = bytes[a];
        // a near call or jmp whose rel32 carries the (self-relative) fix-up: a symbolic branch
        if (size == 5 && (op == 0xE8 || op == 0xE9)) {
            if (const std::string label = relocation(a + 1); !label.empty()) {
                lines.push_back(std::format("        {:<6} {}", op == 0xE8 ? "call" : "jmp", label));
                a += size;
                continue;
            }
        }
        std::vector<std::uint32_t> fixes;
        for (std::uint32_t k = a; k < a + size; ++k) {
            if (code.fixups().contains({object, k})) fixes.push_back(k);
        }
        const bool all_labelled = std::all_of(fixes.begin(), fixes.end(), [&](std::uint32_t k) { return !relocation(k).empty(); });
        if (!fixes.empty() && !row->service && all_labelled) {   // 32-bit offset fix-ups: split around the fields
            std::uint32_t prev = a;
            for (const std::uint32_t k : fixes) {
                if (k > prev) lines.push_back(masm::db_line(bytes.subspan(prev, k - prev)));
                lines.push_back("        dd  offset " + relocation(k));
                prev = k + 4;
            }
            if (prev < a + size) lines.push_back(masm::db_line(bytes.subspan(prev, a + size - prev)));
        } else if (row->service || !fixes.empty() || row->fixup) {
            lines.push_back(masm::db_line(row->raw));   // an int 20h service, or a fix-up that cannot be symbolic
        } else {
            const auto line = masm::masm_line(code, object, *row);
            lines.push_back(line ? *line : masm::db_line(row->raw));
        }
        a += size;
    }
    return lines;
}

SourceAndDef emit_source_and_def(Emission& code, std::string_view vxd_name, const ForceRaw& force_raw, bool functional) {
    const auto& ddb = code.ddb();
    if (functional) idiom::label_fixup_targets(code);   // every internal fix-up target gets a label
    std::vector<std::string> source = {".386p"};
    std::vector<std::string> def = {std::format("VXD {}", vxd_name), std::format("DESCRIPTION '{}'", vxd_name), "", "SEGMENTS"};
    for (const le::Object& o : code.driver().objects()) {
        const std::string segment = std::format("SEG{:02}", o.index);
        const std::string class_name = std::format("LCODE{:02}", o.index);   // 'CODE' makes LINK mark it executable; zero-padded so they sort in order
        source.push_back(std::format("{} segment dword public use32 '{}'", segment, class_name));
        std::map<std::uint32_t, std::string> extra;
        if (ddb && ddb->object == o.index) extra[static_cast<std::uint32_t>(ddb->rva)] = "VXD_DDB";
        std::vector<std::string> body;
        if (functional) {
            body = link_body(code, o.index, extra);
        } else {
            masm::BodyOptions options;
            if (const auto it = force_raw.find(o.index); it != force_raw.end()) options.force_raw = it->second;
            options.extra_labels = extra;
            body = masm::object_body(code, o.index, options).lines;
        }
        source.insert(source.end(), body.begin(), body.end());   // the instructions go to the COFF code section
        source.push_back(segment + " ends");
        def.push_back(std::format("    {:<8} CLASS '{}' {}", segment, class_name, segment_attributes(o.flags)));
    }
    source.push_back("        end");
    def.push_back("");
    def.push_back("EXPORTS");
    def.push_back("        VXD_DDB @1");
    return {text::join(source, "\r\n") + "\r\n", text::join(def, "\r\n") + "\r\n"};
}

// ---- the byte-exact test, per object ----------------------------------------------------------------------------------------

ForceRawResult byte_exact_force_raw(const Environment& env, const le::Module& driver) {
    ForceRawResult out;
    const Emission code(Disassembly::run(driver, env.names), env.names);
    for (const le::Object& o : driver.objects()) {
        if (!o.executable()) continue;
        const masm::ObjectReport r = masm::compare_object(code, o.index, env.assemblers);
        out.force_raw[o.index] = r.force_raw;
        out.matched[o.index] = r.match;
    }
    return out;
}

// ---- comparing two drivers --------------------------------------------------------------------------------------------------

CompareObjects compare_objects(const le::Module& original, const le::Module& rebuilt) {
    std::vector<std::pair<std::uint32_t, Bytes>> originals;
    for (const le::Object& o : original.objects()) originals.emplace_back(o.index, original.object_data(o));
    CompareObjects out;
    out.total = rebuilt.objects().size();
    for (const le::Object& mine : rebuilt.objects()) {
        const Bytes bytes = rebuilt.object_data(mine);
        std::optional<ObjectMatch> best;
        for (const auto& [index, theirs] : originals) {
            if (theirs.size() != bytes.size()) continue;
            std::size_t same = 0;
            for (std::size_t i = 0; i < bytes.size(); ++i) same += bytes[i] == theirs[i];
            if (!best || same > best->equal) best = ObjectMatch{mine.index, index, same, bytes.size()};
        }
        if (!best) continue;
        out.exact_objects += best->equal == best->length;
        out.objects.push_back(*best);
    }
    return out;
}

// ---- the builds -------------------------------------------------------------------------------------------------------------

namespace {

// ML on the source with the period assembler, then the folder the linker works in.
std::optional<Bytes> assemble_source(const Environment& env, const std::string& source, std::string& log) {
    const masm::Assembled made = masm::assemble_coff(env.assemblers, source);
    log = made.log;
    return made.object;
}

std::string name_of(const Emission& code, std::string_view name) { return name.empty() ? (code.ddb() ? code.ddb()->name : std::string("DEVICE")) : std::string(name); }

}  // namespace

CompareBuild build_and_compare(const Environment& env, const le::Module& driver, std::string_view name, const ForceRaw& force_raw) {
    CompareBuild out;
    Emission code(Disassembly::run(driver, env.names), env.names);
    out.original_length = driver.data().size();
    const SourceAndDef written = emit_source_and_def(code, name, force_raw);
    std::string log;
    const std::optional<Bytes> object = assemble_source(env, written.source, log);
    if (!object) {
        out.stage = "assemble";
        out.log = tail(log, 800);
        return out;
    }
    fs::ScopedTempDir work("cbvxdlink");
    const Linked linked = env.link(*object, written.def, work.path());
    if (!linked.vxd) {
        out.stage = "link";
        out.log = tail(linked.log, 1200);
        out.source = written.source;
        out.def = written.def;
        return out;
    }
    const le::Module mine = le::Module::from_buffer(Bytes(*linked.vxd));
    out.ok = true;
    out.vxd = *linked.vxd;
    out.vxd_length = out.vxd.size();
    out.link_log = tail(linked.log, 400);
    out.report = compare_objects(driver, mine);
    return out;
}

FunctionalBuild build_functional(const Environment& env, const le::Module& driver, std::string_view name) {
    FunctionalBuild out;
    Emission code(Disassembly::run(driver, env.names), env.names);
    out.original_length = driver.data().size();
    const SourceAndDef written = emit_source_and_def(code, name, {}, true);
    std::string log;
    const std::optional<Bytes> object = assemble_source(env, written.source, log);
    if (!object) {
        out.stage = "assemble";
        out.log = tail(log, 1500);
        out.source = written.source;
        return out;
    }
    fs::ScopedTempDir work("cbvxdlink");
    const Linked linked = env.link(*object, written.def, work.path());
    if (!linked.vxd) {
        out.stage = "link";
        out.log = tail(linked.log, 1500);
        out.source = written.source;
        out.def = written.def;
        return out;
    }
    const le::Module mine = le::Module::from_buffer(Bytes(*linked.vxd));
    out.ok = true;
    out.vxd = *linked.vxd;
    out.vxd_length = out.vxd.size();
    out.original_fixups = driver.fixups().size();
    out.original_internal = internal_count(driver);
    out.rebuilt_fixups = mine.fixups().size();
    out.rebuilt_internal = internal_count(mine);
    out.link_log = tail(linked.log, 400);
    return out;
}

Overlay overlay_code(Emission& code, const le::Module& original, ByteView linked, double min_score) {
    const le::Module assembled = le::Module::from_buffer(Bytes(linked.begin(), linked.end()));
    // the offsets the DDK macros regenerate (never written as source), per object
    idiom::PlaceSet emittable;
    for (const auto& [at, insn] : code.instructions()) emittable.insert(at);
    idiom::PlaceSet skip = idiom::control_dispatch(code, emittable).skip;
    const auto& ddb = code.ddb();
    if (ddb) {
        const std::size_t size = idiom::ddb_size(code, *ddb);
        for (std::int64_t k = ddb->rva; k < ddb->rva + static_cast<std::int64_t>(size); ++k) skip.insert({ddb->object, k});
    }
    idiom::PlaceSet available;
    for (const Place& p : emittable) {
        if (!skip.contains(p)) available.insert(p);
    }
    if (const auto table = idiom::service_table(code, available)) {
        for (std::int64_t k = table->at.offset; k < table->at.offset + 4 * static_cast<std::int64_t>(table->count); ++k) skip.insert({table->at.object, k});
    }
    std::map<std::uint32_t, std::set<std::int64_t>> macro_by_object, fixup_bytes;
    for (const Place& p : skip) macro_by_object[p.object].insert(p.offset);
    for (const auto& [at, fixup] : code.fixups()) {
        for (std::int64_t k = at.offset; k < at.offset + 4; ++k) fixup_bytes[at.object].insert(k);   // the whole 4-byte field is loader-written
    }
    std::vector<std::pair<std::uint32_t, Bytes>> assembled_objects;
    for (const le::Object& o : assembled.objects()) assembled_objects.emplace_back(o.index, assembled.object_data(o));

    // Lay `candidate` (the assembled true code) over `recovered`, skipping the macro offsets of the recovered object and never comparing or replacing a
    // byte of a fix-up field. The agreement over the comparable bytes is the match quality that ranks and gates.
    struct Walk {
        Bytes bytes;
        std::size_t agree = 0, comparable = 0;
    };
    const auto walk = [](const Bytes& recovered, const std::set<std::int64_t>& macro, const Bytes& candidate, const std::set<std::int64_t>& fixups) {
        Walk w{recovered, 0, 0};
        std::size_t source = 0;
        for (std::size_t off = 0; off < w.bytes.size(); ++off) {
            if (macro.contains(static_cast<std::int64_t>(off))) continue;
            if (source >= candidate.size()) break;
            if (!fixups.contains(static_cast<std::int64_t>(off))) {
                ++w.comparable;
                if (w.bytes[off] == candidate[source]) ++w.agree;
                else w.bytes[off] = candidate[source];
            }
            ++source;
        }
        return w;
    };
    // every object's context, and the whole score matrix (recovered x assembled)
    struct Meta {
        Bytes recovered;
        std::set<std::int64_t> macro, fixups;
        std::size_t expected = 0;   // the bytes the assembly must supply
    };
    struct Score {
        double score;
        std::uint32_t object, assembled;
        std::size_t length;
    };
    std::map<std::uint32_t, Meta> meta;
    std::vector<Score> scores;
    for (const le::Object& o : original.objects()) {
        const ByteView bytes = code.code(o.index);
        Meta m{Bytes(bytes.begin(), bytes.end()), {}, {}, 0};
        if (const auto it = macro_by_object.find(o.index); it != macro_by_object.end()) m.macro = it->second;
        if (const auto it = fixup_bytes.find(o.index); it != fixup_bytes.end()) m.fixups = it->second;
        m.expected = m.recovered.size() - std::min(m.recovered.size(), m.macro.size());
        for (const auto& [index, candidate] : assembled_objects) {
            const Walk w = walk(m.recovered, m.macro, candidate, m.fixups);
            scores.push_back({w.comparable ? static_cast<double>(w.agree) / static_cast<double>(w.comparable) : 0.0, o.index, index, candidate.size()});
        }
        meta.emplace(o.index, std::move(m));
    }
    std::stable_sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) { return a.score > b.score; });   // the strongest pairs first
    std::map<std::uint32_t, Score> pick;
    std::set<std::uint32_t> taken;
    for (const Score& s : scores) {
        if (!pick.contains(s.object) && !taken.contains(s.assembled)) {
            pick.emplace(s.object, s);
            taken.insert(s.assembled);
        }
    }

    Overlay out;
    for (const le::Object& o : original.objects()) {
        const Meta& m = meta.at(o.index);
        OverlayRow row;
        row.object = o.index;
        row.expected = m.expected;
        row.macro = m.macro.size();
        const auto chosen = pick.find(o.index);
        if (chosen == pick.end()) {
            out.rows.push_back(row);
            continue;
        }
        const auto candidate = std::find_if(assembled_objects.begin(), assembled_objects.end(), [&](const auto& a) { return a.first == chosen->second.assembled; });
        const Walk w = walk(m.recovered, m.macro, candidate->second, m.fixups);
        row.assembled_object = chosen->second.assembled;
        row.score = chosen->second.score;
        row.compared = w.comparable;
        row.assembled_length = chosen->second.length;
        // nothing comparable: the object is entirely macro and fix-up bytes (loader-written), which matches on length alone
        row.ok = chosen->second.length >= m.expected && (w.comparable == 0 || chosen->second.score >= min_score);
        if (row.ok) code.code_mut(o.index) = w.bytes;
        out.rows.push_back(row);
    }
    std::string bad;
    for (const OverlayRow& r : out.rows) {
        if (r.ok) continue;
        bad += std::format("{}obj{}(asm={} score={:.3f} cmp={} need_len>={} got={})", bad.empty() ? "" : "; ", r.object,
                           r.assembled_object ? std::to_string(*r.assembled_object) : "None", r.score, r.compared, r.expected, r.assembled_length);
    }
    if (!bad.empty()) {
        std::string lengths;
        for (const auto& [index, bytes] : assembled_objects) lengths += std::format("{}{}:{}", lengths.empty() ? "" : ", ", index, bytes.size());
        throw FormatError("overlay", 0, "from-source overlay could not confidently match every object (refusing to ship scrambled code). Weak or unmatched: " + bad +
                                           ". Assembled objects (index:length): " + lengths);
    }
    out.vxd = le_writer::build_le(code, original, name_of(code, ""));
    return out;
}

LoadableBuild build_loadable_idiomatic(const Environment& env, const le::Module& driver, std::optional<std::string> name) {
    LoadableBuild out;
    Emission code(Disassembly::run(driver, env.names), env.names);
    out.original_length = driver.data().size();
    const std::string device = name ? *name : name_of(code, "");
    out.source = idiom::emit(code, true);
    const std::vector<masm::Assembled> made = masm::assemble_macro_batch(env.assemblers, std::vector<std::string>{out.source});
    if (made.empty() || !made.front().object) {
        out.stage = "assemble";
        out.log = tail(made.empty() ? std::string() : made.front().log, 1500);
        return out;
    }
    // one .DEF entry per object (a unique class) so LINK keeps each object separate and does not merge by class. Fixed PRELOAD NONDISCARDABLE: the real
    // flags come from the byte-exact writer, not from LINK, and DISCARDABLE makes LINK reorder objects (discardable to the end), which would desync
    // the code overlay.
    std::string per_object;
    for (const le::Object& o : driver.objects()) {
        const idiom::SegmentName s = idiom::object_segment(o.index, o.flags);
        per_object += std::format("    {:<11} CLASS '{}' PRELOAD NONDISCARDABLE\r\n", s.name, s.class_name);
    }
    out.def = std::format("VXD {0}\r\nDESCRIPTION '{0} reconstructed from source'\r\n\r\nSEGMENTS\r\n{1}{2}\r\nEXPORTS\r\n        {0}_DDB @1\r\n", device, kVxdDefSegments, per_object);
    fs::ScopedTempDir work("cbvxdlink");
    const Linked linked = env.link(*made.front().object, out.def, work.path());
    if (!linked.vxd) {
        out.stage = "link";
        out.log = tail(linked.log, 1500);
        return out;
    }
    const le::Module linked_module = le::Module::from_buffer(Bytes(*linked.vxd));
    try {
        Overlay overlay = overlay_code(code, driver, *linked.vxd);   // the assembled code in the loader-accepted container
        out.vxd = std::move(overlay.vxd);
        out.overlay = std::move(overlay.rows);
    } catch (const FormatError& e) {   // a low-confidence object match is a clean failure of this build
        out.stage = "overlay";
        out.log = e.what();
        out.link_log = tail(linked.log, 300);
        return out;
    }
    out.ok = true;
    out.vxd_length = out.vxd.size();
    out.objects = le::Module::from_buffer(Bytes(out.vxd)).objects().size();
    out.link_log = tail(linked.log, 300);
    out.min_overlay_score = out.overlay.empty() ? 0.0 : std::min_element(out.overlay.begin(), out.overlay.end(), [](const OverlayRow& a, const OverlayRow& b) { return a.score < b.score; })->score;
    out.rebuilt_internal = internal_count(linked_module);
    out.original_internal = internal_count(driver);
    return out;
}

// ---- the command ------------------------------------------------------------------------------------------------------------

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "link",
        "the whole driver rebuilt with ML and LINK, and compared",
        {cli::Positional{"file", "a VxD (LE)"}, cli::Positional{"name", "the driver's name (default: the file's)", false}},
        [&](const cli::Args& a) -> int {
            const stdfs::path path = fs::from_text(a.required("file"));
            const auto modules = le::open_modules(path);
            if (modules.empty()) throw FormatError("LE", 0, "the file holds no driver");
            const le::Module& driver = modules.front().second;
            const std::string name = a.value("name") ? *a.value("name") : text::upper(fs::to_text(path.stem()));
            const ForceRawResult tested = byte_exact_force_raw(env, driver);
            const CompareBuild r = build_and_compare(env, driver, name, tested.force_raw);
            if (!r.ok) {
                out("FAILED at {}:\n{}\n", r.stage, r.log);
                return kExitFailed;
            }
            for (const ObjectMatch& row : r.report.objects) {
                out("  my obj{:<2} -> oracle obj{:<2}  {}/{} ({:.1f}%){}\n", row.mine, row.oracle, row.equal, row.length, row.percent(), row.equal == row.length ? "  EXACT" : "");
            }
            out("linked {}: {} bytes (oracle {})  {}/{} objects byte-exact\n", name, r.vxd_length, r.original_length, r.report.exact_objects, r.report.total);
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("link", {command});
    std::vector<std::string> line = {"link"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::link
