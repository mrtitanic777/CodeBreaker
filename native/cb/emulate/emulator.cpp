// cb/emulate/emulator.cpp -- see emulator.hpp.
//
// The hooks are the recorder's (cb/emulate/native_trace over emulate/tracer); this module lays the machine out, writes the frame, starts
// each run and reads the result.
// cb-unity: alone
#include "cb/emulate/emulator.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <unordered_set>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/emulate/unicorn_uc.hpp"
#include "cb/knowledge/package.hpp"

namespace cb::emulate::emulator {

namespace {

constexpr std::uint64_t kPageMask = ~std::uint64_t{kPage - 1};
constexpr std::uint32_t kMinFunctionBytes = 8;               // a shorter "function" is a stub the recovery found, not one to trace
constexpr std::uint64_t kMaxCodeSpan = std::uint64_t{64} << 20;   // give up joining code sections spread wider than this

// The 32-bit x86 registers a run reads and writes, by Unicorn's ids.
struct RegisterIds {
    int esp, eax, eip, ecx, edx, ebx, esi, edi, ebp, st0;
};
const RegisterIds& emu_registers() {
    static const RegisterIds ids = [] {
        auto id = [](std::string_view name) {
            const auto found = unicorn_uc::x86_register(name);
            CB_ASSERT(found.has_value(), "an x86 register Unicorn does not name");
            return *found;
        };
        return RegisterIds{id("ESP"), id("EAX"), id("EIP"), id("ECX"), id("EDX"), id("EBX"), id("ESI"), id("EDI"), id("EBP"), id("ST0")};
    }();
    return ids;
}

Bytes emu_le32(std::uint32_t v) {
    return {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v >> 16), static_cast<std::uint8_t>(v >> 24)};
}

// The pages [address >> 12, (address + max(len, 1) - 1) >> 12] a write of `len` bytes touches.
template <class F>
void emu_each_page(std::uint64_t address, std::uint64_t len, F&& f) {
    const std::uint64_t last = (address + std::max<std::uint64_t>(len, 1) - 1) >> 12;
    for (std::uint64_t page = address >> 12; page <= last; ++page) f(page);
}

// `DLL!name` / `KERNEL.GLOBALALLOC` -> the function's own name; anything else as it is.
std::string_view emu_bare_name(std::string_view name) {
    auto is_word = [](char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; };
    std::size_t i = 0;
    while (i < name.size() && is_word(name[i])) ++i;
    if (i == 0 || i >= name.size() || (name[i] != '.' && name[i] != '!')) return name;
    const std::string_view rest = name.substr(i + 1);
    if (rest.empty() || !std::all_of(rest.begin(), rest.end(), is_word)) return name;
    return rest;
}

// The decode caches of the recorder for one program: the code ranges of a module and of the overlay that is laid out beside it. The ranges'
// bytes are the modules' own (the cache points at them), so the modules are held here.
struct CodeSet {
    std::shared_ptr<const LoadedModule> module, overlay;
    native_trace::TraceCache cache;
    std::vector<native_trace::StaticId> statics;
};

struct IatSlot {
    std::uint32_t address = 0;
    std::array<std::uint8_t, 4> sentinel{};
};

// A fresh emulator with the module laid out: its image, the original beside it (`overlay`), the caller's memory, and a landing place in every
// import slot. Kept between runs when they ask to reuse it.
struct Machine {
    std::unique_ptr<core16::Uc> uc;
    std::shared_ptr<CodeSet> code;
    std::vector<native_trace::Sentinel> sentinels;               // import landing places
    std::unordered_set<std::uint64_t> mapped;                    // page-group bases mapped (membership only)
    std::unordered_set<std::uint64_t> init_pages;                // pages given other contents than the file's (membership only)
    std::map<std::uint64_t, std::vector<IatSlot>> slots;         // page -> the import slots on it (written last, as ever)
    std::unique_ptr<core16::Uc::Context> context;
};

struct MachineKey {
    std::string path, sha, overlay;
    bool stub = false, fast = false;
    friend bool operator==(const MachineKey&, const MachineKey&) = default;
};

}  // namespace

// ---- the Trace ---------------------------------------------------------------------------------------------------------------------

std::string Trace::exit_text() const {
    switch (exit) {
        case Exit::Returned: return "ret";
        case Exit::Truncated: return "truncated";
        case Exit::Faulted: break;
    }
    return "exception: " + fault;
}

