; IBMTOK -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 9769 instructions recovered by recursive descent

Declare_Virtual_Device IBMTOK, 3, 10, IBMTOK_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x251c  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x12b], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x12f], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x133], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x137], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], 0
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x1bc
        xor    eax, eax
        mov    ecx, 0x80
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xcd], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x44], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x46], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x111], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x116], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x118], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x11e], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_00C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 4], eax
loc_1_00E1:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_011B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_010E
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
loc_1_010E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_00E1
loc_1_011B:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_012C
        mov    dword ptr [ebp - 8], 0xd0
loc_1_012C:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1a8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_013F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0425
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_1F20
loc_1_0195:
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_028A
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_1_0224
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x1a8]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_020F
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_00C8
loc_1_020F:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_217C
        jmp    loc_1_0285
loc_1_0224:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x69]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        mov    dword ptr [ebp - 0xc], 0x103
loc_1_0285:
        jmp    loc_1_0420
loc_1_028A:
        cmp    dword ptr [ebp - 0xc], 0x103
        jne    loc_1_031F
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_02C0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2BB4
loc_1_02C0:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x69]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        jmp    loc_1_0420
loc_1_031F:
        cmp    dword ptr [ebp - 0xc], 0xc001000e
        jne    loc_1_0391
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x69]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_0420
loc_1_0391:
        cmp    dword ptr [ebp - 0xc], 0xc001000d
        jne    loc_1_0403
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x6d
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x71]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x71]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x71], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], 0x103
        jmp    loc_1_0420
loc_1_0403:
        push   1
        push   0xa
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
        add    esp, 0x14
loc_1_0420:
        jmp    loc_1_042C
loc_1_0425:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_042C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_1_047B
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_046D
        mov    eax, dword ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x28]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_1_047B
loc_1_046D:
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_5_37FB
loc_1_0476:
        jmp    loc_1_047B
loc_1_047B:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_0483
loc_1_0483:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_048A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 0
        jne    loc_1_04D5
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0BB7
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_059D
loc_1_04D5:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 1
        jne    loc_1_0596
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b5]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0530
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0524
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0C31
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_052B
loc_1_0524:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_052B:
        jmp    loc_1_0591
loc_1_0530:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0552
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_1_0591
loc_1_0552:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0574
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_1_0591
loc_1_0574:
        push   3
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
        add    esp, 0x14
loc_1_0591:
        jmp    loc_1_059D
loc_1_0596:
        mov    dword ptr [ebp - 0xc], 0x10001
loc_1_059D:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_1_05EC
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_05DE
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_1_05EC
loc_1_05DE:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_37FB
loc_1_05E7:
        jmp    loc_1_05EC
loc_1_05EC:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_05F4
loc_1_05F4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_05FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 1
        mov    dword ptr [ebp - 0x20], 0
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 4
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x40], eax
        jmp    loc_1_0ABF
loc_1_0631:
        mov    dword ptr [ebp - 0xc], 6
        jmp    loc_1_0B65
loc_1_063D:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_1_065D
        mov    dword ptr [ebp - 0x10], 0     ; -> obj4:0x0088
        mov    dword ptr [ebp - 4], 0x5c
        jmp    loc_1_066B
loc_1_065D:
        mov    dword ptr [ebp - 0x10], 0     ; -> obj4:0x0004
        mov    dword ptr [ebp - 4], 0x84
loc_1_066B:
        jmp    loc_1_0B65
loc_1_0670:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0692
        mov    dword ptr [ebp - 8], 2
        jmp    loc_1_06F3
loc_1_0692:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1b2]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_06BE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1b4]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_06CA
loc_1_06BE:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_06F3
loc_1_06CA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1b5]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_06EC
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_06F3
loc_1_06EC:
        mov    dword ptr [ebp - 8], 0
loc_1_06F3:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_0B65
loc_1_0705:
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_0B65
loc_1_0717:
        mov    dword ptr [ebp - 0xc], 0xd0
        jmp    loc_1_0B65
loc_1_0723:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x123]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x10], 0x10106
        jne    loc_1_0740
        sub    dword ptr [ebp - 0xc], 0xe
loc_1_0740:
        jmp    loc_1_0B65
loc_1_0745:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x33]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0764
        mov    dword ptr [ebp - 0xc], 0x27100
        jmp    loc_1_076B
loc_1_0764:
        mov    dword ptr [ebp - 0xc], 0x9c40
loc_1_076B:
        jmp    loc_1_0B65
loc_1_0770:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x11f]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x121]
        and    ecx, 0xffff
        imul   eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_079A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x127]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x129]
        and    ecx, 0xffff
        imul   eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_07C4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_07DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x129]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_07F2:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x4f
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_225C
        and    dword ptr [ebp - 0xc], 0xffffff00
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0822
        or     dword ptr [ebp - 0xc], 1
loc_1_0822:
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_0B65
loc_1_0834:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_085A
        mov    dword ptr [ebp - 0x10], 0     ; -> loc_1_2350
        mov    dword ptr [ebp - 4], 0x2c
        jmp    loc_1_0868
loc_1_085A:
        mov    dword ptr [ebp - 0x10], 0     ; -> obj1:0x237C
        mov    dword ptr [ebp - 4], 0x29
loc_1_0868:
        jmp    loc_1_0B65
loc_1_086D:
        mov    word ptr [ebp - 0x24], 0x300
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0B65
loc_1_0885:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        je     loc_1_08A3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd9]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_08AF
loc_1_08A3:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0xc], eax
loc_1_08AF:
        jmp    loc_1_0B65
loc_1_08B4:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_1_08CF
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_092B
loc_1_08CF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0xc], 0
loc_1_08DF:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x28]
        je     loc_1_092B
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_091E
        mov    eax, dword ptr [ebp - 0x2c]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0xd0
        jne    loc_1_091E
        jmp    loc_1_092B
loc_1_091E:
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_1_08DF
loc_1_092B:
        jmp    loc_1_0B65
loc_1_0930:
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x4f
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 6
        jmp    loc_1_0B65
loc_1_0997:
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x49
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 6
        jmp    loc_1_0B65
loc_1_09FE:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_1_0A1C
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0A28
loc_1_0A1C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe5]
        mov    dword ptr [ebp - 0xc], eax
loc_1_0A28:
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 8
        and    eax, 0xff0000
        mov    ecx, dword ptr [ebp - 0xc]
        shr    ecx, 8
        and    ecx, 0xff00
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        shr    ecx, 0x18
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 0x18
        and    ecx, 0xff000000
        or     eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_0A65:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xed]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 8
        and    eax, 0xff0000
        mov    ecx, dword ptr [ebp - 0xc]
        shr    ecx, 8
        and    ecx, 0xff00
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        shr    ecx, 0x18
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        shl    ecx, 0x18
        and    ecx, 0xff000000
        or     eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0B65
loc_1_0AAE:
        mov    dword ptr [ebp - 0x20], 0xc00000bb
        jmp    loc_1_0B65
        db  0E9h, 0A6h, 000h, 000h, 000h
loc_1_0ABF:
        cmp    dword ptr [ebp - 0x40], 0x2010101
        ja     loc_1_0B39
        je     loc_1_0930
        sub    dword ptr [ebp - 0x40], 0x10101
        cmp    dword ptr [ebp - 0x40], 0x12
        ja     loc_1_0AAE
        mov    eax, dword ptr [ebp - 0x40]
        jmp    dword ptr [eax*4]             ; -> obj1:0x0AED
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_063D
        dd  loc_1_0670
        dd  loc_1_0705
        dd  loc_1_0705
        dd  loc_1_0717
        dd  loc_1_0723
        dd  loc_1_0745
        dd  loc_1_0770
        dd  loc_1_079A
        dd  loc_1_07C4
        dd  loc_1_07DB
        dd  loc_1_07F2
        dd  loc_1_0834
        dd  loc_1_0885
        dd  loc_1_08B4
        dd  loc_1_086D
        dd  loc_1_0723
        dd  loc_1_0AAE
        dd  loc_1_0631
loc_1_0B39:
        cmp    dword ptr [ebp - 0x40], 0x2010102
        je     loc_1_0997
        cmp    dword ptr [ebp - 0x40], 0x2010103
        je     loc_1_09FE
        cmp    dword ptr [ebp - 0x40], 0x2010104
        je     loc_1_0A65
        jmp    loc_1_0AAE
loc_1_0B65:
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_1_0BA8
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_0B8F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0x20], 0xc0010014
        jmp    loc_1_0BA8
loc_1_0B8F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_225C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], eax
loc_1_0BA8:
        mov    eax, dword ptr [ebp - 0x20]
        jmp    loc_1_0BB0
loc_1_0BB0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x20
sub_1_0BB7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], 0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05FB
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_0C2A
loc_1_0C2A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_0C31:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        je     loc_1_0C96
        mov    dword ptr [ebp - 0x18], 0xc0010014
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_1_0E4F
loc_1_0C96:
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_1_0DD9
loc_1_0CA1:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0EF1
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_0E2A
loc_1_0CBE:
        push   4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_225C
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0E56
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_0E2A
loc_1_0CEA:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0F78
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_0E2A
loc_1_0D07:
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_0E2A
loc_1_0D13:
        push   4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_225C
        add    dword ptr [ebp - 0x1c], 0x20
        cmp    dword ptr [ebp - 0x1c], 0xd0
        ja     loc_1_0DA8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax + 0x1a8], ecx
        jae    loc_1_0D5F
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1a8], eax
        jmp    loc_1_0DA3
loc_1_0D5F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1a8]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_0D9A
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_0D9A
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00C8
        jmp    loc_1_0DA3
loc_1_0D9A:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
loc_1_0DA3:
        jmp    loc_1_0DAF
loc_1_0DA8:
        mov    dword ptr [ebp - 0x18], 0xc0010014
loc_1_0DAF:
        jmp    loc_1_0E2A
loc_1_0DB4:
        mov    dword ptr [ebp - 0x18], 0xc0010017
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        jmp    loc_1_0E2A
        db  0E9h, 051h, 000h, 000h, 000h
loc_1_0DD9:
        cmp    dword ptr [ebp - 0x24], 0x2010103
        ja     loc_1_0E18
        je     loc_1_0CA1
        cmp    dword ptr [ebp - 0x24], 0x1010e
        je     loc_1_0CBE
        cmp    dword ptr [ebp - 0x24], 0x1010f
        je     loc_1_0D13
        cmp    dword ptr [ebp - 0x24], 0x10112
        je     loc_1_0D07
        jmp    loc_1_0DB4
loc_1_0E18:
        cmp    dword ptr [ebp - 0x24], 0x2010104
        je     loc_1_0CEA
        jmp    loc_1_0DB4
loc_1_0E2A:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_0E47
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
loc_1_0E47:
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_1_0E4F
loc_1_0E4F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_0E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 0x14], 0x8076
        je     loc_1_0E7D
        mov    eax, 0xc00000bb
        jmp    loc_1_0EEA
loc_1_0E7D:
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_2240
loc_1_0EA3:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        cmp    dword ptr [ebp - 4], 0x103
        jne    loc_1_0EE2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0EE2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_1_0EE2:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0EEA
loc_1_0EEA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_0EF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_2010
loc_1_0F2A:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        cmp    dword ptr [ebp - 4], 0x103
        jne    loc_1_0F69
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0F69
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_1_0F69:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0F71
loc_1_0F71:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_0F78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_20B0
loc_1_0FB1:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        cmp    dword ptr [ebp - 4], 0x103
        jne    loc_1_0FF0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0FF0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_1_0FF0:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0FF8
loc_1_0FF8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_0FFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 4], 0xc
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05FB
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0xc00000bb
        jne    loc_1_1209
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_1145
loc_1_107F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x13b]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_1090:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x13f]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10A1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x143]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10B2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x147]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10C3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x163]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10D4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14b]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10E5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15f]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_10F6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xffff
        or     eax, 0xc0011000
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_1112:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x19b]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_1123:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x19f]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_11CB
loc_1_1134:
        mov    dword ptr [ebp - 0x18], 0xc0010017
        jmp    loc_1_11CB
        db  0E9h, 086h, 000h, 000h, 000h
loc_1_1145:
        cmp    dword ptr [ebp - 0x20], 0x2010105
        ja     loc_1_1187
        je     loc_1_10F6
        sub    dword ptr [ebp - 0x20], 0x20101
        cmp    dword ptr [ebp - 0x20], 4
        ja     loc_1_1134
        mov    eax, dword ptr [ebp - 0x20]
        jmp    dword ptr [eax*4]             ; -> obj1:0x1173
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_107F
        dd  loc_1_1090
        dd  loc_1_10A1
        dd  loc_1_10B2
        dd  loc_1_10C3
loc_1_1187:
        cmp    dword ptr [ebp - 0x20], 0x2020101
        ja     loc_1_11B9
        je     loc_1_10D4
        cmp    dword ptr [ebp - 0x20], 0x2010106
        je     loc_1_1112
        cmp    dword ptr [ebp - 0x20], 0x2010107
        je     loc_1_1123
        jmp    loc_1_1134
loc_1_11B9:
        cmp    dword ptr [ebp - 0x20], 0x2020102
        je     loc_1_10E5
        jmp    loc_1_1134
loc_1_11CB:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_1209
        cmp    dword ptr [ebp - 8], 4
        jb     loc_1_11F7
        push   4
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_225C
        add    dword ptr [ebp - 0x14], 4
        jmp    loc_1_1209
loc_1_11F7:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], 0xc0010014
loc_1_1209:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_1_1223
loc_1_1223:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_122A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_1276
        db  0E9h, 0C0h, 000h, 000h, 000h
loc_1_1259:
        mov    dword ptr [ebp - 8], 0x103
        jmp    loc_1_1319
loc_1_1265:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_1_1319
        db  0E9h, 0A3h, 000h, 000h, 000h
loc_1_1276:
        cmp    dword ptr [ebp - 0x14], 0x10111
        ja     loc_1_12AF
        cmp    dword ptr [ebp - 0x14], 0x1010d
        jae    loc_1_1319
        cmp    dword ptr [ebp - 0x14], 0x10101
        jb     loc_1_1265
        cmp    dword ptr [ebp - 0x14], 0x1010b
        jbe    loc_1_1319
        jmp    loc_1_1265
loc_1_12AF:
        cmp    dword ptr [ebp - 0x14], 0x20105
        ja     loc_1_12DB
        cmp    dword ptr [ebp - 0x14], 0x20101
        jae    loc_1_1259
        cmp    dword ptr [ebp - 0x14], 0x10113
        je     loc_1_1319
        jmp    loc_1_1265
loc_1_12DB:
        cmp    dword ptr [ebp - 0x14], 0x2020102
        ja     loc_1_1265
        cmp    dword ptr [ebp - 0x14], 0x2020101
        jae    loc_1_1259
        cmp    dword ptr [ebp - 0x14], 0x2010101
        jb     loc_1_1265
        cmp    dword ptr [ebp - 0x14], 0x2010107
        jbe    loc_1_1319
        jmp    loc_1_1265
        db  0E9h, 04Ch, 0FFh, 0FFh, 0FFh
