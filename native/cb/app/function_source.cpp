// cb/app/function_source.cpp -- see function_source.hpp.
#include "cb/app/function_source.hpp"

#include <algorithm>
#include <cctype>
#include <set>
#include <system_error>

#include "cb/base/config.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/store/functions.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::app::fnsrc {
namespace stdfs = std::filesystem;

namespace {

constexpr std::uintmax_t kMaxSourceBytes = 16u * 1024 * 1024;
constexpr std::size_t kMaxAsmBody = 600;

bool fs_ident_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool fs_ident_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '@' || c == '$' || c == '?'; }

std::string fs_lower(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::string fs_trim(std::string_view s) {
    std::size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return std::string(s.substr(a, b - a));
}

// A file as lines: no line ends, tabs as four spaces (as the preview shows them). Empty when it is missing or too large.
std::vector<std::string> fs_read_lines(const stdfs::path& path) {
    std::vector<std::string> lines;
    std::error_code ec;
    const std::uintmax_t size = stdfs::file_size(path, ec);
    if (ec || size > kMaxSourceBytes) return lines;
    std::string text;
    try {
        text = fs::read_text(path);
    } catch (const std::exception&) {
        return lines;
    }
    std::string line;
    for (const char c : text) {
        if (c == '\n') {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(std::move(line));
            line.clear();
        } else if (c == '\t') {
            line += "    ";
        } else {
            line += c;
        }
    }
    if (!line.empty()) lines.push_back(std::move(line));
    return lines;
}

bool fs_ends_with(const std::string& s, std::string_view tail) { return s.size() >= tail.size() && s.compare(s.size() - tail.size(), tail.size(), tail) == 0; }

// The name of the function a C line at column 0 defines (or opens the definition of); empty when the line is not one.
std::string fs_defined_name(const std::vector<std::string>& lines, std::size_t i) {
    const std::string& line = lines[i];
    if (line.empty() || !fs_ident_start(line[0])) return {};
    const std::size_t open = line.find('(');
    if (open == std::string::npos || line.find(';') != std::string::npos) return {};
    if (line.compare(0, 6, "typedef") == 0) return {};
    std::size_t end = open;
    while (end > 0 && line[end - 1] == ' ') --end;
    std::size_t begin = end;
    while (begin > 0 && fs_ident_char(line[begin - 1])) --begin;
    if (begin == end || !fs_ident_start(line[begin])) return {};
    const std::string name = line.substr(begin, end - begin);
    static const std::set<std::string> kKeywords = {"if", "while", "for", "switch", "return", "sizeof", "else", "do"};
    if (kKeywords.contains(name)) return {};
    // the opening brace is on this line (K&R) or on one of the next two non-blank lines
    if (fs_ends_with(fs_trim(line), "{")) return name;
    std::size_t looked = 0;
    for (std::size_t k = i + 1; k < lines.size() && looked < 3; ++k) {
        const std::string t = fs_trim(lines[k]);
        if (t.empty()) continue;
        ++looked;
        if (t == "{") return name;
        if (t[0] != '(' && t.find(')') == std::string::npos && t.find(',') == std::string::npos) return {};
    }
    return {};
}

bool fs_is_c_file(const stdfs::path& p) {
    const std::string e = fs_lower(p.extension().string());
    return e == ".c" || e == ".cpp" || e == ".cc";
}

bool fs_is_asm_file(const stdfs::path& p) {
    const std::string e = fs_lower(p.extension().string());
    return e == ".asm";
}

std::vector<stdfs::path> fs_files_with(const stdfs::path& folder, bool (*wanted)(const stdfs::path&)) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code e2;
        if (it->is_regular_file(e2) && wanted(it->path())) out.push_back(it->path());
        if (out.size() >= 200) break;
    }
    std::sort(out.begin(), out.end());
    return out;
}

