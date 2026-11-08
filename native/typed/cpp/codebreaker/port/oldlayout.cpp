// codebreaker/port/oldlayout.cpp -- typed C++ port of codebreaker.port.oldlayout (see oldlayout.hpp).

#include "codebreaker/port/oldlayout.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/ctext.hpp"
#include "json.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::port::oldlayout {

using arith16::Loc;
using py::str;

py::dict<py::str, py::list<bool>> PTR_PARAMS;

namespace {

str S(const char* s) { return str(s); }

bool is_(const std::optional<str>& a, const char* s) { return a.has_value() && *a == str(s); }

// {} -- a node that is not there
const Node& empty() {
    static const Node e;
    return e;
}

// (n.get("inner") or [{}])[0]
const Node& first(const Node& n) { return py::truthy(n.inner) ? n.inner[0] : empty(); }

// (n.get("type") or {}).get("qualType", "")
str qt(const Node& n) { return n.type.has_value() && n.type->qualType.has_value() ? *n.type->qualType : S(""); }

// re.sub(r"\s+", "", x)
str squeeze(const str& x) { return py::re::sub(S("\\s+"), S(""), x); }

// ---- the JSON of clang's tree as records (copies of arith16's parsers, which are not exported)
Loc loc_of(const py::V& v) {
    Loc l;
    if (!v.is_dict()) {
        return l;
    }
    const py::V& off = v.get("offset");
    if (off.is_int()) {
        l.offset = off.as_int();
    }
    const py::V& tok = v.get("tokLen");
    if (tok.is_int()) {
        l.tokLen = tok.as_int();
    }
    const py::V& file = v.get("file");
    if (file.is_str()) {
        l.file = str(file.str());
    }
    if (v.get("spellingLoc").is_dict()) {
        l.spellingLoc = std::make_shared<Loc>(loc_of(v.get("spellingLoc")));
    }
    if (v.get("expansionLoc").is_dict()) {
        l.expansionLoc = std::make_shared<Loc>(loc_of(v.get("expansionLoc")));
    }
    if (v.get("includedFrom").is_dict()) {
        l.includedFrom = std::make_shared<Loc>(loc_of(v.get("includedFrom")));
    }
    return l;
}

std::optional<str> json_str(const py::V& v) {
    if (v.is_str()) {
        return str(v.str());
    }
    return std::nullopt;
}

Node node_of(const py::V& v) {
    Node n;
    n.kind = json_str(v.get("kind"));
    n.opcode = json_str(v.get("opcode"));
    n.castKind = json_str(v.get("castKind"));
    if (v.get("type").is_dict()) {
        arith16::TypeRef t;
        t.qualType = json_str(v.get("type").get("qualType"));
        n.type = t;
    }
    const py::V& val = v.get("value");
    if (val.is_intlike()) {
        n.value = arith16::Number(val.as_int());
    } else if (val.is_str()) {
        n.value = arith16::Number(str(val.str()));
    }
    n.loc = loc_of(v.get("loc"));
    if (v.get("range").is_dict()) {
        n.range.begin = loc_of(v.get("range").get("begin"));
        n.range.end = loc_of(v.get("range").get("end"));
    }
    if (v.get("inner").is_list()) {
        for (const py::V& x : v.get("inner").seq()) {
            if (x.is_dict()) {
                n.inner.append(node_of(x));
            }
        }
    }
    return n;
}

// the Extra of each node of the tree (walked together with the JSON it was made from: the same children in the same order)
void index_extra(const Node& n, const py::V& v, Tree& t) {
    Extra e;
    const py::V& id = v.get("id");
    if (id.is_str()) {
        e.id = str(id.str());
    }
    if (v.get("name").is_str()) {
        e.name = str(v.get("name").str());
    }
    if (v.get("tagUsed").is_str()) {
        e.tagUsed = str(v.get("tagUsed").str());
    }
    e.completeDefinition = py::truthy(v.get("completeDefinition"));
    if (v.get("referencedDecl").get("name").is_str()) {
        e.referencedDeclName = str(v.get("referencedDecl").get("name").str());
    }
    if (v.get("referencedMemberDecl").is_str()) {
        e.referencedMemberDecl = str(v.get("referencedMemberDecl").str());
    }
    t.extra[&n] = e;
    if (v.get("inner").is_list()) {
        int64_t i = 0;
        for (const py::V& x : v.get("inner").seq()) {
            if (x.is_dict()) {
                index_extra(n.inner[i], x, t);
                ++i;
            }
        }
    }
}

// the id of a declaration: n["id"]
const str& id_of(const Tree& tree, const Node& n) {
    const Extra& e = tree.ex(n);
    if (!e.id.has_value()) {
        throw py::KeyError("'id'");
    }
    return *e.id;
}

}  // namespace

