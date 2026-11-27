#include "dotnet/decompiler/util/file_utility.hpp"

#include <algorithm>
#include <vector>

#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/platform.hpp"

namespace cb::dotnet::decompiler::util::file_utility {

namespace {

bool is_unc_path(std::string_view file_name) {
    return file_name.size() > 2 && (file_name[0] == '\\' || file_name[0] == '/') && (file_name[1] == '\\' || file_name[1] == '/');
}

char get_separator_for_path(std::string_view input) {
    if ((input.size() > 2 && input[1] == ':') || is_unc_path(input)) return '\\';
    return '/';
}

std::string add_trailing_separator(std::string input) {
    if (input.empty()) return input;
    const char last = input.back();
    if (last == path::kDirectorySeparatorChar || last == path::kAltDirectorySeparatorChar) return input;
    input.push_back(get_separator_for_path(input));
    return input;
}

bool is_separator(char c) { return c == path::kDirectorySeparatorChar || c == path::kAltDirectorySeparatorChar; }

// string.Split(separators): empty parts stay.
std::vector<std::string> split_on_separators(std::string_view text) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= text.size(); ++i) {
        if (i == text.size() || is_separator(text[i])) {
            parts.emplace_back(text.substr(start, i - start));
            start = i + 1;
        }
    }
    return parts;
}

std::string trim_end_separators(std::string_view text) {
    std::size_t end = text.size();
    while (end > 0 && is_separator(text[end - 1])) --end;
    return std::string(text.substr(0, end));
}

bool starts_with_ignore_case(std::string_view text, std::string_view prefix) {
    return text.size() >= prefix.size() && compare_ordinal_ignore_case(text.substr(0, prefix.size()), prefix) == 0;
}

}  // namespace

std::string normalize_path(std::string_view file_name) {
    if (file_name.empty()) return std::string(file_name);

    std::size_t i;
    bool is_web = false;
    for (i = 0; i < file_name.size(); ++i) {
        if (file_name[i] == '/' || file_name[i] == '\\') break;
        if (file_name[i] == ':') {
            if (i > 1) is_web = true;
            break;
        }
    }

    char output_separator = '/';
    bool is_relative;
    bool is_absolute_unix_path = false;

    std::string result;
    if (!is_web && is_unc_path(file_name)) {
        // UNC path
        i = 2;
        output_separator = '\\';
        result.push_back(output_separator);
        is_relative = false;
    } else {
        i = 0;
        is_absolute_unix_path = file_name[0] == '/';
        is_relative = !is_web && !is_absolute_unix_path && (file_name.size() < 2 || file_name[1] != ':');
        if (file_name.size() >= 2 && file_name[1] == ':') output_separator = '\\';
    }
    int levels_back = 0;
    std::size_t segment_start_pos = i;
    for (; i <= file_name.size(); ++i) {
        if (i == file_name.size() || file_name[i] == '/' || file_name[i] == '\\') {
            const std::size_t segment_length = i - segment_start_pos;
            switch (segment_length) {
            case 0:
                // ignore empty segment (if not in web mode)
                if (is_web) result.push_back(output_separator);
                break;
            case 1:
                // ignore /./ segment, but append other one-letter segments
                if (file_name[segment_start_pos] != '.') {
                    if (!result.empty()) result.push_back(output_separator);
                    result.push_back(file_name[segment_start_pos]);
                }
                break;
            case 2:
                if (file_name[segment_start_pos] == '.' && file_name[segment_start_pos + 1] == '.') {
                    // remove previous segment
                    std::ptrdiff_t j;
                    for (j = static_cast<std::ptrdiff_t>(result.size()) - 1; j >= 0 && result[static_cast<std::size_t>(j)] != output_separator; --j) {
                    }
                    if (j > 0) {
                        result.resize(static_cast<std::size_t>(j));
                    } else if (is_absolute_unix_path) {
                        result.clear();
                    } else if (is_relative) {
                        if (result.empty()) ++levels_back;
                        else result.clear();
                    }
                    break;
                }
                // append a normal segment
                [[fallthrough]];
            default:
                if (!result.empty()) result.push_back(output_separator);
                result.append(file_name.substr(segment_start_pos, segment_length));
                break;
            }
            segment_start_pos = i + 1;   // remember start position for next segment
        }
    }
    if (!is_web) {
        if (is_relative) {
            for (int j = 0; j < levels_back; ++j) result.insert(0, std::string("..") + output_separator);
        }
        if (!result.empty() && result.back() == output_separator) result.pop_back();
        if (is_absolute_unix_path) result.insert(0, "/");
        if (result.size() == 2 && result[1] == ':') result.push_back(output_separator);
        if (result.empty()) return ".";
    }
    return result;
}

