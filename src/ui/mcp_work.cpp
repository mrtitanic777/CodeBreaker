// ui/mcp_work.cpp -- what the MCP server works out on asking: one function decompiled now, and two builds of a module side by side.
//
// `decompile_function`: a 32-bit x86 function by itself (the decompiler's compilable32 on that one address); any other kind decompiles its whole module
// once (as_c) and the function is cut out of it by its header. `compare` / `diff`: compare/versions matches the functions of two builds; the comparison
// is kept (an hour) so that the pair `diff` shows is the one the listing came from.
#include <algorithm>
#include <chrono>
#include <format>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "compare/versions.hpp"
#include "compare/versions_internal.hpp"
#include "core/error.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "formats/pe.hpp"
#include "ui/mcp_wiring.hpp"

namespace cb::ui::mcp::wiring {
namespace {

namespace cmp = cb::compare::versions;

std::string work_address_text(std::string_view text) {
    std::string a = text::lower(text::trim(text));
    if (a.starts_with("0x")) return a.substr(2);
    if (a.size() > 1 && a.ends_with('h')) {
        while (a.ends_with('h')) a.pop_back();
    }
    return a;
}

// ---- cutting a function out of a module's C ---------------------------------------------------------------------------------

// Does a function header line name the place `addr` is (hex, or segment:offset)? Compared as numbers: sub_00132 is 132, sub_1_0116 is 1:0116,
// func_0x7f021036 is 7f021036.
bool work_same_place(const std::string& addr, const std::string& header) {
    static const std::regex pattern(R"(\b(?:sub|func|FUN)_(?:0x)?([0-9a-fA-F]+)(?:_([0-9a-fA-F]+))?\s*\()");
    std::smatch m;
    if (!std::regex_search(header, m, pattern)) return false;
    try {
        if (const std::size_t colon = addr.find(':'); colon != std::string::npos) {
            if (!m[2].matched) return false;
            return std::stoull(m[1].str(), nullptr, 16) == std::stoull(addr.substr(0, colon), nullptr, 16) &&
                   std::stoull(m[2].str(), nullptr, 16) == std::stoull(addr.substr(colon + 1), nullptr, 16);
        }
        return !m[2].matched && std::stoull(m[1].str(), nullptr, 16) == std::stoull(addr, nullptr, 16);
    } catch (const std::exception&) {
        return false;
    }
}

std::string work_escape_regex(std::string_view s) {
    std::string out;
    for (const char c : s) {
        if (std::string_view("\\^$.|?*+()[]{}").find(c) != std::string_view::npos) out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

// The function in a whole module's C at `addr` (or named `addr`), with the comment lines just above its header.
std::optional<std::string> work_cut_function(const std::string& text, const std::string& addr) {
    const bool by_address = !addr.empty() && std::all_of(addr.begin(), addr.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || c == ':'; });
    std::optional<std::regex> named;
    if (!by_address) named.emplace("\\b" + work_escape_regex(addr) + "\\s*\\(", std::regex::icase);
    std::vector<std::string> lines;
    {
        std::size_t from = 0;
        while (from < text.size()) {
            std::size_t to = text.find_first_of("\r\n", from);
            if (to == std::string::npos) to = text.size();
            lines.push_back(text.substr(from, to - from));
            if (to < text.size() && text[to] == '\r' && to + 1 < text.size() && text[to + 1] == '\n') ++to;
            from = to + 1;
        }
    }
    auto strip = [](std::string s) {
        return std::string(text::trim(s));
    };
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty() || std::isspace(static_cast<unsigned char>(line[0])) || line.find('(') == std::string::npos) continue;
        std::string right = line;
        while (!right.empty() && std::isspace(static_cast<unsigned char>(right.back()))) right.pop_back();
        const bool opens = right.ends_with("{") || (i + 1 < lines.size() && (strip(lines[i + 1]) == "{" || strip(lines[i + 1]).empty()));
        if (!opens) continue;
        const bool match = named ? std::regex_search(line, *named) : work_same_place(addr, line);
        if (!match) continue;
        std::size_t start = i;
        while (start > 0 && (lines[start - 1].starts_with("/*") || lines[start - 1].starts_with(" *") || lines[start - 1].starts_with("//"))) --start;
        std::size_t end = i + 1;
        while (end < lines.size() && lines[end] != "}") ++end;
        std::string out;
        for (std::size_t k = start; k <= end && k < lines.size(); ++k) out += (k > start ? "\n" : "") + lines[k];
        return out;
    }
    return std::nullopt;
}

// ---- the worker -----------------------------------------------------------------------------------------------------------

class Worker {
public:
    Worker(std::shared_ptr<Context> context, std::function<std::optional<std::pair<std::int64_t, std::optional<std::int64_t>>>(const std::filesystem::path&, std::string_view)> locate)
        : context_(std::move(context)), locate_(std::move(locate)) {}

