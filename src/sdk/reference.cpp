// sdk/reference.cpp -- the reference of include/cb, made from its comments (reference.hpp).
#include "sdk/reference.hpp"

#include <algorithm>
#include <format>
#include <fstream>
#include <regex>
#include <set>
#include <sstream>
#include <system_error>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::sdk_impl::reference {
namespace {

namespace stdfs = std::filesystem;

std::vector<std::string> reference_lines(const stdfs::path& file) {
    std::vector<std::string> lines;
    std::ifstream in(file, std::ios::binary);
    if (!in) throw NotFoundError(file, 0, "cannot be read");
    for (std::string line; std::getline(in, line);) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(std::move(line));
    }
    return lines;
}

// A comment line without its frame: "/** ", " * ", " */", "// ".
std::string reference_unframed(std::string_view line) {
    std::string_view s = line;
    const std::size_t first = s.find_first_not_of(' ');
    if (first == std::string_view::npos) return "";
    s.remove_prefix(first);
    if (s.starts_with("/**")) s.remove_prefix(3);
    else if (s.starts_with("/*")) s.remove_prefix(2);
    else if (s.starts_with("//")) s.remove_prefix(2);
    else if (s.starts_with("*/")) s.remove_prefix(2);
    else if (s.starts_with("*")) s.remove_prefix(1);
    if (const std::size_t end = s.rfind("*/"); end != std::string_view::npos) s = s.substr(0, end);
    if (s.starts_with(' ')) s.remove_prefix(1);
    while (!s.empty() && s.back() == ' ') s.remove_suffix(1);
    return std::string(s);
}

// The name a declaration declares, and its kind.
std::pair<std::string, std::string> reference_declared(const std::string& declaration) {
    static const std::regex function(R"(\b(cb_[a-z0-9_]+)\s*\()");
    static const std::regex define(R"(^#define\s+([A-Za-z0-9_]+))");
    static const std::regex closing(R"(\}\s*([A-Za-z0-9_]+)\s*;\s*$)");
    static const std::regex handle(R"(^typedef\s+struct\s+[A-Za-z0-9_]+\s+([A-Za-z0-9_]+)\s*;)");
    std::smatch m;
    if (std::regex_search(declaration, m, define)) return {m[1].str(), "macro"};
    if (declaration.starts_with("CB_API") && std::regex_search(declaration, m, function)) return {m[1].str(), "function"};
    if (std::regex_search(declaration, m, handle)) return {m[1].str(), "type"};
    if (std::regex_search(declaration, m, closing)) return {m[1].str(), declaration.starts_with("typedef enum") ? "enum" : "type"};
    return {"", ""};
}

// The comment block that starts at `at` ("/**"), parsed; `at` is left on the line after its end.
Entry reference_comment(const std::string& file, const std::vector<std::string>& lines, std::size_t& at) {
    Entry e;
    e.line = static_cast<int>(at) + 1;
    std::vector<std::string> body;
    for (;;) {
        if (at >= lines.size()) throw ParseError(file, static_cast<std::uint32_t>(e.line), 0, "a /** comment that does not end");
        body.push_back(reference_unframed(lines[at]));
        const bool end = lines[at].find("*/") != std::string::npos;
        ++at;
        if (end) break;
    }
    while (!body.empty() && body.back().empty()) body.pop_back();
    if (body.empty()) throw ParseError(file, static_cast<std::uint32_t>(e.line), 0, "an empty /** comment");
    const std::size_t colon = body.front().find(": ");
    if (colon == std::string::npos) throw ParseError(file, static_cast<std::uint32_t>(e.line), 0, "the first line of a /** comment is \"name: summary\"");
    e.name = body.front().substr(0, colon);
    e.summary = body.front().substr(colon + 2);
    std::string* last = nullptr;   // the text a continuation line goes on with
    for (std::size_t k = 1; k < body.size(); ++k) {
        const std::string& line = body[k];
        if (line.starts_with("  ") && last != nullptr) {
            *last += " " + std::string(text::trim(line));
            continue;
        }
        if (!line.starts_with("@")) {
            e.description.push_back(line);
            last = &e.description.back();
            continue;
        }
        const std::size_t space = line.find(' ');
        const std::string tag = line.substr(0, space);
        std::string rest = space == std::string::npos ? "" : line.substr(space + 1);
        auto named = [&](std::vector<Tag>& into) {
            const std::size_t gap = rest.find(' ');
            into.push_back(Tag{rest.substr(0, gap), gap == std::string::npos ? "" : rest.substr(gap + 1)});
            last = &into.back().text;
        };
        if (tag == "@param") named(e.params);
        else if (tag == "@field") named(e.fields);
        else if (tag == "@return") e.returns = rest, last = &e.returns;
        else if (tag == "@ownership") e.ownership = rest, last = &e.ownership;
        else if (tag == "@threads") e.threads = rest, last = &e.threads;
        else if (tag == "@since") e.since = rest, last = &e.since;
        else throw ParseError(file, static_cast<std::uint32_t>(e.line + k), 0, std::format("{} is not a tag of the reference ({})", tag, e.name));
    }
    if (e.since.empty()) throw ParseError(file, static_cast<std::uint32_t>(e.line), 0, std::format("{} has no @since", e.name));
    return e;
}

