// emulate/driver.cpp -- see driver.hpp.
#include "emulate/driver.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <unordered_set>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "emulate/unicorn_uc.hpp"
#include "formats/le.hpp"

namespace cb::emulate::driver {

using core16::Uc;

namespace {

// The 32-bit address of a place of an LE image under the address plan every reader of this program shares (one object per 16M).
std::uint32_t driver_le_place(std::uint32_t object, std::int64_t offset) {
    if (object < 1 || object > 255) throw FormatError("driver image", 0, std::format("object {} is outside the address plan", object));
    return static_cast<std::uint32_t>(core16::le_address(object, offset));
}

void driver_put_u32(std::uint8_t* at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) at[i] = static_cast<std::uint8_t>((v >> (8 * i)) & 0xFF);
}

// The registers the machine's inputs name, with Unicorn's ids.
struct DriverRegisterIds {
    int eax, ebx, ecx, edx, esi, edi, ebp, esp, eip, eflags;
};
const DriverRegisterIds& driver_regs() {
    static const DriverRegisterIds ids = [] {
        auto id = [](std::string_view name) {
            const std::optional<int> r = unicorn_uc::x86_register(name);
            CB_ASSERT(r.has_value(), "an x86 register Unicorn does not name");
            return *r;
        };
        return DriverRegisterIds{id("EAX"), id("EBX"), id("ECX"), id("EDX"), id("ESI"), id("EDI"), id("EBP"), id("ESP"), id("EIP"), id("EFLAGS")};
    }();
    return ids;
}

constexpr std::array<std::string_view, 7> kDriverServiceRegisters = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"};   // native_driver's order

}  // namespace

// ---- the image -----------------------------------------------------------------------------------------------------------------------

Image Image::of(const formats::le::Module& module) {
    std::map<std::uint32_t, Bytes> objects;   // by object number: the regions are in that order
    for (const formats::le::Object& o : module.objects()) objects[o.index] = module.object_data(o);
    for (const formats::le::Reference& ref : module.references()) {
        const auto it = objects.find(ref.at.object);
        // Only the places that hold an address: the 32-bit offset itself (7), or its distance from the end of the field, as in a call (8).
        if (it == objects.end() || ref.at.offset < 0 || static_cast<std::uint64_t>(ref.at.offset) + 4 > it->second.size() ||
            (ref.source != 7 && ref.source != 8))
            continue;
        std::int64_t target = driver_le_place(ref.target_object, ref.target_offset);
        if (ref.source == 8) target -= static_cast<std::int64_t>(driver_le_place(ref.at.object, ref.at.offset)) + 4;
        driver_put_u32(it->second.data() + ref.at.offset, static_cast<std::uint32_t>(target & 0xFFFFFFFF));
    }
    Image image;
    crypto::Hasher digest(crypto::Hasher::Algorithm::Sha1);
    for (const auto& [index, bytes] : objects) {
        if (bytes.empty()) continue;
        const std::uint32_t low = driver_le_place(index, 0);
        image.spans.push_back({low, low + static_cast<std::uint32_t>(bytes.size()), index});
        digest.update(bytes);
        image.regions.push_back({low, bytes});
    }
    // Everywhere the system or another driver can enter it.
    if (const std::optional<formats::le::Descriptor> d = module.descriptor()) {
        if (d->at_control) image.entries.insert_or_assign("control", driver_le_place(d->at_control->object, d->at_control->offset));
        if (d->at_v86_api) image.entries.insert_or_assign("v86_api", driver_le_place(d->at_v86_api->object, d->at_v86_api->offset));
        if (d->at_pm_api) image.entries.insert_or_assign("pm_api", driver_le_place(d->at_pm_api->object, d->at_pm_api->offset));
        const std::vector<std::optional<formats::le::Place>> table = module.service_table(*d);
        for (std::size_t i = 0; i < table.size(); ++i)
            if (table[i]) image.entries.insert_or_assign(std::format("service_{}", i), driver_le_place(table[i]->object, table[i]->offset));
    }
    for (const formats::le::Object& o : module.objects()) image.shape.push_back(o.vsize);
    image.digest = crypto::to_hex(digest.finish());
    return image;
}

Image Image::load(const std::filesystem::path& file, std::optional<std::string_view> member) {
    const std::vector<std::pair<std::string, formats::le::Module>> found = formats::le::open_modules(file);
    if (found.empty()) throw FormatError("driver image", 0, "the file holds no LE image");
    if (member) {
        for (const auto& [name, module] : found)
            if (text::lower(name) == text::lower(*member)) return of(module);
    }
    return of(found.front().second);
}

