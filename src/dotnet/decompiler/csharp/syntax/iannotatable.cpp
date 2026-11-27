// dotnet/decompiler/csharp/syntax/iannotatable.cpp -- see iannotatable.hpp.
#include "dotnet/decompiler/csharp/syntax/iannotatable.hpp"

#include <algorithm>
#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

void AbstractAnnotatable::add_annotation(ArenaObject* annotation) {
    if (annotation == nullptr) throw std::invalid_argument("annotation");
    annotations_.push_back(annotation);
}

ArenaObject* AbstractAnnotatable::find_annotation(const std::function<bool(const ArenaObject*)>& matches) const {
    for (ArenaObject* a : annotations_)
        if (matches(a)) return a;
    return nullptr;
}

void AbstractAnnotatable::remove_annotations_where(const std::function<bool(const ArenaObject*)>& matches) {
    std::erase_if(annotations_, [&](ArenaObject* a) { return matches(a); });
}

void AbstractAnnotatable::clone_annotations() {
    for (ArenaObject*& a : annotations_) {
        if (auto* cloneable = dynamic_cast<ICloneable*>(a)) a = cloneable->clone_object();
    }
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
