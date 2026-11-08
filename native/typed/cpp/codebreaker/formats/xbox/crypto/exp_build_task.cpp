// codebreaker/formats/xbox/crypto/exp_build_task.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.exp_build_task.

#include "codebreaker/formats/xbox/crypto/exp_build_task.hpp"

#include <optional>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/bin2lang.hpp"
#include "codebreaker/formats/xbox/crypto/build_lib.hpp"
#include "codebreaker/formats/xbox/crypto/exp_signer.hpp"

namespace codebreaker::formats::xbox::crypto::exp_build_task {

using bin2lang::Language;
using bin2lang::lang_format;
using build_lib::assemble_devkit_patch;
using build_lib::assemble_patch;
using build_lib::assemble_retail_patch;
using exp_signer::ExpansionMagic;
using exp_signer::sign_exp;

namespace {

const py::str KEY = py::str("Keys/HVX_prv.bin");             // sign_exp's default key file

// sign_exp(in, out, exp_magic=..., encrypt=..., exp_id=...)
void sign(const char* in_file, const char* out_file, ExpansionMagic magic, bool encrypt, int64_t exp_id) {
    sign_exp(py::str(in_file), std::optional<py::str>(py::str(out_file)), KEY, magic, exp_id, encrypt);
}

// lang_format(in, out, Language.CPLUSPLUS, "ExpansionData")
void format(const char* in_file, const char* out_file) {
    lang_format(py::str(in_file), std::optional<py::str>(py::str(out_file)), Language::CPLUSPLUS, py::str("ExpansionData"));
}

}  // namespace

int64_t main() {
    // assemble HVPP
    py::print(py::str("Assembling HVPP.S..."));
    assemble_patch(py::str("Patches/HVPP.S"), py::str("Output/Compiled/HVPP.bin"));

    py::print(py::str("Assembling devkit Spoofy.S..."));
    assemble_devkit_patch(py::str("Patches/Spoofy.S"), py::str("Output/Compiled/Spoofy_dev.bin"));
    py::print(py::str("Assembling retail Spoofy.S..."));
    assemble_retail_patch(py::str("Patches/Spoofy.S"), py::str("Output/Compiled/Spoofy_retail.bin"));

    // generic
    // spoofy
    py::print(py::str("Creating generic Spoofy..."));
    sign("Output/Compiled/Spoofy_dev.bin", "Output/Compiled/Generic/Spoofy_signed.bin", ExpansionMagic::HXPR, true, DEFAULT_EXP_ID);
    format("Output/Compiled/Generic/Spoofy_signed.bin", "Output/Compiled/Generic/Spoofy.h");

    // devkit
    py::print(py::str("Creating generic devkit HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/Generic/HVPP_dev_signed.bin", ExpansionMagic::HXPR, true, DEFAULT_EXP_ID);
    format("Output/Compiled/Generic/HVPP_dev_signed.bin", "Output/Compiled/Generic/HVPP_dev.h");

    // test kit
    py::print(py::str("Creating generic test kit HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/Generic/HVPP_test_signed.bin", ExpansionMagic::SIGM, true, DEFAULT_EXP_ID);
    format("Output/Compiled/Generic/HVPP_test_signed.bin", "Output/Compiled/Generic/HVPP_test.h");

    // retail
    py::print(py::str("Creating generic retail HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/Generic/HVPP_retail_signed.bin", ExpansionMagic::HXPR, false, DEFAULT_EXP_ID);
    format("Output/Compiled/Generic/HVPP_retail_signed.bin", "Output/Compiled/Generic/HVPP_retail.h");

    // xbOnline
    // spoofy
    py::print(py::str("Creating xbOnline Spoofy..."));
    sign("Output/Compiled/Spoofy_dev.bin", "Output/Compiled/xbOnline/Spoofy_signed.bin", ExpansionMagic::HXPR, true, XBONLINE_EXP_ID);
    format("Output/Compiled/xbOnline/Spoofy_signed.bin", "Output/Compiled/xbOnline/Spoofy.h");

    // devkit
    py::print(py::str("Creating xbOnline devkit HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/xbOnline/HVPP_dev_signed.bin", ExpansionMagic::HXPR, true, XBONLINE_EXP_ID);
    format("Output/Compiled/xbOnline/HVPP_dev_signed.bin", "Output/Compiled/xbOnline/HVPP_dev.h");

    // test kit
    py::print(py::str("Creating xbOnline test kit HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/xbOnline/HVPP_test_signed.bin", ExpansionMagic::SIGM, true, XBONLINE_EXP_ID);
    format("Output/Compiled/xbOnline/HVPP_test_signed.bin", "Output/Compiled/xbOnline/HVPP_test.h");

    // retail
    py::print(py::str("Creating xbOnline retail HVPP..."));
    sign("Output/Compiled/HVPP.bin", "Output/Compiled/xbOnline/HVPP_retail_signed.bin", ExpansionMagic::HXPR, false, XBONLINE_EXP_ID);
    format("Output/Compiled/xbOnline/HVPP_retail_signed.bin", "Output/Compiled/xbOnline/HVPP_retail.h");

    py::print(py::str("Done!"));

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::exp_build_task
