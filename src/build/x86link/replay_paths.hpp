// build/x86link/replay_paths.hpp -- what the replays of captured tool runs share (build/x86link/replay.cpp for the links,
// build/x86cc/replay.cpp for the compiles): a case's recorded folders mapped into the folder it is replayed in, and back.
#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::build::replay_paths {

namespace stdfs = std::filesystem;

inline std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

inline std::string slashes(std::string s) {
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
}

// The recorded folders and where each goes in the case's own folder, longest first (a folder inside another wins).
class PathMap {
public:
    void add(const std::string& from, const stdfs::path& to) {
        std::string f = slashes(from);
        while (f.size() > 3 && f.back() == '/') f.pop_back();
        if (f.empty()) return;
        for (const auto& [k, v] : pairs_)
            if (lower(k) == lower(f)) return;
        pairs_.push_back({f, slashes(fs::to_text(to))});
        std::sort(pairs_.begin(), pairs_.end(), [](const auto& a, const auto& b) { return a.first.size() > b.first.size(); });
    }
    // A recorded path (or a text holding them) with each recorded folder replaced by its place here; '/' and '\' alike.
    std::string forward(const std::string& text) const { return swap(text, false); }
    // And back: a text of this run with the case's folders named as the recorded run named them.
    std::string backward(const std::string& text) const { return swap(text, true); }
    std::optional<stdfs::path> place(const std::string& recorded) const {
        const std::string r = slashes(recorded);
        for (const auto& [from, to] : pairs_) {
            if (lower(r).starts_with(lower(from)) && (r.size() == from.size() || r[from.size()] == '/'))
                return fs::from_text(to + r.substr(from.size()));
        }
        return std::nullopt;
    }

private:
    std::string swap(const std::string& text, bool back) const {
        std::string out = text;
        for (const auto& [from, to] : pairs_) {
            const std::string& a = back ? to : from;
            const std::string& b = back ? from : to;
            for (const std::string& form : {a, text::replace_all(a, "/", "\\")}) {
                const std::string want = lower(form);
                std::string low = lower(out);
                std::size_t at = 0;
                while ((at = low.find(want, at)) != std::string::npos) {
                    const std::string with = form.find('\\') != std::string::npos ? text::replace_all(b, "/", "\\") : b;
                    out.replace(at, form.size(), with);
                    low.replace(at, form.size(), lower(with));
                    at += with.size();
                }
            }
        }
        return out;
    }
    std::vector<std::pair<std::string, std::string>> pairs_;
};

}  // namespace cb::build::replay_paths
