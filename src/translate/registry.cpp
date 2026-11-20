// translate/registry.cpp -- see registry.hpp: the registry of front ends and back ends, and the routes computed from it.
#include "translate/registry.hpp"

#include <algorithm>
#include <utility>

namespace cb::translate::registry {

Registry::Registry()
    : names_{{"c", "C"}, {"cpp03", "C++03"}, {"cpp17", "C++17"}, {"csharp", "C#"}, {"java", "Java"}, {"python", "Python"}} {}

void Registry::add(FrontEnd front_end) { front_ends_.push_back(std::move(front_end)); }
void Registry::add(BackEnd back_end) { back_ends_.push_back(std::move(back_end)); }

int Registry::index_of(std::string_view language) const {
    for (std::size_t i = 0; i < names_.size(); ++i)
        if (names_[i].id == language) return static_cast<int>(i);
    return static_cast<int>(names_.size());   // (an unnamed language sorts last)
}

const FrontEnd* Registry::front_end_for(std::string_view language) const {
    for (const FrontEnd& f : front_ends_)
        if (f.language == language) return &f;
    return nullptr;
}

bool Registry::has_back_end(std::string_view language) const {
    return std::any_of(back_ends_.begin(), back_ends_.end(), [&](const BackEnd& b) { return b.language == language; });
}

std::vector<Route> Registry::routes() const {
    std::vector<Route> out;
    for (const BackEnd& b : back_ends_) {
        for (const RuleSet& r : b.rules) {
            const FrontEnd* f = front_end_for(r.from);
            if (f == nullptr) continue;   // no front end reads the source language: no route
            out.push_back(Route{r.from, b.language, b.emitter, r.judge, r.proof, r.needs, f->tool, r.file_level});
        }
    }
    std::stable_sort(out.begin(), out.end(), [&](const Route& a, const Route& b) {
        return std::pair(index_of(a.from), index_of(a.to)) < std::pair(index_of(b.from), index_of(b.to));
    });
    return out;
}

std::vector<Route> Registry::file_routes() const {
    std::vector<Route> out = routes();
    std::erase_if(out, [](const Route& r) { return !r.file_level; });
    return out;
}

const Registry& standard() {
    static const Registry registry = [] {
        Registry r;
        // The one list of modules. Each adds its own piece; a new front end or back end is a line here and a registration in its own folder.
        register_clang_front_end(r);
        register_csharp_front_end(r);
        register_java_front_end(r);
        register_python_front_end(r);
        register_c_back_end(r);
        register_cpp03_back_end(r);
        register_cpp17_back_end(r);
        register_csharp_back_end(r);
        register_csharp_cpp17_back_end(r);
        register_java_back_end(r);
        return r;
    }();
    return registry;
}

}  // namespace cb::translate::registry
