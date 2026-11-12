// cb/java/emit/ownership.hpp -- who owns what: parameter and return spellings, and the heap a class must free.
//
// Shared by the header emitter (to declare the Rule-of-Three destructor and the disabled copy) and the source emitter
// (to define the destructor and the call-site cleanup). Method bodies here are RAW JAVA (`this.field = new T()`,
// `Buf.createBuf(...)`): ownership is derived before the statements are translated, so the scans accept Java's `.` as
// well as C++'s `::` and `->`.
//
// The no-GC rule (the heap-corruption defence: an owning class with a destructor but a default copy constructor
// double-frees when copied, the classic 0xC0000018):
//   * a POINTER field is OWNED (this object must `delete` it) iff SOME method assigns it a fresh heap allocation
//     (`field = new T(...)` or an owning factory `X::createY(...)`) and NO method ever aliases it to a value of
//     unknown provenance (a parameter, another field, a plain getter) -- deleting a BORROWED pointer double-frees.
//     `= NULL` is neutral. Value-type and static fields are never owned.
//   * a METHOD returns an owned container / scalar when every element / the value is a fresh allocation, so the
//     caller owns it and frees it; a method that returns a field or a parameter (a borrowed view) never qualifies.
//   * a PARAMETER is retained by its method unless every use of it in the body is a plain read (`p.` or `p[`); a
//     temporary passed at a retained position must not be freed by the caller.
//
// Every answer is made from the IR and the resolver alone; nothing here is global.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>

#include "cb/core/ordered_map.hpp"
#include "cb/java/ir.hpp"
#include "cb/java/types.hpp"

namespace cb::java::emit::ownership {

using NameSet = std::set<std::string, std::less<>>;

// -- parameters -----------------------------------------------------------------------------------------------------
// Names of value-struct PARAMS used as nullable references in the body (compared with, or assigned, null). A C++ value
// cannot hold null, so such a param is really a reference and must be a pointer -- and callers in this corpus already
// pass the (promoted) pointer for these raytrace-style dual-use types (Vec3D / AxisAlignedBB / MovingObjectPosition).
// Primitives, String, containers and arrays are excluded (they have their own handling).
NameSet nullable_value_params(const ir::Method& method, const types::TypeResolver& resolver);

// Param names the body REASSIGNS (`p = ...`, `p += ...`, `p++`). Java lets a method reassign its parameter (a local
// copy of the reference or value); a C++ `const X&` parameter cannot be reassigned, so such params go by value.
NameSet reassigned_params(const ir::Method& method);

// The C++ parameter list of a method, promoting nullable value-struct params to pointers and passing a mutated
// collection by reference. The single source of truth: the header's declaration and the source's definition agree.
std::string params_str(const ir::Method& method, const types::TypeResolver& resolver);

// For a static field initialized as `new T[]{...}`, the raw brace contents. C++03 cannot `new T[]{...}`, so the emitters
// declare a fixed C array instead.
std::optional<std::string> aggregate_array_body(const ir::Field& field);

// -- what a class frees ---------------------------------------------------------------------------------------------
// {field name: is it allocated as an array (`new T[n]`)} for the POINTER instance fields this object owns and its
// destructor may safely `delete`; fields in the order they are first allocated (declaration initializers, then the
// methods in order).
OrderedMap<std::string, bool> owned_pointers(const ir::JClass& cls, const types::TypeResolver& resolver);

// {field name: the size accessor} for CONTAINER fields (a Java array or List of a reference type) whose EVERY element
// is a fresh allocation and never a borrowed one -- the destructor loop-deletes them. The accessor is `.length` (a
// JArray) or `.size()` (a JList). A single borrowed add disqualifies the field (a listener list that stores passed-in
// observers). Fields in the order their first allocating add appears.
OrderedMap<std::string, std::string> owned_containers(const ir::JClass& cls, const types::TypeResolver& resolver);

// True if the object owns ANY heap: such a class must forbid copying (Rule of Three).
bool owns_heap(const ir::JClass& cls, const types::TypeResolver& resolver);

// -- what a method hands back ---------------------------------------------------------------------------------------
// True iff the method returns a FRESHLY-built container whose every element is a fresh allocation, so the caller owns
// those elements. Conservative: any borrowed add, or returning a container not built locally, disqualifies it (no
// cleanup is emitted and the leak stays flagged).
bool returns_owned_container(const ir::Method& method, const types::TypeResolver& resolver);

// Program-wide: the NAMES of methods that return a freshly-built owned container. Unanimous -- a name is included only
// if some definition qualifies and no same-named definition returns a container that is not owned -- so matching a
// call site by method name alone stays safe.
NameSet owned_return_container_methods(std::span<const ir::JClass> classes, const types::TypeResolver& resolver);

// True iff EVERY non-null return of the method is a fresh `new X(...)` (the caller owns the result).
bool returns_owned_scalar(const ir::Method& method, const types::TypeResolver& resolver);

// {method name: C++ element type} of the methods that return a fresh heap scalar; unanimous across same-named
// definitions (a conflicting element type, or any non-owning definition, drops the name).
std::map<std::string, std::string, std::less<>> owned_scalar_return_methods(std::span<const ir::JClass> classes,
                                                                            const types::TypeResolver& resolver);

// {method name: parameter positions} that SOME definition retains, so a temporary passed there must not be freed. A
// name absent here retains nothing in any definition.
std::map<std::string, std::set<std::size_t>, std::less<>> retained_param_positions(std::span<const ir::JClass> classes);

// Whether the method's return should be a C++ pointer. Reference types always are. A VALUE type (Vec3D) is promoted
// when the body `return new <ReturnType>(...)` or returns null -- the dual-use case where a struct is stored by value
// as a field but heap-allocated on return. Arrays, containers and value-stdlib types (std::string) never are.
bool returns_pointer(const ir::Method& method, const types::TypeResolver& resolver);

// The C++ return type of a method with the dual-use promotion applied. Shared by the header and the source emitters
// so the declaration and the definition agree.
std::string return_cpp_type(const ir::Method& method, const types::TypeResolver& resolver);

}  // namespace cb::java::emit::ownership