// The call target of an assembly line (`call lz_handle_of`, `call near ptr sub_00036`); empty when the line is not a call by name.
std::string fs_call_target(const std::string& line) {
    std::string code = line.substr(0, line.find(';'));
    std::size_t at = 0;
    while (at < code.size() && std::isspace(static_cast<unsigned char>(code[at]))) ++at;
    if (code.compare(at, 4, "call") != 0 || at + 4 >= code.size() || !std::isspace(static_cast<unsigned char>(code[at + 4]))) return {};
    std::string last;
    std::size_t i = at + 4;
    while (i < code.size()) {
        while (i < code.size() && !fs_ident_start(code[i])) ++i;
        const std::size_t b = i;
        while (i < code.size() && fs_ident_char(code[i])) ++i;
        if (i > b) {
            const std::string word = code.substr(b, i - b);
            const std::string low = fs_lower(word);
            if (low != "near" && low != "far" && low != "ptr" && low != "short" && low != "dword" && low != "word" && low != "offset") last = word;
        }
    }
    return last;
}

}  // namespace

std::string address_of_name(std::string_view name) {
    if (name.size() <= 4 || name.substr(0, 4) != "sub_") return {};
    for (const char c : name.substr(4))
        if (std::isxdigit(static_cast<unsigned char>(c)) == 0) return {};
    return "0x" + std::string(name.substr(4));
}

std::string normal_address(std::string_view address) {
    std::string s = fs_lower(fs_trim(address));
    if (s.compare(0, 2, "0x") == 0) s.erase(0, 2);
    std::size_t z = 0;
    while (z + 1 < s.size() && s[z] == '0') ++z;
    return s.substr(z);
}

std::vector<convert::FunctionRecord> scan_folder(const stdfs::path& folder) {
    std::vector<convert::FunctionRecord> out;
    std::set<std::string> seen;
    for (const stdfs::path& file : fs_files_with(folder, fs_is_c_file)) {
        const std::vector<std::string> lines = fs_read_lines(file);
        std::error_code ec;
        const std::string rel = fs::to_text(stdfs::relative(file, folder, ec));
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const std::string name = fs_defined_name(lines, i);
            if (name.empty() || !seen.insert(name).second) continue;
            convert::FunctionRecord r;
            r.name = name;
            r.address = address_of_name(name);
            r.source_file = ec ? file.filename().string() : rel;
            r.source_line = static_cast<int>(i) + 1;
            out.push_back(std::move(r));
        }
    }
    return out;
}

std::map<std::string, convert::FunctionVerdict> stored_verdicts(const stdfs::path& input, const stdfs::path& state) {
    std::map<std::string, convert::FunctionVerdict> out;
    if (input.empty() || state.empty()) return out;
    try {
        std::error_code ec;
        if (!stdfs::is_regular_file(state / "functions.sqlite", ec) && !stdfs::is_regular_file(store::functions::default_path(state), ec)) return out;   // (no store: nothing to read)
        if (!stdfs::is_regular_file(input, ec) || stdfs::file_size(input, ec) > 64u * 1024 * 1024) return out;
        const Bytes bytes = fs::read_file(input);
        const std::string sha = crypto::to_hex(crypto::sha256(bytes));
        for (const auto& [address, cls] : store::functions::latest(state, sha)) {
            if (cls == verify::Class::Verified || cls == verify::Class::Weak) out[normal_address(address)] = convert::FunctionVerdict::Matches;
            else if (cls == verify::Class::Unverified) out[normal_address(address)] = convert::FunctionVerdict::Differs;
        }
    } catch (const std::exception&) {
        out.clear();
    }
    return out;
}

