#include "cb/java/tree.hpp"

#include <algorithm>
#include <system_error>

#include "cb/core/fs.hpp"

namespace cb::java::tree {

namespace stdfs = std::filesystem;

namespace {

void walk(const stdfs::path& dir, std::vector<stdfs::path>& out) {
    std::error_code ec;
    stdfs::directory_iterator it(dir, ec);
    if (ec) return;
    std::vector<stdfs::path> file_paths;
    std::vector<stdfs::path> dir_paths;
    for (const stdfs::directory_entry& entry : it) {
        std::error_code entry_ec;
        if (entry.is_symlink(entry_ec)) {
            if (!entry_ec && entry.is_regular_file(entry_ec)) file_paths.push_back(entry.path());
            continue;   // a link to a directory is not descended into
        }
        if (entry.is_directory(entry_ec)) {
            dir_paths.push_back(entry.path());
        } else if (entry.is_regular_file(entry_ec)) {
            file_paths.push_back(entry.path());
        }
    }
    auto by_name = [](const stdfs::path& a, const stdfs::path& b) { return a.filename().native() < b.filename().native(); };
    std::sort(file_paths.begin(), file_paths.end(), by_name);
    std::sort(dir_paths.begin(), dir_paths.end(), by_name);
    out.insert(out.end(), file_paths.begin(), file_paths.end());
    for (const stdfs::path& sub : dir_paths) walk(sub, out);
}

}  // namespace

std::vector<stdfs::path> files(const stdfs::path& root) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (stdfs::is_directory(root, ec)) walk(root, out);
    return out;
}

std::vector<stdfs::path> files_with_extension(const stdfs::path& root, std::initializer_list<std::string_view> extensions) {
    std::vector<stdfs::path> out;
    for (stdfs::path& p : files(root)) {
        const std::string name = fs::to_text(p.filename());
        const bool wanted = std::any_of(extensions.begin(), extensions.end(),
                                        [&](std::string_view ext) { return name.size() >= ext.size() && name.ends_with(ext); });
        if (wanted) out.push_back(std::move(p));
    }
    return out;
}

}  // namespace cb::java::tree