const Extra& Tree::ex(const Node& n) const {
    static const Extra none;
    auto it = extra.find(&n);
    return it == extra.end() ? none : it->second;
}

std::shared_ptr<Tree> _dump(const py::str& cc, const py::str& src, const py::str& folder) {
    py::list<str> args{cc,
                       S("-fsyntax-only"),
                       S("-Xclang"),
                       S("-ast-dump=json"),
                       S("-fsigned-char"),
                       S("-std=c11"),
                       S("-D_CRT_SECURE_NO_WARNINGS"),
                       S("-D_CRT_NONSTDC_NO_WARNINGS"),
                       S("-I."),
                       src};
    py::subprocess::RunOptions opts;
    opts.cwd = folder;
    opts.text = false;
    py::subprocess::CompletedProcess p = py::subprocess::run(args, opts);
    try {
        py::V doc = py::json_parse(p.stdout_bytes->decode("utf-8", "replace").utf8());
        if (!doc.is_dict()) {
            return nullptr;
        }
        auto t = std::make_shared<Tree>();
        t->root = node_of(doc);
        index_extra(t->root, doc, *t);
        return t;
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") {
            return nullptr;
        }
        throw;
    } catch (const py::ValueError&) {
        return nullptr;
    }
}

void _walk(const Node& node, const WalkFn& fn, Parents& parents) {
    fn(node, parents);
    parents.push_back(&node);
    for (const Node& v : node.inner.vec()) {
        _walk(v, fn, parents);
    }
    parents.pop_back();
}

void _walk(const Node& node, const WalkFn& fn) {
    Parents parents;
    _walk(node, fn, parents);
}

std::tuple<py::dict<py::str, RecInfo>, py::dict<py::str, FieldInfo>> _records(const Tree& tree) {
    py::dict<str, RecInfo> recs;
    py::dict<str, FieldInfo> fields;

    _walk(tree.root, [&](const Node& n, const Parents& parents) {
        if (is_(n.kind, "RecordDecl") && tree.ex(n).completeDefinition) {
            std::optional<str> par;
            for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
                if (is_((*it)->kind, "RecordDecl")) {
                    par = tree.ex(**it).id;
                    break;
                }
            }
            const Extra& e = tree.ex(n);
            str name = e.name;
            str tag = py::truthy(name) ? e.tagUsed + " " + name : S("");
            py::list<str> fids;
            for (const Node& x : n.inner) {
                if (is_(x.kind, "FieldDecl")) {
                    fids.append(id_of(tree, x));
                }
            }
            recs.set(id_of(tree, n), RecInfo(tag, fids, par));
            for (const Node& x : n.inner) {
                if (is_(x.kind, "FieldDecl")) {
                    fields.set(id_of(tree, x), FieldInfo(id_of(tree, n), tree.ex(x).name, qt(x)));
                }
            }
        }
    });
    return {recs, fields};
}

py::str _top(const py::dict<py::str, RecInfo>& recs, const py::str& rid0) {
    py::set<str> seen;
    str rid = rid0;
    while (recs.contains(rid) && !py::truthy(std::get<0>(recs.at(rid))) && std::get<2>(recs.at(rid)).has_value() &&
           py::truthy(*std::get<2>(recs.at(rid))) && !seen.contains(rid)) {
        seen.add(rid);
        rid = *std::get<2>(recs.at(rid));
    }
    return recs.contains(rid) ? std::get<0>(recs.at(rid)) : S("");
}

std::optional<py::str> _rtype(const std::optional<py::str>& t0) {
    str t = py::re::sub(S("\\s+"), S(" "), (t0.has_value() ? *t0 : S("")).replace(S("const "), S(""))).strip();
    std::optional<py::re::Match> m = py::re::fullmatch(S("((?:struct|union) \\w+)(?: \\*)?"), t);
    if (m) {
        return m->group(1);
    }
    return std::nullopt;
}

