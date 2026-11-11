"""StreamIO -- the endian-aware byte stream the Xbox-360-Crypto tools read and write images through.

Upstream's own StreamIO module was not among the files carried over; this one gives the same calls
those files make: a stream over bytes (or a file, by path), little or big endian, with integer,
byte and ctypes-structure reads and writes, at the position or at an offset. The tools also use the
position as an attribute (`offset`), named positions (labels) and a function applied to the bytes at an
offset in place (`perform_function_at`).
"""
import io
import os
import struct
from ctypes import sizeof
from enum import IntEnum
from io import SEEK_CUR, SEEK_END, SEEK_SET

__all__ = ["Endian", "StreamIO", "SEEK_SET", "SEEK_CUR", "SEEK_END"]


class Endian(IntEnum):
    LITTLE = 0
    BIG = 1


class StreamIO:
    def __init__(self, data=None, endian=Endian.LITTLE):
        self.endian = endian
        self._labels = {}
        self._file = isinstance(data, (str, os.PathLike))
        if self._file:
            self._stream = open(data, "r+b" if os.path.exists(data) else "w+b")
        else:
            self._stream = io.BytesIO(bytes(data) if data is not None else b"")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()

    def close(self):
        if self._file:
            self._stream.close()

    def _order(self):
        return ">" if self.endian == Endian.BIG else "<"

    # position
    def seek(self, offset, whence=io.SEEK_SET):
        return self._stream.seek(offset, whence)

    def tell(self):
        return self._stream.tell()

    @property
    def offset(self):
        """The position, as an attribute: `s.offset = n` seeks, `s.offset -= 4` steps back."""
        return self._stream.tell()

    @offset.setter
    def offset(self, value):
        self._stream.seek(value)

    # named positions
    def set_label(self, name, offset=None):
        """Name the position (or `offset`)."""
        self._labels[name] = self._stream.tell() if offset is None else offset

    def get_label(self, name):
        return self._labels[name]

    def goto_label(self, name, offset=0):
        """Seek to the named position plus `offset`."""
        return self._stream.seek(self._labels[name] + offset)

    def getvalue(self):
        if self._file:
            here = self._stream.tell()
            self._stream.seek(0)
            data = self._stream.read()
            self._stream.seek(here)
            return data
        return self._stream.getvalue()

    def length(self):
        here = self._stream.tell()
        end = self._stream.seek(0, io.SEEK_END)
        self._stream.seek(here)
        return end

    # bytes
    def read(self, size=-1):
        return self._stream.read(size)

    def write(self, data):
        return self._stream.write(data)

    def read_bytes(self, size):
        return self._stream.read(size)

    read_ubytes = read_bytes

    def read_byte(self):
        return self._stream.read(1)[0]

    read_ubyte = read_byte

    def read_bytes_at(self, offset, size):
        here = self._stream.tell()
        self._stream.seek(offset)
        data = self._stream.read(size)
        self._stream.seek(here)
        return data

    read_ubytes_at = read_bytes_at

    def write_bytes(self, data):
        return self._stream.write(bytes(data))

    write_ubytes = write_bytes

    def write_bytes_at(self, offset, data):
        here = self._stream.tell()
        self._stream.seek(offset)
        written = self._stream.write(bytes(data))
        self._stream.seek(here)
        return written

    write_ubytes_at = write_bytes_at

    def perform_function_at(self, offset, size, function):
        """Apply `function` to the `size` bytes at `offset`, write what it returns back there (in place) and
        return it; the position is left where it was."""
        here = self._stream.tell()
        self._stream.seek(offset)
        data = bytes(function(self._stream.read(size)))
        self._stream.seek(offset)
        self._stream.write(data)
        self._stream.seek(here)
        return data

    # integers
    def _read(self, fmt):
        size = struct.calcsize(fmt)
        return struct.unpack(self._order() + fmt, self._stream.read(size))[0]

    def _write(self, fmt, value):
        return self._stream.write(struct.pack(self._order() + fmt, value))

    def read_uint8(self): return self._read("B")
    def read_uint16(self): return self._read("H")
    def read_uint32(self): return self._read("I")
    def read_uint64(self): return self._read("Q")
    def read_int8(self): return self._read("b")
    def read_int16(self): return self._read("h")
    def read_int32(self): return self._read("i")
    def read_int64(self): return self._read("q")
    def write_uint8(self, v): return self._write("B", v)
    def write_uint16(self, v): return self._write("H", v)
    def write_uint32(self, v): return self._write("I", v)
    def write_uint64(self, v): return self._write("Q", v)
    def write_int8(self, v): return self._write("b", v)
    def write_int16(self, v): return self._write("h", v)
    def write_int32(self, v): return self._write("i", v)
    def write_int64(self, v): return self._write("q", v)

    # ctypes structures (their own endianness: Little/BigEndianStructure)
    def read_struct(self, struct_type):
        return struct_type.from_buffer_copy(self._stream.read(sizeof(struct_type)))

    read_header = read_struct

    def write_struct(self, value):
        return self._stream.write(bytes(value))
