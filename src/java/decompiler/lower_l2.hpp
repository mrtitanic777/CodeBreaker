// java/decompiler/lower_l2.hpp -- the decompiler's result as L2 (docs/JAVA_TO_CSHARP_L2.md, step 4): each class Fernflower decompiled,
// lowered from its final tree (the ClassNode, its wrapper's methods' statements and expressions) into the translator's IR, in the shape the
// javac front end gives for the same Java text (java/frontend.hpp), so the rule sets and back ends that read javac's units read these the
// same way, and decompiled Java reaches the C# back end without being printed and parsed again.
//
//     DecompileRequest request;
//     request.sources = {"game.jar"};
//     cb::Arena arena;
//     lower_l2::Lowered out = lower_l2::decompile(request, arena);
//     out.units;          // one ir::Unit per root class, lang "java", in the order the classes are written
//     out.text;           // the Java text, as decompile() gives it
//
// What the lowering does not carry is an Unsupported node with its reason (a method Fernflower could not decompile, an assert), never a guess;
// the units say so to whoever reads them (ir::unsupported).
//
// Not upstream code: Fernflower writes Java text only.
#pragma once

#include <vector>

#include "core/arena.hpp"
#include "java/decompiler/decompiler.hpp"
#include "translate/ir.hpp"

namespace cb::java::decompiler {

struct ClassNode;

namespace lower_l2 {

// One root class (with its nested classes) as an ir::Unit made in `arena`. Called while the class's tree is current: after ClassWriter wrote it,
// in the run's context (DecompilerContext::current()).
translate::ir::Node* lower_class(cb::Arena& arena, ClassNode* root);

struct Lowered {
    std::vector<translate::ir::Node*> units;
    DecompileResult text;
};

// Decompiles `request` as decompile() does, and lowers each root class written: its unit in `arena`.
Lowered decompile(const DecompileRequest& request, cb::Arena& arena);

}  // namespace lower_l2
}  // namespace cb::java::decompiler
