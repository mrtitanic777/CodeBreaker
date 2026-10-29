"""Java: a folder of source (or a .jar), translated to C++ (codebreaker/java) and judged by
running both under the JVM."""
import os

from codebreaker.platforms import Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform, Ref


def _recognise(path):
    if path.lower().endswith(".jar"):
        return True
    if os.path.isdir(path) or path.lower().endswith(".java"):
        from codebreaker.verify.lanes import java
        return java.is_module(path)
    return False


PLATFORM = Platform(
    name="java", title="Java", family="bytecode", cpu=Cpu("", decoder="codebreaker.decompile.jar"),
    source="java -> c++", lane="codebreaker.verify.lanes.java", recognise=_recognise,
    loader=Loader(container="source folder, jar"),
    convention=Convention(names=("the JVM's",), order="c", pops="callee"),
    emulator=Emulator(engine="the JVM"),
    compiler=Compiler(language="c++", toolchain="the host C++ compiler (host_cxx, else g++)",
                      tool=Ref("codebreaker.verify.lanes.java", "compiler")),
    packager=Packager(tools=Ref("codebreaker.verify.lanes.java", "jdk")))
