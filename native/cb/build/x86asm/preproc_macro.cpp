// cb/build/x86asm/preproc_macro.cpp -- the preprocessor's macro tables: single-line macros (definition, template parsing,
// removal), multi-line macro objects and their reference counts, the %push context stack. (preproc.c: SMacro, MMacro,
// Context, define_smacro, undef_smacro, parse_mmacro_spec ...)
#include <climits>

#include "cb/build/x86asm/preproc.hpp"

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

// ---- multi-line macro objects, counted by reference ----

MMacro* Preproc::new_mmacro() { return new MMacro(); }

MMacro* Preproc::get_mmacro(MMacro* m) {
    if (m) m->refcnt++;
    return m;
}

// Replace *mp by `next`, dropping the reference *mp held; the macro goes when the last reference does.
MMacro* Preproc::pop_mmacro(MMacro** mp, MMacro* next) {
    MMacro* m = *mp;
    *mp = next;
    if (m) {
        if (m->refcnt == 0) as_.assert_failed("m->refcnt > 0");
        if (!--m->refcnt) {
            if (m->has_name) as_.debug(2, "freeing macro `{}'", m->name);
            if (m->next) as_.assert_failed("!m->next");
            free_mmacro(m);
        }
    }
    return next;
}

void Preproc::put_mmacro(MMacro** mp) { pop_mmacro(mp, nullptr); }

// Pop one level of a macro stack: go on to what `nextp` (a macro that was nested in it) had saved.
void Preproc::pop_mstk(MStk* msp, MMacro* nextp) {
    MStk next;
    if (nextp) next = nextp->mstk;
    pop_mmacro(&msp->mmac, next.mmac);
    pop_mmacro(&msp->mstk, next.mstk);
}

// The state of one invocation: arguments, the call line, its name (what a new invocation starts from).
void Preproc::clear_mmacro(MMacro* m) {
    m->params.clear();
    m->iname.clear();
    m->paramlen.clear();
    delete_tlist(m->iline);
}

void Preproc::free_mmacro(MMacro* m) {
    if (m->refcnt != 0) as_.assert_failed("m->refcnt == 0");
    clear_mmacro(m);
    free_tlist(m->dlist);
    m->defaults.clear();
    free_llist(m->expansion);
    m->next = nullptr;
    delete m;
}

void Preproc::free_mmacro_list(MMacro* list) {
    if (list) list->refcnt--;
    MMacro* tmp;
    for (MMacro* m = list; m; m = tmp) {
        tmp = m->next;
        if (m->next) m->next->refcnt--;
        if (m->refcnt)
            as_.nonfatal("macro {}: refcnt {} on free, should be 0\n", m->name, static_cast<long long>(m->refcnt));
        free_mmacro(m);
    }
}

Line* Preproc::new_line() {
    Line* l = free_lines_;
    if (l) {
        free_lines_ = l->next;
        *l = Line();
        return l;
    }
    line_pool_.emplace_back();
    return &line_pool_.back();
}

void Preproc::free_line(Line* l) {
    put_mmacro(&l->finishes);
    free_tlist(l->first);
    *l = Line();
    l->next = free_lines_;
    free_lines_ = l;
}

void Preproc::free_llist(Line* list) {
    Line* tmp;
    for (Line* l = list; l; l = tmp) {
        tmp = l->next;
        free_line(l);
    }
}

// ---- single-line macros ----

void Preproc::free_smacro_members(SMacro* s) {
    for (SmacParam& p : s->params)
        if (p.def) free_tlist(p.def);
    s->params.clear();
    s->name.clear();
    delete_tlist(s->expansion);
}

void Preproc::clear_smacro(SMacro* s) {
    free_smacro_members(s);
    SMacro* next = s->next;
    *s = SMacro();
    s->next = next;
}

void Preproc::free_smacro(SMacro* s) {
    free_smacro_members(s);
    delete s;
}

SMacro* Preproc::find_smacro_head(SMacroTable& tbl, std::string_view name) {
    auto it = tbl.find(lcase(name));
    return it == tbl.end() ? nullptr : it->second;
}

