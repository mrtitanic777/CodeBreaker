// app/source_preview.cpp -- see source_preview.hpp.
#include "app/source_preview.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <unordered_set>

#include "core/fs.hpp"

namespace cb::app::preview {

namespace {

namespace stdfs = std::filesystem;

std::string sp_lower(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

// The lower-case extension with its dot (".c"), or empty.
std::string sp_ext(std::string_view path) {
    const std::size_t slash = path.find_last_of("/\\");
    const std::string_view name = slash == std::string_view::npos ? path : path.substr(slash + 1);
    const std::size_t dot = name.find_last_of('.');
    return dot == std::string_view::npos ? std::string() : sp_lower(name.substr(dot));
}

std::string sp_stem(std::string_view path) {
    const std::size_t slash = path.find_last_of("/\\");
    const std::string_view name = slash == std::string_view::npos ? path : path.substr(slash + 1);
    const std::size_t dot = name.find_last_of('.');
    return sp_lower(dot == std::string_view::npos ? name : name.substr(0, dot));
}

std::string sp_name(std::string_view path) {
    const std::size_t slash = path.find_last_of("/\\");
    return sp_lower(slash == std::string_view::npos ? path : path.substr(slash + 1));
}

// The order of main sources: .c, .cpp, .cs, .java, .py, .asm; -1 for anything else.
int sp_main_rank(const std::string& ext) {
    if (ext == ".c") return 0;
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") return 1;
    if (ext == ".cs") return 2;
    if (ext == ".java") return 3;
    if (ext == ".py") return 4;
    if (ext == ".asm" || ext == ".s") return 5;
    return -1;
}

// Build scripts and notes list after the sources.
bool sp_is_script(const std::string& name, const std::string& ext) {
    return ext == ".cmd" || ext == ".bat" || ext == ".sh" || ext == ".ps1" || ext == ".txt" || ext == ".md" || ext == ".json" || ext == ".mak" || ext == ".mk" ||
           name == "makefile" || name == "cmakelists.txt";
}

struct Ranked {
    int group = 1;
    int rank = 0;
    std::string sort_key;
    FileEntry entry;
};

}  // namespace

std::vector<FileEntry> list_files(const stdfs::path& folder, std::string_view main_stem) {
    std::vector<Ranked> found;
    std::error_code ec;
    if (!stdfs::is_directory(folder, ec)) return {};
    const std::string stem = sp_lower(main_stem);
    stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end;
    for (; !ec && it != end && found.size() < 4000; it.increment(ec)) {
        std::error_code e2;
        if (!it->is_regular_file(e2)) continue;
        Ranked r;
        r.entry.path = fs::to_text(stdfs::relative(it->path(), folder, e2));
        if (e2 || r.entry.path.empty()) continue;
        r.entry.size = it->file_size(e2);
        const std::string ext = sp_ext(r.entry.path), name = sp_name(r.entry.path);
        const int main_rank = sp_main_rank(ext);
        if (!stem.empty() && main_rank >= 0 && sp_stem(r.entry.path) == stem && r.entry.path.find('/') == std::string::npos) {
            r.group = 0;
            r.rank = main_rank;
        } else {
            r.group = sp_is_script(name, ext) ? 2 : 1;
        }
        r.sort_key = sp_lower(r.entry.path);
        found.push_back(std::move(r));
    }
    std::sort(found.begin(), found.end(), [](const Ranked& a, const Ranked& b) {
        if (a.group != b.group) return a.group < b.group;
        if (a.rank != b.rank) return a.rank < b.rank;
        return a.sort_key < b.sort_key;
    });
    std::vector<FileEntry> out;
    for (Ranked& r : found) {
        if (out.size() >= kMaxListedFiles) break;
        out.push_back(std::move(r.entry));
    }
    return out;
}

std::string chip_for_files(const std::vector<FileEntry>& files) {
    if (files.empty()) return "Source";
    const std::string ext = sp_ext(files.front().path);
    if (ext == ".c") return "C";
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx") return "C++";
    if (ext == ".cs") return "C#";
    if (ext == ".java") return "Java";
    if (ext == ".py") return "Python";
    if (ext == ".asm" || ext == ".s") return "Assembly";
    return "Source";
}

bool is_script_file(std::string_view path) { return sp_is_script(sp_name(path), sp_ext(path)); }

Lang lang_for(std::string_view file_name) {
    const std::string ext = sp_ext(file_name);
    if (ext == ".c" || ext == ".h") return Lang::C;
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".hpp" || ext == ".hh" || ext == ".hxx") return Lang::Cpp;
    if (ext == ".cs") return Lang::CSharp;
    if (ext == ".asm" || ext == ".s" || ext == ".inc") return Lang::Asm;
    return Lang::Plain;
}

// ---- colouring

namespace {

using Words = std::unordered_set<std::string_view>;

const Words& sp_c_words() {
    static const Words words = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if",
                                "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
                                "unsigned", "void", "volatile", "while", "bool", "true", "false", "NULL",
                                // the Windows types and calling conventions decompiled code is written in
                                "BYTE", "WORD", "DWORD", "INT", "UINT", "LONG", "ULONG", "BOOL", "HANDLE", "HFILE", "HWND", "HDC", "HINSTANCE", "LPSTR", "LPCSTR",
                                "LPVOID", "LPOFSTRUCT", "LPARAM", "WPARAM", "LRESULT", "VOID", "PVOID", "WINAPI", "CALLBACK", "FAR", "NEAR", "PASCAL"};
    return words;
}

const Words& sp_cpp_words() {
    static const Words words = {"class", "namespace", "template", "typename", "public", "private", "protected", "virtual", "new", "delete", "this", "using", "try", "catch",
                                "throw", "nullptr", "constexpr", "override", "final", "operator", "explicit", "mutable", "decltype", "noexcept", "static_cast",
                                "const_cast", "reinterpret_cast", "dynamic_cast", "friend", "auto", "concept", "requires"};
    return words;
}

const Words& sp_cs_words() {
    static const Words words = {"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default",
                                "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto",
                                "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out",
                                "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "static",
                                "string", "struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "var",
                                "virtual", "void", "volatile", "while", "async", "await", "yield", "get", "set"};
    return words;
}

const Words& sp_asm_words() {
    static const Words words = {
        // instructions
        "mov", "push", "pop", "call", "ret", "retn", "retf", "jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle", "jc", "jnc", "js", "jns",
        "jo", "jno", "jcxz", "jecxz", "loop", "loope", "loopne", "lea", "add", "adc", "sub", "sbb", "inc", "dec", "cmp", "test", "and", "or", "xor", "not", "neg", "mul",
        "imul", "div", "idiv", "shl", "shr", "sar", "sal", "rol", "ror", "rcl", "rcr", "xchg", "movzx", "movsx", "cdq", "cwd", "cbw", "nop", "int", "iret", "cli", "sti",
        "hlt", "in", "out", "lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd", "movsb", "movsw", "movsd", "scasb", "cmpsb", "rep", "repe", "repne", "cld", "std", "enter",
        "leave", "pushad", "popad", "pushfd", "popfd", "pushf", "popf", "cpuid", "sete", "setne", "setz", "setnz", "cmove", "cmovne", "bswap", "lahf", "sahf",
        // registers
        "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp", "ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "cs", "ds",
        "es", "fs", "gs", "ss", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
        // directives
        "db", "dw", "dd", "dq", "dt", "equ", "times", "org", "bits", "section", "segment", "ends", "proc", "endp", "assume", "global", "extern", "public", "end", "include",
        "macro", "endm", "struc", "endstruc", "align", "resb", "resw", "resd", "resq", "use16", "use32", "byte", "word", "dword", "qword", "ptr", "offset", "short", "near",
        "far", ".code", ".data", ".text", ".bss", ".model", ".stack"};
    return words;
}

bool sp_keyword(Lang lang, std::string_view ident) {
    switch (lang) {
        case Lang::C: return sp_c_words().contains(ident);
        case Lang::Cpp: return sp_c_words().contains(ident) || sp_cpp_words().contains(ident);
        case Lang::CSharp: return sp_cs_words().contains(ident);
        case Lang::Asm: return sp_asm_words().contains(sp_lower(ident));
        case Lang::Plain: break;
    }
    return false;
}

bool sp_ident_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool sp_ident_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool sp_digit(char c) { return c >= '0' && c <= '9'; }
bool sp_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }

