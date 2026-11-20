// build/x86asm/assembly.cpp -- `Assembly`: the run state of x86asm and NASM's error handling over it (error.c,
// errstubs.c, srcfile.c, and the globals of nasm.c and directiv.c). See assembly.hpp.
#include "build/x86asm/assembly.hpp"

#include <algorithm>
#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <system_error>
#include <tuple>

namespace cb::build::x86asm {

namespace {

constexpr char asm_lower(char c) { return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c; }

// Case-insensitive equality of a string with a C string (nasm_stricmp == 0).
bool asm_iequals(std::string_view a, const char* b) {
    const std::size_t n = std::strlen(b);
    if (a.size() != n) return false;
    for (std::size_t i = 0; i < n; ++i)
        if (asm_lower(a[i]) != asm_lower(b[i])) return false;
    return true;
}

// The first `n` characters of `a` equal those of the C string `b` ignoring case, where `b` has at least `n` (nasm_strnicmp == 0).
bool asm_iprefix(std::string_view a, const char* b, std::size_t n) {
    if (a.size() < n) return false;
    for (std::size_t i = 0; i < n; ++i) {
        if (b[i] == '\0') return false;
        if (asm_lower(a[i]) != asm_lower(b[i])) return false;
    }
    return true;
}

bool asm_isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

// The error file formats: how the line number is written around it, and what comes before the message.
struct ErrFormatRow {
    const char* beforeline;
    const char* afterline;
    const char* beforemsg;
};
constexpr ErrFormatRow kErrFormatGnu = {":", "", ": "};
constexpr ErrFormatRow kErrFormatMsvc = {"(", ")", " : "};

constexpr const char* kNoFileName = "nasm";   // what to print if there is no file name

// The limits' names, help, default (0: the maximum) and maximum (0: LIMIT_MAX_VAL). The order is enum NasmLimit.
struct LimitInfo {
    const char* name;
    const char* help;
    std::int64_t default_val;
    std::int64_t max_val;
};
constexpr std::array<LimitInfo, LIMIT_MAX> kLimitInfo = {{
    {"passes", "total number of passes", 0, 0},
    {"stalled-passes", "number of passes without forward progress", 1000, 0},
    {"macro-levels", "levels of macro expansion", 10000, 0},
    {"macro-tokens", "tokens processed during single-line macro expansion", 10000000, 0},
    {"mmacros", "multi-line macros before final return", 100000, 0},
    {"rep", "%rep count", 1000000, 0},
    {"eval", "expression evaluation descent", 8192, 0},
    {"lines", "total source lines processed", 2000000000, 0},
    {"params", "multi-line macro parameter count", 16383, INT_MAX >> 1},
}};

// The decimal (or 0x/0b/0o prefixed) number of a limit value; the preprocessor's readnum replaces this through the hooks.
std::int64_t asm_default_readnum(std::string_view text, bool& error) {
    error = true;
    std::string_view s = text;
    bool negative = false;
    if (!s.empty() && (s[0] == '+' || s[0] == '-')) {
        negative = s[0] == '-';
        s.remove_prefix(1);
    }
    int base = 10;
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) { base = 16; s.remove_prefix(2); }
    else if (s.size() > 2 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) { base = 2; s.remove_prefix(2); }
    else if (s.size() > 2 && s[0] == '0' && (s[1] == 'o' || s[1] == 'O')) { base = 8; s.remove_prefix(2); }
    std::uint64_t value = 0;
    const auto r = std::from_chars(s.data(), s.data() + s.size(), value, base);
    if (s.empty() || r.ec != std::errc() || r.ptr != s.data() + s.size()) return 0;
    error = false;
    return negative ? -static_cast<std::int64_t>(value) : static_cast<std::int64_t>(value);
}

// The CPU names of the CPU directive and their levels (cpunames in directiv.c).
struct CpuName {
    const char* name;
    unsigned level;
};
constexpr std::array<CpuName, 38> kCpuNames = {{
    {"default", IF_DEFAULT},   // must be first
    {"8086", IF_8086},
    {"186", IF_186},
    {"286", IF_286},
    {"386", IF_386},
    {"486", IF_486},
    {"586", IF_PENT},
    {"pentium", IF_PENT},
    {"pentiummmx", IF_PENT},
    {"686", IF_P6},
    {"p6", IF_P6},
    {"ppro", IF_P6},
    {"pentiumpro", IF_P6},
    {"p2", IF_P6},   // +MMX
    {"pentiumii", IF_P6},
    {"p3", IF_KATMAI},
    {"katmai", IF_KATMAI},
    {"p4", IF_WILLAMETTE},
    {"willamette", IF_WILLAMETTE},
    {"prescott", IF_PRESCOTT},
    {"x64", IF_X86_64},
    {"x86-64", IF_X86_64},
    {"ia64", IF_IA64},
    {"ia-64", IF_IA64},
    {"itanium", IF_IA64},
    {"itanic", IF_IA64},
    {"merced", IF_IA64},
    {"nehalem", IF_NEHALEM},
    {"westmere", IF_WESTMERE},
    {"sandybridge", IF_SANDYBRIDGE},
    {"ivybridge", IF_FUTURE},
    {"any", IF_ANY},
    {"all", IF_ANY},
    {"latevex", IF_LATEVEX},
    {"apx", IF_APX},
    {"evex", IF_EVEX},
    {"vex", IF_VEX},
    {nullptr, 0},
}};

void asm_set_cpu_level(IFlags& a, unsigned lvl) {
    a.field[0] = 0;   // not applicable to the CPU type
    iflag_set_all_features(a);   // all feature masking bits set for now
    if (lvl >= IF_ANY) a.set(IF_LATEVEX);   // this is a hack for now
    a.field[IF_CPU_FIELD] &= ~IF_CPU_LEVEL_MASK;
    a.set(lvl);
}

// NASM's make_posix_time: seconds since the epoch of a broken-down time, by IEEE 1003.1:2004 section 4.14.
std::int64_t asm_make_posix_time(const std::tm& tm) {
    const std::int64_t y = tm.tm_year;
    std::int64_t t = (y - 70) * 365 + (y - 69) / 4 - (y - 1) / 100 + (y + 299) / 400;
    t += tm.tm_yday;
    t *= 24;
    t += tm.tm_hour;
    t *= 60;
    t += tm.tm_min;
    t *= 60;
    t += tm.tm_sec;
    return t;
}

}  // namespace