// Remove the macros of the kinds in `what` from a table (a macro that stays when others are removed after it in its chain
// drops out of the chain, as in NASM).
void Preproc::clear_smacro_table(SMacroTable& smt, ClearWhat what) {
    bool empty = true;
    for (auto& entry : smt) {
        SMacro** head = &entry.second;
        SMacro* tmp;
        for (SMacro* s = *head; s; s = tmp) {
            tmp = s->next;
            if (what & (static_cast<int>(s->alias) + 1)) {
                *head = s->next;
                free_smacro(s);
            } else {
                empty = false;
            }
        }
    }
    if (empty) smt.clear();
}

void Preproc::free_mmacro_table(std::unordered_map<std::string, MMacro*>& mmt) {
    for (auto& entry : mmt) free_mmacro_list(entry.second);
    mmt.clear();
}

void Preproc::free_macros() {
    clear_smacro_table(smacros_, CLEAR_ALLDEFINE);
    free_mmacro_table(mmacros_);
}

void Preproc::ctx_pop() {
    Context* c = cstk_;
    cstk_ = cstk_->next;
    clear_smacro_table(c->localmac, CLEAR_ALLDEFINE);
    delete c;
}

// The context a "%$name" (one $ per level up the stack) refers to, and the name without the %$ prefix. A name that is not
// context-local gives no context and itself.
Context* Preproc::get_ctx(std::string_view name, std::string_view* namep) {
    if (namep) *namep = name;
    auto at = [&](std::size_t i) -> char { return i < name.size() ? name[i] : '\0'; };
    if (name.empty() || at(0) != '%' || at(1) != '$') return nullptr;
    if (!cstk_) {
        as_.nonfatal("`{}': context stack is empty", name);
        return nullptr;
    }
    std::size_t pos = 2;
    Context* ctx = cstk_;
    int i = 0;
    while (ctx && at(pos) == '$') {
        pos++;
        i++;
        ctx = ctx->next;
    }
    if (!ctx) {
        as_.nonfatal("`{}': context stack is only {} level{} deep", name.substr(pos), i, (i == 1 ? "" : "s"));
        return nullptr;
    }
    if (namep) *namep = name.substr(pos);
    return ctx;
}

bool Preproc::smacro_defined(Context* ctx, std::string_view name, int nparam, SMacro** defn, bool nocase, bool find_alias) {
    SMacroTable& smtbl = ctx ? ctx->localmac : smacros_;
restart:
    SMacro* m = find_smacro_head(smtbl, name);
    while (m) {
        if (streq(m->name, name, m->casesense && nocase) &&
            (nparam <= 0 || m->nparam == 0 || (nparam >= m->nparam_min && (m->varadic || nparam <= m->nparam)))) {
            if (m->alias && !find_alias) {
                if (!ppconf_.noaliases) {
                    // follow the alias to its target
                    name = m->expansion->text;
                    goto restart;
                }
                m = m->next;   // aliases are off: look past it
                continue;
            }
            if (defn) *defn = m;
            return true;
        }
        m = m->next;
    }
    return false;
}

Tok* Preproc::smacro_expand_default(const SMacro* s, Tok**, int) { return dup_tlist(s->expansion, nullptr); }

