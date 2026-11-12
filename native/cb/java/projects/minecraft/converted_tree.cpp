#include "cb/java/projects/minecraft/converted_tree.hpp"

#include <algorithm>
#include <system_error>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/console/crlf.hpp"

namespace cb::java::projects::minecraft::converted_tree {

std::vector<std::string> cpp_files(const std::filesystem::path& dir) {
    std::vector<std::string> names;
    std::error_code ec;
    for (std::filesystem::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::filesystem::path& p = it->path();
        if (p.extension() == ".cpp") names.push_back(fs::to_text(p.filename()));
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::optional<CppText> read_cpp(const std::filesystem::path& file) {
    std::string raw;
    try {
        raw = fs::read_text(file);
    } catch (const IoError&) {   // an isolation boundary: one unreadable file of a tree is skipped
        return std::nullopt;
    }
    CppText out;
    out.crlf = raw.find("\r\n") != std::string::npos;
    if (!out.crlf) {
        out.text = std::move(raw);
        return out;
    }
    out.text.reserve(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n') continue;
        out.text += raw[i];
    }
    return out;
}

void write_cpp(const std::filesystem::path& file, const std::string& text, bool crlf) {
    fs::write_text(file, crlf ? java::console::to_crlf(text) : text);
}

}  // namespace cb::java::projects::minecraft::converted_tree
