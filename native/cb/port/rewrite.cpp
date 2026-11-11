// cb/port/rewrite.cpp -- see rewrite.hpp.

#include "cb/port/rewrite.hpp"

#include <algorithm>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"

namespace cb::port::pre_ansi {

namespace {

// a cast of type words alone, optional: `(struct s *)`, `(char)`
constexpr std::string_view kArgCast = R"((?:\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)\s*)?)";

std::string trim_end(std::string s) {
    while (!s.empty() && text::is_space(s.back())) s.pop_back();
    return s;
}

std::string_view lstrip(const std::string& s) { return text::ltrim(s); }

std::string where_note(std::string_view file, std::size_t line) { return std::string(file) + ":" + std::to_string(line); }

std::string join_code_lines(const std::vector<std::string>& v, std::size_t from, std::size_t to_inclusive) {
    std::string out;
    for (std::size_t i = from; i <= to_inclusive && i < v.size(); ++i) {
        if (i != from) out += "\n";
        out += v[i];
    }
    return out;
}

}  // namespace

// ---- the preprocessor -----------------------------------------------------------------------------------------------

std::string preprocess(std::string_view text, std::string_view fname, const Profile& profile,
                       const std::set<std::string, std::less<>>& left_out, Notes& notes) {
    static const Regex include_line(R"(\s*#\s*include\s*["<]([^">]+)[">])");
    static const Regex macro_line(R"(\s*#\s*(?:undef|define)\s+(\w+))");
    const std::set<std::string, std::less<>> lib(profile.libc.begin(), profile.libc.end());
    std::vector<std::string> out;
    const std::vector<std::string> lines = split_lines(text);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string& l = lines[i];
        if (const auto m = include_line.match(l)) {
            const std::string header(m->group(1));
            const bool library = std::find(profile.runtime_headers.begin(), profile.runtime_headers.end(), text::lower(header)) !=
                                 profile.runtime_headers.end();
            if (left_out.contains(header) || library) {
                note(notes, "#include of the old library's header -> port.h", where_note(fname, i + 1));
                continue;
            }
        }
        if (const auto m = macro_line.match(l)) {
            if (lib.contains(m->group(1)) || m->group(1) == "FILE") {
                note(notes, "macro of the old library's dropped (the C library of today's stands in)", where_note(fname, i + 1));
                continue;
            }
        }
        out.push_back(l);
    }
    if (fname.ends_with(".c") && std::find(out.begin(), out.end(), "#include \"port.h\"") == out.end())
        out.insert(out.begin(), "#include \"port.h\"");   // (port.h first of all: the program's own headers need its types)
    return join_lines(out);
}

// ---- the definitions ----------------------------------------------------------------------------------------------------

namespace {

bool is_void_type(std::string_view t) {
    t = text::trim(t);
    return t == "void" || t == "static void";
}

// The leading indentation of a line, then `text` with its own indentation dropped
std::string indent_then(const std::string& line, std::string_view prefix) {
    return std::string(indent_of(line)) + std::string(prefix) + std::string(lstrip(line));
}

}  // namespace

