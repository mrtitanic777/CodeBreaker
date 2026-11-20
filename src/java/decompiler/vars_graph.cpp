// VarVersionEdge, VarVersionNode, VarVersionsGraph (de.fernflower.modules.decompiler.vars).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <deque>
#include <stdexcept>

#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

std::string VarVersionEdge::to_string() const { return source->to_string() + " ->" + std::to_string(type) + "-> " + dest->to_string(); }

VarVersionNode::VarVersionNode(int var, int version) : var(var), version(version), live(make<SFormsFastMapDirect>()) {}

std::vector<VarVersionNode*> VarVersionNode::get_predecessors() const {
    std::vector<VarVersionNode*> lst;
    lst.reserve(preds.size());
    for (VarVersionEdge* edge : preds) lst.push_back(edge->source);
    return lst;
}

VarVersionNode* VarVersionsGraph::create_node(const VarVersionPair& ver) {
    VarVersionNode* node = make<VarVersionNode>(ver.var, ver.version);
    nodes.add_with_key(node, ver);
    return node;
}

void VarVersionsGraph::add_nodes(const std::vector<VarVersionNode*>& colnodes, const std::vector<VarVersionPair>& colpaars) {
    for (std::size_t i = 0; i < colnodes.size(); i++) nodes.add_with_key(colnodes[i], colpaars.at(i));
}

bool VarVersionsGraph::is_dominator_set(VarVersionNode* node, const LinkedSet<VarVersionNode*>& domnodes) {
    if (domnodes.size() == 1) {
        if (!engine_) throw std::runtime_error("dominators not initialized");
        return engine_->is_dominator(node, domnodes.first());
    }

    std::unordered_set<VarVersionNode*> marked;

    if (domnodes.contains(node)) return true;

    std::deque<VarVersionNode*> lst_nodes;
    lst_nodes.push_back(node);

    while (!lst_nodes.empty()) {
        VarVersionNode* nd = lst_nodes.front();
        lst_nodes.pop_front();

        if (marked.contains(nd)) continue;
        marked.insert(nd);

        if (nd->preds.empty()) return false;

        for (VarVersionEdge* edge : nd->preds) {
            VarVersionNode* pred = edge->source;
            if (!marked.contains(pred) && !domnodes.contains(pred)) lst_nodes.push_back(pred);
        }
    }

    return true;
}

void VarVersionsGraph::init_dominators() {
    LinkedSet<VarVersionNode*> roots;

    for (VarVersionNode* node : nodes) {
        if (node->preds.empty()) roots.add(node);
    }

    engine_.emplace(get_reversed_post_order(roots), roots.items(), [](VarVersionNode* n) { return n->get_predecessors(); });
}

std::vector<VarVersionNode*> VarVersionsGraph::get_reversed_post_order(const LinkedSet<VarVersionNode*>& roots) {
    std::vector<VarVersionNode*> lst;
    std::unordered_set<VarVersionNode*> set_visited;

    for (VarVersionNode* root : roots) {
        std::vector<VarVersionNode*> lst_temp;
        add_to_reverse_post_order_list_iterative(root, lst_temp, set_visited);
        lst.insert(lst.end(), lst_temp.begin(), lst_temp.end());
    }

    return lst;
}

void VarVersionsGraph::add_to_reverse_post_order_list_iterative(VarVersionNode* root, std::vector<VarVersionNode*>& lst,
                                                                std::unordered_set<VarVersionNode*>& set_visited) {
    std::unordered_map<VarVersionNode*, std::vector<VarVersionEdge*>> map_node_succs;
    std::vector<VarVersionNode*> stack_node;
    std::vector<int> stack_index;
    std::vector<VarVersionNode*> post;   // the original adds each finished node at the front of lst: here at the back, reversed at the end

    stack_node.push_back(root);
    stack_index.push_back(0);

    while (!stack_node.empty()) {
        VarVersionNode* node = stack_node.back();
        int index = stack_index.back();
        stack_index.pop_back();

        set_visited.insert(node);

        auto it = map_node_succs.find(node);
        if (it == map_node_succs.end()) it = map_node_succs.emplace(node, node->succs.items()).first;
        const std::vector<VarVersionEdge*>& lst_succs = it->second;

        for (; index < static_cast<int>(lst_succs.size()); index++) {
            VarVersionNode* succ = lst_succs[static_cast<std::size_t>(index)]->dest;

            if (!set_visited.contains(succ)) {
                stack_index.push_back(index + 1);

                stack_node.push_back(succ);
                stack_index.push_back(0);

                break;
            }
        }

        if (index == static_cast<int>(lst_succs.size())) {
            post.push_back(node);
            stack_node.pop_back();
        }
    }

    lst.insert(lst.begin(), post.rbegin(), post.rend());
}

}  // namespace cb::java::decompiler
