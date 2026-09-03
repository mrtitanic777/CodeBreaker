; WSVV -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 25453 instructions recovered by recursive descent

Declare_Virtual_Device WSVV, 1, 0, WSVV_Control, 030F6h, Undefined_Init_Order
;   DDK version 0400h   init_order 05400C000h   flags 00000h
;   service table: obj1:0x12b3c, 1 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x13958  R-X
loc_1_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax + 0x84]
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, esi
        cmp    eax, 0x4c
        je     loc_1_0161
        call   sub_1_2880
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0031
        mov    edi, 0x2747
        jmp    loc_1_0166
loc_1_0031:
        push   esi
        call   sub_1_0F09
        mov    ebx, eax
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 4
        dec    dword ptr [eax + 0x48]
        push   edi
        inc    dword ptr [esi + 0x38]
        push   dword ptr [esp + 0x18]
        call   sub_1_2960
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 8
        mov    dword ptr [edi + 0x68], eax
        mov    ebp, dword ptr [ebx + 0x58]
        mov    dword ptr [edi + 0x88], ebx
        lea    eax, [ebx + 0x1c]
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x20], 0x18
        lea    eax, [ebx + 0x34]
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x24], 0x18
        mov    dword ptr [edi + 0x8c], ebp
        mov    dword ptr [ebp], edi
        mov    dword ptr [ebx + 0x10], edi
        push   ebx
        call   sub_1_074D
        add    esp, 4
        test   byte ptr [edi + 0x40], 8
        je     loc_1_00A5
        push   1
        push   2
        push   ebx
        call   sub_1_07CD
        add    esp, 0xc
loc_1_00A5:
        test   byte ptr [edi + 0x40], 0x10
        je     loc_1_00B8
        push   1
        push   3
        push   ebx
        call   sub_1_07CD
        add    esp, 0xc
loc_1_00B8:
        test   byte ptr [edi + 0x80], 8
        je     loc_1_00CE
        push   1
        push   1
        push   ebx
        call   sub_1_07CD
        add    esp, 0xc
loc_1_00CE:
        cmp    dword ptr [ebx + 0x18], 0x60000002
        jne    loc_1_00DE
        mov    dword ptr [ebx + 0x18], 0x60000003
loc_1_00DE:
        push   0
        push   2
        push   edi
        call   sub_1_F3AC
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0xc
        mov    ecx, dword ptr [eax + 0x50]
        and    ecx, 0xfffffff7
        mov    dword ptr [eax + 0x50], ecx
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, esi
        cmp    eax, 0x4c
        jne    loc_1_010C
        mov    eax, dword ptr [esp + 0x14]
        and    ecx, 0xfffffff7
        mov    dword ptr [eax + 0x50], ecx
loc_1_010C:
        mov    eax, dword ptr [ebp + 0x2c]
        sub    eax, ebp
        cmp    eax, 0x2c
        je     loc_1_013D
        mov    eax, dword ptr [ebp + 0x34]
        sub    eax, ebp
        cmp    eax, 0x34
        je     loc_1_012D
        push   0
        push   1
        push   edi
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_012D:
        mov    eax, dword ptr [ebp + 0x3c]
        sub    eax, ebp
        cmp    eax, 0x3c
        je     loc_1_0157
        push   0
        push   4
        jmp    loc_1_014E
loc_1_013D:
        test   byte ptr [ebx + 0x1b], 0x10
        je     loc_1_0157
        mov    dword ptr [edi + 0x4c], 7
        push   0
        push   0x20
loc_1_014E:
        push   edi
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_0157:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], edi
        sub    edi, edi
        jmp    loc_1_0166
loc_1_0161:
        mov    edi, 0x2733
loc_1_0166:
        push   esi
        call   sub_1_0E7C
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0180:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    ebp, dword ptr [esp + 0x40]
        cmp    ebp, 0x18
        je     loc_1_019C
        mov    edi, 0x271e
        jmp    loc_1_032C
loc_1_019C:
        push   ebp
        push   dword ptr [esp + 0x40]
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    loc_1_01B7
        mov    edi, 0x271e
        jmp    loc_1_032C
loc_1_01B7:
        lea    edi, [esp + 0x1c]
        mov    esi, eax
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   ebp
        push   eax
        call   sub_1_F2B5
        mov    esi, dword ptr [esp + 0x40]
        add    esp, 8
        push   esi
        call   sub_1_29BC
        movzx  eax, word ptr [esp + 0x20]
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2F7E
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0323
        test   byte ptr [esi + 0x40], 0x20
        jne    loc_1_0241
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0B56
        add    esp, 8
        or     eax, eax
        je     loc_1_0241
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 0x2740
        jmp    loc_1_032C
loc_1_0241:
        call   sub_1_09F0
        mov    edi, eax
        or     edi, edi
        jne    loc_1_025F
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 0x2747
        jmp    loc_1_032C
loc_1_025F:
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_02A4
        push   0
        call   sub_1_1760
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0291
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 0x2747
        jmp    loc_1_032C
loc_1_0291:
        mov    dword ptr [edi + 0x54], ebx
        mov    dword ptr [esi + 0x8c], ebx
        push   esi
        push   ebx
        call   sub_1_17E3
        add    esp, 8
loc_1_02A4:
        movzx  eax, word ptr [esp + 0x1e]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        push   esi
        push   edi
        call   sub_1_0C44
        add    esp, 0x20
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_02E6
        mov    dword ptr [edi + 0x18], esi
        mov    dword ptr [esi + 0x4c], 2
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    edi, edi
        jmp    loc_1_032C
loc_1_02E6:
        or     ebx, ebx
        je     loc_1_0304
        push   ebx
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [edi + 0x54], 0
        mov    dword ptr [esi + 0x8c], 0
loc_1_0304:
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        push   ebp
        call   sub_1_2C80
        add    esp, 4
        mov    edi, eax
        jmp    loc_1_032C
loc_1_0323:
        push   esi
        call   sub_1_29C4
        add    esp, 4
loc_1_032C:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0340:
        sub    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0350:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x44]
        and    dword ptr [ebx + 0x50], 0xffffffef
        cmp    dword ptr [ebx + 0x4c], 1
        jne    loc_1_03C5
        mov    ebp, dword ptr [esp + 0x48]
        mov    word ptr [esp + 0x28], 0x12
        mov    ax, word ptr [ebp + 2]
        lea    esi, [ebp + 8]
        mov    word ptr [esp + 0x2a], ax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x2c], eax
        lea    edi, [esp + 0x30]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    eax, [esp + 0x28]
        push   0x18
        push   eax
        push   ebx
        call   sub_1_0180
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_05BE
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_03BF
        mov    dword ptr [ebx + 0x54], 0
        push   0
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0xc
        jmp    loc_1_03C9
loc_1_03BF:
        and    dword ptr [ebx + 0x54], 0xffffffef
        jmp    loc_1_03C9
loc_1_03C5:
        mov    ebp, dword ptr [esp + 0x48]
loc_1_03C9:
        cmp    dword ptr [esp + 0x4c], 0x18
        jne    loc_1_05B9
        push   dword ptr [esp + 0x4c]
        push   ebp
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    loc_1_03EA
        jmp    loc_1_05B9
loc_1_03EA:
        lea    edi, [esp + 0x10]
        mov    esi, eax
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x4c]
        push   eax
        call   sub_1_F2B5
        add    esp, 8
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_053C
        mov    eax, dword ptr [ebx + 0x88]
        cmp    byte ptr [esp + 0x18], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x19], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1a], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1b], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1c], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1d], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1e], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x1f], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x20], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x21], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x22], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x23], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x24], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x25], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x26], 0
        jne    loc_1_04AA
        cmp    byte ptr [esp + 0x27], 0
        jne    loc_1_04AA
        or     eax, eax
        je     loc_1_04A3
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [ebx + 0x88], 0
loc_1_04A3:
        sub    eax, eax
        jmp    loc_1_05BE
loc_1_04AA:
        mov    ecx, dword ptr [ebx + 0x84]
        mov    dx, word ptr [esp + 0x10]
        cmp    word ptr [ecx + 0x1c], dx
        jne    loc_1_0532
        or     eax, eax
        jne    loc_1_04FD
        push   0
        call   sub_1_05D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_04D7
        mov    eax, 0x2747
        jmp    loc_1_05BE
loc_1_04D7:
        mov    dword ptr [ebx + 0x88], eax
        mov    dword ptr [eax + 0x10], ebx
        mov    ecx, dword ptr [ebx + 0x84]
        mov    dword ptr [eax + 0x14], ecx
        lea    ecx, [eax + 0x34]
        mov    dword ptr [eax + 0x18], 0x60000004
        mov    dword ptr [ebx + 0x18], ecx
        mov    dword ptr [ebx + 0x24], 0x18
loc_1_04FD:
        lea    esi, [esp + 0x18]
        lea    edi, [eax + 0x3c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [ebx + 0x4c], 6
        and    dword ptr [ebx + 0x54], 0xfffffffd
        and    dword ptr [ebx + 0x50], 0xfffffffd
        push   0
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_1_05BE
loc_1_0532:
        mov    eax, 0x2726
        jmp    loc_1_05BE
loc_1_053C:
        cmp    word ptr [esp + 0x10], 0x12
        jne    loc_1_05B2
        push   ebx
        call   sub_1_29BC
        add    esp, 4
        call   sub_1_2DE0
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0568
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_05BE
loc_1_0568:
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        push   ebx
        call   sub_1_29E2
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        je     loc_1_05A2
        cmp    esi, 0xff
        je     loc_1_05A2
        push   edi
        call   sub_1_2E44
        add    esp, 4
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        push   esi
        call   sub_1_2C80
        add    esp, 4
        jmp    loc_1_05BE
loc_1_05A2:
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 0x2733
        jmp    loc_1_05BE
loc_1_05B2:
        mov    eax, 0x2726
        jmp    loc_1_05BE
loc_1_05B9:
        mov    eax, 0x271e
loc_1_05BE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_05D0:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_05EE
        push   0
        call   sub_1_1760
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_05F0
        sub    eax, eax
        jmp    loc_1_0651
loc_1_05EE:
        sub    ebx, ebx
loc_1_05F0:
        call   sub_1_F3A4
        push   0x5c
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0616
        or     ebx, ebx
        je     loc_1_0612
        push   ebx
        call   sub_1_17C1
        add    esp, 4
loc_1_0612:
        sub    eax, eax
        jmp    loc_1_0651
loc_1_0616:
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x17
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x18], 1
        mov    dword ptr [esi + 0x58], ebx
        mov    dword ptr [esi], 0x1383c      ; -> loc_1_1383C
        mov    eax, dword ptr [0x13840]      ; -> loc_1_13840
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x13840]      ; -> loc_1_13840
        push   esi
        mov    dword ptr [eax], esi
        mov    dword ptr [0x13840], esi      ; -> loc_1_13840
        call   sub_1_06C9
        add    esp, 4
        mov    eax, esi
loc_1_0651:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0655:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x58]
        cmp    eax, edi
        je     loc_1_0670
        push   eax
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [esi + 0x58], edi
loc_1_0670:
        cmp    dword ptr [esi + 0x4c], 0
        je     loc_1_06BD
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], 7
        test   eax, 0x40000000
        je     loc_1_06AD
        push   0
        push   esi
        call   sub_1_08BC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_06AD
        cmp    edi, 0xff
        je     loc_1_06C6
        push   1
        push   esi
        call   sub_1_08BC
        add    esp, 8
        mov    edi, eax
loc_1_06AD:
        cmp    edi, 0xff
        je     loc_1_06C6
        push   esi
        call   sub_1_093C
        jmp    loc_1_06C3
loc_1_06BD:
        push   esi
        call   sub_1_0986
loc_1_06C3:
        add    esp, 4
loc_1_06C6:
        pop    edi
        pop    esi
        ret    
sub_1_06C9:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret    
sub_1_06D1:
        mov    edx, dword ptr [esp + 4]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 8]
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        jne    loc_1_06EE
        push   edx
        call   sub_1_0655
        add    esp, 4
        mov    eax, 1
loc_1_06EE:
        ret    
sub_1_06EF:
        sub    esp, 0x10
        push   esi
        push   edi
        lea    edi, [esp + 8]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    esi, dword ptr [esp + 0x1c]
        lea    eax, [esp + 8]
        push   esi
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 8]
        add    esp, 8
        or     eax, eax
        jne    loc_1_0747
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x4c], eax
        mov    edi, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x14], edi
        push   edi
        call   sub_1_0B31
        add    esp, 4
        lea    eax, [esp + 8]
        push   dword ptr [edi + 0x10]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x10]
        add    esp, 8
        or     eax, eax
        jne    loc_1_0747
        sub    eax, eax
loc_1_0747:
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_074D:
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x2c], 0x4655424e
        lea    eax, [esp + 0x30]
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x28], 0x26
        mov    dword ptr [esp + 0x20], eax
        mov    dword ptr [esp + 0x24], 0
        lea    edi, [esp + 0xc]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    ebx, dword ptr [esp + 0x5c]
        push   1
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x18]
        push   3
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x3c]
        add    esp, 0x14
        mov    word ptr [ebx + 0x1c], 0x12
        lea    esi, [esp + 0x3c]
        lea    edi, [ebx + 0x24]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [ebx + 0x20], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
sub_1_07CD:
        sub    esp, 0x28
        push   edi
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 0x18], 0x400
        lea    edi, [esp + 8]
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0x200
        mov    dword ptr [esp + 0x24], 0x300
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x28], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x30]
        push   4
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [esp + 0xc], eax
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        pop    edi
        add    esp, 0x28
        ret    
sub_1_0837:
        sub    esp, 0x24
        push   edi
        mov    dword ptr [esp + 0x14], 0x400
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0x200
        lea    edi, [esp + 4]
        mov    dword ptr [esp + 0x20], 0x300
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x24], eax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x38]
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0x38]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        pop    edi
        add    esp, 0x24
        ret    
sub_1_089A:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [eax + 0x18], 6
        test   ecx, 0x40000000
        je     loc_1_08BB
        push   1
        push   eax
        call   sub_1_08BC
        add    esp, 8
loc_1_08BB:
        ret    
sub_1_08BC:
        sub    esp, 0x14
        mov    edx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [edx + 0x10]
        or     ecx, ecx
        je     loc_1_0902
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_0902
        cmp    word ptr [ecx + 0x10], 0
        je     loc_1_08F4
        movzx  eax, word ptr [ecx + 0x12]
        imul   eax, eax, 0x3e8
        mov    dword ptr [esp], eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    ax, 4
        jmp    loc_1_090E
loc_1_08F4:
        mov    dword ptr [esp], 0
        mov    ax, 4
        jmp    loc_1_090E
loc_1_0902:
        mov    dword ptr [esp], 0
        mov    ax, 2
loc_1_090E:
        mov    ecx, dword ptr [edx + 0x4c]
        mov    dword ptr [esp + 4], ecx
        mov    dword ptr [esp + 8], 0x99e    ; -> loc_1_099E
        mov    dword ptr [esp + 0xc], edx
        push   0
        push   0
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x1c]
        add    esp, 0x28
        ret    
sub_1_093C:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x4c]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 0xc], 0x9ba  ; -> sub_1_09BA
        mov    dword ptr [esp + 0x10], esi
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0xc]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0xff
        je     loc_1_097E
        push   0
        push   edi
        push   esi
        call   sub_1_09BA
        add    esp, 0xc
loc_1_097E:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_0986:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        call   sub_1_F396
        add    esp, 4
        ret    
loc_1_099E:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xc]
        or     ecx, 2
        mov    dword ptr [eax + 0xc], ecx
        test   cl, 1
        je     loc_1_09B9
        push   eax
        call   sub_1_093C
        add    esp, 4
loc_1_09B9:
        ret    
sub_1_09BA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x4c], 0
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_09D6
        push   eax
        call   sub_1_0B39
        add    esp, 4
loc_1_09D6:
        push   esi
        call   sub_1_0986
        add    esp, 4
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   esi
        push   edi
        call   sub_1_F3A4
        push   0x58
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     loc_1_0A54
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x16
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x14], 1
        lea    eax, [esi + 0x44]
        mov    dword ptr [esi + 0x48], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0x4c]
        mov    dword ptr [esi + 0x50], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi], 0x13678      ; -> loc_1_13678
        mov    eax, dword ptr [0x1367c]      ; -> loc_1_1367C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x1367c]      ; -> loc_1_1367C
        push   esi
        mov    dword ptr [eax], esi
        mov    dword ptr [0x1367c], esi      ; -> loc_1_1367C
        call   sub_1_0B0B
        add    esp, 4
        mov    eax, esi
loc_1_0A54:
        pop    edi
        pop    esi
        ret    
sub_1_0A57:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        inc    dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0x14], 4
        mov    eax, dword ptr [esi + 0x44]
        sub    eax, esi
        cmp    eax, 0x44
        je     loc_1_0A90
loc_1_0A74:
        push   esi
        call   sub_1_0F2D
        add    esp, 4
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    eax, dword ptr [esi + 0x44]
        sub    eax, esi
        cmp    eax, 0x44
        jne    loc_1_0A74
loc_1_0A90:
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, esi
        cmp    eax, 0x4c
        je     loc_1_0AB6
loc_1_0A9A:
        push   esi
        call   sub_1_0F09
        add    esp, 4
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, esi
        cmp    eax, 0x4c
        jne    loc_1_0A9A
loc_1_0AB6:
        mov    eax, dword ptr [esi + 0x54]
        or     eax, eax
        je     loc_1_0ACD
        push   eax
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [esi + 0x54], 0
loc_1_0ACD:
        dec    dword ptr [esi + 8]
        cmp    dword ptr [esi + 0x34], 0
        jne    loc_1_0B08
        cmp    edi, 0x40000003
        jne    loc_1_0AF2
        push   0
        push   0
        push   0
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   dword ptr [esi + 0x10]
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_0AF2:
        cmp    dword ptr [esi + 0x10], 0
        push   esi
        je     loc_1_0B00
        call   sub_1_0F54
        jmp    loc_1_0B05
loc_1_0B00:
        call   sub_1_0F9E
loc_1_0B05:
        add    esp, 4
loc_1_0B08:
        pop    edi
        pop    esi
        ret    
sub_1_0B0B:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret    
sub_1_0B13:
        mov    edx, dword ptr [esp + 4]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 8]
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        jne    loc_1_0B30
        push   edx
        call   sub_1_0A57
        add    esp, 4
        mov    eax, 1
loc_1_0B30:
        ret    
sub_1_0B31:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x34]
        ret    
sub_1_0B39:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x34]
        dec    eax
        mov    dword ptr [ecx + 0x34], eax
        jne    loc_1_0B55
        cmp    dword ptr [ecx + 8], 0
        jne    loc_1_0B55
        push   ecx
        call   sub_1_0A57
        add    esp, 4
loc_1_0B55:
        ret    
sub_1_0B56:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [0x13678]      ; -> loc_1_13678
        cmp    edx, 0x13678                  ; -> loc_1_13678
        je     loc_1_0C37
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, 4
loc_1_0B78:
        cmp    dword ptr [edx + 0x14], esi
        je     loc_1_0C29
        mov    edi, dword ptr [edx + 0x18]
        cmp    dword ptr [edi + 0x2c], ecx
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 8]
        cmp    byte ptr [edx + 0x24], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 9]
        cmp    byte ptr [edx + 0x25], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xa]
        cmp    byte ptr [edx + 0x26], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xb]
        cmp    byte ptr [edx + 0x27], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xc]
        cmp    byte ptr [edx + 0x28], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xd]
        cmp    byte ptr [edx + 0x29], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xe]
        cmp    byte ptr [edx + 0x2a], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0xf]
        cmp    byte ptr [edx + 0x2b], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0x10]
        cmp    byte ptr [edx + 0x2c], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x2d]
        cmp    byte ptr [eax + 0x11], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [eax + 0x12]
        cmp    byte ptr [edx + 0x2e], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x2f]
        cmp    byte ptr [eax + 0x13], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x30]
        cmp    byte ptr [eax + 0x14], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x31]
        cmp    byte ptr [eax + 0x15], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x32]
        cmp    byte ptr [eax + 0x16], bl
        jne    loc_1_0C29
        mov    bl, byte ptr [edx + 0x33]
        cmp    byte ptr [eax + 0x17], bl
        jne    loc_1_0C29
        mov    ebx, dword ptr [eax + 4]
        mov    edi, dword ptr [edx + 0x20]
        cmp    ebx, edi
        je     loc_1_0C3D
        cmp    edi, -1
        je     loc_1_0C3D
        cmp    ebx, -1
        je     loc_1_0C3D
loc_1_0C29:
        mov    edx, dword ptr [edx]
        cmp    edx, 0x13678                  ; -> loc_1_13678
        jne    loc_1_0B78
loc_1_0C37:
        sub    eax, eax
loc_1_0C39:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0C3D:
        mov    eax, 1
        jmp    loc_1_0C39
sub_1_0C44:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x12], 1
        mov    byte ptr [esp + 0x13], 0
        mov    dword ptr [esp + 0x24], 1
        lea    edi, [esp + 0x2c]
        mov    word ptr [esp + 0x28], 0x1a
        mov    word ptr [esp + 0x2a], 0x15
        mov    eax, dword ptr [esp + 0x64]
        mov    dword ptr [esp + 0x3c], eax
        mov    eax, dword ptr [esp + 0x68]
        mov    dword ptr [esp + 0x40], eax
        sub    eax, eax
        mov    esi, dword ptr [esp + 0x60]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    edi, [esp + 0x14]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        lea    eax, [esp + 0x12]
        mov    esi, dword ptr [esp + 0x5c]
        push   eax
        lea    eax, [esp + 0x28]
        push   esi
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0D64
        mov    edi, dword ptr [esp + 0x4c]
        mov    ebx, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x14], 0x40000002
        mov    dword ptr [edi + 0x10], ebx
        mov    ebp, dword ptr [esp + 0x54]
        mov    word ptr [edi + 0x1c], bp
        push   edi
        call   sub_1_0D6C
        mov    ecx, dword ptr [esp + 0x54]
        add    esp, 4
        mov    dword ptr [ecx + 0x84], edi
        lea    eax, [edi + 0x1c]
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x20], 0x18
        mov    dword ptr [ecx + 0x28], ebp
        mov    eax, dword ptr [esp + 0x58]
        mov    dword ptr [ecx + 0x2c], eax
        mov    dword ptr [ecx + 0x30], esi
        push   edi
        push   0x1095                        ; -> loc_1_1095
        push   1
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   ebx
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   edi
        push   0x1111                        ; -> loc_1_1111
        push   2
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   ebx
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   edi
        push   0x1114                        ; -> loc_1_1114
        push   3
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   ebx
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   edi
        push   0x116b                        ; -> loc_1_116B
        push   5
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   ebx
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   edi
        push   0x11c2                        ; -> loc_1_11C2
        push   4
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   ebx
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        sub    eax, eax
loc_1_0D64:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
sub_1_0D6C:
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x2c], 0x4655424e
        lea    eax, [esp + 0x30]
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x28], 0x26
        mov    dword ptr [esp + 0x20], eax
        mov    dword ptr [esp + 0x24], 0
        lea    edi, [esp + 0xc]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    ebx, dword ptr [esp + 0x5c]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x18]
        push   3
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x3c]
        add    esp, 0x14
        mov    word ptr [ebx + 0x1c], 0x12
        mov    eax, dword ptr [esp + 0x50]
        mov    word ptr [ebx + 0x1e], ax
        mov    eax, dword ptr [esp + 0x4c]
        lea    esi, [esp + 0x3c]
        mov    dword ptr [ebx + 0x20], eax
        lea    edi, [ebx + 0x24]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [ebx + 0x20], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
sub_1_0DFB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        call   sub_1_05D0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0E16
        mov    eax, 1
        jmp    loc_1_0E77
loc_1_0E16:
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        cmp    dword ptr [esi + 0x34], 1
        sbb    ebp, ebp
        neg    ebp
        push   edi
        call   sub_1_06EF
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_0E3F
        push   edi
        call   sub_1_0655
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_0E77
loc_1_0E3F:
        mov    ebx, dword ptr [esi + 0x18]
        mov    dword ptr [edi + 0x10], ebx
        push   edi
        push   esi
        call   sub_1_0EE8
        add    esp, 8
        push   ebx
        push   dword ptr [edi + 0x58]
        call   sub_1_17E3
        add    esp, 8
        or     ebp, ebp
        je     loc_1_0E75
        push   esi
        push   0xfd0                         ; -> loc_1_0FD0
        push   0
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        push   dword ptr [esi + 0x10]
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_0E75:
        sub    eax, eax
loc_1_0E77:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0E7C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 0x38]
        mov    ebx, dword ptr [esi + 0x3c]
        mov    ebp, dword ptr [esi + 0x40]
        cmp    ecx, eax
        je     loc_1_0EC3
        cmp    ebp, ebx
        jbe    loc_1_0EC3
        mov    edi, ebp
        sub    edi, ebx
        cmp    edi, ecx
        jb     loc_1_0EA1
        mov    edi, ecx
loc_1_0EA1:
        or     edi, edi
        je     loc_1_0EB8
loc_1_0EA5:
        push   esi
        call   sub_1_0DFB
        add    esp, 4
        or     eax, eax
        jne    loc_1_0EB8
        dec    edi
        inc    ebx
        or     edi, edi
        jne    loc_1_0EA5
loc_1_0EB8:
        mov    dword ptr [esi + 0x38], edi
        mov    dword ptr [esi + 0x3c], ebx
        mov    dword ptr [esi + 0x40], ebp
        jmp    loc_1_0EC5
loc_1_0EC3:
        sub    eax, eax
loc_1_0EC5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0ECA:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 0x50]
        lea    eax, [ecx + 0x4c]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x50]
        mov    dword ptr [edx + 0x54], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 0x50], esi
        pop    esi
        ret    
sub_1_0EE8:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 0x50]
        lea    eax, [ecx + 0x44]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 0x48]
        mov    dword ptr [edx + 0x54], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 0x48], esi
        inc    dword ptr [ecx + 0x3c]
        pop    esi
        ret    
sub_1_0F09:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x4c]
        lea    esi, [ecx + 0x4c]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 0x4c
        jne    loc_1_0F21
        sub    eax, eax
        jmp    loc_1_0F2B
loc_1_0F21:
        mov    ecx, dword ptr [eax]
        sub    eax, 0x50
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
loc_1_0F2B:
        pop    esi
        ret    
sub_1_0F2D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x44]
        lea    edx, [esi + 0x44]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 0x44
        jne    loc_1_0F45
        sub    eax, eax
        jmp    loc_1_0F52
loc_1_0F45:
        mov    ecx, dword ptr [eax]
        sub    eax, 0x50
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 0x3c]
loc_1_0F52:
        pop    esi
        ret    
sub_1_0F54:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 0xc], 0xfb6  ; -> sub_1_0FB6
        mov    dword ptr [esp + 0x10], esi
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0xff
        je     loc_1_0F96
        push   0
        push   edi
        push   esi
        call   sub_1_0FB6
        add    esp, 0xc
loc_1_0F96:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
sub_1_0F9E:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        call   sub_1_F396
        add    esp, 4
        ret    
sub_1_0FB6:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 0x10], 0
        call   sub_1_0F9E
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h
loc_1_0FD0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [esp + 0x10], eax
        cmp    dword ptr [ebx + 0x14], 4
        je     loc_1_1088
        mov    eax, dword ptr [ebx + 0x44]
        sub    eax, ebx
        cmp    eax, 0x44
        je     loc_1_1088
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_1088
        cmp    dword ptr [esp + 0x1c], 0x22
        jb     loc_1_1088
        mov    eax, dword ptr [esp + 0x20]
        cmp    word ptr [eax + 6], 0x15
        jne    loc_1_1088
        push   ebx
        call   sub_1_0F2D
        mov    ebp, eax
        add    esp, 4
        mov    dword ptr [ebp + 0x18], 0x60000002
        mov    word ptr [ebp + 0x34], 0x12
        mov    esi, dword ptr [esp + 0x20]
        add    esi, 8
        lea    edi, [ebp + 0x3c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x20]
        push   ebp
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp + 0x38], eax
        mov    eax, dword ptr [esp + 0x14]
        inc    dword ptr [eax + 0x48]
        call   sub_1_06C9
        add    esp, 4
        push   ebx
        call   sub_1_0B0B
        mov    eax, dword ptr [esp + 0x3c]
        add    esp, 4
        mov    dword ptr [eax], 0x12eb       ; -> loc_1_12EB
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [eax], ebp
        mov    eax, 0x18
        jmp    loc_1_108D
loc_1_1088:
        mov    eax, 0xc00000d0
loc_1_108D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_1095:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 0xc]
        or     eax, 1
        mov    dword ptr [edx + 0xc], eax
        test   al, 2
        je     loc_1_10B2
        push   edx
        call   sub_1_0655
        add    esp, 4
        jmp    loc_1_110D
loc_1_10B2:
        mov    ecx, dword ptr [edx + 0x18]
        cmp    ecx, 7
        je     loc_1_110D
        mov    eax, dword ptr [esp + 0x20]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x3fffffff
        add    eax, 6
        mov    dword ptr [edx + 0x18], eax
        cmp    ecx, 0x60000002
        je     loc_1_110D
        mov    esi, dword ptr [edx + 0x10]
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_10F2
        mov    eax, dword ptr [esi + 0x8c]
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        jne    loc_1_110D
loc_1_10F2:
        push   0
        push   0x20
        push   esi
        call   sub_1_F3AC
        add    esp, 0xc
        push   0x2746
        push   esi
        call   sub_1_F3B2
        add    esp, 8
loc_1_110D:
        sub    eax, eax
        pop    esi
        ret    
loc_1_1111:
        sub    eax, eax
        ret    
loc_1_1114:
        push   esi
        mov    esi, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esp + 0xc]
        mov    dword ptr [esi], 0
        test   byte ptr [edx + 0x1b], 0x20
        je     loc_1_1158
        mov    eax, dword ptr [edx + 0x10]
        test   byte ptr [eax + 0x40], 0x80
        jne    loc_1_1158
        push   0x12a00                       ; -> loc_1_12A00
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x24]
        push   dword ptr [edx + 0x58]
        call   sub_1_135D
        add    esp, 0x14
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0x18
        jmp    loc_1_1169
loc_1_1158:
        push   edx
        call   sub_1_089A
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esi], ecx
loc_1_1169:
        pop    esi
        ret    
loc_1_116B:
        push   esi
        mov    esi, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esp + 0xc]
        mov    dword ptr [esi], 0
        test   byte ptr [edx + 0x1b], 0x20
        je     loc_1_11AF
        mov    eax, dword ptr [edx + 0x10]
        test   byte ptr [eax + 0x40], 0x80
        jne    loc_1_11AF
        push   0x12a04                       ; -> loc_1_12A04
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x24]
        push   dword ptr [edx + 0x58]
        call   sub_1_135D
        add    esp, 0x14
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0x18
        jmp    loc_1_11C0
loc_1_11AF:
        push   edx
        call   sub_1_089A
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esi], ecx
loc_1_11C0:
        pop    esi
        ret    
loc_1_11C2:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 0x18]
        or     eax, eax
        je     loc_1_1292
        mov    ecx, dword ptr [eax + 0x88]
        or     ecx, ecx
        je     loc_1_1292
        mov    al, byte ptr [ecx + 0x3c]
        add    ecx, 0x34
        mov    esi, dword ptr [esp + 0x14]
        cmp    byte ptr [esi + 8], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 9]
        cmp    byte ptr [esi + 9], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xa]
        cmp    byte ptr [esi + 0xa], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xb]
        cmp    byte ptr [esi + 0xb], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xc]
        cmp    byte ptr [esi + 0xc], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xd]
        cmp    byte ptr [esi + 0xd], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xe]
        cmp    byte ptr [esi + 0xe], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0xf]
        cmp    byte ptr [esi + 0xf], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x10]
        cmp    byte ptr [esi + 0x10], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x11]
        cmp    byte ptr [esi + 0x11], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x12]
        cmp    byte ptr [esi + 0x12], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x13]
        cmp    byte ptr [esi + 0x13], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x14]
        cmp    byte ptr [esi + 0x14], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x15]
        cmp    byte ptr [esi + 0x15], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x16]
        cmp    byte ptr [esi + 0x16], al
        jne    loc_1_1284
        mov    al, byte ptr [ecx + 0x17]
        cmp    byte ptr [esi + 0x17], al
        jne    loc_1_1284
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [esi + 0x18]
        cmp    eax, ecx
        je     loc_1_1296
        cmp    eax, -1
        je     loc_1_1296
        cmp    ecx, -1
        je     loc_1_1296
loc_1_1284:
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    loc_1_12E8
loc_1_1292:
        mov    esi, dword ptr [esp + 0x14]
loc_1_1296:
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [ecx], 0
        test   byte ptr [edx + 0x17], 0x40
        je     loc_1_12E3
        push   0x12a00                       ; -> loc_1_12A00
        push   0
        push   0
        push   dword ptr [esp + 0x34]
        push   dword ptr [edx + 0x54]
        call   sub_1_135D
        add    esp, 0x14
        or     eax, eax
        je     loc_1_12E3
        add    esi, 8
        lea    edi, [eax + 0x40]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    ecx, dword ptr [esp + 0x34]
        add    eax, 0x8a
        mov    dword ptr [ecx], eax
        mov    eax, 0x18
        jmp    loc_1_12E8
loc_1_12E3:
        mov    eax, 0x17
loc_1_12E8:
        pop    edi
        pop    esi
        ret    
loc_1_12EB:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x14], 0
        mov    edi, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esi + 0x10]
        jne    loc_1_1318
        push   esi
        push   edi
        call   sub_1_0ECA
        add    esp, 8
        push   0
        push   8
        push   ebx
        call   sub_1_F3AC
        add    esp, 0xc
        jmp    loc_1_1347
loc_1_1318:
        cmp    dword ptr [esi + 0x18], 0x60000002
        jne    loc_1_1347
        mov    ecx, dword ptr [esi + 0x54]
        lea    eax, [esi + 0x50]
        mov    dword ptr [esi + 0x18], 1
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        push   esi
        push   dword ptr [ebx + 0x84]
        call   sub_1_0EE8
        add    esp, 8
        dec    dword ptr [ebx + 0x48]
loc_1_1347:
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_06D1
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_135D:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_1_138D
        cmp    dword ptr [esi + 0x1c], 0
        ja     loc_1_138D
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [esi + 0x18], eax
        ja     loc_1_138D
        cmp    dword ptr [esi + 0x14], 0x10
        ja     loc_1_138D
        mov    ebx, dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_12F70
        add    esp, 4
        mov    edi, eax
        jmp    loc_1_1393
loc_1_138D:
        sub    edi, edi
        mov    ebx, dword ptr [esp + 0x14]
loc_1_1393:
        or     edi, edi
        je     loc_1_13E1
        mov    dword ptr [edi + 0x18], esi
        push   dword ptr [esp + 0x18]
        call   sub_1_2DBB
        add    esp, 4
        or     dword ptr [edi + 0x1c], eax
        push   dword ptr [esi]
        call   sub_1_29BC
        mov    ecx, dword ptr [esp + 0x20]
        add    esp, 4
        or     ecx, ecx
        jne    loc_1_13C1
        lea    ecx, [edi + 0x8a]
loc_1_13C1:
        mov    eax, dword ptr [edi + 0x28]
        add    eax, 0xc
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], 0x13f4   ; -> loc_1_13F4
        mov    dword ptr [ecx + 0xc], edi
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, edi
        jmp    loc_1_13F0
loc_1_13E1:
        add    dword ptr [esi + 0x1c], ebx
        mov    eax, dword ptr [esp + 0x18]
        and    eax, 0x60
        mov    dword ptr [esi + 0x20], eax
        sub    eax, eax
loc_1_13F0:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_13F4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x1c]
        and    eax, 1
        mov    edi, dword ptr [esi]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xfffffffd
        add    ebx, 4
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_143F
        mov    dword ptr [ebp + 0x18], edi
        push   dword ptr [esp + 0x1c]
        push   ebp
        call   sub_1_1324F
        add    esp, 8
        push   ebp
        push   esi
        call   sub_1_1833
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 8
        add    dword ptr [esi + 0x18], eax
        inc    dword ptr [esi + 0x14]
        jmp    loc_1_144C
loc_1_143F:
        or     dword ptr [edi + 0x3c], 1
        push   ebp
        call   sub_1_1301F
        add    esp, 4
loc_1_144C:
        push   edi
        call   sub_1_29C4
        add    esp, 4
        or     eax, eax
        jne    loc_1_1470
        push   dword ptr [esp + 0x18]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   ebx
        push   edi
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_1470:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1480:
        sub    esp, 8
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 0x4004667f
        je     loc_1_149E
        cmp    eax, 0x40047307
        je     loc_1_14B5
        jmp    loc_1_1565
loc_1_149E:
        push   dword ptr [esp + 0x14]
        call   sub_1_1572
        mov    ecx, dword ptr [esp + 0x20]
        add    esp, 4
        mov    dword ptr [ecx], eax
        jmp    loc_1_156A
loc_1_14B5:
        mov    edi, dword ptr [esp + 0x14]
        cmp    dword ptr [edi + 0x2c], 1
        jne    loc_1_1565
        test   byte ptr [edi + 0x40], 0x10
        je     loc_1_1565
        mov    eax, dword ptr [edi + 0x88]
        or     eax, eax
        jne    loc_1_14E6
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_156A
loc_1_14E6:
        lea    ecx, [esp + 8]
        push   8
        push   ecx
        push   5
        push   eax
        call   sub_1_0837
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1509
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    esi, eax
        jmp    loc_1_156A
loc_1_1509:
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_1523
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_1528
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_156A
loc_1_1523:
        cmp    dword ptr [esp + 0xc], 0
loc_1_1528:
        jle    loc_1_1536
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 0
        jmp    loc_1_156A
loc_1_1536:
        mov    eax, dword ptr [edi + 0x8c]
        or     eax, eax
        jne    loc_1_154C
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], 1
        jmp    loc_1_156A
loc_1_154C:
        mov    ecx, dword ptr [esp + 0xc]
        neg    ecx
        cmp    dword ptr [eax + 0x18], ecx
        mov    eax, 0
        setbe  al
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], eax
        jmp    loc_1_156A
loc_1_1565:
        mov    esi, 0x2726
loc_1_156A:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 8
        ret    
sub_1_1572:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        sub    eax, eax
        mov    edi, dword ptr [esi + 0x8c]
        cmp    edi, eax
        jne    loc_1_1586
        jmp    loc_1_15C9
loc_1_1586:
        cmp    dword ptr [esi + 0x2c], 1
        jne    loc_1_15B1
        mov    eax, dword ptr [edi + 0x18]
        test   byte ptr [esi + 0x40], 0x10
        jne    loc_1_15C0
        mov    edx, dword ptr [edi + 0x3c]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -0x3c
        je     loc_1_15C0
loc_1_15A1:
        sub    eax, dword ptr [edx + 0x18]
        mov    edx, dword ptr [edx]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -0x3c
        jne    loc_1_15A1
        jmp    loc_1_15C0
loc_1_15B1:
        mov    ecx, dword ptr [edi + 0x2c]
        mov    edx, ecx
        sub    edx, edi
        cmp    edx, 0x2c
        je     loc_1_15C0
        mov    eax, dword ptr [ecx + 0x20]
loc_1_15C0:
        mov    ecx, dword ptr [esi + 0x38]
        cmp    eax, ecx
        jbe    loc_1_15C9
        mov    eax, ecx
loc_1_15C9:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h
loc_1_15D0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_1_29BC
        add    esp, 4
        mov    edi, dword ptr [esi + 0x84]
        mov    ebx, dword ptr [esp + 0x14]
        add    dword ptr [edi + 0x38], ebx
        mov    dword ptr [edi + 0x40], ebx
        push   edi
        call   sub_1_0E7C
        add    esp, 4
        mov    eax, dword ptr [edi + 0x44]
        sub    eax, edi
        cmp    eax, 0x44
        jne    loc_1_1613
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_1639
loc_1_1613:
        mov    dword ptr [edi + 0x14], 0x40000003
        mov    dword ptr [esi + 0x4c], 3
        movzx  eax, bl
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi + 0x54], 0x37
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    eax, eax
loc_1_1639:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
sub_1_1640:
        mov    ecx, dword ptr [esp + 0xc]
        or     ecx, ecx
        jne    loc_1_1654
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0xc000009a
        jmp    loc_1_169A
loc_1_1654:
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        jne    loc_1_1667
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0xc000009a
        jmp    loc_1_169A
loc_1_1667:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x10], 0x4655424e
        mov    dword ptr [eax], 0
loc_1_169A:
        ret    0x14
sub_1_169D:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, esi
        imul   eax, eax, 0x14
        add    eax, 0x11
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        jne    loc_1_16CD
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0xc000009a
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], 0
        jmp    loc_1_1700
loc_1_16CD:
        mov    dword ptr [eax + 0xc], 0x14
        lea    edx, [eax + 0x10]
        mov    dword ptr [eax + 8], edx
        cmp    esi, 1
        jbe    loc_1_16EA
        dec    esi
loc_1_16E0:
        lea    ecx, [edx + 0x14]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        dec    esi
        jne    loc_1_16E0
loc_1_16EA:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
loc_1_1700:
        pop    esi
        ret    0xc
sub_1_1704:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_1_171B
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     loc_1_171B
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 8], eax
loc_1_171B:
        ret    4
sub_1_171E:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    loc_1_172D
        mov    eax, 0xc000009a
        jmp    loc_1_1738
loc_1_172D:
        push   eax
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
loc_1_1738:
        ret    4
sub_1_173B:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_1_1759
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_1_1750
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
loc_1_1750:
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
loc_1_1759:
        ret    0xc
        db  000h, 000h, 000h, 000h
sub_1_1760:
        push   edi
        call   sub_1_F3A4
        push   0x44
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_177A
        sub    eax, eax
        jmp    loc_1_17BF
loc_1_177A:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x11
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edx + 0x24]
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x2c]
        mov    dword ptr [edx + 0x30], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x34]
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x3c]
        mov    dword ptr [edx + 0x40], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_1_17BD
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [edx + 0x10], eax
loc_1_17BD:
        mov    eax, edx
loc_1_17BF:
        pop    edi
        ret    
sub_1_17C1:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_18C9
        add    esp, 4
        push   esi
        call   sub_1_190E
        add    esp, 4
        push   esi
        call   sub_1_192D
        add    esp, 4
        pop    esi
        ret    
sub_1_17E3:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [eax + 0x34]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [edx + 0x10], eax
        ret    
sub_1_17FA:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        lea    edx, [esi + 8]
        lea    eax, [ecx + 0x24]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x28], edx
        mov    dword ptr [esi], 0x1364c      ; -> loc_1_1364C
        mov    eax, dword ptr [0x13650]      ; -> loc_1_13650
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x13650]      ; -> loc_1_13650
        mov    dword ptr [eax], esi
        mov    dword ptr [0x13650], esi      ; -> loc_1_13650
        pop    esi
        ret    
sub_1_1833:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [edx + 0x1c], 1
        je     loc_1_1847
        lea    esi, [ecx + 0x3c]
        jmp    loc_1_184A
loc_1_1847:
        lea    esi, [ecx + 0x34]
loc_1_184A:
        lea    eax, [ecx + 0x2c]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x30]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0x30], edx
        mov    dword ptr [edx + 8], esi
        lea    ecx, [edx + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4], ecx
        pop    esi
        ret    
sub_1_186D:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x24]
        lea    esi, [ecx + 0x24]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 0x24
        jne    loc_1_1885
        sub    eax, eax
        jmp    loc_1_1899
loc_1_1885:
        mov    ecx, dword ptr [eax]
        sub    eax, 8
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
loc_1_1899:
        pop    esi
        ret    
sub_1_189B:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x2c]
        lea    esi, [ecx + 0x2c]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 0x2c
        jne    loc_1_18B3
        sub    eax, eax
        jmp    loc_1_18C7
loc_1_18B3:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    edx, dword ptr [eax + 0xc]
        lea    ecx, [eax + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
loc_1_18C7:
        pop    esi
        ret    
sub_1_18C9:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        cmp    eax, 0x24
        je     loc_1_190A
        mov    ebx, dword ptr [edi]
loc_1_18DC:
        push   edi
        call   sub_1_186D
        add    esp, 4
        push   eax
        call   sub_1_1301F
        add    esp, 4
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        sub    eax, 0x24
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        or     esi, esi
        je     loc_1_18DC
loc_1_190A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_190E:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_1_1913:
        push   esi
        call   sub_1_189B
        add    esp, 4
        or     eax, eax
        je     loc_1_192B
        push   eax
        call   sub_1_1301F
        add    esp, 4
        jmp    loc_1_1913
loc_1_192B:
        pop    esi
        ret    
sub_1_192D:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_1940:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x1c]
        sub    esi, esi
        push   dword ptr [esp + 0x1c]
        call   sub_1_F270
        add    esp, 8
        mov    edi, eax
        cmp    edi, esi
        je     loc_1_19EF
        cmp    dword ptr [esp + 0x2c], esi
        je     loc_1_19EF
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x54], 0xfffffffe
        cmp    dword ptr [eax + 0x2c], 2
        jne    loc_1_19D2
        mov    ebx, dword ptr [esp + 0x24]
        cmp    ebx, esi
        je     loc_1_19AF
        mov    ebp, dword ptr [esp + 0x28]
        cmp    ebp, esi
        je     loc_1_19AF
        mov    esi, dword ptr [ebp]
        push   esi
        push   ebx
        call   sub_1_F270
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_19A2
        mov    ebp, 0x271e
        jmp    loc_1_19F8
loc_1_19A2:
        cmp    dword ptr [ebp], 0x18
        je     loc_1_19B3
        mov    ebp, 0x271e
        jmp    loc_1_19F8
loc_1_19AF:
        sub    ebx, ebx
        sub    ebp, ebp
loc_1_19B3:
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   ebx
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   edi
        push   dword ptr [esp + 0x2c]
        call   sub_1_1C21
        add    esp, 0x1c
        mov    ebp, eax
        jmp    loc_1_19F8
loc_1_19D2:
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   edi
        push   dword ptr [esp + 0x24]
        call   sub_1_1A1A
        add    esp, 0x14
        mov    ebp, eax
        jmp    loc_1_19F4
loc_1_19EF:
        mov    ebp, 0x271e
loc_1_19F4:
        mov    ebx, dword ptr [esp + 0x24]
loc_1_19F8:
        push   dword ptr [esp + 0x1c]
        push   edi
        call   sub_1_F2B5
        add    esp, 8
        or     esi, esi
        je     loc_1_1A13
        push   esi
        push   ebx
        call   sub_1_F2B5
        add    esp, 8
loc_1_1A13:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1A1A:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x28], 0
        mov    esi, dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x18], 0
        mov    edi, dword ptr [esi + 0x8c]
        mov    eax, dword ptr [esp + 0x3c]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x1c], eax
        push   edi
        push   esi
        call   sub_1_1D84
        add    esp, 8
        mov    ecx, dword ptr [edi + 0x2c]
        lea    edx, [edi + 0x2c]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 0x2c
        je     loc_1_1B42
        mov    eax, dword ptr [esi + 0x40]
        and    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x20], eax
        or     eax, eax
        je     loc_1_1A8D
        mov    dword ptr [esp + 0x14], edx
        mov    eax, dword ptr [ecx + 0x1c]
        and    eax, 1
        mov    dword ptr [esp + 0x24], eax
        jmp    loc_1_1AA0
loc_1_1A8D:
        test   byte ptr [esp + 0x3c], 1
        je     loc_1_1A99
        lea    eax, [edi + 0x3c]
        jmp    loc_1_1A9C
loc_1_1A99:
        lea    eax, [edi + 0x34]
loc_1_1A9C:
        mov    dword ptr [esp + 0x14], eax
loc_1_1AA0:
        mov    eax, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [eax]
        cmp    eax, ebx
        je     loc_1_1B39
loc_1_1AAE:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_1AC5
        mov    ebp, ebx
        mov    eax, dword ptr [ebx + 0x1c]
        and    eax, 1
        cmp    eax, dword ptr [esp + 0x24]
        je     loc_1_1AC8
        jmp    loc_1_1B39
loc_1_1AC5:
        lea    ebp, [ebx - 8]
loc_1_1AC8:
        push   dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [ebx]
        push   dword ptr [esp + 0x3c]
        push   ebp
        push   dword ptr [esp + 0x40]
        call   sub_1_130AB
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        mov    eax, dword ptr [ebp + 0x30]
        cmp    dword ptr [ebp + 0x28], eax
        jne    loc_1_1B1D
        mov    eax, dword ptr [ebp + 0x34]
        cmp    dword ptr [ebp + 0x2c], eax
        jne    loc_1_1B1D
        mov    eax, dword ptr [ebp + 0x20]
        sub    dword ptr [edi + 0x18], eax
        dec    dword ptr [edi + 0x14]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        add    esp, 4
loc_1_1B1D:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x18], eax
        sub    dword ptr [esp + 0x38], eax
        je     loc_1_1B39
        add    dword ptr [esp + 0x34], eax
        cmp    dword ptr [esp + 0x14], ebx
        jne    loc_1_1AAE
loc_1_1B39:
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_1B72
        jmp    loc_1_1B6A
loc_1_1B42:
        mov    eax, dword ptr [esi + 0x88]
        or     eax, eax
        je     loc_1_1B6A
        cmp    dword ptr [eax + 0x18], 0x40000005
        jne    loc_1_1B6A
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_1B72
        push   0
        push   0x20
        push   esi
        call   sub_1_F3AC
        add    esp, 0xc
        jmp    loc_1_1B72
loc_1_1B6A:
        mov    dword ptr [esp + 0x28], 0x2733
loc_1_1B72:
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_1BA4
        call   sub_1_2117
        push   edi
        push   esi
        call   sub_1_1D84
        add    esp, 8
        mov    eax, dword ptr [edi + 0x3c]
        sub    eax, edi
        cmp    eax, 0x3c
        jne    loc_1_1B96
        and    dword ptr [esi + 0x50], 0xfffffffb
loc_1_1B96:
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        cmp    eax, 0x34
        jne    loc_1_1BA4
        and    dword ptr [esi + 0x50], 0xfffffffe
loc_1_1BA4:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_1C0B
        mov    ecx, dword ptr [esp + 0x3c]
        and    ecx, 1
        je     loc_1_1BBD
        mov    eax, dword ptr [edi + 0x3c]
        sub    eax, edi
        sub    eax, 0x3c
        jmp    loc_1_1BC5
loc_1_1BBD:
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        sub    eax, 0x34
loc_1_1BC5:
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    loc_1_1BED
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_1C0B
        mov    eax, dword ptr [esi + 0x88]
        cmp    dword ptr [eax + 0x18], 0x40000005
        jne    loc_1_1C0B
loc_1_1BED:
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0xfffffffd
        add    eax, 4
        mov    ecx, eax
        not    ecx
        and    dword ptr [esi + 0x50], ecx
        push   0
        push   eax
        push   esi
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_1C0B:
        mov    eax, dword ptr [esp + 0x40]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x28]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_1C21:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], 0
        mov    ebx, dword ptr [eax + 0x8c]
        push   ebx
        push   dword ptr [esp + 0x1c]
        call   sub_1_1D84
        add    esp, 8
        mov    ebp, dword ptr [ebx + 0x2c]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 0x2c
        je     loc_1_1D19
        mov    ecx, dword ptr [esp + 0x28]
        or     ecx, ecx
        je     loc_1_1C83
        mov    eax, dword ptr [esp + 0x18]
        lea    esi, [ebp + 0x40]
        mov    eax, dword ptr [eax + 0x28]
        mov    word ptr [ecx], ax
        lea    edi, [ecx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax], 0x18
loc_1_1C83:
        mov    esi, dword ptr [esp + 0x24]
        mov    eax, esi
        and    eax, 2
        push   eax
        push   dword ptr [esp + 0x24]
        push   ebp
        push   dword ptr [esp + 0x28]
        call   sub_1_130AB
        mov    ecx, dword ptr [esp + 0x40]
        add    esp, 0x10
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x30]
        cmp    dword ptr [ebp + 0x28], eax
        jne    loc_1_1CE0
        mov    eax, dword ptr [ebp + 0x34]
        cmp    dword ptr [ebp + 0x2c], eax
        jne    loc_1_1CE0
        mov    eax, dword ptr [ebp + 0x20]
        sub    dword ptr [ebx + 0x18], eax
        dec    dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        add    esp, 4
        jmp    loc_1_1D12
loc_1_1CE0:
        mov    eax, dword ptr [ebp + 0x20]
        sub    dword ptr [ebx + 0x18], eax
        dec    dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        mov    dword ptr [esp + 0x14], 0x2738
        add    esp, 4
loc_1_1D12:
        call   sub_1_2117
        jmp    loc_1_1D25
loc_1_1D19:
        mov    dword ptr [esp + 0x10], 0x2733
        mov    esi, dword ptr [esp + 0x24]
loc_1_1D25:
        test   esi, 2
        jne    loc_1_1D4C
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, ebx
        cmp    eax, 0x2c
        jne    loc_1_1D3F
        mov    eax, dword ptr [esp + 0x18]
        and    dword ptr [eax + 0x50], 0xfffffffe
loc_1_1D3F:
        push   ebx
        push   dword ptr [esp + 0x1c]
        call   sub_1_1D84
        add    esp, 8
loc_1_1D4C:
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_1D78
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, ebx
        cmp    eax, 0x2c
        je     loc_1_1D78
        mov    eax, dword ptr [esp + 0x18]
        push   0
        push   1
        and    dword ptr [eax + 0x50], 0xfffffffe
        push   dword ptr [esp + 0x20]
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_1D78:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_1D84:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x28]
        mov    eax, dword ptr [edi + 0x84]
        test   byte ptr [eax + 0x17], 0x40
        je     loc_1_1E9C
        mov    ebp, dword ptr [esp + 0x2c]
        mov    ebx, dword ptr [ebp + 0x1c]
        or     ebx, ebx
        je     loc_1_1E9C
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp + 0x18], eax
        ja     loc_1_1E9C
        cmp    dword ptr [ebp + 0x14], 0x10
        ja     loc_1_1E9C
        push   ebx
        call   sub_1_12F70
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_1E9C
        mov    dword ptr [esi + 0x18], edi
        push   dword ptr [ebp + 0x20]
        add    ebp, 0x20
        call   sub_1_2DBB
        add    esp, 4
        or     dword ptr [esi + 0x1c], eax
        mov    dword ptr [esp + 0x18], 0x1ea4 ; -> sub_1_1EA4
        mov    dword ptr [esp + 0x1c], esi
        push   edi
        call   sub_1_29BC
        add    esp, 4
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_1E47
        mov    eax, dword ptr [edi + 0x84]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [esi + 0x38]
        mov    dword ptr [esi + 0x6e], eax
        mov    eax, dword ptr [esi + 0x28]
        add    eax, 0xc
        push   eax
        mov    dword ptr [esi + 0x6a], 0x22
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        lea    eax, [esi + 0x72]
        push   eax
        lea    eax, [esi + 0x5a]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x34]
        add    esp, 0x18
        jmp    loc_1_1E71
loc_1_1E47:
        mov    eax, dword ptr [edi + 0x88]
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x28]
        add    eax, 0xc
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x28]
        add    esp, 0x10
loc_1_1E71:
        or     eax, eax
        je     loc_1_1E90
        cmp    eax, 0xff
        je     loc_1_1E9C
        push   esi
        call   sub_1_1301F
        add    esp, 4
        push   edi
        call   sub_1_29C4
        add    esp, 4
        jmp    loc_1_1E9C
loc_1_1E90:
        push   ebx
        push   0
        push   esi
        call   sub_1_1EA4
        add    esp, 0xc
loc_1_1E9C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_1EA4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x1c]
        and    eax, 1
        mov    edi, dword ptr [esi + 0x8c]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xfffffffd
        add    ebx, 4
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_1EFE
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x1c], 0
        push   dword ptr [esp + 0x1c]
        push   ebp
        call   sub_1_1324F
        add    esp, 8
        push   ebp
        push   edi
        call   sub_1_1833
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 8
        add    dword ptr [edi + 0x18], eax
        inc    dword ptr [edi + 0x14]
        jmp    loc_1_1F0B
loc_1_1EFE:
        or     dword ptr [esi + 0x3c], 1
        push   ebp
        call   sub_1_1301F
        add    esp, 4
loc_1_1F0B:
        push   dword ptr [esp + 0x18]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_1_F3AC
        add    esp, 0xc
        push   esi
        call   sub_1_29C4
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1F30:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x2c]
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_2013
        mov    eax, dword ptr [esp + 0x3c]
        or     eax, eax
        jne    loc_1_1F85
        cmp    dword ptr [ebx + 0x88], 0
        je     loc_1_1F7B
        mov    ecx, dword ptr [ebx + 0x18]
        lea    edi, [ebx + 0xa4]
        lea    esi, [ecx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 0xb4], eax
        movzx  eax, word ptr [ecx + 2]
        mov    dword ptr [ebx + 0xb8], eax
        jmp    loc_1_1FCF
loc_1_1F7B:
        mov    edi, 0x2749
        jmp    loc_1_210D
loc_1_1F85:
        mov    ebp, dword ptr [esp + 0x40]
        cmp    ebp, 0x18
        jne    loc_1_2108
        push   ebp
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    loc_1_1FA5
        jmp    loc_1_2108
loc_1_1FA5:
        lea    esi, [eax + 8]
        lea    edi, [ebx + 0xa4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0xb4], ecx
        movzx  ecx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0xb8], ecx
        push   ebp
        push   eax
        call   sub_1_F2B5
        add    esp, 8
loc_1_1FCF:
        mov    dword ptr [ebx + 0xbc], 1
        cmp    dword ptr [ebx + 0x4c], 1
        jne    loc_1_201D
        mov    word ptr [esp + 0x10], 0x12
        lea    edi, [esp + 0x18]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x14], 0xffffffff
        lea    eax, [esp + 0x10]
        push   0x18
        push   eax
        push   ebx
        call   sub_1_0180
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        je     loc_1_201D
        jmp    loc_1_210D
loc_1_2013:
        mov    dword ptr [ebx + 0xbc], 0
loc_1_201D:
        mov    esi, dword ptr [esp + 0x34]
        push   esi
        push   dword ptr [esp + 0x34]
        call   sub_1_F270
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_20FE
        mov    edi, dword ptr [esp + 0x44]
        or     edi, edi
        je     loc_1_20FE
        mov    eax, dword ptr [ebx + 0x8c]
        mov    ecx, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 4], ecx
        jb     loc_1_2063
        cmp    dword ptr [eax + 8], 0x10
        ja     loc_1_2063
        push   esi
        call   sub_1_12F70
        add    esp, 4
        jmp    loc_1_2065
loc_1_2063:
        sub    eax, eax
loc_1_2065:
        or     eax, eax
        je     loc_1_208D
        push   0
        push   edi
        push   eax
        push   dword ptr [esp + 0x44]
        push   esi
        push   ebp
        push   ebx
        call   sub_1_2213
        add    esp, 0x1c
        mov    edi, eax
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
        jmp    loc_1_210D
loc_1_208D:
        and    dword ptr [ebx + 0x50], 0xfffffffd
        test   byte ptr [ebx + 0x41], 1
        jne    loc_1_20B0
        cmp    dword ptr [ebx + 0x58], 0
        je     loc_1_20F5
        or     dword ptr [ebx + 0x80], 4
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
        jmp    loc_1_20CD
loc_1_20B0:
        mov    dword ptr [ebx + 0x98], ebp
        mov    dword ptr [ebx + 0x9c], esi
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [ebx + 0xa0], eax
        or     dword ptr [ebx + 0x80], 2
loc_1_20CD:
        mov    dword ptr [ebx + 0x90], 0x1362c ; -> loc_1_1362C
        lea    ecx, [ebx + 0x90]
        mov    eax, dword ptr [0x13630]      ; -> loc_1_13630
        mov    dword ptr [ebx + 0x94], eax
        mov    eax, dword ptr [0x13630]      ; -> loc_1_13630
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x13630], ecx      ; -> loc_1_13630
loc_1_20F5:
        mov    dword ptr [edi], esi
        mov    edi, 0x2733
        jmp    loc_1_210D
loc_1_20FE:
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
loc_1_2108:
        mov    edi, 0x271e
loc_1_210D:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_2117:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [0x1362c]      ; -> loc_1_1362C
        cmp    esi, 0x1362c                  ; -> loc_1_1362C
        je     loc_1_220B
        mov    edi, 0x100
        mov    ebx, 0x10
loc_1_213A:
        lea    ebp, [esi - 0x90]
        mov    esi, dword ptr [esi]
        test   byte ptr [ebp + 0x80], 4
        jne    loc_1_21D4
        test   dword ptr [ebp + 0x40], edi
        je     loc_1_21E8
        mov    eax, dword ptr [ebp + 0x8c]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0xc], ecx
        ja     loc_1_217B
        cmp    dword ptr [eax + 8], ebx
        ja     loc_1_217B
        push   dword ptr [ebp + 0x9c]
        call   sub_1_12F70
        add    esp, 4
        jmp    loc_1_217D
loc_1_217B:
        sub    eax, eax
loc_1_217D:
        or     eax, eax
        je     loc_1_21FF
        mov    edx, dword ptr [ebp + 0x94]
        lea    ecx, [ebp + 0x90]
        push   1
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esp + 0x14]
        and    dword ptr [ebp + 0x80], 0xfffffffd
        push   ecx
        push   eax
        push   dword ptr [ebp + 0xa0]
        push   dword ptr [ebp + 0x9c]
        push   dword ptr [ebp + 0x98]
        push   ebp
        call   sub_1_2213
        add    esp, 0x1c
        push   dword ptr [ebp + 0x9c]
        push   dword ptr [ebp + 0x98]
        call   sub_1_F2B5
        add    esp, 8
        jmp    loc_1_21FF
loc_1_21D4:
        push   0
        push   2
        push   ebp
        call   sub_1_F3AC
        add    esp, 0xc
        and    dword ptr [ebp + 0x80], 0xfffffffb
loc_1_21E8:
        mov    ecx, dword ptr [ebp + 0x94]
        mov    eax, dword ptr [ebp + 0x90]
        add    ebp, 0x90
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_21FF:
        cmp    esi, 0x1362c                  ; -> loc_1_1362C
        jne    loc_1_213A
loc_1_220B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_2213:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    ebp, dword ptr [esp + 0x30]
        mov    ebx, dword ptr [esp + 0x20]
        mov    dword ptr [ebp + 0x18], ebx
        push   dword ptr [esp + 0x2c]
        call   sub_1_2D98
        add    esp, 4
        lea    ecx, [ebp + 0x38]
        or     dword ptr [ebp + 0x1c], eax
        mov    dword ptr [ecx], 1
        mov    word ptr [ebp + 0x3c], 0x1a
        mov    word ptr [ebp + 0x3e], 0x15
        lea    esi, [ebx + 0xa4]
        mov    eax, dword ptr [ebx + 0xb4]
        mov    dword ptr [ebp + 0x50], eax
        mov    eax, dword ptr [ebx + 0xb8]
        mov    dword ptr [ebp + 0x54], eax
        lea    edi, [ebp + 0x40]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebp + 0x6e], ecx
        mov    dword ptr [ebp + 0x6a], 0x22
        mov    eax, dword ptr [ebx + 0x8c]
        mov    ecx, dword ptr [esp + 0x28]
        add    dword ptr [eax + 0xc], ecx
        push   ecx
        inc    dword ptr [eax + 8]
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_1_13196
        lea    eax, [esp + 0x24]
        add    esp, 0xc
        push   eax
        lea    eax, [esp + 0x18]
        push   1
        push   eax
        push   ebp
        call   sub_1_22EE
        mov    ecx, dword ptr [esp + 0x44]
        add    esp, 0x10
        mov    dword ptr [ecx], eax
        cmp    eax, -1
        jne    loc_1_22C8
        push   dword ptr [esp + 0x14]
        call   sub_1_2C80
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
loc_1_22C8:
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_22E2
        and    dword ptr [ebx + 0x54], 0xfffffffd
        push   dword ptr [esp + 0x10]
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_22E2:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_22EE:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x2c]
        mov    esi, dword ptr [edi + 0x18]
        push   esi
        mov    eax, dword ptr [esi + 0x8c]
        mov    dword ptr [esp + 0x14], eax
        call   sub_1_29BC
        mov    eax, dword ptr [esp + 0x3c]
        add    esp, 4
        mov    dword ptr [eax], 0
        mov    dword ptr [esp + 0x1c], 0x243c ; -> loc_1_243C
        mov    dword ptr [esp + 0x20], edi
        mov    ebx, dword ptr [edi + 0x20]
        mov    eax, dword ptr [0x13778]      ; -> loc_1_13778
        inc    eax
        mov    dword ptr [0x13778], eax      ; -> loc_1_13778
        mov    ecx, dword ptr [edi + 0x10]
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_2369
        mov    eax, dword ptr [esi + 0x84]
        add    ecx, 0xc
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x14]
        push   ecx
        push   eax
        lea    eax, [edi + 0x5a]
        push   ebx
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x30]
        add    esp, 0x14
        jmp    loc_1_238E
loc_1_2369:
        mov    eax, dword ptr [esi + 0x88]
        add    ecx, 0xc
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [esp + 0x18], eax
        push   ecx
        push   ebx
        lea    eax, [esp + 0x20]
        push   dword ptr [edi + 0x1c]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x2c]
        add    esp, 0x10
loc_1_238E:
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x30]
        cmp    ebp, 0xff
        mov    dword ptr [eax], ebp
        mov    eax, dword ptr [0x13778]      ; -> loc_1_13778
        je     loc_1_23A4
        dec    eax
loc_1_23A4:
        mov    dword ptr [0x13778], eax      ; -> loc_1_13778
        or     ebp, ebp
        je     loc_1_23CA
        cmp    ebp, 1
        je     loc_1_23D5
        cmp    ebp, 0xff
        je     loc_1_240C
        mov    ebx, 0xffffffff
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    loc_1_240C
loc_1_23CA:
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    loc_1_240C
loc_1_23D5:
        mov    dword ptr [0x13778], eax      ; -> loc_1_13778
        or     eax, eax
        jne    loc_1_23EE
        mov    ebx, 0xffffffff
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    loc_1_240C
loc_1_23EE:
        mov    eax, dword ptr [esp + 0x38]
        cmp    dword ptr [esp + 0x34], 0
        mov    dword ptr [eax], 1
        je     loc_1_240C
        push   edi
        push   dword ptr [esp + 0x14]
        call   sub_1_17FA
        add    esp, 8
loc_1_240C:
        or     ebp, ebp
        je     loc_1_2415
        cmp    ebx, -1
        jne    loc_1_2432
loc_1_2415:
        mov    eax, dword ptr [esi + 0x8c]
        mov    ecx, dword ptr [edi + 0x20]
        sub    dword ptr [eax + 0xc], ecx
        dec    dword ptr [eax + 8]
        push   edi
        call   sub_1_1301F
        add    esp, 4
        call   sub_1_2117
loc_1_2432:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_243C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x18]
        dec    dword ptr [0x13778]           ; -> loc_1_13778
        call   sub_1_2479
        mov    eax, dword ptr [edi + 0x8c]
        mov    ecx, dword ptr [esi + 0x20]
        sub    dword ptr [eax + 0xc], ecx
        dec    dword ptr [eax + 8]
        push   esi
        call   sub_1_1301F
        add    esp, 4
        call   sub_1_2117
        push   edi
        call   sub_1_29C4
        add    esp, 4
        pop    edi
        pop    esi
        ret    
sub_1_2479:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 0x1364c                  ; -> loc_1_1364C
        sub    ebx, ebx
loc_1_2487:
        cmp    dword ptr [0x1364c], edi      ; -> loc_1_1364C
        je     loc_1_2515
        mov    esi, dword ptr [0x1364c]      ; -> loc_1_1364C
        mov    eax, esi
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [0x1364c], eax      ; -> loc_1_1364C
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 0x10]
        mov    ebp, dword ptr [esi + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   ebx
        push   eax
        push   esi
        call   sub_1_22EE
        add    esp, 0x10
        cmp    eax, -1
        je     loc_1_250C
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_1_2487
        mov    eax, dword ptr [esi + 0x18]
        mov    edx, dword ptr [eax + 0x8c]
        mov    eax, dword ptr [0x1364c]      ; -> loc_1_1364C
        mov    dword ptr [esi], eax
        lea    ecx, [esi + 8]
        mov    dword ptr [esi + 4], 0x1364c  ; -> loc_1_1364C
        mov    eax, dword ptr [0x1364c]      ; -> loc_1_1364C
        add    edx, 0x24
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [0x1364c], esi      ; -> loc_1_1364C
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0xc], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_1_2515
loc_1_250C:
        push   ebp
        call   sub_1_2B23
        add    esp, 4
loc_1_2515:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  000h, 000h, 000h
loc_1_2520:
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, esi
        je     loc_1_253D
        cmp    eax, 1
        je     loc_1_2579
        cmp    eax, 2
        je     loc_1_253D
        mov    esi, 0x2726
        jmp    loc_1_2579
loc_1_253D:
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 0x2c], 1
        jne    loc_1_2579
        mov    eax, dword ptr [edi + 0x8c]
        or     eax, eax
        je     loc_1_2579
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        je     loc_1_2579
        push   dword ptr [edi + 0x88]
        call   sub_1_089A
        add    esp, 4
        push   0x2745
        push   0x20
        push   edi
        call   sub_1_F3AC
        add    esp, 0xc
loc_1_2579:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
loc_1_2580:
        call   sub_1_2880
        or     eax, eax
        jne    loc_1_258B
        sub    eax, eax
loc_1_258B:
        ret    
loc_1_258C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x84]
        or     eax, eax
        je     loc_1_25A7
        cmp    dword ptr [eax + 0x18], esi
        jne    loc_1_25A7
        mov    dword ptr [eax + 0x18], 0
loc_1_25A7:
        cmp    word ptr [esi + 0x10], 0
        je     loc_1_25B5
        cmp    word ptr [esi + 0x12], 0
        je     loc_1_25C5
loc_1_25B5:
        mov    eax, dword ptr [esi + 0x8c]
        mov    ecx, dword ptr [eax + 0x2c]
        sub    ecx, eax
        cmp    ecx, 0x2c
        je     loc_1_25E1
loc_1_25C5:
        push   esi
        call   sub_1_2B23
        add    esp, 4
        mov    eax, dword ptr [esi + 0x88]
        or     eax, eax
        je     loc_1_25E1
        push   eax
        call   sub_1_089A
        add    esp, 4
loc_1_25E1:
        push   esi
        call   sub_1_2B3B
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2600:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_29BC
        add    esp, 4
        sub    esi, esi
        cmp    dword ptr [esp + 0x20], 0xffff
        je     loc_1_262A
        mov    esi, 0x2726
        jmp    loc_1_2757
loc_1_262A:
        mov    eax, dword ptr [esp + 0x24]
        cmp    eax, 2
        je     loc_1_264F
        cmp    eax, 0x7001
        je     loc_1_2693
        cmp    eax, 0x7002
        je     loc_1_2724
        mov    esi, 0x273a
        jmp    loc_1_2757
loc_1_264F:
        cmp    dword ptr [ebx + 0x2c], 2
        jne    loc_1_265F
        mov    esi, 0x273a
        jmp    loc_1_2757
loc_1_265F:
        mov    eax, dword ptr [esp + 0x2c]
        mov    ebp, dword ptr [esp + 0x28]
        mov    edx, dword ptr [eax]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [ebx + 0x80]
        and    eax, 8
        cmp    eax, 1
        sbb    al, al
        inc    al
        mov    byte ptr [ebp], al
        jmp    loc_1_2757
loc_1_2693:
        mov    eax, dword ptr [esp + 0x2c]
        mov    ebp, dword ptr [esp + 0x28]
        mov    edx, dword ptr [eax]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, -1
        jne    loc_1_26CE
        cmp    dword ptr [0x136ec], 0        ; -> loc_1_136EC
        je     loc_1_2757
        mov    eax, dword ptr [0x136f0]      ; -> loc_1_136F0
        jmp    loc_1_26D7
loc_1_26CE:
        push   eax
        call   sub_1_EE10
        add    esp, 4
loc_1_26D7:
        or     eax, eax
        je     loc_1_2757
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_2757
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ecx
        call   sub_1_10879
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2700
        mov    eax, 1
        jmp    loc_1_271F
loc_1_2700:
        cmp    dword ptr [esp + 0x10], 2
        jne    loc_1_270E
        mov    eax, 6
        jmp    loc_1_271F
loc_1_270E:
        cmp    dword ptr [esp + 0x10], 1
        mov    eax, 0xa
        je     loc_1_271F
        mov    eax, 2
loc_1_271F:
        mov    byte ptr [ebp], al
        jmp    loc_1_2757
loc_1_2724:
        mov    eax, dword ptr [esp + 0x2c]
        mov    edx, dword ptr [eax]
        cmp    edx, 4
        jne    loc_1_2741
        mov    eax, dword ptr [ebx + 0x8c]
        mov    ebp, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp], eax
        jmp    loc_1_2757
loc_1_2741:
        mov    ebp, dword ptr [esp + 0x28]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_1_2757:
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_276A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_29BC
        mov    ebp, dword ptr [esp + 0x28]
        add    esp, 4
        mov    ebx, dword ptr [esp + 0x20]
        push   ebp
        push   ebx
        call   sub_1_285F
        add    esp, 8
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
        cmp    dword ptr [esp + 0x18], 0xffff
        je     loc_1_27AA
        mov    edi, 0x2726
        jmp    loc_1_284F
loc_1_27AA:
        mov    eax, dword ptr [esp + 0x1c]
        cmp    eax, 8
        je     loc_1_27D1
        cmp    eax, 0x100
        je     loc_1_27F1
        cmp    eax, 0x1001
        je     loc_1_2811
        cmp    eax, 0x1002
        je     loc_1_2822
        cmp    eax, 0x8000
        je     loc_1_2833
        jmp    loc_1_284F
loc_1_27D1:
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_27D9
        jmp    loc_1_284A
loc_1_27D9:
        mov    eax, dword ptr [esi + 0x88]
        or     eax, eax
        je     loc_1_284F
        push   ecx
        push   2
        push   eax
        call   sub_1_07CD
        add    esp, 0xc
        jmp    loc_1_284F
loc_1_27F1:
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_1_27F9
        jmp    loc_1_284A
loc_1_27F9:
        mov    eax, dword ptr [esi + 0x88]
        or     eax, eax
        je     loc_1_284F
        push   ecx
        push   3
        push   eax
        call   sub_1_07CD
        add    esp, 0xc
        jmp    loc_1_284F
loc_1_2811:
        movzx  eax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 0x8c]
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_284F
loc_1_2822:
        movzx  eax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 0x8c]
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [ecx + 0x10], eax
        jmp    loc_1_284F
loc_1_2833:
        cmp    ebp, 4
        jb     loc_1_284A
        cmp    word ptr [ebx], 0x4d2
        jne    loc_1_284A
        or     dword ptr [esi + 0x80], 1
        sub    edi, edi
        jmp    loc_1_284F
loc_1_284A:
        mov    edi, 0x273a
loc_1_284F:
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_285F:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_2867:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     loc_1_287A
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     loc_1_2867
        sub    eax, eax
        jmp    loc_1_287F
loc_1_287A:
        mov    eax, 1
loc_1_287F:
        ret    
sub_1_2880:
        push   esi
        push   edi
        call   sub_1_F3A4
        push   0xc0
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     loc_1_28E2
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x30
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x38], 0x2710
        mov    dword ptr [esi + 0x34], 0x2710
        mov    dword ptr [esi + 0x74], 0x13834 ; -> loc_1_13834
        lea    ecx, [esi + 0x74]
        mov    eax, dword ptr [0x13838]      ; -> loc_1_13838
        mov    dword ptr [esi + 0x78], eax
        mov    eax, dword ptr [0x13838]      ; -> loc_1_13838
        push   esi
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x13838], ecx      ; -> loc_1_13838
        call   sub_1_29BC
        add    esp, 4
        mov    eax, esi
loc_1_28E2:
        pop    edi
        pop    esi
        ret    
sub_1_28E5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x80]
        test   al, 2
        je     loc_1_2911
        and    eax, 0xfffffffd
        mov    dword ptr [esi + 0x80], eax
        push   dword ptr [esi + 0x9c]
        push   dword ptr [esi + 0x98]
        call   sub_1_F2B5
        add    esp, 8
loc_1_2911:
        mov    dword ptr [esi + 0x8c], 0
        mov    eax, dword ptr [esi + 0x88]
        or     eax, eax
        je     loc_1_2938
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [esi + 0x88], 0
loc_1_2938:
        mov    eax, dword ptr [esi + 0x84]
        or     eax, eax
        je     loc_1_2955
        push   eax
        call   sub_1_0B13
        add    esp, 4
        mov    dword ptr [esi + 0x84], 0
loc_1_2955:
        push   esi
        call   sub_1_2B53
        add    esp, 4
        pop    esi
        ret    
sub_1_2960:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        mov    edi, edx
        mov    ebx, dword ptr [esp + 0x10]
        mov    esi, ebx
        mov    ecx, 0x1d
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx + 0x50], 0
        mov    dword ptr [edx + 0x4c], 6
        mov    dword ptr [edx + 0x54], 0x10
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [ebx + 0x80]
        mov    dword ptr [edx + 0x80], eax
        mov    eax, dword ptr [ebx + 0x84]
        or     eax, eax
        mov    dword ptr [edx + 0x84], eax
        je     loc_1_29B8
        push   eax
        call   sub_1_0B0B
        add    esp, 4
loc_1_29B8:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_29BC:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x7c]
        ret    
sub_1_29C4:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 0x7c]
        dec    edx
        mov    dword ptr [ecx + 0x7c], edx
        jne    loc_1_29E1
        push   ecx
        call   sub_1_28E5
        add    esp, 4
        mov    eax, 1
loc_1_29E1:
        ret    
sub_1_29E2:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x30]
        mov    ebx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [ebp + 0x52]
        mov    dword ptr [esp + 0x10], eax
        push   dword ptr [ebx + 0x84]
        push   dword ptr [esp + 0x14]
        call   sub_1_06EF
        add    esp, 8
        or     eax, eax
        jne    loc_1_2B1B
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0x34]
        mov    dword ptr [eax + 0x10], ebx
        mov    dword ptr [ebp + 0x30], 1
        add    esi, 8
        mov    word ptr [ebp + 0x34], 0x1a
        mov    word ptr [ebp + 0x36], 0x15
        mov    eax, dword ptr [esi - 4]
        mov    dword ptr [ebp + 0x48], eax
        lea    edi, [ebp + 0x38]
        movzx  eax, word ptr [esi - 6]
        mov    dword ptr [ebp + 0x4c], eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   ebx
        call   sub_1_29BC
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        mov    eax, dword ptr [eax + 0x58]
        mov    dword ptr [ebx + 0x8c], eax
        push   ebx
        push   eax
        call   sub_1_17E3
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 8
        mov    dword ptr [ebx + 0x88], eax
        mov    dword ptr [ebx + 0x4c], 5
        and    dword ptr [ebx + 0x50], 0xfffffffd
        mov    esi, dword ptr [eax + 0x18]
        mov    dword ptr [eax + 0x18], 0x60000004
        push   eax
        call   sub_1_06C9
        mov    dword ptr [esp + 0x18], 0
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x1c], 0x2b8f ; -> sub_1_2B8F
        mov    dword ptr [esp + 0x20], ebp
        lea    eax, [ebp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   ebp
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        mov    eax, dword ptr [0x12a58]      ; -> obj1:0x12A58
        call   dword ptr [eax + 0x18]
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        je     loc_1_2B0C
        cmp    edi, 0xff
        je     loc_1_2B19
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x4c], 2
        mov    dword ptr [eax + 0x18], esi
        mov    dword ptr [ebx + 0x88], 0
        mov    dword ptr [ebx + 0x8c], 0
        push   eax
        call   sub_1_06D1
        add    esp, 4
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        jmp    loc_1_2B19
loc_1_2B0C:
        push   0
        push   0
        push   ebp
        call   sub_1_2B8F
        add    esp, 0xc
loc_1_2B19:
        mov    eax, edi
loc_1_2B1B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_2B23:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x8c]
        or     eax, eax
        je     loc_1_2B3A
        push   eax
        call   sub_1_18C9
        add    esp, 4
loc_1_2B3A:
        ret    
sub_1_2B3B:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x8c]
        or     eax, eax
        je     loc_1_2B52
        push   eax
        call   sub_1_190E
        add    esp, 4
loc_1_2B52:
        ret    
sub_1_2B53:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 0x78]
        lea    eax, [edx + 0x74]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edx + 0x80]
        test   al, 2
        jne    loc_1_2B72
        test   al, 4
        je     loc_1_2B85
loc_1_2B72:
        mov    ecx, dword ptr [edx + 0x94]
        lea    eax, [edx + 0x90]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_2B85:
        push   edx
        call   sub_1_F396
        add    esp, 4
        ret    
sub_1_2B8F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x18], 0
        mov    ebx, dword ptr [eax + 0x52]
        mov    ebp, dword ptr [ebx + 0x10]
        jne    loc_1_2C18
        cmp    dword ptr [ebx + 0x18], 0x60000004
        jne    loc_1_2C18
        mov    dword ptr [ebp + 0x4c], 6
        mov    esi, eax
        add    esi, 0x38
        lea    edi, [ebx + 0x3c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x48]
        mov    dword ptr [ebx + 0x38], eax
        lea    eax, [ebx + 0x1c]
        mov    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp + 0x20], 0x18
        lea    eax, [ebx + 0x34]
        mov    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp + 0x24], 0x18
        push   ebx
        call   sub_1_074D
        add    esp, 4
        test   byte ptr [ebp + 0x40], 8
        je     loc_1_2C00
        push   1
        push   2
        push   ebx
        call   sub_1_07CD
        add    esp, 0xc
loc_1_2C00:
        test   byte ptr [ebp + 0x80], 8
        je     loc_1_2C3C
        push   1
        push   1
        push   ebx
        call   sub_1_07CD
        add    esp, 0xc
        jmp    loc_1_2C3C
loc_1_2C18:
        mov    dword ptr [ebp + 0x4c], 2
        mov    dword ptr [ebp + 0x88], 0
        mov    dword ptr [ebp + 0x8c], 0
        push   ebx
        call   sub_1_06D1
        add    esp, 4
loc_1_2C3C:
        push   dword ptr [esp + 0x14]
        call   sub_1_2E44
        add    esp, 4
        push   ebp
        call   sub_1_29C4
        add    esp, 4
        or     eax, eax
        jne    loc_1_2C7B
        push   dword ptr [esp + 0x18]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   0x10
        push   ebp
        call   sub_1_F3AC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_2C7B
        mov    dword ptr [ebp + 0x54], 8
loc_1_2C7B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2C80:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0xfe
        jg     loc_1_2C99
        je     loc_1_2CB9
        cmp    eax, 0x22
        ja     loc_1_2C99
        jmp    dword ptr [eax*4 + 0x2d0c]    ; -> obj1:0x2D0C
loc_1_2C99:
        mov    eax, 0x2726
        jmp    loc_1_2D0B
loc_1_2CA0:
        sub    eax, eax
        jmp    loc_1_2D0B
loc_1_2CA4:
        mov    eax, 0x2747
        jmp    loc_1_2D0B
loc_1_2CAB:
        mov    eax, 0x2740
        jmp    loc_1_2D0B
loc_1_2CB2:
        mov    eax, 0x2741
        jmp    loc_1_2D0B
loc_1_2CB9:
        mov    eax, 0x2726
        jmp    loc_1_2D0B
loc_1_2CC0:
        mov    eax, 0x2738
        jmp    loc_1_2D0B
loc_1_2CC7:
        mov    eax, 0x274d
        jmp    loc_1_2D0B
loc_1_2CCE:
        mov    eax, 0x2745
        jmp    loc_1_2D0B
loc_1_2CD5:
        mov    eax, 0x2746
        jmp    loc_1_2D0B
loc_1_2CDC:
        mov    eax, 0x274c
        jmp    loc_1_2D0B
loc_1_2CE3:
        mov    eax, 0x2775
        jmp    loc_1_2D0B
loc_1_2CEA:
        mov    eax, 0x2743
        jmp    loc_1_2D0B
loc_1_2CF1:
        mov    eax, 0x2751
        jmp    loc_1_2D0B
loc_1_2CF8:
        mov    eax, 0x2757
        jmp    loc_1_2D0B
loc_1_2CFF:
        mov    eax, 0x2714
        jmp    loc_1_2D0B
loc_1_2D06:
        mov    eax, 0x271e
loc_1_2D0B:
        ret    
        ; ---- dispatch table (35 entries) ----
        dd  loc_1_2CA0
        dd  loc_1_2CA4
        dd  loc_1_2CAB
        dd  loc_1_2CB2
        dd  loc_1_2CA4
        dd  loc_1_2C99
        dd  loc_1_2CB2
        dd  loc_1_2CB9
        dd  loc_1_2C99
        dd  loc_1_2CC0
        dd  loc_1_2CB9
        dd  loc_1_2CB9
        dd  loc_1_2C99
        dd  loc_1_2C99
        dd  loc_1_2CC7
        dd  loc_1_2CB9
        dd  loc_1_2CB9
        dd  loc_1_2CB9
        dd  loc_1_2CB9
        dd  loc_1_2CCE
        dd  loc_1_2CD5
        dd  loc_1_2CDC
        dd  loc_1_2CE3
        dd  loc_1_2C99
        dd  loc_1_2C99
        dd  loc_1_2CB9
        dd  loc_1_2CB9
        dd  loc_1_2CEA
        dd  loc_1_2CF1
        dd  loc_1_2CEA
        dd  loc_1_2CF8
        dd  loc_1_2CB9
        dd  loc_1_2CFF
        dd  loc_1_2D06
        dd  loc_1_2CFF
sub_1_2D98:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 1
        je     loc_1_2DA8
        mov    eax, 0x20
loc_1_2DA8:
        test   ch, 0x80
        je     loc_1_2DB0
        or     eax, 0x40
loc_1_2DB0:
        test   cl, 4
        je     loc_1_2DBA
        or     eax, 0x200
loc_1_2DBA:
        ret    
sub_1_2DBB:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 0x40
        je     loc_1_2DCB
        mov    eax, 1
loc_1_2DCB:
        test   cl, 0x10
        je     loc_1_2DD5
        or     eax, 0x8000
loc_1_2DD5:
        ret    
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2DE0:
        push   esi
        push   edi
        push   1
        call   sub_1_05D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_2DF6
        sub    eax, eax
        jmp    loc_1_2E41
loc_1_2DF6:
        call   sub_1_F3A4
        push   0x56
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_2E18
        push   esi
        call   sub_1_0655
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_2E41
loc_1_2E18:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x15
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [edx + 0x10], 0x22
        lea    eax, [edx + 0x30]
        mov    dword ptr [edx + 0x14], eax
        mov    dword ptr [edx + 0x28], 0x22
        mov    dword ptr [edx + 0x2c], eax
        mov    dword ptr [edx + 0x52], esi
        mov    eax, edx
loc_1_2E41:
        pop    edi
        pop    esi
        ret    
sub_1_2E44:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x52]
        or     eax, eax
        je     loc_1_2E60
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [esi + 0x52], 0
loc_1_2E60:
        push   esi
        call   sub_1_2E6B
        add    esp, 4
        pop    esi
        ret    
sub_1_2E6B:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2E80:
        sub    esp, 0x10
        push   ebx
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0xc], 0
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 4], 0
        mov    bl, byte ptr [esp + 0x20]
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   ebx
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x30]
        push   3
        push   0x12a98                       ; -> loc_1_12A98
        call   sub_1_3089
        add    esp, 0x24
        or     eax, eax
        jne    loc_1_2F34
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   ebx
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x30]
        push   3
        push   0x12aa4                       ; -> loc_1_12AA4
        call   sub_1_3089
        add    esp, 0x24
        or     eax, eax
        jne    loc_1_2F77
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   ebx
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x30]
        push   3
        push   0x12ab0                       ; -> loc_1_12AB0
        call   sub_1_3089
        add    esp, 0x24
loc_1_2F34:
        or     eax, eax
        jne    loc_1_2F77
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_2F46
        mov    eax, 0x273f
        jmp    loc_1_2F79
loc_1_2F46:
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_2F54
        mov    eax, 0x273c
        jmp    loc_1_2F79
loc_1_2F54:
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_2F62
        mov    eax, 0x273b
        jmp    loc_1_2F79
loc_1_2F62:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_2F70
        mov    eax, 0x2739
        jmp    loc_1_2F79
loc_1_2F70:
        mov    eax, 0x2726
        jmp    loc_1_2F79
loc_1_2F77:
        sub    eax, eax
loc_1_2F79:
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_2F7E:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x10]
        push   eax
        sub    esi, esi
        lea    eax, [esp + 0x18]
        mov    edi, dword ptr [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [edi]
        mov    ebx, dword ptr [esp + 0x34]
        push   eax
        lea    eax, [esp + 0x2c]
        push   dword ptr [ebx]
        mov    ebp, dword ptr [esp + 0x38]
        push   eax
        push   dword ptr [ebp]
        push   3
        push   0x12a98                       ; -> loc_1_12A98
        call   sub_1_3089
        add    esp, 0x24
        cmp    eax, esi
        je     loc_1_2FC8
        mov    ecx, 0x12a98                  ; -> loc_1_12A98
        jmp    loc_1_3030
loc_1_2FC8:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [edi]
        push   eax
        lea    eax, [esp + 0x2c]
        push   dword ptr [ebx]
        push   eax
        push   dword ptr [ebp]
        push   3
        push   0x12aa4                       ; -> loc_1_12AA4
        call   sub_1_3089
        add    esp, 0x24
        or     eax, eax
        je     loc_1_2FFD
        mov    ecx, 0x12aa4                  ; -> loc_1_12AA4
        jmp    loc_1_3030
loc_1_2FFD:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   dword ptr [edi]
        push   eax
        lea    eax, [esp + 0x2c]
        push   dword ptr [ebx]
        push   eax
        push   dword ptr [ebp]
        push   3
        push   0x12ab0                       ; -> loc_1_12AB0
        call   sub_1_3089
        add    esp, 0x24
        or     eax, eax
        je     loc_1_3044
        mov    ecx, 0x12ab0                  ; -> loc_1_12AB0
loc_1_3030:
        movzx  eax, byte ptr [ecx]
        mov    dword ptr [ebp], eax
        movzx  eax, byte ptr [ecx + 1]
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [ecx + 2]
        mov    dword ptr [edi], eax
        jmp    loc_1_307F
loc_1_3044:
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_3052
        mov    esi, 0x273f
        jmp    loc_1_307F
loc_1_3052:
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_3060
        mov    esi, 0x273c
        jmp    loc_1_307F
loc_1_3060:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_306E
        mov    esi, 0x273b
        jmp    loc_1_307F
loc_1_306E:
        cmp    dword ptr [esp + 0x10], 0
        mov    esi, 0x2739
        jne    loc_1_307F
        mov    esi, 0x2726
loc_1_307F:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_3089:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     loc_1_3106
        mov    edi, dword ptr [esp + 0x14]
        mov    cl, byte ptr [esp + 0x1c]
        mov    dl, byte ptr [esp + 0x24]
        mov    bl, byte ptr [esp + 0x2c]
loc_1_30A8:
        cmp    cl, byte ptr [edi]
        jne    loc_1_30B6
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], 1
loc_1_30B6:
        cmp    dl, byte ptr [edi + 1]
        jne    loc_1_30C5
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], 1
loc_1_30C5:
        mov    al, byte ptr [edi + 2]
        cmp    bl, al
        jne    loc_1_30D7
        mov    ebp, dword ptr [esp + 0x30]
        mov    dword ptr [ebp], 1
loc_1_30D7:
        cmp    cl, byte ptr [edi]
        jne    loc_1_30FC
        cmp    dl, byte ptr [edi + 1]
        jne    loc_1_30EF
        cmp    bl, al
        je     loc_1_30EF
        mov    ebp, dword ptr [esp + 0x34]
        mov    dword ptr [ebp], 1
loc_1_30EF:
        cmp    cl, byte ptr [edi]
        jne    loc_1_30FC
        cmp    dl, byte ptr [edi + 1]
        jne    loc_1_30FC
        cmp    bl, al
        je     loc_1_310D
loc_1_30FC:
        add    edi, 3
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    loc_1_30A8
loc_1_3106:
        sub    eax, eax
loc_1_3108:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_310D:
        mov    eax, 1
        jmp    loc_1_3108
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3120:
        mov    eax, dword ptr [0x13664]      ; -> loc_1_13664
        or     eax, eax
        je     loc_1_3133
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x13664], ecx      ; -> loc_1_13664
        jmp    loc_1_313D
loc_1_3133:
        push   0x24
        call   sub_1_F390
        add    esp, 4
loc_1_313D:
        ret    
sub_1_313E:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x13664]      ; -> loc_1_13664
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x13664], eax      ; -> loc_1_13664
        ret    
sub_1_3150:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, edx
        and    ecx, 0xffff
        cmp    dword ptr [0x13670], ecx      ; -> loc_1_13670
        jbe    loc_1_317B
        mov    eax, dword ptr [0x1377c]      ; -> loc_1_1377C
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     loc_1_317B
        movzx  ecx, word ptr [eax + 0x48]
        shr    edx, 0x10
        cmp    ecx, edx
        je     loc_1_317D
loc_1_317B:
        sub    eax, eax
loc_1_317D:
        ret    
sub_1_317E:
        push   ebx
        push   esi
        push   edi
loc_1_3181:
        mov    ebx, dword ptr [0x13670]      ; -> loc_1_13670
        sub    eax, eax
        mov    esi, dword ptr [0x1377c]      ; -> loc_1_1377C
        mov    ecx, dword ptr [0x13858]      ; -> loc_1_13858
        cmp    eax, ebx
        je     loc_1_31AD
loc_1_3199:
        cmp    ecx, ebx
        jne    loc_1_319F
        sub    ecx, ecx
loc_1_319F:
        cmp    dword ptr [esi + ecx*4], 0
        je     loc_1_31AB
        inc    ecx
        inc    eax
        cmp    eax, ebx
        jb     loc_1_3199
loc_1_31AB:
        cmp    eax, ebx
loc_1_31AD:
        jb     loc_1_3210
        cmp    ebx, 0x100
        je     loc_1_3246
        add    ebx, 0x10
        cmp    ebx, 0x100
        jb     loc_1_31CB
        mov    ebx, 0x100
loc_1_31CB:
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0x13670]      ; -> loc_1_13670
        mov    edx, dword ptr [0x1377c]      ; -> loc_1_1377C
        je     loc_1_3246
        shl    ecx, 2
        mov    edi, eax
        mov    esi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x1377c], eax      ; -> loc_1_1377C
        mov    dword ptr [0x13670], ebx      ; -> loc_1_13670
        push   edx
        call   sub_1_F396
        add    esp, 4
        jmp    loc_1_3181
loc_1_3210:
        mov    eax, dword ptr [esp + 0x10]
        mov    dx, word ptr [0x13818]        ; -> loc_1_13818
        mov    word ptr [eax + 0x48], dx
        mov    byte ptr [eax + 0x43], cl
        mov    dword ptr [esi + ecx*4], eax
        lea    eax, [ecx + 1]
        mov    dword ptr [0x13858], eax      ; -> loc_1_13858
        movzx  eax, dx
        shl    eax, 0x10
        and    ecx, 0xffff
        or     eax, ecx
        inc    dx
        mov    word ptr [0x13818], dx        ; -> loc_1_13818
        jmp    loc_1_324B
loc_1_3246:
        mov    eax, 0xffffffff
loc_1_324B:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_324F:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, ecx
        mov    edx, dword ptr [ebx + 4]
        lea    esi, [edx + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [edx + 0x38]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_3282
        cmp    byte ptr [eax + 0x42], 0x51
        jne    loc_1_3282
        mov    dword ptr [ebx + 4], eax
        mov    eax, 1
        jmp    loc_1_32DF
loc_1_3282:
        mov    edx, dword ptr [ebx]
loc_1_3284:
        or     eax, eax
        je     loc_1_329C
        mov    ecx, 0x51
loc_1_328D:
        cmp    byte ptr [eax + 0x42], cl
        je     loc_1_3298
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_328D
loc_1_3298:
        or     eax, eax
        jne    loc_1_32C4
loc_1_329C:
        lea    ecx, [edx*4 + 0x138d8]        ; -> loc_1_138D8
loc_1_32A3:
        add    ecx, 4
        inc    edx
        cmp    ecx, 0x13918                  ; -> loc_1_13918
        jae    loc_1_32BB
        cmp    dword ptr [ecx], 0
        je     loc_1_32A3
        mov    eax, dword ptr [edx*4 + 0x138d8] ; -> loc_1_138D8
loc_1_32BB:
        cmp    edx, 0x10
        jne    loc_1_3284
        or     eax, eax
        je     loc_1_32D0
loc_1_32C4:
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], edx
        mov    eax, 1
        jmp    loc_1_32DF
loc_1_32D0:
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0
        sub    eax, eax
loc_1_32DF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_32E3:
        push   ebx
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edx]
        or     esi, esi
        mov    eax, dword ptr [edx + 4]
        jne    loc_1_333D
        or     eax, eax
        jne    loc_1_333D
        mov    eax, dword ptr [esp + 0x10]
        lea    ecx, [esi*4 + 0x138d8]        ; -> loc_1_138D8
        mov    dword ptr [eax], 1
loc_1_3307:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_3321
        mov    ebx, 0x51
loc_1_3312:
        cmp    byte ptr [eax + 0x42], bl
        je     loc_1_331D
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_3312
loc_1_331D:
        or     eax, eax
        jne    loc_1_3331
loc_1_3321:
        add    ecx, 4
        inc    esi
        cmp    ecx, 0x13918                  ; -> loc_1_13918
        jb     loc_1_3307
        or     eax, eax
        je     loc_1_3361
loc_1_3331:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], eax
        mov    eax, 1
        jmp    loc_1_3363
loc_1_333D:
        cmp    esi, 0x10
        jae    loc_1_3357
        mov    ecx, dword ptr [esi*4 + 0x138d8] ; -> loc_1_138D8
        or     ecx, ecx
        je     loc_1_3357
loc_1_334D:
        cmp    ecx, eax
        je     loc_1_3366
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_334D
loc_1_3357:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
loc_1_3361:
        sub    eax, eax
loc_1_3363:
        pop    esi
        pop    ebx
        ret    
loc_1_3366:
        cmp    byte ptr [ecx + 0x42], 0x51
        jne    loc_1_3357
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    loc_1_3363
sub_1_337D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        movzx  ecx, byte ptr [esi + 3]
        movzx  eax, byte ptr [esi + 2]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1]
        add    ecx, eax
        movzx  eax, byte ptr [esi]
        add    ecx, eax
        and    ecx, 0xf
        mov    eax, dword ptr [ecx*4 + 0x138d8] ; -> loc_1_138D8
        or     eax, eax
        je     loc_1_3552
        mov    edi, dword ptr [esp + 0x18]
        mov    dl, byte ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x20]
        mov    ebp, 0xffffffff
loc_1_33BD:
        cmp    eax, ebx
        jbe    loc_1_3548
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 0x28], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 0x29], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 0x2a], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 0x2b], cl
        jne    loc_1_3456
        mov    cl, byte ptr [eax + 0x2c]
        cmp    byte ptr [esi + 4], cl
        jne    loc_1_3456
        mov    cl, byte ptr [eax + 0x2d]
        cmp    byte ptr [esi + 5], cl
        jne    loc_1_3456
        mov    cl, byte ptr [eax + 0x2e]
        cmp    byte ptr [esi + 6], cl
        jne    loc_1_3456
        mov    cl, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 7], cl
        jne    loc_1_3456
        mov    cl, byte ptr [eax + 0x30]
        cmp    byte ptr [esi + 8], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 0x31], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xa]
        cmp    byte ptr [eax + 0x32], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xb]
        cmp    byte ptr [eax + 0x33], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xc]
        cmp    byte ptr [eax + 0x34], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xd]
        cmp    byte ptr [eax + 0x35], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xe]
        cmp    byte ptr [eax + 0x36], cl
        jne    loc_1_3456
        mov    cl, byte ptr [esi + 0xf]
        cmp    byte ptr [eax + 0x37], cl
        jne    loc_1_3456
        cmp    dword ptr [eax + 0x38], edi
        jne    loc_1_3456
        cmp    byte ptr [eax + 0x42], dl
        je     loc_1_3554
loc_1_3456:
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 0x28], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 0x29], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 0x2a], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 0x2b], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [eax + 0x2c]
        cmp    byte ptr [esi + 4], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [eax + 0x2d]
        cmp    byte ptr [esi + 5], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [eax + 0x2e]
        cmp    byte ptr [esi + 6], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 7], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [eax + 0x30]
        cmp    byte ptr [esi + 8], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 0x31], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xa]
        cmp    byte ptr [eax + 0x32], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xb]
        cmp    byte ptr [eax + 0x33], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xc]
        cmp    byte ptr [eax + 0x34], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xd]
        cmp    byte ptr [eax + 0x35], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xe]
        cmp    byte ptr [eax + 0x36], cl
        jne    loc_1_34E3
        mov    cl, byte ptr [esi + 0xf]
        cmp    byte ptr [eax + 0x37], cl
        jne    loc_1_34E3
        cmp    dword ptr [eax + 0x38], ebp
        jne    loc_1_34E3
        cmp    byte ptr [eax + 0x42], dl
        je     loc_1_3554
loc_1_34E3:
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 0x28], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 0x29], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 0x2a], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 0x2b], cl
        jne    loc_1_3548
        mov    cl, byte ptr [eax + 0x2c]
        cmp    byte ptr [esi + 4], cl
        jne    loc_1_3548
        mov    cl, byte ptr [eax + 0x2d]
        cmp    byte ptr [esi + 5], cl
        jne    loc_1_3548
        mov    cl, byte ptr [eax + 0x2e]
        cmp    byte ptr [esi + 6], cl
        jne    loc_1_3548
        mov    cl, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 7], cl
        jne    loc_1_3548
        mov    cl, byte ptr [eax + 0x30]
        cmp    byte ptr [esi + 8], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 0x31], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 0xa]
        cmp    byte ptr [eax + 0x32], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 0xb]
        cmp    byte ptr [eax + 0x33], cl
        jne    loc_1_3548
        mov    cl, byte ptr [esi + 0xc]
        cmp    byte ptr [eax + 0x34], cl
loc_1_3548:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_33BD
loc_1_3552:
        sub    eax, eax
loc_1_3554:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3559:
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 0x28]
        movzx  eax, byte ptr [edx + 0x29]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 0x2a]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 0x2b]
        add    ecx, eax
        and    ecx, 0xf
        lea    eax, [ecx*4 + 0x138d8]        ; -> loc_1_138D8
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_358F
loc_1_3583:
        cmp    ecx, edx
        ja     loc_1_358F
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_3583
loc_1_358F:
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
        ret    
sub_1_3594:
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 0x28]
        movzx  eax, byte ptr [edx + 0x29]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 0x2a]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 0x2b]
        add    ecx, eax
        and    ecx, 0xf
        lea    ecx, [ecx*4 + 0x138d8]        ; -> loc_1_138D8
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_35D0
loc_1_35BE:
        cmp    eax, edx
        je     loc_1_35CC
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_35BE
        jmp    loc_1_35D0
loc_1_35CC:
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
loc_1_35D0:
        ret    
sub_1_35D1:
        mov    eax, dword ptr [0x13634]      ; -> loc_1_13634
        or     eax, eax
        je     loc_1_35EE
        mov    ecx, dword ptr [esp + 4]
loc_1_35DE:
        cmp    ecx, -1
        je     loc_1_35F1
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_1_35F1
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_35DE
loc_1_35EE:
        sub    eax, eax
loc_1_35F0:
        ret    
loc_1_35F1:
        mov    byte ptr [eax + 0x18], 1
        jmp    loc_1_35F0
sub_1_35F7:
        push   edi
        push   0x69
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x1a
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [0x13634]      ; -> loc_1_13634
        mov    dword ptr [0x13634], edx      ; -> loc_1_13634
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x10], 0xe2
        mov    byte ptr [edx + 0x1a], 0
        mov    byte ptr [edx + 0x18], 0
        mov    byte ptr [edx + 0x30], 1
        mov    eax, dword ptr [0x13674]      ; -> loc_1_13674
        mov    dword ptr [edx + 0x35], eax
        mov    dword ptr [edx + 0x31], eax
        mov    dword ptr [edx + 0x5d], 0
        mov    eax, dword ptr [0x137ec]      ; -> loc_1_137EC
        mov    dword ptr [edx + 0x61], eax
        mov    eax, dword ptr [0x13788]      ; -> loc_1_13788
        mov    dword ptr [edx + 0x65], eax
        lea    eax, [edx + 0x45]
        mov    dword ptr [edx + 0x39], 0
        mov    dword ptr [edx + 0x55], 0x32
        mov    dword ptr [edx + 0x4d], 0
        mov    dword ptr [edx + 0x3d], 0
        mov    dword ptr [edx + 0x41], 0
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 0x49], eax
        mov    eax, edx
        pop    edi
        ret    
        db  056h, 08Bh, 074h, 024h, 008h, 00Bh, 0F6h, 074h, 036h, 08Bh, 00Dh
        dd  loc_1_13634              ; fixup
        db  08Bh, 0D1h, 08Bh, 0C1h, 00Bh, 0C9h, 074h, 028h, 03Bh, 0C6h, 074h, 00Ah, 08Bh, 0D0h, 08Bh, 000h
        db  00Bh, 0C0h, 075h, 0F4h, 0EBh, 01Ah, 03Bh, 0C8h, 075h, 009h, 08Bh, 000h, 0A3h
        dd  loc_1_13634              ; fixup
        db  0EBh, 004h, 08Bh, 006h, 089h, 002h, 056h, 0E8h, 0D2h, 0BCh, 000h, 000h, 083h, 0C4h, 004h, 05Eh
        db  0C3h
loc_1_36C9:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_C657
        mov    esi, dword ptr [0x13634]      ; -> loc_1_13634
        or     esi, esi
        je     loc_1_3966
loc_1_36E3:
        cmp    byte ptr [esi + 0x18], 1
        jne    loc_1_395C
        mov    al, byte ptr [esi + 0x30]
        cmp    al, 1
        je     loc_1_36FC
        cmp    al, 4
        jne    loc_1_395C
loc_1_36FC:
        mov    eax, dword ptr [esi + 0x4d]
        or     eax, eax
        je     loc_1_3732
        dec    eax
        mov    dword ptr [esi + 0x4d], eax
        jne    loc_1_3732
        inc    dword ptr [esi + 0x51]
        cmp    dword ptr [0x1359c], 0x1359c  ; -> loc_1_1359C
        je     loc_1_371D
        call   sub_1_BB33
loc_1_371D:
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esi + 0xc]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
loc_1_3732:
        mov    eax, dword ptr [esi + 0x39]
        or     eax, eax
        je     loc_1_395C
        dec    eax
        mov    dword ptr [esi + 0x39], eax
        jne    loc_1_395C
        mov    ecx, dword ptr [esi + 0x59]
        mov    eax, dword ptr [esi + 0x5d]
        add    eax, ecx
        mov    dword ptr [esi + 0x5d], eax
        cmp    dword ptr [esi + 0x61], eax
        jbe    loc_1_376E
        cmp    dword ptr [esi + 0x65], eax
        ja     loc_1_3766
        mov    eax, dword ptr [esi + 0x45]
        sub    eax, esi
        cmp    eax, 0x45
        je     loc_1_376E
loc_1_3766:
        mov    dword ptr [esi + 0x39], ecx
        jmp    loc_1_395C
loc_1_376E:
        mov    dword ptr [esi + 0x5d], 0
        call   sub_1_A7B0
        mov    ebx, eax
        mov    edi, dword ptr [ebx + 4]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 0xc], 0x3c
        mov    byte ptr [edi], 0xfe
        mov    cx, word ptr [edi + 0x33]
        and    cx, 0x7fff
        mov    word ptr [edi + 0x33], cx
        mov    eax, dword ptr [esi + 0x45]
        sub    eax, esi
        cmp    eax, 0x45
        mov    edx, dword ptr [0x13648]      ; -> loc_1_13648
        mov    ebp, dword ptr [0x1376c]      ; -> loc_1_1376C
        je     loc_1_37BB
        or     cx, 0x8000
        mov    word ptr [edi + 0x33], cx
loc_1_37BB:
        cmp    dword ptr [esi + 0x3d], 0
        je     loc_1_37CA
        mov    dword ptr [esi + 0x41], 0
        jmp    loc_1_37D0
loc_1_37CA:
        lea    ecx, [esi + 0x41]
        inc    dword ptr [esi + 0x41]
loc_1_37D0:
        mov    dword ptr [esi + 0x3d], 0
        cmp    byte ptr [esi + 0x30], 4
        jne    loc_1_37EB
        mov    ebp, 4
        mov    dword ptr [esi + 0x41], 0
        jmp    loc_1_380D
loc_1_37EB:
        cmp    dword ptr [esi + 0x41], ebp
        jb     loc_1_3808
        cmp    eax, 0x45
        jne    loc_1_3808
        cmp    dword ptr [esi + 0x65], edx
        jb     loc_1_3808
        mov    ebp, 3
        mov    dword ptr [esi + 0x41], 0
        jmp    loc_1_380D
loc_1_3808:
        mov    ebp, 2
loc_1_380D:
        push   ebx
        movzx  eax, byte ptr [edi + 0x1c]
        movzx  ecx, byte ptr [edi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x17]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [edi + 0x37], ax
        push   ebp
        push   0
        push   0x3c
        push   ebx
        push   dword ptr [esi + 0xc]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        dec    dword ptr [esi + 0x3d]
        cmp    eax, 0x3c
        je     loc_1_395C
        cmp    dword ptr [esi + 0x59], 0x7b
        jne    loc_1_3929
        mov    dword ptr [esi + 0x39], 0x7b
        mov    dword ptr [esi + 0x59], 0x7b
        cmp    dword ptr [esi + 0x41], 0
        jbe    loc_1_391E
        mov    dword ptr [esi + 0x41], 0
loc_1_391E:
        push   ebx
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_395C
loc_1_3929:
        push   2
        push   ebp
        push   0
        push   0
        push   0
        push   0x3c
        push   ebx
        push   esi
        push   dword ptr [esi + 0xc]
        call   sub_1_3A0F
        add    esp, 0x24
        mov    dword ptr [esi + 0x39], 0x7b
        mov    dword ptr [esi + 0x59], 0x7b
        cmp    dword ptr [esi + 0x41], 0
        jbe    loc_1_395C
        mov    dword ptr [esi + 0x41], 0
loc_1_395C:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_36E3
loc_1_3966:
        push   0
        push   dword ptr [0x13944]           ; -> loc_1_13944
        push   0x3980                        ; -> loc_1_3980
        call   sub_1_129CC
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_3980:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0x36c9   ; -> loc_1_36C9
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0xb
        call   sub_1_129A4
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x1c], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  056h, 057h, 0FFh, 074h, 024h, 00Ch, 0E8h, 0F0h, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0F8h
        db  00Bh, 0FFh, 074h, 022h, 08Dh, 077h, 045h, 08Bh, 00Eh, 08Bh, 0C1h, 02Bh, 0C7h, 083h, 0F8h, 045h
        db  074h, 014h, 08Bh, 001h, 089h, 006h, 089h, 070h, 004h, 051h, 0E8h, 039h, 0F7h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 00Bh, 0FFh, 075h, 0E1h, 05Fh, 05Eh, 0C3h
sub_1_3A0F:
        call   sub_1_3120
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x18], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [esp + 0x24]
        test   cl, 1
        je     loc_1_3A64
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 0x45]
        add    edx, 0x45
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        jmp    loc_1_3A83
loc_1_3A64:
        test   cl, 2
        je     loc_1_3A6B
        jmp    loc_1_3A6F
loc_1_3A6B:
        or     ecx, ecx
        jne    loc_1_3A83
loc_1_3A6F:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 0x49]
        add    edx, 0x45
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
loc_1_3A83:
        ret    
sub_1_3A84:
        push   9
        call   sub_1_F390
        mov    cl, byte ptr [esp + 0xc]
        add    esp, 4
        mov    byte ptr [eax + 4], cl
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 5], ecx
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0x2c]
        mov    dword ptr [ecx + 0x2c], eax
        mov    dword ptr [eax], edx
        dec    dword ptr [ecx + 0x10]
        ret    
sub_1_3AAC:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x2c]
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_1_3AC1
        mov    dword ptr [eax], 0
loc_1_3AC1:
        or     ecx, ecx
        je     loc_1_3AD4
        mov    al, byte ptr [esp + 8]
loc_1_3AC9:
        cmp    byte ptr [ecx + 4], al
        je     loc_1_3ADA
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_3AC9
loc_1_3AD4:
        mov    eax, 1
loc_1_3AD9:
        ret    
loc_1_3ADA:
        sub    eax, eax
        jmp    loc_1_3AD9
sub_1_3ADE:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_129F8
        imul   eax, eax, 0xd
        mov    ecx, 0xe1
        sub    edx, edx
        div    ecx
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [edx + 1]
        mov    ebx, edi
        cmp    edi, ecx
        ja     loc_1_3B31
loc_1_3B02:
        cmp    ebx, 1
        jb     loc_1_3B22
        cmp    ebx, 0xe1
        ja     loc_1_3B22
        lea    eax, [esp + 0xc]
        push   eax
        push   ebx
        push   esi
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_3B2D
loc_1_3B22:
        inc    ebx
        cmp    ebx, 0xe1
        jbe    loc_1_3B02
        jmp    loc_1_3B31
loc_1_3B2D:
        mov    eax, ebx
        jmp    loc_1_3B61
loc_1_3B31:
        mov    ebx, 1
        cmp    edi, ebx
        jbe    loc_1_3B5F
loc_1_3B3A:
        cmp    ebx, 1
        jb     loc_1_3B5A
        cmp    ebx, 0xe1
        ja     loc_1_3B5A
        lea    eax, [esp + 0xc]
        push   eax
        push   ebx
        push   esi
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_3B68
loc_1_3B5A:
        inc    ebx
        cmp    ebx, edi
        jb     loc_1_3B3A
loc_1_3B5F:
        sub    eax, eax
loc_1_3B61:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_3B68:
        mov    eax, ebx
        jmp    loc_1_3B61
sub_1_3B6C:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        mov    bl, byte ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    edx, eax
        mov    esi, edx
        or     edx, edx
        je     loc_1_3B8E
loc_1_3B81:
        cmp    byte ptr [esi + 4], bl
        je     loc_1_3B8E
        mov    edx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_3B81
loc_1_3B8E:
        or     esi, esi
        je     loc_1_3BB2
        cmp    byte ptr [esi + 4], bl
        jne    loc_1_3BB2
        inc    dword ptr [ecx + 0x10]
        cmp    edx, esi
        jne    loc_1_3BA5
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 0x2c], eax
        jmp    loc_1_3BA9
loc_1_3BA5:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
loc_1_3BA9:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_3BB2:
        pop    esi
        pop    ebx
        ret    
sub_1_3BB5:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        sub    edi, edi
        lea    esi, [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    eax, edi
        jle    loc_1_3BDE
        mov    ecx, 0x15
loc_1_3BCB:
        cmp    word ptr [esi + 2], cx
        je     loc_1_3BE3
        movzx  edx, word ptr [esi]
        add    edx, 4
        add    esi, edx
        inc    edi
        cmp    edi, eax
        jl     loc_1_3BCB
loc_1_3BDE:
        sub    al, al
loc_1_3BE0:
        pop    edi
        pop    esi
        ret    
loc_1_3BE3:
        cmp    word ptr [esi], 0x1a
        jb     loc_1_3BDE
        mov    ecx, dword ptr [esi + 0x14]
        add    esi, 4
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax], ecx
        mov    al, 1
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_3BE0
sub_1_3C0A:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [ecx], 0
        or     eax, eax
        je     loc_1_3C2C
        cmp    byte ptr [eax], 0
        je     loc_1_3C2C
loc_1_3C1E:
        cmp    byte ptr [eax], 1
        jne    loc_1_3C26
        mov    byte ptr [ecx], 1
loc_1_3C26:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_1_3C1E
loc_1_3C2C:
        ret    
loc_1_3C2D:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        jne    loc_1_3C64
        push   0x5273                        ; -> loc_1_5273
        push   0x12
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     loc_1_3C5A
        mov    eax, 0x1b
        jmp    loc_1_3D9C
loc_1_3C5A:
        mov    dword ptr [0x13654], 1        ; -> loc_1_13654
loc_1_3C64:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x40]
        call   sub_1_3BB5
        add    esp, 0x10
        or     al, al
        jne    loc_1_3C8D
        mov    eax, 3
        jmp    loc_1_3D9C
loc_1_3C8D:
        lea    eax, [esp + 0x13]
        push   eax
        push   dword ptr [esp + 0x40]
        call   sub_1_3C0A
        add    esp, 8
        push   0x8e
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_3CBE
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 0x23
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
loc_1_3CBE:
        mov    ebx, dword ptr [esp + 0x38]
        push   0
        push   ebx
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_337D
        add    esp, 0x10
        cmp    dword ptr [esp + 0x14], -1
        je     loc_1_3CFF
        or     eax, eax
        je     loc_1_3D19
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_3D19
        or     ebp, ebp
        je     loc_1_3CF5
        push   ebp
        call   sub_1_F396
        add    esp, 4
loc_1_3CF5:
        mov    eax, 2
        jmp    loc_1_3D9C
loc_1_3CFF:
        push   dword ptr [esp + 0x14]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_3D97
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp + 0x3c], eax
loc_1_3D19:
        or     ebp, ebp
        je     loc_1_3D90
        lea    eax, [ebp + 0x10]
        mov    dword ptr [eax], eax
        mov    dword ptr [ebp + 0x14], eax
        lea    eax, [ebp + 0x20]
        lea    esi, [esp + 0x1c]
        mov    dword ptr [eax], eax
        mov    dword ptr [ebp + 0x24], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x38], eax
        lea    edi, [ebp + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        cmp    ebx, 0x51
        mov    byte ptr [ebp + 0x42], bl
        jne    loc_1_3D52
        mov    eax, dword ptr [0x13758]      ; -> loc_1_13758
        inc    eax
        mov    dword ptr [0x13758], eax      ; -> loc_1_13758
loc_1_3D52:
        mov    word ptr [ebp + 0x40], 0x80
        mov    word ptr [ebp + 0x40], 0x88
        push   ebp
        call   sub_1_317E
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        push   ebp
        jne    loc_1_3D7E
        call   sub_1_F396
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_3D9C
loc_1_3D7E:
        call   sub_1_3559
        mov    eax, dword ptr [esp + 0x34]
        add    esp, 4
        mov    dword ptr [eax], esi
        sub    eax, eax
        jmp    loc_1_3D9C
loc_1_3D90:
        mov    eax, 1
        jmp    loc_1_3D9C
loc_1_3D97:
        mov    eax, 4
loc_1_3D9C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_3DA4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        sub    esi, esi
        cmp    byte ptr [edi + 0x42], 0x51
        mov    eax, dword ptr [0x13758]      ; -> loc_1_13758
        jne    loc_1_3DC1
        cmp    eax, esi
        je     loc_1_3DC1
        dec    eax
loc_1_3DC1:
        mov    dword ptr [0x13758], eax      ; -> loc_1_13758
        or     eax, eax
        jne    loc_1_3E38
        push   dword ptr [edi + 0x38]
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    loc_1_3E38
        mov    ebx, dword ptr [edi + 0x38]
        cmp    ebx, -1
        jne    loc_1_3DE4
        mov    ebx, dword ptr [edi + 0x3c]
loc_1_3DE4:
        push   ebx
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_3E01
        cmp    byte ptr [eax + 0x1a], 1
        jne    loc_1_3E01
        push   ebx
        push   0x12
        call   sub_1_EA12
        jmp    loc_1_3E35
loc_1_3E01:
        push   ebx
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        je     loc_1_3E2C
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_3E2C
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_10879
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_3E38
loc_1_3E2C:
        push   1
        push   0x12
        call   sub_1_F02D
loc_1_3E35:
        add    esp, 8
loc_1_3E38:
        test   byte ptr [edi + 0x40], 0x20
        je     loc_1_3E49
        mov    ecx, dword ptr [edi + 0x18]
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
loc_1_3E49:
        mov    eax, dword ptr [0x1377c]      ; -> loc_1_1377C
        movzx  ecx, byte ptr [edi + 0x43]
        mov    dword ptr [eax + ecx*4], 0
        push   edi
        call   sub_1_3594
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x4a]
        or     ebx, ebx
        je     loc_1_3EAC
        sub    eax, eax
        mov    ecx, 1
loc_1_3E70:
        mov    edx, dword ptr [ebx]
        cmp    edx, eax
        je     loc_1_3EA0
        cmp    byte ptr [edx + 0xbc], al
        je     loc_1_3E94
        test   byte ptr [edx + 0xbe], cl
        jne    loc_1_3E94
        inc    dword ptr [edx + 0xa6]
        mov    dword ptr [edx + 0xfe], esi
        mov    esi, edx
loc_1_3E94:
        mov    dword ptr [edx + 0x102], eax
        mov    dword ptr [edx + 0x112], eax
loc_1_3EA0:
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebx + 8]
        cmp    ebx, eax
        jne    loc_1_3E70
loc_1_3EAC:
        mov    eax, dword ptr [edi + 0x20]
        lea    ebx, [edi + 0x20]
        sub    eax, edi
        cmp    eax, 0x20
        je     loc_1_3EE2
loc_1_3EB9:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   7
        push   dword ptr [ebp + 0x2c]
        call   dword ptr [ebp + 0x28]
        add    esp, 0xc
        push   ebp
        call   sub_1_BACB
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 0x20
        jne    loc_1_3EB9
loc_1_3EE2:
        mov    eax, dword ptr [edi + 0x10]
        lea    ebx, [edi + 0x10]
        sub    eax, edi
        cmp    eax, 0x10
        je     loc_1_3F28
loc_1_3EEF:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   7
        push   dword ptr [ebp + 0x2c]
        call   dword ptr [ebp + 0x28]
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x24]
        or     eax, eax
        je     loc_1_3F16
        push   eax
        call   sub_1_BA30
        add    esp, 4
loc_1_3F16:
        push   ebp
        call   sub_1_BA9B
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 0x10
        jne    loc_1_3EEF
loc_1_3F28:
        push   edi
        call   sub_1_F396
        add    esp, 4
        or     esi, esi
        je     loc_1_3F59
loc_1_3F35:
        dec    dword ptr [esi + 0xa6]
        or     dword ptr [esi + 0x60], 0x40000000
        mov    edi, dword ptr [esi + 0xfe]
        push   0x40
        push   esi
        call   sub_1_4B72
        add    esp, 8
        mov    esi, edi
        or     edi, edi
        jne    loc_1_3F35
loc_1_3F59:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_3F61:
        mov    eax, dword ptr [0x13878]      ; -> loc_1_13878
        or     eax, eax
        je     loc_1_3F73
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [0x13878], ecx      ; -> loc_1_13878
loc_1_3F73:
        ret    
sub_1_3F74:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x13878]      ; -> loc_1_13878
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [0x13878], eax      ; -> loc_1_13878
        ret    
loc_1_3F87:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        push   dword ptr [ebx]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_3FA3
        jmp    loc_1_4042
loc_1_3FA3:
        mov    ax, word ptr [esi + 0x40]
        test   al, 0x40
        jne    loc_1_3FCF
        cmp    word ptr [esi + 0x46], 0
        jne    loc_1_3FCF
        or     ax, 0x40
        mov    word ptr [esi + 0x40], ax
        and    ax, 0xff7f
        mov    word ptr [esi + 0x40], ax
        push   esi
        call   sub_1_3DA4
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_4047
loc_1_3FCF:
        test   al, 1
        jne    loc_1_4042
        mov    edi, dword ptr [esi + 0xc]
        call   sub_1_3F61
        or     eax, eax
        je     loc_1_403B
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 0x14], ecx
        mov    cx, word ptr [esi + 0x40]
        and    cx, 0xfffd
        mov    word ptr [esi + 0x40], cx
        or     cx, 1
        mov    word ptr [esi + 0x40], cx
        and    cx, 0xff7f
        mov    word ptr [esi + 0x40], cx
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [eax], 0
        or     edi, edi
        je     loc_1_4034
loc_1_4016:
        push   0
        push   7
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_3F74
        add    esp, 4
        or     edi, edi
        jne    loc_1_4016
loc_1_4034:
        mov    eax, 0xff
        jmp    loc_1_4047
loc_1_403B:
        mov    eax, 1
        jmp    loc_1_4047
loc_1_4042:
        mov    eax, 6
loc_1_4047:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_404B:
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_4059
        mov    eax, 0xb
        jmp    loc_1_409B
loc_1_4059:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 8]
        dec    ecx
        cmp    ecx, 6
        ja     loc_1_406C
        jmp    dword ptr [ecx*4 + 0x409c]    ; -> obj1:0x409C
loc_1_406C:
        mov    eax, 0xb
        jmp    loc_1_409B
loc_1_4073:
        mov    eax, 0xb
        jmp    loc_1_409B
loc_1_407A:
        mov    eax, 0xb
        jmp    loc_1_409B
loc_1_4081:
        mov    ecx, dword ptr [esp + 0x10]
        cmp    byte ptr [ecx], 0
        mov    ecx, dword ptr [esp + 4]
        je     loc_1_4095
        or     word ptr [ecx + 0x40], 8
        jmp    loc_1_409B
loc_1_4095:
        and    word ptr [ecx + 0x40], 0xfff7
loc_1_409B:
        ret    
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_4073
        dd  loc_1_407A
        dd  loc_1_407A
        dd  loc_1_4081
        dd  loc_1_4073
        dd  loc_1_407A
        dd  loc_1_407A
sub_1_40B8:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   dword ptr [edi]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_40D3
        jmp    loc_1_4178
loc_1_40D3:
        mov    ax, word ptr [esi + 0x40]
        test   al, 0x80
        je     loc_1_4178
        test   al, 0x40
        jne    loc_1_412A
        cmp    word ptr [esi + 0x46], 0
        jne    loc_1_412A
        or     ax, 0x40
        push   dword ptr [esp + 0x18]
        mov    word ptr [esi + 0x40], ax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_1_404B
        add    esp, 0x10
        mov    edi, eax
        mov    ax, word ptr [esi + 0x40]
        test   al, 7
        jne    loc_1_411D
        and    ax, 0xffbf
        mov    word ptr [esi + 0x40], ax
        mov    eax, edi
        jmp    loc_1_417D
loc_1_411D:
        push   esi
        call   sub_1_423D
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_417D
loc_1_412A:
        call   sub_1_3F61
        or     eax, eax
        je     loc_1_4171
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esp + 0x10]
        add    esi, 0xc
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0xc], ecx
        or     word ptr [esi + 0x34], 2
        cmp    dword ptr [esi], 0
        je     loc_1_4168
loc_1_4161:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_1_4161
loc_1_4168:
        mov    dword ptr [esi], eax
        mov    eax, 0xff
        jmp    loc_1_417D
loc_1_4171:
        mov    eax, 1
        jmp    loc_1_417D
loc_1_4178:
        mov    eax, 6
loc_1_417D:
        pop    edi
        pop    esi
        ret    
loc_1_4180:
        push   dword ptr [esp + 4]
        call   sub_1_3150
        add    esp, 4
        or     eax, eax
        je     loc_1_421F
        test   byte ptr [eax + 0x40], 0x80
        je     loc_1_421F
        sub    edx, edx
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 5
        ja     loc_1_41B0
        jmp    dword ptr [ecx*4 + 0x4225]    ; -> obj1:0x4225
loc_1_41B0:
        mov    edx, 0xa
        jmp    loc_1_421B
loc_1_41B7:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x5a], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x5e], ecx
        jmp    loc_1_421B
loc_1_41C7:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x62], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x66], ecx
        jmp    loc_1_421B
loc_1_41D7:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x6a], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x6e], ecx
        jmp    loc_1_421B
loc_1_41E7:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x72], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x76], ecx
        jmp    loc_1_421B
loc_1_41F7:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x7a], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x7e], ecx
        jmp    loc_1_421B
loc_1_4207:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x82], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x86], ecx
loc_1_421B:
        mov    eax, edx
        jmp    loc_1_4224
loc_1_421F:
        mov    eax, 6
loc_1_4224:
        ret    
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_41B7
        dd  loc_1_41C7
        dd  loc_1_41D7
        dd  loc_1_41E7
        dd  loc_1_41F7
        dd  loc_1_4207
sub_1_423D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [esi + 0x40], 7
        je     loc_1_42FB
loc_1_424D:
        mov    edi, dword ptr [esi + 0xc]
        mov    ax, word ptr [esi + 0x40]
        test   al, 1
        jne    loc_1_4304
        test   al, 2
        je     loc_1_429F
loc_1_4260:
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        jne    loc_1_4272
        and    word ptr [esi + 0x40], 0xfffd
loc_1_4272:
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_404B
        add    esp, 0x10
        push   0
        push   eax
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        push   edi
        call   sub_1_3F74
        add    esp, 4
        test   byte ptr [esi + 0x40], 2
        jne    loc_1_4260
loc_1_429F:
        mov    dx, word ptr [esi + 0x40]
        test   dl, 4
        je     loc_1_42F1
        mov    edi, dword ptr [esi + 0x10]
        lea    ecx, [esi + 0x10]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x10
        je     loc_1_42E8
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        inc    word ptr [esi + 0x46]
        and    dx, 0xffbf
        mov    word ptr [esi + 0x40], dx
        push   edi
        push   esi
        call   sub_1_BCEE
        add    esp, 8
        mov    ax, word ptr [esi + 0x46]
        dec    ax
        mov    word ptr [esi + 0x46], ax
        jne    loc_1_42FB
        or     word ptr [esi + 0x40], 0x40
        jmp    loc_1_42F1
loc_1_42E8:
        and    dx, 0xfffb
        mov    word ptr [esi + 0x40], dx
loc_1_42F1:
        test   byte ptr [esi + 0x40], 7
        jne    loc_1_424D
loc_1_42FB:
        and    word ptr [esi + 0x40], 0xffbf
loc_1_4301:
        pop    edi
        pop    esi
        ret    
loc_1_4304:
        push   esi
        call   sub_1_3DA4
        add    esp, 4
        push   0
        push   0
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        push   edi
        call   sub_1_3F74
        add    esp, 4
        jmp    loc_1_4301
sub_1_4325:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x46]
        dec    ax
        mov    word ptr [ecx + 0x46], ax
        jne    loc_1_4349
        mov    ax, word ptr [ecx + 0x40]
        test   al, 0x40
        jne    loc_1_4349
        test   al, 7
        je     loc_1_4349
        or     ax, 0x40
        mov    word ptr [ecx + 0x40], ax
loc_1_4349:
        ret    
sub_1_434A:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x46]
        dec    ax
        mov    word ptr [ecx + 0x46], ax
        jne    loc_1_4377
        mov    ax, word ptr [ecx + 0x40]
        test   al, 0x40
        jne    loc_1_4377
        test   al, 7
        je     loc_1_4377
        or     ax, 0x40
        mov    word ptr [ecx + 0x40], ax
        push   ecx
        call   sub_1_423D
        add    esp, 4
loc_1_4377:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_4380:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, 0x3c
        sub    ebp, ebp
        sub    esi, esi
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, ecx
        shl    eax, 0x10
        mov    edx, ecx
        shr    edx, 0x10
        or     eax, edx
        add    eax, ecx
        shr    eax, 0x10
        mov    edi, dword ptr [esp + 0x18]
loc_1_43A6:
        test   ebp, 1
        je     loc_1_43C3
        mov    ecx, eax
        and    ecx, 0xff
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
        xor    esi, 1
loc_1_43C3:
        mov    ecx, dword ptr [edi + 4]
        add    ecx, ebx
        push   dword ptr [edi + 0xc]
        push   ecx
        push   eax
        call   sub_1_F151
        add    esp, 0xc
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0xc]
        mov    edi, dword ptr [edi]
        cmp    edi, ebx
        jne    loc_1_43A6
        cmp    esi, ebx
        je     loc_1_43F6
        mov    ecx, eax
        and    ecx, 0xff
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
loc_1_43F6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  056h, 08Bh, 04Ch, 024h, 008h, 08Bh, 0C1h, 0C1h, 0E0h, 010h, 08Bh, 0D1h, 0C1h, 0EAh, 010h, 00Bh
        db  0C2h, 003h, 0C1h, 0C1h, 0E8h, 010h, 08Bh, 074h, 024h, 00Ch, 0FFh, 076h, 00Ch, 0FFh, 076h, 008h
        db  050h, 0E8h, 030h, 0ADh, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 036h, 00Bh, 0F6h, 075h, 0EBh, 05Eh
        db  0C3h
sub_1_442C:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ebx, dword ptr [esp + 0x24]
        cmp    ebx, eax
        je     loc_1_44D9
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [ebx + 0xc]
        mov    edx, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], ecx
        mov    ecx, dword ptr [edx + 0xc]
        sub    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x18], ecx
loc_1_445B:
        mov    ebp, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ebp, ecx
        jb     loc_1_4469
        mov    ebp, ecx
loc_1_4469:
        mov    ecx, dword ptr [esp + 0x28]
        cmp    ebp, ecx
        jb     loc_1_4473
        mov    ebp, ecx
loc_1_4473:
        mov    esi, dword ptr [edx + 8]
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [esp + 0x10]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, dword ptr [esp + 0x2c]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    eax, ebp
        sub    dword ptr [esp + 0x18], ebp
        jne    loc_1_44AE
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_1_44D9
        mov    dword ptr [esp + 0x2c], 0
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x18], ecx
        jmp    loc_1_44B2
loc_1_44AE:
        add    dword ptr [esp + 0x2c], ebp
loc_1_44B2:
        sub    dword ptr [esp + 0x14], ebp
        jne    loc_1_44CF
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_1_44D9
        mov    dword ptr [esp + 0x10], 0
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x14], ecx
        jmp    loc_1_44D3
loc_1_44CF:
        add    dword ptr [esp + 0x10], ebp
loc_1_44D3:
        sub    dword ptr [esp + 0x28], ebp
        jne    loc_1_445B
loc_1_44D9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_44E1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [esp + 0x20]
        cmp    dword ptr [eax + 0xc], edx
        ja     loc_1_44FC
loc_1_44F2:
        sub    edx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 0xc], edx
        jbe    loc_1_44F2
loc_1_44FC:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_453B
loc_1_4503:
        mov    ebp, dword ptr [eax + 0xc]
        sub    ebp, edx
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    ebx, ebp
        jb     loc_1_4512
        mov    ebx, ebp
loc_1_4512:
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [esp + 0x14]
        add    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    ebp, ebx
        jne    loc_1_4531
        mov    eax, dword ptr [eax]
        sub    edx, edx
loc_1_4531:
        add    dword ptr [esp + 0x14], ebx
        sub    dword ptr [esp + 0x1c], ebx
        jne    loc_1_4503
loc_1_453B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4540:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, dword ptr [ecx]
        mov    edx, dword ptr [eax + 4]
        add    edx, esi
        mov    ecx, dword ptr [eax + 0xc]
        sub    ecx, esi
        mov    dword ptr [esp + 0x10], ecx
        mov    ebx, dword ptr [esp + 0x20]
loc_1_4563:
        mov    ebp, dword ptr [esp + 0x10]
        cmp    ebp, ebx
        jb     loc_1_456D
        mov    ebp, ebx
loc_1_456D:
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, ebp
        add    dword ptr [esp + 0x1c], ebp
        sub    ebx, ebp
        je     loc_1_459F
        sub    dword ptr [esp + 0x10], ebp
        jne    loc_1_4563
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x10], ecx
        jmp    loc_1_4563
loc_1_459F:
        mov    ecx, dword ptr [esp + 0x24]
        sub    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_45B0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x20]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x10], eax
loc_1_45C9:
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, ebx
        jb     loc_1_45D3
        mov    eax, ebx
loc_1_45D3:
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [esp + 0x18], eax
        sub    ebx, eax
        je     loc_1_4607
        sub    dword ptr [esp + 0x10], eax
        jne    loc_1_4603
        mov    edx, dword ptr [edx]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x10], eax
        jmp    loc_1_45C9
loc_1_4603:
        add    ebp, eax
        jmp    loc_1_45C9
loc_1_4607:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_460F:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edi], 1
        mov    word ptr [edi + 6], 0x15
        add    edi, 4
        mov    esi, dword ptr [esp + 0x10]
        mov    word ptr [edi], 0x1a
        mov    dword ptr [edi + 0x14], eax
        add    edi, 4
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_1_463E:
        push   esi
        sub    esi, esi
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, esi
        je     loc_1_467F
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, esi
        je     loc_1_467F
        cmp    eax, 0x22
        jb     loc_1_4673
        push   dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x10], 0x22
        push   dword ptr [esp + 0x10]
        push   dword ptr [ecx + 0x14]
        call   sub_1_460F
        add    esp, 0xc
        jmp    loc_1_467F
loc_1_4673:
        mov    dword ptr [ecx + 0x10], 0
        mov    esi, 0x1a
loc_1_467F:
        mov    eax, esi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4690:
        ret    
loc_1_4691:
        ret    
sub_1_4692:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_46B0
        push   esi
        push   eax
        call   sub_1_4C3F
        add    esp, 8
loc_1_46B0:
        push   esi
        call   sub_1_F396
        add    esp, 4
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_46C7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        sub    ebp, ebp
        mov    edi, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x1c]
        test   byte ptr [esi + 0xd], 1
        mov    word ptr [esi + 0xe], 1
        jne    loc_1_46FA
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebp
        je     loc_1_46F4
        push   esi
        push   eax
        call   sub_1_4C3F
        add    esp, 8
loc_1_46F4:
        and    byte ptr [esi + 0xd], 0xfd
        jmp    loc_1_46FF
loc_1_46FA:
        mov    ebp, 1
loc_1_46FF:
        mov    word ptr [esi + 0xe], 0
        or     ebp, ebp
        je     loc_1_4714
        push   esi
        call   sub_1_4692
        add    esp, 4
        jmp    loc_1_471E
loc_1_4714:
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
loc_1_471E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4723:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x1388c]      ; -> loc_1_1388C
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x1388c], ecx      ; -> loc_1_1388C
        ret    
sub_1_4735:
        mov    eax, dword ptr [0x1388c]      ; -> loc_1_1388C
        or     eax, eax
        je     loc_1_4748
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x1388c], ecx      ; -> loc_1_1388C
        jmp    loc_1_4776
loc_1_4748:
        mov    ecx, dword ptr [0x13660]      ; -> loc_1_13660
        cmp    dword ptr [0x12abc], ecx      ; -> obj1:0x12ABC
        mov    eax, 0
        jbe    loc_1_476B
        push   0x1c
        call   sub_1_F390
        add    esp, 4
        mov    ecx, dword ptr [0x13660]      ; -> loc_1_13660
loc_1_476B:
        or     eax, eax
        je     loc_1_4776
        inc    ecx
        mov    dword ptr [0x13660], ecx      ; -> loc_1_13660
loc_1_4776:
        ret    
sub_1_4777:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, ecx
        and    edx, 0xffffff
        cmp    dword ptr [0x137bc], edx      ; -> loc_1_137BC
        jbe    loc_1_479F
        mov    eax, dword ptr [0x12ac0]      ; -> obj1:0x12AC0
        mov    eax, dword ptr [eax + edx*4]
        or     eax, eax
        je     loc_1_47A1
        shr    ecx, 0x18
        cmp    cl, byte ptr [eax + 0xc]
        je     loc_1_47A1
loc_1_479F:
        sub    eax, eax
loc_1_47A1:
        ret    
sub_1_47A2:
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_1_47A6:
        mov    edx, dword ptr [0x12ac0]      ; -> obj1:0x12AC0
        mov    ebx, dword ptr [0x137bc]      ; -> loc_1_137BC
loc_1_47B2:
        mov    esi, dword ptr [0x1385c]      ; -> loc_1_1385C
        sub    eax, eax
        cmp    ebx, eax
        je     loc_1_47D0
loc_1_47BE:
        cmp    ebx, esi
        jne    loc_1_47C4
        sub    esi, esi
loc_1_47C4:
        cmp    dword ptr [edx + esi*4], 0
        je     loc_1_47D0
        inc    esi
        inc    eax
        cmp    ebx, eax
        ja     loc_1_47BE
loc_1_47D0:
        mov    dword ptr [0x12ac0], edx      ; -> obj1:0x12AC0
        mov    dword ptr [0x137bc], ebx      ; -> loc_1_137BC
        cmp    ebx, eax
        ja     loc_1_485E
        cmp    ebx, dword ptr [0x12b0c]      ; -> sub_1_12B0C
        je     loc_1_488F
        add    ebx, 0x10
        mov    eax, dword ptr [0x12b0c]      ; -> sub_1_12B0C
        cmp    ebx, eax
        jb     loc_1_47FA
        mov    ebx, eax
loc_1_47FA:
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        mov    ebp, dword ptr [0x12ac0]      ; -> obj1:0x12AC0
        mov    esi, dword ptr [0x137bc]      ; -> loc_1_137BC
        je     loc_1_488F
        lea    ecx, [ebx*4]
        mov    edi, edx
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        lea    ecx, [esi*4]
        mov    edi, edx
        mov    esi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x12ac0], edx      ; -> obj1:0x12AC0
        mov    dword ptr [0x137bc], ebx      ; -> loc_1_137BC
        cmp    ebp, eax
        je     loc_1_47B2
        push   ebp
        call   sub_1_F396
        add    esp, 4
        jmp    loc_1_47A6
loc_1_485E:
        mov    dword ptr [0x12ac0], edx      ; -> obj1:0x12AC0
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edx + esi*4], ecx
        lea    eax, [esi + 1]
        mov    dword ptr [0x1385c], eax      ; -> loc_1_1385C
        mov    al, byte ptr [0x13868]        ; -> loc_1_13868
        inc    al
        mov    byte ptr [0x13868], al        ; -> loc_1_13868
        mov    byte ptr [ecx + 0xc], al
        movzx  eax, al
        shl    eax, 0x18
        or     eax, esi
        mov    dword ptr [ecx + 0x10], eax
        jmp    loc_1_4894
loc_1_488F:
        mov    eax, 0xffffffff
loc_1_4894:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4899:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x12ac0]      ; -> obj1:0x12AC0
        and    ecx, 0xffffff
        mov    dword ptr [eax + ecx*4], 0
        ret    
sub_1_48B0:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x2afa
        je     loc_1_48C8
        cmp    eax, 0x2afb
        je     loc_1_48CF
        mov    eax, dword ptr [esp + 8]
        jmp    loc_1_48D4
loc_1_48C8:
        mov    eax, 0x1c
        jmp    loc_1_48D4
loc_1_48CF:
        mov    eax, 0x1d
loc_1_48D4:
        ret    
sub_1_48D5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x102]
        or     ecx, ecx
        je     loc_1_48ED
        cmp    dword ptr [ecx], eax
        jne    loc_1_48ED
        mov    dword ptr [ecx], 0
loc_1_48ED:
        ret    
sub_1_48EE:
        push   dword ptr [esp + 4]
        call   sub_1_48D5
        add    esp, 4
        ret    
sub_1_48FB:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0x102]
        cmp    esi, edx
        je     loc_1_4925
        mov    ax, word ptr [esi + 0xe]
        dec    ax
        mov    word ptr [esi + 0xe], ax
        jne    loc_1_491B
        mov    edx, dword ptr [esi + 0x20]
loc_1_491B:
        mov    dword ptr [ecx + 0x102], 0
loc_1_4925:
        or     edx, edx
        je     loc_1_492F
        push   esi
        call   edx
        add    esp, 4
loc_1_492F:
        pop    esi
        ret    
sub_1_4931:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    bl, byte ptr [edi + 0xbc]
        or     bl, bl
        je     loc_1_4B6D
        mov    byte ptr [edi + 0xbc], 0
        push   edi
        call   sub_1_48D5
        add    esp, 4
        push   edi
        call   sub_1_CD4E
        add    esp, 4
        cmp    bl, 3
        jbe    loc_1_4979
        cmp    bl, 9
        jae    loc_1_4979
        test   byte ptr [edi + 0x63], 0x40
        je     loc_1_4979
        push   edi
        call   sub_1_AD7A
        add    esp, 4
loc_1_4979:
        mov    al, byte ptr [edi + 0x10e]
        test   al, 0x80
        je     loc_1_498A
        mov    ebx, 0x14
        jmp    loc_1_49C3
loc_1_498A:
        test   al, 0x40
        je     loc_1_4995
        mov    ebx, 0x13
        jmp    loc_1_49C3
loc_1_4995:
        test   al, 0x20
        je     loc_1_499D
        push   0x15
        jmp    loc_1_49B3
loc_1_499D:
        test   al, 0x10
        je     loc_1_49A8
        mov    ebx, 0xe
        jmp    loc_1_49C3
loc_1_49A8:
        test   al, 8
        mov    ebx, 0
        je     loc_1_49C3
        push   0x1c
loc_1_49B3:
        push   dword ptr [edi + 0xc0]
        call   sub_1_48B0
        add    esp, 8
        mov    ebx, eax
loc_1_49C3:
        push   dword ptr [edi + 0x8c]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_49E5
        mov    cl, byte ptr [edi + 0x98]
        push   ecx
        push   eax
        call   sub_1_3B6C
        add    esp, 8
loc_1_49E5:
        mov    esi, dword ptr [edi + 0xea]
        or     esi, esi
        je     loc_1_4A04
        push   0
        push   ebx
        push   dword ptr [esi + 0xc]
        call   dword ptr [esi + 8]
        add    esp, 0xc
        push   esi
        call   sub_1_4723
        add    esp, 4
loc_1_4A04:
        mov    esi, dword ptr [edi + 0x11a]
        or     esi, esi
        je     loc_1_4A23
        push   0
        push   ebx
        push   dword ptr [esi + 0xc]
        call   dword ptr [esi + 8]
        add    esp, 0xc
        push   esi
        call   sub_1_4723
        add    esp, 4
loc_1_4A23:
        mov    eax, dword ptr [edi + 0x40]
        lea    ebp, [edi + 0x40]
        sub    eax, edi
        cmp    eax, 0x40
        je     loc_1_4A92
loc_1_4A30:
        mov    esi, dword ptr [ebp]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    ecx, 0
        mov    eax, dword ptr [esi + 0x18]
        dec    eax
        mov    dword ptr [esi + 0x18], eax
        or     eax, eax
        setl   cl
        or     eax, eax
        mov    eax, 0
        setle  al
        add    eax, ecx
        je     loc_1_4A85
        mov    eax, dword ptr [esi + 0x2c]
        or     eax, eax
        je     loc_1_4A6E
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x2c], 0
loc_1_4A6E:
        push   0
        push   ebx
        push   dword ptr [esi + 0xc]
        call   dword ptr [esi + 8]
        add    esp, 0xc
        push   esi
        call   sub_1_A7D4
        add    esp, 4
        jmp    loc_1_4A88
loc_1_4A85:
        mov    dword ptr [esi + 0x10], ebx
loc_1_4A88:
        mov    eax, dword ptr [ebp]
        sub    eax, edi
        cmp    eax, 0x40
        jne    loc_1_4A30
loc_1_4A92:
        cmp    dword ptr [edi + 0xd6], 0
        je     loc_1_4AC7
loc_1_4A9B:
        mov    esi, dword ptr [edi + 0xd6]
        push   0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0xd6], eax
        push   ebx
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0xc
        push   esi
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [edi + 0xd6], 0
        jne    loc_1_4A9B
loc_1_4AC7:
        cmp    dword ptr [edi + 0x11e], 0
        je     loc_1_4AFC
loc_1_4AD0:
        mov    esi, dword ptr [edi + 0x11e]
        push   0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0x11e], eax
        push   ebx
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0xc
        push   esi
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [edi + 0x11e], 0
        jne    loc_1_4AD0
loc_1_4AFC:
        mov    eax, dword ptr [edi + 0xe2]
        or     eax, eax
        je     loc_1_4B0F
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B0F:
        mov    eax, dword ptr [edi + 0x122]
        or     eax, eax
        je     loc_1_4B22
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B22:
        cmp    dword ptr [edi + 0xd2], 0
        je     loc_1_4B5B
loc_1_4B2B:
        mov    esi, dword ptr [edi + 0xd2]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0xd2], eax
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_4B49
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B49:
        push   esi
        call   sub_1_F396
        add    esp, 4
        cmp    dword ptr [edi + 0xd2], 0
        jne    loc_1_4B2B
loc_1_4B5B:
        push   edi
        call   sub_1_48FB
        add    esp, 4
        push   edi
        call   sub_1_CF22
        add    esp, 4
loc_1_4B6D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4B72:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [esp + 8]
        or     byte ptr [eax + 0x10e], cl
        mov    cl, byte ptr [eax + 0xbe]
        test   cl, 1
        jne    loc_1_4BB0
        or     cl, 1
        mov    byte ptr [eax + 0xbe], cl
        inc    byte ptr [eax + 0xba]
        or     dword ptr [eax + 0x64], 1
        cmp    dword ptr [eax + 0xa6], 0
        jne    loc_1_4BB0
        push   eax
        call   sub_1_4931
        add    esp, 4
loc_1_4BB0:
        ret    
sub_1_4BB1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xa6]
        dec    eax
        mov    dword ptr [ecx + 0xa6], eax
        jne    loc_1_4BDB
        mov    al, byte ptr [ecx + 0xbe]
        or     al, al
        je     loc_1_4BDB
        test   al, 1
        je     loc_1_4BDB
        push   ecx
        call   sub_1_4931
        add    esp, 4
loc_1_4BDB:
        ret    
loc_1_4BDC:
        push   esi
        push   edi
        push   0x28
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_4C37
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x14], eax
        push   esi
        call   sub_1_47A2
        add    esp, 4
        cmp    eax, -1
        je     loc_1_4C2E
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        mov    word ptr [esi + 0xe], 0
        mov    byte ptr [esi + 0xd], 0
        sub    eax, eax
        mov    dword ptr [esi + 0x20], 0x4690 ; -> loc_1_4690
        jmp    loc_1_4C3C
loc_1_4C2E:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_4C37:
        mov    eax, 1
loc_1_4C3C:
        pop    edi
        pop    esi
        ret    
sub_1_4C3F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 0x42
        cmp    dword ptr [esi + 8], 0
        je     loc_1_4C70
        mov    edx, dword ptr [esp + 0xc]
        sub    ecx, ecx
loc_1_4C53:
        mov    eax, dword ptr [esi + 8]
        cmp    edx, eax
        je     loc_1_4C63
        mov    esi, eax
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_4C53
        jmp    loc_1_4C70
loc_1_4C63:
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [edx + 4], 0
loc_1_4C70:
        pop    esi
        ret    
loc_1_4C72:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [edi]
        push   ebx
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_4D41
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 0x38]
        cmp    ecx, -1
        jne    loc_1_4C9C
        mov    ecx, dword ptr [eax + 0x3c]
loc_1_4C9C:
        push   ecx
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_4CB4
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_4CB4
        dec    ecx
        mov    dword ptr [eax + 0x14], ecx
loc_1_4CB4:
        push   ebx
        call   sub_1_4899
        add    esp, 4
        mov    cl, byte ptr [esi + 0xd]
        or     cl, 1
        mov    byte ptr [esi + 0xd], cl
        cmp    word ptr [esi + 0xe], 0
        je     loc_1_4D2D
        mov    edx, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x18], eax
        test   cl, 2
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x20], 0x4692 ; -> sub_1_4692
        je     loc_1_4CF5
        push   0
        push   0x20
        push   ebx
        call   edx
        add    esp, 0xc
loc_1_4CF5:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_4D26
        cmp    byte ptr [eax + 0xbc], 0
        je     loc_1_4D26
        or     dword ptr [eax + 0x60], 0x40000000
        test   byte ptr [eax + 0xbe], 1
        jne    loc_1_4D1F
        push   0x40
        push   eax
        call   sub_1_4B72
        add    esp, 8
loc_1_4D1F:
        mov    eax, 0xff
        jmp    loc_1_4D46
loc_1_4D26:
        mov    eax, 0xff
        jmp    loc_1_4D46
loc_1_4D2D:
        mov    dword ptr [esi + 0x18], 0x4691 ; -> loc_1_4691
        push   esi
        call   sub_1_4692
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_4D46
loc_1_4D41:
        mov    eax, 0xf
loc_1_4D46:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4D4A:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [esp + 0x10]
        mov    edi, dword ptr [eax]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_4D6B
        mov    eax, 6
        jmp    loc_1_4D9A
loc_1_4D6B:
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_4D95
        cmp    dword ptr [eax + 4], 0
        je     loc_1_4D85
        mov    eax, 0x10
        jmp    loc_1_4D9A
loc_1_4D85:
        mov    dword ptr [eax + 4], esi
        mov    ecx, dword ptr [esi + 0x4a]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [esi + 0x4a], eax
        sub    eax, eax
        jmp    loc_1_4D9A
loc_1_4D95:
        mov    eax, 0xf
loc_1_4D9A:
        pop    edi
        pop    esi
        ret    
loc_1_4D9D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_4DFD
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     loc_1_4DF6
        cmp    dword ptr [eax], 0
        jne    loc_1_4DEF
        cmp    word ptr [eax + 0xe], 0
        jne    loc_1_4DD1
        push   eax
        push   ecx
        call   sub_1_4C3F
        add    esp, 8
        sub    eax, eax
        jmp    loc_1_4E02
loc_1_4DD1:
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0x18], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], 0x46c7 ; -> loc_1_46C7
        or     byte ptr [eax + 0xd], 2
        mov    eax, 0xff
        jmp    loc_1_4E02
loc_1_4DEF:
        mov    eax, 0x12
        jmp    loc_1_4E02
loc_1_4DF6:
        mov    eax, 0x11
        jmp    loc_1_4E02
loc_1_4DFD:
        mov    eax, 0xf
loc_1_4E02:
        pop    esi
        ret    
sub_1_4E04:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        test   byte ptr [edx + 0xd], 3
        je     loc_1_4E1B
        mov    eax, 0xf
        jmp    loc_1_4EB2
loc_1_4E1B:
        cmp    dword ptr [edx], 0
        jne    loc_1_4EAD
        mov    ebx, dword ptr [edx + 4]
        or     ebx, ebx
        je     loc_1_4EA6
        mov    ecx, dword ptr [esp + 0x14]
        lea    esi, [ebx + 0x28]
        lea    edi, [ecx + 0x6c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x38]
        cmp    eax, -1
        jne    loc_1_4E44
        mov    eax, dword ptr [ebx + 0x3c]
loc_1_4E44:
        mov    dword ptr [ecx + 0x8c], eax
        mov    esi, dword ptr [ebx + 0x72]
        mov    dword ptr [ecx + 0x112], esi
        mov    al, byte ptr [ebx + 0x42]
        or     esi, esi
        mov    byte ptr [ecx + 0x99], al
        mov    eax, dword ptr [ebx + 0x76]
        mov    dword ptr [ecx + 0x116], eax
        mov    dword ptr [ecx + 0x68], 0x7cca ; -> loc_1_7CCA
        je     loc_1_4E77
        mov    dword ptr [ecx + 0x68], 0x7fd3 ; -> loc_1_7FD3
loc_1_4E77:
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [ecx + 0xee], eax
        mov    eax, dword ptr [edx + 0x24]
        or     eax, dword ptr [ecx + 0x60]
        test   eax, 0x2000000
        mov    dword ptr [ecx + 0x60], eax
        je     loc_1_4EA2
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [ecx + 0xce], eax
        mov    byte ptr [ecx + 0xbf], 0
loc_1_4EA2:
        sub    eax, eax
        jmp    loc_1_4EB2
loc_1_4EA6:
        mov    eax, 0x11
        jmp    loc_1_4EB2
loc_1_4EAD:
        mov    eax, 0x12
loc_1_4EB2:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4EB6:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x34]
        cmp    dword ptr [esp + 0x3c], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x18], eax
        je     loc_1_513E
        mov    eax, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_513E
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   ecx
        call   sub_1_3BB5
        add    esp, 0x10
        or     al, al
        je     loc_1_513E
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x10], eax
        call   sub_1_4735
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_4F1E
        mov    eax, 1
        jmp    loc_1_5143
loc_1_4F1E:
        call   sub_1_CE5D
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_4F3C
        push   ebp
        call   sub_1_4723
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_5143
loc_1_4F3C:
        mov    eax, dword ptr [esp + 0x38]
        or     eax, eax
        je     loc_1_4F69
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_4F69
        mov    ecx, 0x64
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0xffffff
        jbe    loc_1_4F62
        mov    eax, 0xffffff
        jmp    loc_1_4F63
loc_1_4F62:
        inc    eax
loc_1_4F63:
        mov    word ptr [ebp + 0x1a], ax
        jmp    loc_1_4F6F
loc_1_4F69:
        mov    word ptr [ebp + 0x1a], 0
loc_1_4F6F:
        mov    word ptr [ebp + 0x18], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [ebx + 0x7c]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 0xc], eax
        or     dword ptr [ebx + 0x60], 8
        lea    esi, [esp + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x18]
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_5123
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        jne    loc_1_4FDD
        push   0x5273                        ; -> loc_1_5273
        push   0x12
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     loc_1_4FD3
        mov    eax, 0x20
        jmp    loc_1_5143
loc_1_4FD3:
        mov    dword ptr [0x13654], 1        ; -> loc_1_13654
loc_1_4FDD:
        push   dword ptr [esp + 0x3c]
        push   ebx
        push   esi
        call   sub_1_4E04
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_1_5128
        mov    dword ptr [esi], ebx
        inc    word ptr [esi + 0xe]
        mov    dword ptr [ebx + 0x102], esi
        mov    eax, dword ptr [0x136a0]      ; -> loc_1_136A0
        sub    eax, 0x3c
        mov    dword ptr [ebx + 0x9e], eax
        push   ebx
        call   sub_1_B12A
        add    esp, 4
        mov    dword ptr [ebx + 0xa6], 1
        mov    byte ptr [ebx + 0xbc], 2
        mov    dword ptr [ebx + 0xea], ebp
        push   ebx
        call   sub_1_CCC6
        add    esp, 4
        or     eax, eax
        jne    loc_1_506A
        mov    dword ptr [ebx + 0xea], 0
        dec    dword ptr [ebx + 0xa6]
        push   0x40
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebp
        call   sub_1_4723
        add    esp, 4
        mov    eax, 2
        jmp    loc_1_5143
loc_1_506A:
        test   byte ptr [ebx + 0xbe], 1
        jne    loc_1_5113
        cmp    byte ptr [ebx + 0xbc], 2
        jne    loc_1_5113
        cmp    dword ptr [esp + 0x10], -1
        jne    loc_1_5095
        mov    eax, dword ptr [ebx + 0x8c]
        mov    dword ptr [esp + 0x10], eax
loc_1_5095:
        push   dword ptr [esp + 0x10]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_50AB
        cmp    byte ptr [eax + 0x1a], 0
        jne    loc_1_50D1
loc_1_50AB:
        push   0
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   0x12
        call   sub_1_E8C0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_50D1
        cmp    eax, 0x2b0a
        je     loc_1_50D1
        mov    eax, 0xf
        jmp    loc_1_5143
loc_1_50D1:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   0x12
        call   sub_1_E90F
        add    esp, 0x10
        mov    esi, eax
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        cmp    esi, 0x2b0a
        jne    loc_1_5101
        mov    eax, 0xff
        jmp    loc_1_5143
loc_1_5101:
        or     esi, esi
        jne    loc_1_510C
        mov    eax, 0xff
        jmp    loc_1_5143
loc_1_510C:
        mov    eax, 0xf
        jmp    loc_1_5143
loc_1_5113:
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_5143
loc_1_5123:
        mov    edi, 0xf
loc_1_5128:
        push   ebp
        call   sub_1_4723
        add    esp, 4
        push   ebx
        call   sub_1_F396
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_5143
loc_1_513E:
        mov    eax, 3
loc_1_5143:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 044h, 024h, 004h, 08Bh, 040h, 04Ah, 00Bh, 0C0h, 074h, 01Ah, 0BAh, 001h, 000h, 000h, 000h
        db  08Bh, 008h, 00Bh, 0C9h, 074h, 008h, 088h, 091h, 0BCh, 000h, 000h, 000h, 0EBh, 009h, 08Bh, 040h
        db  008h, 00Bh, 0C0h, 075h, 0EBh, 02Bh, 0C0h, 0C3h
sub_1_5173:
        push   esi
        call   sub_1_F389
        mov    esi, eax
        call   sub_1_F37E
        xor    esi, eax
        mov    eax, esi
        pop    esi
        ret    
sub_1_5186:
        sub    esp, 0x14
        push   esi
        mov    esi, 1
loc_1_518F:
        call   sub_1_129F8
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [0x13770]      ; -> loc_1_13770
        cmp    dword ptr [esp + 4], eax
        jne    loc_1_51B6
        mov    dword ptr [esp + 0x14], esi
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        jmp    loc_1_518F
loc_1_51B6:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x13770], eax      ; -> loc_1_13770
        pop    esi
        add    esp, 0x14
        ret    
sub_1_51C4:
        mov    eax, dword ptr [0x13770]      ; -> loc_1_13770
        shr    eax, 0x10
        ret    
sub_1_51CD:
        mov    al, 0x69
        ret    
sub_1_51D0:
        push   esi
        call   sub_1_5173
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        call   sub_1_5186
        mov    word ptr [esi + 4], ax
        call   sub_1_51C4
        mov    word ptr [esi + 6], ax
        mov    byte ptr [esi + 8], 0xc0
        call   sub_1_51CD
        mov    byte ptr [esi + 9], al
        mov    al, byte ptr [0x12ac4]        ; -> obj1:0x12AC4
        mov    byte ptr [esi + 0xa], al
        mov    al, byte ptr [0x12ac5]        ; -> obj1:0x12AC5
        mov    byte ptr [esi + 0xb], al
        mov    al, byte ptr [0x12ac6]        ; -> obj1:0x12AC6
        mov    byte ptr [esi + 0xc], al
        mov    al, byte ptr [0x12ac7]        ; -> obj1:0x12AC7
        mov    byte ptr [esi + 0xd], al
        mov    al, byte ptr [0x12ac8]        ; -> obj1:0x12AC8
        mov    byte ptr [esi + 0xe], al
        mov    al, byte ptr [0x12ac9]        ; -> loc_1_12AC9
        mov    byte ptr [esi + 0xf], al
        sub    eax, eax
        pop    esi
        ret    4
sub_1_5230:
        push   ebx
        push   esi
        sub    bl, bl
        mov    edx, 0x138d8                  ; -> loc_1_138D8
        mov    eax, dword ptr [esp + 0xc]
loc_1_523D:
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     loc_1_525D
        mov    esi, 0x80
loc_1_5248:
        test   word ptr [ecx + 0x40], si
        je     loc_1_5253
        cmp    dword ptr [ecx + 0x3c], eax
        je     loc_1_525B
loc_1_5253:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_5248
        jmp    loc_1_525D
loc_1_525B:
        mov    bl, 1
loc_1_525D:
        cmp    bl, 1
        je     loc_1_526D
        add    edx, 4
        cmp    edx, 0x13918                  ; -> loc_1_13918
        jb     loc_1_523D
loc_1_526D:
        movzx  eax, bl
        pop    esi
        pop    ebx
        ret    
loc_1_5273:
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x5c]
        cmp    eax, 0xec
        ja     loc_1_5330
        je     loc_1_5B1D
        cmp    eax, 0xe6
        ja     loc_1_52FB
        je     loc_1_55FA
        or     eax, eax
        je     loc_1_53D0
        cmp    eax, 0xe3
        je     loc_1_546D
        cmp    eax, 0xe4
        je     loc_1_5494
        cmp    eax, 0xe5
        je     loc_1_5541
loc_1_52C5:
        cmp    dword ptr [esp + 0x5c], 1
        jb     loc_1_53C6
        cmp    dword ptr [esp + 0x5c], 0xe1
        ja     loc_1_53C6
        mov    ebx, dword ptr [esp + 0x64]
        push   ebx
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_5341
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_52FB:
        cmp    eax, 0xe7
        jb     loc_1_52C5
        cmp    eax, 0xe8
        jbe    loc_1_5637
        cmp    eax, 0xe9
        je     loc_1_568D
        cmp    eax, 0xea
        je     loc_1_58B7
        cmp    eax, 0xeb
        je     loc_1_5A97
        jmp    loc_1_52C5
loc_1_5330:
        sub    eax, 0xed
        cmp    eax, 0x12
        ja     loc_1_52C5
        jmp    dword ptr [eax*4 + 0x7455]    ; -> obj1:0x7455
loc_1_5341:
        push   0x10
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_535B
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_535B:
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0xc], ebx
        push   0
        push   ebx
        push   edi
        mov    dword ptr [esi], 0
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        cmp    ebx, eax
        jne    loc_1_53BC
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 0x41], 0
        push   dword ptr [esp + 0x58]
        push   ebx
        push   esi
        call   sub_1_99FA
        add    esp, 0xc
        mov    esi, eax
        call   sub_1_8EF7
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_744D
loc_1_53BC:
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_53C6:
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_53D0:
        mov    ebx, dword ptr [esp + 0x64]
        push   ebx
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_53ED
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_53ED:
        push   0x10
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_5407
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5407:
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0xc], ebx
        push   0
        push   ebx
        push   edi
        mov    dword ptr [esi], 0
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        cmp    ebx, eax
        je     loc_1_543A
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_543A:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 0x41], 0
        push   dword ptr [esp + 0x58]
        push   ebx
        push   esi
        call   sub_1_C3BC
        add    esp, 0xc
        mov    esi, eax
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_744D
loc_1_546D:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        cmp    dword ptr [esp + 0x64], 1
        jne    loc_1_744D
        mov    byte ptr [eax + 0x30], 2
        mov    dword ptr [eax + 0x39], 0
        jmp    loc_1_744D
loc_1_5494:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    dword ptr [esi], 0
        push   0x3c
        push   dword ptr [esi + 4]
        mov    dword ptr [esi + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_744B
        mov    byte ptr [eax + 0x1a], 0
        mov    dword ptr [eax + 0x39], 0
        mov    ecx, dword ptr [eax + 0x41]
        or     ecx, ecx
        je     loc_1_54F0
        dec    ecx
        mov    dword ptr [eax + 0x41], ecx
loc_1_54F0:
        cmp    dword ptr [0x13758], 0        ; -> loc_1_13758
        jne    loc_1_744B
        sub    bl, bl
        mov    edx, 0x138d8                  ; -> loc_1_138D8
loc_1_5504:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_5528
        mov    ecx, 0x80
loc_1_550F:
        test   word ptr [eax + 0x40], cx
        je     loc_1_551E
        mov    esi, dword ptr [esp + 0x58]
        cmp    dword ptr [eax + 0x3c], esi
        je     loc_1_5526
loc_1_551E:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_550F
        jmp    loc_1_5528
loc_1_5526:
        mov    bl, 1
loc_1_5528:
        cmp    bl, 1
        je     loc_1_744B
        add    edx, 4
        cmp    edx, 0x13918                  ; -> loc_1_13918
        jb     loc_1_5504
        jmp    loc_1_744B
loc_1_5541:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_55F0
        movzx  ecx, byte ptr [eax + 0x30]
        cmp    ecx, 1
        je     loc_1_5570
        cmp    ecx, 3
        je     loc_1_744B
        cmp    ecx, 4
        jne    loc_1_744B
loc_1_5570:
        mov    ebx, dword ptr [esp + 0x64]
        cmp    ebx, 1
        je     loc_1_5596
        cmp    ebx, 2
        je     loc_1_559F
        cmp    ebx, 3
        je     loc_1_55AC
        cmp    ebx, 4
        jb     loc_1_558D
        cmp    ebx, 5
        jbe    loc_1_55B9
loc_1_558D:
        mov    byte ptr [eax + 0x30], 1
        jmp    loc_1_744B
loc_1_5596:
        mov    byte ptr [eax + 0x30], 1
        jmp    loc_1_744B
loc_1_559F:
        mov    byte ptr [eax + 0x30], 2
        mov    dword ptr [eax + 0x5d], 0
        jmp    loc_1_55E4
loc_1_55AC:
        mov    byte ptr [eax + 0x30], 3
        mov    dword ptr [eax + 0x5d], 0
        jmp    loc_1_55E4
loc_1_55B9:
        mov    byte ptr [eax + 0x30], 1
        mov    byte ptr [eax + 0x19], 1
        mov    dword ptr [eax + 0x41], 0
        mov    dword ptr [eax + 0x5d], 0
        mov    ecx, dword ptr [eax + 0x45]
        sub    ecx, eax
        cmp    ecx, 0x45
        mov    dword ptr [eax + 0x39], 0x12c
        jne    loc_1_744B
loc_1_55E4:
        mov    dword ptr [eax + 0x39], 0
        jmp    loc_1_744B
loc_1_55F0:
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_55FA:
        mov    ebx, dword ptr [esp + 0x64]
        mov    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        or     al, al
        je     loc_1_5625
        cmp    al, 1
        jb     loc_1_561B
        cmp    al, 0xe1
        ja     loc_1_561B
        push   ebx
        push   dword ptr [esp + 0x64]
        call   sub_1_A828
        jmp    loc_1_562F
loc_1_561B:
        push   ebx
        push   0
        call   sub_1_A828
        jmp    loc_1_562F
loc_1_5625:
        push   ebx
        push   dword ptr [esp + 0x64]
        call   sub_1_BC34
loc_1_562F:
        add    esp, 8
        jmp    loc_1_744B
loc_1_5637:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    loc_1_5661
        push   dword ptr [esp + 0x58]
        call   sub_1_35F7
        add    esp, 4
        or     eax, eax
        jne    loc_1_5661
        mov    eax, 0x2afc
        jmp    loc_1_744D
loc_1_5661:
        cmp    dword ptr [esp + 0x5c], 0xe7
        jne    loc_1_567C
        mov    byte ptr [eax + 0x18], 1
        mov    byte ptr [eax + 0x30], 1
        mov    byte ptr [eax + 0x2b], 0
        jmp    loc_1_744B
loc_1_567C:
        mov    byte ptr [eax + 0x18], 0
        mov    byte ptr [eax + 0x30], 1
        mov    byte ptr [eax + 0x2b], 1
        jmp    loc_1_744B
loc_1_568D:
        lea    eax, [esp + 0x44]
        push   eax
        call   sub_1_51D0
        cmp    eax, 0x720
        jne    loc_1_56A0
        sub    eax, eax
loc_1_56A0:
        or     eax, eax
        jne    loc_1_56B0
        lea    esi, [esp + 0x44]
        lea    edi, [esp + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_56B0:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_56DE
        push   dword ptr [esp + 0x58]
        call   sub_1_35F7
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_56EE
        mov    eax, 0x2afc
        jmp    loc_1_744D
loc_1_56DE:
        cmp    byte ptr [ebx + 0x1a], 1
        jne    loc_1_56EE
        mov    eax, 0x2b08
        jmp    loc_1_744D
loc_1_56EE:
        cmp    byte ptr [ebx + 0x2b], 0
        je     loc_1_58AD
        mov    byte ptr [ebx + 0x18], 1
        mov    byte ptr [ebx + 0x1a], 1
        lea    esi, [esp + 0x34]
        lea    edi, [ebx + 0x1b]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x14], eax
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0xc], 0x3c
        lea    esi, [esp + 0x34]
        mov    byte ptr [ebp], 0xea
        lea    edi, [ebp + 3]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [esp + 0x34]
        lea    edi, [ebp + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x60]
        mov    edx, ecx
        shl    edx, 0x10
        mov    eax, ecx
        and    eax, 0xff00
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        shr    eax, 0x10
        and    ecx, 0xff0000
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp + 0x23], edx
        movzx  ecx, byte ptr [ebp + 6]
        movzx  eax, byte ptr [ebp + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [ebp + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x12]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0x3c
        mov    word ptr [ebp + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_5895
        push   2
        push   0
        push   dword ptr [esp + 0x64]
        push   0x74a1                        ; -> loc_1_74A1
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   ebx
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        mov    eax, dword ptr [ebx + 0x55]
        mov    dword ptr [ebx + 0x4d], eax
        mov    dword ptr [ebx + 0x51], 0
        mov    byte ptr [ebx + 0x1a], 0
        jmp    loc_1_744B
loc_1_5895:
        cmp    dword ptr [ebx + 0x39], 0
        jne    loc_1_744B
        mov    eax, dword ptr [ebx + 0x31]
        mov    dword ptr [ebx + 0x39], eax
        mov    dword ptr [ebx + 0x59], eax
        jmp    loc_1_744B
loc_1_58AD:
        mov    eax, 0x2afc
        jmp    loc_1_744D
loc_1_58B7:
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x14], eax
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   0x3c
        push   ebx
        mov    dword ptr [eax + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        cmp    eax, 0x3c
        je     loc_1_58FE
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_58FE:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_5946
        push   dword ptr [esp + 0x58]
        call   sub_1_35F7
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_592C
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_592C:
        or     word ptr [ebx + 0x33], 0x4000
        lea    esi, [ebx + 3]
        lea    edi, [ebp + 0x1b]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [ebp + 0x18], 1
        mov    byte ptr [ebp + 0x1a], 1
        jmp    loc_1_5977
loc_1_5946:
        cmp    byte ptr [ebp + 0x2b], 0
        jne    loc_1_5954
        and    word ptr [ebx + 0x33], 0xbfff
        jmp    loc_1_5977
loc_1_5954:
        or     word ptr [ebx + 0x33], 0x4000
        mov    byte ptr [ebp + 0x1a], 1
        mov    byte ptr [ebp + 0x18], 1
        mov    eax, dword ptr [ebp + 0x41]
        or     eax, eax
        je     loc_1_596D
        dec    eax
        mov    dword ptr [ebp + 0x41], eax
loc_1_596D:
        lea    esi, [ebx + 3]
        lea    edi, [ebp + 0x1b]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_5977:
        mov    byte ptr [ebx], 0xeb
        movzx  ecx, byte ptr [ebx + 6]
        movzx  eax, byte ptr [ebx + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [ebx + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0x3c
        mov    word ptr [ebx + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   0
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_5A97:
        call   sub_1_A7B0
        mov    edi, eax
        push   0
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [edi], 0
        push   0x3c
        push   esi
        mov    dword ptr [edi + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        cmp    eax, 0x3c
        je     loc_1_5AD9
        push   edi
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5AD9:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    ecx, dword ptr [eax + 0x41]
        or     ecx, ecx
        je     loc_1_5AF0
        dec    ecx
        mov    dword ptr [eax + 0x41], ecx
loc_1_5AF0:
        mov    dword ptr [eax + 0x4d], 0
        test   byte ptr [esi + 0x34], 0x40
        push   edi
        je     loc_1_5B0B
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_5B0B:
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5B1D:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 4
        or     eax, eax
        jne    loc_1_5B3B
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5B3B:
        mov    ebx, dword ptr [esp + 0x64]
        push   ebx
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        jne    loc_1_5B56
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5B56:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x2c], ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [esp + 0x1c], edx
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_744D
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    edx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0xc], 0x3c
        mov    ecx, dword ptr [edx + 0x10]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 0x18
        or     edx, eax
        mov    dword ptr [ebp + 0x2b], edx
        mov    edx, dword ptr [esp + 0x1c]
        inc    ecx
        mov    dword ptr [edx + 0x10], ecx
        mov    eax, ecx
        sub    eax, dword ptr [edx + 0x14]
        or     eax, eax
        jle    loc_1_5BC7
        mov    dword ptr [edx + 0x14], ecx
loc_1_5BC7:
        mov    edx, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [edx + 0x48]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebp + 0x2f], edx
        mov    edx, dword ptr [esp + 0x1c]
        cmp    byte ptr [edx + 0xbc], 3
        mov    word ptr [ebp + 0x33], 0x12f0
        je     loc_1_5C0D
        mov    word ptr [ebp + 0x33], 0x2f0
loc_1_5C0D:
        mov    edx, dword ptr [esp + 0x1c]
        mov    esi, edx
        mov    ax, word ptr [edx + 0x4c]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 0x35], cx
        mov    dword ptr [edx + 0x94], ebx
        add    esi, 0x6c
        lea    edi, [ebp + 3]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    eax, [ebp + 0x13]
        mov    esi, dword ptr [esp + 0x1c]
        add    esi, 0x7c
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 0x2c]
        mov    byte ptr [ebp], 0xed
        mov    al, byte ptr [ecx + 0x42]
        and    edx, 0xff00
        mov    byte ptr [ebp + 2], al
        mov    eax, ebx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ebx
        and    eax, 0xff0000
        mov    ecx, ebx
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp + 0x23], edx
        push   dword ptr [esp + 0x18]
        call   sub_1_3ADE
        mov    byte ptr [esp + 0x17], al
        add    esp, 4
        or     al, al
        jne    loc_1_5C9F
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5C9F:
        push   ebx
        push   dword ptr [esp + 0x17]
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        mov    edx, dword ptr [esp + 0x28]
        mov    al, byte ptr [esp + 0x1f]
        add    esp, 0xc
        mov    byte ptr [edx + 0x98], al
        mov    byte ptr [ebp + 1], al
        mov    byte ptr [ebp + 0x3b], 0
        movzx  ecx, byte ptr [ebp + 6]
        movzx  eax, byte ptr [ebp + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [ebp + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x12]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    edx, dword ptr [esp + 0x1c]
        mov    word ptr [ebp + 0x37], ax
        mov    eax, dword ptr [edx + 0xa2]
        push   2
        push   0
        push   0x3c
        mov    dword ptr [edx + 0xc8], eax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_5DEC
        push   2
        push   2
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
loc_1_5DEC:
        mov    eax, 0x2b0a
        jmp    loc_1_744D
loc_1_5DF6:
        call   sub_1_A7B0
        mov    ebp, eax
        push   0
        mov    esi, dword ptr [ebp + 4]
        mov    dword ptr [ebp], 0
        push   0x3c
        push   esi
        mov    dword ptr [ebp + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        mov    dword ptr [esp + 0x3c], eax
        add    esp, 0x14
        cmp    eax, 0x3c
        je     loc_1_5E3D
        push   ebp
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_5E3D:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_5E5D
        push   dword ptr [esp + 0x58]
        call   sub_1_35F7
        add    esp, 4
        mov    edi, eax
loc_1_5E5D:
        mov    eax, dword ptr [edi + 0x41]
        or     eax, eax
        je     loc_1_5E68
        dec    eax
        mov    dword ptr [edi + 0x41], eax
loc_1_5E68:
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_601E
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_5EA2
        cmp    dword ptr [esp + 0x20], edi
        je     loc_1_601E
loc_1_5EA2:
        or     word ptr [esi + 0x33], 0x8000
        push   edi
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        je     loc_1_5ED2
        push   edi
        push   ebx
        push   edi
        call   sub_1_3A84
        add    esp, 0xc
        mov    byte ptr [esi + 1], bl
        mov    ebx, 2
        or     word ptr [esi + 0x33], 0x4000
        jmp    loc_1_5EDD
loc_1_5ED2:
        mov    ebx, 2
        and    word ptr [esi + 0x33], 0xbfff
loc_1_5EDD:
        mov    byte ptr [esi], 0xee
        mov    byte ptr [esi + 0x3b], 0xfa
        mov    ecx, edi
        and    ecx, 0xff00
        mov    eax, edi
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0xff0000
        mov    edx, edi
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [esi + 0x27], ecx
        push   ebp
        movzx  eax, byte ptr [esi + 0x17]
        movzx  ecx, byte ptr [esi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   ebx
        push   0
        push   0x3c
        push   ebp
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   ebx
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   ebp
        push   edi
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_601E:
        mov    ebx, edi
        cmp    dword ptr [esp + 0x20], ebx
        je     loc_1_6034
        push   ebx
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3A84
        add    esp, 0xc
loc_1_6034:
        mov    byte ptr [esi], 0xee
        or     word ptr [esi + 0x33], 0x4000
        mov    byte ptr [esi + 0x3b], 0xfa
        mov    ecx, ebx
        and    ecx, 0xff00
        mov    eax, ebx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, ebx
        and    eax, 0xff0000
        shr    ebx, 0x10
        or     eax, ebx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [esi + 0x27], ecx
        push   ebp
        inc    dword ptr [edi + 0x14]
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0x3c
        push   ebp
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_6175
        push   2
        push   0
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   ebp
        push   edi
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
loc_1_6175:
        push   0x10
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_618F
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_618F:
        mov    dword ptr [edi + 8], esi
        mov    dword ptr [edi + 4], 5
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [edi + 0xc], eax
        push   dword ptr [esp + 0x58]
        mov    dword ptr [edi], 0
        push   dword ptr [esp + 0x2c]
        push   edi
        call   sub_1_99FA
        add    esp, 0xc
        mov    esi, eax
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_744D
loc_1_61C9:
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x14], eax
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   0x3c
        push   esi
        mov    dword ptr [eax + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        cmp    eax, 0x3c
        je     loc_1_6210
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_6210:
        mov    ecx, dword ptr [esi + 0x23]
        mov    ebp, ecx
        and    ebp, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     ebp, eax
        shl    ebp, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     ebp, eax
        push   ebp
        call   sub_1_4777
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_625B
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_625B:
        mov    edi, dword ptr [ebx]
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 4
        or     eax, eax
        jne    loc_1_6287
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_6287:
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax + 0x41]
        or     ecx, ecx
        je     loc_1_6296
        dec    ecx
        mov    dword ptr [eax + 0x41], ecx
loc_1_6296:
        mov    ax, word ptr [esi + 0x33]
        test   ah, 0x40
        je     loc_1_65C7
        test   ah, 0x80
        je     loc_1_6597
        mov    ecx, dword ptr [esi + 0x27]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [edi + 0x9a], edx
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        je     loc_1_630A
        mov    bl, byte ptr [esi + 1]
        push   ebp
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        add    esp, 0xc
        and    word ptr [esi + 0x33], 0x7fff
        jmp    loc_1_6318
loc_1_630A:
        push   dword ptr [esp + 0x18]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
loc_1_6318:
        mov    al, byte ptr [edi + 0x98]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_3B6C
        add    esp, 8
        or     bl, bl
        je     loc_1_6463
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [edi + 0x98], bl
        mov    byte ptr [esi + 0x3b], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   2
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_6463:
        and    word ptr [esi + 0x33], 0xbfff
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 0x3b], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_6587
        push   2
        push   0
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
loc_1_6587:
        push   0x40
        push   edi
        call   sub_1_4B72
        add    esp, 8
        jmp    loc_1_744B
loc_1_6597:
        mov    eax, dword ptr [esp + 0x18]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [eax + 0x38]
        cmp    ecx, -1
        je     loc_1_65AF
        cmp    dword ptr [esp + 0x58], ecx
        je     loc_1_65B6
loc_1_65AF:
        mov    esi, dword ptr [esp + 0x58]
        mov    dword ptr [eax + 0x3c], esi
loc_1_65B6:
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_65C7:
        mov    al, byte ptr [edi + 0x98]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_3B6C
        add    esp, 8
        push   0x10
        push   edi
        call   sub_1_4B72
        add    esp, 8
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_65F6:
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x14], eax
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   0x3c
        push   esi
        mov    dword ptr [eax + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        mov    edi, eax
        cmp    edi, 0x3c
        je     loc_1_663F
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_663F:
        test   byte ptr [esi + 0x34], 0x40
        jne    loc_1_665B
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_665B:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 4
        or     eax, eax
        jne    loc_1_6685
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_6685:
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx + 0x41]
        or     eax, eax
        je     loc_1_6694
        dec    eax
        mov    dword ptr [ecx + 0x41], eax
loc_1_6694:
        cmp    byte ptr [esi + 0x3b], 0
        jne    loc_1_6A56
        test   byte ptr [esi + 0x34], 0x80
        jne    loc_1_6700
        mov    ecx, dword ptr [esp + 0x18]
        push   0x10
        inc    dword ptr [ecx + 0x14]
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_66C5
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_66C5:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 4], 5
        mov    dword ptr [ebx + 0xc], edi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 0x58]
        push   edi
        push   ebx
        call   sub_1_99FA
        add    esp, 0xc
        push   ebx
        call   sub_1_F396
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_6700:
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_68AD
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_6744
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x20], eax
        je     loc_1_68AD
loc_1_6744:
        or     word ptr [esi + 0x33], 0x8000
        push   dword ptr [esp + 0x18]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        jne    loc_1_6769
        mov    edi, 2
        and    word ptr [esi + 0x33], 0xbfff
        jmp    loc_1_6788
loc_1_6769:
        push   dword ptr [esp + 0x18]
        push   ebx
        mov    edi, 2
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        add    esp, 0xc
        mov    byte ptr [esi + 1], bl
        or     word ptr [esi + 0x33], 0x4000
loc_1_6788:
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 0x3b], 0xfa
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   edi
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   edi
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_68AD:
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x20], eax
        je     loc_1_68C8
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        add    esp, 0xc
loc_1_68C8:
        mov    ecx, dword ptr [esp + 0x18]
        inc    dword ptr [ecx + 0x14]
        mov    ax, word ptr [esi + 0x33]
        push   dword ptr [esp + 0x14]
        and    ax, 0x7fff
        movzx  ecx, byte ptr [esi + 6]
        mov    word ptr [esi + 0x33], ax
        or     ax, 0x4000
        mov    word ptr [esi + 0x33], ax
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 0x3b], 0xfa
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_6A01
        push   2
        push   0
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
loc_1_6A01:
        push   0x10
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_6A1B
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_6A1B:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 4], 5
        mov    dword ptr [ebx + 0xc], edi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 0x58]
        push   edi
        push   ebx
        call   sub_1_99FA
        add    esp, 0xc
        push   ebx
        call   sub_1_F396
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_6A56:
        mov    ecx, dword ptr [esi + 0x23]
        mov    edi, ecx
        and    edi, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edi, eax
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        jne    loc_1_6A9F
        push   dword ptr [esp + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_6A9F:
        mov    ebp, dword ptr [eax]
        test   byte ptr [esi + 0x34], 0x80
        jne    loc_1_6ABF
        mov    ecx, dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        inc    dword ptr [ecx + 0x14]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_6ABF:
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_6C73
        lea    eax, [esp + 0x20]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3AAC
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_6AFF
        cmp    dword ptr [esp + 0x20], edi
        je     loc_1_6C8A
loc_1_6AFF:
        or     word ptr [esi + 0x33], 0x8000
        push   dword ptr [esp + 0x18]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        jne    loc_1_6B1F
        and    word ptr [esi + 0x33], 0xbfff
        jmp    loc_1_6B36
loc_1_6B1F:
        push   edi
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        add    esp, 0xc
        mov    byte ptr [esi + 1], bl
        or     word ptr [esi + 0x33], 0x4000
loc_1_6B36:
        mov    al, byte ptr [ebp + 0x98]
        push   eax
        mov    edi, 2
        push   dword ptr [esp + 0x1c]
        call   sub_1_3B6C
        add    esp, 8
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 0x3b], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        push   dword ptr [esp + 0x14]
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   edi
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   edi
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_6C73:
        cmp    dword ptr [esp + 0x20], edi
        je     loc_1_6C8A
        push   edi
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_3A84
        add    esp, 0xc
loc_1_6C8A:
        mov    ecx, dword ptr [esp + 0x18]
        inc    dword ptr [ecx + 0x14]
        mov    al, byte ptr [ebp + 0x98]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_3B6C
        add    esp, 8
        mov    al, byte ptr [esi + 1]
        mov    byte ptr [ebp + 0x98], al
        mov    ax, word ptr [esi + 0x33]
        push   dword ptr [esp + 0x14]
        and    ax, 0x7fff
        movzx  ecx, byte ptr [esi + 6]
        mov    word ptr [esi + 0x33], ax
        or     ax, 0x4000
        mov    word ptr [esi + 0x33], ax
        mov    byte ptr [esi], 0xfb
        mov    byte ptr [esi + 0x3b], 0
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        movzx  ecx, byte ptr [esi + 6]
        not    ax
        push   dword ptr [esp + 0x14]
        mov    word ptr [esi + 0x37], ax
        movzx  eax, byte ptr [esi + 0x17]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   0x3c
        mov    word ptr [esi + 0x37], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_744B
        push   2
        push   2
        push   dword ptr [esp + 0x64]
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x78]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_744B
loc_1_6EC0:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        push   dword ptr [esp + 0x58]
        call   sub_1_EE10
        add    esp, 4
        mov    ebp, eax
        or     esi, esi
        je     loc_1_744B
        cmp    byte ptr [esi + 0x1a], 1
        jne    loc_1_744B
        call   sub_1_A7B0
        mov    edi, eax
        push   edi
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0xc], 0x3c
        mov    byte ptr [ebx], 0xfd
        movzx  eax, byte ptr [ebx + 0x17]
        movzx  ecx, byte ptr [ebx + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x37], ax
        or     ebp, ebp
        je     loc_1_744B
        mov    ecx, dword ptr [ebp + 0x14]
        or     ecx, ecx
        je     loc_1_744B
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   ecx
        call   sub_1_10879
        mov    dword ptr [esp + 0x30], eax
        add    esp, 0xc
        cmp    eax, 1
        jne    loc_1_7040
        push   2
        push   0
        push   0x3c
        push   edi
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        mov    byte ptr [esi + 0x1a], 0
        mov    dword ptr [esi + 0x39], 0
        mov    eax, dword ptr [esi + 0x41]
        or     eax, eax
        je     loc_1_744B
        dec    eax
        mov    dword ptr [esi + 0x41], eax
        jmp    loc_1_744B
loc_1_7040:
        cmp    dword ptr [esp + 0x24], 0
        mov    byte ptr [esi + 0x1a], 0
        mov    dword ptr [esi + 0x39], 0
        mov    eax, dword ptr [esi + 0x41]
        jne    loc_1_708F
        or     eax, eax
        je     loc_1_705D
        dec    eax
        mov    dword ptr [esi + 0x41], eax
loc_1_705D:
        cmp    dword ptr [0x13758], 0        ; -> loc_1_13758
        jne    loc_1_744B
        push   dword ptr [esp + 0x58]
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    loc_1_744B
        push   1
        push   0x12
        call   sub_1_F02D
        add    esp, 8
        jmp    loc_1_744B
loc_1_708F:
        or     eax, eax
        je     loc_1_744B
        dec    eax
        mov    dword ptr [esi + 0x41], eax
        jmp    loc_1_744B
loc_1_70A0:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    dword ptr [esi], 0
        push   0x3c
        push   dword ptr [esi + 4]
        mov    dword ptr [esi + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_744B
loc_1_70E3:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    edi, dword ptr [esi + 4]
        mov    dword ptr [esi], 0
        push   0x3c
        push   edi
        mov    dword ptr [esi + 0xc], 0x3c
        push   dword ptr [esp + 0x64]
        push   0x12
        call   sub_1_EF50
        add    esp, 0x14
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        mov    ecx, dword ptr [eax + 0x3d]
        or     ecx, ecx
        je     loc_1_7124
        dec    ecx
        mov    dword ptr [eax + 0x3d], ecx
loc_1_7124:
        test   byte ptr [edi + 0x34], 0x80
        je     loc_1_7168
        mov    cl, byte ptr [eax + 0x30]
        cmp    cl, 1
        je     loc_1_7137
        cmp    cl, 4
        jne    loc_1_7168
loc_1_7137:
        cmp    byte ptr [eax + 0x19], 1
        je     loc_1_7168
        mov    dword ptr [eax + 0x39], 0
        mov    ecx, dword ptr [eax + 0x35]
        mov    dword ptr [eax + 0x39], ecx
        mov    dword ptr [eax + 0x59], ecx
        mov    dword ptr [eax + 0x5d], 0
        mov    ecx, dword ptr [0x137ec]      ; -> loc_1_137EC
        mov    dword ptr [eax + 0x61], ecx
        mov    ecx, dword ptr [0x13788]      ; -> loc_1_13788
        mov    dword ptr [eax + 0x65], ecx
        jmp    loc_1_71AA
loc_1_7168:
        mov    cl, byte ptr [eax + 0x30]
        cmp    cl, 1
        je     loc_1_7175
        cmp    cl, 4
        jne    loc_1_71A6
loc_1_7175:
        cmp    byte ptr [eax + 0x19], 1
        je     loc_1_71A6
        mov    dword ptr [eax + 0x39], 0
        mov    ecx, dword ptr [eax + 0x31]
        mov    dword ptr [eax + 0x39], ecx
        mov    dword ptr [eax + 0x59], ecx
        mov    dword ptr [eax + 0x5d], 0
        mov    ecx, dword ptr [0x137ec]      ; -> loc_1_137EC
        mov    dword ptr [eax + 0x61], ecx
        mov    ecx, dword ptr [0x13648]      ; -> loc_1_13648
        mov    dword ptr [eax + 0x65], ecx
        jmp    loc_1_71AA
loc_1_71A6:
        mov    byte ptr [eax + 0x19], 0
loc_1_71AA:
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 0x2b0c
        jmp    loc_1_744D
loc_1_71BD:
        push   dword ptr [esp + 0x58]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    loc_1_71E7
        push   dword ptr [esp + 0x58]
        call   sub_1_35F7
        add    esp, 4
        or     eax, eax
        jne    loc_1_71E7
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_71E7:
        movzx  ecx, byte ptr [eax + 0x30]
        cmp    ecx, 1
        je     loc_1_7207
        cmp    ecx, 2
        je     loc_1_7263
        cmp    ecx, 3
        je     loc_1_7416
        cmp    ecx, 4
        jne    loc_1_744B
loc_1_7207:
        mov    ebx, dword ptr [esp + 0x64]
        cmp    ebx, 2
        je     loc_1_7228
        cmp    ebx, 3
        je     loc_1_7247
        cmp    ebx, 4
        jb     loc_1_744B
        cmp    ebx, 5
        jbe    loc_1_7250
        jmp    loc_1_744B
loc_1_7228:
        mov    byte ptr [eax + 0x30], 1
        cmp    dword ptr [eax + 0x39], 0
        jne    loc_1_723B
        mov    ecx, dword ptr [eax + 0x31]
        mov    dword ptr [eax + 0x39], ecx
        mov    dword ptr [eax + 0x59], ecx
loc_1_723B:
        mov    dword ptr [eax + 0x3d], 0
        jmp    loc_1_744B
loc_1_7247:
        mov    byte ptr [eax + 0x30], 1
        jmp    loc_1_744B
loc_1_7250:
        mov    ecx, dword ptr [eax + 0x45]
        sub    ecx, eax
        cmp    ecx, 0x45
        je     loc_1_7443
        jmp    loc_1_742E
loc_1_7263:
        mov    ebx, dword ptr [esp + 0x64]
        cmp    ebx, 2
        jb     loc_1_744B
        cmp    ebx, 3
        jbe    loc_1_728C
        cmp    ebx, 4
        jb     loc_1_744B
        cmp    ebx, 5
        jbe    loc_1_740A
        jmp    loc_1_744B
loc_1_728C:
        mov    byte ptr [eax + 0x30], 1
        cmp    ebx, 2
        je     loc_1_7299
        mov    byte ptr [eax + 0x30], 4
loc_1_7299:
        cmp    dword ptr [eax + 0x39], 0
        jne    loc_1_72A8
        mov    ecx, dword ptr [eax + 0x31]
        mov    dword ptr [eax + 0x39], ecx
        mov    dword ptr [eax + 0x59], ecx
loc_1_72A8:
        mov    dword ptr [eax + 0x5d], 0
        mov    ecx, dword ptr [0x13844]      ; -> loc_1_13844
        mov    dword ptr [eax + 0x61], ecx
        mov    ecx, dword ptr [0x1382c]      ; -> loc_1_1382C
        mov    dword ptr [eax + 0x65], ecx
        mov    dword ptr [eax + 0x3d], 0
        call   sub_1_A7B0
        mov    esi, eax
        or     esi, esi
        je     loc_1_73EB
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        jne    loc_1_72E8
        mov    eax, 0x2b09
        jmp    loc_1_744D
loc_1_72E8:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 0xc], 0x3c
        mov    byte ptr [edi], 0xfe
        push   esi
        movzx  eax, byte ptr [edi + 0x17]
        movzx  ecx, byte ptr [edi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x13]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x14]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x15]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x16]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x18]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x19]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1a]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xa]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1b]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xb]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1c]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xc]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1d]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xd]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1e]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xe]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x1f]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0xf]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x20]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x21]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x10]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x11]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x22]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0x12]
        add    ecx, eax
        add    ecx, 0x8c00
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0
        push   esi
        mov    word ptr [edi + 0x37], ax
        push   dword ptr [esp + 0x68]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        push   esi
        call   sub_1_A7C2
        add    esp, 4
loc_1_73EB:
        push   dword ptr [esp + 0x58]
        call   sub_1_CF55
        add    esp, 4
        cmp    dword ptr [0x1359c], 0x1359c  ; -> loc_1_1359C
        je     loc_1_744B
        call   sub_1_BB33
        jmp    loc_1_744B
loc_1_740A:
        mov    ecx, dword ptr [eax + 0x45]
        sub    ecx, eax
        cmp    ecx, 0x45
        je     loc_1_7443
        jmp    loc_1_742E
loc_1_7416:
        mov    ebx, dword ptr [esp + 0x64]
        cmp    ebx, 4
        jb     loc_1_744B
        cmp    ebx, 5
        ja     loc_1_744B
        mov    ecx, dword ptr [eax + 0x45]
        sub    ecx, eax
        cmp    ecx, 0x45
        je     loc_1_7443
loc_1_742E:
        mov    dword ptr [eax + 0x3d], 0
        mov    dword ptr [eax + 0x41], 0
        mov    dword ptr [eax + 0x39], 3
loc_1_7443:
        mov    byte ptr [eax + 0x30], 1
        mov    byte ptr [eax + 0x19], 1
loc_1_744B:
        sub    eax, eax
loc_1_744D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_5DF6
        dd  loc_1_61C9
        dd  loc_1_52C5
        dd  loc_1_744D
        dd  loc_1_744D
        dd  loc_1_52C5
        dd  loc_1_52C5
        dd  loc_1_52C5
        dd  loc_1_52C5
        dd  loc_1_52C5
        dd  loc_1_52C5
        dd  loc_1_744D
        dd  loc_1_744D
        dd  loc_1_744D
        dd  loc_1_65F6
        dd  loc_1_6EC0
        dd  loc_1_70A0
        dd  loc_1_70E3
        dd  loc_1_71BD
loc_1_74A1:
        sub    esp, 4
        push   dword ptr [esp + 8]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    loc_1_74BB
        mov    eax, 0x2b0e
        jmp    loc_1_7539
loc_1_74BB:
        cmp    dword ptr [esp + 0x10], 0xe9
        je     loc_1_74CC
        mov    ecx, 0x2b0f
        jmp    loc_1_7537
loc_1_74CC:
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        jne    loc_1_74ED
        mov    dword ptr [eax + 0x4d], 0
        mov    dword ptr [eax + 0x51], 0
        mov    byte ptr [eax + 0x1a], 1
        mov    ecx, 0x2b0e
        jmp    loc_1_7537
loc_1_74ED:
        cmp    ecx, 0x2b09
        jne    loc_1_7533
        cmp    dword ptr [eax + 0x4d], 0
        jne    loc_1_752C
        mov    ecx, dword ptr [0x12b14]      ; -> obj1:0x12B14
        cmp    dword ptr [eax + 0x51], ecx
        jne    loc_1_751F
        mov    byte ptr [eax + 0x1a], 0
        mov    ecx, 0x2b0d
        mov    dword ptr [eax + 0x51], 0
        mov    dword ptr [eax + 0x39], 0
        jmp    loc_1_7537
loc_1_751F:
        mov    ecx, dword ptr [eax + 0x55]
        mov    dword ptr [eax + 0x4d], ecx
        mov    ecx, 0x2b0f
        jmp    loc_1_7537
loc_1_752C:
        mov    ecx, 0x2b0f
        jmp    loc_1_7537
loc_1_7533:
        mov    ecx, dword ptr [esp]
loc_1_7537:
        mov    eax, ecx
loc_1_7539:
        add    esp, 4
        ret    
loc_1_753D:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x30]
        cmp    dword ptr [esp + 0x38], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x10], eax
        je     loc_1_7586
        mov    eax, dword ptr [esp + 0x38]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_7586
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   ecx
        call   sub_1_3BB5
        add    esp, 0x10
        or     al, al
        jne    loc_1_7586
        mov    eax, 3
        jmp    loc_1_7668
loc_1_7586:
        call   sub_1_4735
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_759B
        mov    eax, 1
        jmp    loc_1_7668
loc_1_759B:
        call   sub_1_CE5D
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_75B9
        push   ebx
        call   sub_1_4723
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_7668
loc_1_75B9:
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        jne    loc_1_75E9
        push   0x5273                        ; -> loc_1_5273
        push   0x12
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     loc_1_75DF
        mov    eax, 0x20
        jmp    loc_1_7668
loc_1_75DF:
        mov    dword ptr [0x13654], 1        ; -> loc_1_13654
loc_1_75E9:
        mov    ax, word ptr [esp + 0x34]
        mov    word ptr [ebx + 0x18], ax
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [ebp + 0x7c]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [ebp + 0xea], ebx
        lea    esi, [esp + 0x1c]
        mov    byte ptr [ebp + 0xbc], 1
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x10]
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_7654
        push   dword ptr [esp + 0x38]
        push   ebp
        push   esi
        call   sub_1_4E04
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_7668
        mov    dword ptr [esi], ebp
        mov    dword ptr [ebp + 0x102], esi
        inc    word ptr [esi + 0xe]
        mov    eax, 0xff
        jmp    loc_1_7668
loc_1_7654:
        push   ebp
        call   sub_1_CF22
        add    esp, 4
        push   ebx
        call   sub_1_4723
        add    esp, 4
        sub    eax, eax
loc_1_7668:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_7670:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_B12A
        add    esp, 4
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        or     eax, 2
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
        push   esi
        call   sub_1_A929
        add    esp, 4
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    esi
        ret    
loc_1_769F:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [ebx]
        call   sub_1_4735
        mov    esi, eax
        or     esi, esi
        jne    loc_1_76BA
        mov    eax, 1
        jmp    loc_1_7734
loc_1_76BA:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0xc], eax
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_7724
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_7724
        test   byte ptr [ecx + 0xbe], 1
        jne    loc_1_7724
        cmp    byte ptr [ecx + 0xbc], 3
        jne    loc_1_7724
        mov    eax, dword ptr [ecx + 0x60]
        test   al, 4
        jne    loc_1_7724
        cmp    dword ptr [ecx + 0xea], 0
        jne    loc_1_7724
        mov    dword ptr [ecx + 0xea], esi
        or     eax, 4
        mov    dword ptr [ecx + 0x60], eax
        inc    dword ptr [ecx + 0xa6]
        push   ecx
        call   sub_1_7670
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_7734
loc_1_7724:
        mov    edi, 0xf
        push   esi
        call   sub_1_4723
        add    esp, 4
        mov    eax, edi
loc_1_7734:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_7738:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        push   dword ptr [ebx]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_7896
        mov    edi, dword ptr [eax]
        or     edi, edi
        je     loc_1_7896
        mov    si, word ptr [esp + 0x18]
        test   si, 2
        je     loc_1_779C
        cmp    byte ptr [edi + 0xbc], 0
        je     loc_1_7792
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_778B
        or     dword ptr [edi + 0x60], 0x40000000
        push   0x40
        push   edi
        call   sub_1_4B72
        add    esp, 8
loc_1_778B:
        sub    eax, eax
        jmp    loc_1_789B
loc_1_7792:
        mov    eax, 0x19
        jmp    loc_1_789B
loc_1_779C:
        call   sub_1_4735
        mov    ecx, eax
        or     ecx, ecx
        je     loc_1_788F
        mov    word ptr [ecx + 0x18], 0
        mov    dword ptr [ecx + 0x14], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 8], eax
        test   si, 1
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ecx + 0xc], eax
        jne    loc_1_786C
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_77FC
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_77FC
        mov    esi, 0x64
        sub    edx, edx
        div    esi
        inc    eax
        cmp    eax, 0xffffff
        jbe    loc_1_77F5
        mov    eax, 0xffffff
        jmp    loc_1_77F6
loc_1_77F5:
        inc    eax
loc_1_77F6:
        mov    word ptr [ecx + 0x1a], ax
        jmp    loc_1_7802
loc_1_77FC:
        mov    word ptr [ecx + 0x1a], 0
loc_1_7802:
        mov    al, byte ptr [edi + 0xbc]
        cmp    al, 4
        jne    loc_1_7840
        mov    byte ptr [edi + 0xbc], 5
        inc    byte ptr [edi + 0xba]
        or     dword ptr [edi + 0x64], 1
        mov    dword ptr [edi + 0xea], ecx
        or     dword ptr [edi + 0x60], 0x10000
        inc    dword ptr [edi + 0xa6]
        push   edi
        call   sub_1_B37E
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_789B
loc_1_7840:
        cmp    al, 7
        jne    loc_1_785C
        mov    byte ptr [edi + 0xbc], 9
        push   0
        push   0
        push   edi
        call   sub_1_7998
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_1_789B
loc_1_785C:
        push   ecx
        call   sub_1_4723
        add    esp, 4
        mov    eax, 0x19
        jmp    loc_1_789B
loc_1_786C:
        mov    word ptr [ecx + 0x1a], 0
        cmp    dword ptr [edi + 0x11a], 0
        jne    loc_1_7888
        mov    dword ptr [edi + 0x11a], ecx
        mov    eax, 0xff
        jmp    loc_1_789B
loc_1_7888:
        mov    eax, 0x19
        jmp    loc_1_789B
loc_1_788F:
        mov    eax, 1
        jmp    loc_1_789B
loc_1_7896:
        mov    eax, 0xf
loc_1_789B:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_789F:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0xde], 0
        jne    loc_1_78CC
        cmp    dword ptr [eax + 0x126], 0
        jne    loc_1_78CC
        cmp    dword ptr [eax + 0xd6], 0
        jne    loc_1_78CC
        cmp    dword ptr [eax + 0x11e], 0
        mov    eax, 1
        je     loc_1_78CE
loc_1_78CC:
        sub    eax, eax
loc_1_78CE:
        ret    
sub_1_78CF:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    byte ptr [ebx + 0xbc], 3
        jbe    loc_1_7994
        test   byte ptr [ebx + 0x60], 0x10
        jne    loc_1_7994
        mov    esi, dword ptr [esp + 0x14]
        cmp    esi, 0x16
        jne    loc_1_790F
        push   ebx
        call   sub_1_789F
        add    esp, 4
        or     eax, eax
        jne    loc_1_790F
        or     dword ptr [ebx + 0x60], 0x100000
        jmp    loc_1_7994
loc_1_790F:
        mov    eax, dword ptr [ebx + 0x60]
        or     eax, 0x10
        mov    dword ptr [ebx + 0x60], eax
        and    eax, 0xffefffff
        mov    dword ptr [ebx + 0x60], eax
        mov    edi, dword ptr [ebx + 0x11a]
        or     edi, edi
        je     loc_1_7948
        mov    dword ptr [ebx + 0x11a], 0
        push   0
        push   esi
        push   dword ptr [edi + 0xc]
        call   dword ptr [edi + 8]
        add    esp, 0xc
        push   edi
        call   sub_1_4723
        jmp    loc_1_7991
loc_1_7948:
        mov    edx, dword ptr [ebx + 0xee]
        mov    eax, dword ptr [ebx + 0x102]
        or     eax, eax
        je     loc_1_7994
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     loc_1_7994
        mov    eax, dword ptr [edi + 0x62]
        mov    ebx, dword ptr [edi + 0x66]
        or     eax, eax
        je     loc_1_7994
        inc    word ptr [edi + 0x46]
        sub    esi, 0x16
        cmp    esi, 1
        sbb    ecx, ecx
        and    ecx, 2
        add    ecx, 2
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   edx
        push   ebx
        call   eax
        add    esp, 0x1c
        push   edi
        call   sub_1_4325
loc_1_7991:
        add    esp, 4
loc_1_7994:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7998:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x14], 0
        mov    esi, dword ptr [esp + 0xc]
        je     loc_1_7A06
        push   esi
        call   sub_1_789F
        add    esp, 4
        or     eax, eax
        je     loc_1_79EC
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_1_79CE
        call   sub_1_F21C
        mov    dword ptr [esi + 0xce], eax
        mov    byte ptr [esi + 0xbc], 0xa
        jmp    loc_1_79D9
loc_1_79CE:
        push   0
        push   esi
        call   sub_1_4B72
        add    esp, 8
loc_1_79D9:
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   0x16
        push   esi
        call   sub_1_78CF
        jmp    loc_1_7A45
loc_1_79EC:
        cmp    dword ptr [esp + 0x10], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x200000
        or     eax, 0x1000000
        or     dword ptr [esi + 0x60], eax
        jmp    loc_1_7A8E
loc_1_7A06:
        mov    eax, dword ptr [esi + 0x60]
        test   al, 0x10
        je     loc_1_7A6A
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_1_7A29
        call   sub_1_F21C
        mov    dword ptr [esi + 0xce], eax
        mov    byte ptr [esi + 0xbc], 0xa
        jmp    loc_1_7A34
loc_1_7A29:
        push   0
        push   esi
        call   sub_1_4B72
        add    esp, 8
loc_1_7A34:
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   0
        push   esi
        call   sub_1_8FCD
loc_1_7A45:
        add    esp, 8
        test   byte ptr [esi + 0xbe], 1
        jne    loc_1_7A8E
        or     edi, edi
        je     loc_1_7A8E
        mov    dword ptr [esi + 0xc8], 0x3e8
        push   esi
        call   sub_1_48FB
        add    esp, 4
        jmp    loc_1_7A8E
loc_1_7A6A:
        cmp    dword ptr [esp + 0x10], 1
        cmc    
        sbb    ecx, ecx
        and    ecx, 0x200000
        or     ecx, eax
        or     ecx, 0x1000000
        mov    dword ptr [esi + 0x60], ecx
        push   0
        push   esi
        call   sub_1_8FCD
        add    esp, 8
loc_1_7A8E:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_7AA0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x12acc]      ; -> loc_1_12ACC
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x12acc], ecx      ; -> loc_1_12ACC
        ret    
sub_1_7AB2:
        mov    eax, dword ptr [0x12acc]      ; -> loc_1_12ACC
        or     eax, eax
        je     loc_1_7AC5
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x12acc], ecx      ; -> loc_1_12ACC
        jmp    loc_1_7AF3
loc_1_7AC5:
        mov    ecx, dword ptr [0x12ad0]      ; -> loc_1_12AD0
        cmp    dword ptr [0x12ad4], ecx      ; -> obj1:0x12AD4
        mov    eax, 0
        jbe    loc_1_7AE8
        push   0x24
        call   sub_1_F390
        add    esp, 4
        mov    ecx, dword ptr [0x12ad0]      ; -> loc_1_12AD0
loc_1_7AE8:
        or     eax, eax
        je     loc_1_7AF3
        inc    ecx
        mov    dword ptr [0x12ad0], ecx      ; -> loc_1_12AD0
loc_1_7AF3:
        ret    
sub_1_7AF4:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax], 0
        je     loc_1_7B04
loc_1_7AFD:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    loc_1_7AFD
loc_1_7B04:
        ret    
sub_1_7B05:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x10]
        je     loc_1_7B40
        mov    ebx, 5
loc_1_7B19:
        mov    eax, dword ptr [esi + 0xc]
        cmp    edi, eax
        jb     loc_1_7B3A
        sub    edi, eax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 4], ebx
        jne    loc_1_7B34
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_7B34:
        or     edi, edi
        jne    loc_1_7B19
        jmp    loc_1_7B40
loc_1_7B3A:
        sub    dword ptr [esi + 0xc], edi
        add    dword ptr [esi + 8], edi
loc_1_7B40:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7B46:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x20]
        cmp    dword ptr [ebp + 4], 5
        je     loc_1_7C5E
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esp + 0x28]
        cmp    eax, ebx
        jb     loc_1_7B68
        mov    eax, ebx
loc_1_7B68:
        mov    dword ptr [esp + 0x18], eax
        add    eax, 0x10
        push   eax
        call   sub_1_F390
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        or     eax, eax
        je     loc_1_7C5A
        mov    edx, eax
        mov    ecx, eax
        mov    dword ptr [esp + 0x10], edx
        mov    dword ptr [ecx], 0
        mov    ecx, eax
        mov    edi, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 4], 5
        mov    ecx, eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [esp + 0x14]
        add    edi, 0x10
        mov    dword ptr [ecx + 8], edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    ebx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 0x20], ecx
        or     ecx, ecx
        je     loc_1_7C3E
loc_1_7BD2:
        mov    ecx, dword ptr [esp + 0x20]
        cmp    dword ptr [ecx + 4], 5
        je     loc_1_7C3E
        or     ebx, ebx
        je     loc_1_7C3E
        mov    ebp, dword ptr [ecx + 0xc]
        cmp    ebp, ebx
        jb     loc_1_7BE9
        mov    ebp, ebx
loc_1_7BE9:
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        je     loc_1_7C4A
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 5
        mov    dword ptr [eax + 0xc], ebp
        lea    edi, [eax + 0x10]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 8], edi
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [esp + 0x20]
        sub    ebx, ebp
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 0x20], ecx
        or     ecx, ecx
        jne    loc_1_7BD2
loc_1_7C3E:
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [edx], ecx
        jmp    loc_1_7C78
loc_1_7C4A:
        push   dword ptr [esp + 0x14]
        call   sub_1_95BA
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_7CC2
loc_1_7C5A:
        sub    eax, eax
        jmp    loc_1_7CC2
loc_1_7C5E:
        mov    dword ptr [esp + 0x14], ebp
        mov    dword ptr [esp + 0x10], ebp
        add    ebp, 0xc
        mov    ebx, dword ptr [esp + 0x28]
        cmp    dword ptr [ebp], ebx
        jbe    loc_1_7C75
        mov    dword ptr [ebp], ebx
loc_1_7C75:
        sub    ebx, dword ptr [ebp]
loc_1_7C78:
        or     ebx, ebx
        je     loc_1_7C97
loc_1_7C7C:
        mov    edx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 0x10], edx
        cmp    dword ptr [edx + 0xc], ebx
        jbe    loc_1_7C8E
        mov    dword ptr [edx + 0xc], ebx
loc_1_7C8E:
        mov    edx, dword ptr [esp + 0x10]
        sub    ebx, dword ptr [edx + 0xc]
        jne    loc_1_7C7C
loc_1_7C97:
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_7CB4
        push   eax
        call   sub_1_95BA
        mov    edx, dword ptr [esp + 0x14]
        add    esp, 4
        mov    dword ptr [edx], 0
loc_1_7CB4:
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x14]
loc_1_7CC2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_7CCA:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        lea    eax, [esp + 8]
        push   esi
        mov    edi, dword ptr [esp + 0x1c]
        push   eax
        push   edi
        call   sub_1_7B46
        add    esp, 0xc
        or     eax, eax
        je     loc_1_7D19
        mov    edx, dword ptr [esp + 0x10]
        cmp    dword ptr [edx + 0xe2], 0
        jne    loc_1_7CFF
        mov    dword ptr [edx + 0xe2], eax
        jmp    loc_1_7D07
loc_1_7CFF:
        mov    ecx, dword ptr [edx + 0xe6]
        mov    dword ptr [ecx], eax
loc_1_7D07:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xe6], eax
        add    dword ptr [edx + 0xde], esi
        jmp    loc_1_7D24
loc_1_7D19:
        push   edi
        call   sub_1_95BA
        add    esp, 4
        sub    esi, esi
loc_1_7D24:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_1_7D2C:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x14], 0
        mov    ebx, dword ptr [ecx + 0x58]
        mov    ebp, dword ptr [ebx + 0x20]
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [ebx + 0x1c]
        sub    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        sub    ecx, edx
        cmp    eax, ecx
        jb     loc_1_7D59
        mov    eax, ecx
loc_1_7D59:
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebp + 4]
        add    eax, edx
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [esp + 0x18], eax
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x24], ecx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [esp + 0x40]
        and    eax, 0x800
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x80000000
        mov    dword ptr [esp + 0x2c], eax
        mov    dword ptr [esp + 0x30], eax
        or     dword ptr [ebx + 0x14], eax
loc_1_7D9B:
        mov    eax, dword ptr [esp + 0x48]
        sub    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edx, dword ptr [esp + 0x24]
        cmp    ecx, edx
        jb     loc_1_7DB1
        mov    ecx, edx
loc_1_7DB1:
        cmp    eax, ecx
        jb     loc_1_7DB7
        mov    eax, ecx
loc_1_7DB7:
        mov    dword ptr [esp + 0x10], eax
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x14], eax
        add    dword ptr [ebx + 0xc], eax
        sub    dword ptr [esp + 0x24], eax
        jne    loc_1_7E17
        mov    esi, dword ptr [esp + 0x18]
        mov    edx, esi
        mov    eax, dword ptr [esp + 0x14]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 0x18], edx
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_7E06
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x24], ecx
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 0x28], eax
loc_1_7E06:
        cmp    dword ptr [esi + 4], 5
        jne    loc_1_7E1F
        push   esi
        call   sub_1_F396
        add    esp, 4
        jmp    loc_1_7E1F
loc_1_7E17:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x28], eax
loc_1_7E1F:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esp + 0x20], eax
        jne    loc_1_7EB0
        mov    eax, dword ptr [ebx + 0x1c]
        sub    eax, dword ptr [ebx + 0xc]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     loc_1_7E54
        or     eax, eax
        je     loc_1_7E54
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, eax
        jb     loc_1_7E47
        mov    ecx, eax
loc_1_7E47:
        mov    dword ptr [esp + 0x20], ecx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_1_7EB8
loc_1_7E54:
        mov    eax, dword ptr [ebx + 0x14]
        or     eax, 0x80000000
        mov    dword ptr [ebx + 0x14], eax
        test   ah, 1
        je     loc_1_7E75
        push   0x200
        push   dword ptr [esp + 0x40]
        call   sub_1_8EB3
        add    esp, 8
loc_1_7E75:
        mov    dword ptr [esp + 0x30], 1
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_1_7EC6
        mov    ebp, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, ecx
        jb     loc_1_7E92
        mov    eax, ecx
loc_1_7E92:
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_7EB8
        mov    eax, dword ptr [esp + 0x2c]
        or     dword ptr [ebx + 0x14], eax
        jmp    loc_1_7EB8
loc_1_7EB0:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x1c], eax
loc_1_7EB8:
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        jne    loc_1_7D9B
loc_1_7EC6:
        mov    ecx, dword ptr [esp + 0x3c]
        or     ebx, ebx
        mov    dword ptr [ecx + 0x58], ebx
        je     loc_1_7EE7
        mov    dword ptr [ebx + 0x20], ebp
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ecx + 0x68], 0x7d2c ; -> sub_1_7D2C
        jmp    loc_1_7EF2
loc_1_7EE7:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0x68], 0x7cca ; -> loc_1_7CCA
loc_1_7EF2:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [ecx + 0x5c]
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, eax
        cmp    ecx, edx
        jb     loc_1_7F05
        mov    ecx, edx
loc_1_7F05:
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0x5c], eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_7F9C
        mov    edx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x48]
        sub    eax, dword ptr [esp + 0x14]
        push   eax
        mov    dword ptr [edx + 0xc], ecx
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_7B46
        add    esp, 0xc
        or     eax, eax
        je     loc_1_7FA3
        mov    ecx, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [ecx + 0xe2]
        or     ecx, ecx
        jne    loc_1_7F70
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0xe2], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ecx + 0xe6], eax
        jmp    loc_1_7F80
loc_1_7F70:
        mov    edx, dword ptr [esp + 0x34]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0xe2], eax
loc_1_7F80:
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [esp + 0x3c]
        sub    eax, dword ptr [esp + 0x14]
        add    dword ptr [ecx + 0xde], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x14], eax
        jmp    loc_1_7FAF
loc_1_7F9C:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_7FAF
loc_1_7FA3:
        push   dword ptr [esp + 0x18]
        call   sub_1_95BA
        add    esp, 4
loc_1_7FAF:
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_7FC7
        push   0x100
        push   dword ptr [esp + 0x40]
        call   sub_1_8EB3
        add    esp, 8
loc_1_7FC7:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
loc_1_7FD3:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        call   sub_1_7AB2
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_81D9
        mov    esi, dword ptr [esp + 0x34]
        mov    edi, dword ptr [esp + 0x40]
        mov    dword ptr [esi + 0x5c], edi
        or     dword ptr [esi + 0x60], 0x80
        lea    eax, [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x3c]
        push   eax
        lea    eax, [esp + 0x14]
        push   dword ptr [ebx + 8]
        push   eax
        push   edi
        push   dword ptr [ebx + 0xc]
        push   0x230
        push   dword ptr [esi + 0xee]
        push   dword ptr [esi + 0x116]
        call   dword ptr [esi + 0x112]
        mov    dword ptr [esp + 0x38], eax
        add    esp, 0x20
        and    dword ptr [esi + 0x60], 0xffffff7f
        cmp    eax, 0x18
        jne    loc_1_80F4
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ebp + 0x20], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [esp + 0x2c]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp + 0x10], 0
        mov    dword ptr [ebp + 0xc], 0
        mov    eax, dword ptr [esi + 0xd6]
        or     eax, eax
        jne    loc_1_80A0
        mov    dword ptr [esi + 0xd6], ebp
        mov    dword ptr [esi + 0xda], ebp
        mov    dword ptr [ebp], 0
        jmp    loc_1_80A9
loc_1_80A0:
        mov    dword ptr [ebp], eax
        mov    dword ptr [esi + 0xd6], ebp
loc_1_80A9:
        mov    dword ptr [esi + 0x58], ebp
        mov    dword ptr [esi + 0x68], 0x7d2c ; -> sub_1_7D2C
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esi + 0x5c], eax
        sub    edi, dword ptr [esp + 0x10]
        je     loc_1_80E2
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_1_7B05
        add    esp, 8
        push   edi
        push   eax
        push   dword ptr [esp + 0x40]
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        jmp    loc_1_80EB
loc_1_80E2:
        push   ebx
        call   sub_1_95BA
        add    esp, 4
loc_1_80EB:
        mov    eax, dword ptr [esp + 0x10]
        jmp    loc_1_81E7
loc_1_80F4:
        push   ebp
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [esp + 0x18], 0x17
        jne    loc_1_810C
        mov    dword ptr [esp + 0x10], 0
loc_1_810C:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esi + 0x5c], eax
        cmp    dword ptr [esi + 0xd6], 0
        je     loc_1_8123
        mov    dword ptr [esi + 0x68], 0x7d2c ; -> sub_1_7D2C
loc_1_8123:
        cmp    edi, dword ptr [esp + 0x10]
        jne    loc_1_813B
        push   ebx
        call   sub_1_95BA
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        jmp    loc_1_81E7
loc_1_813B:
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_1_7B05
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esi + 0xd6], 0
        jne    loc_1_81BC
        mov    dword ptr [esi + 0x68], 0x7cca ; -> loc_1_7CCA
        mov    eax, edi
        sub    eax, dword ptr [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ebx
        call   sub_1_7B46
        add    esp, 0xc
        or     eax, eax
        je     loc_1_81AD
        mov    ecx, dword ptr [esi + 0xe2]
        or     ecx, ecx
        jne    loc_1_818F
        mov    dword ptr [esi + 0xe2], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xe6], eax
        jmp    loc_1_819B
loc_1_818F:
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 0xe2], eax
loc_1_819B:
        mov    eax, edi
        sub    eax, dword ptr [esp + 0x10]
        add    dword ptr [esi + 0xde], eax
        mov    dword ptr [esp + 0x10], edi
        jmp    loc_1_81B6
loc_1_81AD:
        push   ebx
        call   sub_1_95BA
        add    esp, 4
loc_1_81B6:
        mov    eax, dword ptr [esp + 0x10]
        jmp    loc_1_81E7
loc_1_81BC:
        sub    edi, dword ptr [esp + 0x10]
        push   edi
        push   ebx
        push   dword ptr [esp + 0x40]
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0x10
        jmp    loc_1_81E7
loc_1_81D9:
        push   dword ptr [esp + 0x3c]
        call   sub_1_95BA
        add    esp, 4
        sub    eax, eax
loc_1_81E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
sub_1_81EF:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0xc], 0
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x28]
loc_1_8203:
        mov    ecx, dword ptr [esi + 0xde]
        mov    dword ptr [esi + 0x5c], ecx
        or     dword ptr [esi + 0x60], 0x80
        mov    edx, dword ptr [esi + 0xe2]
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x10]
        push   dword ptr [edx + 8]
        push   eax
        push   ecx
        push   dword ptr [edx + 0xc]
        push   0x230
        push   dword ptr [esi + 0xee]
        push   dword ptr [esi + 0x116]
        call   dword ptr [esi + 0x112]
        add    esp, 0x20
        and    dword ptr [esi + 0x60], 0xffffff7f
        cmp    eax, 0x18
        je     loc_1_82B9
        cmp    eax, 0x17
        jne    loc_1_8259
        mov    dword ptr [esp + 0xc], edi
loc_1_8259:
        mov    eax, dword ptr [esp + 0xc]
        sub    dword ptr [esi + 0x5c], eax
        mov    ebx, dword ptr [esi + 0xde]
        mov    eax, dword ptr [esi + 0xe2]
        cmp    ebx, dword ptr [esp + 0xc]
        je     loc_1_837F
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_1_7B05
        add    esp, 8
        cmp    dword ptr [esi + 0xd6], edi
        jne    loc_1_83C0
        mov    dword ptr [esi + 0xe2], eax
        mov    eax, dword ptr [esp + 0xc]
        sub    dword ptr [esi + 0xde], eax
        cmp    dword ptr [esi + 0x5c], edi
        jne    loc_1_83F3
        cmp    dword ptr [esi + 0x112], edi
        jne    loc_1_8203
        jmp    loc_1_83F3
loc_1_82B9:
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x10], 0
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, dword ptr [esi + 0xd6]
        or     eax, eax
        jne    loc_1_8316
        mov    dword ptr [esi + 0xd6], ecx
        mov    dword ptr [esi + 0xda], ecx
        mov    dword ptr [ecx], 0
        jmp    loc_1_831E
loc_1_8316:
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0xd6], ecx
loc_1_831E:
        mov    dword ptr [esi + 0x58], ecx
        mov    dword ptr [esi + 0x68], 0x7d2c ; -> sub_1_7D2C
        mov    edi, dword ptr [esi + 0xde]
        mov    eax, dword ptr [esi + 0xe2]
        mov    dword ptr [esi + 0xde], 0
        mov    dword ptr [esi + 0xe2], 0
        mov    ecx, dword ptr [esp + 0xc]
        sub    dword ptr [esi + 0x5c], ecx
        sub    edi, dword ptr [esp + 0xc]
        je     loc_1_8377
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_1_7B05
        add    esp, 8
        push   edi
        push   eax
        push   0x800
        push   esi
        call   sub_1_7D2C
        add    esp, 0x10
        jmp    loc_1_83FF
loc_1_8377:
        push   eax
        call   sub_1_95BA
        jmp    loc_1_83FC
loc_1_837F:
        mov    dword ptr [esi + 0xde], 0
        mov    dword ptr [esi + 0xe2], 0
        cmp    dword ptr [esi + 0xd6], 0
        jne    loc_1_83AE
        cmp    dword ptr [esi + 0x112], 0
        je     loc_1_83B5
        mov    dword ptr [esi + 0x68], 0x7fd3 ; -> loc_1_7FD3
        jmp    loc_1_83B5
loc_1_83AE:
        mov    dword ptr [esi + 0x68], 0x7d2c ; -> sub_1_7D2C
loc_1_83B5:
        push   eax
        call   sub_1_95BA
        add    esp, 4
        jmp    loc_1_83F3
loc_1_83C0:
        mov    dword ptr [esi + 0x68], 0x7d2c ; -> sub_1_7D2C
        mov    dword ptr [esi + 0xde], 0
        mov    dword ptr [esi + 0xe2], 0
        sub    ebx, dword ptr [esp + 0xc]
        push   ebx
        push   eax
        push   0x800
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 0x1c], eax
        add    esp, 0x10
loc_1_83F3:
        push   dword ptr [esp + 0x2c]
        call   sub_1_7AA0
loc_1_83FC:
        add    esp, 4
loc_1_83FF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_8406:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [esp + 0x34]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x30]
        je     loc_1_8449
        cmp    dword ptr [esi + 0x122], 0
        jne    loc_1_8430
        test   byte ptr [esi + 0x61], 0x40
        je     loc_1_847D
loc_1_8430:
        add    esi, 0x122
        cmp    dword ptr [esi], 0
        je     loc_1_8442
loc_1_843B:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_1_843B
loc_1_8442:
        mov    dword ptr [esi], edi
        jmp    loc_1_871E
loc_1_8449:
        mov    edi, dword ptr [esi + 0x122]
        or     edi, edi
        je     loc_1_871E
        test   byte ptr [esi + 0x61], 0x40
        jne    loc_1_871E
        mov    ebx, dword ptr [esi + 0x126]
        mov    dword ptr [esi + 0x122], 0
        mov    dword ptr [esi + 0x126], 0
        jmp    loc_1_8481
loc_1_847D:
        mov    ebx, dword ptr [esp + 0x38]
loc_1_8481:
        or     dword ptr [esi + 0x60], 0x4000
        inc    byte ptr [esi + 0xba]
        or     dword ptr [esi + 0x64], 1
loc_1_8492:
        mov    dword ptr [esp + 0x10], 0
        mov    ebp, dword ptr [esi + 0x11e]
        or     ebp, ebp
        je     loc_1_84AF
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esi + 0x11e], eax
        jmp    loc_1_852D
loc_1_84AF:
        mov    ebp, dword ptr [esi + 0xd6]
        lea    ecx, [esi + 0xd6]
        or     ebp, ebp
        je     loc_1_8578
        mov    eax, 0x40
        sub    edx, edx
loc_1_84CA:
        test   dword ptr [ebp + 0x14], eax
        je     loc_1_84D4
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_84DF
loc_1_84D4:
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        cmp    ebp, edx
        jne    loc_1_84CA
        jmp    loc_1_852D
loc_1_84DF:
        cmp    dword ptr [esi + 0xda], ebp
        jne    loc_1_84FF
        cmp    dword ptr [esi + 0xd6], ebp
        mov    dword ptr [esi + 0xda], 0
        je     loc_1_84FF
        mov    dword ptr [esi + 0xda], ecx
loc_1_84FF:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 0x58], ebp
        jne    loc_1_852D
        mov    dword ptr [esi + 0x58], eax
        or     eax, eax
        jne    loc_1_852D
        cmp    dword ptr [esi + 0x112], 0
        je     loc_1_8526
        cmp    dword ptr [esi + 0x5c], 0
        mov    dword ptr [esi + 0x68], 0x7fd3 ; -> loc_1_7FD3
        je     loc_1_852D
loc_1_8526:
        mov    dword ptr [esi + 0x68], 0x7cca ; -> loc_1_7CCA
loc_1_852D:
        or     ebp, ebp
        je     loc_1_8578
        push   0
        push   ebx
        push   dword ptr [ebp + 0x20]
        push   edi
        call   sub_1_442C
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   dword ptr [ebp + 4]
        add    esp, 0xc
        push   ebp
        call   sub_1_7AA0
        add    esp, 4
        mov    eax, dword ptr [esi + 0x12a]
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, eax
        cmp    ecx, edx
        jb     loc_1_856B
        mov    ecx, edx
loc_1_856B:
        sub    eax, ecx
        mov    dword ptr [esi + 0x12a], eax
        jmp    loc_1_866C
loc_1_8578:
        cmp    dword ptr [esi + 0x12a], 0
        jne    loc_1_86E6
        mov    ecx, dword ptr [esi + 0xee]
        mov    eax, dword ptr [esi + 0x102]
        or     eax, eax
        je     loc_1_86E6
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_86E6
        test   byte ptr [eax + 0x40], 0x80
        je     loc_1_86E6
        mov    edx, dword ptr [eax + 0x82]
        or     edx, edx
        je     loc_1_86E6
        mov    eax, dword ptr [eax + 0x86]
        mov    dword ptr [esi + 0x12a], ebx
        lea    ebp, [esp + 0x18]
        push   ebp
        lea    ebp, [esp + 0x14]
        push   dword ptr [edi + 8]
        push   ebp
        push   ebx
        push   dword ptr [edi + 0xc]
        push   0x250
        push   ecx
        push   eax
        call   edx
        add    esp, 0x20
        cmp    eax, 0x18
        jne    loc_1_8645
        cmp    dword ptr [esp + 0x10], ebx
        mov    eax, 0
        je     loc_1_8617
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_7B05
        add    esp, 8
        push   0
        mov    edi, eax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   edi
        call   sub_1_442C
        add    esp, 0x10
loc_1_8617:
        add    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [esi + 0x12a]
        mov    edx, ecx
        mov    ebp, dword ptr [esp + 0x10]
        cmp    ebp, ecx
        jb     loc_1_862D
        mov    ebp, ecx
loc_1_862D:
        sub    edx, ebp
        push   eax
        push   0
        mov    dword ptr [esi + 0x12a], edx
        push   dword ptr [esp + 0x2c]
        call   dword ptr [esp + 0x2c]
        add    esp, 0xc
        jmp    loc_1_866C
loc_1_8645:
        cmp    eax, 0x17
        jne    loc_1_8652
        mov    dword ptr [esp + 0x10], 0
loc_1_8652:
        mov    eax, dword ptr [esi + 0x12a]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 0x10]
        cmp    edx, eax
        jb     loc_1_8664
        mov    edx, eax
loc_1_8664:
        sub    ecx, edx
        mov    dword ptr [esi + 0x12a], ecx
loc_1_866C:
        cmp    dword ptr [esp + 0x10], ebx
        jne    loc_1_868F
        push   edi
        call   sub_1_95BA
        add    esp, 4
        mov    edi, dword ptr [esi + 0x122]
        mov    ebx, dword ptr [esi + 0x126]
        lea    ebp, [esi + 0x126]
        jmp    loc_1_86CB
loc_1_868F:
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_7B05
        add    esp, 8
        mov    edi, eax
        lea    ebp, [esi + 0x126]
        sub    ebx, dword ptr [esp + 0x10]
        add    ebx, dword ptr [ebp]
        lea    eax, [esi + 0x122]
        cmp    dword ptr [esi + 0x122], 0
        je     loc_1_86CB
        push   edi
        call   sub_1_7AF4
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x122]
        mov    dword ptr [eax], ecx
loc_1_86CB:
        mov    dword ptr [esi + 0x122], 0
        mov    dword ptr [ebp], 0
        or     edi, edi
        jne    loc_1_8492
        jmp    loc_1_8703
loc_1_86E6:
        push   edi
        call   sub_1_7AF4
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x122]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x122], edi
        add    dword ptr [esi + 0x126], ebx
loc_1_8703:
        and    dword ptr [esi + 0x60], 0xffffbfff
        mov    al, byte ptr [esi + 0xba]
        dec    al
        mov    byte ptr [esi + 0xba], al
        jne    loc_1_871E
        and    dword ptr [esi + 0x64], 0xfffffffe
loc_1_871E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_8726:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx + 0xd6]
        mov    eax, esi
        or     eax, eax
        je     loc_1_8744
loc_1_8737:
        or     dword ptr [eax + 0x14], 0x80000000
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_8737
loc_1_8744:
        mov    ecx, dword ptr [edx + 0x11e]
        mov    eax, ecx
        or     eax, eax
        je     loc_1_875D
loc_1_8750:
        or     dword ptr [eax + 0x14], 0x80000000
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_8750
loc_1_875D:
        or     esi, esi
        jne    loc_1_8765
        or     ecx, ecx
        je     loc_1_8773
loc_1_8765:
        push   0x100
        push   edx
        call   sub_1_8EB3
        add    esp, 8
loc_1_8773:
        pop    esi
        ret    
sub_1_8775:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x20]
        je     loc_1_87D3
        lea    eax, [esi + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_87C3
        mov    dword ptr [ebx + 0xc], esi
        mov    dword ptr [ebx + 4], 5
        lea    eax, [ebx + 0x10]
        mov    dword ptr [ebx + 8], eax
        push   dword ptr [esp + 0x1c]
        push   esi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_44E1
        add    esp, 0x10
        mov    dword ptr [edi], ebx
        jmp    loc_1_87D3
loc_1_87C3:
        mov    dword ptr [edi], 0
        mov    ebx, dword ptr [esp + 0x28]
        or     ebx, ebx
        je     loc_1_882D
        jmp    loc_1_8827
loc_1_87D3:
        mov    ebx, dword ptr [esp + 0x28]
        or     ebx, ebx
        je     loc_1_882D
        mov    edi, dword ptr [esp + 0x18]
        sub    edi, esi
        sub    edi, dword ptr [esp + 0x1c]
        lea    eax, [edi + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_8827
        mov    dword ptr [ebp + 0xc], edi
        mov    dword ptr [ebp + 4], 5
        lea    eax, [ebp + 0x10]
        mov    dword ptr [ebp + 8], eax
        mov    ecx, dword ptr [esp + 0x1c]
        add    ecx, esi
        push   ecx
        push   edi
        mov    dword ptr [ebp], 0
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_44E1
        add    esp, 0x10
        mov    dword ptr [ebx], ebp
        jmp    loc_1_882D
loc_1_8827:
        mov    dword ptr [ebx], 0
loc_1_882D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_8832:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi + 0x60]
        test   ch, 0x20
        je     loc_1_886C
        mov    edi, dword ptr [esp + 0x50]
        mov    ebx, dword ptr [edi + 0xc]
        or     ebx, ebx
        je     loc_1_8860
        mov    edx, dword ptr [esp + 0x58]
        cmp    dword ptr [edx], ebx
        jb     loc_1_8860
        mov    ebp, 1
        dec    ebx
        jmp    loc_1_8882
loc_1_8860:
        and    dword ptr [edi + 0x10], 0xffffdfff
        jmp    loc_1_8A74
loc_1_886C:
        mov    edi, dword ptr [esp + 0x50]
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0xc]
        mov    edx, dword ptr [esp + 0x58]
        inc    ebp
        mov    eax, dword ptr [edx]
        cmp    ebp, eax
        jb     loc_1_8882
        mov    ebp, eax
loc_1_8882:
        mov    edx, dword ptr [esp + 0x58]
        mov    eax, dword ptr [edx]
        sub    eax, ebp
        sub    eax, ebx
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [edi]
        add    eax, ebx
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [ebp + eax - 1]
        mov    dword ptr [esp + 0x14], eax
        test   ecx, 0x4000000
        jne    loc_1_8A31
        mov    dword ptr [esp + 0x10], 0
        test   byte ptr [esi + 0x61], 0x80
        je     loc_1_897E
        mov    ecx, dword ptr [esi + 0x48]
        mov    eax, dword ptr [esi + 0x12e]
        mov    edx, ecx
        sub    edx, eax
        jns    loc_1_897E
        mov    ebx, dword ptr [esi + 0x132]
        mov    ebp, ebx
        sub    ebp, eax
        mov    edx, dword ptr [esp + 0x58]
        inc    ebp
        mov    edx, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, edx
        jb     loc_1_88EA
        mov    eax, edx
loc_1_88EA:
        mov    dword ptr [esp + 0x24], eax
        sub    ecx, ebx
        lea    ebx, [edx + ecx]
        or     ebx, ebx
        jne    loc_1_88F9
        sub    ebx, ebx
loc_1_88F9:
        lea    eax, [esp + 0x10]
        or     ebx, ebx
        jne    loc_1_8903
        sub    eax, eax
loc_1_8903:
        push   eax
        push   0
        push   ebp
        push   dword ptr [esp + 0x30]
        push   edx
        push   dword ptr [esp + 0x68]
        call   sub_1_8775
        add    esp, 0x18
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_8968
        mov    eax, dword ptr [esi + 0x132]
        inc    eax
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x34], eax
        test   byte ptr [esi + 0x61], 0x20
        je     loc_1_8947
        inc    dword ptr [esp + 0x34]
loc_1_8947:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 0x30], eax
        push   ebx
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_9770
        add    esp, 0x10
loc_1_8968:
        mov    edx, dword ptr [esp + 0x58]
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edx], eax
        and    dword ptr [edi + 0x10], 0xffffdfff
        jmp    loc_1_8A74
loc_1_897E:
        lea    eax, [esp + 0x10]
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_898B
        sub    eax, eax
loc_1_898B:
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   ebp
        mov    edx, dword ptr [esp + 0x64]
        push   ebx
        push   dword ptr [edx]
        push   dword ptr [esp + 0x68]
        call   sub_1_8775
        add    esp, 0x18
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_89E6
        mov    eax, dword ptr [edi]
        add    eax, ebp
        add    eax, ebx
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [edi + 0x10]
        and    eax, 0xffffdfff
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 0x30], eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_9770
        add    esp, 0x10
loc_1_89E6:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_8A29
        mov    eax, dword ptr [esi + 0x60]
        test   ah, 0x80
        jne    loc_1_8A07
        or     eax, 0x8000
        mov    dword ptr [esi + 0x60], eax
        inc    byte ptr [esi + 0xba]
        or     dword ptr [esi + 0x64], 1
loc_1_8A07:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x12e], eax
        mov    eax, dword ptr [esp + 0x14]
        push   ebp
        mov    dword ptr [esi + 0x132], eax
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_8406
        add    esp, 0xc
loc_1_8A29:
        mov    edx, dword ptr [esp + 0x58]
        mov    dword ptr [edx], ebx
        jmp    loc_1_8A74
loc_1_8A31:
        and    dword ptr [edi + 0x10], 0xffffdfff
        test   ch, 0x80
        je     loc_1_8A4D
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, dword ptr [esi + 0x132]
        or     eax, eax
        jg     loc_1_8A60
        jmp    loc_1_8A74
loc_1_8A4D:
        or     ecx, 0x8000
        mov    dword ptr [esi + 0x60], ecx
        inc    byte ptr [esi + 0xba]
        or     dword ptr [esi + 0x64], 1
loc_1_8A60:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x12e], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x132], eax
loc_1_8A74:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
loc_1_8A7C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   dword ptr [esi]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_8C8E
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     loc_1_8C87
        mov    edi, dword ptr [esp + 0x18]
        mov    al, byte ptr [ebx + 0xbc]
        mov    bp, word ptr [edi]
        cmp    al, 4
        jb     loc_1_8AB5
        cmp    al, 6
        jbe    loc_1_8ADD
loc_1_8AB5:
        cmp    dword ptr [ebx + 0xde], 0
        je     loc_1_8AC5
        test   bp, 0x20
        jne    loc_1_8ADD
loc_1_8AC5:
        cmp    dword ptr [ebx + 0x126], 0
        je     loc_1_8C80
        test   bp, 0x40
        je     loc_1_8C80
loc_1_8ADD:
        test   byte ptr [ebx + 0xbe], 1
        jne    loc_1_8C80
        call   sub_1_7AB2
        or     eax, eax
        je     loc_1_8C79
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 0x1c], ecx
        movzx  ecx, bp
        mov    dword ptr [eax + 0x18], edi
        mov    dword ptr [eax + 0x10], 0
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 0x14], ecx
        mov    cx, bp
        and    cl, 0x60
        cmp    cl, 0x40
        je     loc_1_8C2C
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebx + 0xd6], 0
        jne    loc_1_8B50
        mov    dword ptr [ebx + 0xd6], eax
        jmp    loc_1_8B58
loc_1_8B50:
        mov    ecx, dword ptr [ebx + 0xda]
        mov    dword ptr [ecx], eax
loc_1_8B58:
        mov    dword ptr [ebx + 0xda], eax
        test   bp, 0x40
        je     loc_1_8B94
        cmp    dword ptr [ebx + 0x126], 0
        je     loc_1_8B94
        inc    dword ptr [ebx + 0xa6]
        push   0
        push   0
        push   ebx
        call   sub_1_8406
        add    esp, 0xc
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8B94:
        cmp    dword ptr [ebx + 0x58], 0
        jne    loc_1_8C25
        mov    dword ptr [ebx + 0x58], eax
        test   byte ptr [ebx + 0x60], 0x80
        jne    loc_1_8C25
        mov    dword ptr [ebx + 0x68], 0x7d2c ; -> sub_1_7D2C
        mov    eax, dword ptr [ebx + 0xe2]
        or     eax, eax
        jne    loc_1_8BC2
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8BC2:
        mov    ecx, dword ptr [ebx + 0xde]
        mov    dword ptr [ebx + 0xe2], 0
        mov    dword ptr [ebx + 0xde], 0
        inc    dword ptr [ebx + 0xa6]
        push   ecx
        push   eax
        push   0x800
        push   ebx
        call   sub_1_7D2C
        add    esp, 0x10
        mov    esi, dword ptr [ebx + 0x4c]
        push   ebx
        call   sub_1_A8F8
        add    esp, 4
        cmp    eax, esi
        je     loc_1_8C10
        push   0x200
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_8C10:
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        call   sub_1_8DFA
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8C25:
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8C2C:
        lea    ecx, [ebx + 0x11e]
        cmp    dword ptr [ecx], 0
        je     loc_1_8C3E
loc_1_8C37:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    loc_1_8C37
loc_1_8C3E:
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebx + 0x122], 0
        je     loc_1_8C72
        inc    dword ptr [ebx + 0xa6]
        push   0
        push   0
        push   ebx
        call   sub_1_8406
        add    esp, 0xc
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8C72:
        mov    eax, 0xff
        jmp    loc_1_8C93
loc_1_8C79:
        mov    eax, 1
        jmp    loc_1_8C93
loc_1_8C80:
        mov    eax, 0x19
        jmp    loc_1_8C93
loc_1_8C87:
        mov    eax, 0x19
        jmp    loc_1_8C93
loc_1_8C8E:
        mov    eax, 0xf
loc_1_8C93:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8CA0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_8DF5
        mov    eax, dword ptr [edi + 0x60]
        test   al, 0x40
        jne    loc_1_8DF5
        cmp    dword ptr [edi + 0xd6], 0
        je     loc_1_8DF5
        or     eax, 0x40
        mov    dword ptr [edi + 0x60], eax
        mov    ebx, 0x80000000
        sub    ebp, ebp
loc_1_8CDA:
        mov    esi, dword ptr [edi + 0xd6]
        mov    eax, ebp
        test   dword ptr [esi + 0x14], ebx
        je     loc_1_8D23
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi + 0x58], esi
        jne    loc_1_8CF1
        mov    dword ptr [edi + 0x58], eax
loc_1_8CF1:
        mov    dword ptr [edi + 0xd6], eax
        cmp    eax, ebp
        jne    loc_1_8D02
        mov    dword ptr [edi + 0x68], 0x7cca ; -> loc_1_7CCA
loc_1_8D02:
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, ebp
        je     loc_1_8D0E
        mov    word ptr [eax], 0x30
loc_1_8D0E:
        push   dword ptr [esi + 0xc]
        push   ebp
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0xc
        mov    eax, esi
        mov    esi, dword ptr [edi + 0xd6]
loc_1_8D23:
        cmp    esi, ebp
        jne    loc_1_8D68
        cmp    dword ptr [edi + 0xe2], ebp
        je     loc_1_8D72
        cmp    dword ptr [edi + 0x112], ebp
        je     loc_1_8DE8
        cmp    dword ptr [edi + 0x5c], ebp
        jne    loc_1_8DE8
        push   eax
        push   edi
        call   sub_1_81EF
        add    esp, 8
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        cmp    dword ptr [edi + 0xd6], ebp
        je     loc_1_8DF1
        jmp    loc_1_8CDA
loc_1_8D68:
        or     eax, eax
        je     loc_1_8DF1
        jmp    loc_1_8DE8
loc_1_8D72:
        push   eax
        call   sub_1_7AA0
        add    esp, 4
        mov    eax, dword ptr [edi + 0x60]
        test   eax, 0x1100000
        jne    loc_1_8DA6
        cmp    dword ptr [edi + 0x112], 0
        je     loc_1_8D9D
        cmp    dword ptr [edi + 0x5c], 0
        jne    loc_1_8D9D
        mov    dword ptr [edi + 0x68], 0x7fd3 ; -> loc_1_7FD3
        jmp    loc_1_8DF1
loc_1_8D9D:
        mov    dword ptr [edi + 0x68], 0x7cca ; -> loc_1_7CCA
        jmp    loc_1_8DF1
loc_1_8DA6:
        test   eax, 0x1000000
        je     loc_1_8DCC
        and    eax, 0xffffffbf
        mov    dword ptr [edi + 0x60], eax
        inc    dword ptr [edi + 0xa6]
        push   1
        and    eax, 0x200000
        push   eax
        push   edi
        call   sub_1_7998
        add    esp, 0xc
        jmp    loc_1_8DF5
loc_1_8DCC:
        test   eax, 0x100000
        je     loc_1_8DF1
        and    eax, 0xffefffff
        mov    dword ptr [edi + 0x60], eax
        push   0x16
        push   edi
        call   sub_1_78CF
        add    esp, 8
        jmp    loc_1_8DF1
loc_1_8DE8:
        push   eax
        call   sub_1_7AA0
        add    esp, 4
loc_1_8DF1:
        and    dword ptr [edi + 0x60], 0xffffffbf
loc_1_8DF5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_8DFA:
        push   esi
        push   edi
        cmp    dword ptr [0x13640], 0x13640  ; -> loc_1_13640
        je     loc_1_8EB0
        mov    edi, 0x13640                  ; -> loc_1_13640
loc_1_8E11:
        mov    esi, dword ptr [0x13640]      ; -> loc_1_13640
        mov    eax, esi
        sub    esi, 0x106
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x13640], eax      ; -> loc_1_13640
        mov    eax, dword ptr [esi + 0x106]
        mov    dword ptr [eax + 4], edi
        test   byte ptr [esi + 0xbe], 1
        jne    loc_1_8E97
loc_1_8E38:
        mov    eax, dword ptr [esi + 0x60]
        test   ah, 7
        je     loc_1_8E97
        test   ah, 1
        je     loc_1_8E56
        and    eax, 0xfffffeff
        mov    dword ptr [esi + 0x60], eax
        push   esi
        call   sub_1_8CA0
        add    esp, 4
loc_1_8E56:
        mov    eax, dword ptr [esi + 0x60]
        test   ah, 4
        je     loc_1_8E75
        and    eax, 0xfffffbff
        mov    dword ptr [esi + 0x60], eax
        inc    dword ptr [esi + 0xa6]
        push   esi
        call   sub_1_B37E
        add    esp, 4
loc_1_8E75:
        mov    eax, dword ptr [esi + 0x60]
        test   ah, 2
        je     loc_1_8E8E
        and    eax, 0xfffffdff
        mov    dword ptr [esi + 0x60], eax
        push   esi
        call   sub_1_ABE1
        add    esp, 4
loc_1_8E8E:
        test   byte ptr [esi + 0xbe], 1
        je     loc_1_8E38
loc_1_8E97:
        and    dword ptr [esi + 0x60], 0xffffffdf
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        cmp    dword ptr [0x13640], edi      ; -> loc_1_13640
        jne    loc_1_8E11
loc_1_8EB0:
        pop    edi
        pop    esi
        ret    
sub_1_8EB3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 0x60]
        or     ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0x60], ecx
        test   cl, 0x20
        jne    loc_1_8EF6
        or     ecx, 0x20
        mov    dword ptr [edx + 0x60], ecx
        inc    dword ptr [edx + 0xa6]
        mov    eax, dword ptr [0x13644]      ; -> loc_1_13644
        lea    ecx, [edx + 0x106]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x13644]      ; -> loc_1_13644
        mov    dword ptr [edx + 0x10a], eax
        mov    dword ptr [0x13644], ecx      ; -> loc_1_13644
        mov    dword ptr [ecx], 0x13640      ; -> loc_1_13640
loc_1_8EF6:
        ret    
sub_1_8EF7:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        test   al, 3
        je     loc_1_8FC4
        test   al, 0x10
        jne    loc_1_8FC4
        or     eax, 0x10
        mov    esi, 0x13948                  ; -> loc_1_13948
        mov    edi, 0x13720                  ; -> loc_1_13720
loc_1_8F1C:
        test   al, 1
        je     loc_1_8F62
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
        cmp    dword ptr [0x13948], esi      ; -> loc_1_13948
        je     loc_1_8F5F
        mov    ebx, dword ptr [0x13948]      ; -> loc_1_13948
        mov    eax, ebx
        push   0
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x13948], eax      ; -> loc_1_13948
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], esi
        push   dword ptr [ebx + 0x10]
        push   dword ptr [ebx + 0xc]
        call   dword ptr [ebx + 8]
        add    esp, 0xc
        push   ebx
        call   sub_1_4723
        add    esp, 4
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        jmp    loc_1_8F62
loc_1_8F5F:
        and    eax, 0xfffffffe
loc_1_8F62:
        test   al, 2
        je     loc_1_8FB4
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
        cmp    dword ptr [0x13720], edi      ; -> loc_1_13720
        jne    loc_1_8F78
        and    eax, 0xfffffffd
        jmp    loc_1_8FB4
loc_1_8F78:
        mov    ebx, dword ptr [0x13720]      ; -> loc_1_13720
        mov    eax, ebx
        mov    ecx, 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x13720], eax      ; -> loc_1_13720
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    eax, dword ptr [ebx + 0x10]
        or     eax, eax
        jne    loc_1_8F9B
        mov    ecx, dword ptr [ebx + 0x14]
loc_1_8F9B:
        push   ecx
        push   eax
        push   dword ptr [ebx + 0xc]
        call   dword ptr [ebx + 8]
        add    esp, 0xc
        push   ebx
        call   sub_1_A7D4
        add    esp, 4
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
loc_1_8FB4:
        test   al, 3
        jne    loc_1_8F1C
        and    eax, 0xffffffef
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
loc_1_8FC4:
        call   sub_1_8DFA
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_8FCD:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0xea]
        or     esi, esi
        je     loc_1_902A
        mov    dword ptr [eax + 0xea], 0
        push   dword ptr [eax + 0x8c]
        add    eax, 0x7c
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_463E
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        or     eax, 1
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
        mov    eax, dword ptr [0x1394c]      ; -> loc_1_1394C
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [0x1394c]      ; -> loc_1_1394C
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [0x1394c], esi      ; -> loc_1_1394C
        mov    dword ptr [esi], 0x13948      ; -> loc_1_13948
loc_1_902A:
        pop    esi
        ret    
sub_1_902C:
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x58]
        sub    ebx, ebx
        test   byte ptr [edi + 0x40], 0x80
        jne    loc_1_9044
        jmp    loc_1_92CB
loc_1_9044:
        mov    ecx, dword ptr [edi + 0x4a]
        or     ecx, ecx
        mov    esi, dword ptr [esp + 0x5c]
        je     loc_1_914E
loc_1_9053:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_913A
        cmp    byte ptr [eax + 0xbc], 1
        jne    loc_1_913A
        test   byte ptr [eax + 0xbe], 1
        jne    loc_1_913A
        mov    dl, byte ptr [esi]
        cmp    byte ptr [eax + 0x7c], dl
        jne    loc_1_913A
        mov    dl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 0x7d], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x7e]
        cmp    byte ptr [esi + 2], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x7f]
        cmp    byte ptr [esi + 3], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x80]
        cmp    byte ptr [esi + 4], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x81]
        cmp    byte ptr [esi + 5], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x82]
        cmp    byte ptr [esi + 6], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x83]
        cmp    byte ptr [esi + 7], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x84]
        cmp    byte ptr [esi + 8], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x85]
        cmp    byte ptr [esi + 9], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x86]
        cmp    byte ptr [esi + 0xa], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x87]
        cmp    byte ptr [esi + 0xb], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x88]
        cmp    byte ptr [esi + 0xc], dl
        jne    loc_1_913A
        mov    dl, byte ptr [eax + 0x89]
        cmp    byte ptr [esi + 0xd], dl
        jne    loc_1_913A
        mov    dl, byte ptr [esi + 0xe]
        cmp    byte ptr [eax + 0x8a], dl
        jne    loc_1_913A
        mov    dl, byte ptr [esi + 0xf]
        cmp    byte ptr [eax + 0x8b], dl
        jne    loc_1_913A
        mov    edx, dword ptr [esp + 0x60]
        cmp    dword ptr [eax + 0x8c], edx
        je     loc_1_9147
loc_1_913A:
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    loc_1_9053
        jmp    loc_1_9152
loc_1_9147:
        mov    ebx, 1
        jmp    loc_1_9152
loc_1_914E:
        mov    eax, dword ptr [esp + 0x1c]
loc_1_9152:
        or     ebx, ebx
        je     loc_1_9175
        mov    ecx, dword ptr [eax + 0xea]
        inc    dword ptr [eax + 0xa6]
        test   byte ptr [ecx + 0x18], 1
        jne    loc_1_92CD
        or     dword ptr [eax + 0x60], 4
        jmp    loc_1_92CD
loc_1_9175:
        cmp    dword ptr [edi + 0x5a], 0
        je     loc_1_92CB
        call   sub_1_CE5D
        mov    ebx, eax
        call   sub_1_4735
        mov    ebp, eax
        or     ebx, ebx
        je     loc_1_92B1
        or     ebp, ebp
        je     loc_1_92BE
        mov    eax, dword ptr [edi + 0x5a]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [edi + 0x5e]
        mov    dword ptr [esp + 0x18], eax
        push   dword ptr [esp + 0x60]
        lea    eax, [esp + 0x34]
        push   esi
        push   eax
        call   sub_1_460F
        add    esp, 0xc
        inc    word ptr [edi + 0x46]
        mov    byte ptr [ebx + 0xbc], 1
        mov    dword ptr [ebx + 0xea], ebp
        mov    al, byte ptr [edi + 0x42]
        mov    byte ptr [ebx + 0x99], al
        lea    eax, [esp + 0x20]
        push   eax
        or     dword ptr [ebx + 0x60], 4
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x44]
        push   0
        push   eax
        push   0x22
        push   dword ptr [esp + 0x38]
        call   dword ptr [esp + 0x38]
        add    esp, 0x24
        cmp    eax, 0x18
        jne    loc_1_9297
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, 0xf
        mov    esi, dword ptr [edi + 0x4a]
        or     esi, esi
        je     loc_1_9275
        mov    ecx, 3
loc_1_922E:
        mov    edx, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0x14], edx
        jne    loc_1_923C
        test   byte ptr [esi + 0xd], cl
        je     loc_1_9245
loc_1_923C:
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    loc_1_922E
        jmp    loc_1_9271
loc_1_9245:
        mov    dword ptr [ebx + 0xa6], 1
        push   dword ptr [esp + 0x28]
        push   ebx
        push   esi
        call   sub_1_4E04
        add    esp, 0xc
        or     eax, eax
        je     loc_1_9265
        sub    esi, esi
        jmp    loc_1_9271
loc_1_9265:
        mov    dword ptr [esi], ebx
        mov    dword ptr [ebx + 0x102], esi
        inc    word ptr [esi + 0xe]
loc_1_9271:
        or     esi, esi
        jne    loc_1_928A
loc_1_9275:
        push   eax
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        push   ebx
        call   sub_1_CF22
        add    esp, 4
        sub    ebx, ebx
loc_1_928A:
        push   edi
        call   sub_1_4325
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_92CD
loc_1_9297:
        push   ebp
        call   sub_1_4723
        add    esp, 4
        push   ebx
        call   sub_1_CF22
        add    esp, 4
        push   edi
        call   sub_1_4325
        jmp    loc_1_92C8
loc_1_92B1:
        or     ebp, ebp
        je     loc_1_92BE
        push   ebp
        call   sub_1_4723
        add    esp, 4
loc_1_92BE:
        or     ebx, ebx
        je     loc_1_92CB
        push   ebx
        call   sub_1_CF22
loc_1_92C8:
        add    esp, 4
loc_1_92CB:
        sub    eax, eax
loc_1_92CD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
sub_1_92D5:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [eax + 0x11], 4
        jne    loc_1_9300
        cmp    byte ptr [esi + 0xbc], 0xa
        jne    loc_1_92F7
        mov    dword ptr [esi + 0xc8], 0x3e8
loc_1_92F7:
        push   esi
        call   sub_1_ABE1
        add    esp, 4
loc_1_9300:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    esi
        ret    
sub_1_930B:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [esp + 0x28]
        sub    ebx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x1c]
        cmp    ebx, eax
        mov    dword ptr [esp + 0x14], eax
        jne    loc_1_93F4
        cmp    byte ptr [esi + 0xbc], 4
        jne    loc_1_93F4
        mov    edi, dword ptr [esi + 0x40]
        lea    eax, [esi + 0x40]
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi + 0x10], edx
        sub    ebp, ebp
        mov    dword ptr [esi + 0xc], edx
        mov    dword ptr [esi + 0x30], 0
        mov    dword ptr [esi + 0x34], 0
        mov    dword ptr [esi + 0x38], 0
        mov    dword ptr [esi + 0x3c], 0
        mov    dword ptr [esi + 0x1c], 0
loc_1_9373:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [ebx + 0x10], ebp
        mov    edx, dword ptr [ebx + 0x18]
        dec    edx
        mov    dword ptr [ebx + 0x18], edx
        cmp    edx, ebp
        mov    ecx, ebp
        setl   cl
        cmp    edx, ebp
        mov    edx, ebp
        setle  dl
        add    ecx, edx
        cmp    ecx, ebp
        je     loc_1_93EB
        mov    ecx, dword ptr [ebx + 0x2c]
        cmp    ecx, ebp
        je     loc_1_93A1
        mov    dword ptr [ecx], ebp
        mov    dword ptr [ebx + 0x2c], ebp
loc_1_93A1:
        mov    ecx, dword ptr [0x13830]      ; -> loc_1_13830
        sub    ecx, dword ptr [ebx + 0x30]
        add    dword ptr [esi + 0xfa], ecx
        mov    ecx, dword ptr [esi + 0xf2]
        mov    edx, dword ptr [ebx + 0x14]
        add    edx, ecx
        cmp    ecx, edx
        mov    ecx, ebp
        seta   cl
        mov    dword ptr [esi + 0xf2], edx
        add    dword ptr [esi + 0xf6], ecx
        mov    ecx, dword ptr [0x13724]      ; -> loc_1_13724
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [0x13724]      ; -> loc_1_13724
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [0x13724], ebx      ; -> loc_1_13724
        mov    dword ptr [ebx], 0x13720      ; -> loc_1_13720
loc_1_93EB:
        cmp    edi, eax
        jne    loc_1_9373
        jmp    loc_1_9573
loc_1_93F4:
        mov    dword ptr [esp + 0x18], ebx
        mov    ecx, dword ptr [esi + 0x40]
        mov    edi, ecx
        mov    dword ptr [esp + 0x10], 0
        or     ebx, ebx
        mov    eax, dword ptr [esp + 0x1c]
        je     loc_1_9456
loc_1_940D:
        lea    edx, [esi + 0x40]
        cmp    edi, edx
        je     loc_1_9456
        mov    eax, edi
        mov    edx, dword ptr [edi + 0x20]
        cmp    ebx, edx
        jb     loc_1_9429
        sub    ebx, edx
        mov    dword ptr [esp + 0x10], edi
        mov    edi, dword ptr [edi]
        jne    loc_1_940D
        jmp    loc_1_9456
loc_1_9429:
        mov    edx, dword ptr [eax + 0x28]
        sub    dword ptr [eax + 0x20], ebx
        mov    ebp, dword ptr [edx + 0xc]
        sub    ebp, dword ptr [eax + 0x24]
        cmp    ebx, ebp
        jae    loc_1_9443
        mov    edx, dword ptr [eax + 0x24]
        add    edx, ebx
        mov    dword ptr [eax + 0x24], edx
        jmp    loc_1_9454
loc_1_9443:
        mov    edx, dword ptr [edx]
        sub    ebx, ebp
        mov    ebp, dword ptr [edx + 0xc]
        cmp    ebx, ebp
        jae    loc_1_9443
        mov    dword ptr [eax + 0x24], ebx
        mov    dword ptr [eax + 0x28], edx
loc_1_9454:
        sub    ebx, ebx
loc_1_9456:
        mov    edx, dword ptr [esp + 0x28]
        sub    edx, dword ptr [esi + 0x10]
        or     edx, edx
        jle    loc_1_94A4
        lea    edx, [esi + 0x40]
        cmp    edi, edx
        je     loc_1_9481
        mov    dword ptr [esi + 0x30], edi
        mov    edx, dword ptr [edi + 0x28]
        mov    dword ptr [esi + 0x34], edx
        mov    edx, dword ptr [edi + 0x24]
        mov    eax, edi
        mov    dword ptr [esi + 0x38], edx
        mov    edx, dword ptr [edi + 0x20]
        mov    dword ptr [esi + 0x3c], edx
        jmp    loc_1_949D
loc_1_9481:
        mov    dword ptr [esi + 0x30], 0
        mov    dword ptr [esi + 0x34], 0
        mov    dword ptr [esi + 0x38], 0
        mov    dword ptr [esi + 0x3c], 0
loc_1_949D:
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 0x10], edx
loc_1_94A4:
        sub    ebx, dword ptr [esp + 0x18]
        add    ebx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x1c], ebx
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 0xc], edx
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_9580
        lea    edx, [esi + 0x40]
        cmp    edi, edx
        jne    loc_1_94CF
        mov    dword ptr [edx], edx
        mov    dword ptr [esi + 0x44], edx
        jmp    loc_1_94D8
loc_1_94CF:
        mov    dword ptr [esi + 0x40], edi
        lea    edx, [esi + 0x40]
        mov    dword ptr [edi + 4], edx
loc_1_94D8:
        cmp    ecx, edi
        je     loc_1_9573
loc_1_94E0:
        mov    ebp, dword ptr [ecx]
        mov    dword ptr [ecx + 0x10], 0
        mov    ebx, dword ptr [ecx + 0x18]
        dec    ebx
        mov    dword ptr [ecx + 0x18], ebx
        or     ebx, ebx
        mov    edx, 0
        setl   dl
        or     ebx, ebx
        mov    ebx, 0
        setle  bl
        add    edx, ebx
        je     loc_1_9569
        mov    edx, dword ptr [ecx + 0x2c]
        or     edx, edx
        je     loc_1_951C
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx + 0x2c], 0
loc_1_951C:
        mov    edx, dword ptr [0x13830]      ; -> loc_1_13830
        sub    edx, dword ptr [eax + 0x30]
        add    dword ptr [esi + 0xfa], edx
        mov    ebx, dword ptr [esi + 0xf2]
        mov    edx, dword ptr [eax + 0x14]
        add    edx, ebx
        mov    dword ptr [esi + 0xf2], edx
        cmp    ebx, edx
        mov    edx, 0
        seta   dl
        add    dword ptr [esi + 0xf6], edx
        mov    edx, dword ptr [0x13724]      ; -> loc_1_13724
        mov    dword ptr [edx], ecx
        mov    edx, dword ptr [0x13724]      ; -> loc_1_13724
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [0x13724], ecx      ; -> loc_1_13724
        mov    dword ptr [ecx], 0x13720      ; -> loc_1_13720
loc_1_9569:
        mov    ecx, ebp
        cmp    ebp, edi
        jne    loc_1_94E0
loc_1_9573:
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        or     eax, 2
        mov    dword ptr [0x13800], eax      ; -> loc_1_13800
loc_1_9580:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_9588:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        mov    eax, dword ptr [esp + 8]
        je     loc_1_95B8
loc_1_9595:
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, ecx
        cmp    edx, esi
        jb     loc_1_95A0
        mov    edx, esi
loc_1_95A0:
        sub    esi, edx
        add    dword ptr [eax + 8], edx
        sub    ecx, edx
        mov    dword ptr [eax + 0xc], ecx
        jne    loc_1_95B4
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_95B8
        mov    eax, ecx
loc_1_95B4:
        or     esi, esi
        jne    loc_1_9595
loc_1_95B8:
        pop    esi
        ret    
sub_1_95BA:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_1_95E3
        mov    edi, 5
loc_1_95C9:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_95DD
        mov    esi, dword ptr [eax]
        push   eax
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_95DF
loc_1_95DD:
        mov    eax, dword ptr [eax]
loc_1_95DF:
        or     eax, eax
        jne    loc_1_95C9
loc_1_95E3:
        pop    edi
        pop    esi
        ret    
sub_1_95E6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0xd2]
        mov    ebx, dword ptr [esi + 0x48]
loc_1_95F7:
        or     edi, edi
        je     loc_1_9653
        mov    ecx, dword ptr [edi + 4]
        mov    eax, ebx
        sub    eax, ecx
        js     loc_1_9653
        mov    eax, dword ptr [edi + 0xc]
        and    eax, 0x100
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        add    eax, dword ptr [edi + 8]
        add    eax, ecx
        mov    ecx, ebx
        sub    ecx, eax
        js     loc_1_965A
        push   dword ptr [edi + 0x14]
        call   sub_1_95BA
        add    esp, 4
        mov    ebp, dword ptr [edi]
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    edi, ebp
        mov    dword ptr [esi + 0xd2], ebp
        or     ebp, ebp
        jne    loc_1_95F7
        mov    al, byte ptr [esi + 0xba]
        dec    al
        mov    byte ptr [esi + 0xba], al
        jne    loc_1_9653
        and    dword ptr [esi + 0x64], 0xfffffffe
loc_1_9653:
        sub    eax, eax
loc_1_9655:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_965A:
        mov    ebp, ebx
        mov    ecx, dword ptr [esp + 0x18]
        sub    ebp, dword ptr [edi + 4]
        mov    dword ptr [ecx], ebx
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        cmp    dword ptr [edi + 8], ebp
        je     loc_1_9691
        push   ebp
        push   dword ptr [edi + 0x14]
        call   sub_1_7B05
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [esp + 0x1c]
        sub    ecx, ebp
        mov    dword ptr [eax], ecx
        jmp    loc_1_96AB
loc_1_9691:
        push   dword ptr [edi + 0x14]
        call   sub_1_95BA
        mov    ebx, 0x13848                  ; -> loc_1_13848
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 4
        mov    dword ptr [eax], 0
loc_1_96AB:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0xd2], eax
        or     eax, eax
        jne    loc_1_96CB
        mov    al, byte ptr [esi + 0xba]
        dec    al
        mov    byte ptr [esi + 0xba], al
        jne    loc_1_96CB
        and    dword ptr [esi + 0x64], 0xfffffffe
loc_1_96CB:
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_9655
sub_1_96DB:
        push   ebx
        push   esi
        push   edi
        push   0x1c
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_96F2
        sub    eax, eax
        jmp    loc_1_976C
loc_1_96F2:
        mov    ebx, dword ptr [esp + 0x1c]
        lea    eax, [ebx + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_9715
        push   esi
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_976C
loc_1_9715:
        mov    dword ptr [edi + 4], 5
        mov    dword ptr [edi + 0xc], ebx
        mov    dword ptr [edi], 0
        lea    eax, [edi + 0x10]
        mov    dword ptr [edi + 8], eax
        or     ebx, ebx
        je     loc_1_973F
        push   0
        push   ebx
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_44E1
        add    esp, 0x10
loc_1_973F:
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 8], ebx
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 0x14], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x18], edi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        mov    eax, 1
loc_1_976C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_9770:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax + 0x10]
        and    eax, 0x100
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [esp + 0x28]
        mov    esi, dword ptr [esp + 0x24]
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0x30]
        add    eax, ecx
        mov    dword ptr [esp + 0x1c], eax
        mov    edi, dword ptr [esi + 0xd2]
        add    esi, 0xd2
        or     edi, edi
        je     loc_1_990A
loc_1_97B1:
        mov    edx, dword ptr [edi + 0xc]
        and    edx, 0x100
        lea    ecx, [edi + 4]
        mov    dword ptr [esp + 0x14], ecx
        mov    ebp, dword ptr [ecx]
        lea    eax, [edi + 8]
        mov    dword ptr [esp + 0x18], eax
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0x28]
        lea    ebx, [ebp + ecx]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, ebx
        or     eax, eax
        jg     loc_1_98FA
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, ebp
        js     loc_1_990A
        cmp    dword ptr [esp + 0x30], 0
        mov    eax, dword ptr [esp + 0x28]
        je     loc_1_99CE
        mov    eax, dword ptr [eax + 0x10]
        test   ah, 1
        je     loc_1_9819
        mov    edx, dword ptr [esp + 0x1c]
        sub    edx, ebx
        or     edx, edx
        jle    loc_1_993F
loc_1_9819:
        sub    ebp, ecx
        mov    dword ptr [esp + 0x10], ebp
        or     ebp, ebp
        jle    loc_1_98A2
        test   ah, 0x20
        jne    loc_1_9852
        test   byte ptr [edi + 0xd], 0x20
        je     loc_1_9852
        push   ebp
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   esi
        call   sub_1_96DB
        add    esp, 0x10
        or     eax, eax
        je     loc_1_9848
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax]
loc_1_9848:
        mov    dword ptr [esp + 0x10], 0
        jmp    loc_1_98A2
loc_1_9852:
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_993F
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0xc], ebp
        lea    eax, [esi + 0x10]
        push   0
        push   ebp
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 0x34]
        push   eax
        call   sub_1_44E1
        mov    eax, dword ptr [esp + 0x28]
        add    esp, 0x10
        add    dword ptr [eax], ebp
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [edi + 0x14], esi
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
loc_1_98A2:
        mov    edx, dword ptr [edi + 0xc]
        test   dh, 1
        jne    loc_1_9947
        mov    eax, dword ptr [esp + 0x28]
        mov    esi, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, ebx
        add    esi, eax
        or     esi, esi
        jle    loc_1_9994
        mov    eax, dword ptr [esp + 0x28]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_9949
        test   dh, 0x20
        jne    loc_1_9949
        cmp    dword ptr [esp + 0x10], 0
        jg     loc_1_9949
        sub    ebx, ecx
        push   ebx
        push   dword ptr [esp + 0x30]
        call   sub_1_7B05
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x30]
        add    esp, 8
        add    dword ptr [eax], ebx
        sub    dword ptr [eax + 0xc], ebx
        jmp    loc_1_98FE
loc_1_98FA:
        or     edx, edx
        jne    loc_1_993F
loc_1_98FE:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_97B1
loc_1_990A:
        mov    eax, dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x30]
        mov    edi, dword ptr [eax + 0xd2]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   esi
        call   sub_1_96DB
        add    esp, 0x10
        or     eax, eax
        je     loc_1_993F
        or     edi, edi
        jne    loc_1_993F
        mov    eax, dword ptr [esp + 0x24]
        inc    byte ptr [eax + 0xba]
        or     dword ptr [eax + 0x64], 1
loc_1_993F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_9947:
        sub    esi, esi
loc_1_9949:
        or     esi, esi
        jle    loc_1_9994
        lea    eax, [esi + 0x10]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_9994
        mov    dword ptr [ebp + 4], 5
        mov    dword ptr [ebp + 0xc], esi
        lea    ecx, [ebp + 0x10]
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 8], ecx
        sub    ebx, dword ptr [eax]
        push   ebx
        push   esi
        push   dword ptr [esp + 0x34]
        push   ecx
        call   sub_1_44E1
        add    esp, 0x10
        mov    eax, dword ptr [edi + 0x18]
        add    dword ptr [edi + 8], esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [eax], ebp
        mov    dword ptr [edi + 0x18], ebp
loc_1_9994:
        mov    eax, dword ptr [esp + 0x28]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_993F
        mov    edx, dword ptr [eax + 0xc]
        add    edx, dword ptr [eax]
        mov    eax, dword ptr [edi + 0xc]
        test   ah, 0x20
        je     loc_1_99BB
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [edi + 0x10]
        mov    ecx, edx
        sub    ecx, eax
        jns    loc_1_99C3
        mov    edx, eax
        jmp    loc_1_99C3
loc_1_99BB:
        or     eax, 0x2000
        mov    dword ptr [edi + 0xc], eax
loc_1_99C3:
        sub    edx, dword ptr [edi + 4]
        mov    dword ptr [edi + 0x10], edx
        jmp    loc_1_993F
loc_1_99CE:
        test   byte ptr [eax + 0x11], 1
        je     loc_1_993F
        mov    ecx, dword ptr [edi + 0xc]
        test   ch, 1
        jne    loc_1_993F
        cmp    dword ptr [eax], ebx
        jne    loc_1_993F
        or     ecx, 0x100
        mov    dword ptr [edi + 0xc], ecx
        jmp    loc_1_993F
sub_1_99FA:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x3c]
        cmp    dword ptr [esp + 0x40], 0x3c
        mov    ebp, dword ptr [ebx + 8]
        jb     loc_1_A3E9
        mov    ecx, dword ptr [ebp + 0x2b]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [esp + 0x18], edx
        mov    ecx, dword ptr [ebp + 0x2f]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [esp + 0x1c], edx
        mov    ax, word ptr [ebp + 0x35]
        mov    cx, ax
        and    ecx, 0xff
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esp + 0x20], ecx
        mov    ax, word ptr [ebp + 0x39]
        mov    cx, ax
        and    ecx, 0xff
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esp + 0x24], ecx
        movzx  eax, word ptr [ebp + 0x33]
        mov    dword ptr [esp + 0x28], eax
        mov    ecx, dword ptr [ebp + 0x23]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esp + 0x2c], edx
        mov    al, byte ptr [ebp + 1]
        mov    byte ptr [esp + 0x34], al
        mov    al, byte ptr [ebp + 0x33]
        and    al, 0xf3
        shr    al, 2
        movzx  eax, al
        cmp    eax, dword ptr [esp + 0x40]
        ja     loc_1_A3E9
        sub    dword ptr [esp + 0x40], eax
        sub    dword ptr [ebx + 0xc], eax
        add    dword ptr [ebx + 8], eax
        push   dword ptr [esp + 0x34]
        lea    esi, [ebp + 3]
        push   dword ptr [esp + 0x48]
        push   esi
        lea    eax, [ebp + 0x13]
        push   eax
        call   sub_1_C92C
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        je     loc_1_A673
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [edi + 0xce], eax
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [edi + 0x48], eax
        jne    loc_1_9D5A
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x3700
        cmp    eax, dword ptr [edi + 0x64]
        jne    loc_1_9D5A
        sub    esi, esi
        inc    dword ptr [edi + 0xa6]
        mov    ecx, dword ptr [edi + 0xc]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_9C4F
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0x14]
        cmp    eax, esi
        jg     loc_1_9C4F
        push   dword ptr [esp + 0x1c]
        push   edi
        call   sub_1_930B
        add    esp, 8
        mov    ecx, dword ptr [edi + 0xc4]
        cmp    ecx, esi
        je     loc_1_9BF3
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0xaa]
        cmp    eax, esi
        jle    loc_1_9BF3
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        sub    ax, cx
        movsx  ecx, ax
        mov    dword ptr [edi + 0xc4], esi
        mov    eax, dword ptr [edi + 0xae]
        mov    edx, eax
        shr    edx, 3
        sub    ecx, edx
        lea    edx, [eax + ecx]
        mov    dword ptr [edi + 0xae], edx
        movsx  ecx, cx
        mov    eax, dword ptr [edi + 0xb2]
        mov    esi, eax
        shr    esi, 3
        sub    eax, esi
        add    eax, ecx
        mov    dword ptr [edi + 0xb2], eax
        shr    edx, 2
        add    edx, eax
        shr    edx, 1
        cmp    edx, 0xa
        ja     loc_1_9BE0
        mov    edx, 0xa
loc_1_9BE0:
        cmp    edx, 0x3e8
        jb     loc_1_9BED
        mov    edx, 0x3e8
loc_1_9BED:
        mov    dword ptr [edi + 0xa2], edx
loc_1_9BF3:
        mov    esi, dword ptr [edi + 0x24]
        mov    eax, dword ptr [edi + 0x9e]
        mov    ecx, dword ptr [edi + 0x20]
        cmp    esi, ecx
        jae    loc_1_9C16
        cmp    dword ptr [edi + 0x28], esi
        jbe    loc_1_9C0A
        jmp    loc_1_9C11
loc_1_9C0A:
        imul   eax, eax
        sub    edx, edx
        div    esi
loc_1_9C11:
        add    esi, eax
        mov    dword ptr [edi + 0x24], esi
loc_1_9C16:
        mov    byte ptr [edi + 0xbd], 0
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [edi + 0x14], eax
        jne    loc_1_9C32
        mov    dword ptr [edi + 0xc8], 0
        jmp    loc_1_9C3E
loc_1_9C32:
        mov    eax, dword ptr [edi + 0xa2]
        mov    dword ptr [edi + 0xc8], eax
loc_1_9C3E:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        cmp    eax, ecx
        ja     loc_1_9CA3
        jmp    loc_1_9CA1
loc_1_9C4F:
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0x14]
        or     eax, eax
        jle    loc_1_9C6D
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_9C6D:
        cmp    dword ptr [esp + 0x1c], ecx
        jne    loc_1_9CC1
        mov    eax, dword ptr [edi + 0x50]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 0x18]
        js     loc_1_9C8F
        cmp    dword ptr [esp + 0x18], eax
        jne    loc_1_9CC1
        mov    eax, dword ptr [edi + 0x54]
        sub    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        jg     loc_1_9CC1
loc_1_9C8F:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [edi + 0x20]
        mov    ecx, dword ptr [esp + 0x20]
        cmp    eax, ecx
        ja     loc_1_9CA3
loc_1_9CA1:
        mov    eax, ecx
loc_1_9CA3:
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x50], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x54], eax
        cmp    dword ptr [edi + 0x1c], 1
        cmc    
        sbb    esi, esi
        and    esi, 0x400
loc_1_9CC1:
        mov    ebp, dword ptr [edi + 0x4c]
        mov    eax, dword ptr [esp + 0x40]
        cmp    ebp, eax
        jb     loc_1_9CCE
        mov    ebp, eax
loc_1_9CCE:
        or     ebp, ebp
        jne    loc_1_9CE3
        cmp    dword ptr [esp + 0x40], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x200
        or     esi, eax
        jmp    loc_1_9D43
loc_1_9CE3:
        push   ebp
        push   ebx
        or     dword ptr [edi + 0x64], 2
        push   dword ptr [esp + 0x30]
        push   edi
        call   dword ptr [edi + 0x68]
        add    esp, 0x10
        add    dword ptr [edi + 0x48], eax
        sub    dword ptr [edi + 0x4c], eax
        and    dword ptr [edi + 0x64], 0xfffffffd
        mov    edx, dword ptr [edi + 0x60]
        mov    ecx, edx
        and    ecx, 0x800000
        cmp    ecx, 1
        cmc    
        sbb    ecx, ecx
        and    ecx, 0x400
        or     esi, ecx
        and    edx, 0xff7fffff
        mov    dword ptr [edi + 0x60], edx
        test   dh, 8
        jne    loc_1_9D3D
        cmp    ebp, eax
        jne    loc_1_9D3D
        or     edx, 0x800
        mov    dword ptr [edi + 0x60], edx
        mov    word ptr [edi + 0xcc], 3
        jmp    loc_1_9D43
loc_1_9D3D:
        or     esi, 0x200
loc_1_9D43:
        or     esi, esi
        je     loc_1_A3E0
        push   esi
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    loc_1_A3E0
loc_1_9D5A:
        mov    al, byte ptr [edi + 0xbc]
        cmp    al, 3
        jne    loc_1_9D6E
        test   byte ptr [edi + 0x60], 4
        je     loc_1_A3E9
loc_1_9D6E:
        cmp    al, 4
        je     loc_1_9D7D
        test   byte ptr [edi + 0x64], 2
        je     loc_1_9D7D
        jmp    loc_1_A3E9
loc_1_9D7D:
        or     al, al
        je     loc_1_A661
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_A661
        mov    ecx, dword ptr [edi + 0x60]
        and    ecx, 0x1200000
        cmp    ecx, 0x1000000
        je     loc_1_A661
        cmp    al, 2
        jne    loc_1_9EFD
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_9DD1
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [edi + 0x10], eax
        je     loc_1_9DD1
        push   dword ptr [esp + 0x44]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_9DD1:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_9E11
        movzx  ecx, byte ptr [edi + 0xbd]
        cmp    ecx, dword ptr [0x12b10]      ; -> obj1:0x12B10
        jne    loc_1_9E02
        push   0xe
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        push   0x10
        push   edi
        call   sub_1_4B72
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_9E02:
        mov    dword ptr [edi + 0xc8], 5
        jmp    loc_1_A3E9
loc_1_9E11:
        test   byte ptr [esp + 0x29], 2
        je     loc_1_A3E9
        inc    dword ptr [edi + 0xa6]
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x48], eax
        test   byte ptr [esp + 0x29], 0x20
        je     loc_1_9E49
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_9E41
        dec    dword ptr [esp + 0x24]
        jmp    loc_1_9E49
loc_1_9E41:
        and    dword ptr [esp + 0x28], 0xffffdfff
loc_1_9E49:
        mov    eax, dword ptr [0x136a0]      ; -> loc_1_136A0
        sub    eax, 0x3c
        mov    dword ptr [edi + 0xb6], eax
        mov    dword ptr [edi + 0x9e], eax
        mov    byte ptr [edi + 0xbd], 0
        mov    dword ptr [edi + 0xc8], 0
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_9EE2
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x50], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x54], eax
        push   edi
        call   sub_1_B0F6
        add    esp, 4
        push   0
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_A3E0
        and    dword ptr [esp + 0x28], 0xfffffdff
        mov    eax, dword ptr [esp + 0x28]
        and    ah, 0x37
        cmp    ah, 0x10
        jne    loc_1_9F09
        cmp    dword ptr [esp + 0x40], 0
        jne    loc_1_9F09
        jmp    loc_1_A3E0
loc_1_9EE2:
        mov    byte ptr [edi + 0xbc], 3
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [edi + 0x10], eax
        push   edi
        call   sub_1_A929
        add    esp, 4
        jmp    loc_1_A3E0
loc_1_9EFD:
        lea    eax, [edi + 0xa6]
        inc    dword ptr [edi + 0xa6]
loc_1_9F09:
        cmp    dword ptr [edi + 0x4c], 0
        mov    eax, dword ptr [esp + 0x18]
        je     loc_1_9FFF
        sub    eax, dword ptr [edi + 0x48]
        jns    loc_1_9F93
        test   byte ptr [esp + 0x29], 2
        je     loc_1_9F33
        and    dword ptr [esp + 0x28], 0xfffffdff
        inc    dword ptr [esp + 0x18]
        dec    dword ptr [esp + 0x24]
loc_1_9F33:
        mov    ecx, dword ptr [edi + 0x48]
        sub    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x100
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        add    eax, dword ptr [esp + 0x40]
        cmp    eax, ecx
        ja     loc_1_9F64
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_9F64:
        sub    dword ptr [esp + 0x40], ecx
        add    dword ptr [esp + 0x18], ecx
        sub    dword ptr [esp + 0x24], ecx
        push   ecx
        push   ebx
        call   sub_1_9588
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esp + 0x24], 0
        jge    loc_1_9F93
        mov    dword ptr [esp + 0x24], 0
        and    dword ptr [esp + 0x28], 0xffffdfff
loc_1_9F93:
        mov    ecx, dword ptr [esp + 0x28]
        and    ecx, 0x100
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x200
        cmp    eax, 1
        sbb    edx, edx
        inc    edx
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        add    edx, eax
        add    edx, dword ptr [esp + 0x40]
        je     loc_1_9FBB
        dec    edx
loc_1_9FBB:
        mov    eax, dword ptr [edi + 0x48]
        mov    esi, dword ptr [edi + 0x4c]
        lea    eax, [eax + esi - 1]
        mov    esi, dword ptr [esp + 0x18]
        sub    esi, eax
        lea    eax, [edx + esi]
        or     eax, eax
        jle    loc_1_A043
        or     esi, esi
        jle    loc_1_9FE9
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_9FE9:
        or     ecx, ecx
        je     loc_1_9FF6
        and    dword ptr [esp + 0x28], 0xfffffeff
        dec    edx
loc_1_9FF6:
        lea    eax, [edx + esi]
        sub    dword ptr [esp + 0x40], eax
        jmp    loc_1_A043
loc_1_9FFF:
        cmp    dword ptr [edi + 0x48], eax
        je     loc_1_A017
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A017:
        cmp    dword ptr [esp + 0x40], 0
        jne    loc_1_A025
        test   byte ptr [esp + 0x29], 3
        je     loc_1_A043
loc_1_A025:
        and    dword ptr [esp + 0x28], 0xfffffcff
        mov    dword ptr [esp + 0x40], 0
        push   0x200
        push   edi
        call   sub_1_8EB3
        add    esp, 8
loc_1_A043:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_A08F
        cmp    byte ptr [edi + 0xbc], 3
        jne    loc_1_A05D
        test   byte ptr [edi + 0x60], 8
        je     loc_1_A05D
        mov    al, 0x10
        jmp    loc_1_A05F
loc_1_A05D:
        mov    al, 0x80
loc_1_A05F:
        push   eax
        push   edi
        call   sub_1_4B72
        add    esp, 8
        cmp    byte ptr [edi + 0xbc], 0xa
        je     loc_1_A3E0
        push   edi
        call   sub_1_48EE
        add    esp, 4
        push   0x14
        push   edi
        call   sub_1_78CF
        add    esp, 8
        jmp    loc_1_A3E0
loc_1_A08F:
        test   byte ptr [esp + 0x29], 2
        je     loc_1_A0B9
        dec    dword ptr [edi + 0xa6]
        push   0x40
        push   edi
        call   sub_1_4B72
        add    esp, 8
        push   dword ptr [esp + 0x44]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A0B9:
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_A3E0
        cmp    byte ptr [edi + 0xbc], 3
        mov    ecx, dword ptr [edi + 0xc]
        mov    eax, ecx
        jne    loc_1_A145
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_A12A
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0x14]
        or     eax, eax
        jg     loc_1_A12A
        mov    byte ptr [edi + 0xbd], 0
        mov    dword ptr [edi + 0xc8], 0
        inc    ecx
        mov    dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x50], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x54], eax
        push   edi
        call   sub_1_B0F6
        add    esp, 4
        push   0
        push   edi
        call   sub_1_8FCD
        jmp    loc_1_A3CF
loc_1_A12A:
        push   edi
        call   sub_1_4BB1
        add    esp, 4
        push   dword ptr [esp + 0x44]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A145:
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_A2C7
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0x14]
        or     eax, eax
        jg     loc_1_A2C7
        push   dword ptr [esp + 0x1c]
        push   edi
        call   sub_1_930B
        add    esp, 8
        mov    ecx, dword ptr [edi + 0xc4]
        or     ecx, ecx
        je     loc_1_A1F1
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0xaa]
        or     eax, eax
        jle    loc_1_A1F1
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        sub    ax, cx
        mov    dword ptr [edi + 0xc4], 0
        mov    edx, dword ptr [edi + 0xae]
        mov    ecx, edx
        shr    ecx, 3
        sub    ax, cx
        movsx  eax, ax
        lea    ecx, [edx + eax]
        mov    dword ptr [edi + 0xae], ecx
        cdq    
        xor    eax, edx
        sub    eax, edx
        mov    esi, dword ptr [edi + 0xb2]
        mov    edx, esi
        shr    edx, 3
        sub    ax, dx
        movsx  eax, ax
        add    eax, esi
        mov    dword ptr [edi + 0xb2], eax
        shr    ecx, 2
        add    ecx, eax
        shr    ecx, 1
        cmp    ecx, 0xa
        ja     loc_1_A1DE
        mov    ecx, 0xa
loc_1_A1DE:
        cmp    ecx, 0x3e8
        jb     loc_1_A1EB
        mov    ecx, 0x3e8
loc_1_A1EB:
        mov    dword ptr [edi + 0xa2], ecx
loc_1_A1F1:
        mov    ecx, dword ptr [edi + 0x24]
        mov    esi, dword ptr [edi + 0x20]
        cmp    esi, ecx
        jbe    loc_1_A220
        cmp    dword ptr [edi + 0x28], ecx
        jbe    loc_1_A208
        add    ecx, dword ptr [edi + 0x9e]
        jmp    loc_1_A217
loc_1_A208:
        mov    eax, dword ptr [edi + 0x9e]
        imul   eax, eax
        sub    edx, edx
        div    ecx
        add    ecx, eax
loc_1_A217:
        cmp    esi, ecx
        jb     loc_1_A21D
        mov    esi, ecx
loc_1_A21D:
        mov    dword ptr [edi + 0x24], esi
loc_1_A220:
        mov    byte ptr [edi + 0xbd], 0
        mov    ecx, dword ptr [edi + 0x14]
        cmp    dword ptr [esp + 0x1c], ecx
        lea    esi, [edi + 0xc8]
        je     loc_1_A240
        mov    eax, dword ptr [edi + 0xa2]
        mov    dword ptr [esi], eax
        jmp    loc_1_A246
loc_1_A240:
        mov    dword ptr [esi], 0
loc_1_A246:
        mov    eax, dword ptr [edi + 0x60]
        test   eax, 0x20000000
        je     loc_1_A274
        cmp    dword ptr [esp + 0x1c], ecx
        jne    loc_1_A274
        and    eax, 0xdffeffff
        mov    dword ptr [edi + 0x60], eax
        movzx  eax, byte ptr [edi + 0xbc]
        cmp    eax, 5
        je     loc_1_A27E
        cmp    eax, 8
        je     loc_1_A2A3
        cmp    eax, 9
        je     loc_1_A2B5
loc_1_A274:
        mov    eax, 1
        jmp    loc_1_A310
loc_1_A27E:
        mov    byte ptr [edi + 0xbc], 6
        push   0
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        mov    al, byte ptr [0x12b18]        ; -> obj1:0x12B18
        mov    byte ptr [edi + 0xbd], al
        mov    dword ptr [esi], 0xa
        jmp    loc_1_A274
loc_1_A2A3:
        push   0
        push   1
        push   edi
        call   sub_1_7998
        add    esp, 0xc
        jmp    loc_1_A3E9
loc_1_A2B5:
        push   0
        push   0
        push   edi
        call   sub_1_7998
        add    esp, 0xc
        jmp    loc_1_A3E9
loc_1_A2C7:
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [edi + 0x14]
        or     eax, eax
        jle    loc_1_A2E5
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A2E5:
        cmp    dword ptr [esp + 0x1c], ecx
        jne    loc_1_A30E
        mov    eax, dword ptr [edi + 0x50]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 0x18]
        js     loc_1_A307
        cmp    dword ptr [esp + 0x18], eax
        jne    loc_1_A30E
        mov    eax, dword ptr [edi + 0x54]
        sub    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        jg     loc_1_A30E
loc_1_A307:
        mov    eax, 1
        jmp    loc_1_A310
loc_1_A30E:
        sub    eax, eax
loc_1_A310:
        or     eax, eax
        je     loc_1_A3D2
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [edi + 0x20]
        mov    ecx, dword ptr [esp + 0x20]
        cmp    eax, ecx
        ja     loc_1_A32C
        mov    eax, ecx
loc_1_A32C:
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x50], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x54], eax
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_A38B
        mov    eax, dword ptr [edi + 0x40]
        sub    eax, edi
        cmp    eax, 0x40
        je     loc_1_A3D2
        mov    eax, dword ptr [edi + 0x60]
        and    eax, 0xfffffbff
        mov    dword ptr [edi + 0x60], eax
        mov    byte ptr [edi + 0xbd], 0
        mov    ecx, dword ptr [edi + 0xa2]
        mov    dword ptr [edi + 0xc8], ecx
        test   eax, 0x80000
        jne    loc_1_A3D2
        or     eax, 0x80000
        mov    dword ptr [edi + 0x60], eax
        inc    byte ptr [edi + 0xba]
        or     dword ptr [edi + 0x64], 1
        jmp    loc_1_A3D2
loc_1_A38B:
        mov    eax, dword ptr [edi + 0x60]
        test   eax, 0x80000
        je     loc_1_A3B8
        mov    byte ptr [edi + 0xbd], 0
        and    eax, 0xffb7ffff
        mov    dword ptr [edi + 0x60], eax
        mov    al, byte ptr [edi + 0xba]
        dec    al
        mov    byte ptr [edi + 0xba], al
        jne    loc_1_A3B8
        and    dword ptr [edi + 0x64], 0xfffffffe
loc_1_A3B8:
        cmp    dword ptr [edi + 0x1c], 0
        jne    loc_1_A3C4
        test   byte ptr [edi + 0x62], 1
        je     loc_1_A3D2
loc_1_A3C4:
        push   0x400
        push   edi
        call   sub_1_8EB3
loc_1_A3CF:
        add    esp, 8
loc_1_A3D2:
        cmp    dword ptr [esp + 0x40], 0
        ja     loc_1_A3F3
        test   byte ptr [esp + 0x29], 1
        jne    loc_1_A3F3
loc_1_A3E0:
        push   edi
loc_1_A3E1:
        call   sub_1_4BB1
loc_1_A3E6:
        add    esp, 4
loc_1_A3E9:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
loc_1_A3F3:
        mov    cl, byte ptr [edi + 0xbc]
        cmp    cl, 4
        jb     loc_1_A64E
        cmp    cl, 6
        ja     loc_1_A64E
        mov    eax, dword ptr [edi + 0x60]
        and    eax, 0x1000000
        jne    loc_1_A64E
        mov    edx, dword ptr [esp + 0x18]
        cmp    dword ptr [edi + 0x48], edx
        jne    loc_1_A603
        mov    eax, dword ptr [edi + 0x64]
        test   al, 2
        je     loc_1_A42F
        jmp    loc_1_A3E0
loc_1_A42F:
        or     eax, 2
        mov    dword ptr [edi + 0x64], eax
        mov    esi, dword ptr [esp + 0x14]
        mov    ebp, 0x2000
loc_1_A43E:
        test   dword ptr [esp + 0x28], ebp
        je     loc_1_A465
        lea    eax, [esp + 0x40]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebx
        push   eax
        push   edi
        call   sub_1_8832
        add    esp, 0x10
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_A5D6
loc_1_A465:
        cmp    dword ptr [esp + 0x40], 0
        jbe    loc_1_A4C4
        push   dword ptr [esp + 0x40]
        push   ebx
        push   dword ptr [esp + 0x30]
        push   edi
        call   dword ptr [edi + 0x68]
        add    esp, 0x10
        mov    ebx, eax
        add    dword ptr [edi + 0x48], ebx
        sub    dword ptr [edi + 0x4c], ebx
        mov    eax, dword ptr [edi + 0x60]
        test   ah, 8
        je     loc_1_A49C
        push   0x200
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    loc_1_A4AD
loc_1_A49C:
        or     eax, 0x800
        mov    dword ptr [edi + 0x60], eax
        mov    word ptr [edi + 0xcc], 3
loc_1_A4AD:
        cmp    ebx, dword ptr [esp + 0x40]
        jne    loc_1_A548
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_A5D6
loc_1_A4C4:
        mov    ecx, dword ptr [edi + 0x60]
        test   ch, 0x80
        je     loc_1_A51F
        test   ecx, 0x4000000
        jne    loc_1_A4EF
        mov    eax, dword ptr [edi + 0x48]
        cmp    dword ptr [edi + 0x12e], eax
        jne    loc_1_A4EB
        mov    eax, dword ptr [edi + 0x132]
        inc    eax
        mov    dword ptr [edi + 0x48], eax
        jmp    loc_1_A4F5
loc_1_A4EB:
        sub    esi, esi
        jmp    loc_1_A4F5
loc_1_A4EF:
        mov    esi, dword ptr [0x12b08]      ; -> obj1:0x12B08
loc_1_A4F5:
        mov    eax, dword ptr [edi + 0x48]
        sub    eax, dword ptr [edi + 0x132]
        sub    eax, esi
        je     loc_1_A51F
        and    ecx, 0xffff7fff
        mov    dword ptr [edi + 0x60], ecx
        mov    al, byte ptr [edi + 0xba]
        dec    al
        mov    byte ptr [edi + 0xba], al
        jne    loc_1_A51F
        and    dword ptr [edi + 0x64], 0xfffffffe
loc_1_A51F:
        test   byte ptr [esp + 0x29], 1
        jne    loc_1_A555
        lea    eax, [esp + 0x40]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   edi
        call   sub_1_95E6
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_A43E
        jmp    loc_1_A5D6
loc_1_A548:
        push   0x200
        push   edi
        call   sub_1_8EB3
        jmp    loc_1_A5D3
loc_1_A555:
        sub    esi, esi
        inc    dword ptr [edi + 0x48]
        push   0x200
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        push   edi
        call   sub_1_8726
        add    esp, 4
        movzx  eax, byte ptr [edi + 0xbc]
        cmp    eax, 3
        jl     loc_1_A5C7
        cmp    eax, 4
        jle    loc_1_A58E
        cmp    eax, 5
        je     loc_1_A5A6
        cmp    eax, 6
        je     loc_1_A5B4
        jmp    loc_1_A5C7
loc_1_A58E:
        mov    byte ptr [edi + 0xbc], 7
        inc    byte ptr [edi + 0xba]
        or     dword ptr [edi + 0x64], 1
        mov    esi, 1
        jmp    loc_1_A5C7
loc_1_A5A6:
        mov    byte ptr [edi + 0xbc], 8
        mov    esi, 1
        jmp    loc_1_A5C7
loc_1_A5B4:
        inc    dword ptr [edi + 0xa6]
        push   1
        push   1
        push   edi
        call   sub_1_7998
        add    esp, 0xc
loc_1_A5C7:
        or     esi, esi
        je     loc_1_A5D6
        push   0x16
        push   edi
        call   sub_1_78CF
loc_1_A5D3:
        add    esp, 8
loc_1_A5D6:
        and    dword ptr [edi + 0x64], 0xfffffffd
        mov    eax, dword ptr [edi + 0x60]
        test   eax, 0x800000
        je     loc_1_A3E0
        and    eax, 0xff7fffff
        mov    dword ptr [edi + 0x60], eax
        push   0x400
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    loc_1_A3E0
loc_1_A603:
        cmp    cl, 4
        jb     loc_1_A63B
        cmp    cl, 6
        ja     loc_1_A63B
        or     eax, eax
        jne    loc_1_A63B
        push   dword ptr [esp + 0x40]
        lea    eax, [esp + 0x1c]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9770
        add    esp, 0x10
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        push   edi
        call   sub_1_4BB1
        add    esp, 4
        jmp    loc_1_A3E9
loc_1_A63B:
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A64E:
        lea    eax, [esp + 0x18]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A661:
        push   dword ptr [esp + 0x44]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A673:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_A67F
        jmp    loc_1_A3E9
loc_1_A67F:
        mov    eax, dword ptr [esp + 0x28]
        and    ah, 0x12
        cmp    ah, 2
        jne    loc_1_A793
        push   0
        mov    al, byte ptr [ebp + 2]
        push   eax
        lea    eax, [ebp + 0x13]
        push   dword ptr [esp + 0x4c]
        push   eax
        call   sub_1_337D
        add    esp, 0x10
        or     eax, eax
        je     loc_1_A793
        push   dword ptr [esp + 0x44]
        push   esi
        push   eax
        call   sub_1_902C
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A793
        lea    edi, [ebx + 0x7c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    edi, [ebx + 0x6c]
        lea    esi, [ebp + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ebx + 0x8c], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [ebx + 0x9a], eax
        mov    al, byte ptr [esp + 0x34]
        mov    byte ptr [ebx + 0x98], al
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x48], eax
        mov    eax, dword ptr [0x136a0]      ; -> loc_1_136A0
        sub    eax, 0x3c
        mov    dword ptr [ebx + 0xb6], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0x18], eax
        mov    byte ptr [ebx + 0xbc], 3
        or     dword ptr [ebx + 0x64], 2
        push   ebx
        call   sub_1_CCC6
        add    esp, 4
        or     eax, eax
        jne    loc_1_A74D
        push   0x13
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        dec    dword ptr [ebx + 0xa6]
        push   0x40
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        jmp    loc_1_A3E9
loc_1_A74D:
        and    dword ptr [ebx + 0x64], 0xfffffffd
        mov    eax, dword ptr [ebx + 0x60]
        test   eax, 0x800000
        je     loc_1_A771
        and    eax, 0xff7fffff
        mov    dword ptr [ebx + 0x60], eax
        push   0x400
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_A771:
        test   byte ptr [ebx + 0x60], 4
        jne    loc_1_A788
        push   0
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        push   ebx
        jmp    loc_1_A3E1
loc_1_A788:
        push   ebx
        call   sub_1_7670
        jmp    loc_1_A3E6
loc_1_A793:
        push   dword ptr [esp + 0x44]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    loc_1_A3E9
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_A7B0:
        mov    eax, dword ptr [0x13824]      ; -> loc_1_13824
        or     eax, eax
        je     loc_1_A7C1
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x13824], ecx      ; -> loc_1_13824
loc_1_A7C1:
        ret    
sub_1_A7C2:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x13824]      ; -> loc_1_13824
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x13824], ecx      ; -> loc_1_13824
        ret    
sub_1_A7D4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x13820]      ; -> loc_1_13820
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x13820], ecx      ; -> loc_1_13820
        ret    
sub_1_A7E6:
        mov    eax, dword ptr [0x13820]      ; -> loc_1_13820
        or     eax, eax
        je     loc_1_A7F9
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x13820], ecx      ; -> loc_1_13820
        jmp    loc_1_A827
loc_1_A7F9:
        mov    ecx, dword ptr [0x13890]      ; -> loc_1_13890
        cmp    dword ptr [0x12ad8], ecx      ; -> obj1:0x12AD8
        mov    eax, 0
        jbe    loc_1_A81C
        push   0x34
        call   sub_1_F390
        add    esp, 4
        mov    ecx, dword ptr [0x13890]      ; -> loc_1_13890
loc_1_A81C:
        or     eax, eax
        je     loc_1_A827
        inc    ecx
        mov    dword ptr [0x13890], ecx      ; -> loc_1_13890
loc_1_A827:
        ret    
sub_1_A828:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_1_A8D5
        mov    eax, dword ptr [esp + 0x18]
        sub    ecx, ecx
        mov    edi, dword ptr [eax]
        movzx  eax, word ptr [esi + 8]
loc_1_A844:
        cmp    ecx, eax
        jae    loc_1_A84D
        mov    edi, dword ptr [edi]
        inc    ecx
        jmp    loc_1_A844
loc_1_A84D:
        sub    ebx, ebx
loc_1_A84F:
        movzx  eax, word ptr [esi + 0xa]
        cmp    eax, ebx
        jbe    loc_1_A864
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_1704
        inc    ebx
        jmp    loc_1_A84F
loc_1_A864:
        mov    eax, dword ptr [esi]
        sub    edi, edi
        cmp    dword ptr [esi + 4], edi
        jbe    loc_1_A8D5
        mov    ebx, dword ptr [0x13800]      ; -> loc_1_13800
loc_1_A873:
        mov    ebp, dword ptr [eax]
        mov    edx, dword ptr [eax + 0x18]
        dec    edx
        mov    dword ptr [eax + 0x18], edx
        or     edx, edx
        mov    ecx, 0
        setl   cl
        or     edx, edx
        mov    edx, 0
        setle  dl
        add    ecx, edx
        je     loc_1_A8C7
        mov    ecx, dword ptr [eax + 0x2c]
        or     ecx, ecx
        je     loc_1_A8A8
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 0x2c], 0
loc_1_A8A8:
        mov    ecx, dword ptr [0x13724]      ; -> loc_1_13724
        or     ebx, 2
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [0x13724]      ; -> loc_1_13724
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [0x13724], eax      ; -> loc_1_13724
        mov    dword ptr [eax], 0x13720      ; -> loc_1_13720
loc_1_A8C7:
        mov    dword ptr [0x13800], ebx      ; -> loc_1_13800
        mov    eax, ebp
        inc    edi
        cmp    dword ptr [esi + 4], edi
        ja     loc_1_A873
loc_1_A8D5:
        push   dword ptr [esp + 0x18]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, dword ptr [0x13800]      ; -> loc_1_13800
        test   al, 2
        jne    loc_1_A8EE
        test   al, 1
        je     loc_1_A8F3
loc_1_A8EE:
        call   sub_1_8EF7
loc_1_A8F3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_A8F8:
        push   esi
        mov    eax, dword ptr [0x12b08]      ; -> obj1:0x12B08
        mov    ecx, dword ptr [esp + 8]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 0xde]
        mov    esi, dword ptr [ecx + 0x9e]
        shr    eax, 1
        cmp    esi, eax
        jb     loc_1_A918
        mov    esi, eax
loc_1_A918:
        mov    eax, edx
        sub    eax, dword ptr [ecx + 0x4c]
        cmp    esi, eax
        ja     loc_1_A924
        mov    dword ptr [ecx + 0x4c], edx
loc_1_A924:
        mov    eax, dword ptr [ecx + 0x4c]
        pop    esi
        ret    
sub_1_A929:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    ebp, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp + 0xa2]
        mov    dword ptr [ebp + 0xc8], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_AA91
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0xc], 0x3c
        mov    al, byte ptr [ebp + 0x98]
        lea    esi, [ebp + 0x6c]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 0x94]
        mov    dword ptr [ebx + 0x23], eax
        lea    edi, [ebx + 3]
        mov    al, byte ptr [ebp + 0x99]
        mov    byte ptr [ebx + 2], al
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0x7c]
        lea    edi, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 0x18
        or     edx, eax
        mov    dword ptr [ebx + 0x2b], edx
        inc    ecx
        mov    dword ptr [ebp + 0x10], ecx
        mov    eax, ecx
        sub    eax, dword ptr [ebp + 0x14]
        or     eax, eax
        jle    loc_1_A9D4
        mov    dword ptr [ebp + 0x14], ecx
loc_1_A9D4:
        mov    ecx, dword ptr [ebp + 0x48]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2f], edx
        cmp    byte ptr [ebp + 0xbc], 3
        mov    word ptr [ebx + 0x33], 0x12f0
        je     loc_1_AA12
        mov    word ptr [ebx + 0x33], 0x2f0
loc_1_AA12:
        push   dword ptr [ebp + 0x8c]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        push   dword ptr [esp + 0x10]
        mov    ax, word ptr [ebp + 0x4c]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x35], cx
        mov    eax, dword ptr [ebp + 0x2c]
        add    eax, 0x3c00
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   0x3c
        mov    word ptr [ebx + 0x37], ax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ebp + 0x8c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_AA8A
        push   2
        push   2
        push   0
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x28]
        push   esi
        push   dword ptr [ebp + 0x8c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_AA91
loc_1_AA8A:
        mov    dword ptr [esi + 0x41], 0
loc_1_AA91:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_AA99:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_ABD9
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0xc], 0x3d
        mov    ecx, dword ptr [ebp + 0xc]
        dec    ecx
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2b], edx
        mov    ecx, dword ptr [ebp + 0x48]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2f], edx
        mov    word ptr [ebx + 0x33], 0x10f0
        push   ebp
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x35], cx
        inc    byte ptr [ebp + 0xbf]
        mov    al, byte ptr [ebp + 0x98]
        mov    byte ptr [ebx], al
        lea    esi, [ebp + 0x6c]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 0x94]
        mov    dword ptr [ebx + 0x23], eax
        mov    al, byte ptr [ebp + 0x99]
        lea    edi, [ebx + 3]
        mov    byte ptr [ebx + 2], al
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0x7c]
        lea    edi, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [ebp + 0x2c]
        add    eax, 0x3d00
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x37], ax
        push   dword ptr [ebp + 0x8c]
        call   sub_1_35D1
        add    esp, 4
        push   2
        push   0
        push   0x3d
        mov    esi, eax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ebp + 0x8c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3d
        je     loc_1_ABD2
        push   2
        push   2
        push   0
        push   0
        push   0
        push   0x3d
        push   dword ptr [esp + 0x28]
        push   esi
        push   dword ptr [ebp + 0x8c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_ABD9
loc_1_ABD2:
        mov    dword ptr [esi + 0x41], 0
loc_1_ABD9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_ABE1:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_AD72
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0xc], 0x3c
        mov    ecx, dword ptr [ebp + 0x48]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2f], edx
        push   dword ptr [ebp + 0x8c]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x18]
        or     eax, eax
        je     loc_1_AC63
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        lea    eax, [eax + edx - 1]
        mov    edx, ecx
        sub    edx, eax
        js     loc_1_AC66
        mov    ecx, eax
        jmp    loc_1_AC66
loc_1_AC63:
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_AC66:
        test   byte ptr [ebp + 0x63], 0x20
        je     loc_1_AC7C
        mov    eax, dword ptr [ebp + 0x14]
        sub    eax, ecx
        cmp    eax, 1
        mov    word ptr [ebx + 0x33], 0x11f0
        je     loc_1_AC82
loc_1_AC7C:
        mov    word ptr [ebx + 0x33], 0x10f0
loc_1_AC82:
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2b], edx
        push   ebp
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x35], cx
        and    dword ptr [ebp + 0x60], 0xfffff5ff
        mov    word ptr [ebp + 0xcc], 0
        mov    al, byte ptr [ebp + 0x98]
        lea    esi, [ebp + 0x6c]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 0x94]
        mov    dword ptr [ebx + 0x23], eax
        lea    edi, [ebx + 3]
        mov    al, byte ptr [ebp + 0x99]
        mov    byte ptr [ebx + 2], al
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0x7c]
        lea    edi, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [ebp + 0x2c]
        add    eax, 0x3c00
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x37], ax
        cmp    byte ptr [ebp + 0xbc], 8
        mov    esi, 6
        je     loc_1_AD26
        sub    esi, esi
loc_1_AD26:
        push   esi
        push   0
        push   0x3c
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ebp + 0x8c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_AD67
        push   2
        push   esi
        push   0
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebp + 0x8c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_AD72
loc_1_AD67:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x41], 0
loc_1_AD72:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_AD7A:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_AEA2
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0xc], 0x3c
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2b], edx
        mov    word ptr [ebx + 0x33], 0x4f0
        mov    word ptr [ebx + 0x35], 0
        mov    al, byte ptr [ebp + 0x98]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 0x94]
        mov    dword ptr [ebx + 0x23], eax
        mov    al, byte ptr [ebp + 0x99]
        mov    byte ptr [ebx + 2], al
        push   dword ptr [ebp + 0x8c]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        lea    esi, [ebp + 0x6c]
        lea    edi, [ebx + 3]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0x7c]
        lea    edi, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [ebp + 0x2c]
        add    eax, 0x3c00
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   0x3c
        mov    word ptr [ebx + 0x37], ax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ebp + 0x8c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, 0x3c
        je     loc_1_AE7D
        push   2
        push   2
        push   0
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebp + 0x8c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_AE88
loc_1_AE7D:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x41], 0
loc_1_AE88:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_AEA2
        mov    al, byte ptr [ebp + 0x98]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_3B6C
        add    esp, 8
loc_1_AEA2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_AEAA:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x18]
        test   byte ptr [ebp + 0x34], 4
        jne    loc_1_B0EE
        push   dword ptr [esp + 0x1c]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_AEDC
        mov    cl, byte ptr [ebp + 1]
        push   ecx
        push   eax
        call   sub_1_3B6C
        add    esp, 8
loc_1_AEDC:
        mov    esi, 0x3c
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_B0EE
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0xc], esi
        mov    ax, word ptr [ebp + 0x33]
        test   ah, 2
        je     loc_1_AF0C
        mov    esi, 0x3d
loc_1_AF0C:
        test   ah, 1
        je     loc_1_AF12
        inc    esi
loc_1_AF12:
        test   ah, 0x10
        je     loc_1_AF25
        mov    eax, dword ptr [ebp + 0x2f]
        mov    dword ptr [ebx + 0x2b], eax
        mov    word ptr [ebx + 0x33], 0x4f0
        jmp    loc_1_AF80
loc_1_AF25:
        mov    dword ptr [ebx + 0x2b], 0
        mov    ecx, dword ptr [ebp + 0x2b]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        lea    ecx, [esi + edx]
        mov    edx, ecx
        shl    edx, 0x10
        mov    eax, ecx
        and    eax, 0xff00
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 0x2f], edx
        mov    word ptr [ebx + 0x33], 0x14f0
loc_1_AF80:
        mov    word ptr [ebx + 0x35], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebx], al
        mov    al, byte ptr [ebp + 1]
        lea    esi, [ebp + 0x13]
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [ebp + 2]
        mov    byte ptr [ebx + 2], al
        mov    eax, dword ptr [ebp + 0x23]
        lea    edi, [ebx + 3]
        mov    dword ptr [ebx + 0x23], eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    ecx, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 3]
        mov    edi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movzx  edx, byte ptr [ebx + 5]
        movzx  eax, byte ptr [ebx + 0x16]
        add    edx, eax
        push   dword ptr [esp + 0x10]
        movzx  eax, byte ptr [ebx + 0x14]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x15]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 6]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x17]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x18]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x19]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1a]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xa]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1b]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xb]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1c]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xc]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1d]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xd]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1e]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xe]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x1f]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0xf]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x20]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x21]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x10]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x11]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x22]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0x12]
        add    edx, eax
        movzx  eax, byte ptr [ecx]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    edx, eax
        add    edx, 0x8c00
        push   edx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   0x3c
        mov    word ptr [ebx + 0x37], ax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x2c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        mov    esi, eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_35D1
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_B0C3
        mov    al, byte ptr [ebp + 1]
        push   eax
        push   edi
        call   sub_1_3B6C
        add    esp, 8
loc_1_B0C3:
        cmp    esi, 0x3c
        je     loc_1_B0E7
        push   2
        push   2
        push   0
        push   0
        push   0
        push   0x3c
        push   dword ptr [esp + 0x28]
        push   edi
        push   dword ptr [esp + 0x3c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_B0EE
loc_1_B0E7:
        mov    dword ptr [edi + 0x41], 0
loc_1_B0EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_B0F6:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x9e]
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    byte ptr [eax + 0xbc], 4
        mov    cl, byte ptr [eax + 0xba]
        dec    cl
        mov    byte ptr [eax + 0xba], cl
        jne    loc_1_B125
        and    dword ptr [eax + 0x64], 0xfffffffe
loc_1_B125:
        and    dword ptr [eax + 0x60], 0xfffffff7
        ret    
sub_1_B12A:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x10], 0x3e8
        mov    dword ptr [ecx + 0xc], 0x3e8
        mov    dword ptr [ecx + 0x14], 0x3e8
        mov    eax, dword ptr [0x12b08]      ; -> obj1:0x12B08
        mov    dword ptr [ecx + 0x4c], eax
        mov    dword ptr [ecx + 0xc0], 0
        movzx  eax, byte ptr [ecx + 0x80]
        movzx  edx, byte ptr [ecx + 0x6f]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x81]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x79]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x8a]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7a]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x8b]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7b]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7c]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7d]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x6c]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7e]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x6d]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x6e]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x7f]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x70]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x71]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x82]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x72]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x83]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x73]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x84]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x74]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x85]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x75]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x86]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x76]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x87]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x77]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x88]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x78]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 0x89]
        add    edx, eax
        add    edx, 0x5000
        mov    dword ptr [ecx + 0x2c], edx
        mov    byte ptr [ecx + 0xbd], 0
        mov    dword ptr [ecx + 0xc4], 0
        mov    dword ptr [ecx + 0xae], 0
        mov    eax, dword ptr [0x13944]      ; -> loc_1_13944
        mov    edx, eax
        imul   edx, edx, 0xf
        mov    dword ptr [ecx + 0xb2], edx
        imul   eax, eax, 5
        mov    dword ptr [ecx + 0xa2], eax
        mov    dword ptr [ecx + 0xc8], 0
        mov    word ptr [ecx + 0xcc], 0
        ret    
sub_1_B288:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        mov    al, byte ptr [ecx + 0x98]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [ecx + 0x99]
        mov    byte ptr [ebx + 2], al
        mov    edx, dword ptr [ecx + 0x94]
        mov    esi, edx
        shl    esi, 0x10
        mov    eax, edx
        and    eax, 0xff00
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        shr    eax, 0x10
        and    edx, 0xff0000
        or     eax, edx
        shr    eax, 8
        or     esi, eax
        mov    dword ptr [ebx + 0x23], esi
        mov    edx, dword ptr [ecx + 0x9a]
        mov    esi, edx
        shl    esi, 0x10
        mov    eax, edx
        and    eax, 0xff00
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        shr    eax, 0x10
        and    edx, 0xff0000
        or     eax, edx
        shr    eax, 8
        or     esi, eax
        mov    dword ptr [ebx + 0x27], esi
        lea    esi, [ecx + 0x6c]
        lea    edi, [ebx + 3]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ecx + 0x7c]
        lea    edi, [ebx + 0x13]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edx, dword ptr [ecx + 0x10]
        mov    esi, edx
        shl    esi, 0x10
        mov    eax, edx
        and    eax, 0xff00
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    edx, 0x18
        or     esi, edx
        mov    dword ptr [ebx + 0x2b], esi
        mov    edx, dword ptr [ecx + 0x48]
        mov    esi, edx
        shl    esi, 0x10
        mov    eax, edx
        and    eax, 0xff00
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    edx, 0x18
        or     esi, edx
        mov    dword ptr [ebx + 0x2f], esi
        mov    word ptr [ebx + 0x33], 0x10f0
        push   ecx
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x35], cx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_B37E:
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x58]
        mov    eax, dword ptr [esi + 0x60]
        test   eax, 0x40000
        jne    loc_1_B8D5
        test   byte ptr [esi + 0x64], 2
        jne    loc_1_B8D5
        or     eax, 0x40000
        mov    dword ptr [esi + 0x60], eax
        test   eax, 0x10000000
        jne    loc_1_B7EE
loc_1_B3B4:
        mov    ecx, dword ptr [esi + 0x10]
        sub    ecx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, ecx
        mov    dword ptr [esp + 0x4c], eax
        mov    edx, dword ptr [esi + 0x24]
        mov    eax, dword ptr [esi + 0x18]
        cmp    edx, eax
        jb     loc_1_B3CF
        mov    edx, eax
loc_1_B3CF:
        sub    edx, ecx
        mov    edi, dword ptr [esi + 0x60]
        mov    eax, edi
        and    eax, 0x400000
        shr    eax, 0x16
        cmp    eax, edx
        jg     loc_1_B3E4
        mov    eax, edx
loc_1_B3E4:
        mov    dword ptr [esp + 0x48], eax
        mov    eax, dword ptr [esi + 0x9e]
        mov    ecx, dword ptr [esp + 0x4c]
        mov    edx, dword ptr [esp + 0x48]
        cmp    ecx, edx
        jb     loc_1_B3FC
        mov    ecx, edx
loc_1_B3FC:
        cmp    ecx, eax
        jb     loc_1_B402
        mov    ecx, eax
loc_1_B402:
        mov    dword ptr [esp + 0x30], ecx
        cmp    ecx, eax
        je     loc_1_B438
        or     ecx, ecx
        je     loc_1_B416
        mov    eax, ecx
        cmp    dword ptr [esp + 0x4c], eax
        je     loc_1_B438
loc_1_B416:
        cmp    dword ptr [esp + 0x48], 0
        je     loc_1_B7FA
        test   edi, 0x410000
        jne    loc_1_B438
        mov    eax, dword ptr [esi + 0x20]
        shr    eax, 1
        cmp    eax, dword ptr [esp + 0x30]
        ja     loc_1_B7FA
loc_1_B438:
        call   sub_1_A7B0
        mov    dword ptr [esp + 0x44], eax
        or     eax, eax
        je     loc_1_B874
        mov    edi, eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x38], ebp
        lea    edx, [ebp + 0x3c]
        mov    dword ptr [esp + 0x14], edx
        push   ebp
        push   esi
        call   sub_1_B288
        mov    edx, dword ptr [esp + 0x1c]
        add    esp, 8
        mov    word ptr [edx + 8], 0
        mov    word ptr [edx + 0xa], 0
        mov    dword ptr [edx + 4], 0
        mov    eax, dword ptr [esp + 0x30]
        or     eax, eax
        mov    dword ptr [esp + 0x10], eax
        je     loc_1_B671
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [edx], eax
        sub    ebx, ebx
loc_1_B49D:
        mov    eax, dword ptr [esp + 0x34]
        mov    edx, dword ptr [esp + 0x14]
        inc    dword ptr [eax + 0x18]
        inc    dword ptr [edx + 4]
        mov    ecx, dword ptr [esi + 0x38]
        cmp    ecx, ebx
        jne    loc_1_B4EC
        mov    ebp, dword ptr [esi + 0x3c]
        cmp    dword ptr [esp + 0x10], ebp
        jb     loc_1_B4EC
        cmp    word ptr [edx + 0xa], bx
        jne    loc_1_B4EC
        cmp    dword ptr [eax + 0x2c], ebx
        jne    loc_1_B4EC
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [edi], eax
loc_1_B4CB:
        mov    edx, dword ptr [esp + 0x14]
        inc    word ptr [edx + 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [edi], ebx
        jne    loc_1_B4CB
        mov    eax, dword ptr [esp + 0x34]
        sub    dword ptr [esp + 0x10], ebp
        mov    dword ptr [eax + 0x2c], edi
        mov    dword ptr [esi + 0x3c], ebx
        jmp    loc_1_B5B6
loc_1_B4EC:
        lea    eax, [esi + 0x34]
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [esp + 0x20], eax
        mov    dword ptr [esp + 0x18], ecx
loc_1_B4FA:
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_1_173B
        mov    eax, dword ptr [esp + 0x18]
        sub    dword ptr [esp + 0x1c], eax
        mov    ebp, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x10]
        cmp    ebp, eax
        jb     loc_1_B523
        mov    ebp, eax
loc_1_B523:
        mov    eax, dword ptr [esp + 0x2c]
        add    eax, dword ptr [esp + 0x18]
        push   ebp
        push   eax
        lea    eax, [esp + 0x2c]
        push   dword ptr [0x1378c]           ; -> loc_1_1378C
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_1_1640
        cmp    dword ptr [esp + 0x28], ebx
        mov    edx, dword ptr [esp + 0x14]
        jne    loc_1_B57D
        inc    word ptr [edx + 0xa]
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esp + 0x24]
        cmp    dword ptr [esp + 0x1c], ebp
        ja     loc_1_B570
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x18], ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x20], eax
        jmp    loc_1_B574
loc_1_B570:
        add    dword ptr [esp + 0x18], ebp
loc_1_B574:
        sub    dword ptr [esi + 0x3c], ebp
        sub    dword ptr [esp + 0x10], ebp
        jmp    loc_1_B599
loc_1_B57D:
        cmp    word ptr [edx + 0xa], bx
        jne    loc_1_B58D
        cmp    word ptr [edx + 8], bx
        je     loc_1_B864
loc_1_B58D:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x10], ebx
loc_1_B599:
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_1_B5A8
        cmp    dword ptr [esi + 0x3c], ebx
        jne    loc_1_B4FA
loc_1_B5A8:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x34], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x38], eax
loc_1_B5B6:
        mov    eax, dword ptr [esp + 0x34]
        mov    cl, byte ptr [eax + 0x1c]
        mov    dl, cl
        and    dl, 1
        cmp    dl, bl
        je     loc_1_B602
        mov    ebp, dword ptr [esi + 0x3c]
        mov    eax, dword ptr [esi + 0x60]
        and    eax, 0x2000
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        sub    bp, ax
        mov    eax, dword ptr [esp + 0x30]
        sub    bp, word ptr [esp + 0x10]
        add    ax, bp
        mov    byte ptr [esp + 0x41], al
        mov    byte ptr [esp + 0x40], ah
        mov    ebp, dword ptr [esp + 0x38]
        mov    ax, word ptr [esp + 0x40]
        or     word ptr [ebp + 0x33], 0x2000
        mov    word ptr [ebp + 0x39], ax
loc_1_B602:
        cmp    dword ptr [esi + 0x3c], ebx
        jne    loc_1_B665
        mov    ebp, dword ptr [esp + 0x38]
        mov    eax, dword ptr [esp + 0x34]
        or     word ptr [ebp + 0x33], 0x800
        mov    ebp, dword ptr [eax]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -0x40
        je     loc_1_B652
        test   cl, 2
        je     loc_1_B62D
        mov    dword ptr [esp + 0x3c], 2
loc_1_B62D:
        mov    dword ptr [esp + 0x34], ebp
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [esi + 0x3c], eax
        mov    eax, dword ptr [ebp + 0x24]
        cmp    dl, bl
        mov    dword ptr [esi + 0x38], eax
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x30], ebp
        je     loc_1_B665
        test   byte ptr [ebp + 0x1c], 1
        jne    loc_1_B665
        jmp    loc_1_B68F
loc_1_B652:
        test   cl, 2
        je     loc_1_B65F
        mov    dword ptr [esp + 0x3c], 2
loc_1_B65F:
        mov    dword ptr [esi + 0x30], ebx
        mov    dword ptr [esi + 0x34], ebx
loc_1_B665:
        cmp    dword ptr [esp + 0x10], ebx
        jne    loc_1_B49D
        jmp    loc_1_B68F
loc_1_B671:
        test   byte ptr [esi + 0x62], 1
        je     loc_1_B892
        mov    edx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [edx], 0
        mov    dword ptr [eax], 0
loc_1_B68F:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esp + 0x30], eax
        mov    edi, dword ptr [esi + 0x10]
        mov    edx, dword ptr [esp + 0x30]
        add    edx, edi
        mov    dword ptr [esi + 0x10], edx
        mov    eax, dword ptr [esi + 0x14]
        cmp    edi, eax
        jne    loc_1_B6AC
        jmp    loc_1_B6B4
loc_1_B6AC:
        mov    ecx, edx
        sub    ecx, eax
        or     ecx, ecx
        jle    loc_1_B6D1
loc_1_B6B4:
        mov    dword ptr [esi + 0x14], edx
        cmp    dword ptr [esi + 0xc4], 0
        jne    loc_1_B6D1
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [esi + 0xc4], eax
        mov    dword ptr [esi + 0xaa], edi
loc_1_B6D1:
        mov    eax, dword ptr [esp + 0x30]
        cmp    dword ptr [esp + 0x4c], eax
        jne    loc_1_B717
        mov    ecx, dword ptr [esi + 0x60]
        test   ecx, 0x10000
        je     loc_1_B717
        cmp    eax, dword ptr [esp + 0x48]
        jae    loc_1_B717
        mov    ebp, dword ptr [esp + 0x38]
        inc    edx
        or     word ptr [ebp + 0x33], 0x100
        mov    dword ptr [esi + 0x10], edx
        mov    dword ptr [esi + 0x14], edx
        or     ecx, 0x30000000
        mov    dword ptr [esi + 0x60], ecx
        and    ecx, 0xfffeffff
        mov    dword ptr [esi + 0x60], ecx
        mov    dword ptr [esp + 0x3c], 6
loc_1_B717:
        add    dword ptr [esp + 0x30], 0x3c
        push   dword ptr [esp + 0x44]
        sub    eax, eax
        mov    al, byte ptr [esp + 0x35]
        mov    ah, byte ptr [esp + 0x34]
        add    eax, dword ptr [esi + 0x2c]
        push   eax
        call   sub_1_4380
        not    ax
        mov    ebp, dword ptr [esp + 0x40]
        add    esp, 8
        mov    word ptr [ebp + 0x37], ax
        cmp    dword ptr [esi + 0xc8], 0
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        jne    loc_1_B75B
        mov    ecx, dword ptr [esi + 0xa2]
        mov    dword ptr [esi + 0xc8], ecx
loc_1_B75B:
        mov    word ptr [esi + 0xcc], 0
        and    dword ptr [esi + 0x60], 0xffbff5ff
        mov    dword ptr [esi + 0xce], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    word ptr [esi + 0x110], 0
        mov    dword ptr [eax + 0xc], 0x3c
        cmp    dword ptr [esp + 0x3c], 2
        je     loc_1_B79B
        cmp    dword ptr [esp + 0x3c], 6
        je     loc_1_B79B
        mov    dword ptr [esp + 0x3c], 0
loc_1_B79B:
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x50]
        push   dword ptr [esi + 0x8c]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, dword ptr [esp + 0x30]
        push   dword ptr [esi + 0x8c]
        jne    loc_1_B8A3
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 0x41], 0
        mov    dword ptr [esi + 0xc0], 0
        test   byte ptr [esi + 0x63], 0x10
        je     loc_1_B3B4
loc_1_B7EE:
        and    dword ptr [esi + 0x60], 0xff3bfbff
        jmp    loc_1_B8DD
loc_1_B7FA:
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_B81C
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_B7EE
        cmp    word ptr [esi + 0x110], 0
        jne    loc_1_B7EE
        mov    word ptr [esi + 0x110], 0xc8
        jmp    loc_1_B7EE
loc_1_B81C:
        mov    eax, dword ptr [esi + 0x60]
        test   eax, 0x80000
        je     loc_1_B83D
        cmp    dword ptr [esi + 0xc8], 0
        jne    loc_1_B7EE
        mov    eax, dword ptr [esi + 0xa2]
        mov    dword ptr [esi + 0xc8], eax
        jmp    loc_1_B7EE
loc_1_B83D:
        or     eax, 0x80000
        mov    dword ptr [esi + 0x60], eax
        mov    byte ptr [esi + 0xbd], 0
        mov    eax, dword ptr [esi + 0xa2]
        mov    dword ptr [esi + 0xc8], eax
        inc    byte ptr [esi + 0xba]
        or     dword ptr [esi + 0x64], 1
        jmp    loc_1_B7EE
loc_1_B864:
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x18]
        call   sub_1_A828
        add    esp, 8
loc_1_B874:
        cmp    dword ptr [esi + 0xc8], 0
        jne    loc_1_B889
        mov    eax, dword ptr [esi + 0xa2]
        mov    dword ptr [esi + 0xc8], eax
loc_1_B889:
        and    dword ptr [esi + 0x60], 0xffbbfbff
        jmp    loc_1_B8DD
loc_1_B892:
        push   dword ptr [esp + 0x44]
        call   sub_1_A7C2
        add    esp, 4
        jmp    loc_1_B7EE
loc_1_B8A3:
        call   sub_1_35D1
        add    esp, 4
        push   2
        push   dword ptr [esp + 0x40]
        push   0
        push   0
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x5c]
        push   eax
        push   dword ptr [esi + 0x8c]
        call   sub_1_3A0F
        add    esp, 0x24
        jmp    loc_1_B7EE
loc_1_B8D5:
        or     eax, 0x800000
        mov    dword ptr [esi + 0x60], eax
loc_1_B8DD:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
loc_1_B8EE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        push   dword ptr [ebx]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_BA24
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_1_BA1D
        mov    al, byte ptr [esi + 0xbc]
        cmp    al, 4
        je     loc_1_B924
        cmp    al, 7
        jne    loc_1_BA16
loc_1_B924:
        test   byte ptr [esi + 0xbe], 1
        jne    loc_1_BA16
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_1_BA12
        call   sub_1_A7E6
        or     eax, eax
        je     loc_1_BA0B
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 0xc], ecx
        mov    edx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x28], edx
        mov    dword ptr [eax + 0x14], edi
        mov    dword ptr [eax + 0x20], edi
        mov    dword ptr [eax + 0x18], 1
        mov    dword ptr [eax + 0x24], 0
        mov    dword ptr [eax + 0x2c], 0
        mov    ecx, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [eax + 0x30], ecx
        mov    byte ptr [eax + 0x1c], 0
        mov    cx, word ptr [esp + 0x18]
        test   cl, 0x20
        je     loc_1_B993
        mov    byte ptr [eax + 0x1c], 1
loc_1_B993:
        test   ch, 2
        je     loc_1_B99C
        or     byte ptr [eax + 0x1c], 2
loc_1_B99C:
        mov    ebx, dword ptr [esi + 0x1c]
        add    ebx, edi
        mov    dword ptr [esi + 0x1c], ebx
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, esi
        sub    ecx, 0x40
        cmp    ecx, 1
        sbb    ebp, ebp
        neg    ebp
        mov    ecx, dword ptr [esi + 0x44]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 0x44], eax
        lea    ecx, [esi + 0x40]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esi + 0x30], 0
        jne    loc_1_B9D9
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [esi + 0x34], edx
        mov    dword ptr [esi + 0x38], 0
        mov    dword ptr [esi + 0x3c], edi
loc_1_B9D9:
        or     ebp, ebp
        je     loc_1_B9DF
        jmp    loc_1_B9F5
loc_1_B9DF:
        test   byte ptr [esi + 0x62], 2
        je     loc_1_B9F5
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, dword ptr [esi + 0x14]
        add    eax, ebx
        cmp    eax, dword ptr [esi + 0x9e]
        jae    loc_1_B9F5
loc_1_B9F5:
        inc    dword ptr [esi + 0xa6]
        push   esi
        call   sub_1_B37E
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_BA29
loc_1_BA0B:
        mov    eax, 1
        jmp    loc_1_BA29
loc_1_BA12:
        sub    eax, eax
        jmp    loc_1_BA29
loc_1_BA16:
        mov    eax, 0x19
        jmp    loc_1_BA29
loc_1_BA1D:
        mov    eax, 0x19
        jmp    loc_1_BA29
loc_1_BA24:
        mov    eax, 0xf
loc_1_BA29:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h
sub_1_BA30:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x1363c]      ; -> loc_1_1363C
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x1363c], ecx      ; -> loc_1_1363C
        ret    
sub_1_BA42:
        mov    edx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 0x40]
        test   al, 0x20
        jne    loc_1_BA7C
        and    ax, 0xfffb
        mov    word ptr [edx + 0x40], ax
        or     ax, 0x20
        mov    word ptr [edx + 0x40], ax
        mov    eax, dword ptr [0x13784]      ; -> loc_1_13784
        lea    ecx, [edx + 0x18]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x13784]      ; -> loc_1_13784
        mov    dword ptr [edx + 0x1c], eax
        mov    dword ptr [0x13784], ecx      ; -> loc_1_13784
        mov    dword ptr [ecx], 0x13780      ; -> loc_1_13780
loc_1_BA7C:
        ret    
sub_1_BA7D:
        mov    eax, dword ptr [0x137fc]      ; -> loc_1_137FC
        or     eax, eax
        je     loc_1_BA90
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x137fc], ecx      ; -> loc_1_137FC
        jmp    loc_1_BA9A
loc_1_BA90:
        push   0x36
        call   sub_1_F390
        add    esp, 4
loc_1_BA9A:
        ret    
sub_1_BA9B:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x137fc]      ; -> loc_1_137FC
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x137fc], eax      ; -> loc_1_137FC
        ret    
sub_1_BAAD:
        mov    eax, dword ptr [0x1371c]      ; -> loc_1_1371C
        or     eax, eax
        je     loc_1_BAC0
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x1371c], ecx      ; -> loc_1_1371C
        jmp    loc_1_BACA
loc_1_BAC0:
        push   0x32
        call   sub_1_F390
        add    esp, 4
loc_1_BACA:
        ret    
sub_1_BACB:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x1371c]      ; -> loc_1_1371C
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x1371c], ecx      ; -> loc_1_1371C
        ret    
loc_1_BADD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0xbb33   ; -> sub_1_BB33
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0xb
        call   sub_1_129A4
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x1c], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_BB33:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x1359c], 0x1359c  ; -> loc_1_1359C
        je     loc_1_BB86
        mov    edx, 0x1359c                  ; -> loc_1_1359C
loc_1_BB48:
        mov    ecx, dword ptr [0x1359c]      ; -> loc_1_1359C
        mov    eax, ecx
        sub    ecx, 0x18
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x1359c], eax      ; -> loc_1_1359C
        mov    eax, dword ptr [ecx + 0x18]
        lea    esi, [ecx + 0x18]
        mov    dword ptr [eax + 4], edx
        mov    eax, dword ptr [0x1369c]      ; -> loc_1_1369C
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [0x1369c]      ; -> loc_1_1369C
        mov    dword ptr [ecx + 0x1c], eax
        mov    dword ptr [0x1369c], esi      ; -> loc_1_1369C
        mov    dword ptr [esi], 0x13698      ; -> loc_1_13698
        cmp    dword ptr [0x1359c], edx      ; -> loc_1_1359C
        jne    loc_1_BB48
loc_1_BB86:
        cmp    dword ptr [0x13698], 0x13698  ; -> loc_1_13698
        je     loc_1_BC2F
        mov    edi, 0x13698                  ; -> loc_1_13698
loc_1_BB9B:
        mov    esi, dword ptr [0x13698]      ; -> loc_1_13698
        mov    eax, esi
        sub    esi, 0x18
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x13698], eax      ; -> loc_1_13698
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 4], edi
        mov    ax, word ptr [esi + 0x40]
        and    ax, 0xffdf
        mov    word ptr [esi + 0x40], ax
        test   al, 0x40
        jne    loc_1_BC1B
        mov    ecx, dword ptr [esi + 0x10]
        lea    edx, [esi + 0x10]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x10
        je     loc_1_BC23
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        cmp    dword ptr [ecx + 0x32], 3
        jne    loc_1_BC02
        inc    word ptr [esi + 0x46]
        mov    ebx, dword ptr [ecx + 0x28]
        mov    ebp, dword ptr [ecx + 0x2c]
        push   ecx
        call   sub_1_BA9B
        add    esp, 4
        or     ebx, ebx
        je     loc_1_BC10
        push   0
        push   0x1c
        push   ebp
        call   ebx
        add    esp, 0xc
        jmp    loc_1_BC10
loc_1_BC02:
        inc    word ptr [esi + 0x46]
        push   ecx
        push   esi
        call   sub_1_BCEE
        add    esp, 8
loc_1_BC10:
        push   esi
        call   sub_1_434A
        add    esp, 4
        jmp    loc_1_BC23
loc_1_BC1B:
        or     ax, 4
        mov    word ptr [esi + 0x40], ax
loc_1_BC23:
        cmp    dword ptr [0x13698], edi      ; -> loc_1_13698
        jne    loc_1_BB9B
loc_1_BC2F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_BC34:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [0x13780], 0x13780  ; -> loc_1_13780
        mov    edi, dword ptr [esi + 0x28]
        mov    ebx, dword ptr [esi + 0x2c]
        mov    bp, word ptr [esi + 0x30]
        jne    loc_1_BC60
        push   dword ptr [esp + 0x18]
        call   sub_1_BA30
        add    esp, 4
        jmp    loc_1_BCD0
loc_1_BC60:
        mov    ecx, dword ptr [0x13780]      ; -> loc_1_13780
        mov    eax, ecx
        sub    ecx, 0x18
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x13780], eax      ; -> loc_1_13780
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [eax + 4], 0x13780  ; -> loc_1_13780
        mov    edx, dword ptr [ecx + 0x10]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0x10
        je     loc_1_BCCA
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 0x24], eax
        mov    edx, dword ptr [0x135a0]      ; -> loc_1_135A0
        lea    eax, [ecx + 0x18]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [0x135a0]      ; -> loc_1_135A0
        mov    dword ptr [ecx + 0x1c], edx
        mov    dword ptr [0x135a0], eax      ; -> loc_1_135A0
        mov    dword ptr [eax], 0x1359c      ; -> loc_1_1359C
        push   0
        mov    eax, dword ptr [0x13944]      ; -> loc_1_13944
        imul   eax, eax, 6
        push   eax
        push   0xbadd                        ; -> loc_1_BADD
        call   sub_1_129CC
        mov    dword ptr [0x1380c], eax      ; -> loc_1_1380C
        jmp    loc_1_BCD0
loc_1_BCCA:
        and    word ptr [ecx + 0x40], 0xffdf
loc_1_BCD0:
        push   esi
        call   sub_1_BA9B
        add    esp, 4
        or     edi, edi
        je     loc_1_BCE9
        movzx  eax, bp
        push   eax
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
loc_1_BCE9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_BCEE:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x24]
        push   dword ptr [ebx + 0x18]
        call   sub_1_35D1
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 4
        or     eax, eax
        je     loc_1_BD24
        mov    ecx, eax
        cmp    byte ptr [ecx + 0x1a], 0
        jne    loc_1_BD4B
        mov    ecx, eax
        cmp    dword ptr [ecx + 0x51], 0
        jne    loc_1_BD4B
        mov    ecx, eax
        cmp    dword ptr [ecx + 0x4d], 0
        jne    loc_1_BD4B
loc_1_BD24:
        push   0
        push   dword ptr [ebx + 0x1c]
        push   dword ptr [ebx + 0x18]
        push   0x12
        call   sub_1_E8C0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_BD4B
        cmp    eax, 0x2b0a
        je     loc_1_BD4B
        mov    eax, 1
        jmp    loc_1_BFDF
loc_1_BD4B:
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        jne    loc_1_BD76
        mov    ecx, dword ptr [0x1363c]      ; -> loc_1_1363C
        or     ecx, ecx
        je     loc_1_BD67
        mov    eax, dword ptr [ecx]
        mov    dword ptr [0x1363c], eax      ; -> loc_1_1363C
loc_1_BD67:
        mov    dword ptr [esp + 0x14], ecx
        or     ecx, ecx
        je     loc_1_BF08
        mov    dword ptr [ebx + 0x24], ecx
loc_1_BD76:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [ebx + 0x20]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 0xc], 0x25
        mov    ax, word ptr [ebx + 0x30]
        add    ax, 0x25
        mov    word ptr [esp + 0x12], ax
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 0x21], cx
        mov    byte ptr [ebp], 0
        mov    esi, dword ptr [esp + 0x20]
        add    esi, 0x28
        lea    edi, [ebp + 1]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebx + 8]
        lea    edi, [ebp + 0x11]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   dword ptr [esp + 0x14]
        mov    word ptr [ebp + 0x23], 0
        movzx  eax, byte ptr [ebp + 0x13]
        movzx  edx, byte ptr [ebp + 2]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x14]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x15]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x16]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 6]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x17]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x18]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x19]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1a]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xa]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1b]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xb]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xc]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1d]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xd]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1e]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xe]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x1f]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0xf]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x20]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x10]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x12]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0x11]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1]
        add    edx, eax
        movzx  eax, cx
        add    edx, eax
        add    edx, 0x5100
        push   edx
        call   sub_1_4380
        add    esp, 8
        cmp    ax, 0xffff
        je     loc_1_BE9F
        not    ax
loc_1_BE9F:
        movzx  esi, word ptr [esp + 0x12]
        push   2
        push   ebx
        push   esi
        mov    word ptr [ebp + 0x23], ax
        push   dword ptr [esp + 0x20]
        push   dword ptr [ebx + 0x18]
        push   0x12
        call   sub_1_D7E0
        add    esp, 0x18
        cmp    eax, esi
        je     loc_1_BFAA
        mov    dword ptr [0x1392c], eax      ; -> loc_1_1392C
        mov    dword ptr [0x137f8], esi      ; -> loc_1_137F8
        mov    si, word ptr [ebx + 0x30]
        movzx  ecx, si
        mov    dword ptr [0x13950], ecx      ; -> loc_1_13950
        mov    ecx, dword ptr [esp + 0x18]
        sub    si, ax
        mov    dword ptr [ecx + 0x41], 0
        inc    dword ptr [ecx + 0x3d]
        mov    word ptr [ebx + 0x30], si
        mov    edx, dword ptr [ebx + 0x20]
        or     eax, eax
        je     loc_1_BF34
loc_1_BEF9:
        mov    ecx, dword ptr [edx + 0xc]
        cmp    ecx, eax
        ja     loc_1_BF2D
        sub    eax, ecx
        mov    edx, dword ptr [edx]
        jne    loc_1_BEF9
        jmp    loc_1_BF34
loc_1_BF08:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [ecx + 0x10]
        add    ecx, 0x10
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_BA42
        add    esp, 4
        jmp    loc_1_BFDD
loc_1_BF2D:
        add    ax, si
        mov    word ptr [ebx + 0x30], ax
loc_1_BF34:
        mov    dword ptr [ebx + 0x20], edx
        inc    dword ptr [ebx + 0x32]
        push   dword ptr [esp + 0x14]
        call   sub_1_BA30
        add    esp, 4
        mov    dword ptr [ebx + 0x24], 0
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebx + 4], eax
        mov    eax, edx
        mov    dword ptr [edx + 0x14], ebx
        add    eax, 0x10
        mov    dword ptr [ebx], eax
        cmp    dword ptr [0x1359c], 0x1359c  ; -> loc_1_1359C
        jne    loc_1_BF82
        mov    ecx, edx
        mov    eax, dword ptr [0x135a0]      ; -> loc_1_135A0
        add    ecx, 0x18
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x135a0]      ; -> loc_1_135A0
        jmp    loc_1_BF99
loc_1_BF82:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [0x135a0]      ; -> loc_1_135A0
        add    ecx, 0x18
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x135a0]      ; -> loc_1_135A0
        mov    edx, dword ptr [esp + 0x20]
loc_1_BF99:
        mov    dword ptr [edx + 0x1c], eax
        mov    dword ptr [0x135a0], ecx      ; -> loc_1_135A0
        mov    dword ptr [ecx], 0x1359c      ; -> loc_1_1359C
        jmp    loc_1_BFDD
loc_1_BFAA:
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x41], 0
        mov    eax, dword ptr [edx + 0x10]
        add    edx, 0x10
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 0x20]
        cmp    ecx, 0x10
        je     loc_1_BFD3
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        jmp    loc_1_BFDD
loc_1_BFD3:
        mov    edx, dword ptr [esp + 0x20]
        and    word ptr [edx + 0x40], 0xfffb
loc_1_BFDD:
        sub    eax, eax
loc_1_BFDF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_BFE7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_BA7D
        mov    ebp, dword ptr [esp + 0x14]
        mov    edi, eax
        push   dword ptr [ebp]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_C109
        test   byte ptr [esi + 0x40], 0x80
        je     loc_1_C109
        or     edi, edi
        je     loc_1_C102
        lea    ebx, [edi + 0x18]
        lea    eax, [edi + 0x1c]
        push   eax
        push   ebx
        lea    eax, [edi + 8]
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        push   dword ptr [eax + 0x14]
        call   sub_1_3BB5
        add    esp, 0x10
        or     al, al
        je     loc_1_C0F2
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x2c], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [edi + 0x30], ax
        mov    dword ptr [edi + 0x32], 0
        cmp    dword ptr [esi + 0x38], -1
        jne    loc_1_C06D
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [ebx], eax
loc_1_C06D:
        mov    ax, word ptr [esi + 0x40]
        test   al, 0x20
        jne    loc_1_C0D4
        test   al, 0x40
        jne    loc_1_C0CC
        mov    ecx, dword ptr [0x1363c]      ; -> loc_1_1363C
        or     ecx, ecx
        je     loc_1_C08A
        mov    eax, dword ptr [ecx]
        mov    dword ptr [0x1363c], eax      ; -> loc_1_1363C
loc_1_C08A:
        mov    dword ptr [edi + 0x24], ecx
        or     ecx, ecx
        je     loc_1_C0C1
        inc    word ptr [esi + 0x46]
        push   edi
        push   esi
        call   sub_1_BCEE
        add    esp, 8
        or     eax, eax
        je     loc_1_C0B1
        cmp    eax, 0xff
        je     loc_1_C0B1
        mov    eax, 1
        jmp    loc_1_C11B
loc_1_C0B1:
        push   esi
        call   sub_1_434A
        add    esp, 4
        mov    eax, 0xff
        jmp    loc_1_C11B
loc_1_C0C1:
        push   esi
        call   sub_1_BA42
        add    esp, 4
        jmp    loc_1_C0D4
loc_1_C0CC:
        or     ax, 4
        mov    word ptr [esi + 0x40], ax
loc_1_C0D4:
        mov    dword ptr [edi + 0x24], 0
        mov    eax, dword ptr [esi + 0x14]
        add    esi, 0x10
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        mov    eax, 0xff
        jmp    loc_1_C11B
loc_1_C0F2:
        push   edi
        call   sub_1_BA9B
        add    esp, 4
        mov    eax, 3
        jmp    loc_1_C11B
loc_1_C102:
        mov    eax, 1
        jmp    loc_1_C11B
loc_1_C109:
        or     edi, edi
        je     loc_1_C116
        push   edi
        call   sub_1_BA9B
        add    esp, 4
loc_1_C116:
        mov    eax, 6
loc_1_C11B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_C120:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_BAAD
        mov    ebx, dword ptr [esp + 0x14]
        mov    edi, eax
        push   dword ptr [ebx]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_C1D2
        test   byte ptr [esi + 0x40], 0x80
        je     loc_1_C1D2
        or     edi, edi
        je     loc_1_C1CB
        mov    ecx, dword ptr [esp + 0x18]
        or     ecx, ecx
        je     loc_1_C17A
        cmp    dword ptr [ecx + 0x10], 0
        je     loc_1_C17A
        lea    eax, [edi + 0x1c]
        push   eax
        lea    eax, [edi + 0x18]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        push   dword ptr [ecx + 0x14]
        call   sub_1_3BB5
        add    esp, 0x10
        jmp    loc_1_C17E
loc_1_C17A:
        mov    al, byte ptr [esp + 0xf]
loc_1_C17E:
        or     al, al
        je     loc_1_C1BB
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x24], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 0x2c], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [edi + 0x20], eax
        add    esi, 0x20
        mov    eax, dword ptr [esp + 0x20]
        mov    word ptr [edi + 0x30], ax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    eax, 0xff
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        jmp    loc_1_C1E4
loc_1_C1BB:
        push   edi
        call   sub_1_BACB
        add    esp, 4
        mov    eax, 3
        jmp    loc_1_C1E4
loc_1_C1CB:
        mov    eax, 1
        jmp    loc_1_C1E4
loc_1_C1D2:
        or     edi, edi
        je     loc_1_C1DF
        push   edi
        call   sub_1_BACB
        add    esp, 4
loc_1_C1DF:
        mov    eax, 6
loc_1_C1E4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_C1EB:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [esp + 0x44]
        mov    ebp, dword ptr [esp + 0x54]
        test   byte ptr [esi + 0x40], 0x80
        je     loc_1_C3B4
        mov    ebx, dword ptr [esi + 0x20]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x20
        mov    edi, dword ptr [esp + 0x50]
        je     loc_1_C308
        mov    cl, byte ptr [edi]
loc_1_C22A:
        cmp    byte ptr [ebx + 8], cl
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 9]
        cmp    byte ptr [edi + 1], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xa]
        cmp    byte ptr [edi + 2], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xb]
        cmp    byte ptr [edi + 3], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xc]
        cmp    byte ptr [edi + 4], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xd]
        cmp    byte ptr [edi + 5], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xe]
        cmp    byte ptr [edi + 6], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0xf]
        cmp    byte ptr [edi + 7], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x10]
        cmp    byte ptr [edi + 8], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x11]
        cmp    byte ptr [edi + 9], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x12]
        cmp    byte ptr [edi + 0xa], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x13]
        cmp    byte ptr [edi + 0xb], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x14]
        cmp    byte ptr [edi + 0xc], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x15]
        cmp    byte ptr [edi + 0xd], al
        jne    loc_1_C2A7
        mov    al, byte ptr [ebx + 0x16]
        cmp    byte ptr [edi + 0xe], al
        jne    loc_1_C2A7
        mov    al, byte ptr [edi + 0xf]
        cmp    byte ptr [ebx + 0x17], al
        je     loc_1_C2B8
loc_1_C2A7:
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x20
        jne    loc_1_C22A
        jmp    loc_1_C308
loc_1_C2B8:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esi + 0x38], -1
        jne    loc_1_C2CD
        mov    ebp, 0xffffffff
loc_1_C2CD:
        push   0x25
        movzx  eax, word ptr [ebx + 0x30]
        push   eax
        push   dword ptr [ebx + 0x20]
        push   dword ptr [esp + 0x54]
        call   sub_1_442C
        add    esp, 0x10
        mov    esi, eax
        push   ebp
        push   edi
        push   dword ptr [ebx + 0x24]
        call   sub_1_463E
        add    esp, 0xc
        push   esi
        push   eax
        push   dword ptr [ebx + 0x2c]
        call   dword ptr [ebx + 0x28]
        add    esp, 0xc
        push   ebx
        call   sub_1_BACB
        jmp    loc_1_C3B1
loc_1_C308:
        mov    ebx, dword ptr [esi + 0x7a]
        or     ebx, ebx
        je     loc_1_C3B4
        mov    eax, dword ptr [esi + 0x7e]
        mov    dword ptr [esp + 0x18], eax
        inc    word ptr [esi + 0x46]
        cmp    dword ptr [esi + 0x38], -1
        jne    loc_1_C329
        mov    ebp, 0xffffffff
loc_1_C329:
        push   ebp
        lea    eax, [esp + 0x20]
        push   edi
        push   eax
        call   sub_1_460F
        lea    eax, [esp + 0x1c]
        add    esp, 0xc
        push   eax
        mov    eax, dword ptr [esp + 0x4c]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0x25
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x58]
        sub    eax, 0x25
        push   eax
        mov    eax, dword ptr [esp + 0x58]
        mov    eax, dword ptr [eax + 0xc]
        sub    eax, 0x25
        push   eax
        push   0x200
        push   0
        lea    eax, [esp + 0x38]
        push   0
        push   eax
        push   0x22
        push   dword ptr [esp + 0x40]
        call   ebx
        add    esp, 0x2c
        cmp    eax, 0x18
        jne    loc_1_C3AB
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0x25
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   dword ptr [esp + 0x54]
        call   sub_1_442C
        add    esp, 0x10
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   0
        push   dword ptr [eax + 0xc]
        call   dword ptr [eax + 8]
        add    esp, 0xc
loc_1_C3AB:
        push   esi
        call   sub_1_4325
loc_1_C3B1:
        add    esp, 4
loc_1_C3B4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
sub_1_C3BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 8]
        mov    ax, word ptr [edi + 0x21]
        mov    bx, ax
        and    ebx, 0xff
        shr    ax, 8
        shl    ebx, 8
        movzx  eax, ax
        or     ebx, eax
        cmp    ebx, 0x25
        jae    loc_1_C3E7
        jmp    loc_1_C436
loc_1_C3E7:
        mov    eax, dword ptr [esp + 0x18]
        cmp    ebx, eax
        je     loc_1_C40D
        jae    loc_1_C436
        mov    eax, esi
        mov    edx, ebx
        or     esi, esi
        je     loc_1_C40D
loc_1_C3F9:
        mov    ecx, dword ptr [eax + 0xc]
        cmp    ecx, edx
        jb     loc_1_C402
        mov    ecx, edx
loc_1_C402:
        mov    dword ptr [eax + 0xc], ecx
        sub    edx, ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_C3F9
loc_1_C40D:
        push   0
        mov    ebp, dword ptr [esp + 0x20]
        push   0x51
        push   ebp
        lea    eax, [edi + 0x11]
        push   eax
        call   sub_1_337D
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_C428
        jmp    loc_1_C436
loc_1_C428:
        push   ebp
        inc    edi
        push   edi
        push   ebx
        push   esi
        push   eax
        call   sub_1_C1EB
        add    esp, 0x14
loc_1_C436:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
sub_1_C440:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [ecx + 4]
        test   byte ptr [ebx + 0xbe], 1
        je     loc_1_C45F
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [edx], 1
        jmp    loc_1_C46D
loc_1_C45F:
        movzx  eax, byte ptr [ebx + 0xbc]
        mov    edx, dword ptr [esp + 0x14]
        inc    eax
        mov    dword ptr [edx], eax
loc_1_C46D:
        lea    esi, [ebx + 0x6c]
        lea    edi, [edx + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebx + 0x7c]
        lea    edi, [edx + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x8c]
        mov    dword ptr [edx + 0x24], eax
        mov    al, byte ptr [ebx + 0x98]
        mov    byte ptr [edx + 0x28], al
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_1_C4A3
        mov    dword ptr [ecx + 4], eax
        mov    eax, 1
        jmp    loc_1_C4D1
loc_1_C4A3:
        mov    edx, dword ptr [ecx]
        inc    edx
        cmp    edx, 0x20
        jae    loc_1_C4C2
        lea    eax, [edx*4 + 0x135a4]        ; -> loc_1_135A4
loc_1_C4B2:
        cmp    dword ptr [eax], 0
        jne    loc_1_C4D5
        add    eax, 4
        inc    edx
        cmp    eax, 0x13624                  ; -> loc_1_13624
        jb     loc_1_C4B2
loc_1_C4C2:
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        sub    eax, eax
loc_1_C4D1:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_C4D5:
        mov    eax, dword ptr [edx*4 + 0x135a4] ; -> loc_1_135A4
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], edx
        mov    eax, 1
        jmp    loc_1_C4D1
loc_1_C4E8:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    loc_1_C52C
        or     eax, eax
        jne    loc_1_C52C
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        lea    eax, [esi*4 + 0x135a4]        ; -> loc_1_135A4
loc_1_C50B:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    loc_1_C520
        add    eax, 4
        inc    esi
        cmp    eax, 0x13624                  ; -> loc_1_13624
        jb     loc_1_C50B
        or     ecx, ecx
        je     loc_1_C550
loc_1_C520:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        jmp    loc_1_C552
loc_1_C52C:
        cmp    esi, 0x20
        jae    loc_1_C546
        mov    ecx, dword ptr [esi*4 + 0x135a4] ; -> loc_1_135A4
        or     ecx, ecx
        je     loc_1_C546
loc_1_C53C:
        cmp    eax, ecx
        je     loc_1_C554
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_C53C
loc_1_C546:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
loc_1_C550:
        sub    eax, eax
loc_1_C552:
        pop    esi
        ret    
loc_1_C554:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    loc_1_C552
sub_1_C565:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax*4 + 0x135a4] ; -> loc_1_135A4
        or     eax, eax
        je     loc_1_C582
        mov    ecx, dword ptr [esp + 8]
loc_1_C578:
        cmp    ecx, eax
        jb     loc_1_C582
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_C578
loc_1_C582:
        ret    
sub_1_C583:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 0xc]
        mov    dword ptr [ecx + 0x10], eax
        cmp    dword ptr [ecx + 0x14], eax
        je     loc_1_C5A1
        and    dword ptr [ecx + 0x60], 0xefffffff
loc_1_C5A1:
        mov    al, byte ptr [ecx + 0xbc]
        cmp    al, 3
        jbe    loc_1_C5FE
        cmp    al, 0xa
        je     loc_1_C5FE
        mov    edi, dword ptr [ecx + 0x40]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, 0x40
        je     loc_1_C5FE
        or     edx, edx
        je     loc_1_C5CC
loc_1_C5BF:
        mov    eax, dword ptr [edi + 0x20]
        cmp    edx, eax
        jb     loc_1_C5CC
        sub    edx, eax
        mov    edi, dword ptr [edi]
        jne    loc_1_C5BF
loc_1_C5CC:
        mov    dword ptr [ecx + 0x30], edi
        mov    eax, dword ptr [edi + 0x20]
        sub    eax, edx
        mov    dword ptr [ecx + 0x3c], eax
        mov    esi, dword ptr [edi + 0x28]
        mov    edi, dword ptr [edi + 0x24]
        or     edx, edx
        je     loc_1_C5F5
loc_1_C5E1:
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, edi
        cmp    edx, eax
        jb     loc_1_C5F5
        sub    edx, eax
        mov    edi, 0
        mov    esi, dword ptr [esi]
        jne    loc_1_C5E1
loc_1_C5F5:
        mov    dword ptr [ecx + 0x34], esi
        lea    eax, [edx + edi]
        mov    dword ptr [ecx + 0x38], eax
loc_1_C5FE:
        pop    edi
        pop    esi
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0xc657   ; -> sub_1_C657
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0xb
        call   sub_1_129A4
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x1c], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_C657:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        inc    dword ptr [0x13830]           ; -> loc_1_13830
        sub    edi, edi
loc_1_C665:
        mov    ebx, dword ptr [edi*4 + 0x135a4] ; -> loc_1_135A4
        or     ebx, ebx
        je     loc_1_C8F0
loc_1_C674:
        mov    cl, byte ptr [ebx + 0xbc]
        lea    esi, [ebx + 0xbc]
        or     cl, cl
        je     loc_1_C8E6
        test   byte ptr [ebx + 0xbe], 1
        jne    loc_1_C8E6
        mov    eax, dword ptr [ebx + 0xc8]
        or     eax, eax
        je     loc_1_C78F
        dec    eax
        mov    dword ptr [ebx + 0xc8], eax
        jne    loc_1_C78F
        mov    al, byte ptr [ebx + 0xbd]
        inc    al
        mov    byte ptr [ebx + 0xbd], al
        cmp    cl, 2
        jne    loc_1_C6CB
        mov    ecx, dword ptr [0x12b10]      ; -> obj1:0x12B10
        jmp    loc_1_C6D1
loc_1_C6CB:
        mov    ecx, dword ptr [0x12b18]      ; -> obj1:0x12B18
loc_1_C6D1:
        movzx  eax, al
        cmp    eax, ecx
        ja     loc_1_C86A
        mov    dword ptr [ebx + 0xc4], 0
        mov    eax, dword ptr [ebx + 0xa2]
        add    eax, eax
        cmp    eax, 0x3e8
        jb     loc_1_C6FA
        mov    eax, 0x3e8
loc_1_C6FA:
        mov    dword ptr [ebx + 0xa2], eax
        push   dword ptr [ebx + 0xc]
        push   ebx
        call   sub_1_C583
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x60]
        lea    ecx, [ebx + 0x60]
        test   eax, 0x80000
        jne    loc_1_C743
        mov    edx, dword ptr [ebx + 0x18]
        mov    eax, dword ptr [ebx + 0x24]
        cmp    edx, eax
        jb     loc_1_C725
        mov    edx, eax
loc_1_C725:
        shr    edx, 1
        mov    eax, dword ptr [ebx + 0x9e]
        add    eax, eax
        cmp    edx, eax
        ja     loc_1_C735
        mov    edx, eax
loc_1_C735:
        mov    dword ptr [ebx + 0x28], edx
        mov    eax, dword ptr [ebx + 0x9e]
        mov    dword ptr [ebx + 0x24], eax
        jmp    loc_1_C74A
loc_1_C743:
        or     eax, 0x400000
        mov    dword ptr [ecx], eax
loc_1_C74A:
        movzx  eax, byte ptr [esi]
        sub    eax, 2
        cmp    eax, 8
        ja     loc_1_C78F
        jmp    dword ptr [eax*4 + 0xc908]    ; -> obj1:0xC908
loc_1_C75C:
        push   ebx
        call   sub_1_A929
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_C8E8
loc_1_C776:
        or     dword ptr [ecx], 0x10000
loc_1_C77C:
        mov    ebp, 1
        push   0x400
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C78F:
        mov    ax, word ptr [ebx + 0x110]
        or     ax, ax
        je     loc_1_C7C0
        dec    ax
        mov    word ptr [ebx + 0x110], ax
        jne    loc_1_C7C0
        or     dword ptr [ebx + 0x60], 0x400000
        mov    ebp, 1
        push   0x400
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C7C0:
        mov    ax, word ptr [ebx + 0xcc]
        or     ax, ax
        je     loc_1_C7EA
        dec    ax
        mov    word ptr [ebx + 0xcc], ax
        jne    loc_1_C7EA
        mov    ebp, 1
        push   0x200
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C7EA:
        cmp    byte ptr [esi], 4
        jne    loc_1_C8AD
        test   byte ptr [ebx + 0x63], 2
        je     loc_1_C8AD
        mov    ecx, dword ptr [0x13830]      ; -> loc_1_13830
        sub    ecx, dword ptr [ebx + 0xce]
        cmp    ecx, dword ptr [0x12b00]      ; -> obj1:0x12B00
        jbe    loc_1_C8A6
        sub    ecx, dword ptr [0x12b00]      ; -> obj1:0x12B00
        movzx  eax, byte ptr [ebx + 0xbf]
        mov    edx, eax
        imul   edx, dword ptr [0x12b04]      ; -> loc_1_12B04
        cmp    edx, ecx
        jae    loc_1_C8AD
        cmp    dword ptr [0x12b18], eax      ; -> obj1:0x12B18
        jbe    loc_1_C86A
        push   ebx
        call   sub_1_AA99
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_C8E8
loc_1_C851:
        push   1
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_C8E8
loc_1_C86A:
        inc    dword ptr [ebx + 0xa6]
        push   ebx
        call   sub_1_48EE
        add    esp, 4
        push   0x15
        push   ebx
        call   sub_1_78CF
        add    esp, 8
        push   0x20
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_C8E8
loc_1_C8A6:
        mov    byte ptr [ebx + 0xbf], 0
loc_1_C8AD:
        test   dword ptr [ebx + 0x60], 0x20010008
        je     loc_1_C8E6
        mov    eax, dword ptr [ebx + 0xea]
        mov    cx, word ptr [eax + 0x1a]
        or     cx, cx
        je     loc_1_C8E6
        dec    cx
        mov    word ptr [eax + 0x1a], cx
        jne    loc_1_C8E6
        push   0x20
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_C8E8
loc_1_C8E6:
        mov    ebx, dword ptr [ebx]
loc_1_C8E8:
        or     ebx, ebx
        jne    loc_1_C674
loc_1_C8F0:
        inc    edi
        cmp    edi, 0x20
        jb     loc_1_C665
        or     ebp, ebp
        je     loc_1_C903
        call   sub_1_8DFA
loc_1_C903:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_C75C
        dd  loc_1_C75C
        dd  loc_1_C77C
        dd  loc_1_C776
        dd  loc_1_C78F
        dd  loc_1_C77C
        dd  loc_1_C776
        dd  loc_1_C776
        dd  loc_1_C851
sub_1_C92C:
        push   ebx
        push   esi
        mov    eax, dword ptr [0x13754]      ; -> loc_1_13754
        or     eax, eax
        mov    ecx, dword ptr [esp + 0xc]
        je     loc_1_CADE
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [eax + 0x6c], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [ecx + 1]
        cmp    byte ptr [eax + 0x6d], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x6e]
        cmp    byte ptr [ecx + 2], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x6f]
        cmp    byte ptr [ecx + 3], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x70]
        cmp    byte ptr [ecx + 4], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x71]
        cmp    byte ptr [ecx + 5], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x72]
        cmp    byte ptr [ecx + 6], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x73]
        cmp    byte ptr [ecx + 7], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x74]
        cmp    byte ptr [ecx + 8], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x75]
        cmp    byte ptr [ecx + 9], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x76]
        cmp    byte ptr [ecx + 0xa], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x77]
        cmp    byte ptr [ecx + 0xb], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [eax + 0x78]
        cmp    byte ptr [ecx + 0xc], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [ecx + 0xd]
        cmp    byte ptr [eax + 0x79], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [ecx + 0xe]
        cmp    byte ptr [eax + 0x7a], dl
        jne    loc_1_CADE
        mov    dl, byte ptr [ecx + 0xf]
        cmp    byte ptr [eax + 0x7b], dl
        mov    edx, dword ptr [esp + 0x10]
        jne    loc_1_CAE2
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 0x7c], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [edx + 1]
        cmp    byte ptr [eax + 0x7d], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x7e]
        cmp    byte ptr [edx + 2], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x7f]
        cmp    byte ptr [edx + 3], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x80]
        cmp    byte ptr [edx + 4], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x81]
        cmp    byte ptr [edx + 5], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x82]
        cmp    byte ptr [edx + 6], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x83]
        cmp    byte ptr [edx + 7], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x84]
        cmp    byte ptr [edx + 8], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x85]
        cmp    byte ptr [edx + 9], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x86]
        cmp    byte ptr [edx + 0xa], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x87]
        cmp    byte ptr [edx + 0xb], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x88]
        cmp    byte ptr [edx + 0xc], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [eax + 0x89]
        cmp    byte ptr [edx + 0xd], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [edx + 0xe]
        cmp    byte ptr [eax + 0x8a], bl
        jne    loc_1_CAE2
        mov    bl, byte ptr [edx + 0xf]
        cmp    byte ptr [eax + 0x8b], bl
        jne    loc_1_CAE2
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x8c], esi
        jne    loc_1_CAE2
        mov    bl, byte ptr [esp + 0x18]
        cmp    byte ptr [eax + 0x98], bl
        jne    loc_1_CAE2
        jmp    loc_1_CCC3
loc_1_CADE:
        mov    edx, dword ptr [esp + 0x10]
loc_1_CAE2:
        movzx  esi, byte ptr [ecx + 2]
        movzx  eax, byte ptr [edx + 2]
        add    esi, eax
        movzx  eax, byte ptr [edx + 3]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 3]
        add    esi, eax
        movzx  eax, byte ptr [edx + 1]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 1]
        add    esi, eax
        movzx  eax, byte ptr [ecx]
        add    esi, eax
        movzx  eax, byte ptr [edx]
        add    esi, eax
        and    esi, 0x1f
        mov    eax, dword ptr [esi*4 + 0x135a4] ; -> loc_1_135A4
        or     eax, eax
        je     loc_1_CCC3
loc_1_CB20:
        mov    bl, byte ptr [ecx]
        cmp    byte ptr [eax + 0x6c], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [ecx + 1]
        cmp    byte ptr [eax + 0x6d], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [ecx + 2]
        cmp    byte ptr [eax + 0x6e], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [ecx + 3]
        cmp    byte ptr [eax + 0x6f], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x70]
        cmp    byte ptr [ecx + 4], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x71]
        cmp    byte ptr [ecx + 5], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x72]
        cmp    byte ptr [ecx + 6], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x73]
        cmp    byte ptr [ecx + 7], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x74]
        cmp    byte ptr [ecx + 8], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x75]
        cmp    byte ptr [ecx + 9], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x76]
        cmp    byte ptr [ecx + 0xa], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x77]
        cmp    byte ptr [ecx + 0xb], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x78]
        cmp    byte ptr [ecx + 0xc], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x79]
        cmp    byte ptr [ecx + 0xd], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x7a]
        cmp    byte ptr [ecx + 0xe], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x7b]
        cmp    byte ptr [ecx + 0xf], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 0x7c], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [edx + 1]
        cmp    byte ptr [eax + 0x7d], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [edx + 2]
        cmp    byte ptr [eax + 0x7e], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [edx + 3]
        cmp    byte ptr [eax + 0x7f], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x80]
        cmp    byte ptr [edx + 4], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x81]
        cmp    byte ptr [edx + 5], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x82]
        cmp    byte ptr [edx + 6], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x83]
        cmp    byte ptr [edx + 7], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x84]
        cmp    byte ptr [edx + 8], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x85]
        cmp    byte ptr [edx + 9], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x86]
        cmp    byte ptr [edx + 0xa], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x87]
        cmp    byte ptr [edx + 0xb], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x88]
        cmp    byte ptr [edx + 0xc], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x89]
        cmp    byte ptr [edx + 0xd], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x8a]
        cmp    byte ptr [edx + 0xe], bl
        jne    loc_1_CCB2
        mov    bl, byte ptr [eax + 0x8b]
        cmp    byte ptr [edx + 0xf], bl
        jne    loc_1_CCB2
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x8c], esi
        jne    loc_1_CCB2
        mov    bl, byte ptr [esp + 0x18]
        cmp    byte ptr [eax + 0x98], bl
        je     loc_1_CCBE
loc_1_CCB2:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_CB20
        jmp    loc_1_CCC3
loc_1_CCBE:
        mov    dword ptr [0x13754], eax      ; -> loc_1_13754
loc_1_CCC3:
        pop    esi
        pop    ebx
        ret    
sub_1_CCC6:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    edx, [esi + 0x7c]
        lea    ecx, [esi + 0x6c]
        movzx  eax, byte ptr [esi + 0x7e]
        movzx  edi, byte ptr [esi + 0x7d]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x6d]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x6e]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x7f]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x6f]
        add    edi, eax
        movzx  eax, byte ptr [edx]
        add    edi, eax
        movzx  eax, byte ptr [ecx]
        add    edi, eax
        mov    al, byte ptr [esi + 0x98]
        push   eax
        push   dword ptr [esi + 0x8c]
        push   edx
        and    edi, 0x1f
        push   ecx
        call   sub_1_C92C
        add    esp, 0x10
        or     eax, eax
        je     loc_1_CD20
        sub    eax, eax
        jmp    loc_1_CD4B
loc_1_CD20:
        lea    ecx, [edi*4 + 0x135a4]        ; -> loc_1_135A4
        cmp    dword ptr [ecx], 0
        je     loc_1_CD39
loc_1_CD2C:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        jbe    loc_1_CD39
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    loc_1_CD2C
loc_1_CD39:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx], esi
        or     dword ptr [esi + 0x60], 0x80000000
        mov    eax, 1
loc_1_CD4B:
        pop    edi
        pop    esi
        ret    
sub_1_CD4E:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 0x60]
        test   edx, 0x80000000
        je     loc_1_CDB1
        movzx  esi, byte ptr [ecx + 0x7c]
        movzx  eax, byte ptr [ecx + 0x7d]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x6c]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x7e]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x6d]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x6e]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x7f]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x6f]
        add    esi, eax
        and    esi, 0x1f
        lea    esi, [esi*4 + 0x135a4]        ; -> loc_1_135A4
loc_1_CD96:
        mov    eax, dword ptr [esi]
        cmp    eax, ecx
        je     loc_1_CDA4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_CD96
        jmp    loc_1_CDB1
loc_1_CDA4:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        and    edx, 0x7fffffff
        mov    dword ptr [ecx + 0x60], edx
loc_1_CDB1:
        cmp    dword ptr [0x13754], ecx      ; -> loc_1_13754
        jne    loc_1_CDC3
        mov    dword ptr [0x13754], 0        ; -> loc_1_13754
loc_1_CDC3:
        pop    esi
        ret    
sub_1_CDC5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_F21C
        mov    edx, eax
        sub    esi, esi
        sub    eax, eax
        mov    ecx, 0x135a4                  ; -> loc_1_135A4
loc_1_CDDC:
        mov    ebx, ecx
        mov    edi, dword ptr [ecx]
        or     edi, edi
        je     loc_1_CE25
loc_1_CDE4:
        cmp    byte ptr [edi + 0xbc], 0xa
        jne    loc_1_CE1D
        cmp    dword ptr [edi + 0xa6], 0
        jne    loc_1_CE1D
        test   byte ptr [edi + 0xbe], 1
        jne    loc_1_CE1D
        or     eax, eax
        je     loc_1_CE0F
        mov    ebp, edx
        sub    ebp, dword ptr [edi + 0xce]
        cmp    ebp, esi
        jbe    loc_1_CE1D
loc_1_CE0F:
        mov    dword ptr [esp + 0x10], ebx
        mov    eax, edi
        mov    esi, edx
        sub    esi, dword ptr [edi + 0xce]
loc_1_CE1D:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_CDE4
loc_1_CE25:
        add    ecx, 4
        cmp    ecx, 0x13624                  ; -> loc_1_13624
        jb     loc_1_CDDC
        or     eax, eax
        je     loc_1_CE55
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        and    dword ptr [eax + 0x60], 0x7fffffff
        cmp    dword ptr [0x13754], eax      ; -> loc_1_13754
        jne    loc_1_CE55
        mov    dword ptr [0x13754], 0        ; -> loc_1_13754
loc_1_CE55:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_CE5D:
        push   edi
        mov    edx, dword ptr [0x12ae0]      ; -> obj1:0x12AE0
        or     edx, edx
        je     loc_1_CE71
        mov    eax, dword ptr [edx]
        mov    dword ptr [0x12ae0], eax      ; -> obj1:0x12AE0
        jmp    loc_1_CEB7
loc_1_CE71:
        mov    eax, dword ptr [0x12aec]      ; -> loc_1_12AEC
        cmp    eax, dword ptr [0x12ae4]      ; -> obj1:0x12AE4
        ja     loc_1_CE89
        call   sub_1_CDC5
        mov    edx, eax
        or     edx, edx
        jne    loc_1_CEB7
loc_1_CE89:
        mov    eax, dword ptr [0x12ae8]      ; -> obj1:0x12AE8
        cmp    dword ptr [0x12ae4], eax      ; -> obj1:0x12AE4
        jae    loc_1_CF1E
        push   0x136
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_CEB1
        mov    eax, edx
        jmp    loc_1_CF20
loc_1_CEB1:
        inc    dword ptr [0x12ae4]           ; -> obj1:0x12AE4
loc_1_CEB7:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x4d
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [edx + 0x60], eax
        cmp    dword ptr [0x12b1c], 1        ; -> obj1:0x12B1C
        cmc    
        sbb    eax, eax
        and    eax, 0x2000
        or     eax, 0x20000
        mov    dword ptr [edx + 0x60], eax
        mov    eax, dword ptr [0x136a0]      ; -> loc_1_136A0
        sub    eax, 0x3c
        mov    dword ptr [edx + 0xb6], eax
        mov    dword ptr [edx + 0x9e], eax
        lea    eax, [edx + 0x40]
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 0x44], eax
        mov    dword ptr [edx + 0x30], 0
        mov    eax, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [edx + 0xce], eax
        mov    eax, edx
        mov    byte ptr [edx + 0xba], 1
        mov    dword ptr [edx + 0x64], 0x1001
        jmp    loc_1_CF20
loc_1_CF1E:
        sub    eax, eax
loc_1_CF20:
        pop    edi
        ret    
sub_1_CF22:
        mov    eax, dword ptr [0x12aec]      ; -> loc_1_12AEC
        cmp    eax, dword ptr [0x12ae4]      ; -> obj1:0x12AE4
        jae    loc_1_CF43
        dec    dword ptr [0x12ae4]           ; -> obj1:0x12AE4
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        jmp    loc_1_CF54
loc_1_CF43:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x12ae0]      ; -> obj1:0x12AE0
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x12ae0], ecx      ; -> obj1:0x12AE0
loc_1_CF54:
        ret    
sub_1_CF55:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 0x135a4                  ; -> loc_1_135A4
        mov    esi, dword ptr [esp + 0x14]
loc_1_CF62:
        mov    ebp, dword ptr [edi]
        or     ebp, ebp
        je     loc_1_CFBE
        sub    ebx, ebx
loc_1_CF6A:
        cmp    byte ptr [ebp + 0xbc], bl
        je     loc_1_CFB7
        test   byte ptr [ebp + 0xbe], 1
        jne    loc_1_CFB7
        mov    eax, dword ptr [ebp + 0x8c]
        cmp    eax, esi
        jne    loc_1_CFB7
        cmp    dword ptr [ebp + 0xc8], ebx
        je     loc_1_CFB7
        cmp    eax, esi
        jne    loc_1_CFB7
        mov    eax, dword ptr [ebp + 0x40]
        sub    eax, ebp
        sub    eax, 0x40
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, ebx
        jne    loc_1_CFB7
        push   0x400
        push   ebp
        call   sub_1_8EB3
        add    esp, 8
        call   sub_1_8DFA
loc_1_CFB7:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, ebx
        jne    loc_1_CF6A
loc_1_CFBE:
        add    edi, 4
        cmp    edi, 0x13624                  ; -> loc_1_13624
        jb     loc_1_CF62
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h
loc_1_CFD0:
        sub    esp, 0xc4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xd8]
        cmp    eax, 1
        je     loc_1_D00F
        cmp    eax, 2
        je     loc_1_D019
        cmp    eax, 3
        je     loc_1_D075
        cmp    eax, 4
        je     loc_1_D13A
        cmp    eax, 5
        je     loc_1_D1AF
        mov    eax, 0x1f
        jmp    loc_1_D20A
loc_1_D00F:
        mov    eax, 0x1f
        jmp    loc_1_D20A
loc_1_D019:
        mov    dword ptr [esp + 0x10], 0x100
        mov    dword ptr [esp + 0x14], 0xffffffff
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0xffda
        mov    dword ptr [esp + 0x20], 0x9af
        mov    dword ptr [esp + 0x24], 1
        mov    esi, 0x28
        mov    eax, dword ptr [0x12b08]      ; -> obj1:0x12B08
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 0
        mov    eax, dword ptr [0x13598]      ; -> loc_1_13598
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x34], 0
        jmp    loc_1_D1C9
loc_1_D075:
        mov    esi, 0x26
        lea    edi, [esp + 0x10]
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    dword ptr [esp + 0xe4], eax
        mov    eax, dword ptr [esp + 0xd4]
        mov    dword ptr [esp + 0x10], 1
        push   dword ptr [eax]
        je     loc_1_D0FB
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_D0F1
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_D0D1
        push   dword ptr [ecx + 0x8c]
        add    ecx, 0x6c
        lea    eax, [esp + 0x18]
        push   ecx
        push   eax
        call   sub_1_460F
        add    esp, 0xc
        jmp    loc_1_D1C9
loc_1_D0D1:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_D0F1
        push   dword ptr [eax + 0x38]
        add    eax, 0x28
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_460F
        add    esp, 0xc
        jmp    loc_1_D1C9
loc_1_D0F1:
        mov    eax, 0xf
        jmp    loc_1_D20A
loc_1_D0FB:
        call   sub_1_3150
        add    esp, 4
        or     eax, eax
        jne    loc_1_D111
        mov    eax, 6
        jmp    loc_1_D20A
loc_1_D111:
        test   byte ptr [eax + 0x40], 0x80
        jne    loc_1_D121
        mov    eax, 6
        jmp    loc_1_D20A
loc_1_D121:
        push   dword ptr [eax + 0x38]
        add    eax, 0x28
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_460F
        add    esp, 0xc
        jmp    loc_1_D1C9
loc_1_D13A:
        mov    eax, dword ptr [esp + 0xd4]
        mov    esi, 0x31
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_D1A8
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     loc_1_D1A8
        lea    edi, [esp + 0x10]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        movzx  eax, byte ptr [edx + 0xbc]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x28], 0xffffffff
        mov    dword ptr [esp + 0x2c], 0xffffffff
        mov    eax, dword ptr [edx + 0xae]
        shr    eax, 2
        add    eax, dword ptr [edx + 0xb2]
        and    eax, 0xfffffffe
        imul   eax, eax, 0x32
        shr    eax, 1
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x34], 0
        jmp    loc_1_D1C9
loc_1_D1A8:
        mov    eax, 0xf
        jmp    loc_1_D20A
loc_1_D1AF:
        lea    edi, [esp + 0x10]
        sub    eax, eax
        mov    ecx, 0x30
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x10], 0x100
        mov    esi, 0xc0
loc_1_D1C9:
        lea    edx, [esp + 0x10]
        mov    ebx, dword ptr [esp + 0xe0]
        mov    dword ptr [esp + 0xc], 0
        mov    edi, dword ptr [ebx]
        lea    eax, [esp + 0xc]
        push   eax
        mov    ecx, esi
        cmp    ecx, edi
        mov    eax, dword ptr [esp + 0xe0]
        jb     loc_1_D1F2
        mov    ecx, edi
loc_1_D1F2:
        push   ecx
        push   edx
        push   eax
        call   sub_1_4540
        add    esp, 0x10
        cmp    esi, edi
        mov    eax, 9
        ja     loc_1_D20A
        mov    dword ptr [ebx], esi
        sub    eax, eax
loc_1_D20A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc4
        ret    
loc_1_D214:
        mov    eax, 0xfe
        ret    
loc_1_D21A:
        mov    eax, 0xfe
        ret    
loc_1_D220:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x58]
        mov    ecx, dword ptr [esp + 0x50]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [ecx]
        or     esi, esi
        jne    loc_1_D2AA
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 8], 0x100
        jne    loc_1_D4BC
        cmp    dword ptr [ecx + 0xc], 0x100
        jne    loc_1_D4BC
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_1_D4BC
        mov    eax, dword ptr [0x13918]      ; -> loc_1_13918
        shl    eax, 3
        cmp    eax, dword ptr [esp + 0x14]
        jbe    loc_1_D285
        mov    eax, 0x21
        jmp    loc_1_D4C1
loc_1_D285:
        mov    edx, dword ptr [esp + 0x58]
        lea    ecx, [esp + 0x10]
        mov    dword ptr [edx], eax
        push   ecx
        push   eax
        push   dword ptr [0x137f0]           ; -> loc_1_137F0
        push   dword ptr [esp + 0x60]
        call   sub_1_4540
        add    esp, 0x10
        sub    eax, eax
        jmp    loc_1_D4C1
loc_1_D2AA:
        cmp    esi, 0x400
        je     loc_1_D2C4
        cmp    esi, 0x401
        je     loc_1_D2C4
        mov    eax, 0xfe
        jmp    loc_1_D4C1
loc_1_D2C4:
        cmp    dword ptr [ecx + 4], 0
        je     loc_1_D2D4
        mov    eax, 0xfe
        jmp    loc_1_D4C1
loc_1_D2D4:
        mov    edx, dword ptr [esp + 0x58]
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 0x100
        jne    loc_1_D343
        cmp    dword ptr [ecx + 0xc], 0x100
        jne    loc_1_D4BC
        cmp    dword ptr [ecx + 0x10], 1
        jne    loc_1_D4BC
        cmp    dword ptr [esp + 0x14], 4
        jb     loc_1_D339
        sub    esi, 0x400
        cmp    esi, 1
        sbb    eax, eax
        add    eax, 0x408
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   4
        push   eax
        push   dword ptr [esp + 0x60]
        call   sub_1_4540
        add    esp, 0x10
        sub    eax, eax
        jmp    loc_1_D4C1
loc_1_D339:
        mov    eax, 0x21
        jmp    loc_1_D4C1
loc_1_D343:
        cmp    eax, 0x200
        jne    loc_1_D4B0
        mov    eax, dword ptr [ecx + 0xc]
        cmp    eax, 0x300
        jne    loc_1_D3FD
        cmp    dword ptr [esp + 0x14], 8
        jb     loc_1_D4BC
        cmp    dword ptr [ecx + 0x10], 5
        jne    loc_1_D4BC
        mov    eax, dword ptr [esp + 0x4c]
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_D4BC
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     loc_1_D4BC
        mov    eax, dword ptr [edx + 0x60]
        and    eax, 0x4008000
        cmp    eax, 0x4008000
        jne    loc_1_D3C4
        mov    ecx, dword ptr [edx + 0x132]
        mov    eax, ecx
        sub    eax, dword ptr [edx + 0x12e]
        inc    eax
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edx + 0xde]
        sub    eax, dword ptr [edx + 0x48]
        add    eax, ecx
        mov    dword ptr [esp + 0x20], eax
        jmp    loc_1_D3D4
loc_1_D3C4:
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
loc_1_D3D4:
        mov    edx, dword ptr [esp + 0x58]
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   8
        push   eax
        mov    dword ptr [edx], 8
        push   dword ptr [esp + 0x60]
        call   sub_1_4540
        add    esp, 0x10
        sub    eax, eax
        jmp    loc_1_D4C1
loc_1_D3FD:
        cmp    eax, 0x100
        je     loc_1_D409
        jmp    loc_1_D4BC
loc_1_D409:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 1
        je     loc_1_D41D
        cmp    eax, 0x101
        je     loc_1_D427
        jmp    loc_1_D4BC
loc_1_D41D:
        mov    eax, 0xfe
        jmp    loc_1_D4C1
loc_1_D427:
        cmp    esi, 0x401
        jne    loc_1_D43B
        mov    esi, 0x14
        mov    edi, 0x12af0                  ; -> loc_1_12AF0
        jmp    loc_1_D445
loc_1_D43B:
        mov    esi, 0x29
        mov    edi, 0x12af8                  ; -> loc_1_12AF8
loc_1_D445:
        lea    eax, [esp + 0x18]
        push   eax
        sub    ebx, ebx
        push   dword ptr [esp + 0x60]
        call   dword ptr [edi]
        add    esp, 8
        mov    ebp, eax
        cmp    dword ptr [esp + 0x18], ebx
        jne    loc_1_D45F
        jmp    loc_1_D4BC
loc_1_D45F:
        or     ebp, ebp
        je     loc_1_D49F
loc_1_D463:
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebx
        cmp    eax, esi
        jl     loc_1_D49F
        lea    eax, [esp + 0x1c]
        push   eax
        add    ebx, esi
        push   dword ptr [esp + 0x60]
        call   dword ptr [edi + 4]
        add    esp, 8
        mov    ebp, eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   esi
        push   eax
        push   dword ptr [esp + 0x60]
        call   sub_1_4540
        mov    dword ptr [esp + 0x64], eax
        add    esp, 0x10
        or     ebp, ebp
        jne    loc_1_D463
loc_1_D49F:
        mov    edx, dword ptr [esp + 0x58]
        cmp    ebp, 1
        mov    dword ptr [edx], ebx
        cmc    
        sbb    eax, eax
        and    eax, 9
        jmp    loc_1_D4C1
loc_1_D4B0:
        cmp    eax, 0x300
        mov    eax, 0x1a
        je     loc_1_D4C1
loc_1_D4BC:
        mov    eax, 0x1a
loc_1_D4C1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
loc_1_D4C9:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi]
        cmp    eax, 0x400
        je     loc_1_D4E5
        cmp    eax, 0x401
        je     loc_1_D4E5
        jmp    loc_1_D6BF
loc_1_D4E5:
        cmp    dword ptr [esi + 4], 0
        je     loc_1_D4F0
        jmp    loc_1_D6BF
loc_1_D4F0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x100
        jne    loc_1_D504
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D504:
        cmp    eax, 0x200
        jne    loc_1_D6B3
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 0x300
        jne    loc_1_D5CF
        mov    edi, 0x1a
        cmp    dword ptr [esp + 0x1c], 4
        jae    loc_1_D530
        mov    eax, edi
        jmp    loc_1_D6C4
loc_1_D530:
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     loc_1_D5C8
        sub    edx, edx
        sub    edi, edi
        mov    esi, dword ptr [esi + 0x10]
        cmp    esi, 1
        je     loc_1_D568
        cmp    esi, 2
        je     loc_1_D56F
        cmp    esi, 3
        je     loc_1_D576
        cmp    esi, 4
        je     loc_1_D57D
        mov    edi, 0x1a
        jmp    loc_1_D582
loc_1_D568:
        mov    edx, 0x20000
        jmp    loc_1_D582
loc_1_D56F:
        mov    edx, 0x2000000
        jmp    loc_1_D582
loc_1_D576:
        mov    edx, 0x4000000
        jmp    loc_1_D582
loc_1_D57D:
        mov    edx, 0x2000
loc_1_D582:
        or     edi, edi
        jne    loc_1_D5C8
        mov    ecx, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [ecx]
        or     ebx, ebx
        je     loc_1_D595
        or     dword ptr [eax + 0x24], edx
        jmp    loc_1_D59C
loc_1_D595:
        mov    ecx, edx
        not    ecx
        and    dword ptr [eax + 0x24], ecx
loc_1_D59C:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_D5C8
        or     ebx, ebx
        je     loc_1_D5AB
        or     dword ptr [eax + 0x60], edx
        jmp    loc_1_D5B0
loc_1_D5AB:
        not    edx
        and    dword ptr [eax + 0x60], edx
loc_1_D5B0:
        cmp    esi, 2
        jne    loc_1_D5C8
        mov    ecx, dword ptr [0x13830]      ; -> loc_1_13830
        mov    dword ptr [eax + 0xce], ecx
        mov    byte ptr [eax + 0xbf], 0
loc_1_D5C8:
        mov    eax, edi
        jmp    loc_1_D6C4
loc_1_D5CF:
        cmp    eax, 0x200
        jne    loc_1_D5F2
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esi + 0x10]
        push   dword ptr [esp + 0x1c]
        call   sub_1_40B8
        add    esp, 0x10
        jmp    loc_1_D6C4
loc_1_D5F2:
        cmp    eax, 0x100
        je     loc_1_D603
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D603:
        cmp    dword ptr [esi + 0x10], 0x101
        jne    loc_1_D6AC
        cmp    dword ptr [esp + 0x1c], 0x29
        je     loc_1_D621
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D621:
        mov    ecx, dword ptr [esp + 0x18]
        cmp    dword ptr [ecx], 0xc
        je     loc_1_D634
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D634:
        mov    al, byte ptr [ecx + 0x28]
        push   eax
        push   dword ptr [ecx + 0x24]
        lea    eax, [ecx + 0x14]
        push   eax
        add    ecx, 4
        push   ecx
        call   sub_1_C92C
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_D6A5
        inc    dword ptr [esi + 0xa6]
        cmp    byte ptr [esi + 0xbc], 0
        je     loc_1_D698
        test   byte ptr [esi + 0xbe], 1
        jne    loc_1_D698
        or     dword ptr [esi + 0x60], 0x40000000
        push   0x40
        push   esi
        call   sub_1_4B72
        add    esp, 8
        cmp    byte ptr [esi + 0xbc], 0xa
        je     loc_1_D698
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   0x14
        push   esi
        call   sub_1_78CF
        add    esp, 8
loc_1_D698:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_D6C4
loc_1_D6A5:
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D6AC:
        mov    eax, 0x1a
        jmp    loc_1_D6C4
loc_1_D6B3:
        cmp    eax, 0x300
        mov    eax, 0xfe
        je     loc_1_D6C4
loc_1_D6BF:
        mov    eax, 0xfe
loc_1_D6C4:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D6D0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0x100
        jb     loc_1_D724
        cmp    ecx, 0x102
        ja     loc_1_D724
        mov    edi, dword ptr [ecx*4 + 0x12728] ; -> obj1:0x12728
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_D704
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        je     loc_1_D742
loc_1_D704:
        mov    dword ptr [esi + 0x10], eax
        push   esi
        push   0
        push   eax
        call   edi
        add    esp, 0xc
        push   eax
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_1_F2B5
        add    esp, 8
        pop    eax
loc_1_D720:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_D724:
        cmp    ecx, 0
        je     loc_1_D735
        cmp    ecx, -1
        je     loc_1_D73E
        mov    eax, 1
        jmp    loc_1_D720
loc_1_D735:
        xor    eax, eax
        mov    ecx, 0
        jmp    loc_1_D720
loc_1_D73E:
        xor    eax, eax
        jmp    loc_1_D720
loc_1_D742:
        mov    eax, 0x6f8
        jmp    loc_1_D720
WSVV_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [ebp + 0x10]
        push   eax
        movzx  eax, word ptr [ebp + 0x38]
        push   eax
        push   ebx
        call   sub_1_F2F8
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_D7D6
        or     eax, eax
        je     loc_1_D77B
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        je     loc_1_D7D6
loc_1_D77B:
        mov    esi, eax
        mov    edx, eax
        movzx  ecx, word ptr [ebp + 0x1c]
        mov    edi, dword ptr [ecx*4 + 0x12728] ; -> obj1:0x12728
        movzx  ecx, byte ptr [ecx + 0x12a34] ; -> obj1:0x12A34
        jecxz  loc_1_D7B2
loc_1_D793:
        movzx  eax, word ptr [esi]
        push   eax
        movzx  eax, word ptr [esi + 2]
        push   eax
        push   ebx
        call   sub_1_F2F8
        add    esp, 0xc
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     loc_1_D7D6
        add    esi, 4
        dec    ecx
        jne    loc_1_D793
loc_1_D7B2:
        mov    esi, edx
        push   ebx
        push   edx
        call   edi
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   esi
        call   sub_1_F2B5
        add    esp, 8
        and    dword ptr [ebp + 0x2c], 0xfffffffe
loc_1_D7D1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_D7D6:
        mov    word ptr [ebp + 0x1c], 0x100
        jmp    loc_1_D7D1
loc_1_D7DE:
        ret    
        db  000h
sub_1_D7E0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x2c]
        call   sub_1_EE10
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    dword ptr [esp + 0x20], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_DBFB
        push   dword ptr [esp + 0x2c]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        cmp    dword ptr [esp + 0x28], 0x12
        jne    loc_1_DBEB
        mov    al, byte ptr [esi + 0x30]
        cmp    al, 4
        jne    loc_1_D83A
        cmp    dword ptr [esp + 0x3c], 4
        je     loc_1_D83A
        mov    dword ptr [esp + 0x3c], 0
loc_1_D83A:
        cmp    al, 1
        je     loc_1_D857
        cmp    al, 4
        je     loc_1_D857
        cmp    al, 3
        je     loc_1_DBFB
        cmp    al, 2
        jne    loc_1_DBEB
        jmp    loc_1_DBFB
loc_1_D857:
        cmp    dword ptr [esi + 0x39], 0
        jne    loc_1_D86C
        mov    eax, dword ptr [esi + 0x31]
        mov    dword ptr [esi + 0x39], eax
        cmp    dword ptr [esi + 0x59], 0x7b
        je     loc_1_D86C
        mov    dword ptr [esi + 0x59], eax
loc_1_D86C:
        cmp    dword ptr [esp + 0x3c], 1
        je     loc_1_DBEB
        cmp    dword ptr [esp + 0x3c], 3
        je     loc_1_DB9F
        cmp    dword ptr [esp + 0x3c], 4
        je     loc_1_D8A9
        cmp    dword ptr [esp + 0x3c], 5
        je     loc_1_D8A9
        cmp    dword ptr [esp + 0x3c], 2
        je     loc_1_D8A9
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_D8A9
        cmp    dword ptr [esp + 0x3c], 6
        jne    loc_1_DBEB
loc_1_D8A9:
        or     esi, esi
        je     loc_1_DB41
        mov    edi, dword ptr [esi + 0x45]
        lea    ecx, [esi + 0x45]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x45
        je     loc_1_DB41
        inc    dword ptr [esi + 0x3d]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edi + 0x20]
        add    eax, 0xc
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [edi + 0xc]
        push   dword ptr [esp + 0x14]
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_DBFB
        mov    dword ptr [ebx], 0x43434d20
        mov    ecx, dword ptr [edi + 0x20]
        and    ecx, 0xff00
        mov    eax, dword ptr [edi + 0x20]
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [edi + 0x20]
        and    eax, 0xff0000
        mov    edx, dword ptr [edi + 0x20]
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [edi + 0x20]
        lea    eax, [ebx + 0xc]
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_45B0
        add    esp, 0xc
        movzx  ecx, word ptr [ebx + 2]
        movzx  eax, word ptr [ebx + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0xc]
        add    ecx, eax
        movzx  eax, word ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, word ptr [ebx]
        add    eax, ecx
        mov    ecx, eax
        and    ecx, 0xff00
        mov    edx, eax
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0xff0000
        shr    eax, 0x10
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        mov    dword ptr [ebx + 8], ecx
        cmp    byte ptr [esi + 0x30], 4
        jne    loc_1_D98C
        cmp    ebp, 4
        je     loc_1_D98C
        sub    ebp, ebp
loc_1_D98C:
        cmp    ebp, 1
        je     loc_1_D9C4
        cmp    ebp, 2
        jb     loc_1_D9A5
        cmp    ebp, 3
        jbe    loc_1_D9AF
        cmp    ebp, 4
        jb     loc_1_D9A5
        cmp    ebp, 5
        jbe    loc_1_D9C4
loc_1_D9A5:
        cmp    ebp, 6
        jne    loc_1_D9C8
        dec    dword ptr [esi + 0x3d]
        jmp    loc_1_D9C8
loc_1_D9AF:
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_D9C4
        cmp    dword ptr [esp + 0x3c], 6
        je     loc_1_D9C4
        cmp    dword ptr [esp + 0x3c], 2
        jne    loc_1_D9C8
loc_1_D9C4:
        mov    dword ptr [esp + 0x3c], ebp
loc_1_D9C8:
        push   0
        push   dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x18]
        push   ebx
        push   dword ptr [ecx + 0x14]
        call   sub_1_103FD
        add    esp, 0x10
        mov    ebp, eax
        push   ebx
        call   sub_1_F396
        add    esp, 4
        mov    eax, dword ptr [edi + 0x18]
        cmp    ebp, dword ptr [esp + 0x14]
        jne    loc_1_DA3B
        or     eax, eax
        je     loc_1_DA0A
        push   0
        push   dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        push   dword ptr [edi + 0x14]
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
loc_1_DA0A:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DA2D
        push   dword ptr [edi + 0x14]
        push   dword ptr [edi + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe6
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
loc_1_DA2D:
        push   edi
        call   sub_1_313E
        add    esp, 4
        jmp    loc_1_D8A9
loc_1_DA3B:
        or     eax, eax
        je     loc_1_DB2C
        push   0x2b09
        push   dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        push   dword ptr [edi + 0x14]
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
        cmp    eax, 0x2b0e
        je     loc_1_DAFB
        cmp    eax, 0x2b0f
        je     loc_1_DB2C
        cmp    eax, 0x2b0d
        jne    loc_1_DBFB
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DA9E
        push   dword ptr [edi + 0x14]
        push   dword ptr [edi + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe6
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
loc_1_DA9E:
        push   edi
        call   sub_1_313E
        add    esp, 4
        mov    eax, dword ptr [esi + 0x45]
        lea    edi, [esi + 0x45]
        sub    eax, esi
        cmp    eax, 0x45
        je     loc_1_DBFB
loc_1_DAB8:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DAE4
        push   dword ptr [ebx + 0x14]
        push   dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe6
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
loc_1_DAE4:
        push   ebx
        call   sub_1_313E
        add    esp, 4
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 0x45
        jne    loc_1_DAB8
        jmp    loc_1_DBFB
loc_1_DAFB:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DB1E
        push   dword ptr [edi + 0x14]
        push   dword ptr [edi + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe6
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
loc_1_DB1E:
        push   edi
        call   sub_1_313E
        add    esp, 4
        jmp    loc_1_DBFB
loc_1_DB2C:
        mov    eax, dword ptr [esi + 0x45]
        add    esi, 0x45
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [esi], edi
        jmp    loc_1_DBFB
loc_1_DB41:
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        call   sub_1_DC05
        add    esp, 0x18
        mov    esi, eax
        cmp    dword ptr [esp + 0x34], esi
        jne    loc_1_DBFB
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_DBEF
        cmp    dword ptr [esp + 0x3c], 6
        je     loc_1_DBEF
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DBEF
        push   dword ptr [esp + 0x3c]
        push   0
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe5
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
        jmp    loc_1_DBEF
loc_1_DB9F:
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        call   sub_1_DC05
        add    esp, 0x18
        mov    esi, eax
        cmp    dword ptr [esp + 0x34], esi
        jne    loc_1_DBFB
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_DBEF
        push   dword ptr [esp + 0x3c]
        push   0
        mov    ecx, dword ptr [esp + 0x18]
        push   0xe5
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 0x10
        jmp    loc_1_DBEF
loc_1_DBEB:
        mov    esi, dword ptr [esp + 0x1c]
loc_1_DBEF:
        cmp    dword ptr [esp + 0x34], esi
        jne    loc_1_DBFB
        mov    eax, dword ptr [esp + 0x34]
        jmp    loc_1_DBFD
loc_1_DBFB:
        sub    eax, eax
loc_1_DBFD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_DC05:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        push   esi
        call   sub_1_EE10
        add    esp, 4
        mov    edi, eax
        push   esi
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [edi + 0x14]
        call   sub_1_10879
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_DC50
        cmp    dword ptr [esi + 0x39], 0
        je     loc_1_DC50
        mov    dword ptr [esi + 0x39], 0
loc_1_DC50:
        mov    ebx, dword ptr [esp + 0x3c]
        cmp    ebx, 6
        jne    loc_1_DC5E
        dec    dword ptr [esi + 0x3d]
        sub    ebx, ebx
loc_1_DC5E:
        cmp    dword ptr [esp + 0x34], 0
        je     loc_1_DD67
        mov    eax, dword ptr [esp + 0x34]
        add    eax, 0xc
        mov    dword ptr [esp + 0x10], eax
        or     esi, esi
        je     loc_1_DC7B
        inc    dword ptr [esi + 0x3d]
loc_1_DC7B:
        push   dword ptr [esp + 0x10]
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_DC94
        sub    eax, eax
        jmp    loc_1_DDB7
loc_1_DC94:
        mov    dword ptr [ebp], 0x43434d20
        mov    ecx, dword ptr [esp + 0x34]
        and    ecx, 0xff00
        mov    eax, dword ptr [esp + 0x34]
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [esp + 0x34]
        and    eax, 0xff0000
        mov    edx, dword ptr [esp + 0x34]
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        push   dword ptr [esp + 0x34]
        mov    dword ptr [ebp + 4], ecx
        push   dword ptr [esp + 0x34]
        lea    eax, [ebp + 0xc]
        push   eax
        call   sub_1_45B0
        add    esp, 0xc
        movzx  ecx, word ptr [ebp + 2]
        movzx  eax, word ptr [ebp + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0xc]
        add    ecx, eax
        movzx  eax, word ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, word ptr [ebp]
        add    eax, ecx
        mov    ecx, eax
        and    ecx, 0xff00
        mov    edx, eax
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0xff0000
        shr    eax, 0x10
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        push   ebx
        mov    dword ptr [ebp + 8], ecx
        push   dword ptr [esp + 0x14]
        push   ebp
        push   dword ptr [edi + 0x14]
        call   sub_1_103FD
        add    esp, 0x10
        mov    ebx, eax
        push   ebp
        call   sub_1_F396
        add    esp, 4
        cmp    ebx, dword ptr [esp + 0x10]
        jne    loc_1_DD8C
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_1_DD8C
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x3c]
        push   0xe6
        push   dword ptr [edi + 4]
        call   ecx
        add    esp, 0x10
        jmp    loc_1_DD8C
loc_1_DD67:
        cmp    ebx, 2
        jne    loc_1_DD88
        mov    dword ptr [esp + 0x10], 0
        push   ebx
        push   0
        push   0
        push   dword ptr [edi + 0x14]
        call   sub_1_103FD
        add    esp, 0x10
        mov    ebx, eax
        jmp    loc_1_DD8C
loc_1_DD88:
        mov    ebx, dword ptr [esp + 0x20]
loc_1_DD8C:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_DDA8
        cmp    dword ptr [esi + 0x39], 0
        jne    loc_1_DDA8
        mov    eax, dword ptr [esi + 0x31]
        mov    dword ptr [esi + 0x39], eax
        cmp    dword ptr [esi + 0x59], 0x7b
        je     loc_1_DDA8
        mov    dword ptr [esi + 0x59], eax
loc_1_DDA8:
        cmp    ebx, dword ptr [esp + 0x10]
        mov    eax, 0
        jne    loc_1_DDB7
        mov    eax, dword ptr [esp + 0x34]
loc_1_DDB7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  000h
sub_1_DDC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 0x1c]
        sub    ecx, edx
        cmp    ecx, 4
        jb     loc_1_DE78
        mov    dword ptr [0x13880], 0x43434d20 ; -> loc_1_13880
        mov    ecx, dword ptr [esp + 0x18]
loc_1_DDEC:
        mov    edi, dword ptr [ecx + 0x40]
        mov    bl, byte ptr [edi]
        mov    byte ptr [esp + 0x10], bl
        mov    edi, dword ptr [ecx + 0x40]
        mov    bl, byte ptr [edi + 1]
        mov    byte ptr [esp + 0x11], bl
        mov    edi, dword ptr [ecx + 0x40]
        mov    bl, byte ptr [edi + 2]
        mov    byte ptr [esp + 0x12], bl
        mov    edi, dword ptr [ecx + 0x40]
        mov    bl, byte ptr [edi + 3]
        mov    byte ptr [esp + 0x13], bl
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [0x13874], edi      ; -> loc_1_13874
        shl    edi, 0x10
        mov    ebx, dword ptr [esp + 0x10]
        and    ebx, 0xff00
        or     ebx, edi
        shl    ebx, 8
        mov    ebp, dword ptr [esp + 0x10]
        and    ebp, 0xff0000
        mov    edi, dword ptr [esp + 0x10]
        shr    edi, 0x10
        or     ebp, edi
        shr    ebp, 8
        or     ebx, ebp
        mov    dword ptr [0x1387c], ebx      ; -> loc_1_1387C
        cmp    dword ptr [esp + 0x10], 0x43434d20
        jne    loc_1_DE5E
        mov    esi, 1
        jmp    loc_1_DE62
loc_1_DE5E:
        inc    edx
        inc    dword ptr [ecx + 0x40]
loc_1_DE62:
        or     esi, esi
        jne    loc_1_DE71
        lea    edi, [eax - 3]
        cmp    edi, edx
        ja     loc_1_DDEC
loc_1_DE71:
        cmp    esi, 1
        jne    loc_1_DE78
        mov    eax, edx
loc_1_DE78:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_DE80:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [0x13934]      ; -> loc_1_13934
        mov    dword ptr [esp + 0x14], ecx
        mov    eax, dword ptr [esp + 0x20]
        cmp    eax, 1
        je     loc_1_DEC7
        cmp    eax, 3
        je     loc_1_E5C2
        cmp    eax, 4
        je     loc_1_E607
        cmp    eax, 5
        je     loc_1_E652
        cmp    eax, 6
        je     loc_1_E66B
        jmp    loc_1_E68A
loc_1_DEC7:
        mov    eax, dword ptr [ebx + 0x38]
        mov    dword ptr [ebx + 0x40], eax
        sub    ebp, ebp
        cmp    dword ptr [esp + 0x24], ebp
        je     loc_1_E68A
loc_1_DED9:
        cmp    dword ptr [ebx + 0x34], 1
        je     loc_1_E266
        cmp    dword ptr [ebx + 0x4c], 0
        jne    loc_1_E0F1
        cmp    dword ptr [ebx + 0x44], 0
        jne    loc_1_E069
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0xd
        jae    loc_1_DF45
        push   0xd
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x4c], eax
        mov    byte ptr [ebx + 0x54], 0
        mov    edi, eax
        mov    dword ptr [ebx + 0x50], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [ebx + 0x50], eax
        add    byte ptr [ebx + 0x54], al
        add    dword ptr [ebx + 0x40], eax
        add    ebp, eax
        jmp    loc_1_E57C
loc_1_DF45:
        mov    ecx, dword ptr [ebx + 0x40]
        mov    esi, dword ptr [ecx + 8]
        mov    edx, esi
        and    edx, 0xff00
        mov    eax, esi
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, esi
        and    eax, 0xff0000
        shr    esi, 0x10
        or     eax, esi
        shr    eax, 8
        or     edx, eax
        movzx  eax, word ptr [ecx + 2]
        movzx  esi, byte ptr [ecx + 0xc]
        add    esi, eax
        movzx  eax, word ptr [ecx + 6]
        add    esi, eax
        movzx  eax, word ptr [ecx]
        add    esi, eax
        movzx  eax, word ptr [ecx + 4]
        add    esi, eax
        cmp    esi, edx
        je     loc_1_DF9D
        mov    dword ptr [ebx + 0x34], 1
        inc    ecx
        mov    dword ptr [ebx + 0x40], ecx
        jmp    loc_1_E57C
loc_1_DF9D:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 0x55], ecx
        push   ecx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x44], eax
        mov    edi, eax
        mov    dword ptr [ebx + 0x48], edi
        mov    dword ptr [ebx + 0x5d], 0
        add    ebp, 0xc
        mov    esi, dword ptr [ebx + 0x40]
        add    esi, 0xc
        mov    dword ptr [ebx + 0x40], esi
        mov    edx, dword ptr [esp + 0x24]
        sub    edx, ebp
        mov    eax, dword ptr [ebx + 0x55]
        cmp    edx, eax
        jb     loc_1_DFF9
        mov    edx, eax
loc_1_DFF9:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebx + 0x40], edx
        add    dword ptr [ebx + 0x48], edx
        mov    eax, dword ptr [ebx + 0x5d]
        add    eax, edx
        mov    dword ptr [ebx + 0x5d], eax
        add    ebp, edx
        mov    edx, dword ptr [ebx + 0x55]
        cmp    eax, edx
        jne    loc_1_E57C
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ecx + 0x10]
        or     esi, esi
        je     loc_1_E044
        mov    eax, dword ptr [ebx + 0x44]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 0x3c], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        add    esp, 0x10
loc_1_E044:
        push   dword ptr [ebx + 0x44]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x5d], 0
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x48], 0
        jmp    loc_1_E57C
loc_1_E069:
        mov    edx, dword ptr [ebx + 0x55]
        sub    edx, dword ptr [ebx + 0x5d]
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        cmp    edx, eax
        jb     loc_1_E07B
        mov    edx, eax
loc_1_E07B:
        mov    edi, dword ptr [ebx + 0x48]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebx + 0x48], edx
        add    dword ptr [ebx + 0x40], edx
        mov    eax, dword ptr [ebx + 0x5d]
        add    eax, edx
        mov    dword ptr [ebx + 0x5d], eax
        add    ebp, edx
        mov    edx, dword ptr [ebx + 0x55]
        cmp    eax, edx
        jne    loc_1_E57C
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ecx + 0x10]
        or     esi, esi
        je     loc_1_E0CC
        mov    eax, dword ptr [ebx + 0x44]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 0x3c], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        add    esp, 0x10
loc_1_E0CC:
        push   dword ptr [ebx + 0x44]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x5d], 0
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x48], 0
        jmp    loc_1_E57C
loc_1_E0F1:
        mov    eax, 0xd
        movzx  ecx, byte ptr [ebx + 0x54]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 0x24]
        sub    ecx, ebp
        cmp    eax, ecx
        jb     loc_1_E108
        mov    eax, ecx
loc_1_E108:
        mov    edi, dword ptr [ebx + 0x50]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, 0xd
        add    dword ptr [ebx + 0x50], eax
        movzx  edx, byte ptr [ebx + 0x54]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    loc_1_E140
        mov    cl, byte ptr [ebx + 0x54]
        add    cl, al
        mov    byte ptr [ebx + 0x54], cl
        add    dword ptr [ebx + 0x40], eax
        add    ebp, eax
        jmp    loc_1_E57C
loc_1_E140:
        mov    edx, dword ptr [ebx + 0x4c]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, ecx
        and    esi, 0xff00
        mov    edi, ecx
        shl    edi, 0x10
        or     esi, edi
        shl    esi, 8
        mov    edi, ecx
        and    edi, 0xff0000
        shr    ecx, 0x10
        or     edi, ecx
        shr    edi, 8
        or     esi, edi
        movzx  ecx, word ptr [edx + 2]
        movzx  edi, byte ptr [edx + 0xc]
        add    edi, ecx
        movzx  ecx, word ptr [edx + 6]
        add    edi, ecx
        movzx  ecx, word ptr [edx]
        add    edi, ecx
        movzx  ecx, word ptr [edx + 4]
        add    edi, ecx
        cmp    edi, esi
        jne    loc_1_E55A
        add    dword ptr [ebx + 0x40], eax
        add    ebp, eax
        mov    edx, dword ptr [edx + 4]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 0x55], ecx
        push   ecx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x44], eax
        mov    edi, eax
        mov    dword ptr [ebx + 0x48], edi
        mov    dword ptr [ebx + 0x5d], 0
        mov    esi, dword ptr [ebx + 0x4c]
        add    esi, 0xc
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    dword ptr [ebx + 0x48]
        inc    dword ptr [ebx + 0x5d]
        push   dword ptr [ebx + 0x4c]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x50], 0
        mov    dword ptr [ebx + 0x4c], 0
        mov    byte ptr [ebx + 0x54], 0
        mov    edx, dword ptr [ebx + 0x55]
        sub    edx, dword ptr [ebx + 0x5d]
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        cmp    edx, eax
        jb     loc_1_E20F
        mov    edx, eax
loc_1_E20F:
        mov    edi, dword ptr [ebx + 0x48]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebp, edx
        mov    eax, dword ptr [ebx + 0x5d]
        add    eax, edx
        mov    dword ptr [ebx + 0x5d], eax
        add    dword ptr [ebx + 0x48], edx
        add    dword ptr [ebx + 0x40], edx
        mov    edx, dword ptr [ebx + 0x55]
        cmp    eax, edx
        jne    loc_1_E57C
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ecx + 0x10]
        or     esi, esi
        je     loc_1_E538
        mov    eax, dword ptr [ebx + 0x44]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 0x3c], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        jmp    loc_1_E535
loc_1_E266:
        cmp    dword ptr [ebx + 0x4c], 0
        jne    loc_1_E3C6
        push   dword ptr [esp + 0x24]
        push   ebp
        push   ebx
        call   sub_1_DDC0
        add    esp, 0xc
        mov    ebp, eax
        cmp    ebp, dword ptr [esp + 0x24]
        je     loc_1_E580
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        cmp    eax, 0xd
        jb     loc_1_E58B
        mov    ecx, dword ptr [ebx + 0x40]
        mov    esi, dword ptr [ecx + 8]
        mov    edx, esi
        and    edx, 0xff00
        mov    eax, esi
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, esi
        and    eax, 0xff0000
        shr    esi, 0x10
        or     eax, esi
        shr    eax, 8
        or     edx, eax
        movzx  eax, word ptr [ecx + 2]
        movzx  esi, byte ptr [ecx + 0xc]
        add    esi, eax
        movzx  eax, word ptr [ecx + 6]
        add    esi, eax
        movzx  eax, word ptr [ecx]
        add    esi, eax
        movzx  eax, word ptr [ecx + 4]
        add    esi, eax
        cmp    esi, edx
        je     loc_1_E2F3
        mov    dword ptr [ebx + 0x34], 1
        lea    eax, [ecx + ebp]
        mov    dword ptr [ebx + 0x40], eax
        jmp    loc_1_E57C
loc_1_E2F3:
        mov    dword ptr [ebx + 0x34], 0
        mov    ecx, dword ptr [ecx + 4]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebx + 0x55], edx
        push   edx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x44], eax
        mov    edi, eax
        mov    dword ptr [ebx + 0x48], edi
        mov    dword ptr [ebx + 0x5d], 0
        mov    esi, dword ptr [ebx + 0x40]
        add    esi, 0xc
        mov    dword ptr [ebx + 0x40], esi
        add    ebp, 0xc
        mov    edx, dword ptr [esp + 0x24]
        sub    edx, ebp
        mov    eax, dword ptr [ebx + 0x55]
        cmp    edx, eax
        jb     loc_1_E356
        mov    edx, eax
loc_1_E356:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebx + 0x40], edx
        add    dword ptr [ebx + 0x48], edx
        mov    eax, dword ptr [ebx + 0x5d]
        add    eax, edx
        mov    dword ptr [ebx + 0x5d], eax
        add    ebp, edx
        mov    esi, dword ptr [ebx + 0x55]
        cmp    eax, esi
        jne    loc_1_E57C
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [ecx + 0x10]
        or     edx, edx
        je     loc_1_E3A1
        mov    eax, dword ptr [ebx + 0x44]
        push   esi
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 0x3c], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   edx
        add    esp, 0x10
loc_1_E3A1:
        push   dword ptr [ebx + 0x44]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x5d], 0
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x48], 0
        jmp    loc_1_E57C
loc_1_E3C6:
        mov    eax, 0xd
        movzx  ecx, byte ptr [ebx + 0x54]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 0x24]
        sub    ecx, ebp
        cmp    eax, ecx
        jb     loc_1_E3DD
        mov    eax, ecx
loc_1_E3DD:
        mov    edi, dword ptr [ebx + 0x50]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, 0xd
        add    dword ptr [ebx + 0x50], eax
        movzx  edx, byte ptr [ebx + 0x54]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    loc_1_E415
        mov    cl, byte ptr [ebx + 0x54]
        add    cl, al
        mov    byte ptr [ebx + 0x54], cl
        add    dword ptr [ebx + 0x40], eax
        add    ebp, eax
        jmp    loc_1_E57C
loc_1_E415:
        mov    edx, dword ptr [ebx + 0x4c]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, ecx
        and    esi, 0xff00
        mov    edi, ecx
        shl    edi, 0x10
        or     esi, edi
        shl    esi, 8
        mov    edi, ecx
        and    edi, 0xff0000
        shr    ecx, 0x10
        or     edi, ecx
        shr    edi, 8
        or     esi, edi
        movzx  edi, word ptr [edx + 2]
        movzx  ecx, byte ptr [edx + 0xc]
        add    ecx, edi
        movzx  edi, word ptr [edx + 6]
        add    ecx, edi
        movzx  edi, word ptr [edx]
        add    ecx, edi
        movzx  edi, word ptr [edx + 4]
        add    ecx, edi
        cmp    ecx, esi
        jne    loc_1_E55A
        mov    dword ptr [ebx + 0x34], 0
        add    dword ptr [ebx + 0x40], eax
        add    ebp, eax
        mov    ecx, dword ptr [edx + 4]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebx + 0x55], edx
        push   edx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x44], eax
        mov    edi, eax
        mov    dword ptr [ebx + 0x48], edi
        mov    dword ptr [ebx + 0x5d], 0
        mov    esi, dword ptr [ebx + 0x4c]
        add    esi, 0xc
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    dword ptr [ebx + 0x48]
        inc    dword ptr [ebx + 0x5d]
        push   dword ptr [ebx + 0x4c]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x50], 0
        mov    dword ptr [ebx + 0x4c], 0
        mov    byte ptr [ebx + 0x54], 0
        mov    edx, dword ptr [ebx + 0x55]
        sub    edx, dword ptr [ebx + 0x5d]
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        cmp    edx, eax
        jb     loc_1_E4EB
        mov    edx, eax
loc_1_E4EB:
        mov    edi, dword ptr [ebx + 0x48]
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebp, edx
        mov    eax, dword ptr [ebx + 0x5d]
        add    eax, edx
        mov    dword ptr [ebx + 0x5d], eax
        add    dword ptr [ebx + 0x48], edx
        add    dword ptr [ebx + 0x40], edx
        mov    esi, dword ptr [ebx + 0x55]
        cmp    eax, esi
        jne    loc_1_E57C
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [ecx + 0x10]
        or     edx, edx
        je     loc_1_E538
        mov    eax, dword ptr [ebx + 0x44]
        push   esi
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 0x3c], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   edx
loc_1_E535:
        add    esp, 0x10
loc_1_E538:
        push   dword ptr [ebx + 0x44]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x48], 0
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x5d], 0
        jmp    loc_1_E57C
loc_1_E55A:
        push   edx
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 0x34], 1
        mov    dword ptr [ebx + 0x50], 0
        mov    dword ptr [ebx + 0x4c], 0
        mov    byte ptr [ebx + 0x54], 0
loc_1_E57C:
        cmp    ebp, dword ptr [esp + 0x24]
loc_1_E580:
        jb     loc_1_DED9
        jmp    loc_1_E68A
loc_1_E58B:
        push   0xd
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 0x4c], eax
        mov    edi, eax
        mov    dword ptr [ebx + 0x50], edi
        mov    eax, dword ptr [esp + 0x24]
        sub    eax, ebp
        mov    esi, dword ptr [ebx + 0x40]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    byte ptr [ebx + 0x54], al
        add    dword ptr [ebx + 0x50], eax
        add    dword ptr [ebx + 0x40], eax
        jmp    loc_1_E68A
loc_1_E5C2:
        mov    esi, dword ptr [ebx + 4]
        push   esi
        call   sub_1_35D1
        add    esp, 4
        cmp    byte ptr [eax + 0x1a], 0
        jne    loc_1_E68A
        cmp    dword ptr [0x13758], 0        ; -> loc_1_13758
        jne    loc_1_E68A
        push   esi
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    loc_1_E68A
        push   1
        push   0x12
        call   sub_1_F02D
        add    esp, 8
        jmp    loc_1_E68A
loc_1_E607:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_E62D
        push   0
        cmp    dword ptr [esp + 0x28], 1
        push   0
        sbb    ecx, ecx
        add    ecx, 0xe8
        push   ecx
        push   dword ptr [ebx + 4]
        call   eax
        add    esp, 0x10
loc_1_E62D:
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ecx + 8], eax
        jne    loc_1_E68A
        mov    eax, ecx
        add    eax, 0x14
        cmp    dword ptr [eax], 0
        je     loc_1_E68A
        push   eax
        call   sub_1_EE79
        add    esp, 4
        jmp    loc_1_E68A
loc_1_E652:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_E68A
        push   dword ptr [esp + 0x24]
        push   0
        push   0xff
        jmp    loc_1_E682
loc_1_E66B:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_E68A
        push   dword ptr [esp + 0x24]
        push   0
        push   0xe3
loc_1_E682:
        push   dword ptr [ebx + 4]
        call   eax
        add    esp, 0x10
loc_1_E68A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_E692:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0x7d0
        je     loc_1_E6AD
        cmp    eax, 0x7d1
        je     loc_1_E6D5
        cmp    eax, 0x7d2
        je     loc_1_E71F
        jmp    loc_1_E724
loc_1_E6AD:
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [ecx], 0
        jne    loc_1_E6CA
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        add    ecx, 0x14
        push   ecx
        call   sub_1_EE79
        add    esp, 4
        jmp    loc_1_E724
loc_1_E6CA:
        push   ecx
        call   sub_1_E842
        add    esp, 4
        jmp    loc_1_E724
loc_1_E6D5:
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_E6EF
        call   sub_1_F05F
        push   dword ptr [0x13934]           ; -> loc_1_13934
        call   sub_1_F0C3
        add    esp, 4
loc_1_E6EF:
        mov    dword ptr [0x13654], 0        ; -> loc_1_13654
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        or     eax, eax
        je     loc_1_E724
        mov    ecx, eax
        add    eax, 0x20
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax - 0x18], 0
        push   eax
        call   sub_1_EE79
        add    esp, 4
        jmp    loc_1_E724
loc_1_E71F:
        call   sub_1_F05F
loc_1_E724:
        sub    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_E730:
        mov    dword ptr [0x13934], 0x136a4  ; -> loc_1_13934
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x136b4], eax      ; -> loc_1_136B4
        push   dword ptr [0x13934]           ; -> loc_1_13934
        call   sub_1_E75D
        add    esp, 4
        or     eax, eax
        mov    eax, 0xfffffef9
        jne    loc_1_E75C
        sub    eax, eax
loc_1_E75C:
        ret    
sub_1_E75D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x18]
        sub    ebx, ebx
        cmp    dword ptr [esi], ebx
        je     loc_1_E775
        jmp    loc_1_E835
loc_1_E775:
        push   esi
        push   0xe692                        ; -> loc_1_E692
        call   sub_1_F870
        add    esp, 8
        or     eax, eax
        je     loc_1_E78C
        jmp    loc_1_E835
loc_1_E78C:
        mov    dword ptr [esi + 0x1c], 0xafc8
        lea    eax, [esi + 0x14]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        or     eax, eax
        je     loc_1_E7A8
        jmp    loc_1_E835
loc_1_E7A8:
        cmp    dword ptr [esi], 0
        jne    loc_1_E7B2
        jmp    loc_1_E835
loc_1_E7B2:
        mov    dword ptr [esi + 4], 0
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, -2
        je     loc_1_E7FD
loc_1_E7CD:
        push   0
        push   0
        push   ebp
        push   0x12
        call   sub_1_ED99
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_E7E8
        mov    ebx, 1
        add    dword ptr [esi + 8], ebx
loc_1_E7E8:
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        call   sub_1_F7D0
        add    esp, 8
        mov    ebp, eax
        cmp    ebp, -2
        jne    loc_1_E7CD
loc_1_E7FD:
        or     ebx, ebx
        je     loc_1_E81E
        mov    eax, dword ptr [esi + 8]
        add    eax, 9
        imul   eax, eax, 0x1f4
        mov    dword ptr [esi + 0x1c], eax
        lea    eax, [esi + 0x14]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        jmp    loc_1_E829
loc_1_E81E:
        mov    dword ptr [esi], 0
        mov    edi, 0xfffffefb
loc_1_E829:
        cmp    dword ptr [esi], 0
        mov    eax, edi
        jne    loc_1_E83A
        call   sub_1_F05F
loc_1_E835:
        mov    eax, 0xfffffef9
loc_1_E83A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_E842:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x18]
        sub    ebx, ebx
        cmp    dword ptr [esi], ebx
        jne    loc_1_E85C
        mov    eax, 0xfffffef9
        jmp    loc_1_E8AE
loc_1_E85C:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, -2
        je     loc_1_E89D
loc_1_E870:
        push   0
        push   0
        push   ebp
        push   0x12
        call   sub_1_ED99
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_E888
        mov    ebx, 1
loc_1_E888:
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        call   sub_1_F7D0
        add    esp, 8
        mov    ebp, eax
        cmp    ebp, -2
        jne    loc_1_E870
loc_1_E89D:
        or     ebx, ebx
        jne    loc_1_E8AC
        mov    dword ptr [esi], 0
        mov    edi, 0xfffffefb
loc_1_E8AC:
        mov    eax, edi
loc_1_E8AE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_E8C0:
        push   esi
        mov    esi, dword ptr [0x13934]      ; -> loc_1_13934
        push   dword ptr [esp + 0xc]
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        jne    loc_1_E8DE
        mov    eax, 0xfffffefd
        jmp    loc_1_E90D
loc_1_E8DE:
        mov    dword ptr [eax + 0x20], 1
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [esi + 0x10]
        or     ecx, ecx
        mov    dword ptr [eax + 0x24], ecx
        je     loc_1_E90B
        push   0
        push   0
        push   0xe9
        push   dword ptr [eax + 4]
        call   ecx
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_E90D
loc_1_E90B:
        sub    eax, eax
loc_1_E90D:
        pop    esi
        ret    
sub_1_E90F:
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [0x13934]      ; -> loc_1_13934
        push   dword ptr [esp + 0x10]
        call   sub_1_EE10
        add    esp, 4
        cmp    eax, edi
        jne    loc_1_E930
        mov    eax, 0x2b09
        jmp    loc_1_E954
loc_1_E930:
        mov    edx, dword ptr [esi + 0x10]
        or     edx, edx
        je     loc_1_E952
        push   dword ptr [esp + 0x18]
        push   0
        push   0xec
        push   dword ptr [eax + 4]
        call   edx
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        mov    eax, edi
        jne    loc_1_E954
loc_1_E952:
        mov    eax, edi
loc_1_E954:
        pop    edi
        pop    esi
        ret    
sub_1_E957:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, -0x1f
        jae    loc_1_E988
        push   esi
        push   8
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        push   ecx
        call   sub_1_F3E8
        add    esp, 0x14
        or     eax, eax
        je     loc_1_E988
        push   eax
        call   sub_1_EE44
        add    esp, 4
        jmp    loc_1_E9D6
loc_1_E988:
        lea    edi, [esi + 0x14]
        lea    eax, [esi + 8]
        push   eax
        push   esi
        push   0xde80                        ; -> loc_1_DE80
        push   edi
        push   dword ptr [esi + 4]
        call   sub_1_101B3
        add    esp, 0x14
        or     eax, eax
        je     loc_1_E9B0
        push   eax
        call   sub_1_EE4A
        add    esp, 4
        jmp    loc_1_E9D6
loc_1_E9B0:
        push   dword ptr [esi + 0x59]
        push   dword ptr [esi + 0x38]
        push   dword ptr [edi]
        call   sub_1_107D2
        add    esp, 0xc
        or     eax, eax
        je     loc_1_E9CF
        push   eax
        call   sub_1_EE4A
        add    esp, 4
        jmp    loc_1_E9D6
loc_1_E9CF:
        mov    dword ptr [esi + 0x18], 1
loc_1_E9D6:
        pop    edi
        pop    esi
        ret    
sub_1_E9D9:
        push   dword ptr [esp + 8]
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        jne    loc_1_E9F0
        mov    eax, 0xfffffefe
        jmp    loc_1_EA11
loc_1_E9F0:
        mov    ecx, dword ptr [0x13934]      ; -> loc_1_13934
        mov    ecx, dword ptr [ecx + 0x10]
        or     ecx, ecx
        je     loc_1_EA0F
        push   dword ptr [eax + 0x1c]
        push   0
        push   0xf0
        push   dword ptr [eax + 4]
        call   ecx
        add    esp, 0x10
loc_1_EA0F:
        sub    eax, eax
loc_1_EA11:
        ret    
sub_1_EA12:
        push   esi
        push   dword ptr [esp + 0xc]
        call   sub_1_EE10
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_EA2C
        mov    eax, 0xfffffefe
        jmp    loc_1_EA5A
loc_1_EA2C:
        mov    eax, dword ptr [0x13934]      ; -> loc_1_13934
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        je     loc_1_EA4A
        push   dword ptr [esi + 0x1c]
        push   0
        push   0xfc
        push   dword ptr [esi + 4]
        call   eax
        add    esp, 0x10
loc_1_EA4A:
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x1c], 0
        sub    eax, eax
loc_1_EA5A:
        pop    esi
        ret    
sub_1_EA5C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 0x14]
        call   sub_1_FBE0
        add    esp, 4
        push   dword ptr [esi + 0x10]
        call   sub_1_F483
        add    esp, 4
        mov    dword ptr [esi + 0x10], 0
        mov    dword ptr [esi + 0x14], 0
        push   esi
        call   sub_1_EB85
        add    esp, 4
        pop    esi
        ret    
sub_1_EA90:
        push   esi
        push   edi
        cmp    dword ptr [0x13728], 0        ; -> loc_1_13728
        je     loc_1_EB2C
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        mov    esi, dword ptr [eax*4 + 0x13728] ; -> loc_1_13728
        mov    edx, 0
        jbe    loc_1_EAC7
        mov    eax, 0x1372c                  ; -> loc_1_1372C
loc_1_EAB7:
        cmp    dword ptr [eax], esi
        je     loc_1_EAFE
        add    eax, 4
        inc    edx
        cmp    edx, dword ptr [0x13728]      ; -> loc_1_13728
        jb     loc_1_EAB7
loc_1_EAC7:
        cmp    dword ptr [0x136ec], 0xa      ; -> loc_1_136EC
        jae    loc_1_EAE2
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        mov    dword ptr [eax*4 + 0x136f0], esi ; -> loc_1_136F0
        inc    dword ptr [0x136ec]           ; -> loc_1_136EC
loc_1_EAE2:
        push   esi
        call   sub_1_ED66
        add    esp, 4
        mov    dword ptr [esi + 8], 8
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 4], eax
        mov    eax, esi
loc_1_EAFB:
        pop    edi
        pop    esi
        ret    
loc_1_EAFE:
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        dec    eax
        cmp    eax, edx
        jbe    loc_1_EB24
        lea    edi, [edx*4 + 0x1372c]        ; -> loc_1_1372C
loc_1_EB0F:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        dec    eax
        cmp    eax, edx
        ja     loc_1_EB0F
loc_1_EB24:
        dec    dword ptr [0x13728]           ; -> loc_1_13728
        jmp    loc_1_EAC7
loc_1_EB2C:
        push   0x61
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_EB40
        sub    eax, eax
        jmp    loc_1_EAFB
loc_1_EB40:
        mov    dword ptr [esi + 0x59], 0xc350
        push   0xc350
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0x38], eax
        mov    edx, dword ptr [esi + 0x59]
        mov    edi, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        cmp    dword ptr [esi + 0x38], eax
        jne    loc_1_EAC7
        push   esi
        call   sub_1_EC1B
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_EAFB
sub_1_EB85:
        push   esi
        push   edi
        sub    edx, edx
        cmp    dword ptr [0x136ec], edx      ; -> loc_1_136EC
        jbe    loc_1_EC18
        mov    eax, 0x136f0                  ; -> loc_1_136F0
        mov    ecx, dword ptr [esp + 0xc]
loc_1_EB9E:
        cmp    dword ptr [eax], ecx
        je     loc_1_EBB0
        add    eax, 4
        inc    edx
        cmp    edx, dword ptr [0x136ec]      ; -> loc_1_136EC
        jb     loc_1_EB9E
        jmp    loc_1_EC18
loc_1_EBB0:
        sub    esi, esi
        cmp    dword ptr [0x136ec], esi      ; -> loc_1_136EC
        jbe    loc_1_EBFD
        mov    eax, 0x136f0                  ; -> loc_1_136F0
loc_1_EBBF:
        cmp    dword ptr [eax], ecx
        je     loc_1_EBD1
        add    eax, 4
        inc    esi
        cmp    esi, dword ptr [0x136ec]      ; -> loc_1_136EC
        jb     loc_1_EBBF
        jmp    loc_1_EBFD
loc_1_EBD1:
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        dec    eax
        cmp    eax, esi
        jbe    loc_1_EBF7
        lea    edi, [esi*4 + 0x136f0]        ; -> loc_1_136F0
loc_1_EBE2:
        mov    edx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], edx
        mov    edi, eax
        inc    esi
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        dec    eax
        cmp    eax, esi
        ja     loc_1_EBE2
loc_1_EBF7:
        dec    dword ptr [0x136ec]           ; -> loc_1_136EC
loc_1_EBFD:
        cmp    dword ptr [0x13728], 0xa      ; -> loc_1_13728
        jae    loc_1_EC18
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        mov    dword ptr [eax*4 + 0x1372c], ecx ; -> loc_1_1372C
        inc    dword ptr [0x13728]           ; -> loc_1_13728
loc_1_EC18:
        pop    edi
        pop    esi
        ret    
sub_1_EC1B:
        push   esi
        push   edi
        sub    ecx, ecx
        cmp    dword ptr [0x136ec], ecx      ; -> loc_1_136EC
        jbe    loc_1_EC42
        mov    eax, 0x136f0                  ; -> loc_1_136F0
        mov    esi, dword ptr [esp + 0xc]
loc_1_EC30:
        cmp    dword ptr [eax], esi
        je     loc_1_EC6E
        add    eax, 4
        inc    ecx
        cmp    ecx, dword ptr [0x136ec]      ; -> loc_1_136EC
        jb     loc_1_EC30
        jmp    loc_1_EC46
loc_1_EC42:
        mov    esi, dword ptr [esp + 0xc]
loc_1_EC46:
        sub    eax, eax
        cmp    dword ptr [0x13728], eax      ; -> loc_1_13728
        jbe    loc_1_ED63
        mov    ecx, 0x1372c                  ; -> loc_1_1372C
loc_1_EC59:
        cmp    dword ptr [ecx], esi
        je     loc_1_EC6E
        add    ecx, 4
        inc    eax
        cmp    dword ptr [0x13728], eax      ; -> loc_1_13728
        ja     loc_1_EC59
        jmp    loc_1_ED63
loc_1_EC6E:
        mov    eax, dword ptr [esi + 0x38]
        or     eax, eax
        je     loc_1_EC7E
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_EC7E:
        sub    ecx, ecx
        cmp    dword ptr [0x136ec], ecx      ; -> loc_1_136EC
        jbe    loc_1_ECEC
        mov    eax, 0x136f0                  ; -> loc_1_136F0
loc_1_EC8D:
        cmp    dword ptr [eax], esi
        je     loc_1_EC9F
        add    eax, 4
        inc    ecx
        cmp    ecx, dword ptr [0x136ec]      ; -> loc_1_136EC
        jb     loc_1_EC8D
        jmp    loc_1_ECEC
loc_1_EC9F:
        sub    edx, edx
        cmp    dword ptr [0x136ec], edx      ; -> loc_1_136EC
        jbe    loc_1_ECEC
        mov    eax, 0x136f0                  ; -> loc_1_136F0
loc_1_ECAE:
        cmp    dword ptr [eax], esi
        je     loc_1_ECC0
        add    eax, 4
        inc    edx
        cmp    dword ptr [0x136ec], edx      ; -> loc_1_136EC
        ja     loc_1_ECAE
        jmp    loc_1_ECEC
loc_1_ECC0:
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        dec    eax
        cmp    eax, edx
        jbe    loc_1_ECE6
        lea    edi, [edx*4 + 0x136f0]        ; -> loc_1_136F0
loc_1_ECD1:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        dec    eax
        cmp    eax, edx
        ja     loc_1_ECD1
loc_1_ECE6:
        dec    dword ptr [0x136ec]           ; -> loc_1_136EC
loc_1_ECEC:
        sub    ecx, ecx
        cmp    dword ptr [0x13728], ecx      ; -> loc_1_13728
        jbe    loc_1_ED5A
        mov    eax, 0x1372c                  ; -> loc_1_1372C
loc_1_ECFB:
        cmp    dword ptr [eax], esi
        je     loc_1_ED0D
        add    eax, 4
        inc    ecx
        cmp    dword ptr [0x13728], ecx      ; -> loc_1_13728
        ja     loc_1_ECFB
        jmp    loc_1_ED5A
loc_1_ED0D:
        sub    edx, edx
        cmp    dword ptr [0x13728], edx      ; -> loc_1_13728
        jbe    loc_1_ED5A
        mov    eax, 0x1372c                  ; -> loc_1_1372C
loc_1_ED1C:
        cmp    dword ptr [eax], esi
        je     loc_1_ED2E
        add    eax, 4
        inc    edx
        cmp    edx, dword ptr [0x13728]      ; -> loc_1_13728
        jb     loc_1_ED1C
        jmp    loc_1_ED5A
loc_1_ED2E:
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        dec    eax
        cmp    eax, edx
        jbe    loc_1_ED54
        lea    edi, [edx*4 + 0x1372c]        ; -> loc_1_1372C
loc_1_ED3F:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        dec    eax
        cmp    eax, edx
        ja     loc_1_ED3F
loc_1_ED54:
        dec    dword ptr [0x13728]           ; -> loc_1_13728
loc_1_ED5A:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_ED63:
        pop    edi
        pop    esi
        ret    
sub_1_ED66:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    edi, edx
        mov    esi, dword ptr [edx + 0x38]
        mov    ebx, dword ptr [edx + 0x59]
        sub    eax, eax
        mov    ecx, 0x18
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    dword ptr [edx + 0x38], esi
        mov    dword ptr [edx + 0x59], ebx
        mov    edi, esi
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_ED99:
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        call   sub_1_EE10
        add    esp, 4
        cmp    eax, esi
        jne    loc_1_EDE2
        push   edi
        call   sub_1_EA90
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    loc_1_EDC4
        mov    eax, 0xfffffefa
        jmp    loc_1_EDE4
loc_1_EDC4:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edi], eax
        push   edi
        call   sub_1_E957
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_EDE2
        push   edi
        call   sub_1_EB85
        add    esp, 4
loc_1_EDE2:
        mov    eax, esi
loc_1_EDE4:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h
        db  00Ch, 0E8h, 0D3h, 009h, 000h, 000h, 083h, 0C4h, 008h, 0C3h, 0FFh, 074h, 024h, 008h, 0E8h, 03Bh
        db  00Ah, 000h, 000h, 083h, 0C4h, 004h, 0C3h, 000h, 000h
sub_1_EE10:
        push   esi
        sub    ecx, ecx
        cmp    dword ptr [0x136ec], ecx      ; -> loc_1_136EC
        jbe    loc_1_EE37
        mov    edx, 0x136f0                  ; -> loc_1_136F0
        mov    eax, dword ptr [esp + 8]
loc_1_EE24:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi + 4], eax
        je     loc_1_EE3B
        add    edx, 4
        inc    ecx
        cmp    dword ptr [0x136ec], ecx      ; -> loc_1_136EC
        ja     loc_1_EE24
loc_1_EE37:
        sub    eax, eax
loc_1_EE39:
        pop    esi
        ret    
loc_1_EE3B:
        mov    eax, dword ptr [ecx*4 + 0x136f0] ; -> loc_1_136F0
        jmp    loc_1_EE39
sub_1_EE44:
        mov    eax, 0xffffffff
        ret    
sub_1_EE4A:
        mov    eax, 0xffffffff
        ret    
loc_1_EE50:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     loc_1_EE75
        mov    dword ptr [eax], 0
        push   dword ptr [esp]
        call   sub_1_EE79
        add    esp, 4
loc_1_EE75:
        add    esp, 4
        ret    
sub_1_EE79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_EE94
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
loc_1_EE94:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_EE98:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_1_EF43
        cmp    dword ptr [esi], 0
        jne    loc_1_EF43
        cmp    dword ptr [esi + 4], 0
        jne    loc_1_EF43
        push   0
        call   sub_1_129B0
        mov    dword ptr [esi], eax
        or     eax, eax
        je     loc_1_EEEF
        push   esi
        push   dword ptr [esi + 8]
        push   0xee50                        ; -> loc_1_EE50
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    loc_1_EEEF
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_1_EEEF:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_EF43
        cmp    dword ptr [esi + 4], 0
        je     loc_1_EF43
        push   4
        push   eax
        call   sub_1_129C0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_EF2F
        push   eax
        call   sub_1_129E8
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], 0
        sub    eax, eax
        jmp    loc_1_EF48
loc_1_EF2F:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    dword ptr [esi], 0
loc_1_EF43:
        mov    eax, 1
loc_1_EF48:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h
sub_1_EF50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_1_EE10
        mov    ebx, eax
        add    esp, 4
        mov    ebp, dword ptr [ebx + 0x59]
        mov    eax, dword ptr [esp + 0x20]
        cmp    ebp, eax
        jb     loc_1_EF6F
        mov    ebp, eax
loc_1_EF6F:
        or     ebx, ebx
        jne    loc_1_EF7A
        mov    eax, 0xfffffefe
        jmp    loc_1_EFA7
loc_1_EF7A:
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [ebx + 0x3c]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [esp + 0x18]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     loc_1_EFA2
        inc    dword ptr [eax + 0x3d]
loc_1_EFA2:
        add    dword ptr [ebx + 0x3c], ebp
        mov    eax, ebp
loc_1_EFA7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_EFC0:
        push   esi
        push   edi
        call   sub_1_F05F
        call   sub_1_F0F0
        mov    eax, dword ptr [0x136ec]      ; -> loc_1_136EC
        lea    esi, [eax*4 + 0x136ec]        ; -> loc_1_136EC
        or     eax, eax
        jbe    loc_1_EFFB
        sub    edi, edi
loc_1_EFDE:
        push   dword ptr [esi]
        call   sub_1_EC1B
        add    esp, 4
        mov    dword ptr [esi], edi
        sub    esi, 4
        dec    dword ptr [0x136ec]           ; -> loc_1_136EC
        cmp    dword ptr [0x136ec], edi      ; -> loc_1_136EC
        ja     loc_1_EFDE
loc_1_EFFB:
        mov    eax, dword ptr [0x13728]      ; -> loc_1_13728
        lea    esi, [eax*4 + 0x13728]        ; -> loc_1_13728
        or     eax, eax
        jbe    loc_1_F02A
        sub    edi, edi
loc_1_F00D:
        push   dword ptr [esi]
        call   sub_1_EC1B
        add    esp, 4
        mov    dword ptr [esi], edi
        sub    esi, 4
        dec    dword ptr [0x13728]           ; -> loc_1_13728
        cmp    dword ptr [0x13728], edi      ; -> loc_1_13728
        ja     loc_1_F00D
loc_1_F02A:
        pop    edi
        pop    esi
        ret    
sub_1_F02D:
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        jne    loc_1_F038
        jmp    loc_1_F05C
loc_1_F038:
        mov    dword ptr [0x13654], 0        ; -> loc_1_13654
        cmp    dword ptr [esp + 8], 0
        je     loc_1_F05C
        call   sub_1_F05F
        push   dword ptr [0x13934]           ; -> loc_1_13934
        call   sub_1_F0C3
        add    esp, 4
loc_1_F05C:
        sub    eax, eax
        ret    
sub_1_F05F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    edi, eax
        cmp    edi, -2
        je     loc_1_F0BA
        sub    ebx, ebx
loc_1_F07B:
        push   edi
        call   sub_1_EE10
        add    esp, 4
        mov    esi, eax
        cmp    esi, ebx
        je     loc_1_F0A5
        cmp    dword ptr [esi + 0x20], ebx
        je     loc_1_F09C
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_E9D9
        add    esp, 8
loc_1_F09C:
        push   esi
        call   sub_1_EA5C
        add    esp, 4
loc_1_F0A5:
        lea    eax, [esp + 0xc]
        push   eax
        push   edi
        call   sub_1_F7D0
        add    esp, 8
        mov    edi, eax
        cmp    edi, -2
        jne    loc_1_F07B
loc_1_F0BA:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_F0C3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 0
        jne    loc_1_F0D4
        mov    eax, 0xfffffef9
        jmp    loc_1_F0EE
loc_1_F0D4:
        call   sub_1_F05F
        push   esi
        call   sub_1_FAEA
        add    esp, 4
        cmp    dword ptr [esi], 0
        mov    eax, 0xfffffef9
        jne    loc_1_F0EE
        sub    eax, eax
loc_1_F0EE:
        pop    esi
        ret    
sub_1_F0F0:
        call   sub_1_FC70
        jmp    loc_1_FAC0
        db  000h, 000h, 0CDh, 020h, 002h, 080h, 052h, 004h, 000h, 000h
WSVV_Service_00:
        mov    ax, 0x100
        clc    
        ret    
WSVV_Control:
        cmp    eax, 1
        je     sub_2_0880
        cmp    eax, 0x1b
        je     loc_2_0973
        cmp    eax, 0x1c
        je     loc_1_F12C
        cmp    eax, 0x23
        je     loc_1_D6D0
        clc    
        ret    
loc_1_F12C:
        call   sub_2_0200
        or     eax, eax
        je     loc_1_F137
        clc    
        ret    
loc_1_F137:
        stc    
        ret    
        db  000h, 000h, 000h
loc_1_F13C:
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        ror    eax, 0x10
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
sub_1_F151:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        jecxz  loc_1_F1CC
        test   esi, 3
        je     loc_1_F1AC
        test   esi, 1
        je     loc_1_F197
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        mov    edx, 0xf13c                   ; -> loc_1_F13C
        xchg   dword ptr [esp], edx
        push   edx
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        ror    eax, 0x10
        inc    esi
        dec    ecx
        je     loc_1_F1CC
        test   esi, 3
        je     loc_1_F1AC
loc_1_F197:
        cmp    ecx, 1
        je     loc_1_F1F9
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    ecx, 2
        je     loc_1_F1CC
loc_1_F1AC:
        cmp    ecx, 4
        jb     loc_1_F1C8
        mov    edx, ecx
        shr    ecx, 2
        and    dl, 3
        mov    eax, eax
loc_1_F1BC:
        add    eax, dword ptr [esi]
        adc    eax, 0
        add    esi, 4
        loop   loc_1_F1BC
        mov    ecx, edx
loc_1_F1C8:
        test   cl, cl
        jne    loc_1_F1D8
loc_1_F1CC:
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
loc_1_F1D8:
        cmp    cl, 1
        je     loc_1_F1F9
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    cl, 2
        jne    loc_1_F1F9
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
loc_1_F1F9:
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
        db  000h, 000h, 000h, 0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_F21C:
        VMMCall  VMM.Get_System_Time             ; cd203f800100
        VxDCall  Dev_0488.Service_8003h          ; cd2003808804
        VxDCall  Dev_0488.Service_8004h          ; cd2004808804
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        VxDCall  Dev_0488.Service_8005h          ; cd2005808804
        VxDCall  Dev_0488.Service_8006h          ; cd2006808804
        mov    eax, dword ptr [0x12b90]      ; -> obj1:0x12B90
        VxDCall  Dev_0488.Service_8009h          ; cd2009808804
        mov    eax, dword ptr [0x12b90]      ; -> obj1:0x12B90
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
        mov    eax, dword ptr [0x12b90]      ; -> obj1:0x12B90
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    byte ptr [eax], al
        db  000h
sub_1_F270:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        mov    ecx, eax
        or     ecx, ebx
        je     loc_1_F2AD
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_F2B1
        add    eax, esi
loc_1_F2AD:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_F2B1:
        xor    eax, eax
        jmp    loc_1_F2AD
sub_1_F2B5:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        mov    ecx, eax
        or     ecx, ebx
        je     loc_1_F2EE
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_F2F2
loc_1_F2EE:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_F2F2:
        xor    eax, eax
        jmp    loc_1_F2EE
        db  000h, 000h
sub_1_F2F8:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        movzx  eax, word ptr [esp + 0x18]
        movzx  esi, word ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_F334
        test   dword ptr [ebx], 0x20
        je     loc_1_F339
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_F334
        add    eax, esi
        cmp    eax, 0x110000
        jae    loc_1_F334
loc_1_F331:
        add    eax, dword ptr [ebx + 4]
loc_1_F334:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_F339:
        shl    eax, 4
        add    eax, esi
        jmp    loc_1_F331
loc_1_F340:
        ret    
sub_1_F341:
        push   ebx
        push   edi
        push   esi
        push   0
        push   0xf340                        ; -> loc_1_F340
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        pop    esi
        pop    edi
        pop    ebx
        ret    
        db  0FFh, 074h, 024h, 004h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h, 0CDh, 020h, 002h, 000h
        db  02Ah, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_1_F37E:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        pop    edi
        ret    
sub_1_F389:
        VxDCall  NDIS.Service_800Dh              ; cd200d802a00
        db  000h
sub_1_F390:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_F396:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_F3A3
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_F3A3:
        ret    
sub_1_F3A4:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        add    byte ptr [eax], al
sub_1_F3AC:
        VxDCall  Dev_003E.Service_8003h          ; cd2003803e00
sub_1_F3B2:
        VxDCall  Dev_003E.Service_8004h          ; cd2004803e00
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_F3C0:
        sub    eax, eax
        cmp    dword ptr [0x138a8], eax      ; -> loc_1_138A8
        jbe    loc_1_F3E7
        mov    ecx, dword ptr [0x138a4]      ; -> loc_1_138A4
        add    ecx, 8
        mov    edx, dword ptr [esp + 4]
loc_1_F3D7:
        cmp    dword ptr [ecx], edx
        je     loc_1_F3E7
        add    ecx, 0xc
        inc    eax
        cmp    dword ptr [0x138a8], eax      ; -> loc_1_138A8
        ja     loc_1_F3D7
loc_1_F3E7:
        ret    
sub_1_F3E8:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_1_F3C0
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [0x138a8]      ; -> loc_1_138A8
        dec    eax
        cmp    eax, esi
        jb     loc_1_F47B
        push   edi
        call   sub_1_F796
        add    esp, 4
        or     eax, eax
        je     loc_1_F417
        mov    eax, 0xfffffeff
        jmp    loc_1_F480
loc_1_F417:
        push   edi
        call   sub_1_F4C9
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    edx, eax
        or     edx, edx
        jne    loc_1_F433
        mov    eax, 0xfffffefa
        jmp    loc_1_F480
loc_1_F433:
        imul   esi, esi, 0xc
        mov    eax, dword ptr [0x138a4]      ; -> loc_1_138A4
        add    eax, esi
        cmp    dword ptr [eax], 0
        jne    loc_1_F449
        mov    eax, 0xfffffef9
        jmp    loc_1_F480
loc_1_F449:
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_F456
        mov    eax, 0xfffffefd
        jmp    loc_1_F480
loc_1_F456:
        mov    dword ptr [edx + 8], 1
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [0x138a4]      ; -> loc_1_138A4
        mov    eax, dword ptr [esi + eax + 4]
        mov    dword ptr [ecx + 4], eax
        sub    eax, eax
        jmp    loc_1_F480
loc_1_F47B:
        mov    eax, 0xfffffefe
loc_1_F480:
        pop    edi
        pop    esi
        ret    
sub_1_F483:
        sub    eax, eax
        cmp    dword ptr [0x13790], eax      ; -> loc_1_13790
        jbe    loc_1_F4A8
        mov    ecx, dword ptr [esp + 4]
loc_1_F491:
        cmp    eax, 0xa
        jae    loc_1_F4A8
        cmp    dword ptr [eax*4 + 0x13794], ecx ; -> loc_1_13794
        je     loc_1_F4AE
        inc    eax
        cmp    eax, dword ptr [0x13790]      ; -> loc_1_13790
        jb     loc_1_F491
loc_1_F4A8:
        mov    eax, 0xfffffef8
loc_1_F4AD:
        ret    
loc_1_F4AE:
        mov    dword ptr [ecx + 8], 0
        mov    dword ptr [ecx + 4], 0
        push   ecx
        call   sub_1_F57B
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_F4AD
sub_1_F4C9:
        push   esi
        push   edi
        cmp    dword ptr [0x137c0], 0        ; -> loc_1_137C0
        je     loc_1_F567
        mov    eax, dword ptr [0x137c0]      ; -> loc_1_137C0
        mov    esi, dword ptr [eax*4 + 0x137c0] ; -> loc_1_137C0
        mov    edi, 0
        jbe    loc_1_F502
loc_1_F4EB:
        cmp    edi, 0xa
        jae    loc_1_F502
        cmp    dword ptr [edi*4 + 0x137c4], esi ; -> loc_1_137C4
        je     loc_1_F531
        inc    edi
        cmp    edi, dword ptr [0x137c0]      ; -> loc_1_137C0
        jb     loc_1_F4EB
loc_1_F502:
        cmp    dword ptr [0x13790], 0xa      ; -> loc_1_13790
        jae    loc_1_F51D
        mov    eax, dword ptr [0x13790]      ; -> loc_1_13790
        mov    dword ptr [eax*4 + 0x13794], esi ; -> loc_1_13794
        inc    dword ptr [0x13790]           ; -> loc_1_13790
loc_1_F51D:
        push   esi
        call   sub_1_F776
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 4
        mov    dword ptr [esi], eax
        mov    eax, esi
loc_1_F52E:
        pop    edi
        pop    esi
        ret    
loc_1_F531:
        mov    eax, dword ptr [0x137c0]      ; -> loc_1_137C0
        dec    eax
        cmp    eax, edi
        jbe    loc_1_F55F
        lea    edx, [edi*4 + 0x137c4]        ; -> loc_1_137C4
loc_1_F542:
        cmp    edx, 0x137e8                  ; -> loc_1_137E8
        jae    loc_1_F55F
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        mov    eax, dword ptr [0x137c0]      ; -> loc_1_137C0
        dec    eax
        cmp    eax, edi
        ja     loc_1_F542
loc_1_F55F:
        dec    dword ptr [0x137c0]           ; -> loc_1_137C0
        jmp    loc_1_F502
loc_1_F567:
        push   0x20
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_F502
        sub    eax, eax
        jmp    loc_1_F52E
sub_1_F57B:
        push   esi
        push   edi
        sub    eax, eax
        cmp    dword ptr [0x13790], eax      ; -> loc_1_13790
        jbe    loc_1_F61E
        mov    edx, dword ptr [esp + 0xc]
loc_1_F58F:
        cmp    eax, 0xa
        jae    loc_1_F61E
        cmp    dword ptr [eax*4 + 0x13794], edx ; -> loc_1_13794
        je     loc_1_F5AC
        inc    eax
        cmp    eax, dword ptr [0x13790]      ; -> loc_1_13790
        jb     loc_1_F58F
        jmp    loc_1_F61E
loc_1_F5AC:
        sub    edi, edi
        cmp    dword ptr [0x13790], edi      ; -> loc_1_13790
        jbe    loc_1_F603
loc_1_F5B6:
        cmp    edi, 0xa
        jae    loc_1_F603
        cmp    dword ptr [edi*4 + 0x13794], edx ; -> loc_1_13794
        je     loc_1_F5CF
        inc    edi
        cmp    edi, dword ptr [0x13790]      ; -> loc_1_13790
        jb     loc_1_F5B6
        jmp    loc_1_F603
loc_1_F5CF:
        mov    eax, dword ptr [0x13790]      ; -> loc_1_13790
        dec    eax
        cmp    eax, edi
        jbe    loc_1_F5FD
        lea    esi, [edi*4 + 0x13794]        ; -> loc_1_13794
loc_1_F5E0:
        cmp    esi, 0x137b8                  ; -> loc_1_137B8
        jae    loc_1_F5FD
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [esi + 4]
        mov    dword ptr [esi], ecx
        mov    esi, eax
        inc    edi
        mov    eax, dword ptr [0x13790]      ; -> loc_1_13790
        dec    eax
        cmp    eax, edi
        ja     loc_1_F5E0
loc_1_F5FD:
        dec    dword ptr [0x13790]           ; -> loc_1_13790
loc_1_F603:
        cmp    dword ptr [0x137c0], 0xa      ; -> loc_1_137C0
        jae    loc_1_F61E
        mov    eax, dword ptr [0x137c0]      ; -> loc_1_137C0
        mov    dword ptr [eax*4 + 0x137c4], edx ; -> loc_1_137C4
        inc    dword ptr [0x137c0]           ; -> loc_1_137C0
loc_1_F61E:
        pop    edi
        pop    esi
        ret    
        db  056h, 057h, 02Bh, 0C0h, 039h, 005h
        dd  loc_1_13790              ; fixup
        db  08Bh, 04Ch, 024h, 00Ch, 076h, 017h, 083h, 0F8h, 00Ah, 073h, 012h, 039h, 00Ch, 085h
        dd  loc_1_13794              ; fixup
        db  074h, 037h, 040h, 03Bh, 005h
        dd  loc_1_13790              ; fixup
        db  072h, 0E9h, 02Bh, 0C0h, 039h, 005h
        dd  loc_1_137C0              ; fixup
        db  00Fh, 086h, 01Dh, 001h, 000h, 000h, 083h, 0F8h, 00Ah, 00Fh, 083h, 014h, 001h, 000h, 000h, 039h
        db  00Ch, 085h
        dd  loc_1_137C4              ; fixup
        db  074h, 00Eh, 040h, 03Bh, 005h
        dd  loc_1_137C0              ; fixup
        db  072h, 0E5h, 0E9h, 0FDh, 000h, 000h, 000h, 02Bh, 0C0h, 039h, 005h
        dd  loc_1_13790              ; fixup
        db  076h, 070h, 083h, 0F8h, 00Ah, 073h, 06Bh, 039h, 00Ch, 085h
        dd  loc_1_13794              ; fixup
        db  074h, 00Bh, 040h, 03Bh, 005h
        dd  loc_1_13790              ; fixup
        db  072h, 0E9h, 0EBh, 057h, 02Bh, 0FFh, 039h, 03Dh
        dd  loc_1_13790              ; fixup
        db  076h, 04Dh, 083h, 0FFh, 00Ah, 073h, 048h, 039h, 00Ch, 0BDh
        dd  loc_1_13794              ; fixup
        db  074h, 00Bh, 047h, 03Bh, 03Dh
        dd  loc_1_13790              ; fixup
        db  072h, 0E9h, 0EBh, 034h, 0A1h
        dd  loc_1_13790              ; fixup
        db  048h, 03Bh, 0C7h, 076h, 024h, 08Dh, 034h, 0BDh
        dd  loc_1_13794              ; fixup
        db  081h, 0FEh
        dd  loc_1_137B8              ; fixup
        db  073h, 015h, 08Bh, 056h, 004h, 08Dh, 046h, 004h, 089h, 016h, 08Bh, 0F0h, 047h, 0A1h
        dd  loc_1_13790              ; fixup
        db  048h, 03Bh, 0C7h, 077h, 0E3h, 0FFh, 00Dh
        dd  loc_1_13790              ; fixup
        db  02Bh, 0C0h, 039h, 005h
        dd  loc_1_137C0              ; fixup
        db  076h, 070h, 083h, 0F8h, 00Ah, 073h, 06Bh, 039h, 00Ch, 085h
        dd  loc_1_137C4              ; fixup
        db  074h, 00Bh, 040h, 03Bh, 005h
        dd  loc_1_137C0              ; fixup
        db  072h, 0E9h, 0EBh, 057h, 02Bh, 0FFh, 039h, 03Dh
        dd  loc_1_137C0              ; fixup
        db  076h, 04Dh, 083h, 0FFh, 00Ah, 073h, 048h, 039h, 00Ch, 0BDh
        dd  loc_1_137C4              ; fixup
        db  074h, 00Bh, 047h, 03Bh, 03Dh
        dd  loc_1_137C0              ; fixup
        db  072h, 0E9h, 0EBh, 034h, 0A1h
        dd  loc_1_137C0              ; fixup
        db  048h, 03Bh, 0C7h, 076h, 024h, 08Dh, 034h, 0BDh
        dd  loc_1_137C4              ; fixup
        db  081h, 0FEh
        dd  loc_1_137E8              ; fixup
        db  073h, 015h, 08Bh, 056h, 004h, 08Dh, 046h, 004h, 089h, 016h, 08Bh, 0F0h, 047h, 0A1h
        dd  loc_1_137C0              ; fixup
        db  048h, 03Bh, 0C7h, 077h, 0E3h, 0FFh, 00Dh
        dd  loc_1_137C0              ; fixup
        db  051h, 0E8h, 026h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 05Fh, 05Eh, 0C3h
sub_1_F776:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0xffffffff
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        ret    
sub_1_F796:
        push   esi
        sub    ecx, ecx
        cmp    dword ptr [0x13790], ecx      ; -> loc_1_13790
        jbe    loc_1_F7BC
        mov    edx, 0x13794                  ; -> loc_1_13794
        mov    eax, dword ptr [esp + 8]
loc_1_F7AA:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi], eax
        je     loc_1_F7C0
        add    edx, 4
        inc    ecx
        cmp    ecx, dword ptr [0x13790]      ; -> loc_1_13790
        jb     loc_1_F7AA
loc_1_F7BC:
        sub    eax, eax
loc_1_F7BE:
        pop    esi
        ret    
loc_1_F7C0:
        mov    eax, 1
        jmp    loc_1_F7BE
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_F7D0:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        je     loc_1_F7DF
        mov    dword ptr [esi], 0xfffffffe
loc_1_F7DF:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, -1
        jne    loc_1_F7EC
        sub    eax, eax
        jmp    loc_1_F7F6
loc_1_F7EC:
        push   eax
        call   sub_1_F3C0
        add    esp, 4
        inc    eax
loc_1_F7F6:
        cmp    dword ptr [0x138a8], eax      ; -> loc_1_138A8
        jbe    loc_1_F81A
        mov    ecx, eax
        imul   ecx, ecx, 0xc
        add    ecx, dword ptr [0x138a4]      ; -> loc_1_138A4
loc_1_F809:
        cmp    dword ptr [ecx], 0
        jne    loc_1_F821
        add    ecx, 0xc
        inc    eax
        cmp    dword ptr [0x138a8], eax      ; -> loc_1_138A8
        ja     loc_1_F809
loc_1_F81A:
        mov    eax, 0xfffffffe
loc_1_F81F:
        pop    esi
        ret    
loc_1_F821:
        or     esi, esi
        je     loc_1_F836
        mov    ecx, eax
        imul   ecx, ecx, 0xc
        mov    edx, dword ptr [0x138a4]      ; -> loc_1_138A4
        mov    ecx, dword ptr [ecx + edx + 8]
        mov    dword ptr [esi], ecx
loc_1_F836:
        imul   eax, eax, 0xc
        mov    ecx, dword ptr [0x138a4]      ; -> loc_1_138A4
        mov    eax, dword ptr [eax + ecx + 8]
        jmp    loc_1_F81F
sub_1_F845:
        cmp    dword ptr [0x138a8], 0        ; -> loc_1_138A8
        mov    eax, 0xfffffffe
        je     loc_1_F861
        push   dword ptr [esp + 4]
        push   -1
        call   sub_1_F7D0
        add    esp, 8
loc_1_F861:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_F870:
        push   1
        push   0x12b94                       ; -> loc_1_12B94
        push   0xf95e                        ; -> loc_1_F95E
        call   sub_1_12910
        add    esp, 0xc
        mov    dword ptr [0x13594], eax      ; -> loc_1_13594
        or     eax, eax
        mov    eax, 0xffffffff
        je     loc_1_F8C4
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x138c4], eax      ; -> sub_1_138C4
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x138c0], eax      ; -> loc_1_138C0
        sub    eax, eax
        mov    dword ptr [0x138a8], 0        ; -> loc_1_138A8
        mov    dword ptr [0x13898], 0        ; -> loc_1_13898
        mov    dword ptr [0x138bc], 0        ; -> loc_1_138BC
loc_1_F8C4:
        ret    
loc_1_F8C5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, 1
        cmp    dword ptr [esi + 0xc], 0
        je     loc_1_F923
        cmp    dword ptr [0x13898], 0        ; -> loc_1_13898
        je     loc_1_F900
        cmp    dword ptr [0x138c4], 0        ; -> sub_1_138C4
        je     loc_1_F900
        push   dword ptr [0x138c0]           ; -> loc_1_138C0
        push   0
        push   0x7d2
        push   0
        call   sub_1_138C4
        add    esp, 0x10
loc_1_F900:
        push   dword ptr [esi + 8]
        push   dword ptr [esi]
        call   sub_1_F9D8
        add    esp, 8
        or     eax, eax
        jne    loc_1_F923
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [0x138a8], eax      ; -> loc_1_138A8
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [0x13898], eax      ; -> loc_1_13898
        jmp    loc_1_F925
loc_1_F923:
        sub    edi, edi
loc_1_F925:
        mov    dword ptr [0x1389c], edi      ; -> loc_1_1389C
        cmp    dword ptr [0x138c4], 0        ; -> sub_1_138C4
        je     loc_1_F94B
        push   dword ptr [0x138c0]           ; -> loc_1_138C0
        push   edi
        push   0x7d0
        push   0
        call   sub_1_138C4
        add    esp, 0x10
loc_1_F94B:
        or     edi, edi
        jne    loc_1_F959
        push   0
        call   sub_1_FAEA
        add    esp, 4
loc_1_F959:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_F95E:
        push   0x12bcc                       ; -> loc_1_12BCC
        push   0
        push   0x10040
        call   sub_1_12918
        add    esp, 0xc
        mov    dword ptr [0x12bc6], eax      ; -> loc_1_12BC6
        push   0x12bc4                       ; -> loc_1_12BC4
        push   6
        push   0xa6
        push   0x12bd8                       ; -> loc_1_12BD8
        call   sub_1_12928
        add    esp, 0x10
        mov    dword ptr [0x13894], eax      ; -> loc_1_13894
        push   dword ptr [0x12bc6]           ; -> loc_1_12BC6
        call   sub_1_12920
        add    esp, 4
        cmp    dword ptr [0x13894], 0x20     ; -> loc_1_13894
        jge    loc_1_F9D7
        cmp    dword ptr [0x138c4], 0        ; -> sub_1_138C4
        je     loc_1_F9CD
        push   dword ptr [0x138c0]           ; -> loc_1_138C0
        push   0
        push   0x7d0
        push   0
        call   sub_1_138C4
        add    esp, 0x10
loc_1_F9CD:
        mov    dword ptr [0x13894], 0        ; -> loc_1_13894
loc_1_F9D7:
        ret    
sub_1_F9D8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x1c]
        cmp    esi, edi
        je     loc_1_FAAD
        cmp    dword ptr [0x138a8], esi      ; -> loc_1_138A8
        je     loc_1_FA09
        push   dword ptr [0x138a4]           ; -> loc_1_138A4
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [0x138a4], edi      ; -> loc_1_138A4
loc_1_FA09:
        cmp    dword ptr [0x138a4], 0        ; -> loc_1_138A4
        jne    loc_1_FA35
        mov    eax, esi
        imul   eax, eax, 0xc
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [0x138a4], eax      ; -> loc_1_138A4
        cmp    dword ptr [0x138a4], 0        ; -> loc_1_138A4
        jne    loc_1_FA35
        mov    eax, 0xfffffefa
        jmp    loc_1_FAB2
loc_1_FA35:
        mov    eax, esi
        imul   eax, eax, 0x44
        mov    dword ptr [esp + 0x10], eax
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    loc_1_FA51
        jmp    loc_1_FAAD
loc_1_FA51:
        or     esi, esi
        je     loc_1_FA94
        lea    ebx, [eax + 4]
        sub    edx, edx
loc_1_FA5A:
        mov    ebp, dword ptr [ebx]
        mov    ecx, dword ptr [0x138a4]      ; -> loc_1_138A4
        mov    dword ptr [edx + ecx], ebp
        mov    ebp, dword ptr [0x138a4]      ; -> loc_1_138A4
        add    ebp, edx
        cmp    dword ptr [ebp], 0
        je     loc_1_FA8B
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [ebp + 4], ecx
        mov    ecx, dword ptr [ebx + 0x3c]
        mov    edi, dword ptr [0x138a4]      ; -> loc_1_138A4
        mov    dword ptr [edx + edi + 8], ecx
        mov    edi, 1
loc_1_FA8B:
        add    ebx, 0x44
        add    edx, 0xc
        dec    esi
        jne    loc_1_FA5A
loc_1_FA94:
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_1_F2B5
        add    esp, 8
        cmp    edi, 1
        sbb    eax, eax
        and    eax, 0xfffffef9
        jmp    loc_1_FAB2
loc_1_FAAD:
        mov    eax, 0xfffffef9
loc_1_FAB2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_FAC0:
        cmp    dword ptr [0x1389c], 0        ; -> loc_1_1389C
        je     loc_1_FAE7
        push   0
        call   sub_1_FAEA
        add    esp, 4
        cmp    eax, 0xfffffef7
        jne    loc_1_FAE7
        push   0x138b0                       ; -> loc_1_138B0
        call   sub_1_EE98
        add    esp, 4
loc_1_FAE7:
        sub    eax, eax
        ret    
sub_1_FAEA:
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x138c0], eax      ; -> loc_1_138C0
        cmp    dword ptr [0x13894], 0        ; -> loc_1_13894
        je     loc_1_FB16
        push   0
        push   0x7d2
        call   sub_1_FBC0
        add    esp, 8
        mov    eax, 0xfffffef7
        jmp    loc_1_FB2D
loc_1_FB16:
        lea    edi, [esp + 4]
        sub    eax, eax
        push   eax
        push   eax
        stosd  dword ptr es:[edi], eax
        push   dword ptr [esp + 0xc]
        call   sub_1_FB32
        add    esp, 0xc
        sub    eax, eax
loc_1_FB2D:
        pop    edi
        add    esp, 4
        ret    
sub_1_FB32:
        cmp    dword ptr [0x138bc], 0        ; -> loc_1_138BC
        jne    loc_1_FB8C
        inc    dword ptr [0x138bc]           ; -> loc_1_138BC
        cmp    dword ptr [0x138a4], 0        ; -> loc_1_138A4
        je     loc_1_FB58
        push   dword ptr [0x138a4]           ; -> loc_1_138A4
        call   sub_1_F396
        add    esp, 4
loc_1_FB58:
        cmp    dword ptr [0x13894], 0        ; -> loc_1_13894
        je     loc_1_FB86
        cmp    dword ptr [0x138c4], 0        ; -> sub_1_138C4
        je     loc_1_FB86
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [0x138c0]           ; -> loc_1_138C0
        push   dword ptr [eax]
        push   0x7d1
        push   0
        call   sub_1_138C4
        add    esp, 0x10
loc_1_FB86:
        dec    dword ptr [0x138bc]           ; -> loc_1_138BC
loc_1_FB8C:
        mov    dword ptr [0x138a4], 0        ; -> loc_1_138A4
        mov    dword ptr [0x138a8], 0        ; -> loc_1_138A8
        mov    dword ptr [0x13894], 0        ; -> loc_1_13894
        push   0x138b0                       ; -> loc_1_138B0
        call   sub_1_EE79
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_FBC0:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        push   0x111
        push   dword ptr [0x13898]           ; -> loc_1_13898
        call   sub_1_F341
        add    esp, 0x10
        ret    
        db  000h, 000h, 000h, 000h
sub_1_FBE0:
        push   esi
        push   edi
        sub    edi, edi
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_FC6B
        mov    esi, dword ptr [esp + 0xc]
loc_1_FBF2:
        cmp    eax, 0xa
        jae    loc_1_FC6B
        cmp    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_FC0B
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_FBF2
        jmp    loc_1_FC6B
loc_1_FC0B:
        cmp    dword ptr [0x12f5c], 0        ; -> obj1:0x12F5C
        je     loc_1_FC29
        push   dword ptr [esi + 0x19e]
        call   sub_1_129E8
        mov    dword ptr [esi + 0x19e], 0
loc_1_FC29:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_1_FC62
        cmp    eax, -0x15
        jae    loc_1_FC62
        cmp    dword ptr [esi + 0x4c], 0
        je     loc_1_FC44
        push   esi
        call   sub_1_1182F
        add    esp, 4
loc_1_FC44:
        push   0
        push   0
        push   0xeffffff2
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        cmp    eax, 1
        je     loc_1_FC62
        mov    edi, 4
loc_1_FC62:
        push   esi
        call   sub_1_FEB9
        add    esp, 4
loc_1_FC6B:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_FC70:
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0x12f04], edi      ; -> obj1:0x12F04
        je     loc_1_FC9F
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        lea    esi, [eax*4 + 0x12f30]        ; -> obj1:0x12F30
loc_1_FC88:
        push   dword ptr [esi]
        call   sub_1_FBE0
        add    esp, 4
        mov    edi, eax
        sub    esi, 4
        cmp    esi, 0x12f30                  ; -> obj1:0x12F30
        jne    loc_1_FC88
loc_1_FC9F:
        cmp    dword ptr [0x12f30], 0        ; -> obj1:0x12F30
        je     loc_1_FCC6
        sub    esi, esi
loc_1_FCAA:
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        push   dword ptr [eax*4 + 0x12f30]   ; -> obj1:0x12F30
        call   sub_1_FF5F
        add    esp, 4
        cmp    dword ptr [0x12f30], esi      ; -> obj1:0x12F30
        jne    loc_1_FCAA
loc_1_FCC6:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  056h, 02Bh, 0C9h, 039h, 00Dh
        dd  obj1:0x12F04             ; fixup
        db  076h, 01Ch, 0BAh
        dd  obj1:0x12F08             ; fixup
        db  08Bh, 044h, 024h, 008h, 08Bh, 032h, 039h, 046h, 018h, 074h, 010h, 083h, 0C2h, 004h, 041h, 03Bh
        db  00Dh
        dd  obj1:0x12F04             ; fixup
        db  072h, 0EDh, 02Bh, 0C0h, 05Eh, 0C3h, 08Bh, 004h, 08Dh
        dd  obj1:0x12F08             ; fixup
        db  0EBh, 0F5h
sub_1_FCFF:
        push   esi
        push   edi
        cmp    dword ptr [0x12f30], 0        ; -> obj1:0x12F30
        je     loc_1_FD7A
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        mov    esi, dword ptr [eax*4 + 0x12f30] ; -> obj1:0x12F30
        mov    edi, 0
        jbe    loc_1_FE3E
loc_1_FD21:
        cmp    edi, 0xa
        jae    loc_1_FE3E
        cmp    dword ptr [edi*4 + 0x12f34], esi ; -> obj1:0x12F34
        je     loc_1_FD41
        inc    edi
        cmp    edi, dword ptr [0x12f30]      ; -> obj1:0x12F30
        jb     loc_1_FD21
        jmp    loc_1_FE3E
loc_1_FD41:
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        dec    eax
        cmp    eax, edi
        jbe    loc_1_FD6F
        lea    edx, [edi*4 + 0x12f34]        ; -> obj1:0x12F34
loc_1_FD52:
        cmp    edx, 0x12f58                  ; -> obj1:0x12F58
        jae    loc_1_FD6F
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        dec    eax
        cmp    eax, edi
        ja     loc_1_FD52
loc_1_FD6F:
        dec    dword ptr [0x12f30]           ; -> obj1:0x12F30
        jmp    loc_1_FE3E
loc_1_FD7A:
        push   0x1ba
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_FD94
        sub    eax, eax
        jmp    loc_1_FEB6
loc_1_FD94:
        push   0x9c40
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0xa4], eax
        or     eax, eax
        jne    loc_1_FDBB
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_FEB6
loc_1_FDBB:
        mov    dword ptr [esi + 0xa8], 0x9c40
        push   0x9c40
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0xac], eax
        mov    dword ptr [esi + 0xb4], eax
        or     eax, eax
        jne    loc_1_FDF2
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_FEB6
loc_1_FDF2:
        mov    dword ptr [esi + 0xb0], 0x9c40
        mov    dword ptr [esi + 0xb8], 0x9c40
        mov    dword ptr [esi + 0x10], 0
        push   0x9c40
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0xe0], eax
        or     eax, eax
        jne    loc_1_FE34
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_FEB6
loc_1_FE34:
        mov    dword ptr [esi + 0xe4], 0x9c40
loc_1_FE3E:
        push   esi
        call   sub_1_100ED
        add    esp, 4
        or     esi, esi
        je     loc_1_FEB4
        cmp    dword ptr [0x12f04], 0xa      ; -> obj1:0x12F04
        jae    loc_1_FE66
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        mov    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        inc    dword ptr [0x12f04]           ; -> obj1:0x12F04
loc_1_FE66:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [esi + 0x70], 0x3e8
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x58], 1
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0xb4]
        add    eax, 0x9c3f
        mov    dword ptr [esi + 0xc0], eax
        mov    dword ptr [esi + 0xd8], 0xea60
        push   esi
        call   sub_1_11F60
        add    esp, 4
loc_1_FEB4:
        mov    eax, esi
loc_1_FEB6:
        pop    edi
        pop    esi
        ret    
sub_1_FEB9:
        push   esi
        push   edi
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_FF5C
        mov    edx, dword ptr [esp + 0xc]
loc_1_FECD:
        cmp    eax, 0xa
        jae    loc_1_FF5C
        cmp    dword ptr [eax*4 + 0x12f08], edx ; -> obj1:0x12F08
        je     loc_1_FEEA
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_FECD
        jmp    loc_1_FF5C
loc_1_FEEA:
        sub    edi, edi
        cmp    dword ptr [0x12f04], edi      ; -> obj1:0x12F04
        jbe    loc_1_FF41
loc_1_FEF4:
        cmp    edi, 0xa
        jae    loc_1_FF41
        cmp    dword ptr [edi*4 + 0x12f08], edx ; -> obj1:0x12F08
        je     loc_1_FF0D
        inc    edi
        cmp    edi, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_FEF4
        jmp    loc_1_FF41
loc_1_FF0D:
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        dec    eax
        cmp    eax, edi
        jbe    loc_1_FF3B
        lea    esi, [edi*4 + 0x12f08]        ; -> obj1:0x12F08
loc_1_FF1E:
        cmp    esi, 0x12f2c                  ; -> obj1:0x12F2C
        jae    loc_1_FF3B
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [esi + 4]
        mov    dword ptr [esi], ecx
        mov    esi, eax
        inc    edi
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        dec    eax
        cmp    eax, edi
        ja     loc_1_FF1E
loc_1_FF3B:
        dec    dword ptr [0x12f04]           ; -> obj1:0x12F04
loc_1_FF41:
        cmp    dword ptr [0x12f30], 0xa      ; -> obj1:0x12F30
        jae    loc_1_FF5C
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        mov    dword ptr [eax*4 + 0x12f34], edx ; -> obj1:0x12F34
        inc    dword ptr [0x12f30]           ; -> obj1:0x12F30
loc_1_FF5C:
        pop    edi
        pop    esi
        ret    
sub_1_FF5F:
        push   esi
        push   edi
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        mov    esi, dword ptr [esp + 0xc]
        jbe    loc_1_FF86
loc_1_FF6F:
        cmp    eax, 0xa
        jae    loc_1_FF86
        cmp    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_FFB4
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_FF6F
loc_1_FF86:
        sub    eax, eax
        cmp    dword ptr [0x12f30], eax      ; -> obj1:0x12F30
        jbe    loc_1_100EA
loc_1_FF94:
        cmp    eax, 0xa
        jae    loc_1_100EA
        cmp    dword ptr [eax*4 + 0x12f34], esi ; -> obj1:0x12F34
        je     loc_1_FFB4
        inc    eax
        cmp    dword ptr [0x12f30], eax      ; -> obj1:0x12F30
        ja     loc_1_FF94
        jmp    loc_1_100EA
loc_1_FFB4:
        mov    eax, dword ptr [esi + 0xa4]
        or     eax, eax
        je     loc_1_FFC7
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFC7:
        mov    eax, dword ptr [esi + 0xb4]
        or     eax, eax
        je     loc_1_FFDA
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFDA:
        mov    eax, dword ptr [esi + 0xe0]
        or     eax, eax
        je     loc_1_FFED
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFED:
        push   esi
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_10070
loc_1_10000:
        cmp    eax, 0xa
        jae    loc_1_10070
        cmp    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_10019
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_10000
        jmp    loc_1_10070
loc_1_10019:
        sub    edi, edi
        cmp    dword ptr [0x12f04], edi      ; -> obj1:0x12F04
        jbe    loc_1_10070
loc_1_10023:
        cmp    edi, 0xa
        jae    loc_1_10070
        cmp    dword ptr [edi*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_1003C
        inc    edi
        cmp    edi, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_10023
        jmp    loc_1_10070
loc_1_1003C:
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        dec    eax
        cmp    eax, edi
        jbe    loc_1_1006A
        lea    edx, [edi*4 + 0x12f08]        ; -> obj1:0x12F08
loc_1_1004D:
        cmp    edx, 0x12f2c                  ; -> obj1:0x12F2C
        jae    loc_1_1006A
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        mov    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        dec    eax
        cmp    eax, edi
        ja     loc_1_1004D
loc_1_1006A:
        dec    dword ptr [0x12f04]           ; -> obj1:0x12F04
loc_1_10070:
        sub    eax, eax
        cmp    dword ptr [0x12f30], eax      ; -> obj1:0x12F30
        jbe    loc_1_100EA
loc_1_1007A:
        cmp    eax, 0xa
        jae    loc_1_100EA
        cmp    dword ptr [eax*4 + 0x12f34], esi ; -> obj1:0x12F34
        je     loc_1_10093
        inc    eax
        cmp    dword ptr [0x12f30], eax      ; -> obj1:0x12F30
        ja     loc_1_1007A
        jmp    loc_1_100EA
loc_1_10093:
        sub    edi, edi
        cmp    dword ptr [0x12f30], edi      ; -> obj1:0x12F30
        jbe    loc_1_100EA
loc_1_1009D:
        cmp    edi, 0xa
        jae    loc_1_100EA
        cmp    dword ptr [edi*4 + 0x12f34], esi ; -> obj1:0x12F34
        je     loc_1_100B6
        inc    edi
        cmp    edi, dword ptr [0x12f30]      ; -> obj1:0x12F30
        jb     loc_1_1009D
        jmp    loc_1_100EA
loc_1_100B6:
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        dec    eax
        cmp    eax, edi
        jbe    loc_1_100E4
        lea    edx, [edi*4 + 0x12f34]        ; -> obj1:0x12F34
loc_1_100C7:
        cmp    edx, 0x12f58                  ; -> obj1:0x12F58
        jae    loc_1_100E4
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        mov    eax, dword ptr [0x12f30]      ; -> obj1:0x12F30
        dec    eax
        cmp    eax, edi
        ja     loc_1_100C7
loc_1_100E4:
        dec    dword ptr [0x12f30]           ; -> obj1:0x12F30
loc_1_100EA:
        pop    edi
        pop    esi
        ret    
sub_1_100ED:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [edx + 0xa4]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [edx + 0xac]
        mov    edi, edx
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [edx + 0xe0]
        mov    dword ptr [esp + 0x14], eax
        mov    esi, dword ptr [edx + 0xa8]
        sub    eax, eax
        mov    ebx, dword ptr [edx + 0xb0]
        mov    ebp, dword ptr [edx + 0xe4]
        mov    ecx, 0x6e
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, esi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, esi
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 0xa4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0xac], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0xe0], eax
        mov    dword ptr [edx + 0xa8], esi
        mov    dword ptr [edx + 0xb0], ebx
        mov    dword ptr [edx + 0xe4], ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0xb4], eax
        mov    dword ptr [edx + 0xb8], ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_101B3:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    ebx, dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_FCFF
        mov    edi, dword ptr [esp + 0x1c]
        add    esp, 4
        mov    dword ptr [edi], eax
        mov    ecx, eax
        cmp    ecx, esi
        jne    loc_1_101DC
        mov    esi, 3
        jmp    loc_1_1025F
loc_1_101DC:
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0x74], eax
        mov    dword ptr [ecx + 0x50], 0
        mov    dword ptr [ecx + 0x54], 0
        cmp    ebx, -0x1f
        jb     loc_1_101FD
        mov    esi, 4
        jmp    loc_1_1025F
loc_1_101FD:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x14], eax
        push   0
        push   0x11570                       ; -> loc_1_11570
        push   0xeffffff1
        push   eax
        call   sub_1_12958
        add    esp, 0x10
        cmp    eax, 1
        je     loc_1_10224
        mov    esi, 4
        jmp    loc_1_1025F
loc_1_10224:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 0x1b2], eax
        mov    ebx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x5c], ebx
        push   ecx
        call   sub_1_1180D
        add    esp, 4
        or     eax, eax
        je     loc_1_1024B
        mov    esi, 4
        jmp    loc_1_1025F
loc_1_1024B:
        mov    ebp, dword ptr [edi]
        mov    dword ptr [ebp + 0x4c], 1
        mov    dword ptr [ebp + 0x5c], ebx
        call   sub_1_129A4
        mov    dword ptr [ebp], eax
loc_1_1025F:
        or     esi, esi
        je     loc_1_10272
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_1_10272
        push   eax
        call   sub_1_FBE0
        add    esp, 4
loc_1_10272:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_10279:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    dword ptr [esp + 0x18], 0x30
        mov    ebx, dword ptr [esp + 0x4c]
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [ebx + 0x14]
        call   sub_1_12938
        lea    esi, [esp + 0x20]
        add    esp, 8
        lea    edi, [ebx + 0x1c]
        mov    ecx, 0xc
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [esp + 0x1c], 0x2580
        jae    loc_1_102BE
        mov    dword ptr [esp + 0x1c], 0x2580
loc_1_102BE:
        and    dword ptr [esp + 0x20], 0xfffff51d
        or     dword ptr [esp + 0x20], 0x119
        mov    byte ptr [esp + 0x2e], 8
        mov    byte ptr [esp + 0x2f], 0
        mov    byte ptr [esp + 0x30], 0
        lea    eax, [esp + 0x18]
        push   0x73
        push   eax
        push   dword ptr [ebx + 0x14]
        call   sub_1_12930
        lea    eax, [esp + 0x1c]
        add    esp, 0xc
        push   eax
        push   dword ptr [ebx + 0xe4]
        push   dword ptr [ebx + 0xe0]
        push   dword ptr [ebx + 0xa8]
        push   dword ptr [ebx + 0xa4]
        push   dword ptr [ebx + 0x14]
        call   sub_1_12940
        add    esp, 0x18
        or     eax, eax
        push   ebx
        jne    loc_1_1032B
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 4
        jmp    loc_1_1037E
loc_1_1032B:
        push   0x11510                       ; -> loc_1_11510
        push   0x50
        push   dword ptr [ebx + 0x14]
        call   sub_1_12990
        add    esp, 0x10
        or     eax, eax
        push   ebx
        jne    loc_1_10351
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 4
        jmp    loc_1_1037E
loc_1_10351:
        call   sub_1_11E7C
        add    esp, 4
        or     eax, eax
        je     loc_1_10364
        mov    ebp, 4
        jmp    loc_1_1037E
loc_1_10364:
        push   0
        push   0
        push   0x15
        push   dword ptr [ebx + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
loc_1_1037E:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
sub_1_10388:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0x73
        lea    eax, [esi + 0x1c]
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_12930
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_103AB
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103AB:
        push   0
        push   0
        push   dword ptr [esi + 0x14]
        call   sub_1_12980
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_103C7
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103C7:
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 0x14]
        call   sub_1_12990
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_103E5
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103E5:
        push   0
        push   0
        push   0x10
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        sub    eax, eax
        pop    esi
        ret    
sub_1_103FA:
        ret    
        db  0C3h, 0C3h
sub_1_103FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x10], esi
        push   4
        call   sub_1_12998
        cmp    word ptr [0x13520], 0         ; -> obj1:0x13520
        je     loc_1_1042A
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        sub    eax, eax
        jmp    loc_1_10654
loc_1_1042A:
        inc    word ptr [0x13520]            ; -> obj1:0x13520
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0xe8], 0
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_10647
loc_1_10452:
        cmp    eax, 0xa
        jae    loc_1_10647
        cmp    dword ptr [eax*4 + 0x12f08], ebx ; -> obj1:0x12F08
        je     loc_1_10472
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_10452
        jmp    loc_1_10647
loc_1_10472:
        cmp    dword ptr [ebx + 0x50], 0
        je     loc_1_10647
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_10628
        or     esi, esi
        je     loc_1_10628
        cmp    dword ptr [ebx + 0xf0], 0
        jne    loc_1_104E5
        mov    eax, dword ptr [ebx + 0x80]
        cmp    eax, 7
        je     loc_1_10647
        cmp    eax, 8
        je     loc_1_10647
        cmp    eax, 9
        je     loc_1_10647
        mov    dword ptr [ebx + 0xf0], 1
        mov    dword ptr [ebx + 0x58], 0
        push   ebx
        call   sub_1_11852
        add    esp, 4
        or     eax, eax
        jne    loc_1_104E5
        mov    dword ptr [ebx + 0xf0], 0
        jmp    loc_1_10647
loc_1_104E5:
        cmp    dword ptr [ebx + 0xec], 0
        je     loc_1_10528
        mov    esi, 0x13524                  ; -> loc_1_13524
        lea    edi, [ebp - 4]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    al, byte ptr [ebx + 0xec]
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 0x18]
        push   eax
        push   1
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_11B90
        add    esp, 0x10
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_10602
        mov    dword ptr [ebx + 0xec], 0
loc_1_10528:
        sub    edi, edi
loc_1_1052A:
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_10996
        add    esp, 0x10
        mov    esi, eax
        cmp    esi, edi
        je     loc_1_10554
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0x10
loc_1_10554:
        lea    eax, [ebp - 0xc]
        push   eax
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_11B90
        add    esp, 0x10
        cmp    eax, edi
        je     loc_1_10611
        cmp    esi, edi
        je     loc_1_1057B
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx], al
loc_1_1057B:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_1_105B3
        cmp    esi, edi
        je     loc_1_105B3
        cmp    byte ptr [ebp - 1], 0x10
        je     loc_1_10591
        or     byte ptr [ebp - 1], 0x80
loc_1_10591:
        lea    eax, [ebp - 0x18]
        push   eax
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   sub_1_11B90
        add    esp, 0x10
        cmp    dword ptr [ebp - 0x18], edi
        jne    loc_1_105B3
        movsx  eax, byte ptr [ebp - 1]
        mov    dword ptr [ebx + 0xec], eax
loc_1_105B3:
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0xc], edi
        jne    loc_1_105D1
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
loc_1_105D1:
        mov    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ebx + 0xe8], eax
        cmp    esi, edi
        jne    loc_1_1052A
        mov    eax, dword ptr [ebp + 0x14]
        or     eax, eax
        je     loc_1_10647
        or     esi, esi
        jne    loc_1_10647
        cmp    dword ptr [ebx + 0xf0], 0
        je     loc_1_10647
        push   eax
        push   ebx
        call   sub_1_1065B
        add    esp, 8
        jmp    loc_1_10647
loc_1_10602:
        cmp    dword ptr [ebx + 0x1b6], 0
        je     loc_1_10528
        jmp    loc_1_1063D
loc_1_10611:
        or     esi, esi
        je     loc_1_1061D
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
loc_1_1061D:
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        jmp    loc_1_10647
loc_1_10628:
        cmp    dword ptr [ebx + 0xf0], 0
        je     loc_1_1063D
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_1_1065B
        add    esp, 8
loc_1_1063D:
        mov    dword ptr [ebx + 0xe8], 0
loc_1_10647:
        dec    word ptr [0x13520]            ; -> obj1:0x13520
        mov    eax, dword ptr [ebx + 0xe8]
loc_1_10654:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1065B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_10689
        mov    esi, dword ptr [esp + 0x18]
loc_1_10672:
        cmp    eax, 0xa
        jae    loc_1_10689
        cmp    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_10698
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_10672
loc_1_10689:
        mov    ebx, 5
loc_1_1068E:
        mov    eax, ebx
loc_1_10690:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_10698:
        mov    ebp, dword ptr [esp + 0x1c]
        or     ebp, ebp
        jne    loc_1_106A4
        sub    eax, eax
        jmp    loc_1_10690
loc_1_106A4:
        mov    eax, ebp
        imul   eax, eax, 9
        mov    ecx, 0xffffffff
        mov    edx, dword ptr [eax + 0x12eb5] ; -> loc_1_12EB5
        lea    eax, [esp + 0x10]
        push   eax
        mov    edi, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        push   esi
        call   sub_1_11B90
        add    esp, 0x10
        or     eax, eax
        je     loc_1_10740
        cmp    dword ptr [esp + 0x10], 2
        je     loc_1_106E1
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_106E1:
        cmp    ebp, 1
        je     loc_1_106FA
        cmp    ebp, 2
        jb     loc_1_1068E
        cmp    ebp, 3
        jbe    loc_1_10729
        cmp    ebp, 4
        jb     loc_1_1068E
        cmp    ebp, 5
        ja     loc_1_1068E
loc_1_106FA:
        mov    dword ptr [esi + 0xf0], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x80], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_10690
loc_1_10729:
        mov    dword ptr [esi + 0x58], 1
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        jmp    loc_1_1068E
loc_1_10740:
        mov    ebx, 4
        jmp    loc_1_1068E
sub_1_1074A:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x30]
        sub    ebp, ebp
        mov    eax, dword ptr [esi + 0xe4]
        mov    edi, dword ptr [esp + 0x34]
        cmp    eax, edi
        jb     loc_1_10769
        mov    eax, edi
loc_1_10769:
        imul   eax, eax, 5
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x1c], 0
loc_1_10778:
        push   0
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_1_11CB8
        lea    eax, [esp + 0x20]
        add    esp, 0xc
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     loc_1_107C8
        cmp    dword ptr [esp + 0x1c], edi
        jae    loc_1_107C1
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [esi + 0xe4], eax
        jbe    loc_1_107C1
        cmp    ebp, eax
        jne    loc_1_107BB
        cmp    ebx, 3
        ja     loc_1_107C8
        inc    ebx
loc_1_107BB:
        mov    ebp, dword ptr [esp + 0x1c]
        jmp    loc_1_10778
loc_1_107C1:
        mov    eax, 1
        jmp    loc_1_107CA
loc_1_107C8:
        sub    eax, eax
loc_1_107CA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_107D2:
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_107F7
        mov    ecx, dword ptr [esp + 4]
loc_1_107E0:
        cmp    eax, 0xa
        jae    loc_1_107F7
        cmp    dword ptr [eax*4 + 0x12f08], ecx ; -> obj1:0x12F08
        je     loc_1_107FD
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_107E0
loc_1_107F7:
        mov    eax, 5
loc_1_107FC:
        ret    
loc_1_107FD:
        mov    dword ptr [ecx + 0x80], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0xb4], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0xb8], eax
        sub    eax, eax
        jmp    loc_1_107FC
        db  056h, 02Bh, 0C0h, 039h, 005h
        dd  obj1:0x12F04             ; fixup
        db  076h, 01Bh, 08Bh, 074h, 024h, 008h, 083h, 0F8h, 00Ah, 073h, 012h, 039h, 034h, 085h
        dd  obj1:0x12F08             ; fixup
        db  074h, 010h, 040h, 03Bh, 005h
        dd  obj1:0x12F04             ; fixup
        db  072h, 0E9h, 0B8h, 005h, 000h, 000h, 000h, 05Eh, 0C3h, 068h
        dd  loc_1_13528              ; fixup
        db  06Ah, 00Eh, 056h, 0E8h, 0F2h, 010h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 012h, 056h
        db  0E8h, 007h, 012h, 000h, 000h, 083h, 0C4h, 004h, 083h, 0F8h, 002h, 075h, 004h, 02Bh, 0C0h, 0EBh
        db  0D8h, 0B8h, 002h, 000h, 000h, 000h, 0EBh, 0D1h
sub_1_10879:
        sub    esp, 0xc
        push   esi
        sub    eax, eax
        cmp    dword ptr [0x12f04], eax      ; -> obj1:0x12F04
        jbe    loc_1_108A2
        mov    esi, dword ptr [esp + 0x14]
loc_1_1088B:
        cmp    eax, 0xa
        jae    loc_1_108A2
        cmp    dword ptr [eax*4 + 0x12f08], esi ; -> obj1:0x12F08
        je     loc_1_108AC
        inc    eax
        cmp    eax, dword ptr [0x12f04]      ; -> obj1:0x12F04
        jb     loc_1_1088B
loc_1_108A2:
        mov    eax, 5
loc_1_108A7:
        pop    esi
        add    esp, 0xc
        ret    
loc_1_108AC:
        mov    ecx, dword ptr [esi + 0xf0]
        or     ecx, ecx
        jne    loc_1_108D2
        mov    eax, dword ptr [esi + 0x80]
        cmp    eax, 7
        je     loc_1_108CB
        cmp    eax, 8
        je     loc_1_108CB
        cmp    eax, 9
        jne    loc_1_108D2
loc_1_108CB:
        mov    eax, 2
        jmp    loc_1_108A7
loc_1_108D2:
        cmp    dword ptr [esi + 0x7c], 9
        jne    loc_1_108E3
        or     ecx, ecx
        je     loc_1_108E3
        mov    eax, 2
        jmp    loc_1_108A7
loc_1_108E3:
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        jne    loc_1_108FB
        sub    eax, eax
        jmp    loc_1_108A7
loc_1_108FB:
        cmp    dword ptr [esp + 8], 0
        je     loc_1_10918
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        jmp    loc_1_10948
loc_1_10918:
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_10935
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        jmp    loc_1_10948
loc_1_10935:
        cmp    dword ptr [esi + 0xf0], 0
        je     loc_1_10952
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], 3
loc_1_10948:
        mov    eax, 1
        jmp    loc_1_108A7
loc_1_10952:
        sub    eax, eax
        jmp    loc_1_108A7
        db  056h, 057h, 08Bh, 074h, 024h, 010h, 056h, 08Bh, 07Ch, 024h, 010h, 06Ah, 010h, 057h, 0E8h, 0C4h
        db  01Dh, 000h, 000h, 08Bh, 04Ch, 024h, 020h, 083h, 0C4h, 00Ch, 089h, 001h, 08Bh, 0C8h, 00Bh, 0C9h
        db  074h, 010h, 02Bh, 0CFh, 08Bh, 044h, 024h, 018h, 041h, 089h, 008h, 0B8h, 001h, 000h, 000h, 000h
        db  0EBh, 008h, 08Bh, 044h, 024h, 018h, 089h, 030h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h
sub_1_10996:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        push   edi
        mov    esi, dword ptr [esp + 0x18]
        push   0x10
        sub    ebx, ebx
        push   esi
        call   sub_1_12730
        add    esp, 0xc
        cmp    eax, ebx
        mov    ebp, dword ptr [esp + 0x1c]
        je     loc_1_109C5
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109C5:
        push   edi
        push   0x11
        push   esi
        call   sub_1_12730
        add    esp, 0xc
        or     eax, eax
        je     loc_1_109E2
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109E2:
        push   edi
        push   0x13
        push   esi
        call   sub_1_12730
        add    esp, 0xc
        or     eax, eax
        je     loc_1_109FF
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109FF:
        mov    eax, dword ptr [esp + 0x20]
        pop    ebp
        mov    dword ptr [eax], edi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_10A0C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    edi, [esi + 0x1b6]
        push   edi
        push   0
        push   dword ptr [esi + 0x14]
        call   sub_1_12950
        add    esp, 0xc
        cmp    dword ptr [edi], 0
        je     loc_1_10A32
        mov    dword ptr [esi + 0x6c], 4
loc_1_10A32:
        pop    edi
        pop    esi
        ret    
sub_1_10A35:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x19e]
        or     eax, eax
        je     loc_1_10A54
        push   eax
        call   sub_1_129E8
        mov    dword ptr [esi + 0x19e], 0
loc_1_10A54:
        pop    esi
        ret    
sub_1_10A56:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0x19e], 0
        lea    esi, [eax + 0xf4]
        jne    loc_1_10A81
        push   eax
        push   dword ptr [esi + 0xb2]
        push   0x10ab0                       ; -> loc_1_10AB0
        call   sub_1_129CC
        mov    dword ptr [esi + 0xaa], eax
loc_1_10A81:
        pop    esi
        ret    
sub_1_10A83:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0x19e], 0
        lea    esi, [eax + 0xf4]
        jne    loc_1_10AAE
        push   eax
        push   dword ptr [esi + 0xb2]
        push   0x10ab0                       ; -> loc_1_10AB0
        call   sub_1_129CC
        mov    dword ptr [esi + 0xaa], eax
loc_1_10AAE:
        pop    esi
        ret    
loc_1_10AB0:
        sub    esp, 0x10
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x19e], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0xc], 0
        jne    loc_1_10B1A
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     loc_1_10B0F
        cmp    dword ptr [esp + 8], 0
        je     loc_1_10B1A
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 8], 0
        mov    edx, dword ptr [esp]
        call   sub_1_10D56
        jmp    loc_1_10B26
loc_1_10B0F:
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x6c], 1
loc_1_10B1A:
        push   dword ptr [esp]
        call   sub_1_10A56
        add    esp, 4
loc_1_10B26:
        add    esp, 0x10
        ret    
        db  08Bh, 044h, 024h, 004h, 08Bh, 080h, 088h, 000h, 000h, 000h, 048h, 083h, 0F8h, 001h, 01Bh, 0C0h
        db  0F7h, 0D8h, 0C3h
loc_1_10B3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1a2], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_10B65
        push   eax
        call   sub_1_10BC5
        add    esp, 4
loc_1_10B65:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x88], 1
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x8c]
        or     eax, eax
        je     loc_1_10B8B
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
loc_1_10B8B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xfa], 0x13
        push   dword ptr [ebp - 4]
        call   sub_1_124D7
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_10BA4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x1a2]
        or     eax, eax
        je     loc_1_10BC3
        push   eax
        call   sub_1_129E8
        mov    dword ptr [esi + 0x1a2], 0
loc_1_10BC3:
        pop    esi
        ret    
sub_1_10BC5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x1a2], 0
        lea    edi, [esi + 0xf4]
        je     loc_1_10BE3
        push   esi
        call   sub_1_10BA4
        add    esp, 4
loc_1_10BE3:
        mov    dword ptr [esi + 0x88], 0
        push   esi
        push   dword ptr [edi + 0xb6]
        push   0x10b3d                       ; -> loc_1_10B3D
        call   sub_1_129CC
        mov    dword ptr [edi + 0xae], eax
        pop    edi
        pop    esi
        ret    
sub_1_10C07:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x24]
        sub    ebp, ebp
        mov    dword ptr [ebx + 0xc4], ebp
        mov    dword ptr [ebx + 0xbc], ebp
        mov    eax, dword ptr [ebx + 0xb4]
        mov    dword ptr [ebx + 0xf4], eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [ebx + 0x14]
        call   sub_1_12948
        add    esp, 8
        cmp    eax, ebp
        jne    loc_1_10C53
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 3
        jmp    loc_1_10D3E
loc_1_10C53:
        mov    edx, dword ptr [ebx + 0xc4]
        mov    ecx, dword ptr [ebx + 0xb8]
        sub    ecx, edx
        mov    eax, dword ptr [esp + 0x18]
        cmp    ecx, eax
        jb     loc_1_10C6B
        mov    ecx, eax
loc_1_10C6B:
        lea    eax, [esp + 0x10]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebx + 0xb4]
        add    eax, edx
        push   eax
        push   ebx
        call   sub_1_11AB0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_10C9B
        mov    ebp, 3
        cmp    dword ptr [ebx + 0xc4], 0
        je     loc_1_10D3E
        jmp    loc_1_10CA5
loc_1_10C9B:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [ebx + 0xc4], eax
loc_1_10CA5:
        mov    edi, 0x1352c                  ; -> loc_1_1352C
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 0xb4]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_10CBD
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
loc_1_10CBD:
        je     loc_1_10D13
        mov    edi, 0x13538                  ; -> loc_1_13538
        mov    ecx, 3
        mov    esi, dword ptr [ebx + 0xb4]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_10CD9
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_10CD9:
        je     loc_1_10D13
        mov    edi, 0x13548                  ; -> obj1:0x13548
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 0xb4]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_10CFD
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    loc_1_10CFD
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
loc_1_10CFD:
        je     loc_1_10D13
        mov    edi, 0x13554                  ; -> obj1:0x13554
        mov    ecx, 7
        mov    esi, dword ptr [ebx + 0xb4]
        sub    eax, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_10D13:
        mov    eax, dword ptr [ebx + 0xc4]
        or     eax, eax
        je     loc_1_10D34
        mov    ecx, dword ptr [ebx + 0xb4]
        add    eax, ecx
        dec    eax
        mov    dword ptr [ebx + 0xbc], eax
        mov    dword ptr [ebx + 0xf4], ecx
        jmp    loc_1_10D3E
loc_1_10D34:
        mov    dword ptr [ebx + 0xbc], 0
loc_1_10D3E:
        or     ebp, ebp
        je     loc_1_10D4E
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        mov    dword ptr [ebx + 0x6c], ebp
loc_1_10D4E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_10D56:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], edx
        mov    eax, dword ptr [esp + 0x10]
        inc    dword ptr [eax + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, 0xc
        mov    eax, dword ptr [ecx]
        cmp    eax, 1
        jg     loc_1_10FD9
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1
        je     loc_1_10D9D
        cmp    eax, 2
        je     loc_1_10F29
        cmp    eax, 4
        je     loc_1_10F70
        jmp    loc_1_10F7C
loc_1_10D9D:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x54], 0
        je     loc_1_10F0B
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     loc_1_10F0B
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_10F17
        push   dword ptr [esp + 0x10]
        call   sub_1_11E18
        add    esp, 4
        push   dword ptr [esp + 0x10]
        call   sub_1_10C07
        add    esp, 4
        mov    ebx, 0x32
        push   dword ptr [esp + 0x10]
        call   sub_1_122AE
        add    esp, 4
        or     eax, eax
        jne    loc_1_10F17
        sub    ebp, ebp
loc_1_10E05:
        dec    ebx
        je     loc_1_10F17
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x78]
        cmp    eax, 1
        je     loc_1_10ECD
        cmp    eax, 2
        je     loc_1_10E34
        cmp    eax, 3
        je     loc_1_10E56
        cmp    eax, 4
        je     loc_1_10EFB
        jmp    loc_1_10EE5
loc_1_10E34:
        push   dword ptr [esp + 0x10]
        call   sub_1_1206F
        add    esp, 4
        cmp    eax, ebp
        je     loc_1_10EE5
        push   dword ptr [esp + 0x10]
        call   sub_1_123A0
        jmp    loc_1_10EE2
loc_1_10E56:
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, 0x1355c                  ; -> obj1:0x1355C
        mov    ecx, 2
        mov    esi, dword ptr [eax + 0xb4]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_10E72
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
loc_1_10E72:
        je     loc_1_10ECD
        push   dword ptr [esp + 0x10]
        call   sub_1_11076
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        mov    ecx, dword ptr [eax + 0x1b2]
        cmp    ecx, ebp
        je     loc_1_10EBD
        cmp    dword ptr [eax + 0xbc], ebp
        je     loc_1_10EE5
        mov    edx, dword ptr [eax + 0xf4]
        mov    eax, dword ptr [eax + 0xb4]
        cmp    edx, eax
        jbe    loc_1_10EE5
        sub    edx, eax
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0x5c]
        push   edx
        push   1
        push   dword ptr [esp + 0x1c]
        call   ecx
        add    esp, 0x10
        jmp    loc_1_10EE5
loc_1_10EBD:
        push   1
        push   dword ptr [esp + 0x14]
        call   sub_1_11032
        add    esp, 8
        jmp    loc_1_10EE5
loc_1_10ECD:
        push   dword ptr [esp + 0x10]
        call   sub_1_1206F
        add    esp, 4
        push   dword ptr [esp + 0x10]
        call   sub_1_124D7
loc_1_10EE2:
        add    esp, 4
loc_1_10EE5:
        push   dword ptr [esp + 0x10]
        call   sub_1_122AE
        add    esp, 4
        cmp    eax, ebp
        je     loc_1_10E05
        jmp    loc_1_10F17
loc_1_10EFB:
        push   3
        push   dword ptr [esp + 0x14]
        call   sub_1_11032
        add    esp, 8
        jmp    loc_1_10F17
loc_1_10F0B:
        push   dword ptr [esp + 0x10]
        call   sub_1_10A0C
        add    esp, 4
loc_1_10F17:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x7c], 9
        je     loc_1_10F7C
        push   eax
        call   sub_1_10A56
        jmp    loc_1_10F79
loc_1_10F29:
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        mov    eax, dword ptr [esp + 0x10]
        je     loc_1_10F67
        mov    eax, dword ptr [eax + 0x1b2]
        or     eax, eax
        je     loc_1_10F7C
        mov    ecx, dword ptr [esp + 0x10]
        push   dword ptr [ecx + 0x5c]
        push   dword ptr [esp + 0x20]
        push   2
        push   dword ptr [esp + 0x1c]
        call   eax
        add    esp, 0x10
        jmp    loc_1_10F7C
loc_1_10F67:
        mov    dword ptr [eax + 0x6c], 1
        jmp    loc_1_10F7C
loc_1_10F70:
        push   dword ptr [esp + 0x10]
        call   sub_1_10FF0
loc_1_10F79:
        add    esp, 4
loc_1_10F7C:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x54], 0
        je     loc_1_10FC4
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     loc_1_10FC4
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_10FD0
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [eax + 0xc]
        jmp    loc_1_10FDC
loc_1_10FC4:
        push   dword ptr [esp + 0x10]
        call   sub_1_10A0C
        add    esp, 4
loc_1_10FD0:
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [eax + 0xc]
        jmp    loc_1_10FE8
loc_1_10FD9:
        dec    eax
        mov    dword ptr [ecx], eax
loc_1_10FDC:
        push   dword ptr [esp + 0x10]
        call   sub_1_11E1A
        add    esp, 4
loc_1_10FE8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_10FF0:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 8]
        test   edi, 1
        je     loc_1_1101B
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 8], 0
        push   esi
        call   sub_1_10A56
        add    esp, 4
loc_1_1101B:
        lea    eax, [esp + 8]
        push   eax
        push   edi
        push   dword ptr [esi + 0x14]
        call   sub_1_12968
        add    esp, 0xc
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_1_11032:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    edi, 1
        je     loc_1_1104C
        cmp    edi, 2
        je     loc_1_11061
        mov    esi, dword ptr [esp + 0xc]
loc_1_11046:
        mov    dword ptr [esi + 0x78], edi
        pop    edi
        pop    esi
        ret    
loc_1_1104C:
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x78], 3
        jne    loc_1_11046
        push   esi
        call   sub_1_103FA
        add    esp, 4
        jmp    loc_1_11046
loc_1_11061:
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x78], 3
        jne    loc_1_11046
        push   esi
        call   sub_1_103FA
        add    esp, 4
        jmp    loc_1_11046
sub_1_11076:
        sub    esp, 0x1c
        push   esi
        push   edi
        mov    dword ptr [esp + 0x20], 0
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [esi + 0xf4]
        add    esi, 0xf4
        mov    edi, dword ptr [esp + 0x28]
        mov    eax, dword ptr [edi + 0xb4]
        add    edi, 0xb4
        cmp    ecx, eax
        jbe    loc_1_110B7
        sub    ecx, eax
        push   ecx
        push   eax
        push   dword ptr [esp + 0x30]
        call   sub_1_11470
        add    esp, 0xc
loc_1_110B7:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax + 0xbc]
        inc    eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0x28]
        cmp    dword ptr [eax + 0xc8], 0
        je     loc_1_1113B
        mov    eax, dword ptr [esp + 0xc]
        jmp    loc_1_11179
loc_1_110E0:
        mov    dword ptr [esp + 0x14], 1
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [esp + 0x2c]
        call   dword ptr [eax + 1]
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 8
        cmp    byte ptr [eax], 0x10
        je     loc_1_1111E
        mov    dword ptr [esp + 0x20], 0
        mov    ecx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0xf4], eax
        jmp    loc_1_1122E
loc_1_1111E:
        inc    dword ptr [esp + 0xc]
loc_1_11122:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1112D
        inc    dword ptr [esp + 0xc]
loc_1_1112D:
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax + 0xc8], 0
loc_1_1113B:
        mov    eax, dword ptr [esp + 0x28]
        add    eax, 0xbc
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esp + 0xc]
        inc    eax
        push   eax
        push   0x10
        push   dword ptr [esp + 0x14]
        call   sub_1_12730
        mov    dword ptr [esp + 0x18], eax
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1122E
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_111C7
        mov    eax, ecx
        inc    eax
loc_1_11179:
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 8], 0x12eb0  ; -> loc_1_12EB0
        cmp    byte ptr [0x12eb0], 0         ; -> loc_1_12EB0
        je     loc_1_11122
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, 9
        mov    al, byte ptr [eax]
        mov    byte ptr [esp + 0x10], al
loc_1_111A5:
        mov    eax, dword ptr [esp + 8]
        mov    dl, byte ptr [esp + 0x10]
        cmp    byte ptr [eax], dl
        je     loc_1_110E0
        add    dword ptr [esp + 8], ecx
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax], 0
        jne    loc_1_111A5
        jmp    loc_1_11122
loc_1_111C7:
        mov    edx, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [edx + 0xbc]
        add    edx, 0xbc
        cmp    dword ptr [esp + 0xc], ecx
        jne    loc_1_11223
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x20], 1
        mov    eax, dword ptr [eax + 0xb4]
        cmp    dword ptr [esp + 0xc], eax
        jne    loc_1_11213
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [ecx + 0xf4], eax
        mov    eax, ecx
        mov    dword ptr [edx], 0
        mov    dword ptr [eax + 0xc4], 0
        jmp    loc_1_1122E
loc_1_11213:
        mov    eax, dword ptr [esp + 0x28]
        dec    ecx
        dec    dword ptr [eax + 0xc4]
        mov    dword ptr [edx], ecx
        inc    ecx
        jmp    loc_1_11228
loc_1_11223:
        inc    ecx
        mov    eax, dword ptr [esp + 0x28]
loc_1_11228:
        mov    dword ptr [eax + 0xf4], ecx
loc_1_1122E:
        mov    ecx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0xc8], eax
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
loc_1_11242:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0xc8], 0
        jne    loc_1_1125E
        push   1
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_1_11470
        add    esp, 0xc
loc_1_1125E:
        ret    
loc_1_1125F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xc8], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_11470
        add    esp, 0xc
        mov    dword ptr [esi + 0xf0], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x80], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_112BE
        push   dword ptr [esi + 0x5c]
        push   4
        push   5
        push   esi
        call   eax
        add    esp, 0x10
loc_1_112BE:
        pop    esi
        ret    
loc_1_112C0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0xc8], 0
        jne    loc_1_112E0
        mov    esi, dword ptr [esp + 0xc]
        push   1
        push   esi
        push   eax
        call   sub_1_11470
        add    esp, 0xc
        jmp    loc_1_112E4
loc_1_112E0:
        mov    esi, dword ptr [esp + 0xc]
loc_1_112E4:
        and    byte ptr [esi], 0x11
        pop    esi
        ret    
loc_1_112E9:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0xc8], 0
        jne    loc_1_11309
        mov    esi, dword ptr [esp + 0xc]
        push   1
        push   esi
        push   eax
        call   sub_1_11470
        add    esp, 0xc
        jmp    loc_1_1130D
loc_1_11309:
        mov    esi, dword ptr [esp + 0xc]
loc_1_1130D:
        and    byte ptr [esi], 0x13
        pop    esi
        ret    
loc_1_11312:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xc8], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_11470
        add    esp, 0xc
        mov    dword ptr [esi + 0x58], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_1135B
        push   dword ptr [esi + 0x5c]
        push   2
        push   5
        push   esi
        call   eax
        add    esp, 0x10
loc_1_1135B:
        pop    esi
        ret    
loc_1_1135D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xc8], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_11470
        add    esp, 0xc
        mov    dword ptr [esi + 0x58], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_113A6
        push   dword ptr [esi + 0x5c]
        push   3
        push   5
        push   esi
        call   eax
        add    esp, 0x10
loc_1_113A6:
        pop    esi
        ret    
loc_1_113A8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0xc8], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_11470
        add    esp, 0xc
        mov    dword ptr [esi + 0xf0], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x80], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_11407
        push   dword ptr [esi + 0x5c]
        push   4
        push   5
        push   esi
        call   eax
        add    esp, 0x10
loc_1_11407:
        pop    esi
        ret    
loc_1_11409:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xc8]
        or     eax, eax
        je     loc_1_1142F
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_11470
        add    esp, 0xc
loc_1_1142F:
        mov    dword ptr [esi + 0xf0], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x80], 8
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_1146E
        push   dword ptr [esi + 0x5c]
        push   5
        push   5
        push   esi
        call   eax
        add    esp, 0x10
loc_1_1146E:
        pop    esi
        ret    
sub_1_11470:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x10]
        lea    ecx, [ebx + edi]
        mov    eax, dword ptr [esi + 0xbc]
        cmp    eax, ecx
        jae    loc_1_114CD
        mov    eax, dword ptr [esi + 0xb4]
        cmp    edi, eax
        je     loc_1_114B1
        dec    edi
        mov    dword ptr [esi + 0xbc], edi
        mov    ecx, edi
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [esi + 0xc4], ecx
        inc    edi
        mov    dword ptr [esi + 0xf4], edi
        jmp    loc_1_11509
loc_1_114B1:
        mov    dword ptr [esi + 0xf4], eax
        mov    dword ptr [esi + 0xbc], 0
        mov    dword ptr [esi + 0xc4], 0
        jmp    loc_1_11509
loc_1_114CD:
        sub    eax, ecx
        inc    eax
        push   eax
        push   ecx
        push   edi
        call   sub_1_12832
        add    esp, 0xc
        sub    dword ptr [esi + 0xbc], ebx
        sub    dword ptr [esi + 0xc4], ebx
        mov    ecx, dword ptr [esi + 0xf4]
        cmp    edi, ecx
        ja     loc_1_11509
        sub    ecx, ebx
        mov    dword ptr [esi + 0xf4], ecx
        mov    eax, dword ptr [esi + 0xb4]
        cmp    ecx, eax
        ja     loc_1_11509
        mov    dword ptr [esi + 0xf4], eax
loc_1_11509:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
loc_1_11510:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        inc    dword ptr [edx + 0xc]
        cmp    dword ptr [edx + 0xc], 1
        jg     loc_1_1154B
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 4], ebx
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 8], ebx
        mov    dword ptr [edx + 0x10], 1
        mov    ebx, dword ptr [edx]
        mov    esi, 0x10d56                  ; -> sub_1_10D56
        mov    ecx, 0xb
        sub    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_1154B:
        mov    edx, dword ptr [ebp + 0xc]
        dec    dword ptr [edx + 0xc]
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_11555:
        pushal 
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
loc_1_1155E:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, ecx
        cmp    eax, 0xc8
        jb     loc_1_1155E
        popal  
        ret    
        db  000h
loc_1_11570:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        je     loc_1_11593
        cmp    eax, 9
        je     loc_1_115D4
        cmp    eax, 0xa
        je     loc_1_115EC
        jmp    loc_1_11615
loc_1_11593:
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0x1161e  ; -> loc_1_1161E
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [ebp - 0x10], 0xb
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x1c], esi
        jmp    loc_1_11615
loc_1_115D4:
        mov    dword ptr [esi + 0x50], 0
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_11615
        push   dword ptr [esi + 0x5c]
        push   0
        jmp    loc_1_1160D
loc_1_115EC:
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x50], 1
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_11615
        push   dword ptr [esi + 0x5c]
        push   1
loc_1_1160D:
        push   4
        push   esi
        call   eax
        add    esp, 0x10
loc_1_11615:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1161E:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0xfa], 6
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x80], 1
        push   dword ptr [esp]
        call   sub_1_124D7
        add    esp, 8
        ret    
        db  000h, 000h
sub_1_11650:
        push   esi
        push   edi
        push   0
        push   0
        mov    esi, dword ptr [esp + 0x14]
        push   0xeffffff5
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        cmp    eax, 1
        jne    loc_1_1169B
        mov    dword ptr [esi + 0x54], 1
        push   esi
        call   sub_1_10A56
        add    esp, 4
        push   esi
        call   sub_1_10279
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_11697
        push   esi
        call   sub_1_11E18
        add    esp, 4
loc_1_11697:
        mov    eax, edi
        jmp    loc_1_116A0
loc_1_1169B:
        mov    eax, 2
loc_1_116A0:
        pop    edi
        pop    esi
        ret    
sub_1_116A3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x54], 0
        jne    loc_1_116B0
        jmp    loc_1_11701
loc_1_116B0:
        push   esi
        call   sub_1_10388
        add    esp, 4
        push   esi
        call   sub_1_11E19
        add    esp, 4
        push   esi
        call   sub_1_10A35
        add    esp, 4
        push   0
        push   0
        push   0x10
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        push   0
        push   0
        push   0xeffffff6
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        cmp    eax, 1
        mov    eax, 2
        jne    loc_1_11703
        mov    dword ptr [esi + 0x54], 0
loc_1_11701:
        sub    eax, eax
loc_1_11703:
        pop    esi
        ret    
        db  081h, 0ECh, 090h, 000h, 000h, 000h, 056h, 057h, 0BEh
        dd  loc_1_13568              ; fixup
        db  08Dh, 07Ch, 024h, 00Ch, 066h, 0A5h, 0A4h, 08Bh, 03Dh
        dd  obj1:0x12D8C             ; fixup
        db  0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh
        db  0D1h, 08Dh, 07Ch, 024h, 00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh
        db  0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 0BEh
        dd  loc_1_1356C              ; fixup
        db  08Dh, 07Ch, 024h, 00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 066h, 0A5h
        db  0BFh
        dd  loc_1_12BE0              ; fixup
        db  0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh
        db  0D1h, 08Dh, 07Ch, 024h, 00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh
        db  0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Dh, 07Ch, 024h
        db  00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Dh, 044h, 024h, 008h
        db  08Dh, 079h, 0FFh, 050h, 08Dh, 044h, 024h, 010h, 057h, 08Bh, 0B4h, 024h, 0A4h, 000h, 000h, 000h
        db  050h, 0FFh, 076h, 014h, 0E8h, 0B1h, 011h, 000h, 000h, 083h, 0C4h, 010h, 03Bh, 07Ch, 024h, 008h
        db  076h, 00Bh, 0C7h, 046h, 06Ch, 002h, 000h, 000h, 000h, 02Bh, 0C0h, 0EBh, 031h, 0C7h, 046h, 070h
        db  01Bh, 000h, 000h, 000h, 0C7h, 046h, 07Ch, 001h, 000h, 000h, 000h, 06Ah, 002h, 056h, 0E8h, 049h
        db  0F8h, 0FFh, 0FFh, 083h, 0C4h, 008h, 056h, 0E8h, 07Bh, 002h, 000h, 000h, 083h, 0C4h, 004h, 083h
        db  0F8h, 002h, 0B8h, 000h, 000h, 000h, 000h, 075h, 005h, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh
        db  081h, 0C4h, 090h, 000h, 000h, 000h, 0C3h
sub_1_1180D:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        push   0xeffffff3
        push   dword ptr [eax + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        dec    eax
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 2
        ret    
sub_1_1182F:
        push   0
        push   0
        mov    eax, dword ptr [esp + 0xc]
        push   0xeffffff4
        push   dword ptr [eax + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        dec    eax
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 2
        ret    
sub_1_11852:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_11650
        add    esp, 4
        or     eax, eax
        je     loc_1_11866
        jmp    loc_1_118AD
loc_1_11866:
        push   0
        push   4
        push   esi
        call   sub_1_1194B
        add    esp, 0xc
        or     eax, eax
        je     loc_1_11892
        mov    dword ptr [esi + 0x7c], 3
        push   esi
        call   sub_1_11A6D
        add    esp, 4
        cmp    dword ptr [esi + 0x7c], 4
        mov    eax, 1
        je     loc_1_118AF
loc_1_11892:
        push   esi
        call   sub_1_116A3
        add    esp, 4
        mov    dword ptr [esi + 0x7c], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
loc_1_118AD:
        sub    eax, eax
loc_1_118AF:
        pop    esi
        ret    
        db  056h, 06Ah, 000h, 08Bh, 074h, 024h, 00Ch, 06Ah, 006h, 056h, 0E8h, 08Bh, 000h, 000h, 000h, 083h
        db  0C4h, 00Ch, 00Bh, 0C0h, 074h, 009h, 056h, 0E8h, 0A0h, 001h, 000h, 000h, 083h, 0C4h, 004h, 02Bh
        db  0C0h, 05Eh, 0C3h
sub_1_118D4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_11650
        add    esp, 4
        or     eax, eax
        je     loc_1_118E8
        jmp    loc_1_1192B
loc_1_118E8:
        push   0
        push   5
        push   esi
        call   sub_1_1194B
        add    esp, 0xc
        or     eax, eax
        je     loc_1_11922
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 0x7c], 0
        mov    dword ptr [esi + 0x58], 1
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_1192D
loc_1_11922:
        push   esi
        call   sub_1_116A3
        add    esp, 4
loc_1_1192B:
        sub    eax, eax
loc_1_1192D:
        pop    esi
        ret    
        db  06Ah, 000h, 06Ah, 012h, 0FFh, 074h, 024h, 00Ch, 0E8h, 00Fh, 000h, 000h, 000h, 083h, 0C4h, 00Ch
        db  00Bh, 0C0h, 0B8h, 001h, 000h, 000h, 000h, 075h, 002h, 02Bh, 0C0h, 0C3h
sub_1_1194B:
        sub    esp, 0x90
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x13570                  ; -> loc_1_13570
        lea    edi, [esp + 0x10]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    ebx, dword ptr [esp + 0xa4]
        mov    edi, dword ptr [ebx*4 + 0x12d20] ; -> obj1:0x12D20
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0xa8], 0
        je     loc_1_119D9
        mov    edi, dword ptr [esp + 0xa8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_119D9:
        mov    edi, 0x12be0                  ; -> loc_1_12BE0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    edi, [esp + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [esp + 0xc]
        lea    esi, [ecx - 1]
        push   eax
        lea    eax, [esp + 0x14]
        push   esi
        mov    edi, dword ptr [esp + 0xa8]
        push   eax
        push   edi
        call   sub_1_11B90
        add    esp, 0x10
        cmp    esi, dword ptr [esp + 0xc]
        jbe    loc_1_11A49
        mov    dword ptr [edi + 0x6c], 2
        sub    eax, eax
        jmp    loc_1_11A63
loc_1_11A49:
        mov    dword ptr [edi + 0x70], ebx
        mov    dword ptr [edi + 0x7c], 1
        push   2
        push   edi
        call   sub_1_11032
        add    esp, 8
        mov    eax, 1
loc_1_11A63:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x90
        ret    
sub_1_11A6D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_10A83
        add    esp, 4
        push   0
        push   dword ptr [esi + 0x1aa]
        lea    eax, [esi + 0x192]
        push   eax
        call   sub_1_11CB8
        add    esp, 0xc
        or     eax, eax
        je     loc_1_11AA4
        mov    dword ptr [esi + 0x6c], 2
        mov    dword ptr [esi + 0x7c], 6
loc_1_11AA4:
        mov    eax, dword ptr [esi + 0x7c]
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_11AB0:
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [esp + 0xc], 0
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        mov    esi, dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x24]
        push   esi
        call   sub_1_11AF0
        add    esp, 0x10
        mov    edi, eax
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_12948
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
sub_1_11AF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x24]
        mov    ebx, 0xa
        mov    dword ptr [eax], 0
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x20]
loc_1_11B12:
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     loc_1_11B86
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        push   edi
        push   dword ptr [esi + 0x14]
        call   sub_1_12978
        add    esp, 0x10
        or     eax, eax
        je     loc_1_11B75
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_11B50
        push   esi
        call   sub_1_10A0C
        add    esp, 4
        mov    eax, dword ptr [esi + 0x1b6]
        or     eax, eax
        je     loc_1_11B61
        cmp    eax, -1
        je     loc_1_11B86
        jmp    loc_1_11B12
loc_1_11B50:
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        mov    eax, 1
        jmp    loc_1_11B88
loc_1_11B61:
        push   1
        push   dword ptr [esi + 0x14]
        call   sub_1_12960
        add    esp, 8
        mov    eax, 1
        jmp    loc_1_11B88
loc_1_11B75:
        push   dword ptr [esi + 0x14]
        call   sub_1_12988
        add    esp, 4
        mov    dword ptr [esi + 0x1b6], eax
loc_1_11B86:
        sub    eax, eax
loc_1_11B88:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_11B90:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_1_11BA9
        add    esp, 0x10
        ret    
sub_1_11BA9:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x30]
        lea    eax, [esp + 0x18]
        mov    edi, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x14], 0xa
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 0x28]
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     loc_1_11C40
        test   byte ptr [esp + 0x18], 3
        jne    loc_1_11C40
        mov    ebp, dword ptr [esp + 0x2c]
loc_1_11BE9:
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi + 0x14]
        call   sub_1_12970
        add    esp, 0x10
        or     eax, eax
        je     loc_1_11C40
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi], eax
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_11C1D
        push   esi
        call   sub_1_10A0C
        dec    dword ptr [esp + 0x18]
        add    esp, 4
        jmp    loc_1_11C39
loc_1_11C1D:
        cmp    dword ptr [esp + 0x30], eax
        jbe    loc_1_11C4A
        sub    ebx, dword ptr [esp + 0x10]
        push   ebx
        push   esi
        call   sub_1_1074A
        add    esp, 8
        or     eax, eax
        je     loc_1_11C40
        add    ebp, dword ptr [esp + 0x10]
loc_1_11C39:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_11BE9
loc_1_11C40:
        sub    eax, eax
loc_1_11C42:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_11C4A:
        mov    eax, 1
        jmp    loc_1_11C42
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0
        call   sub_1_129B0
        mov    esi, dword ptr [ebp + 8]
        or     eax, eax
        mov    dword ptr [esi], eax
        je     loc_1_11CB1
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], eax
        mov    ecx, dword ptr [ebp + 0x10]
        or     ecx, ecx
        jne    loc_1_11C89
        mov    ecx, 0x11d98                  ; -> loc_1_11D98
loc_1_11C89:
        push   esi
        push   eax
        push   ecx
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        je     loc_1_11C9F
        mov    eax, 1
        jmp    loc_1_11CB3
loc_1_11C9F:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        sub    eax, eax
        jmp    loc_1_11CB3
loc_1_11CB1:
        sub    eax, eax
loc_1_11CB3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_11CB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        je     loc_1_11D6E
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    loc_1_11D6E
        cmp    dword ptr [esi + 4], 0
        jne    loc_1_11D6E
        mov    dword ptr [esi + 8], edi
        push   0
        call   sub_1_129B0
        mov    dword ptr [esi], eax
        or     eax, eax
        je     loc_1_11D1A
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        jne    loc_1_11CFD
        mov    eax, 0x11d98                  ; -> loc_1_11D98
loc_1_11CFD:
        push   esi
        push   edi
        push   eax
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    loc_1_11D1A
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_1_11D1A:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_11D6E
        cmp    dword ptr [esi + 4], 0
        je     loc_1_11D6E
        push   4
        push   eax
        call   sub_1_129C0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_11D5A
        push   eax
        call   sub_1_129E8
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], 0
        sub    eax, eax
        jmp    loc_1_11D73
loc_1_11D5A:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        mov    dword ptr [esi], 0
loc_1_11D6E:
        mov    eax, 1
loc_1_11D73:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_11D79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_11D94
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
loc_1_11D94:
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_11D98:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     loc_1_11DBD
        mov    dword ptr [eax], 0
        push   dword ptr [esp]
        call   sub_1_11D79
        add    esp, 4
loc_1_11DBD:
        add    esp, 4
        ret    
        db  083h, 0ECh, 004h, 089h, 054h, 024h, 000h, 08Bh, 044h, 024h, 000h, 0C7h, 080h, 0D4h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 074h, 024h, 000h, 0E8h, 0C4h, 0F8h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 08Bh, 044h, 024h, 000h, 0C7h, 080h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh
        db  044h, 024h, 000h, 0C7h, 040h, 07Ch, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 000h, 0C7h, 080h
        db  080h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 06Ah, 001h, 0FFh, 074h, 024h, 004h, 0E8h, 01Eh
        db  0F2h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0C3h
sub_1_11E18:
        ret    
sub_1_11E19:
        ret    
sub_1_11E1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 1
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0x10d56  ; -> sub_1_10D56
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x10], 0xb
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 0x1c], esi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_11E7C:
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   edi
        push   3
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        cmp    eax, edi
        jne    loc_1_11E9E
        mov    edi, 4
        jmp    loc_1_11EBF
loc_1_11E9E:
        push   0
        push   0
        push   5
        push   dword ptr [esi + 0x14]
        call   sub_1_12958
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_11EBA
        mov    edi, 4
        jmp    loc_1_11EBF
loc_1_11EBA:
        call   sub_1_11555
loc_1_11EBF:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  083h, 0ECh, 01Ch, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h, 02Bh
        db  0F6h, 08Bh, 06Ch, 024h, 030h, 02Bh, 0FFh, 08Bh, 09Dh, 0E4h, 000h, 000h, 000h, 08Bh, 044h, 024h
        db  034h, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 06Bh, 0DBh, 00Ah, 0C7h, 044h, 024h, 018h, 000h, 000h
        db  000h, 000h, 083h, 0FBh, 028h, 076h, 005h, 0BBh, 064h, 000h, 000h, 000h, 06Ah, 000h, 08Dh, 044h
        db  024h, 024h, 053h, 050h, 0E8h, 0ABh, 0FDh, 0FFh, 0FFh, 08Dh, 044h, 024h, 020h, 083h, 0C4h, 00Ch
        db  050h, 0FFh, 075h, 014h, 0E8h, 02Bh, 00Ah, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 02Eh
        db  08Bh, 044h, 024h, 034h, 039h, 044h, 024h, 018h, 073h, 01Ch, 08Bh, 044h, 024h, 018h, 039h, 085h
        db  0E4h, 000h, 000h, 000h, 076h, 010h, 03Bh, 0C7h, 075h, 006h, 083h, 0FEh, 00Ah, 077h, 00Fh, 046h
        db  08Bh, 07Ch, 024h, 018h, 0EBh, 0ACh, 0C7h, 044h, 024h, 010h, 001h, 000h, 000h, 000h, 08Bh, 044h
        db  024h, 010h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 01Ch, 0C3h, 000h, 000h
sub_1_11F60:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0xf4]
        mov    eax, dword ptr [ecx + 0xb4]
        mov    dword ptr [edx], eax
        mov    byte ptr [edx + 4], 0
        mov    byte ptr [edx + 5], 0
        lea    eax, [edx + 0xe]
        mov    dword ptr [edx + 6], 0
        mov    dword ptr [edx + 0xa], 0
        mov    dword ptr [edx + 0x9a], eax
        mov    dword ptr [edx + 0xaa], 0
        mov    dword ptr [edx + 0xae], 0
        mov    dword ptr [ecx + 0x1ae], 0
        mov    dword ptr [edx + 0xb2], 0x1e
        mov    dword ptr [edx + 0xb6], 0x1388
        ret    
        db  08Bh, 04Ch, 024h, 004h, 08Dh, 091h, 0F4h, 000h, 000h, 000h, 08Bh, 081h, 0B4h, 000h, 000h, 000h
        db  089h, 002h, 08Dh, 042h, 00Eh, 089h, 082h, 09Ah, 000h, 000h, 000h, 0C7h, 081h, 0AEh, 001h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C3h, 083h, 0ECh, 008h, 080h, 07Ch, 024h, 00Ch, 020h, 07Ch, 015h
        db  080h, 07Ch, 024h, 00Ch, 07Eh, 07Fh, 00Eh, 08Ah, 044h, 024h, 00Ch, 088h, 044h, 024h, 000h, 08Dh
        db  044h, 024h, 001h, 0EBh, 05Fh, 00Fh, 0BEh, 044h, 024h, 00Ch, 083h, 0F8h, 00Ah, 074h, 031h, 083h
        db  0F8h, 00Dh, 0C6h, 044h, 024h, 000h, 03Ch, 074h, 038h, 08Ah, 044h, 024h, 00Ch, 0C0h, 0E8h, 004h
        db  083h, 0E0h, 00Fh, 08Ah, 080h
        dd  loc_1_13574              ; fixup
        db  088h, 044h, 024h, 001h, 08Ah, 044h, 024h, 00Ch, 083h, 0E0h, 00Fh, 08Ah, 080h
        dd  loc_1_13574              ; fixup
        db  088h, 044h, 024h, 002h, 0EBh, 01Bh, 0C6h, 044h, 024h, 000h, 03Ch, 0C6h, 044h, 024h, 001h, 06Ch
        db  0C6h, 044h, 024h, 002h, 066h, 0EBh, 00Ah, 0C6h, 044h, 024h, 001h, 063h, 0C6h, 044h, 024h, 002h
        db  072h, 0C6h, 044h, 024h, 003h, 03Eh, 08Dh, 044h, 024h, 004h, 0C6h, 000h, 000h, 083h, 0C4h, 008h
        db  0C3h
sub_1_1206F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        lea    ebx, [esi + 0xf9]
        mov    dword ptr [esi + 0xfa], 0
        cmp    dword ptr [esi + 0x1ae], 0
        je     loc_1_120A2
        mov    eax, dword ptr [esi + 0xf4]
        cmp    byte ptr [eax], 0xa
        jne    loc_1_120A2
        inc    eax
        mov    dword ptr [esi + 0xf4], eax
loc_1_120A2:
        mov    dword ptr [esi + 0x1ae], 0
        cmp    dword ptr [esi + 0xfe], 1
        mov    edi, 1
        je     loc_1_120D3
        lea    edx, [esi + 0x102]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x23
        rep stosd dword ptr es:[edi], eax
        sub    edi, edi
        mov    dword ptr [esi + 0x18e], edx
loc_1_120D3:
        mov    dword ptr [esi + 0xfe], 0
        mov    ebp, 2
loc_1_120E2:
        push   esi
        call   sub_1_122AE
        add    esp, 4
        or     eax, eax
        jne    loc_1_12219
        mov    al, byte ptr [ebx]
        mov    ecx, dword ptr [esi + 0xf4]
        mov    byte ptr [esi + 0xf8], al
        mov    al, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [ebx], al
        mov    dword ptr [esi + 0xf4], ecx
        push   8
        movsx  eax, al
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        je     loc_1_121EA
        cmp    byte ptr [esi + 0xf8], 0xd
        jne    loc_1_12153
        cmp    byte ptr [ebx], 0xa
        jne    loc_1_12153
        cmp    dword ptr [esi + 0x78], ebp
        je     loc_1_120E2
        mov    dword ptr [esi + 0xfa], 0x12
        mov    dword ptr [esi + 0xfe], 3
        mov    eax, 1
        jmp    loc_1_12290
loc_1_12153:
        or     edi, edi
        je     loc_1_120E2
        mov    al, byte ptr [esi + 0x102]
        push   eax
        call   sub_1_122FA
        add    esp, 4
        or     eax, eax
        je     loc_1_121AB
        lea    eax, [esi + 0x102]
        push   eax
        call   sub_1_12325
        add    esp, 4
        mov    dword ptr [esi + 0xfa], eax
        or     eax, eax
        jne    loc_1_12229
        mov    dword ptr [esi + 0xfe], ebp
        mov    al, byte ptr [ebx]
        cmp    al, 0x20
        jne    loc_1_12273
        mov    ecx, dword ptr [esi + 0x18e]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    dword ptr [esi + 0x18e], ecx
        jmp    loc_1_120E2
loc_1_121AB:
        push   0x80
        movsx  eax, byte ptr [esi + 0x102]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        je     loc_1_1227A
        mov    al, byte ptr [ebx]
        cmp    al, 0xd
        je     loc_1_12243
        cmp    al, 0x20
        jne    loc_1_12273
        mov    ecx, dword ptr [esi + 0x18e]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    dword ptr [esi + 0x18e], ecx
        jmp    loc_1_120E2
loc_1_121EA:
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_1_122CF
        add    esp, 4
        or     eax, eax
        je     loc_1_120E2
        mov    edi, 1
        mov    ecx, dword ptr [esi + 0x18e]
        mov    al, byte ptr [ebx]
        mov    byte ptr [ecx], al
        add    ecx, edi
        mov    dword ptr [esi + 0x18e], ecx
        jmp    loc_1_120E2
loc_1_12219:
        or     edi, edi
        je     loc_1_1228E
        mov    dword ptr [esi + 0xfe], 1
        jmp    loc_1_1228E
loc_1_12229:
        mov    dword ptr [esi + 0xfe], 3
        mov    eax, dword ptr [esi + 0xf4]
        cmp    dword ptr [esi + 0xbc], eax
        jae    loc_1_12267
        jmp    loc_1_1225B
loc_1_12243:
        mov    dword ptr [esi + 0xfe], 4
        mov    eax, dword ptr [esi + 0xf4]
        cmp    dword ptr [esi + 0xbc], eax
        jae    loc_1_12267
loc_1_1225B:
        mov    dword ptr [esi + 0x1ae], 1
        jmp    loc_1_12273
loc_1_12267:
        cmp    byte ptr [eax], 0xa
        jne    loc_1_12273
        inc    eax
        mov    dword ptr [esi + 0xf4], eax
loc_1_12273:
        mov    eax, 1
        jmp    loc_1_12290
loc_1_1227A:
        mov    dword ptr [esi + 0xfa], 0
        mov    dword ptr [esi + 0xfe], 0
loc_1_1228E:
        sub    eax, eax
loc_1_12290:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 044h, 024h, 004h, 08Bh, 088h, 0C0h, 000h, 000h, 000h, 039h, 088h, 0F4h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h, 00Fh, 097h, 0C0h, 0C3h
sub_1_122AE:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xbc]
        or     ecx, ecx
        je     loc_1_122C9
        cmp    dword ptr [eax + 0xf4], ecx
        mov    eax, 0
        jbe    loc_1_122CE
loc_1_122C9:
        mov    eax, 1
loc_1_122CE:
        ret    
sub_1_122CF:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        push   0x107
        movsx  eax, bl
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        jne    loc_1_122F3
        cmp    bl, 0x2d
        mov    eax, 0
        jne    loc_1_122F8
loc_1_122F3:
        mov    eax, 1
loc_1_122F8:
        pop    ebx
        ret    
sub_1_122FA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        push   0x103
        movsx  eax, bl
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        jne    loc_1_1231E
        cmp    bl, 0x2d
        mov    eax, 0
        jne    loc_1_12323
loc_1_1231E:
        mov    eax, 1
loc_1_12323:
        pop    ebx
        ret    
sub_1_12325:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        push   0x103
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        cmp    eax, 1
        sbb    edx, edx
        and    edx, 0xfffffff8
        add    edx, 9
        cmp    edx, 0x12
        jg     loc_1_1238C
        lea    ebp, [edx*4 + 0x12e60]        ; -> obj1:0x12E60
loc_1_12355:
        mov    ebx, dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, ebx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_1237E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_1237E:
        je     loc_1_12393
        add    ebp, 4
        inc    edx
        cmp    ebp, 0x12ea8                  ; -> loc_1_12EA8
        jbe    loc_1_12355
loc_1_1238C:
        sub    eax, eax
loc_1_1238E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_12393:
        mov    eax, edx
        jmp    loc_1_1238E
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_123A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x7c]
        cmp    eax, 1
        je     loc_1_123C5
        cmp    eax, 3
        je     loc_1_12430
        cmp    eax, 7
        je     loc_1_12493
        jmp    loc_1_124D2
loc_1_123C5:
        mov    eax, dword ptr [esi + 0xfa]
        cmp    eax, 0x10
        jne    loc_1_12410
        mov    dword ptr [esi + 0x7c], 2
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0x8c]
        or     eax, eax
        je     loc_1_124C3
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        add    esi, 0x192
        push   esi
        call   sub_1_11D79
        add    esp, 4
        jmp    loc_1_124D2
loc_1_12410:
        cmp    eax, 0xc
        jne    loc_1_124D2
        mov    dword ptr [esi + 0x7c], 5
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        jmp    loc_1_124C3
loc_1_12430:
        mov    eax, dword ptr [esi + 0xfa]
        cmp    eax, 0xa
        jne    loc_1_12485
        mov    dword ptr [esi + 0x7c], 4
        mov    dword ptr [esi + 0xf0], 1
        mov    dword ptr [esi + 0x58], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0x8c]
        or     eax, eax
        je     loc_1_124C3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        add    esi, 0x192
        push   esi
        call   sub_1_11D79
        add    esp, 4
        jmp    loc_1_124D2
loc_1_12485:
        cmp    eax, 0xc
        jne    loc_1_124D2
        mov    dword ptr [esi + 0x7c], 5
        jmp    loc_1_124C3
loc_1_12493:
        mov    eax, dword ptr [esi + 0xfa]
        cmp    eax, 4
        je     loc_1_124AA
        cmp    eax, 0xc
        je     loc_1_124B3
        cmp    eax, 0x10
        je     loc_1_124BC
        jmp    loc_1_124D2
loc_1_124AA:
        mov    dword ptr [esi + 0x7c], 8
        jmp    loc_1_124D2
loc_1_124B3:
        mov    dword ptr [esi + 0x7c], 5
        jmp    loc_1_124C3
loc_1_124BC:
        mov    dword ptr [esi + 0x7c], 2
loc_1_124C3:
        add    esi, 0x192
        push   esi
        call   sub_1_11D79
        add    esp, 4
loc_1_124D2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_124D7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        cmp    eax, 8
        ja     loc_1_12709
        jmp    dword ptr [eax*4 + 0x1270b]   ; -> obj1:0x1270B
loc_1_124F3:
        mov    eax, dword ptr [esi + 0xfa]
        cmp    eax, 6
        je     loc_1_12508
        cmp    eax, 0xc
        je     loc_1_1252D
        jmp    loc_1_12709
loc_1_12508:
        push   esi
        call   sub_1_118D4
        add    esp, 4
        or     eax, eax
        je     loc_1_1251F
        mov    dword ptr [esi + 0x80], 3
loc_1_1251F:
        push   esi
        call   sub_1_10BC5
        add    esp, 4
        jmp    loc_1_12709
loc_1_1252D:
        mov    dword ptr [esi + 0xf0], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    loc_1_12709
loc_1_12550:
        mov    eax, dword ptr [esi + 0xfa]
        or     eax, eax
        je     loc_1_1260C
        cmp    eax, 0xa
        je     loc_1_1261A
        cmp    eax, 0x12
        je     loc_1_1260C
        cmp    dword ptr [esi + 0xc4], 7
        jne    loc_1_125DF
        mov    eax, dword ptr [esi + 0xb4]
        cmp    byte ptr [eax], 0x41
        jne    loc_1_125DF
        cmp    byte ptr [eax + 1], 0x54
        jne    loc_1_125DF
        cmp    byte ptr [eax + 2], 0x2d
        jne    loc_1_125DF
        cmp    byte ptr [eax + 3], 0x53
        jne    loc_1_125DF
        cmp    byte ptr [eax + 4], 0x41
        jne    loc_1_125DF
        cmp    byte ptr [eax + 5], 0x43
        jne    loc_1_125DF
        mov    dword ptr [esi + 0xc4], 0
        push   esi
        call   sub_1_11E19
        add    esp, 4
        push   esi
        call   sub_1_10A35
        add    esp, 4
        push   esi
        call   sub_1_10A83
        add    esp, 4
        push   esi
        call   sub_1_10BA4
        add    esp, 4
        mov    dword ptr [esi + 0x80], 3
        jmp    loc_1_12709
loc_1_125DF:
        mov    dword ptr [esi + 0xf0], 0
        mov    dword ptr [esi + 0x80], 1
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    loc_1_12709
loc_1_1260C:
        push   esi
        call   sub_1_11E19
        add    esp, 4
        jmp    loc_1_12709
loc_1_1261A:
        push   esi
        call   sub_1_10BA4
        add    esp, 4
        mov    dword ptr [esi + 0xf0], 1
        push   esi
        call   sub_1_11E19
        add    esp, 4
        mov    dword ptr [esi + 0x80], 4
        mov    dword ptr [esi + 0x58], 1
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        cmp    dword ptr [0x12f5c], 0        ; -> obj1:0x12F5C
        jne    loc_1_12664
        push   esi
        call   sub_1_10A35
        add    esp, 4
loc_1_12664:
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_12709
        push   dword ptr [esi + 0x5c]
        push   1
        push   6
        jmp    loc_1_12703
loc_1_1267E:
        cmp    dword ptr [esi + 0xfe], 4
        jne    loc_1_1269E
        lea    eax, [esi + 0x102]
        push   eax
        call   sub_1_12876
        add    esp, 4
        mov    dword ptr [esi + 0x84], eax
        jmp    loc_1_12709
loc_1_1269E:
        mov    dword ptr [esi + 0x80], 1
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    loc_1_12709
loc_1_126B3:
        cmp    dword ptr [esi + 0xfe], 4
        je     loc_1_12709
        mov    dword ptr [esi + 0x80], 1
        jmp    loc_1_12709
loc_1_126C8:
        mov    eax, dword ptr [esi + 0xfa]
        cmp    eax, 0x10
        je     loc_1_126D8
        cmp    eax, 0xc
        jne    loc_1_12709
loc_1_126D8:
        push   esi
        call   sub_1_116A3
        add    esp, 4
        mov    dword ptr [esi + 0x80], 1
        mov    dword ptr [esi + 0x7c], 9
        mov    eax, dword ptr [esi + 0x1b2]
        or     eax, eax
        je     loc_1_12709
        push   dword ptr [esi + 0x5c]
        push   1
        push   3
loc_1_12703:
        push   esi
        call   eax
        add    esp, 0x10
loc_1_12709:
        pop    esi
        ret    
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_124F3
        dd  loc_1_12709
        dd  loc_1_12550
        dd  loc_1_12709
        dd  loc_1_1267E
        dd  loc_1_126B3
        dd  loc_1_126C8
        dd  loc_1_126C8
        dd  loc_1_126C8
        db  000h
sub_1_12730:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        jne    loc_1_12740
        jmp    loc_1_12757
loc_1_12740:
        sub    esi, esi
        cmp    edx, esi
        je     loc_1_12757
        mov    ecx, dword ptr [esp + 0x10]
loc_1_1274A:
        movsx  edi, byte ptr [eax]
        cmp    edi, ecx
        je     loc_1_12759
        inc    esi
        inc    eax
        cmp    esi, edx
        jb     loc_1_1274A
loc_1_12757:
        sub    eax, eax
loc_1_12759:
        pop    edi
        pop    esi
        ret    
sub_1_1275C:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 4
        je     loc_1_12782
        cmp    eax, 8
        je     loc_1_12797
        cmp    eax, 0x80
        je     loc_1_127B6
        cmp    eax, 0x103
        je     loc_1_127DF
        cmp    eax, 0x107
        je     loc_1_127FE
loc_1_1277F:
        sub    eax, eax
loc_1_12781:
        ret    
loc_1_12782:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x30
        jl     loc_1_1277F
        cmp    eax, 0x39
        jg     loc_1_1277F
        mov    eax, 1
        jmp    loc_1_12781
loc_1_12797:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x20
        je     loc_1_127AF
        cmp    eax, 0xa
        je     loc_1_127AF
        cmp    eax, 0xd
        je     loc_1_127AF
        cmp    eax, 9
        jne    loc_1_1277F
loc_1_127AF:
        mov    eax, 1
        jmp    loc_1_12781
loc_1_127B6:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x30
        jl     loc_1_127C4
        cmp    eax, 0x39
        jle    loc_1_127D8
loc_1_127C4:
        cmp    eax, 0x61
        jl     loc_1_127CE
        cmp    eax, 0x66
        jle    loc_1_127D8
loc_1_127CE:
        cmp    eax, 0x41
        jl     loc_1_1277F
        cmp    eax, 0x46
        jg     loc_1_1277F
loc_1_127D8:
        mov    eax, 1
        jmp    loc_1_12781
loc_1_127DF:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x61
        jl     loc_1_127ED
        cmp    eax, 0x7a
        jle    loc_1_127F7
loc_1_127ED:
        cmp    eax, 0x41
        jl     loc_1_1277F
        cmp    eax, 0x5a
        jg     loc_1_1277F
loc_1_127F7:
        mov    eax, 1
        jmp    loc_1_12781
loc_1_127FE:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x30
        jl     loc_1_1280C
        cmp    eax, 0x39
        jle    loc_1_12828
loc_1_1280C:
        cmp    eax, 0x61
        jl     loc_1_12816
        cmp    eax, 0x7a
        jle    loc_1_12828
loc_1_12816:
        cmp    eax, 0x41
        jl     loc_1_1277F
        cmp    eax, 0x5a
        jg     loc_1_1277F
loc_1_12828:
        mov    eax, 1
        jmp    loc_1_12781
sub_1_12832:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x10]
        cmp    esi, eax
        mov    edx, dword ptr [esp + 0x18]
        jae    loc_1_1285E
        lea    edi, [eax + edx - 1]
        lea    ecx, [esi + edx - 1]
        sub    esi, esi
        jmp    loc_1_12872
        db  08Ah, 019h, 088h, 01Fh, 049h, 04Fh, 046h, 03Bh, 0F2h, 077h, 0F5h, 0EBh, 014h
loc_1_1285E:
        or     edx, edx
        je     loc_1_12872
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_12872:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_12876:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        sub    edi, edi
        cmp    byte ptr [esi], 0
        je     loc_1_1290B
        mov    ecx, dword ptr [0x12f60]      ; -> loc_1_12F60
        mov    edx, dword ptr [0x137f4]      ; -> loc_1_137F4
loc_1_12893:
        cmp    ecx, 1
        jle    loc_1_128B4
        push   4
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        mov    ecx, dword ptr [0x12f60]      ; -> loc_1_12F60
        mov    edx, dword ptr [0x137f4]      ; -> loc_1_137F4
        jmp    loc_1_128BE
loc_1_128B4:
        movsx  eax, byte ptr [esi]
        mov    ax, word ptr [edx + eax*2]
        and    eax, 4
loc_1_128BE:
        or     eax, eax
        je     loc_1_128CA
        movsx  eax, byte ptr [esi]
        sub    eax, 0x30
        jmp    loc_1_12904
loc_1_128CA:
        cmp    ecx, 1
        jle    loc_1_128EE
        push   0x80
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        mov    ecx, dword ptr [0x12f60]      ; -> loc_1_12F60
        mov    edx, dword ptr [0x137f4]      ; -> loc_1_137F4
        jmp    loc_1_128FA
loc_1_128EE:
        movsx  eax, byte ptr [esi]
        mov    ax, word ptr [edx + eax*2]
        and    eax, 0x80
loc_1_128FA:
        or     eax, eax
        je     loc_1_1290B
        movsx  eax, byte ptr [esi]
        sub    eax, 0x57
loc_1_12904:
        add    edi, eax
        cmp    byte ptr [esi], 0
        jne    loc_1_12893
loc_1_1290B:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_12910:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        add    byte ptr [eax], al
sub_1_12918:
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013801700
        add    byte ptr [eax], al
sub_1_12920:
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014801700
        add    byte ptr [eax], al
sub_1_12928:
        VxDCall  SHELL._SHELL_CallDll            ; cd2018801700
        add    byte ptr [eax], al
sub_1_12930:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        add    byte ptr [eax], al
sub_1_12938:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        add    byte ptr [eax], al
sub_1_12940:
        VxDCall  BIOS.Service_8007h              ; cd2007802b00
        add    byte ptr [eax], al
sub_1_12948:
        VxDCall  BIOS.Service_800Ah              ; cd200a802b00
        add    byte ptr [eax], al
sub_1_12950:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        add    byte ptr [eax], al
sub_1_12958:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_1_12960:
        VxDCall  BIOS.Service_800Fh              ; cd200f802b00
        add    byte ptr [eax], al
sub_1_12968:
        VxDCall  BIOS.Service_8011h              ; cd2011802b00
        add    byte ptr [eax], al
sub_1_12970:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        add    byte ptr [eax], al
sub_1_12978:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        add    byte ptr [eax], al
sub_1_12980:
        VxDCall  BIOS.Service_8014h              ; cd2014802b00
        add    byte ptr [eax], al
sub_1_12988:
        VxDCall  BIOS.Service_8015h              ; cd2015802b00
        add    byte ptr [eax], al
sub_1_12990:
        VxDCall  BIOS.Service_8017h              ; cd2017802b00
        add    byte ptr [eax], al
sub_1_12998:
        pop    edx
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        jmp    edx
        db  000h, 000h
sub_1_129A4:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_1_129B0:
        pop    edx
        pop    ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_1_129C0:
        pop    edx
        pop    eax
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    edx
        db  000h
sub_1_129CC:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_129E8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_1_129F8:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        add    byte ptr [eax], al
loc_1_12A00:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12A04:
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_12A08:
        sub    eax, 0x8700003c               ; -> loc_1_3F87
        aas    
        add    byte ptr [eax], al
        fmul   qword ptr [ebx]
        add    byte ptr [edx + 0x4c], dh     ; -> loc_1_4C72
        add    byte ptr [eax], al
        dec    edx
        dec    ebp
        add    byte ptr [eax], al
        popfd  
        dec    ebp
        add    byte ptr [eax], al
        mov    dh, 0x4e
        add    byte ptr [eax], al
        cmp    byte ptr [edi], dh
        add    byte ptr [0x9f000075], bh     ; -> loc_1_753D
        jbe    loc_1_12A2F
loc_1_12A2F:
        add    byte ptr [edx + ecx*4], bh    ; -> loc_1_8A7C
        add    dh, ch                        ; -> loc_1_B8EE
        mov    eax, 0xbfe70000               ; -> loc_1_BFE7
        add    byte ptr [eax], al
        and    cl, al
        add    byte ptr [eax], al
        add    byte ptr [ecx], 0
        ror    bh, 1
        add    byte ptr [eax], al
        adc    al, 0xd2
        add    byte ptr [eax], al
        sbb    dl, dl
        add    byte ptr [eax], al
        and    dl, dl
        add    byte ptr [eax], al
        leave  
        aam    0
        add    byte ptr [eax], cl            ; -> loc_1_12A08
        sub    al, byte ptr [ecx]
        db  000h
loc_1_12A5C:
        sub    byte ptr [esi], 0
        add    byte ptr [eax - 0x73ffffdb], al ; -> loc_1_2580
        and    eax, 0x1800000                ; -> sub_1_0180
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        add    al, dl                        ; -> loc_1_15D0
        adc    eax, 0x19400000               ; -> loc_1_1940
        add    byte ptr [eax], al
        xor    byte ptr [edi], bl
        add    byte ptr [eax], al
        inc    eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al            ; -> loc_1_2600
        add    byte ptr es:[eax], al
        push   0x27
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], 0
        and    byte ptr [0], ah
        add    byte ptr [eax], al
loc_1_12A98:
        adc    al, byte ptr [ecx]
        push   eax
        adc    al, byte ptr [ecx]
        add    byte ptr [edx], dl
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_12AA4:
        adc    al, byte ptr [edx]
        push   ecx
        adc    al, byte ptr [edx]
        add    byte ptr [edx], dl
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
loc_1_12AB0:
        adc    al, byte ptr [ebx]
        push   edx
        adc    al, byte ptr [ebx]
        add    byte ptr [edx], dl
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 084h
loc_1_12AC9:
        add    byte ptr [eax], al
loc_1_12ACC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12AD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_12AEC:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_12AF0:
        jecxz  loc_1_12B24
        add    byte ptr [eax], al
        dec    edi
        xor    al, byte ptr [eax]
        db  000h
loc_1_12AF8:
        call   sub_1_C440
        les    eax, ptr [eax]
        add    byte ptr [eax], al
        db  0DDh, 06Dh, 000h
loc_1_12B04:
        call   sub_1_12B0C
        and    byte ptr [eax], al
        db  000h
sub_1_12B0C:
        add    byte ptr fs:[eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x53], cl
        push   esi
        push   esi
loc_1_12B24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  loc_1_D7DE               ; fixup
        dd  loc_1_F8C5               ; fixup
        dd  sub_1_FB32               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h
        dd  WSVV_Service_00          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  030F6h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'WSVV    '                  ; DDB_Name
        dd  05400C000h                  ; DDB_Init_Order
        dd  WSVV_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  WSVV_PM_API              ; DDB_PM_API_Proc
        dd  obj1:0x12B3C             ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_12B94:
        das    
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        push   esi
        inc    ebp
        pop    eax
        inc    ebp
        xor    esi, dword ptr [edx]
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12BC4:
        add    dword ptr [eax], eax
loc_1_12BC6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12BCC:
        push   esi
        push   esi
        inc    ebp
        pop    eax
        inc    ebp
        xor    esi, dword ptr [edx]
        inc    ebp
        pop    eax
        inc    ebp
        db  000h
loc_1_12BD8:
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_12BE0:
        or     eax, 0x13000000
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h
loc_1_12BF0:
        adc    byte ptr [eax], bl
        add    byte ptr [eax], al
loc_1_12BF4:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
loc_1_12BF8:
        adc    byte ptr [edi], dl
        add    byte ptr [eax], al
loc_1_12BFC:
        adc    byte ptr [eax + eax], al
        add    byte ptr [eax], dl
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        xchg   ecx, eax
        add    byte ptr [eax], al
loc_1_12C08:
        adc    byte ptr [ebx + 0x30380000], dl
        add    byte ptr [eax], al
loc_1_12C10:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_12C18:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
loc_1_12C20:
        sub    eax, 0x565653
        add    byte ptr [eax], al
        add    byte ptr [0x434153], ch
        add    byte ptr [eax], al
        add    byte ptr [0x32504953], ch
        add    byte ptr [eax], al
        add    byte ptr [0x434953], ch
        add    byte ptr [eax], al
        add    byte ptr [0x515353], ch
        add    byte ptr [eax], al
        add    byte ptr [0x414453], ch
        add    byte ptr [eax], al
        add    byte ptr [0x584653], ch
        add    byte ptr [eax], al
        add    byte ptr [0x4d5253], ch
        add    byte ptr [eax], al
        add    byte ptr [0x4d4553], ch
        add    byte ptr [eax], al
        add    byte ptr [0x544d53], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        inc    ebx
        dec    esp
        inc    ecx
        push   ebx
        push   ebx
        cmp    eax, 0
loc_1_12C7C:
        sub    eax, 0x2d003058
        push   ebx
        push   ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12C88:
        sub    eax, 0x444353
        add    byte ptr [eax], al
        add    byte ptr [0x3f524553], ch
        add    byte ptr [eax], al
        add    byte ptr [0x505353], ch
        add    byte ptr [eax], al
        add    byte ptr [0x525353], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12CB0:
        sub    eax, dword ptr [esi + 0x50]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12CB8:
        sub    eax, dword ptr [esi + 0x4d]
        dec    ecx
        aas    
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    ebp
        dec    ebp
        aas    
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    ebp
        push   edx
        aas    
        add    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
loc_1_12CD4:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
loc_1_12CDC:
        inc    ebx
        add    byte ptr [eax], al
loc_1_12CE0:
        inc    esp
        add    byte ptr [eax], al
loc_1_12CE4:
        sub    eax, 0x524f53
        add    byte ptr [eax], al
        add    byte ptr [0x4f4e53], ch
        add    byte ptr [eax], al
        add    byte ptr [0x544453], ch
        add    byte ptr [eax], al
        add    byte ptr [0x4d5453], ch
        add    byte ptr [eax], al
        add    byte ptr [0x545453], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x30], dl
        add    byte ptr [eax], al
loc_1_12D10:
        push   ebx
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 0x36], dl
        add    byte ptr [eax], al
loc_1_12D18:
        push   ebx
        xor    esi, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> loc_1_12C10
        sub    al, 1
        add    byte ptr [esp + ebp], dl      ; -> obj1:0x12C14
        add    dword ptr [eax], eax
        sbb    byte ptr [ecx + eax], ch
        add    byte ptr [esp + ebp], bl      ; -> obj1:0x12C1C
        add    dword ptr [eax], eax
        and    byte ptr [ecx + eax], ch
        add    byte ptr [eax], ch            ; -> obj1:0x12C28
        sub    al, 1
        add    byte ptr [eax], dh            ; -> obj1:0x12C30
        sub    al, 1
        add    byte ptr [eax], bh            ; -> obj1:0x12C38
        sub    al, 1
        add    byte ptr [eax + 0x2c], al     ; -> obj1:0x12C40
        add    dword ptr [eax], eax
        dec    eax
        sub    al, 1
        add    byte ptr [eax + 0x2c], dl     ; -> obj1:0x12C50
        add    dword ptr [eax], eax
        pop    eax
        sub    al, 1
        add    byte ptr [eax + 0x2c], ah     ; -> obj1:0x12C60
        add    dword ptr [eax], eax
        push   0x7000012c                    ; -> obj1:0x12C70
        sub    al, 1
        add    byte ptr [esp + ebp + 1], bh  ; -> loc_1_12C7C
        add    byte ptr [eax - 0x77fffed4], al ; -> obj1:0x12C80
        sub    al, 1
        add    byte ptr [eax - 0x67fffed4], dl ; -> obj1:0x12C90
        sub    al, 1
        add    byte ptr [eax - 0x57fffed4], ah ; -> obj1:0x12CA0
        sub    al, 1
        add    byte ptr [eax - 0x47fffed4], dh ; -> loc_1_12CB0
        sub    al, 1
        add    al, al                        ; -> obj1:0x12CC0
        sub    al, 1
        add    al, cl                        ; -> obj1:0x12CC8
        sub    al, 1
        add    al, dl                        ; -> obj1:0x12CD0
        sub    al, 1
        add    ah, dl                        ; -> loc_1_12CD4
        sub    al, 1
        add    al, bl                        ; -> obj1:0x12CD8
        sub    al, 1
        add    ah, bl                        ; -> loc_1_12CDC
        sub    al, 1
        add    al, ah                        ; -> loc_1_12CE0
        sub    al, 1
        add    ah, ah                        ; -> loc_1_12CE4
        sub    al, 1
        add    ah, ch                        ; -> obj1:0x12CEC
        sub    al, 1
        add    ah, dh                        ; -> obj1:0x12CF4
        sub    al, 1
        add    ah, bh                        ; -> obj1:0x12CFC
        sub    al, 1
        add    byte ptr [ebp + 0x2d0c0001], al ; -> obj1:0x12D04
        add    dword ptr [eax], eax
        adc    byte ptr [0x2d140001], ch     ; -> obj1:0x12D14
        add    dword ptr [eax], eax
        sbb    byte ptr [0x2d1c0001], ch     ; -> obj1:0x12D1C
        add    dword ptr [eax], eax
loc_1_12DC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12DC8:
        sub    eax, 0x414653
        add    byte ptr [eax], al
        add    byte ptr [0x444d53], ch
        add    byte ptr [eax], al
        add    byte ptr [0x415253], ch
        add    byte ptr [eax], al
        add    byte ptr [0x3a435253], ch
        add    byte ptr [eax], al
        add    byte ptr [0x515253], ch
        add    byte ptr [eax], al
        add    byte ptr [0x565353], ch
        add    byte ptr [eax], al
        add    byte ptr [0x4f5453], ch
        add    byte ptr [eax], al
        add    byte ptr [0x4d5653], ch
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], al
        push   ebx
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12E10:
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [ebx + 0x4f], al
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x4f], cl
        and    byte ptr [ecx + 0x4e], al
        push   ebx
        push   edi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x4f], cl
        and    byte ptr [ebx + 0x41], al
        push   edx
        push   edx
        dec    ecx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
loc_1_12E40:
        dec    esi
        dec    edi
        and    byte ptr [ecx + ecx*2 + 0x41], al
        dec    esp
        push   esp
        dec    edi
        dec    esi
        inc    ebp
        add    byte ptr [edi + 0x4b], cl
        add    byte ptr [eax], al
loc_1_12E50:
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12E58:
        or     eax, 0xa
        add    byte ptr [eax], al
        add    ah, al                        ; -> loc_1_12DC4
        sub    eax, 0x2dc80001               ; -> loc_1_12DC8
        add    dword ptr [eax], eax
        shr    byte ptr [0x2dd80001], 1      ; -> obj1:0x12DD8
        add    dword ptr [eax], eax
        loopne 0x12e9f
        add    dword ptr [eax], eax
        call   0xf0012fa6                    ; -> obj1:0x12DF0
        sub    eax, 0x2df80001               ; -> obj1:0x12DF8
        add    dword ptr [eax], eax
        add    byte ptr [esi], ch
        add    dword ptr [eax], eax
        or     byte ptr [esi], ch
        add    dword ptr [eax], eax
        adc    byte ptr [esi], ch
        add    dword ptr [eax], eax
        sbb    byte ptr [esi], ch
        add    dword ptr [eax], eax
        and    byte ptr [esi], ch
        add    dword ptr [eax], eax
        sub    byte ptr [esi], ch
        add    dword ptr [eax], eax
        xor    al, 0x2e
        add    dword ptr [eax], eax
        inc    eax
        add    dword ptr cs:[eax], eax
        dec    esp
        add    dword ptr cs:[eax], eax
        push   eax
        add    dword ptr cs:[eax], eax
loc_1_12EA8:
        pop    eax
        add    dword ptr cs:[eax], eax
        pop    esp
        add    dword ptr cs:[eax], eax
loc_1_12EB0:
        adc    byte ptr [edx + 0x12], al     ; -> loc_1_11242
        add    dword ptr [eax], eax
loc_1_12EB5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [edi + 0x12], bl     ; -> loc_1_1125F
        add    dword ptr [eax], eax
        lock sub eax, dword ptr [ecx]
        add    byte ptr [ebx], al
        adc    dl, byte ptr [ebx]
        add    dword ptr [eax], eax
        hlt    
        sub    eax, dword ptr [ecx]
        add    byte ptr [edi], dl
        pop    ebp
        adc    eax, dword ptr [ecx]
        add    al, bh                        ; -> loc_1_12BF8
        sub    eax, dword ptr [ecx]
        add    byte ptr [eax + ebp*4], al    ; -> loc_1_113A8
        adc    eax, dword ptr [ecx]
        add    ah, bh                        ; -> loc_1_12BFC
        sub    eax, dword ptr [ecx]
        add    byte ptr [ebx], bl
        or     dword ptr [ecx + eax], edx
        add    byte ptr [eax], al            ; -> obj1:0x12C00
        sub    al, 1
        add    byte ptr [ecx + 0x112c0], dl  ; -> loc_1_112C0
        add    al, 0x2c
        add    dword ptr [eax], eax
        xchg   ebx, eax
        jmp    loc_1_12C08
        db  02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_12F60:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_12F70:
        push   esi
        push   edi
        call   sub_1_132BD
        mov    esi, eax
        or     esi, esi
        je     loc_1_12FDE
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_1_13377
        add    esp, 4
        or     eax, eax
        jne    loc_1_12FE3
        cmp    edi, 0x1000
        jbe    loc_1_12FC3
        push   edi
        push   esi
        call   sub_1_1348B
        add    esp, 8
        or     eax, eax
        je     loc_1_12FC3
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [esi + 0x2c], 0
        mov    dword ptr [esi + 0x34], 0
        mov    eax, esi
        mov    dword ptr [esi + 0x1c], 0x80000000
        jmp    loc_1_12FE0
loc_1_12FC3:
        mov    dword ptr [esi], 0x13668      ; -> loc_1_13668
        mov    eax, dword ptr [0x1366c]      ; -> loc_1_1366C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x1366c]      ; -> loc_1_1366C
        mov    dword ptr [eax], esi
        mov    dword ptr [0x1366c], esi      ; -> loc_1_1366C
loc_1_12FDE:
        sub    eax, eax
loc_1_12FE0:
        pop    edi
        pop    esi
        ret    
loc_1_12FE3:
        lea    ecx, [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [eax + 0xc], 0
        mov    ecx, dword ptr [eax + 0x20]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [esi + 0x30], eax
        mov    eax, esi
        mov    dword ptr [esi + 0x2c], 0
        mov    dword ptr [esi + 0x34], 0
        mov    dword ptr [esi + 0x1c], 0
        jmp    loc_1_12FE0
sub_1_1301F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [esi + 0x1f], 0x80
        jne    loc_1_1304F
        mov    ecx, dword ptr [esi + 0x30]
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        je     loc_1_13044
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        jmp    loc_1_1308D
loc_1_13044:
        push   ecx
        call   sub_1_1336A
        add    esp, 4
        jmp    loc_1_1308D
loc_1_1304F:
        mov    eax, dword ptr [esi + 0x10]
        lea    edi, [esi + 0x10]
        sub    eax, esi
        cmp    eax, 0x10
        je     loc_1_1308D
loc_1_1305C:
        mov    edx, dword ptr [edi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [edx + 8]
        or     ecx, ecx
        je     loc_1_1307B
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        jmp    loc_1_13084
loc_1_1307B:
        push   edx
        call   sub_1_1336A
        add    esp, 4
loc_1_13084:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 0x10
        jne    loc_1_1305C
loc_1_1308D:
        mov    dword ptr [esi], 0x13668      ; -> loc_1_13668
        mov    eax, dword ptr [0x1366c]      ; -> loc_1_1366C
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x1366c]      ; -> loc_1_1366C
        pop    edi
        mov    dword ptr [eax], esi
        mov    dword ptr [0x1366c], esi      ; -> loc_1_1366C
        pop    esi
        ret    
sub_1_130AB:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esi + 0x30]
        mov    ebx, dword ptr [esi + 0x34]
        mov    ecx, dword ptr [esi + 0x28]
        mov    dword ptr [esp + 0x14], ecx
        mov    ecx, dword ptr [esi + 0x2c]
        mov    dword ptr [esp + 0x10], ecx
        test   byte ptr [esi + 0x1f], 0x80
        jne    loc_1_13117
        mov    eax, ecx
        sub    eax, ebx
        mov    ecx, dword ptr [esp + 0x24]
        cmp    eax, ecx
        jb     loc_1_130E2
        mov    eax, ecx
loc_1_130E2:
        or     eax, eax
        je     loc_1_1318E
        mov    edi, dword ptr [esp + 0x1c]
        lea    esi, [edx + ebx + 0x24]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0x28], 0
        jne    loc_1_1318E
        mov    esi, dword ptr [esp + 0x20]
        lea    ecx, [ebx + eax]
        mov    dword ptr [esi + 0x34], ecx
        jmp    loc_1_1318E
loc_1_13117:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_13179
loc_1_1311E:
        cmp    dword ptr [esp + 0x14], edx
        jne    loc_1_1312A
        mov    ecx, dword ptr [esp + 0x10]
        jmp    loc_1_1312D
loc_1_1312A:
        mov    ecx, dword ptr [edx + 0x20]
loc_1_1312D:
        sub    ecx, ebx
        mov    ebp, dword ptr [esp + 0x24]
        cmp    ebp, ecx
        jb     loc_1_13139
        mov    ebp, ecx
loc_1_13139:
        or     ebp, ebp
        je     loc_1_13179
        mov    edi, dword ptr [esp + 0x1c]
        lea    esi, [edx + ebx + 0x24]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, ebp
        sub    dword ptr [esp + 0x24], ebp
        cmp    dword ptr [esp + 0x14], edx
        jne    loc_1_13165
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_1_13179
loc_1_13165:
        cmp    dword ptr [edx + 0x20], ebx
        jne    loc_1_1316E
        sub    ebx, ebx
        mov    edx, dword ptr [edx]
loc_1_1316E:
        add    dword ptr [esp + 0x1c], ebp
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_1311E
loc_1_13179:
        cmp    dword ptr [esp + 0x28], 0
        jne    loc_1_1318A
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x30], edx
        mov    dword ptr [esi + 0x34], ebx
loc_1_1318A:
        sub    eax, dword ptr [esp + 0x24]
loc_1_1318E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_13196:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [ecx + 0x28]
        mov    ebx, dword ptr [ecx + 0x2c]
        test   byte ptr [ecx + 0x1f], 0x80
        jne    loc_1_131E0
        mov    eax, dword ptr [edx + 0x20]
        sub    eax, ebx
        mov    ecx, dword ptr [esp + 0x1c]
        cmp    eax, ecx
        jb     loc_1_131B9
        mov    eax, ecx
loc_1_131B9:
        lea    edi, [edx + ebx + 0x24]
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        add    ebx, eax
        add    dword ptr [ecx + 0x20], eax
        mov    dword ptr [ecx + 0x2c], ebx
        mov    dword ptr [edx + 0x18], ebx
        jmp    loc_1_1324A
loc_1_131E0:
        mov    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_13239
loc_1_131E8:
        mov    ebp, dword ptr [edx + 0x20]
        sub    ebp, ebx
        mov    ecx, dword ptr [esp + 0x1c]
        cmp    ebp, ecx
        jb     loc_1_131F7
        mov    ebp, ecx
loc_1_131F7:
        lea    edi, [edx + ebx + 0x24]
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, ebp
        sub    dword ptr [esp + 0x1c], ebp
        mov    dword ptr [edx + 0x18], ebx
        cmp    dword ptr [edx + 0x20], ebx
        jne    loc_1_1322A
        sub    ebx, ebx
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [esp + 0x14]
        sub    ecx, edx
        cmp    ecx, -0x10
        je     loc_1_13237
loc_1_1322A:
        add    dword ptr [esp + 0x18], ebp
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_131E8
        jmp    loc_1_13239
loc_1_13237:
        sub    edx, edx
loc_1_13239:
        mov    ecx, dword ptr [esp + 0x14]
        sub    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 0x28], edx
        mov    dword ptr [ecx + 0x2c], ebx
        add    dword ptr [ecx + 0x20], eax
loc_1_1324A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1324F:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [edx + 0x20], esi
        test   byte ptr [edx + 0x1f], 0x80
        jne    loc_1_13269
        mov    eax, dword ptr [edx + 0x30]
        mov    dword ptr [edx + 0x28], eax
        jmp    loc_1_1328E
loc_1_13269:
        mov    ecx, dword ptr [edx + 0x10]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, -0x10
        je     loc_1_13291
loc_1_13275:
        mov    eax, dword ptr [ecx + 0x20]
        cmp    esi, eax
        jbe    loc_1_1328B
        sub    esi, eax
        mov    ecx, dword ptr [ecx]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, -0x10
        jne    loc_1_13275
        jmp    loc_1_13291
loc_1_1328B:
        mov    dword ptr [edx + 0x28], ecx
loc_1_1328E:
        mov    dword ptr [edx + 0x2c], esi
loc_1_13291:
        pop    esi
        ret    
sub_1_13293:
        push   edi
        call   sub_1_F3A4
        push   0x9e
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_132B9
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x27
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
loc_1_132B9:
        mov    eax, edx
        pop    edi
        ret    
sub_1_132BD:
        cmp    dword ptr [0x13668], 0x13668  ; -> loc_1_13668
        je     loc_1_132F0
        mov    eax, dword ptr [0x13668]      ; -> loc_1_13668
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x13668  ; -> loc_1_13668
        mov    dword ptr [0x13668], ecx      ; -> loc_1_13668
        mov    dword ptr [eax + 0x20], 0
        lea    ecx, [eax + 0x10]
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [ecx], ecx
        jmp    loc_1_1331E
loc_1_132F0:
        cmp    dword ptr [0x13718], 0xa0     ; -> loc_1_13718
        jae    loc_1_1331C
        call   sub_1_13293
        or     eax, eax
        je     loc_1_1331C
        mov    ecx, dword ptr [0x13718]      ; -> loc_1_13718
        inc    ecx
        mov    dword ptr [0x13718], ecx      ; -> loc_1_13718
        lea    ecx, [eax + 0x10]
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [ecx], ecx
        jmp    loc_1_1331E
loc_1_1331C:
        sub    eax, eax
loc_1_1331E:
        ret    
sub_1_1331F:
        push   esi
        push   edi
        call   sub_1_F3A4
        mov    esi, dword ptr [esp + 0xc]
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_13365
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    dword ptr [edx + 0x20], esi
        mov    dword ptr [edx + 0x1c], 0x4655424e
        mov    dword ptr [edx + 0xc], eax
        lea    eax, [edx + 0x24]
        mov    dword ptr [edx + 0x18], esi
        mov    dword ptr [edx + 0x10], eax
        mov    dword ptr [edx + 0x14], 0
loc_1_13365:
        mov    eax, edx
        pop    edi
        pop    esi
        ret    
sub_1_1336A:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret    
sub_1_13377:
        push   esi
        sub    eax, eax
        mov    esi, dword ptr [esp + 8]
        cmp    esi, 0x40
        ja     loc_1_133D3
        cmp    dword ptr [0x13764], 0x13764  ; -> loc_1_13764
        je     loc_1_133AA
        mov    eax, dword ptr [0x13764]      ; -> loc_1_13764
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x13764  ; -> loc_1_13764
        mov    dword ptr [0x13764], ecx      ; -> loc_1_13764
        jmp    loc_1_13489
loc_1_133AA:
        cmp    dword ptr [0x13828], 0x50     ; -> loc_1_13828
        jae    loc_1_133D3
        push   0x40
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     loc_1_133D3
        inc    dword ptr [0x13828]           ; -> loc_1_13828
        mov    dword ptr [eax + 8], 0x13764  ; -> loc_1_13764
        jmp    loc_1_13489
loc_1_133D3:
        cmp    esi, 0x5e0
        ja     loc_1_1342B
        cmp    dword ptr [0x13690], 0x13690  ; -> loc_1_13690
        je     loc_1_13402
        mov    eax, dword ptr [0x13690]      ; -> loc_1_13690
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x13690  ; -> loc_1_13690
        mov    dword ptr [0x13690], ecx      ; -> loc_1_13690
        jmp    loc_1_13489
loc_1_13402:
        cmp    dword ptr [0x1391c], 0x28     ; -> loc_1_1391C
        jae    loc_1_1342B
        push   0x5e0
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     loc_1_1342B
        inc    dword ptr [0x1391c]           ; -> loc_1_1391C
        mov    dword ptr [eax + 8], 0x13690  ; -> loc_1_13690
        jmp    loc_1_13489
loc_1_1342B:
        cmp    esi, 0x1000
        ja     loc_1_13480
        cmp    dword ptr [0x13884], 0x13884  ; -> loc_1_13884
        je     loc_1_13457
        mov    eax, dword ptr [0x13884]      ; -> loc_1_13884
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], 0x13884  ; -> loc_1_13884
        mov    dword ptr [0x13884], ecx      ; -> loc_1_13884
        jmp    loc_1_13489
loc_1_13457:
        cmp    dword ptr [0x13638], 0x14     ; -> loc_1_13638
        jae    loc_1_13489
        push   0x1000
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     loc_1_13489
        inc    dword ptr [0x13638]           ; -> loc_1_13638
        mov    dword ptr [eax + 8], 0x13884  ; -> loc_1_13884
        jmp    loc_1_13489
loc_1_13480:
        push   esi
        call   sub_1_1331F
        add    esp, 4
loc_1_13489:
        pop    esi
        ret    
sub_1_1348B:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    edi, [esp + 0x10]
        mov    ebp, dword ptr [esp + 0x40]
        or     ebp, ebp
        mov    esi, dword ptr [esp + 0x3c]
        je     loc_1_13515
loc_1_134A2:
        mov    ebx, ebp
        cmp    ebx, 0x1000
        jb     loc_1_134B1
        mov    ebx, 0x1000
loc_1_134B1:
        push   ebx
        call   sub_1_13377
        add    esp, 4
        or     eax, eax
        je     loc_1_134E2
        lea    ecx, [eax + 0xc]
        mov    dword ptr [edi + 0xc], ecx
        lea    ecx, [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    edi, eax
        mov    dword ptr [esi + 0x14], eax
        mov    ecx, dword ptr [eax + 0x20]
        mov    dword ptr [eax + 0x18], ecx
        sub    ebp, ebx
        jne    loc_1_134A2
        jmp    loc_1_13515
loc_1_134E2:
        mov    eax, dword ptr [esi + 0x10]
        lea    edi, [esi + 0x10]
        sub    eax, esi
        cmp    eax, 0x10
        je     loc_1_13511
loc_1_134EF:
        mov    eax, dword ptr [edi]
        mov    ebx, eax
        mov    edx, dword ptr [eax]
        mov    dword ptr [edx + 4], edi
        mov    dword ptr [edi], edx
        mov    ecx, dword ptr [ebx + 8]
        sub    edx, esi
        mov    dword ptr [ebx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        cmp    edx, 0x10
        mov    dword ptr [ecx + 4], ebx
        jne    loc_1_134EF
loc_1_13511:
        sub    eax, eax
        jmp    loc_1_13518
loc_1_13515:
        mov    eax, dword ptr [esi + 0x10]
loc_1_13518:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  000h, 000h, 000h, 000h
loc_1_13524:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13528:
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_1_1352C:
        or     eax, 0x5252450a
        dec    edi
        push   edx
        or     eax, 0xa
loc_1_13538:
        or     eax, 0x204f4e0a
        inc    ebx
        inc    ecx
        push   edx
        push   edx
        dec    ecx
        inc    ebp
        push   edx
        or     eax, 0xd00000a
        or     al, byte ptr [ebx + 0x4f]
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        or     eax, 0x5441000a
        sub    eax, 0xd434153
        add    byte ptr [0x5252450a], cl
        dec    edi
        push   edx
        or     eax, 0xa
loc_1_13568:
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_1_1356C:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13570:
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_1_13574:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        popal  
        bound  esp, qword ptr [ebx + 0x64]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        and    al, 0x48
        popal  
        jb     loc_1_13590
loc_1_13590:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_13594:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13598:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1359C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_135A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_135A4:
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
loc_1_13624:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1362C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1363C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13640:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13644:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13648:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1364C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13654:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13668:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1366C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13674:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1367C:
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
loc_1_13690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1369C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136B4:
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
loc_1_136D8:
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
loc_1_136EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136F0:
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
loc_1_13718:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1371C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13720:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13724:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13728:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1372C:
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
loc_1_13754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1376C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1377C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13788:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1378C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13790:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13794:
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
loc_1_137B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137C4:
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
loc_1_137E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13800:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1380C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13818:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1381C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13828:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1382C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13830:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13834:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13838:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1383C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13840:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13844:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13848:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1384C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1385C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13874:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1387C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13880:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13884:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13888:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1388C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13890:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13894:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13898:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1389C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_138C4:
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
loc_1_138D8:
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
loc_1_13918:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1391C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13920:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1392C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13934:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13938:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13944:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13948:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1394C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13950:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13954:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xa38  R-X
        db  083h, 0ECh, 040h, 053h, 056h, 057h, 055h, 0C7h, 005h
        dd  loc_1_13768              ; fixup
        dd  loc_1_13764              ; fixup
        db  0C7h, 005h
        dd  loc_1_13764              ; fixup
        dd  loc_1_13764              ; fixup
        db  0C7h, 005h
        dd  loc_1_13694              ; fixup
        dd  loc_1_13690              ; fixup
        db  0C7h, 005h
        dd  loc_1_13690              ; fixup
        dd  loc_1_13690              ; fixup
        db  0C7h, 005h
        dd  loc_1_13888              ; fixup
        dd  loc_1_13884              ; fixup
        db  0C7h, 005h
        dd  loc_1_13884              ; fixup
        dd  loc_1_13884              ; fixup
        db  0C7h, 005h
        dd  loc_1_1366C              ; fixup
        dd  loc_1_13668              ; fixup
        db  0C7h, 005h
        dd  loc_1_13668              ; fixup
        dd  loc_1_13668              ; fixup
        db  0C7h, 044h, 024h, 020h
        dd  loc_1_13764              ; fixup
        db  0C7h, 044h, 024h, 024h
        dd  loc_1_13690              ; fixup
        db  0C7h, 044h, 024h, 028h
        dd  loc_1_13884              ; fixup
        db  0C7h, 044h, 024h, 02Ch, 040h, 000h, 000h, 000h, 0C7h, 044h, 024h, 030h, 0E0h, 005h, 000h, 000h
        db  0C7h, 044h, 024h, 034h, 000h, 010h, 000h, 000h, 08Dh, 044h, 024h, 044h, 0C7h, 044h, 024h, 038h
        db  020h, 000h, 000h, 000h, 0C7h, 044h, 024h, 03Ch, 010h, 000h, 000h, 000h, 0C7h, 044h, 024h, 040h
        db  008h, 000h, 000h, 000h, 0C7h, 044h, 024h, 044h
        dd  loc_1_13828              ; fixup
        db  0C7h, 044h, 024h, 048h
        dd  loc_1_1391C              ; fixup
        db  0C7h, 044h, 024h, 04Ch
        dd  loc_1_13638              ; fixup
        db  089h, 044h, 024h, 01Ch, 08Dh, 044h, 024h, 038h, 089h, 044h, 024h, 018h, 08Dh, 044h, 024h, 02Ch
        db  089h, 044h, 024h, 014h, 08Dh, 044h, 024h, 020h, 089h, 044h, 024h, 010h, 08Bh, 044h, 024h, 010h
        db  08Bh, 030h, 08Bh, 044h, 024h, 014h, 08Bh, 038h, 08Bh, 044h, 024h, 018h, 08Bh, 018h, 08Bh, 044h
        db  024h, 01Ch, 08Bh, 028h, 08Bh, 0C3h, 04Bh, 00Bh, 0C0h, 074h, 04Fh, 0E8h
        dd  sub_1_13293              ; fixup
        db  00Bh, 0C0h, 074h, 046h, 0C7h, 000h
        dd  loc_1_13668              ; fixup
        db  08Bh, 00Dh
        dd  loc_1_1366C              ; fixup
        db  089h, 048h, 004h, 08Bh, 00Dh
        dd  loc_1_1366C              ; fixup
        db  057h, 089h, 001h, 0A3h
        dd  loc_1_1366C              ; fixup
        db  0FFh, 005h
        dd  loc_1_13718              ; fixup
        db  0E8h
        dd  sub_1_1331F              ; fixup
        db  083h, 0C4h, 004h, 00Bh, 0C0h, 074h, 017h, 089h, 030h, 08Bh, 04Eh, 004h, 089h, 048h, 004h, 089h
        db  001h, 089h, 046h, 004h, 08Bh, 0C3h, 0FFh, 045h, 000h, 04Bh, 00Bh, 0C0h, 075h, 0B1h, 083h, 044h
        db  024h, 01Ch, 004h, 083h, 044h, 024h, 018h, 004h, 083h, 044h, 024h, 014h, 004h, 083h, 044h, 024h
        db  010h, 004h, 08Dh, 044h, 024h, 02Ch, 039h, 044h, 024h, 010h, 00Fh, 082h, 070h, 0FFh, 0FFh, 0FFh
        db  083h, 03Dh
        dd  loc_1_13828              ; fixup
        db  000h, 074h, 022h, 083h, 03Dh
        dd  loc_1_1391C              ; fixup
        db  000h, 074h, 019h, 083h, 03Dh
        dd  loc_1_13638              ; fixup
        db  000h, 074h, 010h, 083h, 03Dh
        dd  loc_1_13718              ; fixup
        db  000h, 074h, 007h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 063h, 08Dh, 074h, 024h, 020h, 08Bh, 03Eh
        db  039h, 03Fh, 074h, 018h, 08Bh, 007h, 08Bh, 0C8h, 08Bh, 000h, 051h, 089h, 078h, 004h, 089h, 007h
        db  0E8h
        dd  sub_1_1336A              ; fixup
        db  083h, 0C4h, 004h, 039h, 03Fh, 075h, 0E8h, 083h, 0C6h, 004h, 08Dh, 044h, 024h, 02Ch, 03Bh, 0F0h
        db  072h, 0D7h, 081h, 03Dh
        dd  loc_1_13668              ; fixup
        dd  loc_1_13668              ; fixup
        db  074h, 028h, 0BEh
        dd  loc_1_13668              ; fixup
        db  08Bh, 00Dh
        dd  loc_1_13668              ; fixup
        db  08Bh, 0C1h, 051h, 08Bh, 000h, 089h, 070h, 004h, 0A3h
        dd  loc_1_13668              ; fixup
        db  0E8h
        dd  sub_1_F396               ; fixup
        db  083h, 0C4h, 004h, 039h, 035h
        dd  loc_1_13668              ; fixup
        db  075h, 0DDh, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 040h, 0C3h, 000h, 000h, 000h
sub_2_0200:
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        je     loc_2_021F
        push   1
        push   0x12
        call   sub_1_F02D
        add    esp, 8
        mov    dword ptr [0x13654], 0        ; -> loc_1_13654
loc_2_021F:
        call   sub_1_EFC0
        sub    eax, eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0230:
        call   sub_2_0670
        or     eax, eax
        jne    loc_2_023D
        sub    eax, eax
        jmp    loc_2_02B9
loc_2_023D:
        call   0
        or     eax, eax
        mov    eax, 0
        je     loc_2_02B9
        mov    dword ptr [0x13838], 0x13834  ; -> loc_1_13838
        mov    dword ptr [0x13834], 0x13834  ; -> loc_1_13834
        mov    dword ptr [0x1367c], 0x13678  ; -> loc_1_1367C
        mov    dword ptr [0x13678], 0x13678  ; -> loc_1_13678
        mov    dword ptr [0x13840], 0x1383c  ; -> loc_1_13840
        mov    dword ptr [0x1383c], 0x1383c  ; -> loc_1_1383C
        mov    dword ptr [0x13650], 0x1364c  ; -> loc_1_13650
        mov    dword ptr [0x1364c], 0x1364c  ; -> loc_1_1364C
        mov    dword ptr [0x13630], 0x1362c  ; -> loc_1_13630
        mov    dword ptr [0x1362c], 0x1362c  ; -> loc_1_1362C
        call   sub_1_F3A4
        mov    eax, 1
loc_2_02B9:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_02C0:
        push   esi
        push   edi
        mov    dword ptr [0x13634], 0        ; -> loc_1_13634
        mov    dword ptr [0x13664], 0        ; -> loc_1_13664
        mov    word ptr [0x13818], 1         ; -> loc_1_13818
        mov    dword ptr [0x13670], 0x20     ; -> loc_1_13670
        mov    dword ptr [0x13858], 0        ; -> loc_1_13858
        push   0x80
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [0x1377c], eax      ; -> loc_1_1377C
        or     eax, eax
        jne    loc_2_030D
        sub    eax, eax
        jmp    loc_2_0371
loc_2_030D:
        push   0x78
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_2_032F
        push   dword ptr [0x1377c]           ; -> loc_1_1377C
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_0371
loc_2_032F:
        mov    dword ptr [0x13878], 0        ; -> loc_1_13878
        mov    esi, 5
loc_2_033E:
        push   edi
        call   sub_1_3F74
        add    esp, 4
        add    edi, 0x18
        dec    esi
        jne    loc_2_033E
        mov    edi, 0x138d8                  ; -> loc_1_138D8
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        push   eax
        push   dword ptr [0x13944]           ; -> loc_1_13944
        push   0x3980                        ; -> loc_1_3980
        call   sub_1_129CC
        mov    eax, 1
loc_2_0371:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0380:
        mov    eax, 1
        ret    
sub_2_0386:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0390:
        mov    dword ptr [0x13948], 0x13948  ; -> loc_1_13948
        mov    dword ptr [0x1394c], 0x13948  ; -> loc_1_1394C
        mov    dword ptr [0x13720], 0x13720  ; -> loc_1_13720
        mov    dword ptr [0x13724], 0x13720  ; -> loc_1_13724
        mov    dword ptr [0x13640], 0x13640  ; -> loc_1_13640
        mov    eax, 1
        mov    dword ptr [0x13644], 0x13640  ; -> loc_1_13644
        mov    dword ptr [0x13800], 0        ; -> loc_1_13800
        mov    dword ptr [0x1384c], 6        ; -> loc_1_1384C
        mov    dword ptr [0x13854], 0        ; -> loc_1_13854
        mov    dword ptr [0x13848], 0        ; -> loc_1_13848
        mov    dword ptr [0x13850], 0        ; -> loc_1_13850
        ret    
sub_2_0404:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0410:
        sub    esp, 8
        push   esi
        push   edi
        push   0x32
        lea    eax, [esp + 0xc]
        push   0x1381c                       ; -> loc_1_1381C
        push   eax
        call   sub_1_169D
        cmp    dword ptr [esp + 8], 0
        je     loc_2_0434
        sub    eax, eax
        jmp    loc_2_04EC
loc_2_0434:
        push   0x32
        lea    eax, [esp + 0xc]
        push   0x1378c                       ; -> loc_1_1378C
        push   eax
        call   sub_1_169D
        cmp    dword ptr [esp + 8], 0
        je     loc_2_045E
        push   dword ptr [0x1381c]           ; -> loc_1_1381C
        call   sub_1_171E
        sub    eax, eax
        jmp    loc_2_04EC
loc_2_045E:
        mov    dword ptr [0x13938], 0x48     ; -> loc_1_13938
        push   0xe10
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [0x13920], eax      ; -> loc_1_13920
        or     eax, eax
        jne    loc_2_0498
        push   dword ptr [0x1381c]           ; -> loc_1_1381C
        call   sub_1_171E
        push   dword ptr [0x1378c]           ; -> loc_1_1378C
        call   sub_1_171E
        sub    eax, eax
        jmp    loc_2_04EC
loc_2_0498:
        sub    esi, esi
        mov    eax, dword ptr [0x13938]      ; -> loc_1_13938
        sub    edi, edi
loc_2_04A1:
        push   eax
        mov    ecx, esi
        imul   ecx, eax
        add    ecx, dword ptr [0x13920]      ; -> loc_1_13920
        push   ecx
        lea    eax, [esp + 0x14]
        push   dword ptr [0x1381c]           ; -> loc_1_1381C
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_1640
        cmp    dword ptr [esp + 8], edi
        jne    loc_2_04F2
        push   dword ptr [esp + 0xc]
        call   sub_1_A7C2
        add    esp, 4
        inc    esi
        cmp    esi, 0x32
        mov    eax, dword ptr [0x13938]      ; -> loc_1_13938
        jl     loc_2_04A1
        mov    byte ptr [0x12adc], 1         ; -> obj1:0x12ADC
        mov    eax, 1
loc_2_04EC:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_2_04F2:
        push   dword ptr [0x1381c]           ; -> loc_1_1381C
        call   sub_1_171E
        push   dword ptr [0x1378c]           ; -> loc_1_1378C
        call   sub_1_171E
        push   dword ptr [0x13920]           ; -> loc_1_13920
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_04EC
sub_2_051A:
        cmp    byte ptr [0x12adc], 0         ; -> obj1:0x12ADC
        je     loc_2_0547
        push   dword ptr [0x1381c]           ; -> loc_1_1381C
        call   sub_1_171E
        push   dword ptr [0x1378c]           ; -> loc_1_1378C
        call   sub_1_171E
        push   dword ptr [0x13920]           ; -> loc_1_13920
        call   sub_1_F396
        add    esp, 4
loc_2_0547:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0550:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [0x137fc], 0        ; -> loc_1_137FC
        lea    eax, [esp + 0x18]
        push   0x14
        push   eax
        mov    dword ptr [0x1371c], 0        ; -> loc_1_1371C
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_169D
        cmp    dword ptr [esp + 0x10], 0
        je     loc_2_058A
        sub    eax, eax
        jmp    loc_2_0622
loc_2_058A:
        push   0x2e4
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_2_05AA
        push   dword ptr [esp + 0x18]
        call   sub_1_171E
        sub    eax, eax
        jmp    loc_2_0622
loc_2_05AA:
        sub    ebx, ebx
        mov    esi, edi
        sub    ebp, ebp
loc_2_05B0:
        push   0x25
        push   esi
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_1640
        cmp    dword ptr [esp + 0x10], ebp
        jne    loc_2_062A
        push   dword ptr [esp + 0x14]
        call   sub_1_BA30
        add    esp, 4
        add    esi, 0x25
        inc    ebx
        cmp    ebx, 0x14
        jl     loc_2_05B0
        mov    dword ptr [0x13780], 0x13780  ; -> loc_1_13780
        mov    dword ptr [0x13784], 0x13780  ; -> loc_1_13784
        mov    dword ptr [0x1359c], 0x1359c  ; -> loc_1_1359C
        mov    dword ptr [0x135a0], 0x1359c  ; -> loc_1_135A0
        mov    eax, 1
        mov    dword ptr [0x13698], 0x13698  ; -> loc_1_13698
        mov    dword ptr [0x1369c], 0x13698  ; -> loc_1_1369C
loc_2_0622:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_2_062A:
        push   dword ptr [esp + 0x18]
        call   sub_1_171E
        push   edi
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        jmp    loc_2_0622
sub_2_0640:
        push   edi
        mov    edi, 0x135a4                  ; -> loc_1_135A4
        sub    eax, eax
        mov    ecx, 0x20
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0x13754], eax      ; -> loc_1_13754
        mov    dword ptr [0x13830], eax      ; -> loc_1_13830
        mov    eax, 1
        pop    edi
        ret    
sub_2_0660:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0670:
        push   esi
        sub    esi, esi
        cmp    dword ptr [0x13944], esi      ; -> loc_1_13944
        jne    loc_2_0685
        mov    dword ptr [0x13944], 0x32     ; -> loc_1_13944
loc_2_0685:
        cmp    dword ptr [0x13648], 0        ; -> loc_1_13648
        jne    loc_2_0698
        mov    dword ptr [0x13648], 0x15     ; -> loc_1_13648
loc_2_0698:
        mov    eax, dword ptr [0x13674]      ; -> loc_1_13674
        or     eax, eax
        jne    loc_2_06A6
        mov    eax, 7
loc_2_06A6:
        mov    dword ptr [0x13674], eax      ; -> loc_1_13674
        or     eax, eax
        jne    loc_2_06B9
        mov    dword ptr [0x13674], 7        ; -> loc_1_13674
loc_2_06B9:
        cmp    dword ptr [0x1376c], 0        ; -> loc_1_1376C
        jne    loc_2_06CC
        mov    dword ptr [0x1376c], 1        ; -> loc_1_1376C
loc_2_06CC:
        mov    dword ptr [0x13788], 7        ; -> loc_1_13788
        mov    dword ptr [0x137ec], 0x8c     ; -> loc_1_137EC
        mov    dword ptr [0x1382c], 0x1c     ; -> loc_1_1382C
        mov    dword ptr [0x13844], 0x8c     ; -> loc_1_13844
        cmp    dword ptr [0x136d8], 0        ; -> loc_1_136D8
        jne    loc_2_0707
        mov    dword ptr [0x136d8], 0x4e20   ; -> loc_1_136D8
loc_2_0707:
        cmp    dword ptr [0x136a0], 0        ; -> loc_1_136A0
        jne    loc_2_071A
        mov    dword ptr [0x136a0], 0x7d0    ; -> loc_1_136A0
loc_2_071A:
        call   sub_2_0984
        or     eax, eax
        jne    loc_2_0728
        jmp    loc_2_0859
loc_2_0728:
        call   sub_1_F21C
        mov    dword ptr [0x13598], eax      ; -> loc_1_13598
        mov    dword ptr [0x13654], 0        ; -> loc_1_13654
        mov    ecx, 0x64
        mov    eax, dword ptr [0x12b00]      ; -> obj1:0x12B00
        sub    edx, edx
        div    ecx
        mov    dword ptr [0x12b00], eax      ; -> obj1:0x12B00
        mov    eax, dword ptr [0x12b04]      ; -> loc_1_12B04
        sub    edx, edx
        div    ecx
        mov    dword ptr [0x12b04], eax      ; -> loc_1_12B04
        call   sub_1_F3A4
        call   sub_2_02C0
        or     eax, eax
        je     loc_2_081D
        call   sub_1_F3A4
        call   sub_2_0550
        or     eax, eax
        je     loc_2_081D
        mov    eax, dword ptr [0x12b0c]      ; -> sub_1_12B0C
        cmp    eax, 0xfffffe
        jb     loc_2_0792
        mov    eax, 0xfffffe
loc_2_0792:
        mov    dword ptr [0x12b0c], eax      ; -> sub_1_12B0C
        call   sub_1_F3A4
        call   sub_2_0380
        or     eax, eax
        je     loc_2_081D
        call   sub_1_F3A4
        call   sub_2_0640
        or     eax, eax
        je     loc_2_081D
        mov    esi, 1
        call   sub_1_F3A4
        call   sub_2_0390
        or     eax, eax
        je     loc_2_081D
        call   sub_1_F3A4
        call   sub_2_0410
        or     eax, eax
        je     loc_2_081D
        call   sub_1_F3A4
        push   0x100
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [0x137f0], eax      ; -> loc_1_137F0
        or     eax, eax
        je     loc_2_081D
        mov    dword ptr [eax], 0x400
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0x401
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [0x13918], 2        ; -> loc_1_13918
        call   sub_1_F3A4
        mov    eax, esi
        jmp    loc_2_085B
loc_2_081D:
        call   sub_2_051A
        call   sub_2_0404
        or     esi, esi
        je     loc_2_0830
        call   sub_2_0660
loc_2_0830:
        call   sub_2_0386
        cmp    dword ptr [0x13654], 0        ; -> loc_1_13654
        je     loc_2_0854
        push   1
        push   0x12
        call   sub_1_F02D
        add    esp, 8
        mov    dword ptr [0x13654], 0        ; -> loc_1_13654
loc_2_0854:
        call   sub_1_F3A4
loc_2_0859:
        sub    eax, eax
loc_2_085B:
        pop    esi
        ret    
        db  000h, 000h, 000h
sub_2_0860:
        mov    eax, 0x488
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        jecxz  loc_2_086F
        clc    
        ret    
loc_2_086F:
        mov    edx, 0x9a4                    ; -> loc_2_09A4
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        ret    
sub_2_0880:
        mov    edi, 0x13954                  ; -> loc_1_13954
        mov    ecx, 0x13954                  ; -> loc_1_13954
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        cmp    byte ptr [0xa35], 0           ; -> obj2:0x0A35
        jne    loc_2_092B
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0x9b7                    ; -> loc_2_09B7
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x13944], eax      ; -> loc_1_13944
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0x9c4                    ; -> obj2:0x09C4
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x13648], eax      ; -> loc_1_13648
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0x9da                    ; -> obj2:0x09DA
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x13674], eax      ; -> loc_1_13674
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0x9f7                    ; -> loc_2_09F7
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x1376c], eax      ; -> loc_1_1376C
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0xa12                    ; -> obj2:0x0A12
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x136d8], eax      ; -> loc_1_136D8
        xor    eax, eax
        mov    esi, 0x9b2                    ; -> loc_2_09B2
        mov    edi, 0xa23                    ; -> loc_2_0A23
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        mov    dword ptr [0x136a0], eax      ; -> loc_1_136A0
loc_2_092B:
        call   sub_2_0860
        jb     loc_2_0966
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_0966
        VxDCall  Dev_003E.Service_0000h          ; cd2000003e00
        jb     loc_2_0966
        mov    eax, 0x30f6
        mov    ebx, 1
        lea    ecx, [0x12a5c]                ; -> loc_1_12A5C
        VxDCall  Dev_003E.Service_0001h          ; cd2001003e00
        or     eax, eax
        je     loc_2_0966
        call   sub_2_0230
        or     eax, eax
        je     loc_2_0966
        ret    
loc_2_0966:
        mov    eax, 0x30f6
        VxDCall  Dev_003E.Service_0002h          ; cd2002003e00
        stc    
        ret    
loc_2_0973:
        inc    byte ptr [0xa35]              ; -> obj2:0x0A35
        call   sub_2_0880
        jb     loc_2_0982
        clc    
        ret    
loc_2_0982:
        stc    
        ret    
sub_2_0984:
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_09A0
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_09A0
        VxDCall  Dev_0488.Service_000Ah          ; cd200a008804
        mov    dword ptr [0x12b90], eax      ; -> obj1:0x12B90
        clc    
loc_2_09A0:
        cmc    
        sbb    eax, eax
        ret    
loc_2_09A4:
        push   esi
        push   esp
        inc    esp
        dec    ecx
        xor    edi, dword ptr cs:[eax]
        add    byte ptr ss:[ebp + 0x53], cl
        push   esi
        push   esi
        db  000h
loc_2_09B2:
        push   edi
        push   ebx
        push   esi
        push   esi
        db  000h
loc_2_09B7:
        dec    ebp
        push   ebx
        push   eax
        jb     0xa13
        js     loc_2_0A03
        push   esp
        imul   esp, dword ptr [ebx + 0x6b], 0x44785600
        push   esp
        imul   esp, dword ptr [ebx + 0x6b], 0x72655073
        pop    eax
        insd   dword ptr es:[edi], dx
        imul   esi, dword ptr [eax + edx*2 + 0x65], 0x646f6972
        add    byte ptr [esi + 0x78], dl
        inc    esp
        push   esp
        imul   esp, dword ptr [ebx + 0x6b], 0x72655073
        inc    ecx
        arpl   word ptr [ebx + 0x4f], bp
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        jns    0xa45
        insd   dword ptr es:[edi], dx
        imul   esi, dword ptr [eax + edx*2 + 0x65], 0x646f6972
        db  000h
loc_2_09F7:
        dec    esi
        jne    0xa67
        push   ecx
        jne    0xa66
        je     0xa50
        jb     0xa6c
loc_2_0A03:
        outsd  dx, dword ptr [esi]
        jae    0xa49
        outsw  dx, word ptr gs:[esi]
        jb     0xa71
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x63655200
        jbe    0xa6a
        je     0xa7a
        je     0xa80
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
loc_2_0A23:
        dec    ebp
        popal  
        js     0xa90
        insd   dword ptr es:[edi], dx
        jne    0xa97
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     0xa84
        imul   edi, dword ptr [edx + 0x65], 0x56535721
VxD__LTEXT ENDS