loc_1_1319:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_1_13AE
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_1_136F
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_1_137D
loc_1_136F:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xd1]
        mov    dword ptr [ecx], eax
loc_1_137D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_13A6
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2BB4
loc_1_13A6:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_141F
loc_1_13AE:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_13C8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0FFF
        mov    dword ptr [ebp - 8], eax
loc_1_13C8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_1_1417
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_1409
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_1_1417
loc_1_1409:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_37FB
loc_1_1412:
        jmp    loc_1_1417
loc_1_1417:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_141F
loc_1_141F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_1426:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_146D
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_1_14C7
loc_1_146D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_148F
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_1_14C7
loc_1_148F:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_14C0
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1AB1
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_14C7
loc_1_14C0:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_14C7:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_1_1516
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_1508
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_1_1516
loc_1_1508:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_37FB
loc_1_1511:
        jmp    loc_1_1516
loc_1_1516:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_151E
loc_1_151E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_1525:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_155A
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x1e
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 4
loc_1_155A:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_157C
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_1628
loc_1_157C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_1_1588
loc_1_1588:
        test   byte ptr [ebp + 0xc], 0x30
        je     loc_1_159E
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_1628
loc_1_159E:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x10], 4
        jne    loc_1_15C8
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        jmp    loc_1_15D1
loc_1_15C8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
loc_1_15D1:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_1_160D
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_1_161B
loc_1_160D:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0xd1]
        mov    dword ptr [ecx], eax
loc_1_161B:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_1628:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1630
loc_1_1630:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_1637:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_166C
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x46
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 9
loc_1_166C:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_168E
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_16FD
loc_1_168E:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_1_16E2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_1_16F0
loc_1_16E2:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0xd1]
        mov    dword ptr [ecx], eax
loc_1_16F0:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_16FD:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1705
loc_1_1705:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_170C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_1741
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x6e
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 0xa
loc_1_1741:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_1763
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_17D2
loc_1_1763:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_1_17B7
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_1_17C5
loc_1_17B7:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0xd1]
        mov    dword ptr [ecx], eax
loc_1_17C5:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_17D2:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_17DA
loc_1_17DA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_17E1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_17F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        push   0                             ; -> loc_5_0ABD
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_216A
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x55]
        inc    edx
        xor    al, al
        out    dx, al
        push   0xc350
        call   sub_1_2170
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x55]
        add    edx, 2
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_1858
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_1_1858:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_1880
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x43]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xf1]
        mov    byte ptr [ecx + 0x1e00], al
loc_1_1880:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        push   0                             ; -> loc_5_0A96
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_216A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e08], 0xc0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x12b]
        mov    dword ptr [ebp - 0xc], eax
loc_1_18B4:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_1917
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_18E5
loc_1_18E5:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x143]
        push   0xc001000c
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_18B4
loc_1_1917:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x112], 0
        je     loc_1_1973
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x112]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_194C
loc_1_194C:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x143]
        push   0xc001000c
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_1_1973:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1982
loc_1_197F:
        inc    dword ptr [ebp - 4]
loc_1_1982:
        cmp    dword ptr [ebp - 4], 0x80
        jae    loc_1_19F4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x1bc]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_19EF
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_19C8
loc_1_19C8:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x143]
        push   0xc001000c
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_1_19EF:
        jmp    loc_1_197F
loc_1_19F4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_19FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ac], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2C68
loc_1_1A3E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 4], eax
loc_1_1A47:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_1A8A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1A47
loc_1_1A8A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        dec    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20C6
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_1AB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 4], eax
loc_1_1AC3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_1AFA
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1AC3
loc_1_1AFA:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1ae], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 0
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_1B37
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
loc_1_1B37:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h, 000h
loc_1_1B40:
        mov    al, byte ptr [0x2308]         ; -> obj1:0x2308
        cmp    al, 0
        jne    loc_1_1B65
        inc    byte ptr [0x2308]             ; -> obj1:0x2308
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_1B67
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_1B67
loc_1_1B65:
        clc    
        ret    
loc_1_1B67:
        stc    
        ret    
IBMTOK_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1B78
        jmp    dword ptr [eax*4 + 0x1b7a]    ; -> obj1:0x1B7A
loc_1_1B78:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_1B40
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B78
        dd  loc_1_1B40
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1BF0:
        push   ebp
        mov    ebp, esp
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
        call   sub_1_1C20
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 8
        cmp    dword ptr [ebp + 0x14], 0xe
        jb     loc_1_1D40
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_1CE4
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_1C62
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_1C6C
loc_1_1C62:
        cmp    al, 0xc0
        jne    loc_1_1C88
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_1C88
loc_1_1C6C:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_1C88
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_1C88
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_1C88
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_1C8C
loc_1_1C88:
        mov    byte ptr [ebp - 2], 0
loc_1_1C8C:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_1CA0
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_1D4B
        db  08Bh, 0FFh
loc_1_1CA0:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_1CBC
        mov    dword ptr [ebp - 8], 0x4000
loc_1_1CBC:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0xc], edx
        jmp    loc_1_1D4B
loc_1_1CE4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_1D30
        mov    edx, dword ptr [esi + 0x1a]
        lea    eax, [esi + 0x18]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_1D07
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_1D0E
loc_1_1D07:
        mov    dword ptr [ebp - 0x10], 1
loc_1_1D0E:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1D20
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_1D4E
        db  08Dh, 049h, 000h
loc_1_1D20:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1D4E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D30:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_1D4E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D40:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_1D4B:
        mov    esi, dword ptr [ebp + 8]
loc_1_1D4E:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_1E31
loc_1_1D59:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_1D9D
        test   ah, 0x40
        je     loc_1_1D72
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_1D9D
loc_1_1D72:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_1D7D
        test   ch, 0x20
        jne    loc_1_1D9D
loc_1_1D7D:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_1D89
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_1D9D
loc_1_1D89:
        test   cl, 0x10
        je     loc_1_1D94
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1D9D
loc_1_1D94:
        test   cl, 0x20
        je     loc_1_1E26
loc_1_1D9D:
        inc    dword ptr [edi + 0x18]
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
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_20D2
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_1E26
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1DDF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1DDF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1DF0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1DF6
        db  08Bh, 0FFh
loc_1_1DF0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_1DF6:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_1E05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_20D8
loc_1_1E05:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_217C
        mov    edi, ebx
        jmp    loc_1_1E29
loc_1_1E26:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1E29:
        or     edi, edi
        jne    loc_1_1D59
loc_1_1E31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1E40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E50
        pop    ebp
        ret    4
sub_1_1E50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_1ECE
loc_1_1E60:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_1E76
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_20D8
loc_1_1E76:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_1EC7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1E8C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1E8C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1EA0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1EA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1EA0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_1EA6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_217C
        mov    edi, ebx
        jmp    loc_1_1ECA
loc_1_1EC7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1ECA:
        or     edi, edi
        jne    loc_1_1E60
loc_1_1ECE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 0x10
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     loc_1_1F32
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_1F32
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_1F32
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F32:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_1F42
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F42:
        test   esi, 0x20
        je     loc_1_1F53
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1F53:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_1F70
        test   esi, 0x10
        je     loc_1_1F70
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_1F70:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_1FFA
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_1F8C
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_1F97
loc_1_1F8C:
        cmp    cl, 0xc0
        jne    loc_1_1FB3
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_1FB3
loc_1_1F97:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_1FB3
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_1FB3
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_1FB3
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_1FB7
loc_1_1FB3:
        mov    byte ptr [ebp - 1], 0
loc_1_1FB7:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1FE0
        test   esi, 8
        je     loc_1_1FD0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_1FD0:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1FE0:
        test   esi, 0x6000
        je     loc_1_1FF1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1FF1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1FFA:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_208A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_2090:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_2096:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_209C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_20A2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_20A8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_20AE:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_20B4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_20BA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_20C6:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_20CC:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_20D2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_20D8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_20DE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_20E4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_20EA:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_20F0:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_20F6:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_20FC:
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
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_2152:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_2158:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_215E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_2164:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_216A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_2170:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_2176:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_217C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_21A0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_21C4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_21E8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_21F4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_21FA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_2200:
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
sub_1_2242:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_2248:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_225C:
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
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IBMTOK  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  IBMTOK_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_2324:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2328:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_232C:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_2330:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_2334:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_2338:
        xor    al, 0x58
        xor    esi, dword ptr [esi]
loc_1_233C:
        and    byte ptr [eax], bh
        xor    dword ptr [0x6b6e6c54], esi
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_2348:
        push   esp
        insb   byte ptr es:[edi], dx
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ebx], 0
        add    byte ptr [eax], al
loc_1_2350:
        dec    ecx
        bound  ebp, qword ptr [ebp + 0x20]
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x23d3
        jb     loc_1_23D1
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_23CF
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_2390
        push   eax
        inc    ebx
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_23A9
        add    byte ptr [ecx + 0x62], cl
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], cl
        je     0x2406
        outsd  dx, dword ptr [esi]
loc_1_2390:
        jb     loc_1_23FD
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_23FB
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_23BC
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_23D2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
loc_1_23A9:
        jne    0x241e
        push   esp
        jns    0x241e
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_2408
        jns    loc_1_2426
        add    byte ptr gs:[ecx + 0x4f], cl
        inc    edx
        inc    ecx
loc_1_23BC:
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_23C0:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_23CF:
        inc    ebp
        push   ebx
loc_1_23D1:
        push   ebx
loc_1_23D2:
        add    byte ptr [eax], al
loc_1_23D4:
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
        dec    ebp
        inc    ecx
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
loc_1_23FB:
        pop    edx
        inc    ebp
loc_1_23FD:
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [esi], al
        add    byte ptr [ebx], al
        add    byte ptr [edx], al
        db  000h
loc_1_2408:
        pop    es
        add    byte ptr [edi], cl
        add    byte ptr [esi], al
        add    byte ptr [ebx], cl
        add    byte ptr [ebx], al
        add    byte ptr [edx], cl
        add    byte ptr [edx], al
        add    byte ptr [0], al
loc_1_241B:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2426:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2434:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2438:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_245C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_246C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_24BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_250C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x22c0  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_246C
        mov    word ptr [ebp - 0x18], 6
        mov    word ptr [ebp - 0x16], 6
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_2348
        mov    word ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0xe], 2
        mov    word ptr [ebp - 0xc], 0xe000
        mov    word ptr [ebp - 0xa], 0xe001
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp + 8], eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0                             ; -> loc_1_246C
        call   sub_1_208A
loc_2_0055:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    byte ptr [eax + 0x1c], 3
        mov    eax, dword ptr [ebp - 0x1c]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x22], 0     ; -> loc_2_0B18
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x26], 0     ; -> loc_1_013F
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x36], 0     ; -> loc_1_048A
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x2a], 0     ; -> loc_5_0000
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x2e], 0     ; -> loc_5_01F0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x32], 0     ; -> loc_1_1426
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x3e], 0     ; -> loc_2_150E
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x3a], 0     ; -> loc_1_122A
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x42], 0     ; -> loc_2_0EFC
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x46], 0     ; -> loc_2_14A2
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x1c]
        add    edx, 0x4a
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   0x36
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_246C
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2096
loc_2_00FB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_011C
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_246C
        push   eax
        call   sub_1_2090
loc_2_0112:
        mov    eax, 0xc0000001
        jmp    loc_2_0123
loc_2_011C:
        xor    eax, eax
        jmp    loc_2_0123
loc_2_0123:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_2_012A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        jmp    loc_2_0138
loc_2_0138:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_054D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0000
loc_2_014A:
        lea    edi, [ebp - 0x30]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [0]            ; -> obj1:0x2318
        mov    dword ptr [ebp - 0x26], eax
        mov    dword ptr [ebp - 0x1a], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        mov    dword ptr [ebp - 0x16], eax
        mov    dword ptr [ebp - 0x12], 0x10
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xbd
        mov    dword ptr [ebp - 0xe], eax
        cmp    dword ptr [0x52], 0           ; -> loc_1_24BE
        jne    loc_2_01AF
        mov    dword ptr [ebp - 0x1a], 1
        mov    dword ptr [ebp - 0x16], 0x110
        mov    dword ptr [ebp - 0x12], 4
        mov    eax, 0                        ; -> loc_1_246C
        add    eax, 0x52
        mov    dword ptr [ebp - 0xe], eax
loc_2_01AF:
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc1]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc5
        push   eax
        call   sub_1_20A2
loc_2_01D7:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_01EC
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_0546
loc_2_01EC:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x18]
        test   eax, eax
        je     loc_2_022B
        push   0
        push   0xaa00000a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_020F:
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_0221:
        mov    eax, 0xc0000001
        jmp    loc_2_0546
loc_2_022B:
        mov    word ptr [ebp - 8], 0
        jmp    loc_2_023A
loc_2_0236:
        inc    word ptr [ebp - 8]
loc_2_023A:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0x10
        jge    loc_2_0274
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbd]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x79], eax
        jmp    loc_2_0236
loc_2_0274:
        cmp    dword ptr [0], 1              ; -> obj1:0x2318
        jne    loc_2_02AC
        push   0                             ; -> loc_1_246C
        call   sub_5_3D90
loc_2_028B:
        test   eax, eax
        jne    loc_2_02A2
        mov    dword ptr [0], 0              ; -> obj1:0x2310
        jmp    loc_2_02AC
loc_2_02A2:
        mov    dword ptr [0], 1              ; -> obj1:0x2310
loc_2_02AC:
        mov    dword ptr [0], 0              ; -> obj1:0x2310
        cmp    dword ptr [0], 1              ; -> obj1:0x2318
        jne    loc_2_031E
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> loc_1_2434
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x55], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xbd], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbd]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        push   0                             ; -> loc_1_246C
        call   sub_5_3FA0
loc_2_0302:
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0319
        mov    eax, 0xc0010006
        jmp    loc_2_0546
loc_2_0319:
        jmp    loc_2_032E
loc_2_031E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2f
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_40D0
loc_2_032E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1540
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_035D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_0353:
        mov    eax, 0xc0010006
        jmp    loc_2_0546
loc_2_035D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0380
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_2_0380:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x55]
        inc    edx
        xor    al, al
        out    dx, al
        push   0xc350
        call   sub_1_2170
loc_2_0394:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x55]
        add    edx, 2
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2f]
        push   eax
        push   0                             ; -> loc_5_0D29
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0                             ; -> loc_5_0AE1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_215E
loc_2_03EC:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0512
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_05DD
        test   eax, eax
        je     loc_2_044B
        push   1
        push   0
        push   2
        push   0xaa000004
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_0421:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2164
loc_2_042D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_043C:
        mov    eax, 0xc0010006
        jmp    loc_2_0546
        db  0E9h, 0C2h, 000h, 000h, 000h
loc_2_044B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc9
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x75
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x49
        push   eax
        push   0                             ; -> loc_1_17E1
        push   0                             ; -> loc_1_1525
        push   0                             ; -> loc_1_170C
        push   0                             ; -> loc_1_1637
        call   sub_2_1D50
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_04CC
        push   2
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_04A2:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2164
loc_2_04AE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_04BD:
        mov    eax, 0xc000009a
        jmp    loc_2_0546
        db  0E9h, 041h, 000h, 000h, 000h