py::str callee_name(const Tree& tree, const Node* call) {
    const Node* c = call != nullptr ? &first(*call) : &empty();
    while ((is_(c->kind, "ImplicitCastExpr") || is_(c->kind, "ParenExpr")) && py::truthy(c->inner)) {
        c = &c->inner[0];
    }
    return is_(c->kind, "DeclRefExpr") ? tree.ex(*c).referencedDeclName : S("");
}

py::set<py::str> byte_records(const py::list<std::shared_ptr<Tree>>& trees) {
    py::set<str> out;

    for (const std::shared_ptr<Tree>& tp : trees) {
        const Tree& tree = *tp;
        auto call_of = [](const Parents& parents) -> const Node* {
            for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
                if (is_((*it)->kind, "CallExpr")) {
                    return *it;
                }
            }
            return nullptr;
        };
        auto read_write = [&](const str& t, const Parents& parents) {
            const Node* call = call_of(parents);
            str name = callee_name(tree, call);
            if (t.endswith(S("*")) && _rtype(t) && py::re::fullmatch(S("(?:dos_)?(?:read|write|fread|fwrite)"), name)) {
                out.add(*_rtype(t));
            }
        };
        _walk(tree.root, [&](const Node& n, const Parents& parents) {
            if ((is_(n.kind, "ImplicitCastExpr") || is_(n.kind, "CStyleCastExpr")) &&
                (qt(n).replace(S(" "), S("")) == S("void*") || qt(n).replace(S(" "), S("")) == S("constvoid*"))) {
                const Node& x = arith16::bare(first(n));   // (read(fd, p, 12): a record's pointer given as the
                str t = qt(x);                             //  bytes to fill or write)
                read_write(t, parents);
                return;
            }
            if (!is_(n.kind, "CStyleCastExpr") || !(qt(n).replace(S(" "), S("")) == S("char*") || qt(n).replace(S(" "), S("")) == S("unsignedchar*"))) {
                return;
            }
            const Node* x = &arith16::bare(first(n));
            if (is_(x->kind, "ImplicitCastExpr")) {
                x = &arith16::bare(first(*x));
            }
            if (is_(x->kind, "UnaryOperator") && is_(x->opcode, "&")) {
                const Node* y = &arith16::bare(first(*x));
                if (is_(y->kind, "MemberExpr")) {   // (the bytes of one number - (char *)&p->type: its
                    str mt = qt(*y);                //  bytes the same in either layout)
                    if (!(_rtype(mt) || mt.contains(S("[")) || mt.startswith(std::make_tuple("struct", "union")))) {
                        return;
                    }
                }
                while (is_(y->kind, "MemberExpr")) {   // (a member of a record: the record it is in)
                    const Node& base = arith16::bare(first(*y));
                    std::optional<str> bt = _rtype(qt(base));
                    if (bt) {
                        out.add(*bt);
                        return;
                    }
                    y = &base;
                }
                std::optional<str> t = _rtype(qt(*y));
                if (t) {
                    out.add(*t);
                }
            } else {   // ((char *)h given to read or write: its bytes)
                str t = qt(*x);
                read_write(t, parents);
            }
        });
    }
    return out;
}

namespace {

// the (offset, tokLen, file) of a location as place() reads it
struct Where {
    std::optional<int64_t> offset;
    int64_t tokLen;
    std::optional<str> file;
};

Where where_of(const Loc& x) {
    if (x.expansionLoc) {
        const Loc& e = *x.expansionLoc;
        // x["expansionLoc"].get("_file") or x.get("_file")
        std::optional<str> f = e._file.has_value() && py::truthy(*e._file) ? e._file : x._file;
        return {e.offset, e.tokLen.value_or(0), f};
    }
    if (x.spellingLoc) {
        return {std::nullopt, 0, std::nullopt};
    }
    return {x.offset, x.tokLen.value_or(0), x._file};
}

}  // namespace

