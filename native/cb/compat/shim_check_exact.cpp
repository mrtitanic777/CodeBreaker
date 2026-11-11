// Compile-only check of the exact shims that call old ports which cannot be linked into cb_tests (the habits comparers
// pascal_join's `run` resolves "equivalent" names to): the shim must keep compiling until its last consumer is refactored.
#include "codebreaker/exact/pascal_join.hpp"

namespace {
[[maybe_unused]] void use_pascal_join() {
    codebreaker::exact::pascal_join::Project project;
    (void)codebreaker::exact::pascal_join::run(project, false);
}
}  // namespace
