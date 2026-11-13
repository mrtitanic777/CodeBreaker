// cb-unity: alone
// cb/verify/lanes/win32_inputs.cpp -- the 32-bit lane's inputs: the fixed vectors, how a member function is given its arguments, the
// conversions to and from the guided search's inputs, and the values a symbolic exploration sets (see win32.hpp).

#include <algorithm>
#include <cstring>

#include "cb/verify/lanes/win32_internal.hpp"

namespace cb::verify::lanes {
namespace {

using Vector = Win32Lane::Vector;

constexpr std::uint32_t kLow32 = 0xFFFFFFFFu;

std::int64_t signed_of(std::uint64_t v) { return static_cast<std::int64_t>(v); }

// The little-endian bytes of dwords, one after the other.
Bytes dwords_le(std::size_t count, const auto& value_of) {
    Bytes out;
    out.reserve(count * 4);
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t v = value_of(i);
        for (int b = 0; b < 4; ++b) out.push_back(static_cast<std::uint8_t>(v >> (8 * b)));
    }
    return out;
}

Vector make_vector(const std::vector<std::uint64_t>& args, std::optional<std::vector<guided::MemInit>> memory = std::nullopt,
                   std::optional<std::uint64_t> stub_ret = std::nullopt) {
    Vector v;
    for (const std::uint64_t a : args) v.args.push_back(signed_of(a));
    v.mem_init = std::move(memory);
    if (stub_ret) v.stub_ret = signed_of(*stub_ret);
    return v;
}

}  // namespace

Vector Win32Lane::in_ecx(const Vector& v) {
    if (v.ecx) return v;
    Vector w = v;
    w.ecx = static_cast<std::uint32_t>(v.args.at(0));
    w.args.assign(v.args.begin() + 1, v.args.end());
    w.args.push_back(0);
    w.given = v.args;
    return w;
}

Vector Win32Lane::in_ecx_edx(const Vector& v) {
    if (v.ecx) return v;
    Vector w = v;
    w.ecx = static_cast<std::uint32_t>(v.args.at(0));
    w.edx = static_cast<std::uint32_t>(v.args.at(1));
    w.args.assign(v.args.begin() + 2, v.args.end());
    w.args.push_back(0);
    w.args.push_back(0);
    w.given = v.args;
    return w;
}

std::vector<guided::MemInit> Win32Lane::provisioned_memory() {
    return {guided::MemInit{kObject, dwords_le(0x400, [](std::size_t i) { return kSafe + 4 * static_cast<std::uint32_t>(i); })},
            guided::MemInit{kSafe, Bytes(0x1000, 0xC3)}};
}

std::vector<Vector> Win32Lane::vectors() {
    const std::size_t n = kArity;
    std::vector<std::uint64_t> object;   // provisioned objects, one per argument
    for (std::size_t i = 0; i < n; ++i) object.push_back(kObject + 0x40 * (i % 32));
    // Every argument gets a DIFFERENT value wherever the environment allows it: with one value in every slot, a body that uses its
    // parameters in the wrong order behaves identically.
    std::vector<Vector> out;
    out.push_back(make_vector(std::vector<std::uint64_t>(n, 0)));
    out.push_back(make_vector(lane_common::counting(n, 0x200000, 0x100)));   // bare scratch pointers
    out.push_back(make_vector(object, provisioned_memory()));                // provisioned objects
    out.push_back(make_vector(lane_common::counting(n)));
    return out;
}

