// cb/build/x86asm/preproc.cpp -- the preprocessor's input side and the pass loop: opening files (through the host),
// reading lines, the stack of open inputs, `%include` search, the predefined lines, and pp_getline. (preproc.c: pp_reset,
// pp_getline, pp_tokline, read_line, line_from_file, inc_fopen, pp_pre_define ...)
#include "cb/build/x86asm/preproc.hpp"

#include "cb/build/x86asm/preproc_masm.hpp"

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

namespace {

// How NASM joins a directory and a file name on this platform (nasm_catfile): a separator is added unless the directory is
// empty or ends in one.
std::string pp_catfile(const std::string& dir, const std::string& file) {
    if (dir.empty()) return file;
    const char last = dir.back();
    if (last == '/' || last == '\\' || last == ':') return dir + file;
    return dir + '\\' + file;
}

}  // namespace

Preproc::Preproc(Assembly& assembly, PpHost host) : as_(assembly), host_(std::move(host)) {
    masm_ = std::make_unique<MasmFront>(as_, [this] { return istk_ && !istk_->conds.empty() && !emitting(istk_->conds.back()); });
    as_.hooks.pp_suppress_error = [this](ErrFlags severity) { return suppress_error(severity); };
    as_.hooks.pp_error_list_macros = [this](ErrFlags severity) { error_list_macros(severity); };
    use_loaded_.assign(use_packages().size(), false);
}

Preproc::~Preproc() {
    as_.hooks.pp_suppress_error = nullptr;
    as_.hooks.pp_error_list_macros = nullptr;
}

void Preproc::init(int opt) {
    ppopt_ = opt;
    use_loaded_.assign(use_packages().size(), false);
}

void Preproc::masm_prescan(const std::string& text) {
    if (masm_) masm_->prescan_consts(text);
}

bool Preproc::suppress_error(ErrFlags severity) const {
    if (istk_ && !istk_->conds.empty() &&
        ((severity & ERR_PP_PRECOND) ? istk_->conds.back() == CondState::NEVER : !emitting(istk_->conds.back())))
        return true;
    return false;
}

void Preproc::error_list_macros(ErrFlags severity) {
    severity |= ERR_PP_LISTMACRO | ERR_NO_SEVERITY | ERR_HERE;
    while (const void* mp = as_.src.error_down()) {
        const MMacro* m = static_cast<const MMacro*>(mp);
        if ((m->nolist & NL_LIST) || m->where.filename.empty()) break;
        as_.report(severity, "... from macro `" + m->name + "' defined");
    }
    as_.src.error_reset();
}

// ---- opening files ----

// Look for a file: as named, then under each -I directory (unless INC_EXACT). The text of the file, if one was found
// (with `probe` only whether it exists matters).
std::optional<std::string> Preproc::inc_search(std::string_view file, std::string* slpath, int* omp, bool probe) {
    (void)probe;
    const int omode = *omp;
    std::size_t ip = 0;
    const std::string name(file);
    std::string prefix;
    const bool use_ipath = !(omode & INC_EXACT);
    while (true) {
        const std::string sp = pp_catfile(prefix, name);
        std::optional<std::string> data = host_.read_file ? host_.read_file(sp) : std::nullopt;
        if (data) {
            *slpath = sp;
            if (prefix.empty()) *omp |= INC_EXACT;   // found as named: this is an exact path
            return data;
        }
        if (!use_ipath || ip >= ipath_.size()) {
            slpath->clear();
            return std::nullopt;
        }
        prefix = ipath_[ip++];
    }
}

