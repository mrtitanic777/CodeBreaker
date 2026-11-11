#include "cb/source/ne_asm/segdiff.hpp"

#include <algorithm>
#include <format>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::source::ne_asm::segdiff {
namespace stdfs = std::filesystem;

std::size_t relocation_width(std::uint8_t source) {
    switch (source) {
        case 0: return 1;
        case 2: return 2;
        case 3: return 4;
        case 5: return 2;
        case 11: return 6;
        default: return 2;
    }
}

OrderedMap<std::string, formats::omf::FunctionBytes> compile_group(const Compiler16& compile, std::string_view c_source, std::string_view flags) {
    return formats::omf::function_bytes(compile(c_source, flags));
}

Shipped shipped_segment(const formats::ne::Module& module, std::size_t segment) {
    const std::vector<formats::ne::Segment> segments = module.segments();
    if (segment < 1 || segment > segments.size()) throw UsageError(std::format("segment {} does not exist (the module has {})", segment, segments.size()));
    const formats::ne::Segment& seg = segments[segment - 1];
    Shipped out;
    if (seg.file_offset == 0) return out;   // no data in the file (uninitialised): nothing to compare
    const ByteView view = io::checked_range(module.data(), seg.file_offset, seg.length);
    out.code.assign(view.begin(), view.end());
    const std::size_t size = out.code.size();

    // The masked set MUST match the link-file writer's NE relocation restore pass exactly: the rebuild leaves shipped bytes only
    // at the sites it restores, so any byte masked here but NOT restored there is a false "already-matching" that lets a
    // mis-disassembled table entry stay live and then diff in the real rebuild. The writer requires the WHOLE fix-up site to fit
    // in the segment before visiting it (`cur + width <= size`), and so does this.
    const auto cover = [&](std::size_t at, std::size_t width) {
        for (std::size_t k = 0; k < width; ++k) out.mask.insert(at + k);
    };
    for (const formats::ne::Relocation& rec : module.relocations(seg)) {
        const std::size_t width = relocation_width(rec.source);
        const std::size_t first = rec.offset;
        if (rec.additive()) {   // the field holds an addend, not a link: only the site itself
            if (first + width <= size) cover(first, width);
            continue;
        }
        // a chain: the field holds the offset of the next site, until 0xFFFF
        std::set<std::size_t> seen;
        std::size_t cur = first;
        while (cur != 0xFFFF && cur + width <= size && !seen.contains(cur)) {
            seen.insert(cur);
            cover(cur, width);
            if (cur + 1 >= size) break;   // the two-byte link is not there
            cur = out.code[cur] | (static_cast<std::size_t>(out.code[cur + 1]) << 8);
        }
    }
    return out;
}

Verdict diff(ByteView ours, const Mask& our_mask, ByteView shipped, const Mask& shipped_mask) {
    Mask skip = our_mask;
    skip.insert(shipped_mask.begin(), shipped_mask.end());
    const std::size_t n = std::min(ours.size(), shipped.size());
    Verdict v;
    v.our_len = ours.size();
    v.shipped_len = shipped.size();
    v.length_match = ours.size() == shipped.size();
    v.masked = skip.size();
    std::vector<bool> covered(n, false);   // the masks over the compared length
    for (const std::uint64_t at : skip) {
        if (at < n) covered[static_cast<std::size_t>(at)] = true;
    }
    for (std::size_t i = 0; i < n; ++i) {
        if (covered[i] || ours[i] == shipped[i]) continue;
        if (!v.first_diff) v.first_diff = i;
        ++v.n_diff;
    }
    v.exact = v.length_match && v.n_diff == 0;
    return v;
}

std::string render(const Verdict& verdict, std::optional<ByteView> ours, std::optional<ByteView> shipped, const Mask& our_mask,
                   const Mask& shipped_mask) {
    const char* what = verdict.exact ? "BYTE-EXACT" : verdict.length_match ? "length ok, content differs" : "length differs";
    std::string out = std::format("verdict: {}\n  our={} shipped={} bytes, masked={}, diffs={}", what, verdict.our_len, verdict.shipped_len, verdict.masked,
                                  verdict.n_diff);
    if (verdict.first_diff) out += std::format(", first@0x{:x}", *verdict.first_diff);
    if (!verdict.exact && ours && shipped && verdict.first_diff) {
        const std::size_t i = *verdict.first_diff;
        const std::size_t lo = i >= 4 ? i - 4 : 0;
        const std::size_t hi = std::min({ours->size(), shipped->size(), i + 8});
        const auto show = [&](ByteView bytes) {
            std::string line;
            for (std::size_t k = lo; k < hi; ++k) {
                const bool masked = our_mask.contains(k) || shipped_mask.contains(k);
                line += (k > lo ? " " : "") + (masked ? std::string("--") : std::format("{:02x}", bytes[k]));
            }
            return line;
        };
        out += std::format("\n  @0x{:x} ours    : {}\n  @0x{:x} shipped : {}", lo, show(*ours), lo, show(*shipped));
    }
    return out;
}

const std::string_view kSelftestGroup = R"(
int helper(int x)        { return x + 1; }
int caller(int y)        { return helper(y) * 2 + helper(y + 1); }
)";

