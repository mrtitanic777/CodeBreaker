"""Java: a folder of source (or a .jar), translated to C++ (native/cb/java) and judged by
running both under the JVM. The lane, the transpiler and the judge are native (the `java` command)."""
import os

from codebreaker.platforms import Compiler, Convention, Cpu, Emulator, Loader, Packager, Platform


def _recognise(path):
    if path.lower().endswith(".jar"):
        return True
    if os.path.isfile(path):
        return path.lower().endswith(".java")
    if os.path.isdir(path):
        for _dp, _dirs, files in os.walk(path):
            if any(f.endswith(".java") for f in files):
                return True
    return False


PLATFORM = Platform(
    name="java", title="Java", family="bytecode", cpu=Cpu("", decoder="codebreaker.decompile.jar"),
    source="java -> c++", lane="", recognise=_recognise,
    loader=Loader(container="source folder, jar"),
    convention=Convention(names=("the JVM's",), order="c", pops="callee"),
    emulator=Emulator(engine="the JVM"),
    compiler=Compiler(language="c++", toolchain="the host C++ compiler (host_cxx, else g++)"),
    packager=Packager())
