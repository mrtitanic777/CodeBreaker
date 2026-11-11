#include "cb/source/assembly.hpp"

#include <array>
#include <format>
#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::source::assembly {
namespace stdfs = std::filesystem;
namespace {

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
char upper(char c) { return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 'a' + 'A') : c; }

constexpr std::array<std::pair<std::string_view, std::string_view>, 9> kSpelling = {{
    {"pushal", "pushad"}, {"popal", "popad"}, {"int3", "int 3"}, {"retn", "ret"}, {"fwait", "wait"},
    {"pushaw", "pusha"}, {"popaw", "popa"}, {"retfw", "retf"}, {"iretw", "iret"},
}};

// A tool run as the writers use one: the exit code and everything the tool said, or a reason it could not run.
struct Ran {
    bool ok = false;
    std::string log;
};

Ran run_tool(const Tools& tools, std::vector<std::string> argv, const stdfs::path& work) {
    try {
        const process::ProcessResult r = process::run({.argv = std::move(argv), .cwd = work, .env = tools.env, .err = process::Capture::Merge, .timeout = tools.timeout});
        if (r.timed_out) return {false, std::format("timed out after {} s", std::chrono::duration_cast<std::chrono::seconds>(tools.timeout).count())};
        return {r.exit_code == 0, std::string(text::trim(text::from_latin1(r.out)))};
    } catch (const ToolError& e) {   // the program could not be started: the message says why
        return {false, e.what()};
    }
}

}  // namespace

Made assemble(const Tools& tools, const stdfs::path& source, const stdfs::path& work) {
    if (tools.wasm.empty()) return {std::nullopt, "no assembler (openwatcom is not configured)"};
    const stdfs::path dir = stdfs::absolute(work);   // the assembler runs with this as its working folder
    const stdfs::path obj = dir / (source.stem().string() + ".obj");
    std::error_code ec;
    stdfs::remove(obj, ec);
    const Ran r = run_tool(tools, {fs::to_text(tools.wasm), "-zq", "-fo=" + fs::to_text(obj), fs::to_text(stdfs::absolute(source))}, dir);
    return {r.ok && stdfs::is_regular_file(obj, ec) ? std::optional<stdfs::path>(obj) : std::nullopt, r.log};
}

Made link(const Tools& tools, std::span<const std::string> lines, const stdfs::path& out, const stdfs::path& work) {
    if (tools.wlink.empty()) return {std::nullopt, "no linker (openwatcom is not configured)"};
    const stdfs::path dir = stdfs::absolute(work), target = stdfs::absolute(out);
    stdfs::path script = target;
    script.replace_extension(".lnk");
    std::string body;
    for (const std::string& line : lines) body += line + "\r\n";
    body += "name " + fs::to_text(target) + "\r\n";
    fs::write_text(script, body);
    std::error_code ec;
    stdfs::remove(target, ec);
    const Ran r = run_tool(tools, {fs::to_text(tools.wlink), "@" + fs::to_text(script)}, dir);
    return {r.ok && stdfs::is_regular_file(target, ec) ? std::optional<stdfs::path>(target) : std::nullopt, r.log};
}

std::string operands(std::string_view text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        const bool starts = text[i] == '0' && i + 1 < text.size() && text[i + 1] == 'x' && (i == 0 || !is_word(text[i - 1]));
        std::size_t end = i + 2;
        if (starts) {
            while (end < text.size() && is_lower_hex(text[end])) ++end;
        }
        if (starts && end > i + 2 && (end == text.size() || !is_word(text[end]))) {
            out += '0';
            for (std::size_t k = i + 2; k < end; ++k) out += upper(text[k]);
            out += 'h';
            i = end;
        } else {
            out += text[i++];
        }
    }
    return out;
}

std::string_view mnemonic(std::string_view name) {
    for (const auto& [from, to] : kSpelling) {
        if (name == from) return to;
    }
    return name;
}

std::optional<StringOp> string_op(std::string_view text) {
    static constexpr std::string_view kPrefixes[] = {"rep ", "repe ", "repne "};
    static constexpr std::string_view kOps[] = {"movs", "cmps", "stos", "lods", "scas", "ins", "outs"};
    // the prefix is optional, and is tried first as the longest that fits, as `rep ` does not begin `repe `
    for (const std::string_view prefix : {kPrefixes[0], kPrefixes[1], kPrefixes[2], std::string_view()}) {
        if (!text.starts_with(prefix)) continue;
        const std::string_view rest = text.substr(prefix.size());
        for (const std::string_view op : kOps) {
            if (!rest.starts_with(op) || rest.size() <= op.size()) continue;
            const char width = rest[op.size()];
            if ((width != 'b' && width != 'w' && width != 'd') || (rest.size() > op.size() + 1 && is_word(rest[op.size() + 1]))) continue;
            return StringOp{prefix, op, width};
        }
    }
    return std::nullopt;
}