loc_2_04CC:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19], 0     ; -> loc_5_3AB1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x19]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x21], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x25], eax
        push   0x7530
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1d
        push   eax
        call   sub_1_2152
loc_2_0506:
        xor    eax, eax
        jmp    loc_2_0546
        db  0E9h, 034h, 000h, 000h, 000h
loc_2_0512:
        push   3
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_052C:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_053E:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_0546
loc_2_0546:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_2_054D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x61], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x5d], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x65], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x6d
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x71], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x71]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x6d], eax
        jmp    loc_2_05A0
loc_2_05A0:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x129], 0x100
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b2], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_2_05DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0E97
loc_2_05F6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2a]
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0625
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0625
        push   0x186a0
        call   sub_1_2170
loc_2_061D:
        dec    dword ptr [ebp - 4]
        jmp    loc_2_05F6
loc_2_0625:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2a]
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0647
        mov    eax, 0xc0010006
        jmp    loc_2_0B11
        db  0E9h, 0CAh, 004h, 000h, 000h
loc_2_0647:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        add    eax, ecx
        mov    word ptr [ebp - 0x18], ax
        mov    ax, word ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xf9], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_2_072D
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0x30
        jne    loc_2_06F8
        push   7
        push   4
        push   0x32
        push   0xaa00000a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_06E6:
        add    esp, 0x14
        mov    eax, 0xc0010006
        jmp    loc_2_0B11
        db  0E9h, 030h, 000h, 000h, 000h
loc_2_06F8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        push   eax
        push   7
        push   4
        push   3
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_071B:
        add    esp, 0x18
        mov    eax, 0xc0010006
        jmp    loc_2_0B11
        db  0E9h, 011h, 000h, 000h, 000h
loc_2_072D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2a], 1
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 1
        je     loc_2_0763
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x33], 1
        jmp    loc_2_076A
loc_2_0763:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x33], 0
loc_2_076A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x3b], 0x2000
        jbe    loc_2_09DB
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11f]
        mov    byte ptr [ebp - 0x24], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x3b]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x3b], 0x10000
        jne    loc_2_07AB
        sub    dword ptr [ebp - 0x20], 0x2830
        jmp    loc_2_07B2
loc_2_07AB:
        sub    dword ptr [ebp - 0x20], 0x630
loc_2_07B2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x33]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_07DB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x41]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x121], ax
        jmp    loc_2_07EC
loc_2_07DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3f]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x121], ax
loc_2_07EC:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_2_082F
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x129]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        shr    eax, 1
        and    eax, 0xffff
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x127], ax
        jmp    loc_2_0861
loc_2_082F:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x129]
        and    ecx, 0xffff
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        cdq    
        idiv   ecx
        and    eax, 0xffff
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x127], ax
loc_2_0861:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x127]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x129]
        and    edx, 0xffff
        imul   ecx, edx
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x121]
        and    ecx, 0xffff
        cmp    eax, ecx
        jae    loc_2_0935
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        cmp    eax, 0x1000
        jae    loc_2_08D9
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x121], 0x800
        jmp    loc_2_0935
loc_2_08D9:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        cmp    eax, 0x2000
        jae    loc_2_0901
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x121], 0x1000
        jmp    loc_2_0935
loc_2_0901:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        cmp    eax, 0x4000
        jae    loc_2_0929
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x121], 0x2000
        jmp    loc_2_0935
loc_2_0929:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x121], 0x4000
loc_2_0935:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x123]
        jbe    loc_2_0966
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x123]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x121], ax
loc_2_0966:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        sub    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x123], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x121]
        and    ecx, 0xffff
        xor    edx, edx
        mov    dl, byte ptr [ebp - 0x24]
        imul   ecx, edx
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x129]
        mov    eax, dword ptr [ebp - 0x20]
        sub    edx, edx
        div    ecx
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x127]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x127], ax
        jmp    loc_2_0A4B
loc_2_09DB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x121], 0x800
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x11f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x123]
        jbe    loc_2_0A24
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x123]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x121], ax
loc_2_0A24:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        sub    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x123], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x127], 0xf
loc_2_0A4B:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x14], eax
        push   6
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x4f
        push   eax
        call   sub_1_225C
loc_2_0A8E:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x49]
        test   eax, eax
        jne    loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4a]
        test   eax, eax
        jne    loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4b]
        test   eax, eax
        jne    loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4c]
        test   eax, eax
        jne    loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4d]
        test   eax, eax
        jne    loc_2_0AFA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4e]
        test   eax, eax
        jne    loc_2_0AFA
        push   6
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x49
        push   eax
        call   sub_1_225C
loc_2_0AFA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e0f], 0xff
        xor    eax, eax
        jmp    loc_2_0B11
loc_2_0B11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_2_0B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_2_0B40
loc_2_0B3D:
        inc    dword ptr [ebp - 0xc]
loc_2_0B40:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp + 0x18], eax
        jbe    loc_2_0B66
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx + eax*4], 1
        jne    loc_2_0B61
        jmp    loc_2_0B66
loc_2_0B61:
        jmp    loc_2_0B3D
loc_2_0B66:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp + 0x18], eax
        jne    loc_2_0B7C
        mov    eax, 0xc0010019
        jmp    loc_2_0E90
loc_2_0B7C:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x96
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2176
loc_2_0BA6:
        test   eax, eax
        jne    loc_2_0E12
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_1E40
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0C5B
        push   4
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_0C3C:
        add    esp, 0x14
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_217C
loc_2_0C4F:
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_2_0E0D
loc_2_0C5B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xd0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1a8], 0xd0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x5d
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x61]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x61], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1b3]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0E0D
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 1
        mov    dword ptr [ebp - 0x10], 0x103
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1b4]
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0E0D
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1b4], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x19f], 3
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0xf9]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x14], eax
        mov    edi, dword ptr [ebp - 0x14]
        xor    eax, eax
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0xa
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x49
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x127]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        mov    ah, byte ptr [ecx + 0x127]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x18], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x129]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        mov    ah, byte ptr [ecx + 0x129]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 0x121]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x11f]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    byte ptr [ecx + 0x1e], al
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
loc_2_0E0D:
        jmp    loc_2_0E39
loc_2_0E12:
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_0E2C:
        add    esp, 0x14
        mov    eax, 0xc000009a
        jmp    loc_2_0E90
loc_2_0E39:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x20]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_2_0E88
        mov    eax, dword ptr [ebp - 0x20]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0E7A
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x20]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_2_0E88
loc_2_0E7A:
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_5_37FB
loc_2_0E83:
        jmp    loc_2_0E88
loc_2_0E88:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_2_0E90
loc_2_0E90:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x24
sub_2_0E97:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e08], 0xc0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e0f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e0c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0EF5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x43]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xf1]
        mov    byte ptr [ecx + 0x1e00], al
loc_2_0EF5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_2_0EFC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1cc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x28], 8
        mov    word ptr [ebp - 0x26], 8
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x23B0
        mov    word ptr [ebp - 0x50], 7
        mov    word ptr [ebp - 0x4e], 7
        mov    dword ptr [ebp - 0x4c], 0     ; -> loc_1_23C0
        mov    word ptr [ebp - 0x14], 0xb
        mov    word ptr [ebp - 0x12], 0xb
        mov    dword ptr [ebp - 0x10], 0     ; -> loc_1_23D4
        mov    word ptr [ebp - 0xc], 0xe
        mov    word ptr [ebp - 0xa], 0xe
        mov    dword ptr [ebp - 8], 0        ; -> obj1:0x23F0
        mov    dword ptr [ebp - 0x3c], 0
        mov    byte ptr [ebp - 0x20], 1
        mov    byte ptr [ebp - 0x30], 0
        mov    byte ptr [ebp - 0x44], 0
        mov    byte ptr [ebp - 0x40], 0
        mov    dword ptr [ebp - 0x5c], 0xa20
        mov    dword ptr [ebp - 0x38], 0
        mov    dword ptr [ebp - 0x2c], 2
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x3bc
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_2176
loc_2_0F96:
        test   eax, eax
        je     loc_2_0FA8
        mov    eax, 0xc000009a
        jmp    loc_2_149B
loc_2_0FA8:
        mov    edi, dword ptr [ebp - 0x18]
        xor    eax, eax
        mov    ecx, 0xef
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x123], 0x4628
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x19f], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0xc1], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_21F4
loc_2_0FEE:
        cmp    dword ptr [ebp - 0x3c], 0
        je     loc_2_1012
        push   0
        push   0x3bc
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_217C
loc_2_1008:
        mov    eax, 0xc0000001
        jmp    loc_2_149B
loc_2_1012:
        push   1
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_21FA
loc_2_1029:
        cmp    dword ptr [ebp - 0x3c], 0
        jne    loc_2_104E
        mov    eax, dword ptr [ebp - 0x34]
        cmp    dword ptr [eax + 4], 3
        jne    loc_2_104E
        mov    dword ptr [0], 3              ; -> obj1:0x2318
        mov    byte ptr [ebp - 0x44], 1
loc_2_104E:
        push   0
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_21FA
loc_2_1065:
        cmp    dword ptr [ebp - 0x3c], 0
        jne    loc_2_1091
        mov    eax, dword ptr [ebp - 0x34]
        cmp    dword ptr [eax + 4], 1
        jne    loc_2_1082
        inc    byte ptr [0]                  ; -> obj1:0x2314
loc_2_1082:
        mov    dword ptr [0], 1              ; -> obj1:0x2318
        jmp    loc_2_109B
loc_2_1091:
        mov    dword ptr [0], 2              ; -> obj1:0x2318
loc_2_109B:
        cmp    dword ptr [0], 1              ; -> obj1:0x2318
        jne    loc_2_1109
        push   0
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_21FA
loc_2_10BF:
        cmp    dword ptr [ebp - 0x3c], 0
        jne    loc_2_10E3
        mov    eax, dword ptr [ebp - 0x34]
        cmp    dword ptr [eax + 4], 1
        jne    loc_2_10DF
        mov    byte ptr [ebp - 0x20], 1
        jmp    loc_2_10E3
loc_2_10DF:
        mov    byte ptr [ebp - 0x20], 0
loc_2_10E3:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x20]
        test   eax, eax
        je     loc_2_10FF
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x55], 0xa20
        jmp    loc_2_1109
loc_2_10FF:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x55], 0xa24
loc_2_1109:
        cmp    dword ptr [0], 2              ; -> obj1:0x2318
        jne    loc_2_12F4
        lea    eax, [ebp - 0x1b8]
        push   eax
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_20F6
loc_2_112E:
        cmp    dword ptr [ebp - 0x3c], 0
        je     loc_2_114B
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_2200
loc_2_1141:
        mov    eax, 0xc0000001
        jmp    loc_2_149B
loc_2_114B:
        lea    eax, [ebp - 0xb4]
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp - 0x38]
        shl    eax, 0xc
        add    eax, 0xc80
        mov    word ptr [ebp - 0x6c], ax
        mov    byte ptr [ebp - 0x78], 0
loc_2_1167:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x78]
        test   eax, eax
        jne    loc_2_120D
        mov    eax, dword ptr [ebp - 0x70]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 0x74], al
        inc    dword ptr [ebp - 0x70]
        mov    eax, dword ptr [ebp - 0x70]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0x1bc], ax
        add    dword ptr [ebp - 0x70], 2
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x74]
        test   al, 0x80
        jne    loc_2_11A1
        mov    byte ptr [ebp - 0x78], 1
loc_2_11A1:
        mov    eax, dword ptr [ebp - 0x70]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 0x68], al
        inc    dword ptr [ebp - 0x70]
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x74]
        test   al, 0x40
        je     loc_2_11C9
        mov    eax, dword ptr [ebp - 0x70]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 0x60], al
        inc    dword ptr [ebp - 0x70]
        jmp    loc_2_11CD
loc_2_11C9:
        mov    byte ptr [ebp - 0x60], 0
loc_2_11CD:
        mov    eax, dword ptr [ebp - 0x1bc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x6c]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_2_11EE
        jmp    loc_2_1167
loc_2_11EE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x60]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x64]
        and    eax, ecx
        mov    byte ptr [ebp - 0x64], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x68]
        or     al, byte ptr [ebp - 0x64]
        mov    byte ptr [ebp - 0x64], al
        jmp    loc_2_1167
loc_2_120D:
        mov    al, byte ptr [ebp - 0x64]
        and    eax, 6
        shr    eax, 1
        mov    dword ptr [ebp - 0x1c8], eax
        jmp    loc_2_1286
loc_2_1221:
        mov    dword ptr [ebp - 0x2c], 2
        jmp    loc_2_12C0
loc_2_122D:
        mov    dword ptr [ebp - 0x2c], 3
        jmp    loc_2_12C0
loc_2_1239:
        mov    dword ptr [ebp - 0x2c], 5
        jmp    loc_2_12C0
loc_2_1245:
        mov    dword ptr [ebp - 0x2c], 6
        jmp    loc_2_12C0
loc_2_1251:
        mov    dword ptr [ebp - 0x2c], 7
        jmp    loc_2_12C0
loc_2_125D:
        mov    dword ptr [ebp - 0x2c], 0xa
        jmp    loc_2_12C0
loc_2_1269:
        mov    dword ptr [ebp - 0x2c], 0xb
        jmp    loc_2_12C0
loc_2_1275:
        mov    dword ptr [ebp - 0x2c], 0xf
        jmp    loc_2_12C0
        db  0E9h, 03Ah, 000h, 000h, 000h
loc_2_1286:
        cmp    dword ptr [ebp - 0x1c8], 7
        ja     loc_2_12C0
        mov    eax, dword ptr [ebp - 0x1c8]
        jmp    dword ptr [eax*4]             ; -> obj2:0x12A0
        ; ---- dispatch table (8 entries) ----
        dd  loc_2_1221
        dd  loc_2_122D
        dd  loc_2_1239
        dd  loc_2_1245
        dd  loc_2_1251
        dd  loc_2_125D
        dd  loc_2_1269
        dd  loc_2_1275
loc_2_12C0:
        mov    al, byte ptr [ebp - 0x64]
        and    eax, 6
        shr    eax, 1
        mov    byte ptr [ebp - 0x40], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x40]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x2f], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    dword ptr [0], eax            ; -> obj1:0x2320
        mov    eax, dword ptr [ebp - 0x6c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x55], eax
        mov    byte ptr [0], 1               ; -> obj1:0x231C
loc_2_12F4:
        cmp    dword ptr [0], 3              ; -> obj1:0x2318
        jne    loc_2_137C
        lea    eax, [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_20F0
loc_2_1316:
        cmp    dword ptr [ebp - 0x3c], 0
        je     loc_2_1329
        mov    byte ptr [ebp - 0x30], 1
        jmp    loc_2_1454
loc_2_1329:
        mov    al, byte ptr [ebp - 0x55]
        and    eax, 1
        mov    dword ptr [ebp - 0x1cc], eax
        jmp    loc_2_135D
loc_2_133A:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x55], 0xa20
        jmp    loc_2_137C