RewrittenFile rewrite_definitions(std::string_view text, std::string_view fname, OrderedMap<std::string, Fn>& fns,
                                const ParamTypes& param_types, Notes& notes) {
    static const Regex return_value(R"(\breturn\b\s*[^\s;])");
    static const Regex bare_return(R"(\breturn\s*;)");
    static const Regex char_pointer(R"(\bchar\s*\*)");
    static const Regex call_statement(R"((\w+)\s*\([^;]*\)\s*;)");
    static const Regex assigned_call(R"((\w+)\s*=\s*\w+\s*\([^;]*\)\s*;)");
    static const Regex harmless_statement(
        R"((?:\+\+|--)?\s*[\w.\->\[\]]+\s*(?:\+\+|--)?\s*;|[\w.\->\[\]]+\s*=\s*(?:\d+|0x[0-9a-fA-F]+)\s*;)");
    static const Regex block_close(R"(\}\s*(?:else\b.*)?)");
    static const Regex else_word(R"(\belse\b)");
    static const Regex if_or_else(R"(\s*(?:if|else)\b)");
    static const Regex assignment_of_call(R"([\w.\->\[\]]+\s*=\s*\w+\s*\([^;]*\)\s*;)");
    static const Regex integer_words(
        R"(\b(?:unsigned\s+int|unsigned(?!\s+(?:char|long|short))|short|int)\b(?!\s*\*))");

    std::vector<std::string> lines = split_lines(text);
    const std::vector<std::string> code = split_lines(base::ctext::blank(text));
    const std::vector<Definition> defs = definitions(lines, code);
    const std::vector<int> depth = brace_depths(code);
    RewrittenFile result;
    for (const Definition& d : defs) result.functions.push_back(d.fn.name);
    for (auto d = defs.rbegin(); d != defs.rend(); ++d) {
        const Fn& f = d->fn;
        const std::size_t s = d->first, j = d->open, k = d->last;
        fns.try_emplace(f.name, f);   // (a function the program lists but the first pass did not: added)
        Fn* g = fns.find(f.name);
        g->params = f.params;
        g->is_static = f.is_static;
        g->implicit = f.implicit;
        g->ret = ((g->keep_int || g->ret_set) && !g->ret.empty()) ? g->ret : f.ret;
        for (auto pt = param_types.lower_bound({f.name, 0}); pt != param_types.end() && pt->first.first == f.name; ++pt)
            if (pt->first.second < g->params.size()) g->params[pt->first.second].type = pt->second;
        std::string body = join_code_lines(code, j, k);
        std::optional<Param> last;
        if (g->params.size() >= 2) last = g->params.back();
        std::optional<RegexMatch> walk;
        std::optional<Regex> walk_rx;
        if (last && body.find(last->name) != std::string::npos) {
            walk_rx.emplace(R"(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?&\s*)" + regex_quote(last->name) + R"(\s*;)");
            walk = walk_rx->search(body);
        }
        // (a function that walks its own arguments from the place of the first, by a format of its own -- "PNTPN":
        //  ap = &args; *ap; ap++ -- each argument a word (two a long) then: here a slot each, read from the variable
        //  arguments; its callers give each as an intptr_t)
        if (walk && char_pointer.search(g->params[g->params.size() - 2].type)) {
            const std::string ap(walk->group(1));
            const Regex ap_is_int_pointer(R"(\b(?:int|short|int16_t|unsigned)\s*\*\s*)" + regex_quote(ap) + R"(\s*;)");
            const Regex address_passed(R"([(,]\s*)" + std::string(kArgCast) + "&\\s*" + regex_quote(last->name) + R"(\s*[,)])");
            if (ap_is_int_pointer.search(body) && !address_passed.search(body)) {
                const std::string prev_name = g->params[g->params.size() - 2].name;
                g->varargs = true;
                g->va_slots = true;
                g->loose = false;
                g->params.pop_back();
                const Regex take_address(R"(\b)" + regex_quote(ap) + R"(\s*=\s*(?:\([^()]*\)\s*)?&\s*)" + regex_quote(last->name) + R"(\s*;)");
                const Regex int_pointer_decl(R"(\b(?:int|short|int16_t|unsigned)(\s*\*\s*)" + regex_quote(ap) + R"(\s*;))");
                const Regex long_read(R"(\*\s*\(\s*long\s*\*\s*\)\s*)" + regex_quote(ap) + R"(\b)");
                const Regex step_two(R"(\b)" + regex_quote(ap) + R"(\s*\+=\s*2\s*;)");
                for (std::size_t x = j; x <= k; ++x) {
                    std::string l = lines[x];
                    l = take_address.replace_all(l, ap + " = dos_args_;");
                    l = int_pointer_decl.replace_all(l, "intptr_t$1");
                    l = long_read.replace_all(l, "((int32_t)*" + ap + ")");
                    l = step_two.replace_all(l, ap + " += 1;");
                    lines[x] = l;
                }
                lines[j] = trim_end(lines[j]) + " DOS_VA_ARGS(dos_args_, " + prev_name + ");";
                note(notes,
                     "function walking its own arguments from the place of the first (by a format of its own): the variable "
                     "arguments, a slot each (DOS_VA_ARGS)",
                     where_note(fname, s + 1) + " (" + f.name + ")");
                body = join_code_lines(split_lines(base::ctext::blank(join_lines(lines))), j, k);
                last.reset();
            }
        }
        // (the old way of a function of any number of arguments: the place of the first of them given on -- they
        //  followed it on the stack, a word each; here they are put in words of their own)
        if (last) {
            const Regex address_passed(R"([(,]\s*)" + std::string(kArgCast) + "&\\s*" + regex_quote(last->name) + R"(\s*[,)])");
            const Regex declared_int(R"(^(?:register\s+)?(?:int|short|unsigned|int16_t|intptr_t)\s+)" + regex_quote(last->name) + "$");
            if (address_passed.search(body) && declared_int.search(text::trim(last->type)) &&
                char_pointer.search(g->params[g->params.size() - 2].type)) {
                const std::string prev_name = g->params[g->params.size() - 2].name;
                g->varargs = true;
                g->loose = false;
                g->params.pop_back();
                const Regex arg_only(R"(([(,]\s*)" + std::string(kArgCast) + R"()&\s*)" + regex_quote(last->name) + R"(\b(?=\s*[,)]))");
                for (std::size_t x = j; x <= k; ++x)
                    if (arg_only.search(code[x]))
                        lines[x] = arg_only.replace_all(lines[x], [&](const RegexMatch& mm) { return std::string(mm.group(1)) + last->name; });
                lines[j] = trim_end(lines[j]) + " DOS_VA_WORDS(" + last->name + ", " + prev_name + ");";
                note(notes,
                     "function of any number of arguments the old way (the place of the first given on): ..., its arguments put "
                     "in words (DOS_VA_WORDS, by its format)",
                     where_note(fname, s + 1) + " (" + f.name + ")");
                body = join_code_lines(split_lines(base::ctext::blank(join_lines(lines))), j, k);
            }
        }
        g->body = body;
        const bool valued = return_value.search(body).has_value();
        std::size_t bare = 0;
        bare_return.for_each_match(body, [&](const RegexMatch&) {
            ++bare;
            return true;
        });
        if (f.name == "main" && g->ret.empty()) g->ret = "int";
        g->returns_void = !valued && g->ret.empty() && f.name != "main" && !g->keep_int;
        const std::string ret_or_int = g->ret.empty() ? "int" : g->ret;
        if (!valued && bare == 0 && (!g->ret.empty() || g->keep_int) && !is_void_type(ret_or_int) && f.name != "main") {
            // (a function that gives a value, falling off its end: what the register held -- the value of its last
            //  assignment of a call's, when nothing after it gives the register another)
            std::vector<std::size_t> stmts;
            for (std::size_t x = j + 1; x < k; ++x)
                if (!text::trim(code[x]).empty()) stmts.push_back(x);
            std::optional<std::pair<std::size_t, std::string>> last_assign;
            std::optional<RegexMatch> tail;
            if (!stmts.empty()) tail = call_statement.full_match(text::trim(code[stmts.back()]));
            const Fn* callee = tail ? fns.find(std::string(tail->group(1))) : nullptr;
            if (callee != nullptr) {
                const std::string callee_ret = callee->ret.empty() ? "int" : callee->ret;
                if (!text::trim(callee_ret).empty() && text::trim(callee_ret) != "void" && !callee->returns_void &&
                    depth[stmts.back()] == depth[j] + 1) {
                    // (its last statement a call of one that gives a value: what that gave was in the register -- so given)
                    const std::size_t x = stmts.back();
                    lines[x] = indent_then(lines[x], "return ");
                    note(notes, "function giving a value that fell off its end after a call (the value in the register): the call's value returned",
                         where_note(fname, x + 1) + " (" + f.name + ")");
                    stmts.clear();
                }
            }
            for (auto x = stmts.rbegin(); x != stmts.rend(); ++x) {
                const std::string_view st = text::trim(code[*x]);
                if (const auto a = assigned_call.full_match(st)) {
                    last_assign = {*x, std::string(a->group(1))};
                    break;
                }
                if (!harmless_statement.full_match(st)) break;
            }
            if (last_assign && find_word(join_code_lines(code, j, last_assign->first - 1), last_assign->second) != std::string::npos &&
                last_assign->first > j) {
                lines[k] = std::string(indent_of(lines[last_assign->first])) + "return " + last_assign->second +
                           ";\t\t/* (what the register held: the old function fell off its end) */\n" + lines[k];
                note(notes, "function giving a value that fell off its end (the value in the register): its last value returned",
                     where_note(fname, k + 1) + " (" + f.name + ")");
            }
        }
        if (valued && !is_void_type(ret_or_int) && f.name != "main") {
            // (a function that gives a value but whose last statement, an if-else, has a branch falling off its end
            //  after x = f(...): what the register held then was that value -- given)
            std::vector<int> at;
            int dep = 0;
            for (std::size_t x = j; x <= k; ++x) {
                at.push_back(dep);
                for (char c : code[x]) dep += (c == '{') - (c == '}');
            }
            const int D = at.size() > 1 ? at[1] : 1;   // (the body's depth)
            std::vector<std::size_t> body_lines, closes;
            for (std::size_t x = j + 1; x < k; ++x)
                if (!text::trim(code[x]).empty()) body_lines.push_back(x);
            for (const std::size_t x : body_lines)
                if (text::trim(code[x]).starts_with('}') && at[x - j] == D + 1 && block_close.full_match(text::trim(code[x]))) closes.push_back(x);
            std::string last_at_depth;
            bool any_at_depth = false;
            for (const std::size_t x : body_lines)
                if (at[x - j] == D) {
                    last_at_depth = code[x];
                    any_at_depth = true;
                }
            bool any_else = false;
            for (const std::size_t c : closes)
                if (else_word.search(code[c])) any_else = true;
            if (!closes.empty() && !body_lines.empty() && closes.back() == body_lines.back() && any_at_depth &&
                if_or_else.match(last_at_depth) && any_else) {
                for (const std::size_t c : closes) {
                    std::optional<std::size_t> prev;
                    for (const std::size_t x : body_lines)
                        if (x < c) prev = x;
                    if (!prev || at[*prev - j] != D + 1) continue;
                    const std::string_view st = text::trim(code[*prev]);
                    if (assignment_of_call.full_match(st) && !st.starts_with("return")) {
                        lines[*prev] = indent_then(lines[*prev], "return ");
                        note(notes,
                             "function giving a value whose branch fell off its end after an assignment of a call (the value in the register): that value returned",
                             where_note(fname, *prev + 1) + " (" + f.name + ")");
                    }
                }
            }
        }
        if (bare != 0 && (valued || !g->returns_void) && text::trim(ret_or_int) != "void") {   // (or an int to its callers)
            g->bare = bare;
            for (std::size_t x = j; x <= k; ++x) {
                if (bare_return.search(code[x])) {
                    lines[x] = bare_return.replace_all(lines[x], "return 0;");
                    note(notes, "bare return in a function that gives a value elsewhere: return 0 (what the register held is not known)",
                         where_note(fname, x + 1));
                }
            }
        }
        if (g->loose) {   // (the old-style definition kept -- its calls unchecked: what it gives as its callers say --
                          //  an int when it never said -- and its integers)
            if (g->ret.empty()) g->ret = g->returns_void ? "void" : "int";
            const std::string sep = g->ret.ends_with('*') ? "" : " ";
            const std::string storage = g->is_static ? "static " : "";
            if (const auto here = call_starts(lines[s], g->name); !here.empty()) {
                lines[s] = storage + g->ret + sep + lines[s].substr(here.front());
            } else if (s + 1 < lines.size()) {
                if (const auto next = call_starts(lines[s + 1], g->name); !next.empty()) {
                    lines[s] = "";   // (the type on the line before: the head on one line)
                    lines[s + 1] = storage + g->ret + sep + lines[s + 1].substr(next.front());
                }
            }
            for (std::size_t x = s + 1; x < j; ++x)
                if (text::trim(lines[x]).ends_with(';')) lines[x] = integer_words.replace_all(lines[x], "intptr_t");
            std::vector<std::string> undeclared;
            for (const Param& p : g->params)
                if (g->implicit.contains(p.name)) undeclared.push_back(p.name);
            if (!undeclared.empty() && !lstrip(lines[j]).starts_with("intptr_t"))   // (never declared: old C's int -- here intptr_t)
                lines[j] = "intptr_t " + text::join(undeclared, ", ") + "; " + std::string(lstrip(lines[j]));
            note(notes, "old-style definition kept: its integer parameters intptr_t (the callers may give pointers)",
                 where_note(fname, s + 1) + " (" + g->name + ")");
            continue;
        }
        note(notes, "old-style definition -> prototype", where_note(fname, s + 1));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(s), lines.begin() + static_cast<std::ptrdiff_t>(j));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(s), g->proto());
    }
    std::vector<const Fn*> statics;   // (the file's own functions: declared at its head)
    for (const Definition& d : defs)
        if (d.fn.is_static) statics.push_back(fns.find(d.fn.name));
    if (!statics.empty()) {
        std::size_t at = 0;
        for (std::size_t i = 0; i < lines.size(); ++i)
            if (lines[i].starts_with("#include")) at = i + 1;
        std::vector<std::string> protos;
        for (const Fn* fn : statics) protos.push_back(fn->proto() + ";");
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), protos.begin(), protos.end());
    }
    result.text = join_lines(lines);
    return result;
}

