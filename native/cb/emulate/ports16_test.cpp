// cb/emulate/ports16_test.cpp -- the UART and PIC models, the log, and the hooks on a real Unicorn engine.
#include "cb/emulate/ports16.hpp"

#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

using namespace cb;
using namespace cb::emulate::ports16;

CB_TEST("emulate.ports16.uart_line_status_says_transmitter_empty", [] {
    Devices d;
    // base 0x3F8, register 5: THRE | TEMT, and data ready while "AT\r\n" waits
    CB_CHECK_EQ(d.read(0x3FD), 0x61);
    for (std::uint8_t want : kReceived) CB_CHECK_EQ(d.read(0x3F8), want);   // the receiver drains in order
    CB_CHECK_EQ(d.read(0x3F8), 0x00);                                                        // then reads 0
    CB_CHECK_EQ(d.read(0x3FD), 0x60);                                                        // and data-ready drops
});

CB_TEST("emulate.ports16.every_com_port_and_base_zero_has_a_uart", [] {
    Devices d;
    for (std::uint32_t base : {0x3F8u, 0x2F8u, 0x3E8u, 0x2E8u, 0x000u}) CB_CHECK_EQ(d.read(base + 5), 0x61);
    // COMM.DRV's TRMCOM polls port 5 for the transmitter, its handler reads ports 2 and 6 of base 0
    CB_CHECK_EQ(d.read(2), 0x01);       // no interrupt pending
    CB_CHECK_EQ(d.read(6), 0xB0);       // CTS | DSR | DCD
    CB_CHECK_EQ(d.log().size(), 7u);    // modelled ports are logged
});

CB_TEST("emulate.ports16.registers_read_back_what_was_written", [] {
    Devices d;
    d.write(0x3FB, 0x83);                                // line control: DLAB, 8 bits
    CB_CHECK_EQ(d.read(0x3FB), 0x83);
    d.write(0x3F8, 0x01);                                // divisor low (DLAB)
    d.write(0x3F9, 0x00);                                // divisor high
    CB_CHECK_EQ(d.read(0x3F8), 0x01);
    CB_CHECK_EQ(d.read(0x3F9), 0x00);
    CB_CHECK_EQ(d.uart(0).divisor_low(), 0x01);
    d.write(0x3FB, 0x03);                                // DLAB off: reg 1 is the interrupt enable
    d.write(0x3F9, 0xFF);
    CB_CHECK_EQ(d.read(0x3F9), 0x0F);                    // only four bits exist
    d.write(0x3FC, 0xFF);
    CB_CHECK_EQ(d.read(0x3FC), 0x1F);                    // modem control keeps five
    d.write(0x3FF, 0x5A);
    CB_CHECK_EQ(d.read(0x3FF), 0x5A);                    // scratch
    // a write to a read-only register is taken and forgotten
    d.write(0x3FD, 0x00);
    CB_CHECK_EQ(d.read(0x3FD), 0x61);
});

CB_TEST("emulate.ports16.interrupt_identification_follows_the_enables", [] {
    Devices d;
    d.write(0x3F9, 0x01);                                // receive-data interrupt on
    CB_CHECK_EQ(d.read(0x3FA), 0x04);                    // data waiting
    d.write(0x3FA, 0x02);                                // FIFO control: receiver reset
    CB_CHECK_EQ(d.read(0x3FA), 0x01);                    // nothing left, nothing pending (and the FIFO bit is off)
    d.write(0x3F9, 0x02);                                // transmitter-empty interrupt on, transmitter empty
    CB_CHECK_EQ(d.read(0x3FA), 0x02);                    // signalled once
    CB_CHECK_EQ(d.read(0x3FA), 0x01);                    // reading it cleared it
    d.write(0x3FA, 0x01);                                // FIFO enable
    CB_CHECK_EQ(d.read(0x3FA), 0xC1);                    // FIFO bits show
});

CB_TEST("emulate.ports16.loopback_reads_the_outputs_back_in_the_modem_status", [] {
    Devices d;
    d.write(0x3FC, 0x10 | 0x03);                         // loopback, DTR and RTS
    // DTR -> DSR (bit 5), RTS -> CTS (bit 4)
    CB_CHECK_EQ(d.read(0x3FE), 0x20 | 0x10);
});

CB_TEST("emulate.ports16.pic_mask_eoi_and_initialisation", [] {
    Devices d;
    CB_CHECK_EQ(d.read(0x21), 0xB8);                     // COM1, COM2 masked at the start
    CB_CHECK_EQ(d.read(0xA1), 0x8F);
    d.write(0x21, 0x00);                                 // OCW1: unmask all
    CB_CHECK_EQ(d.read(0x21), 0x00);
    d.write(0x20, 0x0B);                                 // OCW3: read the in-service register
    CB_CHECK_EQ(d.read(0x20), 0x00);
    // an initialisation sequence takes its words and ends with the mask cleared
    d.write(0x21, 0xFF);
    d.write(0x20, 0x11);                                 // ICW1 with ICW4: three words follow
    CB_CHECK_EQ(d.read(0x21), 0x00);
    d.write(0x21, 0x08);                                 // ICW2
    d.write(0x21, 0x04);                                 // ICW3
    d.write(0x21, 0x01);                                 // ICW4
    CB_CHECK_EQ(d.read(0x21), 0x00);                     // none of them was taken for the mask
    d.write(0x21, 0x55);
    CB_CHECK_EQ(d.read(0x21), 0x55);                     // the next write is
    CB_CHECK_EQ(d.pic(0).mask(), 0x55);
});

