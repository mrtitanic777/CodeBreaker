// codebreaker/source/ne_asm/datacode.cpp -- typed C++ port of codebreaker.source.ne_asm.datacode.

#include "codebreaker/source/ne_asm/datacode.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::datacode {

namespace {

// the module's docstring, which RawDescriptionHelpFormatter prints as the help's description
const char* const DOC = u8R"DOC(
datacode.py — separate DATA from CODE in a segment (readability track #3/#5).

Code segments carry embedded data — strings, jump tables, constant blobs — which
a linear disassembler (objdump) blindly decodes as bogus instructions
(`outs dx,...`, `arpl`, `imul bp,[di+0x61],0x7972`...). Those show up as `db`
byte-dumps and drag down the "live %" even though they were never instructions.

This classifier finds the data regions so asmrecon can emit them as READABLE data
(`db 'primary',0`, `dw L_case1,...`) and exclude them from the instruction/live
count. Two detectors to start:

  * strings  — runs of >= MINSTR printable ASCII (optionally NUL-terminated)
  * garbage  — clusters of the tell-tale "this is not code" mnemonics objdump
               emits over data (outs/ins/arpl/segment-prefix soup)

    python datacode.py MOUSE --seg 1        # list the data regions found
)DOC";

std::ostream* g_out = nullptr;
std::ostream* g_err = nullptr;
std::ostream& out_() { return g_out ? *g_out : std::cout; }
std::ostream& err_() { return g_err ? *g_err : std::cerr; }

bool printable(int64_t b) { return (b >= 0x20 && b < 0x7F) || b == 0x09; }

// the lines of "-" patterns of argparse for this parser ------------------------------------------------------------
constexpr int kNone = -1, kHelp = 0, kSeg = 1;

struct OptionString {
    const char* text;
    int action;
};
// parser._option_string_actions, in the order the actions were added
const OptionString OPTION_STRINGS[] = {{"-h", kHelp}, {"--help", kHelp}, {"--seg", kSeg}};

struct Tuple {
    int action;
    py::str option_string;
    std::optional<py::str> sep;
    std::optional<py::str> explicit_arg;
};

// argparse.ArgumentError, as str() of it is written
struct ArgumentError {
    std::optional<py::str> argument;
    py::str message;
    py::str text() const { return argument ? py::str("argument ") + *argument + py::str(": ") + message : message; }
};

py::str usage_text(const py::str& prog) { return py::str("usage: ") + prog + py::str(" [-h] [--seg SEG] module\n"); }

[[noreturn]] void parser_error(const py::str& prog, const py::str& message) {
    err_() << usage_text(prog).utf8();
    err_() << prog.utf8() << ": error: " << message.utf8() << "\n";
    throw py::SystemExit(2);
}

py::str action_name(int action) { return action == kHelp ? py::str("-h/--help") : py::str("--seg"); }

// parser._get_option_tuples(arg): the abbreviations and the concatenated single-dash forms of an option string
std::vector<Tuple> get_option_tuples(const py::str& arg) {
    std::vector<Tuple> result;
    const std::u32string u = arg.u32();
    const size_t eq = u.find(U'=');
    const py::str option_prefix = eq == std::u32string::npos ? arg : py::str(u.substr(0, eq));
    std::optional<py::str> sep, explicit_arg;
    if (eq != std::u32string::npos) {
        sep = py::str("=");
        explicit_arg = py::str(u.substr(eq + 1));
    }
    if (u[0] == U'-' && u[1] == U'-') {
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text).startswith(option_prefix)) result.push_back({o.action, py::str(o.text), sep, explicit_arg});
        }
    } else if (u[0] == U'-' && u[1] != U'-') {
        const py::str short_option_prefix(u.substr(0, 2));
        const py::str short_explicit_arg(u.substr(2));
        for (const OptionString& o : OPTION_STRINGS) {
            if (py::str(o.text) == short_option_prefix) {
                result.push_back({o.action, py::str(o.text), py::str(""), short_explicit_arg});
            } else if (py::str(o.text).startswith(option_prefix)) {
                result.push_back({o.action, py::str(o.text), sep, explicit_arg});
            }
        }
    }
    return result;
}

