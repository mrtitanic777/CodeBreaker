// main() of cb_tests: runs every registered test (see testing.hpp for the options).
#include "cb/core/testing.hpp"
#include "cb/core/win.hpp"

int main(int argc, char** argv) {
    cb::win::use_utf8_console();
    return cb::testing::run_all(argc, argv);
}
