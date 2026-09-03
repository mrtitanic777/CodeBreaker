; SMC8000W -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 9075 instructions recovered by recursive descent

Declare_Virtual_Device SMC8000W, 3, 10, SMC8000W_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3628  R-X
        db  055h, 033h, 0C9h, 08Bh, 0ECh, 056h, 08Bh, 045h, 008h, 08Bh, 090h, 05Bh, 001h, 000h, 000h, 085h
        db  0D2h, 074h, 010h, 08Bh, 072h, 018h, 03Bh, 0CEh, 073h, 002h, 08Bh, 0CEh, 08Bh, 052h, 00Ch, 085h
        db  0D2h, 075h, 0F0h, 085h, 0C9h, 075h, 005h, 0B9h, 0EEh, 000h, 000h, 000h, 050h, 089h, 088h, 0F7h
        db  001h, 000h, 000h, 083h, 0C1h, 01Dh, 0C1h, 0E9h, 004h, 066h, 089h, 048h, 03Ch, 0E8h
        dd  sub_2_1B80               ; fixup
        db  05Eh, 05Dh, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0050:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_006E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_006E:
        push   0
        inc    dword ptr [esi + 0x15f]
        inc    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [esi + 0x147]
        push   eax
        push   ecx
        call   sub_2_2ED0
        mov    edi, eax
        test   edi, edi
        jne    loc_1_00AE
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_00A6
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_00D4
loc_1_00A6:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_00D4
loc_1_00AE:
        cmp    edi, 0x103
        jne    loc_1_00C0
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_00D4
loc_1_00C0:
        cmp    edi, 0xc001000e
        jne    loc_1_00D1
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_00D1:
        dec    dword ptr [ebx + 0x1c]
loc_1_00D4:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_0147
        mov    eax, dword ptr [esi + 0x15b]
        cmp    eax, ebx
        jne    loc_1_00EF
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x15b], eax
        jmp    loc_1_0102
loc_1_00EF:
        cmp    dword ptr [eax + 0xc], ebx
        je     loc_1_00FC
loc_1_00F4:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_1_00F4
loc_1_00FC:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0102:
        mov    eax, dword ptr [esi + 0x1f7]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_0113
        push   esi
        call   0
loc_1_0113:
        cmp    dword ptr [esi + 0x15b], 0
        jne    loc_1_014C
        push   esi
        lea    eax, [esi + 0xd5]
        push   0                             ; -> loc_1_1900
        push   eax
        call   sub_1_1C62
        test   al, al
        jne    loc_1_014C
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        jmp    loc_1_014C
loc_1_0147:
        mov    edi, 0x103
loc_1_014C:
        mov    eax, dword ptr [esi + 0x15f]
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        jne    loc_1_0194
        cmp    byte ptr [esi + 0x1ed], 0
        je     loc_1_016F
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        je     loc_1_0183
loc_1_016F:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_0194
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_1_0194
loc_1_0183:
        push   0
        add    esi, 0x1fb
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0194:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_01A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x15f]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_1_01C7
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_0209
loc_1_01C7:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_1_01DE
        push   eax
        push   edi
        push   esi
        call   sub_1_0680
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0209
loc_1_01DE:
        cmp    ecx, 1
        jne    loc_1_0202
        cmp    byte ptr [esi + 0x1ec], 0
        je     loc_1_01F5
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_0209
loc_1_01F5:
        push   eax
        push   edi
        push   esi
        call   sub_1_06D0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0209
loc_1_0202:
        mov    dword ptr [ebp - 4], 0x10001
loc_1_0209:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_0279
        mov    eax, dword ptr [ebx + 0x15b]
        cmp    edi, eax
        jne    loc_1_0226
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x15b], eax
        jmp    loc_1_0239
loc_1_0226:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_0233
loc_1_022B:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_022B
loc_1_0233:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0239:
        mov    eax, dword ptr [ebx + 0x1f7]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_024A
        push   ebx
        call   0
loc_1_024A:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 0x15b], 0
        jne    loc_1_0279
        push   ebx
        push   0                             ; -> loc_1_1900
        add    ebx, 0xd5
        push   ebx
        call   sub_1_1C62
loc_1_0279:
        mov    eax, dword ptr [esi + 0x15f]
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        jne    loc_1_02C1
        cmp    byte ptr [esi + 0x1ed], 0
        je     loc_1_029C
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        je     loc_1_02B0
loc_1_029C:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_02C1
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_1_02C1
loc_1_02B0:
        push   0
        add    esi, 0x1fb
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_02C1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_02D0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        mov    dword ptr [ebp - 0x18], eax
        push   esi
        mov    dword ptr [ebp - 0xc], eax
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010101
        ja     loc_1_030D
        je     loc_1_04C4
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_032E
        jmp    dword ptr [eax*4]             ; -> obj1:0x0630
loc_1_030D:
        cmp    eax, 0x1010102
        je     loc_1_04E3
        cmp    eax, 0x1010103
        je     loc_1_0502
        cmp    eax, 0x1010104
        je     loc_1_05D9
loc_1_032E:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_05E5
loc_1_033A:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_034F
        mov    esi, 0                        ; -> loc_1_3470
        mov    edi, 0x5c
        jmp    loc_1_05E5
loc_1_034F:
        mov    esi, 0                        ; -> loc_1_33F4
        mov    edi, 0x7c
        jmp    loc_1_05E5
loc_1_035E:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 4
        cmp    byte ptr [ebx + 0x1ec], 0
        jne    loc_1_0378
        mov    dword ptr [ebp - 0x14], 0
loc_1_0378:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_05E5
loc_1_0385:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_05E5
loc_1_0392:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_05E5
loc_1_039E:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_05E5
loc_1_03AA:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_05E5
loc_1_03B6:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x3a]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xcd]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_05E5
loc_1_03D3:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x16]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0x3a]
        xor    eax, eax
        shl    ecx, 0xa
        mov    ax, word ptr [ebx + 0xcd]
        imul   edx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_05E5
loc_1_03FB:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x38]
        jmp    loc_1_05E2
loc_1_0409:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x38]
        jmp    loc_1_05E2
loc_1_0417:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x2a
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_2050
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_1_05E5
loc_1_043D:
        mov    esi, 0                        ; -> obj1:0x359C
        mov    edi, 0xd
        jmp    loc_1_05E5
loc_1_044C:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_0466
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x147]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_05E5
loc_1_0466:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_05E5
loc_1_0477:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_048B
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x1f7]
        jmp    loc_1_05E2
loc_1_048B:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_05E5
loc_1_0499:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_05E5
loc_1_04AC:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_05E5
loc_1_04B8:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_05E5
loc_1_04C4:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x2a
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_2050
        jmp    loc_1_05E5
loc_1_04E3:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x24
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_2050
        jmp    loc_1_05E5
loc_1_0502:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x5c], 0
        je     loc_1_0517
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_05E5
loc_1_0517:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_0574
        mov    ecx, dword ptr [ebx + 0x147]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 0x1c], edx
        jae    loc_1_0549
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_1_05E5
loc_1_0549:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_33F0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_1_05EB
        jmp    loc_1_05E5
loc_1_0574:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x147]
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        push   edx
        call   sub_2_32F0
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_05A7
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_1_05E5
loc_1_05A7:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        mov    edx, dword ptr [ebx + 0x147]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_3330
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_05E5
loc_1_05D9:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x163]
loc_1_05E2:
        mov    dword ptr [ebp - 8], eax
loc_1_05E5:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0624
loc_1_05EB:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_0624
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_0614
        sub    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_0614:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_2050
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_0624:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_033A
        dd  loc_1_035E
        dd  loc_1_0385
        dd  loc_1_0385
        dd  loc_1_0392
        dd  loc_1_039E
        dd  loc_1_03AA
        dd  loc_1_03B6
        dd  loc_1_03D3
        dd  loc_1_03FB
        dd  loc_1_0409
        dd  loc_1_0417
        dd  loc_1_043D
        dd  loc_1_044C
        dd  loc_1_0477
        dd  loc_1_0499
        dd  loc_1_04AC
        dd  loc_1_032E
        dd  loc_1_04B8
        db  08Dh, 064h, 024h, 000h
sub_1_0680:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x18]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_02D0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_06D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_1_070D
        je     loc_1_081C
        cmp    eax, 0x1010e
        je     loc_1_071F
        cmp    eax, 0x1010f
        je     loc_1_077A
        cmp    eax, 0x10112
        je     loc_1_07F5
loc_1_070D:
        mov    ebx, 0xc00000bb
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0857
loc_1_071F:
        cmp    edi, 4
        je     loc_1_0736
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0857
loc_1_0736:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_2050
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_0763
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_1_0857
loc_1_0763:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_0870
        jmp    loc_1_0855
loc_1_077A:
        cmp    edi, 4
        je     loc_1_0791
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0857
loc_1_0791:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_2050
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_07EE
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x1f7]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_07C8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x1f7], eax
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_0857
loc_1_07C8:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_1_07E6
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_07E6
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x18], eax
        call   0
        jmp    loc_1_0857
loc_1_07E6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_0857
loc_1_07EE:
        mov    ebx, 0xc0010014
        jmp    loc_1_0857
loc_1_07F5:
        cmp    edi, 4
        je     loc_1_0809
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0857
loc_1_0809:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_2050
        jmp    loc_1_0857
loc_1_081C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_1_083A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0857
loc_1_083A:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        call   sub_1_08B0
loc_1_0855:
        mov    ebx, eax
loc_1_0857:
        test   ebx, ebx
        jne    loc_1_0865
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_0865:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_0870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_08A5
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x147]
        push   ecx
        call   sub_2_3270
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_1_08A5:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_1_08B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_08E9
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    edx, dword ptr [ecx + 0x147]
        push   edx
        call   sub_2_2F90
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_08E9:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0900:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        push   ecx
        mov    dword ptr [ebp - 0x14], eax
        push   edx
        mov    ecx, dword ptr [esi + 0x14]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   1
        push   ecx
        push   0
        push   ebx
        call   sub_1_02D0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_1_09E0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_1_0970
        je     loc_1_09AF
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_097E
        jmp    dword ptr [eax*4]             ; -> obj1:0x09F8
loc_1_0970:
        cmp    eax, 0x1020102
        je     loc_1_09B7
        cmp    eax, 0x1020103
        je     loc_1_09BF
loc_1_097E:
        mov    dword ptr [ebp - 8], 0xc00000bb
        jmp    loc_1_09C8
loc_1_0987:
        mov    eax, dword ptr [ebx + 0x18c]
        jmp    loc_1_09C5
loc_1_098F:
        mov    eax, dword ptr [ebx + 0x190]
        jmp    loc_1_09C5
loc_1_0997:
        mov    eax, dword ptr [ebx + 0x194]
        jmp    loc_1_09C5
loc_1_099F:
        mov    eax, dword ptr [ebx + 0x1b4]
        jmp    loc_1_09C5
loc_1_09A7:
        mov    eax, dword ptr [ebx + 0x1b8]
        jmp    loc_1_09C5
loc_1_09AF:
        mov    eax, dword ptr [ebx + 0x1b0]
        jmp    loc_1_09C5
loc_1_09B7:
        mov    eax, dword ptr [ebx + 0x198]
        jmp    loc_1_09C5
loc_1_09BF:
        mov    eax, dword ptr [ebx + 0x19c]
loc_1_09C5:
        mov    dword ptr [ebp - 4], eax
loc_1_09C8:
        cmp    edi, 4
        jb     loc_1_09E0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_2050
        add    dword ptr [ebp - 0xc], 4
loc_1_09E0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        pop    edi
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_0987
        dd  loc_1_098F
        dd  loc_1_0997
        dd  loc_1_099F
        dd  loc_1_09A7
        db  08Dh, 064h, 024h, 000h
loc_1_0A10:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_0A37
        je     loc_1_0A68
        cmp    eax, 0x10101
        jb     loc_1_0A63
        cmp    eax, 0x10111
        jbe    loc_1_0A68
        jmp    loc_1_0A63
loc_1_0A37:
        cmp    eax, 0x1010104
        ja     loc_1_0A55
        cmp    eax, 0x1010101
        jae    loc_1_0A68
        cmp    eax, 0x20101
        jb     loc_1_0A63
        cmp    eax, 0x20105
        jbe    loc_1_0A68
        jmp    loc_1_0A63
loc_1_0A55:
        cmp    eax, 0x1020101
        jb     loc_1_0A63
        cmp    eax, 0x1020103
        jbe    loc_1_0A68
loc_1_0A63:
        mov    esi, 0xc00000bb
loc_1_0A68:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x15f]
        test   esi, esi
        jne    loc_1_0A7E
        push   ecx
        push   edi
        call   sub_1_0900
        mov    esi, eax
loc_1_0A7E:
        mov    eax, dword ptr [edi + 0x15f]
        dec    eax
        mov    dword ptr [edi + 0x15f], eax
        jne    loc_1_0AC6
        cmp    byte ptr [edi + 0x1ed], 0
        je     loc_1_0AA1
        lea    eax, [edi + 0x307]
        cmp    byte ptr [eax], 0
        je     loc_1_0AB5
loc_1_0AA1:
        cmp    dword ptr [edi + 0x177], 0
        je     loc_1_0AC6
        lea    eax, [edi + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_1_0AC6
loc_1_0AB5:
        push   0
        add    edi, 0x1fb
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0AC6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1B70
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x30e]
        push   eax
        mov    edi, 0x300
        push   ecx
        call   sub_1_1C50
loc_1_0AF6:
        push   0x2710
        call   sub_1_1C68
        dec    edi
        jne    loc_1_0AF6
        mov    byte ptr [esi + 0x309], 1
        cmp    esi, dword ptr [0x42]         ; -> obj1:0x33E6
        jne    loc_1_0B1F
        mov    eax, dword ptr [esi + 0x157]
        mov    dword ptr [0x42], eax         ; -> obj1:0x33E6
        jmp    loc_1_0B4D
loc_1_0B1F:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x33E6
        cmp    dword ptr [ecx + 0x157], esi
        je     loc_1_0B3B
loc_1_0B2D:
        mov    ecx, dword ptr [ecx + 0x157]
        cmp    dword ptr [ecx + 0x157], esi
        jne    loc_1_0B2D
loc_1_0B3B:
        mov    eax, dword ptr [ecx + 0x157]
        mov    edx, dword ptr [eax + 0x157]
        mov    dword ptr [ecx + 0x157], edx
loc_1_0B4D:
        lea    eax, [esi + 0xd5]
        push   eax
        call   sub_1_1C5C
        mov    eax, dword ptr [esi + 0x147]
        push   eax
        call   sub_2_2DB0
        mov    eax, dword ptr [esi + 0x52]
        push   eax
        call   sub_1_1BA0
        push   0
        push   0x31b
        push   esi
        call   sub_1_1C74
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B90:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x33A4
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_1B94
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x33A8
        push   ecx
        call   sub_1_1B88
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 8]
        mov    esi, dword ptr [ebx + 8]
        push   ecx
        lea    eax, [edi + 0x24]
        push   0
        push   0
        mov    dword ptr [ebp - 4], eax
        push   0
        push   edi
        call   sub_1_1C98
        cmp    dword ptr [ebp - 8], 0xc
        jb     loc_1_0DBB
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_0DBB
        cmp    byte ptr [esi + 0x1ec], 0
        je     loc_1_0C16
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C16:
        cmp    byte ptr [esi + 0x1ee], 0
        je     loc_1_0C2D
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C2D:
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_0C41
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C41:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 0x1c]
        push   esi
        inc    dword ptr [esi + 0x15f]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_0E10
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     loc_1_0C95
        cmp    dword ptr [esi + 0x177], 0
        jne    loc_1_0C81
        mov    dword ptr [esi + 0x17b], edi
        mov    dword ptr [esi + 0x177], edi
        jmp    loc_1_0D1F
loc_1_0C81:
        mov    eax, dword ptr [esi + 0x17b]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x17b], edi
        jmp    loc_1_0D1F
loc_1_0C95:
        cmp    dword ptr [esi + 0x167], 0
        je     loc_1_0CA9
        mov    eax, dword ptr [esi + 0x16b]
        mov    dword ptr [eax + 0x24], edi
        jmp    loc_1_0D12
loc_1_0CA9:
        cmp    dword ptr [esi + 0x16f], 0
        jne    loc_1_0CC2
        xor    ebx, ebx
        mov    dword ptr [esi + 0x173], edi
        mov    dword ptr [esi + 0x16f], edi
        jmp    loc_1_0CD1
loc_1_0CC2:
        mov    ebx, dword ptr [esi + 0x173]
        mov    dword ptr [ebx + 0x24], edi
        mov    dword ptr [esi + 0x173], edi
loc_1_0CD1:
        mov    byte ptr [esi + 0x31a], 0
        push   esi
        push   edi
        call   sub_2_2560
        cmp    ax, 6
        jne    loc_1_0D1F
        test   ebx, ebx
        jne    loc_1_0CFF
        mov    dword ptr [esi + 0x173], 0
        mov    dword ptr [esi + 0x16f], 0
        jmp    loc_1_0D0C
loc_1_0CFF:
        mov    dword ptr [esi + 0x173], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_0D0C:
        mov    dword ptr [esi + 0x167], edi
loc_1_0D12:
        mov    dword ptr [esi + 0x16b], edi
        mov    byte ptr [esi + 0x31a], 0
loc_1_0D1F:
        mov    edi, 0x103
        mov    eax, dword ptr [esi + 0x15f]
        test   byte ptr [esi + 0xc5], 8
        je     loc_1_0D6E
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        cmp    byte ptr [esi + 0x307], 0
        jne    loc_1_0DB0
        test   eax, eax
        jne    loc_1_0DB0
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_0DB0
        mov    byte ptr [esi + 0x307], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_1180
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0D6E:
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        jne    loc_1_0DB0
        cmp    byte ptr [esi + 0x1ed], 0
        je     loc_1_0D8B
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        je     loc_1_0D9F
loc_1_0D8B:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_0DB0
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_1_0DB0
loc_1_0D9F:
        push   0
        add    esi, 0x1fb
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0DB0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DBB:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DD0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     loc_1_0DF2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_0DE5:
        mov    bl, byte ptr [ecx + esi]
        cmp    byte ptr [esi + eax], bl
        jne    loc_1_0DFA
        inc    esi
        cmp    esi, edx
        jb     loc_1_0DE5
loc_1_0DF2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_0DFA:
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E10:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        lea    esi, [eax + 0x24]
        push   6
        push   0
        lea    ecx, [ebp - 8]
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0
        push   eax
        push   ecx
        call   sub_1_1700
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0x147]
        cmp    byte ptr [ecx + 0x28], dl
        mov    ebx, dword ptr [eax + 0xc]
        jne    loc_1_0E63
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 0x24
        push   ecx
        call   sub_1_0DD0
        test   eax, eax
        jne    loc_1_0E63
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_0E63:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x71], 2
        je     loc_1_0E78
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0E78:
        test   bl, 0x20
        je     loc_1_0E89
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0E89:
        mov    al, byte ptr [4]              ; -> obj1:0x339C
        cmp    byte ptr [ebp - 4], al
        jne    loc_1_0EB5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj1:0x3398
        call   sub_1_0DD0
        test   eax, eax
        jne    loc_1_0EB5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0EB5:
        test   byte ptr [ebp - 8], 1
        je     loc_1_0EC9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0EC9:
        cmp    byte ptr [esi + 9], 0
        je     loc_1_0EDB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0EDB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0EF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_0F09
        mov    eax, 0xc0010002
        pop    ebp
        ret    4
loc_1_0F09:
        cmp    byte ptr [ecx + 0x1ed], 0
        je     loc_1_0F18
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_0F18:
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [ecx + 0x15f]
        mov    byte ptr [ecx + 0x1ed], 1
        mov    dword ptr [ecx + 0x1ef], eax
        mov    eax, dword ptr [ecx + 0x15f]
        dec    eax
        mov    dword ptr [ecx + 0x15f], eax
        jne    loc_1_0F76
        cmp    byte ptr [ecx + 0x1ed], 0
        je     loc_1_0F51
        lea    eax, [ecx + 0x307]
        cmp    byte ptr [eax], 0
        je     loc_1_0F65
loc_1_0F51:
        cmp    dword ptr [ecx + 0x177], 0
        je     loc_1_0F76
        lea    eax, [ecx + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_1_0F76
loc_1_0F65:
        push   0
        add    ecx, 0x1fb
        push   ecx
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0F76:
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  090h
loc_1_0F80:
        xor    eax, eax
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F90:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_2_1FC0
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0FD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_1D90
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x15f]
loc_1_1000:
        cmp    byte ptr [esi + 0x307], 0
        jne    loc_1_101E
loc_1_1009:
        mov    byte ptr [esi + 0x307], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_1180
        mov    bl, al
loc_1_101E:
        cmp    byte ptr [esi + 0x308], 0
        jne    loc_1_104A
        mov    byte ptr [esi + 0x308], 1
        push   esi
        call   sub_1_16C0
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_1046
        cmp    byte ptr [esi + 0x307], 0
        je     loc_1_1009
