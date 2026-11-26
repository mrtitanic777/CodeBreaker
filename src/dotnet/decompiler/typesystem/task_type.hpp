// dotnet/decompiler/typesystem/task_type.hpp -- Task, Task<T> and custom task types ([AsyncMethodBuilder]): what an async method
// returns, unpacked to the type awaited, and the builder type a task type is built with.
//
//     const IType* result = task_type::unpack_any_task(compilation, method->return_type());   // Task<int> -> int, Task -> void
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TaskType), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem::task_type {

// Task<T> -> T, Task -> void, anything else unchanged.
const IType* unpack_task(const ICompilation& compilation, const IType* type);
// As unpack_task, also for custom task types.
const IType* unpack_any_task(const ICompilation& compilation, const IType* type);
bool is_task(const IType* type);
// A type with [AsyncMethodBuilder(typeof(Builder))] and at most one type parameter; *builder_type gets Builder.
bool is_custom_task(const IType* type, const IType** builder_type);
bool is_non_generic_task_type(const IType* task, FullTypeName* builder_type_name);
bool is_generic_task_type(const IType* task, FullTypeName* builder_type_name);
// Task for void, Task<element_type> otherwise.
const IType* create(const ICompilation& compilation, const IType* element_type);

}  // namespace cb::dotnet::decompiler::typesystem::task_type
