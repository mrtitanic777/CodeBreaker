// verify/lanes/win64.cpp -- see win64.hpp.
#include "verify/lanes/win64.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <set>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "core16_types.hpp"
#include "emulate/unicorn_uc.hpp"
#include "formats/pe.hpp"
#include "platforms/decoder.hpp"

namespace stdfs = std::filesystem;

namespace cb::verify::lanes {

namespace {

constexpr std::uint64_t kPage = 0x1000;
std::uint64_t page_down(std::uint64_t a) { return a & ~(kPage - 1); }
std::uint64_t page_up(std::uint64_t a) { return (a + kPage - 1) & ~(kPage - 1); }

// RBX, RBP, RSI, RDI, R12-R15 at entry: a function that hands one back changed is caught though nothing here keeps a value there.
struct Saved {
    const char* name;
    std::uint64_t sentinel;
};
constexpr Saved kSaved[] = {{"RBX", 0xB0B0B0B0B0B0B0B0}, {"RBP", 0xBDBDBDBDBDBDBDBD}, {"RSI", 0x5151515151515151}, {"RDI", 0xD1D1D1D1D1D1D1D1},
                            {"R12", 0x1212121212121212}, {"R13", 0x1313131313131313}, {"R14", 0x1414141414141414}, {"R15", 0x1515151515151515}};
constexpr const char* kArgRegisters[] = {"RCX", "RDX", "R8", "R9"};

int reg(std::string_view name) {
    const std::optional<int> id = emulate::unicorn_uc::x86_register(name);
    if (!id) throw InternalError(std::format("win64: Unicorn has no register {}", name));
    return *id;
}

std::string vector_text(const Win64Lane::Vector& v) {
    std::string out = "args";
    for (const std::int64_t a : v.args) out += std::format(" {:x}", static_cast<std::uint64_t>(a));
    if (v.stub_ret) out += std::format(" ret {:x}", static_cast<std::uint64_t>(*v.stub_ret));
    if (v.pointers) out += " pointers";
    return out;
}

// The bytes the scratch memory starts with: counting up, so a pointer argument reads something other than zeros; or each 8-byte
// word the address of the word 0x48 bytes on (round the scratch memory), so a structure's pointer leads somewhere readable.
const Bytes& scratch_bytes(bool pointers) {
    static const Bytes counting = [] {
        Bytes b(Win64Lane::kScratchSize);
        for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i * 7 + 1);
        return b;
    }();
    static const Bytes linked = [] {
        Bytes b(Win64Lane::kScratchSize);
        for (std::size_t o = 0; o < b.size(); o += 8) {
            const std::uint64_t to = Win64Lane::kScratchBase + (o + 0x48) % Win64Lane::kScratchSize;
            for (int k = 0; k < 8; ++k) b[o + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(to >> (8 * k));
        }
        return b;
    }();
    return pointers ? linked : counting;
}

void map_image(core16::Uc& uc, const Win64Lane::Image& image) {
    uc.mem_map(image.base, page_up(image.end) - image.base);
    for (const Win64Lane::Image::Region& r : image.regions) uc.mem_write(r.va, r.bytes);
}

}  // namespace

// ---- the inputs ----------------------------------------------------------------------------------------------------------------

std::vector<Win64Lane::Vector> Win64Lane::vectors() {
    const auto s = [](std::uint64_t k) { return static_cast<std::int64_t>(kScratchBase + 0x1000 * k); };
    std::vector<Vector> out;
    const auto add = [&](std::vector<std::int64_t> args, std::optional<std::int64_t> ret = std::nullopt, bool pointers = false) {
        Vector v;
        v.args = std::move(args);
        v.args.resize(kArguments, 0);
        v.stub_ret = ret;
        v.pointers = pointers;
        out.push_back(std::move(v));
    };
    add({0, 0, 0, 0, 0, 0});
    add({1, 2, 3, 4, 5, 6}, 1);
    add({s(0), s(1), s(2), s(3), s(4), s(5)});
    add({s(0), 1, s(2), 0, 0, 0}, 1);
    add({-1, 0x7FFFFFFF, 0x80, 0x10, -2, 3});
    add({s(0), 0, 0, 0, 0, 0}, 0x123);
    add({0x100, s(1), 0xFFFF, s(3), 0, 1});
    add({s(0), s(1), s(2), s(3), s(4), s(5)}, 1, true);
    add({s(0), 0, s(2), 2, 0, 0}, 0, true);
    return out;
}

std::vector<Win64Lane::Vector> Win64Lane::directed(ByteView code, std::uint64_t va, std::size_t most) {
    thread_local platforms::DecoderSet decoders;
    std::vector<std::uint64_t> seen;   // the constants, each once, in the order the code first has them
    for (const platforms::Insn& insn : decoders.get(platforms::Arch::X86, 64).disasm(code, va)) {
        if (insn.mnemonic != "cmp" && insn.mnemonic != "sub") continue;
        const std::string_view ops = insn.op_str;
        const std::size_t comma = ops.rfind(", ");
        if (comma == std::string_view::npos) continue;
        std::string_view literal = ops.substr(comma + 2);
        const bool negative = literal.starts_with("-");
        if (negative) literal.remove_prefix(1);
        std::optional<std::uint64_t> c = literal.starts_with("0x") ? text::parse_int<std::uint64_t>(literal.substr(2), 16) : text::parse_int<std::uint64_t>(literal);
        if (!c) continue;
        const std::uint64_t value = negative ? ~*c + 1 : *c;
        if (value != 0 && value != 1 && std::ranges::find(seen, value) == seen.end()) seen.push_back(value);
    }
    std::vector<Vector> out;
    for (std::size_t k = 0; k < seen.size() && k < most; ++k) {
        const auto c = static_cast<std::int64_t>(seen[k]);
        Vector all;
        all.args.assign(kArguments, c);
        all.stub_ret = c;
        out.push_back(all);
        Vector beside;   // (pointers into the scratch memory, the constant between them)
        for (std::size_t a = 0; a < kArguments; ++a) beside.args.push_back(a % 2 == 0 ? static_cast<std::int64_t>(kScratchBase + 0x1000 * a) : c);
        beside.stub_ret = c;
        out.push_back(beside);
    }
    return out;
}

// ---- an image ------------------------------------------------------------------------------------------------------------------

Win64Lane::Image Win64Lane::read_image(const stdfs::path& path) {
    const formats::pe::Image pe = formats::pe::Image::open(path);
    if (pe.header().machine != formats::pe::kMachineX64) throw FormatError(fs::to_text(path), 0, "not an x64 module");
    Image out;
    out.base = pe.header().image_base;
    out.end = out.base + pe.optional_header().image_size;
    const ByteView file = pe.data();
    out.regions.push_back({out.base, Bytes(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(file.size(), pe.optional_header().headers_size))), false});
    for (const formats::pe::Section& s : pe.sections()) {
        const ByteView data = pe.section_data(s);
        out.regions.push_back({out.base + s.virtual_address, Bytes(data.begin(), data.end()), (s.characteristics & 0x80000000u) != 0});
        if ((s.characteristics & 0x80000000u) != 0 && s.virtual_size > data.size())   // (the zeros past the file's bytes are written too)
            out.regions.back().bytes.resize(s.virtual_size, 0);
    }
    for (const formats::pe::Import& i : pe.imports()) out.slots.emplace(i.slot_va, i.name);
    return out;
}