loc_1_1046:
        test   bl, bl
        jne    loc_1_1000
loc_1_104A:
        mov    eax, dword ptr [esi + 0x15f]
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        jne    loc_1_1092
        cmp    byte ptr [esi + 0x1ed], 0
        je     loc_1_106B
        cmp    byte ptr [esi + 0x307], 0
        je     loc_1_107D
loc_1_106B:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_1092
        cmp    byte ptr [esi + 0x307], 0
        jne    loc_1_1092
loc_1_107D:
        push   0
        lea    eax, [esi + 0x1fb]
        mov    byte ptr [esi + 0x307], 1
        push   eax
        call   sub_1_1C4A
loc_1_1092:
        cmp    byte ptr [esi + 0x146], 0
        je     loc_1_10AD
        push   esi
        push   0                             ; -> sub_2_1D30
        add    esi, 0xd5
        push   esi
        call   sub_1_1C62
loc_1_10AD:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    dword ptr [esi + 0x17f], ebx
        mov    byte ptr [esi + 0x183], 1
        push   0
        push   ebx
        call   sub_1_1C98
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_116B
        mov    edi, dword ptr [esi + 0x1f7]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_1104
        mov    edi, eax
loc_1_1104:
        push   edi
        lea    eax, [esi + 0x20b]
        push   0
        push   ebx
        push   eax
        call   sub_1_1700
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_1142
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 0x20b]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0x147]
        push   ecx
        push   esi
        push   eax
        call   sub_1_1D60
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1142:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0xe
        sub    eax, 0xe
        lea    ecx, [esi + 0x219]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 0x20b]
        push   0xe
        mov    ecx, dword ptr [esi + 0x147]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_1D60
loc_1_116B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        inc    dword ptr [esi + 0x15f]
        cmp    byte ptr [esi + 0x146], 0
        je     loc_1_11AA
        push   0                             ; -> sub_2_1D90
        lea    eax, [esi + 0xd5]
        push   eax
        jmp    loc_1_11BC
loc_1_11AA:
        call   sub_2_1DF0
        lea    ecx, [esi + 0xd5]
        push   esi
        push   0                             ; -> sub_2_1D30
        push   ecx
loc_1_11BC:
        call   sub_1_1C62
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_1287
loc_1_11CE:
        mov    edi, dword ptr [esi + 0x177]
        cmp    dword ptr [esi + 0x17b], edi
        jne    loc_1_11E6
        mov    dword ptr [esi + 0x17b], 0
loc_1_11E6:
        mov    eax, dword ptr [edi + 0x24]
        push   edi
        push   esi
        mov    dword ptr [esi + 0x177], eax
        call   sub_1_10C0
        push   0
        inc    dword ptr [esi + 0x18c]
        mov    ebx, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 0x1c]
        jne    loc_1_127A
        mov    eax, dword ptr [edi + 0x15b]
        cmp    ebx, eax
        jne    loc_1_1227
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x15b], eax
        jmp    loc_1_123A
loc_1_1227:
        cmp    dword ptr [eax + 0xc], ebx
        je     loc_1_1234
loc_1_122C:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_1_122C
loc_1_1234:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_123A:
        mov    eax, dword ptr [edi + 0x1f7]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_124B
        push   edi
        call   0
loc_1_124B:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   ebx
        call   sub_1_1C74
        cmp    dword ptr [edi + 0x15b], 0
        jne    loc_1_127A
        push   edi
        push   0                             ; -> loc_1_1900
        add    edi, 0xd5
        push   edi
        call   sub_1_1C62
loc_1_127A:
        cmp    dword ptr [esi + 0x177], 0
        jne    loc_1_11CE
loc_1_1287:
        push   esi
        call   sub_2_2080
        sub    ax, 0x12
        cmp    ax, 1
        sbb    al, al
        neg    al
        cmp    byte ptr [esi + 0x183], 0
        mov    byte ptr [ebp - 4], al
        je     loc_1_12B8
        mov    byte ptr [esi + 0x183], 0
        mov    eax, dword ptr [esi + 0x147]
        push   eax
        call   sub_1_1F40
loc_1_12B8:
        cmp    byte ptr [esi + 0x1ed], 0
        je     loc_1_167A
        cmp    dword ptr [esi + 0x15f], 1
        jne    loc_1_167A
        push   esi
        lea    eax, [esi + 0xd5]
        mov    byte ptr [esi + 0x1ee], 1
        push   0                             ; -> sub_2_1D90
        push   eax
        call   sub_1_1C62
        mov    edi, dword ptr [esi + 0x15b]
        test   edi, edi
        je     loc_1_1388
loc_1_12F9:
        push   0
        inc    dword ptr [edi + 0x1c]
        push   0
        mov    eax, dword ptr [edi]
        push   0x40010004
        push   eax
        call   sub_1_1BD6
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_137D
        mov    eax, dword ptr [ebx + 0x15b]
        cmp    eax, edi
        jne    loc_1_132A
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x15b], eax
        jmp    loc_1_133D
loc_1_132A:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1337
loc_1_132F:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_132F
loc_1_1337:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_133D:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x1f7], eax
        jne    loc_1_134E
        push   ebx
        call   0
loc_1_134E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 0x15b], 0
        jne    loc_1_137D
        push   ebx
        push   0                             ; -> loc_1_1900
        add    ebx, 0xd5
        push   ebx
        call   sub_1_1C62
loc_1_137D:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_12F9
loc_1_1388:
        push   esi
        call   sub_2_1BB0
        test   ax, ax
        je     loc_1_13AF
        mov    byte ptr [esi + 0x1ec], 1
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        jmp    loc_1_13B6
loc_1_13AF:
        mov    byte ptr [esi + 0x1ec], 0
loc_1_13B6:
        cmp    dword ptr [esi + 0x16f], 0
        je     loc_1_13EE
        mov    eax, dword ptr [esi + 0x167]
        mov    ecx, dword ptr [esi + 0x173]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x16f]
        mov    dword ptr [esi + 0x167], edx
        mov    dword ptr [esi + 0x173], 0
        mov    dword ptr [esi + 0x16f], 0
loc_1_13EE:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_1_1412
        mov    eax, dword ptr [esi + 0x167]
        mov    ecx, dword ptr [esi + 0x17b]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x177]
        mov    dword ptr [esi + 0x167], edx
loc_1_1412:
        mov    dword ptr [esi + 0x17b], 0
        mov    dword ptr [esi + 0x177], 0
        cmp    dword ptr [esi + 0x167], 0
        je     loc_1_14D0
loc_1_1433:
        mov    ecx, dword ptr [esi + 0x167]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x167], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BB2
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_14C3
        mov    eax, dword ptr [ebx + 0x15b]
        cmp    eax, edi
        jne    loc_1_1470
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x15b], eax
        jmp    loc_1_1483
loc_1_1470:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_147D
loc_1_1475:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1475
loc_1_147D:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1483:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x1f7], eax
        jne    loc_1_1494
        push   ebx
        call   0
loc_1_1494:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 0x15b], 0
        jne    loc_1_14C3
        push   ebx
        push   0                             ; -> loc_1_1900
        add    ebx, 0xd5
        push   ebx
        call   sub_1_1C62
loc_1_14C3:
        cmp    dword ptr [esi + 0x167], 0
        jne    loc_1_1433
loc_1_14D0:
        mov    dword ptr [esi + 0x16b], 0
        cmp    byte ptr [esi + 0x1ec], 0
        jne    loc_1_14E9
        push   esi
        call   sub_2_1E90
loc_1_14E9:
        mov    byte ptr [esi + 0x1ee], 0
        mov    edi, dword ptr [esi + 0x15b]
        test   edi, edi
        je     loc_1_15C8
loc_1_14FE:
        inc    dword ptr [edi + 0x1c]
        cmp    byte ptr [esi + 0x1ec], 0
        je     loc_1_151B
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0x40010007
        push   eax
        call   sub_1_1BD6
loc_1_151B:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 0x1ec], 1
        adc    eax, -1
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_1BD6
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BDC
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_15BD
        mov    eax, dword ptr [ebx + 0x15b]
        cmp    eax, edi
        jne    loc_1_156A
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x15b], eax
        jmp    loc_1_157D
loc_1_156A:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1577
loc_1_156F:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_156F
loc_1_1577:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_157D:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x1f7], eax
        jne    loc_1_158E
        push   ebx
        call   0
loc_1_158E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 0x15b], 0
        jne    loc_1_15BD
        push   ebx
        push   0                             ; -> loc_1_1900
        add    ebx, 0xd5
        push   ebx
        call   sub_1_1C62
loc_1_15BD:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_14FE
loc_1_15C8:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 0x1ef]
        cmp    byte ptr [esi + 0x1ec], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0xc0000001
        push   eax
        push   edx
        call   sub_1_1BBE
        mov    ebx, dword ptr [esi + 0x1ef]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 0x1c]
        jne    loc_1_166D
        mov    ecx, dword ptr [edi + 0x15b]
        cmp    ebx, ecx
        jne    loc_1_160E
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x15b], eax
        jmp    loc_1_162D
loc_1_160E:
        mov    eax, dword ptr [esi + 0x1ef]
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_1_1621
loc_1_1619:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_1_1619
loc_1_1621:
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ebx, dword ptr [esi + 0x1ef]
loc_1_162D:
        mov    eax, dword ptr [edi + 0x1f7]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_163E
        push   edi
        call   0
loc_1_163E:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   ebx
        call   sub_1_1C74
        cmp    dword ptr [edi + 0x15b], 0
        jne    loc_1_166D
        push   edi
        push   0                             ; -> loc_1_1900
        add    edi, 0xd5
        push   edi
        call   sub_1_1C62
loc_1_166D:
        mov    byte ptr [esi + 0x1ed], 0
        push   esi
        call   sub_2_1E90
loc_1_167A:
        mov    byte ptr [esi + 0x307], 0
        cmp    byte ptr [esi + 0x146], 0
        push   esi
        jne    loc_1_1691
        call   sub_2_1E50
        push   esi
loc_1_1691:
        push   0                             ; -> sub_2_1D30
        lea    eax, [esi + 0xd5]
        push   eax
        call   sub_1_1C62
        mov    al, byte ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 0x15f]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x1ee], 0
        je     loc_1_16DC
        mov    byte ptr [esi + 0x308], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_16DC:
        mov    byte ptr [esi + 0x31a], 0
        push   esi
        call   sub_2_2390
        mov    byte ptr [esi + 0x308], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        push   esi
        push   ecx
        call   sub_1_1C98
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_1754
loc_1_1725:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_1754
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1725
loc_1_1754:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1765
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1765:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_1774:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_1783
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_1786
loc_1_1783:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1786:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_2050
        cmp    esi, edi
        je     loc_1_17C2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_17CD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        jmp    loc_1_1774
loc_1_17C2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_17CD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_17E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebx + 8]
        add    esi, 0xe
        cmp    dword ptr [edi + 0x17f], 0
        je     loc_1_186B
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_1C98
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1859
loc_1_1819:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 0x17f]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1700
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_1859
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1819
loc_1_1859:
        sub    ebx, esi
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_186B:
        cmp    byte ptr [edi + 0x183], 0
        je     loc_1_18EB
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        push   edx
        call   sub_2_2610
        test   ax, ax
        je     loc_1_18E0
        cmp    ax, 0x13
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], 0
        jne    loc_1_18D2
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 0x105], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 0x109], ecx
        mov    dword ptr [edi + 0x10d], esi
        mov    dword ptr [edi + 0x111], edx
        mov    dword ptr [edi + 0x115], eax
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_18D2:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_18E0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_18EB:
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1900:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_1F40
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1920:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x31a], 0
        je     loc_1_1A29
        cmp    byte ptr [esi + 0xf4], 0
        je     loc_1_1A29
        push   esi
        call   sub_2_1D90
        mov    byte ptr [esi + 0x31a], 0
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        cmp    dword ptr [esi + 0x16f], 0
        je     loc_1_19A1
        xor    edi, edi
loc_1_196E:
        mov    edx, dword ptr [esi + 0x16f]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x16f], eax
        test   eax, eax
        jne    loc_1_1989
        mov    dword ptr [esi + 0x173], edi
loc_1_1989:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x16f], edi
        jne    loc_1_196E
loc_1_19A1:
        cmp    dword ptr [esi + 0x167], 0
        je     loc_1_19DF
        xor    edi, edi
loc_1_19AC:
        mov    edx, dword ptr [esi + 0x167]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x167], eax
        test   eax, eax
        jne    loc_1_19C7
        mov    dword ptr [esi + 0x16b], edi
loc_1_19C7:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x167], edi
        jne    loc_1_19AC
loc_1_19DF:
        mov    byte ptr [esi + 0x31a], 0
        mov    byte ptr [esi + 0xf4], 0
        push   esi
        call   sub_2_1BB0
        test   ax, ax
        je     loc_1_1A14
        mov    byte ptr [esi + 0x1ec], 1
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        jmp    loc_1_1A1B
loc_1_1A14:
        mov    byte ptr [esi + 0x1ec], 0
loc_1_1A1B:
        push   esi
        call   sub_2_1E90
        push   esi
        call   sub_2_1D30
        jmp    loc_1_1A42
loc_1_1A29:
        cmp    dword ptr [esi + 0x16f], 0
        jne    loc_1_1A3B
        cmp    dword ptr [esi + 0x167], 0
        je     loc_1_1A42
loc_1_1A3B:
        mov    byte ptr [esi + 0x31a], 1
loc_1_1A42:
        push   0x1388
        add    esi, 0x30e
        push   esi
        call   sub_1_1C4A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1A60:
        mov    al, byte ptr [0]              ; -> obj1:0x20FC
        cmp    al, 0
        jne    loc_1_1A85
        inc    byte ptr [0]                  ; -> obj1:0x20FC
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_1A87
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_1A87
loc_1_1A85:
        clc    
        ret    
loc_1_1A87:
        stc    
        ret    
SMC8000W_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1A98
        jmp    dword ptr [eax*4]             ; -> obj1:0x1A9A
loc_1_1A98:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_1A60
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A98
        dd  loc_1_1A60
        db  0CCh, 0CCh
sub_1_1B08:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
sub_1_1B1C:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_1B45:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_1B59:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_1B82:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_1B88:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_1B8E:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_1B94:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_1B9A:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_1BA0:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_1BAC:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_1BB2:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_1BBE:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_1BCA:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_1BD0:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_1BD6:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_1BDC:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_1BE8:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_1BF4:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        int3   
        int3   
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_1C4A:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_1C50:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_1C56:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_1C5C:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_1C62:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_1C68:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_1C6E:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_1C74:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_1C98:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_1CBC:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_1CE0:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_1CEC:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_1CF2:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_1CF8:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
sub_1_1D3A:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_1D40:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
sub_1_1D60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1D90
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_1D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_1E30
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_1DD0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_1DC5
        cmp    al, 0xff
        jne    loc_1_1DC5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_1DC5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_1E37
loc_1_1DC5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_1E37
        db  08Bh, 0FFh
loc_1_1DD0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_1E20
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1DF6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_1DFD
loc_1_1DF6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_1DFD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1E10
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1E3A
        db  08Dh, 064h, 024h, 000h
loc_1_1E10:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_1E3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E20:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1E3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E30:
        mov    dword ptr [ebp - 4], 0x20
loc_1_1E37:
        mov    esi, dword ptr [ebp + 8]
loc_1_1E3A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1F31
loc_1_1E45:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_1E9D
        test   al, 2
        je     loc_1_1E89
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_3450
        or     al, al
        je     loc_1_1E89
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_1E9D
loc_1_1E89:
        test   byte ptr [ebp - 4], 2
        je     loc_1_1E94
        test   bl, 4
        jne    loc_1_1E9D
loc_1_1E94:
        test   bl, 0x20
        je     loc_1_1F26
loc_1_1E9D:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_1BCA
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1F26
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1EDF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1EDF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1EF0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1EF6
        db  08Bh, 0FFh
loc_1_1EF0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1EF6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1F05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1BD0
loc_1_1F05:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_1C74
        mov    edi, ebx
        jmp    loc_1_1F29
loc_1_1F26:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1F29:
        or     edi, edi
        jne    loc_1_1E45
loc_1_1F31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1F40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1F50
        pop    ebp
        ret    4
sub_1_1F50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1FCE
loc_1_1F60:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1FC7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1BD0
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1FC7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1F8C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1F8C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1FA0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1FA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FA0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1FA6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_1C74
        mov    edi, ebx
        jmp    loc_1_1FCA
loc_1_1FC7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1FCA:
        or     edi, edi
        jne    loc_1_1F60
loc_1_1FCE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_1FF5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1FF5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_2031
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_2022
        cmp    bl, 0xff
        jne    loc_1_2022
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_2022
        test   al, 8
        je     loc_1_201B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_201B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2022:
        test   al, 4
        jne    loc_1_202A
        test   al, 2
        je     loc_1_2049
loc_1_202A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2031:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_2049
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_2049
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2049:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_2050:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
sub_1_2070:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_2090
        or     ecx, ecx
        je     loc_1_20A6
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_20A4
loc_1_2090:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_20A4
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_20A4:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_20A6:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SMC8000W'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SMC8000W_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 0CCh, 0CCh, 0CCh
sub_1_2100:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0xfa0
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        in     al, dx
        mov    bl, al
        or     al, 0x80
        out    dx, al
        and    bl, 0x7f
        call   sub_1_1C68
        mov    di, word ptr [esi + 4]
        mov    al, bl
        mov    edx, edi
        mov    ebx, dword ptr [esi + 0xc5]
        out    dx, al
        test   bl, 0x18
        je     loc_1_217D
        in     al, dx
        cmp    byte ptr [esi + 0x146], 0
        mov    cl, al
        je     loc_1_2152
        and    cl, 0x3f
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        in     al, dx
        or     al, 0x10
        jmp    loc_1_2191
loc_1_2152:
        test   bh, 8
        je     loc_1_2160
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        and    al, 0xcf
        out    dx, al
loc_1_2160:
        test   ebx, 0x8000000
        jne    loc_1_2174
        and    cl, 0x3f
        mov    edx, edi
        or     cl, 0x40
        mov    al, cl
        jmp    loc_1_2191
loc_1_2174:
        and    cl, 0x3f
        mov    edx, edi
        mov    al, cl
        jmp    loc_1_2191
loc_1_217D:
        mov    eax, dword ptr [esi + 0x18]
        shr    eax, 0xd
        and    al, 0x3f
        test   ebx, 0x8000000
        jne    loc_1_2192
        or     al, 0x40
        mov    edx, edi
loc_1_2191:
        out    dx, al
loc_1_2192:
        xor    cl, cl
        xor    eax, eax
        xor    edx, edx
loc_1_2198:
        cmp    byte ptr [esi + eax + 0x24], dl
        je     loc_1_21A0
        mov    cl, 1
loc_1_21A0:
        inc    eax
        cmp    eax, 6
        jb     loc_1_2198
        xor    edi, edi
loc_1_21A8:
        xor    eax, eax
        inc    edi
        mov    ax, word ptr [esi + 4]
        cmp    edi, 6
        lea    edx, [eax + edi + 7]
        in     al, dx
        mov    byte ptr [esi + edi + 0x29], al
        jb     loc_1_21A8
        test   cl, cl
        jne    loc_1_21D3
        xor    eax, eax
loc_1_21C3:
        mov    dl, byte ptr [esi + eax + 0x2a]
        lea    ecx, [esi + eax]
        inc    eax
        mov    byte ptr [ecx + 0x24], dl
        cmp    eax, 6
        jb     loc_1_21C3
loc_1_21D3:
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_21E9
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
        jmp    loc_1_21F4
loc_1_21E9:
        xor    edx, edx
        mov    al, 0x21
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
loc_1_21F4:
        out    dx, al
        xor    ecx, ecx
        xor    al, al
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x1a]
        out    dx, al
        lea    edx, [ecx + 0x1b]
        out    dx, al
        lea    edx, [ecx + 0x17]
        in     al, dx
        test   al, 0x80
        jne    loc_1_221F
        mov    edi, 0x400
loc_1_2212:
        push   0x3e8
        call   sub_1_1C68
        dec    edi
        jne    loc_1_2212
loc_1_221F:
        mov    ecx, dword ptr [esi + 0xc5]
        mov    eax, ecx
        and    eax, 0x8000000
        cmp    eax, 1
        sbb    al, al
        and    al, 8
        add    al, 0x40
        test   ecx, 0x80008
        je     loc_1_223F
        or     al, 1
loc_1_223F:
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        add    edx, 0x1e
        out    dx, al
        call   sub_2_1FC0
        mov    di, word ptr [esi + 4]
        mov    al, 4
        mov    cl, byte ptr [esi + 0x13a]
        lea    edx, [edi + 0x1d]
        out    dx, al
        lea    edx, [edi + 0x11]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x12]
        mov    al, byte ptr [esi + 0x13b]
        out    dx, al
        lea    edx, [edi + 0x13]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x17]
        mov    al, 0xff
        out    dx, al
        mov    byte ptr [esi + 0xd3], 0x3f
        mov    ebx, dword ptr [esi + 0xc5]
        and    ebx, 0x8000000
        je     loc_1_229F
        test   byte ptr [esi + 0x49], 4
        jne    loc_1_229F
        mov    byte ptr [esi + 0xd3], 0x7f