void sp_add(std::vector<Span>& spans, std::size_t from, std::size_t to, Tok kind) {
    if (to > from) spans.push_back({static_cast<std::uint32_t>(from), static_cast<std::uint32_t>(to - from), kind});
}

// A string or character literal starting at `i` (the quote): the end of it (past the closing quote, or the end of the line).
std::size_t sp_quoted(const std::string& line, std::size_t i, bool escapes) {
    const char quote = line[i];
    std::size_t j = i + 1;
    while (j < line.size()) {
        if (escapes && line[j] == '\\' && j + 1 < line.size()) {
            j += 2;
            continue;
        }
        if (line[j] == quote) return j + 1;
        ++j;
    }
    return line.size();
}

void sp_c_line(Lang lang, const std::string& line, bool& in_block, std::vector<Span>& spans) {
    std::size_t i = 0;
    const std::size_t n = line.size();
    if (in_block) {
        const std::size_t close = line.find("*/");
        if (close == std::string::npos) {
            sp_add(spans, 0, n, Tok::Comment);
            return;
        }
        sp_add(spans, 0, close + 2, Tok::Comment);
        i = close + 2;
        in_block = false;
    }
    bool only_space_before = true;
    for (std::size_t k = 0; k < i; ++k) only_space_before = only_space_before && (line[k] == ' ');
    while (i < n) {
        const char c = line[i];
        if (c == '/' && i + 1 < n && line[i + 1] == '/') {
            sp_add(spans, i, n, Tok::Comment);
            return;
        }
        if (c == '/' && i + 1 < n && line[i + 1] == '*') {
            const std::size_t close = line.find("*/", i + 2);
            if (close == std::string::npos) {
                sp_add(spans, i, n, Tok::Comment);
                in_block = true;
                return;
            }
            sp_add(spans, i, close + 2, Tok::Comment);
            i = close + 2;
            only_space_before = false;
            continue;
        }
        if (c == '"' || c == '\'') {
            const std::size_t end = sp_quoted(line, i, true);
            sp_add(spans, i, end, Tok::String);
            i = end;
            only_space_before = false;
            continue;
        }
        if (c == '#' && only_space_before && lang != Lang::CSharp) {   // a preprocessor directive: "#include"
            std::size_t j = i + 1;
            while (j < n && line[j] == ' ') ++j;
            while (j < n && sp_ident_char(line[j])) ++j;
            sp_add(spans, i, j, Tok::Keyword);
            i = j;
            only_space_before = false;
            continue;
        }
        if (sp_digit(c) || (c == '.' && i + 1 < n && sp_digit(line[i + 1]))) {
            std::size_t j = i + 1;
            if (c == '0' && j < n && (line[j] == 'x' || line[j] == 'X')) {
                ++j;
                while (j < n && sp_hex(line[j])) ++j;
            } else {
                while (j < n && (sp_digit(line[j]) || line[j] == '.' || ((line[j] == 'e' || line[j] == 'E') && j + 1 < n && (sp_digit(line[j + 1]) || line[j + 1] == '-' || line[j + 1] == '+'))))
                    j += (line[j] == 'e' || line[j] == 'E') ? 2 : 1;
            }
            while (j < n && (line[j] == 'u' || line[j] == 'U' || line[j] == 'l' || line[j] == 'L' || line[j] == 'f' || line[j] == 'F')) ++j;
            sp_add(spans, i, j, Tok::Number);
            i = j;
            only_space_before = false;
            continue;
        }
        if (sp_ident_start(c)) {
            std::size_t j = i + 1;
            while (j < n && sp_ident_char(line[j])) ++j;
            if (sp_keyword(lang, std::string_view(line).substr(i, j - i))) sp_add(spans, i, j, Tok::Keyword);
            i = j;
            only_space_before = false;
            continue;
        }
        if (c != ' ') only_space_before = false;
        ++i;
    }
}

