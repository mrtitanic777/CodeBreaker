// patch.cpp -- applying a patch file to a source folder (see java/projects/minecraft/prep/patch.hpp).
//
// Ported from DiffPatch (codechicken.diffpatch: PatchOperation, patch.Patcher, util.PatchFile / Patch / Diff / LineRange / CharRepresenter /
// FileCollector / Utils) as RetroMCP's TaskApplyPatch uses it, MIT License, (c) 2018 covers1624, Chicken-Bones.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/patch.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cwctype>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <regex>
#include <set>
#include <stdexcept>
#include <unordered_map>

namespace cb::java::minecraft::prep {

namespace fs = std::filesystem;

namespace {

const char* const DEV_NULL = "/dev/null";

// ---- Java text: UTF-8 files read as Files.readAllLines does, and the UTF-16 units DiffPatch's character work is written on ----

// Java's strict UTF-8 decoder: false where Files.readAllLines would throw MalformedInputException.
bool valid_utf8(const std::string& s) {
    const std::size_t n = s.size();
    std::size_t i = 0;
    while (i < n) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80) { ++i; continue; }
        int len;
        std::uint32_t cp;
        if (c >= 0xC2 && c <= 0xDF) { len = 2; cp = c & 0x1F; }
        else if (c >= 0xE0 && c <= 0xEF) { len = 3; cp = c & 0x0F; }
        else if (c >= 0xF0 && c <= 0xF4) { len = 4; cp = c & 0x07; }
        else return false;
        if (i + len > n) return false;
        for (int k = 1; k < len; ++k) {
            const unsigned char cc = static_cast<unsigned char>(s[i + k]);
            if ((cc & 0xC0) != 0x80) return false;
            cp = (cp << 6) | (cc & 0x3F);
        }
        if (len == 3 && (cp < 0x800 || (cp >= 0xD800 && cp <= 0xDFFF))) return false;
        if (len == 4 && (cp < 0x10000 || cp > 0x10FFFF)) return false;
        i += len;
    }
    return true;
}

std::u16string to_u16(const std::string& s) {
    std::u16string out;
    out.reserve(s.size());
    const std::size_t n = s.size();
    for (std::size_t i = 0; i < n;) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80) { out.push_back(c); ++i; continue; }
        int len = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : 2;
        std::uint32_t cp = c & (len == 4 ? 0x07 : len == 3 ? 0x0F : 0x1F);
        for (int k = 1; k < len && i + k < n; ++k) cp = (cp << 6) | (static_cast<unsigned char>(s[i + k]) & 0x3F);
        i += len;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            out.push_back(static_cast<char16_t>(0xD800 + (cp >> 10)));
            out.push_back(static_cast<char16_t>(0xDC00 + (cp & 0x3FF)));
        } else {
            out.push_back(static_cast<char16_t>(cp));
        }
    }
    return out;
}

// Character.isLetter / isDigit on a UTF-16 unit (ASCII exactly; elsewhere the C library's classification of the unit).
bool is_letter(char16_t c) { return c < 0x80 ? ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) : std::iswalpha(static_cast<wint_t>(c)) != 0; }
bool is_digit(char16_t c) { return c < 0x80 ? (c >= '0' && c <= '9') : std::iswdigit(static_cast<wint_t>(c)) != 0 || (c >= 0x660 && c <= 0x669); }
bool is_letter_or_digit(char16_t c) { return is_letter(c) || is_digit(c); }

// Files.readAllLines: the lines of the file (\n, \r\n or \r end one; a last line without an end is a line; "a\n" is one line). nullopt where the
// file cannot be read or is not UTF-8 (an IOException in Java, which DiffPatch catches per file).
std::optional<std::vector<std::string>> read_all_lines(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    const std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (!valid_utf8(bytes)) return std::nullopt;
    std::vector<std::string> lines;
    std::size_t start = 0;
    const std::size_t n = bytes.size();
    for (std::size_t i = 0; i < n; ++i) {
        if (bytes[i] == '\n' || bytes[i] == '\r') {
            lines.emplace_back(bytes, start, i - start);
            if (bytes[i] == '\r' && i + 1 < n && bytes[i + 1] == '\n') ++i;
            start = i + 1;
        }
    }
    if (start < n) lines.emplace_back(bytes, start, n - start);
    return lines;
}

std::optional<std::string> read_all_bytes(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

std::string join(const std::vector<std::string>& lines, const std::string& sep) {
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i) out += sep;
        out += lines[i];
    }
    return out;
}