// The harness's own write: a call through nothing lands in zeros, and the stand-in writes there.
Run record(const std::shared_ptr<const Trace>& trace_ptr, const std::optional<std::array<std::uint32_t, 4>>& sentinels) {
    const Trace& t = *trace_ptr;
    const native_trace::TraceRecord& rec = t.record;
    static const std::string kHarness = "add byte ptr [eax], al";
    auto is_harness = [&](std::uint64_t pc) {
        const std::string* mnemonic = rec.mnemonics.find(pc);
        return mnemonic != nullptr && *mnemonic == kHarness;
    };
    Run out;
    out.exit = t.exit_text();
    for (const native_trace::MemWrite& w : rec.mem_writes) {
        out.writes.push_back({w.addr, static_cast<std::uint32_t>(w.size), w.value, w.old, is_harness(w.pc)});
    }
    for (const native_trace::LeanWrite& w : rec.lean_writes) {   // a lean run's writes carry what their bytes held before the run
        std::uint64_t old = 0;
        for (std::uint64_t i = 0; i < std::min<std::uint64_t>(w.size, 8); ++i) {
            const std::uint64_t b = w.addr + i;
            const Bytes* page = rec.pages0.find(b & kPageMask);
            const std::uint64_t byte = page != nullptr && (b & (kPage - 1)) < page->size() ? (*page)[b & (kPage - 1)] : 0;
            old |= byte << (8 * i);
        }
        out.writes.push_back({w.addr, static_cast<std::uint32_t>(w.size), w.value, old, w.pc != 0 && is_harness(w.pc)});
    }
    const std::int64_t seen = rec.writes_seen ? *rec.writes_seen : static_cast<std::int64_t>(rec.mem_writes.size() + rec.lean_writes.size());
    out.capped = seen >= kMaxMem;
    out.results.insert_or_assign("EAX", t.ret ? Register(std::uint64_t{*t.ret}) : Register(std::monostate{}));
    out.results.insert_or_assign("ST0", t.st0 ? Register(*t.st0) : Register(std::monostate{}));
    if (rec.api_calls) {
        for (const native_trace::ApiCall& c : *rec.api_calls) out.calls.push_back(Call{c.api, c.args, {}});
    }
    if (t.esp_delta) out.stack = *t.esp_delta;
    if (t.saved_regs) {
        const auto& given = sentinels ? *sentinels : kSavedSentinels;
        const char* const names[4] = {"EBX", "ESI", "EDI", "EBP"};
        Preserved p;
        for (std::size_t k = 0; k < 4; ++k) p.insert_or_assign(names[k], SavedRegister{given[k], (*t.saved_regs)[k]});
        out.preserved = std::move(p);
    }
    for (const auto& [address, text] : rec.mnemonics) out.path.push_back(address);   // every instruction run, first seen first
    out.steps = static_cast<std::uint64_t>(rec.insn_count);
    out.truncated = rec.truncated;
    out.endian = Endian::Little;
    out.raw = trace_ptr;
    return out;
}

std::vector<native_trace::OutParam> out_params(const symbols::prototypes::Prototype& prototype) {
    std::vector<native_trace::OutParam> out;
    std::int64_t place = 0;
    for (const symbols::prototypes::Parameter& p : prototype.parameters) {
        std::string_view type = text::trim(p.type);
        if (const std::size_t at = type.find_last_of(" \t"); at != std::string_view::npos) type = type.substr(at + 1);   // the last word: `const LPDWORD` is LPDWORD
        if (type == "PHKEY" || type == "PHANDLE" || type == "LPHANDLE") {
            out.push_back({place, std::nullopt});
        } else if (type == "LPDWORD" || type == "PDWORD" || type == "LPLONG" || type == "PLONG" || type == "PULONG" || type == "LPUINT" ||
                   type == "PUINT") {
            out.push_back({place, 0});
        }
        ++place;
    }
    return out;
}

std::optional<std::int64_t> builtin_argc(knowledge::Knowledge& kb, std::string_view name) {
    std::optional<knowledge::Value> model = kb.api_model("win32", name);
    if (!model && !name.empty() && (name.back() == 'A' || name.back() == 'W')) {   // the ANSI / wide variant has its base's count
        model = kb.api_model("win32", name.substr(0, name.size() - 1));
    }
    if (!model) return std::nullopt;
    const knowledge::Value* argc = model->get("argc");
    return argc != nullptr ? argc->integer() : std::nullopt;
}