    Json decompile_function(const std::filesystem::path& path, std::string_view addr);
    Json compare(const std::filesystem::path& a, const std::filesystem::path& b);
    Json diff(const std::filesystem::path& a, const std::filesystem::path& b, std::string_view addr);

private:
    std::shared_ptr<const cmp::Comparison> comparison(const std::filesystem::path& a, const std::filesystem::path& b);

    std::shared_ptr<Context> context_;
    std::function<std::optional<std::pair<std::int64_t, std::optional<std::int64_t>>>(const std::filesystem::path&, std::string_view)> locate_;
    std::mutex decompile_lock_, compare_lock_;
    std::map<std::string, std::pair<std::chrono::steady_clock::time_point, std::shared_ptr<const cmp::Comparison>>> compared_;
    std::map<std::string, std::pair<std::chrono::steady_clock::time_point, std::string>> whole_;
};

Json Worker::decompile_function(const std::filesystem::path& path, std::string_view addr_in) {
    const std::string a = work_address_text(addr_in);
    const bool x86 = context_->corpus().lane(path) == base::Lane::Pe && formats::pe::Image::open(path).header().machine == formats::pe::kMachineI386;
    std::lock_guard hold(decompile_lock_);
    decompile::decompiler::Decompiler& decompiler = context_->kit().decompiler();
    Json out = Json::object();
    if (x86) {
        if (!locate_) throw UsageError("addresses are not available in this server");
        const std::int64_t va = locate_(path, addr_in)->first;
        const std::vector<decompile::decompiler::Va> starts{va};
        const decompile::c_text::Compilable32 made = decompiler.compilable32(path, starts);
        const auto name = made.by_va.find(va);
        if (name == made.by_va.end() || !made.texts.contains(name->second)) return Json{{"error", std::format("no function at {:x}", va)}};
        out["name"] = name->second;
        out["text"] = made.texts.at(name->second);
        out["addr"] = std::format("{:x}", va);
        return out;
    }
    const std::string key = fs::to_text(path);
    auto hit = whole_.find(key);
    if (hit == whole_.end() || std::chrono::steady_clock::now() - hit->second.first > std::chrono::hours{1}) {
        hit = whole_.insert_or_assign(key, std::pair{std::chrono::steady_clock::now(), decompiler.as_c(path).text}).first;
    }
    const std::optional<std::string> cut = work_cut_function(hit->second.second, a);
    if (!cut) return Json{{"error", std::format("no function header naming {} in the module's C; run_command decompile writes all of it", addr_in)}};   // (the tool says it as it is)
    out["text"] = *cut;
    return out;
}

std::shared_ptr<const cmp::Comparison> Worker::comparison(const std::filesystem::path& a, const std::filesystem::path& b) {
    const std::string key = fs::to_text(a) + "\x01" + fs::to_text(b);
    {
        std::lock_guard hold(compare_lock_);
        const auto hit = compared_.find(key);
        if (hit != compared_.end() && std::chrono::steady_clock::now() - hit->second.first < std::chrono::hours{1}) return hit->second.second;
    }
    auto made = std::make_shared<const cmp::Comparison>(cmp::compare(a, b));
    std::lock_guard hold(compare_lock_);
    compared_[key] = {std::chrono::steady_clock::now(), made};
    return made;
}

Json Worker::compare(const std::filesystem::path& a, const std::filesystem::path& b) {
    const std::shared_ptr<const cmp::Comparison> c = comparison(a, b);
    const Json whole = Json::parse(c->json(false, nullptr));
    Json out = Json::object();
    out["report"] = c->report();
    out["counts"] = whole.at("counts");
    out["code"] = whole.at("code");
    out["pairs"] = whole.at("pairs");
    Json added = Json::array(), removed = Json::array();
    for (const Json& x : whole.at("added")) added.push_back(x.at("key"));
    for (const Json& x : whole.at("removed")) removed.push_back(x.at("key"));
    out["added"] = std::move(added);
    out["removed"] = std::move(removed);
    out["names"] = whole.at("names");
    return out;
}

// One function in two builds, instruction by instruction: `addr` is its address (or name) in the first build, or in the second. The listings are lined
// up by the kinds of instruction and operand, each line marked ' ' the same (addresses aside), '~' the same instruction with other registers or
// constants, '-' only in the first, '+' only in the second.
Json Worker::diff(const std::filesystem::path& pa, const std::filesystem::path& pb, std::string_view addr_in) {
    const std::shared_ptr<const cmp::Comparison> c = comparison(pa, pb);
    const std::string want = work_address_text(addr_in), low = text::lower(text::trim(addr_in));
    const cmp::Side& a = c->from();
    const cmp::Side& b = c->to();
    const cmp::Pair* pair = nullptr;
    std::size_t index = 0;
    for (std::size_t i = 0; i < c->pairs().size() && pair == nullptr; ++i)
        if (c->pairs()[i].from == want || c->pairs()[i].to == want) pair = &c->pairs()[index = i];
    for (std::size_t i = 0; i < c->pairs().size() && pair == nullptr; ++i)
        if (text::lower(c->pairs()[i].from_name) == low || text::lower(c->pairs()[i].to_name) == low) pair = &c->pairs()[index = i];
    auto listing = [](const cmp::Function& f) {
        std::string out;
        for (std::size_t i = 0; i < f.rows.size(); ++i) out += std::format("{}{:x}  {}", i ? "\n" : "", f.rows[i].address, f.rows[i].text);
        return out;
    };
    if (pair == nullptr) {
        for (const auto& [side, keys, what] : {std::tuple<const cmp::Side&, const std::vector<std::size_t>&, const char*>{a, c->removed(), "removed: only in the first build"},
                                               std::tuple<const cmp::Side&, const std::vector<std::size_t>&, const char*>{b, c->added(), "added: only in the second build"}}) {
            for (const std::size_t k : keys) {
                const cmp::Function& f = side.function(k);
                if (f.key != want && text::lower(f.name) != low) continue;
                Json out = Json::object();
                out["status"] = what;
                out["addr"] = f.key;
                out["name"] = f.name;
                out["listing"] = listing(f);
                return out;
            }
        }
        throw UsageError(std::format("no function '{}' in either build (compare_versions lists the pairs)", addr_in));
    }
    const cmp::Function* fa = a.find(pair->from);
    const cmp::Function* fb = b.find(pair->to);
    if (fa == nullptr || fb == nullptr) throw UsageError("the pair names a function its build does not have");
    Json out = Json::object();
    out["from"] = pair->from;
    out["to"] = pair->to;
    out["from_name"] = pair->from_name;
    out["to_name"] = pair->to_name;
    out["status"] = pair->identical ? "identical" : "changed";
    out["level"] = pair->level ? std::string(cmp::level_name(*pair->level)) : std::string();
    out["score"] = pair->score;
    out["sizes"] = Json::array({pair->size_from, pair->size_to});
    cmp::detail::Interner interner;
    const std::vector<int> sa = interner.all(fa->shape), sb = interner.all(fb->shape);
    cmp::detail::SeqMatcher matcher(sa, sb);
    std::vector<std::string> lines;
    for (const auto& op : matcher.opcodes()) {
        if (op.tag == cmp::detail::SeqMatcher::Tag::Equal) {
            for (int i = op.i1, j = op.j1; i < op.i2; ++i, ++j) {
                const std::size_t x = static_cast<std::size_t>(i), y = static_cast<std::size_t>(j);
                lines.push_back(std::format("{} {:x} {:<36} {:x} {}", fa->text[x] == fb->text[y] ? " " : "~", fa->rows[x].address, fa->rows[x].text, fb->rows[y].address, fb->rows[y].text));
            }
            continue;
        }
        for (int i = op.i1; i < op.i2; ++i) lines.push_back(std::format("- {:x} {}", fa->rows[static_cast<std::size_t>(i)].address, fa->rows[static_cast<std::size_t>(i)].text));
        for (int j = op.j1; j < op.j2; ++j) lines.push_back(std::format("+ {:47} {:x} {}", "", fb->rows[static_cast<std::size_t>(j)].address, fb->rows[static_cast<std::size_t>(j)].text));
    }
    std::string joined;
    std::int64_t differ = 0;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        joined += (i ? "\n" : "") + lines[i];
        if (lines[i][0] != ' ') ++differ;
    }
    out["diff"] = joined;
    out["differ"] = differ;
    (void)index;
    return out;
}

}  // namespace

void fill_work(Services& services, const std::shared_ptr<Context>& context) {
    auto worker = std::make_shared<Worker>(context, services.locate);
    services.decompile_function = [worker](const std::filesystem::path& p, std::string_view addr) { return worker->decompile_function(p, addr); };
    services.compare = [worker](const std::filesystem::path& a, const std::filesystem::path& b) { return worker->compare(a, b); };
    services.diff = [worker](const std::filesystem::path& a, const std::filesystem::path& b, std::string_view addr) { return worker->diff(a, b, addr); };
}

}  // namespace cb::ui::mcp::wiring
