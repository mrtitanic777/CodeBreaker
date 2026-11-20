; srammtd.vxd -- assembled with NASM: nasm -f bin -o srammtd.vxd this-file

; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one
; Microsoft's tools write; NASM itself writes the other
%assign _r_ah 4
%assign _r_al 0
%assign _r_ax 0
%assign _r_bh 7
%assign _r_bl 3
%assign _r_bp 5
%assign _r_bx 3
%assign _r_ch 5
%assign _r_cl 1
%assign _r_cx 1
%assign _r_dh 6
%assign _r_di 7
%assign _r_dl 2
%assign _r_dx 2
%assign _r_eax 0
%assign _r_ebp 5
%assign _r_ebx 3
%assign _r_ecx 1
%assign _r_edi 7
%assign _r_edx 2
%assign _r_esi 6
%assign _r_esp 4
%assign _r_si 6
%assign _r_sp 4
%assign _w_ah 0
%assign _w_al 0
%assign _w_ax 2
%assign _w_bh 0
%assign _w_bl 0
%assign _w_bp 2
%assign _w_bx 2
%assign _w_ch 0
%assign _w_cl 0
%assign _w_cx 2
%assign _w_dh 0
%assign _w_di 2
%assign _w_dl 0
%assign _w_dx 2
%assign _w_eax 4
%assign _w_ebp 4
%assign _w_ebx 4
%assign _w_ecx 4
%assign _w_edi 4
%assign _w_edx 4
%assign _w_esi 4
%assign _w_esp 4
%assign _w_si 2
%assign _w_sp 2
%macro _rr 3
  %if (_w_%2 == 2 && __?BITS?__ == 32) || (_w_%2 == 4 && __?BITS?__ == 16)
    db 0x66
  %endif
  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3
%endmacro
%define adc_ _rr 0x10,
%define add_ _rr 0x00,
%define and_ _rr 0x20,
%define cmp_ _rr 0x38,
%define mov_ _rr 0x88,
%define or_ _rr 0x08,
%define sbb_ _rr 0x18,
%define sub_ _rr 0x28,
%define xor_ _rr 0x30,
o1_file equ 0x400

section part0 start=0x0 vstart=0x0 align=1
            db   04Dh, 05Ah, 000h, 000h, 002h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
            db   0B8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
            db   00Eh, 01Fh, 0BAh, 00Eh, 000h, 0B4h, 009h, 0CDh, 021h, 0B8h, 001h, 04Ch, 0CDh
            db   '!This program cannot be run in DOS mode.'
            db   00Dh, 00Ah, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Ch, 045h, 000h, 000h, 000h
            db   000h, 000h, 000h, 002h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 003h, 000h, 001h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 010h, 000h, 000h, 05Ch, 006h, 000h, 000h, 00Eh, 001h, 000h, 000h, 000h
            db   000h, 000h, 000h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C4h, 000h, 000h, 000h, 001h
            db   000h, 000h, 000h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 0E0h, 000h, 000h, 000h, 0EBh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 0F5h, 000h, 000h, 000h, 0FDh, 000h, 000h, 000h, 003h, 002h, 000h, 000h, 000h
            db   000h, 000h, 000h, 003h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h
            db   000h, 000h, 000h, 05Ch, 00Ah, 000h, 000h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 092h, 00Ah, 000h, 000h, 0F0h, 001h, 000h, 000h, 000h, 000h, 000h, 004h, 070h
            db   006h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 020h, 000h, 000h, 001h, 000h, 000h, 000h, 001h
            db   000h, 000h, 000h, 04Ch, 043h, 04Fh, 044h, 000h, 000h, 001h, 000h, 007h
            db   'SRAMMTD'
            db   000h, 000h, 000h, 001h, 003h, 001h, 000h, 003h, 03Ch, 005h, 000h, 000h, 000h

section fixpage start=0x175 vstart=0 align=1
    dd fr_p0-fr_p0
    dd fr_p1-fr_p0