// ---- the Tracer --------------------------------------------------------------------------------------------------------------------

struct Tracer::Impl {
    Environment env;
    std::mutex mutex;   // guards the caches below (not a run: a run is the caller's thread's)
    std::unordered_map<std::string, std::shared_ptr<const LoadedModule>> by_name;   // the name a module was asked for by -> the same, without asking the disk
    std::unordered_map<std::string, std::shared_ptr<const LoadedModule>> by_path;
    std::map<std::pair<std::string, std::string>, std::shared_ptr<CodeSet>> code_sets;   // (module path, overlay path)
    std::vector<std::pair<MachineKey, std::unique_ptr<Machine>>> machines;               // oldest first
    std::unordered_map<std::string, std::vector<native_trace::OutParam>> outs;           // by API name
    native_trace::Governor governor;
    bool default_stub = false;
    ImportArgc default_argc;

    std::shared_ptr<const LoadedModule> carve(const std::filesystem::path& path);
    std::shared_ptr<CodeSet> code_set(const std::shared_ptr<const LoadedModule>& module, const std::shared_ptr<const LoadedModule>& overlay);
    std::unique_ptr<Machine> make_machine(const std::shared_ptr<const LoadedModule>& module, const std::shared_ptr<const LoadedModule>& overlay,
                                          std::uint32_t secbase, std::uint32_t size, bool stub, const std::vector<MemoryInit>& first);
    std::vector<native_trace::OutParam> outs_of(std::string_view api);
    native_trace::ImportResolver resolver(const ImportArgc& argc);
};

Tracer::Tracer(Environment environment) : impl_(std::make_unique<Impl>()) {
    if (!environment.resolve) throw UsageError("Tracer needs a module resolver");
    impl_->env = std::move(environment);
}

Tracer::~Tracer() = default;

native_trace::Governor& Tracer::governor() { return impl_->governor; }

void Tracer::set_default_stub(bool enabled, ImportArgc argc) {
    impl_->default_stub = enabled;
    impl_->default_argc = std::move(argc);
}

// A PE can have several code sections (kernel32: _FREQASM, .text, _INIT). They are laid out at their real addresses in one buffer starting at
// the lowest; the gaps between them (data sections) are zero-filled, which is exactly what an unmapped page reads as on demand, so nothing
// outside the code changes. Function offsets are relative to that buffer, as callers expect (`va0 + off`).
std::shared_ptr<const LoadedModule> Tracer::Impl::carve(const std::filesystem::path& path) {
    const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(path);
    std::span<const formats::pe_functions::CodeSection> sections = carve.sections();
    if (sections.empty()) throw FormatError("emulator", 0, path.string() + ": no executable section");
    std::uint64_t lo = UINT64_MAX, hi = 0;
    for (const auto& s : sections) {
        lo = std::min<std::uint64_t>(lo, s.va);
        hi = std::max<std::uint64_t>(hi, std::uint64_t{s.va} + s.data.size());
    }
    if (sections.size() > 1 && hi - lo > kMaxCodeSpan) {   // pathological layout: the first section only
        sections = sections.first(1);
        lo = sections[0].va;
        hi = lo + sections[0].data.size();
    }
    auto m = std::make_shared<LoadedModule>();
    m->path = path;
    m->va0 = static_cast<std::uint32_t>(lo);
    m->code.assign(static_cast<std::size_t>(hi - lo), 0);
    for (const formats::pe_functions::CodeSection& s : sections) {
        const std::uint32_t base = s.va - m->va0;
        std::copy(s.data.begin(), s.data.end(), m->code.begin() + base);
        if (!m->section.empty()) m->section += '+';
        m->section += s.name;
        m->ranges.push_back({s.va, s.va + static_cast<std::uint32_t>(s.data.size())});
        // the carve's functions that start inside this section, as offsets from the buffer's start (the end cut at the section's)
        for (const auto& [start, f] : carve.functions()) {
            if (s.contains(start)) m->functions[base + (start - s.va)] = base + (std::min(f.end, s.end()) - s.va);
        }
    }
    std::erase_if(m->functions, [](const auto& f) { return f.second - f.first < kMinFunctionBytes; });
    for (const auto& [slot, name] : carve.imports()) m->imports.emplace(slot, name);
    m->sha256 = crypto::to_hex(crypto::sha256(carve.image().data()));
    return m;
}

