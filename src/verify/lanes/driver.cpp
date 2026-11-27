#include "verify/lanes/driver.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "base/cache.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "emulate/native_shadow.hpp"
#include "formats/le.hpp"
#include "jobs/pool.hpp"
#include "platforms/decoder.hpp"
#include "platforms/vxd.hpp"
#include "verify/teeth.hpp"

namespace cb::verify::lanes {

namespace {

namespace edrv = emulate::driver;
namespace stdfs = std::filesystem;

// The numbers a register input has in the path explorer (concolic::InputVar::Place::Reg): the seven registers, then the two flags.
constexpr std::int64_t kFlagCarry = 7, kFlagZero = 8;

constexpr std::array<std::string_view, 7> kRegisterNames = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"};

// A value as two builds of the same code agree on it (lane_common::argument): an address in the stack is "S"; one in the driver is "P",
// somewhere in the driver, when the two files are not laid out alike.
std::string drv_value(const edrv::Image& image, std::uint64_t v, bool aligned) {
    lane_common::ArgumentContext context;
    context.frame = {lane_common::AddressRange{edrv::kStackLow, edrv::kStackHigh}};
    if (!aligned) context.image = std::function<bool(std::int64_t)>([&image](std::int64_t a) { return image.holds(static_cast<std::uint32_t>(a)); });
    return lane_common::argument(static_cast<std::int64_t>(v), context);
}

std::string drv_service(const symbols::driver_services::Table& table, std::uint32_t device, std::uint32_t service) {
    const std::optional<std::string> dev = table.device_name(device);
    const std::string name = dev && !dev->empty() ? *dev : std::format("{:04X}", device);
    return std::format("{}.{}{}", name, table.label(device, service & symbols::driver_services::kServiceNumberMask), (service & 0x8000) != 0 ? "!" : "");
}

std::string drv_digest16(const Bytes& data) { return crypto::to_hex(crypto::sha1(data)).substr(0, 16); }

// What the run left in memory outside its stack: (bytes changed, digest). Memory outside the driver by address; the driver's own by address too when
// the two files are laid out alike, else as the values stored in the order they were stored. nullopt: too many to record.
std::optional<std::pair<std::size_t, std::string>> drv_effects(const edrv::Image& image, const emulate::Run& r, bool aligned) {
    if (r.capped) return std::nullopt;
    struct Held {
        bool token = false;
        std::string text;         // "S" or "P"
        std::uint8_t byte = 0;
    };
    std::map<std::uint64_t, Held> state;            // sorted by address: the digest depends on it
    std::map<std::uint64_t, std::uint8_t> before;   // the byte each place held before the first write to it
    struct Own {
        std::uint32_t size = 0;
        bool token = false;
        std::string text;
        std::uint64_t value = 0;
    };
    std::vector<Own> own;
    for (const emulate::MemoryWrite& w : r.writes) {
        const std::uint32_t n = std::min<std::uint32_t>(w.size, 8);
        const std::uint64_t value = w.value;
        if (edrv::kStackLow <= w.address && w.address < edrv::kStackHigh) continue;
        const bool inside = image.holds(static_cast<std::uint32_t>(w.address));
        if (inside && !aligned) {
            Own o;
            o.size = n;
            if (n == 4) {
                o.token = true;
                o.text = drv_value(image, value, aligned);
            } else {
                o.value = n >= 8 ? value : value & ((std::uint64_t{1} << (8 * n)) - 1);
            }
            own.push_back(std::move(o));
            continue;
        }
        const std::uint64_t old = w.old.value_or(0);
        for (std::uint32_t i = 0; i < n; ++i) before.try_emplace(w.address + i, static_cast<std::uint8_t>(old >> (8 * i)));
        if (n == 4) {
            const std::string token = drv_value(image, value, aligned);
            if (token == "S" || token == "P") {
                for (std::uint32_t i = 0; i < 4; ++i) state[w.address + i] = Held{true, token, 0};
                continue;
            }
        }
        for (std::uint32_t i = 0; i < n; ++i) state[w.address + i] = Held{false, {}, static_cast<std::uint8_t>(value >> (8 * i))};
    }
    io::ByteWriter w;
    std::size_t changed = 0;
    for (const auto& [address, held] : state) {
        const auto was = before.find(address);
        if (!held.token && was != before.end() && was->second == held.byte) continue;   // a place that ended up with the value it had
        ++changed;
        w.u64(address);
        w.u8(held.token ? 1 : 0);
        if (held.token) w.cstr(held.text);
        else w.u8(held.byte);
    }
    for (const Own& o : own) {
        ++changed;
        w.u32(o.size);
        w.u8(o.token ? 1 : 0);
        if (o.token) w.cstr(o.text);
        else w.u64(o.value);
    }
    return std::pair{changed, drv_digest16(w.data())};
}

void drv_put32(Bytes& out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}

// Everything of an input that decides what a run does, as bytes (a part of the fingerprint's key).
Bytes drv_vector_bytes(const DriverVector& v) {
    Bytes out;
    for (std::uint32_t r : v.regs) drv_put32(out, r);
    drv_put32(out, static_cast<std::uint32_t>(v.stack.size()));
    for (std::uint32_t w : v.stack) drv_put32(out, w);
    drv_put32(out, v.flags);
    drv_put32(out, static_cast<std::uint32_t>(v.memory.size()));
    for (const edrv::MemoryInit& m : v.memory) {
        drv_put32(out, m.address);
        drv_put32(out, static_cast<std::uint32_t>(m.data.size()));
        out.insert(out.end(), m.data.begin(), m.data.end());
    }
    drv_put32(out, static_cast<std::uint32_t>(v.stub_ret.value_or(0)));
    drv_put32(out, v.stub_carry ? 1 : 0);
    return out;
}

edrv::RunOptions drv_options(const DriverVector& v, emulate::native_shadow::Shadow* shadow = nullptr) {
    edrv::RunOptions o;
    o.registers.eax = v.reg(DriverRegister::Eax);
    o.registers.ebx = v.reg(DriverRegister::Ebx);
    o.registers.ecx = v.reg(DriverRegister::Ecx);
    o.registers.edx = v.reg(DriverRegister::Edx);
    o.registers.esi = v.reg(DriverRegister::Esi);
    o.registers.edi = v.reg(DriverRegister::Edi);
    o.registers.ebp = v.reg(DriverRegister::Ebp);
    o.stack = v.stack;
    o.memory = v.memory;
    o.service_eax = static_cast<std::uint32_t>(v.stub_ret.value_or(0));
    o.service_carry = v.stub_carry;
    o.flags = v.flags;
    o.shadow = shadow;
    return o;
}

// The comparer's printing of an input's arguments: the six general registers, then the stack words.
void drv_set_args(DriverVector& v) {
    v.args.clear();
    for (std::size_t i = 0; i < 6; ++i) v.args.push_back(v.regs[i]);
    for (std::uint32_t w : v.stack) v.args.push_back(w);
}

concolic::PathEntry drv_path_entry(const emulate::native_shadow::PathEntry& e) {
    concolic::PathEntry p;
    p.site = static_cast<std::int64_t>(e.site);
    p.taken = e.taken;
    p.cond = e.condition;
    switch (e.kind) {
    case emulate::native_shadow::PathKind::Jump: p.kind = concolic::Kind::Jump; break;
    case emulate::native_shadow::PathKind::Inner: p.kind = concolic::Kind::Inner; break;
    case emulate::native_shadow::PathKind::Pin: p.kind = concolic::Kind::Pin; break;
    }
    return p;
}

// The number the path explorer gives a register input, by the name it was entered under ("eax" ... "ebp", "cf", "zf").
std::optional<std::int64_t> drv_register_number(std::string_view name) {
    for (std::size_t i = 0; i < kRegisterNames.size(); ++i)
        if (kRegisterNames[i] == name) return static_cast<std::int64_t>(i);
    if (name == "cf") return kFlagCarry;
    if (name == "zf") return kFlagZero;
    return std::nullopt;
}

// Input `v` with the named inputs (the path explorer's variables) set to `values`; nullopt when nothing changed.
std::optional<DriverVector> drv_derive(const DriverVector& v, const concolic::Values& values, const concolic::Vars& names) {
    DriverVector out = v;
    bool changed = false;
    for (const auto& [name, value] : values) {
        const auto known = names.find(name);
        if (known == names.end()) continue;
        const concolic::InputVar& var = known->second;
        const std::uint32_t word = static_cast<std::uint32_t>(value);
        using Place = concolic::InputVar::Place;
        if (var.place == Place::Arg && var.where >= 0 && static_cast<std::size_t>(var.where) < out.stack.size()) {
            std::uint32_t& slot = out.stack[static_cast<std::size_t>(var.where)];
            changed |= slot != word;
            slot = word;
        } else if (var.place == Place::Reg && var.where >= 0 && var.where < static_cast<std::int64_t>(out.regs.size())) {
            std::uint32_t& slot = out.regs[static_cast<std::size_t>(var.where)];
            changed |= slot != word;
            slot = word;
        } else if (var.place == Place::Reg && (var.where == kFlagCarry || var.where == kFlagZero)) {
            const std::uint32_t bit = var.where == kFlagCarry ? 1 : 0x40;
            const std::uint32_t now = value != 0 ? (out.flags | bit) : (out.flags & ~bit);
            changed |= now != out.flags;
            out.flags = now;
        } else if (var.place == Place::Ret) {
            changed |= static_cast<std::uint32_t>(v.stub_ret.value_or(0)) != word;
            out.stub_ret = word;
        } else if (var.place == Place::Carry) {
            const bool carry = value != 0;
            changed |= v.stub_carry != carry;
            out.stub_carry = carry;
        } else if (var.place == Place::Mem) {
            changed |= (static_cast<std::uint64_t>(var.value) & 0xFF) != (value & 0xFF);
            out.memory.push_back({static_cast<std::uint32_t>(var.where), Bytes{static_cast<std::uint8_t>(value & 0xFF)}});
        }
    }
    if (!changed) return std::nullopt;
    drv_set_args(out);
    return out;
}

// The order judged: the control procedure, then the services, then the rest; shorter labels first.
std::string drv_order_key(std::string_view label) {
    return std::format("{}{}{:04}{}", label != "control" ? 1 : 0, label.starts_with("service") ? 0 : 1, label.size(), label);
}

std::string drv_trim_note(std::string_view note) { return clip_note(note); }

}  // namespace

// ---- inputs ------------------------------------------------------------------------------------------------------------------------

std::vector<edrv::MemoryInit> driver_memory(std::optional<std::uint32_t> fill) {
    const std::uint32_t word = fill.value_or(edrv::kSafe);
    auto repeated = [&](std::size_t words) {
        Bytes b;
        for (std::size_t i = 0; i < words; ++i) drv_put32(b, word);
        return b;
    };
    Bytes sequence;
    for (int i = 0; i < 512; ++i) sequence.push_back(static_cast<std::uint8_t>(i & 0xFF));
    return {{edrv::kVm, repeated(0x40)},
            {edrv::kClient, Bytes(0x100, 0)},
            {edrv::kBuffer, sequence},
            {edrv::kObject, repeated(0x100)},
            {edrv::kSafe, Bytes(16, 0xC3)}};
}

DriverVector driver_vector(const DriverVectorSpec& s) {
    DriverVector v;
    v.reg(DriverRegister::Eax) = s.eax;
    v.reg(DriverRegister::Ebx) = s.ebx;
    v.reg(DriverRegister::Ecx) = s.ecx;
    v.reg(DriverRegister::Edx) = s.edx;
    v.reg(DriverRegister::Esi) = s.esi;
    v.reg(DriverRegister::Edi) = s.edi;
    v.reg(DriverRegister::Ebp) = edrv::kClient;
    v.stack = s.stack;
    v.memory = driver_memory(s.fill);
    v.stub_ret = s.ret;
    v.stub_carry = s.carry;
    v.flags = s.flags;
    drv_set_args(v);
    return v;
}

std::string driver_key(std::uint32_t object, std::int64_t offset) { return std::format("{}:{:x}", object, offset); }

bool driver_entry_before(std::string_view a, std::string_view b) { return drv_order_key(a) < drv_order_key(b); }

// ---- the lane ----------------------------------------------------------------------------------------------------------------------

struct DriverLane::State {
    mutable std::mutex lock;
    std::map<std::string, std::shared_ptr<const edrv::Image>> images;   // by path; lookup only
};

DriverLane::DriverLane(DriverLaneSpec spec) : state_(std::make_shared<State>()), spec_(std::move(spec)) {
    if (spec_.services == nullptr) throw UsageError("the driver lane needs the table of service names");
}

const std::string& DriverLane::name() const {
    static const std::string kName = "le";
    return kName;
}

std::shared_ptr<const edrv::Image> DriverLane::image(const stdfs::path& path) const {
    const std::string key = fs::to_text(path);
    {
        const std::lock_guard guard(state_->lock);
        if (const auto it = state_->images.find(key); it != state_->images.end()) return it->second;
    }
    auto loaded = std::make_shared<const edrv::Image>(edrv::Image::load(path));   // (read outside the lock: another thread may read the same file)
    const std::lock_guard guard(state_->lock);
    return state_->images.try_emplace(key, std::move(loaded)).first->second;
}

void DriverLane::forget_image(const stdfs::path& path) const {
    const std::lock_guard guard(state_->lock);
    state_->images.erase(fs::to_text(path));
}

DriverInputs DriverLane::inputs(std::string_view label) const {
    DriverInputs out;
    if (label == "control") {
        std::vector<std::uint32_t> messages;
        for (const auto& [number, name] : spec_.services->messages()) messages.push_back(number);
        if (messages.empty())
            for (std::uint32_t m = 0; m < 0x33; ++m) messages.push_back(m);
        for (std::uint32_t m : messages) out.first.push_back(driver_vector({.eax = m}));
        for (std::uint32_t m : {0u, 1u, 2u, 0x1Bu, 0x1Cu, 0x23u}) out.more.push_back(driver_vector({.eax = m, .ret = 1}));
        for (std::uint32_t m : {0u, 1u, 2u, 0x1Bu}) out.more.push_back(driver_vector({.eax = m, .carry = true}));
        out.more.push_back(driver_vector({.eax = 0x70000000}));
        out.more.push_back(driver_vector({.eax = 0xFFFFFFFF}));
        for (std::uint32_t m : {0u, 1u, 2u, 3u, 0x1Bu, 0x23u}) out.more.push_back(driver_vector({.eax = m, .flags = 0x40}));
        return out;
    }
    using Six = std::array<std::uint32_t, 6>;
    auto six = [](const Six& r, std::vector<std::uint32_t> stack, std::uint32_t ret = 0, bool carry = false, std::optional<std::uint32_t> fill = std::nullopt,
                  std::uint32_t flags = 1) {
        return driver_vector({.eax = r[0], .ebx = r[1], .ecx = r[2], .edx = r[3], .esi = r[4], .edi = r[5], .stack = std::move(stack), .fill = fill,
                              .ret = ret, .carry = carry, .flags = flags});
    };
    Six objs{}, plain{};
    std::vector<std::uint32_t> objs_stack, plain_stack, objs_far, plain_far;
    for (std::uint32_t i = 0; i < 6; ++i) {
        objs[i] = edrv::kObject + 0x40 * i;
        plain[i] = edrv::kBuffer + 0x20 * i;
        objs_stack.push_back(objs[i]);
        plain_far.push_back(edrv::kBuffer + 0x100 + 0x20 * i);
        objs_far.push_back(edrv::kObject + 0x200 + 0x40 * i);
    }
    out.first = {six({0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}),
                 six({1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}),
                 six(plain, plain_far),
                 six(objs, objs_far),
                 six({0, edrv::kVm, 0, 0, edrv::kBuffer, edrv::kObject}, {edrv::kVm, 0, 1, edrv::kBuffer, 0, 0})};
    out.more = {six(objs, objs_stack, 1),
                six(objs, objs_stack, edrv::kObject + 0x800),
                six(objs, objs_stack, 0, true),
                six({1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}, 1),
                six({0xFFFFFFFF, 0xFFFFFFFE, 0x80000000, 0x7FFFFFFF, 0xFFFF, 0x100}, {0xFFFFFFFF, 0x8000, 0xFF, 1, 0, 2}),
                six(objs, objs_stack, 1, false, 0xFFFFFFFF),
                six(objs, objs_stack, 0, false, 3),
                six({6, 5, 4, 3, 2, 1}, {6, 5, 4, 3, 2, 1}),
                six({objs[0], 2, 3, objs[1], 5, 6}, {objs[2], 1, 2, 3, 4, 5}),
                six(objs, objs_stack, 0, false, std::nullopt, 0x40),
                six({1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}, 0, false, std::nullopt, 0)};
    return out;
}

// ---- from a run to a fingerprint ---------------------------------------------------------------------------------------------------

std::pair<compare::Fingerprint, DriverDetail> DriverLane::run_detailed(const edrv::Image& image, std::uint32_t entry, const DriverVector& v,
                                                                       bool aligned) const {
    DriverDetail detail;
    emulate::Run r;
    try {
        emulate::native_trace::Governor governor;
        edrv::Environment env;
        env.governor = &governor;
        env.resolver = spec_.resolver;
        r = edrv::recorded(env, image, entry, drv_options(v));
    } catch (const NotFoundError&) {
        throw;   // a tool that is missing is not an observation
    } catch (const Error& e) {   // isolation boundary: a run that cannot be made is a run that was not an observation
        compare::Fingerprint fp;
        fp.err = std::format("{}: {}", e.kind(), e.what());
        return {fp, detail};
    }
    std::vector<lane_common::Called> called;
    for (const emulate::Call& c : r.calls) {
        lane_common::Called k;
        const auto* target = std::get_if<emulate::ServiceCall>(&c.detail);
        if (target != nullptr && target->device) k.name = drv_service(*spec_.services, *target->device, target->number);
        else k.name = "outside:" + drv_value(image, target != nullptr ? target->number : 0, aligned);
        k.args = c.args;
        called.push_back(std::move(k));
    }
    std::vector<std::string> deps = lane_common::calls(
        called, [&](std::int64_t x) { return drv_value(image, static_cast<std::uint64_t>(x), aligned); }, nullptr, nullptr, edrv::kMaxCalls);
    for (std::size_t i = 0; i < r.io.size() && i < edrv::kMaxCalls; ++i) {
        const emulate::IoAccess& p = r.io[i];
        deps.push_back(std::format("{}({:x},{},{:x})", p.kind, p.port, p.size.value_or(0), p.value));
    }
    const bool done = r.ended() == "ret";
    const auto memory = drv_effects(image, r, aligned);
    const auto reg = [&](std::string_view name) { return std::get<std::uint64_t>(r.results.at(std::string(name))); };
    if (done) {
        detail.result.push_back(std::format("{}", reg("flags")));
        for (std::string_view n : edrv::kResultRegisters) detail.result.push_back(drv_value(image, reg(n), aligned));
    }
    detail.memory = memory;

    lane_common::FingerprintParts parts;
    parts.ret = static_cast<std::int64_t>(reg("eax"));
    parts.saved = std::nullopt;
    parts.cov.assign(r.path.begin(), r.path.end());   // (sorted: the Run's path is)
    // the comparer's "what it left behind": memory, and for a driver the flags and registers it hands back, which are results here just as EAX is
    if (memory) {
        Bytes text(memory->second.begin(), memory->second.end());
        for (const std::string& s : detail.result) {
            text.push_back(0);
            text.insert(text.end(), s.begin(), s.end());
        }
        parts.writes = std::format("{}:{}", memory->first, drv_digest16(text));
    } else {
        parts.writes = std::nullopt;
    }
    return {lane_common::fingerprint(r, std::move(deps), std::move(parts)), std::move(detail)};
}

compare::Fingerprint DriverLane::fingerprint(const edrv::Image& image, std::uint32_t entry, const DriverVector& v, bool aligned) const {
    const std::optional<std::string> key = spec_.store ? std::optional<std::string>(compare::cache_key({"le", kRev, image.digest, entry, aligned, drv_vector_bytes(v)}))
                                                       : std::nullopt;
    const lane_common::FingerprintStore none;
    return lane_common::remembered(spec_.store ? *spec_.store : none, key, [&] { return run_detailed(image, entry, v, aligned).first; });
}

std::string DriverLane::explain(const compare::Fingerprint& a, const DriverDetail& da, const compare::Fingerprint& b, const DriverDetail& db) {
    const std::string d = compare::difference_detail(a, b);
    if (!d.starts_with("writes")) return d;
    if (da.memory != db.memory) {
        const auto count = [](const DriverDetail& x) { return x.memory ? std::to_string(x.memory->first) : std::string("?"); };
        return std::format("memory left behind differs: {} bytes changed vs {}", count(da), count(db));
    }
    const std::vector<std::string>& ra = da.result;
    const std::vector<std::string>& rb = db.result;
    if (ra.empty() != rb.empty() || (!ra.empty() && ra[0] != rb[0])) {
        const auto flags = [](const std::vector<std::string>& r) { return r.empty() ? std::string("--") : std::format("{:02x}", text::parse_int<unsigned>(r[0]).value_or(0)); };
        return std::format("flags handed back differ: carry/zero {} vs {}", flags(ra), flags(rb));
    }
    std::vector<std::string> parts;
    for (std::size_t i = 0; i < edrv::kResultRegisters.size(); ++i) {
        const std::string x = ra.size() > 1 + i ? ra[1 + i] : std::string("none"), y = rb.size() > 1 + i ? rb[1 + i] : std::string("none");
        if (x != y) parts.push_back(std::format("{} {} vs {}", edrv::kResultRegisters[i], x, y));
    }
    return "registers handed back differ: " + text::join(parts, ", ");
}

// ---- the path explorer ---------------------------------------------------------------------------------------------------------------

bool DriverLane::concolic_ready() const {
    return spec_.lifters != nullptr && lane_common::guided_on() && lane_common::concolic_on() && spec_.lifters->get(emulate::pcode::Mode::X86Flat32) != nullptr;
}

std::pair<std::vector<DriverVector>, std::shared_ptr<concolic::Exploration<DriverVector>>> DriverLane::explore_vectors(
    const edrv::Image& image, std::uint32_t entry, std::span<DriverVector> seeds) const {
    emulate::pcode::Lifter* lifter = spec_.lifters != nullptr ? spec_.lifters->get(emulate::pcode::Mode::X86Flat32) : nullptr;
    if (lifter == nullptr) return {{}, std::make_shared<concolic::Exploration<DriverVector>>()};
    using emulate::native_shadow::Place;
    std::vector<std::shared_ptr<const emulate::native_shadow::Run>> kept_runs;   // the conditions' nodes live in these until the exploration ends

    // The run of the original with its shadow: the registers it is entered with (EAX-EDI), the carry and zero flags, its stack words, the bytes of the
    // scratch memory its pointers lead into, and what a service call answers in EAX and carry are the inputs.
    const std::function<std::optional<concolic::Observation>(DriverVector&)> run = [&](DriverVector& v) -> std::optional<concolic::Observation> {
        emulate::native_shadow::Shadow shadow(emulate::native_shadow::Setup::for_lifter(
            emulate::native_shadow::Kind::Driver32, *lifter, {{edrv::kScratch, edrv::kScratch + emulate::emulator::kScratchSize}}));
        const std::uint32_t esp = emulate::emulator::kStackTop - 4 * static_cast<std::uint32_t>(v.stack.size()) - 4;
        std::vector<Place> places;
        for (std::size_t k = 0; k < v.stack.size(); ++k)
            places.push_back({.where = std::uint64_t{esp + 4 + 4 * static_cast<std::uint32_t>(k)}, .value = v.stack[k], .input = static_cast<std::int64_t>(k)});
        for (std::size_t i = 0; i < 6; ++i) {
            std::string upper(kRegisterNames[i]);
            std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
            places.push_back({.where = upper, .value = v.regs[i], .input = std::string(kRegisterNames[i])});
        }
        shadow.enter(places, 32);
        const std::vector<Place> flags = {{.where = std::string("CF"), .value = v.flags & 1, .input = std::string("cf")},
                                          {.where = std::string("ZF"), .value = (v.flags >> 6) & 1, .input = std::string("zf")}};
        shadow.enter(flags, 8);
        emulate::Run r;
        try {
            emulate::native_trace::Governor governor;
            edrv::Environment env;
            env.governor = &governor;
            env.resolver = spec_.resolver;
            r = edrv::recorded(env, image, entry, drv_options(v, &shadow));
        } catch (const NotFoundError&) {
            throw;
        } catch (const Error&) {   // isolation boundary: a run that cannot be made is no observation
            return std::nullopt;
        }
        const std::shared_ptr<const emulate::native_shadow::Run> made = shadow.run();
        kept_runs.push_back(made);
        concolic::Observation ob;
        for (const emulate::native_shadow::Step& s : made->trail) ob.trail.push_back({static_cast<std::int64_t>(s.site), s.taken});
        for (const emulate::native_shadow::PathEntry& e : made->path) ob.path.push_back(drv_path_entry(e));
        for (const auto& [var_name, var] : made->vars) {
            concolic::InputVar iv;
            iv.bits = var.bits;
            iv.value = static_cast<std::int64_t>(var.value);
            using Kind = emulate::native_shadow::VarKind;
            if (var.kind == Kind::Arg && std::holds_alternative<std::uint64_t>(var.where)) {
                iv.place = concolic::InputVar::Place::Arg;
                iv.where = static_cast<std::int64_t>(std::get<std::uint64_t>(var.where));
            } else if (var.kind == Kind::Mem && std::holds_alternative<std::uint64_t>(var.where)) {
                iv.place = concolic::InputVar::Place::Mem;
                iv.where = static_cast<std::int64_t>(std::get<std::uint64_t>(var.where));
            } else if (var.kind == Kind::Reg && std::holds_alternative<std::string>(var.where)) {
                const auto number = drv_register_number(std::get<std::string>(var.where));
                if (!number) continue;
                iv.place = concolic::InputVar::Place::Reg;
                iv.where = *number;
            } else if (var.kind == Kind::Ret) {
                iv.place = concolic::InputVar::Place::Ret;
            } else if (var.kind == Kind::Carry) {
                iv.place = concolic::InputVar::Place::Carry;
            } else {
                continue;
            }
            ob.vars[var_name] = iv;
        }
        ob.lost = made->lost;
        ob.cut = r.truncated;
        if (r.truncated) ob.lost.insert("truncated");
        return ob;
    };
    const std::function<std::optional<DriverVector>(const DriverVector&, const concolic::Values&, const concolic::Vars&)> derive = &drv_derive;
    // (each seed is run again with its shadow: the driver's runs keep no branch log to know their paths by)
    auto got = std::make_shared<concolic::Exploration<DriverVector>>(lane_common::explored<DriverVector>(
        run, seeds, derive, [&image](std::int64_t a) { return image.holds(static_cast<std::uint32_t>(a)); }, {}, 0, static_cast<int>(seeds.size())));
    return {got->kept, got};
}

// ---- the profile and the kind interface ---------------------------------------------------------------------------------------------

Profile DriverLane::profile(const stdfs::path&) const {
    std::vector<std::string> tools;
    if (spec_.tools)
        for (const auto& [tool_name, path] : spec_.tools()) tools.push_back(tool_name + "=" + base::cache::tool_id(path));
    const DriverInputs control = inputs("control"), service = inputs("service_0");
    const bool ready = concolic_ready();
    std::string stamp;
    if (ready) {
        const emulate::pcode::PcodeConfig& config = spec_.lifters->config();
        stamp = base::cache::tool_id(config.exe) + "," + base::cache::tool_id(config.sla(emulate::pcode::Mode::X86Flat32));
    }
    const symbols::driver_services::Table::Size size = spec_.services->size();
    const std::string settings = std::format(
        "kind=le judge={} comparer={} cap={} inputs={},{},{},{} segments={} tools=[{}] services={},{},{} paths={},{},{},{}", kRev, compare::kRevision,
        edrv::kInstructionCap, control.first.size(), control.more.size(), service.first.size(), service.more.size(), spec_.segment_classes,
        text::join(tools, ";"), size.service_tables, size.services_named, size.devices_named, ready, lane_common::kPathBudget, lane_common::kMaxPaths, stamp);
    const Bytes bytes(settings.begin(), settings.end());
    return {crypto::to_hex(crypto::sha1(bytes)).substr(0, 12), settings};
}

stdfs::path DriverLane::source_folder(const stdfs::path& module_path) const {
    const std::string name = text::lower(fs::to_text(module_path.stem()));
    std::vector<stdfs::path> roots;
    if (const auto env = fs::get_env("CB_DRIVER_SOURCES"); env && !env->empty()) roots.push_back(fs::from_text(*env));
    if (spec_.source_tree) roots.push_back(*spec_.source_tree);
    for (const stdfs::path& root : roots) {
        std::error_code ec;
        if (stdfs::is_regular_file(root / name / (name + ".asm"), ec)) return root / name;
    }
    return {};
}

std::unique_ptr<Prepared> DriverLane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    auto m = std::make_unique<PreparedDriver>();
    const auto sources_named = fs::get_env("CB_DRIVER_SOURCES");
    if (sources_named && !sources_named->empty()) m->source = source_folder(module_path);
    std::string text;
    if (!m->source.empty()) {
        text = fs::read_text(m->source / (fs::to_text(m->source.filename()) + ".asm"));
    } else {
        if (!spec_.writer) throw UsageError("the driver lane has no writer for the driver's source");
        m->written = spec_.writer(module_path, work / "source");
        if (m->written->linked) text = fs::read_text(m->written->source);
    }
    const OrderedMap<std::string, formats::le::Place> points = platforms::vxd_entry_points(module_path);
    std::vector<std::string> labels;
    for (const auto& [label, place] : points) labels.push_back(label);
    std::stable_sort(labels.begin(), labels.end(), [](const std::string& a, const std::string& b) { return driver_entry_before(a, b); });
    std::set<formats::le::Place> taken;
    m->header = text;
    for (const std::string& label : labels) {   // two labels, one procedure: judged once
        if (taken.insert(points.at(label)).second) m->order.push_back(label);
    }
    // the unit judged is an entry point; its text is the whole assembly file (in the header)
    const std::string file = !m->source.empty() ? fs::to_text(m->source.filename()) : fs::to_text(m->written->source.filename());
    for (const std::string& label : m->order) {
        m->texts[label] = text.empty() ? std::string() : std::format("; entry point {} of {}", label, file);
        const formats::le::Place at = points.at(label);
        if (!text.empty()) m->key[label] = driver_key(at.object, at.offset);
        m->all_keys.push_back(driver_key(at.object, at.offset));
    }
    std::sort(m->all_keys.begin(), m->all_keys.end());
    Bytes sig_text;
    const std::string rev = std::format("{}", kRev);
    sig_text.insert(sig_text.end(), rev.begin(), rev.end());
    sig_text.push_back(0);
    sig_text.insert(sig_text.end(), text.begin(), text.end());
    m->sig = crypto::to_hex(crypto::sha1(sig_text));
    return m;
}

