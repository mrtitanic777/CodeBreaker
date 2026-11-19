// cb/symbols/identify.cpp -- see identify.hpp.
#include "cb/symbols/identify.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <set>

#include "cb/build/flat_thunk.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/analyze/dos_functions.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe_functions.hpp"
#include "cb/formats/powerpc_image.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/symbols/workbench.hpp"

namespace cb::symbols::identify {

namespace rl = runtime_library;
namespace describe = decompile::describe;
namespace pef = formats::pe_functions;

struct Identifier::Impl {
    Sources s;
    platforms::DecoderSet decoders;
    pef::CarveCache carves;
    std::optional<rl::SignatureTable> table32, table16;

    explicit Impl(Sources sources) : s(sources) {}

    // The signatures of one width, indexed once (the knowledge base's rows of the platform, in the order it holds them).
    const rl::SignatureTable& table(rl::Bits bits) {
        std::optional<rl::SignatureTable>& slot = bits == rl::Bits::X32 ? table32 : table16;
        if (!slot) {
            const std::vector<rl::SignatureRow> rows = s.knowledge.signatures(knowledge::platform(static_cast<int>(bits)));
            slot = rl::SignatureTable::from_rows(rows);
        }
        return *slot;
    }

    static bool is_i386(const std::filesystem::path& module) { return formats::pe::Image::open(module).header().machine == formats::pe::kMachineI386; }
    static bool is_powerpc(const std::filesystem::path& module) {
        return formats::pe::Image::open(module).header().machine == formats::powerpc_image::kMachine;
    }

    // Whether the thunk compiler made the module. A file that does not read as one of the kinds classify knows is not one.
    static bool is_flat_thunk(const std::filesystem::path& module) {
        try {
            return build::flat_thunk::classify(module).subtype == build::flat_thunk::Subtype::FlatThunk;
        } catch (const Error&) {
            return false;
        }
    }

    Found pe(const std::filesystem::path& module) {
        Found out;
        if (!is_i386(module)) return out;
        const std::shared_ptr<const pef::PeCarve> carve = carves.get(module);
        const formats::pe::Image& image = carve->image();
        rl::Pe32Module m;
        m.stem = text::upper(fs::to_text(module.stem()));
        m.image_base = image.header().image_base;
        for (const pef::CodeSection& section : carve->sections()) m.code.push_back({section.va, section.data});
        for (const auto& [start, function] : carve->functions()) m.function_starts.push_back(start);
        for (const formats::pe::Export& e : image.exports()) m.exports.push_back({static_cast<std::uint32_t>(e.ordinal), e.name, e.rva});
        m.flat_thunk = is_flat_thunk(module);
        if (rl::runtime_library_of(m.stem) != nullptr) m.ordinal_names = s.ordinals.table(m.stem);
        for (auto& [address, what] : rl::identify_pe(table(rl::Bits::X32), m, decoders)) out.emplace(address, std::move(what));
        return out;
    }

    // A real-mode program: the functions its analysis finds, matched in the image as the machine would see it. An address is a linear one.
    Found dos(const std::filesystem::path& module) {
        const formats::dos::Program program = formats::dos::read_program(fs::read_file(module), fs::to_text(module.filename()));
        rl::DosImage image{program.image, {}};
        if (!program.ds) return {};
        for (const auto& [va, fn] : analyze::dos_functions::discover(program).functions)
            if (va >= program.base && va - program.base < program.image.size()) image.function_starts.push_back(static_cast<std::uint32_t>(va - program.base));
        std::sort(image.function_starts.begin(), image.function_starts.end());
        Found out;
        for (auto& [offset, what] : rl::identify_dos(table(rl::Bits::X16), image)) out.emplace(static_cast<Address>(program.base) + offset, std::move(what));
        return out;
    }

