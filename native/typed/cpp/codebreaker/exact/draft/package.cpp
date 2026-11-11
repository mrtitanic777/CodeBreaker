// codebreaker/exact/draft/package.cpp -- see package.hpp.

#include "codebreaker/exact/draft/package.hpp"

#include <algorithm>
#include <set>

#include "codebreaker/exact/draft/text.hpp"
#include "codebreaker/exact/x86.hpp"

namespace codebreaker::exact::draft {

namespace {

py::str ps(const char* utf8) { return py::str(utf8); }

py::str lstrip_underscores(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    while (i < u.size() && u[i] == U'_') ++i;
    return py::str(u.substr(i));
}

py::str hex_lower(int64_t v) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}

bool has_prefix(const py::str& s, const char* p) { return s.startswith(py::str(p)); }

// re.search(rb"\xcd[\x34-\x3d]", code)
bool has_emulator_call(const py::bytes& code) {
    const std::string& r = code.raw();
    for (size_t i = 0; i + 1 < r.size(); ++i)
        if (static_cast<unsigned char>(r[i]) == 0xCD && static_cast<unsigned char>(r[i + 1]) >= 0x34 && static_cast<unsigned char>(r[i + 1]) <= 0x3D) return true;
    return false;
}

// re.search(rb"\x9b[\xd8-\xdf]", raw)
bool has_fwait_esc(const py::bytes& raw) {
    const std::string& r = raw.raw();
    for (size_t i = 0; i + 1 < r.size(); ++i)
        if (static_cast<unsigned char>(r[i]) == 0x9B && static_cast<unsigned char>(r[i + 1]) >= 0xD8 && static_cast<unsigned char>(r[i + 1]) <= 0xDF) return true;
    return false;
}

// the statements without the model's "register variables" comment
py::list<py::str> without_regvar_comments(const py::list<py::str>& stmts) {
    py::list<py::str> out;
    for (const py::str& s : stmts)
        if (!has_prefix(s, "/* register variables")) out.append(s);
    return out;
}

// a register parameter's load is not a statement: `reg = p;` in the first three, dropped
py::list<py::str> without_param_loads(const py::list<py::str>& stmts, const py::str& reg, const py::str& p) {
    py::list<py::str> out;
    int64_t k = 0;
    for (const py::str& s : stmts) {
        if (!(k < 3 && py::re::fullmatch(py::percent(ps("%s = %s;"), reg, p), s.strip()))) out.append(s);
        ++k;
    }
    return out;
}

// sorted(set(params) | {p})
py::list<py::str> with_param(const py::list<py::str>& params, const py::str& p) {
    std::set<std::u32string> all;
    for (const py::str& x : params) all.insert(x.u32());
    all.insert(p.u32());
    py::list<py::str> out;
    for (const auto& s : all) out.append(py::str(s));
    return out;
}

// str(e).splitlines()[0]: the text up to its first line break (Python's: \n \r \x0b \x0c \x1c-\x1e \x85    )
py::str first_line(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    while (i < u.size() && !(u[i] == U'\n' || u[i] == U'\r' || u[i] == 0x0B || u[i] == 0x0C || (u[i] >= 0x1C && u[i] <= 0x1E) || u[i] == 0x85 ||
                             u[i] == 0x2028 || u[i] == 0x2029))
        ++i;
    return py::str(u.substr(0, i));
}

py::str param_name(int64_t ix) { return py::str(py::str(char32_t('a' + ix))); }       // chr(ord("a") + ix)

}  // namespace

FpInfo FPINFO;
py::list<int64_t> FRAME{0};
bool flow16_cmpswitch = false;

// ---- table_hook ----------------------------------------------------------------------------------------------------------------
struct TableHook::Impl {
    py::list<x86::Insn> ins;
};

TableHook::TableHook(const py::bytes& code) : code_(code), impl_(std::make_shared<Impl>()) {
    impl_->ins = x86::disasm(code, 0, code.size(), false);
}

