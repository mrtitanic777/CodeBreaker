#include <algorithm>
#include <set>
#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/stubs.hpp"

namespace {

using namespace cb::java::stubs;

CB_TEST("java.stubs.classify", [] {
    CB_CHECK(classify_stub("GL11") != nullptr);
    CB_CHECK(classify_stub("GL11")->id == Subsystem::Rendering);
    CB_CHECK_EQ(std::string(classify_stub("SoundSystem")->key), "audio");
    CB_CHECK_EQ(std::string(classify_stub("Keyboard")->key), "input");
    CB_CHECK_EQ(std::string(classify_stub("GZIPInputStream")->key), "compression");
    CB_CHECK_EQ(std::string(classify_stub("Class")->key), "reflection");
    CB_CHECK_EQ(std::string(classify_stub("Sys")->key), "system");
    CB_CHECK_EQ(std::string(classify_stub("Class")->sdk_hint), "RTTI (typeid) + manual factory registry");
    CB_CHECK(classify_stub("NoSuchStub") == nullptr);
    CB_CHECK(classify_stub("") == nullptr);
    CB_CHECK(classify_stub("gl11") == nullptr);   // exact, case-sensitive
});

CB_TEST("java.stubs.first_claim_wins", [] {
    // ConnectException is listed under both networking and exceptions; networking comes first.
    CB_CHECK(classify_stub("ConnectException")->id == Subsystem::Networking);
    CB_CHECK(classify_stub("EOFException")->id == Subsystem::Exceptions);
});

CB_TEST("java.stubs.table_shape", [] {
    auto all = subsystems();
    CB_CHECK_EQ(all.size(), std::size_t{12});
    CB_CHECK_EQ(std::string(all.front().key), "rendering");
    CB_CHECK_EQ(std::string(all.back().key), "system");
    std::size_t members = 0;
    std::set<std::string> keys;
    for (const SubsystemInfo& s : all) {
        CB_CHECK(std::is_sorted(s.members.begin(), s.members.end()));   // the table is kept sorted
        CB_CHECK(!s.label.empty() && !s.description.empty() && !s.sdk_hint.empty());
        keys.emplace(s.key);
        members += s.members.size();
    }
    CB_CHECK_EQ(keys.size(), std::size_t{12});
    CB_CHECK_EQ(members, std::size_t{131});   // as the Python sets had them (ConnectException counted in both)
});

}  // namespace