// parser._parse_optional(arg): nullopt for a positional
std::optional<std::vector<Tuple>> parse_optional(const py::str& arg) {
    if (arg.empty()) return std::nullopt;
    const std::u32string u = arg.u32();
    if (u[0] != U'-') return std::nullopt;
    for (const OptionString& o : OPTION_STRINGS) {
        if (arg == py::str(o.text)) return std::vector<Tuple>{{o.action, arg, std::nullopt, std::nullopt}};
    }
    if (u.size() == 1) return std::nullopt;
    const size_t eq = u.find(U'=');
    if (eq != std::u32string::npos) {
        const py::str option_string(u.substr(0, eq));
        for (const OptionString& o : OPTION_STRINGS) {
            if (option_string == py::str(o.text)) {
                return std::vector<Tuple>{{o.action, option_string, py::str("="), py::str(u.substr(eq + 1))}};
            }
        }
    }
    std::vector<Tuple> tuples = get_option_tuples(arg);
    if (!tuples.empty()) return tuples;
    // re.compile(r'-\.?\d').match: it looks like a negative number, so it was meant to be positional
    size_t k = 1;
    if (k < u.size() && u[k] == U'.') ++k;
    if (k < u.size() && py::str(std::u32string(1, u[k])).isdigit()) return std::nullopt;
    if (u.find(U' ') != std::u32string::npos) return std::nullopt;
    return std::vector<Tuple>{{kNone, arg, std::nullopt, std::nullopt}};
}

struct Parsed {
    py::str module;
    int64_t seg = 1;
};

// the help text: the usage, the description as it is (RawDescriptionHelpFormatter), the arguments
py::str help_text(const py::str& prog) {
    std::string doc = DOC;
    size_t a = doc.find_first_not_of('\n'), b = doc.find_last_not_of('\n');
    doc = doc.substr(a, b - a + 1);
    return usage_text(prog) + py::str("\n") + py::str(doc) + py::str("\n\npositional arguments:\n  module\n\noptions:\n  -h, --help  show this help message and exit\n  --seg SEG\n");
}