std::optional<py::list<int64_t>> TableHook::operator()(int64_t addr) const {
    const py::list<x86::Insn>& ins = impl_->ins;
    std::optional<int64_t> k;
    for (int64_t j = 0; j < ins.size(); ++j)
        if (ins[j].address == addr) {
            k = j;
            break;
        }
    if (!k) return std::nullopt;
    auto m = py::re::search(ps("cs:\\[bx ([+-]) (0x[0-9a-f]+)\\]"), ins[*k].op_str);
    std::optional<int64_t> n;
    // next((int(x.group(1), 0) for i in reversed(ins[max(0, k - 8):k]) for x in [re.fullmatch(...)] if i.mnemonic == "cmp" and x), None)
    for (int64_t j = *k - 1; j >= std::max<int64_t>(0, *k - 8); --j) {
        auto x = py::re::fullmatch(ps("\\w+, (0x[0-9a-f]+|\\d+)"), ins[j].op_str);
        if (ins[j].mnemonic == ps("cmp") && x) {
            n = py::int_parse(x->group(1), 0);
            break;
        }
    }
    if (!m || !n) return std::nullopt;
    int64_t v = py::int_parse(m->group(2), 16);
    int64_t t = (((m->group(1) == ps("+")) ? v : -v) & 0xFFFF) - base;
    if (!(0 <= t && t + 2 * (*n + 1) <= code_.size())) return std::nullopt;
    py::list<int64_t> out;
    for (int64_t j = 0; j <= *n; ++j) {
        // struct.unpack_from("<H", code, t + 2 * j)[0] - hook.base
        int64_t at = t + 2 * j;
        if (at + 2 > code_.size()) throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(at + 2) + " bytes for unpacking 2 bytes at offset " + std::to_string(at) +
                                                         " (actual buffer size is " + std::to_string(code_.size()) + ")");
        out.append((code_[at] | (code_[at + 1] << 8)) - base);
    }
    return out;
}

std::shared_ptr<TableHook> table_hook(const py::bytes& code) { return std::make_shared<TableHook>(code); }

// ---- fp_native -------------------------------------------------------------------------------------------------------------------
py::bytes fp_native(const py::bytes& code) {
    std::string b = code.raw();
    if (!has_emulator_call(code)) return py::bytes(code.raw());
    for (const x86::Insn& i : x86::disasm(code, 0, code.size(), true)) {
        const std::string& raw = i.bytes.raw();
        if (raw.empty() || static_cast<unsigned char>(raw[0]) != 0xCD || raw.size() < 2 || !(0x34 <= static_cast<unsigned char>(raw[1]) && static_cast<unsigned char>(raw[1]) <= 0x3D)) continue;
        int64_t a = i.address;
        auto set2 = [&](int64_t at, const std::string& v) {      // b[a:a + n] = v (the same length)
            for (size_t q = 0; q < v.size() && static_cast<size_t>(at) + q < b.size(); ++q) b[static_cast<size_t>(at) + q] = v[q];
        };
        unsigned char r1 = static_cast<unsigned char>(raw[1]);
        if (r1 <= 0x3B) {
            set2(a, std::string({static_cast<char>(0x9B), static_cast<char>(0xD8 + r1 - 0x34)}));
        } else if (r1 == 0x3D) {
            set2(a, std::string({static_cast<char>(0x9B), static_cast<char>(0x90)}));
        } else if (raw.size() >= 3) {
            unsigned char r2 = static_cast<unsigned char>(raw[2]);
            int seg = (r2 & 0xC0) == 0x00 ? 0x26 : (r2 & 0xC0) == 0x40 ? 0x2E : (r2 & 0xC0) == 0x80 ? 0x36 : 0x3E;
            set2(a, std::string({static_cast<char>(0x9B), static_cast<char>(seg), static_cast<char>(0xD8 | (r2 & 7))}));
        }
    }
    return py::bytes(b);
}

