// Compile-only check of the shims at the old header paths for analyze/facts and build/resources (W2-B3): the surface the old ports
// call, over native/cb. Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is
// refactored.
#include "codebreaker/analyze/facts.hpp"
#include "codebreaker/build/resources.hpp"

namespace codebreaker::shim_check {

py::str w2b3_shims(const py::str& path, const py::list<formats::resources::Resource>& original) {
    py::str out;
    {
        namespace facts = analyze::facts;
        const std::shared_ptr<facts::FactStore> store = facts::open(path);
        facts::Json value = facts::loads(py::str("{\"a\": [1, 2]}"));
        py::list<facts::Row> rows;
        rows.append(facts::Row(py::str("401000"), py::str("convention"), value));
        store->put(py::str("m"), py::str("conventions"), rows);
        for (const facts::Fact& f : store->get(py::str("m"), py::str("conventions"), std::nullopt, py::str("convention")).vec())
            out += f.addr + facts::dumps(f.value);
        out += facts::key(py::str("ne"), 0x10000010);
        out += py::str(std::to_string(store->tally(py::str("m")).size()));
        store->close();
    }
    {
        namespace resources = build::resources;
        resources::Result r = resources::rebuild(path, path, 3);
        out += r.line() + py::str(std::to_string(r.same() ? 1 : 0));
        resources::Result c = resources::compare(original, resources::read_res(path, resources::WIDE), resources::UTF8_SOURCE, r);
        out += c.error;
        out += py::str(std::to_string(resources::main(py::list<py::str>{path})));
    }
    return out;
}

}  // namespace codebreaker::shim_check
