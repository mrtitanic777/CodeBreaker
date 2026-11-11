#include "cb/formats/xbox/crypto/format_key.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::formats::xbox::crypto::format_key {

std::string byte_array_source(ByteView data, std::string_view variable, std::size_t per_line) {
    if (data.empty()) throw FormatError("format_key", 0, "nothing to embed: the key file is empty");
    per_line = std::max<std::size_t>(per_line, 1);
    std::string out = std::format(
        "#ifndef BYTE\n"
        "typedef unsigned char BYTE;\n"
        "#endif\n"
        "\n"
        "#ifndef __{0}__\n"
        "#define __{0}__\n"
        "BYTE {0}[] = {{\n",
        variable);
    for (std::size_t at = 0; at < data.size(); at += per_line) {
        const std::size_t end = std::min(data.size(), at + per_line);
        out += '\t';
        for (std::size_t i = at; i < end; ++i) out += std::format("{}0x{:02X}", i == at ? "" : ", ", data[i]);
        if (end < data.size()) out += ',';   // the last line has no trailing comma
        out += '\n';
    }
    out += "};\n#endif\n";
    return out;
}

std::string write_exp_pub_key(const std::filesystem::path& base) {
    const Bytes key = fs::read_file(base / kKeyFile);
    const std::string source = byte_array_source(key);
    fs::write_text(base / kHeaderFile, source);
    return source;
}

}  // namespace cb::formats::xbox::crypto::format_key
