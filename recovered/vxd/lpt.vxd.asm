; LPT -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 5827 instructions recovered by recursive descent

Declare_Virtual_Device LPT, 1, 0, LPT_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x489c  R-X
        db  083h, 0ECh, 008h, 056h, 057h, 08Bh, 074h, 024h, 014h, 066h, 0C7h, 044h, 024h, 00Ah, 000h, 000h
        db  066h, 08Bh, 0BEh, 094h, 000h, 000h, 000h, 06Ah, 023h, 00Fh, 0B7h, 0C7h, 040h, 06Ah, 000h, 06Ah
        db  008h, 050h, 0E8h, 011h, 03Eh, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 00Fh, 084h, 085h, 000h
        db  000h, 000h, 08Dh, 044h, 024h, 00Ch, 050h, 08Dh, 044h, 024h, 00Eh, 06Ah, 002h, 050h, 057h, 0E8h
        db  06Fh, 03Ch, 000h, 000h, 08Ah, 064h, 024h, 01Ah, 08Ah, 044h, 024h, 01Bh, 066h, 089h, 044h, 024h
        db  01Ah, 083h, 0C4h, 010h, 066h, 03Dh, 002h, 000h, 072h, 05Dh, 00Fh, 0B7h, 044h, 024h, 00Ah, 06Ah
        db  001h, 083h, 0E8h, 002h, 050h, 0E8h
        dd  sub_6_0018               ; fixup
        db  083h, 0C4h, 008h, 089h, 086h, 0A8h, 000h, 000h, 000h, 08Dh, 044h, 024h, 00Ch, 050h, 00Fh, 0B7h
        db  044h, 024h, 00Eh, 083h, 0E8h, 002h, 050h, 0FFh, 0B6h, 0A8h, 000h, 000h, 000h, 066h, 08Bh, 086h
        db  094h, 000h, 000h, 000h, 050h, 0E8h, 01Fh, 03Ch, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 083h, 0C4h
        db  010h, 089h, 086h, 0A4h, 000h, 000h, 000h, 08Bh, 046h, 034h, 089h, 046h, 00Ch, 0C7h, 046h, 034h
        db  000h, 000h, 000h, 000h, 056h, 0E8h, 004h, 001h, 000h, 000h, 083h, 0C4h, 004h, 05Fh, 05Eh, 083h
        db  0C4h, 008h, 0C3h
sub_1_00BD:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        movzx  eax, word ptr [esi + 0x98]
        cmp    eax, 1
        je     loc_1_00D9
        cmp    eax, 2
        je     loc_1_0128
        jmp    loc_1_01B0
loc_1_00D9:
        mov    byte ptr [esi + 0xb3], 3
        mov    byte ptr [esi + 0xb0], 0xff
        mov    dword ptr [esi + 0xb4], 0x1d6e ; -> loc_1_1D6E
        mov    edi, dword ptr [esp + 0x10]
        cmp    edi, 1
        cmc    
        sbb    al, al
        and    al, 4
        push   eax
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_01A2
        or     edi, edi
        je     loc_1_01B0
        push   esi
        call   0
        add    esp, 4
        jmp    loc_1_01B0
loc_1_0128:
        mov    byte ptr [esi + 0xb3], 3
        mov    byte ptr [esi + 0xb0], 0x10
        mov    dword ptr [esi + 0xb4], 0x1d48 ; -> loc_1_1D48
        mov    edi, dword ptr [esp + 0x10]
        cmp    edi, 1
        sbb    al, al
        and    al, 0xfc
        add    al, 0x14
        push   eax
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_0176
        or     edi, edi
        je     loc_1_01B0
        push   esi
        call   sub_1_0920
        add    esp, 4
        push   esi
        call   sub_1_01B8
        add    esp, 4
        jmp    loc_1_01B0
loc_1_0176:
        or     eax, eax
        jne    loc_1_01A2
        cmp    edi, 1
        cmc    
        sbb    al, al
        and    al, 4
        push   eax
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_01A2
        or     edi, edi
        je     loc_1_01B0
        push   esi
        call   0
        add    esp, 4
        jmp    loc_1_01B0
loc_1_01A2:
        mov    byte ptr [esi + 0xb2], 0x80
        mov    byte ptr [esi + 0xb1], 0xff
loc_1_01B0:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_1_01B8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi + 0xb2]
        cmp    al, 0x80
        je     loc_1_0283
        movzx  ebx, word ptr [esi + 0x94]
        lea    edi, [ebx + 0x402]
        add    ebx, 2
        cmp    al, 0x40
        jne    loc_1_01F1
        push   0x14
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        jmp    loc_1_0283
loc_1_01F1:
        test   al, 0x10
        je     loc_1_0221
        test   byte ptr [esi + 0xb3], 4
        je     loc_1_0216
        push   esi
        call   sub_1_07BB
        add    esp, 4
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xdf
        push   eax
        push   ebx
        jmp    loc_1_0219
loc_1_0216:
        push   0x34
        push   edi
loc_1_0219:
        call   sub_1_3F2E
        add    esp, 8
loc_1_0221:
        cmp    dword ptr [esi + 0xac], 4
        je     loc_1_0237
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_1_0237
        push   1
        jmp    loc_1_0239
loc_1_0237:
        push   0
loc_1_0239:
        mov    ax, word ptr [esi + 0x94]
        push   eax
        call   sub_1_3A70
        add    esp, 8
        cmp    word ptr [esi + 0x9a], 2
        jne    loc_1_025E
        push   0x14
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_025E:
        mov    byte ptr [esi + 0xb2], 0x80
        mov    byte ptr [esi + 0xb3], 3
        mov    dword ptr [esi + 0xac], 0
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0
loc_1_0283:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0287:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    bx, word ptr [esi + 0x94]
        movzx  ebp, bx
        lea    eax, [ebp + 1]
        mov    dword ptr [esp + 0x10], eax
        lea    edi, [ebp + 2]
        add    ebp, 0x402
        test   byte ptr [esp + 0x1c], 4
        je     loc_1_02BD
        mov    dword ptr [esi + 0xac], 4
loc_1_02BD:
        push   0x34
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_3B3C
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 0x10000
        je     loc_1_02FE
        push   0x14
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x8000
        mov    eax, ebx
        jmp    loc_1_03AF
loc_1_02FE:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0xf0
        push   0xf0
        push   dword ptr [esp + 0x1c]
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0349
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x8000
        sub    eax, eax
        jmp    loc_1_03AF
loc_1_0349:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xfc
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x34
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   0x74
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0xb2], 0x10
        mov    byte ptr [esi + 0xb3], 0x23
        mov    byte ptr [esi + 0xb1], 0x10
        mov    dword ptr [esi + 0xb4], 0x1d22 ; -> loc_1_1D22
        mov    dword ptr [esi + 0xb8], 0x1dec ; -> loc_1_1DEC
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, 0x10000
        mov    dword ptr [esi + 0x34], 0
loc_1_03AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_03B7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bl, byte ptr [esp + 0x18]
        test   bl, 4
        mov    esi, dword ptr [esp + 0x14]
        je     loc_1_03D2
        mov    dword ptr [esi + 0xac], 4
loc_1_03D2:
        mov    bp, word ptr [esi + 0x9a]
        cmp    bp, 2
        jne    loc_1_03F6
        movzx  eax, word ptr [esi + 0x94]
        add    eax, 0x402
        push   0x34
        push   eax
        call   sub_1_3F2E
        add    esp, 8
loc_1_03F6:
        mov    di, word ptr [esi + 0x94]
        push   ebx
        push   edi
        call   sub_1_3B3C
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 0x10000
        je     loc_1_043E
        cmp    bp, 2
        jne    loc_1_042A
        movzx  eax, di
        add    eax, 0x402
        push   0x14
        push   eax
        call   sub_1_3F2E
        add    esp, 8
loc_1_042A:
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x8000
        mov    eax, ebx
        jmp    loc_1_04BF
loc_1_043E:
        mov    byte ptr [esi + 0xb2], 0
        mov    byte ptr [esi + 0xb3], 0x16
        mov    byte ptr [esi + 0xb1], 0
        movzx  edi, di
        mov    dword ptr [esi + 0xb8], 0x1e36 ; -> loc_1_1E36
        lea    eax, [edi + 1]
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    loc_1_0479
        mov    byte ptr [esi + 0xb3], 0x1c
        jmp    loc_1_04AD
loc_1_0479:
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0xb3], 0x16
loc_1_04AD:
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0
        mov    eax, 0x10000
loc_1_04BF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   esi
        push   edi
        push   0x4894                        ; -> obj1:0x4894
        call   sub_3_0E00
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
        add    esp, 4
loc_1_04E7:
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     loc_1_0517
        push   0x4898                        ; -> obj1:0x4898
        call   sub_3_0E00
        add    esp, 4
        mov    eax, dword ptr [0x4898]       ; -> obj1:0x4898
        sub    eax, dword ptr [0x4894]       ; -> obj1:0x4894
        cmp    eax, edi
        jl     loc_1_04E7
        sub    eax, eax
loc_1_0514:
        pop    edi
        pop    esi
        ret    
loc_1_0517:
        mov    eax, 1
        jmp    loc_1_0514
sub_1_051E:
        push   esi
        push   edi
        push   0x4894                        ; -> obj1:0x4894
        call   sub_3_0E00
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
        add    esp, 4
loc_1_0535:
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 2
        je     loc_1_0565
        push   0x4898                        ; -> obj1:0x4898
        call   sub_3_0E00
        add    esp, 4
        mov    eax, dword ptr [0x4898]       ; -> obj1:0x4898
        sub    eax, dword ptr [0x4894]       ; -> obj1:0x4894
        cmp    eax, edi
        jl     loc_1_0535
        sub    eax, eax
loc_1_0562:
        pop    edi
        pop    esi
        ret    
loc_1_0565:
        mov    eax, 1
        jmp    loc_1_0562
        db  056h, 08Bh, 074h, 024h, 008h, 080h, 0BEh, 0B0h, 000h, 000h, 000h, 010h, 075h, 042h, 068h
        dd  obj1:0x4894              ; fixup
        db  0E8h
        dd  sub_3_0E00               ; fixup
        db  083h, 0C4h, 004h, 083h, 0BEh, 060h, 001h, 000h, 000h, 000h, 074h, 02Eh, 068h
        dd  obj1:0x4898              ; fixup
        db  0E8h
        dd  sub_3_0E00               ; fixup
        db  083h, 0C4h, 004h, 0A1h
        dd  obj1:0x4898              ; fixup
        db  02Bh, 005h
        dd  obj1:0x4894              ; fixup
        db  083h, 0F8h, 064h, 072h, 0DAh, 08Bh, 046h, 034h, 089h, 046h, 00Ch, 0C7h, 046h, 034h, 000h, 002h
        db  000h, 000h, 02Bh, 0C0h, 05Eh, 0C3h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 0F7h
sub_1_05C5:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        movzx  edi, word ptr [eax + 0x94]
        movzx  ecx, byte ptr [eax + 0xb3]
        lea    esi, [edi + 0x402]
        add    edi, 2
        cmp    ecx, 0x23
        je     loc_1_0607
        cmp    ecx, 0x26
        je     loc_1_0614
        cmp    ecx, 0x2c
        je     loc_1_0614
        push   0x10
        push   eax
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_1_0645
        sub    eax, eax
        jmp    loc_1_064A
loc_1_0607:
        push   0x34
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        jmp    loc_1_0625
loc_1_0614:
        push   eax
        call   sub_1_07BB
        add    esp, 4
        or     eax, eax
        jne    loc_1_0625
        sub    eax, eax
        jmp    loc_1_064A
loc_1_0625:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xdf
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x74
        push   esi
        call   sub_1_3F2E
        add    esp, 8
loc_1_0645:
        mov    eax, 1
loc_1_064A:
        pop    edi
        pop    esi
        ret    
sub_1_064D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
loc_1_0658:
        movzx  eax, byte ptr [esi + 0xb3]
        cmp    eax, 3
        je     loc_1_067B
        cmp    eax, 0x23
        je     loc_1_068D
        cmp    eax, 0x26
        je     loc_1_06A2
loc_1_066E:
        mov    eax, 1
loc_1_0673:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_067B:
        push   0x10
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_069E
loc_1_068D:
        push   esi
        call   sub_1_0711
        add    esp, 4
        or     eax, eax
        jne    loc_1_0658
        sub    eax, eax
        jmp    loc_1_0673
loc_1_069E:
        sub    eax, eax
        jmp    loc_1_0673
loc_1_06A2:
        movzx  edi, word ptr [esi + 0x94]
        lea    eax, [edi + 0x402]
        mov    dword ptr [esp + 0x10], eax
        lea    ebp, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 0x20
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x74
        push   dword ptr [esp + 0x14]
        call   sub_1_3F2E
        add    esp, 8
        push   ebp
        call   sub_1_3E90
        add    esp, 4
        test   al, 0x40
        je     loc_1_06F8
        mov    byte ptr [esi + 0xb3], 0x26
        jmp    loc_1_066E
loc_1_06F8:
        and    bl, 0xfd
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0xb3], 0x2c
        jmp    loc_1_066E
sub_1_0711:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   0xa
        movzx  edi, word ptr [esi + 0x94]
        lea    ebx, [edi + 0x402]
        lea    ebp, [edi + 1]
        add    edi, 2
        push   ebx
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_074C
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x100
        sub    eax, eax
        jmp    loc_1_07B6
loc_1_074C:
        push   0x34
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   edi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 2
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        and    bl, 0xfb
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0
        push   0x20
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_07AA
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x2000
        sub    eax, eax
        jmp    loc_1_07B6
loc_1_07AA:
        mov    byte ptr [esi + 0xb3], 0x26
        mov    eax, 1
loc_1_07B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_07BB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        push   0x34
        movzx  ebp, word ptr [edi + 0x94]
        lea    eax, [ebp + 0x402]
        lea    esi, [ebp + 2]
        inc    ebp
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 4
        push   ebx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0xe0
        push   0xe0
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0821
        mov    eax, dword ptr [edi + 0x34]
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x34], 0x2000
        sub    eax, eax
        jmp    loc_1_083A
loc_1_0821:
        and    bl, 0xfd
        push   ebx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [edi + 0xb3], 0x23
        mov    eax, 1
loc_1_083A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_083F:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x28]
        movzx  eax, word ptr [edx + 0x94]
        mov    dword ptr [esp + 0x18], esi
        lea    ecx, [eax + 0x400]
        mov    dword ptr [esp + 0x10], ecx
        add    eax, 0x402
        mov    dword ptr [esp + 0x14], eax
        movzx  edi, word ptr [edx + 0x122]
        mov    eax, esi
        cmp    edi, eax
        jb     loc_1_087B
        mov    edi, eax
loc_1_087B:
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [ecx], esi
        movzx  eax, word ptr [edx + 0x120]
        cmp    eax, 1
        je     loc_1_08AB
        cmp    eax, 2
        je     loc_1_08E4
loc_1_0892:
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x2c]
        sub    eax, esi
        mov    dword ptr [ecx], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_08AB:
        mov    ebp, dword ptr [esp + 0x24]
loc_1_08AF:
        push   0x64
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_0892
        movzx  ebx, di
        push   ebx
        push   ebp
        sub    esi, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3E9E
        add    esp, 0xc
        add    ebp, ebx
        cmp    di, si
        jbe    loc_1_08DE
        mov    di, si
loc_1_08DE:
        or     esi, esi
        jne    loc_1_08AF
        jmp    loc_1_0892
loc_1_08E4:
        mov    ebp, dword ptr [esp + 0x24]
loc_1_08E8:
        push   0x64
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_0892
        movzx  ebx, di
        push   ebx
        push   ebp
        sub    esi, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3ECB
        add    esp, 0xc
        add    ebp, ebx
        cmp    di, si
        jbe    loc_1_0917
        mov    di, si
loc_1_0917:
        or     esi, esi
        jne    loc_1_08E8
        jmp    loc_1_0892
sub_1_0920:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        push   0x34
        movzx  edi, word ptr [esi + 0x94]
        add    edi, 0x402
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_0711
        add    esp, 4
        or     eax, eax
        je     loc_1_0A06
        push   0x64
        push   edi
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        je     loc_1_0973
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0
        jmp    loc_1_0A06
loc_1_0973:
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0xe]
        push   2
        push   eax
        push   esi
        call   sub_1_083F
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0A06
        mov    ah, byte ptr [esp + 0xa]
        mov    al, byte ptr [esp + 0xb]
        mov    word ptr [esp + 0xa], ax
        cmp    ax, 2
        jbe    loc_1_0A06
        sub    word ptr [esp + 0xa], 2
        cmp    word ptr [esi + 0x120], 2
        jne    loc_1_09C8
        mov    ax, word ptr [esp + 0xa]
        shr    ax, 1
        mov    cx, word ptr [esp + 0xa]
        and    cx, 1
        add    ax, cx
        mov    word ptr [esp + 0xa], ax
