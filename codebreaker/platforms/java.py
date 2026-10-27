"""Java: a folder of source (or a .jar), translated to C++ (codebreaker/java) and judged by
running both under the JVM."""
import os

from codebreaker.platforms import Platform


def _recognise(path):
    if path.lower().endswith(".jar"):
        return True
    if os.path.isdir(path) or path.lower().endswith(".java"):
        from codebreaker.verify.lanes import java
        return java.is_module(path)
    return False


PLATFORM = Platform(
    name="java", title="Java", family="bytecode", container="source folder, jar",
    cpu="", source="java -> c++", lane="codebreaker.verify.lanes.java", recognise=_recognise)