// String.indexOf(str, from) / lastIndexOf(str, from) on UTF-16 text, with Java's clamping.
int java_index_of(const std::u16string& text, const std::u16string& str, int from) {
    const int len = static_cast<int>(text.size());
    const int slen = static_cast<int>(str.size());
    if (from >= len) return slen == 0 ? len : -1;
    if (from < 0) from = 0;
    if (slen == 0) return from;
    const std::size_t at = text.find(str, static_cast<std::size_t>(from));
    return at == std::u16string::npos ? -1 : static_cast<int>(at);
}

int java_last_index_of(const std::u16string& text, const std::u16string& str, int from) {
    const int len = static_cast<int>(text.size());
    const int slen = static_cast<int>(str.size());
    const int right = len - slen;
    if (from < 0) return -1;
    if (from > right) from = right;
    if (slen == 0) return from;
    if (from < 0) return -1;
    const std::size_t at = text.rfind(str, static_cast<std::size_t>(from));
    return at == std::u16string::npos ? -1 : static_cast<int>(at);
}

// String.format("%.2f", v): Java rounds the shortest decimal digits of the double half up.
std::string format_2f(double v) {
    if (std::isnan(v)) return "NaN";
    if (std::isinf(v)) return v < 0 ? "-Infinity" : "Infinity";
    char buf[512];
    const auto r = std::to_chars(buf, buf + sizeof buf, std::fabs(v), std::chars_format::fixed);
    std::string s(buf, r.ptr);
    const std::size_t dot = s.find('.');
    std::string ip = dot == std::string::npos ? s : s.substr(0, dot);
    std::string fp = dot == std::string::npos ? "" : s.substr(dot + 1);
    while (fp.size() < 3) fp.push_back('0');
    const bool up = fp[2] >= '5';
    fp.resize(2);
    if (up) {
        std::string digits = ip + fp;
        int i = static_cast<int>(digits.size()) - 1;
        while (i >= 0) {
            if (digits[i] == '9') { digits[i] = '0'; --i; }
            else { ++digits[i]; break; }
        }
        if (i < 0) digits.insert(digits.begin(), '1');
        ip = digits.substr(0, digits.size() - 2);
        fp = digits.substr(digits.size() - 2);
    }
    const bool neg = v < 0 && (ip != "0" || fp != "00");
    return (neg ? "-" : "") + ip + "." + fp;
}

// ---- util: Operation, Diff, LineRange, Patch ----

enum class Operation { DELETE, INSERT, EQUAL };

const char* operation_prefix(Operation op) { return op == Operation::DELETE ? "-" : op == Operation::INSERT ? "+" : " "; }

struct Diff {
    Operation op;
    std::string text;
    std::string to_string() const { return operation_prefix(op) + text; }
};

struct LineRange {
    int start = 0;
    int end = 0;

    LineRange() = default;
    LineRange(int s, int e) : start(s), end(e) {}
    static LineRange from_start_len(int s, int len) { return LineRange(s, s + len); }

    bool contains(int i) const { return start <= i && i < end; }
    bool contains(const LineRange& r) const { return r.start >= start && r.end <= end; }
    int get_length() const { return end - start; }
};

// A hunk.
struct Patch {
    std::vector<Diff> diffs;
    int start1 = 0;
    int start2 = 0;
    int length1 = 0;
    int length2 = 0;

    LineRange trim_range(const LineRange& range) const {
        int start = 0;
        const int size = static_cast<int>(diffs.size());
        while (start < size && diffs[start].op == Operation::EQUAL) ++start;
        if (start == size) return LineRange::from_start_len(range.start, 0);
        int end = size;
        while (end > start && diffs[end - 1].op == Operation::EQUAL) --end;
        return LineRange(range.start + start, range.end - (size - end));
    }

    void recalculate_length() {
        length1 = static_cast<int>(diffs.size());
        length2 = static_cast<int>(diffs.size());
        for (const Diff& d : diffs) {
            if (d.op == Operation::DELETE) --length2;
            else if (d.op == Operation::INSERT) --length1;
        }
    }

    std::string get_header() const {
        return "@@ -" + std::to_string(start1 + 1) + "," + std::to_string(length1) + " +" + std::to_string(start2 + 1) + "," + std::to_string(length2) + " @@";
    }
    std::vector<std::string> get_context_lines() const {
        std::vector<std::string> out;
        for (const Diff& d : diffs) if (d.op != Operation::INSERT) out.push_back(d.text);
        return out;
    }
    std::vector<std::string> get_patched_lines() const {
        std::vector<std::string> out;
        for (const Diff& d : diffs) if (d.op != Operation::DELETE) out.push_back(d.text);
        return out;
    }
    LineRange get_range2() const { return LineRange::from_start_len(start2, length2); }
    LineRange get_trimmed_range2() const { return trim_range(get_range2()); }
};