loc_2_1349:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x55], 0xa24
        jmp    loc_2_137C
        db  0E9h, 01Fh, 000h, 000h, 000h
loc_2_135D:
        cmp    dword ptr [ebp - 0x1cc], 0
        je     loc_2_133A
        cmp    dword ptr [ebp - 0x1cc], 1
        je     loc_2_1349
        jmp    loc_2_137C
loc_2_137C:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_21FA
loc_2_1393:
        cmp    dword ptr [ebp - 0x3c], 0
        jne    loc_2_13AC
        mov    eax, dword ptr [ebp - 0x34]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x123], eax
loc_2_13AC:
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        call   sub_1_20FC
loc_2_13C1:
        cmp    dword ptr [ebp - 4], 6
        jne    loc_2_1454
        cmp    dword ptr [ebp - 0x3c], 0
        jne    loc_2_1454
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 0x49
        mov    dword ptr [ebp - 0x1c0], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 0x1c4], eax
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x1c4]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x1c0]
        mov    byte ptr [ecx + 5], al
loc_2_1454:
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        call   sub_1_2200
loc_2_145D:
        mov    al, byte ptr [ebp - 0x30]
        push   eax
        mov    al, byte ptr [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_2_012A
        mov    dword ptr [ebp - 0x3c], eax
        cmp    dword ptr [ebp - 0x3c], 0
        je     loc_2_1493
        push   0
        push   0x3bc
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_217C
loc_2_1493:
        mov    eax, dword ptr [ebp - 0x3c]
        jmp    loc_2_149B
loc_2_149B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_2_14A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x1d
        push   eax
        call   sub_1_2158
loc_2_14C1:
        push   0x2625a0
        call   sub_1_2170
loc_2_14CB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2164
loc_2_14D4:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_1E20
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_20A8
loc_2_14F2:
        push   0
        push   0x3bc
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_217C
loc_2_1502:
        jmp    loc_2_1507
loc_2_1507:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_2_150E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_209C
loc_2_1527:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_2090
loc_2_1534:
        jmp    loc_2_1539
loc_2_1539:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_2_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x7c
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj1:0x231C
        test   eax, eax
        je     loc_2_156B
        mov    eax, dword ptr [0]            ; -> obj1:0x2320
        shl    eax, 0xc
        add    eax, 0x800
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x55], eax
loc_2_156B:
        mov    byte ptr [0], 0               ; -> obj1:0x231C
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> obj1:0x231C
        test   eax, eax
        jne    loc_2_15B7
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x55]
        in     al, dx
        mov    byte ptr [ebp - 0x48], al
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x55]
        add    edx, 2
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1
        shl    eax, 0x13
        mov    cl, byte ptr [ebp - 0x48]
        and    ecx, 0xfc
        shl    ecx, 0xb
        or     eax, ecx
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_2_15DC
loc_2_15B7:
        mov    ax, word ptr [0]              ; -> obj1:0x2422
        and    eax, 0xfe00
        not    eax
        mov    word ptr [ebp - 0x20], ax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xfc00
        shl    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
loc_2_15DC:
        mov    ax, word ptr [0]              ; -> obj1:0x2422
        and    eax, 0xfe00
        not    eax
        mov    word ptr [ebp - 0x20], ax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xfc00
        shl    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 4], eax
        push   0x2000
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xf1
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_2242
loc_2_1626:
        cmp    dword ptr [ebp - 0x30], 0
        je     loc_2_1650
        push   0
        push   0xaa000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_1646:
        add    esp, 0xc
        xor    al, al
        jmp    loc_2_1D3F
loc_2_1650:
        mov    dword ptr [ebp - 0x50], 0
        jmp    loc_2_165F
loc_2_165C:
        inc    dword ptr [ebp - 0x50]
loc_2_165F:
        cmp    dword ptr [ebp - 0x50], 3
        jae    loc_2_16CC
        mov    eax, dword ptr [ebp - 0x50]
        mov    dword ptr [ebp + eax*4 - 0x5c], 0
        mov    dword ptr [ebp - 0x60], 0
        jmp    loc_2_1684
loc_2_1680:
        add    dword ptr [ebp - 0x60], 2
loc_2_1684:
        cmp    dword ptr [ebp - 0x60], 0x10
        jae    loc_2_16C7
        mov    eax, dword ptr [ebp - 0x50]
        shl    eax, 4
        add    eax, dword ptr [ebp - 0x60]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xf1]
        mov    al, byte ptr [eax + ecx + 0x1f30]
        mov    byte ptr [ebp - 0x4c], al
        mov    eax, dword ptr [ebp - 0x50]
        mov    eax, dword ptr [ebp + eax*4 - 0x5c]
        shl    eax, 4
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x4c]
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0x50]
        mov    dword ptr [ebp + ecx*4 - 0x5c], eax
        jmp    loc_2_1680
loc_2_16C7:
        jmp    loc_2_165C
loc_2_16CC:
        mov    eax, dword ptr [ebp - 0x5c]
        cmp    dword ptr [0], eax            ; -> loc_1_2328
        jne    loc_2_1705
        mov    eax, dword ptr [ebp - 0x58]
        cmp    dword ptr [4], eax            ; -> loc_1_232C
        jne    loc_2_1705
        mov    eax, dword ptr [ebp - 0x54]
        cmp    dword ptr [8], eax            ; -> loc_1_2330
        jne    loc_2_1705
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x34], 1
        jmp    loc_2_1744
loc_2_1705:
        mov    eax, dword ptr [ebp - 0x5c]
        cmp    dword ptr [0], eax            ; -> loc_1_2334
        jne    loc_2_173D
        mov    eax, dword ptr [ebp - 0x58]
        cmp    dword ptr [4], eax            ; -> loc_1_2338
        jne    loc_2_173D
        mov    eax, dword ptr [8]            ; -> loc_1_233C
        cmp    dword ptr [ebp - 0x54], eax
        jne    loc_2_173D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x34], 0
        jmp    loc_2_1744
loc_2_173D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x34], 1
loc_2_1744:
        cmp    dword ptr [0], 0              ; -> obj1:0x2310
        jne    loc_2_1774
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e01]
        mov    byte ptr [ebp - 0x3c], al
        mov    al, byte ptr [ebp - 0x3c]
        and    eax, 0xc
        shr    eax, 2
        mov    byte ptr [ebp - 0x3c], al
        jmp    loc_2_1783
loc_2_1774:
        mov    ax, word ptr [0]              ; -> obj1:0x2422
        and    eax, 0xc
        shr    eax, 2
        mov    byte ptr [ebp - 0x3c], al
loc_2_1783:
        mov    ax, word ptr [0]              ; -> obj1:0x2422
        and    eax, 0xc
        shr    eax, 2
        mov    byte ptr [ebp - 0x24], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_1998
        cmp    dword ptr [0], 0              ; -> obj1:0x2310
        jne    loc_2_17D0
        mov    al, byte ptr [ebp - 0x48]
        and    eax, 0xfc
        shr    eax, 1
        or     eax, 0x80
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x43], al
        jmp    loc_2_17F0
loc_2_17D0:
        mov    ax, word ptr [0]              ; -> loc_1_2458
        and    eax, 0xfc00
        shr    eax, 1
        or     eax, 0x8000
        mov    word ptr [ebp - 0x34], ax
        mov    al, byte ptr [ebp - 0x34]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x43], al
loc_2_17F0:
        mov    ax, word ptr [0]              ; -> loc_1_2458
        and    eax, 0xfc00
        shr    eax, 1
        or     eax, 0x8000
        mov    word ptr [ebp - 0x34], ax
        mov    eax, dword ptr [ebp - 0x34]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x43], ah
        cmp    dword ptr [0], 0              ; -> obj1:0x2310
        jne    loc_2_18A6
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        mov    dword ptr [ebp - 0x64], eax
        jmp    loc_2_187D
loc_2_182F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        test   eax, eax
        jne    loc_2_1848
        mov    dword ptr [ebp - 0x38], 0x2000
        jmp    loc_2_184F
loc_2_1848:
        mov    dword ptr [ebp - 0x38], 0x4000
loc_2_184F:
        mov    byte ptr [ebp - 0x28], 4
        jmp    loc_2_18A1
loc_2_1858:
        mov    dword ptr [ebp - 0x38], 0x8000
        mov    byte ptr [ebp - 0x28], 8
        jmp    loc_2_18A1
loc_2_1868:
        mov    dword ptr [ebp - 0x38], 0x10000
        mov    byte ptr [ebp - 0x28], 0x10
        jmp    loc_2_18A1
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_187D:
        cmp    dword ptr [ebp - 0x64], 3
        ja     loc_2_18A1
        mov    eax, dword ptr [ebp - 0x64]
        jmp    dword ptr [eax*4]             ; -> obj2:0x1891
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_182F
        dd  loc_2_182F
        dd  loc_2_1858
        dd  loc_2_1868
loc_2_18A1:
        jmp    loc_2_1925
loc_2_18A6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        mov    dword ptr [ebp - 0x68], eax
        jmp    loc_2_1901
loc_2_18B3:
        mov    dword ptr [ebp - 0x38], 0x10000
        mov    byte ptr [ebp - 0x28], 0x10
        jmp    loc_2_1925
loc_2_18C3:
        mov    dword ptr [ebp - 0x38], 0x8000
        mov    byte ptr [ebp - 0x28], 8
        jmp    loc_2_1925
loc_2_18D3:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        test   eax, eax
        jne    loc_2_18EC
        mov    dword ptr [ebp - 0x38], 0x2000
        jmp    loc_2_18F3
loc_2_18EC:
        mov    dword ptr [ebp - 0x38], 0x4000
loc_2_18F3:
        mov    byte ptr [ebp - 0x28], 4
        jmp    loc_2_1925
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_1901:
        cmp    dword ptr [ebp - 0x68], 3
        ja     loc_2_1925
        mov    eax, dword ptr [ebp - 0x68]
        jmp    dword ptr [eax*4]             ; -> obj2:0x1915
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_18B3
        dd  loc_2_18C3
        dd  loc_2_18D3
        dd  loc_2_18D3
loc_2_1925:
        mov    eax, dword ptr [ebp - 0x38]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3b], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x43]
        and    eax, 0xff
        shl    eax, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xf5
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_2242
loc_2_1969:
        cmp    dword ptr [ebp - 0x30], 0
        je     loc_2_1993
        push   0
        push   0xaa000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_1989:
        add    esp, 0xc
        xor    al, al
        jmp    loc_2_1D3F
loc_2_1993:
        jmp    loc_2_1A69
loc_2_1998:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        mov    dword ptr [ebp - 0x6c], eax
        jmp    loc_2_19DA
loc_2_19A5:
        mov    dword ptr [ebp - 0x2c], 0x2000
        jmp    loc_2_19FE
loc_2_19B1:
        mov    dword ptr [ebp - 0x2c], 0x4000
        jmp    loc_2_19FE
loc_2_19BD:
        mov    dword ptr [ebp - 0x2c], 0x8000
        jmp    loc_2_19FE
loc_2_19C9:
        mov    dword ptr [ebp - 0x2c], 0x10000
        jmp    loc_2_19FE
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_19DA:
        cmp    dword ptr [ebp - 0x6c], 3
        ja     loc_2_19FE
        mov    eax, dword ptr [ebp - 0x6c]
        jmp    dword ptr [eax*4]             ; -> obj2:0x19EE
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_19A5
        dd  loc_2_19B1
        dd  loc_2_19BD
        dd  loc_2_19C9
loc_2_19FE:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xfe
        shl    eax, 0xc
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xf5
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_2242
loc_2_1A36:
        cmp    dword ptr [ebp - 0x30], 0
        je     loc_2_1A60
        push   0
        push   0xaa000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_1A56:
        add    esp, 0xc
        xor    al, al
        jmp    loc_2_1D3F
loc_2_1A60:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3b], eax
loc_2_1A69:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2f]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x40], eax
        cmp    dword ptr [0], 0              ; -> obj1:0x2310
        jne    loc_2_1B29
        mov    al, byte ptr [ebp - 0x48]
        and    eax, 3
        mov    dword ptr [ebp - 0x70], eax
        jmp    loc_2_1B05
loc_2_1A92:
        mov    dword ptr [ebp - 0x44], 2
        jmp    loc_2_1B29
loc_2_1A9E:
        mov    dword ptr [ebp - 0x44], 3
        jmp    loc_2_1B29
loc_2_1AAA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_1AC9
        mov    dword ptr [ebp - 0x44], 6
        jmp    loc_2_1AD0
loc_2_1AC9:
        mov    dword ptr [ebp - 0x44], 0xa
loc_2_1AD0:
        jmp    loc_2_1B29
loc_2_1AD5:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_1AF4
        mov    dword ptr [ebp - 0x44], 7
        jmp    loc_2_1AFB
loc_2_1AF4:
        mov    dword ptr [ebp - 0x44], 0xb
loc_2_1AFB:
        jmp    loc_2_1B29
        db  0E9h, 024h, 000h, 000h, 000h
loc_2_1B05:
        cmp    dword ptr [ebp - 0x70], 3
        ja     loc_2_1B29
        mov    eax, dword ptr [ebp - 0x70]
        jmp    dword ptr [eax*4]             ; -> obj2:0x1B19
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_1A92
        dd  loc_2_1A9E
        dd  loc_2_1AAA
        dd  loc_2_1AD5
loc_2_1B29:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x35], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1fa6]
        mov    byte ptr [ebp - 0x18], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x74], eax
        jmp    loc_2_1BF6
loc_2_1B4F:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x37], eax
        jmp    loc_2_1C26
loc_2_1B5D:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x37], 0x2000
        jmp    loc_2_1C26
loc_2_1B6C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x37], 0x4000
        jmp    loc_2_1C26
loc_2_1B7B:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x37], 0x8000
        jmp    loc_2_1C26
loc_2_1B8A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x37], 0x10000
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x35], 1
        jmp    loc_2_1C26
loc_2_1BA0:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x37], 0x10000
        jmp    loc_2_1C26
loc_2_1BAF:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        push   eax
        push   6
        push   2
        push   3
        push   0xaa00000a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_2_1BCF:
        add    esp, 0x18
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_1BEA
        jmp    loc_2_1BEA
loc_2_1BEA:
        xor    al, al
        jmp    loc_2_1D3F
        db  0E9h, 030h, 000h, 000h, 000h
loc_2_1BF6:
        sub    dword ptr [ebp - 0x74], 0xa
        cmp    dword ptr [ebp - 0x74], 5
        ja     loc_2_1BAF
        mov    eax, dword ptr [ebp - 0x74]
        jmp    dword ptr [eax*4]             ; -> obj2:0x1C0E
        ; ---- dispatch table (6 entries) ----
        dd  loc_2_1BA0
        dd  loc_2_1B8A
        dd  loc_2_1B7B
        dd  loc_2_1B6C
        dd  loc_2_1B5D
        dd  loc_2_1B4F
