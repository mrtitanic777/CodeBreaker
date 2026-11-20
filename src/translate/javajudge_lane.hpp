// translate/javajudge_lane.hpp -- the java lane (verify/lanes/java) as the tree judge's TreeLane.
//
// The tree judge (javajudge.hpp) asks for a conversion with a body back end and then for each class's verdict. The lane does both:
// it transpiles into a dialect, compiles each class on the host compiler and runs it beside the JVM. What differs between the two
// back ends is the dialect (C++03 for the 360's, C++17), so it is given to the lane as an option, made new for each judgement: the
// judge holds no settings that outlive one.
//
//     assembly::Assembly assembly(java_data, front_end);
//     JavaTreeLane lane(tools, [&](std::string_view cpp_std) { return assembly.converter(std::string(cpp_std)); });
//     auto verdicts = javajudge::judge_tree(lane, java_src, work, Backend::Cpp17);
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "translate/javajudge.hpp"
#include "verify/lanes/java.hpp"

namespace cb::translate::javajudge {

// What a back end asks of the lane: the transpiler's label for the standard and the host compiler's `-std=` word.
struct Dialect {
    std::string_view label;      // "C++03", "C++17"
    std::string_view standard;   // "c++03", "c++17"
};
Dialect dialect_of(Backend backend);

class JavaTreeLane final : public TreeLane {
public:
    // The conversion of Java source into C++ of a dialect, by its label (java::assembly::Assembly::converter).
    using ConverterFor = std::function<verify::lanes::JavaLane::Convert(std::string_view cpp_std)>;

    JavaTreeLane(verify::lanes::JavaLane::ToolsFn tools, ConverterFor converter_for)
        : tools_(std::move(tools)), converter_for_(std::move(converter_for)) {}

    // Transpile `src` into <work>/cpp as the back end's dialect, build the lane's units; the classes it can judge, by name.
    std::vector<std::string> prepare(const std::filesystem::path& src, const std::filesystem::path& work, Backend backend) override;
    LaneResult judge(const std::string& cls) override;
    std::size_t gaps(const std::string& cls) override;

private:
    verify::lanes::JavaLane::ToolsFn tools_;
    ConverterFor converter_for_;
    std::unique_ptr<verify::lanes::JavaLane> lane_;
    std::unique_ptr<verify::lanes::Prepared> prepared_;
    verify::lanes::Built built_;
};

}  // namespace cb::translate::javajudge