// ---- util.PatchFile: one file's hunks, read from a patch file ----

struct PatchFile {
    std::string name;
    std::string base_path;
    std::string patched_path;
    bool no_new_line = false;
    bool has_patched_path = false;
    std::vector<Patch> patches;

    static std::string remove_start(const std::string& s, const std::string& prefix) {
        return !prefix.empty() && s.compare(0, prefix.size(), prefix) == 0 ? s.substr(prefix.size()) : s;
    }
    std::string get_base_path(const std::string& prefix) const { return remove_start(base_path, prefix); }
    std::string get_patched_path(const std::string& prefix) const { return remove_start(patched_path, prefix); }

    // PatchFile.fromLines (a patch file of many files; `--- ` begins one).
    static std::vector<PatchFile> from_lines(const std::string& name, const std::vector<std::string>& lines, bool verify_headers) {
        static const std::regex HUNK_OFFSET(R"(@@ -(\d+),(\d+) \+([_\d]+),(\d+) @@)");
        std::vector<PatchFile> list;
        int delta = 0;
        int i = 0;
        std::optional<PatchFile> patch_file;
        Patch* patch = nullptr;
        for (const std::string& line : lines) {
            ++i;
            if (line.empty()) continue;  // blank lines are ignored

            if (line.compare(0, 4, "--- ") == 0) {
                if (patch_file) list.push_back(std::move(*patch_file));
                patch_file.emplace();
                patch = nullptr;
                delta = 0;
                patch_file->name = name;
                patch_file->base_path = line.substr(4);
                continue;
            }
            if (line.compare(0, 4, "+++ ") == 0) {
                if (!patch_file) throw std::runtime_error("NullPointerException: '+++' line before any '---' in '" + name + "' at " + std::to_string(i));
                patch_file->patched_path = line.substr(4);
                patch_file->has_patched_path = true;
                continue;
            }
            if (!patch_file) throw std::runtime_error("NullPointerException: patch line before any '---' in '" + name + "' at " + std::to_string(i));

            switch (line[0]) {
                case '@': {
                    std::smatch m;
                    if (!std::regex_search(line, m, HUNK_OFFSET))
                        throw std::runtime_error("Invalid patch line in '" + name + "' at " + std::to_string(i) + ":'" + line + "'");
                    patch_file->patches.emplace_back();
                    patch = &patch_file->patches.back();
                    patch->start1 = std::stoi(m[1].str()) - 1;
                    patch->length1 = std::stoi(m[2].str());
                    patch->length2 = std::stoi(m[4].str());
                    const std::string start2 = m[3].str();
                    if (start2 == "_") {
                        patch->start2 = patch->start1 + delta;
                    } else {
                        patch->start2 = std::stoi(start2) - 1;
                        if (verify_headers && patch->start2 != patch->start1 + delta)
                            throw std::runtime_error("Applied Offset Mismatch in '" + name + "' at " + std::to_string(i) + ". Expected: " +
                                                     std::to_string(patch->start1 + delta + 1) + ", Actual: " + std::to_string(patch->start2 + 1));
                    }
                    delta += patch->length2 - patch->length1;
                    break;
                }
                case ' ':
                case '+':
                case '-': {
                    if (!patch) throw std::runtime_error("NullPointerException: hunk line before any hunk header in '" + name + "' at " + std::to_string(i));
                    const Operation op = line[0] == ' ' ? Operation::EQUAL : line[0] == '+' ? Operation::INSERT : Operation::DELETE;
                    patch->diffs.push_back({op, line.substr(1)});
                    break;
                }
                case '\\':
                    if (line == "\\ No newline at end of file") {
                        patch_file->no_new_line = true;
                        break;
                    }
                    [[fallthrough]];
                default:
                    throw std::runtime_error("Invalid patch line in '" + line + "' at " + std::to_string(i) + ":'" + line + "'");
            }
        }
        if (patch_file) list.push_back(std::move(*patch_file));
        return list;
    }
};

// ---- util.CharRepresenter: equal lines (and equal words) as equal single characters ----

class CharRepresenter {
public:
    CharRepresenter() {
        char_to_line_.push_back(std::u16string(1, u'\0'));  // lets avoid the 0 char
        for (char16_t i = 0; i < 0x80; ++i) char_to_word_.push_back(std::u16string(1, i));  // ascii chars keep their own values
    }

    const std::u16string& get_word_for_char(char16_t ch) const {
        if (ch >= char_to_word_.size()) throw std::runtime_error("ArrayIndexOutOfBoundsException: word char " + std::to_string(ch));
        return char_to_word_[ch];
    }