loc_1_229F:
        lea    edx, [edi + 0x1f]
        mov    al, byte ptr [esi + 0xd3]
        out    dx, al
        lea    edx, [edi + 0x10]
        test   ebx, ebx
        je     loc_1_22B4
        mov    al, 0x41
        jmp    loc_1_22B6
loc_1_22B4:
        mov    al, 0x61
loc_1_22B6:
        out    dx, al
        xor    ebx, ebx
loc_1_22B9:
        lea    edx, [edi + ebx + 0x11]
        mov    al, byte ptr [esi + ebx + 0x24]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jb     loc_1_22B9
        lea    eax, [ecx + 1]
        lea    edx, [edi + 0x17]
        out    dx, al
        xor    ecx, ecx
        and    eax, 0xff
        mov    cx, word ptr [esi + 0x38]
        imul   ecx, eax
        add    ecx, dword ptr [esi + 0x1c]
        cmp    byte ptr [esi + 0x146], 0
        mov    dword ptr [esi + 0x13c], ecx
        je     loc_1_231E
        lea    ecx, [edi + 7]
        xor    al, al
        mov    edx, ecx
        out    dx, al
        out    dx, al
        mov    ax, word ptr [esi + 0x20]
        test   al, 0x7f
        jbe    loc_1_2316
        shl    ax, 9
        lea    edx, [edi + 2]
        mov    edi, eax
        and    edi, 0xffff
loc_1_230F:
        xor    eax, eax
        dec    edi
        out    dx, ax
        jne    loc_1_230F
loc_1_2316:
        mov    edx, ecx
        xor    al, al
        out    dx, al
        out    dx, al
        jmp    loc_1_2346
loc_1_231E:
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        test   eax, 0x3fffff
        jbe    loc_1_2346
loc_1_232D:
        mov    eax, dword ptr [esi + 0x1c]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        mov    ax, word ptr [esi + 0x20]
        and    eax, 0xffff
        shl    eax, 8
        cmp    eax, ecx
        ja     loc_1_232D
loc_1_2346:
        mov    edi, dword ptr [esi + 0xc5]
        mov    ebx, edi
        and    ebx, 0x8000000
        je     loc_1_2363
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
        jmp    loc_1_236E
loc_1_2363:
        xor    edx, edx
        mov    al, 0x21
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
loc_1_236E:
        out    dx, al
        mov    ecx, edi
        and    ecx, 8
        je     loc_1_237E
        test   edi, 0x3c00000
        je     loc_1_238C
loc_1_237E:
        mov    eax, edi
        and    eax, 0x3c00000
        cmp    eax, 0x800000
        jne    loc_1_2399
loc_1_238C:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 5
        in     al, dx
        or     al, 4
        out    dx, al
loc_1_2399:
        test   edi, 0x50
        jne    loc_1_23C0
        mov    eax, dword ptr [esi + 0x18]
        pop    edi
        shr    eax, 0xd
        and    al, 0x3f
        or     al, 0x40
        mov    byte ptr [esi + 0x145], al
        mov    byte ptr [esi + 0x144], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_23C0:
        test   ecx, ecx
        je     loc_1_23DB
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 0x144], 0
        mov    byte ptr [esi + 0x145], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_23DB:
        test   edi, 0x10
        je     loc_1_2452
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 5
        test   ebx, ebx
        in     al, dx
        je     loc_1_242B
        test   edi, 0x80000
        je     loc_1_2406
        mov    cl, al
        or     cl, 0x80
        mov    byte ptr [esi + 0x144], cl
        jmp    loc_1_241C
loc_1_2406:
        cmp    word ptr [esi + 0xa], 8
        jbe    loc_1_2416
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2416:
        mov    byte ptr [esi + 0x144], al
loc_1_241C:
        pop    edi
        mov    byte ptr [esi + 0x145], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_242B:
        and    al, 0x1f
        mov    ecx, dword ptr [esi + 0x18]
        shr    ecx, 3
        or     al, cl
        test   edi, 0x80000
        je     loc_1_2448
        mov    cl, al
        or     cl, 0x80
        mov    byte ptr [esi + 0x144], cl
loc_1_2448:
        lea    ecx, [esi + 0x145]
        mov    byte ptr [ecx], al
        jmp    loc_1_2462
loc_1_2452:
        lea    ecx, [esi + 0x145]
        mov    byte ptr [esi + 0x144], 1
        mov    byte ptr [ecx], 1
loc_1_2462:
        test   edi, 0x80000
        je     loc_1_2474
        or     byte ptr [esi + 0x144], 0x40
        or     byte ptr [ecx], 0x40
loc_1_2474:
        xor    edx, edx
        mov    al, byte ptr [ecx]
        mov    dx, word ptr [esi + 4]
        pop    edi
        add    edx, 5
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0xf0]
        mov    word ptr [esi + 0xf2], cx
        lea    eax, [ecx + 1]
        mov    word ptr [esi + 0xf0], ax
        cmp    word ptr [esi + 0x3a], ax
        jne    loc_1_24C0
        mov    word ptr [esi + 0xf0], 0
loc_1_24C0:
        movsx  eax, word ptr [esi + 0xf0]
        cmp    dword ptr [esi + eax*4 + 0xfd], 2
        je     loc_1_24DA
        mov    word ptr [esi + 0xf0], 0xffff
loc_1_24DA:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, cx
        mov    cx, word ptr [esi + ebx*4 + 0xf5]
        lea    edx, [edi + 0x14]
        mov    ax, word ptr [esi + 0xcd]
        imul   eax, ebx
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 0x15]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x16]
        mov    al, ch
        out    dx, al
        cmp    byte ptr [esi + 0x138], 0
        je     loc_1_252C
        test   dword ptr [esi + 0xc5], 0x8100000
        jne    loc_1_252C
        mov    byte ptr [esi + 0x139], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_252C:
        lea    edx, [edi + 0x10]
        mov    al, 0x26
        mov    byte ptr [esi + 0xf4], 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_2540:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0xcd]
        lea    edx, [ebp - 0x24]
        movsx  ecx, word ptr [esi + 0xee]
        imul   eax, ecx
        add    eax, dword ptr [esi + 0x1c]
        lea    ecx, [ebp - 0x34]
        push   ecx
        xor    ebx, ebx
        push   edx
        mov    dword ptr [ebp - 0x1c], eax
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   ebx
        push   eax
        call   sub_1_1C98
        mov    eax, dword ptr [esi + 0x119]
        mov    dword ptr [ebp - 0x28], ebx
        mov    byte ptr [ebp - 3], bl
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [esi + 0xc5]
        and    eax, 0x8080000
        cmp    eax, 0x8080000
        jne    loc_1_25B0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
        in     al, dx
        test   al, 4
        jne    loc_1_25B0
        mov    byte ptr [ebp - 3], 1
        mov    al, 0x10
        out    dx, al
loc_1_25B0:
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_28A7
loc_1_25BA:
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x24]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    ecx, dword ptr [esi + 0xc5]
        test   cl, 8
        mov    dword ptr [ebp - 0x20], ecx
        je     loc_1_2789
        test   ecx, 0x800000
        jne    loc_1_2789
        mov    eax, dword ptr [ebp - 0xc]
        xor    ecx, ecx
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x1c]
        and    al, 3
        mov    cl, al
        mov    byte ptr [ebp - 2], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        mov    ebx, dword ptr [eax]
        mov    eax, 4
        mov    dword ptr [ebp - 8], ebx
loc_1_2610:
        sub    eax, ecx
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x18], eax
        jae    loc_1_2620
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], eax
loc_1_2620:
        xor    eax, eax
        cmp    dword ptr [ebp - 0x14], eax
        je     loc_1_2650
loc_1_2627:
        mov    cl, byte ptr [ebp - 2]
        mov    edx, 0xff
        add    cl, al
        inc    edi
        shl    cl, 3
        inc    eax
        shl    edx, cl
        mov    ebx, edx
        not    ebx
        and    dword ptr [ebp - 8], ebx
        xor    ebx, ebx
        mov    bl, byte ptr [edi - 1]
        shl    ebx, cl
        and    ebx, edx
        or     dword ptr [ebp - 8], ebx
        cmp    dword ptr [ebp - 0x14], eax
        ja     loc_1_2627
loc_1_2650:
        test   byte ptr [ebp - 0x1d], 8
        je     loc_1_266F
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x144]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_1_266F:
        mov    edx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_2696
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_2696:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], edi
        add    dword ptr [ebp - 0x10], 4
        sub    ecx, dword ptr [ebp - 0x14]
        mov    edi, ecx
        shr    edi, 2
        test   edi, edi
        je     loc_1_2702
loc_1_26AC:
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_26CE
        xor    ebx, ebx
        mov    al, byte ptr [esi + 0x144]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_1_26CE:
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [edx], ebx
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_26F7
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_26F7:
        add    dword ptr [ebp - 0x10], 4
        add    dword ptr [ebp - 0xc], 4
        dec    edi
        jne    loc_1_26AC
loc_1_2702:
        test   cl, 3
        je     loc_1_277E
        and    cl, 3
        mov    edi, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], 0
        mov    edx, 0
        je     loc_1_2735
        xor    eax, eax
        mov    al, cl
loc_1_271F:
        lea    ecx, [edx*8]
        xor    ebx, ebx
        mov    bl, byte ptr [edi]
        inc    edi
        shl    ebx, cl
        inc    edx
        or     dword ptr [ebp - 8], ebx
        cmp    edx, eax
        jb     loc_1_271F
loc_1_2735:
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_2757
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x144]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_1_2757:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_277E
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_277E:
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2827
loc_1_2789:
        cmp    byte ptr [esi + 0x146], 0
        je     loc_1_27D8
        lea    edi, [esi + 0xd5]
        push   esi
        push   0                             ; -> sub_2_1D90
        push   edi
        call   sub_1_1C62
        push   1
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        mov    edx, dword ptr [ebp - 0x1c]
        push   edx
        push   esi
        call   sub_1_2C10
        cmp    byte ptr [esi + 0x307], 0
        je     loc_1_27CA
        cmp    byte ptr [esi + 0x308], 0
        jne    loc_1_2827
loc_1_27CA:
        push   esi
        push   0                             ; -> sub_2_1D30
        push   edi
        call   sub_1_1C62
        jmp    loc_1_2827
loc_1_27D8:
        test   byte ptr [ebp - 0x1d], 8
        je     loc_1_27F7
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x144]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_1_27F7:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x1c]
        push   ecx
        push   edx
        call   sub_1_2050
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_2827
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_2827:
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp - 0x1c], eax
        add    dword ptr [ebp - 0x28], eax
        cmp    byte ptr [ebp - 3], 0
        je     loc_1_2890
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [ebp - 0x2c], eax
        jae    loc_1_2890
        movsx  eax, word ptr [esi + 0xee]
        mov    dword ptr [esi + eax*4 + 0xfd], 2
        cmp    word ptr [esi + 0xf0], -1
        jne    loc_1_2890
        mov    ax, word ptr [esi + 0xee]
        cmp    word ptr [esi + 0xf2], -1
        mov    word ptr [esi + 0xf0], ax
        jne    loc_1_2890
        or     byte ptr [esi + 0x48], 0x20
        movsx  eax, ax
        mov    ecx, dword ptr [ebp - 0x34]
        push   esi
        mov    dword ptr [esi + eax*4 + 0xf5], ecx
        call   sub_1_2490
        mov    dword ptr [ebp - 0x2c], 0xffffffff
loc_1_2890:
        lea    eax, [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x24]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_1_25BA
loc_1_28A7:
        cmp    byte ptr [ebp - 3], 0
        je     loc_1_28B9
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
        out    dx, al
loc_1_28B9:
        cmp    dword ptr [ebp - 0x28], 0x3c
        jae    loc_1_28DA
        movsx  eax, word ptr [esi + 0xee]
        pop    edi
        mov    dword ptr [esi + eax*4 + 0xf5], 0x3c
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_28DA:
        movsx  ecx, word ptr [esi + 0xee]
        mov    eax, dword ptr [ebp - 0x28]
        pop    edi
        mov    dword ptr [esi + ecx*4 + 0xf5], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2900:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 0x18
        mov    al, byte ptr [esi + 0x3c]
        out    dx, al
        push   0                             ; -> loc_2_2BE0
        lea    eax, [esi + 0xd5]
        push   eax
        call   sub_1_1C62
        mov    cl, byte ptr [esi + 0xed]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 4], cl
        add    edx, 0x13
        in     al, dx
        lea    ecx, [eax + 1]
        cmp    byte ptr [esi + 0x13b], cl
        ja     loc_1_294F
        mov    cl, byte ptr [esi + 0x13a]
loc_1_294F:
        cmp    cl, byte ptr [ebp - 4]
        je     loc_1_2B3F
loc_1_2958:
        mov    ebx, dword ptr [esi + 0xc5]
        mov    eax, ebx
        and    al, 7
        cmp    al, 1
        jne    loc_1_29CD
        xor    edx, edx
        mov    edi, ecx
        mov    dx, word ptr [esi + 0x38]
        and    edi, 0xff
        imul   edi, edx
        add    edi, dword ptr [esi + 0x1c]
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 2], al
        mov    cl, byte ptr [edi + 1]
        mov    dword ptr [ebp - 0xc], 0
        mov    byte ptr [ebp - 1], cl
        mov    bx, word ptr [edi + 2]
        mov    word ptr [ebp - 0xc], bx
        sub    dword ptr [ebp - 0xc], edi
        jns    loc_1_29BB
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x13b]
        mov    al, byte ptr [esi + 0x13a]
        sub    ecx, eax
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        add    ecx, ebx
        dec    ecx
        imul   ecx, edx
        sub    ecx, edi
        mov    dword ptr [ebp - 0xc], ecx
loc_1_29BB:
        cmp    byte ptr [ebp - 0xc], 0xfc
        jbe    loc_1_2A95
        add    dword ptr [ebp - 0xc], edx
        jmp    loc_1_2A95
loc_1_29CD:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 0x38]
        mov    al, cl
        mov    dword ptr [ebp - 0xc], 0
        imul   edi, eax
        add    edi, dword ptr [esi + 0x1c]
        cmp    byte ptr [esi + 0x146], 0
        je     loc_1_2A34
        push   esi
        lea    eax, [esi + 0xd5]
        push   0                             ; -> sub_2_1D90
        push   eax
        call   sub_1_1C62
        push   0
        lea    eax, [ebp - 0xc]
        push   2
        lea    ecx, [edi + 2]
        push   eax
        push   ecx
        push   esi
        call   sub_1_2C10
        push   0
        lea    ecx, [ebp - 2]
        push   1
        push   ecx
        push   edi
        push   esi
        call   sub_1_2C10
        push   0
        lea    ecx, [ebp - 1]
        push   1
        lea    edx, [edi + 1]
        push   ecx
        push   edx
        push   esi
        call   sub_1_2C10
        jmp    loc_1_2A82
loc_1_2A34:
        and    ebx, 0x8000000
        je     loc_1_2A55
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x144]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        or     al, 0x40
        out    dx, al
loc_1_2A55:
        mov    ax, word ptr [edi + 2]
        test   ebx, ebx
        mov    word ptr [ebp - 0xc], ax
        mov    cl, byte ptr [edi]
        mov    byte ptr [ebp - 2], cl
        mov    dl, byte ptr [edi + 1]
        mov    byte ptr [ebp - 1], dl
        je     loc_1_2A82
        xor    edx, edx
        mov    al, byte ptr [ebp - 3]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_2A82:
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 0x13a], al
        ja     loc_1_2B05
        cmp    byte ptr [esi + 0x13b], al
        jb     loc_1_2B05
loc_1_2A95:
        test   byte ptr [ebp - 2], 1
        je     loc_1_2ABB
        add    edi, 4
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 4
        push   esi
        push   eax
        mov    dword ptr [esi + 0x134], edi
        mov    dword ptr [esi + 0x140], eax
        call   sub_1_2F40
        mov    word ptr [ebp - 6], ax
loc_1_2ABB:
        mov    cl, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 0x13b]
        cmp    cl, al
        jbe    loc_1_2AD3
        mov    cl, byte ptr [esi + 0x13a]
        sub    cl, al
        add    cl, byte ptr [ebp - 1]
loc_1_2AD3:
        cmp    byte ptr [esi + 0x13a], cl
        jne    loc_1_2AE2
        and    eax, 0xff
        jmp    loc_1_2AE6
loc_1_2AE2:
        xor    eax, eax
        mov    al, cl
loc_1_2AE6:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x13
        out    dx, al
        cmp    word ptr [ebp - 6], 5
        jne    loc_1_294F
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B05:
        mov    cl, byte ptr [ebp - 4]
        mov    dl, byte ptr [esi + 0x13b]
        mov    al, cl
        mov    byte ptr [ebp - 1], cl
        cmp    cl, dl
        jbe    loc_1_2B21
        mov    al, byte ptr [esi + 0x13a]
        sub    al, dl
        add    al, cl
loc_1_2B21:
        cmp    byte ptr [esi + 0x13a], al
        jne    loc_1_2B2F
        xor    eax, eax
        mov    al, dl
        jmp    loc_1_2B34
loc_1_2B2F:
        and    eax, 0xff
loc_1_2B34:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x13
        out    dx, al
loc_1_2B3F:
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 0x17
        push   0                             ; -> loc_2_2BE0
        out    dx, al
        lea    eax, [esi + 0xd5]
        push   eax
        call   sub_1_1C62
        mov    cl, byte ptr [esi + 0xed]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 4], cl
        add    edx, 0x13
        in     al, dx
        lea    ecx, [eax + 1]
        cmp    byte ptr [esi + 0x13b], cl
        ja     loc_1_2B81
        mov    cl, byte ptr [esi + 0x13a]
loc_1_2B81:
        xor    eax, eax
        xor    edx, edx
        mov    ax, word ptr [esi + 0x38]
        mov    dl, cl
        imul   eax, edx
        add    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x13c], eax
        cmp    cl, byte ptr [ebp - 4]
        jne    loc_1_2958
        cmp    byte ptr [esi + 0x138], 0
        je     loc_1_2BFE
        xor    ecx, ecx
        mov    al, 0x10
        test   dword ptr [esi + 0xc5], 0x8100000
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 0x138], 0
        lea    edx, [ecx + 0x17]
        out    dx, al
        je     loc_1_2BD6
        lea    edx, [ecx + 0x13]
        pop    edi
        in     al, dx
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
loc_1_2BD6:
        mov    al, 0xff
        out    dx, al
        lea    edx, [ecx + 0x1d]
        xor    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x139], 0
        mov    al, 0x22
        je     loc_1_2BF3
        mov    byte ptr [esi + 0x139], 0
        mov    al, 0x26
loc_1_2BF3:
        mov    byte ptr [esi + 0x31a], 0
        lea    edx, [ecx + 0x10]
        out    dx, al
loc_1_2BFE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x14]
        test   ebx, ebx
        jne    loc_1_2C26
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_2C26:
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
        lea    edx, [ecx + 7]
        out    dx, al
        test   edi, 1
        mov    al, ah
        out    dx, al
        je     loc_1_2C57
        cmp    byte ptr [ebp + 0x18], 0
        lea    edx, [ecx + 2]
        je     loc_1_2C52
        mov    al, byte ptr [edi]
        out    dx, al
        jmp    loc_1_2C55
loc_1_2C52:
        in     al, dx
        mov    byte ptr [edi], al
loc_1_2C55:
        dec    ebx
        inc    edi
loc_1_2C57:
        test   ebx, ebx
        jne    loc_1_2C64
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_2C64:
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_1_2CC8
        cmp    ebx, 1
        jbe    loc_1_2CAE
        mov    eax, ebx
        shr    eax, 1
        push   eax
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        add    eax, 2
        push   eax
        call   sub_1_1B1C
        test   bl, 1
        je     loc_1_2D19
        and    ebx, 0xfffffffe
        push   1
        add    edi, ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B08
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_2CAE:
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B08
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_2CC8:
        cmp    ebx, 1
        jbe    loc_1_2D08
        mov    eax, ebx
        shr    eax, 1
        push   eax
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        add    eax, 2
        push   eax
        call   sub_1_1B59
        test   bl, 1
        je     loc_1_2D19
        and    ebx, 0xfffffffe
        push   1
        add    edi, ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B45
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_2D08:
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B45
loc_1_2D19:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  04Ch, 024h, 00Ch, 066h, 083h, 07Ch, 024h, 008h, 001h, 08Bh, 0ECh, 08Bh, 091h, 009h, 001h, 000h
        db  000h, 08Bh, 081h, 015h, 001h, 000h, 000h, 0C7h, 081h, 009h, 001h, 000h, 000h, 000h, 000h, 000h
        db  000h, 08Bh, 000h, 08Bh, 089h, 005h, 001h, 000h, 000h, 050h, 0B8h, 000h, 000h, 000h, 000h, 083h
        db  0D0h, 0FFh, 025h, 001h, 000h, 000h, 0C0h, 050h, 052h, 051h, 0E8h, 047h, 0EEh, 0FFh, 0FFh, 066h
        db  033h, 0C0h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x16f]
        test   ecx, ecx
        jne    loc_1_2D9D
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2D9D:
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x16f], eax
        test   eax, eax
        jne    loc_1_2DB4
        mov    dword ptr [esi + 0x173], 0
