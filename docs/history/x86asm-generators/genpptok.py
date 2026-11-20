import re
src=open('../../../../native/nasm/asm/pptok.c').read()
m=re.search(r'pp_directives\[184\] = \{(.*?)\};',src,re.S)
names=[x.strip().rstrip(',') for x in m.group(1).strip().split('\n')]
hdr=open('../../../../native/nasm/asm/pptok.h').read()
enum=re.search(r'enum preproc_token \{(.*?)\};',hdr,re.S).group(1)
ids=re.findall(r'(PP_\w+)\s*=\s*(-?\d+)',enum)
out=['// cb/build/x86asm/pptok.hpp -- the preprocessor directive tokens (NASM asm/pptok.dat, pptok.h, pptok.c), generated once from',
'// the generated C and kept as data: the enum, the directive spellings, and the lookups by name.','#pragma once','',
'#include <cstdint>','#include <string_view>','','namespace cb::build::x86asm {','','enum PpToken : int {']
for n,v in ids: out.append(f'    {n} = {v},')
out.append('};')
out.append('''
// A condition token's test (the IF/ELIF stem is dropped), whether it is the negative form, and the class tests.
constexpr int pp_cond(int x) { return x & 0x1f; }
constexpr bool pp_is_cond(int x) { return static_cast<unsigned>(x) < static_cast<unsigned>(PP_ALIASES); }
constexpr bool pp_cond_negative(int x) { return (x & 0x20) != 0; }
constexpr bool pp_has_case(int x) { return x >= PP_ASSIGN; }
constexpr bool pp_insensitive(int x) { return (x & 1) != 0; }
constexpr bool pp_is_if(int x) { return x >= PP_IF && x < PP_ELIF; }
constexpr bool pp_is_elif(int x) { return x >= PP_ELIF && x < PP_ALIASES; }
inline constexpr int PP_TOKLEN_MAX = 15;

// The directive spelling ("%define"), or nullptr for the reserved slots with no directive.
const char* pp_directive_name(int token);
// The token for a "%name" (case-blind), PP_invalid if there is none; and for TASM mode's names without the %.
int pp_token_hash(std::string_view name);
int pp_tasm_token_hash(std::string_view name);

}  // namespace cb::build::x86asm''')
open('../../../../native/cb/build/x86asm/pptok.hpp','w').write('\n'.join(out)+'\n')
c=['// cb/build/x86asm/pptok.cpp -- the preprocessor directive names and their lookup (NASM asm/pptok.c); see pptok.hpp.','#include "cb/build/x86asm/pptok.hpp"','',
'#include <array>','#include <string>','#include <unordered_map>','','namespace cb::build::x86asm {','','namespace {','','const char* const kDirectives[184] = {']
for n in names: c.append(f'    {n},')
c.append('};')
tasm=re.findall(r'^@(\w+)',open('../../../../native/nasm/asm/pptok.dat').read(),re.M)
c.append('const char* const kTasm[] = {'+', '.join('"%'+t+'"' for t in tasm)+'};')
c.append('''
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

}  // namespace cb::build::x86asm''')
open('../../../../native/cb/build/x86asm/pptok.cpp','w').write('\n'.join(c)+'\n')
print(len(names),len(ids),tasm)
