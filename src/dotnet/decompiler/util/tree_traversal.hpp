// dotnet/decompiler/util/tree_traversal.hpp -- pre-order and post-order walks of a tree given by a function from a node to its children,
// calling `visit` on each node in walk order. The children function runs as upstream's lazy walk calls it (a node's children are
// asked for just after the node is reached), so a children function with side effects sees the same order as upstream.
//
//     post_order(root, [](Node* n) { return n->children; }, [](Node* n) { ... });
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: TreeTraversal), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <cstddef>
#include <optional>
#include <vector>

namespace cb::dotnet::decompiler::util {

// Children: a callable returning std::optional<std::vector<T>> (nullopt: a leaf, upstream's null) or std::vector<T> (never a leaf).
template <class T, class Children, class Visit>
void pre_order(const std::vector<T>& input, Children recursion, Visit visit) {
    struct Frame {
        std::vector<T> items;
        std::size_t next = 0;
    };
    std::vector<Frame> stack;
    stack.push_back({input, 0});
    while (!stack.empty()) {
        while (stack.back().next < stack.back().items.size()) {
            T element = stack.back().items[stack.back().next++];
            visit(element);
            auto children = recursion(element);
            if constexpr (requires { children.has_value(); }) {
                if (children) stack.push_back({std::move(*children), 0});
            } else {
                stack.push_back({std::move(children), 0});
            }
        }
        stack.pop_back();
    }
}

template <class T, class Children, class Visit>
void pre_order(const T& root, Children recursion, Visit visit) {
    pre_order(std::vector<T>{root}, recursion, visit);
}

template <class T, class Children, class Visit>
void post_order(const std::vector<T>& input, Children recursion, Visit visit) {
    struct Frame {
        std::vector<T> items;
        std::size_t next = 0;  // the index after the current item
    };
    std::vector<Frame> stack;
    stack.push_back({input, 0});
    while (!stack.empty()) {
        while (stack.back().next < stack.back().items.size()) {
            T element = stack.back().items[stack.back().next++];
            auto children = recursion(element);
            if constexpr (requires { children.has_value(); }) {
                if (children)
                    stack.push_back({std::move(*children), 0});
                else
                    visit(element);
            } else {
                stack.push_back({std::move(children), 0});
            }
        }
        stack.pop_back();
        if (!stack.empty()) visit(stack.back().items[stack.back().next - 1]);
    }
}

template <class T, class Children, class Visit>
void post_order(const T& root, Children recursion, Visit visit) {
    post_order(std::vector<T>{root}, recursion, visit);
}

}  // namespace cb::dotnet::decompiler::util
