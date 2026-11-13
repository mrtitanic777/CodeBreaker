// cb/formats/xbox/crypto/flashplan.hpp -- the RGH/JTAG flash planner: an advisor, not a builder.
//
//     flashplan::Plan plan = flashplan::plan("Corona 4G", "RGH 3");     // image type glitch3, secure-boot block KSB
//     flashplan::cpld_part(0x06E5E093);                                 // "XC2C64A (CoolRunner-II)"
//
// Given a console model and the intended hack, it says which xeBuild *image type* and *secure-boot block* a build needs, and names a
// glitch-chip CPLD by its JTAG IDCODE. It writes no image (that is xeBuild's work; a wrong image bricks the console). The mappings are
// reimplemented clean-room from Genexis.
//
// The command `crypto flashplan` has the operations plan, image-type, secure-boot, cpld, motherboards and hacks.
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>

#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::flashplan {

enum class Motherboard { Xenon, Zephyr, Falcon, Opus, Jasper, Trinity, Corona, Winchester, Unknown };

// The xeBuild image types a console class and hack resolve to.
enum class ImageType { Retail, Glitch, Glitch2, Glitch3, Jtag, DevGl, DevRgl, Devkit };

// The secure-boot block an image needs by motherboard.
enum class SecureBootBlock { Ksb, Psb, Xsb };

struct Plan {
    Motherboard motherboard = Motherboard::Unknown;
    std::string console_class;
    std::string hack;
    ImageType image_type = ImageType::Retail;
    SecureBootBlock secure_boot_block = SecureBootBlock::Xsb;
    bool nandmu = false;
};

std::string_view name(Motherboard board);          // lower case: "corona", "unknown"
std::string_view name(ImageType type);             // "glitch3"
std::string_view name(SecureBootBlock block);      // "KSB"

// The board family a model string names (a substring, case ignored).
Motherboard classify_motherboard(std::string_view model);

// The image type for a console class ("Retail", "FreeBoot"/"RGH"/"JTAG", "Devkit"; case ignored) and a hack name ("RGH 1", "RGH 3", "DevGL" ...).
// FreeBoot with a hack that is none of the known groups is glitch2 (RGH 1.2 / RGH 2 / S-RGH), a Devkit one is a plain devkit; anything
// else is retail.
ImageType image_type(std::string_view console_class, std::string_view hack);

// Corona 4G (or a build with a NAND memory unit) takes KSB; Jasper and Trinity PSB; the rest XSB.
SecureBootBlock secure_boot_block(std::string_view model, bool nandmu = false);

// The glitch chip an IDCODE belongs to ("Unknown JTAG device" when it is none of the known).
std::string_view cpld_part(std::uint32_t idcode);

Plan plan(std::string_view model, std::string_view hack, std::string_view console_class = "FreeBoot", bool nandmu = false);

// The names of the boards, and of the hacks in each image type's group, in the order the tool lists them.
std::span<const Motherboard> motherboards();

cli::Command command();

}  // namespace cb::formats::xbox::crypto::flashplan