// Parse a macro's parameter list "(name, =name, &name, name+ ...)" after its name. On return *tpp points at the link
// after the template (white space skipped). With a `tmpl` the parameters are recorded in it; without one the
// parameters are only counted. Returns the parameter count (0 when there is no template).
int Preproc::parse_smacro_template(Tok*** tpp, SMacro* tmpl) {
    int nparam = 0;
    int flags;
    std::vector<SmacParam> params;
    bool err, done;
    bool greedy = false;
    bool parsing_radix;
    char radix;
    Tok** tn = *tpp;
    Tok* t = *tn;
    Tok* name;

    if (!tok_is(t, '(')) goto finish;

    if (tmpl) {
        Tok* tx = t;
        Tok** txpp = &tx;
        const int sparam = parse_smacro_template(&txpp, nullptr);
        if (!sparam) goto finish;
        params.resize(static_cast<std::size_t>(sparam));
    }

    tn = &t->next;
    t = *tn;
    name = nullptr;
    flags = 0;
    radix = 0;
    parsing_radix = false;
    err = done = false;
    while (!done) {
        if (!t) {
            if (name || flags) as_.nonfatal("`)' expected to terminate macro template");
            else as_.nonfatal("parameter identifier expected");
            break;
        }
        bool bad = false;
        bool end_param = false;
        switch (t->type) {
            case ID:
                if (parsing_radix) {
                    for (const char* cp = t->text.c_str(); cp && *cp; cp++) {
                        switch (*cp | 0x20) {
                            case 'b': case 'y': case 'd': case 't': case 'o': case 'q': case 'h': case 'x':
                                radix = *cp;
                                break;
                            case 's': flags &= ~SPARM_UNSIGNED; break;
                            case 'u': flags |= SPARM_UNSIGNED; break;
                            default:
                                as_.nonfatal("invalid radix specifier `/{}'", t->text);
                                cp = nullptr;
                                break;
                        }
                        if (!cp) break;
                    }
                } else {
                    if (name) bad = true;
                    else name = t;
                }
                break;
            case '=': flags |= SPARM_EVAL; break;
            case '&': flags |= SPARM_STR; break;
            case DBL_AND: flags |= SPARM_STR | SPARM_CONDQUOTE; break;
            case '!': flags |= SPARM_NOSTRIP; break;
            case '+':
                flags |= SPARM_GREEDY | SPARM_OPTIONAL;
                greedy = true;
                break;
            case '/':
                if (!(flags & SPARM_EVAL)) as_.nonfatal("radix specifier for parameter without `='");
                parsing_radix = true;
                break;
            case ',':
                if (greedy) as_.nonfatal("greedy parameter must be last");
                end_param = true;
                break;
            case ')':
                done = true;
                end_param = true;
                break;
            case WHITESPACE: break;
            default: bad = true; break;
        }
        if (bad && !err) {
            as_.nonfatal("garbage `{}' in macro parameter list", t->text);
            err = true;
        }
        if (end_param) {
            if (tmpl) {
                if (name) params[static_cast<std::size_t>(nparam)].name = name->text;
                params[static_cast<std::size_t>(nparam)].flags = flags;
                params[static_cast<std::size_t>(nparam)].radix = radix;
            }
            nparam++;
            name = nullptr;
            flags = 0;
            parsing_radix = false;
            radix = 0;
        }
        tn = &t->next;
        t = *tn;
    }

finish:
    while (t && t->type == WHITESPACE) {
        tn = &t->next;
        t = t->next;
    }
    *tpp = tn;
    if (tmpl) {
        tmpl->nparam = nparam;
        tmpl->varadic = greedy;
        tmpl->params = std::move(params);
    }
    return nparam;
}

// Parameter tokens of an expansion: names in `tmpl` become parameter-number tokens (or `type`).
void Preproc::mark_smac_params(Tok* tline, const SMacro* tmpl, int type) {
    for (Tok* t = tline; t; t = t->next) {
        if (t->type != ID && t->type != XDEF_PARAM) continue;
        for (int i = 0; i < tmpl->nparam; i++)
            if (t->text == tmpl->params[static_cast<std::size_t>(i)].name) t->type = type ? type : tok_smac_param(i);
    }
}