CB_TEST("emulate.ports16.ports_nothing_answers_read_zero_and_are_not_logged", [] {
    Devices d;
    CB_CHECK_EQ(d.read(0x60), 0);
    CB_CHECK_EQ(d.read(0x1234), 0);
    d.write(0x60, 0xFF);
    CB_CHECK_EQ(d.log().size(), 0u);
    // a wider port number counts by its low 16 bits
    CB_CHECK_EQ(d.read(0x10003FD), 0x61);
    d.write(0x2000021, 0x12);
    CB_CHECK_EQ(d.read(0x21), 0x12);
});

CB_TEST("emulate.ports16.log_orders_accesses_by_the_clock_and_ends_in_one_marker", [] {
    std::uint64_t step = 100;
    Devices d([&] { return step++; });
    d.write(0x3FB, 0x80);
    (void)d.read(0x3FB);
    CB_CHECK_EQ(d.log().size(), 2u);
    CB_CHECK(d.log()[0] == (LogEntry{100, Access::Out, 0x3FB, 0x80}));
    CB_CHECK(d.log()[1] == (LogEntry{101, Access::In, 0x3FB, 0x80}));
    for (int i = 0; i < 1000; ++i) (void)d.read(0x3FF);
    CB_CHECK_EQ(d.log().size(), kMaxLog + 1);            // capped, then one marker
    CB_CHECK(d.log().back().kind == Access::Truncated);
    CB_CHECK(d.log()[kMaxLog - 1].kind == Access::In);
    // no clock: every step is 0
    Devices quiet;
    (void)quiet.read(0x3FD);
    CB_CHECK_EQ(quiet.log()[0].step, 0u);
});

CB_TEST("emulate.ports16.a_word_access_is_two_byte_accesses", [] {
    Devices d;
    d.write(0x3FB, 0x80);
    d.out(0x3F8, 2, 0x0201);                             // divisor low 1, high 2
    CB_CHECK_EQ(d.in(0x3F8, 2), 0x0201u);
    CB_CHECK_EQ(d.in(0x3F8, 1), 0x01u);
    d.clear_log();
    CB_CHECK_EQ(d.log().size(), 0u);
    CB_CHECK_EQ(d.uart(0).divisor_high(), 2);            // the state stays
});

// The hooks on a real engine: 16-bit code that polls the line status, reads a received byte, sets the divisor with
// DLAB on, and reads it back as a word (the second port's byte above the first's).
CB_TEST("emulate.ports16.hooks_hand_in_and_out_to_the_devices", [] {
    const auto& uc = emu::unicorn::api();
    uc_engine* engine = nullptr;
    emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_16, &engine), "uc_open");
    const std::uint8_t code[] = {
        0xBA, 0xFD, 0x03, 0xEC, 0x88, 0xC3,    // mov dx,3FDh / in al,dx / mov bl,al
        0xBA, 0xF8, 0x03, 0xEC, 0x88, 0xC7,    // mov dx,3F8h / in al,dx / mov bh,al
        0xBA, 0xFB, 0x03, 0xB0, 0x80, 0xEE,    // mov dx,3FBh / mov al,80h / out dx,al
        0xBA, 0xF8, 0x03, 0xB0, 0x01, 0xEE,    // mov dx,3F8h / mov al,1 / out dx,al
        0xBA, 0xF9, 0x03, 0xB0, 0x00, 0xEE,    // mov dx,3F9h / mov al,0 / out dx,al
        0xBA, 0xF8, 0x03, 0xED,                // mov dx,3F8h / in ax,dx
    };
    emu::unicorn::check(uc.uc_mem_map(engine, 0x1000, 0x1000, UC_PROT_ALL), "uc_mem_map");
    emu::unicorn::check(uc.uc_mem_write(engine, 0x1000, code, sizeof code), "uc_mem_write");
    std::uint64_t clock = 0;
    {
        Devices devices([&] { return clock++; });
        PortHooks hooks(engine, devices);
        emu::unicorn::check(uc.uc_emu_start(engine, 0x1000, 0x1000 + sizeof code, 0, 0), "uc_emu_start");
        std::uint64_t bx = 0, ax = 0;
        uc.uc_reg_read(engine, UC_X86_REG_BX, &bx);
        uc.uc_reg_read(engine, UC_X86_REG_AX, &ax);
        CB_CHECK_EQ(bx & 0xFF, 0x61u);          // the line status
        CB_CHECK_EQ((bx >> 8) & 0xFF, 0x41u);   // 'A' from the receiver
        CB_CHECK_EQ(ax & 0xFFFF, 0x0001u);      // divisor low in AL, divisor high in AH
        CB_CHECK_EQ(devices.uart(0).divisor_low(), 1);
        CB_CHECK_EQ(devices.uart(0).line_control(), 0x80);
        const std::vector<LogEntry> expected = {
            {0, Access::In, 0x3FD, 0x61},  {1, Access::In, 0x3F8, 0x41},  {2, Access::Out, 0x3FB, 0x80},
            {3, Access::Out, 0x3F8, 0x01}, {4, Access::Out, 0x3F9, 0x00}, {5, Access::In, 0x3F8, 0x01},
            {6, Access::In, 0x3F9, 0x00},
        };
        CB_CHECK(std::vector<LogEntry>(devices.log().begin(), devices.log().end()) == expected);
    }                                           // the hooks come off before the devices go
    emu::unicorn::check(uc.uc_close(engine), "uc_close");
});