loc_1_09C8:
        movzx  eax, word ptr [esp + 0xa]
        push   1
        push   eax
        call   sub_6_0018
        add    esp, 8
        mov    dword ptr [esi + 0xa8], eax
        lea    eax, [esp + 0xc]
        push   eax
        movzx  eax, word ptr [esp + 0xe]
        push   eax
        push   dword ptr [esi + 0xa8]
        push   esi
        call   sub_1_083F
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0A06
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0xa4], eax
loc_1_0A06:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  056h, 057h, 08Bh, 074h, 024h, 00Ch, 06Ah, 014h, 00Fh, 0B7h, 086h, 094h, 000h, 000h, 000h, 08Dh
        db  078h, 002h, 005h, 002h, 004h, 000h, 000h, 050h, 0E8h, 005h, 035h, 000h, 000h, 083h, 0C4h, 008h
        db  08Bh, 086h, 034h, 001h, 000h, 000h, 066h, 081h, 078h, 00Ah, 000h, 008h, 075h, 00Eh, 0FFh, 0B6h
        db  038h, 001h, 000h, 000h, 0E8h, 0AEh, 02Eh, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 086h, 034h, 001h
        db  000h, 000h, 0C7h, 086h, 058h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 0C7h, 040h, 00Ah
        db  000h, 000h, 0C7h, 046h, 058h, 000h, 000h, 000h, 000h, 0C7h, 046h, 05Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 04Ch, 000h, 000h, 000h, 000h, 0C7h, 046h, 054h, 000h, 000h, 000h, 000h, 056h, 0E8h
        db  0ECh, 0FAh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h, 0B8h, 000h, 000h, 000h, 000h, 074h, 038h
        db  056h, 0E8h, 026h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 068h, 0CCh, 000h, 000h, 000h, 057h, 0E8h
        db  08Eh, 034h, 000h, 000h, 083h, 0C4h, 008h, 068h, 0C8h, 000h, 000h, 000h, 057h, 0E8h, 080h, 034h
        db  000h, 000h, 083h, 0C4h, 008h, 068h, 0CCh, 000h, 000h, 000h, 057h, 0E8h, 072h, 034h, 000h, 000h
        db  083h, 0C4h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh, 0C3h
sub_1_0AC7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        movzx  edi, word ptr [esi + 0x94]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0x40
        mov    byte ptr [esp + 0xf], al
        mov    bl, al
        or     bl, 0xc
        push   ebx
        push   edi
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 0x17]
        add    esp, 8
        or     al, 8
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x4894                        ; -> obj1:0x4894
        call   sub_3_0E00
        add    esp, 4
loc_1_0B16:
        push   0x4898                        ; -> obj1:0x4898
        call   sub_3_0E00
        add    esp, 4
        mov    eax, dword ptr [0x4898]       ; -> obj1:0x4898
        sub    eax, dword ptr [0x4894]       ; -> obj1:0x4894
        cmp    eax, 0x12c
        jl     loc_1_0B16
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        movzx  eax, word ptr [esi + 0x94]
        inc    eax
        push   eax
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        mov    byte ptr [esi + 0xb3], 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  056h, 08Bh, 074h, 024h, 008h, 083h, 0BEh, 060h, 001h, 000h, 000h, 000h, 075h, 037h, 056h, 0E8h
        db  09Eh, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h, 074h, 02Ah, 066h, 083h, 0BEh, 020h, 001h
        db  000h, 000h, 001h, 056h, 075h, 007h, 0E8h, 029h, 02Ah, 000h, 000h, 0EBh, 005h, 0E8h, 078h, 028h
        db  000h, 000h, 083h, 0C4h, 004h, 080h, 0B6h, 06Eh, 001h, 000h, 000h, 001h, 056h, 0E8h, 01Ah, 0FCh
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 05Eh, 0C3h
sub_1_0BA6:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        sub    esp, 8
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x140], 0
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x3b], 1
        je     loc_1_0BE1
        push   eax
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        je     loc_1_0DC1
loc_1_0BE1:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x168], 0x80
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x160], 0
        je     loc_1_0C0A
        cmp    byte ptr [eax + 0xb2], 0
        je     loc_1_0C0A
        jmp    loc_1_0DB7
loc_1_0C0A:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0xb2]
        or     al, al
        je     loc_1_0C6A
        cmp    al, 0x40
        je     loc_1_0C20
        cmp    al, 0x10
        jne    loc_1_0C44
loc_1_0C20:
        mov    eax, dword ptr [esp + 4]
        push   0x64
        movzx  eax, word ptr [eax + 0x94]
        add    eax, 0x402
        push   eax
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_0C44
        jmp    loc_1_0DB3
loc_1_0C44:
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        add    esp, 4
        push   0
        push   dword ptr [esp + 8]
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_1_0C6A
        jmp    loc_1_0DB3
loc_1_0C6A:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x160], 1
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xbc], 0x172e ; -> loc_1_172E
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x14c], 0x10
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 4]
        add    ecx, 0x54
        mov    esi, dword ptr [eax + 0x40]
        sub    esi, dword ptr [ecx]
        mov    dword ptr [esp + 8], 0
        mov    eax, dword ptr [eax + 0x3c]
        add    eax, dword ptr [ecx]
        lea    ecx, [esp + 8]
        push   ecx
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ax, word ptr [eax + 0x94]
        push   eax
        call   sub_1_3CB3
        add    esp, 0x10
        cmp    dword ptr [esp + 8], 0
        je     loc_1_0D20
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    dword ptr [eax + 0x4c], ecx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    dword ptr [eax + 0x54], ecx
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x150], 1
        je     loc_1_0D20
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0x54]
        cmp    dword ptr [ecx + 0xf0], eax
        jg     loc_1_0D20
        push   1
        push   dword ptr [esp + 8]
        call   sub_1_1B99
        add    esp, 8
loc_1_0D20:
        cmp    dword ptr [esp + 8], esi
        jne    loc_1_0D53
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [eax + 0x160], 0
        mov    eax, dword ptr [esp + 4]
        xor    dword ptr [eax + 0x168], 0x80
        jmp    loc_1_0DDE
loc_1_0D53:
        mov    eax, dword ptr [esp + 4]
        movzx  esi, word ptr [eax + 0x94]
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    loc_1_0D7B
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0xb3], 0x1c
        jmp    loc_1_0DB3
loc_1_0D7B:
        add    esi, 2
        push   esi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   esi
        call   sub_1_3F2E
        mov    eax, dword ptr [esp + 0xc]
        add    esp, 8
        mov    byte ptr [eax + 0xb3], 0x16
loc_1_0DB3:
        mov    eax, dword ptr [esp + 4]
loc_1_0DB7:
        xor    dword ptr [eax + 0x168], 0x80
loc_1_0DC1:
        push   dword ptr [esp + 4]
        push   0x3e8
        push   0xbb0                         ; -> sub_1_0BB0
        call   sub_6_0040
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x140], eax
loc_1_0DDE:
        pop    esi
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0DF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        movzx  ebx, word ptr [edi + 0x94]
        mov    esi, dword ptr [edi + 0x3c]
        lea    eax, [ebx + 0x402]
        mov    dword ptr [esp + 0x10], eax
        add    ebx, 0x400
        add    esi, dword ptr [edi + 0x54]
        push   eax
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     loc_1_0E7F
loc_1_0E27:
        mov    ebp, dword ptr [edi + 0x4c]
        cmp    ebp, dword ptr [edi + 0x40]
        ja     loc_1_0E7F
        movzx  eax, word ptr [edi + 0x120]
        cmp    eax, 1
        je     loc_1_0E42
        cmp    eax, 2
        je     loc_1_0E57
        jmp    loc_1_0E6D
loc_1_0E42:
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [esi], al
        inc    esi
        inc    ebp
        mov    dword ptr [edi + 0x4c], ebp
        inc    dword ptr [edi + 0x54]
        jmp    loc_1_0E6D
loc_1_0E57:
        mov    dx, bx
        in     ax, dx
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 1], ah
        inc    esi
        inc    esi
        add    ebp, 2
        mov    dword ptr [edi + 0x4c], ebp
        add    dword ptr [edi + 0x54], 2
loc_1_0E6D:
        push   dword ptr [esp + 0x10]
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        jne    loc_1_0E27
loc_1_0E7F:
        call   sub_1_39D1
        mov    dword ptr [edi + 0x88], eax
        test   byte ptr [edi + 0x150], 1
        je     loc_1_0EA9
        mov    eax, dword ptr [edi + 0x54]
        cmp    dword ptr [edi + 0xf0], eax
        jg     loc_1_0EA9
        push   1
        push   edi
        call   sub_1_1B99
        add    esp, 8
loc_1_0EA9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0EB1:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], edx
        mov    eax, dword ptr [esp + 8]
        movzx  esi, word ptr [eax + 0x94]
        add    esi, 0x402
        cmp    dword ptr [eax + 0x14c], 8
        jne    loc_1_0F0C
        movzx  eax, word ptr [eax + 0x120]
        cmp    eax, 1
        je     loc_1_0EEA
        cmp    eax, 2
        je     loc_1_0EFB
        jmp    loc_1_0FE0
loc_1_0EEA:
        push   dword ptr [esp + 8]
        call   sub_1_35B3
        add    esp, 4
        jmp    loc_1_0FE0
loc_1_0EFB:
        push   dword ptr [esp + 8]
        call   sub_1_3409
        add    esp, 4
        jmp    loc_1_0FE0
loc_1_0F0C:
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [eax + 0x58]
        or     edi, edi
        jne    loc_1_0F7C
        push   0xa
        push   esi
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_0F45
        push   dword ptr [esp + 8]
        push   0xa
        push   0xeb1                         ; -> loc_1_0EB1
        call   sub_6_0040
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_0FE0
loc_1_0F45:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0xec], edi
        jg     loc_1_0F55
        or     edi, edi
        jne    loc_1_0F7C
loc_1_0F55:
        push   dword ptr [esp + 8]
        call   sub_1_196F
        mov    eax, dword ptr [esp + 0xc]
        add    esp, 4
        test   byte ptr [eax + 0x150], 2
        je     loc_1_0F7C
        push   2
        push   dword ptr [esp + 0xc]
        call   sub_1_1B99
        add    esp, 8
loc_1_0F7C:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_0FC4
        push   esi
        call   sub_1_3E90
        mov    ecx, dword ptr [esp + 0xc]
        add    esp, 4
        movzx  ecx, word ptr [ecx + 0x120]
        cmp    ecx, 1
        je     loc_1_0FA6
        cmp    ecx, 2
        je     loc_1_0FB5
        jmp    loc_1_0FE0
loc_1_0FA6:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_2F45
        add    esp, 8
        jmp    loc_1_0FE0
loc_1_0FB5:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_2DFC
        add    esp, 8
        jmp    loc_1_0FE0
loc_1_0FC4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x14c], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x160], 0
loc_1_0FE0:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_0FE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 0x94]
        add    ecx, 0x402
        mov    eax, dword ptr [eax + 0x14c]
        cmp    eax, 1
        je     loc_1_1018
        cmp    eax, 2
        je     loc_1_113D
        jmp    loc_1_11CC
loc_1_1018:
        push   0xa
        push   ecx
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_1044
        push   dword ptr [ebp - 4]
        push   0xa
        push   0xfe6                         ; -> loc_1_0FE6
        call   sub_6_0040
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_11CC
loc_1_1044:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x158], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x160], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x134]
        cmp    word ptr [eax + 0xa], 0x800
        jne    loc_1_10D7
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x138]
        mov    cx, word ptr [eax + 0xe]
        cmp    word ptr [eax + 0xc], cx
        jne    loc_1_109A
        push   eax
        call   sub_1_38F3
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x134]
        mov    word ptr [eax + 0xa], 0
loc_1_109A:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_10D7
        mov    eax, dword ptr [eax + 0x138]
        mov    cx, word ptr [eax + 0xe]
        cmp    word ptr [eax + 0xc], cx
        jae    loc_1_112E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xb2]
        sub    al, 0x10
        cmp    al, 1
        sbb    al, al
        and    al, 0x20
        add    al, 0x54
        push   eax
        push   dword ptr [ebp - 4]
        call   sub_1_2950
        add    esp, 8
        jmp    loc_1_11CC
loc_1_10D7:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 0xec]
        cmp    dword ptr [ecx + 0x58], eax
        jge    loc_1_1121
        push   ecx
        call   sub_1_196F
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x150], 2
        je     loc_1_1107
        push   2
        push   eax
        call   sub_1_1B99
        add    esp, 8
loc_1_1107:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x160], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14c], 0
loc_1_1121:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_11CC
loc_1_112E:
        push   edx
        mov    edx, dword ptr [ebp - 4]
        call   sub_1_2A16
        pop    edx
        jmp    loc_1_11CC
loc_1_113D:
        call   sub_1_39D1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x160], 0
        push   dword ptr [ebp - 4]
        call   sub_1_17B0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x158], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x134]
        mov    word ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14c], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 0xf0]
        cmp    dword ptr [ecx + 0x54], eax
        jl     loc_1_11B4
        mov    eax, ecx
        test   byte ptr [eax + 0x150], 1
        je     loc_1_11CC
        push   1
        push   ecx
        call   sub_1_1B99
        add    esp, 8
        jmp    loc_1_11CC
loc_1_11B4:
        push   dword ptr [ebp - 4]
        push   0xa
        push   0x2c2a                        ; -> loc_1_2C2A
        call   sub_6_0040
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x140], eax
loc_1_11CC:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_11D0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        test   byte ptr [esi + 0x16e], 1
        jne    loc_1_1247
        mov    bl, byte ptr [esp + 0x14]
        test   bl, 0x10
        jne    loc_1_1247
        movzx  eax, word ptr [esi + 0x94]
        lea    edi, [eax + 0x402]
        inc    eax
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    loc_1_1247
        mov    al, bl
        or     al, 0x14
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    al, byte ptr [esi + 0x16e]
        or     al, 1
        mov    byte ptr [esi + 0x16e], al
        push   0x200
        push   esi
        call   sub_1_1C08
        add    esp, 8
        test   bl, 4
        jne    loc_1_1240
        and    bl, 0xfb
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_1240:
        mov    eax, 1
        jmp    loc_1_1249
loc_1_1247:
        sub    eax, eax
loc_1_1249:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_124D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        sti    
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1267:
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], edx
        mov    eax, dword ptr [esp + 0xc]
        movzx  esi, word ptr [eax + 0x94]
        add    esi, 0x402
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [esp + 0xb], al
        mov    bl, al
        and    bl, 0xe0
        cmp    bl, 0x60
        je     loc_1_12B3
        cmp    bl, 0x40
        je     loc_1_12B3
        push   dword ptr [esp + 0x10]
        call   sub_1_124D
        add    esp, 4
        stc    
        jmp    loc_1_1341
loc_1_12B3:
        mov    eax, dword ptr [esp + 0xc]
        test   byte ptr [eax + 0x16e], 0x80
        je     loc_1_12D0
        push   dword ptr [esp + 0xb]
        push   dword ptr [esp + 0x10]
        call   sub_1_11D0
        add    esp, 8
loc_1_12D0:
        test   byte ptr [esp + 0xb], 4
        jne    loc_1_12E6
        push   dword ptr [esp + 0x10]
        call   sub_1_124D
        add    esp, 4
        stc    
        jmp    loc_1_1341
loc_1_12E6:
        sub    bl, 0x60
        cmp    bl, 1
        sbb    al, al
        and    al, 0x20
        add    al, 0x54
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0x10]
        call   sub_1_124D
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 4
        cmp    dword ptr [eax + 0x164], 0
        je     loc_1_1340
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0xf8], ecx
        jne    loc_1_1340
        cmp    dword ptr [eax + 0x158], 0
        je     loc_1_1332
        push   eax
        push   0xfe6                         ; -> loc_1_0FE6
        jmp    loc_1_133B
loc_1_1332:
        push   dword ptr [esp + 0xc]
        push   0xeb1                         ; -> loc_1_0EB1
loc_1_133B:
        call   sub_6_0028
loc_1_1340:
        clc    
loc_1_1341:
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 018h, 00Fh, 0B7h, 0BEh, 094h, 000h
        db  000h, 000h, 08Dh, 06Fh, 002h, 047h, 057h, 0E8h, 02Dh, 02Bh, 000h, 000h, 083h, 0C4h, 004h, 0A8h
        db  080h, 075h, 005h, 0E9h, 0A2h, 000h, 000h, 000h, 055h, 0E8h, 01Bh, 02Bh, 000h, 000h, 083h, 0C4h
        db  004h, 08Ah, 0D8h, 00Ch, 020h, 050h, 055h, 0E8h, 0ABh, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 080h
        db  0E3h, 0EBh, 053h, 055h, 0E8h, 09Eh, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 023h, 06Ah, 000h
        db  06Ah, 040h, 057h, 0E8h, 099h, 02Ah, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 05Ah, 08Ah
        db  0C3h, 024h, 0FEh, 088h, 044h, 024h, 010h, 0FFh, 074h, 024h, 010h, 055h, 0E8h, 076h, 02Bh, 000h
        db  000h, 083h, 0C4h, 008h, 080h, 0CBh, 0FBh, 0FFh, 074h, 024h, 010h, 055h, 0E8h, 066h, 02Bh, 000h
        db  000h, 083h, 0C4h, 008h, 053h, 055h, 0E8h, 05Ch, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 053h, 055h
        db  0E8h, 052h, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 023h, 06Ah, 040h, 06Ah, 040h, 057h, 0E8h
        db  04Dh, 02Ah, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 00Eh, 0C6h, 086h, 0B3h, 000h, 000h
        db  000h, 023h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 013h, 0C7h, 046h, 034h, 000h, 004h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh
        db  083h, 0C4h, 004h, 0C3h
sub_1_141B:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x10], 1
        mov    dword ptr [esp + 0x14], 3
        mov    dword ptr [esp + 0x18], 5
        mov    dword ptr [esp + 0x1c], 7
        mov    dword ptr [esp + 0x20], 0xc2
        mov    dword ptr [esp + 0x24], 0xc6
        mov    dword ptr [esp + 0x28], 0xca
        mov    si, word ptr [esp + 0x34]
        mov    dword ptr [esp + 0x2c], 0xce
        movzx  eax, si
        mov    edi, dword ptr [esp + eax*4 + 0x10]
        push   edi
        call   sub_1_3E90
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
loc_1_147F:
        mov    bl, al
        push   edi
        call   sub_1_3E90
        mov    byte ptr [esp + 0x13], al
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
        cmp    bl, al
        jne    loc_1_147F
        movzx  ax, al
        movzx  cx, byte ptr [esp + 0xf]
        shl    ax, 8
        or     ax, cx
        inc    ax
        cmp    si, 3
        jbe    loc_1_14B7
        add    ax, ax