section fixrec start=0x17d vstart=0 align=1
fr_p0:
    db 0x27,0x00
    db 3
    db 0x01
    dw o1_L5c0
    dw o1_D146-0x0
    dw o1_D18c-0x0
    dw o1_D1a9-0x0
    db 0x07,0x00
    dw o1_D113-0x0
    db 0x01
    dw o1_L283
    db 0x07,0x00
    dw o1_D163-0x0
    db 0x01
    dw o1_L5c4
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L304
    dw o1_S310-0x0
    dw o1_D4f1-0x0
    db 0x07,0x00
    dw o1_S491-0x0
    db 0x01
    dw o1_L5c8
    db 0x07,0x00
    dw o1_D76-0x0
    db 0x01
    dw o1_L58c
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L58e
    dw o1_D70-0x0
    dw o1_D8b-0x0
    db 0x07,0x00
    dw o1_D554-0x0
    db 0x01
    dw o1_L211
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L594
    dw o1_D9a-0x0
    dw o1_Da5-0x0
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L457
    dw o1_S463-0x0
    dw o1_D4fd-0x0
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L59a
    dw o1_De6-0x0
    dw o1_D118-0x0
    db 0x07,0x00
    dw o1_Def-0x0
    db 0x01
    dw o1_L59c
    db 0x07,0x00
    dw o1_S31e-0x0
    db 0x01
    dw o1_D65c
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L31c
    dw o1_S328-0x0
    dw o1_D4f5-0x0
    db 0x07,0x00
    dw o1_D100-0x0
    db 0x01
    dw o1_L59e
    db 0x07,0x00
    dw o1_S3b2-0x0
    db 0x01
    dw o1_start+0x660
    db 0x07,0x00
    dw o1_S306-0x0
    db 0x01
    dw o1_start+0x664
    db 0x07,0x00
    dw o1_Df8-0x0
    db 0x01
    dw o1_L5a6
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L5a8
    dw o1_D1c3-0x0
    dw o1_D1e6-0x0
    db 0x07,0x00
    dw o1_S459-0x0
    db 0x01
    dw o1_start+0x668
    db 0x07,0x00
    dw o1_D1ca-0x0
    db 0x01
    dw o1_L5aa
    db 0x07,0x00
    dw o1_D1d4-0x0
    db 0x01
    dw o1_L5ac
    db 0x27,0x00
    db 4
    db 0x01
    dw o1_start+0x66c
    dw o1_S30c-0x0
    dw o1_S324-0x0
    dw o1_S3b8-0x0
    dw o1_S45f-0x0
    db 0x07,0x00
    dw o1_D1db-0x0
    db 0x01
    dw o1_L5b0
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L3b0
    dw o1_S3bc-0x0
    dw o1_D4f9-0x0
    db 0x07,0x00
    dw o1_S521-0x0
    db 0x01
    dw o1_D4f1
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L5b2
    dw o1_Db9-0x0
    dw o1_Dbe-0x0
    db 0x07,0x00
    dw o1_Dd8-0x0
    db 0x01
    dw o1_L5b4
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L5b8
    dw o1_D140-0x0
    dw o1_D16d-0x0
    db 0x07,0x00
    dw o1_D15b-0x0
    db 0x01
    dw o1_L5ba
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_L5bc
    dw o1_D150-0x0
    dw o1_D192-0x0
fr_p1:

section part1 start=0x283 vstart=0x283 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section o1 start=o1_file vstart=0 align=1
bits 32
o1_start:
            db   0C8h, 008h, 000h, 000h, 089h, 05Dh, 0FCh, 083h, 0FAh, 002h, 00Fh, 085h, 0F2h, 001h, 000h, 000h
            db   08Dh, 045h, 0F8h, 06Ah, 000h, 053h, 053h, 050h, 0CDh, 020h, 009h, 000h, 033h, 000h, 083h, 0C4h
            db   010h, 08Bh, 07Dh, 0F8h, 00Bh, 0FFh, 00Fh, 084h, 0D6h, 001h, 000h, 000h, 06Ah, 001h, 06Ah, 038h
            db   0CDh, 020h, 04Fh, 000h, 001h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 00Fh, 084h, 0C1h, 001h, 000h
            db   000h, 089h, 045h, 0F8h, 06Ah, 000h, 050h, 053h, 053h, 0CDh, 020h, 00Ah, 000h, 033h, 000h, 083h
            db   0C4h, 010h, 08Bh, 05Dh, 0F8h, 0C7h, 043h, 010h, 0FFh, 0FFh, 0FFh, 0FFh, 066h, 08Bh, 007h, 066h
            db   089h, 003h, 08Bh, 047h, 002h, 089h, 043h, 004h, 08Bh, 045h, 0FCh, 089h, 043h, 008h, 0C6h, 005h
