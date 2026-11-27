// translate/support/l2schema.hpp -- the L2 schema check (config/translate/l2.schema.json; draft 2020-12, the keywords it uses) on a
// document read as Python's json.loads reads it (py::V), with jsonschema's words for why a document is not L2.
//
//     l2schema::Schema s(py::json_parse(schema_text));
//     std::string why; bool ok = s.valid(py::json_parse(doc_text), &why);
#pragma once
#include <string>

#include "translate/support/pyvalue.hpp"

namespace l2schema {

class Schema {
public:
    explicit Schema(const py::V& schema);
    bool valid(const py::V& doc, std::string* why = nullptr) const;

private:
    py::V root_;
    bool check(const py::V& s, const py::V& v, std::string& why, const std::string& at) const;
    const py::V& resolve(const std::string& ref) const;
};

}  // namespace l2schema