std::shared_ptr<const LoadedModule> Tracer::load(std::string_view module) {
    Impl& s = *impl_;
    const std::string key(module);
    {
        const std::lock_guard<std::mutex> hold(s.mutex);
        if (const auto it = s.by_name.find(key); it != s.by_name.end()) return it->second;
    }
    // a "name#member" selector addresses one module inside a bundle: the file is the bundle's, by the name before it
    const std::string_view file_name = module.substr(0, module.find('#'));
    const std::filesystem::path path = s.env.resolve(file_name);
    const std::string path_key = path.lexically_normal().string();
    {
        const std::lock_guard<std::mutex> hold(s.mutex);
        if (const auto it = s.by_path.find(path_key); it != s.by_path.end()) {
            s.by_name[key] = it->second;
            return it->second;
        }
    }
    std::shared_ptr<const LoadedModule> carved = s.carve(path);   // (the costly part: not under the lock)
    const std::lock_guard<std::mutex> hold(s.mutex);
    const auto kept = s.by_path.emplace(path_key, carved).first;   // (a module another thread made meanwhile wins)
    s.by_name[key] = kept->second;
    return kept->second;
}

ResolvedFunction Tracer::resolve(std::string_view module, const FuncSelector& function) {
    std::shared_ptr<const LoadedModule> m = load(module);
    const auto& functions = m->functions;
    if (functions.empty()) throw FormatError("emulator", 0, std::format("{}: no functions recovered in section {}", module, m->section));
    ResolvedFunction out;
    out.module = m;
    if (std::holds_alternative<std::monostate>(function)) {
        out.off = functions.begin()->first;
        out.end = functions.begin()->second;
        return out;
    }
    std::int64_t value;
    if (const auto* text = std::get_if<std::string>(&function)) {
        const auto parsed = text::parse_int<std::int64_t>(text::trim(*text), 0);
        if (!parsed) throw UsageError(std::format("{}: a function is an offset or an address, not \"{}\"", module, *text));
        value = *parsed;
    } else {
        value = static_cast<std::int64_t>(std::get<std::uint64_t>(function));
    }
    const std::int64_t off = value >= m->va0 ? value - m->va0 : value;   // a value at or above va0 is an address
    // snap to the enclosing function when the address lands inside one
    if (off >= 0 && off <= UINT32_MAX) {
        auto it = functions.upper_bound(static_cast<std::uint32_t>(off));
        if (it != functions.begin()) {
            --it;
            if (static_cast<std::uint32_t>(off) < it->second) {
                out.off = it->first;
                out.end = it->second;
                return out;
            }
        }
    }
    if (off >= 0 && off < static_cast<std::int64_t>(m->code.size())) {
        // An explicit address the recovery did not mark as a function (an export not reached by the call-graph traversal). The caller knows it
        // is a function, so it is accepted as a standalone start: its end is the next recovered function's start, else the section's.
        const auto next = functions.upper_bound(static_cast<std::uint32_t>(off));
        out.off = static_cast<std::uint32_t>(off);
        out.end = next != functions.end() ? next->first : static_cast<std::uint32_t>(m->code.size());
        return out;
    }
    throw FormatError("emulator", 0,
                      std::format("{}: no function at offset 0x{:x} (VA 0x{:x})", module, off, static_cast<std::int64_t>(m->va0) + off));
}

std::vector<native_trace::OutParam> Tracer::Impl::outs_of(std::string_view api) {
    {
        const std::lock_guard<std::mutex> hold(mutex);
        if (const auto it = outs.find(std::string(api)); it != outs.end()) return it->second;
    }
    std::vector<native_trace::OutParam> out;
    if (env.prototype) {
        if (const auto prototype = env.prototype(emu_bare_name(api))) out = out_params(*prototype);
    }
    const std::lock_guard<std::mutex> hold(mutex);
    return outs.emplace(std::string(api), std::move(out)).first->second;
}

// What the recorder asks of a run's caller: an import's argument count, and what it gives back through a pointer.
native_trace::ImportResolver Tracer::Impl::resolver(const ImportArgc& argc) {
    return [this, argc](const native_trace::ImportName& name) {
        native_trace::ImportAnswer answer;
        if (name.dll == "CALLBACK") {
            answer.argc = 0;
        } else {
            if (argc) answer.argc = argc(name);
            if (answer.argc && *answer.argc != 0) answer.outs = outs_of(name.name);
        }
        return answer;
    };
}