// ---- inline_goto_arms ------------------------------------------------------------------------------------------------------------
py::list<py::str> inline_goto_arms(const py::list<py::str>& lines_in) {
    std::vector<py::str> lines(lines_in.vec().begin(), lines_in.vec().end());
    for (int round = 0; round < 8; ++round) {
        struct Hit {
            size_t k;
            py::str indent;
            std::vector<py::str> body;
            py::str label;
            size_t at;
        };
        std::optional<Hit> hit;
        for (size_t k = 0; k < lines.size(); ++k) {
            auto g = py::re::fullmatch(ps("(\\s*)goto (L_\\w+);"), lines[k]);
            if (!g || k == 0 || !py::re::search(ps("\\{\\s*$"), lines[k - 1]) || k + 1 >= lines.size() || !lines[k + 1].strip().startswith(ps("}"))) continue;
            std::optional<size_t> at;
            for (size_t j = 0; j < lines.size(); ++j)
                if (lines[j].strip() == g->group(2) + ps(":")) {
                    at = j;
                    break;
                }
            if (!at) continue;
            int64_t ind = lines[*at].size() - lines[*at].lstrip().size();
            size_t j = *at + 1;
            int64_t depth = 0;
            while (j < lines.size()) {                           // (to the end of the label's block)
                if (depth == 0 && lines[j].strip().startswith(ps("}"))) break;
                depth += lines[j].count(ps("{")) - lines[j].count(ps("}"));
                ++j;
            }
            std::vector<py::str> tail(lines.begin() + static_cast<long>(*at) + 1, lines.begin() + static_cast<long>(j));
            bool label_in_tail = false;
            for (const py::str& x : tail)
                if (py::re::fullmatch(ps("\\s*L_\\w+:"), x)) label_in_tail = true;
            if (tail.empty() || tail.size() > 40 || label_in_tail || (*at < k && k < j)) continue;
            Hit h;
            h.k = k;
            h.indent = g->group(1);
            for (const py::str& x : tail) h.body.push_back(x.strip().empty() ? x : g->group(1) + x.slice(ind, x.size()));
            h.label = g->group(2);
            h.at = *at;
            hit = h;
            break;
        }
        if (!hit) break;
        lines.erase(lines.begin() + static_cast<long>(hit->k));
        lines.insert(lines.begin() + static_cast<long>(hit->k), hit->body.begin(), hit->body.end());
        bool used = false;
        for (const py::str& x : lines)
            if (py::re::search(ps("\\bgoto ") + hit->label + ps(";"), x)) used = true;
        if (!used) {
            std::vector<py::str> kept;
            for (const py::str& x : lines)
                if (!(x.strip() == hit->label + ps(":"))) kept.push_back(x);
            lines = kept;
        }
    }
    return py::list<py::str>(lines);
}