void sp_asm_line(const std::string& line, std::vector<Span>& spans) {
    std::size_t i = 0;
    const std::size_t n = line.size();
    while (i < n) {
        const char c = line[i];
        if (c == ';') {
            sp_add(spans, i, n, Tok::Comment);
            return;
        }
        if (c == '"' || c == '\'') {
            const std::size_t end = sp_quoted(line, i, false);
            sp_add(spans, i, end, Tok::String);
            i = end;
            continue;
        }
        if (c == '$' && i + 1 < n && sp_hex(line[i + 1])) {   // $FF
            std::size_t j = i + 1;
            while (j < n && sp_hex(line[j])) ++j;
            sp_add(spans, i, j, Tok::Number);
            i = j;
            continue;
        }
        if (sp_digit(c)) {   // 17, 0x10, 0FFh, 1Ch: the whole run of letters and digits
            std::size_t j = i + 1;
            while (j < n && sp_ident_char(line[j])) ++j;
            sp_add(spans, i, j, Tok::Number);
            i = j;
            continue;
        }
        if (sp_ident_start(c) || c == '.' || c == '@') {
            std::size_t j = i + 1;
            while (j < n && (sp_ident_char(line[j]) || line[j] == '.' || line[j] == '@' || line[j] == '$')) ++j;
            if (sp_keyword(Lang::Asm, std::string_view(line).substr(i, j - i))) sp_add(spans, i, j, Tok::Keyword);
            i = j;
            continue;
        }
        ++i;
    }
}

}  // namespace