    Found ne(const std::filesystem::path& module) {
        const formats::ne::Module image = formats::ne::Module::open_file(module);
        rl::Ne16Module m;
        m.module_name = image.module_name();
        const ByteView file = image.data();
        for (const formats::ne::Segment& segment : image.segments()) {
            ByteView bytes;
            if (segment.file_offset != 0 && segment.file_offset < file.size())
                bytes = file.subspan(segment.file_offset, std::min<std::uint64_t>(segment.length, file.size() - segment.file_offset));
            m.segments.push_back({segment.index, !segment.is_data(), bytes});
        }
        for (const auto& [place, distance] : describe::find_functions(image)) m.function_starts.push_back({static_cast<std::uint16_t>(place.seg), static_cast<std::uint16_t>(place.off)});
        std::sort(m.function_starts.begin(), m.function_starts.end());
        for (const formats::ne::Entry& e : image.entries())
            if (e.kind != formats::ne::Entry::Kind::Unused) m.entries.push_back({static_cast<std::uint16_t>(e.ordinal), e.segment, e.offset});
        if (rl::runtime_library_of(text::upper(m.module_name)) != nullptr) m.ordinal_names = s.ordinals.table(text::upper(m.module_name));
        Found out;
        for (auto& [place, what] : rl::identify_ne(table(rl::Bits::X16), m)) out.emplace(static_cast<Address>(describe::address(place.segment, place.offset)), std::move(what));
        return out;
    }
};

Identifier::Identifier(Sources sources) : impl_(std::make_unique<Impl>(sources)) {}
Identifier::~Identifier() = default;

Found Identifier::identify(const std::filesystem::path& module) {
    const std::optional<base::Lane> lane = impl_->s.corpus.lane(module);
    if (lane == base::Lane::Pe) return impl_->pe(module);
    if (lane == base::Lane::Ne) return impl_->ne(module);
    if (lane == base::Lane::Dos) return impl_->dos(module);
    return {};
}

std::size_t Identifier::function_count(const std::filesystem::path& module) {
    const std::optional<base::Lane> lane = impl_->s.corpus.lane(module);
    if (lane == base::Lane::Ne) return describe::find_functions(formats::ne::Module::open_file(module)).size();
    if (lane == base::Lane::Dos) {
        const formats::dos::Program program = formats::dos::read_program(fs::read_file(module), fs::to_text(module.filename()));
        return program.ds ? analyze::dos_functions::discover(program).functions.size() : 0;
    }
    if (lane == base::Lane::Pe) {
        if (Impl::is_powerpc(module)) return formats::powerpc_image::Image::read(module).functions().size();
        return impl_->carves.get(module)->functions().size();
    }
    return 0;
}

rl::CompilerInfo Identifier::compiler(const std::filesystem::path& module, const Found& found) {
    const std::optional<base::Lane> lane = impl_->s.corpus.lane(module);
    const Bytes file = fs::read_file(module);
    rl::CompilerFacts facts;
    facts.file = file;
    if (lane == base::Lane::Pe) {
        const formats::pe::Image image = formats::pe::Image::open(module);
        facts.kind = rl::CompilerFacts::Kind::Pe;
        const formats::pe::OptionalHeader optional = image.optional_header();
        facts.linker_major = optional.linker_major;
        facts.linker_minor = optional.linker_minor;
        facts.pe_header_offset = image.header().e_lfanew;
        std::set<std::string> dlls;   // (each module once: the facts are the set of modules it imports from)
        for (const formats::pe::Import& i : image.imports()) dlls.insert(i.dll);
        facts.imported_modules.assign(dlls.begin(), dlls.end());
    } else if (lane == base::Lane::Ne) {
        const formats::ne::Module image = formats::ne::Module::open_file(module);
        facts.kind = rl::CompilerFacts::Kind::Ne;
        facts.linker_major = image.header().linker_version;
        facts.linker_minor = image.header().linker_revision;
        facts.module_references = image.module_refs();
    }
    std::vector<std::string> libraries;
    libraries.reserve(found.size());
    for (const auto& [address, what] : found) libraries.push_back(what.library);
    return rl::describe_compiler(facts, libraries);
}

// ---- the command -----------------------------------------------------------------------------------------------------------

namespace {

constexpr std::string_view kUsage = "identify MODULE | identify --learn [--add] \"WHAT=FILE\" ...";

// "what=file" as `--learn` takes it; an argument without `=` is passed over.
std::vector<rl::LearnSource> learn_sources(const std::vector<std::string>& arguments) {
    std::vector<rl::LearnSource> out;
    for (const std::string& a : arguments) {
        const std::size_t at = a.find('=');
        if (at == std::string::npos) continue;
        out.push_back({a.substr(0, at), fs::from_text(a.substr(at + 1))});
    }
    return out;
}

int learn_command(const Environment& environment, const std::vector<std::string>& arguments, bool add) {
    platforms::DecoderSet decoders;
    const std::vector<rl::LearnSource> sources = learn_sources(arguments);
    rl::Learned learned = rl::learn(sources, decoders, [](const std::string& line) { cb::out("{}\n", line); });
    std::filesystem::path file = environment.signature_file;
    if (file.empty()) file = environment.config->checkout_root() / "data" / "symbols" / "runtime_signatures.json";
    if (add && std::filesystem::exists(file)) {   // (what is already learned stays; the new libraries come after it)
        const Bytes kept = fs::read_file(file);
        rl::Learned all = rl::read_signature_file(as_text(kept));
        rl::merge(all, learned);
        learned = std::move(all);
    }
    const std::string text = rl::format_signature_file(learned);
    fs::write_file(file, ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
    cb::out("{} 32-bit and {} 16-bit signatures\n", learned.count(rl::Bits::X32), learned.count(rl::Bits::X16));
    return 0;
}

}  // namespace

cli::Command command(const Environment& environment) {
    return cli::Command{
        "identify",
        "which of a module's functions came with the compiler (the C runtime, MFC), and which compiler built it",
        {cli::Option{"--learn", "", "make the signatures from library files: the arguments are WHAT=FILE (\"Visual C++ 2.0 C runtime=C:\\\\msvc20\\\\lib\\\\libc.lib\")",
                     cli::Arg::Flag},
         cli::Option{"--add", "", "with --learn: keep the signatures already learned and add these after them", cli::Arg::Flag},
         cli::Positional{"arguments", "MODULE (a corpus name or a path), or WHAT=FILE... with --learn", false, true}},
        [environment](const cli::Args& args) -> int {
            const std::vector<std::string> arguments = args.values("arguments");
            if (args.flag("--learn")) return learn_command(environment, arguments, args.flag("--add"));
            if (arguments.empty()) throw UsageError(std::string(kUsage));
            const std::optional<std::filesystem::path> found_path = environment.corpus->retail(arguments[0]);
            if (!found_path) throw NotFoundError(std::filesystem::path(arguments[0]), 0, "no module '" + arguments[0] + "' in the corpus");
            const std::filesystem::path path = *found_path;
            Workbench bench(environment);
            Identifier& identifier = bench.identifier();
            const Found found = identifier.identify(path);
            const bool is_ne = environment.corpus->lane(path) == base::Lane::Ne;
            for (const auto& [address, what] : found) {
                std::string where;
                if (is_ne) {
                    const core16::SegOff place = describe::where(static_cast<describe::Va>(address));
                    where = std::format("{}:{:04x}", place.seg, place.off);
                } else {
                    where = std::format("{:08x}", address);
                }
                cb::out("{}  {:<40} {}\n", where, what.name.substr(0, 40), what.library);
            }
            const std::size_t total = identifier.function_count(path);
            const rl::CompilerInfo info = identifier.compiler(path, found);
            cb::out("{}: {} of {} functions are a runtime library's\n", text::lower(fs::to_text(path.filename())), found.size(), total);
            cb::out("built with: {} (linker {})\n", info.compiler, info.linker.empty() ? "?" : info.linker);
            for (const std::string& line : info.evidence) cb::out("    {}\n", line);
            return 0;
        },
        {}, {}, {}};
}

}  // namespace cb::symbols::identify
