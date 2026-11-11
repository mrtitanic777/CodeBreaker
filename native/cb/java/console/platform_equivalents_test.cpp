#include <set>
#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/console/platform_equivalents.hpp"

namespace {

using namespace cb::java::console::platform_equivalents;

CB_TEST("java.console.platform_equivalents.table_shape", [] {
    // The table as the Python module held it: ten subsystems, 45 entries (22 real, 18 stub, 5 dead).
    const auto subs = subsystems();
    CB_CHECK_EQ(subs.size(), std::size_t{10});
    CB_CHECK_EQ(subs[0].name, "video");
    CB_CHECK_EQ(subs[9].name, "image");
    std::set<std::string> seen;
    std::size_t real = 0, stub = 0, dead = 0, hand = 0, xdk = 0;
    for (const Subsystem& s : subs) {
        CB_CHECK(!s.entries.empty());
        for (const Equiv& e : s.entries) {
            CB_CHECK(!e.basename.empty());
            CB_CHECK(!e.xdk.empty());
            CB_CHECK(!e.lib.empty());
            CB_CHECK(seen.insert(std::string(e.basename)).second);     // a basename names one entry
            real += e.status == Status::Real;
            stub += e.status == Status::Stub;
            dead += e.status == Status::Dead;
            hand += kind(e) == Kind::Handwritten;
            xdk += kind(e) == Kind::Xdk;
        }
    }
    CB_CHECK_EQ(seen.size(), std::size_t{45});
    CB_CHECK_EQ(real, std::size_t{22});
    CB_CHECK_EQ(stub, std::size_t{18});
    CB_CHECK_EQ(dead, std::size_t{5});
    CB_CHECK_EQ(hand, std::size_t{8});
    CB_CHECK_EQ(xdk, std::size_t{32});
});

CB_TEST("java.console.platform_equivalents.lookup", [] {
    const Equiv* gl = lookup("GL11");
    CB_CHECK(gl != nullptr);
    CB_CHECK_EQ(gl->lib, "d3d9.lib");
    CB_CHECK(gl->status == Status::Real);
    CB_CHECK(gl->methods.empty());

    const Equiv* glu = lookup("GLU");
    CB_CHECK(glu != nullptr && glu->methods.size() == 3);
    CB_CHECK_EQ(glu->methods[0].method, "gluPerspective");
    CB_CHECK_EQ(glu->methods[2].detail, "no equiv -- returns \"\" (log only)");

    CB_CHECK(lookup("Socket") != nullptr);
    CB_CHECK(lookup("socket") == nullptr);        // names are case-sensitive
    CB_CHECK(lookup("") == nullptr);
    CB_CHECK(lookup("NoSuchClass") == nullptr);
    CB_CHECK_EQ(status_name(Status::Real), "real");
    CB_CHECK_EQ(status_name(Status::Stub), "stub");
    CB_CHECK_EQ(status_name(Status::Dead), "dead");
});

CB_TEST("java.console.platform_equivalents.annotation", [] {
    // an XDK function exists: resolve the stub to it
    CB_CHECK_EQ(annotation("Socket"),
                "// [360] equivalent: XNetStartup(BYPASS_SECURITY) + socket/connect/send/recv (Winsock)  (xnet.lib)");
    // hand-implemented: outer parentheses of the description are trimmed
    CB_CHECK_EQ(annotation("DataInputStream"),
                "// [360] NO XDK EQUIVALENT -- hand-implemented: pure big-endian byte codec -- no XDK");
    CB_CHECK_EQ(annotation("Inflater"),
                "// [360] NO XDK EQUIVALENT -- hand-implemented: NO XDK zlib -- bundled puff.c (Inflate.cpp: zlibInflate/rawInflate");   // (strip("()") takes the closing parenthesis of the description too, as the original did)
    // dead on console: the note says why (an entry with no note says nothing after the colon)
    CB_CHECK_EQ(annotation("HttpURLConnection"),
                "// [360] no equivalent -- dead on console: Only the minecraft.net asset/skin downloader -- dead offline.");
    CB_CHECK_EQ(annotation("CodecWav"), "// [360] no equivalent -- dead on console: ");
    // nothing known: the emitter leaves the stub alone
    CB_CHECK_EQ(annotation("Frobnicator"), "");
});

CB_TEST("java.console.platform_equivalents.kinds", [] {
    CB_CHECK(kind(*lookup("GL11")) == Kind::Xdk);
    CB_CHECK(kind(*lookup("BufferUtils")) == Kind::Handwritten);    // (CRT)
    CB_CHECK(kind(*lookup("GZIPInputStream")) == Kind::Handwritten);   // (bundled)
    CB_CHECK(kind(*lookup("DataOutputStream")) == Kind::Handwritten);  // (none)
    CB_CHECK(kind(*lookup("CodecJOrbis")) == Kind::Dead);               // dead wins over the lib
    CB_CHECK(kind(Status::Real, "xnet.lib") == Kind::Xdk);
    CB_CHECK(kind(Status::Stub, "(bundled)") == Kind::Handwritten);
    CB_CHECK(kind(Status::Dead, "(none)") == Kind::Dead);
    CB_CHECK(kind(Status::Real, "") == Kind::Xdk);                      // an entry with no lib at all is still an XDK call
    CB_CHECK_EQ(kind_name(Kind::Xdk), "xdk");
    CB_CHECK_EQ(kind_name(Kind::Handwritten), "handwritten");
    CB_CHECK_EQ(kind_name(Kind::Dead), "dead");
    CB_CHECK(parse_status("real") == Status::Real && parse_status("stub") == Status::Stub && parse_status("dead") == Status::Dead);
    CB_CHECK(!parse_status("Real") && !parse_status(""));
});

}  // namespace