// ---- local retypes --------------------------------------------------------------------------------------------------------

std::string apply_local_retypes(std::string_view text, std::string_view fname, const std::vector<LocalRetype>& retypes) {
    struct Mine {
        const LocalRetype* r;
        Regex plain_decl, list_decl, in_prototype, after_comma;
        std::string spaced;   // the type and the space before the name
    };
    std::vector<Mine> mine;
    for (const LocalRetype& r : retypes) {
        if (r.file != fname) continue;
        const std::string v = regex_quote(r.variable);
        mine.push_back({&r,
                        Regex(R"((?m)^(\s*(?:register\s+)?)(?:u?int16_t|int|unsigned)\s+)" + v + R"(\s*;)"),
                        Regex(R"((?m)^(\s*(?:register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*))" + v + R"(\s*((?:,\s*\w+\s*)*);)"),
                        Regex(R"(\((?:u?int16_t|int|unsigned)\s+)" + v + R"(\s*([,)]))"),
                        Regex(R"(,\s*(?:u?int16_t|int|unsigned)\s+)" + v + R"(\s*([,)]))"),
                        r.type + (r.type.ends_with('*') ? "" : " ")});
    }
    if (mine.empty()) return std::string(text);
    std::vector<std::string> lines = split_lines(text);
    const std::vector<std::string> code = split_lines(base::ctext::blank(text));
    struct Span4 {
        std::string name;
        std::size_t first, last;
    };
    std::vector<Span4> defs;
    for (const NamedDefinition& d : definitions_any(lines, code)) defs.push_back({d.name, d.first, d.last});
    for (const Definition& d : definitions(lines, code)) defs.push_back({d.fn.name, d.first, d.last});
    std::stable_sort(defs.begin(), defs.end(), [](const Span4& a, const Span4& b) { return a.first > b.first; });   // (new style and old)
    for (const Span4& d : defs) {
        for (const Mine& m : mine) {
            if (m.r->function != d.name) continue;
            if (d.last >= lines.size()) continue;
            std::string body = join_code_lines(lines, d.first, d.last);
            const std::string& var = m.r->variable;
            body = m.plain_decl.replace_all(body, [&](const RegexMatch& x) { return std::string(x.group(1)) + m.spaced + var + ";"; });
            body = m.list_decl.replace_all(body, [&](const RegexMatch& x) { return SplitDecl{var, m.r->type}(x); });
            body = m.in_prototype.replace_all(body, [&](const RegexMatch& x) { return "(" + m.spaced + var + std::string(x.group(1)); });
            body = m.after_comma.replace_all(body, [&](const RegexMatch& x) { return ", " + m.spaced + var + std::string(x.group(1)); });
            const std::vector<std::string> now = split_lines(body);
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(d.first), lines.begin() + static_cast<std::ptrdiff_t>(d.last + 1));
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(d.first), now.begin(), now.end());
        }
    }
    return join_lines(lines);
}