    char16_t add_line(const std::u16string& line) {
        const auto it = line_to_char_.find(line);
        if (it != line_to_char_.end()) return it->second;
        char_to_line_.push_back(line);
        const char16_t c = static_cast<char16_t>(char_to_line_.size() - 1);
        line_to_char_.emplace(line, c);
        return c;
    }

    char16_t add_word(const std::u16string& word) {
        if (word.size() == 1 && word[0] <= 0x80) return word[0];
        const auto it = word_to_char_.find(word);
        if (it != word_to_char_.end()) return it->second;
        char_to_word_.push_back(word);
        const char16_t c = static_cast<char16_t>(char_to_word_.size() - 1);
        word_to_char_.emplace(word, c);
        return c;
    }

    std::u16string words_to_chars(const std::string& utf8_line) {
        const std::u16string line = to_u16(utf8_line);
        std::u16string buf;
        const std::size_t n = line.size();
        for (std::size_t i = 0, len; i < n; i += len) {
            const char16_t c = line[i];
            len = 1;  // identify word
            if (is_letter(c)) {
                while (i + len < n && is_letter_or_digit(line[i + len])) ++len;
            } else if (is_digit(c)) {
                while (i + len < n && is_digit(line[i + len])) ++len;
            } else if (c == u' ' || c == u'\t') {
                while (i + len < n && line[i + len] == c) ++len;
            }
            buf.push_back(add_word(line.substr(i, len)));
        }
        return buf;
    }

    std::u16string lines_to_chars(const std::vector<std::string>& lines) {
        std::u16string buf;
        buf.reserve(lines.size());
        for (const std::string& l : lines) buf.push_back(add_line(to_u16(l)));
        return buf;
    }

    int get_max_word_char() const { return static_cast<int>(char_to_word_.size()); }

private:
    std::vector<std::u16string> char_to_line_;
    std::unordered_map<std::u16string, char16_t> line_to_char_;
    std::vector<std::u16string> char_to_word_;
    std::unordered_map<std::u16string, char16_t> word_to_char_;
};

// ---- patch.Patcher: applying one file's hunks to its lines ----

// the offset distance which constitutes a warning for a patch: 10% of file length, or 10x patch length, whichever is longer
int offset_warn_distance(int patch_length, int file_length) { return std::max(patch_length * 10, file_length / 10); }

struct Result;

// A hunk extended with what the matching needs (Patcher.WorkingPatch).
struct WorkingPatch : Patch, std::enable_shared_from_this<WorkingPatch> {
    std::shared_ptr<Result> result;
    std::u16string lm_context, lm_patched;
    std::vector<std::u16string> wm_context, wm_patched;

    explicit WorkingPatch(const Patch& other) : Patch(other) {}

    void fail();
    void succeed(PatchMode mode, std::shared_ptr<Patch> applied);
    void add_offset_result(int offset, int file_length);

    void lines_to_chars(CharRepresenter& rep) {
        lm_context = rep.lines_to_chars(get_context_lines());
        lm_patched = rep.lines_to_chars(get_patched_lines());
    }
    void words_to_chars(CharRepresenter& rep) {
        wm_context.clear();
        wm_patched.clear();
        for (const std::string& l : get_context_lines()) wm_context.push_back(rep.words_to_chars(l));
        for (const std::string& l : get_patched_lines()) wm_patched.push_back(rep.words_to_chars(l));
    }

    std::optional<LineRange> get_keepout_range2() const;
    std::optional<int> get_applied_delta() const;
};

struct Result {
    std::shared_ptr<Patch> patch;
    bool success = false;
    std::optional<PatchMode> mode;
    int search_offset = 0;
    std::shared_ptr<Patch> applied_patch;
    int offset = 0;
    bool offset_warning = false;
    float fuzzy_quality = 0;

    std::string summary() const {
        if (!success) return "FAILURE: " + patch->get_header();
        if (mode == PatchMode::ACCESS) return "ACCESS: " + patch->get_header();
        if (mode == PatchMode::OFFSET)
            return std::string(offset_warning ? "WARNING" : "OFFSET") + ": " + patch->get_header() + " offset " + std::to_string(offset) + " lines";
        return "EXACT: " + patch->get_header();
    }
};

