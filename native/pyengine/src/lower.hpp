// lower.hpp -- a typed Python function into L2 (the old front end's lower.py), and the module units
// (its translate / build_unit): records named and declared, classes with their attributes, each
// module's functions and names, and the ledger of what was refused and why.
#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "infer.hpp"
#include "ir.hpp"

namespace pyengine {

struct Refused {
    Issue issue;
    Refused(std::string cls, std::string detail = "", int line = 0) : issue{std::move(cls), std::move(detail), line} {}
};

struct RecordField {
    std::string key;
    T type;
    bool required;
};

class Records {
public:
    explicit Records(Program& p);
    std::vector<std::pair<int, std::string>> names;     // root -> struct name (in order)
    std::map<int, std::string> name_of, module, owner_fn;
    std::map<int, int> home;
    std::vector<RecordField> fields(int s);
    bool is_ref(int s);
    bool holds(const T& t, int s, int depth = 0);
private:
    Program& p;
    std::string name_for(int s, const std::string& fq);
};

class Context {
public:
    Context(Program& p, const std::string& int_policy);
    Program& p;
    Records records;
    std::string int_policy;
    std::set<int> used_records;
    std::string qual_record(int s);
    TyP ty(const T& t, int line = 0, const std::string& what = "value");
};

struct Lowered {
    IRP md;                        // the MethodDecl, or null when refused
    IRP extra;                     // (new) a declaration it needs beside it (a generator's class)
    std::optional<Refused> refused;
    std::set<std::string> calls;
    std::set<std::pair<std::string, std::string>> globals_used;
    bool has_fl = false;
};

struct ModuleResult {
    std::string name;
    IRP unit;
    std::vector<std::pair<std::string, IRP>> translated;   // in the module's order
    std::vector<std::pair<std::string, Issue>> refused;
    std::vector<IRP> extra;
    std::map<std::string, int> lines;
};

struct TranslateResult {
    std::map<std::string, ModuleResult> modules;
    std::map<std::string, Lowered> lowered;
};

TranslateResult translate(Program& p, const std::string& int_policy, const std::vector<std::string>* modules = nullptr);

}  // namespace pyengine
