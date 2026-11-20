// build/x86asm/driver.cpp -- x86asm's driver: NASM's asm/nasm.c.
//
// `run` is one complete run of nasm: the command line (both passes of it, the response files, the long options), the setup
// of the modules around the assembler (scanner, evaluator, label table, preprocessor, parser, directives, encoder, listing,
// the output format), the pass loop with its optimization passes and their convergence, and the end of the run: the files
// that were written, the messages, the exit status.
//
// NASM keeps the state of all this in file statics; here it is the members of `Engine`, one per run. The files it reads
// come from the request (full paths, case-blind on Windows; the `read` callback for the rest) and the files it writes go
// into the result under the names it was given.
#include "build/x86asm/driver.hpp"

#include <algorithm>
#include <cctype>
#include <climits>
#include <cstring>
#include <ctime>
#include <format>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/directives.hpp"
#include "build/x86asm/encode.hpp"
#include "build/x86asm/eval.hpp"
#include "build/x86asm/floats.hpp"
#include "build/x86asm/format.hpp"
#include "build/x86asm/help.hpp"
#include "build/x86asm/labels.hpp"
#include "build/x86asm/listing.hpp"
#include "build/x86asm/nctype.hpp"
#include "build/x86asm/parse.hpp"
#include "build/x86asm/preproc.hpp"
#include "build/x86asm/quote.hpp"
#include "build/x86asm/scan.hpp"
#include "build/x86asm/tokhash.hpp"

