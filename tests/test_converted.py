"""Converted/: one project folder per binary (codebreaker/base/converted.py)."""
import json
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.base import converted    # noqa: E402


def _binary(folder, name, data):
    path = os.path.join(folder, name)
    with open(path, "wb") as f:
        f.write(data)
    return path


def test_layout_and_readme():
    with tempfile.TemporaryDirectory() as d:
        src = _binary(d, "game.exe", b"MZ" + b"\0" * 200)
        p = converted.project(src, base=os.path.join(d, "Converted"))
        for folder in converted.FOLDERS:
            assert os.path.isdir(os.path.join(p.root, folder)), folder
        assert os.path.isfile(os.path.join(p.original, "game.exe"))
        out = os.path.join(p.src, "game.c")
        open(out, "w").write("int main(void) { return 0; }\n")
        p.record("decompile", [out], notes="1 of 1 functions")
        data = json.load(open(p.manifest))
        assert data["file"] == "game.exe" and data["steps"][0]["outputs"] == [os.path.join("src", "game.c")]
        readme = open(os.path.join(p.root, "README.txt")).read()
        assert "decompile" in readme and "src" in readme and data["sha1"] in readme


def test_one_project_per_binary():
    """The same file finds its project again (by SHA-1); another file of the same name gets name_2;
    a command run again replaces its own step."""
    with tempfile.TemporaryDirectory() as d:
        base = os.path.join(d, "Converted")
        a = _binary(d, "tool.exe", b"MZ one")
        first = converted.project(a, base=base)
        first.record("decompile", [])
        first.record("decompile", [])
        assert converted.project(a, base=base).root == first.root
        assert len(first.data()["steps"]) == 1
        os.makedirs(os.path.join(d, "other"))
        b = _binary(os.path.join(d, "other"), "tool.exe", b"MZ two")
        second = converted.project(b, base=base)
        assert second.root != first.root and os.path.basename(second.root) == "tool_2"


def test_generic_names():
    """default.xex and the like are named after their folder (or the XEX's own name)."""
    with tempfile.TemporaryDirectory() as d:
        os.makedirs(os.path.join(d, "Puzzler"))
        path = _binary(os.path.join(d, "Puzzler"), "default.exe", b"MZ")
        assert converted.default_name(path) == "Puzzler"