// ---- model_texts -----------------------------------------------------------------------------------------------------------------
py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> model_texts(InverseModel& inv, const py::bytes& code_in, const py::str& name,
                                                                      std::optional<int64_t> base) {
    using Text = std::tuple<py::str, py::dict<py::str, int64_t>>;
    py::bytes raw = code_in, code = fp_native(code_in);
    if (inv.FRAME_AFTER_BP) inv.FRAME_AFTER_BP = true;            // (a draft is of 3.00 or later code: its -Gs frames,
    if (inv.REGVARS_EAGER) inv.REGVARS_EAGER = true;              //  its register variables)
    py::list<Text> out;
    Recovery rec;
    try {
        rec = inv.recover(code);
    } catch (const py::Exception&) {
        if (raw == code) throw;
        rec = inv.recover(raw);                                   // (the linear walk lost in the 8087's code: as it was)
    }
    FRAME[0] = rec.fs;                                            // (the frame's size: fill_frame)
    py::dict<py::str, int64_t> regparam = inv.LAST.regparam.copy();
    out.append(Text(inv.to_c(name, rec.fs, rec.params, rec.stmts, rec.pt, rec.loc), regparam));
    if (inv.has_recover_cfg) {
        flow16_cmpswitch = true;                                  // (switches by compares read as switches)
        bool old_has = inv.has_switch;
        SwitchHook old = inv.SWITCH;
        if (base && inv.has_switch) {                             // (and by a table: where it sends each value)
            std::shared_ptr<TableHook> hook = table_hook(code);
            hook->base = *base;
            inv.SWITCH = [hook](int64_t addr) { return (*hook)(addr); };
        }
        Recovery cfg;
        try {
            cfg = inv.recover_cfg(code);
        } catch (const py::Exception&) {
            try {
                _fpinfo(inv, raw, code);               // (the except body runs before the finally)
            } catch (...) {
                flow16_cmpswitch = false;
                if (inv.has_switch) inv.SWITCH = old;
                throw;
            }
            flow16_cmpswitch = false;
            if (inv.has_switch) inv.SWITCH = old;
            return out;
        } catch (...) {
            flow16_cmpswitch = false;
            if (inv.has_switch) inv.SWITCH = old;
            throw;
        }
        flow16_cmpswitch = false;
        if (inv.has_switch) inv.SWITCH = old;
        (void)old_has;
        py::list<py::str> stmts = without_regvar_comments(cfg.stmts);
        py::list<py::str> params = cfg.params;
        for (const auto& kv : regparam.items()) {                 // (a register parameter's load is not a statement)
            py::str p = param_name(std::get<1>(kv));
            stmts = without_param_loads(stmts, std::get<0>(kv), p);
            if (!params.contains(p)) params = with_param(params, p);
        }
        py::str t = inv.to_c(name, cfg.fs, params, stmts, cfg.pt, cfg.loc);
        if (t != std::get<0>(out[0])) out.append(Text(t, regparam));
        bool any_fp_place = false;
        for (const py::str& k : inv.LAST.fptypes.keys())
            if (py::re::search(ps("->|\\["), k)) any_fp_place = true;
        if (inv.FPCASTS && any_fp_place) {
            inv.FPCASTS = true;                                   // (the 8087's fields at their own widths: read again with
            flow16_cmpswitch = true;                              //  casts - a union's place used both ways)
            try {
                Recovery r2 = inv.recover_cfg(code);
                py::list<py::str> stmts2 = without_regvar_comments(r2.stmts);
                py::list<py::str> params2 = r2.params;
                for (const auto& kv : regparam.items()) {
                    py::str p = param_name(std::get<1>(kv));
                    stmts2 = without_param_loads(stmts2, std::get<0>(kv), p);
                    if (!params2.contains(p)) params2 = with_param(params2, p);
                }
                py::str t2 = inv.to_c(name, r2.fs, params2, stmts2, r2.pt, r2.loc);
                bool all_differ = true;
                for (const Text& x : out)
                    if (!(t2 != std::get<0>(x))) all_differ = false;
                if (all_differ) out.append(Text(t2, regparam));
            } catch (const py::Exception&) {
            }
            inv.FPCASTS = false;
            flow16_cmpswitch = false;
        }
        // (a long compared the compiler's way - the high words, then the low words without sign - three nested ifs: read
        //  again with the unsigned compares marked, and the ladders folded into one long comparison)
        py::list<x86::Insn> ins = x86::disasm(code, 0, code.size(), false);     // (or a compare without sign: the text with it marked)
        bool unsigned_ = false;
        for (int64_t q = 0; q + 1 < ins.size(); ++q) {
            const x86::Insn& a_ = ins[q];
            const x86::Insn& b_ = ins[q + 1];
            if (a_.mnemonic == ps("cmp") && (b_.mnemonic == ps("jb") || b_.mnemonic == ps("jbe") || b_.mnemonic == ps("ja") || b_.mnemonic == ps("jae"))) {
                // "bp" not in a_.op_str.split(",")[1]
                const std::u32string& u = a_.op_str.u32();
                size_t comma = u.find(U',');
                if (comma == std::u32string::npos) throw py::IndexError("list index out of range");
                size_t comma2 = u.find(U',', comma + 1);
                std::u32string second = u.substr(comma + 1, comma2 == std::u32string::npos ? std::u32string::npos : comma2 - comma - 1);
                if (second.find(U"bp") == std::u32string::npos) {
                    unsigned_ = true;
                    break;
                }
            }
        }
        if (inv.has_fold_ladders &&
            (unsigned_ || py::re::search(ps("if \\(\\((\\w+) [<>]= [^)]*\\)\\) \\{\\n\\s*if \\(\\(\\1 [<>]= "), t))) {
            if (!inv.UNSIGNED_CAST) throw py::AttributeError("module '" + inv.module_name().utf8() + "' has no attribute 'UNSIGNED_CAST'");
            std::optional<bool> old_unsigned = inv.UNSIGNED_CAST;
            try {
                inv.UNSIGNED_CAST = true;
                flow16_cmpswitch = true;
                Recovery r2 = inv.recover_cfg(code);
                py::list<py::str> stmts2 = without_regvar_comments(inv._fold_ladders(inline_goto_arms(r2.stmts)));
                for (const auto& kv : regparam.items()) stmts2 = without_param_loads(stmts2, std::get<0>(kv), param_name(std::get<1>(kv)));
                py::str t2 = inv.to_c(name, r2.fs, params, stmts2, r2.pt, r2.loc);
                if (t2 != t) out.append(Text(t2, regparam));
            } catch (const py::Exception&) {
            }
            inv.UNSIGNED_CAST = old_unsigned;
            flow16_cmpswitch = false;
        }
    }
    _fpinfo(inv, raw, code);
    for (const auto& kv : regparam.items()) {                     // (a register parameter's register given the parameter:
        py::str p = param_name(std::get<1>(kv));                  //  itself - the model's reload at a join, no statement)
        py::list<Text> next;
        for (const Text& x : out)
            next.append(Text(py::re::sub(py::percent(ps("(?m)^[ \\t]*%s = %s;\\n"), std::get<0>(kv), p), ps(""), std::get<0>(x)), std::get<1>(x)));
        out = next;
    }
    return out;
}

