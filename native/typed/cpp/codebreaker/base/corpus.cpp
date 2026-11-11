// codebreaker/base/corpus.cpp -- the shim's translation unit (see corpus.hpp): the process-wide corpus of the old layer's
// settings, wired to the old formats ports for what a file IS. COMPAT(py): deleted with corpus.hpp; each probe goes when the
// formats module it calls is refactored (formats/detect, formats/ne, formats/xex, the .xbe reader).

#include "codebreaker/base/corpus.hpp"

#include <memory>
#include <mutex>
#include <vector>

#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/xex.hpp"

namespace codebreaker::base::corpus {

namespace detect = ::codebreaker::formats::detect;
namespace ne = ::codebreaker::formats::ne;
namespace xex = ::codebreaker::formats::xex;
namespace newc = ::cb::base;

std::function<py::str(const py::str&)> xbe_to_pe;

namespace {

newc::FormatProbes old_probes() {
    newc::FormatProbes probes;
    probes.lane = [](const std::filesystem::path& p) -> std::optional<newc::Lane> {
        const std::optional<py::str> l = detect::lane(::cb::compat::legacy::native(p));
        if (!l) return std::nullopt;
        return newc::lane_from_name(l->utf8());
    };
    // platforms.named("powerpc").readable: a .xex is handed over as the PowerPC image inside it; platforms.named("xbe").readable:
    // an .xbe as the PE image made of it
    probes.readable = [](const std::filesystem::path& p) -> std::filesystem::path {
        py::str path = ::cb::compat::legacy::native(p);
        if (xex::is_xex(path)) path = xex::basefile(path);
        if (py::open_rb(path).read(4) == py::bytes("XBEH")) {
            if (!xbe_to_pe) throw py::RuntimeError("corpus::xbe_to_pe is not set: formats.xbox.xbe has no C++ port yet");
            path = xbe_to_pe(path);
        }
        return ::cb::compat::legacy::path_of(path);
    };
    probes.boot_image_modules = [](const std::filesystem::path& image, const std::filesystem::path& overlay) {
        std::vector<newc::BootModule> out;
        try {
            const auto modules = ne::open_boot_image(::cb::compat::legacy::native(image), ::cb::compat::legacy::native(overlay));
            for (const auto& entry : modules.items())
                out.push_back({::cb::compat::from_py(std::get<0>(entry)), ::cb::compat::from_py(ne::standalone(std::get<1>(entry)))});
        } catch (const py::OSError& e) {
            throw ::cb::IoError(image, 0, e.what());
        } catch (const py::ValueError& e) {
            throw ::cb::FormatError("ne boot image", 0, e.what());
        }
        return out;
    };
    return probes;
}

}  // namespace

namespace detail {

::cb::base::Corpus& instance() {
    static std::mutex m;
    static std::vector<std::unique_ptr<newc::Corpus>> built;   // (kept: a reference handed out stays good)
    static newc::Products from;
    std::lock_guard<std::mutex> hold(m);
    const newc::Products now = ::cb::compat::legacy::corpus_products();
    if (built.empty() || !(from == now)) {
        built.push_back(std::make_unique<newc::Corpus>(now, old_probes()));
        from = now;
    }
    return *built.back();
}

}  // namespace detail

}  // namespace codebreaker::base::corpus