std::vector<Vector> Win32Lane::confirm_vectors() {
    // Four inputs are few; these add boundary integers, a reversed ordering and mixes of pointers and integers, so a body is not called
    // verified on the strength of four lucky inputs.
    const std::size_t n = kArity;
    const std::vector<guided::MemInit> provisioned = provisioned_memory();
    std::vector<std::uint64_t> object;
    for (std::size_t i = 0; i < n; ++i) object.push_back(kObject + 0x40 * (i % 32));
    static constexpr std::uint64_t edge[] = {0x7FFFFFFF, 0x80000000, 0x10000, 0xFFFF, 0xFF, 0x100, 0x8000, 0x7FFF};
    std::vector<std::uint64_t> edges;
    for (std::size_t i = 0; i < n; ++i) edges.push_back(edge[i % std::size(edge)]);
    const std::vector<std::uint64_t> ints = lane_common::counting(n);
    // Objects whose fields are not pointers: all-ones (every flag bit set, counts large) and small counts. A field tested with `& 0x40` or
    // compared with an index is false on the pointer-filled object, so everything behind such a test never ran.
    const std::vector<guided::MemInit> ones = {{kObject, Bytes(0x1000, 0xFF)}, {kSafe, Bytes(16, 0xC3)}};
    const std::vector<guided::MemInit> small = {{kObject, dwords_le(0x400, [](std::size_t) { return std::uint32_t{3}; })}, {kSafe, Bytes(16, 0xC3)}};
    // Objects whose words are zero and non-zero by turns, both ways round. On an object that is the same all through, a field read one byte
    // off (`p + 0x3d` for `p + 0x3c`) reads what the right one holds, and `if (p->x == 0)` goes the same way whichever field it tests.
    std::vector<std::vector<guided::MemInit>> striped;
    for (const std::size_t phase : {std::size_t{0}, std::size_t{1}}) {
        striped.push_back({{kObject, dwords_le(0x400, [phase](std::size_t i) { return i % 2 == phase ? std::uint32_t{0} : kSafe + 4 * static_cast<std::uint32_t>(i); })},
                           {kSafe, Bytes(0x1000, 0xC3)}});
    }
    std::vector<Vector> out;
    // Imported calls return 0 by default, so the code after `if (!CreateThing()) return` never ran. Run again with imports succeeding:
    // returning 1, and returning a valid object.
    out.push_back(make_vector(object, provisioned, 1));
    out.push_back(make_vector(object, provisioned, kObject + 0x800));
    out.push_back(make_vector(ints, std::nullopt, 1));
    out.push_back(make_vector(object, ones, 1));
    out.push_back(make_vector(object, small, 1));
    out.push_back(make_vector(object, small));
    out.push_back(make_vector(object, striped[0], 1));
    out.push_back(make_vector(object, striped[1], 1));
    out.push_back(make_vector(lane_common::descending(n, 0xFFFFFFFF)));   // small negatives
    out.push_back(make_vector(edges));                                    // boundary integers
    out.push_back(make_vector(lane_common::descending(n, n)));            // distinct, reversed
    std::vector<std::uint64_t> pointer_then_ints{object[0]};
    for (std::size_t i = 1; i < n; ++i) pointer_then_ints.push_back(i + 2);
    out.push_back(make_vector(pointer_then_ints, provisioned));           // pointer, then integers
    std::vector<std::uint64_t> int_then_pointers{5};
    int_then_pointers.insert(int_then_pointers.end(), object.begin() + 1, object.end());
    out.push_back(make_vector(int_then_pointers, provisioned));           // integer, then pointers
    out.push_back(make_vector(lane_common::alternating(n, lane_common::Pattern{[](std::size_t i) -> std::uint64_t { return 0x100 + i; }},
                                                       lane_common::Pattern{[&object](std::size_t i) -> std::uint64_t { return object[i]; }}),
                              provisioned));
    return out;
}

// ---- to and from the guided search's inputs --------------------------------------------------------------------------------------

guided::Input Win32Lane::to_guided(const Vector& v) {
    guided::Input g;
    for (const std::int64_t a : v.args) g.args.push_back(static_cast<std::uint64_t>(a));
    g.mem_init = v.mem_init;
    if (v.stub_ret) g.stub_ret = static_cast<std::uint64_t>(*v.stub_ret);
    if (v.ecx) g.extra["ecx"] = static_cast<std::int64_t>(*v.ecx);
    if (v.edx) g.extra["edx"] = static_cast<std::int64_t>(*v.edx);
    if (v.given) g.extra["given"] = *v.given;
    if (v.room) g.extra["room"] = *v.room;
    if (v.sentinels) g.extra["sentinels"] = std::vector<std::int64_t>(v.sentinels->begin(), v.sentinels->end());
    return g;
}