loc_1_14B7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
sub_1_14BE:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        movzx  edi, word ptr [esi + 0x94]
        add    edi, 0x402
        push   dword ptr [esi + 0x130]
        call   sub_1_38B6
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xf7
        or     al, 0x14
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    ax, word ptr [esi + 0x126]
        push   eax
        call   sub_1_141B
        add    esp, 4
        movzx  ecx, ax
        mov    eax, dword ptr [esi + 0x14c]
        cmp    eax, 1
        je     loc_1_151C
        cmp    eax, 2
        je     loc_1_1536
        jmp    loc_1_1587
loc_1_151C:
        mov    eax, dword ptr [esi + 0x58]
        add    eax, ecx
        mov    dword ptr [esi + 0x58], eax
        sub    dword ptr [esi + 0x5c], ecx
        or     eax, eax
        jne    loc_1_1587
        push   esi
        call   sub_1_19E0
        add    esp, 4
        jmp    loc_1_1587
loc_1_1536:
        movzx  ebx, word ptr [esi + 0x94]
        mov    eax, dword ptr [esi + 0x134]
        push   ecx
        push   dword ptr [eax + 4]
        add    ebx, 2
        push   1
        push   esi
        call   sub_1_1870
        add    esp, 0x10
        push   esi
        call   sub_1_0DF0
        add    esp, 4
        push   esi
        call   sub_1_07BB
        add    esp, 4
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xdf
        push   eax
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   0x74
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_1587:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_158B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_14BE
        add    esp, 4
        mov    dword ptr [esi + 0x160], 0
        mov    dword ptr [esi + 0x158], 0
        mov    eax, dword ptr [esi + 0x14c]
        cmp    eax, 1
        je     loc_1_15C2
        cmp    eax, 2
        je     loc_1_161F
        jmp    loc_1_1646
loc_1_15C2:
        mov    eax, dword ptr [esi + 0x134]
        cmp    word ptr [eax + 0xa], 0x800
        jne    loc_1_15EA
        push   dword ptr [esi + 0x138]
        call   sub_1_38F3
        add    esp, 4
        mov    eax, dword ptr [esi + 0x134]
        mov    word ptr [eax + 0xa], 0
loc_1_15EA:
        mov    eax, dword ptr [esi + 0x13c]
        or     eax, eax
        je     loc_1_1604
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x13c], 0
loc_1_1604:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_1646
        push   esi
        push   0xa
        push   0x2a16                        ; -> sub_1_2A16
        call   sub_6_0040
        mov    dword ptr [esi + 0x13c], eax
        jmp    loc_1_1646
loc_1_161F:
        cmp    dword ptr [esi + 0x140], 0
        jne    loc_1_1646
        mov    eax, dword ptr [esi + 0x54]
        cmp    dword ptr [esi + 0xf0], eax
        jle    loc_1_1646
        push   esi
        push   0xa
        push   0x2c2a                        ; -> loc_1_2C2A
        call   sub_6_0040
        mov    dword ptr [esi + 0x140], eax
loc_1_1646:
        pop    esi
        ret    
loc_1_1648:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_165C
        push   esi
        call   sub_1_19E0
        add    esp, 4
loc_1_165C:
        mov    dword ptr [esi + 0x160], 0
        mov    eax, dword ptr [esi + 0x13c]
        or     eax, eax
        je     loc_1_1680
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x13c], 0
loc_1_1680:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_1699
        push   esi
        push   0xa
        push   0x2faf                        ; -> sub_1_2FAF
        call   sub_6_0040
        mov    dword ptr [esi + 0x13c], eax
loc_1_1699:
        pop    esi
        ret    
loc_1_169B:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        movzx  edi, word ptr [ebx + 0x94]
        lea    esi, [edi + 2]
        add    edi, 0x402
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 0x14
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        call   sub_1_0DF0
        add    esp, 4
        push   ebx
        call   sub_1_07BB
        add    esp, 4
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xdf
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   0x74
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [ebx + 0x160], 0
        cmp    dword ptr [ebx + 0x140], 0
        jne    loc_1_172A
        mov    eax, dword ptr [ebx + 0x54]
        cmp    dword ptr [ebx + 0xf0], eax
        jle    loc_1_172A
        push   ebx
        push   0xa
        push   0x365a                        ; -> loc_1_365A
        call   sub_6_0040
        mov    dword ptr [ebx + 0x140], eax
loc_1_172A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_172E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x94]
        inc    eax
        cmp    byte ptr [ecx + 0xb2], 0
        jne    loc_1_176B
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    loc_1_176B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x140], 0
        jne    loc_1_176B
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_0BB0
        pop    edx
loc_1_176B:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 0x160], 0
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1780:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x134]
        mov    eax, dword ptr [esi + 0x148]
        mov    dword ptr [edi], eax
        push   eax
        push   dword ptr [edi + 4]
        push   2
        push   esi
        call   sub_1_17CB
        add    esp, 0x10
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x134]
        pop    edi
        mov    eax, dword ptr [eax]
        pop    esi
        ret    
sub_1_17B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x134]
        push   dword ptr [ecx]
        push   dword ptr [ecx + 4]
        push   1
        push   eax
        call   sub_1_1870
        add    esp, 0x10
        ret    
sub_1_17CB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 1
        je     loc_1_17E4
        cmp    eax, 2
        je     loc_1_182F
        jmp    loc_1_1869
loc_1_17E4:
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0x50]
        mov    ecx, dword ptr [esi + 0x3c]
        add    ecx, edi
        mov    ebp, dword ptr [esi + 0x4c]
        mov    ebx, ebp
        or     ebx, ebx
        jne    loc_1_180A
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 2
        sub    eax, eax
        jmp    loc_1_186B
loc_1_180A:
        mov    eax, dword ptr [esp + 0x20]
        cmp    ebx, eax
        jbe    loc_1_1814
        mov    ebx, eax
loc_1_1814:
        push   ebx
        push   ecx
        sub    ebp, ebx
        push   dword ptr [esp + 0x24]
        call   sub_1_39DA
        add    esp, 0xc
        lea    eax, [edi + ebx]
        mov    dword ptr [esi + 0x50], eax
        mov    dword ptr [esi + 0x4c], ebp
        jmp    loc_1_1869
loc_1_182F:
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0x5c]
        mov    eax, dword ptr [esi + 0xf4]
        mov    ebp, dword ptr [esi + 0x58]
        mov    ebx, ebp
        mov    ecx, dword ptr [eax + 4]
        add    ecx, edi
        mov    eax, dword ptr [esp + 0x20]
        cmp    ebx, eax
        jbe    loc_1_1850
        mov    ebx, eax
loc_1_1850:
        push   ebx
        push   ecx
        sub    ebp, ebx
        push   dword ptr [esp + 0x24]
        call   sub_1_39DA
        add    esp, 0xc
        lea    eax, [edi + ebx]
        mov    dword ptr [esi + 0x5c], eax
        mov    dword ptr [esi + 0x58], ebp
loc_1_1869:
        mov    eax, ebx
loc_1_186B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1870:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 1
        je     loc_1_1889
        cmp    eax, 2
        je     loc_1_18D8
        jmp    loc_1_1968
loc_1_1889:
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [esi + 0x54]
        mov    edi, dword ptr [esi + 0x40]
        sub    edi, ebx
        or     edi, edi
        jg     loc_1_18AD
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 1
        sub    eax, eax
        jmp    loc_1_196A
loc_1_18AD:
        mov    ecx, dword ptr [esi + 0x3c]
        add    ecx, ebx
        mov    ebp, dword ptr [esp + 0x20]
        cmp    edi, ebp
        jb     loc_1_18BC
        mov    edi, ebp
loc_1_18BC:
        push   edi
        push   dword ptr [esp + 0x20]
        push   ecx
        call   sub_1_39DA
        add    esp, 0xc
        lea    eax, [ebx + edi]
        mov    dword ptr [esi + 0x54], eax
        add    dword ptr [esi + 0x4c], edi
        jmp    loc_1_1968
loc_1_18D8:
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_18EE
        push   esi
        call   sub_1_199C
        add    esp, 4
loc_1_18EE:
        mov    ebx, dword ptr [esi + 0xf4]
        cmp    dword ptr [ebx + 4], 0
        jne    loc_1_191F
        mov    ebp, dword ptr [esp + 0x20]
        push   1
        push   ebp
        call   sub_6_0018
        add    esp, 8
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], ebp
        mov    dword ptr [esi + 0x58], 0
        mov    dword ptr [esi + 0x60], 0
        jmp    loc_1_1923
loc_1_191F:
        mov    ebp, dword ptr [esp + 0x20]
loc_1_1923:
        mov    edi, dword ptr [ebx + 8]
        sub    edi, dword ptr [esi + 0x58]
        or     edi, edi
        jg     loc_1_193E
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x100
        sub    eax, eax
        jmp    loc_1_196A
loc_1_193E:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, dword ptr [esi + 0x60]
        cmp    edi, ebp
        jb     loc_1_194A
        mov    edi, ebp
loc_1_194A:
        push   edi
        push   dword ptr [esp + 0x20]
        push   ecx
        call   sub_1_39DA
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x60]
        add    eax, edi
        mov    dword ptr [esi + 0x60], eax
        mov    eax, dword ptr [esi + 0x58]
        add    eax, edi
        mov    dword ptr [esi + 0x58], eax
loc_1_1968:
        mov    eax, edi
loc_1_196A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_196F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_199A
        push   esi
        call   sub_1_19E0
        add    esp, 4
        mov    eax, dword ptr [esi + 0xf4]
        or     eax, eax
        je     loc_1_199A
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x58], eax
        mov    dword ptr [esi + 0x5c], 0
loc_1_199A:
        pop    esi
        ret    
sub_1_199C:
        push   1
        push   0xc
        call   sub_6_0018
        add    esp, 8
        or     eax, eax
        jne    loc_1_19B0
        sub    eax, eax
        jmp    loc_1_19DF
loc_1_19B0:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [edx + 0xf4]
        or     ecx, ecx
        jne    loc_1_19CC
        mov    dword ptr [edx + 0xf4], eax
        jmp    loc_1_19DA
loc_1_19CC:
        cmp    dword ptr [ecx], 0
        je     loc_1_19D8
loc_1_19D1:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    loc_1_19D1
loc_1_19D8:
        mov    dword ptr [ecx], eax
loc_1_19DA:
        mov    eax, 1
loc_1_19DF:
        ret    
sub_1_19E0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0xf4]
        or     esi, esi
        je     loc_1_1A1E
        cmp    byte ptr [edi + 0x16c], 0
        jne    loc_1_1A0B
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_1A0B
        push   0
        push   eax
        call   sub_6_0020
        add    esp, 8
loc_1_1A0B:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0xf4], eax
        push   0
        push   esi
        call   sub_6_0020
        add    esp, 8
loc_1_1A1E:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1A30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x134]
        cmp    word ptr [eax + 0xa], 0x800
        jne    loc_1_1A4E
        mov    eax, 1
        jmp    loc_1_1AF1
loc_1_1A4E:
        mov    ecx, dword ptr [esi + 0x138]
        mov    eax, dword ptr [esi + 0x58]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0xf4]
        push   ecx
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ecx + 8], 0
        mov    word ptr [ecx + 0xc], 0x11
        mov    word ptr [ecx + 0xe], 0
        call   sub_1_38C1
        add    esp, 4
        or     eax, eax
        je     loc_1_1AEF
        mov    eax, dword ptr [esi + 0x138]
        sub    edi, edi
        cmp    word ptr [eax + 0xe], di
        jbe    loc_1_1AC2
loc_1_1A95:
        mov    ax, word ptr [esi + 0x120]
        push   eax
        push   dword ptr [esi + 0x138]
        call   sub_1_3916
        add    esp, 8
        or     eax, eax
        je     loc_1_1AE1
        mov    eax, dword ptr [esi + 0x138]
        inc    edi
        inc    word ptr [eax + 0xc]
        movzx  eax, word ptr [eax + 0xe]
        cmp    eax, edi
        jg     loc_1_1A95
loc_1_1AC2:
        mov    eax, dword ptr [esi + 0x138]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [esi + 0x134]
        mov    word ptr [eax + 0xa], 0x800
        mov    eax, 1
        jmp    loc_1_1AF1
loc_1_1AE1:
        push   dword ptr [esi + 0x138]
        call   sub_1_38F3
        add    esp, 4
loc_1_1AEF:
        sub    eax, eax
loc_1_1AF1:
        pop    edi
        pop    esi
        ret    
sub_1_1AF4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x134]
        push   eax
        mov    dword ptr [eax], 0x1000
        mov    word ptr [eax + 0xa], 0
        push   dword ptr [esi + 0x130]
        call   sub_1_3774
        add    esp, 8
        mov    eax, dword ptr [esi + 0x134]
        cmp    word ptr [eax + 0xa], 0
        je     loc_1_1B34
        lea    edi, [esi + 0x148]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        jmp    loc_1_1B6D
loc_1_1B34:
        mov    dword ptr [eax], 0x1000
        push   0xf
        push   1
        push   eax
        call   sub_1_37D2
        add    esp, 0xc
        mov    dword ptr [esi + 0x144], eax
        mov    eax, dword ptr [esi + 0x134]
        lea    edi, [esi + 0x148]
        cmp    word ptr [eax + 0xa], 0x8000
        jne    loc_1_1B67
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        jmp    loc_1_1B6D
loc_1_1B67:
        mov    dword ptr [edi], 0
loc_1_1B6D:
        mov    eax, dword ptr [esi + 0x134]
        push   eax
        mov    cx, word ptr [eax + 0xa]
        mov    word ptr [esi + 0x15c], cx
        call   sub_1_384A
        add    esp, 4
        or     eax, eax
        jne    loc_1_1B91
        mov    dword ptr [edi], 0
loc_1_1B91:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_1_1B99:
        push   esi
        push   edi
        movzx  esi, byte ptr [esp + 0x10]
        cmp    esi, 1
        je     loc_1_1BB1
        cmp    esi, 2
        je     loc_1_1BC3
        cmp    esi, 4
        je     loc_1_1BD5
        jmp    loc_1_1C05
loc_1_1BB1:
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0xdc]
        mov    ecx, dword ptr [edi + 0xcc]
        jmp    loc_1_1BE5
loc_1_1BC3:
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0xd8]
        mov    ecx, dword ptr [edi + 0xc8]
        jmp    loc_1_1BE5
loc_1_1BD5:
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0xd4]
        mov    ecx, dword ptr [edi + 0xc4]
loc_1_1BE5:
        or     ecx, ecx
        je     loc_1_1BF3
        push   0
        push   esi
        push   eax
        push   edi
        call   ecx
        add    esp, 0x10
loc_1_1BF3:
        not    esi
        and    dword ptr [edi + 0x150], esi
        mov    dword ptr [edi + 0x160], 0
loc_1_1C05:
        pop    edi
        pop    esi
        ret    
sub_1_1C08:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x150], 0x40
        je     loc_1_1C3F
        mov    ecx, dword ptr [esi + 0x30]
        mov    eax, dword ptr [esp + 0xc]
        or     dword ptr [ecx], eax
        test   dword ptr [esi + 0xe0], eax
        je     loc_1_1C32
        push   4
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_1C32:
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0
loc_1_1C3F:
        pop    esi
        ret    
sub_1_1C41:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_199C
        add    esp, 4
        mov    ebp, dword ptr [esi + 0xf4]
        mov    ebx, ebp
        cmp    dword ptr [ebx], 0
        je     loc_1_1C66
loc_1_1C5F:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx], 0
        jne    loc_1_1C5F
loc_1_1C66:
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 8], edi
        cmp    byte ptr [esi + 0x16c], 0
        jne    loc_1_1C94
        push   1
        push   edi
        call   sub_6_0018
        add    esp, 8
        push   edi
        mov    dword ptr [ebx + 4], eax
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_39DA
        add    esp, 0xc
        jmp    loc_1_1C9B
loc_1_1C94:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 4], eax
loc_1_1C9B:
        cmp    dword ptr [ebp], 0
        jne    loc_1_1CAB
        mov    dword ptr [esi + 0x5c], 0
        mov    dword ptr [esi + 0x58], edi
loc_1_1CAB:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1CB2:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_1C41
        mov    ecx, dword ptr [ebp + 0x14]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x13c], 0
        jne    loc_1_1D20
        cmp    dword ptr [eax + 0x158], 0
        jne    loc_1_1D16
        cmp    dword ptr [eax + 0x148], 0
        je     loc_1_1D16
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0x154]
        cmp    dword ptr [ecx + 0x58], eax
        jbe    loc_1_1D16
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x130], 0
        je     loc_1_1D16
        cmp    dword ptr [eax + 0xf8], 0
        je     loc_1_1D16
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_2A16
        jmp    loc_1_1D1F
loc_1_1D16:
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_2FAF
loc_1_1D1F:
        pop    edx
loc_1_1D20:
        pop    ebp
        ret    
loc_1_1D22:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 0x10]
        mov    byte ptr [ecx + 0xb2], 0x10
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_1CB2
        add    esp, 0x10
        mov    eax, 1
        ret    
loc_1_1D48:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 0x10]
        mov    byte ptr [ecx + 0xb2], 0x40
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_1CB2
        add    esp, 0x10
        mov    eax, 1
        ret    
loc_1_1D6E:
        push   esi
        push   dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_1C41
        mov    ecx, dword ptr [esp + 0x20]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 0x13c], 0
        jne    loc_1_1DE5
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_1DA6
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        je     loc_1_1DCC
loc_1_1DA6:
        or     dword ptr [esi + 0x168], 0x80
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_209D
        add    esp, 4
        xor    dword ptr [esi + 0x168], 0x80
loc_1_1DCC:
        cmp    dword ptr [esi + 0x58], 0
        jle    loc_1_1DE5
        push   esi
        push   0xa
        push   0x2123                        ; -> loc_1_2123
        call   sub_6_0040
        mov    dword ptr [esi + 0x13c], eax
loc_1_1DE5:
        mov    eax, 1
        pop    esi
        ret    
loc_1_1DEC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x148], 0
        je     loc_1_1E21
        mov    eax, dword ptr [esi + 0x154]
        cmp    dword ptr [esi + 0x40], eax
        jbe    loc_1_1E21
        cmp    dword ptr [esi + 0x130], 0
        je     loc_1_1E21
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_1_1E21
        push   esi
        push   0xa
        push   0x2c2a                        ; -> loc_1_2C2A
        jmp    loc_1_1E29