o1_D70:
    dd o1_L58e
            db   000h, 0BBh
o1_D76:
    dd o1_L58c
            db   066h, 0B8h, 00Bh, 0AFh, 066h, 0B9h, 00Eh, 000h, 0CDh, 020h, 001h, 000h, 07Ch, 009h, 066h, 081h
            db   03Dh
o1_D8b:
    dd o1_L58e
            db   043h, 053h, 00Fh, 085h, 063h, 001h, 000h, 000h, 066h, 081h, 03Dh
o1_D9a:
    dd o1_L594
            db   001h, 002h, 074h, 00Fh, 066h, 081h, 03Dh
o1_Da5:
    dd o1_L594
            db   010h, 002h, 00Fh, 085h, 049h, 001h, 000h, 000h, 08Bh, 05Dh, 0F8h, 066h, 08Bh, 003h, 066h, 0A3h
o1_Db9:
    dd o1_L5b2
            db   0BBh
o1_Dbe:
    dd o1_L5b2
            db   066h, 0B8h, 00Ch, 0AFh, 066h, 0B9h, 006h, 000h, 0CDh, 020h, 001h, 000h, 07Ch, 009h, 00Fh, 082h
            db   024h, 001h, 000h, 000h, 066h, 0A1h
o1_Dd8:
    dd o1_L5b4
            db   08Bh, 05Dh, 0F8h, 066h, 089h, 043h, 016h, 066h, 0C7h, 005h
o1_De6:
    dd o1_L59a
            db   00Ah, 000h, 066h, 0C7h, 005h
o1_Def:
    dd o1_L59c
            db   080h, 000h, 066h, 0C7h, 005h
o1_Df8:
    dd o1_L5a6
            db   001h, 002h, 08Dh, 035h
o1_D100:
    dd o1_L59e
            db   066h, 0C7h, 006h, 000h, 000h, 066h, 0C7h, 046h, 002h, 000h, 000h, 089h, 05Eh, 004h, 0BEh
o1_D113:
    dd o1_L283
            db   0BBh
o1_D118:
    dd o1_L59a
            db   066h, 0B8h, 010h, 0AFh, 066h, 0B9h, 00Eh, 000h, 0CDh, 020h, 001h, 000h, 07Ch, 009h, 00Fh, 082h
            db   0CAh, 000h, 000h, 000h, 08Bh, 05Dh, 0F8h, 066h, 089h, 053h, 002h, 083h, 04Bh, 00Ch, 001h, 066h
            db   08Bh, 003h, 066h, 0A3h
o1_D140:
    dd o1_L5b8
            db   0C7h, 005h
o1_D146:
    dd o1_L5c0
            db   000h, 000h, 000h, 000h, 0C7h, 005h
o1_D150:
    dd o1_L5bc
            db   000h, 000h, 000h, 000h, 066h, 0C7h, 005h
o1_D15b:
    dd o1_L5ba
            db   00Ch, 000h, 0C6h, 005h
o1_D163:
    dd o1_L5c4
            db   080h, 066h, 08Bh, 053h, 002h, 0BBh
o1_D16d:
    dd o1_L5b8
            db   066h, 0B8h, 021h, 0AFh, 066h, 0B9h, 00Dh, 000h, 0CDh, 020h, 001h, 000h, 07Ch, 009h, 072h, 079h
            db   08Bh, 05Dh, 0F8h, 066h, 089h, 053h, 014h, 06Ah, 000h, 0FFh, 035h
o1_D18c:
    dd o1_L5c0
            db   0FFh, 035h
o1_D192:
    dd o1_L5bc
            db   0CDh, 020h, 06Ch, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 074h, 056h, 089h, 043h
            db   018h, 08Bh, 015h