void Preproc::list_smacro_def(int op, const Context* ctx, const SMacro* m) {
    std::string def;
    if (ctx) {
        const int context_depth = static_cast<int>(cstk_->depth - ctx->depth + 1);
        def = "[" + (ctx->name ? *ctx->name : std::string()) + "::" + std::to_string(ctx->number) + "] %" +
              std::string(static_cast<std::size_t>(context_depth), '$');
    }
    def += m->name;
    if (m->nparam) {
        def += '(';
        for (int i = 0; i < m->nparam; i++) {
            const SmacParam& pr = m->params[static_cast<std::size_t>(i)];
            if (i) def += ',';
            if (pr.flags & SPARM_EVAL) def += '=';
            if (pr.flags & SPARM_STR) {
                def += '&';
                if (pr.flags & SPARM_CONDQUOTE) def += '&';
            }
            if (pr.flags & SPARM_NOSTRIP) def += '!';
            def += pr.name;
            if (pr.flags & (SPARM_GREEDY | SPARM_VARADIC)) def += '+';
            const bool slash = pr.radix || (pr.flags & SPARM_UNSIGNED);
            if (slash) def += '/';
            if (pr.flags & SPARM_UNSIGNED) def += 'u';
            if (pr.radix) def += pr.radix;
        }
        def += ')';
    }
    def += ' ';
    for (const Tok* t = m->expansion; t; t = t->next) def += t->text;
    if (ppdbg_ & PDBG_LIST_SMACROS) as_.listmsg("{} {}", pp_directive_name(op), def);
}

// Define a single-line macro (or redefine it) with `expansion` (in reverse order) and the parameters of `tmpl`.
// Returns the macro, or null after an error.
SMacro* Preproc::define_smacro(std::string_view mname, bool casesense, Tok* expansion, SMacro* tmpl) {
    SMacro* smac;
    bool defining_alias = false;
    int nparam = 0;
    bool defined;

    if (tmpl) {
        defining_alias = tmpl->alias;
        nparam = tmpl->nparam;
        if (nparam && !defining_alias) mark_smac_params(expansion, tmpl, 0);
    }

    Context* ctx = get_ctx(mname, &mname);
    defined = smacro_defined(ctx, mname, nparam, &smac, casesense, true);
    if (defined) {
        if (smac->alias) {
            if (smac->in_progress) {
                as_.nonfatal("macro alias loop");
                goto fail;
            }
            if (!defining_alias && !ppconf_.noaliases) {
                // redefining an alias redefines what it points to
                smac->in_progress++;
                SMacro* s = define_smacro(smac->expansion->text, casesense, expansion, tmpl);
                smac->in_progress--;
                return s;
            }
        }
        if (casesense ^ smac->casesense) {
            as_.warn(WARN_PP_MACRO_DEF_CASE_SINGLE,
                     "case {}sensitive definition of macro `{}' will shadow {}sensitive macro `{}'",
                     casesense ? "" : "in", mname, smac->casesense ? "" : "in", smac->name);
            defined = false;
        } else if ((!!nparam) ^ (!!smac->nparam)) {
            as_.warn(WARN_PP_MACRO_DEF_PARAM_SINGLE, "macro `{}' defined both with and without parameters", mname);
            defined = false;
        } else if (smac->nparam < nparam) {
            as_.warn(WARN_PP_MACRO_DEF_GREEDY_SINGLE, "defining macro `{}' shadows previous greedy definition", mname);
            defined = false;
        }
    }

    if (defined) {
        clear_smacro(smac);
    } else {
        SMacroTable& smtbl = ctx ? ctx->localmac : smacros_;
        SMacro*& smhead = smtbl[lcase(mname)];
        smac = new SMacro();
        smac->next = smhead;
        smhead = smac;
    }

    smac->name = std::string(mname);
    smac->casesense = casesense;
    smac->expansion = reverse_tokens(expansion);
    smac->expand = nullptr;
    smac->nparam = nparam;
    smac->nparam_min = nparam;

    if (tmpl) {
        smac->params = std::move(tmpl->params);
        smac->alias = tmpl->alias;
        smac->recursive = tmpl->recursive;
        if (tmpl->expand) {
            smac->expand = tmpl->expand;
            smac->expandpvt = tmpl->expandpvt;
        }
        if (nparam) {
            int nparam_min = nparam;
            smac->varadic = !!(smac->params[static_cast<std::size_t>(nparam - 1)].flags & (SPARM_GREEDY | SPARM_VARADIC));
            while (nparam_min > 1) {
                if (!(smac->params[static_cast<std::size_t>(nparam_min - 1)].flags & SPARM_OPTIONAL)) break;
                nparam_min--;
            }
            smac->nparam_min = nparam_min;
        }
    }

    if (ppdbg_ & (PDBG_SMACROS | PDBG_LIST_SMACROS))
        list_smacro_def((smac->alias ? PP_DEFALIAS : PP_DEFINE) + !casesense, ctx, smac);
    return smac;

fail:
    delete_tlist(expansion);
    if (tmpl) free_smacro_members(tmpl);
    return nullptr;
}

