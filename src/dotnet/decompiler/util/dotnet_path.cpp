#include "dotnet/decompiler/util/dotnet_path.hpp"

#include "core/fs.hpp"

namespace cb::dotnet::decompiler::util::path {

namespace {

bool is_sep(char c) {
#ifdef _WIN32
    return c == '\\' || c == '/';
#else
    return c == '/';
#endif
}

bool is_ascii_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

// PathInternal.GetRootLength (Windows): "C:", "C:\", "\", "\\server\share", "\\?\C:\".
std::size_t root_length(std::string_view path) {
#ifdef _WIN32
    const std::size_t n = path.size();
    std::size_t i = 0;
    const bool device = n >= 4 && is_sep(path[0]) && is_sep(path[1]) && (path[2] == '.' || path[2] == '?') && is_sep(path[3]);
    const bool unc = n >= 2 && is_sep(path[0]) && is_sep(path[1]) && !device;
    if (device) {
        i = 4;
        if (n >= 8 && (path[2] == '?') && (path.substr(4, 3) == "UNC" || path.substr(4, 3) == "unc") && is_sep(path[7])) {
            i = 8;
            // a UNC device path: server and share follow
            int segments = 2;
            while (i < n && segments > 0) {
                if (is_sep(path[i]) && --segments == 0) break;
                ++i;
            }
            return i;
        }
        if (n >= 6 && is_ascii_letter(path[4]) && path[5] == ':') {
            i = 6;
            if (i < n && is_sep(path[i])) ++i;
            return i;
        }
        while (i < n && !is_sep(path[i])) ++i;
        return i < n ? i + 1 : i;
    }
    if (unc) {
        i = 2;
        int segments = 2;
        while (i < n && segments > 0) {
            if (is_sep(path[i]) && --segments == 0) break;
            ++i;
        }
        return i;
    }
    if (n >= 1 && is_sep(path[0])) return 1;
    if (n >= 2 && path[1] == ':' && is_ascii_letter(path[0])) {
        i = 2;
        if (n >= 3 && is_sep(path[2])) ++i;
        return i;
    }
    return 0;
#else
    return !path.empty() && path[0] == '/' ? 1 : 0;
#endif
}

// PathInternal.NormalizeDirectorySeparators: '/' becomes '\' (Windows) and repeated separators one, except the two that start a UNC path.
std::string normalize_separators(std::string_view path) {
#ifdef _WIN32
    std::string result;
    result.reserve(path.size());
    bool previous_sep = false;
    for (std::size_t i = 0; i < path.size(); ++i) {
        const char c = path[i];
        if (is_sep(c)) {
            if (previous_sep && !(i == 1)) continue;
            result.push_back('\\');
            previous_sep = true;
        } else {
            result.push_back(c);
            previous_sep = false;
        }
    }
    return result;
#else
    return std::string(path);
#endif
}

}  // namespace

bool is_directory_separator(char c) { return is_sep(c); }

bool is_path_rooted(std::string_view path) {
#ifdef _WIN32
    return (path.size() >= 1 && is_sep(path[0])) || (path.size() >= 2 && path[1] == ':');
#else
    return !path.empty() && path[0] == '/';
#endif
}

std::string combine(std::string_view path1, std::string_view path2) {
    if (path1.empty()) return std::string(path2);
    if (path2.empty()) return std::string(path1);
    if (is_path_rooted(path2)) return std::string(path2);
    std::string result(path1);
    if (!is_sep(path1.back())) result.push_back(kDirectorySeparatorChar);
    result.append(path2);
    return result;
}

std::string combine(std::string_view path1, std::string_view path2, std::string_view path3) { return combine(combine(path1, path2), path3); }

std::string get_file_name(std::string_view path) {
    // after the last separator, but not inside the root ("C:x" is "x", and a UNC share is the root: no file name)
    const std::size_t root = root_length(path);
    std::size_t i = path.size();
    while (i > 0 && !is_sep(path[i - 1])) --i;
    // i is one past the last separator (0: none)
    const std::size_t last_sep_plus_one = i;
    return std::string(path.substr(last_sep_plus_one == 0 || last_sep_plus_one - 1 < root ? root : last_sep_plus_one));
}

std::string get_extension(std::string_view path) {
    const std::size_t length = path.size();
    for (std::size_t i = length; i-- > 0;) {
        const char c = path[i];
        if (c == '.') {
            if (i != length - 1) return std::string(path.substr(i));
            return {};
        }
        if (is_sep(c)) break;
    }
    return {};
}

std::optional<std::string> get_directory_name(std::string_view path) {
    if (path.empty()) return std::nullopt;
    const std::size_t root = root_length(path);
    if (path.size() <= root) return std::nullopt;
    std::size_t end = path.size();
    while (end > root && !is_sep(path[--end])) {
    }
    while (end > root && is_sep(path[end - 1])) --end;
    return normalize_separators(path.substr(0, end));
}

std::string change_extension(std::string_view path, std::optional<std::string_view> extension) {
    if (path.empty()) return {};
    std::size_t sub_length = path.size();
    for (std::size_t i = path.size(); i-- > 0;) {
        const char c = path[i];
        if (c == '.') {
            sub_length = i;
            break;
        }
        if (is_sep(c)) break;
    }
    std::string result(path.substr(0, sub_length));
    if (!extension) return result;
    if (extension->empty()) return result.empty() || result.back() != '.' ? result + "." : result;
    if (extension->front() != '.') result.push_back('.');
    result.append(*extension);
    return result;
}

std::string native_text(const std::filesystem::path& path) {
    std::string text = cb::fs::to_text(path);
#ifdef _WIN32
    for (char& c : text)
        if (c == '/') c = kDirectorySeparatorChar;
#endif
    return text;
}

std::filesystem::path from_text(std::string_view file_name) { return cb::fs::from_text(file_name); }

std::string get_file_name_without_extension(std::string_view path) {
    const std::string name = get_file_name(path);
    const std::size_t dot = name.rfind('.');
    return dot == std::string::npos ? name : name.substr(0, dot);
}

}  // namespace cb::dotnet::decompiler::util::path