o1_D1a9:
    dd o1_L5c0
            db   089h, 053h, 028h, 003h, 0C2h, 089h, 043h, 01Ch, 04Ah, 089h, 053h, 024h, 0F7h, 0D2h, 089h, 053h
            db   020h, 066h, 08Bh, 003h, 066h, 0A3h
o1_D1c3:
    dd o1_L5a8
            db   066h, 0C7h, 005h
o1_D1ca:
    dd o1_L5aa
            db   000h, 010h, 08Bh, 043h, 004h, 0A3h
o1_D1d4:
    dd o1_L5ac
            db   066h, 0C7h, 005h
o1_D1db:
    dd o1_L5b0
            db   000h, 000h, 066h, 08Bh, 053h, 002h, 0BBh
o1_D1e6:
    dd o1_L5a8
            db   066h, 0B8h, 01Ah, 0AFh, 066h, 0B9h, 00Ah, 000h, 0CDh, 020h, 001h, 000h, 07Ch, 009h, 073h, 00Eh
            db   08Bh, 075h, 0F8h, 0E8h, 02Bh, 000h, 000h, 000h, 0B8h, 013h, 000h, 000h, 000h, 0F9h, 0C9h, 0C3h
            db   0B8h, 000h, 001h, 000h, 000h, 0F8h, 0C3h
o1_L211:
    cmp eax, 0x22
    je 0
    clc
    ret
o1_L21c:
    mov_ ebx, esi
    and dword [ebx + 0xc], 0xfffffffe
    test dword [ebx + 0xc], strict dword 2
    jne short o1_L22d
    clc
    ret
o1_L22d:
    mov_ ebx, esi
    or dword [ebx + 0xc], 2
    test dword [ebx + 0xc], strict dword 1
    stc
    jne short o1_L282
    movzx edx, word [ebx + 0x14]
    or_ edx, edx
    je short o1_L257
    push ebx
    mov ax, 0xaf1d
    mov cx, strict word 0
    int 0x20
    add dword [eax], eax
    jl short o1_L25d
    pop ebx
    jb short o1_L282
o1_L257:
    movzx edx, word [ebx + 2]
    or_ edx, edx
o1_L25d:
    je short o1_L271
    push ebx
    mov ax, 0xaf02
    mov cx, strict word 0
    int 0x20
    add dword [eax], eax
    jl short o1_L277
    pop ebx
    jb short o1_L282
o1_L271:
    push 0
    push ebx
    int 0x20
    push ecx
o1_L277:
    add byte [ecx], al
    add byte [dword ebx - 0x4799f73c], al
    add byte [eax], al
    clc
o1_L282:
    ret
o1_L283:
    cmp al, 0x82
    je short o1_L21c
    cmp cx, word [esi]
    jne short o1_L298
    cmp al, 0x12
    je near o1_L501
    cmp al, 5
    je short o1_L22d
o1_L298:
    mov ax, strict word 0
    clc
    ret
            db   000h, 000h
o1_L2a0:
    push esi
    and esi, dword [ebx + 0x20]
    cmp esi, dword [ebx + 0x10]
    clc
    je short o1_L2d1
    push eax
    push ebx
    push edx
    push edi
    mov dword [ebx + 0x10], esi
    mov_ edi, esi
    mov ax, strict word 0
    mov dx, word [ebx + 0x14]
    mov bh, 0
    mov bl, byte [ebp + 0x11]
    call dword [ebp + 0x64]
    pop edi
    pop edx
    pop ebx
    pop eax
    jae short o1_L2d1
    mov dword [ebx + 0x10], strict dword 0xffffffff
    stc
o1_L2d1:
    pop esi
    ret
            db   03Ch, 000h, 0B0h, 078h, 075h, 003h, 08Ah, 043h, 02Dh, 0B0h
            db   'x:C,u'
            db   001h, 0C3h, 052h, 053h, 088h, 043h, 02Ch, 066h, 08Bh, 055h, 002h, 08Ah, 0D8h, 08Ah, 0FBh, 066h
            db   0B8h, 003h, 000h, 0FFh, 055h, 064h, 05Bh, 05Ah, 0C3h, 000h, 0F8h, 0C3h