// Open an include (or probe for one): see NASM's inc_fopen. `omode` is a mix of the INC_* flags; the include path entry
// is added to `dhead` (the dependency list). `found_fhe` gets the record of the file if found. Returns true when the file
// was opened (and its text is in *text).
bool Preproc::inc_open(std::string_view file, std::vector<std::string>* dhead, FileHashEntry** found_fhe, int omode, std::string* text) {
    FileHashEntry* fhe = nullptr;
    std::string path;
    bool have_path = false;
    std::optional<std::string> fp;   // the text, when the file was opened
    const std::int64_t pass = as_.pass_count();
    bool skip_open = !!(omode & INC_PROBE);
    const std::string key(file);

    auto hit = file_hash_.find(key);
    if (hit != file_hash_.end()) {
        fhe = hit->second.get();
        have_path = fhe->has_path;
        path = fhe->path;
        if ((omode ^ fhe->omode) & INC_EXACT) {
            if (omode & INC_EXACT) have_path = false;   // an exact open does not take a path found by search
            else if (!have_path) fhe = nullptr;          // a search must be redone for a name that was only tried exact
        }
    }

    if (!fhe) {
        std::string pptr;
        std::optional<std::string> data = inc_search(file, &pptr, &omode, !!(omode & INC_PROBE));
        have_path = data.has_value();
        path = pptr;
        if (!(omode & INC_PROBE)) fp = std::move(data);

        auto entry = std::make_unique<FileHashEntry>();
        fhe = entry.get();
        fhe->has_path = have_path;
        fhe->path = path;
        fhe->full = fhe;
        fhe->omode = omode & INC_EXACT;
        file_hash_[key] = std::move(entry);   // replaces an older record of the name

        if (have_path) {
            const std::string fullpath = host_.realpath ? host_.realpath(path) : path;
            if (key != fullpath) {
                // The same file may be reached by other names: they share one "full" record (for %require)
                auto fit = file_hash_.find(fullpath);
                FileHashEntry* full;
                if (fit != file_hash_.end()) {
                    full = fit->second.get();
                } else {
                    auto f = std::make_unique<FileHashEntry>();
                    full = f.get();
                    full->has_path = true;
                    full->path = fullpath;
                    full->full = full;
                    full->omode = INC_EXACT;
                    file_hash_[fullpath] = std::move(f);
                }
                fhe->full = full;
            }
        }
    }

    if (dhead) dhead->push_back(have_path ? path : key);

    if (have_path) {
        skip_open |= ((omode | fhe->full->omode) & INC_REQUIRED) && (fhe->full->include_pass >= pass);
        if (!skip_open) {
            fp = host_.read_file ? host_.read_file(path) : std::nullopt;
            if (fp) fhe->full->include_pass = pass;
        }
    }

    if (!fp && !skip_open && (omode & INC_NEEDED))
        as_.nonfatal("unable to open include file `{}': {}", file, "No such file or directory");

    if (found_fhe) *found_fhe = have_path ? fhe : nullptr;
    if (fp && text) *text = std::move(*fp);
    return fp.has_value();
}

// ---- reading lines ----

// The next line of the file (continued lines joined, comments left in), or nullopt at the end.
std::optional<std::string> Preproc::line_from_file(Include* inc) {
    inc->where.lineno += inc->lineskip + inc->lineinc;
    as_.src.set_linnum(inc->where.lineno);
    inc->lineskip = 0;

    const std::string& d = inc->data;
    std::size_t& pos = inc->datapos;
    std::string buf;
    bool cont = false;
    while (true) {
        if (pos >= d.size()) {
            if (buf.empty()) return std::nullopt;   // nothing at all: the end of the file
            break;                                  // the last line has no newline
        }
        const char c = d[pos++];
        if (c == '\r') {
            if (pos < d.size() && d[pos] == '\n') pos++;   // "\r\n" is one line end
            if (cont) {
                cont = false;
                continue;
            }
            break;
        }
        if (c == '\n') {
            if (cont) {
                cont = false;
                continue;
            }
            break;
        }
        if (c == 032 || c == 0) break;   // ^Z (and a NUL, which ends a C string) end the line
        if (c == '\\' && pos < d.size() && (d[pos] == '\r' || d[pos] == '\n')) {
            cont = true;   // a backslash at the end of the line continues it
            inc->lineskip += inc->lineinc;
            continue;
        }
        buf.push_back(c);
    }
    return buf;
}

// The next line of the standard macro blocks being fed; at their end the predefined lines are injected.
std::optional<std::string> Preproc::line_from_stdmac() {
    if (!stdcur_ || stdpos_ >= stdcur_->size()) {
        const std::vector<std::string_view>* next = stdmaclist_ < stdmacset_.size() ? stdmacset_[stdmaclist_] : nullptr;
        stdcur_ = nullptr;
        if (!next) {
            if (do_predef_) inject_predefs();
            return std::nullopt;
        }
        stdmacset_[stdmaclist_++] = nullptr;
        stdcur_ = next;
        stdpos_ = 0;
        if (stdcur_->empty()) return line_from_stdmac();
    }
    return std::string((*stdcur_)[stdpos_++]);
}

void Preproc::inject_predefs() {
    for (Line* pd = predef_; pd; pd = pd->next) {
        Line* l = new_line();
        l->next = istk_->expansion;
        l->first = dup_tlist(pd->first, nullptr);
        istk_->expansion = l;
    }
    do_predef_ = false;
}