loc_2_1C26:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1fa8]
        mov    byte ptr [ebp - 0x1c], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x36], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1faa]
        mov    byte ptr [ebp - 0x18], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x78], eax
        jmp    loc_2_1C8D
loc_2_1C5E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3f], 0x800
        jmp    loc_2_1CA6
loc_2_1C6C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3f], 0x1000
        jmp    loc_2_1CA6
loc_2_1C7A:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3f], 0x1170
        jmp    loc_2_1CA6
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1C8D:
        cmp    dword ptr [ebp - 0x78], 0xd
        je     loc_2_1C7A
        cmp    dword ptr [ebp - 0x78], 0xe
        je     loc_2_1C6C
        jmp    loc_2_1C5E
loc_2_1CA6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1fac]
        mov    byte ptr [ebp - 0x18], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x7c], eax
        jmp    loc_2_1D10
loc_2_1CC5:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x41], 0x800
        jmp    loc_2_1D38
loc_2_1CD3:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x41], 0x1000
        jmp    loc_2_1D38
loc_2_1CE1:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x41], 0x2000
        jmp    loc_2_1D38
loc_2_1CEF:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x41], 0x4000
        jmp    loc_2_1D38
loc_2_1CFD:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x41], 0x4628
        jmp    loc_2_1D38
        db  0E9h, 028h, 000h, 000h, 000h
loc_2_1D10:
        sub    dword ptr [ebp - 0x7c], 0xb
        cmp    dword ptr [ebp - 0x7c], 3
        ja     loc_2_1CC5
        mov    eax, dword ptr [ebp - 0x7c]
        jmp    dword ptr [eax*4]             ; -> obj2:0x1D28
        ; ---- dispatch table (4 entries) ----
        dd  loc_2_1CFD
        dd  loc_2_1CEF
        dd  loc_2_1CE1
        dd  loc_2_1CD3
loc_2_1D38:
        mov    al, 1
        jmp    loc_2_1D3F
loc_2_1D3F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_1D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x241c]       ; -> obj1:0x241C
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_2176
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_1D80
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_1D80:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x2e], 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 0x18], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 0x19], al
        add    ecx, 0x18
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1e], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x22], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x26], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x2a], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E20:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_217C
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_1E59
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1E59:
        mov    eax, dword ptr [0x241c]       ; -> obj1:0x241C
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_2176
        or     eax, eax
        je     loc_2_1E79
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1E79:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_1E80:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_1E93
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_1E80
        jmp    loc_2_1EA2
loc_2_1E93:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_1EA2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_1EB8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1EB8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x1c], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_2_1F20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2240
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_1F49
        cmp    ebx, 0x103
        jne    loc_2_1F61
loc_2_1F49:
        push   0
        push   0x2418                        ; -> obj1:0x2418
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2010
        or     eax, eax
        je     loc_2_1F61
        mov    ebx, eax
loc_2_1F61:
        or     ebx, ebx
        je     loc_2_1F6D
        cmp    ebx, 0x103
        jne    loc_2_1F98
loc_2_1F6D:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_1F98
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_1F98
        push   0
        push   0x2418                        ; -> obj1:0x2418
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_20B0
        or     eax, eax
        je     loc_2_1F98
        mov    ebx, eax
loc_2_1F98:
        or     ebx, ebx
        je     loc_2_1FA4
        cmp    ebx, 0x103
        jne    loc_2_2005
loc_2_1FA4:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_2000
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_1FBA
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1FBA:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_1FC9
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_1FCF
loc_2_1FC9:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_1FCF:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_1FDE
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_20D8
loc_2_1FDE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_217C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_2000:
        mov    ebx, 0xc001000e
loc_2_2005:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_2010:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 0x10
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [edi + 4], 0
        or     eax, eax
        je     loc_2_2070
loc_2_2063:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_2063
loc_2_2070:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     loc_2_20A4
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x1e]
        or     eax, eax
        je     loc_2_20A6
        cmp    eax, 0x103
        je     loc_2_20A6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0xc], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_20A4:
        sub    eax, eax
loc_2_20A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_20B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        mov    al, byte ptr [0x2418]         ; -> obj1:0x2418
        cmp    byte ptr [ecx], al
        jne    loc_2_2158
        mov    al, byte ptr [0x2419]         ; -> obj1:0x2419
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_2158
        mov    al, byte ptr [0x241a]         ; -> obj1:0x241A
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_2158
        mov    al, byte ptr [0x241b]         ; -> loc_1_241B
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_2158
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_2139
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_21D3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2139:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_214D
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_214D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2158:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_2190
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_2174
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2174:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_21D3
        mov    byte ptr [ebx + 0x1d], 1
        inc    dword ptr [esi + 0xc]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_2190:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_21A6
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_21A6:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_21C0
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_21C0
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_21C0:
        cmp    eax, 1
        jne    loc_2_21D3
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_21D3
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_21D3:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_2_2206
        cmp    eax, 0x103
        je     loc_2_2206
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2206:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_2220
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2220:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2240:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        mov    dword ptr [esi + 0x10], 0
        je     loc_2_227D
        lea    esp, [esp]
loc_2_2270:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_2270
loc_2_227D:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_22B1
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x26]
        or     eax, eax
        je     loc_2_22B3
        cmp    eax, 0x103
        je     loc_2_22B3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_22B1:
        sub    eax, eax
loc_2_22B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0xe4  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh, 001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h
        db  004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h
        db  013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h
        db  009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h
        db  00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h
        db  001h, 001h, 002h, 000h, 002h, 001h, 002h, 000h, 003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h
        db  005h, 001h, 002h, 000h, 001h, 001h, 001h, 002h, 002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h
        db  004h, 001h, 001h, 002h, 005h, 001h, 001h, 002h, 006h, 001h, 001h, 002h, 007h, 001h, 001h, 002h
        db  001h, 001h, 002h, 002h, 002h, 001h, 002h, 002h, 001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h
        db  003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h
        db  011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h
        db  008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h
        db  00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h
        db  00Fh, 001h, 001h, 000h, 001h, 001h, 001h, 002h, 002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h
        db  004h, 001h, 001h, 002h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x4150  R-X
loc_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_21A0
loc_5_002C:
        cmp    dword ptr [ebp - 0xc], 0xe
        jb     loc_5_0048
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x123], ecx
        jae    loc_5_0052
loc_5_0048:
        mov    eax, 0xc001000f
        jmp    loc_5_01E9
loc_5_0052:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b5]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0131
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0125
        test   byte ptr [ebp + 0xc], 1
        je     loc_5_009A
loc_5_009A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x12b], 0
        jne    loc_5_00CD
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x12b], eax
        jmp    loc_5_00DC
loc_5_00CD:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x12f]
        mov    dword ptr [ecx + 0x24], eax
loc_5_00DC:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x12f], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x18]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0120
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2BB4
loc_5_0120:
        jmp    loc_5_012C
loc_5_0125:
        mov    dword ptr [ebp - 0x10], 0xc0010002
loc_5_012C:
        jmp    loc_5_0192
loc_5_0131:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0153
        mov    dword ptr [ebp - 0x10], 0xc001000d
        jmp    loc_5_0192
loc_5_0153:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b3]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0175
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_0192
loc_5_0175:
        push   2
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_018F:
        add    esp, 0x14
loc_5_0192:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_5_01E1
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_01D3
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x1c]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_01E1
loc_5_01D3:
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_5_37FB
        jmp    loc_5_01E1
loc_5_01E1:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_5_01E9
loc_5_01E9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_5_01F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b5]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0499
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_048D
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11a]
        mov    word ptr [ebp - 0x18], ax
        mov    dword ptr [ebp - 0x28], 0
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        jmp    loc_5_0260
loc_5_0260:
        jmp    loc_5_0265
loc_5_0265:
        push   0
        lea    eax, [ebp - 0x34]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_21A0
loc_5_027A:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_5_047B
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x34]
        push   eax
        call   sub_1_21C4
loc_5_0295:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xf5]
        add    eax, 2
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x11c]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 0x1c]
        add    eax, 6
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0x11c]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    dword ptr [ebp - 0x14], eax
loc_5_0308:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp - 0x28], eax
        jae    loc_5_0473
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_5_0350
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x34]
        push   eax
        call   sub_1_21E8
loc_5_032B:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_033A
        jmp    loc_5_0473
loc_5_033A:
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x34]
        push   eax
        call   sub_1_21C4
loc_5_034B:
        jmp    loc_5_0308
loc_5_0350:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_5_03C0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0x18], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_5_0379
        jmp    loc_5_0473
loc_5_0379:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 6
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 0x14], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 0x14]
        mov    al, byte ptr [ebp - 0x13]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_5_0308
loc_5_03C0:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_5_0408
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 0x14], eax
        jae    loc_5_03F1
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x14]
        neg    eax
        sub    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp - 0x14], 0
        jmp    loc_5_0308
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_03F1:
        mov    eax, dword ptr [ebp + 0x10]
        add    dword ptr [ebp - 0x2c], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0x10]
        neg    eax
        sub    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp + 0x10], 0
loc_5_0408:
        mov    eax, dword ptr [ebp + 0x14]
        sub    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x24]
        cmp    eax, ecx
        jb     loc_5_0421
        mov    eax, ecx
loc_5_0421:
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x3c]
        mov    ecx, dword ptr [ebp - 0x38]
        cmp    eax, ecx
        jb     loc_5_0434
        mov    eax, ecx
loc_5_0434:
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_225C
loc_5_0448:
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x28], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x38]
        neg    eax
        sub    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x38]
        neg    eax
        sub    dword ptr [ebp - 0x24], eax
        jmp    loc_5_0308
loc_5_0473:
        mov    eax, dword ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
loc_5_047B:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_0494
loc_5_048D:
        mov    dword ptr [ebp - 8], 0xc001000c
loc_5_0494:
        jmp    loc_5_04FA
loc_5_0499:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_04BB
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_5_04FA
loc_5_04BB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b4]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_04DD
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_5_04FA
loc_5_04DD:
        push   1
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_04F7:
        add    esp, 0x14
loc_5_04FA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x40]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x40]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_5_0549
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_053B
        mov    eax, dword ptr [ebp - 0x40]
        mov    ecx, dword ptr [ebp - 0x40]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_0549
loc_5_053B:
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        call   sub_5_37FB
        jmp    loc_5_0549
loc_5_0549:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_0551
loc_5_0551:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
sub_5_0558:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_0580
        jmp    loc_5_0695
loc_5_0580:
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21A0
loc_5_0595:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_05A4
        jmp    loc_5_0695
loc_5_05A4:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_21C4
loc_5_05B5:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_5_068D
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_5_05FD
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_21E8
loc_5_05D8:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_5_05E7
        jmp    loc_5_068D
loc_5_05E7:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_21C4
loc_5_05F8:
        jmp    loc_5_05B5
loc_5_05FD:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_0645
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jbe    loc_5_062E
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x10]
        neg    eax
        sub    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_5_05B5
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_062E:
        mov    eax, dword ptr [ebp + 0xc]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0xc]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp + 0xc], 0
loc_5_0645:
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jb     loc_5_0658
        mov    eax, ecx
loc_5_0658:
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_225C
loc_5_066C:
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x18]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        jmp    loc_5_05B5
loc_5_068D:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
loc_5_0695:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_5_06C4
        jmp    loc_5_07E5
loc_5_06C4:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_21A0
loc_5_06D9:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_06E8
        jmp    loc_5_07E5
loc_5_06E8:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_21C4
loc_5_06F9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_5_06FF:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 0x10], eax
        jae    loc_5_07DD
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_0747
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_21E8
loc_5_0722:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_5_0731
        jmp    loc_5_07DD
loc_5_0731:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_21C4
loc_5_0742:
        jmp    loc_5_06FF
loc_5_0747:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_078F
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_5_0778
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0xc]
        neg    eax
        sub    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_06FF
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_0778:
        mov    eax, dword ptr [ebp + 0x14]
        add    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0x14]
        neg    eax
        sub    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp + 0x14], 0
loc_5_078F:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    eax, ecx
        jb     loc_5_07AE
        mov    eax, ecx
loc_5_07AE:
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_225C
loc_5_07C2:
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 0x10], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x1c]
        neg    eax
        sub    dword ptr [ebp - 0xc], eax
        jmp    loc_5_06FF
loc_5_07DD:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
loc_5_07E5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_0814
        jmp    loc_5_0A05
loc_5_0814:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_21A0
loc_5_0829:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_0838
        jmp    loc_5_0A05
loc_5_0838:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_21C4
loc_5_0849:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_21A0
loc_5_085E:
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_5_086D
        jmp    loc_5_0A05
loc_5_086D:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_21C4
loc_5_087E:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_5_09FD
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_5_08C6
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_21E8
loc_5_08A1:
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_5_08B0
        jmp    loc_5_09FD
loc_5_08B0:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_21C4
loc_5_08C1:
        jmp    loc_5_087E
loc_5_08C6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_0902
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_21E8
loc_5_08DD:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_08EC
        jmp    loc_5_09FD
loc_5_08EC:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_21C4
loc_5_08FD:
        jmp    loc_5_087E
loc_5_0902:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_094A
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jbe    loc_5_0933
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x10]
        neg    eax
        sub    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_5_087E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_0933:
        mov    eax, dword ptr [ebp + 0xc]
        add    dword ptr [ebp - 0x1c], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0xc]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp + 0xc], 0
loc_5_094A:
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_0992
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0x18], eax
        jbe    loc_5_097B
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_087E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_097B:
        mov    eax, dword ptr [ebp + 0x18]
        add    dword ptr [ebp - 0x18], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0x18]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 0x18], 0
loc_5_0992:
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jb     loc_5_09AB
        mov    eax, ecx
loc_5_09AB:
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x28]
        cmp    eax, ecx
        jb     loc_5_09BE
        mov    eax, ecx
loc_5_09BE:
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_225C
loc_5_09D2:
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x28]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x28]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        jmp    loc_5_087E
loc_5_09FD:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
loc_5_0A05:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
loc_5_0A0C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0x24
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xdb
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 0x44], al
        mov    al, 1
        jmp    loc_5_0A4A
loc_5_0A4A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0A51:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0x18
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xe7
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 0x44], al
        mov    al, 1
        jmp    loc_5_0A8F
loc_5_0A8F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0A96:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1ac], 1
        mov    al, 1
        jmp    loc_5_0AB6
loc_5_0AB6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0ABD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 0
        mov    al, 1
        jmp    loc_5_0ADA
loc_5_0ADA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0AE1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e09]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x2a]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0B42
        mov    eax, dword ptr [ebp - 4]
        inc    byte ptr [eax + 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x18]
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_5_0B42
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x55]
        inc    edx
        xor    al, al
        out    dx, al
        xor    al, al
        jmp    loc_5_0D22
loc_5_0B42:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        not    eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xf1]
        mov    byte ptr [ecx + 0x1e29], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b5]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0B90
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xdf
        mov    ecx, dword ptr [ebp - 4]
        or     al, byte ptr [ecx + 0x45]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x45], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x20
        mov    byte ptr [ebp - 8], al
        jmp    loc_5_0BA8
