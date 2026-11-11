#include "cb/platforms/java.hpp"

namespace cb::platforms {

Platform java() {
    Platform p;
    p.id = Id::Java;
    p.name = "java";
    p.title = "Java";
    p.family = "bytecode";
    p.source = SourceLanguage::JavaToCxx;
    p.lane = "java";
    p.steps = Steps::Lanes;
    p.loader.container = "source folder, jar";
    p.cpu.decoder = OwnDecoder::Jar;   // (Java has no SLEIGH language, word size, registers or stack)
    p.convention.names = {"the JVM's"};
    p.convention.order = CallOrder::C;
    p.convention.pops = Pops::Callee;
    p.emulator.engine = Engine::Jvm;   // run by the JVM itself, not by a runner of ours
    p.compiler = {Language::Cxx, "the host C++ compiler (host_cxx, else g++)"};
    return p;
}

}  // namespace cb::platforms