// The next source line from the current input (the MASM front end first hands out what it queued).
std::optional<std::string> Preproc::read_line() {
    std::optional<std::string> line;
    if (as_.masm_mode) {
        line = masm_->take_queued();
        if (line) return line;
    }
    const bool is_file = istk_->is_file;
    line = is_file ? line_from_file(istk_) : line_from_stdmac();
    if (!line) return std::nullopt;

    if (as_.masm_mode && is_file && !(!istk_->conds.empty() && !emitting(istk_->conds.back()))) {
        // The END directive stops reading the file
        const char* q = line->c_str();
        while (*q == ' ' || *q == '\t') q++;
        if (!strnicmp_(q, "end", 3) && !isidchar(q[3])) {
            const char* r = q + 3;
            while (*r == ' ' || *r == '\t') r++;
            if (*r != ':' && *r != '=') {
                istk_->datapos = istk_->data.size();
                return std::nullopt;
            }
        }
    }
    if (as_.masm_mode) line = masm_->xform(std::move(*line));

    if (!istk_->nolist && host_.list_line) host_.list_line(LIST_READ, istk_->where.lineno, *line);
    return line;
}

// ---- the standard macro blocks ----

void Preproc::pp_add_stdmac(const std::vector<std::string_view>* macros) {
    std::size_t i = 0;
    for (; i < stdmacset_.size() && stdmacset_[i]; ++i)
        if (stdmacset_[i] == macros) return;   // already queued
    if (i >= stdmacset_.size() - 1) as_.assert_failed("mp < &stdmacset[ARRAY_SIZE(stdmacset)-1]");
    stdmacset_[i] = macros;
}

// Start reading standard macros: a new input level that is fed from the queued blocks.
void Preproc::pp_start_stdmac() {
    stdmaclist_ = 0;
    Include* inc = new Include();
    inc->next = istk_;
    inc->nolist = inc->noline = !(host_.list_option && host_.list_option('b'));
    inc->where = istk_->where;
    istk_ = inc;
    if (!istk_->nolist && host_.list_uplevel) host_.list_uplevel(LIST_INCLUDE);
    if (!istk_->noline) {
        as_.src.set(0, std::string_view());
        istk_->where = as_.src.where();
    }
}

void Preproc::pp_add_limits_stdmac() {
    Tok* t = nullptr;
    for (int i = LIMIT_MAX - 1; i > 0; i--) {
        t = make_tok_qstr(t, Assembly::limit_name(static_cast<NasmLimit>(i)));
        t = make_tok_char(t, ',');
    }
    t = make_tok_qstr(t, Assembly::limit_name(static_cast<NasmLimit>(0)));
    define_smacro("__?NASM_LIMITS?__", true, t, nullptr);
}

void Preproc::pp_add_list_options_default_stdmac() {
    define_smacro("__?LIST_OPTIONS_DEFAULT?__", true, get_list_options(cmdline_list_options_), nullptr);
}

void Preproc::pp_reset_stdmac(PreprocMode mode) {
    int apass;
    pp_start_stdmac();
    pp_add_magic_stdmac();
    if (as_.tasm_compatible_mode) pp_add_stdmac(&standard_macros(StandardMacros::Tasm));
    pp_add_stdmac(&standard_macros(StandardMacros::Nasm));
    pp_add_stdmac(&standard_macros(StandardMacros::Version));
    if (format_macros_) {
        format_views_.assign(format_macros_->begin(), format_macros_->end());
        pp_add_stdmac(&format_views_);
    }
    pp_add_limits_stdmac();
    pp_add_list_options_default_stdmac();
    do_predef_ = true;
    switch (mode) {
        case PP_NORMAL: apass = as_.pass_final() ? 2 : 1; break;
        case PP_DEPS: apass = 0; break;
        case PP_PREPROC: apass = 3; break;
        default: as_.panic_here();
    }
    define_smacro("__?PASS?__", true, make_tok_num(nullptr, apass), nullptr);
}