std::optional<std::tuple<int64_t, int64_t>> place(const Node& n, const py::str& path) {
    const Loc& b = n.range.begin;
    const Loc& e = n.range.end;
    Where bw = where_of(b), ew = where_of(e);
    if (!bw.offset.has_value() || !ew.offset.has_value() || !(bw.file.has_value() && *bw.file == path) ||
        !(ew.file.has_value() && *ew.file == path) || *ew.offset < *bw.offset) {
        return std::nullopt;
    }
    int64_t b0 = *bw.offset, e0 = *ew.offset, el = ew.tokLen;
    if (b0 == e0 && b.expansionLoc) {
        if (!arith16::TEXT.has_value()) {
            throw py::TypeError("'NoneType' object is not subscriptable");
        }
        str tail = arith16::TEXT->slice(e0 + el, e0 + el + 40);
        if (py::re::match(S("\\s*\\("), tail)) {
            return std::nullopt;   // (all inside a macro with arguments)
        }
    }
    return std::make_tuple(b0, e0 + el);
}

std::tuple<Edits, Found, Refused> edits_of(const Tree& tree, const py::str& text, const py::str& path, const py::set<py::str>& olds) {
    auto rf = _records(tree);
    const py::dict<str, RecInfo>& recs = std::get<0>(rf);
    const py::dict<str, FieldInfo>& fields = std::get<1>(rf);
    py::dict<str, str> ptrf;
    for (const str& fid : fields.keys()) {
        const FieldInfo& fi = fields.at(fid);
        const str& t = std::get<2>(fi);
        if (olds.contains(_top(recs, std::get<0>(fi))) && t.rstrip().endswith(S("*")) && !t.contains(S("("))) {
            ptrf.set(fid, t);
        }
    }
    arith16::MAIN = path;
    arith16::TEXT = text;
    Edits out;
    Found found;
    Refused refused;

    py::set<str> intf;   // (its int members: a pointer kept in one, a word)
    for (const str& fid : fields.keys()) {
        const FieldInfo& fi = fields.at(fid);
        if (olds.contains(_top(recs, std::get<0>(fi))) &&
            py::re::fullmatch(S("(?:unsigned |signed )?(?:short|int|u?int16_t|intptr_t)( int)?"), std::get<2>(fi).strip())) {   // (intptr_t: a word declare makes one again)
            intf.add(fid);
        }
    }

    auto tptr = [](const Node& x) { return qt(x); };

    auto int_word = [&](const Node& n, const Parents& parents) {
        // an int member of a record kept old given a pointer, or read as one: the handle of it
        const Node& par = !parents.empty() ? *parents.back() : empty();
        const std::optional<str>& k = par.kind;
        auto p = place(n, path);
        if (!p) {
            return;
        }
        int64_t depth = static_cast<int64_t>(parents.size());
        if (is_(k, "BinaryOperator") && is_(par.opcode, "=") && py::truthy(par.inner) && &par.inner[0] == &n) {
            if (py::len(par.inner) < 2) {
                throw py::IndexError("list index out of range");
            }
            const Node* rhs = &par.inner[1];
            while ((is_(rhs->kind, "ParenExpr") || (is_(rhs->kind, "ImplicitCastExpr") && is_(rhs->castKind, "IntegralCast"))) &&   // (n->i2 =
                   py::truthy(rhs->inner)) {                                                                               //  (intptr_t)(f()): to 16 bits too)
                rhs = &rhs->inner[0];
            }
            if ((is_(rhs->kind, "ImplicitCastExpr") && is_(rhs->castKind, "PointerToIntegral")) ||
                (is_(rhs->kind, "CStyleCastExpr") && is_(rhs->castKind, "PointerToIntegral"))) {
                auto q = place(*rhs, path);
                if (is_(rhs->kind, "CStyleCastExpr") && py::truthy(rhs->inner)) {   // ((intptr_t)(f()): the pointer
                    auto q2 = place(rhs->inner[0], path);                           //  inside the cast made the handle)
                    q = q2 ? q2 : q;
                }
                if (q) {   // (a pointer given it: its handle)
                    out.append(std::make_tuple(std::get<0>(*q), depth, S("dos_ph(")));
                    out.append(std::make_tuple(std::get<1>(*q), -depth, S(")")));
                    const Node* gp = &empty();
                    for (auto it = parents.rbegin() + 1; it < parents.rend(); ++it) {   // reversed(parents[:-1])
                        if (!is_((*it)->kind, "ParenExpr")) {
                            gp = *it;
                            break;
                        }
                    }
                    auto w = place(par, path);   // (the value of it as a pointer: the pointer again)
                    if (w && is_(gp->castKind, "IntegralToPointer")) {
                        out.append(std::make_tuple(std::get<0>(*w), depth - 1, S("dos_hp(")));
                        out.append(std::make_tuple(std::get<1>(*w), -(depth - 1), S(")")));
                    }
                }
            }
            return;
        }
        const Node* up = &empty();
        for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
            const Node& x = **it;
            if (!(is_(x.kind, "ParenExpr") ||
                  (is_(x.kind, "ImplicitCastExpr") && (is_(x.castKind, "LValueToRValue") || is_(x.castKind, "IntegralCast"))))) {
                up = &x;
                break;
            }
        }
        if ((is_(up->kind, "ImplicitCastExpr") || is_(up->kind, "CStyleCastExpr")) && is_(up->castKind, "IntegralToPointer")) {
            out.append(std::make_tuple(std::get<0>(*p), depth, S("dos_hp(")));   // (read as a pointer: what the handle stands for)
            out.append(std::make_tuple(std::get<1>(*p), -depth, S(")")));
            return;
        }
        if (is_(up->kind, "BinaryOperator") && is_(up->opcode, "=") && py::len(up->inner) == 2 && &up->inner[0] != &n &&
            is_(up->inner[0].kind, "DeclRefExpr") &&
            (squeeze(tptr(up->inner[0])) == S("intptr_t") || squeeze(tptr(up->inner[0])) == S("longlong"))) {   // (g_74be = n->i2, the
            out.append(std::make_tuple(std::get<0>(*p), depth, S("(intptr_t)dos_hp(")));                        //  variable one the port made hold pointers: the
            out.append(std::make_tuple(std::get<1>(*p), -depth, S(")")));                                       //  pointer the handle stands for)
            return;
        }
        if (is_(up->kind, "CallExpr") && py::truthy(PTR_PARAMS)) {   // (given to an old-style function's parameter that is
            // args = (up.get("inner") or [])[1:]                    //  a pointer - no conversion said so: the definition's
            std::vector<const Node*> args;                           //  parameter does)
            for (int64_t i = 1; i < py::len(up->inner); ++i) {
                args.push_back(&up->inner[i]);
            }
            std::optional<size_t> at;
            for (size_t i = 0; i < parents.size(); ++i) {
                if (parents[i] == up) {
                    at = i;
                    break;
                }
            }
            std::vector<const Node*> chain;
            if (at.has_value()) {
                for (size_t i = *at + 1; i < parents.size(); ++i) {
                    chain.push_back(parents[i]);
                }
                chain.push_back(&n);
            }
            const Node* arg = !chain.empty() ? chain[0] : nullptr;
            std::optional<int64_t> kk;
            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == arg) {
                    kk = static_cast<int64_t>(i);
                    break;
                }
            }
            str cn = callee_name(tree, up);
            if (kk.has_value() && PTR_PARAMS.contains(cn)) {
                const py::list<bool>& ps = PTR_PARAMS.at(cn);
                if (py::truthy(ps) && *kk < py::len(ps) && ps[*kk]) {
                    out.append(std::make_tuple(std::get<0>(*p), depth, S("dos_hp(")));
                    out.append(std::make_tuple(std::get<1>(*p), -depth, S(")")));
                }
            }
        }
    };

    _walk(tree.root, [&](const Node& n, const Parents& parents) {
        if (!is_(n.kind, "MemberExpr")) {
            return;
        }
        str fid = tree.ex(n).referencedMemberDecl;
        if (intf.contains(fid)) {
            int_word(n, parents);
            return;
        }
        if (!ptrf.contains(fid)) {
            return;
        }
        auto p = place(n, path);
        if (!p) {
            return;
        }
        str t = ptrf.at(fid);
        found.set(std::make_tuple(_top(recs, std::get<0>(fields.at(fid))), std::get<1>(fields.at(fid))), t);
        int64_t depth = static_cast<int64_t>(parents.size());
        const Node& par = !parents.empty() ? *parents.back() : empty();
        const std::optional<str>& k = par.kind;
        if (is_(k, "BinaryOperator") && is_(par.opcode, "=") && py::truthy(par.inner) && &par.inner[0] == &n) {
            if (py::len(par.inner) < 2) {
                throw py::IndexError("list index out of range");
            }
            const Node& rhs = par.inner[1];
            auto q = place(rhs, path);
            if (q) {   // (a write: the handle of what is given)
                out.append(std::make_tuple(std::get<0>(*q), depth, S("dos_ph(")));
                out.append(std::make_tuple(std::get<1>(*q), -depth, S(")")));
                const Node& gp = parents.size() > 1 ? *parents[parents.size() - 2] : empty();
                auto w = place(par, path);
                const std::optional<str>& gk = gp.kind;
                const py::list<Node>& gi = gp.inner;
                std::optional<int64_t> cond;
                if (is_(gk, "IfStmt") || is_(gk, "WhileStmt") || is_(gk, "SwitchStmt")) {
                    cond = 0;
                } else if (is_(gk, "DoStmt")) {
                    cond = 1;
                } else if (is_(gk, "ForStmt")) {
                    cond = 2;
                }
                bool used = gk.has_value() && !(is_(gk, "CompoundStmt") || is_(gk, "CaseStmt") || is_(gk, "DefaultStmt") || is_(gk, "LabelStmt")) &&
                            (!cond.has_value() || (py::len(gi) > *cond && &gi[*cond] == &par));   // (a statement of an if: not used)
                if (w && used) {
                    out.append(std::make_tuple(std::get<0>(*w), depth - 1, S("((") + t + ")dos_hp("));   // (its value used - t = n->left = x: the
                    out.append(std::make_tuple(std::get<1>(*w), -(depth - 1), S("))")));                 //  pointer again, not the handle)
                }
            }
            return;
        }
        if ((is_(k, "UnaryOperator") && (is_(par.opcode, "&") || is_(par.opcode, "++") || is_(par.opcode, "--"))) ||
            (is_(k, "CompoundAssignOperator") && py::truthy(par.inner) && &par.inner[0] == &n)) {
            refused.append(std::make_tuple(std::get<0>(*p), par.opcode));
            return;
        }
        out.append(std::make_tuple(std::get<0>(*p), depth, S("((") + t + ")dos_hp("));   // (a read: the pointer the handle stands for)
        out.append(std::make_tuple(std::get<1>(*p), -depth, S("))")));
    });
    return {out, found, refused};
}

