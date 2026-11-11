// cb/core/z3_api.hpp -- the Z3 solver's C API, called through the prebuilt libz3.dll.
//
// `cb::z3::api()` loads libz3.dll (beside the executable, see dynlib.hpp) on first use and returns a table of
// typed function pointers, one per Z3 function CodeBreaker calls: contexts and solvers, bit-vector and boolean
// terms, models. The types come from the real <z3/z3.h> (native/third_party/include/z3, Z3 5.3.2, the
// z3-solver 5.3.2.0 wheel's library). Includes the Z3 C headers: only for the solver wrapper in cb/verify, never for
// a module's public header. A function not on the list is reachable through `library().function<Fn>(name)`.
#pragma once

#pragma warning(push, 0)
#include <z3/z3.h>
#pragma warning(pop)

#include <string>
#include <string_view>

#include "cb/core/dynlib.hpp"

namespace cb::z3 {

#define CB_Z3_FUNCTIONS(X)                                                                          \
    X(Z3_get_version) X(Z3_mk_config) X(Z3_del_config) X(Z3_set_param_value) X(Z3_global_param_set)   \
    X(Z3_mk_context) X(Z3_mk_context_rc) X(Z3_del_context) X(Z3_inc_ref) X(Z3_dec_ref)                \
    X(Z3_get_error_code) X(Z3_get_error_msg) X(Z3_set_error_handler)                                  \
    X(Z3_mk_string_symbol) X(Z3_mk_int_symbol) X(Z3_mk_bool_sort) X(Z3_mk_bv_sort) X(Z3_mk_array_sort)  \
    X(Z3_mk_const) X(Z3_mk_fresh_const) X(Z3_mk_func_decl) X(Z3_mk_app) X(Z3_mk_numeral)               \
    X(Z3_mk_unsigned_int64) X(Z3_mk_int64) X(Z3_mk_true) X(Z3_mk_false)                                \
    X(Z3_mk_eq) X(Z3_mk_distinct) X(Z3_mk_not) X(Z3_mk_ite) X(Z3_mk_and) X(Z3_mk_or) X(Z3_mk_xor)      \
    X(Z3_mk_implies) X(Z3_mk_iff)                                                                      \
    X(Z3_mk_bvnot) X(Z3_mk_bvneg) X(Z3_mk_bvand) X(Z3_mk_bvor) X(Z3_mk_bvxor) X(Z3_mk_bvadd)           \
    X(Z3_mk_bvsub) X(Z3_mk_bvmul) X(Z3_mk_bvudiv) X(Z3_mk_bvsdiv) X(Z3_mk_bvurem) X(Z3_mk_bvsrem)       \
    X(Z3_mk_bvshl) X(Z3_mk_bvlshr) X(Z3_mk_bvashr) X(Z3_mk_rotate_left) X(Z3_mk_rotate_right)          \
    X(Z3_mk_bvult) X(Z3_mk_bvule) X(Z3_mk_bvugt) X(Z3_mk_bvuge) X(Z3_mk_bvslt) X(Z3_mk_bvsle)           \
    X(Z3_mk_bvsgt) X(Z3_mk_bvsge)                                                                      \
    X(Z3_mk_concat) X(Z3_mk_extract) X(Z3_mk_zero_ext) X(Z3_mk_sign_ext) X(Z3_mk_repeat)               \
    X(Z3_mk_select) X(Z3_mk_store) X(Z3_mk_const_array)                                                \
    X(Z3_mk_solver) X(Z3_solver_inc_ref) X(Z3_solver_dec_ref) X(Z3_solver_assert) X(Z3_solver_check)   \
    X(Z3_solver_check_assumptions) X(Z3_solver_get_model) X(Z3_solver_push) X(Z3_solver_pop)           \
    X(Z3_solver_reset) X(Z3_solver_get_reason_unknown) X(Z3_solver_set_params) X(Z3_solver_to_string)   \
    X(Z3_mk_params) X(Z3_params_inc_ref) X(Z3_params_dec_ref) X(Z3_params_set_uint)                    \
    X(Z3_model_inc_ref) X(Z3_model_dec_ref) X(Z3_model_eval) X(Z3_model_to_string)                     \
    X(Z3_simplify) X(Z3_get_numeral_uint64) X(Z3_get_numeral_int64) X(Z3_get_numeral_string)           \
    X(Z3_ast_to_string) X(Z3_get_sort) X(Z3_get_bv_sort_size) X(Z3_get_ast_kind) X(Z3_is_numeral_ast)

struct Api {
#define CB_X(name) decltype(&::name) name = nullptr;
    CB_Z3_FUNCTIONS(CB_X)
#undef CB_X
};

// The loaded library's function table; loads libz3.dll on the first call. Throws NotFoundError if the DLL or
// one of the functions is missing. Thread-safe.
const Api& api();
const DynLib& library();

// Z3's version as "major.minor.build.revision".
std::string version_string();

}  // namespace cb::z3
