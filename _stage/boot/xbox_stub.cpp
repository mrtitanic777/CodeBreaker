// xbox_stub.cpp -- what formats::media asks of the Xbox containers (formats/xbox/volumes.cpp): no Xbox volume here. The harness reads FAT disk images;
// the typed volumes.cpp (and the STFS / FATX / GoD ports it pulls in) is not linked: it does not build against this tree's runtime
// (it calls py::datetime::datetime::strftime_minute, which the runtime has not).
#include <optional>

#include "codebreaker/formats/media.hpp"

namespace codebreaker::formats::xbox::volumes {

std::optional<py::list<std::shared_ptr<codebreaker::formats::media::Volume>>> volumes(const py::str& path, bool long_names);

std::optional<py::list<std::shared_ptr<codebreaker::formats::media::Volume>>> volumes(const py::str& path, bool long_names) {
    (void)path;
    (void)long_names;
    return std::nullopt;
}

}  // namespace codebreaker::formats::xbox::volumes
