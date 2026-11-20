// fixes_constants.cpp -- Transform.replaceCommonConstants, RDI's entry point: the pass itself (each ldc or sipush offered to every constant of
// ConstantPool in turn, the first that replaces it wins) is area D's prep::replace_common_constants (prep/constants.hpp); this is Transform's name for it.
//
// Ported from RDI (org.mcphackers.rdi.injector.transform.Transform), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/constants.hpp"
#include "java/projects/minecraft/prep/fixes.hpp"

namespace cb::java::minecraft::prep {

void Transform::replace_common_constants(ClassStorage& storage) { prep::replace_common_constants(storage); }

}  // namespace cb::java::minecraft::prep
