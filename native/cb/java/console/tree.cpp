#include "cb/java/console/tree.hpp"

#include <algorithm>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::java::console::tree {

namespace stdfs = std::filesystem;

namespace {

bool by_name(const std::string& a, const std::string& b) {
    const std::string la = text::lower(a), lb = text::lower(b);
    return la != lb ? la < lb : a < b;
}

// Returns false when the visitor asked to stop.
bool walk_dir(const stdfs::path& dir, std::size_t depth, const WalkOptions& options,
              const std::function<bool(const Listing&)>& visit) {
    Listing listing;
    listing.dir = dir;
    std::vector<std::string> enter;   // subfolders to descend into
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const stdfs::directory_entry& entry = *it;
        std::error_code status_ec;
        const bool is_dir = entry.is_directory(status_ec);
        std::string name = fs::to_text(entry.path().filename());
        if (is_dir && !status_ec) {
            if (options.skip_dir && options.skip_dir(name)) continue;
            if (options.max_depth && depth >= *options.max_depth) continue;
            const bool link = entry.is_symlink(status_ec);
            listing.subdirs.push_back(name);
            if (!link) enter.push_back(std::move(name));
        } else {
            listing.files.push_back(std::move(name));
        }
    }
    std::sort(listing.files.begin(), listing.files.end(), by_name);
    std::sort(listing.subdirs.begin(), listing.subdirs.end(), by_name);
    std::sort(enter.begin(), enter.end(), by_name);
    if (!visit(listing)) return false;
    for (const std::string& name : enter)
        if (!walk_dir(dir / name, depth + 1, options, visit)) return false;
    return true;
}

}  // namespace

void walk(const stdfs::path& root, const WalkOptions& options, const std::function<bool(const Listing&)>& visit) {
    std::error_code ec;
    if (!stdfs::is_directory(root, ec)) return;
    walk_dir(root, 0, options, visit);
}

std::vector<stdfs::path> files_below(const stdfs::path& root, const WalkOptions& options) {
    std::vector<stdfs::path> out;
    walk(root, options, [&](const Listing& l) {
        for (const std::string& f : l.files) out.push_back(l.dir / f);
        return true;
    });
    return out;
}

}  // namespace cb::java::console::tree
