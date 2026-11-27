// source/java_writer.cpp -- a Java program's C++ source in a folder, every class labelled with what is known (java_writer.hpp).

#include "source/java_writer.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "source/authorship.hpp"

namespace cb::source::java_writer {

namespace {

constexpr std::size_t kSaidColumns = 200;     // how much of a reason a class's file says
constexpr std::size_t kReportColumns = 160;   // and STATUS.md

std::string first_line(std::string_view reason) {
    const std::vector<std::string_view> lines = text::lines(reason);
    return lines.empty() ? std::string() : std::string(text::utf8_prefix(lines[0], kSaidColumns));
}

std::string counts_text(const store::ClassCount& counts) {
    std::vector<std::string> parts;
    for (const store::Class c : store::kClassOrder) {
        const auto n = counts.find(c);
        if (n != counts.end() && n->second != 0) parts.push_back(std::format("{} {}", store::title(c), n->second));
    }
    return text::join(parts, ", ");
}

// Does the folder hold a .h or .cpp that nobody here wrote?
bool holds_files_written_by_hand(const std::filesystem::path& dir) {
    std::error_code ec;
    if (!std::filesystem::is_directory(dir, ec)) return false;
    for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(dir)) {
        const std::string ext = e.path().extension().string();
        if (e.is_regular_file() && (ext == ".h" || ext == ".cpp") && !made_by_us(e.path())) return true;
    }
    return false;
}

}  // namespace

std::string banner(std::string_view file, store::Class status, std::string_view said) {
    return std::format("// {} -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [{}] {}\n", file, store::title(status), said);
}

std::vector<ClassRow> class_rows(store::FunctionStore& store, std::string_view module, std::string_view profile) {
    const std::map<std::string, store::Best> best = store.best(module, profile);
    std::vector<ClassRow> rows;
    for (const std::string& name : store.addresses(module)) {   // (sorted)
        ClassRow row{name, store::Class::NoSource, ""};
        const auto found = best.find(name);
        std::string why = "not judged on the current verifier";
        std::string detail;
        if (found != best.end()) {
            row.status = found->second.cls;
            why = found->second.candidate.reason;
            detail = found->second.candidate.detail;
        }
        row.said = first_line(row.status == store::Class::NoCompile ? why : (detail.empty() ? why : detail));
        rows.push_back(std::move(row));
    }
    return rows;
}

std::string status_report(std::string_view name, const std::vector<ClassRow>& rows, const store::ClassCount& counts) {
    std::vector<std::string> lines = {std::format("# {} -- C++03 transpiled from Java", name), "",
                                      std::format("{} classes: {}", rows.size(), counts_text(counts)), "", "| class | status | why |", "|---|---|---|"};
    for (const ClassRow& r : rows) {
        const std::string why = text::replace_all(text::utf8_prefix(r.said, kReportColumns), "|", "\\|");
        lines.push_back(std::format("| {} | {} | {} |", r.name, store::title(r.status), why));
    }
    return text::join(lines, "\n") + "\n";
}

Written write_java(std::string_view name, store::FunctionStore& store, std::string_view module, std::string_view profile,
                   const std::filesystem::path& out_dir, const std::filesystem::path& state_dir, const Conversion& convert, const Log& log) {
    const auto say = [&](const std::string& line) {
        if (log) log(line);
    };
    Written out;
    out.folder = out_dir;
    if (holds_files_written_by_hand(out_dir)) {
        say(std::format("{}: holds files written by hand -- the C++ goes to the state folder instead", fs::to_text(out_dir)));
        out.folder = state_dir;
    }
    convert(out.folder);
    out.rows = class_rows(store, module, profile);
    for (const store::Class c : store::kClassOrder) out.counts[c] = 0;
    for (const ClassRow& r : out.rows) {
        ++out.counts[r.status];
        for (const char* ext : {".cpp", ".h"}) {   // (a class's .cpp, or its .h when it has none)
            const std::filesystem::path target = out.folder / (r.name + ext);
            if (!std::filesystem::is_regular_file(target)) continue;
            const Bytes old = fs::read_file(target);
            std::string stamped = banner(r.name + ext, r.status, r.said);
            stamped.append(as_text(old));
            fs::write_file(target, as_bytes(stamped));
            break;
        }
    }
    fs::write_file(out.folder / "STATUS.md", as_bytes(status_report(name, out.rows, out.counts)));
    say(std::format("{}: {} classes -> {}  ({})", name, out.rows.size(), fs::to_text(out.folder), counts_text(out.counts)));
    return out;
}

}  // namespace cb::source::java_writer