void Preproc::undef_smacro(std::string_view mname, bool undefalias) {
    Context* ctx = get_ctx(mname, &mname);
    SMacroTable& smtbl = ctx ? ctx->localmac : smacros_;
    auto it = smtbl.find(lcase(mname));
    if (it == smtbl.end()) return;
    SMacro** sp = &it->second;
    SMacro* s;
    while ((s = *sp) != nullptr) {
        if (streq(s->name, mname, s->casesense)) {
            if (s->alias && !undefalias) {
                if (!ppconf_.noaliases) {
                    if (s->in_progress) {
                        as_.nonfatal("macro alias loop");
                    } else {
                        s->in_progress = true;
                        undef_smacro(s->expansion->text, false);
                        s->in_progress = false;
                    }
                }
            } else {
                if (host_.list_option && host_.list_option('d')) list_smacro_def(s->alias ? PP_UNDEFALIAS : PP_UNDEF, ctx, s);
                *sp = s->next;
                free_smacro(s);
                continue;
            }
        }
        sp = &s->next;
    }
}

int Preproc::read_param_count(std::string_view str) {
    bool err = false;
    const std::int64_t result = as_.hooks.readnum ? as_.hooks.readnum(str, err) : 0;
    if (err || result < 0) {
        as_.nonfatal("unable to parse parameter count `{}'", str);
        return 0;
    } else if (result > as_.nasm_limit[LIMIT_PARAMS]) {
        as_.nonfatal("parameter count `{}' is too large (max {})", str, as_.nasm_limit[LIMIT_PARAMS]);
        return 0;
    }
    return static_cast<int>(result);
}

// Count and split the arguments of a macro call: params[1..n] point at each argument's first token. The return is the
// pointer to the link of a trailing comma (a trailing empty argument), if there is one.
Tok** Preproc::count_mmac_params(Tok* tline, int* nparamp, std::vector<Tok*>* paramsp) {
    int nparam = 0;
    Tok* t;
    Tok **comma = nullptr, **maybe_comma = nullptr;
    std::vector<Tok*> params(static_cast<std::size_t>(PARAM_DELTA), nullptr);

    t = skip_white(tline);
    if (t) {
        while (true) {
            if (static_cast<std::size_t>(nparam) + 2 >= params.size()) params.resize(params.size() + PARAM_DELTA, nullptr);
            params[static_cast<std::size_t>(++nparam)] = t;
            if (tok_is(t, '{')) {
                int brace = 1;
                comma = nullptr;
                while (brace && (t = t->next)) {
                    brace += tok_is(t, '{');
                    brace -= tok_is(t, '}');
                }
                if (t) {
                    t = skip_white(t->next);
                    if (tok_isnt(t, ',')) as_.nonfatal("braces do not enclose all of macro parameter");
                } else {
                    as_.nonfatal("expecting closing brace in macro parameter");
                }
            }
            if (!t) break;
            maybe_comma = &t->next;
            while (tok_isnt(t, ',')) {
                if (!tok_white(t)) comma = nullptr;
                maybe_comma = &t->next;
                t = t->next;
            }
            if (!t) break;
            comma = maybe_comma;
            t = skip_white(t->next);
        }
    }
    params[static_cast<std::size_t>(nparam) + 1] = nullptr;
    *paramsp = std::move(params);
    *nparamp = nparam;
    return comma;
}

