"""EXEPACK as LINK /EXEPACK stores a program: what pack() writes, unpack_exepack() undoes."""
from codebreaker.build import exepack
from codebreaker.formats import dos


def _stub():
    # stand-in unpacker code: the format only needs its message at the end
    return b"\x90" * 20 + exepack.MESSAGE


def test_pack_round_trip_both_headers():
    image = bytes(range(256)) * 4 + bytes(300) + b"\x41" * 40 + bytes(range(64)) + b"\xFF" * 16
    fills = [(1024, 300, 0), (1324, 40, 0x41), (1428, 16, 0xFF)]
    relocs = [0x10, 0x20, 0x1F0]
    for words in (8, 9):
        stored, (cs, ip, ss, sp) = exepack.pack(image, relocs, (3, 0x44), (0x50, 0x200), fills, _stub(), words)
        assert ip == 2 * words and sp == 0x80
        module, back, real_cs, real_ip, real_ss, real_sp = dos.unpack_exepack(stored, cs, ip)
        assert module == image + bytes(-len(image) % 16) and back == relocs     # a paragraph's worth
        assert (real_cs, real_ip, real_ss, real_sp) == (3, 0x44, 0x50, 0x200)
        layout = exepack.layout(stored, cs, ip)
        assert layout.fills == fills and layout.stub_code == _stub() and layout.words == words