loc_5_0B90:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x45]
        and    eax, 0xff
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x45], 0
loc_5_0BA8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        or     al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x44], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1ac]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0BDC
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1ad], 1
loc_5_0BDC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1b2]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0C96
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        add    eax, ecx
        mov    word ptr [ebp - 0x18], ax
        mov    ax, word ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xf9], ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf5]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        add    eax, ecx
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_5_0C70
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x2a], 1
loc_5_0C70:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0C8F
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x55]
        add    edx, 3
        xor    al, al
        out    dx, al
loc_5_0C8F:
        xor    al, al
        jmp    loc_5_0D22
loc_5_0C96:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x34]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0CB5
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 0x55]
        add    edx, 3
        xor    al, al
        out    dx, al
loc_5_0CB5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    loc_5_0CFC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x1c
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x46], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        not    eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0xf1]
        mov    byte ptr [ecx + 0x1e28], al
loc_5_0CFC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0D1B
        mov    al, 1
        jmp    loc_5_0D22
        db  0E9h, 007h, 000h, 000h, 000h
loc_5_0D1B:
        xor    al, al
        jmp    loc_5_0D22
loc_5_0D22:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0D29:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x29], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0D89
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xff
        push   eax
        push   0xc
        push   8
        push   3
        push   0xaa000003
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_0D7F:
        add    esp, 0x18
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x46], 0
loc_5_0D89:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        test   al, 0x24
        je     loc_5_0DB8
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x47]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0DB8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_0DF3
loc_5_0DB8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        test   al, 0x18
        je     loc_5_0DEC
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x48]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0DEC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_1700
        jmp    loc_5_0DEC
loc_5_0DEC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_0DF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x7c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0E8B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1ad]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0E8B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2283
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_5_0E86
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0E78
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_0E86
loc_5_0E78:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_37FB
        jmp    loc_5_0E86
loc_5_0E86:
        jmp    loc_5_16F9
loc_5_0E8B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x47], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], eax
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   0                             ; -> loc_5_0A0C
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_216A
loc_5_0EB6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   al, 0x24
        je     loc_5_16A3
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   al, 0x20
        je     loc_5_1461
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1b4]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_12C2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xf9]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x2c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x2c]
        test   eax, eax
        jne    loc_5_11AD
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x36]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1057
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0x3fff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xfb], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        sar    eax, 0xe
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x10b], al
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0x3fff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xff], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        sar    eax, 0xe
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x10c], al
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0x3fff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x103], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        sar    eax, 0xe
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x10d], al
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0x3fff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x107], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        sar    eax, 0xe
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x10e], al
        jmp    loc_5_1113
loc_5_1057:
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xfb], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xff], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x103], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x107], eax
loc_5_1113:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1b6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x20], eax
loc_5_1153:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x20]
        je     loc_5_119F
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 0x1c]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1192
        mov    eax, dword ptr [ebp - 0x28]
        mov    byte ptr [eax + 0x1c], 0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20AE
loc_5_1192:
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_5_1153
loc_5_119F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2C68
        jmp    loc_5_12BD
loc_5_11AD:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp - 0x24]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1b6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1b6]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1b6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x20], eax
loc_5_121F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x20]
        je     loc_5_12BD
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 0x1c]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_12B8
        mov    eax, dword ptr [ebp - 0x28]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xffff
        or     eax, 0xc0011000
        push   eax
        push   0xc0010007
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20AE
loc_5_1274:
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_1_217C
loc_5_12AD:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x59]
        jmp    loc_5_12B8
loc_5_12B8:
        jmp    loc_5_121F
loc_5_12BD:
        jmp    loc_5_1461
loc_5_12C2:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x112], 0
        je     loc_5_12FA
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2A97
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x34]
        test   eax, eax
        je     loc_5_12F5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_12F5:
        jmp    loc_5_1461
loc_5_12FA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_1461
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x3c], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     loc_5_1453
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        je     loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 7
        je     loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 6
        je     loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x1e
        je     loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_1392:
        add    esp, 0x18
        jmp    loc_5_144E
loc_5_139A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xd5], 0
        jne    loc_5_13AA
loc_5_13AA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd5]
        cmp    dword ptr [eax + 0x10], 8
        jne    loc_5_1422
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        test   eax, eax
        jne    loc_5_13F4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd5]
        cmp    dword ptr [eax + 4], 0
        je     loc_5_13EB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2E4C
        jmp    loc_5_13F4
loc_5_13EB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2D8F
loc_5_13F4:
        push   0
        push   0x14
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd5]
        push   eax
        call   sub_1_217C
loc_5_1407:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd5], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
        jmp    loc_5_144E
loc_5_1422:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_32E7
        and    eax, 0xff
        test   eax, eax
        je     loc_5_144E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_144E:
        jmp    loc_5_145C
loc_5_1453:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_145C:
        jmp    loc_5_1461
loc_5_1461:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   al, 4
        je     loc_5_167A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xff]
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0xa
        jne    loc_5_1647
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x111]
        and    eax, 0xff
        add    eax, 0x7f
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 0x4c], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 0x48], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x50], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x50]
        test   eax, eax
        jne    loc_5_14DB
        mov    dword ptr [ebp - 0x44], 0
        jmp    loc_5_154A
loc_5_14DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x50]
        cmp    eax, 0x22
        jne    loc_5_1543
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 0x5c], al
        mov    al, byte ptr [ebp - 0x5c]
        and    eax, 0xc0
        shr    eax, 6
        mov    byte ptr [ebp - 0x54], al
        mov    al, byte ptr [ebp - 0x5c]
        and    eax, 0xc
        shr    eax, 2
        mov    byte ptr [ebp - 0x58], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x58]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x54]
        cmp    eax, ecx
        jne    loc_5_152B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x54]
        test   eax, eax
        je     loc_5_1537
loc_5_152B:
        mov    dword ptr [ebp - 0x44], 0x10001
        jmp    loc_5_153E
loc_5_1537:
        mov    dword ptr [ebp - 0x44], 0
loc_5_153E:
        jmp    loc_5_154A
loc_5_1543:
        mov    dword ptr [ebp - 0x44], 0xc0000001
loc_5_154A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x4c]
        inc    eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 0x4c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x4c]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x1bc]
        mov    dword ptr [ebp - 0x64], eax
        cmp    dword ptr [ebp - 0x64], 0
        jne    loc_5_1580
        jmp    loc_5_160C
loc_5_1580:
        mov    eax, dword ptr [ebp - 0x64]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x6c], eax
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_159C
loc_5_159C:
        mov    eax, dword ptr [ebp - 0x6c]
        mov    byte ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 0x1bc], 0
        mov    eax, dword ptr [ebp - 0x64]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x60], eax
        mov    eax, dword ptr [ebp - 0x60]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x68], eax
        cmp    dword ptr [ebp - 0x44], 0
        jne    loc_5_15E6
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x13b]
        jmp    loc_5_15EF
loc_5_15E6:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x143]
loc_5_15EF:
        mov    eax, dword ptr [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x60]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x68]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_5_1606:
        mov    eax, dword ptr [ebp - 0x68]
        dec    dword ptr [eax + 0x14]
loc_5_160C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x48]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x4c]
        cmp    eax, ecx
        jne    loc_5_154A
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x4c]
        inc    eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x111], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
        jmp    loc_5_166A
loc_5_1647:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_1667:
        add    esp, 0x18
loc_5_166A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 1
loc_5_167A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp - 0x70]
        mov    dword ptr [ebp - 0x78], eax
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x74], eax
        lea    eax, [ebp - 0x78]
        push   eax
        push   0                             ; -> loc_5_0A0C
        mov    eax, dword ptr [ebp - 0x70]
        push   eax
        call   sub_1_216A
loc_5_169E:
        jmp    loc_5_0EB6
loc_5_16A3:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x47], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x7c], eax
        mov    eax, dword ptr [ebp - 0x7c]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x7c]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_5_16F9
        mov    eax, dword ptr [ebp - 0x7c]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_16EB
        mov    eax, dword ptr [ebp - 0x7c]
        mov    ecx, dword ptr [ebp - 0x7c]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_16F9
loc_5_16EB:
        mov    eax, dword ptr [ebp - 0x7c]
        push   eax
        call   sub_5_37FB
        jmp    loc_5_16F9
loc_5_16F9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_1700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xa0
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x48], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x4c], eax
        mov    eax, dword ptr [ebp - 0x4c]
        mov    dword ptr [ebp - 0x54], eax
        lea    eax, [ebp - 0x44]
        mov    dword ptr [ebp - 0x50], eax
        lea    eax, [ebp - 0x54]
        push   eax
        push   0                             ; -> loc_5_0A51
        mov    eax, dword ptr [ebp - 0x4c]
        push   eax
        call   sub_1_216A
loc_5_173D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x44]
        test   al, 0x18
        je     loc_5_2211
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x44]
        test   al, 8
        je     loc_5_1F81
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 0x14], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0xa0], eax
        jmp    loc_5_1F4D
loc_5_1775:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x103]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        test   al, 0x40
        je     loc_5_188B
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x14
        lea    eax, [ebp - 0x58]
        push   eax
        call   sub_1_2176
loc_5_17CE:
        test   eax, eax
        je     loc_5_17F8
        push   5
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_17F0:
        add    esp, 0x14
        jmp    loc_5_1F81
loc_5_17F8:
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_5_1848
        mov    eax, dword ptr [ebp - 0x58]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_5_1862
loc_5_1848:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcd]
        mov    ecx, dword ptr [ebp - 0x58]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x58]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcd], eax
loc_5_1862:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_188B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_188B:
        jmp    loc_5_1F81
loc_5_1890:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 0x20], ax
        mov    ax, word ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11a], ax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0x20]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        add    eax, 2
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 0x1c], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 0x1c]
        mov    al, byte ptr [ebp - 0x1b]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x40]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        sub    eax, eax
        mov    ax, word ptr [ebp - 8]
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    eax, dword ptr [ebp - 0x40]
        add    eax, 6
        mov    dword ptr [ebp - 0x48], eax
        mov    eax, dword ptr [ebp - 0x48]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 0x80
        je     loc_5_1957
        mov    eax, dword ptr [ebp - 0x48]
        mov    al, byte ptr [eax + 0xe]
        and    eax, 0x1f
        add    eax, 0xe
        mov    dword ptr [ebp - 0x2c], eax
        jmp    loc_5_195E
loc_5_1957:
        mov    dword ptr [ebp - 0x2c], 0xe
loc_5_195E:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x11c], ax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x13f]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0x2c], eax
        jbe    loc_5_19B6
        cmp    dword ptr [ebp - 8], 6
        jb     loc_5_19B1
        mov    eax, dword ptr [ebp - 0x48]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_1_1BF0
loc_5_19B1:
        jmp    loc_5_19EC
loc_5_19B6:
        mov    eax, dword ptr [ebp - 0x48]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x1c]
        sub    eax, dword ptr [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0x2c]
        add    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_1_1BF0
loc_5_19EC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_1_1E40
loc_5_19FB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x110]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1A47
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x11e], 0
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_375B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_5_1AF0
loc_5_1A47:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x118]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jne    loc_5_1A74
        mov    ax, word ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x116], ax
        jmp    loc_5_1AB2
loc_5_1A74:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x118]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x118]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x5c], eax
        mov    ax, word ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x5c]
        mov    word ptr [ecx], ax
loc_5_1AB2:
        mov    ax, word ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x118], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1bb]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_1AF0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1bb], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 4
loc_5_1AF0:
        jmp    loc_5_1F81
loc_5_1AF5:
        mov    dword ptr [ebp - 0x60], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x103]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0xffff
        mov    al, ch
        mov    word ptr [ebp - 0x64], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   al, 0x80
        je     loc_5_1C19
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x14
        lea    eax, [ebp - 0x68]
        push   eax
        call   sub_1_2176
loc_5_1B5C:
        test   eax, eax
        je     loc_5_1B86
        push   5
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_1B7E:
        add    esp, 0x14
        jmp    loc_5_1F81
loc_5_1B86:
        mov    eax, dword ptr [ebp - 0x68]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x68]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x68]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_5_1BD6
        mov    eax, dword ptr [ebp - 0x68]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd1]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcd], eax
        jmp    loc_5_1BF0
loc_5_1BD6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcd]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x68]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcd], eax
loc_5_1BF0:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1C19
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_1C19:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   al, 0x20
        je     loc_5_1C2D
        or     dword ptr [ebp - 0x60], 0x40
loc_5_1C2D:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   al, 0x40
        je     loc_5_1C44
        or     dword ptr [ebp - 0x60], 0x80
loc_5_1C44:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   al, 0x80
        je     loc_5_1C5B
        or     dword ptr [ebp - 0x60], 0x100
loc_5_1C5B:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 1
        je     loc_5_1C73
        or     dword ptr [ebp - 0x60], 0x200
loc_5_1C73:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 4
        je     loc_5_1C8B
        or     dword ptr [ebp - 0x60], 0x400
loc_5_1C8B:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 8
        je     loc_5_1CA3
        or     dword ptr [ebp - 0x60], 0x800
loc_5_1CA3:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 0x10
        je     loc_5_1CBB
        or     dword ptr [ebp - 0x60], 0x1000
loc_5_1CBB:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 0x20
        je     loc_5_1CD3
        or     dword ptr [ebp - 0x60], 0x2000
loc_5_1CD3:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 0x40
        je     loc_5_1CEB
        or     dword ptr [ebp - 0x60], 0x4000
loc_5_1CEB:
        mov    eax, dword ptr [ebp - 0x64]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_5_1D03
        or     dword ptr [ebp - 0x60], 0x8000
loc_5_1D03:
        cmp    dword ptr [ebp - 0x60], 0
        je     loc_5_1D73
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x6c], eax
loc_5_1D16:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x6c]
        je     loc_5_1D67
        mov    eax, dword ptr [ebp - 0x6c]
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp - 0x70]
        inc    dword ptr [eax + 0x14]
        push   4
        lea    eax, [ebp - 0x60]
        push   eax
        push   0x40010006
        mov    eax, dword ptr [ebp - 0x70]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
loc_5_1D48:
        mov    eax, dword ptr [ebp - 0x70]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
loc_5_1D54:
        mov    eax, dword ptr [ebp - 0x6c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x6c], eax
        mov    eax, dword ptr [ebp - 0x70]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_1D16
loc_5_1D67:
        mov    eax, dword ptr [ebp - 0x60]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x19b], eax
loc_5_1D73:
        jmp    loc_5_1F81
loc_5_1D78:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 0x14], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x1bc]
        mov    dword ptr [ebp - 0x28], eax
        cmp    dword ptr [ebp - 0x28], 0
        jne    loc_5_1DFF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_1DCF
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x112], 0
        jne    loc_5_1DCF
loc_5_1DCF:
        mov    byte ptr [ebp - 0x30], 0
        lea    eax, [ebp - 0x74]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2A97
        mov    dword ptr [ebp - 0x28], eax
        cmp    dword ptr [ebp - 0x28], 0
        jne    loc_5_1DED
loc_5_1DED:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x74]
        test   eax, eax
        jne    loc_5_1DFA
