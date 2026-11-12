// Compile-only check of the shims at the old header paths of build/dos_tools and coverage/ledger: the old py:: surfaces over cb/build/dos_tools
// and cb/coverage/ledger. The ledger shim calls the old function store, the old corpus port and verify/reconstruct, which cannot be linked
// into cb_tests, so nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumers are refactored. (The shims
// are header-only: including them compiles every definition in them.)
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/coverage/ledger.hpp"
#include "codebreaker/exact/identify.hpp"
#include "codebreaker/exact/repair.hpp"

namespace shim_check_w3b5 {

inline void touch() {
    namespace d = codebreaker::build::dos_tools;
    (void)&d::emulator;
    (void)&d::run;
    (void)&d::masm_root;
    (void)&d::masm;
    namespace l = codebreaker::coverage::ledger;
    (void)&l::open_store;
    (void)&l::profiles;
    (void)&l::corpus_modules;
    (void)&l::extents;
    (void)&l::measure;
    (void)&l::reasons;
    (void)&l::render;
    (void)&l::write_csv;
    (void)&l::to_value;
    (void)&l::main;
    (void)&l::KINDS;
    (void)&l::TITLE;
    (void)&l::COUNTS;
    (void)&codebreaker::exact::repair::main;
    (void)&codebreaker::exact::repair::candidates;
    (void)&codebreaker::exact::repair::_shifts;
    (void)&codebreaker::exact::identify::main;
    (void)&codebreaker::exact::identify::score;
    (void)&codebreaker::exact::identify::candidates;
    (void)&codebreaker::exact::identify::default_loader;
}

}  // namespace shim_check_w3b5