// The declaration that starts at `at`: a #define line, or the lines up to the `;` that ends it outside braces.
std::string reference_declaration(const std::string& file, const std::vector<std::string>& lines, std::size_t& at, int comment_line) {
    while (at < lines.size() && text::trim(lines[at]).empty()) ++at;
    if (at >= lines.size()) throw ParseError(file, static_cast<std::uint32_t>(comment_line), 0, "a /** comment with nothing after it");
    if (lines[at].starts_with("#define")) return lines[at++];
    std::string out;
    int depth = 0;
    for (; at < lines.size(); ++at) {
        const std::string& line = lines[at];
        if (!out.empty()) out += '\n';
        out += line;
        for (const char c : line) depth += c == '{' ? 1 : c == '}' ? -1 : 0;
        if (depth == 0 && line.find(';') != std::string::npos) {
            ++at;
            return out;
        }
    }
    throw ParseError(file, static_cast<std::uint32_t>(comment_line), 0, "a declaration that does not end");
}

Header reference_header(const stdfs::path& path) {
    Header h;
    h.file = fs::to_text(path.filename());
    const std::vector<std::string> lines = reference_lines(path);
    std::size_t at = 0;
    // the overview: the header's first comment (a /* block or // lines)
    if (!lines.empty() && ((lines[0].starts_with("/*") && !lines[0].starts_with("/**")) || lines[0].starts_with("//"))) {
        const bool block = lines[0].starts_with("/*");
        std::vector<std::string> overview;
        for (; at < lines.size(); ++at) {
            if (!block && !lines[at].starts_with("//")) break;
            overview.push_back(reference_unframed(lines[at]));
            if (block && lines[at].find("*/") != std::string::npos) {
                ++at;
                break;
            }
        }
        while (!overview.empty() && overview.back().empty()) overview.pop_back();
        h.overview = text::join(overview, "\n");
    }
    for (; at < lines.size();) {
        const std::string_view line = text::trim(lines[at]);
        if (line.starts_with("/**")) {
            Entry e = reference_comment(h.file, lines, at);
            std::string declaration = reference_declaration(h.file, lines, at, e.line);
            const auto [name, kind] = reference_declared(declaration);
            if (name != e.name)
                throw ParseError(h.file, static_cast<std::uint32_t>(e.line), 0,
                                  std::format("the comment names {} but the declaration after it is {}", e.name, name.empty() ? "nothing the reference knows" : name));
            if (declaration.starts_with("CB_API ")) declaration.erase(0, 7);
            e.kind = kind;
            e.declaration = std::move(declaration);
            h.entries.push_back(std::move(e));
            continue;
        }
        if (line.starts_with("CB_API "))
            throw ParseError(h.file, static_cast<std::uint32_t>(at + 1), 0, std::format("{} has no /** comment above it", reference_declared(std::string(line)).first));
        ++at;
    }
    return h;
}

std::string reference_sentence(std::string s) {
    if (!s.empty() && s[0] >= 'a' && s[0] <= 'z') s[0] = static_cast<char>(s[0] - 'a' + 'A');
    return s;
}

// Markdown table cells may not hold a bar.
std::string reference_cell(std::string s) {
    std::string out;
    for (const char c : s) out += c == '|' ? std::string("\\|") : std::string(1, c);
    return out;
}

