// codebreaker/java/emit/ownership.hpp -- typed C++ port of codebreaker.java.emit.ownership.
//
// ownership.py -- which instance fields a class owns on the heap.
//
// Shared by header.py (to declare the Rule-of-Three destructor + disabled copy)
// and source.py (to define the destructor). A field is "owned heap" iff it is a
// POINTER field (per the resolver) that a constructor assigns via `new`. Value-type
// and static fields are never owned here -- deleting those would be wrong.
//
// This is the heap-corruption defense: an owning class with a destructor but a
// default copy constructor double-frees when copied (the classic 0xC0000018). We
// detect owners here so emit can free them once AND forbid copying.
//
// Notes on the port: classes, methods and resolver are the typed ports of codebreaker.java.ir and
// codebreaker.java.types; a Method / JClass / Field is a py::ref (shared, as in Python).
#pragma once

#include <cstdint>
#include <optional>

#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::emit::ownership {

using MethodRef = py::ref<ir::Method>;
using ClassRef = py::ref<ir::JClass>;
using types::TypeResolver;

// `new T[]{a, b, c}` -- a static array with an aggregate (brace) initializer
extern const py::re::Pattern _AGG_ARRAY;

// Names of value-struct PARAMS used as nullable references in the body (compared/assigned
// null). A C++ value can't hold null, so such a param is really a reference and must be a
// pointer -- and callers in this corpus already pass the (promoted) pointer for these
// raytrace-style dual-use types (Vec3D / AxisAlignedBB / MovingObjectPosition). Excludes
// primitives, String and containers (those have their own handling).
py::set<py::str> nullable_value_params(const MethodRef& method, const TypeResolver& resolver);

// Param names that the body REASSIGNS (`p = ...`, `p += ...`, `p++`). Java lets a method reassign
// its parameter (the param is a local copy of the reference/value); a C++ `const X&` param cannot be
// reassigned -> "no operator = for const JavaString". Such params must be passed BY VALUE (mutable).
py::set<py::str> _reassigned_params(const MethodRef& method);

// The C++ parameter list for a method, promoting nullable value-struct params to pointers.
// Single source of truth so the header declaration and the source definition always agree.
py::str params_str(const MethodRef& method, const TypeResolver& resolver);

// For a static field initialized as `new T[]{...}`, the raw brace contents; else None.
// C++03 cannot `new T[]{...}`, so emit (header + source) declare a fixed C array instead.
std::optional<py::str> aggregate_array_body(const py::ref<ir::Field>& field);

// -- no-GC ownership ----------------------------------------------------------------------------
// A field is OWNED (this object must `delete` it) iff SOME method assigns it a fresh heap allocation
// (`field = new T(...)` or an owning factory `X::createY(...)`) and NO method ever aliases it to a value
// of unknown provenance (a parameter, another field, a plain getter) -- deleting a BORROWED pointer
// double-frees. `= NULL` is neutral. This is broadened from the old ctor-only/`new`-only heuristic so
// factory-allocated (BufferUtils::createIntBuffer) and init()-allocated members are freed too; the
// borrow-exclusion is what keeps a passed-in pointer (a renderer's `level`) from being double-freed.
// NOTE: method bodies here are RAW JAVA (`this.field = new T()`, `Buf.createBuf(...)`), so the patterns
// accept Java `.` as well as C++ `::`/`->` -- ownership is derived before statement translation.
extern const py::re::Pattern _ASSIGN_STMT;
extern const py::re::Pattern _RHS_NEW;
extern const py::re::Pattern _RHS_FACTORY;
extern const py::re::Pattern _RHS_NEUTRAL;
// container element adds: `c.push_back(EXPR)` / `c.add(EXPR)` / `c[i] = EXPR`
extern const py::re::Pattern _ADD_CALL;
extern const py::re::Pattern _ELEM_SET;

bool _is_alloc_rhs(const py::str& rhs);

// A method's body as one string, line-comments stripped (the form every ownership scan wants).
py::str _body_text(const MethodRef& method);

// Body text of every method (ctors included), line-comments stripped.
py::list<py::str> _method_texts(const ClassRef& cls);

