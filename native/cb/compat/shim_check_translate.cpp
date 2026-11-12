// Compile-only check of the shims at the old header paths that call old ports (which cannot be linked into cb_tests): the old
// API of ui/progress, translate/dotnet/judge, translate/l2 (over the old IR tree and the L2 core library), translate/desugar,
// translate/native_l2 and translate/javajudge over cb::ui and cb::translate. Nothing here runs; it is in cb_shim_check so the
// shims stay honest until their last consumer is refactored. (The shims are header-only: including them compiles every
// definition in them.)
#include "codebreaker/translate/backends/c.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/desugar.hpp"
#include "codebreaker/translate/dotnet/judge.hpp"
#include "codebreaker/translate/frontends/csharp/lower.hpp"
#include "codebreaker/translate/frontends/csharp/package.hpp"
#include "codebreaker/translate/javajudge.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "codebreaker/ui/progress.hpp"

namespace shim_check_translate {

inline void touch() {
    namespace progress = codebreaker::ui::progress;
    namespace judge = codebreaker::translate::dotnet::judge;
    (void)&progress::clock;
    (void)&progress::_busy_workers;
    (void)&progress::_last_line;
    (void)&progress::snapshot;
    (void)&progress::_clock;
    (void)&progress::line;
    (void)&progress::main;
    (void)&judge::_norm;
    (void)&judge::compare;
    (void)&judge::_fresh;
    (void)&judge::csharp;
    (void)&judge::_errors;
    (void)&judge::java;
    (void)&judge::cpp;
}

inline void touch_l2() {
    namespace l2 = codebreaker::translate::l2;
    namespace ir = codebreaker::translate::ir;
    // the old tree as L2 documents, the schema check through the library
    ir::NodeP (*from)(const ir::Dict&) = &l2::from_json;
    ir::Dict (*to)(const ir::NodeP&) = &l2::to_json;
    (void)from;
    (void)to;
    (void)&l2::type_json;
    (void)&l2::type_from;
    (void)&l2::sym_json;
    (void)&l2::sym_from;
    (void)&l2::validate;
    (void)&l2::schema;
    // the lanes' module types derive from the shim's Made
    struct Lane : l2::Made {
        using l2::Made::Made;
    };
    Lane lane;
    (void)lane.texts();
}

inline void touch_native_l2() {
    namespace native = codebreaker::translate::native_l2;
    namespace ir = codebreaker::translate::ir;
    (void)static_cast<py::str (*)(const ir::Dict&, const py::list<py::str>&, const std::optional<py::set<py::str>>&, const std::optional<native::Wire>&)>(&native::function);
    (void)static_cast<py::str (*)(const ir::Dict&)>(&native::signature);
    (void)&native::takes32;
    (void)&native::roundtrip_text;
    (void)static_cast<std::tuple<bool, py::str> (*)(const ir::Dict&)>(&native::validate);
    // the back ends' declines are the library's
    namespace c = codebreaker::translate::backends::c;
    namespace cpp17 = codebreaker::translate::backends::cpp17;
    static_assert(std::is_same_v<c::Decline, native::Decline>);
    static_assert(std::is_same_v<cpp17::Decline, native::Cpp17Decline>);
}

inline void touch_desugar_and_judge() {
    namespace desugar = codebreaker::translate::desugar;
    namespace judge = codebreaker::translate::javajudge;
    (void)&desugar::run;
    (void)&judge::judge_tree;
    (void)&judge::judge_program;
    (void)&judge::lower_program;
    (void)&judge::build;
    (void)&judge::set_java_frontend;
}

inline void touch_csharp_lower() {
    // the old front end's calls over the old tree (package.cpp is the one caller)
    namespace lower = codebreaker::translate::frontends::csharp::lower;
    namespace ir = codebreaker::translate::ir;
    (void)static_cast<ir::Dict (*)(const py::str&)>(&lower::load_tree);
    (void)static_cast<py::ref<ir::Unit> (*)(const ir::Dict&, const std::optional<py::str>&)>(&lower::lower);
    (void)&lower::lower_with_refusals;
    (void)&lower::LANG;
}

}  // namespace shim_check_translate