loc_1_2DB4:
        cmp    byte ptr [ecx + 0x2c], 0
        mov    edi, dword ptr [ecx + 0x28]
        je     loc_1_2DEF
        cmp    dword ptr [esi + 0x177], 0
        jne    loc_1_2DD4
        mov    dword ptr [esi + 0x17b], ecx
        mov    dword ptr [esi + 0x177], ecx
        jmp    loc_1_2DE3
loc_1_2DD4:
        mov    eax, dword ptr [esi + 0x17b]
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [esi + 0x17b], ecx
loc_1_2DE3:
        mov    dword ptr [ecx + 0x24], 0
        jmp    loc_1_2E91
loc_1_2DEF:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    loc_1_2E00
        inc    dword ptr [esi + 0x18c]
        jmp    loc_1_2E06
loc_1_2E00:
        inc    dword ptr [esi + 0x194]
loc_1_2E06:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BB2
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_2E91
        mov    eax, dword ptr [ebx + 0x15b]
        cmp    edi, eax
        jne    loc_1_2E3E
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x15b], eax
        jmp    loc_1_2E51
loc_1_2E3E:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_2E4B
loc_1_2E43:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_2E43
loc_1_2E4B:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_2E51:
        mov    eax, dword ptr [ebx + 0x1f7]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_2E62
        push   ebx
        call   0
loc_1_2E62:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   0x75
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 0x15b], 0
        jne    loc_1_2E91
        push   ebx
        push   0                             ; -> loc_1_1900
        add    ebx, 0xd5
        push   ebx
        call   sub_1_1C62
loc_1_2E91:
        mov    edi, dword ptr [esi + 0x167]
        test   edi, edi
        je     loc_1_2F35
        cmp    byte ptr [esi + 0x1ed], 0
        jne    loc_1_2F35
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x167], eax
        test   eax, eax
        jne    loc_1_2EC3
        mov    dword ptr [esi + 0x16b], 0
loc_1_2EC3:
        push   esi
        push   edi
        call   sub_2_2560
        cmp    ax, 6
        jne    loc_1_2F03
loc_1_2ED0:
        mov    eax, dword ptr [esi + 0x167]
        test   eax, eax
        je     loc_1_2EED
        mov    dword ptr [edi + 0x24], eax
        xor    ax, ax
        mov    dword ptr [esi + 0x167], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2EED:
        xor    ax, ax
        mov    dword ptr [esi + 0x167], edi
        mov    dword ptr [esi + 0x16b], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F03:
        test   ax, ax
        jne    loc_1_2F35
        cmp    dword ptr [esi + 0x16f], 0
        jne    loc_1_2F1F
        mov    dword ptr [esi + 0x173], edi
        mov    dword ptr [esi + 0x16f], edi
        jmp    loc_1_2F2E
loc_1_2F1F:
        mov    eax, dword ptr [esi + 0x173]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x173], edi
loc_1_2F2E:
        mov    dword ptr [edi + 0x24], 0
loc_1_2F35:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_2F40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 0x183], 1
        mov    dword ptr [edi + 0x17f], 0
        mov    ebx, dword ptr [edi + 0x1f7]
        add    ebx, 0xe
        cmp    ebx, esi
        jb     loc_1_2F6C
        mov    ebx, esi
loc_1_2F6C:
        push   edi
        lea    eax, [edi + 0x20b]
        push   eax
        push   0
        push   ebx
        call   sub_2_2940
        test   ax, ax
        je     loc_1_2F8B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F8B:
        inc    dword ptr [edi + 0x190]
        cmp    esi, 0xe
        jae    loc_1_2FC1
        cmp    esi, 6
        jb     loc_1_2FE7
        push   0
        lea    eax, [edi + 0x20b]
        push   0
        mov    edx, dword ptr [edi + 0x147]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_1D60
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2FC1:
        sub    esi, 0xe
        sub    ebx, 0xe
        push   esi
        lea    eax, [edi + 0x219]
        push   ebx
        lea    ecx, [edi + 0x20b]
        push   eax
        push   0xe
        push   ecx
        mov    eax, dword ptr [edi + 0x147]
        push   ecx
        push   edi
        push   eax
        call   sub_1_1D60
loc_1_2FE7:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3000:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 0x40], 0
        test   byte ptr [edi + 0xc9], 8
        jne    loc_1_301F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_301F:
        mov    ecx, dword ptr [edi + 0xc5]
        test   ecx, 0x8000000
        jne    loc_1_3035
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3035:
        mov    ax, word ptr [edi + 0x3e]
        test   ax, ax
        je     loc_1_3070
        test   al, 1
        jne    loc_1_304A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_304A:
        test   al, 4
        je     loc_1_305D
        and    ax, 2
        cmp    ax, 1
        sbb    esi, esi
        add    esi, 3
        jmp    loc_1_3096
loc_1_305D:
        test   al, 2
        je     loc_1_3068
        mov    esi, 1
        jmp    loc_1_3096
loc_1_3068:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3070:
        and    cl, 7
        test   cl, 3
        je     loc_1_3080
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3080:
        test   cl, 2
        je     loc_1_308C
        mov    esi, 2
        jmp    loc_1_3096
loc_1_308C:
        test   cl, 4
        je     loc_1_30CD
        mov    esi, 3
loc_1_3096:
        test   esi, 1
        je     loc_1_30B1
        push   edi
        call   sub_1_30E0
        cmp    esi, 1
        jne    loc_1_30B1
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30B1:
        test   byte ptr [edi + 0x40], 2
        je     loc_1_30BF
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30BF:
        push   edi
        call   sub_1_3180
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30CD:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        mov    al, 0x80
        lea    edi, [ecx + 0x10]
        movsx  ecx, cx
        mov    dx, di
        out    dx, al
        lea    edx, [ecx + 0x1d]
        in     al, dx
        mov    dx, di
        mov    byte ptr [ebp - 2], al
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 0x1d]
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        mov    bl, al
        mov    byte ptr [ebp - 1], al
        and    bl, 0xc3
        or     bl, 0x80
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 0xd]
        in     al, dx
        and    al, 0xfd
        or     al, 1
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, di
        mov    al, 0xc0
        out    dx, al
        lea    edx, [ecx + 0x1f]
        in     al, dx
        mov    dx, di
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        test   byte ptr [ebp - 3], 4
        je     loc_1_314E
        or     byte ptr [esi + 0x40], 2
        jmp    loc_1_3166
loc_1_314E:
        lea    edx, [ecx + 4]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 0xd]
        in     al, dx
        or     byte ptr [esi + 0x40], 1
        or     al, 3
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_3166:
        mov    dx, di
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 0x1d]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 4]
        lea    eax, [edi + 0x10]
        movsx  ebx, di
        mov    dx, ax
        mov    word ptr [ebp - 6], ax
        mov    al, 0x80
        out    dx, al
        lea    edx, [ebx + 0x1d]
        in     al, dx
        mov    dx, word ptr [ebp - 6]
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        lea    eax, [ebx + 0x1d]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [ebx + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0xc3
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xd]
        mov    byte ptr [ebp - 0xc], al
        in     al, dx
        and    al, 0xfd
        mov    ecx, dword ptr [esi + 0x1c]
        or     al, 1
        out    dx, al
        cmp    byte ptr [esi + 0x146], 0
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        je     loc_1_31F5
        push   1
        push   0x66
        push   0                             ; -> loc_1_35BC
        push   ecx
        push   esi
        call   sub_1_2C10
        jmp    loc_1_323F
loc_1_31F5:
        test   byte ptr [esi + 0xc8], 8
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        je     loc_1_3217
        lea    edx, [ebx + 5]
        mov    al, byte ptr [esi + 0x144]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        or     al, 0x40
        out    dx, al
loc_1_3217:
        push   0x66
        push   0                             ; -> loc_1_35BC
        push   ecx
        call   sub_1_2050
        test   byte ptr [esi + 0xc8], 8
        je     loc_1_323F
        mov    al, byte ptr [ebp - 2]
        mov    edx, ebx
        and    al, 0xbf
        out    dx, al
        lea    edx, [ebx + 5]
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_1_323F:
        lea    edx, [ebx + 0x17]
        mov    al, 0xff
        out    dx, al
        push   0x400
        push   0
        push   edi
        call   sub_1_32D0
        xor    edi, edi
loc_1_3254:
        push   0x3e8
        call   sub_1_1C68
        lea    edx, [ebx + 0x17]
        in     al, dx
        test   al, 0x7f
        jne    loc_1_326F
        inc    edi
        cmp    edi, 0x300
        jl     loc_1_3254
loc_1_326F:
        mov    cx, word ptr [esi + 0x40]
        lea    edx, [ebx + 0x14]
        in     al, dx
        and    cx, 0xfff8
        test   al, 0x10
        mov    word ptr [esi + 0x40], cx
        jne    loc_1_328D
        or     cl, 4
        mov    word ptr [esi + 0x40], cx
        jmp    loc_1_32AB
loc_1_328D:
        or     cl, 1
        lea    eax, [ebx + 4]
        mov    word ptr [esi + 0x40], cx
        mov    edx, eax
        mov    al, byte ptr [ebp - 0xc]
        out    dx, al
        lea    edx, [ebx + 0xd]
        in     al, dx
        or     al, 3
        out    dx, al
        lea    edx, [ebx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_32AB:
        mov    dx, word ptr [ebp - 6]
        xor    al, al
        out    dx, al
        lea    edx, [ebx + 0x1d]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    cx, word ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    esi, [ecx + 0x10]
        movsx  ecx, cx
        mov    dx, si
        in     al, dx
        and    eax, 0xff
        and    al, 0x3b
        out    dx, al
        lea    edx, [ecx + 0x14]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        lea    edx, [ecx + 0x15]
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, al
        lea    edx, [ecx + 0x16]
        mov    al, ah
        out    dx, al
        mov    dx, si
        xor    ecx, ecx
        in     al, dx
        mov    cl, al
        mov    al, cl
        pop    esi
        or     al, 2
        pop    ebp
        out    dx, al
        mov    al, cl
        or     al, 4
        out    dx, al
        xor    eax, eax
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h
        db  038h, 030h, 030h, 030h, 057h, 02Eh, 033h, 038h, 036h, 020h, 076h, 065h, 072h, 073h, 069h, 06Fh
        db  06Eh, 020h, 031h, 02Eh, 031h, 031h, 064h, 020h, 030h, 031h, 02Fh, 031h, 038h, 02Fh, 039h, 035h
        db  00Ah, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 053h
        db  074h, 061h, 06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h
        db  074h, 065h, 06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 033h, 000h
        db  040h, 028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 034h, 031h, 036h, 05Fh, 04Eh, 044h, 049h
        db  053h, 033h, 05Fh, 073h, 032h, 02Eh, 032h, 05Fh, 076h, 031h, 02Eh, 031h, 031h, 064h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_33F4:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
loc_1_3450:
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_3470:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D8:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_34EC:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3504:
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_3540:
        push   edx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        jne    0x35c2
        push   esp
        jns    0x35c2
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_35AC
        jns    loc_1_35CA
        add    byte ptr gs:[ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     0x35e1
        insd   dword ptr es:[edi], dx
        jne    0x35e8
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     0x35d5
        imul   edi, dword ptr [edx + 0x65], 0x4d000000
        popal  
        js     0x35f5
        insd   dword ptr es:[edi], dx
        jne    0x35fc
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     0x35e9
        imul   edi, dword ptr [edx + 0x65], 0x53000000
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_3619
        jb     0x35d6
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35AC:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0xf0b0a07
loc_1_35BC:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4d], dl
loc_1_35CA:
        inc    ebx
        and    byte ptr [ecx + 0x75], al
        je     0x363f
        and    byte ptr [ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x20], 0x65746544
        arpl   word ptr [eax + 0x54], si
        jae    0x3656
        and    byte ptr [esi + 0x72], al
        popal  
        insd   dword ptr es:[edi], dx
        and    byte ptr cs:[eax + ebp*2 + 0x61], dl
        outsb  dx, byte ptr [esi]
        imul   esp, dword ptr [eax], 0x79
        outsd  dx, dword ptr [esi]
        jne    loc_1_3614
        outsw  dx, word ptr [esi]
        jb     loc_1_3618
        jne    0x366d
        imul   ebp, dword ptr [esi + 0x67], 0x434d5320
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jne    0x366b
        je     0x367d
        and    byte ptr cs:[0x524b5357], ch
        push   esp
        inc    esp
        inc    esp
loc_1_3614:
        pop    edx
        push   eax
        dec    ebp
        inc    edx
loc_1_3618:
        inc    edi
loc_1_3619:
        push   ebx
        inc    esi
        dec    ebp
        push   esp
        dec    edx
        inc    edi
        push   esi
        push   eax
        sub    eax, 0xffff0000
        db  0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3510  R-X
sub_2_0000:
        push   ebp
        mov    eax, 9
        mov    ebp, esp
        sub    esp, 0x28
        mov    word ptr [ebp - 0x10], ax
        push   ebx
        mov    word ptr [ebp - 0xe], ax
        xor    ebx, ebx
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_34D8
        mov    word ptr [ebp - 0x24], 0x6fc0
        mov    word ptr [ebp - 0x22], 0x6fc1
        mov    word ptr [ebp - 0x20], 0x6fc2
        mov    word ptr [ebp - 0x1e], 0xefe5
        mov    word ptr [ebp - 0x1c], 0x61c8
        mov    word ptr [ebp - 0x1a], 0x61c9
        mov    word ptr [ebp - 0x18], 0x6fc6
        mov    word ptr [ebp - 0x28], bx
        push   ebx
        mov    word ptr [ebp - 0x26], ax
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        lea    eax, [ebp - 0x28]
        push   eax
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 0x16], 0xefd5
        mov    word ptr [ebp - 0x14], 0xefd4
        push   edx
        call   sub_1_1B82
        lea    ecx, [ebp - 0x28]
        mov    dword ptr [0x4a], ecx         ; -> obj1:0x33EE
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [4], edx            ; -> obj1:0x33A8
        mov    byte ptr [0x4e], bl           ; -> obj1:0x33F2
        mov    dword ptr [0x3e], ebx         ; -> obj1:0x33E2
        mov    dword ptr [0x42], ebx         ; -> obj1:0x33E6
        mov    byte ptr [8], 3               ; -> obj1:0x33AC
        mov    byte ptr [9], bl              ; -> obj1:0x33AD
        mov    dword ptr [0xa], ebx          ; -> obj1:0x33AE
        mov    dword ptr [0xe], 0            ; -> obj1:0x33B2
        mov    dword ptr [0x12], 0           ; -> obj1:0x33B6
        mov    dword ptr [0x16], 0           ; -> obj1:0x33BA
        mov    dword ptr [0x1a], 0           ; -> obj1:0x33BE
        mov    dword ptr [0x1e], 0           ; -> obj1:0x33C2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x22], 0           ; -> obj1:0x33C6
        mov    dword ptr [0x26], 0           ; -> obj1:0x33CA
        mov    dword ptr [0x2a], 0           ; -> obj1:0x33CE
        mov    dword ptr [0x2e], 0           ; -> obj1:0x33D2
        mov    dword ptr [0x32], 0           ; -> obj1:0x33D6
        push   0x36
        mov    dword ptr [0x36], eax         ; -> obj1:0x33DA
        push   8                             ; -> obj1:0x33AC
        lea    eax, [ebp - 4]
        push   0                             ; -> obj1:0x33A4
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x33DE
        mov    edx, dword ptr [ebp - 8]
        push   edx
        push   0                             ; -> obj1:0x33A4
        push   eax
        call   sub_1_1B8E
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_0150
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1B88
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0150:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0160:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 0x74
        mov    word ptr [ebp - 0x3c], ax
        push   ebx
        mov    word ptr [ebp - 0x3a], ax
        push   esi
        push   edi
        mov    eax, 0xd
        mov    dword ptr [ebp - 0x38], 0     ; -> loc_1_34EC
        mov    dword ptr [ebp - 0x48], 0     ; -> loc_1_3504
        mov    dword ptr [ebp - 0x30], 0     ; -> obj1:0x3530
        mov    word ptr [ebp - 0x4c], ax
        mov    word ptr [ebp - 0x4a], ax
        mov    eax, 4
        mov    dword ptr [ebp - 0x28], 0     ; -> loc_1_3540
        mov    word ptr [ebp - 0x34], ax
        mov    word ptr [ebp - 0x32], ax
        mov    eax, 0xb
        mov    word ptr [ebp - 0x2c], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x2a], ax
        mov    byte ptr [ebp - 1], bl
        mov    eax, 8
        mov    byte ptr [ebp - 2], bl
        mov    word ptr [ebp - 0x44], ax
        mov    byte ptr [ebp - 0x24], bl
        mov    word ptr [ebp - 0x42], ax
        mov    esi, 0x10
        lea    eax, [ebp - 0x23]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x40], 0     ; -> obj1:0x3554
        push   ecx
        lea    edx, [ebp - 0xc]
        mov    word ptr [ebp - 4], 0x280
        mov    dword ptr [eax], ebx
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 0x10]
        push   eax
        push   edx
        call   sub_1_1CEC
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_2_020F
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_020F:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1CF2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0232
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [eax + 4]
loc_2_0232:
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1CF2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0262
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 3
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_2_0262:
        cmp    byte ptr [ebp - 2], 1
        je     loc_2_028F
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1CF2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_028F
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 4], cx
loc_2_028F:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x18]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_1BF4
        cmp    dword ptr [ebp - 0x1c], 6
        jne    loc_2_02C6
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_02C6
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 0x24], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x20], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x1f], cl
loc_2_02C6:
        xor    edi, edi
loc_2_02C8:
        mov    eax, dword ptr [0]            ; -> obj1:0x33A0
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   0x31b
        push   ecx
        call   sub_1_1C6E
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_02EF
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0xaa000002
        jmp    loc_2_02C8
loc_2_02EF:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xc6
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        cmp    byte ptr [ebp - 1], al
        jne    loc_2_032E
        cmp    byte ptr [ebp - 2], 1
        jne    loc_2_032E
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_2_1930
        test   ax, ax
        je     loc_2_032E
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 4], 0
        mov    ebx, 0xaa00000a
loc_2_032E:
        lea    edi, [ebp - 0x74]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    al, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 0x5e], 1
        dec    al
        mov    ecx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    dword ptr [ebp - 0x56], 0x20
        sbb    eax, eax
        lea    edx, [ebp - 0x74]
        and    eax, 2
        and    ecx, 0xffff
        inc    eax
        push   edx
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x6a], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x5a], ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x33A4
        push   edx
        push   eax
        lea    eax, [edx + 0x52]
        push   eax
        call   sub_1_1B9A
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_2_03B3
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1CF8
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x31b
        push   eax
        call   sub_1_1C74
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_03B3:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_03FE
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 0x52]
        push   ecx
        call   sub_1_1D40
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_1CF8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x52]
        push   edx
        call   sub_1_1BA0
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x31b
        push   ecx
        call   sub_1_1C74
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_03FE:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx], al
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x5c], 0
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0xcd], 0x4000
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_2_1A10
        cmp    ax, 0xffff
        jne    loc_2_047C
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x52]
        push   ecx
        call   sub_1_1D40
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_1CF8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x52]
        push   edx
        call   sub_1_1BA0
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x31b
        push   ecx
        call   sub_1_1C74
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_047C:
        cmp    ax, 2
        jne    loc_2_04DF
        push   0
        lea    eax, [ebp - 0x34]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1CF2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_04AC
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0xa], dx
loc_2_04AC:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1CF2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_04D5
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x18], edx
loc_2_04D5:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x146], 0
loc_2_04DF:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1CF8
        lea    eax, [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   esi
        push   2
        push   ecx
        call   sub_2_0540
        test   eax, eax
        je     loc_2_0526
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x52]
        push   ecx
        call   sub_1_1BA0
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x31b
        push   ecx
        call   sub_1_1C74
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0526:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0540:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x163], edi
        cmp    ebx, 2
        jbe    loc_2_0583
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000002
        push   eax
        call   sub_1_1D40
        mov    dword ptr [ebp - 4], 0xc000009a
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0583:
        mov    eax, 0                        ; -> sub_1_1180
        mov    dword ptr [esi + 0x203], esi
        mov    dword ptr [esi + 0x207], eax
        mov    dword ptr [esi + 0x1ff], eax
        mov    dword ptr [esi + 0x15b], 0
        mov    dword ptr [esi + 0x1fb], 0
        mov    dword ptr [esi + 0x153], 0    ; -> obj1:0x33A4
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x33E6
        mov    dword ptr [esi + 0x157], ecx
        mov    dword ptr [0x42], esi         ; -> obj1:0x33E6
        movsx  eax, byte ptr [esi + 0xa]
        cmp    byte ptr [esi], 1
        sbb    ecx, ecx
        add    ecx, 2
        push   ecx
        push   0
        push   eax
        push   eax
        push   0                             ; -> loc_1_0FF0
        mov    eax, dword ptr [esi + 0x52]
        push   esi
        lea    edx, [esi + 0xd5]
        push   0                             ; -> loc_1_0FD0
        lea    ecx, [ebp - 4]
        push   eax
        push   edx
        push   ecx
        call   sub_1_1C56
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_061F
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000005
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_061F:
        cmp    byte ptr [esi + 0x146], 0
        je     loc_2_0785
        mov    dword ptr [esi + 0x1c], 0
