// cb/build/x86asm/preproc_masm_xform.inc -- masm_pp_xform: the line-by-line MASM to NASM rewriter. Included by
// preproc_masm.cpp at namespace level (it is the one big member of MasmFront::Impl).

namespace cb::build::x86asm {

using namespace pp;

namespace {
inline void mx_skip_ws(const char*& s) {
    while (*s == ' ' || *s == '\t') s++;
}
inline bool mx_word_end(char c) { return c == '\0' || c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
}  // namespace

std::string MasmFront::Impl::xform(std::string line) {
    const char* p;
    std::string tmp;
    std::string w1, w2;
    std::size_t l1, l2;

    // "#if" style conditionals: treat as the MASM ones
    {
        const char* h = line.c_str();
        mx_skip_ws(h);
        if (*h == '#' && isidstart(h[1])) {
            std::string cw;
            const char* c = h + 1;
            while (isidchar(*c) && cw.size() + 1 < 32) cw += *c++;
            if (mx_is_cond(cw)) return xform(std::string(h + 1));
        }
    }

    // inside a macro body: the end of the macro, a nested macro definition
    if (!ppstk.empty() && ppstk.back().is_macro) {
        const char* t = line.c_str();
        mx_skip_ws(t);
        if (!strnicmp_(t, "&endm", 5) && mx_word_end(t[5])) return "%endmacro";
        const char* am = std::strstr(t, "&macro");
        if (am) {
            int nl = static_cast<int>(am - t), arity = 0;
            const char* pp = am + 6;
            while (nl > 0 && (t[nl - 1] == ' ' || t[nl - 1] == '\t')) nl--;
            const std::string meat(t, static_cast<std::size_t>(std::min(nl, 159)));
            const std::string nsub = subst_params(meat, ppstk.back().param);
            mx_skip_ws(pp);
            if (*pp) {
                arity = 1;
                for (; *pp; pp++)
                    if (*pp == ',') arity++;
            }
            if (arity) return std::format("%imacro {} 0-{}", nsub, arity);
            return std::format("%imacro {} 0", nsub);
        }
        {
            const char* nsp = t;
            std::string nmbuf;
            while (*nsp && *nsp != ' ' && *nsp != '\t') {
                if (nmbuf.size() + 1 < 256) nmbuf += *nsp;
                nsp++;
            }
            const char* kp = nsp;
            mx_skip_ws(kp);
            if (!nmbuf.empty() && !strnicmp_(kp, "macro", 5) && mx_word_end(kp[5])) {
                std::vector<std::string> param;
                const char* q = kp + 5;
                const std::string sub = subst_params(nmbuf, ppstk.back().param);
                while (*q) {
                    std::string pn;
                    mx_skip_ws(q);
                    while (isidchar(*q)) {
                        if (pn.size() + 1 < 128) pn += *q;
                        q++;
                    }
                    if (!pn.empty()) param.push_back(pn);
                    while (*q && *q != ',') q++;
                    if (*q == ',') q++;
                    else break;
                }
                const int oseq = ++for_seq;
                std::vector<std::string> oparam;
                const std::vector<std::string>& outer = ppstk.back().param;
                for (std::size_t k = 0; k < outer.size(); k++) {
                    ppq_add(std::format("%xdefine _mo{}_{} %{{{}}}", oseq, k + 1, k + 1));
                    oparam.push_back(outer[k]);
                }
                ppq_add("%imacro " + sub + " 0-*");
                MxBlk b;
                b.is_macro = true;
                b.param = std::move(param);
                b.oparam = std::move(oparam);
                b.oseq = oseq;
                ppstk.push_back(std::move(b));
                return ppq_get();
            }
        }
    }
    if (!ppstk.empty() && ppstk.back().is_macro) {
        if (!ppstk.back().oparam.empty()) line = subst_outer(line, ppstk.back().oparam, ppstk.back().oseq);
        line = subst_params(line, ppstk.back().param);
    }
    if (!ppstk.empty() && ppstk.back().is_for && !ppstk.back().oparam.empty()) {
        line = subst_outer(line, ppstk.back().oparam, ppstk.back().oseq);
        line = subst_params(line, {});
    }
    if (!ppstk.empty() && ppstk.back().is_for && !ppstk.back().forvar.empty()) line = for_paste(line, ppstk.back().forvar);
    p = line.c_str();

    // a nested macro definition inside a FOR/skip block is dropped up to its ENDM
    if (!ppstk.empty() && ppstk.back().is_skip) {
        const char* t = line.c_str();
        mx_skip_ws(t);
        if (!strnicmp_(t, "endm", 4) && mx_word_end(t[4])) {
            ppstk.pop_back();
        } else if (mx_line_is_macro_def(t)) {
            MxBlk b;
            b.is_skip = true;
            ppstk.push_back(std::move(b));
        }
        return "";
    }
    if (!ppstk.empty() && ppstk.back().is_for && mx_line_is_macro_def(line.c_str())) {
        MxBlk b;
        b.is_skip = true;
        ppstk.push_back(std::move(b));
        return "";
    }
    if (comment_delim) {
        if (line.find(comment_delim) != std::string::npos) comment_delim = 0;
        return "";
    }
    if (!(!ppstk.empty() && ppstk.back().is_macro)) {
        std::string ar;
        if (rewrite_anon(line, ar)) {
            line = ar;
            p = line.c_str();
        }
    }
    {
        const char* q = line.c_str();
        mx_skip_ws(q);
        if (q[0] == '%' && !strnicmp_(q + 1, "out", 3)) return "";
        if (q[0] == '%' && (q[1] == ' ' || q[1] == '\t')) {
            q++;
            mx_skip_ws(q);
            line = q;
            p = line.c_str();
        }
    }
    // inside a macro or FOR: "name%{1} = expr" assignments to a made-up name
    if (!ppstk.empty() && (ppstk.back().is_macro || ppstk.back().is_for) &&
        (line.find("%{") != std::string::npos || line.find("%[") != std::string::npos)) {
        const char* base = line.c_str();
        const char* s = base;
        const char* eqp = nullptr;
        char qc = 0;
        for (; *s; s++) {
            if (qc) {
                if (*s == qc) qc = 0;
                continue;
            }
            if (*s == '\'' || *s == '"') {
                qc = *s;
                continue;
            }
            if (*s == '=' && s[1] != '=' && (s == base || (s[-1] != '=' && s[-1] != '<' && s[-1] != '>' && s[-1] != '!'))) {
                eqp = s;
                break;
            }
        }
        if (eqp) {
            std::string lhs(base, static_cast<std::size_t>(eqp - base));
            std::size_t b = 0;
            while (b < lhs.size() && (lhs[b] == ' ' || lhs[b] == '\t')) b++;
            lhs = mx_trim_right(lhs.substr(b), " \t");
            if (lhs.find("%{") != std::string::npos || lhs.find("%[") != std::string::npos) {
                const std::string ex = mx_xlat_ops(eqp + 1);
                const char* xr = ex.c_str();
                mx_skip_ws(xr);
                if (mx_atom_ref(xr)) tmp = std::format("%idefine {} {}", lhs, xr);
                else tmp = std::format("%iassign {} {}", lhs, ex);
                ppq_add(tmp);
                return ppq_get();
            }
        }
    }
    {
        std::string w = mx_word(p);
        l1 = w.size();
        w1 = w;
    }
    if (!l1) {
        if (!ppstk.empty() && ppstk.back().is_macro) {
            auto r = paste_struct_instance(line);
            if (r) return *r;
        }
        return line;
    }
    if (locif_cond) {
        if (mx_eq(w1, "endif")) {
            locif_cond.reset();
            return "";
        }
        if (mx_is_cond(w1) && !mx_eq(w1, "else") && !mx_eq(w1, "elseif")) {
            locif_cond.reset();
            return line;
        }
        {
            const std::string body = rewrite_line(line);
            const char* b = body.c_str();
            mx_skip_ws(b);
            return std::format("times (({}) {} 0) {}", *locif_cond, locif_ife ? "==" : "!=", b);
        }
    }
    if ((mx_eq(w1, "if") || mx_eq(w1, "ife")) && std::strchr(p, '$')) {
        std::string ex = mx_xlat_ops(p);
        ex = mx_trim_right(ex, " \t\r");
        locif_cond = ex;
        locif_ife = mx_eq(w1, "ife");
        return "";
    }
    if (*p == ':' && p[1] != ':' && w1[0] != '.' && !mx_has_streg_form(w1.c_str())) {
        const char* rest = p + 1;
        mx_skip_ws(rest);
        std::string rw;
        {
            const char* q = rest;
            rw = mx_word(q);
        }
        static const char* const strops[] = {"lods", "stos", "movs", "scas", "cmps", "ins", "outs", "rep", "repe", "repz", "repne", "repnz"};
        bool hit = false;
        for (const char* s : strops)
            if (mx_eq(rw, s)) hit = true;
        if (hit) {
            const std::string lbl = w1 + ":";
            std::vector<std::string> held;
            held.push_back(xform(std::string(rest)));
            while (held.size() < 7 && ppq_has()) held.push_back(ppq_get());
            ppq_add(lbl);
            for (std::string& h : held) ppq_add(std::move(h));
            return ppq_get();
        }
    }
    if (mx_eq(w1, "localv")) {
        const char* q = p;
        const std::string nm = mx_word(q);
        if (!nm.empty()) lbuf_add(nm);
        const char* c = std::strchr(p, ',');
        if (c) {
            const char* s = c + 1;
            mx_skip_ws(s);
            if (*s == '<') {
                const char* e = std::strchr(s, '>');
                if (e && e > s + 1) {
                    tmp = w1 + " " + std::string(p, static_cast<std::size_t>(s - p)) + std::string(s + 1, static_cast<std::size_t>(e - s - 1));
                    return rewrite_line(tmp);
                }
            }
        }
    }
    if (mx_eq(w1, "ccall")) {
        const char* s = p;
        mx_skip_ws(s);
        if (*s == '<') {
            const char* e = std::strchr(s, '>');
            if (e && e > s + 1) {
                std::string tgt;
                const char* t = s + 1;
                while (t < e) {
                    if ((t == s + 1 || !isidchar(t[-1])) && !strnicmp_(t, "ptr", 3) && !isidchar(t[3])) {
                        t += 3;
                        mx_skip_ws(t);
                        continue;
                    }
                    tgt += *t++;
                }
                return rewrite_line("cCall " + tgt + std::string(e + 1));
            }
        } else if (isidstart(*s)) {
            const char* n = s;
            while (isidchar(*n)) n++;
            const char* g = n;
            mx_skip_ws(g);
            if (*g == '<') return rewrite_line("cCall " + std::string(s, static_cast<std::size_t>(n - s)) + "," + std::string(g));
        }
    }
    if (mx_eq(w1, "DeclareNonstandardCcallService")) {
        const char* s = p;
        mx_skip_ws(s);
        if (*s == '<') {
            const char* e = std::strrchr(s, '>');
            if (e && e > s + 1)
                return rewrite_line(w1 + " {" + std::string(s + 1, static_cast<std::size_t>(e - s - 1)) + "}");
        }
    }
    if ((!strnicmp_(w1, "global", 6) || !strnicmp_(w1, "static", 6)) && l1 == 7) {
        const char suf = tolower_(w1[6]);
        if (suf == 'b' || suf == 'w' || suf == 'd' || suf == 'q' || suf == 't') {
            const char* s = p;
            mx_skip_ws(s);
            if (isidstart(*s)) {
                const char* n = s;
                while (isidchar(*n)) n++;
                const char* g = n;
                mx_skip_ws(g);
                if (*g && *g != ',' && *g != ';')
                    return rewrite_line(w1 + " " + std::string(s, static_cast<std::size_t>(n - s)) + ", " + std::string(g));
            }
        }
    }
    if ((!strnicmp_(w1, "parm", 4) || !strnicmp_(w1, "local", 5)) && l1 >= 5) {
        const char suf = tolower_(w1[l1 - 1]);
        const int bytes = suf == 'b' ? 1 : suf == 'w' ? 2 : suf == 'd' ? 4 : 0;
        if (bytes && (l1 == 5 || l1 == 6)) {
            const char* q = p;
            const std::string nm = mx_word(q);
            if (!nm.empty()) lsz_add(nm, bytes);
        }
    }
    if (mx_eq(w1, "even")) {
        const char* e = p;
        mx_skip_ws(e);
        if (!*e || *e == ';') return "align 2";
    }
    if (mx_eq(w1, "org")) {
        const char* v = p;
        mx_skip_ws(v);
        std::string arg;
        while (*v && *v != ';') arg += *v++;
        std::string ex = mx_trim_right(mx_xlat_ops(arg.c_str()), " \t\r");
        if (mx_eq(ex, "0") || ex.empty()) return "";
        return std::format("times (({}) - ($ - $$)) db 0", ex);
    }
    if (mx_has_streg_form(w1.c_str()) && w1.size() == 3 && *p == ':') return rewrite_line(std::format("${}{}", w1, p));
    if (w1[0] != 'f' && w1[0] != 'F' && std::strchr(p, 's') && std::strchr(p, 't')) {
        std::string esc;
        if (mx_escape_streg(esc, p)) return rewrite_line(w1 + " " + esc);
    }
    if (mx_eq(w1, "purge")) return "";
    if (mx_eq(w1, "lar") || mx_eq(w1, "lsl")) {
        const char* c = std::strchr(p, ',');
        if (c) {
            const char* s = c + 1;
            std::string szw;
            mx_skip_ws(s);
            const char* t = s;
            while (isidchar(*t) && szw.size() + 1 < 16) szw += *t++;
            if (!szw.empty() && (mx_eq(szw, "byte") || mx_eq(szw, "word") || mx_eq(szw, "dword") || mx_eq(szw, "fword") || mx_eq(szw, "qword") || mx_eq(szw, "tbyte"))) {
                return rewrite_line(std::format("{} {}word{}", w1, std::string(p, static_cast<std::size_t>(s - p)), t));
            }
        }
    }
    if (mx_eq(w1, "movzx") || mx_eq(w1, "movsx")) {
        const char* c = std::strchr(p, ',');
        if (c) {
            const char* s = c + 1;
            std::string first;
            mx_skip_ws(s);
            const char* u = s;
            while (isidchar(*u) && first.size() + 1 < 16) first += *u++;
            const bool sized = !first.empty() && (mx_eq(first, "byte") || mx_eq(first, "word") || mx_eq(first, "dword") || mx_eq(first, "fword") ||
                                                  mx_eq(first, "qword") || mx_eq(first, "tbyte"));
            if (!sized) {
                const char* dot = nullptr;
                const char* idstart = nullptr;
                const char* idend = nullptr;
                for (const char* q = s; *q && *q != ',' && *q != ';'; q++) {
                    if (*q == '.') dot = q;
                    if (isidchar(*q)) {
                        if (!idstart || (q > s && !isidchar(q[-1]))) idstart = q;
                        idend = q + 1;
                    }
                }
                if (dot) {
                    idstart = dot + 1;
                    idend = idstart;
                    while (isidchar(*idend)) idend++;
                }
                if (idstart && idend > idstart && static_cast<std::size_t>(idend - idstart) < 128) {
                    const std::string id(idstart, static_cast<std::size_t>(idend - idstart));
                    int sz = field_bytes(id);
                    if (!sz) sz = as.masm_type_query(id);
                    if (!sz) sz = lsz_get(id);
                    if (sz == 1 || sz == 2)
                        return rewrite_line(std::format("{} {}{} {}", w1, std::string(p, static_cast<std::size_t>(s - p)),
                                                        sz == 1 ? "byte" : "word", s));
                }
            }
        }
    }
    {
        static const char* const sops[] = {"lods", "stos", "movs", "scas", "cmps", "ins", "outs"};
        std::string opw = w1;
        const char* scan = p;
        std::string rep;
        if (mx_eq(w1, "rep") || mx_eq(w1, "repe") || mx_eq(w1, "repz") || mx_eq(w1, "repne") || mx_eq(w1, "repnz")) {
            const char* q = p;
            const std::string ob = mx_word(q);
            if (!ob.empty()) {
                rep = w1 + " ";
                opw = ob;
                scan = q;
            }
        }
        bool isop = false;
        for (const char* s : sops)
            if (mx_eq(opw, s)) isop = true;
        if (isop) {
            char sfx = 0;
            const char* r = scan;
            while (*r && !sfx) {
                while (*r == ' ' || *r == '\t' || *r == ',') r++;
                if (isidstart(*r)) {
                    std::string sw;
                    const char* s = r;
                    while (isidchar(*s)) {
                        if (sw.size() < 15) sw += *s;
                        s++;
                    }
                    r = s;
                    if (mx_eq(sw, "byte")) sfx = 'b';
                    else if (mx_eq(sw, "word")) sfx = 'w';
                    else if (mx_eq(sw, "dword")) sfx = 'd';
                    else if (mx_eq(sw, "qword")) sfx = 'q';
                } else if (*r) {
                    r++;
                }
            }
            if (sfx) return rep + opw + sfx;
        }
    }
    if (mx_eq(w1, "comment")) {
        const char* d = p;
        mx_skip_ws(d);
        if (*d) {
            const char delim = *d;
            const char* rest = std::strchr(d + 1, delim);
            if (!rest) comment_delim = delim;
            return "";
        }
    }
    if (w1[0] == '.') {
        static const char* const noops[] = {".xcref", ".cref", ".lall", ".sall", ".xall", ".list", ".nolist", ".xlist", ".lfcond",
                                            ".sfcond", ".tfcond", ".seq", ".alpha", ".listall", ".listif", ".listmacro",
                                            ".listmacroall", ".nolistif", ".nolistmacro"};
        for (const char* n : noops)
            if (mx_eq(w1, n)) return "";
    }
    if (in_struct && !mx_is_cond(w1)) {
        const char* mp = p;
        const std::string mtype = mx_word(mp);
        const std::size_t mtl = mtype.size();
        if (mtl && mx_eq(mtype, "ends")) {
            const bool uni = have_cur && cur.is_union;
            if (uni) ppq_add(std::format("%idefine {}_size {}", cur.name, cur.usize));
            in_struct = false;
            if (have_cur) {
                sdefs.push_back(std::move(cur));
                cur = MxSDef();
                have_cur = false;
            }
            if (!uni) ppq_add("endstruc");
            return ppq_get();
        }
        {
            const char* ares = mx_type_to_res(w1);
            if (ares) {
                int acount = 1;
                const int aesz = mx_type_bytes(w1);
                std::optional<std::string> acx;
                const char* ap = p;
                const char* dupp = nullptr;
                mx_skip_ws(ap);
                for (const char* s = ap; *s && *s != ';'; s++) {
                    if ((s == ap || !isidchar(s[-1])) && !strnicmp_(s, "dup", 3) && !isidchar(s[3])) {
                        dupp = s;
                        break;
                    }
                }
                if (dupp && dupp > ap) {
                    std::size_t clen = static_cast<std::size_t>(dupp - ap);
                    while (clen && (ap[clen - 1] == ' ' || ap[clen - 1] == '\t')) clen--;
                    if (isdigit_(*ap)) acount = std::atoi(ap);
                    else if (clen) acx = rewrite_line(std::string(ap, clen));
                }
                if (have_cur && cur.is_union && !acx && aesz * acount > cur.usize) cur.usize = aesz * acount;
                if (have_cur && cur.is_union) tmp = ";";
                else if (acx) tmp = std::format("resb ({} * ({}))", aesz, *acx);
                else tmp = std::format("resb {}", aesz * acount);
                ppq_add(tmp);
                return ppq_get();
            }
        }
        if (mtl) {
            const char* res = mx_type_to_res(mtype);
            int count = 1;
            std::optional<std::string> cx;
            const int esz = mx_type_bytes(mtype);
            const char* rp = mp;
            mx_skip_ws(rp);
            {
                const char* dupp = nullptr;
                for (const char* s = rp; *s && *s != ';'; s++) {
                    if ((s == rp || !isidchar(s[-1])) && !strnicmp_(s, "dup", 3) && !isidchar(s[3])) {
                        dupp = s;
                        break;
                    }
                }
                if (dupp && dupp > rp) {
                    std::size_t clen = static_cast<std::size_t>(dupp - rp);
                    while (clen && (rp[clen - 1] == ' ' || rp[clen - 1] == '\t')) clen--;
                    if (isdigit_(*rp)) count = std::atoi(rp);
                    else if (clen && clen < 256) cx = rewrite_line(std::string(rp, clen));
                }
            }
            if (have_cur) {
                MxSMember mb;
                const char* dv = mp;
                mb.name = w1;
                mb.dir = mx_type_to_dd(mtype);
                mb.count = count;
                mx_skip_ws(dv);
                const char* de = dv;
                while (*de && *de != ';') de++;
                while (de > dv && (de[-1] == ' ' || de[-1] == '\t')) de--;
                mb.def.assign(dv, static_cast<std::size_t>(de - dv));
                cur.members.push_back(std::move(mb));
                if (cur.is_union && esz * count > cur.usize) cur.usize = esz * count;
            }
            if (have_cur && cur.is_union) {
                tmp = std::format("%idefine {}.{} 0", cur.name, w1);
            } else if (res && cx) {
                tmp = std::format(".{}: {} ({})", w1, res, *cx);
            } else if (res) {
                int resn = count;
                if (esz == 1 && count == 1) {
                    const int bn = mx_db_field_bytes(mp);
                    if (bn > 1) resn = bn;
                }
                tmp = std::format(".{}: {} {}", w1, res, resn);
            } else {
                tmp = std::format(".{}: resb {}_size", w1, mtype);
            }
            ppq_add(tmp);
            if (have_cur && !cur.is_union) ppq_add(std::format("%idefine {} {}.{}", w1, cur.name, w1));
            return ppq_get();
        }
        return line;
    }
    if (mx_eq(w1, "local") && !ppstk.empty() && ppstk.back().is_macro) {
        const char* q = p;
        bool any = false;
        while (*q) {
            std::string nm;
            while (*q == ' ' || *q == '\t' || *q == ',') q++;
            if (!isidstart(*q)) break;
            while (isidchar(*q) && nm.size() + 1 < 128) nm += *q++;
            if (*q == ':')
                while (*q && *q != ',') q++;
            ppq_add(std::format("%define {} %%{}", nm, nm));
            any = true;
        }
        if (!any) return "";
        return ppq_get();
    }
    if (mx_eq(w1, "endm")) {
        if (ppstk.empty()) return line;
        MxBlk b = std::move(ppstk.back());
        ppstk.pop_back();
        if (b.is_for) {
            ppq_add("%endif");
            ppq_add("%rotate 1");
            ppq_add("%endrep");
            ppq_add("%endmacro");
            std::string call = b.forname + " ";
            for (std::size_t i = 0; i < b.item.size(); i++) {
                if (i) call += ", ";
                call += b.item[i];
            }
            if (!b.item.empty()) ppq_add(call);
            ppq_add("%unmacro " + b.forname + " 0-*");
        } else if (b.is_macro) {
            ppq_add("%endmacro");
        } else {
            ppq_add("%endrep");
        }
        return ppq_get();
    }
    if (mx_eq(w1, "exitm")) {
        ppq_add("%exitmacro");
        return ppq_get();
    }
    if (mx_eq(w1, "include")) {
        const char* f = p;
        mx_skip_ws(f);
        std::string fn = f;
        const std::size_t sc = fn.find(';');
        if (sc != std::string::npos) fn.resize(sc);
        fn = mx_trim_right(fn, " \t\r\n");
        if (!fn.empty()) {
            std::size_t bs = 0;
            for (std::size_t i = 0; i < fn.size(); i++)
                if (fn[i] == '/' || fn[i] == '\\') bs = i + 1;
            if (mx_eq(std::string_view(fn).substr(bs), "cmacros.inc")) tmp = "%include \"cmacros_shim.inc\"";
            else tmp = "%include \"" + fn + "\"";
            ppq_add(tmp);
            return ppq_get();
        }
    }
    if (mx_eq(w1, "includelib")) return "";
    if (mx_eq(w1, "option")) return "";
    if (mx_eq(w1, "public")) {
        std::string names;
        const char* r = p;
        bool skip = false;
        mx_skip_ws(r);
        for (; *r && *r != ';'; r++) {
            if (*r == ':') {
                skip = true;
                continue;
            }
            if (*r == ',') {
                skip = false;
                names += ',';
                continue;
            }
            if (skip) continue;
            names += *r;
        }
        names = mx_trim_right(names, " \t,");
        ppq_add(names.empty() ? std::string() : "global " + names);
        return ppq_get();
    }
    if (mx_eq(w1, "extrn")) return extern_emit(p, 0);
    {
        int esz = -1;
        if (mx_eq(w1, "externW")) esz = 2;
        else if (mx_eq(w1, "externD")) esz = 4;
        else if (mx_eq(w1, "externB")) esz = 1;
        else if (mx_eq(w1, "externA") || mx_eq(w1, "externFP") || mx_eq(w1, "externNP") || mx_eq(w1, "externP")) esz = 0;
        if (esz >= 0) return extern_emit(p, esz);
    }
    if (mx_eq(w1, "assume") || mx_eq(w1, "assumes")) return "";
    if ((!strnicmp_(w1, "global", 6) || !strnicmp_(w1, "static", 6)) && w1.size() == 7 &&
        (w1[6] == 'W' || w1[6] == 'w' || w1[6] == 'D' || w1[6] == 'd' || w1[6] == 'B' || w1[6] == 'b') && std::strchr(p, '<')) {
        const std::size_t lt = line.find('<');
        const std::size_t gt = line.rfind('>');
        if (lt != std::string::npos && gt != std::string::npos && gt > lt) {
            line.erase(gt, 1);
            line.erase(lt, 1);
        }
        return rewrite_line(line);
    }
    if (mx_eq(w1, "bits")) {
        const char* b = p;
        mx_skip_ws(b);
        if (isdigit_(*b)) {
            const int n = std::atoi(b);
            ppq_add(std::format("%assign __?MASM_BITS?__ {}", n));
            ppq_add(std::format("bits {}", n));
            return ppq_get();
        }
    }
    if (mx_eq(w1, "for") || mx_eq(w1, "irp") || mx_eq(w1, "forc") || mx_eq(w1, "irpc")) {
        const bool bychar = mx_eq(w1, "forc") || mx_eq(w1, "irpc");
        std::string param;
        const char* q = p;
        std::vector<std::string> item;
        mx_skip_ws(q);
        while (isidchar(*q)) {
            if (param.size() + 1 < 128) param += *q;
            q++;
        }
        mx_skip_ws(q);
        if (*q == ',') q++;
        mx_skip_ws(q);
        if (bychar) {
            const char* s = q;
            if (*s == '<') s++;
            for (; *s && *s != '>'; s++) {
                if (*s == ' ' || *s == '\t') continue;
                item.emplace_back(1, *s);
            }
        } else {
            const char* s = q;
            int depth = 0;
            std::string buf;
            if (*s == '<') {
                s++;
                depth = 1;
            }
            auto trimmed = [](const std::string& b) {
                std::size_t i = 0;
                while (i < b.size() && (b[i] == ' ' || b[i] == '\t')) i++;
                return mx_trim_right(b.substr(i), " \t");
            };
            for (; *s; s++) {
                if (*s == '<') {
                    depth++;
                } else if (*s == '>') {
                    if (depth <= 1) break;
                    depth--;
                }
                if (*s == ',' && depth <= 1) {
                    item.push_back(trimmed(buf));
                    buf.clear();
                    continue;
                }
                if (buf.size() + 1 < 192) buf += *s;
            }
            const std::string t = trimmed(buf);
            if (!t.empty()) item.push_back(t);
        }
        std::vector<std::string> oparam;
        int oseq = 0;
        if (!ppstk.empty() && ppstk.back().is_macro && !ppstk.back().param.empty()) {
            oseq = ++for_seq;
            const std::vector<std::string>& outer = ppstk.back().param;
            for (std::size_t k = 0; k < outer.size(); k++) {
                ppq_add(std::format("%xdefine _mo{}_{} %{{{}}}", oseq, k + 1, k + 1));
                oparam.push_back(outer[k]);
            }
        }
        const std::string nm = std::format("__?masm_for{}?__", ++for_seq);
        ppq_add("%macro " + nm + " 0-*");
        ppq_add("%rep %0");
        ppq_add("%xdefine " + param + " %1");
        ppq_add("%ifnempty %1");
        MxBlk b;
        b.is_for = true;
        b.forname = nm;
        b.item = std::move(item);
        b.forvar = param;
        b.oparam = std::move(oparam);
        b.oseq = oseq;
        ppstk.push_back(std::move(b));
        return ppq_get();
    }
    if (mx_eq(w1, "rept") || mx_eq(w1, "repeat")) {
        mx_skip_ws(p);
        tmp = std::string("%rep ") + p;
        ppstk.emplace_back();
        ppq_add(tmp);
        return ppq_get();
    }
    {
        const char* rest = p;
        const char* dir = nullptr;
        mx_skip_ws(rest);
        if (mx_eq(w1, "if")) dir = "%if";
        else if (mx_eq(w1, "ifdef")) dir = "%ifdef";
        else if (mx_eq(w1, "ifndef")) dir = "%ifndef";
        else if (mx_eq(w1, "elseif")) dir = "%elif";
        else if (mx_eq(w1, "if1")) { dir = "%if"; rest = "1"; }
        else if (mx_eq(w1, "if2")) { dir = "%if"; rest = "0"; }
        if (mx_eq(w1, "elseifdef") || mx_eq(w1, "elseifndef")) {
            const char* s = rest;
            const char* e = rest;
            while (*e && *e != ';') e++;
            while (e > s && (e[-1] == ' ' || e[-1] == '\t')) e--;
            const std::string op(s, static_cast<std::size_t>(e - s));
            ppq_add(std::string(mx_eq(w1, "elseifdef") ? "%elifmacro" : "%elifnmacro") + " " + op);
            return ppq_get();
        }
        std::string ifid;
        if (mx_eq(w1, "ifdef") || mx_eq(w1, "ifndef")) {
            const char* s = rest;
            while (isidchar(*s) && ifid.size() < 127) ifid += *s++;
            if (!ifid.empty() && set_has(defined_tab, ifid)) {
                rest = mx_eq(w1, "ifdef") ? "1" : "0";
                dir = "%if";
            }
        }
        if (dir) {
            if (mx_eq(dir, "%if") || mx_eq(dir, "%elif")) {
                const std::string ex = mx_xlat_ops(rest);
                if (mx_eq(dir, "%if")) emit_opt_guards(ex);
                tmp = std::string(dir) + " " + ex;
            } else if (std::strstr(rest, "%[") || std::strstr(rest, "%{")) {
                const char* s = rest;
                const char* e = rest;
                while (*e && *e != ';') e++;
                while (e > s && (e[-1] == ' ' || e[-1] == '\t')) e--;
                const std::string op(s, static_cast<std::size_t>(e - s));
                tmp = std::string(dir) + " " + op + " __masmL_" + op;
            } else {
                tmp = std::string(dir) + " " + ifid + " __masmL_" + ifid;
            }
            ppq_add(tmp);
            return ppq_get();
        }
        if (mx_eq(w1, "ife")) {
            std::string ex = mx_xlat_ops(rest);
            char qc = 0;
            for (std::size_t i = 0; i < ex.size(); i++) {
                const char c = ex[i];
                if (qc) {
                    if (c == qc) qc = 0;
                } else if (c == '\'' || c == '"') {
                    qc = c;
                } else if (c == ';') {
                    ex.resize(i);
                    break;
                }
            }
            ex = mx_trim_right(ex, " \t");
            emit_opt_guards(ex);
            ppq_add(std::format("%if ({}) == 0", ex));
            return ppq_get();
        }
        if (mx_eq(w1, "else")) {
            ppq_add("%else");
            return ppq_get();
        }
        if (mx_eq(w1, "endif")) {
            ppq_add("%endif");
            return ppq_get();
        }
        {
            const char* cdir = nullptr;
            if (mx_eq(w1, "ifb") || mx_eq(w1, "ifidn0")) cdir = "%ifempty";
            else if (mx_eq(w1, "ifnb")) cdir = "%ifnempty";
            else if (mx_eq(w1, "ifidn")) cdir = "%ifidn";
            else if (mx_eq(w1, "ifidni")) cdir = "%ifidni";
            else if (mx_eq(w1, "ifdif")) cdir = "%ifnidn";
            else if (mx_eq(w1, "ifdifi")) cdir = "%ifnidni";
            if (cdir) {
                std::string stripped;
                for (const char* r = rest; *r; r++)
                    if (*r != '<' && *r != '>') stripped += *r;
                ppq_add(std::string(cdir) + " " + stripped);
                return ppq_get();
            }
        }
        if (mx_eq(w1, ".err")) {
            ppq_add(std::string("%error ") + rest);
            return ppq_get();
        }
        if (mx_eq(w1, ".erre") || mx_eq(w1, ".errnz")) {
            const char* cmp = mx_eq(w1, ".erre") ? "==" : "!=";
            std::string ex = mx_xlat_ops(rest);
            char qc = 0;
            int depth = 0;
            for (std::size_t i = 0; i < ex.size(); i++) {
                const char c = ex[i];
                if (qc) {
                    if (c == qc) qc = 0;
                } else if (c == '\'' || c == '"') {
                    qc = c;
                } else if (c == '(') {
                    depth++;
                } else if (c == ')') {
                    if (depth) depth--;
                } else if (c == ';') {
                    ex.resize(i);
                    break;
                } else if (c == ',' && depth == 0) {
                    ex.resize(i);
                    break;
                }
            }
            ex = mx_trim_right(ex, " \t");
            const std::string rr = rewrite_line(ex);
            ppq_add(std::format("%if ({}) {} 0", rr, cmp));
            ppq_add("%error assertion failed");
            ppq_add("%endif");
            return ppq_get();
        }
        if (mx_eq(w1, ".errnb") || mx_eq(w1, ".errb") || mx_eq(w1, ".errdif") || mx_eq(w1, ".erridn") || mx_eq(w1, ".errdifi") || mx_eq(w1, ".erridni") ||
            mx_eq(w1, ".errdef") || mx_eq(w1, ".errndef"))
            return "";
        if (mx_eq(w1, "echo") || mx_eq(w1, "%out") || mx_eq(w1, ".radix")) return "";
    }
    {
        const char* pe = p;
        mx_skip_ws(pe);
        if (*pe == '=' && pe[1] != '=') {
            const std::string ex = mx_xlat_ops(mx_skip_typeptr(pe + 1));
            const std::string ex2 = rewrite_line(ex);
            std::vector<std::string> fdeps;
            const char* s = ex2.c_str();
            while (*s) {
                if (isidchar(*s)) {
                    const char* st = s;
                    const bool isnum = isdigit_(*s);
                    while (isidchar(*s)) s++;
                    if (!isnum && !mx_is_wordop(std::string_view(st, static_cast<std::size_t>(s - st)))) {
                        const std::string id(st, static_cast<std::size_t>(s - st));
                        if (id.size() < 128) {
                            if (stricmp_(id, w1) && !set_has(defined_tab, id) && set_has(const_tab, id)) {
                                bool dup = false;
                                for (const std::string& d : fdeps)
                                    if (mx_eq(d, id)) dup = true;
                                if (!dup) fdeps.push_back(id);
                            }
                        }
                    }
                } else {
                    s++;
                }
            }
            if (!fdeps.empty()) {
                pendings.push_front({w1, ex2, fdeps});
                return "";
            }
            ppq_add(std::format("%iassign {} {}", w1, ex2));
            note_defined(w1);
            return ppq_get();
        }
    }
    {
        std::string w = mx_word(p);
        l2 = w.size();
        w2 = w;
    }
    if (l2 && (mx_eq(w2, "struct") || mx_eq(w2, "struc") || mx_eq(w2, "union"))) {
        in_struct = true;
        cur = MxSDef();
        cur.name = w1;
        cur.is_union = mx_eq(w2, "union");
        have_cur = true;
        if (cur.is_union) return "";
        ppq_add("struc " + w1);
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "label")) {
        std::string ty;
        const char* r = p;
        mx_skip_ws(r);
        while (isidchar(*r) && ty.size() + 1 < 32) ty += *r++;
        const int sz = mx_type_bytes(ty);
        if (sz > 0) as.masm_type_note(w1, sz);
        ppq_add(w1 + ":");
        ppq_add("%idefine __masmL_" + w1 + " 1");
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "record")) {
        struct Fld {
            std::string nm;
            int w;
        };
        std::vector<Fld> fld;
        int total = 0;
        const char* s = p;
        while (*s && fld.size() < 32) {
            std::string nm;
            int w = 0;
            while (*s == ' ' || *s == '\t' || *s == ',') s++;
            if (!isidstart(*s)) break;
            while (isidchar(*s)) {
                if (nm.size() + 1 < 64) nm += *s;
                s++;
            }
            mx_skip_ws(s);
            if (*s == ':') {
                s++;
                mx_skip_ws(s);
                w = std::atoi(s);
                while (isdigit_(*s)) s++;
            }
            if (w <= 0) break;
            fld.push_back({nm, w});
            total += w;
        }
        MxSDef sd;
        sd.name = w1;
        sd.is_record = true;
        sd.rdir = total <= 8 ? "db" : total <= 16 ? "dw" : "dd";
        int pos = total;
        for (const Fld& f : fld) {
            const int width = f.w;
            pos -= width;
            const unsigned mask = (width >= 32 ? 0xffffffffu : ((1u << width) - 1)) << pos;
            ppq_add(std::format("%idefine {} {}", f.nm, pos));
            ppq_add(std::format("%define MASK_{} 0{:08X}h", f.nm, mask));
            ppq_add(std::format("%define WIDTH_{} {}", f.nm, width));
            MxSMember mb;
            mb.name = f.nm;
            mb.shift = pos;
            mb.count = 1;
            sd.members.push_back(std::move(mb));
        }
        ppq_add(std::format("%idefine {}_size {}", w1, total <= 8 ? 1 : total <= 16 ? 2 : 4));
        sdefs.push_back(std::move(sd));
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "catstr")) {
        std::string rhs;
        const char* s = p;
        int depth = 0;
        mx_skip_ws(s);
        while (*s) {
            if (*s == '<') {
                depth++;
                s++;
                continue;
            }
            if (*s == '>') {
                if (depth) depth--;
                s++;
                continue;
            }
            if (*s == ',' && depth == 0) {
                rhs += " %+ ";
                s++;
                mx_skip_ws(s);
                continue;
            }
            rhs += *s;
            s++;
        }
        ppq_add("%ixdefine " + w1 + " " + rhs);
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "instr")) {
        std::vector<std::string> part(1);
        int depth = 0;
        const char* s = p;
        mx_skip_ws(s);
        for (; *s && part.size() <= 3; s++) {
            if (*s == '<') {
                depth++;
                continue;
            }
            if (*s == '>') {
                if (depth) depth--;
                continue;
            }
            if (*s == ',' && depth == 0) {
                if (part.size() == 3) break;
                part.emplace_back();
                continue;
            }
            if (part.back().size() + 1 < 256) part.back() += *s;
        }
        int start = 1, base = 0, pos = 0;
        const int np = static_cast<int>(part.size());
        if (np >= 3) {
            start = std::atoi(part[0].c_str());
            if (start < 1) start = 1;
            base = 1;
        }
        const char* s1 = part[static_cast<std::size_t>(base)].c_str();
        const char* s2 = static_cast<std::size_t>(base + 1) < part.size() ? part[static_cast<std::size_t>(base + 1)].c_str() : "";
        mx_skip_ws(s1);
        mx_skip_ws(s2);
        if (static_cast<int>(std::strlen(s1)) >= start - 1) {
            const char* hit = std::strstr(s1 + (start - 1), s2);
            if (hit) pos = static_cast<int>(hit - s1) + 1;
        }
        ppq_add(std::format("%iassign {} {}", w1, pos));
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "sizestr")) {
        const char* s = p;
        mx_skip_ws(s);
        if (*s == '<') {
            int n = 0, depth = 1;
            for (const char* t = s + 1; *t; t++) {
                if (*t == '<') depth++;
                else if (*t == '>') {
                    if (--depth == 0) break;
                }
                n++;
            }
            ppq_add(std::format("%iassign {} {}", w1, n));
        } else {
            ppq_add(std::string("%defstr __masm_sstmp ") + s);
            ppq_add("%strlen " + w1 + " __masm_sstmp");
        }
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "substr")) {
        const char* s = p;
        std::string src;
        int depth = 0;
        mx_skip_ws(s);
        for (; *s; s++) {
            if (*s == '<') {
                depth++;
                continue;
            }
            if (*s == '>') {
                if (depth) depth--;
                continue;
            }
            if (*s == ',' && depth == 0) break;
            if (src.size() + 1 < 256) src += *s;
        }
        src = mx_trim_right(src, " \t");
        const std::string rest = (*s == ',') ? std::string(s + 1) : std::string();
        ppq_add("%defstr __masm_sstmp " + src);
        ppq_add("%substr __masm_sstmp2 __masm_sstmp," + rest);
        ppq_add("%deftok " + w1 + " __masm_sstmp2");
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "typedef")) {
        const char* sp = p;
        const std::string sw = mx_word(sp);
        int sz = !sw.empty() ? mx_type_bytes(sw) : 0;
        const char* dir;
        if (sz) {
            dir = mx_type_to_dd(sw);
        } else {
            dir = "dd";
            sz = 4;
        }
        ppq_add(std::string("%idefine ") + w1 + " " + (dir ? dir : "dd"));
        ppq_add(std::format("%idefine {}_size {}", w1, sz));
        return ppq_get();
    }
    {
        MxSDef* sd0 = sdef_find(w1);
        const char* ip0 = p;
        mx_skip_ws(ip0);
        if (sd0 && !sd0->is_record && *ip0 == '<') {
            std::vector<std::string> inits = split_inits(ip0 + 1);
            const int ninit = static_cast<int>(inits.size());
            int mi = 0;
            for (const MxSMember& mb : sd0->members) {
                std::string v = "0";
                const char* dir = mb.dir ? mb.dir : "dd";
                if (mi < ninit) {
                    const std::string val = clean_init(inits[static_cast<std::size_t>(mi)]);
                    if (!val.empty()) v = val;
                }
                mi++;
                if (mb.count > 1) ppq_add(std::format("times {} {} {}", mb.count, dir, v));
                else ppq_add(std::format("{} {}", dir, v));
            }
            return ppq_get();
        }
    }
    if (l2 && sdef_find(w2)) return struct_instance(*sdef_find(w2), w1, p);
    if (l2 && mx_eq(w2, "equ")) {
        const char* v = p;
        mx_skip_ws(v);
        if (*v == '<') {
            std::string buf = mx_trim_right(v, " \t\r");
            if (buf.size() >= 2 && buf[0] == '<' && buf.back() == '>') {
                buf.pop_back();
                buf.erase(0, 1);
            }
            {
                std::string o;
                std::size_t i = 0;
                while (i < buf.size()) {
                    if (!strnicmp_(std::string_view(buf).substr(i), "offset", 6) && (i == 0 || !isidchar(buf[i - 1]))) {
                        std::size_t j = i + 6;
                        while (j < buf.size() && (buf[j] == ' ' || buf[j] == '\t')) j++;
                        if (!strnicmp_(std::string_view(buf).substr(j), "flat", 4) && !isidchar(j + 4 < buf.size() ? buf[j + 4] : '\0')) {
                            j += 4;
                            while (j < buf.size() && (buf[j] == ' ' || buf[j] == '\t')) j++;
                            if (j < buf.size() && buf[j] == ':') {
                                i = j + 1;
                                continue;
                            }
                        }
                    }
                    o += buf[i++];
                }
                buf = o;
            }
            ppq_add("%idefine " + w1 + " " + mx_xlat_ops(buf.c_str()));
            return ppq_get();
        }
        {
            bool numeq = false;
            {
                const char* sk = mx_skip_typeptr(v);
                if (sk != v && (*sk == '\0' || *sk == ';')) {
                    const char* e = v;
                    while (*e && *e != ';') e++;
                    while (e > v && (e[-1] == ' ' || e[-1] == '\t')) e--;
                    const std::size_t n = static_cast<std::size_t>(e - v);
                    if (n && n < 448) {
                        ppq_add("%idefine " + w1 + " " + std::string(v, n));
                        return ppq_get();
                    }
                }
            }
            const std::string ex = mx_xlat_ops(mx_skip_typeptr(v));
            const char* xr = ex.c_str();
            mx_skip_ws(xr);
            if (*xr == '[') {
                tmp = "%idefine " + w1 + " " + rewrite_line(xr);
            } else if (mx_ident_only(xr) && (mx_eq(xr, "short") || mx_eq(xr, "near") || mx_eq(xr, "far") || mx_eq(xr, "flat") || mx_eq(xr, "use32") || mx_eq(xr, "use16"))) {
                tmp = "%idefine " + w1 + " " + xr;
            } else if (mx_ident_only(xr) && stricmp_(xr, w1)) {
                ppq_add(std::string("%ifmacro ") + xr);
                ppq_add("%idefine " + w1 + " " + xr);
                ppq_add("%else");
                ppq_add(w1 + " equ " + xr);
                ppq_add("%endif");
                note_defined(w1);
                return ppq_get();
            } else {
                const std::string rr = rewrite_line(xr);
                auto r = emit_num_const(w1, rr);
                if (r) return *r;
                tmp = w1 + " equ " + rr;
                numeq = true;
            }
            ppq_add(tmp);
            if (numeq) note_defined(w1);
            return ppq_get();
        }
    }
    if (l2 && mx_eq(w2, "textequ")) {
        const char* v = p;
        mx_skip_ws(v);
        std::string buf = mx_trim_right(v, " \t\r");
        if (buf.size() >= 2 && buf[0] == '<' && buf.back() == '>') {
            buf.pop_back();
            buf.erase(0, 1);
        }
        ppq_add("%ixdefine " + w1 + " " + buf);
        return ppq_get();
    }
    if (l2 && mx_eq(w2, "macro")) {
        std::vector<std::string> param;
        const char* q = p;
        mx_skip_ws(q);
        while (*q) {
            std::string nm;
            mx_skip_ws(q);
            while (isidchar(*q)) {
                nm += *q;
                q++;
            }
            if (!nm.empty()) param.push_back(nm);
            while (*q && *q != ',') q++;
            if (*q == ',') q++;
            else break;
        }
        ppq_add("%imacro " + w1 + " 0-*");
        MxBlk b;
        b.is_macro = true;
        b.param = std::move(param);
        ppstk.push_back(std::move(b));
        return ppq_get();
    }
    if (l2 && isidstart(w1[0]) && (mx_eq(w2, "db") || mx_eq(w2, "dw") || mx_eq(w2, "dd") || mx_eq(w2, "dq") || mx_eq(w2, "dt") || mx_eq(w2, "df"))) {
        bool has_dup = false;
        const char* base = line.c_str();
        for (const char* s = base; *s && *s != ';'; s++) {
            if ((s == base || !isidchar(s[-1])) && !strnicmp_(s, "dup", 3) && !isidchar(s[3])) {
                has_dup = true;
                break;
            }
        }
        if (has_dup) {
            std::string rl = rewrite_line(line);
            ppq_add(std::format("{}_size equ $ - {}", w1, w1));
            return rl;
        }
    }
    if (line.find('<') != std::string::npos) {
        std::string br;
        if (mx_angle_args_to_brace(line, br)) line = br;
    }
    line = rewrite_line(line);
    if (line.find_first_of("'\"") != std::string::npos) {
        std::string sf;
        if (mx_fix_dq_strings(line, sf)) line = sf;
    }
    return line;
}

// ---- the public face ----

MasmFront::MasmFront(Assembly& assembly, std::function<bool()> skipping)
    : impl_(std::make_unique<Impl>(assembly, std::move(skipping))) {}
MasmFront::~MasmFront() = default;

void MasmFront::reset() {
    Impl& m = *impl_;
    m.anon_seq = 0;
    m.locif_cond.reset();
    m.defined_tab.clear();
    m.const_tab.clear();
    m.pendings.clear();
}

std::optional<std::string> MasmFront::take_queued() { return impl_->ppq_get_opt(); }

std::string MasmFront::xform(std::string line) { return impl_->xform(std::move(line)); }

void MasmFront::prescan_consts(std::string_view text) { impl_->prescan(text); }

}  // namespace cb::build::x86asm