Vector Win32Lane::from_guided(const guided::Input& g) {
    Vector v;
    for (const std::uint64_t a : g.args) v.args.push_back(signed_of(a));
    v.mem_init = g.mem_init;
    if (g.stub_ret) v.stub_ret = signed_of(*g.stub_ret);
    const auto number = [&](const char* key) -> std::optional<std::int64_t> {
        const auto it = g.extra.find(key);
        return it != g.extra.end() && std::holds_alternative<std::int64_t>(it->second) ? std::optional<std::int64_t>(std::get<std::int64_t>(it->second)) : std::nullopt;
    };
    const auto list = [&](const char* key) -> std::optional<std::vector<std::int64_t>> {
        const auto it = g.extra.find(key);
        return it != g.extra.end() && std::holds_alternative<std::vector<std::int64_t>>(it->second)
                   ? std::optional<std::vector<std::int64_t>>(std::get<std::vector<std::int64_t>>(it->second))
                   : std::nullopt;
    };
    if (const auto ecx = number("ecx")) v.ecx = static_cast<std::uint32_t>(*ecx);
    if (const auto edx = number("edx")) v.edx = static_cast<std::uint32_t>(*edx);
    v.given = list("given");
    v.room = number("room");
    if (const auto s = list("sentinels"); s && s->size() == 4) {
        v.sentinels = std::array<std::uint32_t, 4>{static_cast<std::uint32_t>((*s)[0]), static_cast<std::uint32_t>((*s)[1]),
                                                   static_cast<std::uint32_t>((*s)[2]), static_cast<std::uint32_t>((*s)[3])};
    }
    return v;
}

// ---- what a symbolic exploration sets --------------------------------------------------------------------------------------------

std::optional<Vector> Win32Lane::derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names, const Shape& shape) {
    Vector w = shape ? shape(v) : v;
    std::map<std::uint64_t, std::uint8_t> memory;   // bytes of memory the values set, by address
    bool changed = false;
    for (const auto& [name, raw_value] : values) {
        const auto found = names.find(name);
        if (found == names.end()) continue;
        const concolic::InputVar& var = found->second;
        const std::uint64_t value = raw_value;
        switch (var.place) {
        case concolic::InputVar::Place::Arg:
            if (var.where >= 0 && static_cast<std::size_t>(var.where) < w.args.size()) {
                const std::size_t at = static_cast<std::size_t>(var.where);
                changed |= w.args[at] != static_cast<std::int64_t>(value & kLow32);
                w.args[at] = static_cast<std::int64_t>(value & kLow32);
            }
            break;
        case concolic::InputVar::Place::Reg: {
            std::optional<std::uint32_t>* reg = var.where == 1 ? &w.ecx : var.where == 2 ? &w.edx : nullptr;
            if (reg != nullptr && *reg) {
                changed |= **reg != static_cast<std::uint32_t>(value & kLow32);
                *reg = static_cast<std::uint32_t>(value & kLow32);
            }
            break;
        }
        case concolic::InputVar::Place::Ret:
            changed |= w.stub_ret.value_or(0) != static_cast<std::int64_t>(value & kLow32);
            w.stub_ret = static_cast<std::int64_t>(value & kLow32);
            break;
        case concolic::InputVar::Place::Mem:
            changed |= static_cast<std::uint64_t>(var.value) != (value & 0xFF);
            memory[static_cast<std::uint64_t>(var.where)] = static_cast<std::uint8_t>(value & 0xFF);
            break;
        }
    }
    if (!changed) return std::nullopt;
    if (w.given) {   // (what the calls' arguments are named by)
        std::vector<std::int64_t> given;
        if (w.ecx) given.push_back(*w.ecx);
        if (w.edx) given.push_back(*w.edx);
        given.insert(given.end(), w.args.begin(), w.args.end());
        given.resize(std::min(given.size(), w.given->size()));
        w.given = std::move(given);
    }
    if (!memory.empty()) {   // runs of consecutive bytes, put after the input's own so they are what the run finds there
        std::vector<guided::MemInit> added = w.mem_init.value_or(std::vector<guided::MemInit>());
        for (const auto& [at, byte] : memory) {
            if (!added.empty() && added.size() > (w.mem_init ? w.mem_init->size() : 0) &&
                added.back().address + added.back().data.size() == at) {
                added.back().data.push_back(byte);
            } else {
                added.push_back(guided::MemInit{at, Bytes{byte}});
            }
        }
        w.mem_init = std::move(added);
    }
    return w;
}

