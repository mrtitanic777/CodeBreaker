"""PKLITE 1.15 as PKLITE stores a program: read off its own decompressor, written back the same."""
import struct

import pytest

from codebreaker.base import corpus
from codebreaker.build import pklite
from codebreaker.formats import dos


def _stored(name):
    path = corpus.retail(name)
    if not path:
        pytest.skip("%s is not in the corpus" % name)
    with open(path, "rb") as f:
        data = f.read()
    h = dos._mz(data)
    return data, data[h["header"]:h["size"]]


def test_read_encode_and_pack_extract_as_pklite_did():
    data, module = _stored("extract.exe")
    s = pklite.read(module)
    assert len(s.image) == 86150 and s.tail == b""
    # its tokens written back are its stream, byte for byte
    assert pklite.encode(s.tokens, s.stub) == module[s.start:s.end]
    # the choices PKLITE made are the ones parse() makes
    assert pklite.parse(s.image) == s.tokens
    # and the whole load module comes back: PKLITE keeps a copy of the program's header behind
    # its notice, whose min-alloc is the memory the program asks for beyond itself
    memory = struct.unpack_from("<H", data, data.index(b"Reserved") + 8 + 4 + 8)[0]
    relocs = [(seg, off) for seg, offs in s.relocs for off in offs]
    packed, header = pklite.pack(s.image, relocs, s.regs, module[:s.start], memory)
    assert packed == module and header["ss"] == dos._mz(data)["ss"]
