"""ports16.py -- the devices a 16-bit run talks to through `in` and `out`.

    devices = ports16.Devices()
    value = devices.read(port)          # an `in`
    devices.write(port, value)          # an `out`
    devices.log                         # [(step, "in" | "out", port, value)], in order

Without them every `in` read 0 and every `out` went nowhere: COMM.DRV's functions wait for the
UART's transmitter to be empty (`in al, dx / test al, 20h / jz`) and spun to the step limit, the
original and the rebuild alike, and what they wrote to the chip was not looked at -- so a rebuild
writing the wrong divisor or the wrong line control passed. Here are an 8250/16550A at each of the
four COM ports and the two 8259 interrupt controllers, as much of them as a driver's code paths
see: registers that read back what was written, the line status saying the transmitter is empty,
a few bytes waiting in the receiver, the interrupt identification a handler loops on, the
controllers' masks and in-service registers. No interrupt is ever raised. Each run starts from the
same state, so the original and the rebuild meet the same devices; what they did to them (the
log) is part of what is compared (verify/lanes/win16.py).

A UART answers at base 0 as well: a function run alone finds its port's record as the module's
data has it before OpenComm filled it in, port base 0 -- COMM.DRV's TRMCOM polls port 5 for the
transmitter, its interrupt handler reads ports 2 and 6. (Ports 0-7 are the DMA controller's on a
PC; nothing else run here reaches them.)

Other ports are not modelled: an `in` reads 0 and is not logged, as before.
"""
UART_BASES = (0x3F8, 0x2F8, 0x3E8, 0x2E8, 0x000)
PIC_PORTS = (0x20, 0x21, 0xA0, 0xA1)
RECEIVED = b"\x41\x54\x0d\x0a"          # what each UART has waiting at the start ("AT\r\n")
MAX_LOG = 256                           # accesses logged per run (then "...")

# line status: data ready, transmitter holding register empty, transmitter empty
LSR_DR, LSR_THRE, LSR_TEMT = 0x01, 0x20, 0x40
# modem status: clear to send, data set ready, carrier detect (the deltas clear)
MSR_IDLE = 0x10 | 0x20 | 0x80


class Uart:
    """An 8250 / 16550A. Registers (base +): 0 receive / transmit (divisor low with DLAB),
    1 interrupt enable (divisor high), 2 interrupt identification / FIFO control, 3 line control,
    4 modem control, 5 line status, 6 modem status, 7 scratch."""

    def __init__(self):
        self.dll, self.dlm = 0x0C, 0x00                      # 9600 baud
        self.ier, self.lcr, self.mcr, self.scr = 0x00, 0x03, 0x00, 0x00
        self.fcr = 0x00
        self.rx = bytearray(RECEIVED)
        self.msr = MSR_IDLE
        self.thre_pending = False                            # (an empty transmitter, signalled)

    def _iir(self):
        fifo = 0xC0 if self.fcr & 1 else 0x00
        if self.ier & 0x01 and self.rx:
            return fifo | 0x04                               # received data available
        if self.ier & 0x02 and self.thre_pending:
            self.thre_pending = False                        # (reading it clears it)
            return fifo | 0x02                               # transmitter holding register empty
        if self.ier & 0x08 and self.msr & 0x0F:
            return fifo | 0x00                               # modem status
        return fifo | 0x01                                   # nothing pending

    def read(self, reg):
        dlab = self.lcr & 0x80
        if reg == 0:
            if dlab:
                return self.dll
            return self.rx.pop(0) if self.rx else 0x00
        if reg == 1:
            return self.dlm if dlab else self.ier
        if reg == 2:
            return self._iir()
        if reg == 3:
            return self.lcr
        if reg == 4:
            return self.mcr
        if reg == 5:
            return LSR_THRE | LSR_TEMT | (LSR_DR if self.rx else 0)
        if reg == 6:
            v, self.msr = self.msr, self.msr & 0xF0           # (the deltas clear on reading)
            if self.mcr & 0x10:                              # loopback: the outputs read back
                v = ((self.mcr & 0x02) << 3) | ((self.mcr & 0x01) << 5) | ((self.mcr & 0x04) << 4) \
                    | ((self.mcr & 0x08) << 4)
            return v
        return self.scr

    def write(self, reg, value):
        dlab = self.lcr & 0x80
        if reg == 0:
            if dlab:
                self.dll = value
            else:
                self.thre_pending = True                     # (sent at once: empty again)
        elif reg == 1:
            if dlab:
                self.dlm = value
            else:
                if value & 0x02 and not self.ier & 0x02:
                    self.thre_pending = True                 # (enabling it with the transmitter empty)
                self.ier = value & 0x0F
        elif reg == 2:
            self.fcr = value & 0xC9
            if value & 0x02:
                self.rx.clear()                              # receiver FIFO reset
        elif reg == 3:
            self.lcr = value
        elif reg == 4:
            self.mcr = value & 0x1F
        elif reg == 7:
            self.scr = value