loc_1_1E21:
        push   esi
        push   0xa
        push   0x365a                        ; -> loc_1_365A
loc_1_1E29:
        call   sub_6_0040
        mov    dword ptr [esi + 0x140], eax
        pop    esi
        ret    
loc_1_1E36:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x140], 0
        jne    loc_1_1E57
        push   esi
        push   0xa
        push   0xbb0                         ; -> sub_1_0BB0
        call   sub_6_0040
        mov    dword ptr [esi + 0x140], eax
loc_1_1E57:
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1E60:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0x30
        jne    loc_1_1E6F
        mov    eax, 0x800
        jmp    loc_1_1EA4
loc_1_1E6F:
        test   al, 0x20
        je     loc_1_1E7A
        mov    eax, 0x1000
        jmp    loc_1_1EA4
loc_1_1E7A:
        test   al, 8
        je     loc_1_1E85
        mov    eax, 0x400
        jmp    loc_1_1EA4
loc_1_1E85:
        movzx  ecx, al
        not    ecx
        test   cl, 0x10
        je     loc_1_1E96
        mov    eax, 0x800
        jmp    loc_1_1EA4
loc_1_1E96:
        test   al, 0x80
        mov    eax, 0
        jne    loc_1_1EA4
        mov    eax, 0x200
loc_1_1EA4:
        ret    
sub_1_1EA5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        test   byte ptr [esi + 0x17c], 1
        je     loc_1_1EBE
        call   sub_1_3A3C
        jmp    loc_1_1EC3
loc_1_1EBE:
        call   sub_1_3A12
loc_1_1EC3:
        add    esp, 4
        or     al, al
        mov    eax, 1
        je     loc_1_1EF8
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        movzx  eax, word ptr [esi + 0x94]
        inc    eax
        push   eax
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xf8
        xor    al, 0x48
        push   eax
        call   sub_1_1E60
        add    esp, 4
        mov    dword ptr [esi + 0x34], eax
        sub    eax, eax
loc_1_1EF8:
        pop    esi
        ret    
sub_1_1EFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0x94]
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        lea    edi, [edi + 1]
        mov    byte ptr [esi + 0x16f], 0
        pushfd 
        cli    
        mov    dword ptr [ebp - 4], 0xc8
loc_1_1F25:
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     loc_1_1FB7
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_1_39EF
        add    esp, 8
        cmp    byte ptr [esi + 0x16f], 0
        je     loc_1_1F67
        mov    bl, byte ptr [esi + 0x16f]
loc_1_1F5A:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        dec    bl
        jne    loc_1_1F5A
loc_1_1F67:
        sub    bl, bl
loc_1_1F69:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        test   al, 0x80
        je     loc_1_1F7D
        inc    bl
        cmp    bl, 0x14
        jb     loc_1_1F69
loc_1_1F7D:
        movzx  eax, bl
        or     eax, eax
        je     loc_1_1FA9
        cmp    eax, 0x14
        je     loc_1_1FA9
        mov    al, byte ptr [esi + 0x16f]
        add    al, bl
        mov    byte ptr [esi + 0x16f], al
        cmp    al, 0x14
        jbe    loc_1_1FA9
        mov    byte ptr [esi + 0x16f], 0x14
        mov    dword ptr [ebp - 4], 0
loc_1_1FA9:
        mov    eax, dword ptr [esi + 0x58]
        dec    eax
        mov    dword ptr [esi + 0x58], eax
        inc    dword ptr [esi + 0x5c]
        or     eax, eax
        je     loc_1_1FD1
loc_1_1FB7:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0
        jg     loc_1_1F25
        popfd  
        mov    eax, 1
loc_1_1FCA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1FD1:
        popfd  
        mov    eax, 1
        jmp    loc_1_1FCA
sub_1_1FD9:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        movzx  eax, word ptr [esi + 0x94]
        mov    edi, eax
        mov    dword ptr [esp + 0x14], eax
        inc    edi
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     loc_1_2090
        cmp    byte ptr [esi + 0x16f], 0xff
        jne    loc_1_201C
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_1EFA
        add    esp, 8
        jmp    loc_1_2095
loc_1_201C:
        mov    dword ptr [esp + 0x10], 0x800
loc_1_2024:
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     loc_1_2085
        mov    eax, dword ptr [esp + 0x20]
        inc    dword ptr [esp + 0x20]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_39EF
        add    esp, 8
        mov    ebp, dword ptr [esi + 0x58]
        dec    ebp
        mov    dword ptr [esi + 0x58], ebp
        inc    dword ptr [esi + 0x5c]
        cmp    byte ptr [esi + 0x16f], 0
        je     loc_1_206F
        movzx  ebx, byte ptr [esi + 0x16f]
loc_1_2063:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        dec    ebx
        jne    loc_1_2063
loc_1_206F:
        or     ebp, ebp
        je     loc_1_2090
        movzx  eax, byte ptr [esi + 0x16f]
        movzx  eax, byte ptr [eax + 0x3fe0]  ; -> obj1:0x3FE0
        sub    dword ptr [esp + 0x10], eax
loc_1_2085:
        dec    dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x10], 0
        jg     loc_1_2024
loc_1_2090:
        mov    eax, 1
loc_1_2095:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_209D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_20B1
        mov    dword ptr [esi + 0x34], 0
        jmp    loc_1_2121
loc_1_20B1:
        mov    eax, dword ptr [esi + 0xf4]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [esi + 0x160], 1
        mov    dword ptr [esi + 0xbc], 0x172e ; -> loc_1_172E
        mov    dword ptr [esi + 0x14c], 0x20
        push   eax
        push   esi
        call   sub_1_1FD9
        add    esp, 8
        mov    eax, dword ptr [esi + 0x58]
        cmp    dword ptr [esi + 0xec], eax
        jg     loc_1_20F4
        or     eax, eax
        jne    loc_1_2111
loc_1_20F4:
        push   esi
        call   sub_1_196F
        add    esp, 4
        test   byte ptr [esi + 0x150], 2
        je     loc_1_2111
        push   2
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_2111:
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_2121
        mov    dword ptr [esi + 0x160], 0
loc_1_2121:
        pop    esi
        ret    
loc_1_2123:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 0x3b], 1
        je     loc_1_2141
        push   eax
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        je     loc_1_2175
loc_1_2141:
        mov    eax, dword ptr [esp]
        or     dword ptr [eax + 0x168], 0x80
        push   dword ptr [esp]
        call   sub_1_01B8
        add    esp, 4
        push   dword ptr [esp]
        call   sub_1_209D
        mov    eax, dword ptr [esp + 4]
        add    esp, 4
        xor    dword ptr [eax + 0x168], 0x80
loc_1_2175:
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0x58], 0
        jle    loc_1_2198
        push   eax
        push   0xa
        push   0x2123                        ; -> loc_1_2123
        call   sub_6_0040
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_21A6
loc_1_2198:
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x13c], 0
loc_1_21A6:
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_21B0:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, 1
        jne    loc_1_21C6
        mov    esi, dword ptr [esp + 8]
        and    byte ptr [esi + 0x3b], 0xfe
loc_1_21C2:
        sub    eax, eax
loc_1_21C4:
        pop    esi
        ret    
loc_1_21C6:
        or     eax, eax
        jne    loc_1_21C2
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x168], 1
        jne    loc_1_21C2
        cmp    dword ptr [esi + 0x160], 0
        jne    loc_1_21C2
        cmp    byte ptr [esi + 0xb0], 0xff
        jne    loc_1_21C2
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_1_21FB
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_1_21FB:
        or     byte ptr [esi + 0x3b], 1
        mov    eax, 1
        jmp    loc_1_21C4
sub_1_2206:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jae    loc_1_2217
        xor    eax, eax
loc_1_2217:
        pop    edi
        pop    ebp
        ret    
loc_1_221A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_1_223A
        cmp    eax, 1
        je     loc_1_2274
        mov    dword ptr [esi + 0xc], 0xffffffeb
        sub    eax, eax
        jmp    loc_1_22C1
loc_1_223A:
        mov    dword ptr [esi + 0x58], 0
        mov    dword ptr [esi + 0x5c], 0
        mov    dword ptr [esi + 0x60], 0
        mov    eax, dword ptr [esi + 0x13c]
        or     eax, eax
        je     loc_1_2269
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x13c], 0
loc_1_2269:
        push   esi
        call   sub_1_19E0
        add    esp, 4
        jmp    loc_1_22A3
loc_1_2274:
        mov    dword ptr [esi + 0x4c], 0
        mov    dword ptr [esi + 0x50], 0
        mov    dword ptr [esi + 0x54], 0
        mov    eax, dword ptr [esi + 0x140]
        or     eax, eax
        je     loc_1_22A3
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x140], 0
loc_1_22A3:
        cmp    dword ptr [esi + 0x160], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x200
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
loc_1_22C1:
        pop    esi
        ret    
sub_1_22C3:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_1_22F7
        mov    dword ptr [esi], 0
        cmp    dword ptr [edi + 0x160], 0
        je     loc_1_22EB
        sti    
        push   edi
        call   dword ptr [edi + 0xbc]
        add    esp, 4
loc_1_22EB:
        mov    eax, dword ptr [edi + 0x4c]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x58]
        mov    dword ptr [esi + 8], eax
loc_1_22F7:
        mov    dword ptr [edi + 0xc], 0
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_1_2306:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [esp + 0xe], 0
        mov    esi, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], 0
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_2341
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        jne    loc_1_2341
        mov    dword ptr [esi + 0xe8], 0
        jmp    loc_1_2485
loc_1_2341:
        or     dword ptr [esi + 0x168], 0x80
        mov    bl, byte ptr [esi + 0xb2]
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   4
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_1_237B
        mov    dword ptr [esi + 0xe8], 0
        jmp    loc_1_247B
loc_1_237B:
        mov    di, word ptr [esi + 0x94]
        push   0x23
        push   0
        push   8
        movzx  eax, di
        inc    eax
        push   eax
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_23B1
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0xe8], 0
        jmp    loc_1_247B
loc_1_23B1:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x12]
        push   2
        push   eax
        push   edi
        call   sub_1_3CB3
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_23E2
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0xe8], 0
        jmp    loc_1_247B
loc_1_23E2:
        mov    ah, byte ptr [esp + 0xe]
        mov    al, byte ptr [esp + 0xf]
        mov    word ptr [esp + 0xe], ax
        cmp    ax, 2
        ja     loc_1_240A
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0xe8], 0
        jmp    loc_1_247B
loc_1_240A:
        sub    word ptr [esp + 0xe], 2
        mov    eax, dword ptr [esi + 0xe8]
        movzx  ecx, word ptr [esp + 0xe]
        cmp    ecx, eax
        jbe    loc_1_2424
        mov    word ptr [esp + 0xe], ax
loc_1_2424:
        lea    eax, [esp + 0x10]
        push   eax
        movzx  eax, word ptr [esp + 0x12]
        push   eax
        push   dword ptr [esi + 0xe4]
        mov    ax, word ptr [esi + 0x94]
        push   eax
        call   sub_1_3CB3
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0x10
        mov    dword ptr [esi + 0xe8], eax
        push   esi
        call   sub_1_01B8
        add    esp, 4
        movzx  eax, bl
        or     eax, eax
        je     loc_1_2466
        cmp    eax, 0x10
        je     loc_1_2470
        jmp    loc_1_247B
loc_1_2466:
        push   0
        push   esi
        call   sub_1_03B7
        jmp    loc_1_2478
loc_1_2470:
        push   0x10
        push   esi
        call   sub_1_0287
loc_1_2478:
        add    esp, 8
loc_1_247B:
        xor    dword ptr [esi + 0x168], 0x80
loc_1_2485:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_248C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, 7
        je     loc_1_24BB
        cmp    eax, 0x11
        je     loc_1_24F4
        cmp    eax, 0x12
        je     loc_1_2535
        mov    dword ptr [esi + 0xc], 0xffffffec
        mov    eax, 1
        jmp    loc_1_262D
loc_1_24BB:
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_24D3
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        jne    loc_1_24D3
        jmp    loc_1_262B
loc_1_24D3:
        or     dword ptr [esi + 0x168], 0x80
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_0AC7
        add    esp, 4
        jmp    loc_1_2601
loc_1_24F4:
        cmp    byte ptr [esi + 0xb1], 0xff
        mov    edi, dword ptr [esp + 0x1c]
        jne    loc_1_250C
        mov    dword ptr [edi], 0
        jmp    loc_1_260B
loc_1_250C:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0xe4], eax
        mov    ebx, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0xe8], ebx
        push   esi
        call   sub_1_2306
        add    esp, 4
        mov    eax, dword ptr [esi + 0xe8]
        mov    dword ptr [edi], eax
        jmp    loc_1_260B
loc_1_2535:
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_254D
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        jne    loc_1_254D
        jmp    loc_1_262B
loc_1_254D:
        mov    eax, dword ptr [esi + 0x168]
        or     eax, 0x80
        mov    dword ptr [esi + 0x168], eax
        mov    ebx, dword ptr [esp + 0x18]
        cmp    bl, 0x7f
        jbe    loc_1_2573
        mov    dword ptr [esi + 0xc], 0xffffffeb
        jmp    loc_1_2620
loc_1_2573:
        cmp    word ptr [esi + 0x98], 2
        jne    loc_1_2619
        mov    cl, byte ptr [esi + 0xb3]
        test   cl, 4
        je     loc_1_2599
        cmp    dword ptr [esi + 0x160], 1
        je     loc_1_2619
loc_1_2599:
        cmp    cl, 0x23
        je     loc_1_25BC
        push   0x10
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_1_25BC
        xor    dword ptr [esi + 0x168], 0x80
        jmp    loc_1_262B
loc_1_25BC:
        movzx  edi, word ptr [esi + 0x94]
        push   0x64
        lea    eax, [edi + 0x402]
        push   eax
        call   sub_1_051E
        add    esp, 8
        or     eax, eax
        je     loc_1_25F1
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x100
        xor    dword ptr [esi + 0x168], 0x80
        jmp    loc_1_262B
loc_1_25F1:
        or     ebx, 0x80
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2601:
        xor    dword ptr [esi + 0x168], 0x80
loc_1_260B:
        mov    dword ptr [esi + 0xc], 0
        mov    eax, 1
        jmp    loc_1_262D
loc_1_2619:
        mov    dword ptr [esi + 0xc], 0xfffffff6
loc_1_2620:
        xor    eax, 0x80
        mov    dword ptr [esi + 0x168], eax
loc_1_262B:
        sub    eax, eax
loc_1_262D:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2631:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_2726
        mov    edi, dword ptr [esp + 0x28]
        or     edi, edi
        je     loc_1_2726
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, 7
        je     loc_1_2673
        cmp    eax, 0x11
        je     loc_1_26B3
        cmp    eax, 0x12
        je     loc_1_26FB
        cmp    eax, 0x16
        je     loc_1_2718
        jmp    loc_1_2726
loc_1_2673:
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_268B
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        jne    loc_1_268B
        jmp    loc_1_2726
loc_1_268B:
        or     dword ptr [esi + 0x168], 0x80
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_0AC7
        add    esp, 4
        xor    dword ptr [esi + 0x168], 0x80
        jmp    loc_1_271F
loc_1_26B3:
        cmp    byte ptr [esi + 0xb1], 0xff
        jne    loc_1_26CE
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [eax], 0
        jmp    loc_1_271F
loc_1_26CE:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0xe4], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0xe8], eax
        push   esi
        call   sub_1_2306
        add    esp, 4
        mov    eax, dword ptr [esi + 0xe8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_271F
loc_1_26FB:
        mov    eax, dword ptr [esp + 0x14]
        push   0
        movsx  eax, byte ptr [eax]
        push   eax
        push   0x12
        push   esi
        call   sub_1_248C
        add    esp, 0x10
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        jmp    loc_1_2728
loc_1_2718:
        or     dword ptr [esi + 0x17c], 1
loc_1_271F:
        mov    eax, 0xffffffff
        jmp    loc_1_2728
loc_1_2726:
        sub    eax, eax
loc_1_2728:
        pop    edi
        pop    esi
        ret    
loc_1_272B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        push   esi
        call   sub_1_22C3
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     loc_1_276B
        push   esi
        call   sub_3_04FF
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x34]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x34], 0
        mov    eax, 1
        mov    dword ptr [esi + 0xc], 0
loc_1_276B:
        pop    esi
        ret    
loc_1_276D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 0x98]
        cmp    eax, 1
        je     loc_1_2791
        cmp    eax, 2
        je     loc_1_27F4
        mov    dword ptr [esi + 0xc], 0xffffffeb
        sub    eax, eax
        jmp    loc_1_281E
loc_1_2791:
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_27A8
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        jne    loc_1_27A8
        sub    eax, eax
        jmp    loc_1_281E
loc_1_27A8:
        or     dword ptr [esi + 0x168], 0x80
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_1_27C4
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_1_27C4:
        mov    dword ptr [esi + 0x58], 1
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        call   sub_1_1FD9
        add    esp, 8
        xor    dword ptr [esi + 0x168], 0x80
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_2810
        mov    dword ptr [esi + 0x58], 0
        sub    eax, eax
        jmp    loc_1_2810
loc_1_27F4:
        cmp    byte ptr [esi + 0xb1], 0x10
        push   dword ptr [esp + 0xc]
        jne    loc_1_2805
        push   0x74
        jmp    loc_1_2807
loc_1_2805:
        push   0x54
loc_1_2807:
        push   esi
        call   sub_1_30ED
        add    esp, 0xc
loc_1_2810:
        cmp    eax, 1
        sbb    ecx, ecx
        and    ecx, 0x4000
        mov    dword ptr [esi + 0xc], ecx
loc_1_281E:
        pop    esi
        ret    
