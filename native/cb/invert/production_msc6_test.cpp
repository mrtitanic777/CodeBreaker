// The free idiom of LZEXPAND (segment 11), with its far calls and a long global: the model reads it (the msc6 tool's "retail-free" line).
#include <iostream>

#include "cb/core/crypto.hpp"
#include "cb/core/testing.hpp"
#include "cb/invert/msc6.hpp"

using namespace cb;

CB_TEST("invert.production.msc6_reads_the_retail_free_function", [] {
    const Bytes code = crypto::from_hex(
        "1e589045558bec1e8ed883ec02833e7e00007420ff367e009affff0000ff367e009affff0000c7067e0000002bc0a3ee00a3ec00c606800000"
        "2bc0a3ee00a3ec00a3f200a3f000a3f600a3f400a3fa00a3f800a3fe00a3fc00a30201a300014d4d8be51f5d4dcb");
    invert::msc6::Context c;
    c.imports = {{0, "G"}, {1, "H"}};
    c.globals = {{0xec, {"gL_ec", invert::msc6_small::Width::Long}}};
    const auto r = invert::msc6::invert(code, c);
    CB_CHECK(r.ok());
    if (!r.ok()) std::cout << "declined: " << r.error().what() << "\n";
});