bool Preproc::parse_mmacro_spec(Tok* tline, MMacro* def, std::string_view directive) {
    tline = tline->next;
    tline = skip_white(tline);
    tline = expand_id(tline);
    if (!tok_is(tline, ID)) {
        as_.nonfatal("`{}' expects a macro name", directive);
        return false;
    }

    def->name = tline->text;
    def->has_name = true;
    def->plus = false;
    def->nolist = 0;
    def->nparam_min = 0;
    def->nparam_max = 0;

    tline = expand_smacro(tline->next);
    tline = skip_white(tline);
    if (!tok_is(tline, NUM)) as_.nonfatal("`{}' expects a parameter count", directive);
    else def->nparam_min = def->nparam_max = read_param_count(tline->text);
    if (tline && tok_is(tline->next, '-')) {
        tline = tline->next->next;
        if (tok_is(tline, '*')) {
            def->nparam_max = INT_MAX;
        } else if (!tok_is(tline, NUM)) {
            as_.nonfatal("`{}' expects a parameter count after `-'", directive);
        } else {
            def->nparam_max = read_param_count(tline->text);
            if (def->nparam_min > def->nparam_max) {
                as_.nonfatal("minimum parameter count exceeds maximum");
                def->nparam_max = def->nparam_min;
            }
        }
    }
    if (tline && tok_is(tline->next, '+')) {
        tline = tline->next;
        def->plus = true;
    }
    if (tline && tok_is(tline->next, ID) && tline->next->text.size() == 7 && ieq(tline->next->text, ".nolist")) {
        tline = tline->next;
        if (!(host_.list_option && host_.list_option('f'))) def->nolist |= NL_LIST | NL_LINE;
    }

    // Any remaining tokens are the default parameters
    def->ndefs = 0;
    if (tline && tline->next) {
        def->dlist = tline->next;
        tline->next = nullptr;
        Tok** comma = count_mmac_params(def->dlist, &def->ndefs, &def->defaults);
        if (!ppconf_.sane_empty_expansion && comma) {
            free_tlist(*comma);
            *comma = nullptr;
            def->ndefs--;
            as_.warn(WARN_PP_MACRO_PARAMS_LEGACY,
                     "dropping trailing empty default parameter in definition of multi-line macro `{}'", def->name);
        }
    } else {
        def->dlist = nullptr;
        def->defaults.clear();
    }

    def->expansion = nullptr;
    if (!def->defaults.empty() && def->ndefs > def->nparam_max - def->nparam_min && !def->plus)
        as_.warn(WARN_PP_MACRO_DEFAULTS, "too many default macro parameters in macro `{}'", def->name);
    return true;
}

void Preproc::do_clear(ClearWhat what, bool context) {
    if (context) {
        if (what & CLEAR_ALLDEFINE)
            for (Context* ctx = cstk_; ctx; ctx = ctx->next) clear_smacro_table(ctx->localmac, what);
    } else {
        if (what & CLEAR_ALLDEFINE) clear_smacro_table(smacros_, what);
        if (what & CLEAR_MMACRO) free_mmacro_table(mmacros_);
    }
}

// %arg and %local: a macro named `name` that is [stack pointer + offset].
void Preproc::define_stack_smacro(std::string_view name, int offset) {
    Tok* tt = make_tok_char(nullptr, ')');
    tt = make_tok_num(tt, offset);
    if (!tok_is(tt, '-')) tt = make_tok_char(tt, '+');
    tt = new_token(tt, ID, stack_pointer_);
    tt = make_tok_char(tt, '(');
    define_smacro(name, true, tt, nullptr);
}

void Preproc::assign_smacro(std::string_view mname, bool casesense, Tok* tline, std::string_view dname) {
    tline = expand_smacro(tline);
    PpScan pps;
    pps.tptr = tline;
    pps.ntokens = -1;
    TokenVal tokval;
    std::optional<Expr> evalresult = evaluate_tokens(pps, tokval);
    delete_tlist(tline);
    if (!evalresult) return;
    if (tokval.type != TokenType::EOS)
        as_.warn(WARN_PP_TRAILING, "trailing garbage after expression ignored");
    if (!pp_is_simple(*evalresult)) {
        as_.nonfatal("non-constant value given to `{}'", dname);
    } else {
        tline = make_tok_num(nullptr, pp_reloc_value(*evalresult));
        define_smacro(mname, casesense, tline, nullptr);
    }
}

}  // namespace cb::build::x86asm