loc_1_2820:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        mov    dword ptr [esi + 0xc], 0
        je     loc_1_2859
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        je     loc_1_2859
        mov    edi, dword ptr [esi + 0xb4]
        or     edi, edi
        je     loc_1_2855
        push   dword ptr [esp + 0x18]
        push   ecx
        push   eax
        push   esi
        call   edi
        add    esp, 0x10
        jmp    loc_1_285E
loc_1_2855:
        sub    eax, eax
        jmp    loc_1_285E
loc_1_2859:
        mov    eax, 1
loc_1_285E:
        pop    edi
        pop    esi
        ret    
loc_1_2861:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x160], 0
        je     loc_1_2882
        test   byte ptr [esi + 0x14c], 0x1a
        je     loc_1_2882
        push   esi
        call   dword ptr [esi + 0xbc]
        add    esp, 4
loc_1_2882:
        mov    eax, dword ptr [esi + 0x140]
        or     eax, eax
        je     loc_1_289C
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x140], 0
loc_1_289C:
        cmp    byte ptr [esi + 0x16d], 0
        je     loc_1_28AA
        mov    eax, dword ptr [esi + 0x4c]
        jmp    loc_1_28BD
loc_1_28AA:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   1
        push   esi
        call   sub_1_17CB
        add    esp, 0x10
loc_1_28BD:
        mov    ecx, dword ptr [esp + 0x14]
        pop    esi
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret    
sub_1_28CA:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, 1
        ret    
loc_1_28DB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_04FF
        add    esp, 4
        mov    eax, dword ptr [esi + 0x34]
        cmp    eax, 0x200
        je     loc_1_2914
        cmp    eax, 0x400
        je     loc_1_2920
        cmp    eax, 0x800
        je     loc_1_292C
        cmp    eax, 0x1000
        mov    eax, dword ptr [esp + 0xc]
        je     loc_1_2938
        mov    dword ptr [eax], 0
        jmp    loc_1_293E
loc_1_2914:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0x461
        jmp    loc_1_293E
loc_1_2920:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0x45d
        jmp    loc_1_293E
loc_1_292C:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0x15
        jmp    loc_1_293E
loc_1_2938:
        mov    dword ptr [eax], 0x1c
loc_1_293E:
        mov    eax, 1
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2950:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        movzx  edi, word ptr [esi + 0x94]
        mov    eax, dword ptr [esi + 0x134]
        add    edi, 0x402
        cmp    word ptr [eax + 0xa], 0x800
        jne    loc_1_299E
        lea    ebp, [esi + 0x130]
        push   dword ptr [esi + 0x138]
        push   dword ptr [ebp]
        call   sub_1_3968
        add    esp, 8
        mov    ebx, eax
        mov    eax, dword ptr [esi + 0x138]
        inc    word ptr [eax + 0xc]
        sub    dword ptr [esi + 0x58], ebx
        add    dword ptr [esi + 0x5c], ebx
        jmp    loc_1_29C2
loc_1_299E:
        push   esi
        call   sub_1_1780
        add    esp, 4
        mov    ebx, eax
        lea    ebp, [esi + 0x130]
        push   8
        push   dword ptr [esi + 0x134]
        push   dword ptr [ebp]
        call   sub_1_3865
        add    esp, 0xc
loc_1_29C2:
        mov    dword ptr [esi + 0x158], ebx
        mov    al, byte ptr [esi + 0x16e]
        test   al, 0x80
        mov    bl, byte ptr [esp + 0x18]
        je     loc_1_29E2
        cmp    bl, 0x74
        jne    loc_1_29E2
        test   al, 1
        jne    loc_1_29E2
        xor    bl, 0x10
loc_1_29E2:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        and    bl, 0xfb
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [esi + 0x160], 1
        push   dword ptr [ebp]
        call   sub_1_38A3
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2A16:
        sub    esp, 4
        push   ebx
        mov    dword ptr [esp + 4], edx
        cmp    dword ptr [esp + 4], 0
        je     loc_1_2B9F
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x13c], 0
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x160], 0
        je     loc_1_2A63
        cmp    byte ptr [eax + 0xb2], 0
        je     loc_1_2A7E
        test   byte ptr [eax + 0xb3], 4
        jne    loc_1_2A7E
        test   byte ptr [eax + 0x14c], 5
        jne    loc_1_2B9F
loc_1_2A63:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax + 0xb2]
        or     cl, cl
        jne    loc_1_2A9D
        push   eax
        call   sub_1_01B8
        add    esp, 4
        mov    bl, 0x54
        jmp    loc_1_2AEE
loc_1_2A7E:
        push   dword ptr [esp + 4]
        push   0xa
        push   0x2a16                        ; -> sub_1_2A16
        call   sub_6_0040
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_2B9F
loc_1_2A9D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0xb3], 4
        je     loc_1_2AE0
        push   eax
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    loc_1_2ADC
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        add    esp, 4
        push   0x10
        push   dword ptr [esp + 8]
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_2B9F
loc_1_2ADC:
        mov    bl, 0x74
        jmp    loc_1_2AEE
loc_1_2AE0:
        sub    cl, 0x10
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 0x20
        add    bl, 0x54
loc_1_2AEE:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x120], 2
        jne    loc_1_2B51
        test   byte ptr [eax + 0x58], 1
        je     loc_1_2B51
        mov    eax, dword ptr [eax + 0xf4]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x5c]
        mov    al, byte ptr [ecx + eax]
        push   eax
        push   ebx
        push   dword ptr [esp + 0xc]
        call   sub_1_30ED
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2B43
        push   dword ptr [esp + 4]
        push   0xa
        push   0x2a16                        ; -> sub_1_2A16
        call   sub_6_0040
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_2B9F
loc_1_2B43:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x5c]
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 0x58]
loc_1_2B51:
        push   dword ptr [esp + 4]
        call   sub_1_1A30
        add    esp, 4
        or     eax, eax
        jne    loc_1_2B76
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 0x15c]
        mov    eax, dword ptr [eax + 0x134]
        mov    word ptr [eax + 0xa], cx
loc_1_2B76:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    dword ptr [eax + 0x14c], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0xbc], 0x158b ; -> loc_1_158B
        push   dword ptr [esp + 8]
        call   sub_1_2950
        add    esp, 8
loc_1_2B9F:
        pop    ebx
        add    esp, 4
        ret    
sub_1_2BA4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        movzx  edi, word ptr [esi + 0x94]
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, dword ptr [esi + 0x54]
        mov    dword ptr [esi + 0x158], ecx
        mov    eax, dword ptr [esi + 0x134]
        add    edi, 0x402
        mov    edx, dword ptr [esi + 0x148]
        cmp    edx, ecx
        jb     loc_1_2BD5
        mov    edx, ecx
loc_1_2BD5:
        mov    dword ptr [eax], edx
        push   4
        push   eax
        push   dword ptr [esi + 0x130]
        call   sub_1_3865
        add    esp, 0xc
        push   0x7c
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x78
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [esi + 0x160], 1
        mov    ecx, dword ptr [esi + 0x134]
        mov    ax, word ptr [esi + 0x15c]
        mov    word ptr [ecx + 0xa], ax
        push   dword ptr [esi + 0x130]
        call   sub_1_38A3
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_1_2C2A:
        sub    esp, 4
        mov    dword ptr [esp], edx
        cmp    dword ptr [esp], 0
        je     loc_1_2CCE
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x140], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0x160], 0
        je     loc_1_2C70
        push   eax
        push   0xa
        push   0x2c2a                        ; -> loc_1_2C2A
        call   sub_6_0040
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x140], eax
        jmp    loc_1_2CCE
loc_1_2C70:
        push   dword ptr [esp]
        call   sub_1_064D
        add    esp, 4
        or     eax, eax
        je     loc_1_2CCE
        mov    eax, dword ptr [esp]
        cmp    byte ptr [eax + 0xb3], 0x26
        jne    loc_1_2CA6
        push   eax
        push   0xa
        push   0x2c2a                        ; -> loc_1_2C2A
        call   sub_6_0040
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x140], eax
        jmp    loc_1_2CCE
loc_1_2CA6:
        push   dword ptr [esp]
        call   sub_1_2BA4
        mov    eax, dword ptr [esp + 4]
        add    esp, 4
        mov    dword ptr [eax + 0x14c], 2
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0xbc], 0x158b ; -> loc_1_158B
loc_1_2CCE:
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x1c]
        sub    esi, esi
        mov    eax, dword ptr [eax + 0xf4]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 0x1c]
        add    edi, dword ptr [eax + 0x5c]
        test   byte ptr [esp + 0x20], 1
        je     loc_1_2D33
        mov    eax, edi
        inc    edi
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        call   sub_1_30ED
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_1_2D25
        sub    eax, eax
        jmp    loc_1_2DF4
loc_1_2D25:
        mov    eax, dword ptr [esp + 0x1c]
        dec    dword ptr [esp + 0x20]
        inc    dword ptr [eax + 0x5c]
        dec    dword ptr [eax + 0x58]
loc_1_2D33:
        mov    eax, dword ptr [esp + 0x1c]
        movzx  eax, word ptr [eax + 0x94]
        lea    ecx, [eax + 0x402]
        add    eax, 0x400
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x14], ecx
        movzx  ebx, word ptr [eax + 0x122]
        mov    eax, dword ptr [esp + 0x20]
        cmp    ebx, eax
        jb     loc_1_2D66
        mov    ebx, eax
loc_1_2D66:
        mov    eax, dword ptr [esp + 0x1c]
        sub    ebp, ebp
        mov    dword ptr [eax + 0x160], 1
loc_1_2D76:
        push   dword ptr [esp + 0x14]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     loc_1_2DB5
        cmp    eax, 2
        je     loc_1_2DCB
        movzx  ax, byte ptr [edi]
        movsx  cx, byte ptr [edi + 1]
        inc    edi
        shl    cx, 8
        or     ax, cx
        push   eax
        add    ebp, 2
        push   dword ptr [esp + 0x14]
        call   sub_1_3F5D
        add    esp, 8
        inc    edi
        sub    esi, esi
        jmp    loc_1_2DD4
loc_1_2DB5:
        push   ebx
        push   edi
        add    ebp, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3F70
        add    esp, 0xc
        add    edi, ebx
        sub    esi, esi
        jmp    loc_1_2DD4
loc_1_2DCB:
        inc    esi
        cmp    esi, 0x800
        ja     loc_1_2DE8
loc_1_2DD4:
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, ebp
        cmp    eax, ebx
        jb     loc_1_2DE0
        mov    eax, ebx
loc_1_2DE0:
        mov    ebx, eax
        cmp    ebp, dword ptr [esp + 0x20]
        jb     loc_1_2D76
loc_1_2DE8:
        mov    eax, dword ptr [esp + 0x1c]
        add    dword ptr [eax + 0x5c], ebp
        sub    dword ptr [eax + 0x58], ebp
        mov    eax, ebp
loc_1_2DF4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_2DFC:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esi + 0x58]
        or     ecx, ecx
        jne    loc_1_2E16
        mov    dword ptr [esi + 0x160], 0
        jmp    loc_1_2E65
loc_1_2E16:
        movzx  edi, word ptr [esi + 0x94]
        add    edi, 0x402
        movzx  eax, word ptr [esi + 0x122]
        shl    eax, 3
        cmp    eax, ecx
        jbe    loc_1_2E33
        mov    eax, ecx
loc_1_2E33:
        mov    bl, byte ptr [esp + 0x14]
        push   ebx
        push   eax
        push   esi
        call   sub_1_2CE0
        add    esp, 0xc
        cmp    bl, 0x74
        jne    loc_1_2E58
        mov    al, byte ptr [esi + 0x16e]
        test   al, 0x80
        je     loc_1_2E58
        test   al, 1
        jne    loc_1_2E58
        xor    bl, 0x10
loc_1_2E58:
        xor    bl, 4
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2E65:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2E69:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x20]
        sub    esi, esi
        movzx  eax, word ptr [ecx + 0x94]
        lea    ecx, [eax + 0x402]
        mov    dword ptr [esp + 0x14], ecx
        add    eax, 0x400
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x10], eax
        movzx  ebx, word ptr [ecx + 0x122]
        mov    eax, dword ptr [esp + 0x24]
        cmp    ebx, eax
        jb     loc_1_2EA5
        mov    ebx, eax
loc_1_2EA5:
        mov    ecx, dword ptr [esp + 0x20]
        sub    edi, edi
        mov    dword ptr [ecx + 0x160], 1
        mov    eax, dword ptr [ecx + 0x5c]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ecx + 0xf4]
        mov    ebp, dword ptr [esp + 0x18]
        add    ebp, dword ptr [eax + 4]
loc_1_2EC9:
        push   dword ptr [esp + 0x14]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     loc_1_2EF8
        cmp    eax, 2
        je     loc_1_2F0E
        mov    al, byte ptr [ebp]
        push   eax
        inc    edi
        push   dword ptr [esp + 0x14]
        call   sub_1_3F2E
        add    esp, 8
        inc    ebp
        sub    esi, esi
        jmp    loc_1_2F17
loc_1_2EF8:
        push   ebx
        push   ebp
        add    edi, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3F3F
        add    esp, 0xc
        add    ebp, ebx
        sub    esi, esi
        jmp    loc_1_2F17
loc_1_2F0E:
        inc    esi
        cmp    esi, 0x800
        ja     loc_1_2F2B
loc_1_2F17:
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, edi
        cmp    eax, ebx
        jb     loc_1_2F23
        mov    eax, ebx
loc_1_2F23:
        mov    ebx, eax
        cmp    edi, dword ptr [esp + 0x24]
        jb     loc_1_2EC9
loc_1_2F2B:
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        add    eax, edi
        mov    dword ptr [ecx + 0x5c], eax
        sub    dword ptr [ecx + 0x58], edi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_2F45:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [edi + 0x58]
        or     ecx, ecx
        jne    loc_1_2F5E
        mov    dword ptr [edi + 0x160], 0
        jmp    loc_1_2FAC
loc_1_2F5E:
        movzx  esi, word ptr [edi + 0x94]
        add    esi, 0x402
        movzx  eax, word ptr [edi + 0x122]
        shl    eax, 2
        cmp    eax, ecx
        jbe    loc_1_2F7B
        mov    eax, ecx
loc_1_2F7B:
        push   eax
        push   edi
        call   sub_1_2E69
        mov    cl, byte ptr [esp + 0x18]
        add    esp, 8
        cmp    cl, 0x74
        jne    loc_1_2F9F
        mov    al, byte ptr [edi + 0x16e]
        test   al, 0x80
        je     loc_1_2F9F
        test   al, 1
        jne    loc_1_2F9F
        xor    cl, 0x10
loc_1_2F9F:
        xor    cl, 4
        push   ecx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2FAC:
        pop    edi
        pop    esi
        ret    
sub_1_2FAF:
        sub    esp, 4
        push   ebx
        push   esi
        mov    dword ptr [esp + 8], edx
        cmp    dword ptr [esp + 8], 0
        je     loc_1_30E7
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x13c], 0
        mov    eax, dword ptr [esp + 8]
        movzx  esi, word ptr [eax + 0x94]
        add    esi, 0x402
        cmp    dword ptr [eax + 0x160], 0
        je     loc_1_300A
        cmp    byte ptr [eax + 0xb2], 0
        je     loc_1_3025
        test   byte ptr [eax + 0xb3], 4
        jne    loc_1_3025
        test   byte ptr [eax + 0x14c], 5
        jne    loc_1_30E7
loc_1_300A:
        mov    eax, dword ptr [esp + 8]
        mov    cl, byte ptr [eax + 0xb2]
        or     cl, cl
        jne    loc_1_3044
        push   eax
        call   sub_1_01B8
        add    esp, 4
        mov    bl, 0x54
        jmp    loc_1_3091
loc_1_3025:
        push   dword ptr [esp + 8]
        push   0xa
        push   0x2faf                        ; -> sub_1_2FAF
        call   sub_6_0040
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0x13c], eax
        jmp    loc_1_30E7
loc_1_3044:
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 0xb3], 4
        je     loc_1_3083
        push   eax
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    loc_1_307F
        push   dword ptr [esp + 8]
        call   sub_1_01B8
        add    esp, 4
        push   0x10
        push   dword ptr [esp + 0xc]
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        jne    loc_1_30E7
loc_1_307F:
        mov    bl, 0x74
        jmp    loc_1_3091
loc_1_3083:
        sub    cl, 0x10
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 0x20
        add    bl, 0x54
loc_1_3091:
        push   ebx
        push   esi
        call   sub_1_3F2E
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 8
        movzx  eax, word ptr [eax + 0x120]
        cmp    eax, 1
        je     loc_1_30B2
        cmp    eax, 2
        je     loc_1_30BE
        jmp    loc_1_30CB
loc_1_30B2:
        push   ebx
        push   dword ptr [esp + 0xc]
        call   sub_1_2F45
        jmp    loc_1_30C8
loc_1_30BE:
        push   ebx
        push   dword ptr [esp + 0xc]
        call   sub_1_2DFC
loc_1_30C8:
        add    esp, 8
loc_1_30CB:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0xbc], 0x1648 ; -> loc_1_1648
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x14c], 4
loc_1_30E7:
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_30ED:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        movzx  edi, word ptr [esi + 0x94]
        lea    ebp, [edi + 0x402]
        add    edi, 0x400
        cmp    dword ptr [esi + 0x160], 0
        je     loc_1_3134
        cmp    byte ptr [esi + 0xb2], 0
        je     loc_1_3126
        test   byte ptr [esi + 0xb3], 4
        je     loc_1_3134
loc_1_3126:
        mov    dword ptr [esi + 0x34], 0
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_3134:
        cmp    byte ptr [esp + 0x24], 0x74
        jne    loc_1_316A
        push   esi
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    loc_1_316A
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   0x10
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_1_316A
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_316A:
        movzx  eax, word ptr [esi + 0x120]
        cmp    eax, 1
        je     loc_1_3189
        cmp    eax, 2
        je     loc_1_31C4
        mov    dword ptr [esi + 0x34], 0x10000
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_3189:
        push   0
        push   ebp
        call   sub_1_051E
        add    esp, 8
        or     eax, eax
        je     loc_1_31AC
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x100
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_31AC:
        push   dword ptr [esp + 0x24]
        push   ebp
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 0x30]
        add    esp, 8
        push   eax
        push   edi
        jmp    loc_1_32AC