std::string reference_used_by(const Uses& uses, const std::string& name, std::string_view up) {
    const auto it = uses.find(name);
    if (it == uses.end() || it->second.empty()) return "";
    std::string out;
    for (const std::string& file : it->second) out += std::format("{}[{}]({}{})", out.empty() ? "" : ", ", file, up, file);
    return out;
}

std::string reference_entry(const Entry& e, const Header& h, const Uses& samples, const Uses& tests, std::string_view up, int level) {
    const std::string hashes(static_cast<std::size_t>(level), '#');
    std::ostringstream o;
    o << hashes << " " << e.name << "\n\n";
    o << "`" << h.file << "` · " << e.kind << " · since " << e.since << "\n\n";
    o << reference_sentence(e.summary) << "\n\n";
    o << "```" << (h.file.ends_with(".hpp") ? "cpp" : "c") << "\n" << e.declaration << "\n```\n\n";
    if (!e.description.empty()) o << text::join(e.description, "\n") << "\n\n";
    auto table = [&](std::string_view title, const std::vector<Tag>& tags) {
        if (tags.empty()) return;
        o << "| " << title << " | |\n|---|---|\n";
        for (const Tag& t : tags) o << "| `" << t.name << "` | " << reference_cell(t.text) << " |\n";
        o << "\n";
    };
    table("Parameter", e.params);
    table("Field", e.fields);
    auto part = [&](std::string_view title, const std::string& text) {
        if (!text.empty()) o << "**" << title << ".** " << text << "\n\n";
    };
    part("Returns", e.returns);
    part("Ownership", e.ownership);
    part("Thread safety", e.threads);
    if (e.kind == "function") {
        const std::string in_samples = reference_used_by(samples, e.name, up), in_tests = reference_used_by(tests, e.name, up);
        if (!in_samples.empty()) o << "**Samples.** " << in_samples << "\n\n";
        if (!in_tests.empty()) o << "**Tests.** " << in_tests << "\n\n";
    }
    return o.str();
}

std::string reference_page_name(const std::string& header) {
    std::string base = header;
    std::replace(base.begin(), base.end(), '.', '_');
    return base + ".md";
}

}  // namespace

std::vector<Header> read(const std::filesystem::path& include_dir) {
    std::vector<stdfs::path> files;
    for (const auto& entry : stdfs::directory_iterator(include_dir)) {
        const std::string ext = entry.path().extension().string();
        if (entry.is_regular_file() && (ext == ".h" || ext == ".hpp")) files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end(), [](const stdfs::path& a, const stdfs::path& b) {
        return std::pair(a.extension() == ".hpp", a.filename().string()) < std::pair(b.extension() == ".hpp", b.filename().string());
    });
    std::vector<Header> headers;
    for (const stdfs::path& f : files) headers.push_back(reference_header(f));
    return headers;
}

Uses uses(const std::vector<Header>& headers, const std::filesystem::path& root, const std::vector<std::filesystem::path>& where) {
    std::vector<std::pair<std::string, std::string>> texts;   // file, text
    for (const stdfs::path& folder : where) {
        std::error_code ec;
        if (!stdfs::is_directory(folder, ec)) continue;
        for (auto it = stdfs::recursive_directory_iterator(folder); it != stdfs::recursive_directory_iterator(); ++it) {
            const std::string ext = it->path().extension().string();
            if (!it->is_regular_file() || (ext != ".c" && ext != ".cpp")) continue;   // (code that calls; a header's comments only name)
            std::ifstream in(it->path(), std::ios::binary);
            std::ostringstream text;
            text << in.rdbuf();
            texts.emplace_back(fs::to_text(stdfs::relative(it->path(), root)), text.str());
        }
    }
    std::sort(texts.begin(), texts.end());
    Uses out;
    for (const Header& h : headers)
        for (const Entry& e : h.entries) {
            if (e.kind != "function") continue;
            const std::regex word("\\b" + e.name + "\\b");
            std::vector<std::string>& files = out[e.name];
            for (const auto& [file, text] : texts)
                if (std::regex_search(text, word)) files.push_back(file);
        }
    return out;
}

