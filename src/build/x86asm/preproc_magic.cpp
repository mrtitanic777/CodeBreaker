// build/x86asm/preproc_magic.cpp -- the "magic" single-line macros NASM implements in code: __?FILE?__, __?LINE?__,
// __?BITS?__, __?PTR?__, and the %-functions (%abs, %chr, %strcat, %substr, %num, %map, %sel, %eval, %is..., ...).
// (preproc.c: stdmac_* and pp_add_magic_*)
#include <cstdio>

#include "build/x86asm/preproc.hpp"

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

SMacro* Preproc::define_magic(std::string_view mname, bool casesense, SMacro* tmpl) {
    const int op = pp_get_directive(mname);
    if (op != PP_invalid) pp_op_may_be_function_[static_cast<std::size_t>(op)] = true;
    if (tmpl && tmpl->nparam && tmpl->expand) tmpl->recursive = true;
    return define_smacro(mname, casesense, nullptr, tmpl);
}

void Preproc::pp_add_magic_simple() {
    struct MagicMacro {
        const char* name;
        bool casesense;
        int nparam;
        int flags;
        ExpandFn func;
    };
    static const MagicMacro magic_macros[] = {
        {"__?FILE?__", true, 0, 0, &Preproc::stdmac_file},
        {"__?LINE?__", true, 0, 0, &Preproc::stdmac_line},
        {"__?BITS?__", true, 0, 0, &Preproc::stdmac_bits},
        {"__?PTR?__", true, 0, 0, &Preproc::stdmac_ptr},
        {"__?DEFAULT?__", true, 0, 0, &Preproc::stdmac_default},
        {"__?LIST_OPTIONS?__", true, 0, 0, &Preproc::stdmac_list_options},
        {"%abs", false, 1, SPARM_EVAL, &Preproc::stdmac_abs},
        {"%chr", false, 1, SPARM_EVAL | SPARM_OPTIONAL | SPARM_VARADIC, &Preproc::stdmac_chr},
        {"%count", false, 1, SPARM_VARADIC, &Preproc::stdmac_count},
        {"%depend", false, 1, SPARM_PLAIN, &Preproc::stdmac_depend},
        {"%eval", false, 1, SPARM_EVAL | SPARM_VARADIC, &Preproc::stdmac_join},
        {"%hs2b", false, 1, SPARM_STR | SPARM_CONDQUOTE | SPARM_VARADIC, &Preproc::stdmac_hs2b},
        {"%map", false, 1, SPARM_VARADIC, &Preproc::stdmac_map},
        {"%null", false, 1, SPARM_GREEDY, &Preproc::stdmac_null},
        {"%pathsearch", false, 1, SPARM_PLAIN, &Preproc::stdmac_pathsearch},
        {"%realpath", false, 1, SPARM_PLAIN, &Preproc::stdmac_realpath},
        {"%selbits", false, 1, SPARM_PLAIN | SPARM_VARADIC, &Preproc::stdmac_selbits},
        {"%str", false, 1, SPARM_GREEDY | SPARM_STR, &Preproc::stdmac_join},
        {"%strcat", false, 1, SPARM_STR | SPARM_CONDQUOTE | SPARM_VARADIC, &Preproc::stdmac_strcat},
        {"%strlen", false, 1, SPARM_STR | SPARM_CONDQUOTE, &Preproc::stdmac_strlen},
        {"%tok", false, 1, SPARM_STR | SPARM_CONDQUOTE, &Preproc::stdmac_tok},
    };
    SMacro tmpl;
    for (const MagicMacro& m : magic_macros) {
        tmpl.nparam = m.nparam;
        tmpl.expand = m.func;
        tmpl.params.clear();
        if (m.nparam) {
            int flags = m.flags;
            tmpl.params.resize(static_cast<std::size_t>(m.nparam));
            for (int i = m.nparam - 1; i >= 0; i--) {
                tmpl.params[static_cast<std::size_t>(i)].flags = flags;
                flags &= ~(SPARM_GREEDY | SPARM_VARADIC | SPARM_OPTIONAL);   // only the last parameter is greedy
            }
        }
        define_magic(m.name, m.casesense, &tmpl);
    }
}