void Preproc::reset(std::string_view file, PreprocMode mode, std::vector<std::string>* dep_list) {
    cstk_ = nullptr;
    defining_ = nullptr;
    nested_mac_count_ = 0;
    nested_rep_count_ = 0;
    unique_ = 0;
    masm_->reset();
    deplist_ = dep_list;
    pp_mode_ = mode;
    ppconf_ = Config();
    ppdbg_ = 0;
    if (!(ppopt_ & PP_TRIVIAL)) {
        if (host_.list_option && host_.list_option('s')) ppdbg_ |= PDBG_LIST_SMACROS;
    }
    use_loaded_.assign(use_packages().size(), false);
    stdcur_ = nullptr;
    stdpos_ = 0;
    stdmacset_.fill(nullptr);
    stdmaclist_ = 0;

    istk_ = new Include();
    std::optional<std::string> data = host_.read_file ? host_.read_file(std::string(file)) : std::nullopt;
    if (!data) {
        delete istk_;
        istk_ = nullptr;
        as_.fatalf(ERR_NOFILE, "unable to open input file `{}'{}{}", file, " ", "No such file or directory");
    }
    istk_->is_file = true;
    istk_->data = std::move(*data);
    if (as_.masm_mode) masm_->prescan_consts(istk_->data);
    as_.src.set(0, file);
    istk_->where = as_.src.where();
    istk_->lineinc = 1;

    if (deplist_) deplist_->push_back(std::string(file));
    do_predef_ = false;
    if (!(ppopt_ & PP_TRIVIAL)) pp_reset_stdmac(mode);
}

// Pop the current input; its macro stacks are released by the caller.
Include* Preproc::pop_include_stack() {
    Include* i = istk_;
    if (!i->conds.empty()) {
        // The message names where the unfinished %if was; nasm_fatal throws, so release first
        as_.fatal("expected `%endif' before end of {}", i->is_file ? "file" : "macro package");
    }
    istk_ = i->next;
    if (!i->nolist && host_.list_downlevel) host_.list_downlevel(LIST_INCLUDE);
    if (!i->noline) {
        const SrcLocation whereto = istk_ ? istk_->where : SrcLocation();
        as_.src.update(whereto);
    }
    return i;
}

// ---- the line loop ----

// The next line to give the assembler, as tokens: a source line with its macros expanded; or &tok_pop when an input or macro
// level was popped (the caller looks again); or null for a line that vanished (a directive).
Tok* Preproc::pp_tokline() {
    while (true) {
        Line* l = istk_->expansion;
        Tok* tline = nullptr;
        Tok* dtline;
        std::optional<std::string> line;
        bool suppressed = false;

        // Fast-path: a line that finishes a macro or %rep block
        while (l && l->finishes) {
            MMacro* fm = l->finishes;

            if (!fm->has_name && fm->in_progress > 1 && !l->suppressed) {
                // A %rep block with passes left: run it again (the block's lines are pushed once more)
                fm->in_progress--;
                for (l = fm->expansion; l; l = l->next) {
                    Line* ll = new_line();
                    ll->next = istk_->expansion;
                    ll->first = dup_tlist(l->first, nullptr);
                    ll->where = l->where;
                    istk_->expansion = ll;
                }
                l = istk_->expansion;
                continue;
            } else {
                // The block is finished: pop it
                MMacro* m = istk_->mstk.mstk;
                if (m != fm) as_.assert_failed("m == fm");

                if (defining_) {
                    if (defining_->has_name) as_.panic("defining with name in expansion");
                    else if (m->has_name)
                        as_.fatal("`%rep' without `%endrep' within expansion of macro `{}'", m->name);
                }

                if (m->has_name) {
                    // A macro: its invocation state is released
                    if (!--mmacro_deadman_.levels) mmacro_deadman_ = Deadman();
                    clear_mmacro(m);
                    fm->in_progress = 0;
                }

                if (fm->nolist & NL_LINE) {
                    istk_->noline--;
                } else if (!istk_->noline) {
                    MMacro* sm = const_cast<MMacro*>(static_cast<const MMacro*>(as_.src.macro_current()));
                    if (sm == fm) {
                        as_.src.macro_pop();
                        put_mmacro(&sm);
                    }
                    as_.src.update(l->where);
                }
                if (fm->nolist & NL_LIST) {
                    istk_->nolist--;
                } else if (!istk_->nolist) {
                    if (host_.list_downlevel) host_.list_downlevel(LIST_MACRO);
                }
                istk_->where = l->where;
                pop_mstk(&istk_->mstk, m);
            }

            istk_->expansion = l->next;
            free_line(l);
            return &tok_pop_;
        }

        if (istk_->expansion) {
            // From a macro expansion: tokens already
            Line* ln = istk_->expansion;
            istk_->expansion = ln->next;
            istk_->where = ln->where;
            suppressed = ln->suppressed;
            if (!istk_->noline) as_.src.update(istk_->where);
            tline = ln->first;
            ln->first = nullptr;
            if (!istk_->nolist && !suppressed && host_.list_line) {
                const std::string listline = detoken(tline, false);
                host_.list_line(LIST_MACRO, istk_->where.lineno, listline);
            }
            free_line(ln);
        } else if ((line = read_line())) {
            tline = tokenize(*line);
        } else if (istk_->expansion) {
            // Reading a line queued something (the predefined lines): go round
            continue;
        } else {
            // The end of this input
            Include* i = pop_include_stack();
            put_mmacro(&i->mstk.mstk);
            put_mmacro(&i->mstk.mmac);
            delete i;
            return &tok_pop_;
        }

        // Skipped text and macro-definition text are not expanded
        suppressed |= !istk_->conds.empty() && !emitting(istk_->conds.back());
        if (!defining_ && !suppressed) tline = expand_mmac_params(tline);

        // Is it a directive?
        if (do_directive(tline, &dtline, suppressed) == DIRECTIVE_FOUND) {
            if (dtline) return dtline;
        } else if (defining_) {
            // Inside a macro or %rep definition: collect the line
            MMacro* mmac = defining_->dstk.mmac;
            Line* nl = new_line();
            nl->next = defining_->expansion;
            nl->first = tline;
            nl->finishes = nullptr;
            nl->where = istk_->where;
            defining_->expansion = nl;
            if (mmac) {
                // %00 anywhere in the definition: the macro takes the label
                for (const Tok* t = tline; t; t = t->next)
                    if (t->type == MMACRO_PARAM && t->text == "%00") mmac->capture_label = true;
            }
        } else if (suppressed) {
            delete_tlist(tline);
        } else if (as_.masm_mode) {
            // MASM: a macro call is tried before single-line macros are expanded (so a symbol named like a macro wins)
            if (!expand_mmacro(tline)) {
                tline = expand_smacro(tline);
                if (!expand_mmacro(tline)) return tline;
            }
        } else {
            tline = expand_smacro(tline);
            if (!expand_mmacro(tline)) return tline;
        }
    }
}