std::shared_ptr<CodeSet> Tracer::Impl::code_set(const std::shared_ptr<const LoadedModule>& module, const std::shared_ptr<const LoadedModule>& overlay) {
    const auto key = std::make_pair(module->path.lexically_normal().string(), overlay ? overlay->path.lexically_normal().string() : std::string());
    {
        const std::lock_guard<std::mutex> hold(mutex);
        if (const auto it = code_sets.find(key); it != code_sets.end()) return it->second;
    }
    auto set = std::make_shared<CodeSet>();
    set->module = module;
    set->overlay = overlay;
    // The module's code as it is in the file: an instruction there is decoded once per program, not once per run -- unless something was
    // written over it (then memory is what is read).
    for (const LoadedModule* m : {module.get(), overlay.get()}) {
        if (m == nullptr) continue;
        for (const CodeRange& r : m->ranges) {
            set->statics.push_back(set->cache.add_code(r.lo, ByteView(m->code).subspan(r.lo - m->va0, r.hi - r.lo)));
        }
    }
    const std::lock_guard<std::mutex> hold(mutex);
    return code_sets.emplace(key, std::move(set)).first->second;
}

// When stubbing externals, a unique SENTINEL address is planted in each IAT slot so a `call [iat_slot]` resolves to a NAMED import (not a null
// pointer), and its stdcall argument count can be looked up for correct stack cleanup.
std::unique_ptr<Machine> Tracer::Impl::make_machine(const std::shared_ptr<const LoadedModule>& module, const std::shared_ptr<const LoadedModule>& overlay,
                                                    std::uint32_t secbase, std::uint32_t size, bool stub, const std::vector<MemoryInit>& first) {
    auto mach = std::make_unique<Machine>();
    mach->uc = env.open_engine ? env.open_engine() : unicorn_uc::open_x86(32);
    core16::Uc& uc = *mach->uc;
    mach->code = code_set(module, overlay);
    const std::uint64_t image_size = std::max(size, kPage);
    uc.mem_map(secbase, image_size);
    uc.mem_write(module->va0, module->code);
    uc.mem_map(kScratchBase, kScratchSize);
    uc.mem_map(kStackBase, kStackSize);
    mach->mapped.insert(secbase);
    mach->mapped.insert(kScratchBase);
    for (std::uint64_t a = kStackBase; a < std::uint64_t{kStackBase} + kStackSize; a += kPage) mach->mapped.insert(a);
    auto in_image = [&](std::uint64_t base) { return secbase <= base && base < secbase + image_size; };
    auto in_scratch = [&](std::uint64_t base) { return kScratchBase <= base && base < std::uint64_t{kScratchBase} + kScratchSize; };

    std::vector<MemoryInit> mem_init = first;
    std::map<std::uint32_t, formats::pe_functions::ImportName> imports = module->imports;
    std::optional<std::size_t> laid;   // where the overlay's image is in mem_init
    if (overlay) {
        const std::uint64_t own_hi = secbase + image_size;
        if (overlay->va0 + overlay->code.size() <= secbase || overlay->va0 >= own_hi) {   // never over the traced image
            mem_init.insert(mem_init.begin(), MemoryInit{overlay->va0, overlay->code});
            laid = 0;
            for (const auto& [slot, name] : overlay->imports) imports.emplace(slot, name);   // (the module's own imports win)
        }
    }
    if (stub && !imports.empty()) {
        std::uint32_t sent = kSentinelBase;
        for (const auto& [iat_va, name] : imports) {   // ascending slot address: the sentinels are handed out in that order
            const IatSlot slot{iat_va, {static_cast<std::uint8_t>(sent), static_cast<std::uint8_t>(sent >> 8), static_cast<std::uint8_t>(sent >> 16),
                                        static_cast<std::uint8_t>(sent >> 24)}};
            mem_init.push_back(MemoryInit{iat_va, Bytes(slot.sentinel.begin(), slot.sentinel.end())});
            mach->slots[iat_va & kPageMask].push_back(slot);
            if (((iat_va + 3) & kPageMask) != (iat_va & kPageMask)) mach->slots[(iat_va + 3) & kPageMask].push_back(slot);
            mach->sentinels.push_back({sent, {name.dll, name.name}});
            sent += kSentinelStride;
        }
        const std::uint64_t span = (imports.size() * std::uint64_t{kSentinelStride} + (kPage - 1)) & kPageMask;
        mem_init.push_back(MemoryInit{kSentinelBase, Bytes(span, 0)});   // map and zero the sentinel landing region
    }
    // pages given other contents before the run are not "as in the file"
    for (std::size_t i = 0; i < mem_init.size(); ++i) {
        if (laid && *laid == i) continue;
        emu_each_page(mem_init[i].address, mem_init[i].data.size(), [&](std::uint64_t page) { mach->init_pages.insert(page); });
    }
    // pre-load memory (e.g. a controlled struct buffer a pointer arg points at); pages that lie side by side are mapped together -- every
    // mapping makes every later one slower
    std::set<std::uint64_t> need;
    for (const MemoryInit& item : mem_init) {
        for (std::uint64_t a = item.address; a < std::uint64_t{item.address} + item.data.size();) {
            const std::uint64_t base = a & kPageMask;
            if (!mach->mapped.contains(base) && !in_image(base) && !in_scratch(base)) need.insert(base);
            a = base + kPage;
        }
    }
    std::optional<std::pair<std::uint64_t, std::uint64_t>> run;   // (base, length) of the group being built
    for (const std::uint64_t base : need) {
        if (run && base == run->first + run->second) {
            run->second += kPage;
            continue;
        }
        if (run) uc.mem_map(run->first, run->second);
        run = std::make_pair(base, std::uint64_t{kPage});
    }
    if (run) uc.mem_map(run->first, run->second);
    mach->mapped.insert(need.begin(), need.end());
    for (const MemoryInit& item : mem_init) uc.mem_write(item.address, item.data);
    mach->context = uc.context_save();
    return mach;
}