o1_L2fe:
    clc
    ret
            db   066h, 02Bh, 0C0h, 0C3h
o1_L304:
    db 0xff,0x05                            ; inc dword ptr [0x664]
o1_S306:
    dd o1_start+0x664
o1_L30a:
    db 0xc7,0x05                            ; mov dword ptr [0x66c], 0x304
o1_S30c:
    dd o1_start+0x66c
o1_S310:
    dd o1_L304
    mov ax, strict word 0
    mov_ bx, ax
    ret
o1_L31c:
    db 0xff,0x05                            ; inc dword ptr [0x65c]
o1_S31e:
    dd o1_D65c
o1_L322:
    db 0xc7,0x05                            ; mov dword ptr [0x66c], 0x31c
o1_S324:
    dd o1_start+0x66c
o1_S328:
    dd o1_L31c
    cld
    push ecx
    push edx
    push esi
    push edi
    mov esi, dword [ebp + 4]
    call o1_L2a0
    jae short o1_L33d
    jmp short o1_L39f
o1_L33d:
    and esi, dword [ebx + 0x24]
    add esi, dword [ebx + 0x18]
    mov edi, dword [ebp + 0x18]
    mov ecx, dword [ebp + 0xc]
    or_ ecx, ecx
    jne short o1_L34f
    jmp short o1_L3a3
o1_L34f:
    test esi, strict dword 1
    je short o1_L35b
    movsb
    dec ecx
    je short o1_L3a3
o1_L35b:
    mov byte [ebp + 0x12], cl
    and ecx, 0xfffffffe
    mov_ edx, ecx
o1_L363:
    mov_ ecx, edx
    cmp esi, dword [ebx + 0x1c]
    jb short o1_L37c
    mov esi, dword [ebp + 4]
    add esi, dword [ebp + 0xc]
    sub_ esi, edx
    call o1_L2a0
    jb short o1_L39f
    mov esi, dword [ebx + 0x18]
o1_L37c:
    mov eax, dword [ebx + 0x1c]
    sub_ eax, esi
    cmp_ eax, ecx
    jae short o1_L387
    mov_ ecx, eax
o1_L387:
    sub_ edx, ecx
    shr ecx, 1
    jecxz o1_L39c
    db 0xf3,0x66,0xa5                       ; rep movsw word ptr es:[edi], word ptr [esi]
    or_ edx, edx
    jne short o1_L363
    test byte [ebp + 0x12], 1
    jne short o1_L363
    jmp short o1_L3a3
o1_L39c:
    movsb
    jmp short o1_L3a3
o1_L39f:
    mov_ ebx, eax
    jmp short o1_L3a7
o1_L3a3:
    mov bx, strict word 0
o1_L3a7:
    mov ax, strict word 0
    pop edi
    pop esi
    pop edx
    pop ecx
    ret
o1_L3b0:
    db 0xff,0x05                            ; inc dword ptr [0x660]
o1_S3b2:
    dd o1_start+0x660
o1_L3b6:
    db 0xc7,0x05                            ; mov dword ptr [0x66c], 0x3b0
o1_S3b8:
    dd o1_start+0x66c
o1_S3bc:
    dd o1_L3b0
    cld
    push ecx
    push edx
    push esi
    push edi
    test word [ebx + 0x16], strict word 1
    je short o1_L3d3
    mov ax, strict word 0x1a
    jmp short o1_L44b
o1_L3d3:
    mov esi, dword [ebp + 8]
    call o1_L2a0
    jae short o1_L3df
    jmp short o1_L44b
o1_L3df:
    mov_ edi, esi
    and edi, dword [ebx + 0x24]
    add edi, dword [ebx + 0x18]
    mov esi, dword [ebp + 0x18]
    mov ecx, dword [ebp + 0xc]
    or_ ecx, ecx
    jne short o1_L3f3
    jmp short o1_L447
o1_L3f3:
    test edi, strict dword 1
    je short o1_L401
    movsb
    dec ecx
    jne short o1_L401
    jmp short o1_L447
o1_L401:
    mov byte [ebp + 0x12], cl
    and ecx, 0xfffffffe
    mov_ edx, ecx