std::optional<std::string> Preproc::getline() {
    std::optional<std::string> line;
    while (true) {
        Tok* tline = pp_tokline();
        if (tline == &tok_pop_) {
            if (!istk_) break;
        } else {
            line = detoken(tline, true);
            delete_tlist(tline);
            break;
        }
    }
    if (host_.list_option && host_.list_option('e') && istk_ && !istk_->nolist && line && !line->empty() && host_.list_line)
        host_.list_line(LIST_MACRO, -1, " ;;; " + *line);
    return line;
}

void Preproc::cleanup_pass() {
    if (defining_) {
        if (defining_->has_name) as_.nonfatal("end of input while still defining macro `{}'", defining_->name);
        else as_.nonfatal("end of input while still in %rep");
        defining_->refcnt = 0;   // the definition goes with the pass
        free_mmacro(defining_);
        defining_ = nullptr;
    }
    while (cstk_) ctx_pop();
    free_macros();
    while (istk_) {
        Include* i = pop_include_stack();
        delete i;
    }
}

void Preproc::cleanup_session() {
    free_llist(predef_);
    predef_ = nullptr;
    ipath_.clear();
}

// ---- lines to run before the source (the command line's -P -D -U -p) ----

void Preproc::pre_include(std::string_view fname) {
    Tok* name = new_token(nullptr, INTERNAL_STR, fname);
    Tok* space = new_white(name);
    Tok* inc = new_token(space, PREPROC_ID, "%include");
    Line* l = new_line();
    l->next = predef_;
    l->first = inc;
    l->finishes = nullptr;
    predef_ = l;
}

void Preproc::pre_define(std::string_view definition) {
    std::string def(definition);
    Tok* space = new_white(nullptr);
    Tok* dtok = new_token(space, PREPROC_ID, "%define");
    const std::size_t eq = def.find('=');
    if (eq != std::string::npos) def[eq] = ' ';   // NAME=value is "%define NAME value"
    space->next = tokenize(def);
    Line* l = new_line();
    l->next = predef_;
    l->first = dtok;
    l->finishes = nullptr;
    predef_ = l;
}

void Preproc::pre_undefine(std::string_view definition) {
    Tok* space = new_white(nullptr);
    Tok* dtok = new_token(space, PREPROC_ID, "%undef");
    space->next = tokenize(definition);
    Line* l = new_line();
    l->next = predef_;
    l->first = dtok;
    l->finishes = nullptr;
    predef_ = l;
}

void Preproc::pre_command(std::optional<std::string_view> what, std::string_view string) {
    Tok* def = tokenize(string);
    if (what) {
        Tok* wt = tokenize(*what);
        Tok** tailp = tlist_endptr(&wt);
        *tailp = new_white(def);
        def = wt;
    }
    Line* l = new_line();
    l->next = predef_;
    l->first = def;
    l->finishes = nullptr;
    predef_ = l;
}

}  // namespace cb::build::x86asm