std::optional<std::string> normalize_path(const std::optional<std::string>& file_name) {
    if (!file_name) return std::nullopt;
    return normalize_path(std::string_view(*file_name));
}

bool is_equal_file_name(const std::optional<std::string>& file_name1, const std::optional<std::string>& file_name2) {
    const std::optional<std::string> a = normalize_path(file_name1);
    const std::optional<std::string> b = normalize_path(file_name2);
    if (a.has_value() != b.has_value()) return false;
    if (!a) return true;
    return compare_ordinal_ignore_case(*a, *b) == 0;
}

bool is_base_directory(const std::optional<std::string>& base_directory, const std::optional<std::string>& test_directory) {
    if (!base_directory || !test_directory) return false;
    std::string base = normalize_path(std::string_view(*base_directory));
    if (base == "." || base.empty()) return !path::is_path_rooted(*test_directory);
    base = add_trailing_separator(base);
    const std::string test = add_trailing_separator(normalize_path(std::string_view(*test_directory)));
    return starts_with_ignore_case(test, base);
}

std::string get_relative_path(const std::optional<std::string>& base_directory_path, std::string_view abs_path_in) {
    if (!base_directory_path || base_directory_path->empty()) return std::string(abs_path_in);

    const std::string base_directory = normalize_path(std::string_view(*base_directory_path));
    const std::string abs_path = normalize_path(abs_path_in);

    const std::vector<std::string> b_path = base_directory != "." ? split_on_separators(trim_end_separators(base_directory)) : std::vector<std::string>();
    const std::vector<std::string> a_path = abs_path != "." ? split_on_separators(trim_end_separators(abs_path)) : std::vector<std::string>();
    std::size_t indx = 0;
    for (; indx < std::min(b_path.size(), a_path.size()); ++indx) {
        if (compare_ordinal_ignore_case(b_path[indx], a_path[indx]) != 0) break;
    }

    if (indx == 0 && (path::is_path_rooted(base_directory) || path::is_path_rooted(abs_path))) return abs_path;

    if (indx == b_path.size() && indx == a_path.size()) return ".";
    std::string result;
    for (std::size_t i = indx; i < b_path.size(); ++i) {
        result += "..";
        result.push_back(path::kDirectorySeparatorChar);
    }
    for (std::size_t i = indx; i < a_path.size(); ++i) {
        if (i > indx) result.push_back(path::kDirectorySeparatorChar);
        result += a_path[i];
    }
    if (!result.empty() && result.back() == path::kDirectorySeparatorChar) result.pop_back();
    return result;
}

std::optional<std::string> trim_path(const std::optional<std::string>& path_in, int max_chars) {
    // Lengths are counted in bytes of the UTF-8 text (upstream counts UTF-16 characters: the same for the ASCII paths this is meant for).
    const std::string ellipsis = "\xE2\x80\xA6";   // HORIZONTAL ELLIPSIS
    const int ellipsis_length = 2;

    if (!path_in || static_cast<int>(path_in->size()) <= max_chars) return path_in;
    const std::string& path_text = *path_in;
    char sep = path::kDirectorySeparatorChar;
    if (path_text.find(path::kAltDirectorySeparatorChar) != std::string::npos && path_text.find(path::kDirectorySeparatorChar) == std::string::npos)
        sep = path::kAltDirectorySeparatorChar;
    const std::vector<std::string> parts = split_on_separators(path_text);
    int len = ellipsis_length;   // For initial ellipsis
    int index = static_cast<int>(parts.size());
    // From the end of the path, fit as many parts as possible:
    while (index > 1 && len + static_cast<int>(parts[static_cast<std::size_t>(index) - 1].size()) < max_chars) {
        len += static_cast<int>(parts[static_cast<std::size_t>(index) - 1].size()) + 1;
        --index;
    }

    std::string result = ellipsis;
    // If there's 5 chars left, partially fit another part:
    if (index > 1 && len + 5 <= max_chars) {
        if (index == 2 && static_cast<int>(parts[0].size()) <= ellipsis_length) {
            // If the partial part is part #1, and part #0 is as short as the ellipsis (e.g. just a drive letter), use part #0 instead of the ellipsis.
            result = parts[0];
        }
        result.push_back(sep);
        result += parts[static_cast<std::size_t>(index) - 1].substr(0, static_cast<std::size_t>(max_chars - len - 3));
        result += ellipsis;
    }
    while (index < static_cast<int>(parts.size())) {
        result.push_back(sep);
        result += parts[static_cast<std::size_t>(index)];
        ++index;
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::util::file_utility