namespace cb::build::x86asm {

namespace drvns_driver {

// The `operating_mode` bits.
constexpr unsigned OP_NORMAL = 1u << 0;
constexpr unsigned OP_PREPROCESS = 1u << 1;
constexpr unsigned OP_DEPEND = 1u << 2;

bool drv_ieq(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (nasm_tolower(a[i]) != nasm_tolower(b[i])) return false;
    return true;
}
bool drv_istarts(std::string_view s, std::string_view prefix) { return s.size() >= prefix.size() && drv_ieq(s.substr(0, prefix.size()), prefix); }

std::string_view skip_spaces(std::string_view s) {
    while (!s.empty() && nasm_isspace(s.front())) s.remove_prefix(1);
    return s;
}

// ---- file names ----

// A path in the form the run's files are keyed by: '/' separators, "." and ".." folded, lower case on Windows.
std::string normalize_path(std::string path, bool fold_case) {
    for (char& c : path)
        if (c == '\\') c = '/';
    // Split off a root ("C:/", "/", "//server/").
    std::string root;
    std::size_t start = 0;
    if (path.size() >= 2 && path[1] == ':' && std::isalpha(static_cast<unsigned char>(path[0]))) {
        root = path.substr(0, 2);
        start = 2;
    }
    if (start < path.size() && path[start] == '/') {
        root += '/';
        ++start;
    }
    std::vector<std::string> parts;
    std::size_t i = start;
    while (i <= path.size()) {
        std::size_t j = path.find('/', i);
        if (j == std::string::npos) j = path.size();
        const std::string part = path.substr(i, j - i);
        if (part.empty() || part == ".") {
            // skip
        } else if (part == ".." && !parts.empty() && parts.back() != "..") {
            parts.pop_back();
        } else if (part == ".." && !root.empty() && root.back() == '/') {
            // above the root: stay at the root
        } else {
            parts.push_back(part);
        }
        i = j + 1;
    }
    std::string out = root;
    for (std::size_t k = 0; k < parts.size(); ++k) {
        if (k) out += '/';
        out += parts[k];
    }
    if (fold_case)
        for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

bool is_absolute_path(std::string_view p) {
    if (p.empty()) return false;
    if (p[0] == '/' || p[0] == '\\') return true;
    return p.size() >= 2 && p[1] == ':' && std::isalpha(static_cast<unsigned char>(p[0]));
}

// The files of one run: the request's, by full path.
class RunFiles {
public:
    explicit RunFiles(const Request& request) : request_(request) {
#if defined(_WIN32)
        fold_ = true;
#endif
        for (const auto& [name, data] : request.files) by_key_[normalize_path(name, fold_)] = &data;
    }

    // The full form of a name the run was given: made absolute against the request's folder, '/'-separated.
    std::string full(std::string_view name) const {
        std::string n(name);
        if (!is_absolute_path(n) && !request_.cwd.empty()) n = request_.cwd + "/" + n;
        return normalize_path(n, false);
    }

    std::optional<Bytes> read(std::string_view name) const {
        const std::string full_name = full(name);
        const auto it = by_key_.find(normalize_path(full_name, fold_));
        if (it != by_key_.end()) return *it->second;
        if (request_.read) return request_.read(full_name);
        return std::nullopt;
    }

private:
    const Request& request_;
    bool fold_ = false;
    std::unordered_map<std::string, const Bytes*> by_key_;
};

// "\n" -> "\r\n": what a text-mode file written on Windows holds.
std::string crlf(std::string_view text) {
    std::string out;
    out.reserve(text.size() + text.size() / 32);
    for (const char c : text) {
        if (c == '\n') out += '\r';
        out += c;
    }
    return out;
}
Bytes crlf_bytes(const Bytes& data) {
    Bytes out;
    out.reserve(data.size() + data.size() / 32);
    for (const std::uint8_t c : data) {
        if (c == '\n') out.push_back('\r');
        out.push_back(c);
    }
    return out;
}

// path.c's filename_set_extension: the name with its extension replaced by `extension` (which includes the dot; empty for none).
std::string filename_set_extension(std::string_view inname, std::string_view extension) {
    std::size_t ep = inname.size();
    for (std::size_t p = inname.size(); p-- > 0;) {
        const char c = inname[p];
        if (c == '.') {
            ep = p;   // found the beginning of an existing extension
            break;
        }
        if (c == '/' || c == '\\' || c == ':') {
            ep = p + 1;   // found the beginning of the file name
            break;
        }
    }
    std::string out(inname.substr(0, ep));
    if (!extension.empty()) {
        out += '.';
        out += extension.substr(1);
    }
    return out;
}

// Quote a filename string if and only if it is necessary: it contains control characters, starts or ends with a space or
// quote mark, contains more than one space in a row, or is empty (nasm_quote_filename).
std::string nasm_quote_filename(std::string_view fn) {
    if (fn.empty()) return "\"\"";
    bool quote = false;
    const unsigned char first = static_cast<unsigned char>(fn.front());
    if (first <= ' ' || nasm_isquote(fn.front())) {
        quote = true;
    } else {
        unsigned char cutoff = ' ';
        for (const char ch : fn) {
            const unsigned char c = static_cast<unsigned char>(ch);
            if (c < cutoff) {
                quote = true;
                break;
            }
            cutoff = static_cast<unsigned char>(' ' + (c == ' '));
        }
        if (!quote && (static_cast<unsigned char>(fn.back()) <= ' ' || nasm_isquote(fn.back()))) quote = true;
    }
    return quote ? nasm_quote(fn) : std::string(fn);
}

// Convert a string to a POSIX make-safe form (quote_for_pmake).
std::string quote_for_pmake(std::string_view str) {
    std::string out;
    std::size_t nbs = 0;
    for (const char c : str) {
        switch (c) {
        case ' ':
        case '\t':
            out.append(nbs, '\\');
            out += '\\';
            out += c;
            nbs = 0;
            break;
        case '$':
            out += c;
            out += c;
            nbs = 0;
            break;
        case '#':
            out += '\\';
            out += c;
            nbs = 0;
            break;
        case '\\':
            out += c;
            nbs++;
            break;
        default:
            out += c;
            nbs = 0;
            break;
        }
    }
    out.append(nbs, '\\');   // N backslashes at the end of a filename become 2N
    return out;
}

// Convert a string to a Watcom make-safe form (quote_for_wmake).
std::string quote_for_wmake(std::string_view str) {
    bool quote = false;
    for (const char c : str)
        if (c == ' ' || c == '\t' || c == '&' || c == '"') quote = true;
    std::string out;
    if (quote) out += '"';
    for (const char c : str) {
        switch (c) {
        case '$':
        case '#':
            out += '$';
            out += c;
            break;
        case '"':
            out += c;
            out += c;
            break;
        default: out += c; break;
        }
    }
    if (quote) out += '"';
    return out;
}

// What a long option (`--name`) is (the textopts table).
enum class TextOpt { Bogus, Version, Help, AbortOnPanic, Mangle, Include, Pragma, Before, Limit, KeepAll, NoLine, Debug, Info, Reproducible, Masm, Bits };
enum class NeedArg { No, Yes, Maybe };
struct TextArg {
    const char* label;
    TextOpt opt;
    NeedArg need_arg;
    Directive pvt;
};
constexpr TextArg kTextOpts[] = {
    {"v", TextOpt::Version, NeedArg::No, D_none},
    {"version", TextOpt::Version, NeedArg::No, D_none},
    {"help", TextOpt::Help, NeedArg::Maybe, D_none},
    {"abort-on-panic", TextOpt::AbortOnPanic, NeedArg::No, D_none},
    {"prefix", TextOpt::Mangle, NeedArg::Yes, D_PREFIX},
    {"postfix", TextOpt::Mangle, NeedArg::Yes, D_POSTFIX},
    {"suffix", TextOpt::Mangle, NeedArg::Yes, D_SUFFIX},
    {"gprefix", TextOpt::Mangle, NeedArg::Yes, D_GPREFIX},
    {"gpostfix", TextOpt::Mangle, NeedArg::Yes, D_GPOSTFIX},
    {"gsuffix", TextOpt::Mangle, NeedArg::Yes, D_GSUFFIX},
    {"lprefix", TextOpt::Mangle, NeedArg::Yes, D_LPREFIX},
    {"lpostfix", TextOpt::Mangle, NeedArg::Yes, D_LPOSTFIX},
    {"lsuffix", TextOpt::Mangle, NeedArg::Yes, D_LSUFFIX},
    {"include", TextOpt::Include, NeedArg::Yes, D_none},
    {"pragma", TextOpt::Pragma, NeedArg::Yes, D_none},
    {"before", TextOpt::Before, NeedArg::Yes, D_none},
    {"limit-", TextOpt::Limit, NeedArg::Yes, D_none},
    {"keep-all", TextOpt::KeepAll, NeedArg::No, D_none},
    {"no-line", TextOpt::NoLine, NeedArg::No, D_none},
    {"info", TextOpt::Info, NeedArg::Maybe, D_none},
    {"verbose", TextOpt::Info, NeedArg::Maybe, D_none},
    {"debug", TextOpt::Debug, NeedArg::Maybe, D_none},
    {"reproducible", TextOpt::Reproducible, NeedArg::No, D_none},
    {"masm", TextOpt::Masm, NeedArg::No, D_none},
    {"bits", TextOpt::Bits, NeedArg::Yes, D_none},
};

LabelMangle mangle_of(Directive d, bool& ok) {
    ok = true;
    switch (d) {
    case D_PREFIX:
    case D_GPREFIX: return LabelMangle::GlobalPrefix;
    case D_SUFFIX:
    case D_GSUFFIX:
    case D_POSTFIX:
    case D_GPOSTFIX: return LabelMangle::GlobalSuffix;
    case D_LPREFIX: return LabelMangle::LocalPrefix;
    case D_LSUFFIX:
    case D_LPOSTFIX: return LabelMangle::LocalSuffix;
    default: ok = false; return LabelMangle::GlobalPrefix;
    }
}

// A run ends early (`-h`, `-v`): the status to exit with.
struct ExitRun {
    int code;
};

// ---- the engine ----

class Engine final : public FormatHost {
public:
    Engine(const Request& request, bool text_crlf)
        : request_(request),
          files_(request),
          crlf_(text_crlf),
          pscan_(as_),
          dscan_(as_),
          tscan_(as_),
          floats_(as_),
          labels_(as_),
          eval_(as_, labels_, floats_),
          listing_(as_, [this](std::string_view name) { return open_list(name); }) {}

    RunResult run();

    // ---- FormatHost ----
    Assembly& assembly() override { return as_; }
    Bytes& ofile() override { return ofile_; }
    bool ofile_is_text() const override { return ofmt_ && (ofmt_->flags() & OFMT_TEXT); }
    Bytes* open_write(std::string_view name, bool text) override {
        Extra& e = extra_[std::string(name)];
        e.text = text;
        e.data.clear();
        return &e.data;
    }
    void stdout_write(std::string_view text) override { out_.append(text); }
    void stderr_write(std::string_view text) override { err_.append(text); }
    std::optional<Expr> evaluate_text(std::string_view text, int critical) override {
        tscan_.reset(text);
        TokenVal tokval;
        tokval.type = TokenType::INVALID;
        return eval_.evaluate(scan_fn(tscan_), tokval, nullptr, critical != 0, nullptr);
    }
    void define_label(std::string_view name, std::int32_t segment, std::int64_t offset, bool is_norm) override {
        labels_.define_label(name, segment, offset, is_norm);
    }
    LabelType lookup_label(std::string_view name, std::int32_t& segment, std::int64_t& offset) override {
        return labels_.lookup_label(name, segment, offset);
    }
    std::int32_t seg_alloc() override { return segs_.alloc(); }
    void backend_label(std::string_view name, std::int32_t segment, std::int64_t offset) override { labels_.backend_label(name, segment, offset); }
    ExprAtFront evaluate_critical(std::string_view text) override {
        ExprAtFront r;
        tscan_.reset(text);
        TokenVal tokval;
        tokval.type = TokenType::INVALID;
        const std::optional<Expr> e = eval_.evaluate(scan_fn(tscan_), tokval, nullptr, true, nullptr);
        r.consumed = text.size() - tscan_.tell().size();
        if (e) {
            r.ok = true;
            r.simple = is_simple(*e);
            r.value = reloc_value(*e);
        }
        return r;
    }
    void pre_define(std::string_view definition) override {
        if (pp_) pp_->pre_define(definition);
    }
    void get_boolean_option(std::string_view text, bool& value) override;
    std::vector<DependencyFile> dependency_files() override { return {}; }

private:
    struct Extra {
        Bytes data;
        bool text = false;
    };

    static EvalScanFn scan_fn(Scanner& sc) {
        return [&sc](TokenVal& tv) { return static_cast<TokenType>(sc.scan(tv)); };
    }

    Bytes* open_list(std::string_view name) {
        list_name_written_ = std::string(name);
        return &list_file_;
    }

    bool terminate_after_phase() const { return as_.terminate_after_phase(); }

    // ---- the command line ----
    bool process_arg(std::string_view p, std::optional<std::string_view> q, int pass);
    void process_respfile_text(std::string_view text, int pass);
    void process_args(std::string_view args, int pass);
    void process_response_file(std::string_view file, int pass);
    void open_and_process_respfile(std::string_view respfile, int pass);
    void parse_cmdline(int pass);
    void copy_filename(std::optional<std::string>& dst, std::string_view src, const char* what);
    std::optional<std::string_view> get_opt_param(std::string_view p, std::optional<std::string_view> q, bool& advance);
    std::optional<std::string_view> get_param(std::string_view p, std::optional<std::string_view> q, bool& advance);
    [[noreturn]] void show_help(std::string_view what);
    [[noreturn]] void show_version();

    // ---- setup ----
    void define_macros();
    void preproc_init();
    void make_modules();
    void emit_dependencies();

    // ---- the work ----
    void assemble_file(const std::string& fname);
    void forward_refs_and_process(Insn& ins);
    void preprocess_only();
    void depend_only();
    void close_output(bool error);
    RunResult finish(int exit_code);

    const Request& request_;
    RunFiles files_;
    bool crlf_;

    Assembly as_;
    std::string out_, err_;       // what was written to stdout and to stderr
    bool errors_to_stdout_ = false;   // -s

    // The options (the file statics of nasm.c).
    std::optional<std::string> listname_, errname_;
    bool using_debug_info_ = false;
    std::string debug_format_;
    bool have_debug_format_ = false;
    bool keep_all_ = false;
    std::vector<std::string> include_path_;
    int ppopt_ = 0;
    unsigned operating_mode_ = OP_NORMAL;
    bool depend_emit_phony_ = false;
    bool depend_missing_ok_ = false;
    std::optional<std::string> depend_target_, depend_file_;
    bool wmake_ = false;   // quote_for_make is quote_for_wmake
    std::optional<std::vector<std::string>> depend_list_;
    bool stopoptions_ = false;
    bool inname_set_ = false, outname_set_ = false;
    std::unique_ptr<OutputFormat> ofmt_;
    std::string ofmt_alias_;
    const DebugFormat* dfmt_ = nullptr;
    bool format_given_ = false;
    std::string format_name_ = "bin";

    // The output.
    Bytes ofile_;
    bool ofile_open_ = false;
    std::map<std::string, Extra> extra_;
    Bytes list_file_;
    std::string list_name_written_;
    bool list_removed_ = false;
    std::map<std::string, Bytes> result_files_;

    // The modules.
    Scanner pscan_, dscan_, tscan_;
    Floats floats_;
    LabelTable labels_;
    Evaluator eval_;
    Listing listing_;
    SegmentAllocator segs_;
    std::unique_ptr<Preproc> pp_;
    std::unique_ptr<Directives> directives_;
    std::unique_ptr<Parser> parser_;
    std::unique_ptr<Encoder> encoder_;

    // The command line arguments of the run being parsed (response files add to the stream).
    std::vector<std::string> args_;
};

// ---- the command line ----

std::optional<std::string_view> Engine::get_opt_param(std::string_view p, std::optional<std::string_view> q, bool& advance) {
    advance = false;
    if (p.size() > 2) return skip_spaces(p.substr(2));   // the parameter's in the option
    if (q && !q->empty()) {
        advance = true;
        return q;
    }
    return std::nullopt;
}

std::optional<std::string_view> Engine::get_param(std::string_view p, std::optional<std::string_view> q, bool& advance) {
    const auto r = get_opt_param(p, q, advance);
    if (!r) as_.nonfatalf(ERR_USAGE, "option `-{}' requires an argument", p[1]);
    return r;
}

void Engine::copy_filename(std::optional<std::string>& dst, std::string_view src, const char* what) {
    if (dst) as_.fatal("more than one {} file specified: {}\n", what, src);
    dst = std::string(src);
}

void Engine::show_version() {
    out_ += "NASM version 3.02 compiled by CodeBreaker x86asm\n";
    throw ExitRun{0};
}

void Engine::show_help(std::string_view what) {
    out_ += nasm_help(what, as_.progname, ofmt_ ? ofmt_->shortname() : std::string_view(format_name_));
    throw ExitRun{0};
}

bool Engine::process_arg(std::string_view p, std::optional<std::string_view> q, int pass) {
    std::optional<std::string_view> param;
    bool advance = false;

    if (p.empty()) return false;

    if (p[0] == '-' && !stopoptions_) {
        const char opt = p.size() > 1 ? p[1] : '\0';
        if (opt && std::strchr("oOfpPdDiIlLFXuUZwW@", opt)) {
            // These parameters take values.
            if (!(param = get_param(p, q, advance))) return advance;
        }

        switch (opt) {
        case '@':
            open_and_process_respfile(*param, pass);
            break;
        case 's':
            if (pass == 1) errors_to_stdout_ = true;
            break;

        case 'o':   // output file
            if (pass == 2) {
                if (as_.outname.empty() && !outname_set_) {
                    as_.outname = std::string(*param);
                    outname_set_ = true;
                } else {
                    as_.fatal("more than one {} file specified: {}\n", "output", *param);
                }
            }
            break;

        case 'f':   // output format
            if (pass == 1) {
                std::unique_ptr<OutputFormat> f = make_format(*param, *this, &ofmt_alias_);
                if (!f) as_.fatalf(ERR_USAGE, "unrecognised output format `{}' - use -hf for a list", *param);
                ofmt_ = std::move(f);
                format_given_ = true;
            }
            break;

        case 'O':   // optimization level
            if (pass == 1) {
                std::string_view prm = *param;
                as_.optimizing_set = true;

                if (prm.empty()) {
                    as_.optimizing = OPTIM_ALL_ENABLED;   // naked -O == -Ox
                } else {
                    while (!prm.empty()) {
                        const char c = prm.front();
                        if (c >= '0' && c <= '9') {
                            std::size_t n = 0;
                            unsigned long long v = 0;
                            while (n < prm.size() && prm[n] >= '0' && prm[n] <= '9') v = v * 10 + static_cast<unsigned>(prm[n++] - '0');
                            prm.remove_prefix(n);
                            switch (v) {
                            case 0: as_.optimizing = OPTIM_LEVEL_0; break;
                            case 1: as_.optimizing = OPTIM_LEVEL_1; break;
                            default: as_.optimizing = OPTIM_ALL_ENABLED; break;
                            }
                        } else if (c == 'v' || c == '+') {
                            prm.remove_prefix(1);
                            as_.erropt.verbose_info++;
                        } else if (c == 'x') {
                            prm.remove_prefix(1);
                            as_.optimizing = OPTIM_ALL_ENABLED;
                        } else {
                            as_.fatalf(ERR_USAGE, "unknown optimization option -O{}\n", c);
                        }
                    }
                }
            }
            break;

        case 'p':   // pre-include
        case 'P':
            if (pass == 2) pp_->pre_include(*param);
            break;

        case 'd':   // pre-define
        case 'D':
            if (pass == 2) pp_->pre_define(*param);
            break;

        case 'u':   // un-define
        case 'U':
            if (pass == 2) pp_->pre_undefine(*param);
            break;

        case 'i':   // include search path
        case 'I':
            if (pass == 1) include_path_.emplace_back(*param);
            break;

        case 'l':   // listing file
            if (pass == 2) copy_filename(listname_, *param, "listing");
            break;

        case 'L':   // listing options
            if (pass == 2) listing_.update_options(*param, true);
            break;

        case 'Z':   // error messages file
            if (pass == 1) copy_filename(errname_, *param, "error");
            break;

        case 'F':   // specify debug format
            if (pass == 1) {
                using_debug_info_ = true;
                debug_format_ = std::string(*param);
                have_debug_format_ = true;
            }
            break;

        case 'X':   // specify error reporting format
            if (pass == 1) {
                if (as_.set_error_format(*param)) as_.fatalf(ERR_USAGE, "unrecognized error reporting format `{}'", *param);
            }
            break;

        case 'g':
            if (pass == 1) {
                using_debug_info_ = true;
                if (p.size() > 2) {
                    debug_format_ = std::string(skip_spaces(p.substr(2)));
                    have_debug_format_ = true;
                }
            }
            break;

        case 'h':
        case '?':
            show_help(get_opt_param(p, q, advance).value_or(std::string_view()));

        case 'y':   // legacy option
            show_help("-F");

        case 't':
            if (pass == 1) as_.tasm_compatible_mode = true;
            break;

        case 'v':
            show_version();

        case 'e':   // preprocess only
        case 'E':
            if (pass == 1) operating_mode_ = OP_PREPROCESS;
            break;

        case 'a':   // assemble only - don't preprocess
            if (pass == 1) ppopt_ |= PP_TRIVIAL;
            break;

        case 'w':
        case 'W':
            if (pass == 2) as_.set_warning_status(*param);
            break;

        case 'M': {
            const char m = p.size() > 2 ? p[2] : '\0';
            if (pass == 1) {
                switch (m) {
                case 'W': wmake_ = true; break;
                case 'D':
                case 'F':
                case 'T':
                case 'Q': advance = true; break;
                default: break;
                }
            } else {
                switch (m) {
                case 0: operating_mode_ = OP_DEPEND; break;
                case 'G':
                    operating_mode_ = OP_DEPEND;
                    depend_missing_ok_ = true;
                    break;
                case 'P': depend_emit_phony_ = true; break;
                case 'D':
                    operating_mode_ |= OP_DEPEND;
                    if (q && (q->empty() ? false : ((*q)[0] != '-' || q->size() == 1))) {
                        depend_file_ = std::string(*q);
                        advance = true;
                    }
                    break;
                case 'F':
                    if (q) depend_file_ = std::string(*q);
                    advance = true;
                    break;
                case 'T':
                    if (q) depend_target_ = std::string(*q);
                    advance = true;
                    break;
                case 'Q':
                    if (q) depend_target_ = wmake_ ? quote_for_wmake(*q) : quote_for_pmake(*q);
                    advance = true;
                    break;
                case 'W': break;   // handled in pass 1
                default: as_.nonfatalf(ERR_USAGE, "unknown dependency option `-M{}'", m); break;
                }
            }
            if (advance && (!q || q->empty())) {
                as_.nonfatalf(ERR_USAGE, "option `-M{}' requires a parameter", m);
                break;
            }
            break;
        }

        case '-': {
            std::string_view s = p.substr(2);

            if (s.empty()) {   // -- => stop processing options
                stopoptions_ = true;
                break;
            }

            const TextArg* tx = nullptr;
            std::size_t olen = 0;
            for (const TextArg& cand : kTextOpts) {
                olen = std::strlen(cand.label);
                if (olen > s.size()) continue;
                if (!drv_istarts(s, std::string_view(cand.label, olen))) continue;
                if (cand.label[olen - 1] == '-') {
                    tx = &cand;   // incomplete option
                    break;
                }
                if (s.size() == olen || s[olen] == '=') {
                    tx = &cand;   // complete option
                    break;
                }
            }

            if (!tx) {
                as_.nonfatalf(ERR_USAGE, "unrecognized option `--{}'", s);
                break;   // (NASM carries on with the table's terminator, which is a no-op)
            }

            TextOpt topt = tx->opt;
            std::string_view name = s;   // the option up to a `=`
            std::optional<std::string_view> prm;
            const std::size_t eq = s.find('=', olen);
            if (eq != std::string_view::npos) {
                prm = s.substr(eq + 1);
                name = s.substr(0, eq);
            }

            switch (tx->need_arg) {
            case NeedArg::Yes:   // argument required, and may be standalone
                if (!prm) {
                    prm = q;
                    advance = true;
                }
                // Note: a null string is a valid parameter.
                if (!prm) {
                    as_.nonfatalf(ERR_USAGE, "option `--{}' requires an argument", name);
                    topt = TextOpt::Bogus;
                }
                break;
            case NeedArg::No:   // argument prohibited
                if (prm) {
                    as_.nonfatalf(ERR_USAGE, "option `--{}' does not take an argument", name);
                    topt = TextOpt::Bogus;
                }
                break;
            case NeedArg::Maybe:   // argument permitted, but must be attached with =
                break;
            }

            switch (topt) {
            case TextOpt::Bogus: break;   // we have already errored out
            case TextOpt::Version: show_version();
            case TextOpt::AbortOnPanic: break;
            case TextOpt::Mangle:
                if (pass == 2) {
                    bool ok;
                    const LabelMangle lm = mangle_of(tx->pvt, ok);
                    if (ok) labels_.set_label_mangle(lm, *prm);
                }
                break;
            case TextOpt::Include:
                if (pass == 2) pp_->pre_include(*prm);
                break;
            case TextOpt::Pragma:
                if (pass == 2) pp_->pre_command("%pragma", *prm);
                break;
            case TextOpt::Bits:
                if (pass == 2) pp_->pre_command("BITS", *prm);
                break;
            case TextOpt::Before:
                if (pass == 2) pp_->pre_command(std::nullopt, *prm);
                break;
            case TextOpt::Limit:
                if (pass == 1) as_.set_limit(name.substr(olen), *prm);
                break;
            case TextOpt::KeepAll: keep_all_ = true; break;
            case TextOpt::NoLine: ppopt_ |= PP_NOLINE; break;
            case TextOpt::Debug:
                if (pass == 1) {
                    unsigned long long v = as_.erropt.debug_nasm + 1;
                    if (prm) {
                        v = 0;
                        for (const char c : *prm) {
                            if (c < '0' || c > '9') break;
                            v = v * 10 + static_cast<unsigned>(c - '0');
                        }
                    }
                    as_.erropt.debug_nasm = static_cast<unsigned>(v);
                }
                break;
            case TextOpt::Info:
                if (pass == 1) {
                    unsigned long long v = as_.erropt.verbose_info + 1;
                    if (prm) {
                        v = 0;
                        for (const char c : *prm) {
                            if (c < '0' || c > '9') break;
                            v = v * 10 + static_cast<unsigned>(c - '0');
                        }
                    }
                    as_.erropt.verbose_info = static_cast<unsigned>(v);
                }
                break;
            case TextOpt::Reproducible: as_.reproducible = true; break;
            case TextOpt::Masm: as_.masm_mode = true; break;
            case TextOpt::Help:
                // Allow --help topic without *requiring* topic.
                if (!prm) prm = q;
                show_help(prm.value_or(std::string_view()));
            }
            break;
        }

        default:
            as_.nonfatalf(ERR_USAGE, "unrecognised option `-{}'", opt);
            break;
        }
    } else if (pass == 2) {
        // In theory we could allow multiple input files...
        if (inname_set_) as_.fatal("more than one {} file specified: {}\n", "input", p);
        as_.inname = std::string(p);
        inname_set_ = true;
    }

    return advance;
}

void Engine::open_and_process_respfile(std::string_view respfile, int pass) {
    const auto data = files_.read(respfile);
    if (!data) as_.fatalf(ERR_PERROR, "unable to open response file `{}'", respfile);
    process_respfile_text(as_text(*data), pass);
}

// A response file given with -@: one option per line, an option's parameter on the next line (process_respfile).
void Engine::process_respfile_text(std::string_view text, int pass) {
    std::string prevarg;
    std::size_t pos = 0;
    while (true) {
        if (pos >= text.size()) {
            if (!prevarg.empty()) process_arg(prevarg, std::nullopt, pass);
            return;
        }
        std::size_t eol = text.find('\n', pos);
        std::string line(text.substr(pos, eol == std::string_view::npos ? std::string_view::npos : eol - pos));
        pos = eol == std::string_view::npos ? text.size() : eol + 1;

        // Play safe: remove CRs, LFs and any spurious ^Zs, if any of them are present at the end of the line.
        const std::size_t cut = line.find_first_of("\r\n\032");
        if (cut != std::string::npos) line.resize(cut);
        while (!line.empty() && nasm_isspace(line.back())) line.pop_back();

        std::string_view p = skip_spaces(line);
        std::string prm(p);
        if (process_arg(prevarg, std::string_view(prm), pass)) prm.clear();
        prevarg = prm;
    }
}

// Process args from a string of args, rather than the argv array: used by the response file processing (process_args).
void Engine::process_args(std::string_view args, int pass) {
    char separator = ' ';
    std::size_t p = 0;
    if (p < args.size() && args[p] != '-') separator = args[p++];
    std::optional<std::string> arg;
    while (p < args.size()) {
        const std::size_t q = p;
        while (p < args.size() && args[p] != separator) ++p;
        const std::string current(args.substr(q, p - q));
        while (p < args.size() && args[p] == separator) ++p;
        const std::optional<std::string> prevarg = arg;
        arg = current;
        if (prevarg && process_arg(*prevarg, std::string_view(*arg), pass)) arg.reset();
    }
    if (arg) process_arg(*arg, std::nullopt, pass);
}

void Engine::process_response_file(std::string_view file, int pass) {
    const auto data = files_.read(file);
    if (!data) {
        // perror(file); exit(-1)
        err_ += std::string(file) + ": No such file or directory\n";
        throw ExitRun{255};
    }
    const std::string_view text = as_text(*data);
    std::size_t pos = 0;
    while (pos < text.size()) {
        std::size_t eol = text.find('\n', pos);
        const std::size_t end = eol == std::string_view::npos ? text.size() : eol + 1;
        std::string_view line = text.substr(pos, std::min<std::size_t>(end - pos, 2047));   // char str[2048]
        process_args(line, pass);
        pos = end;
    }
}

void Engine::parse_cmdline(int pass) {
    // Initialize all the warnings to their default state, including warning index 0 used for "always on".
    as_.warning_state = kWarningDefault;

    // (NASMENV is not consulted: a run depends on its request alone.)

    // Now process the actual command line.
    for (std::size_t i = 0; i < args_.size();) {
        if (!args_[i].empty() && args_[i][0] == '@') {
            // We have a response file, so process this as a set of arguments like the environment variable. This allows us
            // to have multiple arguments on a single line, which is different to the -@resp file processing below for
            // regular NASM.
            process_response_file(std::string_view(args_[i]).substr(1), pass);
            ++i;   // (NASM skips the argument after a response file too: `argc--; argv++;` then processes argv[0] again)
            if (i >= args_.size()) break;
        }

        const std::optional<std::string_view> next = i + 1 < args_.size() ? std::optional<std::string_view>(args_[i + 1]) : std::nullopt;
        const bool advance = process_arg(args_[i], next, pass);
        i += advance ? 2 : 1;
    }

    // Look for basic command line typos. This definitely doesn't catch all errors, but it might help cases of fumbled fingers.
    if (pass != 2) return;

    if (!inname_set_)
        as_.fatalf(ERR_USAGE, "no input file specified");
    else if ((errname_ && as_.inname == *errname_) || (outname_set_ && as_.inname == as_.outname) || (listname_ && as_.inname == *listname_) ||
             (depend_file_ && as_.inname == *depend_file_))
        as_.fatal("will not overwrite input file");
}

// ---- setup ----

// Define the system-defined macros that are not part of macros/standard.mac (define_macros).
void Engine::define_macros() {
    const CompileTime& oct = as_.official_compile_time;
    char temp[128];

    auto fmt_time = [&](const char* pattern, const std::tm& tm) {
        std::strftime(temp, sizeof temp, pattern, &tm);
        pp_->pre_define(temp);
    };

    if (oct.have_local) {
        fmt_time("__?DATE?__=\"%Y-%m-%d\"", oct.local);
        fmt_time("__?DATE_NUM?__=%Y%m%d", oct.local);
        fmt_time("__?TIME?__=\"%H:%M:%S\"", oct.local);
        fmt_time("__?TIME_NUM?__=%H%M%S", oct.local);
    }

    if (oct.have_gm) {
        fmt_time("__?UTC_DATE?__=\"%Y-%m-%d\"", oct.gm);
        fmt_time("__?UTC_DATE_NUM?__=%Y%m%d", oct.gm);
        fmt_time("__?UTC_TIME?__=\"%H:%M:%S\"", oct.gm);
        fmt_time("__?UTC_TIME_NUM?__=%H%M%S", oct.gm);
    }

    if (oct.have_posix) pp_->pre_define(std::format("__?POSIX_TIME?__={}", oct.posix));

    // In case if output format is defined by alias we have to put shortname of the alias itself here otherwise ABI
    // backward compatibility gets broken.
    pp_->pre_define(std::format("__?OUTPUT_FORMAT?__={}", ofmt_alias_.empty() ? std::string(ofmt_->shortname()) : ofmt_alias_));

    // Debug format, if any.
    if (dfmt_ != &null_debug_format()) pp_->pre_define(std::format("__?DEBUG_FORMAT?__={}", dfmt_->shortname()));
}

// Initialize the preprocessor, set up the include path, and define the system-included macros. This is called between passes 1
// and 2 of parsing the command options; ofmt and dfmt are defined at this point.
void Engine::preproc_init() {
    pp_->init(ppopt_);
    define_macros();
    pp_->include_path(include_path_);
}

void Engine::get_boolean_option(std::string_view str, bool& val) {
    static const char* const noyes[] = {"no", "yes", "false", "true", "off", "on"};

    str = skip_spaces(str);
    const std::string p(str);   // the scanner reads a buffer that stays put

    TokenVal tokval;
    tokval.type = TokenType::INVALID;
    tscan_.reset(p);

    const int tt = tscan_.scan(tokval);
    if (tt == 0 || tt == ']' || tt == ',') {
        val = true;
        return;
    }

    if (tt == token_value(TokenType::ID)) {
        for (std::size_t i = 0; i < std::size(noyes); i++)
            if (drv_ieq(tokval.text, noyes[i])) {
                val = i & 1;
                return;
            }
    }

    const std::optional<Expr> evalresult = eval_.evaluate(scan_fn(tscan_), tokval, nullptr, true, nullptr);
    if (!evalresult) return;

    if (!is_really_simple(*evalresult)) {
        as_.nonfatal("boolean flag expression must be a constant");
        return;
    }

    val = reloc_value(*evalresult) != 0;
}

// Make the modules that depend on the output format and the options (they exist from the first pass of the command line).
void Engine::make_modules() {
    // ---- the preprocessor ----
    PpHost ph;
    ph.token_hash = [](std::string_view text, TokenVal& tv) { return static_cast<TokenType>(token_hash(text, tv)); };
    ph.evaluate = [this](const PpScanFn& scan, TokenVal& tv, int critical) { return eval_.evaluate(scan, tv, nullptr, critical != 0, nullptr); };
    ph.directive_valid = [this](std::string_view name) { return directives_ && directives_->directive_valid(name); };
    ph.read_file = [this](const std::string& path) -> std::optional<std::string> {
        auto data = files_.read(path);
        if (!data) return std::nullopt;
        return std::string(as_text(*data));
    };
    ph.realpath = [this](const std::string& path) { return files_.full(path); };
    ph.list_line = [this](PpListType type, std::int32_t lineno, std::string_view text) {
        listing_.line(type == LIST_MACRO ? ListType::MACRO : type == LIST_INCLUDE ? ListType::INCLUDE : ListType::READ, lineno, text);
    };
    ph.list_uplevel = [this](PpListType type) { listing_.uplevel(type == LIST_MACRO ? ListType::MACRO : type == LIST_INCLUDE ? ListType::INCLUDE : ListType::READ, 0); };
    ph.list_downlevel = [this](PpListType type) { listing_.downlevel(type == LIST_MACRO ? ListType::MACRO : type == LIST_INCLUDE ? ListType::INCLUDE : ListType::READ); };
    ph.list_option = [this](char c) { return listing_.option(c); };
    ph.readnum = [this](std::string_view text, bool& error) { return readnum(as_, text, error); };
    pp_ = std::make_unique<Preproc>(as_, std::move(ph));
    pp_->set_format_macros(&ofmt_->stdmac());

    // ---- the parser ----
    ParseHost parse_host;
    parse_host.evaluate = [this](Scanner& sc, TokenVal& tv, int* opflags, int critical, EvalHints* hints) {
        return eval_.evaluate(scan_fn(sc), tv, opflags, critical != 0, hints);
    };
    parse_host.define_label = [this](std::string_view name, std::int32_t seg, std::int64_t offset, bool is_norm) {
        labels_.define_label(name, seg, offset, is_norm);
    };
    parser_ = std::make_unique<Parser>(as_, pscan_, floats_, std::move(parse_host));

    // ---- the directives ----
    DirectiveHost dh;
    dh.ofmt = ofmt_.get();
    dh.dfmt = const_cast<DebugFormat*>(dfmt_);
    dh.lfmt = &listing_;
    dh.scanner = &dscan_;
    dh.floats = &floats_;
    dh.evaluate_text = [this](std::string_view text, int critical) { return evaluate_text(text, critical); };
    dh.declare_label = [this](std::string_view name, LabelType type, std::optional<std::string_view> special) {
        return labels_.declare_label(name, type, special.value_or(std::string_view()));
    };
    dh.define_label = [this](std::string_view name, std::int32_t seg, std::int64_t offset, bool is_norm) { labels_.define_label(name, seg, offset, is_norm); };
    dh.set_label_mangle = [this](Directive which, std::string_view value) {
        bool ok;
        const LabelMangle lm = mangle_of(which, ok);
        if (ok) labels_.set_label_mangle(lm, value);
    };
    dh.get_boolean_option = [this](std::string_view text, bool& value) { get_boolean_option(text, value); };
    directives_ = std::make_unique<Directives>(as_, std::move(dh));

    // ---- the encoder ----
    EncoderHooks eh;
    eh.define_label = [this](const std::string& label, std::int32_t segment, std::int64_t offset, bool normal) {
        labels_.define_label(label, segment, offset, normal);
    };
    eh.read_file = [this](const std::string& name) { return files_.read(name); };
    encoder_ = std::make_unique<Encoder>(as_, *ofmt_, &listing_, const_cast<DebugFormat*>(dfmt_), std::move(eh));

    // ---- the label table's calls out ----
    labels_.hooks.symdef = [this](std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) {
        ofmt_->symdef(name, segment, offset, is_global, special);
    };
    labels_.hooks.debug_deflabel = [this](std::string_view name, std::int32_t segment, std::int64_t offset, int is_global, std::string_view special) {
        const_cast<DebugFormat*>(dfmt_)->deflabel(name, segment, offset, is_global, special);
    };
    if (ofmt_->has_herelabel())
        labels_.hooks.herelabel = [this](std::string_view name, LabelType type, std::int32_t segment, std::int32_t& subsection, bool& copyoffset) {
            return ofmt_->herelabel(name, type, segment, subsection, copyoffset);
        };
    labels_.hooks.seg_alloc = [this] { return segs_.alloc(); };

    eval_.segbase = [this](std::int32_t segment) { return ofmt_->segbase(segment); };

    // ---- the assembly's calls out ----
    as_.hooks.pp_suppress_error = [this](ErrFlags severity) { return pp_ && pp_->suppress_error(severity); };
    as_.hooks.pp_error_list_macros = [this](ErrFlags severity) {
        if (pp_) pp_->error_list_macros(severity);
    };
    as_.hooks.list_error = [this](ErrFlags severity, const std::string& text) { listing_.error(severity, text); };
    as_.hooks.final_report = [this](bool) { listing_.cleanup(); };
    as_.hooks.close_output = [this](bool failure) { close_output(failure); };
    as_.hooks.readnum = [this](std::string_view text, bool& error) { return readnum(as_, text, error); };
}

void Engine::close_output(bool error) {
    if (!ofile_open_) return;
    if (!(error && !keep_all_)) result_files_[as_.outname] = ofile_;
    ofile_open_ = false;
}

// ---- the dependency list (-M) ----

void Engine::emit_dependencies() {
    if (!depend_list_) return;

    const std::string wrapstr = wmake_ ? " &\n " : " \\\n ";
    const std::string nulltarget = wmake_ ? "\t%null\n" : "";
    std::string text;

    std::size_t linepos = 0;
    {
        const std::string head = *depend_target_ + " :";
        text += head;
        linepos = head.size();
    }
    for (const std::string& name : *depend_list_) {
        const std::string file = wmake_ ? quote_for_wmake(name) : quote_for_pmake(name);
        if (linepos + file.size() > 62 && linepos > 1) {
            text += wrapstr;
            linepos = 1;
        }
        text += " " + file;
        linepos += file.size() + 1;
    }
    text += "\n\n";

    for (const std::string& name : *depend_list_) {
        if (depend_emit_phony_) {
            const std::string file = wmake_ ? quote_for_wmake(name) : quote_for_pmake(name);
            text += file + " :\n" + nulltarget + "\n";
        }
    }

    if (depend_file_ && *depend_file_ != "-") {
        Bytes& f = extra_[*depend_file_].data;
        extra_[*depend_file_].text = true;
        f.assign(text.begin(), text.end());
    } else {
        out_ += text;
    }
}

// ---- the work ----

void Engine::forward_refs_and_process(Insn& ins) {
    as_.forward_refs(ins);
    encoder_->process_insn(ins);
}

void Engine::assemble_file(const std::string& fname) {
    std::int64_t stall_count = 0;   // make sure we make forward progress...

    as_.reset_limits_before_pass();

    switch (as_.cmd_sb) {
    case 16: break;
    case 32:
        if (!iflag_cpu_level_ok(as_.cmd_cpu, IF_386)) as_.fatal("command line: 32-bit segment size requires a higher cpu");
        break;
    case 64:
        if (!iflag_cpu_level_ok(as_.cmd_cpu, IF_X86_64)) as_.fatal("command line: 64-bit segment size requires a higher cpu");
        break;
    default: as_.panic_here();
    }

    std::uint64_t prev_offset_changed = static_cast<std::uint64_t>(INT64_MAX);

    // (Remove the list file in case we die before the output pass: a file that is only made in the result at the end.)
    if (listname_ && !keep_all_) list_removed_ = true;

    Insn output_ins;

    while (!terminate_after_phase() && !as_.pass_final()) {
        as_.begin_pass();

        if (listname_) {
            if (listing_.on_this_pass()) {
                // Generating a list file on this pass.
                listing_.init(*listname_);
                list_removed_ = false;
            } else if (listing_.active()) {
                // Looks like we used the list engine on a previous pass, but now it is turned off, presumably via %pragma -p.
                listing_.cleanup();
                if (!keep_all_) list_removed_ = true;
            }
        }

        ofmt_->reset();
        as_.switch_segment(ofmt_->section(std::nullopt, as_.globl.bits));
        pp_->reset(fname, PP_NORMAL, depend_list_ ? &*depend_list_ : nullptr);

        as_.globallineno = 0;

        while (true) {
            std::optional<std::string> line = pp_->getline();
            if (!line) break;

            if (++as_.globallineno > as_.nasm_limit[LIMIT_LINES])
                as_.fatal("overall line count exceeds the maximum {}\n", as_.nasm_limit[LIMIT_LINES]);

            // Here we parse our directives; this is not handled by the main parser.
            if (directives_->process_directives(*line)) continue;   // just do final cleanup

            // Not a directive, or even something that starts with [.
            parser_->parse_line(*line, output_ins, as_.globl.bits);
            forward_refs_and_process(output_ins);
        }

        pp_->cleanup_pass();

        if (as_.global_offset_changed) {
            switch (as_.pass_type()) {
            case PASS_OPT:
                // This is the only pass type that can be executed more than once, and therefore has the ability to stall.
                if (as_.global_offset_changed < prev_offset_changed) {
                    prev_offset_changed = as_.global_offset_changed;
                    stall_count = 0;
                } else {
                    stall_count++;
                }

                if (stall_count > as_.nasm_limit[LIMIT_STALLED] || as_.pass_count() >= as_.nasm_limit[LIMIT_PASSES]) {
                    // No convergence, almost certainly dead.
                    as_.nonfatalf(ERR_UNDEAD, "unable to find valid values for all labels after {} passes; stalled for {}, giving up.",
                                  as_.pass_count(), stall_count);
                    as_.nonfatalf(ERR_UNDEAD, "Possible causes: recursive EQUs, macro abuse.");
                }
                break;

            case PASS_STAB:
                as_.warn(WARN_PHASE | ERR_UNDEAD, "phase error during stabilization pass, hoping for the best");
                break;

            case PASS_FINAL:
                as_.nonfatalf(ERR_UNDEAD, "phase error during code generation pass");
                break;

            default:
                break;   // this is normal, we'll keep going...
            }
        }

        as_.error_pass_end();
    }

    as_.print_final_report(terminate_after_phase());

    listing_.cleanup();
}

// -E: write the preprocessed source (the OP_PREPROCESS branch of main).
void Engine::preprocess_only() {
    std::string output;
    std::string quoted_file_name = nasm_quote_filename({});
    std::int32_t linnum = 0;
    std::int32_t lineinc = 0;
    std::string_view cur_name;   // the file name last seen (compared by identity: the names are kept by the assembly)

    as_.location.known = false;

    as_.begin_preproc_pass();
    pp_->reset(as_.inname, PP_PREPROC, depend_list_ ? &*depend_list_ : nullptr);
    as_.error_pass_start(true);

    while (true) {
        std::optional<std::string> line = pp_->getline();
        if (!line) break;

        // We generate %line directives if needed for later programs.
        const SrcLocation where = as_.src.where();
        if (cur_name.data() != where.filename.data() || cur_name.size() != where.filename.size()) {
            cur_name = where.filename;
            linnum = -1;   // force a new %line statement
            lineinc = !where.filename.empty() ? 1 : 0;
            quoted_file_name = nasm_quote_filename(where.filename);
        } else if (lineinc) {
            if (linnum + lineinc == where.lineno) {
                // Add one blank line to account for increment.
                output += '\n';
                linnum += lineinc;
            } else if (linnum - lineinc == where.lineno) {
                // Standing still, probably a macro. Set increment to zero.
                lineinc = 0;
            }
        } else {
            // lineinc == 0
            if (linnum + 1 == where.lineno) lineinc = 1;
        }

        // Skip blank lines if we will need a %line anyway.
        if (!line->empty() || linnum != -1) {
            if (linnum != where.lineno) output += std::format("%line {}{:+} {}\n", where.lineno, lineinc, quoted_file_name);
            linnum = where.lineno + lineinc;
            output += *line;
            output += '\n';
        }
    }

    pp_->cleanup_pass();
    as_.error_pass_end();

    if (as_.outname.empty() && !outname_set_) {
        out_ += output;   // stdout
    } else {
        ofile_.assign(output.begin(), output.end());
        ofile_open_ = true;
    }
    close_output(terminate_after_phase());
}

// -M without assembling: just read the files.
void Engine::depend_only() {
    as_.error_pass_start(true);

    if (depend_missing_ok_) pp_->include_path({});   // "assume generated"

    pp_->reset(as_.inname, PP_DEPS, depend_list_ ? &*depend_list_ : nullptr);
    while (pp_->getline()) {
    }
    pp_->cleanup_pass();
    as_.error_pass_end();
}

RunResult Engine::run() {
    int exit_code = 0;
    try {
        try {
            args_ = request_.args;

            // Do these as early as possible.
            as_.progname = "nasm";
            as_.set_compile_time(std::time(nullptr));

            labels_.init();
            operating_mode_ = OP_NORMAL;

            // The format has a default (bin): pass 1 finds `-f`; make it now so the modules exist for the options.
            // (NASM selects the default at its static initialization.)
            as_.hooks.readnum = [this](std::string_view text, bool& error) { return readnum(as_, text, error); };
            // Pass 1 of the command line needs only the options that set modes; -f is read first below.
            parse_cmdline(1);
            if (terminate_after_phase()) return finish(1);

            // At this point we have ofmt and the name of the desired debug format.
            if (!ofmt_) ofmt_ = make_format("bin", *this, nullptr);
            if (!using_debug_info_) {
                // No debug info, redirect to the null backend (empty stubs).
                dfmt_ = &null_debug_format();
            } else if (!have_debug_format_) {
                dfmt_ = ofmt_->default_dfmt();   // default debug format for this backend
            } else {
                dfmt_ = find_debug_format(*ofmt_, debug_format_);
                if (!dfmt_) as_.fatalf(ERR_USAGE, "unrecognized debug format `{}' for output format `{}'", debug_format_, ofmt_->shortname());
            }

            // Have we enabled TASM mode?
            if (as_.tasm_compatible_mode) ppopt_ |= PP_TASM;   // (nasm_ctype_tasm_mode: not carried into the shared character table)

            // MASM mode: match ML's code generation. ML optimizes immediate sizes (imm8 sign-extension) but does NOT shrink
            // forward jumps to short -- exactly NASM's -O1. Default to it unless the user asked for an explicit -O level (NASM's
            // own default, -Ox, would over-shorten forward branches).
            if (as_.masm_mode && !as_.optimizing_set) as_.optimizing = OPTIM_LEVEL_1;

            make_modules();
            preproc_init();

            parse_cmdline(2);
            if (terminate_after_phase()) return finish(1);

            // MASM compatibility mode: auto-load the "masm" macro package so the user does not have to write an explicit `%use
            // masm'. Pushed after the command-line `--before' lines so it is emitted first, ahead of the input, guaranteeing the
            // package is active for the whole source.
            if (as_.masm_mode) pp_->pre_command("%use", "masm");

            // Save away the default states of warnings and limits.
            as_.error_init();
            as_.keep_limits_from_cmdline();

            // Dependency filename if we are also doing other things.
            if (!depend_file_ && (operating_mode_ & ~OP_DEPEND)) {
                if (outname_set_)
                    depend_file_ = as_.outname + ".d";
                else
                    depend_file_ = filename_set_extension(as_.inname, ".d");
            }

            // If no output file name provided and this is preprocess mode, we're perfectly fine to output into stdout.
            if (!outname_set_ && !(operating_mode_ & OP_PREPROCESS)) {
                as_.outname = filename_set_extension(as_.inname, ofmt_->extension());
                outname_set_ = true;
                if (as_.outname == as_.inname) {
                    as_.outname = "nasm.out";
                    as_.warn(WARN_OTHER, "default output file same as input, using `{}' for output\n", as_.outname);
                }
            }

            if (operating_mode_ & OP_DEPEND) depend_list_.emplace();

            if (!depend_target_) depend_target_ = wmake_ ? quote_for_wmake(as_.outname) : quote_for_pmake(as_.outname);

            as_.reset_global_defaults(as_.cmd_sb);

            if (!(operating_mode_ & (OP_PREPROCESS | OP_NORMAL))) {
                depend_only();
            } else if (operating_mode_ & OP_PREPROCESS) {
                preprocess_only();
            }

            if (operating_mode_ & OP_NORMAL) {
                ofile_open_ = true;

                ofmt_->init();
                const_cast<DebugFormat*>(dfmt_)->init();

                assemble_file(as_.inname);

                if (!terminate_after_phase()) {
                    ofmt_->cleanup();
                    labels_.cleanup();
                }

                close_output(terminate_after_phase());
            }

            pp_->cleanup_session();

            if (depend_list_ && !terminate_after_phase()) emit_dependencies();

            exit_code = terminate_after_phase() ? 1 : 0;
        } catch (const ExitRun& e) {
            exit_code = e.code;
        }
    } catch (const cb::Error&) {
        // A fatal error: Assembly recorded the message and the status NASM would have exited with.
        exit_code = as_.exit_code();
        if (!exit_code) exit_code = 1;
    } catch (const std::exception& e) {
        err_ += std::format("{}: panic: {}\n", as_.progname, e.what());
        exit_code = 4;
    }

    return finish(exit_code);
}

RunResult Engine::finish(int exit_code) {
    close_output(exit_code != 0);   // (an output still open at the end of a clean run is kept)

    RunResult r;
    r.exit = exit_code;

    // The messages: what NASM wrote to its error file (stderr, or stdout with -s, or the -Z file).
    const std::string& diag = as_.error_output();
    std::string errtext = diag;
    if (errname_) {
        extra_[*errname_].text = true;
        extra_[*errname_].data.assign(diag.begin(), diag.end());
        errtext.clear();
    } else if (errors_to_stdout_) {
        out_ = diag + out_;
        errtext.clear();
    }
    err_ = errtext + err_;

    // The files.
    for (auto& [name, data] : result_files_) {
        const bool text = ofmt_ && (ofmt_->flags() & OFMT_TEXT) && name == as_.outname;
        r.files[name] = (text || (operating_mode_ & OP_PREPROCESS && name == as_.outname)) && crlf_ ? crlf_bytes(data) : data;
    }
    for (auto& [name, e] : extra_) r.files[name] = e.text && crlf_ ? crlf_bytes(e.data) : e.data;
    if (listname_ && !list_removed_ && !list_name_written_.empty()) r.files[*listname_] = crlf_ ? crlf_bytes(list_file_) : list_file_;

    r.out = crlf_ ? crlf(out_) : out_;
    r.err = crlf_ ? crlf(err_) : err_;
    return r;
}

}  // namespace drvns_driver
using namespace drvns_driver;

RunResult run(const Request& request, bool text_crlf) {
    Engine engine(request, text_crlf);
    return engine.run();
}

Result assemble(const Request& request) {
    RunResult r = run(request, true);
    Result out;
    out.exit = r.exit;
    out.files = std::move(r.files);
    out.messages = std::move(r.err) + r.out;   // stderr, then stdout
    return out;
}

}  // namespace cb::build::x86asm