// parser.parse_args(argv[1:]) for this parser (see the typing notes of the header)
Parsed parse_args(const py::str& prog, const std::vector<py::str>& arg_strings) {
    std::vector<std::optional<std::vector<Tuple>>> tuples;
    std::string pattern;
    {
        size_t i = 0;
        for (; i < arg_strings.size(); ++i) {
            if (arg_strings[i] == py::str("--")) {
                pattern += '-';
                tuples.push_back(std::nullopt);
                for (++i; i < arg_strings.size(); ++i) {
                    pattern += 'A';
                    tuples.push_back(std::nullopt);
                }
                break;
            }
            auto t = parse_optional(arg_strings[i]);
            pattern += t ? 'O' : 'A';
            tuples.push_back(t);
        }
    }
    Parsed ns;
    bool module_seen = false;
    std::vector<py::str> extras;
    try {
        auto match_argument = [&](int action, const std::string& selected) -> int64_t {   // parser._match_argument
            if (action == kHelp) return 0;
            if (!selected.empty() && selected[0] == 'A') return 1;
            throw ArgumentError{action_name(action), py::str("expected one argument")};
        };
        auto take_seg = [&](const py::str& value) {
            try {
                ns.seg = py::int_parse(value);
            } catch (const py::ValueError&) {
                throw ArgumentError{py::str("--seg"), py::str("invalid int value: ") + py::repr(value)};
            }
        };
        auto consume_positionals = [&](size_t start) -> size_t {
            if (module_seen) return start;
            size_t j = start;
            if (j < pattern.size() && pattern[j] == '-') ++j;
            if (j >= pattern.size() || pattern[j] != 'A') return start;
            const size_t at = j;
            ++j;
            if (j < pattern.size() && pattern[j] == '-') ++j;
            ns.module = arg_strings[at];
            module_seen = true;
            return j;
        };
        auto consume_optional = [&](size_t start_index) -> size_t {
            const std::vector<Tuple>& option_tuples = *tuples[start_index];
            if (option_tuples.size() > 1) {
                py::list<py::str> names;
                for (const Tuple& t : option_tuples) names.append(t.option_string);
                throw ArgumentError{std::nullopt, py::str("ambiguous option: ") + arg_strings[start_index] + py::str(" could match ") +
                                                      py::join(py::str(", "), names)};
            }
            Tuple cur = option_tuples[0];
            struct Pending {
                int action;
                std::vector<py::str> args;
            };
            std::vector<Pending> action_tuples;
            size_t stop = start_index + 1;
            while (true) {
                if (cur.action == kNone) {
                    extras.push_back(arg_strings[start_index]);
                    return start_index + 1;
                }
                if (cur.explicit_arg) {
                    const int64_t arg_count = match_argument(cur.action, "A");
                    const std::u32string os = cur.option_string.u32();
                    if (arg_count == 0 && os[1] != U'-' && !cur.explicit_arg->empty()) {
                        const std::u32string ea = cur.explicit_arg->u32();
                        if ((cur.sep && !cur.sep->empty()) || ea[0] == U'-') {
                            throw ArgumentError{action_name(cur.action), py::str("ignored explicit argument ") + py::repr(*cur.explicit_arg)};
                        }
                        action_tuples.push_back({cur.action, {}});
                        const py::str option_string = py::str(std::u32string(1, os[0]) + std::u32string(1, ea[0]));
                        int next = kNone;
                        for (const OptionString& o : OPTION_STRINGS) {
                            if (option_string == py::str(o.text)) next = o.action;
                        }
                        if (next != kNone) {
                            cur.action = next;
                            cur.option_string = option_string;
                            const std::u32string rest = ea.substr(1);
                            if (rest.empty()) {
                                cur.sep.reset();
                                cur.explicit_arg.reset();
                            } else if (rest[0] == U'=') {
                                cur.sep = py::str("=");
                                cur.explicit_arg = py::str(rest.substr(1));
                            } else {
                                cur.sep = py::str("");
                                cur.explicit_arg = py::str(rest);
                            }
                        } else {
                            extras.push_back(py::str(std::u32string(1, os[0])) + *cur.explicit_arg);
                            stop = start_index + 1;
                            break;
                        }
                    } else if (arg_count == 1) {
                        stop = start_index + 1;
                        action_tuples.push_back({cur.action, {*cur.explicit_arg}});
                        break;
                    } else {
                        throw ArgumentError{action_name(cur.action), py::str("ignored explicit argument ") + py::repr(*cur.explicit_arg)};
                    }
                } else {
                    const size_t start = start_index + 1;
                    const int64_t arg_count = match_argument(cur.action, pattern.substr(start));
                    stop = start + static_cast<size_t>(arg_count);
                    std::vector<py::str> args(arg_strings.begin() + static_cast<std::ptrdiff_t>(start), arg_strings.begin() + static_cast<std::ptrdiff_t>(stop));
                    action_tuples.push_back({cur.action, args});
                    break;
                }
            }
            for (const Pending& p : action_tuples) {
                if (p.action == kHelp) {
                    out_() << help_text(prog).utf8();
                    throw py::SystemExit(0);
                }
                take_seg(p.args[0]);
            }
            return stop;
        };
        // consume Positionals and Optionals alternately, until we have passed the last option string
        size_t start_index = 0;
        int64_t max_option_string_index = -1;
        for (size_t i = 0; i < tuples.size(); ++i) {
            if (tuples[i]) max_option_string_index = static_cast<int64_t>(i);
        }
        while (static_cast<int64_t>(start_index) <= max_option_string_index) {
            size_t next_option_string_index = start_index;
            while (static_cast<int64_t>(next_option_string_index) <= max_option_string_index) {
                if (tuples[next_option_string_index]) break;
                ++next_option_string_index;
            }
            if (start_index != next_option_string_index) {
                const size_t positionals_end_index = consume_positionals(start_index);
                if (positionals_end_index > start_index) {
                    start_index = positionals_end_index;
                    continue;
                }
                start_index = positionals_end_index;
            }
            if (!tuples[start_index]) {
                for (size_t k = start_index; k < next_option_string_index; ++k) extras.push_back(arg_strings[k]);
                start_index = next_option_string_index;
            }
            start_index = consume_optional(start_index);
        }
        const size_t stop_index = consume_positionals(start_index);
        for (size_t k = stop_index; k < arg_strings.size(); ++k) extras.push_back(arg_strings[k]);
        if (!module_seen) throw ArgumentError{std::nullopt, py::str("the following arguments are required: module")};
    } catch (const ArgumentError& e) {
        parser_error(prog, e.text());
    }
    if (!extras.empty()) {
        py::list<py::str> rest;
        for (const py::str& e : extras) rest.append(e);
        parser_error(prog, py::str("unrecognized arguments: ") + py::join(py::str(" "), rest));
    }
    return ns;
}

}  // namespace