py::str apply(const py::str& text0, const Edits& edits) {
    std::map<int64_t, std::vector<std::pair<int64_t, str>>> by;
    for (const auto& [at, depth, s] : edits) {
        by[at].push_back({depth, s});
    }
    str text = text0;
    for (auto it = by.rbegin(); it != by.rend(); ++it) {   // sorted(by, reverse=True)
        std::vector<std::pair<int64_t, str>> opens, closes;
        for (const auto& x : it->second) {
            (x.first >= 0 ? opens : closes).push_back(x);
        }
        auto by_depth = [](const std::pair<int64_t, str>& a, const std::pair<int64_t, str>& b) { return a.first < b.first; };
        std::stable_sort(opens.begin(), opens.end(), by_depth);     // (outer first)
        std::stable_sort(closes.begin(), closes.end(), by_depth);   // (-depth: inner first)
        str s;
        for (const auto& c : closes) {
            s += c.second;   // (the inner closed first)
        }
        for (const auto& o : opens) {
            s += o.second;
        }
        int64_t at = it->first;
        text = text.slice(0, at) + s + text.slice_from(at);
    }
    return text;
}

py::str null_reads(const py::str& text0) {
    str text = text0;
    static const py::re::Pattern cast = py::re::compile(S("\\(\\((?:struct|union)\\s+\\w+\\s*\\*\\s*\\)dos_hp\\("));

    auto close = [](const str& code, int64_t i) -> int64_t {   // (the ')' that closes the '(' at i)
        int64_t d = 0;
        const std::u32string& u = code.u32();
        for (int64_t k = i; k < static_cast<int64_t>(u.size()); ++k) {
            if (u[static_cast<size_t>(k)] == U'(') {
                d += 1;
            } else if (u[static_cast<size_t>(k)] == U')') {
                d -= 1;
            }
            if (d == 0) {
                return k;
            }
        }
        return -1;
    };
    str code = base::ctext::blank(text);
    py::set<int64_t> at;   // (where dos_hp( starts, to become dos_hpr()
    for (const py::re::Match& m : cast.finditer(code)) {
        int64_t k = close(code, m.start());
        if (k > 0 && py::re::match(S("\\s*->"), code.slice_from(k + 1))) {
            at.add(m.end() - py::len(S("dos_hp(")));
        }
    }
    // (a pointer variable is left as it is: given on, a callee would take the null segment for a place -
    //  a run's fault there names it, and sanitize.low_reads reads through it alone)
    for (int64_t i : py::sorted(at, nullptr, true)) {
        text = text.slice(0, i) + "dos_hpr(" + text.slice_from(i + py::len(S("dos_hp(")));
    }
    return text;
}