Built DriverLane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& m = static_cast<const PreparedDriver&>(prepared);
    Built built;
    auto none_built = [&](const std::string& why) {
        for (const std::string& label : m.order) built.missing[label] = why;
    };
    stdfs::path rebuilt;
    if (m.written) {
        if (!m.written->linked) {
            none_built(m.written->notes.empty() ? "did not build" : drv_trim_note(m.written->notes.back()));
            return built;
        }
        rebuilt = m.written->built;
    } else {
        if (!spec_.build_source) throw UsageError("the driver lane has no builder for a driver's source folder");
        const DriverLaneSpec::Rebuilt r = spec_.build_source(m.source, module_path, work / "build");
        if (!r.linked) {
            const std::vector<std::string_view> lines = text::lines(r.log);
            none_built(lines.empty() ? "did not build" : drv_trim_note(lines.back()));
            return built;
        }
        rebuilt = r.image;
    }
    built.items = m.order;
    built.judge = [this, original = module_path, rebuilt](const std::string& label) { return judge_entry(original, rebuilt, label); };
    return built;
}

// ---- judging an entry point ----------------------------------------------------------------------------------------------------------

store::Judgement DriverLane::judge_entry(const stdfs::path& original, const stdfs::path& rebuilt, std::string_view label_view) const {
    const std::string label(label_view);
    using Said = store::Judgement;
    const Said made = lane_common::judged([&]() -> Said {
        std::shared_ptr<const edrv::Image> a, b;
        a = image(original);
        b = image(rebuilt);
        const auto entry_a = a->entries.find(label);
        if (entry_a == nullptr) return Said{store::Verdict::Inconclusive, "the original has no such entry point", {}, {}};
        const auto entry_b = b->entries.find(label);
        if (entry_b == nullptr) return Said{store::Verdict::Divergent, "entry-missing", {}, std::format("the rebuilt driver has no {}", label)};
        const bool aligned = a->shape == b->shape;
        const DriverInputs in = inputs(label);
        std::set<std::int64_t> ran;                       // the original's instructions that ran
        std::optional<DriverVector> last_b;               // the input the rebuilt side was last run on: where a difference was found
        const std::function<compare::Fingerprint(const DriverVector&)> run_a = [&](const DriverVector& v) {
            compare::Fingerprint f = fingerprint(*a, *entry_a, v, aligned);
            ran.insert(f.cov.begin(), f.cov.end());
            return f;
        };
        const std::function<compare::Fingerprint(const DriverVector&)> run_b = [&](const DriverVector& v) {
            last_b = v;
            return fingerprint(*b, *entry_b, v, aligned);
        };
        std::shared_ptr<concolic::Exploration<DriverVector>> found;
        const std::function<std::vector<DriverVector>(const std::set<std::int64_t>&, const std::set<std::int64_t>&)> aimed =
            [&](const std::set<std::int64_t>&, const std::set<std::int64_t>&) -> std::vector<DriverVector> {
            if (!concolic_ready()) return {};
            std::vector<DriverVector> seeds = in.first;
            seeds.insert(seeds.end(), in.more.begin(), in.more.end());
            auto [kept, exploration] = explore_vectors(*a, *entry_a, seeds);
            found = std::move(exploration);
            return kept;
        };
        const compare::Outcome outcome = compare::compare<DriverVector>(run_a, run_b, in.first, in.more, {}, aimed);
        Said j;
        j.verdict = outcome.verdict;
        j.reason = outcome.reason;
        j.limits = outcome.limit_codes();
        if (outcome.verdict != store::Verdict::Divergent) {
            const std::vector<std::string> paths = lane_common::path_limits(found.get());
            j.limits.insert(j.limits.end(), paths.begin(), paths.end());
        }
        if (!aligned) j.limits.push_back("layout-differs");
        j.detail = outcome.detail;
        if (outcome.verdict == store::Verdict::Divergent && outcome.reason == "writes-differ" && last_b) {
            const auto [fa, da] = run_detailed(*a, *entry_a, *last_b, aligned);
            const auto [fb, db] = run_detailed(*b, *entry_b, *last_b, aligned);
            const std::string said = explain(fa, da, fb, db);
            const std::size_t at = outcome.detail.find("  [input");
            j.detail = at == std::string::npos ? said : said + outcome.detail.substr(at);
        }
        if (outcome.verdict != store::Verdict::Divergent) j.detail = std::format("{} instructions of the original ran", ran.size());
        return j;
    });
    return made;
}

