// codebreaker/verify/gate16/contract16.hpp -- COMPAT SHIM over cb/verify/gate16/contract16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/gate16/contract16.{hpp,cpp} now (the calls of each function rebuilt from C beside the
// original's: per callee, the bytes the caller takes off the stack after the call). This header keeps the old py:: API:
//   * `_calls` over a py::bytes image, with callee keys as the old CalleeKey record and the rows as tuples;
//   * `mismatches(original, folder, image, names)`, which describes the two modules with the old describe port (the
//     describer is the one input the new module is given, not one it makes) and hands them over.
// No logic of contract16's own. The old describe port is the COMPAT(py) dependency: it goes when decompile/describe is
// refactored and `mismatches` is called with a cb::Describer.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/verify/gate16/contract16.hpp"
#include "codebreaker/decompile/describe.hpp"

namespace codebreaker::verify::gate16::contract16 {

// ("near", seg, off) / ("far", address) of a direct call
struct CalleeKey {
    py::str kind;       // "near" | "far"
    int64_t a = 0;      // far: the address; near: the segment
    int64_t b = 0;      // near: the offset
};
using TakenOff = std::variant<int64_t, py::str>;
// (callee, bytes taken off after it, argument bytes pushed for it -- negative: at least that, None: not known --
// whether the frame's LEAVE follows)
using CallRow = std::tuple<py::str, TakenOff, std::optional<int64_t>, bool>;

namespace detail {
namespace nv = cb::verify::gate16::contract16;

inline nv::CalleeKey key_of(const CalleeKey& k) {
    if (k.kind == py::str("far")) return nv::FarCall{static_cast<std::uint32_t>(k.a)};
    return nv::NearCall{static_cast<unsigned>(k.a), static_cast<std::uint16_t>(k.b)};
}
inline CalleeKey old_key(const nv::CalleeKey& k) {
    if (const auto* far = std::get_if<nv::FarCall>(&k)) return CalleeKey{py::str("far"), static_cast<int64_t>(far->address), 0};
    const auto& n = std::get<nv::NearCall>(k);
    return CalleeKey{py::str("near"), static_cast<int64_t>(n.segment), static_cast<int64_t>(n.offset)};
}
}  // namespace detail

// The calls of the code in [lo, hi) of segment `seg` in `img`. name_of(key): the callee's name or None; removes(key):
// the bytes a callee removes itself (its `retf n`), or None.   (used by tests)
inline py::list<CallRow> _calls(const py::bytes& img, int64_t seg, int64_t lo, int64_t hi,
                                const std::function<std::optional<py::str>(const CalleeKey&)>& name_of,
                                const std::function<std::optional<int64_t>(const CalleeKey&)>& removes =
                                    [](const CalleeKey&) { return std::nullopt; }) {
    return cb::compat::guard([&] {
        thread_local cb::platforms::DecoderSet decoders;
        const cb::Bytes image = cb::compat::from_py(img);
        const auto names = [&](const detail::nv::CalleeKey& k) -> std::optional<std::string> {
            const std::optional<py::str> n = name_of(detail::old_key(k));
            return n ? std::optional<std::string>(n->utf8()) : std::nullopt;
        };
        const auto rem = [&](const detail::nv::CalleeKey& k) { return removes(detail::old_key(k)); };
        py::list<CallRow> out;
        for (const detail::nv::Call& c : detail::nv::calls(decoders.get(cb::platforms::Arch::X86, 16, true), image,
                                                          static_cast<unsigned>(seg), static_cast<std::size_t>(std::max<int64_t>(lo, 0)),
                                                          static_cast<std::size_t>(std::max<int64_t>(hi, 0)), names, rem)) {
            TakenOff taken = c.taken_off.kind == detail::nv::TakenOff::Kind::Bytes ? TakenOff(c.taken_off.bytes)
                                                                                   : TakenOff(cb::compat::to_py(c.taken_off.text()));
            out.append(CallRow{cb::compat::to_py(c.callee), std::move(taken), c.pushed, c.at_leave});
        }
        return out;
    });
}

// {name: why} for each named function whose calls take off different bytes than the original's calls to the same
// callee (map file m.map in `folder`, and the image of the build).  `original`, `image` are paths.
inline py::dict<py::str, py::str> mismatches(const py::str& original, const py::str& folder, const py::str& image,
                                             const py::list<py::str>& names) {
    return cb::compat::guard([&] {
        // COMPAT(py): remove when decompile/describe is refactored -- the old describer, through cb::Describer
        namespace describe = codebreaker::decompile::describe;
        const detail::nv::Describer describer = [](const std::filesystem::path& module) {
            cb::fs::ScopedTempDir where("contract16");
            const auto d = describe::describe16(cb::compat::to_py(module.generic_string()), cb::compat::to_py(where.path().generic_string()));
            detail::nv::Described out;
            for (const auto& [seg, bytes] : d->images.items()) out.images[static_cast<unsigned>(seg)] = cb::compat::from_py(bytes);
            for (const auto& [va, name] : d->names.items()) out.names[static_cast<std::uint32_t>(va)] = name.utf8();
            out.description = cb::fs::read_text(where.path() / "description.txt");
            out.returns = [](cb::ByteView code, std::uint32_t offset, bool far) -> std::optional<int64_t> {
                const auto r = describe::returns(cb::compat::to_py(code), offset, 600, far ? std::optional<py::str>(py::str("far")) : std::nullopt);
                return std::get<1>(r) >= 0 ? std::optional<int64_t>(std::get<1>(r)) : std::nullopt;
            };
            return out;
        };
        const std::vector<std::string> wanted = cb::compat::from_py(names);
        py::dict<py::str, py::str> out;
        for (const auto& [name, why] : detail::nv::mismatches(describer, original.utf8(), folder.utf8(), image.utf8(), wanted))
            out.set(cb::compat::to_py(name), cb::compat::to_py(why));
        return out;
    });
}

}  // namespace codebreaker::verify::gate16::contract16