std::vector<std::string> data_lines(ByteView data, std::size_t per_line) {
    std::vector<std::string> out;
    const auto plain = [&](ByteView chunk) {
        for (std::size_t k = 0; k < chunk.size(); k += per_line) {
            std::string line = "        db   ";
            const ByteView part = chunk.subspan(k, std::min(per_line, chunk.size() - k));
            for (std::size_t i = 0; i < part.size(); ++i) line += std::format("{}0{:02X}h", i ? ", " : "", part[i]);
            out.push_back(std::move(line));
        }
    };
    const auto printable = [](std::uint8_t b) { return b >= 0x20 && b <= 0x7E && b != 0x27; };   // everything but the quote
    std::size_t at = 0, i = 0;
    while (i < data.size()) {
        if (!printable(data[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < data.size() && printable(data[end])) ++end;
        if (end - i < 5) {
            i = end;
            continue;
        }
        plain(data.subspan(at, i - at));
        const std::string_view run = as_text(data.subspan(i, end - i));
        for (std::size_t k = 0; k < run.size(); k += 60) out.push_back("        db   '" + std::string(run.substr(k, 60)) + "'");
        at = i = end;
    }
    plain(data.subspan(at));
    return out;
}

namespace {

// What the file of known probes calls one: the mode, the text, and the two byte strings of the candidate.
std::string probe_key(const Candidate& c, Mode mode) {
    return std::format("{}|{}|{}|{}", mode == Mode::Flat32 ? "flat32" : "real16", c.text, crypto::to_hex(c.original), crypto::to_hex(c.mask));
}

// The line numbers of the assembler's errors: "file.asm(12): Error! E032 ...".
std::set<int> error_lines(std::string_view messages) {
    std::set<int> lines;
    for (std::size_t i = messages.find('('); i != std::string_view::npos; i = messages.find('(', i + 1)) {
        std::size_t j = i + 1;
        while (j < messages.size() && messages[j] >= '0' && messages[j] <= '9') ++j;
        if (j == i + 1 || j >= messages.size() || messages[j] != ')') continue;
        std::size_t k = j + 1;
        while (k < messages.size() && text::is_space(messages[k])) ++k;
        if (k >= messages.size() || messages[k] != ':') continue;
        ++k;
        while (k < messages.size() && text::is_space(messages[k])) ++k;
        if (!messages.substr(k).starts_with("Error")) continue;
        if (const auto n = text::parse_int<int>(messages.substr(i + 1, j - i - 1))) lines.insert(*n);
    }
    return lines;
}

}  // namespace

std::map<std::string, bool> Prober::known() const {
    std::map<std::string, bool> out;
    try {
        const Json doc = json::load(known_file_);
        if (!doc.is_object()) return out;
        for (auto it = doc.begin(); it != doc.end(); ++it) out[it.key()] = it.value().is_boolean() ? it.value().get<bool>() : !it.value().is_null();
    } catch (const IoError&) {
        out.clear();   // no file yet
    } catch (const ParseError&) {
        out.clear();   // a damaged file knows nothing; it is written afresh
    }
    return out;
}

std::set<Candidate> Prober::probe(std::span<const Candidate> candidates, const std::set<std::string>& labels, const std::set<std::string>& near,
                                  const stdfs::path& outdir, Mode mode) {
    const std::lock_guard<std::mutex> one_at_a_time(mutex_);   // the probes share the file of what is known
    std::map<std::string, bool> known = this->known();
    const std::set<Candidate> wanted(candidates.begin(), candidates.end());
    std::vector<Candidate> todo;
    for (const Candidate& c : wanted) {
        if (!known.contains(probe_key(c, mode))) todo.push_back(c);
    }
    std::set<Candidate> refused;
    const std::string use = mode == Mode::Flat32 ? "use32" : "use16";
    for (int attempt = 0; attempt < 16 && !todo.empty(); ++attempt) {
        std::vector<std::string> lines = mode == Mode::Flat32 ? std::vector<std::string>{".386p", ".model flat"} : std::vector<std::string>{".386p"};
        lines.push_back("PD segment para public " + use + " 'PXCODE'");
        for (const std::string& n : labels) lines.push_back(n + " label " + (near.contains(n) ? "near" : "byte"));
        lines.push_back("PD ends");
        lines.push_back("PC segment para public " + use + " 'PCODE'");
        if (mode == Mode::Real16) lines.push_back("        assume cs:PC, ds:nothing, es:nothing, ss:nothing");
        std::map<int, const Candidate*> line_of;   // the line of the source each candidate's instruction is on
        for (std::size_t n = 0; n < todo.size(); ++n) {
            lines.push_back("        org  " + std::to_string(32 * n));
            line_of[static_cast<int>(lines.size()) + 1] = &todo[n];
            lines.push_back("        " + (refused.contains(todo[n]) ? std::string("nop") : todo[n].text));
        }
        lines.push_back("PC ends");
        lines.push_back("end");

        const stdfs::path src = outdir / "probe.asm";
        std::string body;
        for (const std::string& line : lines) body += line + "\r\n";
        fs::write_file(src, text::to_latin1(body));
        const Made made = assemble(tools_, src, outdir);
        if (made.file) {
            const std::map<std::uint32_t, Bytes> got = last_segment_data(fs::read_file(*made.file));
            for (std::size_t n = 0; n < todo.size(); ++n) {
                const Candidate& c = todo[n];
                const auto at = got.find(static_cast<std::uint32_t>(32 * n));
                const ByteView out = at == got.end() ? ByteView() : ByteView(at->second);
                Bytes outside;   // the bytes the instruction has outside its patched fields
                for (std::size_t i = 0; i < out.size() && i < c.mask.size(); ++i) {
                    if (c.mask[i] == 0) outside.push_back(out[i]);
                }
                known[probe_key(c, mode)] = !refused.contains(c) && out.size() == c.mask.size() && outside == c.original;
            }
            todo.clear();
            break;
        }
        std::set<Candidate> failed;
        for (const int line : error_lines(made.log)) {
            if (const auto it = line_of.find(line); it != line_of.end()) failed.insert(*it->second);
        }
        if (failed.empty()) {   // the probe file itself is refused: nothing is known good
            for (const Candidate& c : todo) known[probe_key(c, mode)] = false;
            todo.clear();
            break;
        }
        refused.insert(failed.begin(), failed.end());
    }
    Json doc = Json::object();
    for (const auto& [key, value] : known) doc[key] = value;   // sorted, so the file is the same whenever it is written
    fs::make_dirs(known_file_.parent_path());
    json::save(known_file_, doc, json::Style::PythonCompatible);

    std::set<Candidate> good;
    for (const Candidate& c : wanted) {
        if (const auto it = known.find(probe_key(c, mode)); it != known.end() && it->second) good.insert(c);
    }
    return good;
}

std::map<std::uint32_t, Bytes> last_segment_data(ByteView object) {
    std::map<std::size_t, std::map<std::uint32_t, Bytes>> chunks;   // segment index -> offset -> data
    std::size_t i = 0, segments = 0;
    const std::size_t n = object.size();
    while (i + 3 <= n) {
        const std::uint8_t kind = object[i];
        const std::size_t end = i + 3 + (object[i + 1] | (static_cast<std::size_t>(object[i + 2]) << 8));
        if (end > n) break;
        if (kind == 0x98 || kind == 0x99) {
            ++segments;
        } else if (kind == 0xA0 || kind == 0xA1) {   // LEDATA: segment index, offset, data, checksum
            const std::size_t width = kind == 0xA1 ? 4 : 2;
            const std::size_t q = i + 4;
            if (q + width + 1 > end) throw FormatError("omf", i, "a data record too short to hold its offset");
            std::uint32_t off = 0;
            for (std::size_t b = 0; b < width; ++b) off |= static_cast<std::uint32_t>(object[q + b]) << (8 * b);
            chunks[object[i + 3]][off] = Bytes(object.begin() + static_cast<std::ptrdiff_t>(q + width), object.begin() + static_cast<std::ptrdiff_t>(end - 1));
        }
        i = end;
    }
    std::map<std::uint32_t, Bytes> out;
    const auto last = chunks.find(segments);
    if (last == chunks.end()) return out;
    std::optional<std::uint32_t> open;   // the record being extended
    for (const auto& [off, data] : last->second) {
        if (open && *open + out[*open].size() == off) {
            Bytes& joined = out[*open];
            joined.insert(joined.end(), data.begin(), data.end());
        } else {
            out[off] = data;
            open = off;
        }
    }
    return out;
}

}  // namespace cb::source::assembly