loc_1_31C4:
        movzx  ebx, word ptr [esi + 0x94]
        push   0x64
        lea    eax, [ebx + 1]
        mov    dword ptr [esp + 0x18], eax
        add    ebx, 2
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    loc_1_31F8
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x100
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_31F8:
        push   0x34
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0xef
        mov    byte ptr [esp + 0x13], al
        and    al, 0xfe
        mov    byte ptr [esp + 0x18], al
        push   dword ptr [esp + 0x18]
        push   ebx
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 0x30]
        add    esp, 8
        push   eax
        push   edi
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 0x1b]
        add    esp, 8
        or     al, 1
        push   eax
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   0x64
        push   0
        push   0x80
        push   dword ptr [esp + 0x20]
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_326D
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x200
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_326D:
        push   dword ptr [esp + 0x18]
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x80
        push   0x80
        push   dword ptr [esp + 0x20]
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_32A7
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], 0x200
        sub    eax, eax
        jmp    loc_1_32D7
loc_1_32A7:
        push   dword ptr [esp + 0x24]
        push   ebp
loc_1_32AC:
        call   sub_1_3F2E
        add    esp, 8
        cmp    byte ptr [esp + 0x24], 0x74
        jne    loc_1_32CB
        mov    byte ptr [esi + 0xb2], 0x10
        mov    byte ptr [esi + 0xb3], 0x23
        jmp    loc_1_32D2
loc_1_32CB:
        mov    byte ptr [esi + 0xb2], 0x40
loc_1_32D2:
        mov    eax, 1
loc_1_32D7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_32DF:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x24]
        sub    esi, esi
        movzx  ecx, word ptr [ecx + 0x94]
        lea    eax, [ecx + 0x402]
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [ecx + 0x400]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 1
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x28], eax
        movzx  ebx, word ptr [ecx + 0x122]
        cmp    ebx, eax
        jb     loc_1_332F
        mov    ebx, eax
loc_1_332F:
        mov    ecx, dword ptr [esp + 0x24]
        sub    edi, edi
        mov    dword ptr [ecx + 0x160], 1
        mov    eax, dword ptr [ecx + 0x54]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [ecx + 0x3c]
        add    ebp, eax
loc_1_334B:
        push   dword ptr [esp + 0x14]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     loc_1_3378
        cmp    eax, 2
        je     loc_1_33A6
        mov    dx, word ptr [esp + 0x10]
        in     ax, dx
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], ah
        inc    ebp
        inc    ebp
        add    edi, 2
        jmp    loc_1_33B8
loc_1_3378:
        inc    esi
        cmp    esi, 0x400
        jbe    loc_1_33BA
        mov    ecx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x18]
        add    dword ptr [ecx + 0x4c], edi
        add    eax, edi
        mov    dword ptr [ecx + 0x54], eax
        or     edi, edi
        je     loc_1_33FF
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0x88], eax
        jmp    loc_1_33FF
loc_1_33A6:
        push   ebx
        push   ebp
        add    ebp, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3ECB
        add    esp, 0xc
        add    edi, ebx
loc_1_33B8:
        sub    esi, esi
loc_1_33BA:
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, edi
        cmp    eax, ebx
        jb     loc_1_33C6
        mov    eax, ebx
loc_1_33C6:
        mov    ebx, eax
        cmp    edi, dword ptr [esp + 0x28]
        jb     loc_1_334B
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_33E4
        mov    dx, word ptr [esp + 0x10]
        in     ax, dx
        mov    byte ptr [ebp], al
        inc    edi
loc_1_33E4:
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0x88], eax
        add    dword ptr [ecx + 0x4c], edi
        mov    eax, dword ptr [esp + 0x18]
        add    eax, edi
        mov    dword ptr [ecx + 0x54], eax
loc_1_33FF:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_3409:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xf0], 0
        je     loc_1_34AE
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, dword ptr [esi + 0x54]
        je     loc_1_34AE
        movzx  eax, word ptr [esi + 0x122]
        shl    eax, 3
        cmp    ecx, eax
        jae    loc_1_3437
        mov    eax, ecx
loc_1_3437:
        push   eax
        push   esi
        call   sub_1_32DF
        add    esp, 8
        test   byte ptr [esi + 0x150], 1
        je     loc_1_3460
        mov    eax, dword ptr [esi + 0x54]
        cmp    dword ptr [esi + 0xf0], eax
        jg     loc_1_3460
        push   1
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_3460:
        mov    eax, dword ptr [esi + 0x54]
        cmp    dword ptr [esi + 0xf0], eax
        jle    loc_1_349A
        movzx  eax, word ptr [esi + 0x94]
        add    eax, 0x402
        mov    cl, byte ptr [esi + 0x16e]
        test   cl, 0x80
        je     loc_1_3489
        test   cl, 1
        mov    cl, 0x64
        je     loc_1_348B
loc_1_3489:
        mov    cl, 0x74
loc_1_348B:
        xor    cl, 4
        push   ecx
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        jmp    loc_1_34AE
loc_1_349A:
        mov    dword ptr [esi + 0x160], 0
        mov    dword ptr [esi + 0x14c], 0
loc_1_34AE:
        pop    esi
        ret    
sub_1_34B0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x20]
        sub    esi, esi
        movzx  eax, word ptr [ecx + 0x94]
        lea    ecx, [eax + 0x402]
        mov    dword ptr [esp + 0x14], ecx
        add    eax, 0x400
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x10], eax
        movzx  ebx, word ptr [ecx + 0x122]
        mov    eax, dword ptr [esp + 0x24]
        cmp    ebx, eax
        jb     loc_1_34EC
        mov    ebx, eax
loc_1_34EC:
        mov    ecx, dword ptr [esp + 0x20]
        sub    edi, edi
        mov    dword ptr [ecx + 0x160], 1
        mov    eax, dword ptr [ecx + 0x54]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [ecx + 0x3c]
        add    ebp, eax
loc_1_3508:
        push   dword ptr [esp + 0x14]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     loc_1_3534
        cmp    eax, 2
        je     loc_1_3562
        push   dword ptr [esp + 0x10]
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [ebp], al
        inc    ebp
        inc    edi
        jmp    loc_1_3574
loc_1_3534:
        inc    esi
        cmp    esi, 0x400
        jbe    loc_1_3576
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x18]
        add    dword ptr [ecx + 0x4c], edi
        add    eax, edi
        mov    dword ptr [ecx + 0x54], eax
        or     edi, edi
        je     loc_1_35A9
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x88], eax
        jmp    loc_1_35A9
loc_1_3562:
        push   ebx
        push   ebp
        add    ebp, ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_3E9E
        add    esp, 0xc
        add    edi, ebx
loc_1_3574:
        sub    esi, esi
loc_1_3576:
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, edi
        cmp    eax, ebx
        jb     loc_1_3582
        mov    eax, ebx
loc_1_3582:
        mov    ebx, eax
        cmp    dword ptr [esp + 0x24], edi
        ja     loc_1_3508
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x88], eax
        add    dword ptr [ecx + 0x4c], edi
        mov    eax, dword ptr [esp + 0x18]
        add    eax, edi
        mov    dword ptr [ecx + 0x54], eax
loc_1_35A9:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_35B3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xf0], 0
        je     loc_1_3658
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, dword ptr [esi + 0x54]
        je     loc_1_3658
        movzx  eax, word ptr [esi + 0x122]
        shl    eax, 2
        cmp    ecx, eax
        jae    loc_1_35E1
        mov    eax, ecx
loc_1_35E1:
        push   eax
        push   esi
        call   sub_1_34B0
        add    esp, 8
        test   byte ptr [esi + 0x150], 1
        je     loc_1_360A
        mov    eax, dword ptr [esi + 0x54]
        cmp    dword ptr [esi + 0xf0], eax
        jg     loc_1_360A
        push   1
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_360A:
        mov    eax, dword ptr [esi + 0x54]
        cmp    dword ptr [esi + 0xf0], eax
        jle    loc_1_3644
        movzx  eax, word ptr [esi + 0x94]
        add    eax, 0x402
        mov    cl, byte ptr [esi + 0x16e]
        test   cl, 0x80
        je     loc_1_3633
        test   cl, 1
        mov    cl, 0x64
        je     loc_1_3635
loc_1_3633:
        mov    cl, 0x74
loc_1_3635:
        xor    cl, 4
        push   ecx
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        jmp    loc_1_3658
loc_1_3644:
        mov    dword ptr [esi + 0x160], 0
        mov    dword ptr [esi + 0x14c], 0
loc_1_3658:
        pop    esi
        ret    
loc_1_365A:
        sub    esp, 4
        mov    dword ptr [esp], edx
        cmp    dword ptr [esp], 0
        je     loc_1_3728
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x140], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0x160], 0
        je     loc_1_36A3
        push   eax
        push   0xa
        push   0x365a                        ; -> loc_1_365A
        call   sub_6_0040
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x140], eax
        jmp    loc_1_3728
loc_1_36A3:
        push   dword ptr [esp]
        call   sub_1_064D
        add    esp, 4
        or     eax, eax
        jne    loc_1_36CF
        push   dword ptr [esp]
        push   0xa
        push   0x365a                        ; -> loc_1_365A
        call   sub_6_0040
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x140], eax
        jmp    loc_1_3728
loc_1_36CF:
        mov    ecx, dword ptr [esp]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 0x54]
        cmp    dword ptr [ecx + 0xf0], eax
        jle    loc_1_3728
        mov    eax, ecx
        mov    dword ptr [eax + 0xbc], 0x169b ; -> loc_1_169B
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x14c], 8
        mov    eax, dword ptr [esp]
        movzx  eax, word ptr [eax + 0x120]
        cmp    eax, 1
        je     loc_1_3711
        cmp    eax, 2
        je     loc_1_371C
        jmp    loc_1_3728
loc_1_3711:
        push   dword ptr [esp]
        call   sub_1_35B3
        jmp    loc_1_3725
loc_1_371C:
        push   dword ptr [esp]
        call   sub_1_3409
loc_1_3725:
        add    esp, 4
loc_1_3728:
        add    esp, 4
        ret    
LPT_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        je     loc_3_0D00
        clc    
        ret    
sub_1_3740:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        jae    loc_1_3753
        xor    eax, eax
loc_1_3753:
        clc    
        pop    esi
        ret    
sub_1_3756:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
        clc    
        ret    
sub_1_3762:
        push   ebx
        xor    ebx, ebx
        movzx  ebx, word ptr [esp + 8]
        VxDCall  VDMAD.VDMAD_Release_Buffer      ; cd200f000400
        jae    loc_1_3772
loc_1_3772:
        pop    ebx
        ret    
sub_1_3774:
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [esp + 0x10]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        mov    edi, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 4], esi
        mov    ecx, dword ptr [edi]
        VxDCall  VDMAD.VDMAD_Request_Buffer      ; cd200e000400
        jb     loc_1_37C2
        mov    dword ptr [edi + 0xc], edx
        mov    word ptr [edi + 0xa], bx
        push   0
        push   ecx
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_37BE
        mov    dword ptr [edi + 4], eax
        mov    eax, 1
loc_1_37B9:
        clc    
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_37BE:
        xor    eax, eax
        jmp    loc_1_37B9
loc_1_37C2:
        cmp    al, 5
        jne    loc_1_37CA
        xor    eax, eax
        jmp    loc_1_37B9
loc_1_37CA:
        cmp    al, 6
        jne    loc_1_37B9
        xor    eax, eax
        jmp    loc_1_37B9
sub_1_37D2:
        push   edi
        push   esi
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [0x488c], 0         ; -> obj1:0x488C
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe
        push   0x488c                        ; -> obj1:0x488C
        push   0x1000
        push   0
        push   ecx
        push   0
        push   1
        push   eax
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    dword ptr [0x4890], eax       ; -> obj1:0x4890
        or     eax, edx
        je     loc_1_3846
        mov    eax, dword ptr [0x488c]       ; -> obj1:0x488C
        mov    dword ptr [esi + 0xc], eax
        mov    ecx, dword ptr [esi]
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_3848
        mov    word ptr [esi + 0xa], 0x8000
        mov    dword ptr [esi + 4], eax
loc_1_383C:
        mov    eax, dword ptr [0x4890]       ; -> obj1:0x4890
        clc    
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_3846:
        jmp    loc_1_383C
loc_1_3848:
        jmp    loc_1_383C
sub_1_384A:
        push   edi
        push   esi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dl, 1
        VxDCall  VDMAD.VDMAD_Lock_DMA_Region     ; cd2009000400
        xor    eax, eax
        jb     loc_1_3862
        inc    eax
loc_1_3862:
        pop    esi
        pop    edi
        ret    
sub_1_3865:
        pushal 
        mov    edi, dword ptr [esp + 0x28]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dl, 1
        VxDCall  VDMAD.VDMAD_Lock_DMA_Region     ; cd2009000400
        jae    loc_1_387B
        jmp    loc_1_38A1
loc_1_387B:
        mov    eax, dword ptr [esp + 0x24]
        mov    bx, word ptr [edi + 0xa]
        or     bh, 0xff
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dl, byte ptr [esp + 0x2c]
        or     dl, 0
        mov    dh, 0
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
loc_1_38A1:
        popal  
        ret    
sub_1_38A3:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
        pop    ebx
        ret    
sub_1_38B6:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
        ret    
sub_1_38C1:
        push   edi
        push   esi
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    eax, eax
        mov    edi, dword ptr [esp + 0x10]
        VxDCall  VDMAD.VDMAD_Scatter_Lock        ; cd200b000400
        mov    eax, 0
        mov    word ptr [edi + 0xc], 0
        jb     loc_1_38EF
        je     loc_1_38ED
        VxDCall  VDMAD.VDMAD_Scatter_Unlock      ; cd200c000400
        jmp    loc_1_38EF
loc_1_38ED:
        mov    eax, edx
loc_1_38EF:
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_38F3:
        push   edi
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    eax, eax
        mov    edi, dword ptr [esp + 0xc]
        VxDCall  VDMAD.VDMAD_Scatter_Unlock      ; cd200c000400
        mov    word ptr [edi + 0xe], 0
        mov    word ptr [edi + 0xc], 0
        pop    ebx
        pop    edi
        ret    
sub_1_3916:
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0xc]
        xor    ebx, ebx
        mov    bx, word ptr [edi + 0xc]
        shl    bx, 3
        add    edi, ebx
        add    edi, 0x10
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        cmp    edx, 0x1000000
        jae    loc_1_3963
        movzx  ebx, word ptr [esp + 0x10]
        dec    ebx
        je     loc_1_3948
        and    ebx, edx
        jne    loc_1_3963
        and    ebx, ecx
        jne    loc_1_3963
loc_1_3948:
        push   edx
        not    dx
        and    edx, 0xffff
        inc    edx
        cmp    ecx, edx
        jbe    loc_1_395B
        xor    eax, eax
        jmp    loc_1_3965
loc_1_395B:
        pop    edx
        mov    eax, 1
        jmp    loc_1_3965
loc_1_3963:
        xor    eax, eax
loc_1_3965:
        pop    ebx
        pop    edi
        ret    
sub_1_3968:
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0x14]
        movzx  ebx, word ptr [edi + 0xc]
        mov    cx, word ptr [edi + 0xe]
        cmp    bx, cx
        jne    loc_1_3980
        xor    eax, eax
        jmp    loc_1_39B6
loc_1_3980:
        shl    bx, 3
        add    edi, ebx
        add    edi, 0x10
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, 0x100
        xor    esi, esi
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edx, 8
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, ecx
loc_1_39B6:
        pop    ebx
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
sub_1_39BC:
        push   ebp
        mov    ebp, esp
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Get_IRQ_Complete_Status ; cd200b000300
        sbb    edx, edx
        inc    edx
        mov    eax, edx
        leave  
        ret    
sub_1_39D1:
        xor    eax, eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        ret    
sub_1_39DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_39EF:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        out    dx, al
        add    dx, 2
        in     al, dx
        and    al, 0x40
        mov    cl, al
        or     al, 0xd
        out    dx, al
        out    dx, al
        out    dx, al
        out    dx, al
        or     cl, 0xc
        mov    al, cl
        out    dx, al
        leave  
        ret    
sub_1_3A12:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
loc_1_3A19:
        in     al, dx
        mov    ah, al
        jmp    loc_1_3A1E
loc_1_3A1E:
        in     al, dx
        cmp    al, ah
        jne    loc_1_3A19
        and    ah, 0xf8
        xor    ah, 0x48
        test   ah, 0x28
        jne    loc_1_3A3A
        test   ah, 0x10
        je     loc_1_3A3A
        and    ah, 0x80
        je     loc_1_3A3A
        xor    eax, eax
loc_1_3A3A:
        leave  
        ret    
sub_1_3A3C:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        and    al, 0xf8
        xor    al, 0x48
        mov    ah, al
        and    al, 0x30
        cmp    al, 0x30
        mov    al, ah
        jne    loc_1_3A56
        and    al, 0xdf
        or     al, 8
loc_1_3A56:
        test   al, 0x28
        jne    loc_1_3A63
        and    ah, 0x80
        mov    al, 1
        je     loc_1_3A63
        xor    eax, eax
loc_1_3A63:
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3A70:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [esp + 0x14]
        lea    esi, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0x40
        mov    bl, al
        or     al, 0xc
        mov    byte ptr [esp + 0xc], al
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        cmp    dword ptr [esp + 0x18], 0
        push   0x23
        je     loc_1_3AE6
        push   0x10
        push   0x50
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_3AF7
loc_1_3ABB:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_3ADF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_3AE6:
        push   0
        push   0x50
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3ABB
loc_1_3AF7:
        mov    al, bl
        or     al, 0xe
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3ABB
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x80
        push   0x80
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        jmp    loc_1_3ADF
sub_1_3B3C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  esi, word ptr [esp + 0x1c]
        lea    ebp, [esi + 1]
        lea    edi, [esi + 2]
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0x40
        mov    bl, al
        or     al, 0xc
        mov    byte ptr [esp + 0x14], al
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        mov    al, bl
        or     al, 6
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x38
        push   0x78
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_3BB3
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    eax, 0x100000
        jmp    loc_1_3C70