void _fpinfo(InverseModel& inv, const py::bytes& raw, const py::bytes& code) {
    const Last& last = inv.LAST;                                  // (what the model read of the 8087's values: localize)
    FPINFO = FpInfo();
    FPINFO.types = last.fptypes.copy();
    FPINFO.ret = last.fpret;
    FPINFO.fp = !(raw == code) || has_fwait_esc(raw);
    text::LONGINFO.clear();                                       // (the longs: shifted without sign; a value widened last)
    text::LONGINFO.unsigned_ = py::set<py::str>::from(last.unsigned_longs);
    py::list<x86::Insn> ins = x86::disasm(code, 0, code.size(), false);
    int64_t j = ins.size() - 1;
    if (!ins.empty() && (ins[j].mnemonic == ps("ret") || ins[j].mnemonic == ps("retf"))) {
        j -= 1;
        while (j >= 0 && (ins[j].mnemonic == ps("pop") || ins[j].op_str == ps("sp, bp"))) j -= 1;
        std::optional<int64_t> epi;
        if (j + 1 < ins.size()) epi = ins[j + 1].address;
        auto gives_dx = [](const x86::Insn& x) {
            if (x.mnemonic == ps("cwd") || x.mnemonic == ps("cdq")) return true;
            return !(x.mnemonic == ps("cmp") || x.mnemonic == ps("test") || x.mnemonic == ps("push")) &&
                   (x.op_str.startswith(ps("dx, ")) || x.op_str == ps("dx"));   // (any write of dx)
        };
        std::vector<int64_t> ends = {j};
        if (epi) {
            py::str a1 = ps("0x") + hex_lower(*epi), a2 = py::to_str(*epi);
            for (int64_t k = 0; k < ins.size(); ++k)
                if (ins[k].mnemonic == ps("jmp") && (ins[k].op_str == a1 || ins[k].op_str == a2) && k > 0) ends.push_back(k - 1);
        }
        auto near = [&](int64_t e) {
            if (e < 0) return false;
            for (int64_t q = std::max<int64_t>(0, e - 2); q <= e; ++q)
                if (gives_dx(ins[q])) return true;
            return false;
        };
        bool all_near = true;
        for (int64_t e : ends)
            if (!near(e)) {
                all_near = false;
                break;
            }
        text::LONGINFO.ret = j >= 0 && all_near;                  // (dx given before every way out - just before it,
    }
}

// ---- the stage ---------------------------------------------------------------------------------------------------------------------
py::dict<int64_t, py::str> _names(Project& prj) {
    py::dict<int64_t, py::str> out;
    Located mp = prj.locate();
    py::list<LibPlace> all = mp.coded + mp.lone;
    for (const LibPlace& p : all) {
        std::vector<LibModule> mods = {p.m};
        for (const LibModule& a : p.also) mods.push_back(a);
        for (const LibModule& m : mods)
            for (const auto& kv : m.pubs.items()) {
                int64_t s = std::get<0>(std::get<1>(kv)), off = std::get<1>(std::get<1>(kv));
                if (s == m.first_code_segment) out.setdefault(p.code + off, lstrip_underscores(std::get<0>(kv)));
            }
    }
    for (const auto& kv : prj.symbols().items()) {
        const py::str& n = std::get<0>(kv);
        const py::str& t = std::get<0>(std::get<1>(kv));
        if (t == ps("C") && !n.contains(ps(":"))) out.setdefault(std::get<1>(std::get<1>(kv)), lstrip_underscores(n));
    }
    return out;
}