std::vector<std::pair<std::string, store::Judgement>> DriverLane::judge_files(const stdfs::path& rebuilt, const stdfs::path& original,
                                                                       const std::vector<std::string>& labels) const {
    const std::shared_ptr<const edrv::Image> a = image(original);
    std::vector<std::string> order;
    for (const auto& [label, at] : a->entries)
        if (labels.empty() || std::find(labels.begin(), labels.end(), label) != labels.end()) order.push_back(label);
    std::stable_sort(order.begin(), order.end(), [](const std::string& x, const std::string& y) { return driver_entry_before(x, y); });
    jobs::RunOptions<std::string, store::Judgement> options;
    options.crashed = [](const std::string&) { return store::Judgement{store::Verdict::Inconclusive, "judge: the emulator crashed on this entry point", {}, {}}; };
    const auto got = jobs::run(
        order, [&](const std::string& label) { return judge_entry(original, rebuilt, label); }, [](const std::string& label) { return label; }, options);
    std::vector<std::pair<std::string, store::Judgement>> out;
    for (const std::string& label : order) out.emplace_back(label, got.at(label));
    return out;
}

// ---- the judge judged ------------------------------------------------------------------------------------------------------------------

std::optional<DriverBreak> DriverLane::break_instruction(const stdfs::path& original, const stdfs::path& out, std::string_view label_view, int nth) const {
    const std::string label(label_view);
    const auto modules = formats::le::open_modules(original);
    if (modules.empty()) return std::nullopt;
    const formats::le::Module& lf = modules.front().second;
    const std::shared_ptr<const edrv::Image> img = image(original);
    const auto entry = img->entries.find(label);
    if (entry == nullptr || lf.is_member()) return std::nullopt;
    // what runs on the first inputs
    std::set<std::uint64_t> cover;
    emulate::native_trace::Governor governor;
    edrv::Environment env;
    env.governor = &governor;
    env.resolver = spec_.resolver;
    for (const DriverVector& v : inputs(label).first) {
        governor = {};
        const emulate::Run r = edrv::recorded(env, *img, *entry, drv_options(v));
        cover.insert(r.path.begin(), r.path.end());
    }
    // the places the loader patches: the four bytes of each fix-up
    std::set<std::int64_t> patched;
    for (const formats::le::Reference& ref : lf.references()) {
        for (const edrv::Span& s : img->spans)
            if (s.object == ref.at.object)
                for (std::int64_t i = 0; i < 4; ++i) patched.insert(static_cast<std::int64_t>(s.low) + ref.at.offset + i);
    }
    platforms::DecoderSet decoders;
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 32);
    struct Candidate {
        std::uint32_t object = 0;
        std::uint64_t offset = 0;
        platforms::Insn insn;
    };
    std::vector<Candidate> picked;
    for (const std::uint64_t address : cover) {
        const auto span = std::find_if(img->spans.begin(), img->spans.end(), [&](const edrv::Span& s) { return s.low <= address && address < s.high; });
        if (span == img->spans.end()) continue;
        const auto region = std::find_if(img->regions.begin(), img->regions.end(), [&](const edrv::Region& r) { return r.address == span->low; });
        if (region == img->regions.end()) continue;
        const std::size_t at = static_cast<std::size_t>(address - span->low);
        if (at >= region->bytes.size()) continue;
        const ByteView code = ByteView(region->bytes).subspan(at, std::min<std::size_t>(15, region->bytes.size() - at));
        const std::vector<platforms::Insn> decoded = decoder.disasm(code, address, 1);
        if (decoded.empty()) continue;
        const platforms::Insn& ins = decoded.front();
        if (ins.mnemonic == "nop" || ins.mnemonic == "int3" || patched.contains(static_cast<std::int64_t>(ins.address + ins.size - 1))) continue;
        picked.push_back({span->object, address - span->low, ins});
    }
    if (nth < 0 || static_cast<std::size_t>(nth) >= picked.size()) return std::nullopt;
    const Candidate& c = picked[static_cast<std::size_t>(nth)];
    const std::optional<std::uint64_t> at = lf.file_offset(c.object, static_cast<std::int64_t>(c.offset) + c.insn.size - 1);
    if (!at || *at >= lf.data().size()) return std::nullopt;
    Bytes data(lf.data().begin(), lf.data().end());
    data[*at] ^= c.insn.size > 1 ? 0x01 : 0x08;   // a register, a displacement or an immediate becomes another
    fs::write_file(out, data);
    return DriverBreak{*at, std::string(text::trim(c.insn.mnemonic + " " + c.insn.op_str))};
}