bool Image::holds(std::uint32_t address) const {
    return std::any_of(spans.begin(), spans.end(), [&](const Span& s) { return s.low <= address && address < s.high; });
}

// ---- the resolver ----------------------------------------------------------------------------------------------------------------------

native_driver::ServiceResolver resolver_of(std::function<std::optional<std::string>(std::uint32_t, std::uint32_t)> name_of,
                                           std::function<std::optional<std::string>(std::string_view)> answers_in) {
    return [name_of = std::move(name_of), answers_in = std::move(answers_in)](std::uint32_t device, std::uint32_t service) -> std::optional<unsigned> {
        const std::optional<std::string> name = name_of ? name_of(device, service) : std::nullopt;
        const std::optional<std::string> where = answers_in ? answers_in(name.value_or("")) : std::nullopt;
        if (!where) return std::nullopt;
        for (unsigned i = 0; i < kDriverServiceRegisters.size(); ++i)
            if (kDriverServiceRegisters[i] == *where) return i;
        return std::nullopt;   // a register the stand-ins do not name: the answer is in EAX
    };
}

// ---- one run -----------------------------------------------------------------------------------------------------------------------------

std::string Record::exit_text() const {
    switch (ending) {
        case Ending::Returned: return "ret";
        case Ending::Truncated: return "truncated";
        case Ending::Hooked: return observed.exit ? observed.exit->text() : std::string("wild");
        case Ending::Stopped: return std::format("exception: stopped at 0x{:x}", number);
        case Ending::Faulted: return "exception: " + fault_text;
    }
    return "ret";
}

Record run(const Environment& env, const Image& image, std::uint32_t entry, const RunOptions& options) {
    if (!env.governor) throw UsageError("a driver run needs a governor");
    const DriverRegisterIds& r = driver_regs();
    constexpr std::uint32_t page = emulator::kPage;
    std::unique_ptr<Uc> machine = env.open_engine ? env.open_engine() : unicorn_uc::open_x86(32);
    Uc& uc = *machine;

    // Maps the pages [address, address + size) not mapped yet, each run of new pages in one piece.
    std::unordered_set<std::uint32_t> mapped;   // page bases; lookup only
    auto provide = [&](std::uint64_t address, std::uint64_t size) {
        std::vector<std::uint64_t> want;
        for (std::uint64_t b = address & ~std::uint64_t{page - 1}; b < address + size; b += page)
            if (!mapped.contains(static_cast<std::uint32_t>(b))) want.push_back(b);
        for (std::size_t at = 0; at < want.size();) {
            std::size_t n = 1;
            while (at + n < want.size() && want[at + n] == want[at] + n * page) ++n;
            uc.mem_map(want[at], n * page);
            for (std::size_t k = 0; k < n; ++k) mapped.insert(static_cast<std::uint32_t>(want[at + k]));
            at += n;
        }
    };
    for (const Region& region : image.regions) {
        provide(region.address, region.bytes.size());
        uc.mem_write(region.address, region.bytes);
    }
    provide(kScratch, emulator::kScratchSize);
    provide(kStackLow, kStackHigh - kStackLow);
    for (const MemoryInit& m : options.memory) {
        provide(m.address, m.data.size());
        uc.mem_write(m.address, m.data);
    }
    std::uint32_t esp = emulator::kStackTop;
    auto push = [&](std::uint32_t word) {
        esp -= 4;
        std::array<std::uint8_t, 4> b;
        driver_put_u32(b.data(), word);
        uc.mem_write(esp, ByteView(b));
    };
    for (auto it = options.stack.rbegin(); it != options.stack.rend(); ++it) push(*it);
    push(kReturn);
    uc.reg_write(r.esp, esp);
    const std::array<std::pair<int, std::optional<std::uint32_t>>, 7> inputs = {{{r.eax, options.registers.eax}, {r.ebx, options.registers.ebx},
                                                                                  {r.ecx, options.registers.ecx}, {r.edx, options.registers.edx},
                                                                                  {r.esi, options.registers.esi}, {r.edi, options.registers.edi},
                                                                                  {r.ebp, options.registers.ebp}}};
    for (const auto& [id, value] : inputs)
        if (value) uc.reg_write(id, *value);
    uc.reg_write(r.eflags, (uc.reg_read(r.eflags) & ~std::uint64_t{0x41}) | (options.flags & 0x41));

    // The hooks are the recorder's (cb/emulate/tracer), attached in this order: the code (the driver's own, told from a call out), memory, the
    // INT 20h service calls, the ports; and a shadow stepped there too. The resolver is asked once per distinct service which register it
    // answers in.
    native_driver::Options o;
    o.vm_address = kVm;
    o.return_address = kReturn;
    o.scratch = {kScratch, kScratch + emulator::kScratchSize};
    for (const Span& s : image.spans) o.spans.push_back({s.low, s.high});
    o.cap = options.cap;
    o.service_eax = options.service_eax;
    o.service_carry = options.service_carry;
    o.governor = env.governor;
    o.resolver = env.resolver;
    if (options.shadow) o.shadow = static_cast<tracer::Shadow*>(options.shadow->handle());
    native_driver::DriverRun hooks(static_cast<uc_struct*>(uc.handle()), std::move(o));

    // No clock: a run ends by returning, faulting, or at a count (instructions, memory accesses, pages) -- so the same run always ends the same
    // way and can be remembered. What the resolver threw comes out of start().
    const int status = hooks.start(entry, static_cast<std::size_t>(options.cap) + 1);
    Record rec;
    rec.observed = hooks.collect();
    if (rec.observed.exit) {
        rec.ending = Ending::Hooked;
    } else if (status != 0) {
        rec.ending = Ending::Faulted;
        rec.fault_text = unicorn_uc::describe_error(status);
    } else if (rec.observed.truncated) {
        rec.ending = Ending::Truncated;
    } else if (const std::uint64_t at = uc.reg_read(r.eip); at != kReturn) {
        rec.ending = Ending::Stopped;
        rec.number = static_cast<std::uint32_t>(at);
    }
    rec.eax = static_cast<std::uint32_t>(uc.reg_read(r.eax));
    rec.flags = static_cast<std::uint32_t>(uc.reg_read(r.eflags) & 0x41);   // carry and zero
    const std::array<int, 6> results = {r.ebx, r.ecx, r.edx, r.esi, r.edi, r.ebp};
    for (std::size_t i = 0; i < results.size(); ++i) rec.registers[i] = static_cast<std::uint32_t>(uc.reg_read(results[i]));
    rec.esp_delta = static_cast<std::uint32_t>(uc.reg_read(r.esp)) - esp;
    return rec;
}