// ---- finish ---------------------------------------------------------------------------------------------------------------

namespace {

std::string null_read(const std::string& text, const NullRead& r) {
    const std::string var = regex_quote(r.var);
    auto wrap = [&](const RegexMatch& m) { return "((" + r.type + ")dos_np(" + std::string(m.group(1)) + "))"; };
    switch (r.kind) {
        case NullRead::Kind::Var:   // (p[0]->op, p[0] a small number at times, or l->op, l a NULL: read through the null segment there)
            return base::ctext::sub_code(text, Regex(R"((?<![\w.>])()" + var + R"()(?=\s*->))"), wrap);
        case NullRead::Kind::Assign:   // ((q = f06df2(p))->kind, NULL at the list's end)
            return base::ctext::sub_code(text, Regex(R"(\(\s*()" + var + R"(\s*=\s*\w+\s*\([^()]*\))\s*\)(?=\s*->))"), wrap);
        case NullRead::Kind::Mem:   // (p->next->prev = ..., the last's next NULL: its link read through the null segment)
            return base::ctext::sub_code(
                text,
                Regex(R"((?<![\w.>])((?:\(\s*\([^()]*\)\s*dos_np\(\s*\w+\s*\)\s*\)|\w+)\s*->\s*)" + var + R"()(?=\s*->))"), wrap);
        case NullRead::Kind::Plain:
            break;
    }
    return base::ctext::sub_code(text, Regex(R"((?<![\w.>])()" + var + R"(\s*\[[^\[\]]*\])(?=\s*->))"), wrap);
}

}  // namespace