// {field_name: is_array} for POINTER instance fields this object owns and the destructor may
// safely `delete`: allocated somewhere and never aliased to a borrowed value (see module note).
py::dict<py::str, bool> owned_pointers(const ClassRef& cls, const TypeResolver& resolver);

// {field_name: size_accessor} for CONTAINER fields (a Java array or List of a reference type) whose
// EVERY element is a fresh allocation and never a borrowed element -- the destructor loop-deletes them.
// accessor is '.length' (JArray) or '.size()' (JList). A single borrowed add disqualifies the field
// (e.g. a listener list that stores passed-in observers).
py::dict<py::str, py::str> owned_containers(const ClassRef& cls, const TypeResolver& resolver);

// True if the object owns ANY heap (a scalar pointer field or a container of owned pointers). Such a
// class must forbid copying, else a by-value copy double-frees on destruction (Rule of Three).
bool owns_heap(const ClassRef& cls, const TypeResolver& resolver);

// -- returned ownership (caller frees) ----------------------------------------------------------
// Some methods BUILD heap objects and return them for the caller to own+free (Level.getCubes returns a
// fresh List of `new AABB`). Java GC'd those; on the 360 the caller must delete them. We detect such
// methods CONSERVATIVELY (fresh local container, every element a fresh allocation, then returned) so the
// emitter can drop an RAII element-deleter at the call site. A method that returns a field/param (a
// borrowed view) never qualifies.
bool _returns_container(const py::ref<ir::JType>& rt);

// True iff the method returns a FRESHLY-built container whose every element is a fresh allocation --
// so the caller owns those elements. Conservative: any borrowed add, or returning a non-locally-built
// container, disqualifies it (we then simply don't emit cleanup and the leak stays flagged).
bool returns_owned_container(const MethodRef& method, const TypeResolver& resolver);

// Program-wide set of method NAMES that return a freshly-built owned container. UNANIMOUS: a name is
// included only if some definition qualifies AND no same-named definition returns a container that is
// NOT freshly-owned -- so matching a call site by method name alone stays safe.
py::set<py::str> owned_return_container_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver);

// -- owned SCALAR return + argument-retention (Pattern B: `getCubes(bb.expand(...))`) ------------
// A method that returns a fresh heap SCALAR (`AABB.expand` -> `return new AABB(...)`) hands the caller an
// object it must free. When such a call is used INLINE as an argument and its result is never captured, it
// leaks. We can hoist it to a temp and RAII-delete it -- but ONLY if the receiving method does not RETAIN
// the argument (store it in a field, return it, add it to a container); otherwise the delete would dangle.

// Gate for scalar-ownership analysis: the return is a single reference/pointer object -- not void,
// an array, or a container.
bool _returns_scalar_pointer(const MethodRef& method, const TypeResolver& resolver);

// True iff EVERY non-null return of the method is a fresh `new X(...)` (the caller owns the result).
bool returns_owned_scalar(const MethodRef& method, const TypeResolver& resolver);

// {method_name: cpp_element_type} for methods that return a fresh heap scalar. UNANIMOUS across defs
// of the same name (conflicting element type or any non-owning def drops the name).
py::dict<py::str, py::str> owned_scalar_return_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver);

// Conservative: a param is RETAINED unless every occurrence in the body is a pure read (`name.` or
// `name[`). Anything else -- stored, returned, passed on, compared -- counts as retained, so we only
// ever free an argument the callee provably just reads.
bool _param_retained(const py::str& name, const py::str& text);

// {method_name: set(param_index)} for parameters that SOME definition retains -- so a temp passed at
// such a position must NOT be auto-freed. A name absent here retains nothing in any definition.
py::dict<py::str, py::set<int64_t>> retained_param_positions(const py::list<ClassRef>& classes);

// Whether the method's return should be a C++ pointer.
//
// Reference types are always pointers. A VALUE type (e.g. Vec3D) is promoted to
// a pointer when the body `return new <ReturnType>(...)` -- the dual-use case
// where a struct is stored by value as a field but heap-allocated on return.
bool returns_pointer(const MethodRef& method, const TypeResolver& resolver);

// The C++ return type for a method, applying dual-use promotion. Shared by
// header.py and source.py so the declaration and definition always agree.
py::str return_cpp_type(const MethodRef& method, const TypeResolver& resolver);

}  // namespace codebreaker::java::emit::ownership