void WorkingPatch::fail() {
    result = std::make_shared<Result>();
    result->patch = shared_from_this();
    result->success = false;
}
void WorkingPatch::succeed(PatchMode mode, std::shared_ptr<Patch> applied) {
    result = std::make_shared<Result>();
    result->patch = shared_from_this();
    result->success = true;
    result->mode = mode;
    result->applied_patch = std::move(applied);
}
void WorkingPatch::add_offset_result(int offset, int file_length) {
    result->offset = offset;  // relative to the applied position of the last patch
    result->offset_warning = offset > offset_warn_distance(length1, file_length);
}
std::optional<LineRange> WorkingPatch::get_keepout_range2() const {
    if (result && result->applied_patch) return result->applied_patch->get_trimmed_range2();
    return std::nullopt;
}
std::optional<int> WorkingPatch::get_applied_delta() const {
    if (result && result->applied_patch) return result->applied_patch->length2 - result->applied_patch->length1;
    return std::nullopt;
}

class Patcher {
public:
    std::vector<std::shared_ptr<WorkingPatch>> patches;
    std::vector<std::string> lines;

    Patcher(const PatchFile& patch_file, const std::vector<std::string>& base) : lines(base) {
        for (const Patch& p : patch_file.patches) patches.push_back(std::make_shared<WorkingPatch>(p));
    }

    // Patcher.patch(mode) for mode up to OFFSET (FUZZY is not ported).
    std::vector<std::shared_ptr<Result>> patch(PatchMode mode) {
        if (applied_) throw std::runtime_error("Already applied");
        applied_ = true;
        if (mode == PatchMode::FUZZY) throw std::runtime_error("PatchMode.FUZZY is not ported");

        for (const auto& patch : patches) {
            if (apply_exact(patch)) continue;
            if (mode >= PatchMode::ACCESS && apply_access(patch)) continue;
            if (mode >= PatchMode::OFFSET && apply_offset(patch)) continue;

            patch->fail();
            patch->result->search_offset = search_offset_;
            search_offset_ -= patch->length2 - patch->length1;
        }
        std::vector<std::shared_ptr<Result>> results;
        for (const auto& p : patches) results.push_back(p->result);
        return results;
    }

private:
    bool applied_ = false;
    // Last here means highest line number, not necessarily most recent.
    std::shared_ptr<Patch> last_applied_patch_;
    // the offset of the last patch (applied location - expected location); if a patch fails, its length delta is subtracted
    int search_offset_ = 0;
    CharRepresenter char_rep_;
    std::optional<std::u16string> lm_text_;
    std::optional<std::vector<std::u16string>> wm_lines_;

    static void check_sublist(int from, int to, std::size_t size) {
        if (from < 0 || to > static_cast<int>(size) || from > to) throw std::runtime_error("IndexOutOfBoundsException: [" + std::to_string(from) + "," + std::to_string(to) + ") of " + std::to_string(size));
    }

    void lines_to_chars() {
        for (const auto& p : patches) p->lines_to_chars(char_rep_);
        lm_text_ = char_rep_.lines_to_chars(lines);
    }

    void words_to_chars() {
        for (const auto& p : patches) p->words_to_chars(char_rep_);
        std::vector<std::u16string> w;
        for (const std::string& l : lines) w.push_back(char_rep_.words_to_chars(l));
        wm_lines_ = std::move(w);
    }

    // patch.getContextLines().containsAll(lines.subList(loc, loc + length1)): each line there is one of the context lines (anywhere in them)
    bool context_contains_lines(const Patch& patch, int loc) const {
        const std::vector<std::string> ctx = patch.get_context_lines();
        const std::set<std::string> have(ctx.begin(), ctx.end());
        for (int i = loc; i < loc + patch.length1; ++i)
            if (!have.count(lines[i])) return false;
        return true;
    }

    LineRange get_modified_range() const { return LineRange(0, last_applied_patch_ ? last_applied_patch_->get_trimmed_range2().end : 0); }

    bool can_apply_safely_at(int loc, const Patch& patch) const {
        if (loc >= get_modified_range().end) return true;
        const LineRange range = LineRange::from_start_len(loc, patch.length1);
        for (const auto& p : patches) {
            const auto r = p->get_keepout_range2();
            if (r && r->contains(range)) return false;
        }
        return true;
    }

