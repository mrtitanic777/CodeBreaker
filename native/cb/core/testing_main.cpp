// main() of cb_tests: runs every registered test (see testing.hpp for the options).
#include "cb/core/testing.hpp"
#include "cb/core/sys.hpp"

int main(int argc, char** argv) {
    cb::sys::use_utf8_console();
    return cb::testing::run_all(argc, argv);
}