std::map<std::string, std::string> pages(const std::vector<Header>& headers, const Uses& samples, const Uses& tests) {
    std::map<std::string, std::string> out;
    std::ostringstream index;
    index << "# The CodeBreaker SDK reference\n\n"
          << "Made from the comments of `include/cb` by `tools\\sdk\\reference.cmd` (src/sdk/reference.cpp); do not edit these pages, edit the "
             "headers. The design is [docs/SDK_DESIGN.md](../../SDK_DESIGN.md); the samples are in [samples/](../../../samples/).\n\n"
          << "## Headers\n\n";
    for (const Header& h : headers) {
        // the overview's first paragraph, on one line
        std::string first = h.overview.substr(0, h.overview.find("\n\n"));
        std::replace(first.begin(), first.end(), '\n', ' ');
        index << "- [" << h.file << "](" << reference_page_name(h.file) << "): " << first.substr(first.find(" -- ") == std::string::npos ? 0 : first.find(" -- ") + 4)
              << "\n";
    }
    index << "\n## Everything, by name\n\n| Name | Kind | Header | Since | Summary |\n|---|---|---|---|---|\n";
    std::vector<std::pair<const Entry*, const Header*>> all;
    for (const Header& h : headers)
        for (const Entry& e : h.entries) all.emplace_back(&e, &h);
    std::sort(all.begin(), all.end(), [](const auto& a, const auto& b) { return a.first->name < b.first->name; });
    for (const auto& [e, h] : all) {
        const std::string link = e->kind == "function" ? "functions/" + e->name + ".md" : reference_page_name(h->file) + "#" + text::lower(e->name);
        index << "| [" << e->name << "](" << link << ") | " << e->kind << " | " << h->file << " | " << e->since << " | "
              << reference_cell(reference_sentence(e->summary)) << " |\n";
    }
    out["README.md"] = index.str();
    for (const Header& h : headers) {
        std::ostringstream page;
        page << "# " << h.file << "\n\n[The reference](README.md)\n\n```\n" << h.overview << "\n```\n\n";
        for (const Entry& e : h.entries) {
            page << reference_entry(e, h, samples, tests, "../../../", 2);
            if (e.kind == "function") page << "[The page of " << e.name << "](functions/" << e.name << ".md)\n\n";
        }
        out[reference_page_name(h.file)] = page.str();
        for (const Entry& e : h.entries) {
            if (e.kind != "function") continue;
            out["functions/" + e.name + ".md"] = "[The reference](../README.md) · [" + h.file + "](../" + reference_page_name(h.file) + ")\n\n" +
                                                 reference_entry(e, h, samples, tests, "../../../../", 1);
        }
    }
    return out;
}

std::size_t write(const std::map<std::string, std::string>& pages, const std::filesystem::path& out) {
    std::error_code ec;
    if (stdfs::is_directory(out, ec))
        for (auto it = stdfs::recursive_directory_iterator(out); it != stdfs::recursive_directory_iterator(); ++it) {
            if (!it->is_regular_file() || it->path().extension() != ".md") continue;
            if (!pages.contains(fs::to_text(stdfs::relative(it->path(), out)))) stdfs::remove(it->path(), ec);
        }
    std::size_t written = 0;
    for (const auto& [name, text] : pages) {
        const stdfs::path file = out / fs::from_text(name);
        stdfs::create_directories(file.parent_path(), ec);
        std::ofstream o(file, std::ios::binary);
        o << text;
        if (!o) throw IoError(file, 0, "cannot be written");
        ++written;
    }
    return written;
}

std::map<std::string, std::string> of_checkout(const std::filesystem::path& checkout) {
    const std::vector<Header> headers = read(checkout / "include" / "cb");
    const Uses samples = uses(headers, checkout, {checkout / "samples"});
    // (the tests are the SDK's own: src/sdk/*_test.cpp)
    const Uses all_tests = uses(headers, checkout, {checkout / "src" / "sdk"});
    Uses only_tests;
    for (const auto& [name, files] : all_tests)
        for (const std::string& f : files)
            if (f.ends_with("_test.cpp") && !f.ends_with("/reference_test.cpp")) only_tests[name].push_back(f);   // (that one names functions as text)
    return pages(headers, samples, only_tests);
}

}  // namespace cb::sdk_impl::reference
