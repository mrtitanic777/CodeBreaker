#include "java/assembly.hpp"

#include <utility>

#include "java/projects/minecraft/package.hpp"

namespace cb::java::assembly {

namespace stdfs = std::filesystem;

namespace {

// The translator's front stage: the front end's units, then the project's IR fixes on them.
translate::backends::cpp03::Lowering assembly_lowering(FrontEnd front_end) {
    return [front_end = std::move(front_end)](Arena& arena, const transpile::TranslateRequest& request) {
        translate::backends::cpp03::Lowered lowered;
        lowered.units = front_end(arena, request.java_dir);
        lowered.patched = request.project.ir_patches(arena, lowered.units, request.platform);
        return lowered;
    };
}

}  // namespace

Assembly::Assembly(JavaData data, FrontEnd front_end)
    : data_(std::move(data)),
      runtime_(data_),
      projects_(data_.path({"projects"})),
      translator_(runtime_, assembly_lowering(std::move(front_end))) {
    projects::minecraft::register_profiles(profiles_);
    projects_.add("minecraft", std::make_unique<projects::minecraft::Minecraft>(data_.path({"platform", "reference"})));
}

transpile::Summary Assembly::convert(const stdfs::path& java_dir, const stdfs::path& out_dir, const transpile::Options& options) {
    return transpile::convert(java_dir, out_dir, options, services());
}

Converter Assembly::converter(std::string cpp_std) {
    return [this, cpp_std = std::move(cpp_std)](const stdfs::path& java_dir, const stdfs::path& out_dir) {
        transpile::Options options;
        options.cpp_std = cpp_std;
        options.write_report = false;
        return convert(java_dir, out_dir, options);
    };
}

}  // namespace cb::java::assembly