const py::set<py::str> GARBAGE_MN{py::str("outs"), py::str("ins"), py::str("arpl"), py::str("bound"), py::str("aaa"), py::str("aas"),
                                  py::str("daa"),  py::str("das"), py::str("lds"),  py::str("les"),   py::str("sahf"), py::str("lahf"),
                                  py::str("xlat"), py::str("hlt"), py::str("into")};
const py::set<py::str> SEGPFX{py::str("fs"), py::str("gs"), py::str("es"), py::str("ds"), py::str("cs"), py::str("ss"),
                              py::str("lock"), py::str("repz"), py::str("repnz")};

bool _texty(const py::bytes& run) {
    for (int64_t b : run) {
        if (0x61 <= b && b <= 0x7A) return true;
    }
    return false;
}

py::list<std::tuple<int64_t, int64_t, py::str>> find_strings(const py::bytes& code, int64_t minlen) {
    py::list<std::tuple<int64_t, int64_t, py::str>> out;
    int64_t i = 0;
    const int64_t n = code.size();
    while (i < n) {
        if (printable(code[i])) {
            int64_t j = i;
            while (j < n && printable(code[j])) j += 1;
            const py::bytes run = code.slice(i, j);
            if (j - i >= minlen && _texty(run)) {
                const int64_t end = (j < n && code[j] == 0) ? j + 1 : j;   // eat a NUL
                out.append(std::make_tuple(i, end, run.decode("latin1")));
                i = end;
                continue;
            }
        }
        i += 1;
    }
    return out;
}

py::list<std::tuple<int64_t, int64_t>> garbage_spans(const py::dict<int64_t, py::str>& rows, const py::bytes& code) {
    (void)code;
    static const py::re::Pattern ws = py::re::compile(py::str("\\s+"));
    static const py::re::Pattern r32 = py::re::compile(py::str("\\b(eax|ebx|ecx|edx|esi|edi|ebp|esp)\\b"));
    auto is_garbage = [&](const py::str& a0) {
        const py::str a = ws.sub(py::str(" "), a0).strip();
        const py::str mn = py::split(a, py::str(" "))[0];
        // a 32-bit register in 16-bit code is objdump decoding data (0x66/0x67 prefix bytes) -- 16-bit modules
        // never use eax/edi/...
        return GARBAGE_MN.contains(mn) || (SEGPFX.contains(mn) && a.contains(py::str(" "))) || a.contains(py::str("(bad)")) ||
               static_cast<bool>(r32.search(a));
    };
    py::list<std::tuple<int64_t, int64_t>> spans;
    std::vector<int64_t> run;
    std::vector<int64_t> srt = rows.keys();
    std::sort(srt.begin(), srt.end());
    // idx[o]: the offset after o in the sorted rows (None for the last)
    py::dict<int64_t, std::optional<int64_t>> idx;
    for (size_t k = 0; k < srt.size(); ++k) {
        idx.set(srt[k], k + 1 < srt.size() ? std::optional<int64_t>(srt[k + 1]) : std::nullopt);
    }
    auto close = [&]() {
        const std::optional<int64_t> next = idx.at(run.back());
        spans.append(std::make_tuple(run.front(), (next && *next != 0) ? *next : run.back()));   // idx[run[-1]] or run[-1]
    };
    for (int64_t o : srt) {
        if (is_garbage(rows.at(o))) {
            run.push_back(o);
        } else {
            if (run.size() >= 3) close();
            run.clear();
        }
    }
    if (run.size() >= 3) close();
    return spans;
}