o1_L409:
    mov_ ecx, edx
    cmp edi, dword [ebx + 0x1c]
    jb short o1_L426
    mov_ edi, esi
    mov esi, dword [ebp + 8]
    add esi, dword [ebp + 0xc]
    sub_ esi, edx
    call o1_L2a0
    jb short o1_L44b
    mov_ esi, edi
    mov edi, dword [ebx + 0x18]
o1_L426:
    mov eax, dword [ebx + 0x1c]
    sub_ eax, edi
    cmp_ eax, ecx
    jae short o1_L431
    mov_ ecx, eax
o1_L431:
    sub_ edx, ecx
    shr ecx, 1
    jecxz o1_L446
    db 0xf3,0x66,0xa5                       ; rep movsw word ptr es:[edi], word ptr [esi]
    or_ edx, edx
    jne short o1_L409
    test byte [ebp + 0x12], 1
    jne short o1_L409
    jmp short o1_L447
o1_L446:
    movsb
o1_L447:
    mov ax, strict word 0
o1_L44b:
    mov_ bx, ax
    mov ax, strict word 0
    pop edi
    pop esi
    pop edx
    pop ecx
    ret
o1_L457:
    db 0xff,0x05                            ; inc dword ptr [0x668]
o1_S459:
    dd o1_start+0x668
o1_L45d:
    db 0xc7,0x05                            ; mov dword ptr [0x66c], 0x457
o1_S45f:
    dd o1_start+0x66c
o1_S463:
    dd o1_L457
    push edx
    mov edx, dword [ebp + 4]
    call o1_L2fe
    jb short o1_L47c
    mov edx, dword [ebp + 8]
    call o1_L2fe
    jae short o1_L486
o1_L47c:
    mov ax, strict word 0x18
    mov bx, strict word 0
    pop edx
    ret
o1_L486:
    pop edx
    push dword [ebp + 4]
    push dword [ebp + 8]
    mov edx, dword [ebp + 0xc]
o1_L490:
    db 0xbe                                 ; mov esi, 0x5c8
o1_S491:
    dd o1_L5c8
o1_L495:
    mov_ ecx, edx
    jecxz o1_L4e3
    mov edi, dword [ebp + 4]
    and edi, 1
    cmp ecx, 0x80
    jb short o1_L4ae
    mov ecx, 0x80
    sub_ ecx, edi
o1_L4ae:
    add_ edi, esi
    mov dword [ebp + 0xc], ecx
    mov dword [ebp + 0x18], edi
    push ebx
    call o1_L31c
    or_ ax, ax
    jne short o1_L4de
    pop ebx
    add dword [ebp + 4], ecx
    mov dword [ebp + 0xc], ecx
    mov dword [ebp + 0x18], edi
    push ebx
    call o1_L3b0
    or_ ax, ax
    jne short o1_L4de
    pop ebx
    add dword [ebp + 8], ecx
    sub_ edx, ecx
    jmp short o1_L495
o1_L4de:
    add esp, 4
    jmp short o1_L4ea
o1_L4e3:
    mov ax, strict word 0
    mov_ bx, ax
o1_L4ea:
    pop dword [ebp + 8]
    pop dword [ebp + 4]
    ret
o1_D4f1:
    dd o1_L304
o1_D4f5:
    dd o1_L31c
o1_D4f9:
    dd o1_L3b0
o1_D4fd:
    dd o1_L457
o1_L501:
    push dword [ebp + 0xc]
    push dword [ebp + 0x10]
    push dword [ebp + 0x18]
    push ebx
    mov dword [ebp + 0x18], ebx
    mov_ ebx, esi
    mov word [ebp + 2], cx
    xchg esi, eax
    mov al, byte [ebp + 0x10]
    xchg esi, eax
    and esi, 3
    shl esi, 2
o1_L51f:
    db 0xff,0x96                            ; call dword ptr [esi + 0x4f1]
o1_S521:
    dd o1_D4f1
    mov word [ebp + 0x14], bx
    pop ebx
    pop dword [ebp + 0x18]
    pop dword [ebp + 0x10]
    pop dword [ebp + 0xc]
    or_ ax, ax
    jne short o1_L539
    ret
