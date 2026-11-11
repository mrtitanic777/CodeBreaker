// cb/port/real_program.hpp -- test support: the part-4 functions run over a stored Port state (the files of a real
// program as the port has them after its constructor, from a run of the Python original). Used by real_program_test.cpp
// and by the harness driver outside the tree, so both run the same code.
//
// Each Case is (tag, key, value): the call made and what the refactored code answered, as JSON. A stored golden file
// has the CRC-32 of each value's canonical JSON; the test compares those.
#pragma once

#include <nlohmann/json.hpp>

#include <algorithm>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "cb/base/ctext.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/port/bss.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/complaints.hpp"
#include "cb/port/frames.hpp"
#include "cb/port/headers.hpp"
#include "cb/port/old_sizes.hpp"
#include "cb/port/original_program.hpp"
#include "cb/port/rewrite.hpp"
#include "cb/port/struct_layout.hpp"
#include "cb/port/union_views.hpp"

namespace cb::port::pre_ansi::real {

using J = nlohmann::ordered_json;

struct Case {
    std::string tag;
    J key;
    J value;
};

class Cases {
public:
    template <class F>
    void attempt(const std::string& tag, J key, F f) {
        J result;
        try {
            result = f();
        } catch (const std::exception& e) {
            result = J{{"error", e.what()}};
        }
        all.push_back({tag, std::move(key), std::move(result)});
    }
    std::vector<Case> all;
};

namespace detail {

inline std::string resolve(const std::string& dir, const std::string& p) {
    return std::filesystem::path(p).is_absolute() || dir.empty() ? p : (std::filesystem::path(dir) / p).string();
}
inline std::size_t code_points(std::string_view s) {
    std::size_t n = 0;
    for (const char c : s) n += (static_cast<unsigned char>(c) & 0xC0) != 0x80;
    return n;
}
inline J crc(std::string_view t) { return J::array({code_points(t), crypto::crc32(as_bytes(t))}); }

inline J spans_json(const std::optional<std::vector<Span>>& s) {
    if (!s) return nullptr;
    J out = J::array();
    for (const Span& x : *s) out.push_back(J::array({x.begin, x.end}));
    return out;
}

// widths() of the port's part 1 (not refactored: the harness's own copy, for proto.h and the like)
inline std::string widths_text(std::string text, bool int16) {
    struct Edit {
        std::size_t a, b;
        std::optional<std::string> neu;
    };
    std::string code = base::ctext::blank(text);
    std::vector<Edit> edits;
    auto sub = [&](const Regex& rx, const std::string& neu, std::size_t lo, std::optional<std::size_t> hi) {
        const std::size_t end = hi ? *hi : code.size();
        rx.for_each_match(std::string_view(code).substr(lo, end - lo), [&](const RegexMatch& m) {
            const std::size_t a = lo + m.start(), b = lo + m.end();
            bool clash = false;
            for (const Edit& e : edits)
                if (a < e.b && e.a < b) clash = true;
            if (!clash) edits.push_back({a, b, neu});
            return true;
        });
    };
    static const Regex struct_head(R"(\b(?:struct|union)\s*\w*\s*\{)");
    static const Regex uns(R"(\bunsigned\s+short(?:\s+int)?\b|\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)");
    static const Regex sgn(R"((?<!unsigned )(?<!signed )\b(?:short\s+int|short|int)\b(?!\w))");
    static const Regex ucast(R"(\(\s*unsigned(?:\s+int)?\s*(?=\*+\s*\)))");
    static const Regex icast(R"(\(\s*int\s*(?=\*+\s*\)))");
    static const Regex ulong(R"(\bunsigned\s+long(?:\s+int)?\b|\blong\s+unsigned(?:\s+int)?\b)");
    static const Regex slong(R"(\blong(?:\s+int)?\b)");
    static const Regex ushort(R"(\bunsigned\s+short(?:\s+int)?\b)");
    static const Regex sshort(R"(\bshort(?:\s+int)?\b)");
    static const Regex mainline(R"((?m)^.*\bmain\s*\(.*$)");
    static const Regex uint_all(R"(\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)");
    static const Regex int_all(R"((?<!unsigned )(?<!signed )\bint\b)");
    struct_head.for_each_match(code, [&](const RegexMatch& m) {
        const std::size_t k = port::pre_ansi::match_brace(code, m.end() - 1);
        sub(uns, "uint16_t", m.end(), k);
        sub(sgn, "int16_t", m.end(), k);
        return true;
    });
    sub(ucast, "(uint16_t ", 0, std::nullopt);
    sub(icast, "(int16_t ", 0, std::nullopt);
    sub(ulong, "uint32_t", 0, std::nullopt);
    sub(slong, "int32_t", 0, std::nullopt);
    sub(ushort, "uint16_t", 0, std::nullopt);
    sub(sshort, "int16_t", 0, std::nullopt);
    if (int16) {
        mainline.for_each_match(code, [&](const RegexMatch& m) {
            edits.push_back({m.start(), m.start(), std::nullopt});
            edits.push_back({m.start(), m.end(), std::nullopt});
            return true;
        });
        sub(uint_all, "uint16_t", 0, std::nullopt);
        sub(int_all, "int16_t", 0, std::nullopt);
        edits.erase(std::remove_if(edits.begin(), edits.end(), [](const Edit& e) { return !e.neu; }), edits.end());
    }
    std::sort(edits.begin(), edits.end(), [](const Edit& x, const Edit& y) { return std::tie(x.a, x.b) > std::tie(y.a, y.b); });
    for (const Edit& e : edits) {
        text = text.substr(0, e.a) + *e.neu + text.substr(e.b);
        code = code.substr(0, e.a) + *e.neu + code.substr(e.b);
    }
    return text;
}

inline const char* kind_key(Complaint c) {
    static const char* keys[] = {"libname", "argtype", "fnptr", "intptr", "assign", "retptr", "retint", "retmix",
                                 "callptr", "neg", "member", "void", "ptrcast", "incomplete", "unionset", "unionarg",
                                 "seqpt", "intcast", "arity"};
    return keys[static_cast<int>(c)];
}

inline J lay_json(const StructLayout& l) {
    J ms = J::array();
    for (const Member& m : l.members) ms.push_back(J::array({m.name, m.offset, m.size}));
    return J::array({l.size, ms});
}

inline J layouts_json(const StructLayouts& ls) {
    J out = J::object();
    for (const auto& kv : ls) out[kv.first] = lay_json(kv.second);
    return out;
}

inline J map_json(const OrderedMap<std::string, std::uint64_t>& m) {
    J out = J::object();
    for (const auto& kv : m) out[kv.first] = kv.second;
    return out;
}

inline std::vector<std::pair<std::string, std::string>> pairs_of(const J& j) {
    std::vector<std::pair<std::string, std::string>> out;
    for (const J& x : j) out.emplace_back(x[0].get<std::string>(), x[1].get<std::string>());
    return out;
}


}  // namespace detail

// Runs every call the harness makes for one target; `profile_dir` is where the state's profile name is found.
inline std::vector<Case> run_target(const J& st, const std::string& profile_dir) {
    using namespace detail;
    Cases cs;
    const std::string target = st["target"].get<std::string>();
    const J jtarget = target;
    static const Regex arrow_or_dot(R"(->|\.)");
    static const Regex call_head(R"(\b([A-Za-z_]\w*)\s*\()");
    static const Regex literal_run(R"re("(?:[^"\\\n]|\\.)*"(?:\s*"(?:[^"\\\n]|\\.)*")*)re");
    {
        const std::string profile_path = resolve(profile_dir, st["profile"].get<std::string>());
        const Profile profile = Profile::load(profile_path);
        Files input_files, pfiles, originals;
        for (const J& x : st["input_files"]) input_files.insert_or_assign(x[0].get<std::string>(), x[1].get<std::string>());
        for (const J& x : st["files"]) pfiles.insert_or_assign(x[0].get<std::string>(), x[1].get<std::string>());
        for (const J& x : st["originals"]) originals.insert_or_assign(x[0].get<std::string>(), x[1].get<std::string>());
        std::set<std::string, std::less<>> left_out;
        for (const J& x : st["left_out"]) left_out.insert(x.get<std::string>());
        Aliases aliases;
        for (const J& x : st["aliases"]) aliases.insert_or_assign(x[0].get<std::string>(), Alias{x[1].get<std::string>(), x[2].get<std::string>()});
        std::set<std::string> view_headers;
        for (const J& x : st["view_headers"]) view_headers.insert(x.get<std::string>());
        FinishRules rules;
        for (const J& x : st["renames"]) rules.renames[x[0].get<std::string>()] = x[1].get<std::string>();
        for (const J& x : st["decl_rules"])
            if (x[1].is_string()) rules.decl_rules.emplace_back(x[0].get<std::string>(), x[1].get<std::string>());
        for (const J& x : st["local_retypes"]) rules.local_retypes.push_back({x[0], x[1], x[2], x[3]});
        const bool int16 = st["int16"].get<bool>();
        const auto data = pairs_of(st["data"]);
        const auto asm_data = pairs_of(st["asm_data"]);
        std::map<std::string, std::string> asm_protos;
        for (const J& x : st["asm_protos"]) asm_protos[x[0].get<std::string>()] = x[1].get<std::string>();
        ParamTypes param_types;
        for (const J& x : st["param_types"]) param_types[{x[0].get<std::string>(), x[1].get<std::size_t>()}] = x[2].get<std::string>();
        BssSizes bss;
        for (const J& x : st["bss"]) bss.insert_or_assign(x[0].get<std::string>(), x[1].get<std::uint64_t>());
        Notes notes;
        for (const J& x : st["notes"]) notes[x[0].get<std::string>()] = x[1].get<std::vector<std::string>>();

        // ---- free functions on every file
        std::vector<std::string> names;
        for (const auto& kv : input_files) names.push_back(kv.first);
        std::sort(names.begin(), names.end());
        std::vector<std::string> texts;
        for (const std::string& n : names) texts.push_back(input_files.at(n));
        for (const std::string& n : names) {
            const std::string& t = input_files.at(n);
            const J key = J::array({jtarget, n});
            cs.attempt("split_view_lists", key, [&] { return crc(split_view_lists(t)); });
            cs.attempt("union_views", key, [&] {
                Notes nn;
                const std::string r = union_views(t, nn, n);
                std::vector<std::string> ks;
                for (const auto& kv : nn) ks.push_back(kv.first);
                std::sort(ks.begin(), ks.end());
                return J::array({crc(r), ks});
            });
            const std::vector<std::string> lines = split_lines(t), code = split_lines(base::ctext::blank(t));
            const auto da = definitions_any(lines, code);
            cs.attempt("definitions_any", key, [&] {
                J out = J::array();
                for (const auto& d : da) out.push_back(J::array({d.name, d.first, d.open, d.last}));
                return out;
            });
            for (const auto& d : da) {
                cs.attempt("_locals", J::array({jtarget, n, d.name}), [&] {
                    J out = J::array();
                    for (const auto& kv : local_decls(code, d.open, d.last)) {
                        const LocalDecl& v = kv.second;
                        out.push_back(J::array({kv.first, v.base, v.declarator, v.size ? J(*v.size) : J(nullptr), v.line, v.is_register}));
                    }
                    return out;
                });
            }
            cs.attempt("object_before", key, [&] {
                J out = J::array();
                for (std::size_t i = 0; i < code.size(); ++i)
                    arrow_or_dot.for_each_match(code[i], [&](const RegexMatch& m) {
                        const auto r = object_before(code[i], m.start());
                        out.push_back(J::array({i, r ? J(*r) : J(-1)}));
                        return true;
                    });
                while (out.size() > 400) out.erase(out.size() - 1);
                return out;
            });
            std::vector<std::pair<std::size_t, std::string>> calls;
            for (std::size_t i = 0; i < code.size(); ++i)
                call_head.for_each_match(code[i], [&](const RegexMatch& m) {
                    if (!is_keyword(m.group(1))) calls.emplace_back(i, std::string(m.group(1)));
                    return true;
                });
            cs.attempt("call_args", key, [&] {
                J out = J::array();
                for (std::size_t c = 0; c < calls.size() && c < 500; ++c)
                    out.push_back(J::array({calls[c].first, calls[c].second, spans_json(call_args(code[calls[c].first], calls[c].second)),
                                            spans_json(call_args(code[calls[c].first], calls[c].second, true))}));
                return out;
            });
            cs.attempt("c_string_bytes", key, [&] {
                J out = J::array();
                literal_run.for_each_match(t, [&](const RegexMatch& m) {
                    if (out.size() < 300) out.push_back(text::from_latin1(c_string_bytes(m.whole())));
                    return true;
                });
                return out;
            });
        }
        cs.attempt("members", J::array({jtarget}), [&] {
            J out = J::object();
            for (const auto& kv : struct_members(texts)) out[kv.first] = kv.second;
            return out;
        });
        cs.attempt("layouts", J::array({jtarget}), [&] { return layouts_json(lay_out_structs(texts, 2, 2)); });
        cs.attempt("layouts4", J::array({jtarget}), [&] { return layouts_json(lay_out_structs(texts, 8, 8)); });
        // ---- size_of, abstract, truthy on every function the program defines
        for (const std::string& n : names) {
            if (!n.ends_with(".c")) continue;
            const std::vector<std::string> lines = split_lines(input_files.at(n)), code = split_lines(base::ctext::blank(input_files.at(n)));
            for (const Definition& d : definitions(lines, code)) {
                std::vector<std::string> piece(code.begin() + static_cast<std::ptrdiff_t>(d.open), code.begin() + static_cast<std::ptrdiff_t>(d.last + 1));
                const std::string body = join_lines(piece);
                for (const Param& p : d.fn.params) {
                    cs.attempt("abstract", J::array({jtarget, n, d.fn.name, p.name}), [&] { return abstract_type(p.type, p.name); });
                    cs.attempt("truthy", J::array({jtarget, n, d.fn.name, p.name}), [&] { return only_tested(body, p.name); });
                }
            }
        }
        const char* samples[] = {
            "conflicting types for 'foo'", "passing argument 2 of 'bar' from incompatible pointer type",
            "incompatible pointer types passing 'int *' (aka 'int *') to parameter of type 'char *'",
            "assignment to 'int' from 'char *' makes integer from pointer without a cast", "too many arguments to function 'f'",
            "request for member 'x' in something not a structure or union", "void value not ignored as it ought to be",
            "'struct s' has no member named 'zz'", "nothing matches this", "cast to pointer from integer of different size",
            "returning 'int' from a function with return type 'char *' makes pointer from integer without a cast",
            "unsequenced modification and access to 'i'", "comparison is always false due to limited range of data type"};
        cs.attempt("said", J::array({jtarget}), [&] {
            J out = J::array();
            for (const char* s : samples) {
                const auto r = said(s);
                out.push_back(r ? J::array({kind_key(r->kind), std::string(r->match.whole())}) : J::array({nullptr, nullptr}));
            }
            return out;
        });

        // ---- the Port
        OrderedMap<std::string, Fn> fns;
        std::vector<std::string> pnames;
        for (const auto& kv : pfiles) pnames.push_back(kv.first);
        std::sort(pnames.begin(), pnames.end());
        for (const std::string& n : pnames) {
            if (!n.ends_with(".c")) continue;
            const std::string& t = pfiles.at(n);
            for (Definition& d : definitions(split_lines(t), split_lines(base::ctext::blank(t)))) {
                fns.try_emplace(d.fn.name, d.fn);
                fns.at(d.fn.name).file = n;
            }
        }
        OriginalProgram original(originals, profile.old_pointer_bytes());
        BssPlan plan(original, profile.runtime_internals);
        plan.set_sizes(bss);
        const J k1 = J::array({jtarget});
        cs.attempt("old_layouts", k1, [&] { return layouts_json(original.layouts()); });
        cs.attempt("old_elem_sizes", k1, [&] { return map_json(original.elem_sizes()); });
        cs.attempt("old_var_sizes", k1, [&] { return map_json(original.var_sizes({})); });
        cs.attempt("allocators", k1, [&] {
            J out = J::object();
            for (const auto& kv : original.allocators(pfiles)) out[kv.first] = std::vector<std::size_t>(kv.second.begin(), kv.second.end());
            return out;
        });
        std::set<std::string> bss_names;
        for (const auto& kv : bss) bss_names.insert(kv.first);
        cs.attempt("bss_decls", k1, [&] {
            J out = J::object();
            for (const auto& kv : original.extern_decls(bss_names)) out[kv.first] = J::array({kv.second.first, kv.second.second});
            return out;
        });
        for (const std::string& n : pnames)
            cs.attempt("old_layouts_for", J::array({jtarget, n}), [&] {
                std::vector<std::pair<std::string, std::uint32_t>> v;
                for (const auto& kv : original.layouts_for(n)) v.emplace_back(kv.first, kv.second.size);
                std::sort(v.begin(), v.end());
                J out = J::array();
                for (const auto& e : v) out.push_back(J::array({e.first, e.second}));
                return out;
            });
        std::map<std::string, std::string> outs;
        for (const std::string& n : pnames) {   // (the Python's harness ran it twice for each file)
            cs.attempt("_preprocessor", J::array({jtarget, n}), [&] { return crc(preprocess(pfiles.at(n), n, profile, left_out, notes)); });
            outs[n] = preprocess(pfiles.at(n), n, profile, left_out, notes);
        }
        for (const std::string& n : pnames)
            if (n.ends_with(".c"))
                cs.attempt("_definitions", J::array({jtarget, n}), [&] { return crc(rewrite_definitions(outs.at(n), n, fns, param_types, notes).text); });
        Files outs2;
        for (const std::string& n : pnames) outs2.insert_or_assign(n, n.ends_with(".c") ? rewrite_definitions(outs.at(n), n, fns, param_types, notes).text : outs.at(n));
        cs.attempt("fns", k1, [&] {
            J out = J::array();
            for (const auto& kv : fns) {
                const Fn& f = kv.second;
                J params = J::array();
                for (const Param& p : f.params) params.push_back(J::array({p.name, p.type}));
                out.push_back(J::array({f.name, f.ret, params, f.is_static, f.returns_void, f.loose, f.varargs ? 1 : 0, f.bare}));
            }
            return out;
        });
        cs.attempt("frames", k1, [&] {
            frames(outs2, fns, profile.old_pointer_bytes(), {}, notes);
            std::map<std::string, J> per_file;
            for (const auto& kv : outs2) per_file[kv.first] = crc(kv.second);
            std::vector<std::string> ks;
            for (const auto& kv : notes) ks.push_back(kv.first);
            std::sort(ks.begin(), ks.end());
            return J::array({per_file, ks});
        });
        cs.attempt("bss_views", k1, [&] {
            plan.bss_views(pfiles, fns, aliases, view_headers, notes);
            std::vector<std::pair<std::string, Alias>> al(aliases.begin(), aliases.end());
            std::sort(al.begin(), al.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            J aj = J::array();
            for (const auto& e : al) aj.push_back(J::array({e.first, J::array({e.second.where, e.second.declaration})}));
            J sj = J::object();
            for (const auto& kv : plan.structs()) sj[kv.first] = J::array({kv.second.type, kv.second.names});
            return J::array({aj, sj, std::vector<std::string>(view_headers.begin(), view_headers.end())});
        });
        cs.attempt("join_bss", k1, [&] {
            J out = J::array();
            std::size_t c = 0;
            for (const auto& kv : bss) {
                if (c++ >= 60) break;
                out.push_back(J::array({kv.first, plan.join_bss(kv.first, aliases, rules.decl_rules)}));
            }
            return out;
        });
        cs.attempt("bss_definitions", k1, [&] { return J(plan.definitions(aliases)); });
        cs.attempt("finish", k1, [&] {
            std::map<std::string, J> per_file;
            for (const auto& kv : outs2) per_file[kv.first] = crc(finish(rules, kv.first, kv.second).text);
            return per_file;
        });
        cs.attempt("headers", k1, [&] {
            const HeaderInputs in{fns, pfiles, data, asm_data, asm_protos, profile, view_headers, int16};
            const WidthsFn widths = [](std::string_view text, std::string_view, bool i16) { return widths_text(std::string(text), i16); };
            const Headers h = make_headers(in, aliases, plan, rules, widths);
            J out = J::object();
            for (const auto& kv : h.files) out[kv.first] = kv.second ? crc(*kv.second) : J(nullptr);
            return out;
        });
        cs.attempt("notes", k1, [&] {
            std::vector<std::pair<std::string, std::vector<std::string>>> v(notes.begin(), notes.end());
            std::sort(v.begin(), v.end());
            J out = J::array();
            for (const auto& e : v) out.push_back(J::array({e.first, e.second}));
            return out;
        });

    }
    return cs.all;
}

// The canonical form of a value (object keys sorted, lists in the order given) as the text its CRC is of; lists the
// harness treats as sets (fns, notes, frames' notes, aliases, allocators' places) are sorted first.
inline std::string canonical(const std::string& tag, const J& value) {
    J v = value;
    auto sorted = [](J list) {
        std::vector<std::string> dumps;
        for (const J& x : list) dumps.push_back(x.dump());
        std::sort(dumps.begin(), dumps.end());
        J out = J::array();
        for (const std::string& d : dumps) out.push_back(J::parse(d));
        return out;
    };
    if (v.is_object() && v.size() == 1 && v.contains("error")) return "{\"error\":true}";
    if (tag == "fns" || tag == "notes") v = sorted(v);
    if (tag == "frames") v[1] = sorted(v[1]);
    if (tag == "bss_views") v[0] = sorted(v[0]);
    if (tag == "allocators")
        for (auto it = v.begin(); it != v.end(); ++it) it.value() = sorted(it.value());
    return nlohmann::json::parse(v.dump()).dump();   // (a json, not an ordered_json: keys sorted)
}

inline std::uint32_t golden_crc(const std::string& tag, const J& value) { return crypto::crc32(as_bytes(canonical(tag, value))); }

}  // namespace cb::port::pre_ansi::real
