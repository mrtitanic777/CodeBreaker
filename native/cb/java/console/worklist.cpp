#include "cb/java/console/worklist.hpp"

#include <algorithm>
#include <format>
#include <system_error>
#include <tuple>

#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::java::console::worklist {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kErrorMark = ": error:";
constexpr std::string_view kMessageMark = "error: ";
constexpr std::size_t kCategoryColumns = 60;

bool is_digit(char c) { return c >= '0' && c <= '9'; }

// "unknown type 'Foo' at 12" -> "unknown type X at N": quoted names and numbers made alike.
std::string normalise_message(std::string_view message) {
    std::string quoted_out;
    for (std::size_t i = 0; i < message.size(); ++i) {
        if (message[i] == '\'') {
            const std::size_t close = message.find('\'', i + 1);
            if (close != std::string_view::npos) {
                quoted_out += 'X';
                i = close;
                continue;
            }
        }
        quoted_out += message[i];
    }
    std::string out;
    for (std::size_t i = 0; i < quoted_out.size(); ++i) {
        if (is_digit(quoted_out[i])) {
            while (i + 1 < quoted_out.size() && is_digit(quoted_out[i + 1])) ++i;
            out += 'N';
        } else {
            out += quoted_out[i];
        }
    }
    return out;
}

}  // namespace

std::size_t count_errors(std::string_view output) {
    std::size_t n = 0;
    for (std::string_view line : text::lines(output))
        if (line.find(kErrorMark) != std::string_view::npos) ++n;
    return n;
}

std::string dominant_category(std::string_view output) {
    // (message, count) in order of first appearance, so that a tie is decided by who came first
    std::vector<std::pair<std::string, std::size_t>> counts;
    for (std::string_view line : text::lines(output)) {
        const std::size_t at = line.find(kMessageMark);
        if (at == std::string_view::npos) continue;
        const std::string normalised = normalise_message(line.substr(at + kMessageMark.size()));
        const std::string key(text::utf8_prefix(normalised, kCategoryColumns));
        auto it = std::find_if(counts.begin(), counts.end(), [&](const auto& c) { return c.first == key; });
        if (it == counts.end()) counts.emplace_back(key, 1);
        else ++it->second;
    }
    if (counts.empty()) return "(unknown)";
    std::size_t best = 0;
    for (std::size_t i = 1; i < counts.size(); ++i)
        if (counts[i].second > counts[best].second) best = i;
    return counts[best].first;
}

Worklist check(const stdfs::path& out_dir, const Options& options) {
    std::vector<std::string> cpps;
    std::error_code ec;
    for (stdfs::directory_iterator it(out_dir, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        if (text::iequals(fs::to_text(it->path().extension()), ".cpp")) cpps.push_back(fs::to_text(it->path().filename()));
    }
    std::sort(cpps.begin(), cpps.end());

    std::vector<std::optional<Failure>> results(cpps.size());
    auto compile_one = [&](std::size_t i) {
        process::ProcessSpec spec;
        spec.argv = {options.compiler, "-std=" + options.standard, "-fsyntax-only", "-I" + fs::to_text(out_dir),
                     fs::to_text(out_dir / cpps[i])};
        spec.out = process::Capture::Discard;
        spec.timeout = options.timeout;
        const process::ProcessResult r = process::run(spec);
        const std::string err = text::sanitize_utf8(as_text(r.err));
        const std::size_t errors = count_errors(err);
        if (errors != 0) results[i] = Failure{errors, cpps[i], dominant_category(err)};
    };
    if (options.jobs > 1 && cpps.size() > 1) {
        parallel::ThreadPool pool(std::min(options.jobs, cpps.size()));
        parallel::parallel_for(pool, cpps.size(), compile_one);
    } else {
        for (std::size_t i = 0; i < cpps.size(); ++i) compile_one(i);
    }

    Worklist out;
    out.total = cpps.size();
    for (std::optional<Failure>& r : results) {
        if (r) out.failing.push_back(std::move(*r));
        else ++out.clean;
    }
    std::sort(out.failing.begin(), out.failing.end(), [](const Failure& a, const Failure& b) {
        return std::tie(a.errors, a.file, a.category) < std::tie(b.errors, b.file, b.category);
    });
    return out;
}

std::string render(const stdfs::path& out_dir, const Worklist& w) {
    const std::string dir = out_dir.u8string();
    std::string md = std::format("# Override worklist ({})\n\n", dir);
    md += std::format("{} / {} clean; {} failing. Sorted by error count (fewest first).\n", w.clean, w.total,
                      w.failing.size());
    md += std::format("Regenerate: `codebreaker java worklist {}`\n\n", dir);
    md += "| errs | file | dominant error |\n|---:|---|---|\n";
    for (const Failure& f : w.failing) md += std::format("| {} | {} | {} |\n", f.errors, f.file, f.category);
    return md;
}

std::string run(const stdfs::path& out_dir, const stdfs::path& worklist_path, const Options& options) {
    const Worklist w = check(out_dir, options);
    if (worklist_path.has_parent_path()) fs::make_dirs(worklist_path.parent_path());
    fs::write_text(worklist_path, render(out_dir, w));
    return std::format("clean={}/{}  failing={}  -> {}", w.clean, w.total, w.failing.size(), worklist_path.u8string());
}

}  // namespace cb::java::console::worklist