Finished finish(const FinishRules& rules, std::string_view fname, std::string_view input) {
    static const Regex include_prefix(R"([ \t]*#\s*include\b)");
    std::string text(input);
    if (!rules.renames.empty()) {   // names that meet the C library's renamed
        std::string alternatives;
        for (const auto& kv : rules.renames) alternatives += (alternatives.empty() ? "" : "|") + regex_quote(kv.first);
        const Regex rx(R"(\b()" + alternatives + R"()\b)");
        const std::string original = text;
        text = base::ctext::sub_code(original, rx, [&](const RegexMatch& m) {
            const std::size_t line_start = m.start() == 0 ? 0 : (original.rfind('\n', m.start() - 1) == std::string::npos ? 0 : original.rfind('\n', m.start() - 1) + 1);
            const std::string_view prefix = std::string_view(original).substr(line_start, m.start() - line_start);
            if (!include_prefix.match(prefix)) return rules.renames.at(std::string(m.group(1)));
            return std::string(m.group(0));   // (#include <setjmp.h>: the header's own name)
        });
    }
    for (const DeclRule& r : rules.decl_rules) text = r.apply(text);
    text = apply_local_retypes(text, fname, rules.local_retypes);
    if (!rules.array_sizes.empty()) {   // (extern T a[]; -- its size, where it is defined)
        std::string alternatives;
        for (const auto& kv : rules.array_sizes) alternatives += (alternatives.empty() ? "" : "|") + regex_quote(kv.first);
        const Regex rx(R"((?m)^(\s*extern\b[^;(){}]*?\b()" + alternatives + R"())\s*\[\s*\])");
        text = rx.replace_all(text, [&](const RegexMatch& m) {
            return std::string(m.group(1)) + "[" + std::to_string(rules.array_sizes.at(std::string(m.group(2)))) + "]";
        });
    }
    // the lines mended: a rule for the n-th line of that text (two alike lines in one file are two places)
    std::map<std::string, std::pair<std::optional<std::string>, std::map<std::size_t, std::string>>> fixes;
    for (auto it = rules.line_fixes.lower_bound(LineFixKey{std::string(fname), "", std::nullopt});
         it != rules.line_fixes.end() && it->first.file == fname; ++it) {
        auto& entry = fixes[it->first.text];
        if (it->first.nth) {
            entry.second[*it->first.nth] = it->second;
        } else {
            entry.first = it->second;
        }
    }
    Finished result;
    std::vector<std::string> out;
    std::map<std::string, std::size_t> seen;
    for (const std::string& l : split_lines(text)) {
        const std::size_t nth = seen[l]++;
        std::string fixed = l;
        if (const auto f = fixes.find(l); f != fixes.end()) {
            if (const auto n = f->second.second.find(nth); n != f->second.second.end()) {
                fixed = n->second;
            } else if (f->second.first) {
                fixed = *f->second.first;
            }
        }
        out.push_back(std::move(fixed));
        result.origin.push_back({l, nth});
    }
    text = join_lines(out);
    for (const NullRead& r : rules.null_reads)   // (on the lines as mended: their rules are by the text before)
        if (r.file == fname) text = null_read(text, r);
    result.text = std::move(text);
    return result;
}

}  // namespace cb::port::pre_ansi
