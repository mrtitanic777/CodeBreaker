// build/source_form.cpp -- a built module's source form (see source_form.hpp).

#include "build/source_form.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::build::source_form {

namespace stdfs = std::filesystem;

namespace {

bool sf_ident_start(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool sf_ident_char(char c) { return sf_ident_start(c) || (c >= '0' && c <= '9'); }

// Words a name may not be: C's keywords and Watcom's, and the assembler's instructions, registers and directives that the build's
// pieces use (a name the assembler would read as one of these).
const std::set<std::string>& sf_reserved() {
    static const std::set<std::string> words = [] {
        std::set<std::string> w;
        for (const char* k : {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for",
                              "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static",
                              "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "far", "near", "huge", "pascal",
                              "cdecl", "fortran", "interrupt", "export", "loadds", "saveregs", "based", "segment", "segname", "self",
                              // the assembler
                              "offset", "seg", "ptr", "byte", "word", "dword", "fword", "qword", "tbyte", "short", "near", "far", "proc", "endp",
                              "ends", "end", "assume", "extrn", "public", "group", "db", "dw", "dd", "dq", "dt", "dup", "equ", "label", "org",
                              "align", "even", "this", "type", "length", "size", "width", "mask", "high", "low", "highword", "lowword", "and", "or",
                              "xor", "not", "shl", "shr", "mod", "eq", "ne", "lt", "le", "gt", "ge", "call", "jmp", "ret", "retf", "push", "pop",
                              "mov", "lea", "les", "lds", "int", "in", "out", "loop", "nop", "lock", "rep", "ax", "bx", "cx", "dx", "si", "di", "bp",
                              "sp", "cs", "ds", "es", "ss", "fs", "gs", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "eax", "ebx", "ecx", "edx",
                              "esi", "edi", "ebp", "esp", "st", "include", "includelib", "comment", "name", "title", "subttl", "page", "option",
                              "struc", "record", "macro", "endm", "local", "if", "else", "endif"})
            w.insert(k);
        return w;
    }();
    return words;
}

std::string sf_backslashed(std::string_view p) { return text::replace_all(p, "/", "\\"); }

// The object a `file` line of the link names, without ".obj".
std::optional<std::string> sf_linked_object(std::string_view line) {
    const std::string_view l = text::trim(line);
    if (!l.starts_with("file ")) return std::nullopt;
    std::string_view stem = text::trim(l.substr(5));
    if (stem.ends_with(".obj")) stem.remove_suffix(4);
    return std::string(stem);
}

// The command line of a tool the build ran (without the program's folder: wmake finds it on PATH).
std::string sf_command_of(const functions::MadeObject& made, std::string_view object, std::string_view source) {
    std::string out = made.tool;
    for (const std::string& f : made.flags) out += " " + f;
    out += std::format(" -fo={} {}", object, source);
    return out;
}

// What the makefile says of itself.
constexpr std::string_view sf_kMakefileHead =
    "# {0} -- the module's source form, as `codebreaker build` wrote it from the module it rebuilt: every object of the image\n"
    "# from the sources beside this file, with OpenWatcom's tools and the options the build used.\n"
    "#\n"
    "#     set WATCOM=<OpenWatcom>  (its binnt64 or binnt on PATH; INCLUDE as OpenWatcom's setvars sets it, for the resources)\n"
    "#     wmake                    links m.dll, the image the build made of {0}\n"
    "#\n"
    "# The C pieces are the functions written in C; the asm pieces are those written in assembly, each with its reason as its first\n"
    "# lines; data.c is the module's data. stub.exe is the original's DOS part, carried as it is.\n"
    "\n"
    ".ERASE\n"
    ".EXTENSIONS:\n"
    "\n";

}  // namespace

// ---- names ----------------------------------------------------------------------------------------------------------------------

std::string rename_identifiers(std::string_view text, const std::map<std::string, std::string>& names) {
    if (names.empty()) return std::string(text);
    std::string out;
    out.reserve(text.size() + text.size() / 8);
    std::size_t i = 0;
    while (i < text.size()) {
        const char c = text[i];
        if (!sf_ident_start(c) || (i > 0 && sf_ident_char(text[i - 1]))) {
            out += c;
            ++i;
            continue;
        }
        std::size_t e = i + 1;
        while (e < text.size() && sf_ident_char(text[e])) ++e;
        const std::string_view word = text.substr(i, e - i);
        if (const auto it = names.find(std::string(word)); it != names.end()) {
            out += it->second;
        } else if (word.size() > 1 && word[0] == '_') {
            const auto bare = names.find(std::string(word.substr(1)));
            if (bare != names.end()) out += "_" + bare->second;
            else out += word;
        } else {
            out += word;
        }
        i = e;
    }
    return out;
}

void identifiers_of(std::string_view text, std::set<std::string>& out) {
    std::size_t i = 0;
    while (i < text.size()) {
        if (!sf_ident_start(text[i]) || (i > 0 && sf_ident_char(text[i - 1]))) {
            ++i;
            continue;
        }
        std::size_t e = i + 1;
        while (e < text.size() && sf_ident_char(text[e])) ++e;
        out.emplace(text.substr(i, e - i));
        i = e;
    }
}

std::map<std::string, std::string> free_renames(const std::map<std::string, std::string>& wanted, const std::set<std::string>& taken) {
    std::set<std::string> used;   // lower case: every identifier there is, and every name given
    for (const std::string& t : taken) used.insert(text::lower(t));
    std::map<std::string, std::string> out;
    for (const auto& [from, to] : wanted) {
        if (from == to || !text::is_identifier(to) || !text::is_identifier(from) || to.starts_with("sub_")) continue;
        const std::string low = text::lower(to);
        // (the decorated form too: a name `_x` would be read as the symbol of `x`)
        if (used.contains(low) || used.contains("_" + low) || sf_reserved().contains(low) || to.starts_with("codebreaker_")) continue;
        out[from] = to;
        used.insert(low);
    }
    return out;
}

// ---- parameters ------------------------------------------------------------------------------------------------------------------

std::optional<int> sdk_words(std::string_view type) {
    std::string t;
    for (std::string_view w : text::split(type, ' ')) {
        if (w.empty() || w == "const" || w == "CONST" || w == "FAR" || w == "far" || w == "_far" || w == "__far" || w == "volatile") continue;
        if (!t.empty()) t += ' ';
        t += w;
    }
    if (t.find("NEAR") != std::string::npos || t.find("near") != std::string::npos) return 1;   // (a near pointer: one word)
    if (t.find('*') != std::string::npos) return 2;   // (the 16-bit headers' pointers are far unless they say near)
    static const std::set<std::string, std::less<>> one = {"WORD", "UINT", "int", "INT", "BOOL", "short", "SHORT", "unsigned", "unsigned int",
                                                          "unsigned short", "ATOM", "WPARAM", "BYTE", "char", "CHAR", "HFILE", "HANDLE", "HWND",
                                                          "HDC", "HINSTANCE", "HMODULE", "HGLOBAL", "HLOCAL", "HMENU", "HICON", "HCURSOR", "HBRUSH",
                                                          "HPEN", "HFONT", "HBITMAP", "HRGN", "HPALETTE", "HRSRC", "HTASK", "HGDIOBJ", "HMETAFILE",
                                                          "HACCEL", "HHOOK", "HDWP", "HDRVR", "HKEY", "COLORREF16", "WCHAR", "UCHAR", "LANGID"};
    static const std::set<std::string, std::less<>> two = {"DWORD", "LONG", "LPARAM", "LRESULT", "COLORREF", "ULONG", "long", "unsigned long",
                                                          "FARPROC", "HOOKPROC", "WNDPROC", "DLGPROC", "TIMERPROC", "HPSTR", "HPCSTR", "SEGPTR",
                                                          "LPCATCHBUF", "HKL"};
    if (one.contains(t)) return 1;
    if (two.contains(t)) return 2;
    if (t.size() > 2 && t.starts_with("LP") && t.find(' ') == std::string::npos) return 2;   // LPSTR, LPRECT, LPOFSTRUCT ...
    if (t.size() > 2 && t.starts_with("NP") && t.find(' ') == std::string::npos) return 1;   // NPSTR ...
    return std::nullopt;
}

std::map<std::string, std::string> sdk_parameter_names(std::string_view c_text, std::string_view function,
                                                       const std::vector<std::pair<std::string, std::string>>& declared) {
    std::map<std::string, std::string> out;
    // the heading: `<...> __stdcall16far <...> NAME(a1,a3,...)` and its old-style declarations, one a line, before the body
    const std::size_t brace = c_text.find('{');
    if (brace == std::string_view::npos) return out;
    const std::string_view head = c_text.substr(0, brace);
    const std::string call = std::string(function) + "(";
    const std::size_t at = head.find(call);
    if (at == std::string_view::npos || head.substr(0, at).find("__stdcall16far") == std::string_view::npos) return out;
    const std::size_t close = head.find(')', at);
    if (close == std::string_view::npos) return out;
    struct Slot {
        std::string name;
        int word = 0, words = 0;
    };
    std::vector<Slot> slots;
    for (std::string_view p : text::split(head.substr(at + call.size(), close - at - call.size()), ',')) {
        p = text::trim(p);
        if (p.size() < 2 || p[0] != 'a') return {};
        const auto n = text::parse_int<int>(p.substr(1), 10);
        if (!n || *n < 1) return {};
        slots.push_back({std::string(p), *n, 0});
    }
    if (slots.empty()) return out;
    // each one's size, from its declaration line
    for (std::string_view line : text::lines(head.substr(close + 1))) {
        line = text::trim(line);
        if (!line.ends_with(';')) continue;
        line.remove_suffix(1);
        const std::size_t sp = line.find_last_of(" *");
        if (sp == std::string_view::npos) continue;
        const std::string_view name = line.substr(sp + 1), type = line.substr(0, sp + 1);
        for (Slot& sl : slots) {
            if (sl.name != name) continue;
            if (type.find('*') != std::string_view::npos) sl.words = type.find("__far") != std::string_view::npos ? 2 : 1;
            else if (type.find('4') != std::string_view::npos) sl.words = 2;
            else if (type.find('2') != std::string_view::npos || type.find('1') != std::string_view::npos) sl.words = 1;
        }
    }
    int next = 1;
    for (const Slot& sl : slots) {
        if (sl.words == 0 || sl.word != next) return {};   // (a gap, or a size not read: the words cannot be told)
        next += sl.words;
    }
    // the header's parameters, last first, word by word
    std::vector<std::pair<int, int>> places;   // (first word, words) of each declared parameter, in the header's order
    int words = 0;
    for (auto it = declared.rbegin(); it != declared.rend(); ++it) {
        const std::optional<int> w = sdk_words(it->first);
        if (!w || it->second.empty() || !text::is_identifier(it->second)) return {};
        places.emplace_back(words + 1, *w);
        words += *w;
    }
    if (words != next - 1) return {};
    for (std::size_t k = 0; k < places.size(); ++k) {
        const auto& [first, size] = places[k];
        const auto& [type, name] = declared[declared.size() - 1 - k];
        std::vector<const Slot*> inside;
        for (const Slot& sl : slots)
            if (sl.word >= first && sl.word < first + size) inside.push_back(&sl);
        if (inside.size() == 1 && inside[0]->words == size) {
            out[inside[0]->name] = name;
        } else if (inside.size() == 2 && size == 2) {
            const bool pointer = type.find('*') != std::string::npos || (type.starts_with("LP") || type.starts_with("HP"));
            out[inside[0]->name] = name + (pointer ? "_off" : "_lo");
            out[inside[1]->name] = name + (pointer ? "_seg" : "_hi");
        }
    }
    return out;
}

// ---- the recipe ------------------------------------------------------------------------------------------------------------------

std::string makefile(std::string_view module, const std::vector<Rule>& rules, std::string_view libpath) {
    std::string out = std::vformat(sf_kMakefileHead, std::make_format_args(module));
    out += "objects =";
    for (const Rule& r : rules) out += " &\n    " + r.object;
    out += "\n\nm.dll: $(objects) m.lnk\n";
    out += std::format("    wlink libpath $(%WATCOM)\\{} @m.lnk\n", libpath);
    for (const Rule& r : rules) {
        out += "\n" + r.object + ":";
        for (const std::string& n : r.needs) out += " " + n;
        out += "\n    " + r.command + "\n";
    }
    return out;
}

// ---- writing --------------------------------------------------------------------------------------------------------------------

SourceForm write(const Input& in) {
    SourceForm form;
    form.folder = in.build_folder / "source";
    fs::remove_tree(form.folder);
    fs::make_dirs(form.folder);

    // every object the link uses, and how it was made
    struct Piece {
        std::string stem;                     // as the build has it: s03/f0000, asm/frame03_01, data
        std::optional<functions::MadeObject> made;
    };
    std::vector<Piece> pieces;
    for (const std::string& line : in.link_lines) {
        if (const std::optional<std::string> stem = sf_linked_object(line)) {
            Piece p{*stem, in.made(in.build_folder / (*stem + ".obj"))};
            if (!p.made) form.missing.push_back(*stem);
            pieces.push_back(std::move(p));
        }
    }

    // the names: free ones only, decided over every text the form holds
    std::set<std::string> taken;
    for (const Piece& p : pieces)
        if (p.made) identifiers_of(p.made->text, taken);
    for (const std::string& line : in.link_lines) identifiers_of(line, taken);
    const stdfs::path header = in.build_folder / "m.h";
    const std::string header_text = stdfs::is_regular_file(header) ? fs::read_text(header) : std::string();
    identifiers_of(header_text, taken);
    form.renamed = free_renames(in.names, taken);

    // a piece holding one function is named after it (s03/f0000 -> s03/GetFileResourceSize); no two in one folder alike, without case
    std::map<std::string, std::string> stems;   // build stem -> written stem
    {
        std::set<std::string> in_use;
        for (const Piece& p : pieces) in_use.insert(text::lower(p.stem));
        for (const Piece& p : pieces) {
            std::string written = p.stem;
            const auto fn = in.stem_function.find(p.stem);
            const std::size_t slash = p.stem.rfind('/');
            if (fn != in.stem_function.end() && slash != std::string::npos && p.made && p.made->source.ends_with(".c")) {
                const auto renamed = form.renamed.find(fn->second);
                const std::string name = renamed != form.renamed.end() ? renamed->second : fn->second;
                const std::string candidate = p.stem.substr(0, slash + 1) + name;
                if (text::is_identifier(name) && !in_use.contains(text::lower(candidate))) {
                    in_use.insert(text::lower(candidate));
                    written = candidate;
                }
            }
            stems[p.stem] = written;
        }
    }

    std::vector<Rule> rules;
    for (const Piece& p : pieces) {
        if (!p.made) continue;
        const std::string& written = stems.at(p.stem);
        const bool c = p.made->source.ends_with(".c");
        const std::string source = written + (c ? ".c" : ".asm");
        std::string body = rename_identifiers(p.made->text, form.renamed);
        // an export's parameters by the SDK's names (in its text alone: they are its own)
        if (const auto fn = in.stem_function.find(p.stem); c && fn != in.stem_function.end()) {
            if (const auto declared = in.parameters.find(fn->second); declared != in.parameters.end()) {
                const auto renamed = form.renamed.find(fn->second);
                const std::string name = renamed != form.renamed.end() ? renamed->second : fn->second;
                std::set<std::string> here = taken;
                identifiers_of(body, here);
                const std::map<std::string, std::string> params = free_renames(sdk_parameter_names(body, name, declared->second), here);
                body = rename_identifiers(body, params);
                form.named_parameters += params.size();
            }
        }
        fs::write_text(form.folder / source, body);
        (c ? form.c_files : form.asm_files) += 1;
        Rule r;
        r.object = sf_backslashed(written + ".obj");
        r.needs.push_back(sf_backslashed(source));
        if (c) r.needs.push_back("m.h");
        // (a C piece was compiled in its own folder, beside a copy of m.h; here there is one m.h, found through -i)
        functions::MadeObject how = *p.made;
        if (c && written.find('/') != std::string::npos) how.flags.push_back("-i=.");
        r.command = sf_command_of(how, r.object, sf_backslashed(source));
        rules.push_back(std::move(r));
    }
    fs::write_text(form.folder / "m.h", rename_identifiers(header_text, form.renamed));

    // the resources: the script and the files it reads, compiled as the build compiled them
    if (in.rc && in.res) {
        const std::string rc = fs::to_text(in.rc->filename()), res = fs::to_text(in.res->filename());
        stdfs::copy_file(*in.rc, form.folder / rc, stdfs::copy_options::overwrite_existing);
        Rule r{res, {rc}, std::format("wrc -q -r -bt=windows -fo={} {}", res, rc)};
        for (const stdfs::path& f : in.resource_files) {
            fs::make_dirs((form.folder / f).parent_path());
            stdfs::copy_file(in.build_folder / f, form.folder / f, stdfs::copy_options::overwrite_existing);
            r.needs.push_back(sf_backslashed(fs::to_text(f)));
        }
        rules.push_back(std::move(r));
    }
    for (const stdfs::path& f : in.carried) stdfs::copy_file(in.build_folder / f, form.folder / f, stdfs::copy_options::overwrite_existing);
    for (const stdfs::path& f : in.documents)
        if (stdfs::is_regular_file(in.build_folder / f)) fs::write_text(form.folder / f, rename_identifiers(fs::read_text(in.build_folder / f), form.renamed));

    // the link: as the build's, its objects by their written names, the library folder given by the makefile
    std::vector<std::string> lnk;
    for (const std::string& line : in.link_lines) {
        if (text::trim(line).starts_with("libpath ")) continue;
        if (const std::optional<std::string> stem = sf_linked_object(line)) {
            const auto it = stems.find(*stem);
            lnk.push_back("file " + (it != stems.end() ? it->second : *stem) + ".obj");
            continue;
        }
        lnk.push_back(rename_identifiers(line, form.renamed));
    }
    fs::write_text(form.folder / "m.lnk", text::join(lnk, "\n") + "\n");
    fs::write_text(form.folder / "makefile", makefile(in.module, rules, in.libpath));
    return form;
}

// ---- the proof ------------------------------------------------------------------------------------------------------------------

std::string check(const SourceForm& form, const toolchain::watcom::OpenWatcom& ow, const stdfs::path& scratch, const stdfs::path& image) {
    fs::remove_tree(scratch);
    fs::copy_tree(form.folder, scratch);
    process::ProcessSpec spec;
    const stdfs::path wmake = ow.tool("wmake");
    if (wmake.empty()) return "OpenWatcom's wmake is not set up";
    spec.argv = {fs::to_text(wmake), "-h"};
    spec.cwd = scratch;
    spec.env = ow.environment(toolchain::watcom::Target::Win16);
    // (its tools by their names, as a person's PATH finds them)
    const std::string path = fs::to_text(wmake.parent_path()) + ";" + fs::get_env("PATH").value_or("");
    spec.env["PATH"] = path;
    spec.timeout = std::chrono::minutes(60);
    const process::ProcessResult r = process::run(spec);
    std::string why;
    const stdfs::path made = scratch / "m.dll";
    if (!r.ok() || !stdfs::is_regular_file(made)) {
        const std::string said = text::from_latin1(r.out) + text::from_latin1(r.err);
        why = std::format("wmake failed (exit {}): {}", r.exit_code, said.size() > 600 ? said.substr(said.size() - 600) : said);
    } else if (fs::read_file(made) != fs::read_file(image)) {
        why = std::format("the image wmake links differs from the build's ({} bytes against {})", fs::file_size(made), fs::file_size(image));
    }
    if (why.empty()) fs::remove_tree(scratch);   // (a failure's copy is kept to look at)
    return why;
}

}  // namespace cb::build::source_form
