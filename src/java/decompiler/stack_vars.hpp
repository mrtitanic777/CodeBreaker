// java/decompiler/stack_vars.hpp -- the stack variables of a method's code: a value the bytecode held on the stack (varN with N >= VarExprent::STACK_BASE)
// that is made by one expression and used by one other is folded into the place of its use, and an assignment nobody reads is dropped or kept only for its
// effect, so `var10000 = a + b; return var10000;` becomes `return a + b;`.
//
//     StackVarsProcessor().simplify_stack_vars(root, method, cls);   // repeats SSA / simplify / inline until nothing changes
//
// Ported from Fernflower (de.fernflower.modules.decompiler.StackVarsProcessor), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in
// this folder).
//
// Notes on the port:
//  * The original's HashMap<VarVersionPair, Exprent> that is only looked up is a std::map; HashSet<VarVersionPair> that the code iterates (and whose
//    order decides the order expressions are copied in: each copy takes an id) is a VarVersionSet (Java's order). HashMap<Integer, HashSet<VarVersionPair>>
//    keeps a null set for the variable assigned itself (std::nullopt).
//  * The original's int[] / Object[] results are small structs.
#pragma once

#include <map>
#include <optional>
#include <set>
#include <vector>

#include "java/decompiler/exprents.hpp"
#include "java/decompiler/fast_sets.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

class StructMethod;
class StructClass;

class StackVarsProcessor {
public:
    // (the set of reordered ifs it makes here and hands to SimplifyExprentsHelper::simplify_stack_vars_statement is a std::set<int>)
    void simplify_stack_vars(RootStatement* root, const StructMethod* mt, const StructClass* cl);

private:
    using VarValueMap = std::map<VarVersionPair, Exprent*>;                          // a version -> the expression to put in its place
    using VarVersionsMap = IntHashMap<std::optional<VarVersionSet>>;                  // a variable -> the versions of it (null: assigned by the statement)
    struct IterateResult {   // the original's int[]{next index (-1: go on), changed}
        int index;
        int changed;
    };
    struct ChildResult {   // the original's Object[]{expression to put in, changed, replaceable}
        Exprent* exprent;
        bool changed;
        bool replaceable;
    };

    static void set_versions_to_null(Statement* stat);
    static void set_exprent_versions_to_null(Exprent* exprent);
    bool iterate_statements(RootStatement* root, SSAUConstructorSparseEx* ssa);
    static Exprent* is_replaceable_var(Exprent* exprent, const VarValueMap& map_var_values, SSAUConstructorSparseEx* ssau);
    static void replace_single_var(Exprent* parent, VarExprent* var, Exprent* dest, SSAUConstructorSparseEx* ssau);
    IterateResult iterate_exprent(std::vector<Exprent*>& lst_exprents, int index, Exprent* next, VarValueMap& map_var_values, SSAUConstructorSparseEx* ssau);
    static VarVersionSet get_all_versions(Exprent* exprent);
    static ChildResult iterate_child_exprent(Exprent* exprent, Exprent* parent, Exprent* next, VarValueMap& map_var_values, SSAUConstructorSparseEx* ssau);
    // The versions that use `var`'s (res); true when some use is not dominated by the assignment.
    static bool get_used_versions(SSAUConstructorSparseEx* ssa, const VarVersionPair& var, std::vector<VarVersionNode*>& res);
    static bool is_version_to_be_replaced(const VarVersionPair& usedvar, const VarVersionsMap& map_vars, SSAUConstructorSparseEx* ssau,
                                          const VarVersionPair& leftpaar);
    // Throws std::runtime_error("inkonsistent live map!") as the original.
    static VarVersionsMap get_all_var_versions(const VarVersionPair& leftvar, Exprent* exprent, SSAUConstructorSparseEx* ssau);
};

}  // namespace cb::java::decompiler