py::str declare(const py::str& text0, const py::set<py::str>& olds, const Found& found) {
    str text = text0;
    py::dict<str, py::dict<str, str>> names;
    for (const str& rec : olds) {
        names.setdefault(rec, py::dict<str, str>());
    }
    for (const auto& [key, t] : found.items()) {
        names.setdefault(std::get<0>(key), py::dict<str, str>()).set(std::get<1>(key), t);
    }
    for (const auto& [rec, ms] : names.items()) {
        py::list<str> parts = py::split(rec);   // rec.split()[-1]
        str tag = parts[-1];
        py::list<py::re::Match> found_defs =
            py::re::finditer(S("\\b(?:struct|union)\\s+") + py::re::escape(tag) + S("\\s*\\{"), text);   // (over the text as it is now)
        for (const py::re::Match& m : found_defs) {
            int64_t d = 0, k = m.end() - 1;
            while (k < py::len(text)) {
                char32_t c = text.u32()[static_cast<size_t>(k)];
                d += c == U'{' ? 1 : c == U'}' ? -1 : 0;
                if (d == 0) {
                    break;
                }
                k += 1;
            }
            str body = text.slice(m.end(), k);

            // (struct cse *c_next, *c_prev; - one declaration each)
            body = py::re::sub(
                S("(?m)^([ \\t]*)((?:struct|union)\\s+\\w+|\\w+(?:\\s+\\w+)*?)\\s*(\\**\\s*\\w+(?:\\s*\\[[^\\]]*\\])?"
                  "(?:\\s*,\\s*\\**\\s*\\w+(?:\\s*\\[[^\\]]*\\])?)+)\\s*;"),
                [](const py::re::Match& x) {
                    str base = x.group(2);
                    py::list<str> dcls;
                    for (const str& dd : py::split(x.group(3), S(","))) {
                        dcls.append(dd.strip());
                    }
                    py::list<str> each;
                    for (const str& dd : dcls) {
                        each.append(base + " " + dd + ";");
                    }
                    return x.group(1) + py::join(S(" "), each);   // (on its one line still)
                },
                body);
            for (const auto& [name, t] : ms.items()) {
                body = py::re::sub(
                    S("(?m)(^[ \\t]*|;[ \\t]*|\\*/[ \\t]*)(?:const\\s+)?(?:struct\\s+|union\\s+)?\\w+(?:\\s+\\w+)*\\s*\\*+\\s*") + py::re::escape(name) + S("\\s*;"),
                    [&](const py::re::Match& x) { return x.group(1) + "uint16_t " + name + ";\t/* (a handle: " + t + ") */"; }, body);
            }
            body = py::re::sub(S("(?m)(^[ \\t]*|;[ \\t]*|\\{[ \\t]*)intptr_t(\\s+\\w+\\s*;)"),   // (a word made as wide as a
                               [](const py::re::Match& x) { return x.group(1) + "int16_t" + x.group(2) + "\t/* (one word, as it was) */"; },   //  pointer: one word again, here)
                               body);
            text = text.slice(0, m.end()) + body + text.slice_from(k);
        }
    }
    return text;
}