DriverSelfCheck DriverLane::self_check(const stdfs::path& original, const std::optional<stdfs::path>& work_in, int per_entry, int entries) const {
    const stdfs::path work = work_in ? *work_in : spec_.state / "driver_judge" / text::lower(fs::to_text(original.stem()));
    fs::make_dirs(work);
    const auto own = judge_files(original, original);
    DriverSelfCheck out;
    for (const auto& [label, judgement] : own) {
        const auto at = std::find_if(out.self.begin(), out.self.end(), [&](const auto& kv) { return kv.first == judgement.verdict; });
        if (at == out.self.end()) out.self.emplace_back(judgement.verdict, 1);
        else ++at->second;
    }
    std::vector<std::string> good;
    for (const auto& [label, judgement] : own)
        if (judgement.verdict == store::Verdict::Equivalent && static_cast<int>(good.size()) < entries) good.push_back(label);
    const std::string ext = fs::to_text(original.extension());
    // (a round each: one entry point, one break)
    std::size_t label_at = 0;
    int nth = 0;
    const std::function<std::optional<teeth::Round>()> next_round = [&]() -> std::optional<teeth::Round> {
        while (label_at < good.size()) {
            if (nth >= per_entry) {
                ++label_at;
                nth = 0;
                continue;
            }
            const std::string& label = good[label_at];
            const stdfs::path copy = work / std::format("broken_{}_{}{}", label, nth, ext);
            const auto changed = break_instruction(original, copy, label, nth);
            if (!changed) {   // no more to break in this one
                ++label_at;
                nth = 0;
                continue;
            }
            ++nth;
            forget_image(copy);
            return teeth::Round{{label, changed->instruction, fs::to_text(copy)}};
        }
        return std::nullopt;
    };
    const std::function<std::vector<store::Verdict>(const teeth::Round&)> judge = [&](const teeth::Round& round) {
        std::vector<store::Verdict> said;
        for (const teeth::Attempt& a : round) said.push_back(judge_entry(original, fs::from_text(a.made), a.name).verdict);
        return said;
    };
    const std::function<teeth::Outcome(const store::Verdict&)> outcome = [](const store::Verdict& v) {
        return v == store::Verdict::Divergent ? teeth::Outcome::Caught : teeth::Outcome::Missed;
    };
    const std::function<void(std::span<const teeth::Planted<store::Verdict>>)> each = [&](std::span<const teeth::Planted<store::Verdict>> planted) {
        for (const teeth::Planted<store::Verdict>& p : planted) {
            ++out.broken;
            if (p.outcome == teeth::Outcome::Caught) ++out.caught;
            else out.missed.push_back({p.attempt->name, p.attempt->what, p.said});
        }
    };
    teeth::plant<store::Verdict>(next_round, judge, outcome, each);
    return out;
}

