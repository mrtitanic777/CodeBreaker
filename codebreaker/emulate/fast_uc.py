"""fast_uc.py -- Unicorn's register and memory reads, the same values for less of Python's time.

    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    fast_uc.speed_up(uc)          # uc.reg_read / uc.mem_read: the same answers, faster

A run under the checker calls back into Python on every instruction and every memory access,
and each callback reads registers and memory: USER's gate made 18 million register reads and
22 million memory reads for 81 functions. Unicorn's Python binding spends most of each on
choosing the register's class (a walk over six ranges, every time), building the ctypes value
to read into, and an audited string buffer. Here the class is chosen once per register, and
a plain integer register is read into one value kept for it; a memory read gets its buffer
from a type made once per size. What comes back is what Unicorn's own call gives -- the same
call into the library, the same value type -- so a run is the same run.
"""
import ctypes


def speed_up(uc):
    """Put the faster reads on this Unicorn instance (its own attributes, over the class's
    methods). -> uc. Left as it is when the binding is not the one this was written for."""
    try:
        from unicorn.unicorn_py3.unicorn import UcError, uclib
        handle = uc._uch
        select = uc._select_reg_class
        default = uc._DEFAULT_REGTYPE
        from unicorn.unicorn_const import UC_ERR_OK as ok
    except Exception:
        return uc
    slow_read, slow_write = uc.reg_read, uc.reg_write
    values = {}                                          # register -> the value read into, or False
    byref, read, write, mem = ctypes.byref, uclib.uc_reg_read, uclib.uc_reg_write, uclib.uc_mem_read

    def plain(reg_id):
        v = values.get(reg_id)
        if v is None:
            v = values[reg_id] = default() if select(reg_id) is default else False
        return v

    def reg_read(reg_id, aux=None):
        v = plain(reg_id) if aux is None else False
        if v is False:
            return slow_read(reg_id, aux)
        status = read(handle, reg_id, byref(v))
        if status != ok:
            raise UcError(status, reg_id)
        return v.value

    def reg_write(reg_id, value):
        if plain(reg_id) is False or isinstance(value, tuple):
            return slow_write(reg_id, value)
        status = write(handle, reg_id, byref(default(value)))
        if status != ok:
            raise UcError(status, reg_id)

    buffers = {}                                         # size -> a buffer read into, copied out

    def mem_read(address, size):
        data = buffers.get(size)
        if data is None:
            data = buffers[size] = (ctypes.c_char * size)()
        status = mem(handle, address, data, size)
        if status != ok:
            raise UcError(status, address, size)
        return bytearray(data)

    uc.reg_read, uc.reg_write = reg_read, reg_write
    uc.mem_read = mem_read
    return uc