Run record(const std::shared_ptr<const Record>& record) {
    const Record& rec = *record;
    const native_driver::DriverRecord& seen = rec.observed;
    Run out;
    out.exit = rec.exit_text();
    out.results.insert_or_assign("eax", Register{std::uint64_t{rec.eax}});
    out.results.insert_or_assign("flags", Register{std::uint64_t{rec.flags}});
    for (std::size_t i = 0; i < kResultRegisters.size(); ++i) out.results.insert_or_assign(std::string(kResultRegisters[i]), Register{std::uint64_t{rec.registers[i]}});
    for (const native_trace::MemWrite& w : seen.mem_writes) out.writes.push_back({w.addr, static_cast<std::uint32_t>(w.size), w.value, w.old, false});
    out.capped = static_cast<std::int64_t>(seen.mem_writes.size()) >= emulator::kMaxMem;
    for (const native_driver::DriverCall& c : seen.calls) {
        Call call;
        std::vector<std::int64_t> args;   // the registers, then the four words at ESP
        for (const std::uint64_t v : c.registers) args.push_back(static_cast<std::int64_t>(v));
        for (const std::uint64_t v : c.stack_words) args.push_back(static_cast<std::int64_t>(v));
        call.args = std::move(args);
        call.detail = c.target;
        out.calls.push_back(std::move(call));
    }
    for (const native_driver::PortAccess& p : seen.ports) {
        out.io.push_back({std::nullopt, p.kind == native_driver::PortAccess::Kind::In ? "in" : "out", p.port, p.value, static_cast<std::uint32_t>(p.size)});
    }
    out.stack = rec.esp_delta;
    out.path = seen.cover;
    std::sort(out.path.begin(), out.path.end());   // the order the addresses were first reached in is not a result
    out.steps = static_cast<std::uint64_t>(seen.insns);
    out.truncated = seen.truncated;
    out.raw = record;
    return out;
}

Run recorded(const Environment& env, const Image& image, std::uint32_t entry, const RunOptions& options) {
    return record(std::make_shared<const Record>(run(env, image, entry, options)));
}

}  // namespace cb::emulate::driver