Trace Tracer::trace(std::string_view module, const FuncSelector& function, const TraceOptions& options) {
    Impl& s = *impl_;
    const RegisterIds& reg = emu_registers();
    const bool stub = options.stub_externals.value_or(s.default_stub);   // inherit the process default
    const ImportArgc& argc = options.argc ? options.argc : s.default_argc;
    const ResolvedFunction rf = resolve(module, function);
    const std::shared_ptr<const LoadedModule>& loaded = rf.module;
    std::shared_ptr<const LoadedModule> overlay;
    if (options.overlay) {
        try {
            overlay = load(*options.overlay);
        } catch (const Error& e) {   // (an overlay that cannot be had is not laid out: the run is the same as one without it)
            log::debug(std::format("emulator: overlay {} not laid out: {}", *options.overlay, e.what()));
        }
    }
    const std::uint32_t va0 = loaded->va0, fva = rf.va();
    std::vector<std::uint32_t> args;
    if (options.args) args = *options.args;
    else args.assign(static_cast<std::size_t>(std::max(options.arity.value_or(4), 0)), 0);   // (no recovered arities here: four dwords)

    const std::uint32_t secbase = va0 & static_cast<std::uint32_t>(kPageMask);
    const std::uint32_t size = static_cast<std::uint32_t>((loaded->code.size() + (va0 - secbase) + (kPage - 1)) & kPageMask);
    const std::uint64_t image_size = std::max(size, kPage);
    auto in_image = [&](std::uint64_t base) { return secbase <= base && base < secbase + image_size; };
    auto in_scratch = [&](std::uint64_t base) { return kScratchBase <= base && base < std::uint64_t{kScratchBase} + kScratchSize; };

    // A machine with the module laid out is kept between runs (reuse) and put back exactly as it was after each: laying a module out costs more
    // than most runs of a function do.
    const bool reuse = options.reuse;
    const MachineKey key{loaded->path.lexically_normal().string(), loaded->sha256, overlay ? overlay->path.lexically_normal().string() : std::string(), stub,
                         options.fast};
    std::unique_ptr<Machine> mach;
    if (reuse) {
        const std::lock_guard<std::mutex> hold(s.mutex);
        for (auto it = s.machines.begin(); it != s.machines.end(); ++it) {
            if (it->first == key) {
                mach = std::move(it->second);
                s.machines.erase(it);
                break;
            }
        }
    }
    if (!mach) mach = s.make_machine(loaded, overlay, secbase, size, stub, reuse ? std::vector<MemoryInit>() : options.mem_init);
    core16::Uc& uc = *mach->uc;

    std::unordered_set<std::uint64_t> mapped = mach->mapped;
    std::unordered_set<std::uint64_t> init_pages = mach->init_pages;
    for (const MemoryInit& item : options.mem_init) emu_each_page(item.address, item.data.size(), [&](std::uint64_t page) { init_pages.insert(page); });
    std::map<std::uint64_t, std::optional<Bytes>> saved;   // page -> what it held (none: not there before)
    if (reuse) {
        uc.context_restore(*mach->context);
        std::set<std::uint64_t> again;   // pages with import slots that the caller's memory was written over
        for (const MemoryInit& item : options.mem_init) {   // this run's memory, over the kept machine
            for (std::uint64_t a = item.address; a < std::uint64_t{item.address} + item.data.size();) {
                const std::uint64_t base = a & kPageMask;
                if (!saved.contains(base)) {
                    if (!mapped.contains(base) && !in_image(base) && !in_scratch(base)) {
                        uc.mem_map(base, kPage);
                        mapped.insert(base);
                        saved.emplace(base, std::nullopt);
                    } else {
                        saved.emplace(base, uc.mem_read(base, kPage));
                    }
                }
                if (mach->slots.contains(base)) again.insert(base);
                a = base + kPage;
            }
            uc.mem_write(item.address, item.data);
        }
        for (const std::uint64_t base : again) {   // import slots are written last, as ever
            for (const IatSlot& slot : mach->slots.at(base)) uc.mem_write(slot.address, slot.sentinel);
        }
        const std::uint64_t top = (kStackTop - 4 * (args.size() + 1)) & kPageMask;
        for (std::uint64_t base = top; base < std::uint64_t{kStackTop} + 1; base += kPage) {
            if (!saved.contains(base) && mapped.contains(base)) saved.emplace(base, uc.mem_read(base, kPage));
        }
    }

    // Build the stdcall frame: args in order above a return sentinel the emulation halts on.
    const std::uint32_t entry_esp = kStackTop - 4 * static_cast<std::uint32_t>(args.size() + 1);
    {
        Bytes frame = emu_le32(kReturnAddress);
        for (const std::uint32_t a : args) append(frame, emu_le32(a));
        uc.mem_write(entry_esp, frame);
    }
    uc.reg_write(reg.esp, entry_esp);
    // Sentinels in the registers a function must preserve: one that hands a register back changed is caught though its caller here holds nothing
    // of value there. (A function's callers keep live values in these; a rebuilt body that loses one breaks them, not itself.)
    const std::array<std::uint32_t, 4>& sentinels = options.sentinels ? *options.sentinels : kSavedSentinels;
    const int preserved_regs[4] = {reg.ebx, reg.esi, reg.edi, reg.ebp};
    for (std::size_t k = 0; k < 4; ++k) uc.reg_write(preserved_regs[k], sentinels[k]);
    if (options.ecx) uc.reg_write(reg.ecx, *options.ecx);   // an argument in ECX: a member function's object
    if (options.edx) uc.reg_write(reg.edx, *options.edx);

    Trace out;
    out.module = std::filesystem::path(std::string(module)).filename().string();   // clean label (the module may be a path)
    out.module_path = loaded->path;
    out.target_sha256 = loaded->sha256;
    out.section = loaded->section;
    out.func_off = rf.off;
    out.func_va = fva;
    out.func_end_va = rf.end_va();
    out.args = args;
    out.entry_esp = entry_esp;

    // The hooks are the recorder's, attached to this Unicorn in trace_function's order, keeping the record -- and a shadow (the path
    // explorer's) stepped there too.
    native_trace::Options o;
    o.cache = &mach->code->cache;
    o.statics = mach->code->statics;
    for (const std::uint64_t p : init_pages) o.init_pages.push_back(p);
    o.mapped.assign(mapped.begin(), mapped.end());
    if (reuse) {
        std::vector<std::uint64_t> keys;
        for (const auto& [base, held] : saved) keys.push_back(base);
        o.saved = std::move(keys);
    }
    o.sentinels = mach->sentinels;
    if (stub && !options.stub_at.empty()) {   // functions entered as if they were imports: a landing at an address replaces one made for it
        for (const native_trace::Sentinel& extra : options.stub_at) {
            const auto same = std::find_if(o.sentinels.begin(), o.sentinels.end(), [&](const auto& have) { return have.address == extra.address; });
            if (same != o.sentinels.end()) *same = extra;
            else o.sentinels.push_back(extra);
        }
    }
    o.module_hash = loaded->sha256;
    o.governor = &s.governor;
    o.fast = options.fast && s.governor.ceil == 0 && options.shadow == nullptr;
    o.lean = options.lean;
    o.reads = options.reads;
    if (options.quiet_stack) o.quiet = native_trace::AddressRange{kStackBase, std::uint64_t{kStackBase} + kStackSize};
    o.stop_wild = options.stop_wild;
    o.stub = stub;
    o.unique = options.stub_ret_unique;
    o.branches = options.branches;
    o.pads = options.pads;
    o.insn_cap = options.insn_cap;
    o.stub_ret = options.stub_ret;
    o.stub_cleanup = options.stub_cleanup;
    o.func = {fva, rf.end_va()};
    o.image = {secbase, secbase + image_size};
    o.lean_low_end = kStackBase - 1;
    o.lean_high_begin = std::uint64_t{kStackBase} + kStackSize;
    o.resolver = s.resolver(argc);
    if (options.shadow != nullptr) o.shadow = static_cast<tracer::Shadow*>(options.shadow->handle());
    native_trace::TraceRun run(static_cast<uc_struct*>(uc.handle()), std::move(o));

    const int status = run.run(fva, kReturnAddress);   // (a fast run is made one instruction at a time when the recorder cannot take a block)
    if (status == 0) {
        out.exit = run.truncated() ? Exit::Truncated : Exit::Returned;
        // The emulator can stop without raising (a divide by zero does): it only RETURNED if it stopped at the return sentinel. Anything else
        // is a fault, wherever it stopped.
        const std::uint64_t eip = uc.reg_read(reg.eip);
        if (out.exit == Exit::Returned && eip != kReturnAddress) {
            out.exit = Exit::Faulted;
            out.fault = std::format("stopped at 0x{:x}", eip);
        }
        out.ret = static_cast<std::uint32_t>(uc.reg_read(reg.eax));
        // The registers a function must hand back unchanged, and where the stack pointer ended up relative to entry: 4 = cdecl, 4 + N = stdcall
        // with N bytes.
        try {
            out.saved_regs = std::array<std::uint32_t, 4>{static_cast<std::uint32_t>(uc.reg_read(reg.ebx)), static_cast<std::uint32_t>(uc.reg_read(reg.esi)),
                                                          static_cast<std::uint32_t>(uc.reg_read(reg.edi)), static_cast<std::uint32_t>(uc.reg_read(reg.ebp))};
            out.esp_delta = static_cast<std::uint32_t>(uc.reg_read(reg.esp)) - entry_esp;
        } catch (const unicorn_uc::UcFault&) {   // (the engine is in no state to be asked: the registers stay unknown)
            out.saved_regs.reset();
            out.esp_delta.reset();
        }
        try {   // x87 return (float functions -> st0)
            out.st0 = uc.reg_read_float(reg.st0);
        } catch (const unicorn_uc::UcFault&) {
        }
    } else {
        out.exit = Exit::Faulted;
        out.fault = unicorn_uc::describe_error(status);
        try {
            out.ret = static_cast<std::uint32_t>(uc.reg_read(reg.eax));
        } catch (const unicorn_uc::UcFault&) {
        }
    }
    out.record = run.collect();
    if (options.lean) out.record.writes_seen = run.lean_seen();
    if (reuse) {   // the machine, as it was, for the next run
        try {
            if (options.lean) uc.mem_write(kStackBase, Bytes(kStackSize, 0));   // (its writes were not followed)
            run.detach();
            for (const auto& [base, held] : saved) {
                if (!held) uc.mem_unmap(base, kPage);
                else uc.mem_write(base, *held);
            }
            run.restore();   // (the pages its hooks saved, after ours)
            const std::lock_guard<std::mutex> hold(s.mutex);
            while (s.machines.size() >= kMaxMachines) s.machines.erase(s.machines.begin());
            s.machines.emplace_back(key, std::move(mach));
        } catch (const Error& e) {   // not put back: the next run makes its own
            log::debug(std::format("emulator: a machine was not kept: {}", e.what()));
        }
    }
    return out;
}

Run Tracer::recorded(std::string_view module, const FuncSelector& function, const TraceOptions& options) {
    auto t = std::make_shared<const Trace>(trace(module, function, options));
    return record(t, options.sentinels);
}

}  // namespace cb::emulate::emulator