loc_1_3BB3:
        mov    al, bl
        or     al, 7
        mov    byte ptr [esp + 0x10], al
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        or     bl, 4
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        cmp    byte ptr [esp + 0x20], 0
        jne    loc_1_3C0D
        push   0x23
        push   0x40
        push   0x50
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3C55
        mov    eax, 0x10000
        jmp    loc_1_3C70
loc_1_3C0D:
        cmp    byte ptr [esp + 0x20], 0x10
        je     loc_1_3C35
        cmp    byte ptr [esp + 0x20], 0x14
        je     loc_1_3C35
        push   0x23
        push   0x50
        push   0x50
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3C55
        mov    eax, 0x10000
        jmp    loc_1_3C70
loc_1_3C35:
        push   0x23
        push   0xd8
        push   0xf8
        push   ebp
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3C55
        mov    eax, 0x10000
        jmp    loc_1_3C70
loc_1_3C55:
        cmp    byte ptr [esp + 0x20], 0
        jne    loc_1_3C60
        push   0
        jmp    loc_1_3C62
loc_1_3C60:
        push   1
loc_1_3C62:
        push   dword ptr [esp + 0x20]
        call   sub_1_3A70
        add    esp, 8
        sub    eax, eax
loc_1_3C70:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_3C78:
        push   ebp
        mov    ebp, esp
        push   eax
        mov    al, byte ptr [ebp + 8]
        rcl    al, 1
        jb     loc_1_3C8A
        bts    ax, 7
        jmp    loc_1_3C8F
loc_1_3C8A:
        btr    ax, 7
loc_1_3C8F:
        shr    al, 4
        mov    ah, byte ptr [ebp + 0xc]
        rcl    ah, 1
        jb     loc_1_3CA0
        bts    ax, 0xf
        jmp    loc_1_3CA5
loc_1_3CA0:
        btr    ax, 0xf
loc_1_3CA5:
        and    ah, 0xf0
        or     al, ah
        mov    byte ptr [ebp + 0xc], al
        pop    eax
        mov    al, byte ptr [ebp + 0xc]
        pop    ebp
        ret    
sub_1_3CB3:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  ebp, word ptr [esp + 0x1c]
        sub    edi, edi
        lea    esi, [ebp + 1]
        add    ebp, 2
        push   ebp
        call   sub_1_3E90
        mov    byte ptr [esp + 0x15], al
        add    esp, 4
        test   al, 2
        je     loc_1_3D2B
        push   esi
        call   sub_1_3E20
        add    esp, 4
        cmp    eax, edi
        je     loc_1_3CF2
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edi
        sub    eax, eax
        jmp    loc_1_3E16
loc_1_3CF2:
        and    byte ptr [esp + 0x11], 0xf0
        mov    al, byte ptr [esp + 0x11]
        or     al, 4
        push   eax
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x40
        push   0x60
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_3D30
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], 0
        sub    eax, eax
        jmp    loc_1_3E16
loc_1_3D2B:
        and    byte ptr [esp + 0x11], 0xf0
loc_1_3D30:
        mov    al, byte ptr [esp + 0x11]
        or     al, 6
        mov    byte ptr [esp + 0x14], al
loc_1_3D3A:
        push   dword ptr [esp + 0x14]
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0
        push   0x40
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3E0B
        push   esi
        call   sub_1_3E90
        mov    byte ptr [esp + 0x16], al
        mov    bl, byte ptr [esp + 0x15]
        add    esp, 4
        or     bl, 4
        push   ebx
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3E0B
        push   dword ptr [esp + 0x14]
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   0x23
        push   0
        push   0x40
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3E0B
        push   esi
        call   sub_1_3E90
        mov    byte ptr [esp + 0x17], al
        add    esp, 4
        push   ebx
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0x13]
        inc    edi
        push   dword ptr [esp + 0x16]
        call   sub_1_3C78
        mov    ecx, dword ptr [esp + 0x28]
        add    esp, 8
        mov    byte ptr [ecx], al
        inc    dword ptr [esp + 0x20]
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_3E38
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3E0B
        push   esi
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    loc_1_3E0B
        cmp    edi, dword ptr [esp + 0x24]
        jb     loc_1_3D3A
loc_1_3E0B:
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edi
        mov    eax, 1
loc_1_3E16:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  000h, 000h
sub_1_3E20:
        push   dword ptr [esp + 4]
        call   sub_1_3E90
        add    esp, 4
        test   al, 8
        mov    eax, 1
        jne    loc_1_3E37
        sub    eax, eax
loc_1_3E37:
        ret    
sub_1_3E38:
        push   ebx
        push   esi
        push   edi
        push   0x4894                        ; -> obj1:0x4894
        call   sub_3_0E00
        mov    esi, dword ptr [esp + 0x14]
        mov    bl, byte ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x20]
        add    esp, 4
loc_1_3E54:
        push   0x4898                        ; -> obj1:0x4898
        call   sub_3_0E00
        add    esp, 4
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, bl
        cmp    al, byte ptr [esp + 0x18]
        je     loc_1_3E87
        mov    eax, dword ptr [0x4898]       ; -> obj1:0x4898
        sub    eax, dword ptr [0x4894]       ; -> obj1:0x4894
        cmp    eax, edi
        jl     loc_1_3E54
        sub    eax, eax
loc_1_3E83:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3E87:
        mov    eax, 1
        jmp    loc_1_3E83
        db  000h, 000h
sub_1_3E90:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        pop    edx
        leave  
        ret    
sub_1_3E9E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_3EB1:
        in     al, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        loop   loc_1_3EB1
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        pop    edx
        leave  
        ret    
sub_1_3ECB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 1
loc_1_3EE0:
        in     ax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_3EE0
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     eax, dx
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 2
loc_1_3F14:
        in     eax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        shr    eax, 0x10
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_3F14
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_3F2E:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        pop    edx
        leave  
        ret    
sub_1_3F3F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_3F52:
        mov    al, byte ptr [ebx + esi]
        out    dx, al
        inc    ebx
        loop   loc_1_3F52
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_3F5D:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        out    dx, ax
        pop    edx
        leave  
        ret    
sub_1_3F70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 1
loc_1_3F85:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        out    dx, ax
        loop   loc_1_3F85
        pop    ebx
        pop    esi
        leave  
        ret    
loc_1_3F95:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        out    dx, eax
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 2
loc_1_3FBC:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        shl    eax, 0x10
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        ror    eax, 0x10
        out    dx, eax
        loop   loc_1_3FBC
        pop    ebx
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 001h, 001h, 001h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 003h, 003h, 003h, 003h, 003h, 003h
loc_1_3FF4:
        retf   0x28
        db  000h
        dd  sub_3_0C98               ; fixup
        dd  loc_3_0922               ; fixup
        dd  loc_1_276D               ; fixup
        dd  loc_3_0955               ; fixup
        dd  sub_1_22C3               ; fixup
        dd  loc_1_272B               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_0A7D               ; fixup
        dd  sub_1_248C               ; fixup
        dd  loc_1_221A               ; fixup
        dd  loc_3_0AA8               ; fixup
        dd  loc_3_0ACE               ; fixup
        dd  loc_1_2820               ; fixup
        dd  loc_1_2861               ; fixup
        dd  loc_3_0AF9               ; fixup
        dd  loc_3_0B91               ; fixup
        dd  loc_3_0B32               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_0CA9               ; fixup
        dd  loc_3_0CD5               ; fixup
        dd  loc_1_28DB               ; fixup
        dd  loc_1_2631               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'LPT     '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  LPT_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x24  R-X
loc_2_0000:
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jae    9
        ret    
        db  068h
        dd  loc_3_0D00               ; fixup
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h, 059h, 080h, 03Dh
        dd  obj4:0x0020              ; fixup
        db  000h, 074h, 002h, 0F8h, 0C3h, 0F8h, 0C3h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xe10  R-X
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 044h, 024h, 018h, 066h, 02Bh, 0FFh, 00Fh, 0B7h
        db  098h, 094h, 000h, 000h, 000h, 08Dh, 043h, 002h, 089h, 044h, 024h, 010h, 08Dh, 0ABh, 002h, 004h
        db  000h, 000h, 08Bh, 044h, 024h, 018h, 081h, 0C3h, 000h, 004h, 000h, 000h, 0FFh, 074h, 024h, 010h
        db  066h, 0C7h, 080h, 022h, 001h, 000h, 000h, 001h, 000h, 066h, 089h, 0B8h, 01Ch, 001h, 000h, 000h
        db  066h, 0C7h, 080h, 01Eh, 001h, 000h, 000h, 001h, 000h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 00Ch, 020h, 050h, 0FFh, 074h, 024h, 014h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  083h, 0C4h, 008h, 068h, 0D0h, 000h, 000h, 000h, 055h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  08Bh, 044h, 024h, 020h, 083h, 0C4h, 008h, 066h, 08Bh, 0B0h, 020h, 001h, 000h, 000h, 066h, 083h
        db  0FEh, 001h, 075h, 00Dh, 068h, 0FFh, 000h, 000h, 000h, 053h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  0EBh, 01Bh, 066h, 083h, 0FEh, 002h, 075h, 00Dh, 068h, 0FFh, 0FFh, 000h, 000h, 053h, 0E8h
        dd  sub_1_3F5D               ; fixup
        db  0EBh, 008h, 06Ah, 0FFh, 053h, 0E8h
        dd  loc_1_3F95               ; fixup
        db  083h, 0C4h, 008h, 066h, 047h, 055h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 08Ah, 0C8h, 080h, 0E1h, 003h, 080h, 0F9h, 001h, 074h, 029h, 0A8h, 004h, 074h
        db  0B6h, 066h, 08Bh, 0CFh, 08Bh, 044h, 024h, 018h, 066h, 00Fh, 0AFh, 0CEh, 066h, 089h, 088h, 01Eh
        db  001h, 000h, 000h, 066h, 083h, 0FEh, 001h, 075h, 014h, 068h, 0FFh, 000h, 000h, 000h, 053h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  0EBh, 022h, 02Bh, 0C0h, 0E9h, 0B1h, 000h, 000h, 000h, 066h, 083h, 0FEh, 002h, 075h, 00Dh, 068h
        db  0FFh, 0FFh, 000h, 000h, 053h, 0E8h
        dd  sub_1_3F5D               ; fixup
        db  0EBh, 008h, 06Ah, 0FFh, 053h, 0E8h
        dd  loc_1_3F95               ; fixup
        db  083h, 0C4h, 008h, 066h, 047h, 055h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 024h, 003h, 03Ch, 002h, 075h, 0B7h, 08Bh, 044h, 024h, 018h, 0FFh, 074h, 024h
        db  010h, 066h, 089h, 0B8h, 022h, 001h, 000h, 000h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 024h, 0DFh, 050h, 0FFh, 074h, 024h, 014h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  083h, 0C4h, 008h, 055h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 024h, 0FBh, 050h, 055h, 0E8h
        dd  sub_1_3F2E               ; fixup
        db  083h, 0C4h, 008h, 066h, 083h, 0FEh, 001h, 075h, 00Bh, 053h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 0EBh, 00Eh, 066h, 083h, 0FEh, 002h, 066h, 08Bh, 0D3h, 075h, 004h, 066h, 0EDh
        db  0EBh, 001h, 0EDh, 066h, 04Fh, 055h, 0E8h
        dd  sub_1_3E90               ; fixup
        db  083h, 0C4h, 004h, 0A8h, 004h, 075h, 005h, 066h, 00Bh, 0FFh, 075h, 0CDh, 08Bh, 044h, 024h, 018h
        db  066h, 00Fh, 0AFh, 0FEh, 066h, 089h, 0B8h, 01Ch, 001h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h
        db  05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h
sub_3_01A8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        movzx  ebx, word ptr [esi + 0x94]
        mov    word ptr [esi + 0x9a], 1
        lea    edi, [ebx + 2]
        add    ebx, 0x402
        push   edi
        call   sub_1_3E90
        add    esp, 4
        mov    cl, al
        and    cl, 3
        cmp    cl, 1
        jne    loc_3_01E7
        and    al, 0xfe
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_3_01E7:
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0x1f
        or     al, 0x20
        push   eax
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     loc_3_0211
        sub    eax, eax
        jmp    loc_3_021F
loc_3_0211:
        mov    word ptr [esi + 0x9a], 2
        mov    eax, 1
loc_3_021F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0223:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        push   0xf4
        movzx  esi, word ptr [edi + 0x94]
        lea    ebx, [esi + 0x402]
        add    esi, 0x400
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    cl, al
        and    cl, 0x70
        mov    word ptr [edi + 0x120], 0
        cmp    cl, 0x10
        jne    loc_3_0273
        mov    word ptr [edi + 0x120], 1
        jmp    loc_3_0290
loc_3_0273:
        or     cl, cl
        jne    loc_3_0282
        mov    word ptr [edi + 0x120], 2
        jmp    loc_3_0290
loc_3_0282:
        cmp    cl, 0x20
        jne    loc_3_02F6
        mov    word ptr [edi + 0x120], 4
loc_3_0290:
        test   al, 0x80
        lea    esi, [edi + 0x16e]
        je     loc_3_029F
        mov    byte ptr [esi], 0x80
        jmp    loc_3_02A2
loc_3_029F:
        mov    byte ptr [esi], 0
loc_3_02A2:
        test   al, 4
        lea    ebp, [edi + 0x128]
        je     loc_3_02B5
        mov    dword ptr [ebp], 0
        jmp    loc_3_02BC
loc_3_02B5:
        mov    dword ptr [ebp], 1
loc_3_02BC:
        call   sub_3_0D40
        or     eax, eax
        je     loc_3_02CC
        mov    dword ptr [ebp], 0
loc_3_02CC:
        push   edi
        call   0
        add    esp, 4
        or     eax, eax
        jne    loc_3_02DB
        jmp    loc_3_02F6
loc_3_02DB:
        cmp    byte ptr [esi], 0x80
        jne    loc_3_02E4
        push   0x14
        jmp    loc_3_02E6
loc_3_02E4:
        push   0x34
loc_3_02E6:
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        mov    eax, 1
        jmp    loc_3_0303
loc_3_02F6:
        push   0x14
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        sub    eax, eax
loc_3_0303:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0310:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        lea    esi, [edi + 0xa0]
        push   esi
        call   sub_6_0008
        add    esp, 4
        mov    dword ptr [edi + 0x170], eax
        or     eax, eax
        jne    loc_3_0333
        sub    eax, eax
        jmp    loc_3_036A
loc_3_0333:
        push   esi
        call   sub_6_0010
        add    esp, 4
        mov    dword ptr [edi + 0x174], eax
        or     eax, eax
        jne    loc_3_034A
        sub    eax, eax
        jmp    loc_3_036A
loc_3_034A:
        push   1
        push   edi
        push   0x21b0                        ; -> loc_1_21B0
        push   eax
        push   1
        call   dword ptr [edi + 0x170]
        add    esp, 0x14
        mov    dword ptr [edi + 0x178], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_3_036A:
        pop    edi
        pop    esi
        ret    
sub_3_036D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x170]
        or     eax, eax
        je     loc_3_0392
        push   0x21b0                        ; -> loc_1_21B0
        push   dword ptr [esi + 0x178]
        push   3
        call   eax
        add    esp, 0xc
        or     byte ptr [esi + 0x3b], 1
loc_3_0392:
        pop    esi
        ret    
sub_3_0394:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x170]
        or     eax, eax
        jne    loc_3_03A7
        sub    eax, eax
        jmp    loc_3_03C9
loc_3_03A7:
        xor    byte ptr [esi + 0x3b], 1
        push   0x21b0                        ; -> loc_1_21B0
        push   dword ptr [esi + 0x178]
        push   2
        call   eax
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jne    loc_3_03C9
        or     byte ptr [esi + 0x3b], 1
loc_3_03C9:
        pop    esi
        ret    
sub_3_03CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0x124]
        or     cx, cx
        je     loc_3_0465
        cmp    dword ptr [esi + 0xf8], 0
        jne    loc_3_0465
        mov    word ptr [esi + 0xfc], cx
        lea    eax, [esi + 0xfc]
        mov    word ptr [esi + 0xfe], 4
        mov    dword ptr [esi + 0x100], 0x1267 ; -> loc_1_1267
        mov    dword ptr [esi + 0x104], 0
        mov    dword ptr [esi + 0x108], 0
        mov    dword ptr [esi + 0x10c], 0
        mov    dword ptr [esi + 0x110], 0
        mov    dword ptr [esi + 0x114], 0x1f4
        mov    dword ptr [esi + 0x118], esi
        push   eax
        call   sub_1_2206
        add    esp, 4
        mov    dword ptr [esi + 0xf8], eax
        or     eax, eax
        je     loc_3_048E
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_3_0465:
        mov    ax, word ptr [esi + 0x126]
        or     ax, ax
        je     loc_3_048E
        cmp    dword ptr [esi + 0x130], 0
        jne    loc_3_048E
        push   0
        movzx  eax, ax
        push   eax
        call   sub_1_3740
        add    esp, 8
        mov    dword ptr [esi + 0x130], eax
