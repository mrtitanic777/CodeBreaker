"""flat_thunk (codebreaker/build/flat_thunk.py): the stub reader and the classifier, on bytes and on stand-in modules."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.build import flat_thunk    # noqa: E402


class _File:
    image_base = 0x10000000


class _Pe:
    """A PE stand-in: the bytes of one stub, and the facts `_stub_signature` asks for."""
    def __init__(self, code):
        self.code = code
        self.f = _File()
        self._smapls_rvas = set()

    def read_at(self, rva, n):
        return self.code[:n]


def test_a_ret_cut_off_inside_its_operand_resolves_nothing():
    """A decode that lands on 0xC2 at the very end of a stub's extent has no operand to read: unresolved, not a struct.error."""
    assert flat_thunk._stub_signature(_Pe(b"\x55\xc2"), 0x1000) == (None, 0)
    assert flat_thunk._stub_signature(_Pe(b"\x55\xc2\x04"), 0x1000) == (None, 0)
    assert flat_thunk._stub_signature(_Pe(b"\x55\xc2\x04\x00"), 0x1000) == (4, 0)


def test_kernel32_is_found_whatever_the_case_of_its_name():
    """A module that imports its thunk helpers from `kernel32.dll` is a flat thunk like one that says KERNEL32.dll."""
    for name in ("KERNEL32.dll", "KERNEL32.DLL", "kernel32.dll", "Kernel32.Dll"):
        assert flat_thunk._kernel32_symbols({name: ["SMapLS", "@21"]}) == ["SMapLS", "@21"], name
    assert flat_thunk._kernel32_symbols({"USER32.dll": ["x"]}) == []
    assert flat_thunk._kernel32_symbols({}) == []

    class _Image(flat_thunk.PE):
        def __init__(self, imports):
            self._imports = imports
            self.exp = [(1, "Foo")]
            self.secs = [(".text", 0x1000, 0x100, 0x200, 0x400)]

        def imports(self):
            return self._imports

        def exports(self):
            return self.exp

    load = flat_thunk.load
    try:
        flat_thunk.load = lambda path: _Image({"kernel32.dll": ["SMapLS_IP_EBP_8", "SUnMapLS"]})
        c = flat_thunk.classify("x.dll")
        assert c["subtype"] == "flat-thunk" and c["n_imports"] == 2, c
        flat_thunk.load = lambda path: _Image({"kernel32.dll": ["CreateFileA"]})
        assert flat_thunk.classify("x.dll")["subtype"] == "win32-c"
    finally:
        flat_thunk.load = load


def main():
    test_a_ret_cut_off_inside_its_operand_resolves_nothing()
    test_kernel32_is_found_whatever_the_case_of_its_name()
    print("ok")


if __name__ == "__main__":
    main()
