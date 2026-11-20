#include "formats/format_builtin.hpp"

#include <cstddef>

namespace cb::formats::engine {

// Written by CMake (embed_formats.cmake) into the build folder: one entry per config/formats/*.fmt.
struct EmbeddedFormat {
    const char* file;
    const char* text;
};
extern const EmbeddedFormat kEmbeddedFormats[];
extern const std::size_t kEmbeddedFormatCount;

const Registry& builtin() {
    static const Registry registry = [] {
        Registry r;
        for (std::size_t i = 0; i < kEmbeddedFormatCount; ++i) r.add_text(kEmbeddedFormats[i].text, kEmbeddedFormats[i].file);
        return r;
    }();
    return registry;
}

Tree read_builtin(std::string_view format, ByteView data) { return builtin().read(format, data); }

}  // namespace cb::formats::engine