// ---- the command -------------------------------------------------------------------------------------------------------------------------

cli::Command DriverLane::command() const {
    return cli::Command{
        "driver-judge",
        "judge a rebuilt virtual device driver against the original, entry point by entry point",
        {cli::Option{"--self-check", "", "judge the judge: the original against itself, then against copies with one instruction broken", cli::Arg::Flag},
         cli::Positional{"first", "the rebuilt driver (with --self-check: the original, a file or a corpus name)", true, false},
         cli::Positional{"original", "the original driver (a file or a corpus name)", false, false}},
        [this](const cli::Args& args) {
            const std::string first = args.required("first");
            const auto find = [&](const std::string& what) -> std::optional<stdfs::path> {
                if (spec_.retail) return spec_.retail(what);
                std::error_code ec;
                return stdfs::is_regular_file(fs::from_text(what), ec) ? std::optional<stdfs::path>(fs::from_text(what)) : std::nullopt;
            };
            if (args.flag("self-check")) {
                if (args.value("original")) throw UsageError("driver-judge --self-check takes one driver");
                const auto path = find(first);
                if (!path) {
                    out("{}: not found\n", first);
                    return 3;
                }
                const DriverSelfCheck r = self_check(*path);
                std::vector<std::string> counts;
                bool bad = false;
                std::vector<std::pair<std::string, std::size_t>> words;
                for (const auto& [verdict, n] : r.self) {
                    words.emplace_back(std::string(word(verdict)), n);
                    bad |= verdict == store::Verdict::Divergent || verdict == store::Verdict::Inconclusive;
                }
                std::sort(words.begin(), words.end());
                for (const auto& [w, n] : words) counts.push_back(std::format("{} {}", w, n));
                out("{} against itself: {}\n", fs::to_text(path->filename()), text::join(counts, ", "));
                out("one instruction broken: {} copies, {} reported divergent\n", r.broken, r.caught);
                for (const auto& m : r.missed) out("    not caught: {} with `{}` changed -> {}\n", m.label, m.instruction, word(m.verdict));
                return bad ? 1 : 0;
            }
            const auto second = args.value("original");
            if (!second) throw UsageError("driver-judge takes the rebuilt driver and the original");
            const stdfs::path rebuilt = fs::from_text(first);
            std::error_code ec;
            const auto original = find(*second);
            if (!stdfs::is_regular_file(rebuilt, ec) || !original) {
                out("{}: not found\n", stdfs::is_regular_file(rebuilt, ec) ? *second : first);
                return 3;
            }
            std::map<std::string, std::size_t> counts;
            for (const auto& [label, j] : judge_files(rebuilt, *original)) {
                ++counts[std::string(word(j.verdict))];
                out("{:<12} {:<12} {}{}{}\n", label, word(j.verdict), j.reason, j.limits.empty() ? "" : "  [" + text::join(j.limits, ",") + "]",
                    j.detail.empty() ? "" : "  " + j.detail);
            }
            std::vector<std::string> parts;
            for (const auto& [w, n] : counts) parts.push_back(std::format("{} {}", w, n));
            out("{} vs {}: {}\n", fs::to_text(rebuilt.filename()), fs::to_text(original->filename()), parts.empty() ? "no entry points" : text::join(parts, ", "));
            return counts.contains("divergent") ? 1 : 0;
        },
        {},
        "Verify",
        ""};
}

}  // namespace cb::verify::lanes
