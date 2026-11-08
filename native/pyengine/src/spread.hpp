// spread.hpp -- f(..., **record) seen as f(..., k=record['k'] ...): spread.cc
#pragma once
#include <string>
#include <vector>

#include "pyfront/ast.hpp"

namespace pyengine {

// a rewritten call: the inference refuses it unless var is a record of exactly these keys, all always there
struct SpreadCheck {
    const pyfront::Call* call = nullptr;
    const pyfront::Name* var = nullptr;
    std::vector<std::string> keys;
};

void spread_record_keywords(pyfront::Module& tree, std::vector<SpreadCheck>& out);

}  // namespace pyengine