    std::shared_ptr<Patch> apply_exact_at(int loc, const std::shared_ptr<WorkingPatch>& patch) {
        check_sublist(loc, loc + patch->length1, lines.size());
        if (!context_contains_lines(*patch, loc)) throw std::runtime_error("Patch engine failure");
        if (!can_apply_safely_at(loc, *patch)) throw std::runtime_error("Patch affects another patch");

        lines.erase(lines.begin() + loc, lines.begin() + loc + patch->length1);
        const std::vector<std::string> patched = patch->get_patched_lines();
        lines.insert(lines.begin() + loc, patched.begin(), patched.end());

        // update the lineModeText
        if (lm_text_) {
            if (loc + patch->length1 > static_cast<int>(lm_text_->size())) throw std::runtime_error("StringIndexOutOfBoundsException");
            *lm_text_ = lm_text_->substr(0, loc) + patch->lm_patched + lm_text_->substr(loc + patch->length1);
        }

        // update the wordModeLines
        if (wm_lines_) {
            check_sublist(loc, loc + patch->length1, wm_lines_->size());
            wm_lines_->erase(wm_lines_->begin() + loc, wm_lines_->begin() + loc + patch->length1);
            wm_lines_->insert(wm_lines_->begin() + loc, patch->wm_patched.begin(), patch->wm_patched.end());
        }

        int patched_delta = 0;
        for (const auto& e : patches) {
            const auto r = e->get_keepout_range2();
            if (r && r->end <= loc) patched_delta += *e->get_applied_delta();
        }
        std::shared_ptr<Patch> applied_patch = patch;
        if (applied_patch->start2 != loc || applied_patch->start1 != loc - patched_delta) {
            applied_patch = std::make_shared<Patch>(static_cast<const Patch&>(*patch));
            applied_patch->start1 = loc - patched_delta;
            applied_patch->start2 = loc;
        }

        // update the applied location for patches following this one in the file, but preceding it in the patch list
        if (loc < get_modified_range().end) {
            for (const auto& p : patches) {
                const auto r = p->get_keepout_range2();
                if (r && r->start > loc) p->result->applied_patch->start2 += applied_patch->length2 - applied_patch->length1;
            }
        } else {
            last_applied_patch_ = applied_patch;
        }

        search_offset_ = applied_patch->start2 - patch->start2;
        return applied_patch;
    }

    bool apply_exact(const std::shared_ptr<WorkingPatch>& patch) { return apply_exact(*patch, patch); }
    bool apply_exact(WorkingPatch& patch, const std::shared_ptr<WorkingPatch>& self) {
        const int loc = patch.start2 + search_offset_;
        if (loc + patch.length1 > static_cast<int>(lines.size())) return false;
        check_sublist(loc, loc + patch.length1, lines.size());
        if (!context_contains_lines(patch, loc)) return false;
        patch.succeed(PatchMode::EXACT, apply_exact_at(loc, self));
        return true;
    }

    bool apply_offset(const std::shared_ptr<WorkingPatch>& patch) {
        if (!lm_text_) lines_to_chars();

        if (patch->length1 > static_cast<int>(lines.size())) return false;

        int loc = patch->start2 + search_offset_;
        if (loc < 0) loc = 0;
        else if (loc >= static_cast<int>(lines.size())) loc = static_cast<int>(lines.size()) - 1;

        int forward = java_index_of(*lm_text_, patch->lm_context, loc);
        int reverse = java_last_index_of(*lm_text_, patch->lm_context, std::min(loc + static_cast<int>(patch->lm_context.size()), static_cast<int>(lines.size()) - 1));

        if (!can_apply_safely_at(forward, *patch)) forward = -1;
        if (!can_apply_safely_at(reverse, *patch)) reverse = -1;

        if (forward < 0 && reverse < 0) return false;

        const int found = reverse < 0 || (forward >= 0 && (forward - loc) < (loc - reverse)) ? forward : reverse;
        patch->succeed(PatchMode::OFFSET, apply_exact_at(found, patch));
        patch->add_offset_result(found - loc, static_cast<int>(lines.size()));
        return true;
    }

    // a hunk whose context differs from the file only in access words (public protected private final) and whitespace
    bool apply_access(const std::shared_ptr<WorkingPatch>& patch) {
        if (!wm_lines_) words_to_chars();

        const int loc = patch->start2 + search_offset_;
        if (loc + patch->length1 > static_cast<int>(lines.size())) return false;
        check_sublist(loc, loc + patch->length1, wm_lines_->size());

        const std::vector<std::u16string> wm_sub(wm_lines_->begin() + loc, wm_lines_->begin() + loc + patch->length1);
        if (patch->wm_context.size() != wm_sub.size()) return false;

        const int max_word = std::max(char_rep_.get_max_word_char(), 0x81);
        std::vector<int> a_counts(max_word), b_counts(max_word);
        std::vector<int> match(patch->wm_context.size());

        for (std::size_t i = 0; i < patch->wm_context.size(); ++i) {
            match[i] = loc + static_cast<int>(i);
            // count words in both lines
            for (const char16_t c : patch->wm_context[i]) ++a_counts[c];
            for (const char16_t c : wm_sub[i]) ++b_counts[c];
        }

        // ensure only the allowed words change in counts
        static const std::set<std::u16string> ACCESS_WORDS = {u"public", u"protected", u"private", u"final", u" ", u"\t"};
        for (int i = 0; i < static_cast<int>(a_counts.size()); ++i) {
            if (a_counts[i] != b_counts[i] && !ACCESS_WORDS.count(char_rep_.get_word_for_char(static_cast<char16_t>(i)))) return false;
        }

        auto fuzzy_patch = std::make_shared<WorkingPatch>(adjust_patch_to_matched_lines(*patch, match, lines));
        fuzzy_patch->words_to_chars(char_rep_);
        if (lm_text_) fuzzy_patch->lines_to_chars(char_rep_);

        patch->succeed(PatchMode::ACCESS, apply_exact_at(loc, fuzzy_patch));
        return true;
    }

