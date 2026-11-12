// cb/decompile/decompiler_lanes.cpp -- the decompiler's lanes: a 32-bit module (PE) and a PowerPC image through the description of the
// module, a virtual device driver, a DOS program, a NES cartridge; any executable as the text of one .c file; and the command.
#include <algorithm>

#include <nlohmann/json.hpp>

#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/decompiler_internal.hpp"
#include "cb/decompile/m6502.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/pe_functions.hpp"
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
Functions lanes_grow(Decompiler& decompiler, Described& d, const LineFor& line_for, int rounds) {
    Run first = decompiler.run(d.path);
    OrderedMap<std::string, std::string> out = first.texts;
    std::map<Va, std::string> by_va = first.by_va;
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
    }
    std::map<std::string, std::string> known;
    for (const auto& [va, name] : d.names) known[hex8(va)] = name;
    Functions result;
    for (const auto& [name, text] : out) result.texts.insert_or_assign(name, known.empty() ? text : by_their_names(text, known));
    for (const auto& [va, name] : by_va) result.order.push_back(name);
    result.by_va = std::move(by_va);
    return result;
}

}  // namespace

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
    Functions grown = lanes_grow(*this, *d, line, 8);
    const symbols::dos_interrupts::ServiceTable table =
        symbols::dos_interrupts::ServiceTable::load(context_.settings.checkout / "codebreaker" / "symbols" / "dos_interrupts.json");
    DosProgramC result;
    result.functions.order = std::move(grown.order);
    result.functions.by_va = std::move(grown.by_va);
    for (const auto& [name, text] : grown.texts) {
        std::vector<symbols::dos_interrupts::Interrupt> calls;
        if (const auto it = d->interrupts.find(name); it != d->interrupts.end())
            for (const analyze::dos_functions::Interrupt& i : it->second)
                calls.push_back({i.number, i.ah ? std::optional<std::uint32_t>(*i.ah) : std::nullopt, i.al ? std::optional<std::uint32_t>(*i.al) : std::nullopt, i.address});
        result.functions.texts.insert_or_assign(name, table.annotate(text, calls));
    }
    result.description = d;
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
        if (parts.empty()) {
            DosProgramC r = decompile_dos(module, std::nullopt);
            texts = std::move(r.functions.texts);
            order = std::move(r.functions.order);
            total = r.description->names.size();
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
    return {std::move(text), order.size(), total};
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