std::vector<convert::FunctionRecord> collect(const stdfs::path& folder, const std::vector<convert::FunctionRecord>& recorded,
                                             const std::map<std::string, convert::FunctionVerdict>& verdicts) {
    std::vector<convert::FunctionRecord> list = recorded;
    if (list.empty()) list = scan_folder(folder);
    bool need_places = false;
    for (const convert::FunctionRecord& r : list) need_places = need_places || r.source_file.empty();
    if (need_places && !recorded.empty()) {
        std::map<std::string, convert::FunctionRecord> places;
        for (convert::FunctionRecord& s : scan_folder(folder)) places.emplace(s.name, std::move(s));
        for (convert::FunctionRecord& r : list) {
            if (!r.source_file.empty()) continue;
            if (const auto it = places.find(r.name); it != places.end()) {
                r.source_file = it->second.source_file;
                r.source_line = it->second.source_line;
            }
        }
    }
    for (convert::FunctionRecord& r : list) {
        if (r.verdict != convert::FunctionVerdict::NotChecked) continue;
        const std::string address = r.address.empty() ? address_of_name(r.name) : r.address;
        if (address.empty()) continue;
        if (const auto it = verdicts.find(normal_address(address)); it != verdicts.end()) r.verdict = it->second;
    }
    return list;
}

Counts count(const std::vector<convert::FunctionRecord>& list) {
    Counts c;
    c.total = static_cast<int>(list.size());
    for (const convert::FunctionRecord& r : list) {
        switch (r.verdict) {
            case convert::FunctionVerdict::Matches: ++c.matches; break;
            case convert::FunctionVerdict::Differs: ++c.differs; break;
            case convert::FunctionVerdict::NotChecked: ++c.unchecked; break;
        }
    }
    return c;
}

std::vector<int> filtered(const std::vector<convert::FunctionRecord>& list, int filter) {
    std::vector<int> out;
    for (std::size_t i = 0; i < list.size(); ++i) {
        const convert::FunctionVerdict v = list[i].verdict;
        if (filter == 0 || (filter == 1 && v == convert::FunctionVerdict::Differs) || (filter == 2 && v == convert::FunctionVerdict::NotChecked))
            out.push_back(static_cast<int>(i));
    }
    return out;
}

std::vector<std::string> c_body(const std::vector<std::string>& file_lines, std::string_view name, int* first_line) {
    for (std::size_t i = 0; i < file_lines.size(); ++i) {
        if (fs_defined_name(file_lines, i) != name) continue;
        std::vector<std::string> body;
        int depth = 0;
        bool opened = false;
        for (std::size_t k = i; k < file_lines.size() && body.size() < 4000; ++k) {
            body.push_back(file_lines[k]);
            for (const char c : file_lines[k]) {
                if (c == '{') {
                    ++depth;
                    opened = true;
                } else if (c == '}') {
                    --depth;
                }
            }
            if (opened && depth <= 0) break;
        }
        // the decompiler's blank line between the signature and its brace is dropped
        if (body.size() > 2 && fs_trim(body[1]).empty() && fs_trim(body[2]) == "{") body.erase(body.begin() + 1);
        if (first_line != nullptr) *first_line = static_cast<int>(i);
        return body;
    }
    return {};
}

std::vector<std::string> asm_body(const std::vector<std::string>& file_lines, std::string_view name) {
    const std::string want = fs_lower(name);
    std::size_t start = file_lines.size();
    for (std::size_t i = 0; i < file_lines.size(); ++i) {
        const std::string& line = file_lines[i];
        if (line.empty() || !fs_ident_start(line[0])) continue;
        std::size_t e = 0;
        while (e < line.size() && fs_ident_char(line[e])) ++e;
        if (fs_lower(std::string_view(line).substr(0, e)) != want) continue;
        const std::string rest = fs_lower(fs_trim(std::string_view(line).substr(e)));
        if ((!rest.empty() && rest[0] == ':') || rest.compare(0, 4, "proc") == 0) {
            start = i;
            break;
        }
    }
    if (start == file_lines.size()) return {};
    std::vector<std::string> body{file_lines[start]};
    for (std::size_t i = start + 1; i < file_lines.size() && body.size() < kMaxAsmBody; ++i) {
        const std::string& line = file_lines[i];
        if (!line.empty() && fs_ident_start(line[0])) {   // a label or a directive at column 0
            std::size_t e = 0;
            while (e < line.size() && fs_ident_char(line[e])) ++e;
            const std::string word = line.substr(0, e);
            const std::string rest = fs_lower(fs_trim(std::string_view(line).substr(e)));
            if (rest.compare(0, 4, "endp") == 0) {
                body.push_back(line);
                break;
            }
            const bool label = !rest.empty() && rest[0] == ':';
            const bool proc = rest.compare(0, 4, "proc") == 0;
            const bool local = word.compare(0, 2, "L_") == 0 || word.compare(0, 2, "l_") == 0;
            if ((label && !local) || proc) break;
        }
        body.push_back(line);
    }
    while (!body.empty() && fs_trim(body.back()).empty()) body.pop_back();
    return body;
}