// ---- one run -------------------------------------------------------------------------------------------------------------------

compare::Fingerprint Win64Lane::fingerprint(const Context& context, std::uint64_t entry, const Vector& v, bool rebuilt, bool novalue,
                                            std::optional<std::pair<std::uint64_t, std::uint64_t>> own) {
    compare::Fingerprint fp;
    try {
        const std::unique_ptr<core16::Uc> uc = emulate::unicorn_uc::open_x86(64);
        map_image(*uc, context.original);
        if (rebuilt && context.rebuilt) {
            map_image(*uc, *context.rebuilt);
            // jmp qword ptr [rip+0] ; <the original's address>   -- 14 bytes: Visual C++ starts every x64 function 16 bytes from the last
            for (const auto& [at, original] : context.originals) {
                if (at == entry) continue;
                Bytes jump = {0xFF, 0x25, 0, 0, 0, 0};
                for (int k = 0; k < 8; ++k) jump.push_back(static_cast<std::uint8_t>(original >> (8 * k)));
                uc->mem_write(at, jump);
            }
        }
        // every import slot lands on one `ret` of its own
        std::vector<std::string> landed;
        for (const auto& [slot, name] : context.original.slots) {
            const std::uint64_t landing = kLandingBase + 16 * landed.size();
            Bytes value(8);
            for (int k = 0; k < 8; ++k) value[static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(landing >> (8 * k));
            uc->mem_write(slot, value);
            landed.push_back(name);
        }
        const std::uint64_t landing_size = page_up(std::max<std::uint64_t>(16 * landed.size(), 16));
        uc->mem_map(kLandingBase, landing_size);
        uc->mem_write(kLandingBase, Bytes(landing_size, 0xC3));
        uc->mem_map(kReturnAddress, kPage);
        uc->mem_write(kReturnAddress, Bytes(kPage, 0xC3));
        uc->mem_map(kScratchBase, kScratchSize);
        uc->mem_write(kScratchBase, scratch_bytes(v.pointers));
        uc->mem_map(kStackBase, kStackSize);
        // the stack as a call leaves it: the return address, the 32 bytes kept for the register arguments, the rest above them
        const std::uint64_t rsp = kStackBase + kStackSize - 0x10000 - 8;
        Bytes frame(8 + 32 + 8 * (kArguments > 4 ? kArguments - 4 : 0), 0);
        for (int k = 0; k < 8; ++k) frame[static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(kReturnAddress >> (8 * k));
        for (std::size_t a = 4; a < v.args.size(); ++a)
            for (int k = 0; k < 8; ++k) frame[40 + 8 * (a - 4) + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(v.args[a]) >> (8 * k));
        uc->mem_write(rsp, frame);
        uc->reg_write(reg("RSP"), rsp);
        for (std::size_t a = 0; a < 4 && a < v.args.size(); ++a) uc->reg_write(reg(kArgRegisters[a]), static_cast<std::uint64_t>(v.args[a]));
        for (const Saved& s : kSaved) uc->reg_write(reg(s.name), s.sentinel);
        uc->reg_write(reg("RAX"), 0);

        const std::uint64_t rebuilt_lo = rebuilt && context.rebuilt ? context.rebuilt->base : 0, rebuilt_hi = rebuilt && context.rebuilt ? context.rebuilt->end : 0;
        const auto argument = [&](std::uint64_t value) -> std::string {
            if (value >= kStackBase && value < kStackBase + kStackSize) return "stack";   // (the two frames are laid out differently)
            if (value >= rebuilt_lo && value < rebuilt_hi) return "rebuilt";
            return std::format("{:x}", value);
        };
        constexpr std::size_t kMaxDeps = 64;
        const std::uint64_t result = static_cast<std::uint64_t>(v.stub_ret.value_or(0));
        uc->hook_add_code(
            [&](core16::Uc& m, std::uint64_t address, std::uint32_t) {
                const std::size_t k = static_cast<std::size_t>((address - kLandingBase) / 16);
                if (k >= landed.size()) return;
                if (fp.deps.size() < kMaxDeps) {
                    const std::string& name = landed[k];
                    std::string call = name + "(";
                    if (const auto n = context.argc.find(name); n != context.argc.end()) {
                        for (int a = 0; a < std::min(n->second, 4); ++a) call += (a == 0 ? "" : ",") + argument(m.reg_read(reg(kArgRegisters[a])));
                    } else {
                        call += "?";
                    }
                    fp.deps.push_back(call + ")");
                } else if (fp.deps.size() == kMaxDeps) {
                    fp.deps.emplace_back("...");
                }
                m.reg_write(reg("RAX"), result);
            },
            kLandingBase, kLandingBase + landing_size - 1);
        std::set<std::int64_t> covered;
        if (own) uc->hook_add_code([&](core16::Uc&, std::uint64_t address, std::uint32_t) { covered.insert(static_cast<std::int64_t>(address)); }, own->first, own->second - 1);
        std::string exit;
        try {
            uc->emu_start(entry, kReturnAddress, 2'000'000, kMaxSteps);
            exit = uc->reg_read(reg("RIP")) == kReturnAddress ? std::string(compare::kExitReturned) : std::string(compare::kExitTruncated);
        } catch (const emulate::unicorn_uc::UcFault& e) {
            // (a fault is a fault: which access faults first -- a read, a write, a fetch -- is the emulated environment's, not the code's)
            exit = "exception: fault";
            if (fs::get_env("CB_WIN64_TRACE"))   // (where it faulted, for the one looking into a verdict)
                std::fprintf(stderr, "win64: %s side, entry %llx: %s at rip %llx (rax %llx rcx %llx rdx %llx rsp %llx)\n", rebuilt ? "rebuilt" : "original",
                             static_cast<unsigned long long>(entry), e.what(), static_cast<unsigned long long>(uc->reg_read(reg("RIP"))),
                             static_cast<unsigned long long>(uc->reg_read(reg("RAX"))), static_cast<unsigned long long>(uc->reg_read(reg("RCX"))),
                             static_cast<unsigned long long>(uc->reg_read(reg("RDX"))), static_cast<unsigned long long>(uc->reg_read(reg("RSP"))));
        }
        fp.exit = exit;
        fp.truncated = exit == compare::kExitTruncated;
        fp.cov.assign(covered.begin(), covered.end());
        if (exit == compare::kExitReturned) {
            fp.ret = novalue ? 0 : static_cast<std::int64_t>(uc->reg_read(reg("RAX")));
            fp.pop = static_cast<std::int64_t>(uc->reg_read(reg("RSP")) - rsp);
            bool saved = true;
            for (const Saved& s : kSaved) saved = saved && uc->reg_read(reg(s.name)) == s.sentinel;
            fp.saved = saved;
        }
        // what it left: the module's writable data and the memory behind the pointer arguments
        Bytes left = uc->mem_read(kScratchBase, kScratchSize);
        for (const Image::Region& r : context.original.regions) {
            if (!r.writable) continue;
            const Bytes now = uc->mem_read(r.va, r.bytes.size());
            left.insert(left.end(), now.begin(), now.end());
        }
        fp.writes = crypto::to_hex(crypto::sha1(left));
    } catch (const emulate::unicorn_uc::UcFault& e) {   // (the machine could not be set up: no observation)
        fp.err = std::string("emulator: ") + e.what();
    }
    return fp;
}

Judgement Win64Lane::judge_function(const Context& context, const Item& item) {
    const lane_common::Judgement made = lane_common::judged([&]() -> lane_common::Judgement {
        const std::function<compare::Fingerprint(const Vector&)> original = [&](const Vector& v) {
            std::optional<std::string> key;
            if (context.sha) key = compare::cache_key_text(std::format("win64 {} {} {:x} {:x} {} | {}", kRev, *context.sha, item.va, item.end, item.novalue, vector_text(v)));
            return lane_common::remembered(context.fingerprints, key, [&] {
                return fingerprint(context, item.va, v, false, item.novalue, std::pair{item.va, item.end});
            });
        };
        const std::function<compare::Fingerprint(const Vector&)> rebuilt = [&](const Vector& v) {
            return fingerprint(context, item.rebuilt_va, v, true, item.novalue);
        };
        const std::vector<Vector> fixed = vectors();
        const std::int64_t insns = std::max<std::int64_t>(static_cast<std::int64_t>(item.end - item.va) / 4, 1);
        ByteView code;
        for (const Image::Region& r : context.original.regions)
            if (item.va >= r.va && item.va < r.va + r.bytes.size())
                code = ByteView(r.bytes).subspan(static_cast<std::size_t>(item.va - r.va), static_cast<std::size_t>(std::min<std::uint64_t>(item.end, r.va + r.bytes.size()) - item.va));
        const compare::Outcome outcome = compare::compare<Vector>(original, rebuilt, fixed, {}, {.insns = insns},
                                                                  [&](const std::set<std::int64_t>&, const std::set<std::int64_t>&) { return directed(code, item.va); });
        return {outcome.verdict, outcome.reason, outcome.limit_codes(), outcome.detail};
    });
    return {made.verdict, made.reason, made.limits, made.detail};
}

// ---- the tools -----------------------------------------------------------------------------------------------------------------

std::optional<build::functions::X64Tools> Win64Lane::find_tools(const std::vector<stdfs::path>& roots) {
    std::vector<stdfs::path> where = roots;
    if (where.empty()) where = {"C:/Program Files/Microsoft Visual Studio", "C:/Program Files (x86)/Microsoft Visual Studio"};
    // (versions compare as numbers, part by part: 14.51.36231 after 14.9)
    const auto parts = [](const std::string& v) {
        std::vector<long> out;
        for (std::string_view p : text::split(v, '.')) out.push_back(text::parse_int<long>(p).value_or(0));
        return out;
    };
    std::optional<build::functions::X64Tools> best;
    std::vector<long> best_version;
    std::error_code ec;
    for (const stdfs::path& root : where) {
        for (const auto& release : stdfs::directory_iterator(root, ec)) {
            for (const auto& edition : stdfs::directory_iterator(release.path(), ec)) {
                for (const auto& version : stdfs::directory_iterator(edition.path() / "VC" / "Tools" / "MSVC", ec)) {
                    const stdfs::path bin = version.path() / "bin" / "Hostx64" / "x64";
                    if (!stdfs::is_regular_file(bin / "cl.exe", ec) || !stdfs::is_regular_file(bin / "link.exe", ec)) continue;
                    const std::vector<long> v = parts(fs::to_text(version.path().filename()));
                    if (!best || v > best_version) {
                        best = build::functions::X64Tools{bin / "cl.exe", bin / "link.exe"};
                        best_version = v;
                    }
                }
            }
        }
    }
    return best;
}

// ---- the kind ------------------------------------------------------------------------------------------------------------------

const std::string& Win64Lane::name() const {
    static const std::string kName = "win64";
    return kName;
}

reconstruct::KindTraits Win64Lane::traits() {
    reconstruct::KindTraits t;
    t.per_function = true;   // a verdict holds while the function and what it reaches are unchanged
    return t;
}

Profile Win64Lane::profile(const stdfs::path&) const {
    std::string settings = std::format("win64 rev {} compare {} options", kRev, compare::kRevision);
    for (const std::string& o : build::functions::cflags_x64()) settings += " " + o;
    settings += " vectors";
    for (const Vector& v : vectors()) settings += " [" + vector_text(v) + "]";
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

std::unique_ptr<Prepared> Win64Lane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    auto p = std::make_unique<PreparedX64>();
    p->module = decompile::c_text::compilable_x64(services_.decompiler, module_path, work);
    const translate::l2::Made& made = p->module.made;
    p->header = made.header();
    p->order = made.order();
    std::string all = p->header;
    for (const auto& [name, text] : made.texts()) p->texts[name] = text;
    for (const std::string& name : p->order)
        if (const auto text = p->texts.find(name); text != p->texts.end()) all += text->second;
    for (const auto& [va, name] : made.by_va()) p->key[name] = std::format("{:x}", va);
    const formats::pe::Image image = formats::pe::Image::open(module_path);
    for (const std::uint32_t rva : image.x64_function_starts()) p->all_keys.push_back(std::format("{:x}", image.header().image_base + rva));
    std::ranges::sort(p->all_keys);
    p->all_keys.erase(std::ranges::unique(p->all_keys).begin(), p->all_keys.end());
    p->sig = crypto::to_hex(crypto::sha1(as_bytes(all)));
    return p;
}

Built Win64Lane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& p = static_cast<const PreparedX64&>(prepared);
    const decompile::c_text::ModuleX64& m = p.module;
    auto ctx = std::make_shared<Context>();
    ctx->original = read_image(module_path);
    const std::uint64_t base = (ctx->original.end + kRebuiltGap + 0xFFFF) & ~std::uint64_t{0xFFFF};
    const OrderedMap<std::string, std::string> written = m.made.texts();
    build::functions::Texts texts;
    for (const auto& [name, text] : written) texts[name] = text;
    const build::functions::BuiltPowerPc b = services_.builder.build_x64(services_.tools, m.made.header(), texts, p.order, work, base);
    if (b.dll) ctx->rebuilt = read_image(*b.dll);
    ctx->sha = services_.content_sha ? services_.content_sha(module_path) : std::nullopt;
    ctx->fingerprints = services_.fingerprints;
    if (services_.import_argc)
        for (const auto& [slot, name] : ctx->original.slots)
            if (const std::optional<int> n = services_.import_argc(name)) ctx->argc[name] = *n;
    // where each function ends: at the next function the module states, or its image's end
    const formats::pe::Image image = formats::pe::Image::open(module_path);
    std::set<std::uint64_t> starts;
    for (const std::uint32_t rva : image.x64_function_starts()) starts.insert(image.header().image_base + rva);

    Built built;
    std::map<std::string, Item> items;
    for (const auto& [name, why] : b.dropped) built.missing[name] = why;
    for (const auto& [va, name] : m.made.by_va()) {
        if (built.missing.contains(name)) continue;
        const auto placed = b.where.find(name);
        if (placed == b.where.end()) {
            built.missing[name] = "not in the build";
            continue;
        }
        Item item;
        item.key = p.key.at(name);
        item.va = va;
        const auto next = starts.upper_bound(va);
        item.end = next != starts.end() ? *next : ctx->original.end;
        item.rebuilt_va = placed->second;
        ctx->originals[placed->second] = va;
        item.novalue = m.novalue.contains(name);
        built.items.push_back(name);
        items[name] = item;
    }
    built.judge = [ctx, items = std::move(items)](const std::string& name) { return judge_function(*ctx, items.at(name)); };
    return built;
}

}  // namespace cb::verify::lanes