loc_2_0633:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0x14]
loc_2_0638:
        mov    al, byte ptr [ecx + edx]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [esi + edx + 0x23], al
        jb     loc_2_0638
        mov    word ptr [esi + 0x3a], bx
        lea    eax, [esi + 0x1b4]
        lea    ecx, [esi + 0x1bc]
        lea    edx, [esi + 0x1b8]
        mov    dword ptr [esi + 0x69], eax
        mov    dword ptr [esi + 0x99], ecx
        lea    eax, [esi + 0x1b0]
        lea    ecx, [esi + 0x1c0]
        mov    dword ptr [esi + 0x6d], edx
        mov    dword ptr [esi + 0x9d], eax
        lea    edx, [esi + 0x1c4]
        lea    eax, [esi + 0x1c8]
        mov    dword ptr [esi + 0xa1], ecx
        mov    dword ptr [esi + 0x71], edx
        lea    ecx, [esi + 0x1cc]
        lea    edx, [esi + 0x1d0]
        mov    dword ptr [esi + 0x75], eax
        mov    dword ptr [esi + 0x79], ecx
        lea    eax, [esi + 0x1d4]
        lea    ecx, [esi + 0x1d8]
        mov    dword ptr [esi + 0x7d], edx
        mov    dword ptr [esi + 0x81], eax
        lea    edx, [esi + 0x1dc]
        lea    eax, [esi + 0x1e0]
        mov    dword ptr [esi + 0x85], ecx
        mov    dword ptr [esi + 0x89], edx
        lea    ecx, [esi + 0x1e4]
        lea    edx, [esi + 0x1e8]
        mov    dword ptr [esi + 0x8d], eax
        mov    dword ptr [esi + 0x91], ecx
        lea    eax, [esi + 0x1a0]
        lea    ecx, [esi + 0x194]
        mov    dword ptr [esi + 0x95], edx
        mov    dword ptr [esi + 0xa5], eax
        lea    edx, [esi + 0x198]
        lea    eax, [esi + 0x19c]
        mov    dword ptr [esi + 0xa9], ecx
        mov    dword ptr [esi + 0xad], edx
        lea    ecx, [esi + 0x1a4]
        lea    edx, [esi + 0x1a8]
        mov    dword ptr [esi + 0xb1], eax
        mov    dword ptr [esi + 0xb5], ecx
        lea    eax, [esi + 0x1ac]
        lea    ebx, [esi + 0x147]
        mov    dword ptr [esi + 0xb9], edx
        mov    dword ptr [esi + 0xc1], eax
        push   esi
        mov    dword ptr [ebx], 0
        mov    word ptr [esi + 0x36], 0x5ea
        mov    word ptr [esi + 0x38], 0x100
        call   sub_2_1BB0
        test   ax, ax
        je     loc_2_07C3
        push   0
        mov    eax, dword ptr [esi + 0x52]
        mov    byte ptr [esi + 0x1ec], 1
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_0802
loc_2_0785:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x18]
        mov    ax, word ptr [esi + 0x16]
        mov    edx, dword ptr [esi + 0x52]
        shl    eax, 0xa
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 0x1c]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_1D3A
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0633
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000001
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        jmp    loc_2_0802
loc_2_07C3:
        push   ebx
        lea    eax, [esi + 0x14f]
        push   eax
        lea    ecx, [esi + 0x24]
        push   ecx
        push   0                             ; -> loc_1_0FC0
        push   0                             ; -> loc_1_0F90
        push   0                             ; -> loc_1_0F80
        push   edi
        call   sub_2_2C90
        test   al, al
        jne    loc_2_0867
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000002
        push   eax
        call   sub_1_1D40
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0xc
loc_2_0802:
        lea    eax, [esi + 0xd5]
        push   eax
        call   sub_1_1C5C
        cmp    esi, dword ptr [0x42]         ; -> obj1:0x33E6
        jne    loc_2_082D
        mov    eax, dword ptr [esi + 0x157]
        pop    edi
        pop    esi
        mov    dword ptr [0x42], eax         ; -> obj1:0x33E6
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_082D:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x33E6
        cmp    dword ptr [ecx + 0x157], esi
        je     loc_2_0849
loc_2_083B:
        mov    ecx, dword ptr [ecx + 0x157]
        cmp    dword ptr [ecx + 0x157], esi
        jne    loc_2_083B
loc_2_0849:
        mov    eax, dword ptr [ecx + 0x157]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x157]
        mov    dword ptr [ecx + 0x157], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0867:
        mov    ecx, 0                        ; -> loc_1_1920
        lea    eax, [esi + 0x30e]
        push   0x1388
        mov    dword ptr [esi + 0x30a], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x312], ecx
        mov    dword ptr [esi + 0x316], esi
        call   sub_1_1C4A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_08A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_2_08C4:
        test   edx, edx
        je     loc_2_08D3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_08C4
        mov    dword ptr [ecx], edx
loc_2_08D3:
        cmp    dword ptr [ecx], -1
        jne    loc_2_08E5
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_08E5:
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x1ec], 0
        je     loc_2_08FE
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_08FE:
        mov    eax, dword ptr [0]            ; -> obj1:0x33A0
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_1C6E
        test   eax, eax
        je     loc_2_0935
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000002
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0935:
        inc    dword ptr [esi + 0x15f]
        cmp    dword ptr [esi + 0x15b], 0
        jne    loc_2_096F
        push   esi
        call   sub_2_1E90
        test   ax, ax
        je     loc_2_096F
        push   0
        mov    eax, dword ptr [esi + 0x52]
        push   0xaa000003
        push   eax
        call   sub_1_1D40
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_096F:
        mov    eax, dword ptr [esi + 0x15b]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x15b], ecx
        mov    ecx, dword ptr [esi + 0x147]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_2E00
        test   al, al
        jne    loc_2_09D5
        push   0
        dec    dword ptr [esi + 0x15f]
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x15b], ecx
        mov    edx, dword ptr [esi + 0x52]
        push   edx
        call   sub_1_1D40
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_09D5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x153]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 0xee
        mov    byte ptr [eax + 0x20], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x71], ecx
        mov    dword ptr [esi + 0x1f7], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 0x15f]
        dec    eax
        mov    dword ptr [esi + 0x15f], eax
        jne    loc_2_0A6C
        cmp    byte ptr [esi + 0x1ed], cl
        je     loc_2_0A47
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], cl
        je     loc_2_0A5B
loc_2_0A47:
        cmp    dword ptr [esi + 0x177], 0
        je     loc_2_0A6C
        lea    eax, [esi + 0x307]
        cmp    byte ptr [eax], 0
        jne    loc_2_0A6C
loc_2_0A5B:
        push   0
        add    esi, 0x1fb
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_2_0A6C:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x18]
        lea    edx, [esi + 0xe]
        in     al, dx
        xor    ebx, ebx
        mov    dword ptr [edi], 0
        mov    byte ptr [ebp - 4], al
        and    al, 0x1e
        shr    al, 1
        mov    bl, al
        test   ebx, ebx
        je     loc_2_0BCF
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_0AB9
        mov    dword ptr [edi], 8
loc_2_0AB9:
        push   ebx
        mov    al, byte ptr [ebp + 0x10]
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0BE0
        test   byte ptr [ebp - 4], 1
        je     loc_2_0AD4
        or     dword ptr [edi], 2
        jmp    loc_2_0AE1
loc_2_0AD4:
        cmp    ebx, 1
        je     loc_2_0ADE
        or     dword ptr [edi], 3
        jmp    loc_2_0AE1
loc_2_0ADE:
        or     dword ptr [edi], 1
loc_2_0AE1:
        cmp    ebx, 2
        jb     loc_2_0B06
        test   byte ptr [ebp - 4], 0x20
        je     loc_2_0B06
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    al, 0x12
        cmp    al, 0x12
        je     loc_2_0AFE
        mov    eax, ecx
        and    al, 0x13
        cmp    al, 0x13
        jne    loc_2_0B06
loc_2_0AFE:
        or     ecx, 0x200000
        mov    dword ptr [edi], ecx
loc_2_0B06:
        cmp    ebx, 3
        jb     loc_2_0B52
        cmp    byte ptr [ebp + 0x10], 0
        jne    loc_2_0B4C
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        and    dword ptr [edi], 0xffd0ffb0
        call   sub_2_10C0
        test   eax, eax
        je     loc_2_0B2D
        or     dword ptr [edi], 0x1000000
        jmp    loc_2_0B33
loc_2_0B2D:
        or     dword ptr [edi], 0x400000
loc_2_0B33:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   esi
        push   edx
        call   sub_2_0D30
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [edi], ecx
        jmp    loc_2_0B62
loc_2_0B4C:
        or     dword ptr [edi], 0x800000
loc_2_0B52:
        push   edi
        mov    al, byte ptr [ebp + 0x10]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0EC0
loc_2_0B62:
        cmp    ebx, 4
        jb     loc_2_0BBB
        mov    ecx, dword ptr [edi]
        or     ecx, 0x20
        test   ecx, 0x1000000
        mov    dword ptr [edi], ecx
        je     loc_2_0BBB
        lea    edx, [esi + 7]
        and    ecx, 0xfc3fffff
        in     al, dx
        shr    al, 4
        mov    dword ptr [edi], ecx
        cmp    al, 2
        jne    loc_2_0B9A
        or     ecx, 0x9000000
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0B9A:
        cmp    al, 4
        jne    loc_2_0BAF
        or     ecx, 0x9000800
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0BAF:
        cmp    al, 3
        jne    loc_2_0BBB
        or     ecx, 0x1000000
        mov    dword ptr [edi], ecx
loc_2_0BBB:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1050
        test   al, al
        je     loc_2_0BCF
        or     dword ptr [edi], 0x100000
loc_2_0BCF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        cmp    dword ptr [ebp + 0x18], 3
        mov    esi, dword ptr [ebp + 0xc]
        jb     loc_2_0C11
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_10C0
        test   eax, eax
        je     loc_2_0C11
        mov    edx, dword ptr [ebp + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        or     dword ptr [edx], 0x10
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0C11:
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_0C59
        lea    edi, [esi + 7]
        mov    edx, edi
        in     al, dx
        mov    cl, al
        mov    al, 0x35
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        in     al, dx
        cmp    al, 0x35
        jne    loc_2_0C3F
        mov    al, 0x3a
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        in     al, dx
        cmp    al, 0x3a
        jne    loc_2_0C3F
        mov    edx, dword ptr [ebp + 0x14]
        or     dword ptr [edx], 0x10
loc_2_0C3F:
        mov    edx, dword ptr [ebp + 0x14]
        test   byte ptr [edx], 0x10
        je     loc_2_0D1B
        mov    edx, edi
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    0x14
loc_2_0C59:
        mov    edi, 1
loc_2_0C5E:
        lea    edx, [esi + edi]
        in     al, dx
        mov    cl, al
        add    edx, 8
        in     al, dx
        cmp    al, cl
        jne    loc_2_0C72
        inc    edi
        cmp    edi, 5
        jb     loc_2_0C5E
loc_2_0C72:
        cmp    edi, 5
        jne    loc_2_0C81
        lea    edx, [esi + 7]
        in     al, dx
        mov    cl, al
        lea    edx, [esi + 0xf]
        in     al, dx
loc_2_0C81:
        cmp    al, cl
        je     loc_2_0D1B
        lea    ecx, [esi + 7]
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, 0x35
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ecx
        in     al, dx
        cmp    al, 0x35
        jne    loc_2_0CAE
        mov    al, 0x3a
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ecx
        in     al, dx
        cmp    al, 0x3a
        jne    loc_2_0CAE
        mov    bl, 1
loc_2_0CAE:
        mov    edx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [edx]
        test   edi, 0x10
        je     loc_2_0CC1
        mov    edx, ecx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_2_0CC1:
        test   bl, bl
        je     loc_2_0CD6
        or     edi, 0x10
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0CD6:
        lea    ebx, [esi + 1]
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        xor    al, 1
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, ebx
        in     al, dx
        xor    al, cl
        test   al, 1
        jne    loc_2_0D16
        and    cl, 0xfe
        or     edi, 0x40
        mov    al, cl
        out    dx, al
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], edi
        mov    edx, ebx
        in     al, dx
        test   al, 1
        je     loc_2_0D1B
        or     edi, 0x80000
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D16:
        mov    edx, ebx
        mov    al, cl
        out    dx, al
loc_2_0D1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0D30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    esi, [edi + 1]
        mov    dword ptr [ebx], 0
        call   sub_2_10C0
        mov    edx, esi
        test   eax, eax
        je     loc_2_0D61
        mov    al, 0x4a
        out    dx, al
loc_2_0D58:
        mov    edx, esi
        in     al, dx
        test   al, 0x40
        jne    loc_2_0D58
        jmp    loc_2_0D84
loc_2_0D61:
        in     al, dx
        and    al, 0xc
        or     al, 2
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0xf
        or     al, 0xa0
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0xc
        or     al, 0x12
        out    dx, al
        in     al, dx
        test   al, 0x10
        je     loc_2_0D84
loc_2_0D7D:
        mov    edx, esi
        in     al, dx
        test   al, 0x10
        jne    loc_2_0D7D
loc_2_0D84:
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_2_0D8D
        or     dword ptr [ebx], 8
loc_2_0D8D:
        lea    edx, [edi + 9]
        in     al, dx
        mov    cl, al
        and    cl, 0xc0
        test   cl, 0x40
        je     loc_2_0DA1
        or     dword ptr [ebx], 0x80
loc_2_0DA1:
        test   cl, 0x80
        je     loc_2_0DAC
        or     dword ptr [ebx], 0x100
loc_2_0DAC:
        and    al, 0x18
        cmp    al, 8
        jne    loc_2_0DDA
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        or     dword ptr [ebx], 0x40
        call   sub_2_10C0
        test   eax, eax
        je     loc_2_0DCD
        lea    edx, [edi + 4]
        in     al, dx
        test   al, 0x10
        je     loc_2_0DDA
        jmp    loc_2_0DD4
loc_2_0DCD:
        mov    edx, esi
        in     al, dx
        test   al, 1
        je     loc_2_0DDA
loc_2_0DD4:
        or     dword ptr [ebx], 0x80000
loc_2_0DDA:
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        and    cl, 7
        jne    loc_2_0DEA
        or     dword ptr [ebx], 1
        jmp    loc_2_0E01
loc_2_0DEA:
        cmp    cl, 2
        jne    loc_2_0DF4
        or     dword ptr [ebx], 3
        jmp    loc_2_0E01
loc_2_0DF4:
        cmp    cl, 3
        jne    loc_2_0DFE
        or     dword ptr [ebx], 4
        jmp    loc_2_0E01
loc_2_0DFE:
        or     dword ptr [ebx], 2
loc_2_0E01:
        test   al, 0x18
        je     loc_2_0E0B
        or     dword ptr [ebx], 0x200000
loc_2_0E0B:
        and    al, 0xe0
        cmp    al, 0x40
        jne    loc_2_0E19
        or     dword ptr [ebx], 0x20000
        jmp    loc_2_0E6A
loc_2_0E19:
        cmp    al, 0x60
        jne    loc_2_0E3F
        mov    eax, dword ptr [ebx]
        test   al, 0x40
        jne    loc_2_0E2A
        or     eax, 0x30000
        jmp    loc_2_0E68
loc_2_0E2A:
        test   eax, 0x80000
        jne    loc_2_0E38
        or     eax, 0x20000
        jmp    loc_2_0E68
loc_2_0E38:
        or     eax, 0x30000
        jmp    loc_2_0E68
loc_2_0E3F:
        cmp    al, 0x80
        jne    loc_2_0E4B
        or     dword ptr [ebx], 0x40000
        jmp    loc_2_0E6A
loc_2_0E4B:
        cmp    al, 0xa0
        jne    loc_2_0E6A
        mov    eax, dword ptr [ebx]
        test   al, 0x40
        je     loc_2_0E63
        test   eax, 0x80000
        jne    loc_2_0E63
        or     eax, 0x40000
        jmp    loc_2_0E68
loc_2_0E63:
        or     eax, 0x50000
loc_2_0E68:
        mov    dword ptr [ebx], eax
loc_2_0E6A:
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_10C0
        test   eax, eax
        je     loc_2_0E92
        lea    edx, [edi + 4]
        in     al, dx
        and    al, 0x43
        out    dx, al
        mov    edx, esi
        mov    al, 0x46
        out    dx, al
loc_2_0E84:
        mov    edx, esi
        in     al, dx
        test   al, 0x40
        jne    loc_2_0E84
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_2_0E92:
        mov    edx, esi
        in     al, dx
        and    al, 0xc
        or     al, 2
        out    dx, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0xf
        or     al, 0x80
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0xc
        or     al, 0x10
        out    dx, al
        in     al, dx
        test   al, 0x10
        je     loc_2_0EB7
loc_2_0EB0:
        mov    edx, esi
        in     al, dx
        test   al, 0x10
        jne    loc_2_0EB0
loc_2_0EB7:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_0EC0:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    dword ptr [ebp + 0x14], 2
        jae    loc_2_0F3C
        cmp    byte ptr [ebp + 0x10], 0
        mov    esi, dword ptr [ebp + 0x18]
        je     loc_2_0EDE
        or     dword ptr [esi], 0x30000
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0EDE:
        mov    ecx, dword ptr [esi]
        test   cl, 0x40
        je     loc_2_0F07
        test   ecx, 0x80000
        je     loc_2_0EFA
        or     ecx, 0x30000
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0EFA:
        or     ecx, 0x20000
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F07:
        test   cl, 0x10
        jne    loc_2_0F19
        or     ecx, 0x20000
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F19:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        test   al, 8
        je     loc_2_0F2F
        or     ecx, 0x40000
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F2F:
        or     ecx, 0x20000
        mov    dword ptr [esi], ecx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F3C:
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x18]
        add    edx, 0xe
        in     al, dx
        mov    edx, dword ptr [esi]
        test   dl, 8
        je     loc_2_0F6B
        test   al, 0x40
        je     loc_2_0F5E
        or     edx, 0x50000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F5E:
        or     edx, 0x30000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F6B:
        mov    ecx, edx
        and    ecx, 0xffff
        dec    ecx
        cmp    ecx, 0x41
        ja     loc_2_0FE9
        mov    cl, byte ptr [ecx]            ; -> obj2:0x1000
        and    ecx, 0xff
        jmp    dword ptr [ecx*4]             ; -> obj2:0x0FF0
loc_2_0F8C:
        test   al, 0x40
        je     loc_2_0F9D
        or     edx, 0x40000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0F9D:
        or     edx, 0x20000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0FAA:
        test   edx, 0x80000
        je     loc_2_0FD0
        test   al, 0x40
        je     loc_2_0FC3
        or     edx, 0x50000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0FC3:
        or     edx, 0x30000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0FD0:
        test   al, 0x40
        je     loc_2_0FE1
        or     edx, 0x40000
        mov    dword ptr [esi], edx
        pop    esi
        pop    ebp
        ret    0x14
loc_2_0FE1:
        or     edx, 0x20000
        mov    dword ptr [esi], edx
loc_2_0FE9:
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_0F8C
        dd  loc_2_0F8C
        dd  loc_2_0FAA
        dd  loc_2_0FE9
        db  000h, 000h, 000h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 001h, 001h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 002h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1050:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edi, [esi + 0x10]
        mov    edx, edi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0xfb
        mov    bl, byte ptr [ebp - 1]
        add    esi, 0x1d
        and    bl, 0x3f
        mov    cl, bl
        or     cl, 0x80
        mov    al, cl
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    edx, edi
        mov    byte ptr [ebp - 2], al
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 0x18
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    cl, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 1]
        and    cl, 0x18
        pop    edi
        pop    esi
        cmp    cl, 0x18
        out    dx, al
        pop    ebx
        mov    esp, ebp
        setne  al
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_10C0:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        add    edx, 0x18
        in     al, dx
        push   ebx
        mov    bl, al
        mov    al, 0x55
        out    dx, al
        in     al, dx
        mov    cl, al
        mov    al, bl
        sub    cl, 0x55
        pop    ebx
        pop    ebp
        cmp    cl, 1
        out    dx, al
        sbb    eax, eax
        neg    eax
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_10F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x18]
        cmp    byte ptr [ebp + 0x10], 0
        mov    byte ptr [esi + 0x27], 0
        je     loc_2_1364
        mov    word ptr [esi + 0xc], 1
        mov    ax, word ptr [esi + 0x28]
        mov    word ptr [esi + 0x1c], ax
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    ecx, 0x6fc2
        jg     loc_2_1152
        cmp    ecx, 0x6fc0
        jge    loc_2_12AF
        cmp    ecx, 0x61c8
        jl     loc_2_1184
        cmp    ecx, 0x61c9
        jle    loc_2_1192
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1152:
        cmp    ecx, 0xefd5
        jg     loc_2_1178
        cmp    ecx, 0xefd4
        jge    loc_2_1192
        cmp    ecx, 0x6fc6
        je     loc_2_1192
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1178:
        cmp    ecx, 0xefe5
        je     loc_2_12AF