std::vector<Link> link_by_calls(const std::vector<std::string>& asm_lines, const std::vector<std::string>& c_lines) {
    std::map<std::string, std::vector<int>> calls;   // callee -> the assembly lines that call it, in order
    for (std::size_t i = 0; i < asm_lines.size(); ++i)
        if (const std::string target = fs_call_target(asm_lines[i]); !target.empty()) calls[target].push_back(static_cast<int>(i));
    std::map<std::string, std::size_t> used;
    std::vector<Link> links;
    for (std::size_t c = 0; c < c_lines.size(); ++c) {
        const std::string& line = c_lines[c];
        for (std::size_t i = 0; i < line.size();) {
            if (!fs_ident_start(line[i])) {
                ++i;
                continue;
            }
            const std::size_t b = i;
            while (i < line.size() && fs_ident_char(line[i])) ++i;
            std::size_t after = i;
            while (after < line.size() && line[after] == ' ') ++after;
            if (after >= line.size() || line[after] != '(') continue;
            const std::string word = line.substr(b, i - b);
            const auto it = calls.find(word);
            if (it == calls.end() || used[word] >= it->second.size()) continue;
            const int a = it->second[used[word]++];
            links.push_back({a, a, static_cast<int>(c), static_cast<int>(c)});
            break;   // one link per C line
        }
    }
    return links;
}

const Link* link_at(const std::vector<Link>& links, Side side, int line) {
    for (const Link& l : links) {
        if (side == Side::Asm ? (line >= l.asm_first && line <= l.asm_last) : (line >= l.c_first && line <= l.c_last)) return &l;
    }
    return nullptr;
}

Pair load_pair(const stdfs::path& folder, const convert::FunctionRecord& record) {
    Pair pair;
    // the C side: the record's file, else every C file
    std::vector<stdfs::path> c_files;
    if (!record.source_file.empty()) c_files.push_back(folder / fs::from_text(record.source_file));
    else c_files = fs_files_with(folder, fs_is_c_file);
    for (const stdfs::path& file : c_files) {
        pair.c_lines = c_body(fs_read_lines(file), record.name);
        if (!pair.c_lines.empty()) break;
    }
    if (pair.c_lines.empty()) pair.c_note = "The source of this function is not in the output folder.";
    // the assembly side: the function's label in the folder's .asm files
    for (const stdfs::path& file : fs_files_with(folder, fs_is_asm_file)) {
        pair.asm_lines = asm_body(fs_read_lines(file), record.name);
        if (pair.asm_lines.empty()) {
            if (const std::string by_address = address_of_name(record.name); by_address.empty() && !record.address.empty()) {
                // a function known by its address: the `sub_XXXXX:` label whose number is that address
                const std::vector<std::string> all = fs_read_lines(file);
                const std::string want = normal_address(record.address);
                for (const std::string& line : all) {
                    if (line.compare(0, 4, "sub_") != 0) continue;
                    const std::size_t colon = line.find(':');
                    if (colon == std::string::npos || normal_address(line.substr(4, colon - 4)) != want) continue;
                    pair.asm_lines = asm_body(all, line.substr(0, colon));
                    break;
                }
            }
        }
        if (!pair.asm_lines.empty()) break;
    }
    if (pair.asm_lines.empty()) pair.asm_note = "No assembly for this function in the output folder.";
    pair.links = link_by_calls(pair.asm_lines, pair.c_lines);
    return pair;
}

}  // namespace cb::app::fnsrc