loc_5_1DFA:
        jmp    loc_5_1E0C
loc_5_1DFF:
        mov    byte ptr [ebp - 0x30], 0
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x24], eax
loc_5_1E0C:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 0x3c], ax
        mov    eax, dword ptr [ebp - 0x3c]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x3c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 2
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_1_21A0
loc_5_1E58:
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_5_0558
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x110]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1EBB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x11e], 1
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CFA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_5_1EF8
loc_5_1EBB:
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3798
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1bb]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_1EF8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1bb], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 4
loc_5_1EF8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x30]
        test   eax, eax
        je     loc_5_1F2E
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1F2E
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_1F2E:
        jmp    loc_5_1F81
loc_5_1F33:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 2
        jmp    loc_5_1F81
        db  0E9h, 034h, 000h, 000h, 000h
loc_5_1F4D:
        sub    dword ptr [ebp - 0xa0], 0x81
        cmp    dword ptr [ebp - 0xa0], 3
        ja     loc_5_1F33
        mov    eax, dword ptr [ebp - 0xa0]
        jmp    dword ptr [eax*4]             ; -> obj5:0x1F71
        ; ---- dispatch table (4 entries) ----
        dd  loc_5_1890
        dd  loc_5_1D78
        dd  loc_5_1775
        dd  loc_5_1AF5
loc_5_1F81:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x44]
        test   al, 0x10
        je     loc_5_21D6
        mov    byte ptr [ebp - 0x7c], 0
        mov    byte ptr [ebp - 0x78], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x110]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1FAC
loc_5_1FAC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1bb]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_5_1FC3
loc_5_1FC3:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        cmp    eax, 0xffff
        je     loc_5_1FE1
        mov    byte ptr [ebp - 0x7c], 1
loc_5_1FE1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x133], 0
        je     loc_5_1FF5
        mov    byte ptr [ebp - 0x78], 1
loc_5_1FF5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x7c]
        test   eax, eax
        je     loc_5_211B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x78]
        test   eax, eax
        je     loc_5_2025
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11e]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_211B
loc_5_2025:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        mov    word ptr [ebp - 0x84], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x116]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x118]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_5_207A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x116], 0xffff
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x118], 0xffff
        jmp    loc_5_20B6
loc_5_207A:
        mov    eax, dword ptr [ebp - 0x84]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0x84]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x80], eax
        mov    eax, dword ptr [ebp - 0x80]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x116], ax
loc_5_20B6:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x11e], 0
        mov    eax, dword ptr [ebp - 0x84]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_375B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x78]
        test   eax, eax
        je     loc_5_20FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_5_2116
loc_5_20FC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1bb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x10
loc_5_2116:
        jmp    loc_5_21D6
loc_5_211B:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x78]
        test   eax, eax
        je     loc_5_21CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x133]
        mov    dword ptr [ebp - 0x8c], eax
        mov    eax, dword ptr [ebp - 0x8c]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x88], eax
        mov    eax, dword ptr [ebp - 0x88]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x133], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x11e], 1
        mov    eax, dword ptr [ebp - 0x8c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CFA
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x7c]
        test   eax, eax
        jne    loc_5_2198
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x133], 0
        je     loc_5_21AD
loc_5_2198:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_5_21C7
loc_5_21AD:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1bb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x10
loc_5_21C7:
        jmp    loc_5_21D6
loc_5_21CC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x110], 1
loc_5_21D6:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x90], eax
        mov    eax, dword ptr [ebp - 0x90]
        mov    dword ptr [ebp - 0x98], eax
        lea    eax, [ebp - 0x44]
        mov    dword ptr [ebp - 0x94], eax
        lea    eax, [ebp - 0x98]
        push   eax
        push   0                             ; -> loc_5_0A51
        mov    eax, dword ptr [ebp - 0x90]
        push   eax
        call   sub_1_216A
loc_5_220C:
        jmp    loc_5_173D
loc_5_2211:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x48], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x9c], eax
        mov    eax, dword ptr [ebp - 0x9c]
        dec    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp - 0x9c]
        cmp    dword ptr [eax + 0x59], 0
        jne    loc_5_227C
        mov    eax, dword ptr [ebp - 0x9c]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_226B
        mov    eax, dword ptr [ebp - 0x9c]
        mov    ecx, dword ptr [ebp - 0x9c]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_227C
loc_5_226B:
        mov    eax, dword ptr [ebp - 0x9c]
        push   eax
        call   sub_5_37FB
        jmp    loc_5_227C
loc_5_227C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2283:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x58
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1a4]
        mov    dword ptr [ebp - 0x58], eax
        jmp    loc_5_2A69
loc_5_229D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        shl    eax, 8
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        add    eax, ecx
        mov    word ptr [ebp - 0x18], ax
        mov    ax, word ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xf9], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_5_241D
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x20], eax
loc_5_232C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x20]
        je     loc_5_237B
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        inc    dword ptr [eax + 0x14]
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
loc_5_235C:
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
loc_5_2368:
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x24]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_232C
loc_5_237B:
        push   1
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        push   eax
        push   7
        push   3
        push   4
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_23A0:
        add    esp, 0x1c
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0000
loc_5_23B9:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ac], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20C6
loc_5_2418:
        jmp    loc_5_2A90
loc_5_241D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xf9]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x14], eax
        mov    edi, dword ptr [ebp - 0x14]
        xor    eax, eax
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x127]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x127]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x18], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x129]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x129]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x121]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + 0x121]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11f]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    byte ptr [ecx + 0x1e], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x28
        jmp    loc_5_2A90
loc_5_24F9:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xf9]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_268C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 6
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp - 0x28]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1b6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1b6]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1b6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xffff
        or     eax, 0xc0011000
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x2c], eax
loc_5_25B0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x2c]
        je     loc_5_2601
        mov    eax, dword ptr [ebp - 0x2c]
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, dword ptr [ebp - 0x34]
        inc    dword ptr [eax + 0x14]
        push   4
        lea    eax, [ebp - 0x30]
        push   eax
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x34]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
loc_5_25E2:
        mov    eax, dword ptr [ebp - 0x34]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
loc_5_25EE:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x34]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_25B0
loc_5_2601:
        push   2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1b6]
        and    eax, 0xffff
        push   eax
        push   7
        push   3
        push   4
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_262D:
        add    esp, 0x1c
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0000
loc_5_2646:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ac], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20C6
loc_5_2687:
        jmp    loc_5_2A90
loc_5_268C:
        mov    eax, dword ptr [ebp - 0x28]
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xfb], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xff], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x103], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xf5]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x107], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe5], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        test   eax, eax
        jne    loc_5_27A7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        test   eax, eax
        jne    loc_5_2785
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19FB
loc_5_2780:
        jmp    loc_5_27A2
loc_5_2785:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x28
loc_5_27A2:
        jmp    loc_5_27C4
loc_5_27A7:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x28
loc_5_27C4:
        jmp    loc_5_2A90
loc_5_27C9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x3c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        test   eax, eax
        jne    loc_5_282C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        test   eax, eax
        jne    loc_5_280A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19FB
loc_5_2805:
        jmp    loc_5_2827
loc_5_280A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x28
loc_5_2827:
        jmp    loc_5_292B
loc_5_282C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x40], eax
loc_5_2842:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x40]
        je     loc_5_2891
        mov    eax, dword ptr [ebp - 0x40]
        mov    dword ptr [ebp - 0x44], eax
        mov    eax, dword ptr [ebp - 0x44]
        inc    dword ptr [eax + 0x14]
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x44]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
loc_5_2872:
        mov    eax, dword ptr [ebp - 0x44]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
loc_5_287E:
        mov    eax, dword ptr [ebp - 0x40]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x44]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_2842
loc_5_2891:
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x3c]
        push   eax
        push   7
        push   3
        push   4
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_28B3:
        add    esp, 0x1c
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0000
loc_5_28CC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ac], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20C6
loc_5_292B:
        jmp    loc_5_2A90
loc_5_2930:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 0x48], eax
        mov    eax, dword ptr [ebp - 0x48]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x4c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x4c]
        test   eax, eax
        jne    loc_5_2960
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19FB
loc_5_295B:
        jmp    loc_5_2A5F
loc_5_2960:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19f], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x50], eax
loc_5_2976:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5d
        cmp    eax, dword ptr [ebp - 0x50]
        je     loc_5_29C5
        mov    eax, dword ptr [ebp - 0x50]
        mov    dword ptr [ebp - 0x54], eax
        mov    eax, dword ptr [ebp - 0x54]
        inc    dword ptr [eax + 0x14]
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x54]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20DE
loc_5_29A6:
        mov    eax, dword ptr [ebp - 0x54]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20E4
loc_5_29B2:
        mov    eax, dword ptr [ebp - 0x50]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x50], eax
        mov    eax, dword ptr [ebp - 0x54]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_2976
loc_5_29C5:
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x4c]
        push   eax
        push   7
        push   3
        push   4
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_29E7:
        add    esp, 0x1c
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0000
loc_5_2A00:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1a3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ac], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ae]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20C6
loc_5_2A5F:
        jmp    loc_5_2A90
        db  0E9h, 027h, 000h, 000h, 000h
loc_5_2A69:
        dec    dword ptr [ebp - 0x58]
        cmp    dword ptr [ebp - 0x58], 3
        ja     loc_5_2A90
        mov    eax, dword ptr [ebp - 0x58]
        jmp    dword ptr [eax*4]             ; -> obj5:0x2A80
        ; ---- dispatch table (4 entries) ----
        dd  loc_5_229D
        dd  loc_5_24F9
        dd  loc_5_27C9
        dd  loc_5_2930
loc_5_2A90:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2A97:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xfe
        jne    loc_5_2AD2
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        xor    eax, eax
        jmp    loc_5_2BAD
loc_5_2AD2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x112]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xff
        je     loc_5_2B41
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x143]
        push   0xc0000001
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_5_2B34:
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [eax + 0x14]
        xor    eax, eax
        jmp    loc_5_2BAD
loc_5_2B41:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xc], 1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_2B70
loc_5_2B70:
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + eax*4 + 0x1bc], 0
        je     loc_5_2B8C
loc_5_2B8C:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    cl, byte ptr [ecx + 0xd]
        and    ecx, 0xff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x1bc], eax
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_5_2BAD
loc_5_2BAD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_2BB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xcd], 0
        je     loc_5_2BF6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F52
        cmp    eax, 0x103
        jne    loc_5_2BF6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_5_2C61
loc_5_2BF6:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x12b], 0
        je     loc_5_2C57
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x12b]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x12b], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x112], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CC1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_5_2C61
loc_5_2C57:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 1
loc_5_2C61:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2C68:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_2C93
loc_5_2C93:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0xfe
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2CC1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0xa
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0xfe
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_2CFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_21A0
loc_5_2D1F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x107]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0xa
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xd]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], 0
        sub    eax, eax
        mov    ax, word ptr [ebp - 4]
        mov    al, ah
        mov    ah, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_2D5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0x1e
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0x80
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2D8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp - 4]
        mov    ah, byte ptr [ecx + 6]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xfb]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x13b], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x13f], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x143], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x147], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2E4C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 6]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x14b], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x14f], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x153], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 9]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x157], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x15b], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x15f], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x163], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xe]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x167], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xf]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x16b], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x16f], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_2F52:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
loc_5_2F5B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xcd], 0
        je     loc_5_32D9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcd]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcd]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcd], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_5_2FA8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd1], 0
loc_5_2FA8:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], 8
        jne    loc_5_2FF5
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     loc_5_2FD0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F30
        jmp    loc_5_2FD9
loc_5_2FD0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2D5D
loc_5_2FD9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
        mov    dword ptr [ebp - 8], 0x103
        jmp    loc_5_31A0
loc_5_2FF5:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_5_315C
loc_5_3003:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0x1010e
        jne    loc_5_3045
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd9]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xdd], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd9], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_3090
loc_5_3045:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0x2010103
        jne    loc_5_3075
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_3090
loc_5_3075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe9], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        mov    dword ptr [ebp - 8], eax
loc_5_3090:
        jmp    loc_5_31A0
loc_5_3095:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd9]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xdd], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd9], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_31A0
loc_5_30C7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_31A0
loc_5_30E7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe9], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_31A0
loc_5_3107:
        mov    dword ptr [ebp - 8], 0x103
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F30
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xf1]
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_5_31A0
loc_5_3133:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   1
        push   9
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_3154:
        add    esp, 0x18
        jmp    loc_5_31A0
loc_5_315C:
        dec    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x10], 9
        ja     loc_5_3133
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]            ; -> obj5:0x3196
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x317E
        ; ---- dispatch table (6 entries) ----
        dd  loc_5_3003
        dd  loc_5_3107
        dd  loc_5_3095
        dd  loc_5_30C7
        dd  loc_5_30E7
        dd  loc_5_3133
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 005h, 003h, 004h
loc_5_31A0:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_5_31C8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd5], eax
        mov    eax, 0x103
        jmp    loc_5_32E0
        db  0E9h, 00Ch, 001h, 000h, 000h
loc_5_31C8:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_32B3
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_5_326E
loc_5_31E0:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20CC
loc_5_31FE:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_32AE
loc_5_3209:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_32AE
loc_5_3217:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_32AE
loc_5_3225:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc1]
        push   eax
        call   sub_1_20EA
loc_5_323A:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x59]
        jmp    loc_5_32AE
loc_5_3245:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   9
        push   5
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_3266:
        add    esp, 0x18
        jmp    loc_5_32AE
loc_5_326E:
        dec    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], 9
        ja     loc_5_3245
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax]            ; -> obj5:0x32A4
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x3290
        ; ---- dispatch table (5 entries) ----
        dd  loc_5_31E0
        dd  loc_5_3225
        dd  loc_5_3209
        dd  loc_5_3217
        dd  loc_5_3245
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_32AE:
        jmp    loc_5_32D4
loc_5_32B3:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xa
        push   5
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_32D1:
        add    esp, 0x18
loc_5_32D4:
        jmp    loc_5_2F5B
loc_5_32D9:
        xor    eax, eax
        jmp    loc_5_32E0
loc_5_32E0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_32E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd5]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_3306
loc_5_3306:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_5_3450
loc_5_3314:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_3369
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2E4C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0FFF
loc_5_3337:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd5], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc1]
        push   eax
        call   sub_1_20EA
loc_5_335E:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x59]
        jmp    loc_5_3394
loc_5_3369:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd5], 0
        push   0xc0000001
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc1]
        push   eax
        call   sub_1_20EA
loc_5_338E:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x59]
loc_5_3394:
        jmp    loc_5_3490
loc_5_3399:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_3404
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd5]
        cmp    dword ptr [eax + 0x14], 0x2010104
        jne    loc_5_33CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe9]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xed], eax
loc_5_33CE:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd5], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20CC
loc_5_33F9:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3438
loc_5_3404:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd5], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        push   0xc0000001
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20CC
loc_5_3432:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
loc_5_3438:
        jmp    loc_5_3490
loc_5_343D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3490
        db  0E9h, 040h, 000h, 000h, 000h