o1_L539:
    stc
    ret
            db   000h
    add byte [eax], al
    add byte [eax], al
    add byte [eax + eax], al
    add byte [eax + eax], al
    add byte [eax], al
    push ebx
    push edx
    inc ecx
    dec ebp
    dec ebp
    push esp
    inc esp
    and byte [eax], al
            db   000h, 080h, 016h
o1_D554:
    dd o1_L211
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   'verPP'
            db   000h, 000h, 000h
            db   '1vsR2vsR3vsR'
o1_L58c:
    add byte [eax], al
o1_L58e:
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
o1_L594:
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
o1_L59a:
    add byte [eax], al
o1_L59c:
    add byte [eax], al
o1_L59e:
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
o1_L5a6:
    add byte [eax], al
o1_L5a8:
    add byte [eax], al
o1_L5aa:
    add byte [eax], al
o1_L5ac:
    add byte [eax], al
    add byte [eax], al
o1_L5b0:
    add byte [eax], al
o1_L5b2:
    add byte [eax], al
o1_L5b4:
    add byte [eax], al
    add byte [eax], al
o1_L5b8:
    add byte [eax], al
o1_L5ba:
    add byte [eax], al
o1_L5bc:
    add byte [eax], al
    add byte [eax], al
o1_L5c0:
    add byte [eax], al
    add byte [eax], al
o1_L5c4:
    add byte [eax], al
    add byte [eax], al
o1_L5c8:
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    inc ebx
    inc ecx
    dec esp
    dec esp
    and byte [ebx + 0x4f], al
    push ebp
    dec esi
    push esp
    inc ebp
    push edx
    push ebx
    sub byte [edx + 0x57], dl
    inc ebp
    inc ebx
    sub dword [edx], edi
o1_D65c:

section part2 start=0xa5c vstart=0xa5c align=1
            db   '$SRAMMTD Virtual Device'
            db   009h
            db   '(Version 4.0)'
            db   000h, 000h, 00Bh
            db   'SRAMMTD_DDB'
            db   001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h, 0E4h, 001h, 000h, 000h, 0E4h
            db   001h, 034h, 000h
            db   'VS_VERSION_INFO'
            db   000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h
            db   000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 001h, 000h, 001h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 075h, 001h, 000h, 000h
            db   'StringFileInfo'
            db   000h, 000h, 061h, 001h, 000h, 000h
            db   '040904E4'
            db   000h, 000h, 000h, 000h, 026h, 000h, 016h, 000h
            db   'CompanyName'
            db   000h
            db   'Microsoft Corporation'
            db   000h, 000h, 000h, 039h, 000h, 025h, 000h
            db   'FileDescription'
            db   000h
            db   'SRAMMTD Virtual Device (Version 4.0)'
            db   000h, 000h, 000h, 000h, 019h, 000h, 009h, 000h
            db   'FileVersion'
            db   000h
            db   '4.00.950'
            db   000h, 000h, 000h, 000h, 01Ch, 000h, 008h, 000h
            db   'InternalName'
            db   000h, 000h, 000h, 000h
            db   'SRAMMTD'
            db   000h, 03Ah, 000h, 026h, 000h
            db   'LegalCopyright'
            db   000h, 000h
            db   'Copyright '
            db   0A9h
            db   ' Microsoft Corp. 1988-1995'
            db   000h, 000h, 000h, 024h, 000h, 00Ch, 000h
            db   'OriginalFilename'
            db   000h, 000h, 000h, 000h
            db   'SRAMMTD.VXD'
            db   000h, 035h, 000h, 025h, 000h
            db   'ProductName'
            db   000h
            db   'Microsoft'
            db   0AEh
            db   ' Windows'
            db   0AEh
            db   ' Operating System'
            db   000h, 000h, 000h, 000h, 01Dh, 000h, 009h, 000h
            db   'ProductVersion'
            db   000h, 000h
            db   '4.00.950'
            db   000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
            db   'VarFileInfo'
            db   000h, 014h, 000h, 004h, 000h
            db   'Translation'
            db   000h, 009h, 004h, 0E4h, 004h