py::dict<int64_t, py::str> run(Project& prj, bool every, std::optional<int64_t> limit, bool verbose) {
    std::optional<py::str> model = prj.inverse_model();
    if (!model || model->empty())
        throw py::SystemExit(py::percent(ps("the toolchain %s names no inverse model (its profile's \"inverse\")"), prj.toolchain_name()));
    std::shared_ptr<InverseModel> inv = prj.load_inverse_model(*model);
    py::dict<int64_t, py::str> names = _names(prj);
    py::bytes img = prj.image();
    py::set<int64_t> have = prj.source_addresses();
    py::dict<int64_t, py::str> out;
    py::os::makedirs(prj.path(ps("drafts")), 0777, true);
    std::vector<std::tuple<int64_t, int64_t>> todo;
    for (const auto& f : prj.functions())
        if (every || !have.contains(std::get<0>(f))) todo.emplace_back(std::get<0>(f), std::get<1>(f));
    // todo[:limit]
    int64_t n = static_cast<int64_t>(todo.size()), upto = n;
    if (limit) {
        upto = *limit < 0 ? std::max<int64_t>(0, n + *limit) : std::min(*limit, n);
    }
    for (int64_t i = 0; i < upto; ++i) {
        int64_t a = std::get<0>(todo[static_cast<size_t>(i)]), ln = std::get<1>(todo[static_cast<size_t>(i)]);
        if (have.contains(a)) continue;
        py::list<std::tuple<py::str, py::dict<py::str, int64_t>>> texts;
        try {
            texts = model_texts(*inv, img.slice(a, a + ln), py::percent(ps("f%05x"), a), a);
        } catch (const py::Exception& e) {                       // (the model declines: it says why)
            std::string msg = e.what();
            if (!msg.empty()) {
                py::str first = first_line(py::str(msg));
                out.set(a, py::percent(ps("declined: %s"), first.slice(0, 100)));
            } else out.set(a, ps("declined"));
            continue;
        }
        py::str src, verdict;
        try {
            std::tie(src, verdict) = prj.prove_texts(*inv, texts, a, ln, names);
        } catch (const py::Exception& e) {                       // (a pass that fails on one function: noted, not fatal)
            out.set(a, py::percent(ps("failed: %s: %s"), py::str(std::string(e.type())), py::str(std::string(e.what())).slice(0, 100)));
            continue;
        }
        out.set(a, verdict);
        if (verdict == ps("identical")) {
            py::file f = py::file::open(prj.path(py::percent(ps("f%05x.c"), a)), ps("w"), std::nullopt, std::nullopt, ps("\n"));
            f.write(py::percent(ps("/* drafted by codebreaker exact draft (%s), identical */\n%s"), *model, src));
            f.close();
        } else {
            py::file f = py::file::open(prj.path(py::os::path::join(ps("drafts"), py::percent(ps("f%05x.c"), a))), ps("w"), std::nullopt, std::nullopt, ps("\n"));
            f.write(py::percent(ps("/* draft (%s): %s */\n%s"), *model, verdict, src));
            f.close();
        }
        if (verbose) py::print(py::percent(ps("%05x %5d  %s"), a, ln, verdict));
    }
    return out;
}

int64_t main(Project& prj, const py::list<py::str>& argv) {
    std::optional<int64_t> lim;
    if (argv.contains(ps("--limit"))) lim = py::int_parse(argv[argv.index(ps("--limit")) + 1], 10);
    py::dict<int64_t, py::str> res = run(prj, argv.contains(ps("--all")), lim, argv.contains(ps("-v")));
    py::dict<py::str, int64_t> kinds;
    for (const py::str& v : res.values()) {
        int64_t colon = v.find(ps(":"));
        py::str k = colon < 0 ? v : v.slice(0, colon);
        kinds.set(k, kinds.get(k, 0) + 1);
    }
    std::vector<std::tuple<py::str, int64_t>> items = kinds.items();
    std::stable_sort(items.begin(), items.end(), [](const auto& x, const auto& y) { return std::get<1>(x) > std::get<1>(y); });
    py::list<py::str> parts;
    for (const auto& kv : items) parts.append(py::percent(ps("%d %s"), std::get<1>(kv), std::get<0>(kv)));
    py::str joined;
    for (int64_t i = 0; i < parts.size(); ++i) joined = joined + (i ? ps(", ") : ps("")) + parts[i];
    py::print(py::percent(ps("%d functions drafted: %s"), res.size(), joined));
    return 0;
}

}  // namespace codebreaker::exact::draft