loc_5_3450:
        dec    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], 9
        ja     loc_5_3490
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax]            ; -> obj5:0x3486
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x3472
        ; ---- dispatch table (5 entries) ----
        dd  loc_5_3399
        dd  loc_5_3314
        dd  loc_5_343D
        dd  loc_5_343D
        dd  loc_5_3490
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_3490:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_5_3569
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_3529
loc_5_34AB:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0x1010e
        jne    loc_5_34DF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdd]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd9], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe5], 0
        jmp    loc_5_34EC
loc_5_34DF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe1], 0
loc_5_34EC:
        jmp    loc_5_3569
loc_5_34F1:
        jmp    loc_5_3569
loc_5_34F6:
        jmp    loc_5_3569
loc_5_34FB:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   9
        push   6
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_351C:
        add    esp, 0x18
        jmp    loc_5_3569
        db  0E9h, 040h, 000h, 000h, 000h
loc_5_3529:
        dec    dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x18], 9
        ja     loc_5_34FB
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax]            ; -> obj5:0x355F
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x354B
        ; ---- dispatch table (5 entries) ----
        dd  loc_5_34AB
        dd  loc_5_34F1
        dd  loc_5_34F6
        dd  loc_5_34F6
        dd  loc_5_34FB
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_3569:
        mov    al, 1
        jmp    loc_5_3570
loc_5_3570:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_3577:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x7fffffff
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0xda], 0x20
        je     loc_5_35A2
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_35CA
loc_5_35A2:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0xda], 0x40
        je     loc_5_35C3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe1]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_5_35CA
loc_5_35C3:
        mov    dword ptr [ebp - 8], 0
loc_5_35CA:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0xe5], ecx
        jne    loc_5_35E8
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_3608
loc_5_35E8:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3617
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe5], eax
        mov    dword ptr [ebp - 4], 0x103
loc_5_3608:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_3610
loc_5_3610:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_3617:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 7
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0xfe
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 2], ah
        mov    al, byte ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 3], al
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_367D
loc_5_367A:
        inc    dword ptr [ebp - 0xc]
loc_5_367D:
        cmp    dword ptr [ebp - 0xc], 4
        jae    loc_5_369D
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ebp + eax - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 6], al
        jmp    loc_5_367A
loc_5_369D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_36A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe9]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36CE
        mov    eax, 0x103
        jmp    loc_5_36C7
loc_5_36C7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_36CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 6
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0xfe
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 2], ah
        mov    al, byte ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 3], al
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_3734
loc_5_3731:
        inc    dword ptr [ebp - 0xc]
loc_5_3734:
        cmp    dword ptr [ebp - 0xc], 4
        jae    loc_5_3754
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ebp + eax - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 6], al
        jmp    loc_5_3731
loc_5_3754:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_375B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x107]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0x81
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_3798:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_37AF
loc_5_37AF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x133], 0
        jne    loc_5_37D0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x133], eax
        jmp    loc_5_37DF
loc_5_37D0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x133]
        mov    dword ptr [ecx + 0x24], eax
loc_5_37DF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x137], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_37FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x59]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_3846
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1a4], 0
        jne    loc_5_3846
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1a4], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_17F4
loc_5_3846:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1a3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_3A1F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x6d
        cmp    dword ptr [eax + 0x6d], ecx
        je     loc_5_3A1F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x6d]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc9]
        push   eax
        call   sub_2_1F20
loc_5_3895:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_3969
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_5_3911
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1a8]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_38EE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00C8
loc_5_38EE:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20B4
loc_5_38FC:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_217C
loc_5_390C:
        jmp    loc_5_3964
loc_5_3911:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x69]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
loc_5_3964:
        jmp    loc_5_3A1F
loc_5_3969:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_5_39FE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_399F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x10f], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_399F:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x65
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x69]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x69]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
        jmp    loc_5_3A1F
loc_5_39FE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xa
        push   7
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_3A1C:
        add    esp, 0x18
loc_5_3A1F:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x65
        cmp    dword ptr [eax + 0x65], ecx
        je     loc_5_3AA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x65]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_5_3AA4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1a8]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_3A65
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00C8
loc_5_3A65:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20B4
loc_5_3A73:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_217C
loc_5_3AA4:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x59]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_3AB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x29]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_3D0F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x112], 0
        jne    loc_5_3B03
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x12b], 0
        jne    loc_5_3B03
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xcd], 0
        je     loc_5_3D0F
loc_5_3B03:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1b5], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x29], 0
        push   0
        push   0xaa000003
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc5]
        push   eax
        call   sub_1_2248
loc_5_3B2A:
        add    esp, 0xc
loc_5_3B2D:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xcd], 0
        je     loc_5_3C57
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xcd]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xcd]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xcd], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xcd], 0
        jne    loc_5_3B7A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xd1], 0
loc_5_3B7A:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_3C0E
loc_5_3B88:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20CC
loc_5_3BA3:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3C52
loc_5_3BAE:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3C52
loc_5_3BBC:
        push   0
        push   0x14
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_217C
loc_5_3BC9:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xd5], 0
        jmp    loc_5_3C52
loc_5_3BDB:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3C52
loc_5_3BE9:
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc1]
        push   eax
        call   sub_1_20EA
loc_5_3BFE:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x59]
        jmp    loc_5_3C52
        db  0E9h, 044h, 000h, 000h, 000h
loc_5_3C0E:
        dec    dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x18], 9
        ja     loc_5_3C52
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax]            ; -> obj5:0x3C48
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x3C30
        ; ---- dispatch table (6 entries) ----
        dd  loc_5_3B88
        dd  loc_5_3BE9
        dd  loc_5_3BAE
        dd  loc_5_3BBC
        dd  loc_5_3BDB
        dd  loc_5_3C52
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h, 004h, 004h
loc_5_3C52:
        jmp    loc_5_3B2D
loc_5_3C57:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x112], 0
        je     loc_5_3CAA
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x112]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x112], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_5_3CA4:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
loc_5_3CAA:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x12b], 0
        je     loc_5_3D03
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x12b]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x12b], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_20BA
loc_5_3CF8:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_3CAA
loc_5_3D03:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x29], 0
        jmp    loc_5_3D46
loc_5_3D0F:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x112], 0
        jne    loc_5_3D3F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x12b], 0
        jne    loc_5_3D3F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xcd], 0
        je     loc_5_3D46
loc_5_3D3F:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x29], 1
loc_5_3D46:
        push   0x7530
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x1d
        push   eax
        call   sub_1_2152
loc_5_3D57:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
        db  000h, 000h
sub_5_3D60:
        pushfd 
        pop    eax
        cli    
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3D70:
        push   ebp
        xor    al, al
        mov    ebp, esp
        mov    edx, dword ptr [esp + 8]
        out    dx, al
        push   dword ptr [ebp + 0xc]
        popfd  
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3D90:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        mov    byte ptr [edx + 0x56], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [edx + 0x52]
        push   esi
        call   sub_5_3D60
        push   esi
        mov    dword ptr [ebp - 0x10], eax
        call   sub_5_4010
        mov    edx, esi
        mov    al, 0xd0
        out    dx, al
        mov    word ptr [ebp - 2], 1
loc_5_3DBE:
        push   esi
        call   sub_5_4010
        push   7
        push   esi
        call   sub_5_4040
        cmp    ax, 0x6d50
        jne    loc_5_3F1F
        push   0
        push   esi
        call   sub_5_4040
        push   1
        push   esi
        call   sub_5_4040
        push   2
        push   esi
        call   sub_5_4040
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [ebp - 2]
        push   8
        lea    eax, [ecx + ecx*4]
        push   esi
        mov    dword ptr [ebp - 0xc], ecx
        lea    edi, [edx + eax*2]
        call   sub_5_4040
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    bx, ax
        push   9
        mov    word ptr [edi + 0x4d], ax
        push   esi
        mov    word ptr [ecx*2], bx          ; -> loc_1_2448
        call   sub_5_4040
        mov    word ptr [edi + 0x4f], ax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx*2], ax          ; -> loc_1_2438
        push   0xa
        mov    word ptr [ebp - 4], ax
        push   esi
        call   sub_5_4040
        mov    ecx, dword ptr [ebp - 0xc]
        test   byte ptr [ebp - 4], 0x20
        mov    word ptr [ecx*2], ax          ; -> loc_1_245C
        jne    loc_5_3E5E
        mov    dword ptr [0], 0              ; -> loc_1_2324
        jmp    loc_5_3E77
loc_5_3E5E:
        mov    dword ptr [0], 1              ; -> loc_1_2324
        inc    bx
        and    bx, 1
        shl    bx, 2
        add    bx, 0xa20
loc_5_3E77:
        mov    ax, word ptr [ebp - 6]
        and    ax, 0x180
        shr    ax, 6
        mov    cx, word ptr [ebp - 4]
        and    eax, 0xffff
        and    cx, 0x40
        shr    cx, 6
        and    ecx, 0xffff
        add    ecx, eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [ecx*2]          ; -> loc_1_2408
        cmp    dword ptr [0], 0              ; -> loc_1_2324
        mov    word ptr [eax*2], dx          ; -> obj1:0x2424
        jne    loc_5_3EF3
        mov    ax, word ptr [edi + 0x4f]
        and    eax, 0x1f
        sub    eax, 0x1f
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    word ptr [edi + 0x51], ax
        test   ax, ax
        jne    loc_5_3F1F
        mov    ax, word ptr [edi + 0x4f]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ax, 0x1f
        shl    ax, 4
        add    ax, 0x200
        mov    word ptr [edi + 0x51], ax
        mov    word ptr [ecx*2], ax          ; -> loc_1_250C
        jmp    loc_5_3F09
loc_5_3EF3:
        mov    word ptr [edi + 0x51], bx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [0], bx              ; -> loc_1_2434
        mov    word ptr [ecx*2], bx          ; -> loc_1_250C
loc_5_3F09:
        mov    ax, word ptr [ebp - 2]
        mov    edx, esi
        sub    al, 0x30
        out    dx, al
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edi + 0x55], 1
        inc    byte ptr [edx + 0x56]
loc_5_3F1F:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 8
        jb     loc_5_3DBE
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   esi
        call   sub_5_3D70
        movsx  ecx, byte ptr [0]             ; -> obj1:0x2314
        mov    ax, word ptr [ecx*2]          ; -> loc_1_250C
        lea    edx, [ecx*2]
        mov    bx, word ptr [edx]            ; -> loc_1_2448
        mov    si, word ptr [edx]            ; -> loc_1_2438
        lea    ecx, [ecx + ecx*4]
        mov    word ptr [0], ax              ; -> loc_1_2434
        mov    ax, word ptr [edx]            ; -> loc_1_245C
        mov    word ptr [0], bx              ; -> obj1:0x2422
        mov    bx, word ptr [edx]            ; -> obj1:0x2424
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [0], si              ; -> obj1:0x2420
        pop    edi
        mov    word ptr [0], ax              ; -> loc_1_2458
        pop    esi
        mov    word ptr [edx + ecx*2 + 0x53], bx
        xor    eax, eax
        mov    al, byte ptr [edx + 0x56]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_3FA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x52]
        push   edi
        call   sub_5_3D60
        cmp    byte ptr [esi + 0x56], 1
        mov    ebx, eax
        jb     loc_5_3FFB
        push   edi
        call   sub_5_4010
        mov    edx, edi
        mov    al, 0xd1
        out    dx, al
        cmp    byte ptr [0], 0               ; -> obj1:0x231C
        mov    al, 0xff
        out    dx, al
        jne    loc_5_3FE7
        movsx  eax, byte ptr [0]             ; -> obj1:0x2314
        xor    ecx, ecx
        lea    edx, [eax + eax*4]
        mov    cx, word ptr [esi + edx*2 + 0x53]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ecx
loc_5_3FE7:
        mov    byte ptr [esi + 0x60], 1
        push   ebx
        push   edi
        call   sub_5_3D70
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_5_3FFB:
        push   ebx
        push   edi
        call   sub_5_3D70
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_5_4010:
        push   ebp
        xor    al, al
        mov    ebp, esp
        mov    ecx, 0xff
        mov    edx, dword ptr [ebp + 8]
        out    dx, al
        out    dx, al
        mov    ax, cx
loc_5_4022:
        out    dx, al
        test   al, 0x80
        je     loc_5_4033
        add    ax, ax
        xor    al, 0x63
        dec    ecx
        jne    loc_5_4022
        pop    ebp
        ret    4
loc_5_4033:
        add    ax, ax
        dec    ecx
        jne    loc_5_4022
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_4040:
        push   ebp
        mov    al, byte ptr [esp + 0xc]
        mov    ebp, esp
        add    al, 0x80
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x1e
        mov    edx, esi
        out    dx, al
loc_5_4056:
        push   0x64
        call   sub_1_2170
loc_5_405D:
        dec    edi
        jne    loc_5_4056
        xor    ecx, ecx
        mov    edi, 0xf
loc_5_4067:
        mov    edx, esi
        in     al, dx
        and    eax, 1
        mov    edx, edi
        dec    edi
        lea    ecx, [eax + ecx*2]
        test   edx, edx
        jne    loc_5_4067
        mov    ax, cx
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  055h, 08Ah, 044h, 024h, 00Ch, 08Bh, 0ECh, 004h, 080h, 056h, 057h, 08Bh, 075h, 008h, 0BFh, 009h
        db  000h, 000h, 000h, 08Bh, 096h, 0A1h, 000h, 000h, 000h, 0EEh, 08Bh, 056h, 055h, 083h, 0C2h, 00Ah
        db  066h, 0EDh, 0F6h, 0C4h, 080h, 074h, 012h, 06Ah, 00Ah, 0E8h
        dd  sub_1_2170               ; fixup
        db  08Bh, 056h, 055h, 083h, 0C2h, 00Ah, 04Fh, 066h, 0EDh, 075h, 0E9h, 08Bh, 056h, 055h, 05Fh, 083h
        db  0C2h, 00Ch, 05Eh, 066h, 0EDh, 05Dh, 0C2h, 008h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_5_40D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x55]
        lea    edx, [ecx + 0xe]
        out    dx, ax
        lea    edx, [ecx + 6]
        in     ax, dx
        mov    word ptr [0], ax              ; -> obj1:0x2420
        lea    edx, [ecx + 8]
        mov    di, ax
        in     ax, dx
        mov    word ptr [0], ax              ; -> obj1:0x2422
        lea    edx, [ecx + 4]
        mov    bx, ax
        in     ax, dx
        and    bx, 0x180
        shr    bx, 6
        and    di, 0x40
        shr    di, 6
        xor    edx, edx
        mov    word ptr [0], ax              ; -> loc_1_2458
        mov    ecx, edi
        mov    eax, ebx
        and    ecx, 0xffff
        and    eax, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dx, word ptr [eax*2]          ; -> loc_1_2408
        pop    edi
        mov    eax, 0x100
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [esi + 0x55]
        pop    esi
        add    edx, 0xe
        pop    ebx
        out    dx, ax
        pop    ebp
        ret    8
        db  090h
VxD__LTEXT ENDS