// ---- error text ----

const char* error_prefix(ErrFlags severity) {
    switch (severity & ERR_MASK) {
    case ERR_LISTMSG: return ";;; ";
    case ERR_NOTE: return "note: ";
    case ERR_DEBUG: return "debug: ";
    case ERR_INFO: return "info: ";
    case ERR_WARNING: return "warning: ";
    case ERR_NONFATAL: return "error: ";
    case ERR_FATAL: return "fatal: ";
    case ERR_CRITICAL: return "critical: ";
    case ERR_PANIC: return "panic: ";
    default: return "internal error: ";
    }
}

// ---- SourcePosition ----

std::string_view SourcePosition::intern(std::string_view name) {
    if (name.empty()) return {};
    return *names_.emplace(name).first;
}

std::string_view SourcePosition::set_fname(std::string_view name) {
    const std::string_view interned = intern(name);
    const std::string_view old = frames_.back().where.filename;
    frames_.back().where.filename = interned;
    return old;
}

std::int32_t SourcePosition::set_linnum(std::int32_t line) {
    const std::int32_t old = frames_.back().where.lineno;
    frames_.back().where.lineno = line;
    return old;
}

void SourcePosition::set(std::int32_t line, std::string_view fname) {
    set_fname(fname);
    set_linnum(line);
}

const void* SourcePosition::error_down() {
    if (error_ + 1 < frames_.size()) {
        ++error_;
        return frames_[error_].macro;
    }
    return nullptr;
}

SrcLocation SourcePosition::update(SrcLocation whence) {
    const SrcLocation old = frames_.back().where;
    frames_.back().where = whence;
    return old;
}

std::int32_t SourcePosition::get(std::int32_t& xline, std::string_view& xname) const {
    const std::string_view xn = xname;
    const std::int32_t xl = xline;
    const std::int32_t line = frames_.back().where.lineno;
    xline = line;
    xname = frames_.back().where.filename;
    if (xn.empty() || xn.data() != frames_.back().where.filename.data()) return -2;
    return line - xl;
}

void SourcePosition::macro_push(const void* macro, SrcLocation where) {
    frames_.push_back({where, macro});
}

void SourcePosition::macro_pop() {
    if (frames_.size() > 1) frames_.pop_back();
    if (error_ >= frames_.size()) error_ = frames_.size() - 1;
}

// ---- Assembly: construction, files, time ----

Assembly::Assembly() : errfmt_{kErrFormatGnu.beforeline, kErrFormatGnu.afterline, kErrFormatGnu.beforemsg} {
    // Initialize all the warnings to their default state, including warning index 0 used for "always on".
    warning_state = kWarningDefault;
    set_cpu(std::nullopt);
    cmd_cpu = cpu;
    set_default_limits();
    reset_global_defaults(0);
    hooks.readnum = asm_default_readnum;
}

