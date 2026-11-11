// Tests of cb/invert/grammars.hpp. The candidate texts were checked by hand against what the compiler oracle accepts
// (the thunk and buffer-read idioms of OLECLI) and against the Python generators' output for the same shapes.
#include "cb/invert/grammars.hpp"

#include <set>

#include "cb/core/testing.hpp"

using namespace cb::invert;

CB_TEST("invert.grammars.thunk_orders_and_conventions", [] {
    const auto c = grammars::thunk({.nargs = 3});
    CB_CHECK_EQ(c.size(), std::size_t{4});
    CB_CHECK_EQ(c[0], "extern int far SIB();\nint far pascal F(int a0, int a1, int a2){ return SIB(a0, a1, a2); }");
    CB_CHECK_EQ(c[1], "extern int far SIB();\nint far pascal F(int a0, int a1, int a2){ return SIB(a2, a1, a0); }");
    CB_CHECK_EQ(c[2], "extern int far SIB();\nint far F(int a0, int a1, int a2){ return SIB(a0, a1, a2); }");
    CB_CHECK_EQ(c[3], "extern int far SIB();\nint far F(int a0, int a1, int a2){ return SIB(a2, a1, a0); }");
    CB_CHECK_EQ(grammars::thunk().size(), std::size_t{4});   // the default shape is three arguments
    CB_CHECK(grammars::thunk() == c);
});

CB_TEST("invert.grammars.thunk_small_shapes", [] {
    const auto none = grammars::thunk({.nargs = 0});
    CB_CHECK_EQ(none.size(), std::size_t{4});
    CB_CHECK_EQ(none[0], "extern int far SIB();\nint far pascal F(){ return SIB(); }");
    CB_CHECK_EQ(none[0], none[1]);   // no arguments: the two orders read the same
    CB_CHECK_EQ(none[3], "extern int far SIB();\nint far F(){ return SIB(); }");
    const auto two = grammars::thunk({.nargs = 2});
    CB_CHECK_EQ(two[1], "extern int far SIB();\nint far pascal F(int a0, int a1){ return SIB(a1, a0); }");
    const auto one = grammars::thunk({.nargs = 1});
    CB_CHECK_EQ(one[2], "extern int far SIB();\nint far F(int a0){ return SIB(a0); }");
});

CB_TEST("invert.grammars.bufread_covers_word_and_long", [] {
    const auto c = grammars::bufread();
    CB_CHECK_EQ(c.size(), std::size_t{12});
    CB_CHECK_EQ(c[0],
                "extern int far SIB();\nint far F(int p, unsigned *out){ unsigned char b[2]; if (SIB(p,(char far*)b,2)){ "
                "*out=b[0]|(b[1]<<8); return 0xffff; } return 0; }");
    CB_CHECK_EQ(c[3],
                "extern int far SIB();\nint far F(int p, unsigned *out){ unsigned char b[2]; if (!SIB(p,(char far*)b,2)) "
                "return 0; *out=*(unsigned*)b; return 0xffff; }");
    CB_CHECK_EQ(c[11],
                "extern int far SIB();\nint far F(int p, long *out){ unsigned char b[6]; long v; if (!SIB(p,(char "
                "far*)b,4)) return 0; v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0]; *out=v; return 0xffff; }");
    const std::set<std::string> distinct(c.begin(), c.end());
    CB_CHECK_EQ(distinct.size(), c.size());
    CB_CHECK(grammars::bufread({.nargs = 9}) == c);   // the shape is not read
});

CB_TEST("invert.grammars.registry", [] {
    const auto all = grammars::all();
    CB_CHECK_EQ(all.size(), std::size_t{2});
    CB_CHECK_EQ(std::string(all[0].name), "thunk");
    CB_CHECK_EQ(std::string(all[1].name), "bufread");
    CB_CHECK_EQ(all[0].generate({}).size(), std::size_t{4});
    CB_CHECK_EQ(all[1].generate({}).size(), std::size_t{12});
});