loc_2_1184:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1192:
        lea    edi, [esi + 0xe]
        mov    cl, byte ptr [esi + 0x2a]
        and    cl, 0xf8
        mov    bl, byte ptr [esi + 0x2d]
        mov    byte ptr [esi + 0x1f], 5
        or     cl, 8
        and    cx, 0xff
        shl    cx, 8
        mov    word ptr [edi], cx
        mov    cl, bl
        and    cl, 0xc
        jne    loc_2_11C0
        mov    word ptr [esi + 0x16], 3
        jmp    loc_2_11DE
loc_2_11C0:
        cmp    cl, 4
        jne    loc_2_11CD
        mov    word ptr [esi + 0x16], 4
        jmp    loc_2_11DE
loc_2_11CD:
        mov    word ptr [esi + 0x16], 0xa
        cmp    cl, 8
        je     loc_2_11DE
        mov    word ptr [esi + 0x16], 0xe
loc_2_11DE:
        mov    cl, byte ptr [esi + 0x2b]
        test   cl, 8
        je     loc_2_1200
        test   cl, 0x80
        mov    dl, cl
        je     loc_2_11F8
        and    edx, 7
        add    edx, 0x7e8
        jmp    loc_2_1218
loc_2_11F8:
        and    edx, 7
        add    edx, 0x68
        jmp    loc_2_1218
loc_2_1200:
        test   cl, 0x80
        mov    dl, cl
        je     loc_2_1212
        and    edx, 7
        add    edx, 0x7e0
        jmp    loc_2_1218
loc_2_1212:
        and    edx, 7
        add    edx, 0x60
loc_2_1218:
        shl    edx, 0xd
        and    cl, 0x30
        shr    cl, 4
        mov    dword ptr [esi + 4], edx
        mov    word ptr [esi + 0x12], 0x40
        mov    dx, 8
        shl    dx, cl
        mov    word ptr [esi + 0x14], dx
        cmp    ax, 0x6fc6
        je     loc_2_123F
        mov    word ptr [esi + 0x12], dx
loc_2_123F:
        mov    al, byte ptr [esi + 0x2c]
        mov    cl, al
        test   al, 8
        je     loc_2_1250
        and    ecx, 7
        add    ecx, 0x68
        jmp    loc_2_1256
loc_2_1250:
        and    ecx, 7
        add    ecx, 0x60
loc_2_1256:
        shl    ecx, 0xd
        shr    al, 4
        mov    dword ptr [esi + 8], ecx
        test   al, al
        jne    loc_2_1274
        mov    word ptr [esi + 0x18], 8
        and    bl, 7
        mov    byte ptr [esi + 0x1e], bl
        jmp    loc_2_1341
loc_2_1274:
        cmp    al, 1
        jne    loc_2_1289
        mov    word ptr [esi + 0x18], 0x10
        and    bl, 7
        mov    byte ptr [esi + 0x1e], bl
        jmp    loc_2_1341
loc_2_1289:
        cmp    al, 2
        jne    loc_2_129E
        mov    word ptr [esi + 0x18], 0x20
        and    bl, 7
        mov    byte ptr [esi + 0x1e], bl
        jmp    loc_2_1341
loc_2_129E:
        mov    word ptr [esi + 0x18], 0
        and    bl, 7
        mov    byte ptr [esi + 0x1e], bl
        jmp    loc_2_1341
loc_2_12AF:
        lea    edi, [esi + 0xe]
        mov    al, byte ptr [esi + 0x2a]
        mov    byte ptr [esi + 0x1f], 4
        and    ax, 0xfe
        shl    ax, 4
        mov    word ptr [edi], ax
        mov    al, byte ptr [esi + 0x2d]
        and    al, 3
        jne    loc_2_12D3
        mov    word ptr [esi + 0x16], 3
        jmp    loc_2_12EF
loc_2_12D3:
        cmp    al, 1
        jne    loc_2_12DF
        mov    word ptr [esi + 0x16], 4
        jmp    loc_2_12EF
loc_2_12DF:
        mov    word ptr [esi + 0x16], 0xa
        cmp    al, 2
        je     loc_2_12EF
        mov    word ptr [esi + 0x16], 0xf
loc_2_12EF:
        mov    al, byte ptr [esi + 0x2b]
        mov    edx, 0x10
        mov    word ptr [esi + 0x12], dx
        and    eax, 0xfc
        shl    eax, 0xc
        mov    word ptr [esi + 0x14], dx
        mov    dword ptr [esi + 4], eax
        mov    al, byte ptr [esi + 0x2c]
        mov    cl, al
        and    ecx, 0xfc
        shl    ecx, 0xc
        and    al, 3
        mov    dword ptr [esi + 8], ecx
        jne    loc_2_1325
        mov    word ptr [esi + 0x18], dx
        jmp    loc_2_1341
loc_2_1325:
        cmp    al, 1
        jne    loc_2_1331
        mov    word ptr [esi + 0x18], 0x20
        jmp    loc_2_1341
loc_2_1331:
        mov    word ptr [esi + 0x18], 0
        cmp    al, 2
        je     loc_2_1341
        mov    word ptr [esi + 0x18], 0x40
loc_2_1341:
        push   esi
        mov    al, byte ptr [ebp + 0x14]
        push   eax
        mov    cl, byte ptr [ebp + 0x10]
        push   ecx
        xor    edx, edx
        mov    dx, word ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_2_0A80
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1364:
        xor    bx, bx
        xor    ecx, ecx
        mov    word ptr [esi + 0xc], cx
        xor    edi, edi
        mov    di, word ptr [esi + 0xe]
loc_2_1373:
        mov    eax, ebx
        inc    bx
        and    eax, 0xffff
        lea    edx, [eax + edi + 8]
        in     al, dx
        add    cl, al
        cmp    bx, 8
        jb     loc_2_1373
        cmp    cl, 0xff
        je     loc_2_139C
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_139C:
        push   esi
        mov    al, byte ptr [ebp + 0x14]
        push   eax
        mov    cl, byte ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edi
        push   edx
        call   sub_2_0A80
        mov    eax, dword ptr [esi]
        and    eax, 0x70000
        cmp    eax, 0x30000
        ja     loc_2_13C8
        je     loc_2_13E0
        cmp    eax, 0x20000
        je     loc_2_13D8
        jmp    loc_2_13F6
loc_2_13C8:
        cmp    eax, 0x40000
        je     loc_2_13E8
        cmp    eax, 0x50000
        je     loc_2_13F0
        jmp    loc_2_13F6
loc_2_13D8:
        mov    word ptr [esi + 0x12], 8
        jmp    loc_2_13F6
loc_2_13E0:
        mov    word ptr [esi + 0x12], 0x10
        jmp    loc_2_13F6
loc_2_13E8:
        mov    word ptr [esi + 0x12], 0x20
        jmp    loc_2_13F6
loc_2_13F0:
        mov    word ptr [esi + 0x12], 0x40
loc_2_13F6:
        mov    ax, word ptr [esi + 0x12]
        test   byte ptr [esi], 0x10
        mov    word ptr [esi + 0x14], ax
        jne    loc_2_1450
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xe]
        mov    byte ptr [esi + 0x1f], 0
        lea    edx, [ecx + 8]
        in     al, dx
        test   al, al
        je     loc_2_1423
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1423:
        lea    edx, [ecx + 9]
        in     al, dx
        test   al, al
        je     loc_2_1439
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1439:
        lea    edx, [ecx + 0xa]
        pop    edi
        in     al, dx
        sub    al, 0xc0
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    eax, eax
        mov    esp, ebp
        and    eax, 2
        pop    ebp
        dec    eax
        ret    0x14
loc_2_1450:
        mov    edi, dword ptr [esi]
        and    edi, 0x3c00000
        jne    loc_2_1460
        mov    byte ptr [esi + 0x1f], 1
        jmp    loc_2_1470
loc_2_1460:
        mov    byte ptr [esi + 0x1f], 3
        cmp    edi, 0x1000000
        je     loc_2_1470
        mov    byte ptr [esi + 0x1f], 2
loc_2_1470:
        xor    cl, cl
        cmp    edi, 0x1000000
        jne    loc_2_14E1
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe]
        lea    edx, [ebx + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xc3
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xd]
        in     al, dx
        mov    cl, al
        lea    edx, [ebx + 4]
        and    cl, 0xc
        and    al, 0x40
        shr    cl, 2
        shr    al, 4
        or     cl, al
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        test   byte ptr [esi], 0x40
        lea    edx, [esi + 0x16]
        je     loc_2_14CA
        xor    eax, eax
        mov    al, cl
        mov    al, byte ptr [eax]            ; -> obj1:0x35B4
        and    eax, 0xffff00ff
        mov    word ptr [edx], ax
        jmp    loc_2_1576
loc_2_14CA:
        xor    eax, eax
        mov    al, cl
        mov    al, byte ptr [eax]            ; -> loc_1_35AC
        and    eax, 0xffff00ff
        mov    word ptr [edx], ax
        jmp    loc_2_1576
loc_2_14E1:
        test   edi, edi
        je     loc_2_14F2
        xor    edx, edx
        mov    dx, word ptr [esi + 0xe]
        inc    edx
        in     al, dx
        mov    cl, al
        and    cl, 4
loc_2_14F2:
        xor    edx, edx
        mov    dx, word ptr [esi + 0xe]
        add    edx, 4
        in     al, dx
        and    al, 0x60
        shr    al, 5
        test   al, al
        jne    loc_2_151A
        lea    edx, [esi + 0x16]
        test   cl, cl
        jne    loc_2_1513
        mov    word ptr [edx], 2
        jmp    loc_2_1576
loc_2_1513:
        mov    word ptr [edx], 0xa
        jmp    loc_2_1576
loc_2_151A:
        cmp    al, 1
        jne    loc_2_152C
        lea    edx, [esi + 0x16]
        test   cl, cl
        je     loc_2_1571
        mov    word ptr [edx], 0xb
        jmp    loc_2_1576
loc_2_152C:
        cmp    al, 2
        jne    loc_2_1555
        test   cl, cl
        jne    loc_2_154B
        test   byte ptr [esi + 2], 0x20
        lea    edx, [esi + 0x16]
        je     loc_2_1544
        mov    word ptr [edx], 5
        jmp    loc_2_1576
loc_2_1544:
        mov    word ptr [edx], 4
        jmp    loc_2_1576
loc_2_154B:
        lea    edx, [esi + 0x16]
        mov    word ptr [edx], 0xf
        jmp    loc_2_1576
loc_2_1555:
        cmp    al, 3
        jne    loc_2_156E
        lea    edx, [esi + 0x16]
        test   cl, cl
        jne    loc_2_1567
        mov    word ptr [edx], 7
        jmp    loc_2_1576
loc_2_1567:
        mov    word ptr [edx], 4
        jmp    loc_2_1576
loc_2_156E:
        lea    edx, [esi + 0x16]
loc_2_1571:
        mov    word ptr [edx], 3
loc_2_1576:
        mov    cx, word ptr [esi + 0x1a]
        and    cx, 0x7fff
        cmp    edi, 0x1000000
        mov    word ptr [esi + 0x1a], cx
        jne    loc_2_15AC
        cmp    word ptr [edx], 0
        jne    loc_2_15A5
        xor    edx, edx
        mov    dx, word ptr [esi + 0xe]
        add    edx, 4
        in     al, dx
        and    eax, 0xff
        test   ah, 0x80
        je     loc_2_15AC
loc_2_15A5:
        or     ch, 0x80
        mov    word ptr [esi + 0x1a], cx
loc_2_15AC:
        mov    dx, word ptr [esi + 0xe]
        in     al, dx
        mov    cl, al
        and    cl, 0x3f
        test   edi, edi
        jne    loc_2_15C6
        or     cl, 0x40
        xor    eax, eax
        mov    al, cl
        jmp    loc_2_165D
loc_2_15C6:
        cmp    edi, 0x1000000
        jne    loc_2_163A
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe]
        lea    edx, [ebx + 4]
        in     al, dx
        mov    ecx, edx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xc3
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        mov    word ptr [ebp - 8], 8
        lea    edx, [ebx + 0xb]
        in     al, dx
        mov    cl, al
        in     al, dx
        and    cl, 0x30
        shr    cl, 4
        mov    dl, al
        shl    word ptr [ebp - 8], cl
        mov    cx, word ptr [ebp - 8]
        and    dl, 0xf
        mov    word ptr [esi + 0x14], cx
        mov    cl, al
        and    cl, 0xc3
        shr    cl, 2
        or     cl, dl
        or     cl, 0x60
        and    ecx, 0xff
        shl    ecx, 0xd
        test   al, 0x80
        mov    dword ptr [esi + 4], ecx
        je     loc_2_1631
        or     ecx, 0xf00000
        mov    dword ptr [esi + 4], ecx
loc_2_1631:
        lea    edx, [ebx + 4]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    loc_2_1663
loc_2_163A:
        xor    edx, edx
        mov    dx, word ptr [esi + 0xe]
        add    edx, 5
        in     al, dx
        and    al, 0x1f
        mov    dl, cl
        shl    al, 3
        and    dl, 0x38
        shr    dl, 3
        xor    ebx, ebx
        or     al, dl
        and    ecx, 7
        mov    bl, al
        lea    eax, [ecx + ebx*8]
loc_2_165D:
        shl    eax, 0xd
        mov    dword ptr [esi + 4], eax
loc_2_1663:
        cmp    edi, 0x1000000
        jne    loc_2_16A6
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe]
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xc3
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xc]
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [ebp - 8]
        and    cl, 0xc3
        and    al, 0xf
        shr    cl, 2
        or     cl, al
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        or     cl, 0x60
        jmp    loc_2_16B8
loc_2_16A6:
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe]
        lea    edx, [ebx + 3]
        in     al, dx
        mov    cl, al
        and    cl, 0x3e
        or     cl, 0x40
loc_2_16B8:
        xor    eax, eax
        mov    al, cl
        shl    eax, 0xd
        cmp    edi, 0x1000000
        mov    dword ptr [esi + 8], eax
        jne    loc_2_1710
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xc3
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xc]
        in     al, dx
        mov    cl, al
        and    cl, 0x30
        cmp    cl, 0x30
        jne    loc_2_16F9
        mov    word ptr [esi + 0x18], 0
        jmp    loc_2_1707
loc_2_16F9:
        shr    cl, 4
        mov    ax, 8
        shl    ax, cl
        mov    word ptr [esi + 0x18], ax
loc_2_1707:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    loc_2_1731
loc_2_1710:
        lea    edx, [ebx + 3]
        in     al, dx
        mov    cl, al
        and    cl, 0xc0
        jne    loc_2_1723
        mov    word ptr [esi + 0x18], 0
        jmp    loc_2_1731
loc_2_1723:
        shr    cl, 6
        mov    ax, 8
        shl    ax, cl
        mov    word ptr [esi + 0x18], ax
loc_2_1731:
        mov    cx, word ptr [esi + 0x1a]
        lea    edx, [ebx + 7]
        in     al, dx
        and    cx, 0x9fff
        test   al, 0xf
        mov    word ptr [esi + 0x1a], cx
        je     loc_2_174D
        or     ch, 0x20
        mov    word ptr [esi + 0x1a], cx
loc_2_174D:
        mov    cx, word ptr [esi + 0x1a]
        lea    eax, [ebx + 4]
        and    cx, 0xe7ff
        mov    edx, eax
        mov    word ptr [esi + 0x1a], cx
        cmp    edi, 0x1000000
        mov    dword ptr [ebp - 8], eax
        in     al, dx
        jne    loc_2_1793
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0xc3
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xd]
        in     al, dx
        test   al, 0x20
        je     loc_2_178A
        or     ch, 0x18
        mov    word ptr [esi + 0x1a], cx
loc_2_178A:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    loc_2_17AF
loc_2_1793:
        test   al, 1
        je     loc_2_179E
        or     ch, 0x10
        mov    word ptr [esi + 0x1a], cx
loc_2_179E:
        test   byte ptr [esi], 0x40
        je     loc_2_17AF
        lea    edx, [ebx + 5]
        in     al, dx
        test   al, 0x20
        je     loc_2_17AF
        or     byte ptr [esi + 0x1b], 8
loc_2_17AF:
        mov    cx, word ptr [esi + 0x1a]
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        and    cx, 0xf8ff
        cmp    byte ptr [ebp + 0x14], 0
        mov    word ptr [esi + 0x1a], cx
        je     loc_2_1812
        and    al, 6
        mov    dl, 1
        cmp    dl, al
        mov    edx, 1
        adc    edx, -1
        cmp    edx, 3
        jne    loc_2_17E5
        or     cl, 6
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_18A5
loc_2_17E5:
        cmp    edx, 2
        jne    loc_2_17F6
        or     cl, 4
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_18A5
loc_2_17F6:
        cmp    al, 1
        jbe    loc_2_1806
        or     cl, 7
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_18A5
loc_2_1806:
        or     cl, 5
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_18A5
loc_2_1812:
        cmp    edi, 0x1000000
        jne    loc_2_1873
        cmp    byte ptr [esi + 0x1f], 3
        je     loc_2_1826
        mov    byte ptr [esi + 0x1e], 0xff
        jmp    loc_2_18A5
loc_2_1826:
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    byte ptr [ebp - 4], 0x43
        mov    al, byte ptr [ebp - 4]
        or     al, 0x80
        out    dx, al
        lea    edx, [ebx + 0xd]
        in     al, dx
        and    al, 3
        cmp    al, 1
        jne    loc_2_184E
        mov    byte ptr [esi + 0x1e], 1
        or     ch, 2
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_186A
loc_2_184E:
        cmp    al, 3
        jne    loc_2_185F
        mov    byte ptr [esi + 0x1e], 2
        or     ch, 4
        mov    word ptr [esi + 0x1a], cx
        jmp    loc_2_186A
loc_2_185F:
        mov    byte ptr [esi + 0x1e], 0
        or     ch, 1
        mov    word ptr [esi + 0x1a], cx
loc_2_186A:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        jmp    loc_2_18A5
loc_2_1873:
        test   byte ptr [esi], 0x20
        je     loc_2_187F
        or     ch, 0x10
        mov    word ptr [esi + 0x1a], cx
loc_2_187F:
        and    al, 6
        cmp    al, 2
        jne    loc_2_188F
        or     byte ptr [esi + 0x1b], 1
        mov    byte ptr [esi + 0x1e], 0
        jmp    loc_2_18A5
loc_2_188F:
        cmp    al, 4
        jne    loc_2_189D
        or     byte ptr [esi + 0x1b], 4
        mov    byte ptr [esi + 0x1e], 2
        jmp    loc_2_18A5
loc_2_189D:
        or     byte ptr [esi + 0x1b], 2
        mov    byte ptr [esi + 0x1e], 1
loc_2_18A5:
        test   byte ptr [esi + 3], 8
        je     loc_2_1923
        lea    ecx, [ebx + 1]
        mov    al, 0x4a
        mov    edx, ecx
        out    dx, al
        in     al, dx
        test   al, 0x40
        je     loc_2_18BF
loc_2_18B8:
        mov    edx, ecx
        in     al, dx
        test   al, 0x40
        jne    loc_2_18B8
loc_2_18BF:
        test   byte ptr [esi + 1], 8
        je     loc_2_18D3
        lea    edx, [ebx + 0xb]
        in     al, dx
        test   al, 0x10
        je     loc_2_18D3
        mov    byte ptr [esi + 0x27], 1
        jmp    loc_2_18D7
loc_2_18D3:
        mov    byte ptr [esi + 0x27], 0
loc_2_18D7:
        lea    edx, [ebx + 0xb]
        in     al, dx
        test   al, 8
        je     loc_2_18E8
        mov    eax, dword ptr [esi]
        or     eax, 0x400
        mov    dword ptr [esi], eax
loc_2_18E8:
        lea    edx, [ebx + 0xc]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        test   al, al
        mov    word ptr [esi + 0x21], dx
        je     loc_2_1906
        test   al, 4
        jne    loc_2_1906
        or     dword ptr [esi], 0x4000
loc_2_1906:
        lea    edx, [ebx + 0xe]
        in     al, dx
        test   al, 1
        je     loc_2_1912
        or     byte ptr [esi + 0x1a], 8
loc_2_1912:
        mov    edx, ecx
        mov    al, 0x46
        out    dx, al
        in     al, dx
        test   al, 0x40
        je     loc_2_1923