// ---- the return value's leftovers -----------------------------------------------------------------------------------------------

std::uint32_t Win32Lane::undefined_ret_bits(const RunOriginal& original, const Vector& v, const compare::Fingerprint& a) {
    if (a.exit != "ret" || !a.ret) return 0;
    // the saved registers hold the opposite bits: every bit of the return value that flips is a leftover
    Vector other = v;
    std::array<std::uint32_t, 4> alt{};
    for (std::size_t i = 0; i < 4; ++i) alt[i] = emulate::emulator::kSavedSentinels[i] ^ 0xFFFFFFFFu;
    other.sentinels = alt;
    const compare::Fingerprint c = original(other);
    if (c.err || c.exit != "ret" || !c.ret || c.deps != a.deps) return 0;
    const std::uint32_t flips = static_cast<std::uint32_t>((*a.ret ^ *c.ret) & kLow32);
    // Only whole upper parts count (the low byte must stay put), so a real result is never excused.
    for (const std::uint32_t mask : {0xFFFF0000u, 0xFFFFFF00u}) {
        if (flips != 0 && (flips & ~mask) == 0) return mask;
    }
    // the whole upper half follows the caller's registers and the low half moves too: the function returns nothing, and EAX is simply what
    // its last instructions left there
    if ((flips & 0xFFFF0000u) == 0xFFFF0000u && (flips & 0xFFFFu) != 0) return 0xFFFFFFFFu;
    return 0;
}

// ---- names of imports -----------------------------------------------------------------------------------------------------------

std::string Win32Lane::normalise_import(std::string_view name) {
    std::string s(name);
    for (char& c : s) {
        c = c >= 'a' && c <= 'z' ? static_cast<char>(c - 'a' + 'A') : c;
        if (c == '!' || c == ':') c = '.';
    }
    std::string out;
    std::size_t start = 0;
    while (start <= s.size()) {
        std::size_t end = s.find('.', start);
        if (end == std::string::npos) end = s.size();
        const std::string part = s.substr(start, end - start);
        static const std::set<std::string> kExtensions = {"DLL", "EXE", "DRV", "SYS", "VXD", "OCX", "CPL"};   // (NDIS.SYS is NDIS)
        if (!kExtensions.contains(part)) {
            if (!out.empty()) out += '.';
            out += part;
        }
        if (end == s.size()) break;
        start = end + 1;
    }
    return out;
}

bool Win32Lane::is_noise(std::string_view api_in) {
    std::string api(api_in);
    for (char& c : api) c = c >= 'a' && c <= 'z' ? static_cast<char>(c - 'a' + 'A') : c;
    static const std::set<std::string, std::less<>> kToolchain = {"KERNEL.GETVERSION", "KERNEL.GETWINFLAGS", "KERNEL.LOCALINIT", "KERNEL.GETVERSION16",
                                                                  "KERNEL32.GETVERSION", "KERNEL32.GETVERSIONEXA", "KERNEL32.GETCURRENTTHREADID"};
    if (kToolchain.contains(api)) return true;
    const auto has = [&](std::string_view part) { return api.find(part) != std::string::npos; };
    return has("SMAPLS") || has("SUNMAPLS") || has("FT_") || has("QT_") || has("THUNK") || api.ends_with(".@0") || has("GETVERSION") || has("GETWINFLAGS");
}

}  // namespace cb::verify::lanes
