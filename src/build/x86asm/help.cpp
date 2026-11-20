// build/x86asm/help.cpp -- the text of `nasm -h` (the help function of NASM's asm/nasm.c).
#include "build/x86asm/help.hpp"

#include <cstring>
#include <format>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/format.hpp"
#include "build/x86asm/nctype.hpp"
#include "build/x86asm/warnings.hpp"

namespace cb::build::x86asm {

namespace drvns_help {

enum HelpWith : int { HW_ALL = 0, HW_OPT = 1, HW_ERR = 2, HW_LIMIT = 3 };

bool help_ieq(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (nasm_tolower(a[i]) != nasm_tolower(b[i])) return false;
    return true;
}
bool help_istarts(std::string_view s, std::string_view p) { return s.size() >= p.size() && help_ieq(s.substr(0, p.size()), p); }

bool help_is(int with, int h) { return with == HW_ALL || with == h; }
bool help_opt(int with) { return help_is(with, HW_OPT); }
bool help_optor(int with, int h) { return help_opt(with) || with == h; }

}  // namespace drvns_help
using namespace drvns_help;

std::string nasm_help(std::string_view what, std::string_view progname, std::string_view current_format) {
    std::string out;
    int with = HW_ERR;

    auto SEEx = [&](const char* x, const char* y) -> std::string { return with == HW_OPT ? std::string(" (see -h ") + x + ")" : std::string(y); };
    auto SEE = [&](const char* x) { return SEEx(x, ""); };

    if (what.empty() || what == "-" || what == "--" || help_ieq(what, "opt") || help_ieq(what, "run") || help_ieq(what, "cmd")) {
        with = HW_OPT;
    } else if (help_ieq(what, "all")) {
        with = HW_ALL;
    } else if (help_istarts(what, "--limit") || help_istarts(what, "limit")) {
        with = HW_LIMIT;
    } else if (help_ieq(what, "--help") || help_ieq(what, "help") || help_ieq(what, "list") || help_ieq(what, "topics") || help_ieq(what, "index")) {
        with = 'h';
    } else if ((what[0] == '-' && what.size() == 2) || what.size() == 1) {
        with = what[0] == '-' ? what[1] : what[0];
        switch (with) {
        case 'h':
        case 'X':
        case 'L':
        case 'f':
        case 'F':
        case 'O':
        case 'w':
        case 'M': break;
        case 'g': with = 'F'; break;
        case 'W': with = 'w'; break;
        case '?': with = 'h'; break;
        case '*': with = HW_ALL; break;
        default: with = HW_ERR; break;
        }
    }

    if (with == HW_ERR) return std::format("Sorry, no help available for `{}'\n", what);

    if (help_opt(with)) {
        out += std::format(
            "Usage: {} [-@ response_file] [options...] [--] filename\n"
            "  Options:\n"
            "    -v (or --v)    print the NASM version number and exit\n"
            "    -@ file        response file; one command line option per line\n",
            progname);
    }
    if (help_optor(with, 'h')) {
        out += "    -h             list command line options and exit (also --help)\n";
        if (with == HW_OPT) out += "    -h -opt        show additional help for option \"-opt\"\n";
        out +=
            "    -h all         show all available command line help\n"
            "    -h topics      show list of help topics (also -h list)\n";
    }
    if (help_is(with, 'h')) {
        out +=
            "    -h -X          show list of error message formats\n"
            "    -h -M          show list of dependency generation options\n"
            "    -h -f          show list of object code output formats\n"
            "    -h -F          show list of debug information formats\n"
            "    -h -L          show list of list file option flags\n"
            "    -h -O          show list of optimization flags\n"
            "    -h -w          show list of warning classes and defaults\n"
            "    -h --limit     show list of resource limits and defaults\n";
    }
    if (help_opt(with)) {
        out +=
            "    -o outfile     write output to outfile\n"
            "    --keep-all     output files will not be removed even if an error happens\n";
    }
    if (help_optor(with, 'X')) out += std::format("    -Xformat       specify error reporting format{}\n", SEE("-X"));
    if (help_is(with, 'X')) {
        out +=
            "    -Xgnu          report errors in GNU format (default)\n"
            "    -Xgcc          alias for -Xgnu\n"
            "    -Xvc           report errors in Visual Studio format\n"
            "    -Xmsvc         alias for -Xvc\n"
            "    -Xms           alias for -Xvc\n";
    }
    if (help_opt(with)) {
        out +=
            "    -s             redirect messages to stdout\n"
            "    -Zfile         redirect messages to file\n"
            "    --info[=lvl]   display optional informational messages\n"
            "    --debug[=lvl]  display NASM internal debugging messages\n";
    }
    if (help_optor(with, 'M')) {
        out += std::format("    -M...          generate Makefile dependencies{}\n",
                           with == HW_OPT ? " (see -h -M)" : " (multiple options permitted):");
    }
    if (help_is(with, 'M')) {
        out +=
            "    -M             generate Makefile dependencies on stdout\n"
            "    -MG            d:o, missing files assumed generated\n"
            "    -MF file       set Makefile dependency file\n"
            "    -MD file       assemble and generate dependencies\n"
            "    -MT file       dependency target name\n"
            "    -MQ file       dependency target name (quoted)\n"
            "    -MP            emit phony targets\n"
            "    -MW            generate output in Watcom wmake syntax\n";
    }
    if (help_optor(with, 'f')) out += std::format("    -f format      select output file format{}\n", SEE("-f"));
    if (help_is(with, 'f')) {
        for (const FormatName& f : list_formats()) {
            if (f.alias_of.empty())
                out += std::format("       {:<20} {}{}\n", f.shortname, f.fullname, f.shortname == current_format ? " [default]" : "");
            else
                out += std::format("       {:<20} Alias for \"{}\"\n", f.shortname, f.alias_of);
        }
    }
    if (help_optor(with, 'F')) {
        out += std::format(
            "    -g             generate debugging information\n"
            "    -F format      select a debugging format{}\n"
            "    -gformat       same as -g -F format\n",
            SEE("-F"));
    }
    if (help_is(with, 'F')) {
        // (No output format of x86asm has a debug format besides the null one, which is not listed.)
    }
    if (help_optor(with, 'L')) {
        out += std::format(
            "    -l listfile    write listing to a list file\n"
            "    -Lflags...     add information to the list file{}\n",
            SEE("-L"));
    }
    if (help_is(with, 'L')) {
        out +=
            "                -L flags for general usage:\n"
            "       -Lb         show builtin macro packages (standard and %use)\n"
            "       -Lc         list the data contents of INCBIN files\n"
            "       -Ld         show byte and repeat counts in decimal, not hex\n"
            "       -Le         show the preprocessed output\n"
            "       -Lf         ignore .nolist (force output)\n"
            "       -LF         ignore [LIST -] directives (force output)\n"
            "       -Lm         show multi-line macro calls with expanded parameters\n"
            "       -Lp         output a list file every pass, in case of errors\n"
            "       -Ls         show all single-line macro definitions\n"
            "       -Lt         list the full output from TIMES and ALIGN directives\n"
            "       -L+         equivalent to -LbdefFmps\n"
            "       -L++        equivalent to -L+ -Lct\n"
            "                -L flags intended for debugging NASM:\n"
            "       -Lw         flush the output after every line (very slow!)\n"
            "       -LX         list instruction pattern line numbers from insns.xda\n"
            "       -L!         enable ALL listing options including debugging\n";
    }
    if (help_optor(with, 'O')) out += std::format("    -Oflags...     select optimization{}\n", SEE("-O"));
    if (help_is(with, 'O')) {
        out +=
            "       -O0       no optimization\n"
            "       -O1       minimal optimization\n"
            "       -Ox       multipass optimization (default, recommended)\n"
            "       -Ov       display the number of passes executed at the end\n";
    }
    if (help_opt(with)) {
        out +=
            "    -t             assemble in limited SciTech TASM compatible mode\n"
            "    -E (or -e)     preprocess only (writes output to stdout by default)\n"
            "    -a             don't preprocess (assemble only)\n"
            "    -Ipath         add a pathname to the include file path\n"
            "    -Pfile         pre-include a file (also --include)\n"
            "    -Dmacro[=str]  pre-define a macro\n"
            "    -Umacro        undefine a macro\n";
    }
    if (help_optor(with, 'w')) {
        out += std::format(
            "    -w+x           enable warning x {}(also -Wx)\n"
            "    -w-x           disable warning x (also -Wno-x)\n"
            "    -w[+-]error    promote all warnings to errors (also -Werror)\n"
            "    -w[+-]error=x  promote warning x to errors (also -Werror=x)\n",
            SEE("-w"));
    }
    if (help_is(with, 'w')) {
        out += "      Defaults in brackets:\n";

        out += std::format("       {:<20} {}\n", kWarningName[WARN_IDX_ALL], kWarningHelp[WARN_IDX_ALL]);

        for (std::size_t i = 1; i < WARN_IDX_ALL; i++) {
            static const char nl_indent[] = "\n                            ";
            const std::string_view me = kWarningName[i];
            const char* prev = kWarningName[i - 1];
            const std::string_view next = kWarningName[i + 1];

            if (prev) {
                const std::size_t prev_len = std::strlen(prev);
                std::size_t dash_at = me.find('-', 1);
                while (dash_at != std::string_view::npos) {
                    const std::size_t prefix_len = dash_at;   // not including final dash
                    if (next.compare(0, prefix_len + 1, me.substr(0, prefix_len + 1)) != 0) {
                        // Only one or last option with this prefix.
                        break;
                    }
                    if (prefix_len >= prev_len || std::strncmp(prev, me.data(), prefix_len) != 0 ||
                        (prev[prefix_len] != '-' && prev[prefix_len] != '\0')) {
                        // This prefix is different from the previous option.
                        out += std::format("       {:<20}{}all warnings prefixed with \"{}\"\n", me.substr(0, prefix_len),
                                           prefix_len > 19 ? nl_indent : " ", me.substr(0, prefix_len + 1));
                    }
                    dash_at = me.find('-', dash_at + 1);
                }
            }

            const std::uint8_t def = kWarningDefault[i];
            out += std::format("       {:<20}{}{}{}\n", me, me.size() > 19 ? nl_indent : " ", kWarningHelp[i],
                               (def & WARN_ST_ERROR) ? " [error]" : (def & WARN_ST_ENABLED) ? " [on]" : " [off]");
        }
    }
    if (help_opt(with)) {
        out +=
            "    --pragma str   pre-executes a specific %pragma\n"
            "    --before str   add line (usually a preprocessor statement) before the input\n"
            "    --bits nn      set bits to nn (equivalent to --before \"BITS nn\")\n"
            "    --no-line      ignore %line directives in input\n"
            "    --gprefix str  prepend the given string to the names of all extern,\n"
            "                   common and global symbols (also --prefix)\n"
            "    --gpostfix str append the given string to the names of all extern,\n"
            "                   common and global symbols (also --postfix)\n"
            "    --lprefix str  prepend the given string to local symbols\n"
            "    --lpostfix str append the given string to local symbols\n"
            "    --reproducible attempt to produce run-to-run identical output\n"
            "    --masm         accept MASM 6.x source compatibility syntax\n";
    }
    if (help_is(with, HW_LIMIT)) {
        out += "    --limit-* val  set execution limit:\n";
        static constexpr const char* kLimitHelp[LIMIT_MAX] = {"total number of passes",
                                                              "number of passes without forward progress",
                                                              "levels of macro expansion",
                                                              "tokens processed during single-line macro expansion",
                                                              "multi-line macros before final return",
                                                              "%rep count",
                                                              "expression evaluation descent",
                                                              "total source lines processed",
                                                              "multi-line macro parameter count"};
        for (int i = 0; i < LIMIT_MAX; i++) {
            out += std::format("       {:<20} {}\n       {:<20} [default {}", Assembly::limit_name(static_cast<NasmLimit>(i)),
                               kLimitHelp[i], "",
                               Assembly::limit_str(Assembly::limit_default(static_cast<NasmLimit>(i))));
            out += std::format(", max {}]\n", Assembly::limit_str(Assembly::limit_max(static_cast<NasmLimit>(i))));
        }
    } else if (help_optor(with, HW_LIMIT)) {
        out += std::format("    --limit-* val  set execution limit{}\n", SEE("--limit"));
    }
    return out;
}

}  // namespace cb::build::x86asm