loc_2_191C:
        mov    edx, ecx
        in     al, dx
        test   al, 0x40
        jne    loc_2_191C
loc_2_1923:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
sub_2_1930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 8]
        lea    edi, [esi + 0x56]
        push   edi
        push   eax
        push   ecx
        push   edx
        call   sub_1_1BE8
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_196A
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        mov    word ptr [eax], 0
        pop    esi
        mov    ax, 0xffff
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_196A:
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 1], al
        xor    eax, eax
        mov    ax, word ptr [edi]
        cmp    eax, 0x6fc2
        jg     loc_2_1993
        cmp    eax, 0x6fc0
        jge    loc_2_19E9
        cmp    eax, 0x61c8
        jl     loc_2_19B1
        cmp    eax, 0x61c9
        jle    loc_2_19C5
        jmp    loc_2_19B1
loc_2_1993:
        cmp    eax, 0xefd5
        jg     loc_2_19AA
        cmp    eax, 0xefd4
        jge    loc_2_19C5
        cmp    eax, 0x6fc6
        je     loc_2_19C5
        jmp    loc_2_19B1
loc_2_19AA:
        cmp    eax, 0xefe5
        je     loc_2_19E9
loc_2_19B1:
        mov    eax, dword ptr [ebp + 0x10]
        pop    edi
        mov    word ptr [eax], 0
        pop    esi
        mov    ax, 0xffff
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_19C5:
        mov    al, byte ptr [esi + 0x58]
        and    al, 0xf8
        or     al, 8
        pop    edi
        mov    cl, al
        pop    esi
        and    ecx, 0xffff00ff
        mov    eax, dword ptr [ebp + 0x10]
        shl    cx, 8
        mov    word ptr [eax], cx
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    0xc
loc_2_19E9:
        mov    cl, byte ptr [esi + 0x58]
        mov    eax, dword ptr [ebp + 0x10]
        and    cx, 0xfe
        pop    edi
        shl    cx, 4
        pop    esi
        mov    word ptr [eax], cx
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 0x16], bx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 0x11], bl
        mov    byte ptr [ebp - 0x10], bl
        mov    cx, word ptr [esi + 4]
        mov    al, byte ptr [esi]
        mov    word ptr [ebp - 0x22], cx
        and    eax, 0xffff00ff
        mov    word ptr [ebp - 0x24], ax
        mov    dl, byte ptr [esi + 1]
        and    edx, 0xffff00ff
        lea    eax, [esi + 0x56]
        mov    word ptr [ebp - 0x20], dx
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        lea    edx, [ebp - 8]
        mov    dword ptr [ebp - 0x2c], 0xd0000
        push   esi
        mov    dword ptr [edx], ecx
        mov    word ptr [ebp - 0x1a], 3
        mov    word ptr [ebp - 0x1e], 8
        mov    word ptr [edx + 4], ax
        call   sub_2_1D90
        lea    ecx, [ebp - 0x30]
        mov    dl, byte ptr [esi + 0x5c]
        push   ecx
        mov    al, byte ptr [esi]
        push   edx
        dec    al
        cmp    al, 1
        mov    edx, dword ptr [esi + 0x52]
        sbb    al, al
        xor    ecx, ecx
        neg    al
        mov    cx, word ptr [esi + 4]
        push   eax
        push   ecx
        push   edx
        call   sub_2_10F0
        mov    edi, eax
        push   esi
        mov    word ptr [esi + 0x3a], 1
        mov    word ptr [esi + 0xcd], 0x600
        call   sub_2_1D30
        mov    ax, word ptr [ebp - 0x22]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dx, word ptr [ebp - 0x1a]
        mov    dword ptr [esi + 0xc5], ecx
        mov    word ptr [esi + 4], ax
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    word ptr [esi + 0xa], dx
        mov    dword ptr [esi + 6], ebx
        mov    dword ptr [esi + 0x18], ecx
        mov    ax, word ptr [ebp - 0x1e]
        mov    dx, word ptr [ebp - 0x1c]
        mov    word ptr [esi + 0x16], ax
        mov    word ptr [esi + 0x20], dx
        cmp    ax, dx
        jae    loc_2_1AE8
        mov    word ptr [esi + 0x20], ax
loc_2_1AE8:
        mov    cx, word ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x28]
        mov    word ptr [esi + 0xc], cx
        mov    dl, byte ptr [ebp - 0x12]
        mov    cx, word ptr [ebp - 0x16]
        and    edx, 0xffff00ff
        mov    word ptr [esi + 0x4a], dx
        mov    dword ptr [esi + 0xe], eax
        mov    word ptr [esi + 0xc9], cx
        mov    al, byte ptr [ebp - 0x20]
        mov    dl, byte ptr [ebp - 0x11]
        mov    byte ptr [esi + 1], al
        mov    cx, word ptr [ebp - 0x14]
        and    edx, 0xffff00ff
        mov    word ptr [esi + 0x4c], dx
        mov    al, byte ptr [ebp - 0x10]
        mov    dx, word ptr [ebp - 0xf]
        and    eax, 0xffff00ff
        mov    word ptr [esi + 0x4e], ax
        test   edi, edi
        mov    word ptr [esi + 2], cx
        mov    al, byte ptr [ebp - 9]
        mov    word ptr [esi + 0x3e], dx
        mov    byte ptr [esi + 0x146], al
        jne    loc_2_1B57
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1B57:
        dec    edi
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        and    eax, 0xffff0003
        pop    ebx
        add    eax, 0xffff
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1B70:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 0x3c], 4
        mov    ebp, esp
        jae    loc_2_1B94
loc_2_1B8E:
        mov    word ptr [eax + 0x3c], 4
loc_2_1B94:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [eax + 0x3c]
        add    edx, 0x18
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1D90
        mov    eax, 0xffffffff
        xor    ecx, ecx
        mov    word ptr [esi + 0xf2], ax
        mov    byte ptr [esi + 0xf4], cl
        mov    word ptr [esi + 0xee], cx
        mov    byte ptr [esi + 0x139], cl
        test   byte ptr [esi + 0xc8], 8
        mov    word ptr [esi + 0xf0], ax
        je     loc_2_1BF8
        and    word ptr [esi + 0x48], 0xfbff
        jmp    loc_2_1BFE
loc_2_1BF8:
        mov    word ptr [esi + 0x48], 0x400
loc_2_1BFE:
        mov    byte ptr [esi + 0x11d], 3
        mov    dword ptr [esi + 0x119], 0x180
        xor    eax, eax
        cmp    word ptr [esi + 0x3a], ax
        jbe    loc_2_1C32
        lea    ecx, [esi + 0xfd]
        xor    edx, edx
loc_2_1C1F:
        mov    dword ptr [ecx], edx
        add    ecx, 4
        inc    eax
        xor    edi, edi
        mov    dword ptr [ecx - 0xc], edx
        mov    di, word ptr [esi + 0x3a]
        cmp    edi, eax
        ja     loc_2_1C1F
loc_2_1C32:
        mov    edx, dword ptr [esi + 0x1c]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xcd]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x3a]
        xor    edi, edi
        imul   eax, ecx
        mov    di, word ptr [esi + 0x16]
        add    eax, edx
        mov    ecx, edi
        mov    dword ptr [esi + 0x120], eax
        shl    ecx, 0xa
        sub    eax, edx
loc_2_1C5C:
        mov    byte ptr [esi + 0xed], 0
        lea    ebx, [edx + ecx]
        and    edi, 0x3fffff
        mov    dword ptr [esi + 0x124], ebx
        xor    ebx, ebx
loc_2_1C74:
        cdq    
        mov    bx, word ptr [esi + 0x38]
        idiv   ebx
        mov    byte ptr [esi + 0x13a], al
        cmp    edi, 0x40
        jne    loc_2_1C8F
        mov    byte ptr [esi + 0x13b], 0xff
        jmp    loc_2_1CA0
loc_2_1C8F:
        mov    eax, ecx
        cdq    
        and    edx, 0xff
loc_2_1C98:
        add    eax, edx
        mov    byte ptr [esi + 0x13b], ah
loc_2_1CA0:
        xor    eax, eax
        mov    dword ptr [esi + 0x13c], eax
        mov    dword ptr [esi + 0x109], eax
        mov    dword ptr [esi + 0x10d], eax
        mov    dword ptr [esi + 0x111], eax
        mov    dword ptr [esi + 0x115], eax
        mov    eax, dword ptr [esi + 0xc5]
        test   al, 8
        jne    loc_2_1CE0
        test   eax, 0x8000000
        je     loc_2_1CE0
        push   esi
        call   sub_2_1B80
        mov    word ptr [esi + 0x11e], 0x187
loc_2_1CE0:
        push   esi
        call   sub_1_2100
        test   al, al
        jne    loc_2_1CFC
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0xd1], 4
        pop    esi
        pop    ebx
loc_2_1CF8:
        pop    ebp
        ret    4
loc_2_1CFC:
        push   esi
        call   sub_1_3000
        push   esi
        call   sub_1_2100
        test   al, al
        jne    loc_2_1D1E
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0xd1], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1D1E:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xd1], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_2_1D30:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0xd1], 1
        je     loc_2_1D49
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_2_1D49:
        test   byte ptr [ecx + 0xc8], 8
        lea    esi, [ecx + 4]
        je     loc_2_1D61
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi]
        add    edx, 0x10
        jmp    loc_2_1D6B
loc_2_1D61:
        xor    edx, edx
        mov    al, 0x20
        mov    dx, word ptr [esi]
        add    edx, 0x10
loc_2_1D6B:
        out    dx, al
        xor    edx, edx
        mov    al, byte ptr [ecx + 0xd3]
        mov    dx, word ptr [esi]
        pop    esi
        add    edx, 0x1f
        pop    ebp
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1D90:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [ecx + 0xc8], 8
        je     loc_2_1DAF
        add    ecx, 4
        xor    edx, edx
        mov    dx, word ptr [ecx]
        xor    al, al
        add    edx, 0x10
        jmp    loc_2_1DBC
loc_2_1DAF:
        add    ecx, 4
        xor    edx, edx
        mov    dx, word ptr [ecx]
        mov    al, 0x20
        add    edx, 0x10
loc_2_1DBC:
        out    dx, al
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [ecx]
        pop    ebp
        add    edx, 0x1f
        out    dx, al
        xor    ax, ax
        ret    4
        db  090h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 080h, 0A0h, 0D3h, 000h, 000h, 000h, 0F5h, 066h
        db  033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
sub_2_1DF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0xd3]
        and    cl, 0xca
        test   byte ptr [eax + 0xc8], 8
        mov    byte ptr [eax + 0xd3], cl
        je     loc_2_1E1E
        test   byte ptr [eax + 0x49], 4
        jne    loc_2_1E1E
        and    cl, 0xbf
        mov    byte ptr [eax + 0xd3], cl
loc_2_1E1E:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 080h, 088h, 0D3h, 000h, 000h, 000h, 00Ah, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0xd3]
        or     cl, 0x35
        test   byte ptr [eax + 0xc8], 8
        mov    byte ptr [eax + 0xd3], cl
        je     loc_2_1E7E
        test   byte ptr [eax + 0x49], 4
        jne    loc_2_1E7E
        or     cl, 0x40
        mov    byte ptr [eax + 0xd3], cl
loc_2_1E7E:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [edi + 0xd1]
        cmp    al, 1
        jne    loc_2_1EAB
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1EAB:
        cmp    al, 3
        jne    loc_2_1EBA
        push   edi
        call   sub_2_1BB0
        test   ax, ax
        jne    loc_2_1F35
loc_2_1EBA:
        cmp    byte ptr [edi + 0xd1], 2
        je     loc_2_1ECD
        mov    ax, 7
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1ECD:
        test   byte ptr [edi + 0xc8], 8
        lea    esi, [edi + 4]
        je     loc_2_1EE5
        xor    edx, edx
        mov    al, 2
        mov    dx, word ptr [esi]
        add    edx, 0x10
        jmp    loc_2_1EEF
loc_2_1EE5:
        xor    edx, edx
        mov    al, 0x22
        mov    dx, word ptr [esi]
        add    edx, 0x10
loc_2_1EEF:
        out    dx, al
        test   byte ptr [edi + 0xc9], 0x10
        je     loc_2_1F05
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi]
        add    edx, 0x1d
        jmp    loc_2_1F0F
loc_2_1F05:
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi]
        add    edx, 0x1d
loc_2_1F0F:
        out    dx, al
        mov    byte ptr [edi + 0xd1], 1
        push   edi
        call   sub_2_1D30
        test   byte ptr [edi + 0xc8], 8
        je     loc_2_1F32
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 6
        in     al, dx
        or     al, 1
        out    dx, al
loc_2_1F32:
        xor    ax, ax
loc_2_1F35:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xd1], 1
        je     loc_2_1F5B
        mov    ax, 0xa
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_1F5B:
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_1F71
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 6
        in     al, dx
        and    al, 0xfe
        out    dx, al
loc_2_1F71:
        push   esi
        call   sub_2_1D90
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_1F8D
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
        jmp    loc_2_1F98
loc_2_1F8D:
        xor    edx, edx
        mov    al, 0x21
        mov    dx, word ptr [esi + 4]
        add    edx, 0x10
loc_2_1F98:
        out    dx, al
        mov    edi, 0x40
loc_2_1F9E:
        push   0x3e8
        call   sub_1_1C68
        dec    edi
        jne    loc_2_1F9E
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xd1], 3
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    edx, edx
        push   ebx
        push   esi
        xor    bl, bl
        push   edi
        mov    al, 0x20
        mov    edi, dword ptr [ebp + 8]
        mov    esi, 0
        mov    dx, word ptr [edi + 4]
        add    edx, 0x10
        out    dx, al
        mov    eax, dword ptr [edi + 0x147]
        test   eax, eax
        je     loc_2_1FEC
        mov    esi, dword ptr [eax + 0xc]
loc_2_1FEC:
        test   esi, 6
        jne    loc_2_2014
        test   dword ptr [edi + 0xc5], 0x8100000
        jne    loc_2_2034
        push   edi
        lea    eax, [edi + 0xd5]
        push   0                             ; -> loc_2_2C50
        push   eax
        call   sub_1_1C62
        jmp    loc_2_2034
loc_2_2014:
        test   dword ptr [edi + 0xc5], 0x8100000
        jne    loc_2_2032
        push   edi
        lea    eax, [edi + 0xd5]
        push   0                             ; -> loc_2_2C10
        push   eax
        call   sub_1_1C62
loc_2_2032:
        mov    bl, 8
loc_2_2034:
        test   esi, 8
        je     loc_2_203F
        or     bl, 4
loc_2_203F:
        test   esi, 0x20
        je     loc_2_204A
        or     bl, 0x1c
loc_2_204A:
        test   byte ptr [edi + 0xc8], 8
        je     loc_2_2056
        or     bl, 1
loc_2_2056:
        xor    edx, edx
        mov    al, bl
        mov    dx, word ptr [edi + 4]
        pop    edi
        add    edx, 0x1c
        pop    esi
        out    dx, al
        xor    ax, ax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 066h, 0B8h, 080h, 000h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_2_2080:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    edx, edx
        push   ebx
        mov    byte ptr [ebp - 2], 1
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        in     al, dx
        and    al, 0x75
        je     loc_2_2351
        xor    ebx, ebx
        mov    byte ptr [ebp - 2], bl
loc_2_20A7:
        test   al, 0x10
        je     loc_2_2155
        mov    byte ptr [esi + 0x138], 1
        test   dword ptr [esi + 0xc5], 0x8100000
        jne    loc_2_214A
        xor    ecx, ecx
        mov    al, 0x21
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x10]
        out    dx, al
        lea    edx, [ecx + 0x1a]
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 0x1b]
        out    dx, al
        lea    edx, [ecx + 0x17]
        in     al, dx
        test   al, 0x80
        jne    loc_2_20F6
        mov    edi, 0x400
loc_2_20E9:
        push   0x3e8
        call   sub_1_1C68
        dec    edi
        jne    loc_2_20E9
loc_2_20F6:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        cmp    byte ptr [esi + 0xf4], bl
        lea    edx, [ecx + 0x1d]
        out    dx, al
        lea    edx, [ecx + 0x10]
        mov    al, 0x22
        out    dx, al
        je     loc_2_214A
        mov    byte ptr [esi + 0xf4], bl
        mov    byte ptr [esi + 0x31a], bl
        movsx  eax, word ptr [esi + 0xf2]
        mov    dword ptr [esi + eax*4 + 0xfd], ebx
        mov    word ptr [esi + 0xf2], 0xffff
        cmp    word ptr [esi + 0xf0], -1
        je     loc_2_2143
        push   esi
        call   sub_1_2490
loc_2_2143:
        push   esi
        push   ebx
        call   sub_1_2D80
loc_2_214A:
        push   esi
        call   sub_1_2900
        jmp    loc_2_233F
loc_2_2155:
        test   al, 0x20
        je     loc_2_21AE
        xor    edx, edx
        mov    ecx, dword ptr [esi + 0x9d]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1d
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0xff
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 0x69]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1e
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0xff
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 0x6d]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1f
        in     al, dx
        and    eax, 0xff
        xor    edx, edx
        add    dword ptr [ecx], eax
        mov    al, 0x20
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        jmp    loc_2_233E
loc_2_21AE:
        test   al, 1
        je     loc_2_21BD
        push   esi
        call   sub_1_2900
        jmp    loc_2_233F
loc_2_21BD:
        test   al, 0x40
        je     loc_2_22D8
        test   byte ptr [esi + 0x49], 4
        je     loc_2_21DB
        xor    edx, edx
        mov    al, 0x40
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        jmp    loc_2_233E
loc_2_21DB:
        cmp    byte ptr [esi + 0x146], bl
        je     loc_2_220E
        push   esi
        lea    eax, [esi + 0xd5]
        push   0                             ; -> sub_2_1D90
        push   eax
        call   sub_1_1C62
        push   ebx
        lea    eax, [ebp - 8]
        push   2
        push   eax
        mov    eax, dword ptr [esi + 0x13c]
        add    eax, 0x10
        push   eax
        push   esi
        call   sub_1_2C10
        jmp    loc_2_225D
loc_2_220E:
        mov    ecx, dword ptr [esi + 0xc5]
        and    ecx, 0x8000000
        je     loc_2_2235
        xor    edi, edi
        mov    al, byte ptr [esi + 0x144]
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 5]
        out    dx, al
        mov    edx, edi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2235:
        mov    eax, dword ptr [esi + 0x13c]
        test   ecx, ecx
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ebp - 8], ax
        je     loc_2_225D
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_2_225D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebp - 8], ecx
        cmp    ecx, 0x5dc
        jbe    loc_2_2288
        xor    edx, edx
        mov    al, 0x40
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        jmp    loc_2_233E
loc_2_2288:
        add    dword ptr [ebp - 8], 0x12
        cmp    dword ptr [ebp - 8], 0x40
        jae    loc_2_2299
        mov    dword ptr [ebp - 8], 0x40
loc_2_2299:
        cmp    dword ptr [ebp - 8], 0x100
        jae    loc_2_22A7
        mov    cl, byte ptr [esi + 0x3c]
        jmp    loc_2_22B0
loc_2_22A7:
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ecx + ecx*2]
        shr    ecx, 4
loc_2_22B0:
        xor    edi, edi
        mov    al, 0x40
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 0x17]
        out    dx, al
        lea    edx, [edi + 0x18]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x17]
        in     al, dx
        test   al, 1
        je     loc_2_22D2
        push   esi
        call   sub_1_2900
        jmp    loc_2_233F
loc_2_22D2:
        test   al, 4
        je     loc_2_233F
        jmp    loc_2_22DC
loc_2_22D8:
        test   al, 4
        je     loc_2_233F
loc_2_22DC:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1c
        in     al, dx
        test   al, 2
        mov    cl, al
        je     loc_2_22F1
        mov    eax, dword ptr [esi + 0x69]
        inc    dword ptr [eax]
loc_2_22F1:
        test   cl, 4
        je     loc_2_230D
        xor    edx, edx
        mov    edi, dword ptr [esi + 0x9d]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1d
        in     al, dx
        and    eax, 0xff
        add    dword ptr [edi], eax
loc_2_230D:
        test   cl, 8
        je     loc_2_231A
        mov    eax, dword ptr [esi + 0xa1]
        inc    dword ptr [eax]
loc_2_231A:
        test   cl, 0x10
        je     loc_2_2333
        xor    edx, edx
        mov    ecx, dword ptr [esi + 0x6d]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x1f
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx], eax
loc_2_2333:
        xor    edx, edx
        mov    al, 4
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
loc_2_233E:
        out    dx, al
loc_2_233F:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        in     al, dx
        and    al, 0x75
        jne    loc_2_20A7
loc_2_2351:
        cmp    byte ptr [esi + 0xd4], 0
        je     loc_2_236D
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xd4], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_236D:
        mov    eax, 0
        pop    edi
        cmp    byte ptr [ebp - 2], 1
        adc    eax, -1
        pop    esi
        and    ax, 3
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2390:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        in     al, dx
        and    al, 0x1a
        test   al, 0xa
        je     loc_2_254A