class Pic:
    """An 8259: the mask (base + 1), and the request / in-service registers (base, as the last
    OCW3 chose); an initialisation sequence is taken in and ends with the mask cleared."""

    def __init__(self, mask):
        self.imr, self.irr, self.isr = mask, 0x00, 0x00
        self.read_isr = False
        self.icw = 0                                         # initialisation words still to come

    def read(self, reg):
        if reg == 1:
            return self.imr
        return self.isr if self.read_isr else self.irr

    def write(self, reg, value):
        if reg == 0:
            if value & 0x10:                                 # ICW1: two or three words follow
                self.icw, self.imr, self.isr, self.read_isr = (3 if value & 0x01 else 2), 0, 0, False
            elif value & 0x08:                               # OCW3
                if value & 0x02:
                    self.read_isr = bool(value & 0x01)
            elif value & 0x20:                               # OCW2: end of interrupt
                self.isr &= self.isr - 1 if not value & 0x40 else ~(1 << (value & 7)) & 0xFF
        elif self.icw:
            self.icw -= 1
        else:
            self.imr = value


class Devices:
    """The ports a run can reach: the UARTs, the two interrupt controllers. Fresh for each run.
    `clock()`: the run's step, logged with each access (to order them among its other effects)."""

    def __init__(self, clock=None):
        self.uarts = {b: Uart() for b in UART_BASES}
        self.pics = {0x20: Pic(0xB8), 0xA0: Pic(0x8F)}      # (COM1, COM2 masked at the start)
        self.log = []
        self.clock = clock or (lambda: 0)

    def _device(self, port):
        base = port & ~7
        if base in self.uarts:
            return self.uarts[base], port & 7
        if port in PIC_PORTS:
            return self.pics[port & ~1], port & 1
        return None, None

    def _note(self, what, port, value):
        if len(self.log) < MAX_LOG:
            self.log.append((self.clock(), what, port, value))
        elif len(self.log) == MAX_LOG:
            self.log.append((self.clock(), "...", 0, 0))

    def read(self, port):
        dev, reg = self._device(port & 0xFFFF)
        if dev is None:
            return 0
        v = dev.read(reg) & 0xFF
        self._note("in", port & 0xFFFF, v)
        return v

    def write(self, port, value):
        dev, reg = self._device(port & 0xFFFF)
        if dev is None:
            return
        dev.write(reg, value & 0xFF)
        self._note("out", port & 0xFFFF, value & 0xFF)


def hook(uc, U, devices):
    """Hand `uc`'s `in` and `out` instructions to `devices`."""
    from unicorn.x86_const import UC_X86_INS_IN, UC_X86_INS_OUT

    def on_in(_uc, port, size, _ud):
        v = devices.read(port)
        if size >= 2:                                        # (a word: the next port's byte above)
            v |= devices.read(port + 1) << 8
        return v

    def on_out(_uc, port, size, value, _ud):
        devices.write(port, value)
        if size >= 2:
            devices.write(port + 1, value >> 8)
    uc.hook_add(U.UC_HOOK_INSN, on_in, None, 1, 0, UC_X86_INS_IN)
    uc.hook_add(U.UC_HOOK_INSN, on_out, None, 1, 0, UC_X86_INS_OUT)