// The %is... functions: one per condition test, "%isdef(x)" is the same as "%ifdef x" as a number.
void Preproc::pp_add_magic_isfunc() {
    SMacro tmpl;
    tmpl.nparam = 1;
    tmpl.expand = &Preproc::stdmac_is;
    for (int pt = PP_IF; pt < (PP_IFN + (PP_IFN - PP_IF)); pt++) {
        const char* dn = pp_directive_name(pt);
        if (!dn) continue;
        tmpl.params.assign(1, SmacParam());
        tmpl.params[0].flags = SPARM_GREEDY;
        const std::string name = std::string("%is") + (dn + 3);
        tmpl.expandpvt = static_cast<std::uint64_t>(pt);
        define_magic(name, false, &tmpl);
    }
}

void Preproc::pp_add_magic_miscfunc() {
    SMacro tmpl;

    tmpl.nparam = 1;
    tmpl.expand = &Preproc::stdmac_join;
    tmpl.params.assign(1, SmacParam());
    tmpl.params[0].flags = SPARM_EVAL | SPARM_UNSIGNED | SPARM_VARADIC;
    tmpl.params[0].radix = 'x';
    define_magic("%hex", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 2;
    tmpl.expand = &Preproc::stdmac_sel;
    tmpl.params.assign(2, SmacParam());
    tmpl.params[0].flags = SPARM_EVAL;
    tmpl.params[1].flags = SPARM_VARADIC;
    define_magic("%sel", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 3;
    tmpl.expand = &Preproc::stdmac_cond;
    tmpl.params.assign(3, SmacParam());
    tmpl.params[0].flags = SPARM_EVAL;
    tmpl.params[1].flags = 0;
    tmpl.params[2].flags = SPARM_OPTIONAL;
    define_magic("%cond", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 3;
    tmpl.expand = &Preproc::stdmac_num;
    tmpl.recursive = true;
    tmpl.params.assign(3, SmacParam());
    tmpl.params[0].flags = SPARM_EVAL;
    tmpl.params[1].flags = SPARM_EVAL | SPARM_OPTIONAL;
    tmpl.params[1].def = make_tok_num(nullptr, -1);
    tmpl.params[2].flags = SPARM_EVAL | SPARM_OPTIONAL;
    tmpl.params[2].def = make_tok_num(nullptr, 10);
    define_magic("%num", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 3;
    tmpl.expand = &Preproc::stdmac_substr;
    tmpl.recursive = true;
    tmpl.params.assign(3, SmacParam());
    tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
    tmpl.params[1].flags = SPARM_EVAL;
    tmpl.params[2].flags = SPARM_EVAL | SPARM_OPTIONAL;
    tmpl.params[2].def = make_tok_num(nullptr, -1);
    define_magic("%substr", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 3;
    tmpl.expand = &Preproc::stdmac_ord;
    tmpl.recursive = true;
    tmpl.params.assign(3, SmacParam());
    tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
    tmpl.params[1].flags = SPARM_EVAL | SPARM_OPTIONAL;
    tmpl.params[1].def = make_tok_num(nullptr, 1);
    tmpl.params[2].flags = SPARM_EVAL | SPARM_OPTIONAL;
    tmpl.params[2].def = make_tok_num(nullptr, 1);
    define_magic("%ord", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 2;
    tmpl.expand = &Preproc::stdmac_b2hs;
    tmpl.recursive = true;
    tmpl.params.assign(2, SmacParam());
    tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
    tmpl.params[1].flags = SPARM_STR | SPARM_CONDQUOTE | SPARM_OPTIONAL;
    tmpl.params[1].def = make_tok_qstr(nullptr, "");
    define_magic("%b2hs", false, &tmpl);

    tmpl = SMacro();
    tmpl.nparam = 2;
    tmpl.expand = &Preproc::stdmac_limit;
    tmpl.recursive = true;
    tmpl.params.assign(2, SmacParam());
    tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
    tmpl.params[1].flags = SPARM_STR | SPARM_CONDQUOTE | SPARM_OPTIONAL;
    define_magic("%limit", false, &tmpl);

    for (int i = 1; i <= 2; i++) {
        tmpl = SMacro();
        tmpl.nparam = i;
        tmpl.expand = &Preproc::stdmac_env;
        tmpl.recursive = true;
        tmpl.params.assign(static_cast<std::size_t>(i), SmacParam());
        tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
        if (i > 1) tmpl.params[1].flags = SPARM_GREEDY;
        define_magic("%env", false, &tmpl);
    }

    for (int i = 0; i < 2; i++) {
        static const char* const names[] = {"%findi", "%find"};
        tmpl = SMacro();
        tmpl.nparam = 2;
        tmpl.expand = &Preproc::stdmac_find;
        tmpl.recursive = true;
        tmpl.params.assign(2, SmacParam());
        tmpl.params[0].flags = SPARM_STR | SPARM_CONDQUOTE;
        tmpl.params[1].flags = SPARM_VARADIC | SPARM_OPTIONAL;
        tmpl.expandpvt = static_cast<std::uint64_t>(i);
        define_magic(names[i], false, &tmpl);
    }
}

void Preproc::pp_add_magic_stdmac() {
    pp_add_magic_simple();
    pp_add_magic_isfunc();
    pp_add_magic_miscfunc();
}

// ---- the expansion functions ----

Tok* Preproc::stdmac_file(const SMacro*, Tok**, int) {
    const std::string_view fname = as_.src.get_fname();
    return fname.empty() ? nullptr : make_tok_qstr(nullptr, fname);
}

Tok* Preproc::stdmac_line(const SMacro*, Tok**, int) { return make_tok_num(nullptr, as_.src.get_linnum()); }

Tok* Preproc::stdmac_bits(const SMacro*, Tok**, int) { return make_tok_num(nullptr, as_.globl.bits); }

Tok* Preproc::stdmac_ptr(const SMacro*, Tok**, int) {
    switch (as_.globl.bits) {
        case 16: return new_token(nullptr, ID, "word");
        case 32: return new_token(nullptr, ID, "dword");
        case 64: return new_token(nullptr, ID, "qword");
        default: as_.panic_here();
    }
}

// __?DEFAULT?__: the DEFAULT directive's state as "rel|abs, fs:rel|abs, gs:rel|abs, bnd|nobnd".
Tok* Preproc::stdmac_default(const SMacro*, Tok**, int) {
    Tok* t = nullptr;
    t = new_token(t, ID, (as_.globl.rel & EAF_NOTFSGS) ? "rel" : "abs");
    t = make_tok_char(t, ',');
    t = new_token(t, ID, "fs");
    t = make_tok_char(t, ':');
    t = new_token(t, ID, (as_.globl.rel & EAF_FS) ? "rel" : "abs");
    t = make_tok_char(t, ',');
    t = new_token(t, ID, "gs");
    t = make_tok_char(t, ':');
    t = new_token(t, ID, (as_.globl.rel & EAF_GS) ? "rel" : "abs");
    t = make_tok_char(t, ',');
    t = new_token(t, ID, as_.globl.bnd ? "bnd" : "nobnd");
    return t;
}

Tok* Preproc::stdmac_is(const SMacro* s, Tok** params, int) {
    Tok* pline = params[0];
    params[0] = nullptr;   // if_condition takes the list
    const int retval = if_condition(pline, static_cast<int>(s->expandpvt), s->name) == CondState::IF_TRUE;
    return make_tok_num(nullptr, retval);
}

// The parameters joined by commas, as they are (%eval, %str, %hex).
Tok* Preproc::stdmac_join(const SMacro*, Tok** params, int nparams) {
    Tok* tline = nullptr;
    for (int i = 0; i < nparams; i++) {
        if (i) tline = make_tok_char(tline, ',');
        Tok* ttmp;
        for (Tok* t = params[i]; t; t = ttmp) {
            ttmp = t->next;
            t->next = tline;
            tline = t;
        }
        params[i] = nullptr;
    }
    return tline;
}

Tok* Preproc::stdmac_strcat(const SMacro*, Tok** params, int nparams) {
    std::string str;
    for (int i = 0; i < nparams; i++) {
        unquote_token(params[i]);
        str += params[i]->text;
    }
    return make_tok_qstr(nullptr, str);
}

Tok* Preproc::stdmac_hs2b(const SMacro*, Tok** params, int nparams) {
    std::string str;
    for (int i = 0; i < nparams; i++) {
        unquote_token(params[i]);
        const std::string& p = params[i]->text;
        int v = -1;
        for (std::size_t j = 0; j < p.size(); j++) {
            const unsigned hv = hexval(p[j]);
            if (hv > 15) {
                // not a hex digit: ends the number
                if (v >= 0) str.push_back(static_cast<char>(v));
                v = -1;
            } else {
                if (v >= 0) {
                    str.push_back(static_cast<char>((v << 4) + static_cast<int>(hv)));
                    v = -1;
                } else {
                    v = static_cast<int>(hv);
                }
            }
        }
        if (v >= 0) str.push_back(static_cast<char>(v));
    }
    return make_tok_qstr(nullptr, str);
}

Tok* Preproc::stdmac_b2hs(const SMacro*, Tok** params, int) {
    const char* const dchars = digit_chars(false);
    unquote_token(params[0]);
    const std::string p = params[0]->text;
    if (p.empty()) return make_tok_qstr(nullptr, "");
    unquote_token(params[1]);
    const std::string sep = params[1]->text;
    std::string str;
    for (std::size_t i = 0; i < p.size(); i++) {
        if (i && !sep.empty()) str += sep;
        const unsigned char b = static_cast<unsigned char>(p[i]);
        str.push_back(dchars[b >> 4]);
        str.push_back(dchars[b & 15]);
    }
    return make_tok_qstr(nullptr, str);
}

Tok* Preproc::stdmac_substr(const SMacro*, Tok** params, int) {
    const std::int64_t start = get_tok_num(params[1], nullptr);
    const std::int64_t count = get_tok_num(params[2], nullptr);
    return pp_substr_common(params[0], start, count);
}

// %ord: the character codes of a part of a string, as numbers.
Tok* Preproc::stdmac_ord(const SMacro*, Tok** params, int) {
    const std::int64_t start = get_tok_num(params[1], nullptr);
    std::int64_t count = get_tok_num(params[2], nullptr);
    const std::string_view txt = pp_get_substr(params[0], start, &count);
    if (!count) return nullptr;
    std::size_t k = 0;
    Tok* t = make_tok_num(nullptr, static_cast<unsigned char>(txt[k++]));
    while (--count) {
        t = make_tok_char(t, ',');
        t = make_tok_num(t, static_cast<unsigned char>(txt[k++]));
    }
    return t;
}

Tok* Preproc::stdmac_chr(const SMacro*, Tok** params, int nparams) {
    std::string buf;
    for (int i = 0; i < nparams; i++)
        if (params[i]) buf.push_back(static_cast<char>(get_tok_num(params[i], nullptr)));
    return make_tok_qstr(nullptr, buf);
}

Tok* Preproc::stdmac_strlen(const SMacro*, Tok** params, int) {
    unquote_token(params[0]);
    return make_tok_num(nullptr, static_cast<std::int64_t>(params[0]->text.size()));
}

Tok* Preproc::stdmac_tok(const SMacro*, Tok** params, int) {
    return reverse_tokens(tokenize(unquote_token_cstr(params[0])));
}

// %sel(n, a, b, ...): the n-th of the following arguments, as a reference to that parameter.
Tok* Preproc::stdmac_sel(const SMacro* s, Tok** params, int nparams) {
    const std::int64_t which = get_tok_num(params[0], nullptr);
    if (which < 1) {
        as_.warn(WARN_PP_SEL_RANGE, "{}({}) is not a valid selector", s->name, which);
        return nullptr;
    } else if (which >= nparams) {
        as_.warn(WARN_PP_SEL_RANGE, "{}({}) exceeds the number of arguments", s->name, which);
        return nullptr;
    }
    return new_token(nullptr, tok_smac_param(static_cast<int>(which)), std::string_view());
}

Tok* Preproc::stdmac_cond(const SMacro*, Tok** params, int nparams) {
    std::int64_t which = get_tok_num(params[0], nullptr);
    which = which ? 1 : 2;
    if (which >= nparams) return nullptr;
    return new_token(nullptr, tok_smac_param(static_cast<int>(which)), std::string_view());
}

// %selbits(a16, a32, a64): the argument for the current BITS.
Tok* Preproc::stdmac_selbits(const SMacro*, Tok**, int nparams) {
    int which = 0;
    for (int b = as_.globl.bits; b > 1; b >>= 1) which++;   // ilog2_32
    which -= 4;
    if (nparams <= which) which = nparams - 1;
    return new_token(nullptr, tok_smac_param(which), std::string_view());
}

Tok* Preproc::stdmac_count(const SMacro*, Tok**, int nparams) { return make_tok_num(nullptr, nparams); }

// %num(value, digits, base): a number as a string in a base.
Tok* Preproc::stdmac_num(const SMacro* s, Tok** params, int) {
    std::int64_t parm[3];
    constexpr int maxlen = 256;
    char decorate = 0;
    for (int i = 0; i < 3; i++) parm[i] = get_tok_num(params[i], nullptr);
    const std::uint64_t n = static_cast<std::uint64_t>(parm[0]);
    std::int64_t dparm = parm[1];
    std::int64_t bparm = parm[2];

    if (bparm < 0) {
        bparm = -bparm;
        switch (bparm) {
            case 2: decorate = 'b'; break;
            case 8: decorate = 'q'; break;
            case 10: decorate = 'd'; break;
            case 16: decorate = 'x'; break;
            default: bparm = -bparm; break;
        }
    }
    if (bparm < 2 || bparm > static_cast<std::int64_t>(NUMSTR_MAXBASE)) {
        as_.nonfatal("invalid base {} in {}()\n", bparm, s->name);
        return nullptr;
    }
    if (dparm < -maxlen || dparm > maxlen) {
        as_.nonfatal("digit count {} specified to {}() too large", dparm, s->name);
        dparm = -1;
    }
    if (!dparm && !n) decorate = 0;
    std::string str = "'";
    if (decorate) {
        str += '0';
        str += decorate;
    }
    str += numstr(n, static_cast<int>(dparm), static_cast<unsigned>(bparm), false);
    str += '\'';
    return new_token(nullptr, STR, str);
}

Tok* Preproc::stdmac_abs(const SMacro*, Tok** params, int) {
    const std::int64_t v = get_tok_num(params[0], nullptr);
    const std::uint64_t u = v < 0 ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
    return new_token(nullptr, NUM, std::to_string(u));
}

// %map(macro [: fixed args [: count]], args...): call a single-line macro on each argument (or group of them).
Tok* Preproc::stdmac_map(const SMacro* s, Tok** params, int nparam) {
    std::string_view mname, ctxname;
    SMacro* smac = nullptr;
    Context* ctx;
    Tok *t, *tline, *mstart;
    int i;
    Tok* fixargs;
    int fixparams;
    int mparams;
    int tparams;
    int greedify;
    std::vector<Tok*> fparam;
    std::vector<Tok*> cparam;

    t = params[0];
    bool idok = false;
    mname = get_id_noskip(&t, "%map", idok);
    if (!idok) return nullptr;
    mstart = t;

    fixargs = nullptr;
    fixparams = 0;
    mparams = 1;
    t = skip_white(t->next);
    if (tok_is(t, ':')) {
        fixargs = t->next;
        fixparams = count_smacro_args(fixargs, &t);
        t = skip_white(t->next);
        if (tok_is(t, ':')) {
            PpScan pps;
            TokenVal tokval;
            Tok* ep;
            pps.tptr = ep = zap_white(expand_smacro_noreset(t->next));
            t->next = nullptr;
            pps.ntokens = -1;
            std::optional<Expr> evalresult = evaluate_tokens(pps, tokval);
            delete_tlist(ep);
            if (!evalresult || tokval.type != TokenType::EOS) {
                as_.nonfatal("invalid expression in parameter count for `{}' in function {}", mname, s->name);
                return nullptr;
            } else if (!pp_is_simple(*evalresult)) {
                as_.nonfatal("non-constant expression in parameter count for `{}' in function {}", mname, s->name);
                return nullptr;
            }
            mparams = static_cast<int>(pp_reloc_value(*evalresult));
            if (mparams < 1) {
                as_.nonfatal("invalid parameter count for `{}' in function {}", mname, s->name);
                return nullptr;
            }
        }
    }

    nparam--;
    params++;   // the rest of the parameters are the expansion arguments
    if (nparam % mparams)
        as_.nonfatal("{} expected a multiple of {} expansion parameters, got {}\n", s->name, mparams, nparam);

    tparams = fixparams + mparams;
    ctx = get_ctx(mname, &ctxname);
    if (!smacro_defined(ctx, ctxname, tparams, &smac, true, false) || smac->nparam == 0 ||
        (smac->in_progress && !smac->recursive)) {
        as_.nonfatal("macro `{}' taking {} parameter{} not found in function {}", mname, tparams, tparams == 1 ? "" : "s", s->name);
        return nullptr;
    }

    if (nparam < mparams) return nullptr;   // nothing to map

    if (fixparams) {
        int nfp = fixparams;
        fparam = parse_smacro_args(&fixargs, &nfp, smac);
        if (nfp < fixparams) {
            fixparams = nfp;
            tparams = fixparams + mparams;
        }
    }

    greedify = 0;
    if (tparams > smac->nparam) {
        if (smac->params[static_cast<std::size_t>(smac->nparam - 1)].flags & SPARM_GREEDY) greedify = smac->nparam;
    }

    cparam.assign(static_cast<std::size_t>(tparams), nullptr);
    tline = nullptr;
    while (1) {
        int xparams;
        for (i = 0; i < fixparams; i++) cparam[static_cast<std::size_t>(i)] = dup_tlist(fparam[static_cast<std::size_t>(i)], nullptr);
        for (i = fixparams; i < tparams; i++) {
            cparam[static_cast<std::size_t>(i)] = *params;
            *params = nullptr;
            params++;
        }

        if (greedify) {
            // The macro's last parameter is greedy: join the extra ones with commas
            Tok** tp = &cparam[static_cast<std::size_t>(greedify - 1)];
            while (*tp) tp = &(*tp)->next;
            for (int k = greedify; k < tparams; k++) {
                *tp = make_tok_char(nullptr, ',');
                tp = steal_tlist(cparam[static_cast<std::size_t>(k)], &(*tp)->next);
                cparam[static_cast<std::size_t>(k)] = nullptr;
            }
            xparams = greedify;
        } else {
            xparams = tparams;
        }

        t = expand_smacro_with_params(smac, mstart, cparam.data(), xparams, nullptr);
        if (t) {
            Tok* rt = reverse_tokens(t);
            t->next = tline;
            tline = rt;
        }
        for (i = 0; i < xparams; i++) delete_tlist(cparam[static_cast<std::size_t>(i)]);

        nparam -= mparams;
        if (nparam < mparams) break;
        tline = make_tok_char(tline, ',');
    }
    for (Tok* fp : fparam) free_tlist(fp);
    return tline;
}

Tok* Preproc::stdmac_pathsearch(const SMacro* s, Tok** params, int) { return pp_do_pathsearch(&params[0], s->name); }

Tok* Preproc::stdmac_depend(const SMacro* s, Tok** params, int) { return pp_do_depend(&params[0], s->name); }

Tok* Preproc::stdmac_realpath(const SMacro* s, Tok** params, int) {
    Tok *t, *ot;
    t = tlist_filename(&params[0], &ot, s->name);
    if (!t) return nullptr;
    FileHashEntry* fhe = nullptr;
    inc_open(t->text, nullptr, &fhe, INC_PROBE | INC_EXACT, nullptr);
    if (fhe) {
        do_delete_token(t);
        return make_tok_qstr(nullptr, fhe->full->path);
    }
    return steal_token(t, ot);
}

Tok* Preproc::stdmac_null(const SMacro*, Tok**, int) { return nullptr; }

// %find / %findi: the position of a string among the following ones, or 0.
Tok* Preproc::stdmac_find(const SMacro* s, Tok** params, int nparam) {
    const bool casesense = s->expandpvt != 0;
    int found = 0;
    unquote_token(params[0]);
    const std::string ref = params[0]->text;
    for (int i = 1; i < nparam; i++) {
        params[i] = expand_sparm_str(params[i], SPARM_STR | SPARM_CONDQUOTE);
        unquote_token(params[i]);
        const std::string& cmp = params[i]->text;
        if (ref.size() == cmp.size() && (casesense ? ref == cmp : ieq(ref, cmp))) {
            found = i;
            break;
        }
    }
    return make_tok_num(nullptr, found);
}

Tok* Preproc::stdmac_env(const SMacro*, Tok** params, int nparam) {
    std::optional<std::string> env = pp_getenv(params[0], false);
    if (!env) {
        if (nparam > 1) return new_token(nullptr, tok_smac_param(1), std::string_view());   // the default
        env = std::string();
    }
    return make_tok_qstr(nullptr, *env);
}

Tok* Preproc::stdmac_limit(const SMacro* s, Tok** params, int) {
    GetLimitWhich which;
    std::int64_t val = 0;
    unquote_token(params[1]);
    const std::string which_str = params[1]->text;
    if (which_str.empty() || ieq(which_str, "current")) {
        which = GET_LIMIT_CURRENT;
    } else if (which_str == "*" || ieq(which_str, "reset") || ieq(which_str, "init")) {
        which = GET_LIMIT_INIT;
    } else if (ieq(which_str, "default")) {
        which = GET_LIMIT_DEFAULT;
    } else if (ieq(which_str, "unlimited") || ieq(which_str, "maximum") || ieq(which_str, "max")) {
        which = GET_LIMIT_MAX;
    } else {
        as_.nonfatal("invalid second argument `{}' to {}()", which_str, s->name);
        return make_tok_num(nullptr, val);
    }
    unquote_token(params[0]);
    const std::string limit = params[0]->text;
    if (limit.empty() || ieq(limit, "unlimited")) val = LIMIT_MAX_VAL;
    else val = as_.get_limit(limit, which);
    return make_tok_num(nullptr, val);
}

// The listing options as a string of their letters.
Tok* Preproc::get_list_options(std::uint64_t optmask) {
    static const char optchars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string s;
    for (unsigned i = 0; i < 62; i++) {
        if (optmask & 4) s.push_back(optchars[i]);
        optmask >>= 1;
    }
    return make_tok_qstr(nullptr, s);
}

Tok* Preproc::stdmac_list_options(const SMacro*, Tok**, int) { return get_list_options(active_list_options_); }

}  // namespace cb::build::x86asm