loc_2_23AD:
        test   al, 0x10
        je     loc_2_23C1
        test   dword ptr [esi + 0xc5], 0x8100000
        je     loc_2_253C
loc_2_23C1:
        test   al, 2
        jne    loc_2_24B4
        test   al, 8
        je     loc_2_251E
        xor    ecx, ecx
        mov    al, 8
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x17]
        out    dx, al
        lea    edx, [ecx + 0x14]
        in     al, dx
        mov    bl, al
        and    bl, 8
        je     loc_2_23F0
        mov    eax, dword ptr [esi + 0xa9]
        jmp    loc_2_23FA
loc_2_23F0:
        test   al, 0x20
        je     loc_2_23FC
        mov    eax, dword ptr [esi + 0xc1]
loc_2_23FA:
        inc    dword ptr [eax]
loc_2_23FC:
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_241C
        mov    al, byte ptr [esi + 0x11d]
        dec    al
        mov    byte ptr [esi + 0x11d], al
        jne    loc_2_2476
        mov    byte ptr [esi + 0x11d], 3
loc_2_241C:
        mov    byte ptr [esi + 0xf4], 0
        mov    byte ptr [esi + 0x31a], 0
        movsx  eax, word ptr [esi + 0xf2]
        mov    dword ptr [esi + eax*4 + 0xfd], 0
        mov    word ptr [esi + 0xf2], 0xffff
        cmp    word ptr [esi + 0xf0], -1
        je     loc_2_2455
        push   esi
        call   sub_1_2490
loc_2_2455:
        push   esi
        mov    eax, 0xc
        cmp    bl, 1
        adc    eax, -1
        push   eax
        call   sub_1_2D80
        cmp    ax, 5
        je     loc_2_254A
        jmp    loc_2_251E
loc_2_2476:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, word ptr [esi + 0xf2]
        mov    cx, word ptr [esi + ebx*4 + 0xf5]
        lea    edx, [edi + 0x14]
loc_2_2490:
        mov    ax, word ptr [esi + 0xcd]
        imul   eax, ebx
        add    eax, dword ptr [esi + 0x18]
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 0x15]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x16]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0x10]
        mov    al, 0x26
        out    dx, al
        jmp    loc_2_251E
loc_2_24B4:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 0x11d], 3
        lea    edx, [ecx + 0x17]
        out    dx, al
        lea    edx, [ecx + 0x14]
        in     al, dx
        test   al, 0x10
        je     loc_2_24D7
        mov    eax, dword ptr [esi + 0xbd]
        inc    dword ptr [eax]
loc_2_24D7:
        mov    byte ptr [esi + 0xf4], 0
        mov    byte ptr [esi + 0x31a], 0
        movsx  eax, word ptr [esi + 0xf2]
        mov    dword ptr [esi + eax*4 + 0xfd], 0
        mov    word ptr [esi + 0xf2], 0xffff
        cmp    word ptr [esi + 0xf0], -1
        je     loc_2_2510
        push   esi
        call   sub_1_2490
loc_2_2510:
        push   esi
        push   0
        call   sub_1_2D80
        cmp    ax, 5
        je     loc_2_254A
loc_2_251E:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x17
        in     al, dx
        and    al, 0x1a
        test   al, 0xa
        jne    loc_2_23AD
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_253C:
        xor    eax, eax
        mov    byte ptr [esi + 0xf4], al
        mov    byte ptr [esi + 0x31a], al
loc_2_254A:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2560:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    di, word ptr [esi + 0xee]
        movsx  eax, di
        lea    ebx, [esi + eax*4 + 0xfd]
        cmp    dword ptr [ebx], 0
        je     loc_2_258A
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_258A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   esi
        call   sub_1_2540
        mov    ax, word ptr [esi + 0xee]
        inc    ax
        mov    word ptr [esi + 0xee], ax
        cmp    ax, 2
        jne    loc_2_25B9
        mov    word ptr [esi + 0xee], 0
loc_2_25B9:
        mov    ax, word ptr [esi + 0x48]
        test   al, 0x20
        jne    loc_2_25F2
        mov    dword ptr [ebx], 2
        cmp    word ptr [esi + 0xf0], -1
        jne    loc_2_25FA
        cmp    word ptr [esi + 0xf2], -1
        mov    word ptr [esi + 0xf0], di
        jne    loc_2_25FA
        push   esi
        call   sub_1_2490
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_25F2:
        and    ax, 0xffdf
        mov    word ptr [esi + 0x48], ax
loc_2_25FA:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2610:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x34
        mov    dword ptr [ecx], 0
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x18]
        lea    ecx, [esi + edx]
        mov    eax, dword ptr [edi + 0x140]
        cmp    ecx, eax
        jbe    loc_2_263D
        sub    eax, edx
        mov    esi, eax
loc_2_263D:
        mov    eax, dword ptr [edi + 0x134]
        push   0
        add    eax, edx
        lea    ecx, [ebp - 0x24]
        push   ecx
        mov    edx, dword ptr [ebp + 0x14]
        push   0
        mov    dword ptr [ebp - 0x20], esi
        push   0
        mov    dword ptr [ebp - 0x18], eax
        push   edx
        call   sub_1_1C98
        lea    ecx, [ebp - 0x2c]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   edx
        push   eax
        call   sub_1_1CBC
        mov    dword ptr [ebp - 0x28], 0
        test   esi, esi
        je     loc_2_2930
loc_2_267E:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x20]
        sub    eax, dword ptr [ebp - 0x28]
        cmp    eax, dword ptr [ebp - 0x20]
        ja     loc_2_268E
        mov    ecx, eax
loc_2_268E:
        mov    eax, dword ptr [edi + 0x124]
        mov    dword ptr [ebp - 0x10], ecx
        mov    ecx, dword ptr [ebp - 0x18]
        add    ecx, dword ptr [ebp - 0x10]
        cmp    ecx, eax
        jb     loc_2_26A7
        sub    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x10], eax
loc_2_26A7:
        mov    ecx, dword ptr [edi + 0xc5]
        test   cl, 8
        je     loc_2_2840
        test   ecx, 0x8100000
        jne    loc_2_2840
        mov    eax, dword ptr [ebp - 0xc]
        xor    edx, edx
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, dword ptr [ebp - 0x18]
        and    al, 3
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x30], edx
        sub    eax, edx
        and    ecx, 0x8000000
        mov    dword ptr [ebp - 0x1c], eax
        je     loc_2_2700
        xor    esi, esi
        mov    al, byte ptr [edi + 0x144]
        mov    si, word ptr [edi + 4]
        lea    edx, [esi + 5]
        out    dx, al
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2700:
        mov    eax, dword ptr [ebp - 0x1c]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], edx
        je     loc_2_2722
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 0x145]
        out    dx, al
loc_2_2722:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, 4
        sub    eax, dword ptr [ebp - 0x30]
        cmp    dword ptr [ebp - 0x10], eax
        jae    loc_2_2735
        mov    eax, dword ptr [ebp - 0x10]
loc_2_2735:
        xor    edx, edx
        test   eax, eax
        je     loc_2_2751
loc_2_273B:
        mov    ebx, dword ptr [ebp - 0x14]
        mov    cl, byte ptr [ebp - 8]
        add    cl, dl
        inc    esi
        shl    cl, 3
        inc    edx
        shr    ebx, cl
        mov    byte ptr [esi - 1], bl
        cmp    eax, edx
        ja     loc_2_273B
loc_2_2751:
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], esi
        add    dword ptr [ebp - 0x1c], 4
        sub    ecx, eax
        mov    dword ptr [ebp - 8], ecx
        shr    ecx, 2
        test   ecx, ecx
        je     loc_2_27BA
        mov    esi, dword ptr [edi + 0xc5]
        mov    eax, dword ptr [ebp - 0x1c]
        and    esi, 0x8000000
        lea    edx, [eax + ecx*4]
        mov    dword ptr [ebp - 0x1c], edx
loc_2_277C:
        test   esi, esi
        je     loc_2_27B3
        xor    ebx, ebx
        mov    al, byte ptr [edi + 0x144]
        mov    bx, word ptr [edi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
        test   esi, esi
        je     loc_2_27B3
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 0x145]
        out    dx, al
loc_2_27B3:
        add    dword ptr [ebp - 0xc], 4
        dec    ecx
        jne    loc_2_277C
loc_2_27BA:
        test   byte ptr [ebp - 8], 3
        je     loc_2_2835
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 0xc]
        and    cl, 3
        mov    ebx, dword ptr [edi + 0xc5]
        and    ebx, 0x8000000
        je     loc_2_27F4
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        mov    dword ptr [ebp - 8], eax
        lea    edx, [eax + 5]
        mov    al, byte ptr [edi + 0x144]
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_27F4:
        mov    eax, dword ptr [ebp - 0x1c]
        test   ebx, ebx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], edx
        je     loc_2_2816
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 0x145]
        out    dx, al
loc_2_2816:
        xor    edx, edx
        test   cl, cl
        je     loc_2_2835
        xor    ebx, ebx
        mov    bl, cl
loc_2_2820:
        lea    ecx, [edx*8]
        mov    eax, dword ptr [ebp - 0x14]
        inc    esi
        inc    edx
        shr    eax, cl
        mov    byte ptr [esi - 1], al
        cmp    ebx, edx
        ja     loc_2_2820
loc_2_2835:
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_28C2
loc_2_2840:
        cmp    byte ptr [edi + 0x146], 0
        je     loc_2_2871
        push   edi
        lea    eax, [edi + 0xd5]
        push   0                             ; -> sub_2_1D90
        push   eax
        call   sub_1_1C62
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        mov    edx, dword ptr [ebp - 0x18]
        push   edx
        push   edi
        call   sub_1_2C10
        jmp    loc_2_28C2
loc_2_2871:
        test   ecx, 0x8000000
        je     loc_2_2892
        xor    ecx, ecx
        mov    al, byte ptr [edi + 0x144]
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2892:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_2050
        test   byte ptr [edi + 0xc8], 8
        je     loc_2_28C2
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [edi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [edi + 0x145]
        out    dx, al
loc_2_28C2:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ebp - 0x18], eax
        sub    dword ptr [ebp - 0x20], eax
        add    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_2_2930
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [edi + 0x124], eax
        jne    loc_2_28EA
        mov    eax, dword ptr [edi + 0x120]
        mov    dword ptr [ebp - 0x18], eax
loc_2_28EA:
        mov    eax, dword ptr [ebp - 0x10]
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 0x28], eax
        mov    ecx, dword ptr [ebp - 0x28]
        cmp    ecx, dword ptr [ebp - 0x2c]
        jne    loc_2_2926
        lea    eax, [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x24]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_2_2930
        lea    eax, [ebp - 0x2c]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 0x24]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    dword ptr [ebp - 0x28], 0
loc_2_2926:
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_2_267E
loc_2_2930:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_2940:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x10]
        lea    ebx, [edx + eax]
        mov    ecx, dword ptr [esi + 0x140]
        cmp    ebx, ecx
        jbe    loc_2_2966
        sub    ecx, eax
        mov    edx, ecx
loc_2_2966:
        mov    eax, dword ptr [esi + 0x134]
        mov    dword ptr [ebp - 0x1c], edx
        mov    dword ptr [ebp - 0x18], eax
        test   edx, edx
        je     loc_2_2BC6
loc_2_297A:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x1c]
        add    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0x124]
        cmp    ecx, eax
        jb     loc_2_2996
        sub    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], eax
loc_2_2996:
        mov    ecx, dword ptr [esi + 0xc5]
        test   cl, 8
        je     loc_2_2B1E
        test   ecx, 0x8100000
        jne    loc_2_2B1E
        mov    eax, dword ptr [ebp - 0x18]
        xor    edx, edx
        and    al, 3
        mov    dword ptr [ebp - 0x24], edi
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x20], edx
        sub    eax, edx
        and    ecx, 0x8000000
        mov    dword ptr [ebp - 0xc], eax
        je     loc_2_29EC
        xor    ebx, ebx
        mov    al, byte ptr [esi + 0x144]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_29EC:
        mov    eax, dword ptr [ebp - 0xc]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], edx
        je     loc_2_2A0E
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_2_2A0E:
        mov    edx, 4
        sub    edx, dword ptr [ebp - 0x20]
        cmp    dword ptr [ebp - 0x14], edx
        jae    loc_2_2A1E
        mov    edx, dword ptr [ebp - 0x14]
loc_2_2A1E:
        xor    ebx, ebx
        test   edx, edx
        je     loc_2_2A3A
loc_2_2A24:
        mov    eax, dword ptr [ebp - 0x10]
        mov    cl, byte ptr [ebp - 8]
        add    cl, bl
        inc    edi
        shl    cl, 3
        inc    ebx
        shr    eax, cl
        mov    byte ptr [edi - 1], al
        cmp    ebx, edx
        jb     loc_2_2A24
loc_2_2A3A:
        add    dword ptr [ebp - 0xc], 4
        mov    eax, dword ptr [ebp - 0x14]
        sub    eax, edx
        mov    ecx, eax
        mov    dword ptr [ebp - 8], eax
        shr    ecx, 2
        test   ecx, ecx
        je     loc_2_2AA1
loc_2_2A4F:
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_2A71
        xor    ebx, ebx
        mov    al, byte ptr [esi + 0x144]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2A71:
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [eax]
        mov    dword ptr [edi], edx
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_2A97
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_2_2A97:
        add    dword ptr [ebp - 0xc], 4
        add    edi, 4
        dec    ecx
        jne    loc_2_2A4F
loc_2_2AA1:
        test   byte ptr [ebp - 8], 3
        je     loc_2_2B19
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0xc5]
        and    al, 3
        and    ecx, 0x8000000
        mov    byte ptr [ebp - 8], al
        je     loc_2_2AD6
        xor    ebx, ebx
        mov    al, byte ptr [esi + 0x144]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2AD6:
        mov    eax, dword ptr [ebp - 0xc]
        test   ecx, ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], edx
        je     loc_2_2AF8
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_2_2AF8:
        xor    ebx, ebx
        cmp    byte ptr [ebp - 8], bl
        je     loc_2_2B19
        xor    edx, edx
        mov    dl, byte ptr [ebp - 8]
loc_2_2B04:
        lea    ecx, [ebx*8]
        mov    eax, dword ptr [ebp - 0x10]
        inc    edi
        inc    ebx
        shr    eax, cl
        mov    byte ptr [edi - 1], al
        cmp    edx, ebx
        ja     loc_2_2B04
loc_2_2B19:
        mov    edi, dword ptr [ebp - 0x24]
        jmp    loc_2_2B9A
loc_2_2B1E:
        cmp    byte ptr [esi + 0x146], 0
        je     loc_2_2B4C
        push   esi
        lea    eax, [esi + 0xd5]
        push   0                             ; -> sub_2_1D90
        push   eax
        call   sub_1_1C62
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    ecx, dword ptr [ebp - 0x18]
        push   edi
        push   ecx
        push   esi
        call   sub_1_2C10
        jmp    loc_2_2B9A
loc_2_2B4C:
        test   ecx, 0x8000000
        je     loc_2_2B6D
        xor    ecx, ecx
        mov    al, byte ptr [esi + 0x144]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 0x40
        out    dx, al
loc_2_2B6D:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        push   eax
        push   ecx
        push   edi
        call   sub_1_2050
        test   byte ptr [esi + 0xc8], 8
        je     loc_2_2B9A
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0xbf
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 0x145]
        out    dx, al
loc_2_2B9A:
        mov    eax, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 0x18], eax
        add    dword ptr [ebp + 0x10], eax
        sub    dword ptr [ebp - 0x1c], eax
        je     loc_2_2BC6
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [esi + 0x124], eax
        jne    loc_2_2BBC
        mov    eax, dword ptr [esi + 0x120]
        mov    dword ptr [ebp - 0x18], eax
loc_2_2BBC:
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_2_297A
loc_2_2BC6:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BE0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    al, 0x60
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x10]
        out    dx, al
        lea    edx, [ecx + 0x17]
        in     al, dx
        xor    edx, edx
        mov    byte ptr [esi + 0xed], al
        mov    dx, word ptr [esi + 4]
        mov    al, 0x20
        add    edx, 0x10
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
loc_2_2C10:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 0x18
        mov    al, 0x60
        lea    esi, [edi + 0x10]
        mov    edx, esi
        out    dx, al
loc_2_2C29:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 0x1f
        lea    edx, [eax + edi]
        mov    al, 0xff
        out    dx, al
        jbe    loc_2_2C29
        mov    edx, esi
        mov    al, 0x20
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2C50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 0x18
        mov    al, 0x60
        lea    esi, [edi + 0x10]
        mov    edx, esi
        out    dx, al
loc_2_2C69:
        xor    eax, eax
        mov    al, cl
        inc    cl
        lea    edx, [eax + edi]
        xor    al, al
        out    dx, al
        cmp    cl, 0x1f
        jbe    loc_2_2C69
        mov    edx, esi
        mov    al, 0x20
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj1:0x3624
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_1C6E
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_2CC4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2CC4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_2CDE
        mov    esi, 2
loc_2_2CDE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1C6E
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_2D12
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2DB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2D12:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_1C6E
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_2D41
        push   eax
        call   sub_2_2DB0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_2D41:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x2c], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0x30], dl
        add    eax, 0x2c
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
loc_2_2D80:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_2DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_2DD0
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_1C74
loc_2_2DD0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_2DE6
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_1C74
loc_2_2DE6:
        push   0
        push   0x32
        push   esi
        call   sub_1_1C74
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_2E19
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2E19:
        mov    eax, dword ptr [0]            ; -> obj1:0x3624
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1C6E
        or     eax, eax
        je     loc_2_2E39
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2E39:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_2E40:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_2E53
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_2E40
        jmp    loc_2_2E62
loc_2_2E53:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_2E62:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_2E78
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2E78:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_2ED0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_3270
        mov    edi, eax
        or     edi, edi
        je     loc_2_2EF9
        cmp    edi, 0x103
        jne    loc_2_2F10
loc_2_2EF9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_2F90
        or     eax, eax
        je     loc_2_2F10
        mov    edi, eax
loc_2_2F10:
        or     edi, edi
        je     loc_2_2F1C
        cmp    edi, 0x103
        jne    loc_2_2F7B
loc_2_2F1C:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_2F76
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_2F32
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2F32:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_2F41
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_2F47
loc_2_2F41:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_2F47:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_2F56
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1BD0
loc_2_2F56:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_1C74
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_2F76:
        mov    edi, 0xc001000e
loc_2_2F7B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_2070
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_2070
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_3014
        sub    eax, eax
        nop    
loc_2_2FF0:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_2FF0
loc_2_3014:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_307F
loc_2_3021:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_3074
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_2070
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_2070
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_3077
loc_2_3074:
        inc    dword ptr [ebp - 4]
loc_2_3077:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_3021
loc_2_307F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_316A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_3093:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_3450
        or     al, al
        je     loc_2_30CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_3158
loc_2_30CA:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_31A4
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        call   sub_1_2070
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_2070
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_3158:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_3093
loc_2_316A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_31F5
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_3180:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_31FB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_31E2
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_31E2
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_31E9
loc_2_31A4:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2070
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_2070
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_31E2:
        mov    dword ptr [ebp - 8], 1
loc_2_31E9:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0xc], edx
        ja     loc_2_3180
loc_2_31F5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_3260
loc_2_31FB:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_3262
        cmp    edi, 0x103
        je     loc_2_3262
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2070
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2070
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_3260:
        sub    edi, edi
loc_2_3262:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_3270:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        mov    dword ptr [edi + 0xc], 0
        je     loc_2_32AD
        lea    esp, [esp]
loc_2_32A0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_32A0
loc_2_32AD:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_32E1
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_2_32E3
        cmp    eax, 0x103
        je     loc_2_32E3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_32E1:
        sub    eax, eax
loc_2_32E3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_32F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_3322
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_3317:
        test   dword ptr [esi], edx
        je     loc_2_331C
        inc    eax
loc_2_331C:
        add    esi, 4
        dec    edi
        jne    loc_2_3317
loc_2_3322:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3330:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_33AC
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_3370:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_339A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_33C6
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_2070
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_339A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_3370
loc_2_33AC:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_33C6:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_33F0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_3420
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_3420:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_2070
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     loc_2_34E9
        dec    esi
        cmp    esi, edx
        jb     loc_2_34E9
        nop    
loc_2_3470:
        cmp    edx, ecx
        jb     loc_2_34E9
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_3493
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_34C7
loc_2_3493:
        jbe    loc_2_34A0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_34C7
        db  08Bh, 0FFh
loc_2_34A0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_34B7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_34C7
loc_2_34B7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_34C7
        mov    dword ptr [ebp - 4], 0
loc_2_34C7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_34F9
        jle    loc_2_34D8
        or     edx, edx
        je     loc_2_34E9
        lea    esi, [edx - 1]
        jmp    loc_2_34DB
loc_2_34D8:
        lea    ecx, [edx + 1]
loc_2_34DB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_3470
loc_2_34E9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_34F9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