bool mentions(const py::str& folder, const py::str& f, const py::list<py::str>& tags, const std::optional<py::set<py::str>>& seen0) {
    py::set<str> seen = seen0.has_value() ? *seen0 : py::set<str>();
    if (seen.contains(f)) {
        return false;
    }
    seen.add(f);
    str t;
    try {
        py::file fh = py::open(py::os::path::join(folder, f), S("r"), S("latin1"));
        t = fh.read_text();
        fh.close();
    } catch (const py::OSError&) {
        return false;
    }
    for (const str& g : tags) {
        if (py::re::search(S("\\b") + py::re::escape(g) + S("\\b"), t)) {
            return true;
        }
    }
    for (const str& h : py::re::findall(S("#\\s*include\\s*\"([^\"]+)\""), t)) {
        if (mentions(folder, h, tags, seen)) {
            return true;
        }
    }
    return false;
}

const py::str CODE_VERSION("oldlayout-cpp-1");

std::tuple<py::dict<py::str, py::str>, py::set<py::str>, Found, py::list<std::tuple<py::str, int64_t, std::optional<py::str>>>>
run(const py::str& cc, const py::str& folder, const py::list<py::str>& files) {
    py::set<str> olds;   // (one file's tree at a time: a tree holds the system's
    str hk = arith16::_headers_key(folder);   //  headers too - all of them at once were gigabytes;
    for (const str& f : files) {              //  a file's result kept by its text and its headers')
        str key = arith16::pass_key(cc, folder, f, S("records"), py::list<str>{CODE_VERSION}, hk);
        auto got = base::cache::value<py::list<str>>(S("port-passes"), key);
        if (got.has_value()) {
            olds.update(*got);
            continue;
        }
        std::shared_ptr<Tree> tree = _dump(cc, f, folder);
        py::set<str> mine;
        if (tree) {
            std::optional<str> cur;
            arith16::mark_files(tree->root, cur);
            mine = byte_records(py::list<std::shared_ptr<Tree>>{tree});
            olds.update(mine);
        }
        base::cache::keep(S("port-passes"), key, py::sorted(mine));
        tree.reset();
    }
    py::dict<str, str> out;
    Found found_all;
    py::list<std::tuple<str, int64_t, std::optional<str>>> refused;
    if (!py::truthy(olds)) {
        return {out, olds, found_all, refused};
    }
    py::list<str> tags;
    for (const str& r : olds) {
        tags.append(py::split(r)[-1]);
    }
    // (the keys carry what the pass depends on besides the file: the records and the old-style functions)
    py::list<str> sorted_olds = py::sorted(olds);
    py::list<str> pp;
    for (const str& name : py::sorted(py::list<str>::from(PTR_PARAMS.keys()))) {
        py::list<str> flags;
        for (bool b : PTR_PARAMS.at(name)) {
            flags.append(b ? S("True") : S("False"));
        }
        pp.append(name + ":" + py::join(S(","), flags));
    }
    for (const str& f : files) {
        if (!mentions(folder, f, tags)) {   // (a file that never names them, nor its headers: none)
            continue;
        }
        str key = arith16::pass_key(cc, folder, f, S("oldlayout"),
                                    py::list<str>{CODE_VERSION, py::join(S(","), sorted_olds), py::join(S(";"), pp)}, hk);
        using Got = std::tuple<std::optional<str>, py::list<std::tuple<str, str, str>>, py::list<std::tuple<int64_t, std::optional<str>>>>;
        auto got = base::cache::value<Got>(S("port-passes"), key);
        if (!got.has_value()) {
            std::shared_ptr<Tree> tree = _dump(cc, f, folder);
            if (!tree) {
                continue;
            }
            std::optional<str> cur;
            arith16::mark_files(tree->root, cur);
            str path = f;
            for (const str& v : arith16::_files(tree->root)) {
                if (v.replace(S("\\"), S("/")).endswith(f)) {
                    path = v;
                    break;
                }
            }
            str text;
            {
                py::file fh = py::open(folder + "/" + f, S("r"), S("latin1"));
                text = fh.read_text();
                fh.close();
            }
            auto [e, found, ref] = edits_of(*tree, text, path, olds);
            py::list<std::tuple<str, str, str>> fl;
            for (const auto& [k, v] : found.items()) {
                fl.append(std::make_tuple(std::get<0>(k), std::get<1>(k), v));
            }
            std::optional<str> nw;
            if (py::truthy(e)) {
                nw = oldlayout::apply(text, e);   // (qualified: ADL would find std::apply)
            }
            Got made(nw, fl, ref);
            base::cache::keep(S("port-passes"), key, made);
            got.emplace(made);
            tree.reset();
        }
        for (const auto& [a_, b_, v] : std::get<1>(*got)) {
            found_all.set(std::make_tuple(a_, b_), v);
        }
        for (const auto& [at, op] : std::get<2>(*got)) {
            refused.append(std::make_tuple(f, at, op));
        }
        if (std::get<0>(*got).has_value()) {
            out.set(f, *std::get<0>(*got));
        }
    }
    return {out, olds, found_all, refused};
}

}  // namespace codebreaker::port::oldlayout
