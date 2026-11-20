// cb/build/x86asm/pptok.cpp -- the preprocessor directive names and their lookup (NASM asm/pptok.c); see pptok.hpp.
#include "cb/build/x86asm/pptok.hpp"

#include <array>
#include <string>
#include <unordered_map>

namespace cb::build::x86asm {

namespace {

const char* const kDirectives[184] = {
    "%if",
    NULL,
    "%ifctx",
    "%ifdef",
    "%ifdefalias",
    "%ifdifi",
    "%ifdirective",
    "%ifempty",
    "%ifenv",
    "%iffile",
    "%ifid",
    "%ifidn",
    "%ifidni",
    "%ifmacro",
    "%ifnum",
    "%ifstr",
    "%iftoken",
    "%ifusable",
    "%ifusing",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "%ifn",
    NULL,
    "%ifnctx",
    "%ifndef",
    "%ifndefalias",
    "%ifndifi",
    "%ifndirective",
    "%ifnempty",
    "%ifnenv",
    "%ifnfile",
    "%ifnid",
    "%ifnidn",
    "%ifnidni",
    "%ifnmacro",
    "%ifnnum",
    "%ifnstr",
    "%ifntoken",
    "%ifnusable",
    "%ifnusing",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "%elif",
    NULL,
    "%elifctx",
    "%elifdef",
    "%elifdefalias",
    "%elifdifi",
    "%elifdirective",
    "%elifempty",
    "%elifenv",
    "%eliffile",
    "%elifid",
    "%elifidn",
    "%elifidni",
    "%elifmacro",
    "%elifnum",
    "%elifstr",
    "%eliftoken",
    "%elifusable",
    "%elifusing",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "%elifn",
    NULL,
    "%elifnctx",
    "%elifndef",
    "%elifndefalias",
    "%elifndifi",
    "%elifndirective",
    "%elifnempty",
    "%elifnenv",
    "%elifnfile",
    "%elifnid",
    "%elifnidn",
    "%elifnidni",
    "%elifnmacro",
    "%elifnnum",
    "%elifnstr",
    "%elifntoken",
    "%elifnusable",
    "%elifnusing",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "%aliases",
    "%arg",
    "%clear",
    "%depend",
    "%else",
    "%endif",
    "%endm",
    "%endmacro",
    "%endrep",
    "%error",
    "%exitmacro",
    "%exitrep",
    "%fatal",
    "%include",
    "%line",
    "%local",
    "%note",
    "%null",
    "%pop",
    "%pragma",
    "%push",
    "%rep",
    "%repl",
    "%require",
    "%rotate",
    "%stacksize",
    "%undef",
    "%undefalias",
    "%use",
    "%warning",
    "%assign",
    "%iassign",
    "%defalias",
    "%idefalias",
    "%define",
    "%idefine",
    "%defstr",
    "%idefstr",
    "%deftok",
    "%ideftok",
    "%macro",
    "%imacro",
    "%pathsearch",
    "%ipathsearch",
    "%rmacro",
    "%irmacro",
    "%strcat",
    "%istrcat",
    "%strlen",
    "%istrlen",
    "%substr",
    "%isubstr",
    "%xdefine",
    "%ixdefine",
    "%unmacro",
    "%unimacro",
};
const char* const kTasm[] = {"%arg", "%elif", "%else", "%endif", "%if", "%ifdef", "%ifdifi", "%ifndef", "%include", "%local"};

std::string lower(std::string_view s) {
    std::string r(s);
    for (char& c : r) if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return r;
}

const std::unordered_map<std::string, int>& table() {
    static const std::unordered_map<std::string, int> t = [] {
        std::unordered_map<std::string, int> m;
        for (int i = 0; i < 184; ++i)
            if (kDirectives[i]) m.emplace(lower(kDirectives[i]), i);
        return m;
    }();
    return t;
}

}  // namespace

const char* pp_directive_name(int token) {
    return token >= 0 && token < 184 ? kDirectives[token] : nullptr;
}

int pp_token_hash(std::string_view name) {
    if (name.size() > static_cast<std::size_t>(PP_TOKLEN_MAX)) return PP_invalid;
    auto it = table().find(lower(name));
    return it == table().end() ? PP_invalid : it->second;
}

int pp_tasm_token_hash(std::string_view name) {
    std::string full = "%" + lower(name);
    for (const char* t : kTasm)
        if (full == t) {
            auto it = table().find(full);
            return it == table().end() ? PP_invalid : it->second;
        }
    return PP_invalid;
}

}  // namespace cb::build::x86asm
