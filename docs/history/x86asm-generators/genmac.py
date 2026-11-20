import re
BS = chr(92)
DQ = chr(34)
src = open('../../../../native/nasm/macros/macros.c', encoding='utf8').read()
blocks = re.findall(r'/\*\n((?: \*[^\n]*\n)+) \*/\n\nstatic const unsigned char (\w+)_blob', src)
want = {'nasm_stdmac_version': 'version', 'nasm_usemac_altreg': 'altreg', 'nasm_usemac_fp': 'fp',
        'nasm_usemac_ifunc': 'ifunc', 'nasm_usemac_masm': 'masm', 'nasm_usemac_smartalign': 'smartalign',
        'nasm_stdmac_tasm': 'tasm', 'nasm_stdmac_nasm': 'nasm', 'nasm_usemac_vtern': 'vtern'}


def esc(s):
    return s.replace(BS, BS + BS).replace(DQ, BS + DQ)


out = ['// cb/build/x86asm/macros_std.cpp -- the standard macros NASM preloads (macros/*.mac and version.mac, as macros.pl reads them:',
       '// comments and blank lines removed, runs of white space squeezed to one space, continuation lines joined), kept as data.',
       '// One line per entry; the preprocessor feeds them to itself as source lines.', '',
       '#include "cb/build/x86asm/preproc.hpp"', '', 'namespace cb::build::x86asm {', '', 'namespace {', '']
for body, name in blocks:
    if name not in want:
        continue
    lines = [l[3:] if l.startswith(' * ') else l[2:] for l in body.split('\n') if l.startswith(' *')]
    out.append('const std::vector<std::string_view> k_%s = {' % want[name])
    for l in lines:
        out.append('    "%s",' % esc(l))
    out.append('};')
    out.append('')
    print(name, len(lines))
out.append('}  // namespace\n')
out.append('''const std::vector<std::string_view>& standard_macros(StandardMacros which) {
    switch (which) {
        case StandardMacros::Version: return k_version;
        case StandardMacros::Nasm: return k_nasm;
        case StandardMacros::Tasm: return k_tasm;
    }
    return k_nasm;
}

const std::vector<UsePackage>& use_packages() {
    static const std::vector<UsePackage> packages = {
        {"altreg", &k_altreg, 0}, {"fp", &k_fp, 1}, {"ifunc", &k_ifunc, 2},
        {"masm", &k_masm, 3}, {"smartalign", &k_smartalign, 4}, {"vtern", &k_vtern, 5},
    };
    return packages;
}

}  // namespace cb::build::x86asm''')
open('../../../../native/cb/build/x86asm/macros_std.cpp', 'w', encoding='utf8').write('\n'.join(out) + '\n')