namespace {

std::string first_line(const std::string& text) { return text.substr(0, text.find('\n')); }

const formats::omf::FunctionBytes* find(const OrderedMap<std::string, formats::omf::FunctionBytes>& group, const std::string& name) {
    return group.find(name);
}

}  // namespace

bool selftest(const Compiler16& compile) {
    cb::out("segdiff selftest\n\n");
    bool ok = true;

    // (a) round-trip: a group matches itself exactly
    const auto group = compile_group(compile, kSelftestGroup);
    const auto* caller = find(group, "_caller");
    if (!caller) {
        cb::out("the group has no _caller\n\nSELFTEST: FAIL\n");
        return false;
    }
    const Verdict same = diff(caller->code, caller->fixup_mask, caller->code, caller->fixup_mask);
    cb::out("(a) round-trip caller vs itself: {}\n", first_line(render(same)));
    ok = ok && same.exact;

    // (b) mutation is detected and located
    std::string mutated(kSelftestGroup);
    const std::size_t at = mutated.find("* 2 +");
    if (at != std::string::npos) mutated.replace(at, 5, "* 3 +");
    const auto mutant_group = compile_group(compile, mutated);
    const auto* mutant = find(mutant_group, "_caller");
    if (!mutant) {
        cb::out("the mutated group has no _caller\n\nSELFTEST: FAIL\n");
        return false;
    }
    const Verdict changed = diff(mutant->code, mutant->fixup_mask, caller->code, caller->fixup_mask);
    cb::out("(b) mutated caller vs original: {}{}\n", first_line(render(changed)), changed.exact ? "(BUG: should differ)" : "");
    ok = ok && !changed.exact;

    // (c) whole-compiland resolution: compiling the callee IN the group makes caller's call to it CONCRETE (fewer masked
    //     bytes) than when the callee is external. This is the reason isolation fails.
    const std::size_t grouped_masked = caller->fixup_mask.size();
    const auto alone = compile_group(compile, "extern int helper(int);\nint caller(int y){return helper(y)*2+helper(y+1);}\n");
    const auto* alone_caller = find(alone, "_caller");
    const std::size_t external_masked = alone_caller ? alone_caller->fixup_mask.size() : 0;
    cb::out("(c) caller masked bytes: grouped={}, callee-external={}  -> {}\n", grouped_masked, external_masked,
            grouped_masked < external_masked ? "grouping resolves the calls" : "??");
    ok = ok && grouped_masked < external_masked;

    cb::out("\nSELFTEST: {}\n", ok ? "PASS" : "FAIL");
    return ok;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "segdiff",
        "compile a function group and compare one function with the original segment",
        {cli::Positional{"csrc", "reconstructed C (a function group)", false},
         cli::Option{"--module", "", "the NE module", cli::Arg::Value, "NAME"},
         cli::Option{"--seg", "", "segment number, from 1", cli::Arg::Value, "N"},
         cli::Option{"--off", "", "where the function starts in the segment", cli::Arg::Value, "OFFSET"},
         cli::Option{"--size", "", "how many bytes to compare (default: the compiled function's length)", cli::Arg::Value, "N"},
         cli::Option{"--func", "", "which function of the group to check", cli::Arg::Value, "NAME"},
         cli::Option{"--flags", "", "the compiler's options", cli::Arg::Value, "FLAGS", std::string(kDefaultFlags)},
         cli::Option{"--selftest", "", "prove the mechanism", cli::Arg::Flag}},
        [&](const cli::Args& a) -> int {
            if (a.flag("--selftest")) return selftest(env.compile) ? kExitOk : kExitFailed;
            const auto csrc = a.value("csrc"), module = a.value("--module"), func = a.value("--func");
            const auto seg = a.integer<std::size_t>("--seg"), off = a.integer<std::size_t>("--off");
            if (!csrc || !module || !seg || !off || !func) throw UsageError("need csrc + --module --seg --off --func (or --selftest)");

            const stdfs::path path = env.corpus.retail(*module).value_or(fs::from_text(*module));
            const auto group = compile_group(env.compile, fs::read_text(fs::from_text(*csrc)), a.value_or("--flags", std::string(kDefaultFlags)));
            const formats::omf::FunctionBytes* ours = group.find(*func);
            if (!ours) {
                std::string have;
                for (const auto& [name, bytes] : group) have += (have.empty() ? "" : ", ") + name;
                cb::err("function {} not in the compiled group; have: {}\n", *func, have);
                return kExitFailed;
            }
            const Shipped segment = shipped_segment(formats::ne::Module::open_file(path), *seg);
            const std::size_t size = a.integer<std::size_t>("--size").value_or(ours->code.size());
            const std::size_t from = std::min(*off, segment.code.size());
            const std::size_t to = std::min(from + size, segment.code.size());
            const ByteView shipped = ByteView(segment.code).subspan(from, to - from);
            Mask shipped_mask;   // the segment's mask, relative to where the function starts
            for (const std::uint64_t m : segment.mask) {
                if (*off <= m && m < *off + size) shipped_mask.insert(m - *off);
            }
            const Verdict verdict = diff(ours->code, ours->fixup_mask, shipped, shipped_mask);
            cb::out("{}\n", render(verdict, ByteView(ours->code), shipped, ours->fixup_mask, shipped_mask));
            return verdict.exact ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
    const cli::Cli program("segdiff", {command});
    std::vector<std::string> line = {"segdiff"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::segdiff