std::vector<std::vector<Span>> highlight(Lang lang, const std::vector<std::string>& lines) {
    std::vector<std::vector<Span>> out(lines.size());
    bool in_block = false;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lang == Lang::C || lang == Lang::Cpp || lang == Lang::CSharp) sp_c_line(lang, lines[i], in_block, out[i]);
        else if (lang == Lang::Asm) sp_asm_line(lines[i], out[i]);
    }
    return out;
}

// ---- reading

std::string size_words(std::uint64_t bytes) {
    char text[64];
    if (bytes < 1024) std::snprintf(text, sizeof text, "%llu bytes", static_cast<unsigned long long>(bytes));
    else if (bytes < 1024ull * 1024) std::snprintf(text, sizeof text, "%llu KB", static_cast<unsigned long long>((bytes + 512) / 1024));
    else std::snprintf(text, sizeof text, "%.1f MB", static_cast<double>(bytes) / (1024.0 * 1024.0));
    return text;
}

Preview make_preview(std::string_view bytes, std::uint64_t total_bytes) {
    constexpr std::size_t kMaxLine = 2000;
    Preview p;
    p.total_bytes = total_bytes;
    const std::string_view head = bytes.substr(0, std::min<std::size_t>(bytes.size(), 4096));
    if (head.find('\0') != std::string_view::npos) {
        p.binary = true;
        p.note = "Binary file, " + size_words(total_bytes) + ".";
        return p;
    }
    if (bytes.starts_with("\xEF\xBB\xBF")) bytes.remove_prefix(3);   // a UTF-8 byte-order mark
    p.truncated = total_bytes > bytes.size() + 3;
    std::size_t at = 0;
    while (at <= bytes.size()) {
        std::size_t end = bytes.find('\n', at);
        if (end == std::string_view::npos) end = bytes.size();
        std::string line;
        for (std::size_t k = at; k < end && line.size() < kMaxLine; ++k) {
            if (bytes[k] == '\r') continue;
            if (bytes[k] == '\t') line += "    ";
            else line += bytes[k];
        }
        p.lines.push_back(std::move(line));
        at = end + 1;
    }
    if (!p.lines.empty() && p.lines.back().empty()) p.lines.pop_back();   // (the empty piece after the last newline)
    if (p.truncated) p.note = "Showing the first " + size_words(bytes.size()) + " of " + size_words(total_bytes) + ".";
    return p;
}

Preview read_preview(const stdfs::path& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in) {
        Preview p;
        p.note = "This file can't be read.";
        return p;
    }
    std::error_code ec;
    const std::uint64_t total = stdfs::file_size(file, ec);
    std::string bytes(kPreviewBytes, '\0');
    in.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    bytes.resize(static_cast<std::size_t>(std::max<std::streamsize>(0, in.gcount())));
    return make_preview(bytes, ec ? bytes.size() : total);
}

}  // namespace cb::app::preview