    static Patch adjust_patch_to_matched_lines(const Patch& patch, const std::vector<int>& match, const std::vector<std::string>& lines) {
        // replace the patch with a copy
        Patch fuzzy_patch = patch;
        std::vector<Diff>& diffs = fuzzy_patch.diffs;

        // keep operations, but replace lines with lines in source text; unmatched patch lines (-1) are deleted;
        // unmatched target lines (increasing offset) are added to the patch
        for (int i = 0, j = 0, ploc = -1; i < patch.length1; ++i) {
            const int mloc = match[i];

            // insert extra target lines into patch
            if (mloc >= 0 && ploc >= 0 && mloc - ploc > 1) {
                const Operation op = diffs.at(j - 1).op == Operation::DELETE && diffs.at(j).op == Operation::DELETE ? Operation::DELETE : Operation::EQUAL;
                for (int l = ploc + 1; l < mloc; ++l) diffs.insert(diffs.begin() + j++, Diff{op, lines.at(l)});
            }
            ploc = mloc;

            // keep insert lines the same
            while (diffs.at(j).op == Operation::INSERT) ++j;

            if (mloc < 0) diffs.erase(diffs.begin() + j);  // unmatched context line
            else diffs.at(j++).text = lines.at(mloc);      // update context to match target file (may be the same)
        }

        fuzzy_patch.recalculate_length();
        return fuzzy_patch;
    }
};

// ---- PatchOperation's folder walk ----

// Utils.indexChildren: every regular file under `root` by its path relative to it, '/'-separated. (A root that is itself a file is one entry, "".)
std::map<std::string, fs::path> index_children(const fs::path& root) {
    std::map<std::string, fs::path> index;
    std::error_code ec;
    if (fs::is_regular_file(root, ec)) {
        index.emplace("", root);
        return index;
    }
    for (fs::recursive_directory_iterator it(root, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        std::string rel = it->path().lexically_relative(root).generic_string();
        if (!rel.empty() && rel[0] == '/') rel.erase(0, 1);
        index.emplace(std::move(rel), it->path());
    }
    return index;
}

// codechicken.diffpatch.util.FileCollector
class FileCollector {
public:
    void consume(const std::string& name, std::vector<std::string> lines) {
        if (name == DEV_NULL) return;
        const auto it = std::find(removed_.begin(), removed_.end(), name);
        if (it != removed_.end()) removed_.erase(it);
        files_[name] = std::move(lines);
    }
    void remove(const std::string& name) {
        if (name == DEV_NULL) return;
        removed_.push_back(name);
    }
    const std::vector<std::string>& get_removed() const { return removed_; }
    const std::map<std::string, std::vector<std::string>>& get() const { return files_; }

private:
    std::map<std::string, std::vector<std::string>> files_;
    std::vector<std::string> removed_;
};

// PatchOperation.doPatch(outputCollector, ..., base, patchFile, ...): one file's hunks; false if any hunk was rejected
bool do_patch_file(FileCollector& output, PatchesSummary& summary, const std::vector<std::string>& base, const PatchFile& patch_file, const PatchOptions& o) {
    Patcher patcher(patch_file, base);
    const std::vector<std::shared_ptr<Result>> results = patcher.patch(o.mode);
    bool rejected = false;
    for (const std::shared_ptr<Result>& result : results) {
        if (result->mode) {
            switch (*result->mode) {
                case PatchMode::EXACT: ++summary.exact_matches; summary.overall_quality += 100; break;
                case PatchMode::ACCESS: ++summary.access_matches; summary.overall_quality += 100; break;
                case PatchMode::OFFSET: ++summary.offset_matches; summary.overall_quality += 100; break;
                case PatchMode::FUZZY: ++summary.fuzzy_matches; summary.overall_quality += static_cast<double>(result->fuzzy_quality) * 100; break;
            }
        } else {
            ++summary.failed_matches;
        }
        if (!result->success) rejected = true;  // DiffPatch builds the rejects text here; no rejects path is kept
    }
    std::vector<std::string> lines = std::move(patcher.lines);
    if (!lines.empty()) {
        if (lines.back().empty()) {
            if (!patch_file.no_new_line) lines.pop_back();  // we end in a new line and shouldn't have one
        } else {
            lines.push_back("");
        }
    }
    output.remove(patch_file.get_base_path(o.a_prefix));
    output.consume(patch_file.get_patched_path(o.b_prefix), std::move(lines));
    return !rejected;
}

void make_parent_dirs(const fs::path& path) {
    std::error_code ec;
    if (!fs::exists(path.parent_path(), ec)) fs::create_directories(path.parent_path());
}

void write_bytes(const fs::path& path, const std::string& bytes) {
    make_parent_dirs(path);
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) throw std::runtime_error("cannot write " + path.string());
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    if (!out) throw std::runtime_error("cannot write " + path.string());
}

}  // namespace