void Assembly::set_compile_time(std::time_t now) {
    CompileTime& oct = official_compile_time;
    oct = CompileTime{};
    oct.t = now;

    const std::tm* best_gm = nullptr;
    std::tm tmp{};
#if defined(_WIN32)
    if (localtime_s(&tmp, &now) == 0) {
#else
    if (localtime_r(&now, &tmp) != nullptr) {
#endif
        oct.local = tmp;
        best_gm = &oct.local;
        oct.have_local = true;
    }
#if defined(_WIN32)
    if (gmtime_s(&tmp, &now) == 0) {
#else
    if (gmtime_r(&now, &tmp) != nullptr) {
#endif
        oct.gm = tmp;
        best_gm = &oct.gm;
        oct.have_gm = true;
        if (!oct.have_local) oct.local = oct.gm;
    } else {
        oct.gm = oct.local;
    }
    if (best_gm) {
        oct.posix = asm_make_posix_time(*best_gm);
        oct.have_posix = true;
    }
}

void Assembly::masm_type_note(std::string_view name, int size) {
    if (size <= 0) return;
    // Fold to lower case, at most 255 characters (the masm_fold of parser.c).
    std::string key;
    for (std::size_t i = 0; i < name.size() && i + 1 < 256; ++i) key.push_back(asm_lower(name[i]));
    masm_types_[key] = size;
}

int Assembly::masm_type_query(std::string_view name) const {
    std::string key;
    for (std::size_t i = 0; i < name.size() && i + 1 < 256; ++i) key.push_back(asm_lower(name[i]));
    const auto it = masm_types_.find(key);
    return it == masm_types_.end() ? 0 : it->second;
}

// ---- passes and the location ----

const char* Assembly::pass_type_name() const {
    static constexpr std::array<const char*, 6> kPassTypes = {"init", "preproc-only", "first", "optimize", "stabilize", "final"};
    return kPassTypes[static_cast<std::size_t>(pass_type_)];
}

void Assembly::begin_preproc_pass() {
    location.known = false;
    pass_type_ = PASS_PREPROC;
}

void Assembly::begin_pass() {
    ++passn_;
    switch (pass_type_) {
    case PASS_INIT:
        pass_type_ = PASS_FIRST;
        break;
    case PASS_OPT:
        if (global_offset_changed) break;   // one more optimization pass
        pass_type_ = static_cast<PassType>(pass_type_ + 1);
        break;
    default:
        pass_type_ = static_cast<PassType>(pass_type_ + 1);
        break;
    }

    error_pass_start(pass_final());
    global_offset_changed = 0;

    // Suppress ERR_PASS2 unless we are actually in the final pass.
    erropt.never = 0;
    if (!pass_final()) erropt.never |= ERR_PASS2;

    reset_global_defaults(cmd_sb);

    cpu = cmd_cpu;

    in_absolute = false;
    if (!pass_first()) {
        forwref_ = 0;   // rewind the forward references
        offsets_.clear();
    }
    location.segment = NO_SEG;
    location.offset = 0;
    if (pass_first()) location.known = true;
}

void Assembly::reset_global_defaults(int bits) {
    globl.bits = bits;
    globl.bnd = false;
    globl.rel = 0;
    globl.reldef = EAF_FS | EAF_GS;
    globl.dollarhex = true;
}

std::int64_t Assembly::switch_segment(std::int32_t segment) {
    location.segment = segment;
    if (segment == NO_SEG) {
        location.offset = absolute.offset;
        in_absolute = true;
    } else {
        const auto it = offsets_.find(segment);
        location.offset = it == offsets_.end() ? 0 : it->second;
        in_absolute = false;
    }
    return location.offset;
}

std::int64_t Assembly::set_curr_offs(std::int64_t offset) {
    location.offset = offset;
    if (in_absolute) absolute.offset = offset;
    else offsets_[location.segment] = offset;
    return offset;
}

std::int64_t Assembly::increment_offset(std::int64_t delta) {
    const std::int64_t newoffs = location.offset + delta;
    if (!delta) return newoffs;
    return set_curr_offs(newoffs);
}

void Assembly::forward_refs(Insn& instruction) {
    // Don't bother for -O1.
    if (instruction.opt & OPTIM_DISABLE_FWREF) return;
    if (forwref_ >= forwrefs_.size()) return;
    if (forwrefs_[forwref_].lineno != globallineno) return;

    do {
        instruction.oprs[static_cast<std::size_t>(forwrefs_[forwref_].operand)].opflags |= OPFLAG_FORWARD;
        ++forwref_;
    } while (forwref_ < forwrefs_.size() && forwrefs_[forwref_].lineno == globallineno);

    if (!pass_first()) return;

    for (int i = 0; i < instruction.operands; ++i) {
        if (instruction.oprs[static_cast<std::size_t>(i)].opflags & OPFLAG_FORWARD) forwrefs_.push_back({globallineno, i});
    }
}

// ---- the CPU ----

void Assembly::set_cpu(std::optional<std::string_view> value) {
    if (!value) {
        asm_set_cpu_level(cpu, kCpuNames[0].level);
        return;
    }

    const std::string s(*value);
    std::size_t p = 0;
    char modifier = '+';
    while (p < s.size()) {
        std::size_t end = s.find_first_of(" ,", p);
        if (end == std::string::npos) end = s.size();
        std::size_t len = end - p;

        while (len && (s[p] == '+' || s[p] == '-' || s[p] == '*')) {
            modifier = s[p++];
            --len;
            if (!len && modifier == '*') cpu = cmd_cpu;
        }

        if (len) {
            bool invert_flag = false;

            if (len >= 3 && asm_iprefix(std::string_view(s).substr(p, 2), "no", 2)) {
                invert_flag = true;
                p += 2;
                len -= 2;
            }

            const CpuName* cpuflag = kCpuNames.data();
            for (; cpuflag->name; ++cpuflag)
                if (asm_iprefix(std::string_view(s).substr(p, len), cpuflag->name, len)) break;

            if (!cpuflag->name) {
                nonfatal("unknown CPU type or flag '{}'", std::string_view(s).substr(p, len));
                return;
            }

            if (cpuflag->level >= IF_CPU_FIRST && cpuflag->level <= IF_ANY) {
                asm_set_cpu_level(cpu, cpuflag->level);
            } else {
                switch (modifier) {
                case '-':
                    invert_flag = !invert_flag;
                    break;
                case '*':
                    invert_flag ^= cmd_cpu.test(cpuflag->level);
                    break;
                default:
                    break;
                }

                cpu.set(cpuflag->level);
                if (invert_flag) cpu.clear(cpuflag->level);
            }
        }
        p += len;
        if (p >= s.size()) break;
        ++p;   // skip separator
    }
}

int Assembly::get_bits(std::string_view value) {
    const std::string s(value);
    int i = std::atoi(s.c_str());

    switch (i) {
    case 16:
        break;   // always safe
    case 32:
        if (!iflag_cpu_level_ok(cpu, IF_386)) {
            nonfatal("cannot specify 32-bit segment on processor below a 386");
            i = 16;
        }
        break;
    case 64:
        if (!iflag_cpu_level_ok(cpu, IF_X86_64)) {
            nonfatal("cannot specify 64-bit segment on processor below an x86-64");
            i = 16;
        }
        break;
    default:
        nonfatal("`{}' is not a valid segment size; must be 16, 32 or 64", value);
        i = 16;
        break;
    }
    return i;
}

// ---- limits ----

std::int64_t Assembly::limit_max(NasmLimit limit) {
    if (limit >= LIMIT_MAX) return 0;
    std::int64_t m = kLimitInfo[static_cast<std::size_t>(limit)].max_val;
    if (!m || m > LIMIT_MAX_VAL) m = LIMIT_MAX_VAL;
    return m;
}

std::int64_t Assembly::limit_default(NasmLimit limit) {
    if (limit >= LIMIT_MAX) return 0;
    const std::int64_t m = limit_max(limit);
    const std::int64_t v = kLimitInfo[static_cast<std::size_t>(limit)].default_val;
    return (!v || v > m) ? m : v;
}

const char* Assembly::limit_name(NasmLimit limit) {
    if (limit >= LIMIT_MAX) return nullptr;
    return kLimitInfo[static_cast<std::size_t>(limit)].name;
}

std::string Assembly::limit_str(std::int64_t limit_val) {
    if (limit_val < LIMIT_MAX_VAL) return std::to_string(limit_val);
    return "unlimited";
}

void Assembly::set_default_limits() {
    // (NASM also lowers the eval depth to fit the stack limit where it can know it; there is no such limit here.)
    for (int i = 0; i < LIMIT_MAX; ++i) nasm_limit[static_cast<std::size_t>(i)] = limit_default(static_cast<NasmLimit>(i));
}

DirectiveResult Assembly::set_limit(std::string_view limit, std::string_view valstr) {
    int i = 0;
    for (; i < LIMIT_MAX; ++i) {
        if (asm_iequals(limit, kLimitInfo[static_cast<std::size_t>(i)].name)) break;
    }
    ErrFlags errlevel = 0;
    if (i >= LIMIT_MAX) {
        errlevel = not_started() ? (ERR_WARNING | WARN_OTHER | ERR_USAGE) : (ERR_WARNING | WARN_PRAGMA_UNKNOWN);
        error(errlevel, "unknown limit: `{}'", limit);
        return DIRR_ERROR;
    }

    const auto lim = static_cast<NasmLimit>(i);
    const std::int64_t max_val = limit_max(lim);
    const std::int64_t default_val = limit_default(lim);
    std::int64_t init_val = nasm_limit_from_cmdline[static_cast<std::size_t>(i)];
    if (!init_val) init_val = default_val;

    std::int64_t val = 0;
    if (valstr == "*" || asm_iequals(valstr, "reset") || asm_iequals(valstr, "init")) {
        val = init_val;
    } else if (asm_iequals(valstr, "default")) {
        val = default_val;
    } else if (asm_iequals(valstr, "unlimited") || asm_iequals(valstr, "maximum") || asm_iequals(valstr, "max")) {
        val = max_val;
    } else {
        errlevel = not_started() ? (ERR_WARNING | WARN_OTHER | ERR_USAGE) : (ERR_WARNING | WARN_PRAGMA_BAD);

        bool rn_error = false;
        val = hooks.readnum ? hooks.readnum(valstr, rn_error) : asm_default_readnum(valstr, rn_error);
        if (rn_error || val < 0) {
            error(errlevel, "invalid limit value: `{} {}'", limit, valstr);
            return DIRR_ERROR;
        }

        if (!val) val = init_val;

        if (val > max_val) {
            val = max_val;
            if (max_val < LIMIT_MAX_VAL) {
                error(errlevel, "excessive limit value `{} {}' capped to {}", limit, valstr, max_val);
                // ... but proceed after the warning ...
            }
        }
    }

    nasm_limit[static_cast<std::size_t>(i)] = val;
    return DIRR_OK;
}

std::int64_t Assembly::get_limit(std::string_view limit, GetLimitWhich which) const {
    for (int i = 0; i < LIMIT_MAX; ++i) {
        if (!asm_iequals(limit, kLimitInfo[static_cast<std::size_t>(i)].name)) continue;
        const auto lim = static_cast<NasmLimit>(i);
        switch (which) {
        case GET_LIMIT_CURRENT: return nasm_limit[static_cast<std::size_t>(i)];
        case GET_LIMIT_INIT: return nasm_limit_from_cmdline[static_cast<std::size_t>(i)];
        case GET_LIMIT_DEFAULT: return limit_default(lim);
        case GET_LIMIT_MAX: return limit_max(lim);
        default: return 0;
        }
    }
    return 0;
}

// ---- warnings ----

void Assembly::push_warnings() { warning_stack_.push_back(warning_state); }

void Assembly::pop_warnings() {
    if (warning_stack_.empty()) {   // (not reachable in NASM: the first push happens after the command line)
        warn(WARN_WARN_STACK_EMPTY, "warning stack empty");
        return;
    }
    warning_state = warning_stack_.back();
    if (warning_stack_.size() == 1) {
        warn(WARN_WARN_STACK_EMPTY, "warning stack empty");
    } else {
        warning_stack_.pop_back();
    }
}

// Called after the command line is parsed, but before the first pass.
void Assembly::init_warnings() {
    push_warnings();
    warning_state_init_ = warning_stack_.back();
}

void Assembly::error_init() {
    erropt.worst = 0;
    init_warnings();
}

// Called before each pass. Buffer warnings if "final" is false.
void Assembly::error_pass_start(bool final) {
    if (warn_list_) assert_failed("!warn_list");

    erropt.worst = 0;
    if (!final) warn_list_.emplace();
}

// Called after the completion of each pass; this MUST preserve erropt.worst.
void Assembly::reset_warnings() {
    if (warning_stack_.empty()) return;
    // Unwind the warning stack. We do NOT delete the last entry!
    warning_stack_.resize(1);
    warning_state = warning_stack_.front();
}

void Assembly::error_pass_end() {
    warn_list_.reset();
    reset_warnings();
}

bool Assembly::warn_active(ErrFlags warn_class) const {
    if (warn_class & erropt.never) return false;
    return (warning_state[warn_idx(warn_class)] & WARN_ST_ENABLED) != 0;
}

// This is called when processing a -w or -W option, or a warning directive. Returns ok if the action was successful.
//
// Special pseudo-warnings (see warnings.dat):
// all   - all possible warnings
// other - any warning not specifically assigned a class
bool Assembly::set_warning_status(std::string_view in) {
    enum class Action { Off, On, Reset };

    std::string_view value = in;
    bool has_value = true;   // NASM's "value" is NULL once it means "every class"
    while (!value.empty() && asm_isspace(value.front())) value.remove_prefix(1);

    Action action = Action::On;
    const char first = value.empty() ? '\0' : value.front();
    switch (first) {
    case '-':
        action = Action::Off;
        value.remove_prefix(1);
        break;
    case '+':
        action = Action::On;
        value.remove_prefix(1);
        break;
    case '*':
        action = Action::Reset;
        value.remove_prefix(1);
        break;
    case 'N':
    case 'n':
        if (asm_iprefix(value, "no-", 3)) {
            action = Action::Off;
            value.remove_prefix(3);
        } else if (asm_iequals(value, "none")) {
            action = Action::Off;
            has_value = false;
        } else {
            action = Action::On;
        }
        break;
    default:
        action = Action::On;
        break;
    }

    std::uint8_t mask = WARN_ST_ENABLED;

    if (has_value && asm_iprefix(value, "error", 5)) {
        const char sep = value.size() > 5 ? value[5] : '\0';
        switch (sep) {
        case '=':
            mask = WARN_ST_ERROR;
            value.remove_prefix(6);
            break;
        case '\0':
            mask = WARN_ST_ERROR;
            has_value = false;
            break;
        default:
            break;   // just an accidental prefix?
        }
    }

    if (has_value && asm_iequals(value, "all")) has_value = false;

    const std::size_t vlen = has_value ? value.size() : 0;
    bool ok = false;

    // This is inefficient, but it shouldn't matter. Note: kWarningAlias[0] is "all".
    for (std::size_t a = 1; a < kWarningAlias.size(); ++a) {
        const WarningAlias& wa = kWarningAlias[a];
        const std::size_t i = wa.warning;

        if (has_value) {
            if (!asm_iprefix(value, wa.name, vlen)) continue;   // not a prefix
            const char sep = wa.name[vlen];
            if (sep != '\0' && sep != '-') continue;            // not a valid prefix
        }

        ok = true;   // at least one action taken
        switch (action) {
        case Action::Off:
            warning_state[i] = static_cast<std::uint8_t>(warning_state[i] & ~mask);
            break;
        case Action::On:
            warning_state[i] = static_cast<std::uint8_t>(warning_state[i] | mask);
            break;
        case Action::Reset:
            warning_state[i] = static_cast<std::uint8_t>(warning_state[i] & ~mask);
            warning_state[i] = static_cast<std::uint8_t>(warning_state[i] | (warning_state_init_[i] & mask));
            break;
        }
    }

    if (!ok && has_value) warn(WARN_UNKNOWN_WARNING, "unknown warning name: {}", value);

    return ok;
}

int Assembly::set_error_format(std::string_view fmt) {
    if (asm_iequals(fmt, "vc") || asm_iequals(fmt, "msvc") || asm_iequals(fmt, "ms")) {
        errfmt_ = {kErrFormatMsvc.beforeline, kErrFormatMsvc.afterline, kErrFormatMsvc.beforemsg};
    } else if (asm_iequals(fmt, "gnu") || asm_iequals(fmt, "gcc")) {
        errfmt_ = {kErrFormatGnu.beforeline, kErrFormatGnu.afterline, kErrFormatGnu.beforemsg};
    } else {
        return -1;
    }
    return 0;
}

// ---- reporting ----

bool Assembly::skip_this_pass(ErrFlags severity) const {
    const ErrFlags type = severity & ERR_MASK;

    // See if it's a pass-specific error or warning which should be skipped. We can never skip fatal errors as by
    // definition they cannot be resumed from.
    if (type >= ERR_FATAL) return false;

    // ERR_LISTMSG and ERR_NOTE messages are always skipped; the list file receives them anyway as this function is not
    // consulted for sending to the list file.
    if (type <= ERR_NOTE) return true;

    // This message is not applicable unless it is the last pass we are going to execute; this can be either the final
    // code-generation pass or the single pass executed in preproc-only mode.
    return (severity & ERR_PASS2) && !pass_final_or_preproc();
}

// Check for a suppressed message (usually warnings or notes).
bool Assembly::is_suppressed(ErrFlags flags) {
    const ErrFlags severity = flags & ERR_MASK;
    const ErrFlags lvl = warn_idx(flags);

    // Fatal errors or higher can never be suppressed.
    if (severity >= ERR_FATAL) return false;

    if (flags & erropt.never) return true;

    switch (severity) {
    case ERR_WARNING:
        if (!(warning_state[lvl] & WARN_ST_ENABLED)) return true;
        break;
    case ERR_INFO:
        if (!info_level(lvl)) return true;
        break;
    case ERR_DEBUG:
        if (!debug_level(lvl)) return true;
        break;
    default:
        break;
    }

    // Suppressed by the preprocessor?
    if (!(flags & ERR_PP_LISTMACRO)) return hooks.pp_suppress_error && hooks.pp_suppress_error(flags);

    return false;
}

// The true error type (the ERR_MASK part) of a severity, accounting for warnings that may need to be promoted to error.
ErrFlags Assembly::true_error_type(ErrFlags severity) const {
    constexpr std::uint8_t warn_is_err = WARN_ST_ENABLED | WARN_ST_ERROR;
    ErrFlags type = severity & ERR_MASK;

    if (type == ERR_WARNING) {
        // Promote warning to error?
        const std::uint8_t state = warning_state[warn_idx(severity)];
        if ((state & warn_is_err) == warn_is_err) type = ERR_NONFATAL;
    }
    return type;
}

// The file and line appropriate for use, after some potential filename mangling.
std::pair<std::string, std::int32_t> Assembly::error_where(ErrFlags severity) const {
    if (severity & ERR_NOFILE) return {std::string(), 0};

    const SrcLocation where = src.where_error();
    if (where.filename.empty()) {
        if (!inname.empty()) return {inname, 0};
        if (!outname.empty()) return {outname, 0};
        return {std::string(), 0};
    }
    return {std::string(where.filename), where.lineno};
}

void Assembly::write_line(ErrFlags severity, ErrFlags true_type, std::string line) {
    error_output_ += line;
    error_output_ += '\n';
    diagnostics_.push_back({severity, true_type, std::move(line)});
}

void Assembly::never_returned() {
    // report() throws for these severities; this only satisfies the compiler's idea of what cannot come back.
    throw AsmError("internal error: a fatal error returned");
}

// For fatal/critical/panic errors, end the run. FATAL errors do the cleanups and tidying up the list process (the
// hooks); CRITICAL ones minimize dependencies; PANIC would abort. Here the end is an AsmError, and the status NASM
// would have exited with is kept in exit_code().
void Assembly::die_hard(ErrFlags true_type, ErrFlags severity, const std::string& message) {
    if (true_type < ERR_CRITICAL) {
        // FATAL shutdown, general cleanup actions are valid.
        print_final_report(true);
        if (hooks.final_report) hooks.final_report(true);
    }

    if (hooks.close_output) hooks.close_output(true);

    if (severity & ERR_USAGE) {
        error_output_ += "Usage: " + progname + " [-@ response_file] [options...] [--] filename\n"
                         "   For additional help:\n"
                         "       " + progname + " -h [run|topics|all|-option]\n";
    }

    exit_code_ = static_cast<int>(true_type - ERR_FATAL + 1);
    been_here_ = false;
    throw AsmError(message);
}

// Error reporting for critical and panic errors: minimize the amount of work to get a message out.
void Assembly::report_critical(ErrFlags severity, const std::string& message) {
    if (been_here_) {
        // Recursive critical error... just die.
        throw AsmError("recursive critical error");
    }
    been_here_ = true;

    const ErrFlags true_type = severity & ERR_MASK;
    erropt.worst = true_type;

    auto [filename, lineno] = error_where(severity);
    if (filename.empty()) filename = kNoFileName;

    std::string line = error_prefix(severity);
    line += filename;
    if (lineno) line += errfmt_.beforeline + std::to_string(lineno) + errfmt_.afterline;
    line += errfmt_.beforemsg;
    line += message;
    write_line(severity, true_type, line);

    die_hard(true_type, severity, line);
}

ErrHold Assembly::error_hold_push() {
    errhold_stack_.emplace_back();
    return errhold_stack_.size();
}

ErrFlags Assembly::error_hold_pop(ErrHold hold, bool issue) {
    ErrFlags worst = 0;

    // Allow calling with a null argument saying no hold in the first place.
    if (hold == 0) return worst;

    // This *must* be the current top of the errhold stack.
    if (hold != errhold_stack_.size()) assert_failed("eh == errhold_stack");

    std::vector<ErrText> held = std::move(errhold_stack_.back());
    errhold_stack_.pop_back();

    if (!held.empty()) {
        if (issue) {
            if (!errhold_stack_.empty()) {
                // Commit the current hold list to the previous level.
                std::vector<ErrText>& up = errhold_stack_.back();
                up.insert(up.end(), std::make_move_iterator(held.begin()), std::make_move_iterator(held.end()));
            } else {
                // Issue errors.
                for (ErrText& et : held) {
                    if (et.true_type > worst) worst = et.true_type;
                    issue_error(et);
                }
            }
        } else {
            // Drop errors.
            for (const ErrText& et : held)
                if (et.true_type > worst) worst = et.true_type;
        }
    }
    return worst;
}

// Common error reporting: the common back end of the error reporting schemes. It records the nature of the warning
// and then the specific error message, and may or may not return (it does not return for FATAL and worse).
void Assembly::report(ErrFlags severity, std::string message) {
    const int c_errno = errno;
    const ErrFlags true_type = true_error_type(severity);

    if (true_type >= ERR_CRITICAL) {
        report_critical(severity, message);
        never_returned();
    }

    if (is_suppressed(severity)) return;

    ErrText et;
    et.c_errno = c_errno;
    et.severity = severity;
    et.true_type = true_type;
    et.msg = std::move(message);
    std::tie(et.filename, et.lineno) = error_where(severity);

    if (!errhold_stack_.empty() && true_type <= ERR_NONFATAL) {
        // It is a tentative error.
        errhold_stack_.back().push_back(std::move(et));
    } else {
        issue_error(et);
    }

    // Don't do this before then, if we do, we lose messages in the list file, as the list file is only generated in
    // the last pass.
    if (skip_this_pass(severity)) return;

    if (!(severity & (ERR_HERE | ERR_PP_LISTMACRO)) && hooks.pp_error_list_macros) hooks.pp_error_list_macros(severity);
}

// Actually print, list and take action on an error.
void Assembly::issue_error(ErrText& et) {
    const ErrFlags severity = et.severity;
    const ErrFlags true_type = et.true_type;
    std::string cerrsep;
    std::string cerrmsg;
    bool buffer = true_type < ERR_NONFATAL || (severity & ERR_HOLD);

    const char* pfx = (severity & ERR_NO_SEVERITY) ? "" : error_prefix(true_type);

    std::string warnsuf;
    if (!(severity & (ERR_HERE | ERR_PP_LISTMACRO))) {
        switch (severity & ERR_MASK) {
        case ERR_WARNING: {
            const unsigned lvl = warn_idx(severity);
            warnsuf = std::string(" [-w+") + (true_type >= ERR_NONFATAL ? "error=" : "") + kWarningName[lvl] + "]";
            break;
        }
        case ERR_DEBUG:
            warnsuf = " [--debug=" + std::to_string(erropt.debug_nasm) + "]";
            break;
        case ERR_INFO:
            warnsuf = " [--info=" + std::to_string(erropt.verbose_info) + "]";
            break;
        default:
            break;   // not WARNING, DEBUG or INFO, not suppressible
        }

        if (severity & ERR_PERROR) {
            cerrsep = ":";
            cerrmsg = std::generic_category().message(et.c_errno);   // (strerror)
        }
    }

    std::string linestr;
    if (et.lineno) linestr = errfmt_.beforeline + std::to_string(et.lineno) + errfmt_.afterline;

    std::string line;
    if (!skip_this_pass(severity)) {
        const std::string file = !et.filename.empty() ? et.filename : std::string(kNoFileName);
        std::string here;

        if (severity & ERR_HERE) here = !et.filename.empty() ? " here" : " in an unknown location";

        if (!warn_list_) buffer = false;

        line = file + linestr + errfmt_.beforemsg + pfx + et.msg + cerrsep + cerrmsg + here + warnsuf;

        if (buffer) {
            // Buffer up warnings and held errors until we either get an error or we are on the code-generation pass.
            warn_list_->push_back({severity, true_type, line});
        } else {
            // Actually output an error. If we have buffered warnings, and this is a non-warning, output them now.
            if (warn_list_) {
                for (Diagnostic& d : *warn_list_) write_line(d.severity, d.true_type, std::move(d.text));
                warn_list_.reset();
            }
            write_line(severity, true_type, line);
        }
    }

    // Are we recursing from error_list_macros?
    if (severity & ERR_PP_LISTMACRO) return;

    // Don't suppress this with skip_this_pass(), or we don't get pass1 or preprocessor warnings in the list file.
    if (hooks.list_error) {
        if (severity & ERR_HERE) {
            if (et.lineno) hooks.list_error(severity, std::string(pfx) + et.msg + " at " + et.filename + ":" + std::to_string(et.lineno) + warnsuf);
            else if (!et.filename.empty()) hooks.list_error(severity, std::string(pfx) + et.msg + " in file " + et.filename + warnsuf);
            else hooks.list_error(severity, std::string(pfx) + et.msg + " in an unknown location" + warnsuf);
        } else {
            hooks.list_error(severity, std::string(pfx) + et.msg + warnsuf);
        }
    }

    if (skip_this_pass(severity)) return;

    if (true_type >= ERR_FATAL) {
        die_hard(true_type, severity, line);
    } else if (!buffer) {
        if (true_type > erropt.worst) erropt.worst = true_type;
        if (true_type >= ERR_NONFATAL) erropt.never |= ERR_UNDEAD;
    }
}

void Assembly::print_final_report(bool failure) {
    // This test is here to reduce the likelihood of a recursive failure.
    if (erropt.verbose_info >= 1) {
        const PassType t = pass_type();
        if (t >= PASS_FIRST) {
            const unsigned end_passes = t > PASS_OPT ? static_cast<unsigned>(t - PASS_OPT) : 0u;
            info(1, "assembly {} after 1+{}+{} passes", failure ? "failed" : "completed", pass_count() - 1 - end_passes, end_passes);
        }
    }
}

void Assembly::panic_here(std::source_location where) {
    const char* func = where.function_name();
    if (!func || !*func) func = "<unknown>";
    panic("internal error in {} at {}:{}\n", func, where.file_name(), where.line());
}

void Assembly::assert_failed(const char* text, std::source_location where) {
    const char* func = where.function_name();
    if (!func || !*func) func = "<unknown>";
    panic("assertion {} failed in {} at {}:{}", text, func, where.file_name(), where.line());
}

void Assembly::warn_dollar_hex() { warn(WARN_NUMBER_DEPRECATED_HEX, "$ prefix for hexadecimal is deprecated"); }

}  // namespace cb::build::x86asm