py::list<Region> data_regions(const segment::Seg& sg) {
    const py::bytes& code = sg.code;
    py::dict<int64_t, py::str> rows;
    for (const auto& oa : sg.rows) rows.set(std::get<0>(oa), std::get<1>(oa));
    std::vector<Region> spans;
    for (const auto& s : find_strings(code)) spans.emplace_back(std::get<0>(s), std::get<1>(s), py::str("str"));
    for (const auto& s : garbage_spans(rows, code)) spans.emplace_back(std::get<0>(s), std::get<1>(s), py::str("data"));
    std::sort(spans.begin(), spans.end(), [](const Region& a, const Region& b) {   // spans.sort(): tuples (start, end, kind)
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
        if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
        return std::get<2>(a).u32() < std::get<2>(b).u32();
    });
    py::list<Region> merged;
    for (const Region& r : spans) {
        const int64_t s = std::get<0>(r), e = std::get<1>(r);
        if (!merged.empty() && s <= std::get<1>(merged[-1])) {
            const Region last = merged[-1];
            merged[-1] = Region(std::get<0>(last), std::max(std::get<1>(last), e), std::get<2>(last));
        } else {
            merged.append(r);
        }
    }
    return merged;
}

void set_streams(std::ostream& out, std::ostream& err) {
    g_out = &out;
    g_err = &err;
}

void main(const py::list<py::str>& argv) {
    if (argv.empty()) throw py::IndexError("list index out of range");
    const py::str prog = py::os::path::basename(argv[0]);
    std::vector<py::str> rest;
    for (int64_t i = 1; i < argv.size(); ++i) rest.push_back(argv[i]);
    const Parsed args = parse_args(prog, rest);
    const py::dict<py::str, py::str> o = segment::load_ordinals(py::str("ORDINALS.md"));
    const std::optional<py::str> found = base::corpus::retail(args.module);
    const formats::ne::NEFile ne(found ? *found : args.module);
    const segment::Seg sg(ne, args.seg, o);
    const py::list<Region> regs = data_regions(sg);
    int64_t tot = 0;
    for (const Region& r : regs) tot += std::get<1>(r) - std::get<0>(r);
    py::print(py::percent(py::str("%s seg%d: %d data region(s), %d bytes (%.1f%% of %d)"), args.module, args.seg, regs.size(), tot,
                          100.0 * static_cast<double>(tot) / static_cast<double>(std::max<int64_t>(1, sg.code.size())), sg.code.size()));
    for (int64_t k = 0; k < regs.size() && k < 40; ++k) {
        const int64_t s = std::get<0>(regs[k]), e = std::get<1>(regs[k]);
        const py::bytes preview = sg.code.slice(s, std::min(e, s + 24));
        std::u32string txt;
        for (int64_t b : preview) txt.push_back((0x20 <= b && b < 0x7f) ? static_cast<char32_t>(b) : U'.');
        py::print(py::percent(py::str("  %04x-%04x  %-4s  %3dB  |%s|"), s, e, std::get<2>(regs[k]), e - s, py::str(txt)));
    }
}

}  // namespace codebreaker::source::ne_asm::datacode
