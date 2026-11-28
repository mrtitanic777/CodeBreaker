// build/module_asm16.cpp -- the part of a 16-bit module that is its own instructions, written as assembly that links in.
//
// A code segment with assembly in it is rebuilt as one frame from its first byte: the assembly, and the lead-in, stay where they were --
// hand-written code reaches into its own segment by number (tables, variables, LODS through CS) -- and where each function rebuilt from C
// was is filler: those are placed after the frame and called by their names.

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "base/cache.hpp"
#include "build/module_internal.hpp"
#include "toolchain/msc6.hpp"
#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "platforms/decoder.hpp"

namespace cb::build::module::internal {

namespace {

// The code that writes, probes and checks the assembly (asm16, build/data, source/assembly, build/msc6) and this pass: a change to the
// behaviour of any of them bumps it, so what an earlier build kept is not taken for what they would make now.
constexpr std::string_view kAssemblyStamp = "build/module assembled16 cb 1; capstone 5";

std::string with_underscores_for_slashes(std::string s) {
    std::replace(s.begin(), s.end(), '/', '_');
    return s;
}

// bytes[from, from + length), clamped to what there is.
ByteView clamped(ByteView bytes, std::size_t from, std::size_t length) {
    if (from >= bytes.size()) return {};
    return bytes.subspan(from, std::min(length, bytes.size() - from));
}

// What a run of functions came to.
struct Done {
    std::vector<std::string> run;
    bool refused = true;       // why is set
    std::string why;
    std::string stem;
    std::shared_ptr<Piece> piece;
};

std::string run_key(const std::vector<std::string>& run) { return text::join(run, "\x1f"); }

// The offsets a cached piece says to write as mnemonics, as text and back.
std::string good_as_text(const std::set<std::uint32_t>& good) {
    std::string s;
    for (std::uint32_t a : good) {
        if (!s.empty()) s += ',';
        s += std::to_string(a);
    }
    return s;
}

std::set<std::uint32_t> good_from_text(std::string_view s) {
    std::set<std::uint32_t> good;
    for (std::string_view n : text::split(s, ','))
        if (const auto v = text::parse_int<std::uint32_t>(n)) good.insert(*v);
    return good;
}

}  // namespace

std::map<std::string, asm16::Import> assemble16(AssemblyJob& job) {
    Built& b = job.b;
    const formats::ne::Module& ne = job.ne;
    const Places& at = job.at;
    const Segments& segs = job.segs;
    std::map<std::string, std::string>& stems = job.stems;
    const std::map<std::string, PublicSymbol>& publics = job.publics;
    const stdfs::path& folder = job.folder;
    const Assembler& assembler = job.assembler;
    const Services& services = job.services;

    // (what the rounds of a piece come to, below, is kept in the shared build cache by every input of them: the assembler, the code that
    // writes, probes and checks the text, the decoder, and the original module's bytes, which say each instruction's bytes, its segment's
    // width (16- or 32-bit) and its relocations; then, per piece, its text, place and stem)
    const std::string asm_stamp = base::cache::key({kAssemblyStamp, assembler.tool_inputs(), ne.data()});
    b.lead_ins.clear();
    b.placed_c.clear();
    b.moved_c.clear();
    b.publics = publics;
    std::map<std::string, asm16::Import> out;
    if (!assembler.available()) return out;

    std::map<std::string, Place> where;   // function -> its place
    std::map<std::uint32_t, std::vector<std::uint32_t>> starts;
    for (const auto& [place, name] : at) {
        where[name] = place;
        starts[place.seg].push_back(place.off);   // (ascending: `at` is ordered by place)
    }
    std::map<std::uint32_t, std::string> in_segment;   // a code segment -> the symbol of the first function in it that is built on its own
    for (const auto& [place, name] : at) {
        if (publics.contains(name) && !b.rejected.contains(name)) in_segment.emplace(place.seg, publics.at(name).symbol);
    }
    std::map<Place, std::string> entries;   // a place called into mid-run -> its symbol
    const std::uint32_t autodata = ne.header().autodata_segment;

    const asm16::SymbolFor symbol_for = [&](const asm16::Reference& r) -> std::optional<std::string> {
        if (const auto* fn = std::get_if<asm16::FunctionRef>(&r)) {
            const Place place{fn->seg, fn->off};
            const auto name = at.find(place);
            if (name != at.end() && publics.contains(name->second)) return publics.at(name->second).symbol;
            const auto entry = entries.find(place);
            // (a function built on its own -- from C, or assembled by itself -- by the name it links under: NETDI's 9:0000 and 48:0000,
            // far pointers to them in assembly beside -- with no symbol, 76 functions of four segments were refused as assembly)
            if (entry == entries.end() && name != at.end() && stems.contains(name->second)) return "_" + name->second;
            return entry == entries.end() ? std::nullopt : std::optional<std::string>(entry->second);
        }
        if (const auto* sg = std::get_if<asm16::SegmentRef>(&r)) {
            // (any symbol in the segment gives its selector -- a function of it built on its own too: NETDI's segments 9 and 48 hold one
            // each, written in assembly, and the selectors of them in the code beside had no symbol)
            if (const auto it = in_segment.find(sg->seg); it != in_segment.end() && !it->second.empty()) return it->second;
            for (const auto& [place, name] : at) {
                if (place.seg == sg->seg && stems.contains(name)) return "_" + name;
            }
            return std::nullopt;
        }
        if (const auto* data = std::get_if<asm16::DataRef>(&r)) {   // a data segment: the auto one, or one carried
            if (autodata != 0 && data->seg == autodata) return "_codebreaker_data";
            return job.carried.contains(data->seg) ? std::optional<std::string>(std::format("_codebreaker_segment_{}", data->seg)) : std::nullopt;
        }
        const auto& imported = std::get<asm16::ImportRef>(r);
        if (const auto* ordinal = std::get_if<std::uint16_t>(&imported.which)) {
            if (!imported.module) return std::nullopt;
            return std::format("_{}_{}", text::sanitize_identifier(*imported.module), *ordinal);
        }
        return "_" + text::sanitize_identifier(std::get<std::string>(imported.which));
    };
    // (from nothing: what an earlier build left there is read below -- which far function may move to the segment beside depends on what
    // the pieces refer to -- and would decide it)
    {
        std::error_code ec;
        stdfs::remove_all(folder / "asm", ec);
        stdfs::create_directories(folder / "asm", ec);
    }

    const PieceAssembler assemble_piece = [&](const std::string& stem, Piece& piece, std::uint32_t lo, std::uint32_t hi) -> std::string {
        const Assembler::Result r = assembler.assemble(stem, piece.f->write(piece.good, std::make_pair(lo, hi)));
        const stdfs::path obj = folder / (stem + ".obj");
        std::error_code ec;
        if (r.code != 0 || !stdfs::is_regular_file(obj, ec)) return "the assembler: " + trimmed_tail(r.message, 160);
        return asm16::check_piece(fs::read_file(obj), *piece.f, lo, hi).value_or("");
    };

    // Written as readable as the assembler allows: each instruction as its mnemonic where the probe says the assembler gives back the
    // original's bytes, and round after round any that did not come back as they were go back to bytes. -> "" or why it could not be.
    const auto assemble_rounds = [&](const std::string& stem, Piece& piece) -> std::string {
        static const Regex error_line(R"(\((\d+)\): Error)");
        asm16::Function& f = *piece.f;
        const std::map<std::uint32_t, source::assembly::Candidate> candidates = f.candidates();
        std::set<source::assembly::Candidate> ok;
        if (!candidates.empty() && services.prober != nullptr) {
            std::vector<source::assembly::Candidate> values;
            for (const auto& [offset, candidate] : candidates) values.push_back(candidate);
            ok = services.prober->probe(values, {}, {}, folder, source::assembly::Mode::Real16);
        }
        std::set<std::uint32_t>& good = piece.good;
        good.clear();
        for (const auto& [offset, candidate] : candidates) {
            if (ok.contains(candidate)) good.insert(offset);
        }
        for (const auto& [offset, instruction] : f.insns()) {
            if (instruction.target) good.insert(offset);
        }
        std::string why = "no rounds";
        for (int round = 0; round < 40; ++round) {
            const Assembler::Result r = assembler.assemble(stem, f.write(good));
            const stdfs::path obj = folder / (stem + ".obj");
            std::error_code ec;
            if (r.code != 0 || !stdfs::is_regular_file(obj, ec)) {
                const std::string written = f.write(good);   // (kept: the lines are views into it)
                const std::vector<std::string_view> lines = text::split(written, '\n');
                std::set<std::string> bad;
                error_line.for_each_match(r.message, [&](const RegexMatch& m) {
                    const auto n = text::parse_int<std::size_t>(m.group(1));
                    if (n && *n >= 1 && *n <= lines.size()) bad.emplace(text::trim(lines[*n - 1]));
                    return true;
                });
                std::vector<std::uint32_t> drop;
                for (std::uint32_t a : good) {
                    if (f.insns().contains(a) && bad.contains(f.mnemonic(a))) drop.push_back(a);
                }
                if (drop.empty()) return "the assembler: " + trimmed_tail(r.message, 160);
                for (std::uint32_t a : drop) good.erase(a);
                continue;
            }
            const toolchain::msc6::Code made = toolchain::msc6::object_code(fs::read_file(obj));
            std::vector<std::uint32_t> hit;
            if (made.code.size() == f.end() - f.start()) {
                const std::vector<std::uint32_t> diff = asm16::differences(made.code, made.mask, f);
                if (diff.empty()) return "";
                const std::uint32_t place = diff[0];
                for (std::uint32_t a : good) {
                    if (a <= place && place < a + f.insns().at(a).bytes.size()) hit.push_back(a);
                }
                if (hit.empty()) {
                    std::optional<std::uint32_t> latest;
                    for (std::uint32_t a : good) {
                        if (a <= place && (!latest || a > *latest)) latest = a;
                    }
                    if (latest) hit.push_back(*latest);
                }
                why = std::format("differs at {:04x}", place);
            } else {
                // longer or shorter: the first instruction whose bytes do not line up is blamed
                why = std::format("{} bytes, the original {}", made.code.size(), f.end() - f.start());
                for (std::uint32_t a : good) {   // (ascending)
                    const std::size_t n = f.insns().at(a).bytes.size();
                    const ByteView mine = clamped(made.code, a - f.start(), n), theirs = clamped(f.blob(), a, n);
                    if (!std::equal(mine.begin(), mine.end(), theirs.begin(), theirs.end())) {
                        hit.push_back(a);
                        break;
                    }
                }
            }
            if (hit.empty()) return why;
            for (std::uint32_t a : hit) good.erase(a);
        }
        return why;
    };
    const auto assemble = [&](const std::string& stem, Piece& piece) -> std::string {
        asm16::Function& f = *piece.f;
        // (what the rounds came to is kept by what decides it -- the module, where the piece is, its names and holes, this code and the
        // assembler: the same piece in a later build is written once, as it ended, and not probed and assembled round after round again)
        const std::string key = base::cache::key({f.text() ? *f.text() : std::string(), f.seg(), f.start(), f.end(), stem, asm_stamp});
        if (services.cache != nullptr) {
            if (const std::optional<Bytes> had = services.cache->get("assembled16", key)) {
                const std::string raw(as_text(*had));
                const std::size_t nul = raw.find('\0');
                const std::string why = raw.substr(0, nul);
                piece.good = nul == std::string::npos ? std::set<std::uint32_t>() : good_from_text(std::string_view(raw).substr(nul + 1));
                if (!why.empty()) return why;
                const Assembler::Result r = assembler.assemble(stem, f.write(piece.good));
                const stdfs::path obj = folder / (stem + ".obj");
                std::error_code ec;
                if (r.code == 0 && stdfs::is_regular_file(obj, ec)) {   // (checked again: the original's bytes)
                    const toolchain::msc6::Code made = toolchain::msc6::object_code(fs::read_file(obj));
                    if (made.code.size() == f.end() - f.start() && asm16::differences(made.code, made.mask, f).empty()) return "";
                }
            }
        }
        const std::string result = assemble_rounds(stem, piece);
        if (services.cache != nullptr) {
            const std::string kept = result + '\0' + good_as_text(piece.good);
            services.cache->put("assembled16", key, ByteView(reinterpret_cast<const std::uint8_t*>(kept.data()), kept.size()));
        }
        return result;
    };

    std::map<std::uint32_t, std::uint32_t> lead;   // a segment -> where its lead-in ends
    // A code segment with assembly in it is rebuilt as one frame from its first byte (see the top of the file).
    std::set<std::uint32_t> whole;
    for (const auto& [seg, offsets] : starts) {
        const auto info = segs.find(seg);
        if (info == segs.end() || info->second.is_data()) continue;
        if (std::any_of(offsets.begin(), offsets.end(), [&](std::uint32_t o) { return job.forced.contains(at.at(Place{seg, o})); })) whole.insert(seg);
    }
    platforms::DecoderSet main_decoders;
    for (const auto& [seg, offsets] : starts) {
        const std::uint32_t first = offsets[0];
        const auto info = segs.find(seg);
        if (first == 0 || info == segs.end() || info->second.is_data() || whole.contains(seg)) continue;
        Piece piece;
        piece.f = std::make_unique<asm16::Function>(
            ne, asm16::Request{.seg = static_cast<std::uint16_t>(seg), .start = 0, .end = first, .publics = {{0, std::format("codebreaker_lead_{}", seg)}}},
            symbol_for, main_decoders);
        const std::string stem = std::format("asm/lead{:02d}", seg);
        if (piece.f->text() && assemble(stem, piece).empty()) {
            lead[seg] = first;
            b.lead_ins.push_back(stem);
            for (const auto& [symbol, import] : piece.f->imports()) out[symbol.substr(1)] = import;
        }
    }
    // the functions written in assembly, each run of them in a segment together -- as the file they were written in had them: a jump from
    // one into the next is a jump to a label -- and each rejected C function on its own
    std::set<std::string> forced = job.forced;
    OrderedMap<std::string, Done> done;   // run (joined names) -> its stem, or why it was refused
    const auto inside_function = [&](std::uint32_t seg, std::uint32_t t) -> std::optional<std::uint32_t> {
        const auto so = starts.find(seg);
        if (so == starts.end()) return std::nullopt;
        std::optional<std::uint32_t> inside;
        for (std::uint32_t o : so->second) {
            if (o <= t && (!inside || o > *inside)) inside = o;
        }
        return inside;
    };
    for (int pass = 0; pass < 40; ++pass) {   // (each pass may find a place entered mid-run: its run is written again)
        std::vector<std::vector<std::string>> runs;
        for (std::uint32_t seg : whole) {   // one frame per such segment
            std::vector<std::string> mine;
            for (std::uint32_t o : starts.at(seg)) {
                const std::string& n = at.at(Place{seg, o});
                if (publics.contains(n) && (forced.contains(n) || b.rejected.contains(n))) mine.push_back(n);
            }
            if (!mine.empty()) runs.push_back(std::move(mine));
        }
        for (const auto& [seg, offsets] : starts) {
            if (whole.contains(seg)) continue;
            std::vector<std::string> run;
            for (std::uint32_t off : offsets) {
                const std::string& name = at.at(Place{seg, off});
                if (forced.contains(name) && publics.contains(name)) {
                    run.push_back(name);
                    continue;
                }
                if (!run.empty()) runs.push_back(std::move(run));
                run.clear();
                if (b.rejected.contains(name) && publics.contains(name)) runs.push_back({name});
            }
            if (!run.empty()) runs.push_back(std::move(run));
        }
        std::set<std::string> joined;
        std::set<Place> wanted;

        // A run written and assembled. Many at once: each its own files.
        using Made = std::pair<std::vector<std::string>, std::shared_ptr<Piece>>;
        const auto one = [&](const std::vector<std::string>& run) -> Made {
            thread_local platforms::DecoderSet decoders;   // (an engine is not shared between threads)
            const Place first = where.at(run[0]);
            const std::uint32_t seg = first.seg;
            std::uint32_t off = first.off;
            const std::uint32_t last = where.at(run.back()).off;
            const auto& offsets = starts.at(seg);
            const auto later = std::find_if(offsets.begin(), offsets.end(), [&](std::uint32_t o) { return o > last; });
            const auto info = segs.find(seg);
            std::uint32_t end = later != offsets.end() ? *later : (info != segs.end() ? info->second.length : last);
            std::vector<asm16::Hole> holes;
            if (whole.contains(seg)) {
                off = 0;
                end = segs.at(seg).length;
                const std::set<std::string> names(run.begin(), run.end());
                for (std::size_t k = 0; k < offsets.size(); ++k) {
                    const std::string& n = at.at(Place{seg, offsets[k]});
                    if (names.contains(n)) continue;
                    const std::uint32_t hi = k + 1 < offsets.size() ? offsets[k + 1] : end;
                    holes.push_back({offsets[k], hi, publics.contains(n) ? publics.at(n).symbol : "_" + n});
                }
            }
            asm16::Request request{.seg = static_cast<std::uint16_t>(seg), .start = off, .end = end};
            for (const std::string& n : run) request.publics.push_back({where.at(n).off, publics.at(n).symbol});
            for (const auto& [place, symbol] : entries) {
                if (place.seg == seg && off <= place.off && place.off < end) {
                    request.publics.push_back({place.off, symbol});
                    request.extra_entries.push_back(place.off);
                }
            }
            if (const auto l = lead.find(seg); l != lead.end()) request.cs_ok = l->second;
            request.whole = whole.contains(seg);
            request.holes = std::move(holes);
            auto piece = std::make_shared<Piece>();
            piece->f = std::make_unique<asm16::Function>(ne, request, symbol_for, decoders);
            if (piece->f->text()) piece->assembled = assemble("asm/" + with_underscores_for_slashes(stems.at(run[0])), *piece);
            return {run, piece};
        };
        std::vector<std::vector<std::string>> todo;
        for (const auto& run : runs)
            if (!done.contains(run_key(run))) todo.push_back(run);
        parallel::ThreadPool pool(std::max<std::size_t>(1, std::min<std::size_t>(todo.size(), static_cast<std::size_t>(parallel::workers()))));
        const std::vector<Made> made = parallel::parallel_map(pool, todo, one);
        for (const auto& [run, piece] : made) {
            const asm16::Function& f = *piece->f;
            const std::uint32_t seg = where.at(run[0]).seg;
            for (const auto& [far_seg, far_off] : f.raw_far_entries()) {   // (from its bytes: a far call into a middle)
                if (!entries.contains(Place{far_seg, far_off})) wanted.insert(Place{far_seg, far_off});
            }
            if (!f.text() && (!f.missing().empty() || !f.far_missing().empty())) {
                // it calls into the middle of a run of assembly (in its segment, or far in another): that place is an entry of its own
                for (std::uint32_t t : f.missing())
                    if (!entries.contains(Place{seg, t})) wanted.insert(Place{seg, t});
                for (const auto& [far_seg, far_off] : f.far_missing())
                    if (!entries.contains(Place{far_seg, far_off})) wanted.insert(Place{far_seg, far_off});
                continue;
            }
            if (!f.text() && f.reaches()) {
                // it jumps into the middle of a function beside it: that one was written in assembly too (nothing in its own code said so);
                // the run takes it in
                if (const auto inside = inside_function(seg, *f.reaches())) {
                    const std::string& nm = at.at(Place{seg, *inside});
                    if (!forced.contains(nm) && publics.contains(nm)) {
                        joined.insert(nm);
                        continue;
                    }
                }
            }
            const std::string why = !f.text() ? f.why() : piece->assembled.value_or("");
            Done d;
            d.run = run;
            d.refused = !why.empty();
            if (d.refused) {
                d.why = why;
            } else {
                d.stem = "asm/" + with_underscores_for_slashes(stems.at(run[0]));
                d.piece = piece;
            }
            done.insert_or_assign(run_key(run), std::move(d));
        }
        for (const Place& pl : wanted) {
            const std::uint32_t seg = pl.seg, t = pl.off;
            // (a segment no function is known to start in has nothing to give an entry: KRNL386's segment 4 and GDI's 45, reached far from a
            // run's bytes -- the build stopped there)
            const auto inside = inside_function(seg, t);
            const auto info = segs.find(seg);
            const std::string* name = nullptr;
            if (inside) {
                if (const auto fn = at.find(Place{seg, *inside}); fn != at.end()) name = &fn->second;
            }
            // (nor does a place past the segment's end: no piece would define it -- a stand-in elsewhere, and the link refused the near call)
            if (name == nullptr || info == segs.end() || t >= std::max(info->second.length, info->second.min_alloc)) continue;
            if (forced.contains(*name)) {
                entries[Place{seg, t}] = std::format("_sub_{}_{:04X}", seg, t);
                std::vector<std::string> gone;
                for (const auto& [key, result] : done) {
                    if (std::find(result.run.begin(), result.run.end(), *name) != result.run.end()) gone.push_back(key);
                }
                for (const std::string& key : gone) done.erase(key);   // its run is written again, with the entry
            } else if (publics.contains(*name)) {
                joined.insert(*name);   // called into mid-body: written in assembly
            }
        }
        if (joined.empty() && wanted.empty()) break;
        for (const std::string& name : joined) {
            if (const auto origin = b.origins.find(name); origin != b.origins.end()) {
                origin->second.language = analyze::origin::Language::Assembly;
                origin->second.evidence.push_back("entered in its middle from assembly beside it");
            }
        }
        forced.insert(joined.begin(), joined.end());
    }
    std::set<std::string> claimed;
    for (auto& [key, result] : done) {
        const std::vector<std::string>& run = result.run;
        if (result.refused) continue;
        if (std::any_of(run.begin(), run.end(), [&](const std::string& n) { return claimed.contains(n); })) continue;
        if (std::any_of(run.begin(), run.end(), [&](const std::string& n) { return !forced.contains(n) && !b.rejected.contains(n); })) continue;
        claimed.insert(run.begin(), run.end());
        const std::uint32_t seg = where.at(run[0]).seg;
        std::optional<std::vector<std::string>> placed;
        if (whole.contains(seg)) {
            placed = frame_pieces(b, *result.piece, seg, stems, folder, assembler, assemble_piece, job.elsewhere);
            if (!placed) {
                for (const std::string& name : run) b.assembly_refused.insert_or_assign(name, "its frame's pieces do not assemble back: " + b.frame_why);
                b.placed_c.clear();
                continue;
            }
            b.lead_ins.insert(b.lead_ins.end(), placed->begin(), placed->end());   // first into its segment, in order
        }
        for (const std::string& name : run) note_assembled(b, name, stems, whole.contains(seg) ? (*placed)[0] : result.stem);
        for (const auto& [symbol, import] : result.piece->f->imports()) out[symbol.substr(1)] = import;
    }
    for (const auto& [key, result] : done) {
        if (!result.refused) continue;
        for (const std::string& name : result.run) {
            if (!claimed.contains(name)) b.assembly_refused.insert_or_assign(name, result.why);
        }
    }
    return out;
}

}  // namespace cb::build::module::internal