std::string PatchesSummary::print(bool slim, const std::string& eol) const {
    std::string s = "Patch Summary:" + eol;
    if (!slim) {
        s += " Unchanged files:  " + std::to_string(unchanged_files) + eol;
        s += " Changed files:    " + std::to_string(changed_files) + eol;
        s += " Added files:      " + std::to_string(added_files) + eol;
        s += " Removed files:    " + std::to_string(removed_files) + eol;
        s += " Missing files:    " + std::to_string(missing_files) + eol;
    }
    s += eol;
    s += " Failed matches:   " + std::to_string(failed_matches) + eol;
    s += " Exact matches:    " + std::to_string(exact_matches) + eol;
    s += " Access matches:   " + std::to_string(access_matches) + eol;
    s += " Offset matches:   " + std::to_string(offset_matches) + eol;
    s += " Fuzzy matches:    " + std::to_string(fuzzy_matches) + eol;
    const double total = failed_matches + exact_matches + access_matches + offset_matches + fuzzy_matches;
    s += "Overall Quality   " + format_2f(overall_quality / total) + "%" + eol;
    return s;
}

PatchOutcome apply_patch(const fs::path& base, const fs::path& out, const fs::path& patches, const PatchOptions& o) {
    PatchOutcome outcome;
    std::error_code ec;
    if (!fs::exists(base, ec) || !fs::exists(patches, ec)) return outcome;
    if (fs::is_regular_file(base, ec) && fs::is_regular_file(patches, ec))
        throw std::runtime_error("patching a single file is not ported (apply_patch takes a folder as base)");

    FileCollector output;
    PatchesSummary& summary = outcome.summary;
    const std::map<std::string, fs::path> base_index = index_children(base);
    const std::map<std::string, fs::path> patch_index = index_children(patches);

    // PatchOperation.doPatch(Set, Set, ...): the patch files, then each file's hunks
    std::vector<PatchFile> patch_files;
    for (const auto& [name, path] : patch_index) {
        const auto lines = read_all_lines(path);
        if (!lines) continue;  // "Failed to read patch file"
        for (PatchFile& pf : PatchFile::from_lines(name, *lines, true)) patch_files.push_back(std::move(pf));
    }

    bool result = true;
    for (const PatchFile& patch : patch_files) {
        const std::string base_path = patch.get_base_path(o.a_prefix);
        if (base_path != DEV_NULL && !base_index.count(base_path)) {
            ++summary.missing_files;
            continue;
        }
        std::vector<std::string> lines;
        if (base_path != DEV_NULL) {
            const auto read = read_all_lines(base_index.at(base_path));
            if (!read) continue;  // "Failed to read file"
            lines = *read;
        }
        result = do_patch_file(output, summary, lines, patch, o) && result;
    }

    // every file of the base, as it is: read before the output folder is removed (it may be the base)
    std::map<std::string, std::string> raw_data;
    for (const auto& [name, path] : base_index) {
        auto bytes = read_all_bytes(path);
        if (!bytes) throw std::runtime_error("cannot read " + path.string());
        raw_data.emplace(name, std::move(*bytes));
    }

    if (fs::exists(out, ec)) fs::remove_all(out, ec);
    const std::vector<std::string>& removed = output.get_removed();
    for (const auto& [name, bytes] : raw_data) {
        if (std::find(removed.begin(), removed.end(), name) == removed.end()) write_bytes(out / fs::u8path(name), bytes);
    }
    for (const auto& [name, lines] : output.get()) write_bytes(out / fs::u8path(name), join(lines, o.line_separator));

    outcome.success = result;
    outcome.log = summary.print(false);
    return outcome;
}

}  // namespace cb::java::minecraft::prep