loc_3_048E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0493:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xf8]
        or     eax, eax
        je     loc_3_04CF
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        mov    eax, dword ptr [esi + 0xf8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        mov    dword ptr [esi + 0xf8], 0
loc_3_04CF:
        mov    eax, dword ptr [esi + 0x130]
        or     eax, eax
        je     loc_3_04FA
        push   eax
        call   sub_1_38B6
        add    esp, 4
        push   dword ptr [esi + 0x130]
        call   sub_1_3756
        add    esp, 4
        mov    dword ptr [esi + 0x130], 0
loc_3_04FA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_04FF:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        movzx  edi, word ptr [esi + 0x94]
        inc    edi
        test   byte ptr [esi + 0x3b], 1
        je     loc_3_0520
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        je     loc_3_0569
loc_3_0520:
        or     dword ptr [esi + 0x168], 0x80
        movzx  eax, byte ptr [esi + 0xb2]
        or     eax, eax
        je     loc_3_0543
        cmp    eax, 0x10
        je     loc_3_056C
        cmp    eax, 0x80
        je     loc_3_0555
        jmp    loc_3_055F
loc_3_0543:
        cmp    dword ptr [esi + 0x160], 1
        je     loc_3_055F
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_3_0555:
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
loc_3_055F:
        xor    dword ptr [esi + 0x168], 0x80
loc_3_0569:
        pop    edi
        pop    esi
        ret    
loc_3_056C:
        push   esi
        call   sub_1_0BA6
        add    esp, 4
        jmp    loc_3_055F
loc_3_0577:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 1
        mov    ebp, dword ptr [esp + 0x38]
        push   3
        push   ebp
        push   0                             ; -> obj4:0x0000
        call   sub_3_0D10
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_08D0
        mov    al, byte ptr [ebp + 3]
        sub    al, 0x31
        cmp    al, 3
        ja     loc_3_08D0
        mov    ebx, dword ptr [esp + 0x30]
        or     ebx, ebx
        je     loc_3_08D0
        movzx  edi, al
        mov    eax, dword ptr [edi*4 + 4]    ; -> obj5:0x0004
        or     eax, eax
        je     loc_3_05D6
        cmp    dword ptr [eax + 0x64], 0x534d5446
        je     loc_3_08D0
loc_3_05D6:
        push   1
        push   0x180
        call   sub_6_0018
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_3_08D0
        mov    dword ptr [edi*4 + 4], esi    ; -> obj5:0x0004
        mov    word ptr [esi + 0x94], bx
        lea    ebx, [esi + 0xa0]
        push   4
        push   ebp
        push   ebx
        call   sub_1_39DA
        add    esp, 0xc
        push   ebx
        push   0x8d8                         ; -> loc_3_08D8
        mov    dword ptr [esi + 0x168], 0
        push   dword ptr [esp + 0x34]
        call   sub_6_0000
        add    esp, 0xc
        push   esi
        call   sub_3_0310
        add    esp, 4
        or     eax, eax
        jne    loc_3_064B
        mov    dword ptr [esi + 0x164], 1
        or     byte ptr [esi + 0x3b], 1
        jmp    loc_3_08D0
loc_3_064B:
        mov    dword ptr [esi + 0x164], 0
        lea    eax, [esp + 0x1c]
        push   1
        push   eax
        mov    byte ptr [esi + 0x3b], 0
        lea    eax, [esp + 0x1b]
        push   eax
        push   3
        push   4                             ; -> obj4:0x0004
        mov    ebx, dword ptr [esp + 0x3c]
        push   0
        push   ebx
        call   sub_6_0084
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_068D
        cmp    byte ptr [esp + 0x13], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi + 0x168], eax
loc_3_068D:
        or     dword ptr [esi + 0x168], 0x80
        mov    word ptr [esi + 0x98], 1
        mov    byte ptr [esi + 0xb2], 0x80
        mov    byte ptr [esi + 0xb3], 3
        lea    eax, [esp + 0x1c]
        push   1
        push   eax
        mov    byte ptr [esi + 0xb0], 0xff
        lea    eax, [esp + 0x1b]
        push   eax
        push   3
        push   0x14                          ; -> obj4:0x0014
        push   0
        push   ebx
        call   sub_6_0084
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_06E2
        cmp    byte ptr [esp + 0x13], 0
        je     loc_3_0816
loc_3_06E2:
        push   esi
        call   sub_3_01A8
        add    esp, 4
        or     eax, eax
        je     loc_3_0816
        push   2
        lea    eax, [esp + 0x1c]
        push   ebx
        push   eax
        call   sub_6_0074
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0816
        mov    eax, dword ptr [esp + 0x34]
        or     eax, eax
        je     loc_3_071B
        lea    ebp, [esi + 0x124]
        jmp    loc_3_0748
loc_3_071B:
        push   0
        push   0
        push   4
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x24]
        push   eax
        call   sub_6_007C
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_0816
        mov    eax, dword ptr [esp + 0x14]
        lea    ebp, [esi + 0x124]
        mov    ax, word ptr [eax + 2]
loc_3_0748:
        mov    word ptr [ebp], ax
        push   4
        push   ebx
        push   0
        call   sub_6_0074
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0772
        movzx  eax, word ptr [ebp]
        push   eax
        call   sub_1_39BC
        add    esp, 4
        or     eax, eax
        je     loc_3_0816
loc_3_0772:
        push   0
        push   0
        push   3
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x24]
        push   eax
        call   sub_6_007C
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_0816
        mov    eax, dword ptr [esp + 0x14]
        push   esi
        movzx  ax, byte ptr [eax + 1]
        mov    word ptr [esi + 0x126], ax
        call   sub_3_0223
        add    esp, 4
        or     eax, eax
        je     loc_3_0816
        push   esi
        call   sub_3_03CB
        add    esp, 4
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_3_0816
        push   1
        push   0x10
        call   sub_6_0018
        add    esp, 8
        mov    dword ptr [esi + 0x134], eax
        push   1
        push   0x98
        call   sub_6_0018
        add    esp, 8
        mov    dword ptr [esi + 0x138], eax
        cmp    dword ptr [esi + 0x134], 0
        je     loc_3_080D
        or     eax, eax
        je     loc_3_080D
        mov    word ptr [esi + 0x98], 2
        mov    byte ptr [esi + 0xb2], 0x40
        mov    byte ptr [esi + 0xb0], 0x10
        jmp    loc_3_0816
loc_3_080D:
        push   esi
        call   sub_3_0493
        add    esp, 4
loc_3_0816:
        mov    dword ptr [0], edi            ; -> obj5:0x0000
        mov    dword ptr [esi + 0x9c], edi
        mov    word ptr [esi], 0x94
        mov    word ptr [esi + 2], 0x100
        mov    dword ptr [esi + 4], 0x3ff4   ; -> loc_1_3FF4
        mov    dword ptr [esi + 8], 0x17
        mov    dword ptr [esi + 0x158], 0
        mov    dword ptr [esi + 0x13c], 0
        mov    dword ptr [esi + 0x140], 0
        mov    dword ptr [esi + 0x160], 0
        mov    dword ptr [esi + 0x150], 0
        mov    dword ptr [esi + 0xec], 0xffffffff
        mov    dword ptr [esi + 0xf0], 0xffffffff
        inc    byte ptr [0x20]               ; -> obj4:0x0020
        push   0
        push   esi
        call   sub_1_00BD
        add    esp, 8
        cmp    byte ptr [esi + 0xb2], 0
        jne    loc_3_08A4
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_3_08A4:
        cmp    byte ptr [esi + 0xb0], 0x10
        jne    loc_3_08C6
        movzx  eax, word ptr [esi + 0x122]
        shl    eax, 2
        mov    dword ptr [esi + 0x154], eax
        push   esi
        call   sub_1_1AF4
        add    esp, 4
loc_3_08C6:
        xor    dword ptr [esi + 0x168], 0x80
loc_3_08D0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_08D8:
        mov    eax, dword ptr [esp + 4]
        movsx  eax, byte ptr [eax + 3]
        mov    eax, dword ptr [eax*4 - 0xc0] ; -> obj5:0xFFFFFF40
        or     eax, eax
        jne    loc_3_08F9
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0xfffffff6
        sub    eax, eax
        jmp    loc_3_0921
loc_3_08F9:
        cmp    dword ptr [eax + 0x164], 0
        je     loc_3_0910
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0xfffffffe
        sub    eax, eax
        jmp    loc_3_0921
loc_3_0910:
        mov    dword ptr [eax + 0x164], 1
        mov    byte ptr [eax + 0x16f], 0xff
loc_3_0921:
        ret    
loc_3_0922:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ecx + 0x16c], al
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ecx + 0x16d], al
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0955:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 0xb2]
        cmp    al, 0x40
        je     loc_3_0968
        cmp    al, 0x10
        jne    loc_3_097F
loc_3_0968:
        movzx  eax, word ptr [esi + 0x94]
        add    eax, 0x402
        push   0x64
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_097F:
        push   dword ptr [esi + 0x13c]
        call   sub_6_005C
        push   dword ptr [esi + 0x140]
        call   sub_6_005C
        push   esi
        call   sub_1_01B8
        add    esp, 4
        cmp    byte ptr [esi + 0x16d], 0
        jne    loc_3_09B9
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, eax
        je     loc_3_09B9
        push   0
        push   eax
        call   sub_6_0020
        add    esp, 8
loc_3_09B9:
        push   esi
        call   sub_1_19E0
        add    esp, 4
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_3_09B9
        cmp    dword ptr [esi + 0xa4], 0
        je     loc_3_09EE
        push   0
        push   dword ptr [esi + 0xa8]
        call   sub_6_0020
        add    esp, 8
        mov    dword ptr [esi + 0xa4], 0
loc_3_09EE:
        cmp    word ptr [esi + 0x98], 2
        jne    loc_3_0A49
        cmp    dword ptr [esi + 0x148], 0
        je     loc_3_0A29
        mov    ax, word ptr [esi + 0x15c]
        cmp    ax, 0x8000
        jne    loc_3_0A20
        push   0
        push   dword ptr [esi + 0x144]
        call   sub_6_006C
        add    esp, 8
        jmp    loc_3_0A29
loc_3_0A20:
        push   eax
        call   sub_1_3762
        add    esp, 4
loc_3_0A29:
        push   0
        push   dword ptr [esi + 0x134]
        call   sub_6_0020
        add    esp, 8
        push   0
        push   dword ptr [esi + 0x138]
        call   sub_6_0020
        add    esp, 8
loc_3_0A49:
        push   esi
        call   sub_3_036D
        add    esp, 4
        push   esi
        call   sub_3_0493
        add    esp, 4
        push   0
        push   esi
        call   sub_6_0020
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj5:0x0000
        mov    dword ptr [eax*4 + 4], 0      ; -> obj5:0x0004
        mov    eax, 1
        pop    esi
        ret    
loc_3_0A7D:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_3_0A9B
        mov    eax, dword ptr [edx + 0xf4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [edx + 0x40]
        mov    dword ptr [ecx + 0x30], eax
loc_3_0A9B:
        mov    dword ptr [edx + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0AA8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        or     dword ptr [ecx + 0xe0], eax
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_3_0AC1
        mov    dword ptr [ecx + 0x30], eax
loc_3_0AC1:
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0ACE:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_3_0AE2
        mov    ecx, dword ptr [edx + 0xe0]
        mov    dword ptr [eax], ecx
loc_3_0AE2:
        mov    eax, dword ptr [esp + 8]
        xor    dword ptr [edx + 0xe0], eax
        mov    dword ptr [edx + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0AF9:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xc4], eax
        mov    ecx, dword ptr [esp + 0xc]
        or     eax, eax
        mov    dword ptr [edx + 0xd4], ecx
        jne    loc_3_0B1E
        and    dword ptr [edx + 0x150], 0xffffffbf
        jmp    loc_3_0B25
loc_3_0B1E:
        or     dword ptr [edx + 0x150], 0x40
loc_3_0B25:
        mov    dword ptr [edx + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0B32:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, -1
        je     loc_3_0B6E
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_3_0B6E
        or     dword ptr [esi + 0x150], 2
        mov    dword ptr [esi + 0xec], eax
        mov    dword ptr [esi + 0xc8], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xd8], eax
        mov    dword ptr [esi + 0xc], 0
        jmp    loc_3_0B8A
loc_3_0B6E:
        and    dword ptr [esi + 0x150], 0xfffffffd
        push   dword ptr [esi + 0x13c]
        call   sub_6_005C
        mov    dword ptr [esi + 0x13c], 0
loc_3_0B8A:
        mov    eax, 1
        pop    esi
        ret    
loc_3_0B91:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0xb8], 0
        jne    loc_3_0BA8
        sub    eax, eax
        jmp    loc_3_0C94
loc_3_0BA8:
        mov    eax, dword ptr [esi + 0x140]
        or     eax, eax
        je     loc_3_0BC2
        push   eax
        call   sub_6_005C
        mov    dword ptr [esi + 0x140], 0
loc_3_0BC2:
        mov    edi, dword ptr [esp + 0x14]
        cmp    edi, -1
        je     loc_3_0C88
        or     edi, edi
        je     loc_3_0C88
        mov    ebx, dword ptr [esp + 0x18]
        or     ebx, ebx
        je     loc_3_0C88
        or     dword ptr [esi + 0x150], 1
        mov    dword ptr [esi + 0xf0], edi
        cmp    dword ptr [esi + 0x4c], 0
        jne    loc_3_0C00
        mov    dword ptr [esi + 0x88], 0
loc_3_0C00:
        cmp    byte ptr [esi + 0x16d], 0
        jne    loc_3_0C29
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, eax
        je     loc_3_0C29
        push   0
        push   eax
        call   sub_6_0020
        add    esp, 8
        mov    dword ptr [esi + 0x3c], 0
        mov    byte ptr [esi + 0x16d], 1
loc_3_0C29:
        push   1
        push   edi
        call   sub_6_0018
        add    esp, 8
        mov    dword ptr [esi + 0x3c], eax
        or     eax, eax
        jne    loc_3_0C46
        and    dword ptr [esi + 0x150], 0xfffffffe
        sub    eax, eax
        jmp    loc_3_0C94
loc_3_0C46:
        mov    dword ptr [esi + 0x40], edi
        mov    byte ptr [esi + 0x16d], 0
        mov    dword ptr [esi + 0x4c], 0
        mov    dword ptr [esi + 0x50], 0
        mov    dword ptr [esi + 0x54], 0
        mov    dword ptr [esi + 0xcc], ebx
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0xdc], eax
        push   esi
        call   dword ptr [esi + 0xb8]
        add    esp, 4
        mov    dword ptr [esi + 0xc], 0
        jmp    loc_3_0C8F
loc_3_0C88:
        and    dword ptr [esi + 0x150], 0xfffffffe
loc_3_0C8F:
        mov    eax, 1
loc_3_0C94:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0C98:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, 1
        ret    
loc_3_0CA9:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], 0x30
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_3_0CCB
        push   eax
        push   ecx
        call   sub_3_0C98
        add    esp, 8
        jmp    loc_3_0CD4
loc_3_0CCB:
        mov    dword ptr [ecx + 0xc], 0xffffffeb
        sub    eax, eax
loc_3_0CD4:
        ret    
loc_3_0CD5:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [esp + 0xc], 0x30
        je     loc_3_0CEB
        mov    dword ptr [ecx + 0xc], 0xffffffeb
        sub    eax, eax
        jmp    loc_3_0CFA
loc_3_0CEB:
        push   -1
        push   dword ptr [esp + 0xc]
        push   ecx
        call   sub_1_28CA
        add    esp, 0xc
loc_3_0CFA:
        ret    
        db  000h, 000h, 000h, 000h, 000h
loc_3_0D00:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_3_0577
        ret    
        db  000h, 000h, 000h
sub_3_0D10:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_3_0D1E:
        xor    eax, eax
        jecxz  loc_3_0D37
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        inc    edi
        inc    esi
        dec    ecx
        or     al, 0x20
        or     ah, 0x20
        cmp    al, ah
        je     loc_3_0D1E
        mov    eax, 1
loc_3_0D37:
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h
sub_3_0D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x14], 0x398
        mov    dword ptr [ebp - 0x10], 0x26e
        mov    dword ptr [ebp - 0xc], 0x15c
        mov    dword ptr [ebp - 8], 0x2e
        pushf  
        cli    
        sub    bl, bl
loc_3_0D69:
        movzx  eax, bl
        mov    esi, dword ptr [ebp + eax*4 - 0x14]
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        push   0xff
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        cmp    al, 7
        je     loc_3_0D9F
        cmp    al, 0xf
        je     loc_3_0D9F
        cmp    al, 0x88
        jne    loc_3_0DBA
loc_3_0D9F:
        push   0xf0
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        or     al, al
        je     loc_3_0DD7
loc_3_0DBA:
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        inc    bl
        cmp    bl, 2
        jb     loc_3_0D69
        popf   
        sub    eax, eax
loc_3_0DD1:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0DD7:
        push   dword ptr [ebp - 1]
        movzx  eax, bl
        push   dword ptr [ebp + eax*4 - 0x14]
        call   sub_1_3F2E
        add    esp, 8
        popf   
        mov    eax, 1
        jmp    loc_3_0DD1
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E00:
        push   ebp
        mov    ebp, esp
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x24  RW-
        db  06Ch, 070h, 074h, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 053h, 068h, 061h, 072h, 069h, 06Eh
        db  067h, 000h, 000h, 000h, 045h, 043h, 050h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x10  RW-
        db  0CDh, 020h, 019h, 080h, 02Bh, 000h, 000h, 000h, 0CDh, 020h, 021h, 080h, 02Bh, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x8c  R-X
sub_6_0000:
        VxDCall  BIOS.Service_8019h              ; cd2019802b00
        add    byte ptr [eax], al
sub_6_0008:
        VxDCall  BIOS.Service_8021h              ; cd2021802b00
        add    byte ptr [eax], al
sub_6_0010:
        VxDCall  BIOS.Service_8022h              ; cd2022802b00
        add    byte ptr [eax], al
sub_6_0018:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_6_0020:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_6_0028:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        ret    8
        db  000h, 000h, 000h
sub_6_0040:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_6_005C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_6_006C:
        VMMCall  VMM._PageFree                   ; cd2055800100
        add    byte ptr [eax], al
sub_6_0074:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_6_007C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_6_0084:
        VxDCall  Dev_2033.Service_803Eh          ; cd203e803320
        dec    esp
        push   eax
VxD__LTEXT ENDS
